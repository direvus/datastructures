#include <stdbool.h>

struct slisti {
    struct slisti *next;
    int value;
};

int slisti_length(const struct slisti *list);
void slisti_destroy(struct slisti *list);
struct slisti *slisti_create(const int value);
struct slisti *slisti_from_array(const int input[], int num);
struct slisti *slisti_append(struct slisti *list, int value);
struct slisti *slisti_get(struct slisti *list, int pos);
struct slisti *slisti_find(struct slisti *list, int value);
struct slisti *slisti_slice(const struct slisti *source, int start, int end);
struct slisti *slisti_insert(struct slisti *list, int pos, int value);
struct slisti *slisti_delete(struct slisti *list, int pos);
struct slisti *slisti_map(const struct slisti *list, int (*fn)(int));
struct slisti *slisti_filter(const struct slisti *list, bool (*fn)(int));
int slisti_reduce(const struct slisti *list, int (*fn)(int, int));
char *slisti_to_json(const struct slisti *list);
struct slisti *slisti_from_json(const char *json);
