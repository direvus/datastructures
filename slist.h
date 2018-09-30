#include <stdbool.h>

struct slist {
    struct slist *next;
    int value;
};

int slist_length(struct slist *list);
void slist_destroy(struct slist *list);
struct slist *slist_create(int value);
struct slist *slist_from_array(int input[], int num);
struct slist *slist_append(struct slist *list, int value);
struct slist *slist_get(struct slist *list, int pos);
struct slist *slist_find(struct slist *list, int value);
struct slist *slist_slice(struct slist *source, int start, int end);
struct slist *slist_insert(struct slist *list, int pos, int value);
struct slist *slist_delete(struct slist *list, int pos);
struct slist *slist_map(struct slist *list, int (*fn)(int));
struct slist *slist_filter(struct slist *list, bool (*fn)(int));
char *slist_to_json(struct slist *list);
struct slist *slist_from_json(char *json);
