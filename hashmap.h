#include "slist.h"

#define HASHMAP_INIT_SIZE 32
#define HASHMAP_SCALE_FACTOR 2
#define HASHMAP_MAX_LOAD 2

struct hashmap {
    unsigned int size;
    unsigned int count;
    struct slist **buckets;
};

struct hashmap *hashmap_create();
