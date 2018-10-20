#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "./hashmap.h"
#include "./hash.h"

#define HASHMAP_INIT_SIZE 32
#define HASHMAP_MAX_SIZE UINT_MAX
#define HASHMAP_SCALE_FACTOR 2
#define HASHMAP_MAX_LOAD 2

struct hashmap *hashmap_create_size(const size_t size) {
    struct hashmap *m;
    m = malloc(sizeof *m);
    m->size = size;
    m->count = 0;
    m->buckets = calloc(size, sizeof (struct hashmap_entry *));
    return m;
}

struct hashmap *hashmap_create() {
    return hashmap_create_size(HASHMAP_INIT_SIZE);
}

static void hashmap_entry_destroy(struct hashmap_entry *e, bool keep_value) {
    if (e->key) {
        free(e->key);
    }
    if (e->value && !keep_value) {
        free(e->value);
    }
    free(e);
}

static void hashmap_buckets_destroy(struct hashmap_entry **buckets, const size_t size, bool keep_values) {
    struct hashmap_entry *curr, *next;
    for (size_t i = 0; i < size; i++) {
        if (buckets[i]) {
            curr = buckets[i];
            while (curr) {
                next = curr->next;
                free(curr);
                curr = next;
            }
            hashmap_entry_destroy(buckets[i], keep_values);
            buckets[i] = 0;
        }
    }
    free(buckets);
}

void hashmap_destroy(struct hashmap *m) {
    hashmap_buckets_destroy(m->buckets, m->size, false);
    free(m);
}

/*
 * Copy the contents of hashmap 'src' into 'dst'.
 *
 * Afterwards all the keys present in 'src' will be present in 'dst' with the
 * same values as in 'src'.  Any keys already present in 'dst' that are absent
 * in 'src' will be unaffected.
 */
void hashmap_copy(struct hashmap *dst, struct hashmap *src) {
    if (!src || !dst) {
        return;
    }

    struct hashmap_entry *e;
    for (unsigned int i = 0; i < src->size; i++) {
        e = src->buckets[i];
        while (e) {
            hashmap_set(dst, e->key, e->value);
            e = e->next;
        }
    }
}

static inline unsigned long hash_bytes(const char *key) {
    return hash_shimmy2(key);
}

static inline size_t hash_index(const char *key, const size_t size) {
    return hash_bytes(key) % size;
}

/*
 * Search the given list of entries for a key matching 'k'.
 *
 * Return a pointer to the matching entry, if found, or NULL otherwise.
 */
static struct hashmap_entry *hashmap_find(struct hashmap_entry *e, const char *k) {
    while (e) {
        if (strcmp(e->key, k) == 0) {
            return e;
        }
        e = e->next;
    }
    return 0;
}

bool hashmap_exists(const struct hashmap *m, const char *k) {
    size_t i = hash_index(k, m->size);
    struct hashmap_entry *e = m->buckets[i];
    while (e) {
        if (strcmp(e->key, k) == 0) {
            return true;
        }
        e = e->next;
    }
    return false;
}

/*
 * Create a new hashmap entry with the given key and value.
 *
 * Return a NULL pointer if the entry cannot be created.
 */
static struct hashmap_entry *hashmap_entry_create(const char *k, void *v) {
    if (!v) {
        return 0;
    }
    struct hashmap_entry *e = malloc(sizeof *e);
    if (!e) {
        return 0;
    }

    e->key = malloc(strlen(k) + 1);
    if (!e->key) {
        return 0;
    }
    strcpy(e->key, k);
    e->value = v;
    e->next = 0;
    return e;
}

/*
 * Set key 'k' to value 'v' in list of entries 'e'.
 *
 * If an entry for the given key already exists in the list, it takes the new
 * value, and the data pointed to by the old value is freed.  Otherwise, a new
 * entry is appended to the list.
 *
 * 'v' must point to alloc'd memory.  When the list of entry is destroyed, the
 * data pointed to by 'v' is freed also.
 *
 * Return the first element of the resulting list, or a NULL pointer if the
 * operation fails.
 *
 * If a valid pointer is passed in for 'created', it will be set to indicate
 * whether a new entry was created.
 */
