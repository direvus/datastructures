#include <stdio.h>
#include "slist.h"

void print_slist(struct slist *list) {
    printf("slist(%d):\n", slist_length(list));
    for(; list != 0; list = list->next) {
        printf("%p %d\n", (void *)list->next, list->value);
    }
    printf("\n");
}

int main(void) {
    struct slist *list = slist_create(1);
    if(list != 0) {
        slist_append(list, 1);
        slist_append(list, 2);
        slist_append(list, 3);
        print_slist(list);
        printf("\n");

        list = slist_insert(list, 0, 0);
        print_slist(list);
        slist_destroy(list);
    }
    return 0;
}
