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
void hashmap_destroy(struct hashmap *m);
