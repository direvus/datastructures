#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include "slisti.h"

/*
 * Return the number of elements in the list.
 */
int slisti_length(const struct slisti *list) {
    if(!list) {
        return 0;
    }
    int length = 1;
    while(list->next) {
        list = list->next;
        length++;
    }
    return length;
}

/*
 * Create a new list cell with the given value.  Return a pointer
 * to the new cell, or NULL on failure.
 */
struct slisti *slisti_create(const int value) {
    struct slisti *cell;
    cell = malloc(sizeof *cell);
    if(cell) {
        cell->next = 0;
        cell->value = value;
    }
    return cell;
}

/*
 * Create a new list with values from the first 'num' elements of 'input'.
 *
 * Return a pointer to the first cell of the new list, or NULL on failure.
 */
struct slisti *slisti_from_array(const int input[], const int num) {
    struct slisti *head = 0;
    struct slisti *tail = 0;
    struct slisti *cell = 0;
    for(int i = 0; i < num; i++) {
        cell = slisti_create(input[i]);
        if(!head) {
            head = cell;
        }
        if(tail) {
            tail->next = cell;
        }
        tail = cell;
    }
    return head;
}

/*
 * Unallocate all cells in the given slisti.
 */
void slisti_destroy(struct slisti *list) {
    if(!list) {
        return;
    }
    struct slisti *next = list->next;
    free(list);
    list = 0;
    while(next) {
        list = next;
        next = list->next;
        free(list);
        list = 0;
    }
}

/*
 * Append an integer to the end of an slisti as a new cell.  Return
 * a pointer to the new cell created, or NULL on failure.
 */
struct slisti *slisti_append(struct slisti *list, int value) {
    if(!list) {
        return 0;
    }
    struct slisti *cell = slisti_create(value);
    if(cell) {
        while(list->next) {
            list = list->next;
        }
        list->next = cell;
    }
    return cell;
}

/*
 * Get the slisti cell in 'list' at the given position.
 *
 * Non-negative values of 'pos' are counted from 'list' as position zero.
 * Negative values of 'pos' are counted from the last element as position -1.
 *
 * Return a NULL pointer if the requested position does not exist in the list.
 */
struct slisti *slisti_get(struct slisti *list, int pos) {
    int len = slisti_length(list);
    if(pos < 0) {
        pos += len;
        if(pos < 0) {
            return 0;
        }
    }
    if(pos >= len) {
        return 0;
    }
    for(int i = 0; list; list = list->next, i++) {
        if(i == pos) {
            return list;
        }
    }
    return 0;
}

/*
 * Return a pointer to the first cell in 'list' that contains 'value'.
 *
 * Return a NULL pointer if no such cell can be found.
 */
struct slisti *slisti_find(struct slisti *list, int value) {
    if(!list) {
        return 0;
    }
    for(; list; list = list->next) {
        if(list->value == value) {
            return list;
        }
    }
    return 0;
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
struct slisti *slisti_insert(struct slisti *list, int pos, int value) {
    struct slisti *new = slisti_create(value);
    if(!new) {
        return 0;
    }
    if(pos < 0) {
        pos += slisti_length(list);
        if(pos < 0) {
            pos = 0;
        }
    }
    int i = 0;
    struct slisti *prev = 0;
    struct slisti *cell;
    for(cell = list; cell; cell = cell->next, i++) {
        if(i == pos) {
            new->next = cell;
            if(prev) {
                prev->next = new;
                return list;
            } else {
                return new;
            }
        }
        prev = cell;
    }

