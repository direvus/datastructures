#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "./hashmap.h"
#include "./hash.h"

#define HASHMAP_INIT_SIZE 32
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

void hashmap_entry_destroy(struct hashmap_entry *e) {
    if (e->key) {
        free(e->key);
    }
    if (e->value) {
        free(e->value);
    }
    free(e);
}

void hashmap_destroy(struct hashmap *m) {
    for (size_t i = 0; i < m->size; i++) {
        if (m->buckets[i]) {
            hashmap_entry_destroy(m->buckets[i]);
            m->buckets[i] = 0;
        }
    }
    free(m->buckets);
    free(m);
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
    while(e) {
        if (strcmp(e->key, k) == 0) {
            return e;
        }
        e = e->next;
    }
    return 0;
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
    struct hashmap_entry *curr = m->buckets[i];
    struct hashmap_entry *prev = 0;
    while(curr) {
        if (strcmp(curr->key, k) == 0) {
            curr->value = v;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    curr = malloc(sizeof *curr);
    if (!curr) {
        return false;
    }

    curr->key = malloc(strlen(k) + 1);
    if (!curr->key) {
        return false;
    }
    strcpy(curr->key, k);
    curr->value = v;
    curr->next = 0;
    if (prev) {
        prev->next = curr;
    } else {
        m->buckets[i] = curr;
    }
    m->count++;
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
