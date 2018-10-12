#include <stdio.h>
#include "slisti.h"

void print_slisti(struct slisti *list) {
    printf("slisti(%d):\n", slisti_length(list));
    for(; list != 0; list = list->next) {
        printf("%p %d\n", (void *)list->next, list->value);
    }
    printf("\n");
}

int main(void) {
    struct slisti *list = slisti_create(1);
    if(list != 0) {
        slisti_append(list, 1);
        slisti_append(list, 2);
        slisti_append(list, 3);
        print_slisti(list);
        printf("\n");

        list = slisti_insert(list, 0, 0);
        print_slisti(list);
        slisti_destroy(list);
    }
    return 0;
}
