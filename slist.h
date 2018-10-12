#include <stdbool.h>

struct slist {
    struct slist *next;
    int value;
};

int slist_length(const struct slist *list);
void slist_destroy(struct slist *list);
struct slist *slist_create(const int value);
struct slist *slist_from_array(const int input[], int num);
struct slist *slist_append(struct slist *list, int value);
struct slist *slist_get(struct slist *list, int pos);
struct slist *slist_find(struct slist *list, int value);
struct slist *slist_slice(const struct slist *source, int start, int end);
struct slist *slist_insert(struct slist *list, int pos, int value);
struct slist *slist_delete(struct slist *list, int pos);
struct slist *slist_map(const struct slist *list, int (*fn)(int));
struct slist *slist_filter(const struct slist *list, bool (*fn)(int));
int slist_reduce(const struct slist *list, int (*fn)(int, int));
char *slist_to_json(const struct slist *list);
struct slist *slist_from_json(const char *json);
