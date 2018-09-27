#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "slist.h"

/*
 * Return the number of elements in the list.
 */
int slist_length(struct slist *list) {
    if(list == 0) {
        return 0;
    }
    int length = 1;
    while(list->next != 0) {
        list = list->next;
        length++;
    }
    return length;
}

/*
 * Create a new list cell with the given value.  Return a pointer
 * to the new cell, or NULL on failure.
 */
struct slist *slist_create(int value) {
    struct slist *cell;
    cell = malloc(sizeof *cell);
    if(cell != 0) {
        cell->value = value;
    }
    return cell;
}

/*
 * Unallocate all cells in the given slist.
 */
void slist_destroy(struct slist *list) {
    if(list == 0) {
        return;
    }
    struct slist *next = list->next;
    free(list);
    while(next != 0) {
        list = next;
        next = list->next;
        free(list);
    }
}

/*
 * Append an integer to the end of an slist as a new cell.  Return
 * a pointer to the new cell created, or NULL on failure.
 */
struct slist *slist_append(struct slist *list, int value) {
    if(list == 0) {
        return 0;
    }
    struct slist *cell = slist_create(value);
    if(cell != 0) {
        while(list->next != 0) {
            list = list->next;
        }
        list->next = cell;
    }
    return cell;
}

/*
 * Insert an integer as a new cell into list at the given position.
 * Return a pointer to the first cell of the list, or NULL on
 * failure.
 *
 * A negative position will be counted from the end of the list, -1
 * meaning the last element, -2 the second last, and so on.
 *
 * If the given position is too high, the new cell will be appended
 * to the end of the list.  If the position is too low, the cell
 * will be inserted at the beginning of the list.
 */
struct slist *slist_insert(struct slist *list, int pos, int value) {
    struct slist *new = slist_create(value);
    if(new == 0) {
        return 0;
    }
    if(pos < 0) {
        pos += slist_length(list);
        if(pos < 0) {
            pos = 0;
        }
    }
    int i = 0;
    struct slist *prev = 0;
    struct slist *cell;
    for(cell = list; cell != 0; cell = cell->next, i++) {
        if(i == pos) {
            new->next = cell;
            if(prev != 0) {
                prev->next = new;
                return list;
            } else {
                return new;
            }
        }
        prev = cell;
    }

    // Never arrived at target position, append instead.
    if(prev != 0) {
        prev->next = new;
        return list;
    } else {
        return new;
    }
}

/*
 * Return the given list formatted as compact JSON.
 *
 * The result is a newly malloc'd string.  It is the caller's responsibility to
 * free the string.
 */
char *slist_json(struct slist *list) {
    /*
     * Get number of bytes needed to write the longest int value.  Assume it's
     * not more than 64 bytes.
     */
    int LEN = 64;
    char buf[LEN];
    int int_size = snprintf(buf, LEN, "%d", INT_MIN);
    int length = slist_length(list);

    /*
     * All integers in the list, plus comma delimiters, plus surrounding square
     * brackets, plus terminating NUL.
     */
    int size = (length * (int_size + 1)) + 3;
    char *result = malloc(size);
    memset(result, 0, size);
    result[0] = '[';
    int pos = 1;
    for(; list != 0; list = list->next) {
        pos += snprintf(&result[pos], size - pos, "%d", list->value);
        if(list->next != 0) {
            result[pos++] = ',';
        }
    }
    result[pos] = ']';
    return result;
}
