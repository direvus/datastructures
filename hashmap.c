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
