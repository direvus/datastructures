#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

struct hashmap *hashmap_create_size(unsigned int size) {
    struct hashmap *m;
    m = malloc(sizeof *m);
    m->size = size;
    m->count = 0;
    m->buckets = calloc(size, sizeof (struct slist *));
    return m;
}

struct hashmap *hashmap_create() {
    return hashmap_create_size(HASHMAP_INIT_SIZE);
}