    // Never arrived at target position, append instead.
    if(prev) {
        prev->next = new;
        return list;
    } else {
        return new;
    }
}

/*
 * Delete a cell from the list at the given position.
 *
 * Non-negative values of 'pos' are counted from 'list' as position zero.
 * Negative values of 'pos' are counted from the last element as position -1.
 *
 * If the requested position does not exist in the list, do nothing.
 *
 * The deleted cell's memory is freed.
 *
 * Return a pointer to the first cell of the resulting list, or NULL if no
 * cells remain.
 */
struct slisti *slisti_delete(struct slisti *list, int pos) {
    if(!list) {
        return 0;
    }
    if(pos < 0) {
        pos += slisti_length(list);
        if(pos < 0) {
            return list;
        }
    }
    struct slisti *prev = 0;
    struct slisti *cell = list;
    for(int i = 0; cell; cell = cell->next, i++) {
        if(i == pos) {
            if(prev) {
                prev->next = cell->next;
            } else {
                list = cell->next;
            }
            free(cell);
            return list;
        }
        prev = cell;
    }
    return list;
}

/*
 * Extract a slice from the given list.
 *
 * Return a pointer to the first cell of a new slisti composed of elements from
 * the source list, beginning at position 'start' and up to (but not including)
 * position 'end'.
 *
 * Non-negative positions will be counted starting from the first cell as
 * position zero.  Negative positions will be be counted from the last cell as
 * position -1.
 *
 * If the requested range does not include any cells, return a NULL pointer.
 */
struct slisti *slisti_slice(const struct slisti *source, int start, int end) {
    if(!source){
        return 0;
    }
    int length = slisti_length(source);
    if(start < 0) {
        start += length;
    }
    if(end < 0) {
        end += length;
    }
    if(end <= start) {
        return 0;
    }
    struct slisti *head = 0;
    struct slisti *tail = 0;
    struct slisti *cell = 0;
    for(int i = 0; i < end && source; i++, source = source->next) {
        if(i >= start) {
            cell = slisti_create(source->value);
            if(!head) {
                head = cell;
            }
            if(tail) {
                tail->next = cell;
            }
            tail = cell;
        }
    }
    return head;
}

/*
 * Create a new slisti by applying a map function to each element in source.
 *
 * The 'fn' argument is a pointer to a map function which accepts an integer
 * element from the source list, and returns the desired integer to be added to
 * the destination list.
 *
 * Return a pointer to the first cell of the new slisti.
 */
struct slisti *slisti_map(const struct slisti *source, int (*fn)(int)) {
    struct slisti *head = 0;
    struct slisti *tail = 0;
    struct slisti *cell = 0;
    while(source) {
        cell = slisti_create(fn(source->value));
        if(!head) {
            head = cell;
        }
        if(tail) {
            tail->next = cell;
        }
        tail = cell;
        source = source->next;
    }
    return head;
}

/*
 * Create a new slisti by applying a filter function to each element in source.
 *
 * The 'fn' argument is a pointer to a filter function which accepts an integer
 * element from the source list, and returns a boolean to indicate whether that
 * element ought to be included in the result list.
 *
 * Return a pointer to the first cell of the new slisti.
 */
struct slisti *slisti_filter(const struct slisti *source, bool (*fn)(int)) {
    struct slisti *head = 0;
    struct slisti *tail = 0;
    struct slisti *cell = 0;
    while(source) {
        if(fn(source->value)) {
            cell = slisti_create(source->value);
            if(!head) {
                head = cell;
            }
            if(tail) {
                tail->next = cell;
            }
            tail = cell;
        }
        source = source->next;
    }
    return head;
}

/*
 * Return an integer by applying a reduce function to each element in source.
 *
 * The 'fn' argument is a pointer to a reduce function which accepts an integer
 * state value, and an integer element from the source list, and returns the
 * new state value.  State values are initialised to zero.
 */
int slisti_reduce(const struct slisti *source, int (*fn)(int, int)) {
    int state = 0;
    while(source) {
        state = fn(state, source->value);
        source = source->next;
    }
    return state;
}

/*
 * Return the given list formatted as compact JSON.
 *
 * The result is a newly malloc'd string.  It is the caller's responsibility to
 * free the string.
 */
char *slisti_to_json(const struct slisti *list) {
    /*
     * Get number of bytes needed to write the longest int value.  Assume it's
     * not more than 64 bytes.
     */
    int LEN = 64;
    char buf[LEN];
    int int_size = snprintf(buf, LEN, "%d", INT_MIN);
    int length = slisti_length(list);

    /*
     * All integers in the list, plus comma delimiters, plus surrounding square
     * brackets, plus terminating NUL.
     */
    int size = (length * (int_size + 1)) + 3;
    char *result = malloc(size);
    memset(result, 0, size);
    result[0] = '[';
    int pos = 1;
    for(; list; list = list->next) {
        pos += snprintf(&result[pos], size - pos, "%d", list->value);
        if(list->next) {
            result[pos++] = ',';
        }
    }
    result[pos] = ']';
    return result;
}

/*
 * Return a pointer to a new list built from nul-terminated JSON ASCII text.
 *
 * The JSON text must decode to a single flat list of integers.  If the JSON
 * text does not represent a list, or if the list is empty, or if any of the
 * list's elements cannot be converted to an int, return a NULL pointer.
 */
struct slisti *slisti_from_json(const char *json) {
    /* Skip whitespace */
    while(isspace(*json)) {
        json++;
    }

    if(*(json++) != '[') {
        return 0;
    }

    struct slisti *head = 0;
    struct slisti *tail = 0;
    struct slisti *cell = 0;
    char *pos = 0;
    long int value;
    while(*json != '\0') {
        errno = 0;
        value = strtol(json, &pos, 10);
        if(json == pos || errno > 0 || value < INT_MIN || value > INT_MAX) {
            break;
        }
        json = pos;
        cell = slisti_create((int) value);
        if(!head) {
            head = cell;
        }
        if(tail) {
            tail->next = cell;
        }
        tail = cell;

        while(isspace(*json)) {
            json++;
        }
        if(*json == ']') {
            return head;
        } else if(*(json++) != ',') {
            break;
        }
    }
    slisti_destroy(head);
    return 0;
}