static struct hashmap_entry *hashmap_set_entry(
        struct hashmap_entry *e,
        const char *k,
        void *v,
        bool *created) {
    if (created) {
        *created = false;
    }
    if (!k || !v) {
        return 0;
    }
    struct hashmap_entry *head = e;
    struct hashmap_entry *tail = 0;

    if (!e) {
        /* Empty list, create and return a new entry. */
        head = hashmap_entry_create(k, v);
        if (head && created) {
            *created = true;
        }
        return head;
    }

    while (e) {
        if (strcmp(e->key, k) == 0) {
            /* Key exists, update value. */
            e->value = v;
            return head;
        }
        tail = e;
        e = e->next;
    }

    /* Key does not exist, append new entry. */
    e = hashmap_entry_create(k, v);
    if (!e) {
        return 0;
    }
    tail->next = e;
    if (created) {
        *created = true;
    }
    return head;
}

/*
 * Set key 'k' to value 'v' in hashmap 'm'.
 *
 * If an entry for the given key already exists, it takes the new value.
 * Otherwise, a new entry is created.
 *
 * 'v' must point to alloc'd memory.  When the map is destroyed, the data
 * pointed to by 'v' will be freed also.
 *
 * Return whether the entry was created successfully.
 */
bool hashmap_set(struct hashmap *m, const char *k, void *v) {
    size_t i = hash_index(k, m->size);
    bool created = false;
    struct hashmap_entry *e = hashmap_set_entry(m->buckets[i], k, v, &created);
    if (!e) {
        return false;
    }
    m->buckets[i] = e;
    if (created) {
        m->count++;
    }

    /*
     * Did adding this entry bring the overall load factor up to the maximum?
     * Do we still have room to expand?  If so, then resize it.
     */
    if (m->count / m->size >= HASHMAP_MAX_LOAD &&
            m->size * HASHMAP_SCALE_FACTOR <= HASHMAP_MAX_SIZE) {
        /*
         * Multiply the current size by the scaling factor, prepare a new
         * bucket array of that size, copy the existing entries into the new
         * bucket array, and then point the hashmap at the new bucket array and
         * destroy the old one.
         */
        const size_t size = m->size * HASHMAP_SCALE_FACTOR;
        struct hashmap_entry **buckets;
        struct hashmap_entry *new;
        buckets = calloc(size, sizeof (struct hashmap_entry *));

        for (unsigned int b = 0; b < m->size; b++) {
            e = m->buckets[b];
            while (e) {
                i = hash_index(e->key, size);
                new = hashmap_set_entry(buckets[i], e->key, e->value, 0);
                if (!new) {
                    /*
                     * Something has gone terribly wrong, abort the resize
                     * operation.
                     */
                    hashmap_buckets_destroy(buckets, size, true);
                    return true;
                }
                buckets[i] = new;
                e = e->next;
            }
        }

        struct hashmap_entry **old_buckets;
        old_buckets = m->buckets;
        m->buckets = buckets;
        hashmap_buckets_destroy(old_buckets, m->size, true);
        m->size = size;
    }
    return true;
}

/*
 * Return the value for key 'k' in hashmap 'm'.
 *
 * If the key does not exist in the map, return a NULL pointer.
 */
void *hashmap_get(struct hashmap *m, const char *k) {
    size_t i = hash_index(k, m->size);
    struct hashmap_entry *e = hashmap_find(m->buckets[i], k);
    if (e) {
        return e->value;
    }
    return 0;
}

/*
 * Delete the entry for 'k' from hashmap 'm'.
 *
 * If the key exists in the map, delete its entry and return true.  Otherwise,
 * do nothing and return false.
 */
bool hashmap_delete(struct hashmap *m, const char *k) {
    if (!m || !k) {
        return false;
    }
    size_t i = hash_index(k, m->size);
    struct hashmap_entry *curr = m->buckets[i];
    struct hashmap_entry *prev = 0;
    while (curr) {
        if (strcmp(curr->key, k) == 0) {
            if (prev) {
                prev->next = curr->next;
            } else {
                m->buckets[i] = curr->next;
            }
            hashmap_entry_destroy(curr, false);
            m->count--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}
