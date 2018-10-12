#include <stdbool.h>

struct hashmap_entry {
    struct hashmap_entry *next;
    char *key;
    void *value;
};

struct hashmap {
    unsigned int size;
    unsigned int count;
    struct hashmap_entry **buckets;
};

struct hashmap *hashmap_create();
void hashmap_destroy(struct hashmap *);
bool hashmap_set(struct hashmap *, const char *, void *);
void *hashmap_get(struct hashmap *, const char *);
bool hashmap_delete(struct hashmap *, const char *);
bool hashmap_exists(const struct hashmap *, const char *);
