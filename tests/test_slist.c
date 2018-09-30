#include <check.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../slist.h"

/*
 * Fills for antique versions of libcheck (thanks, TravisCI).
 */
#ifndef ck_assert_ptr_nonnull
#define ck_assert_ptr_nonnull(X)  (ck_assert(X != 0))
#endif
#ifndef ck_assert_ptr_null
#define ck_assert_ptr_null(X)  (ck_assert(X == 0))
#endif

START_TEST(test_slist_create) {
    struct slist *list = slist_create(0);
    ck_assert_ptr_nonnull(list);
    ck_assert_ptr_null(list->next);
    ck_assert_int_eq(list->value, 0);
    free(list);
}
END_TEST

START_TEST(test_slist_from_array) {
    const int SIZE = 10;
    int input[] = {-339477778, 1951527226, 1011318566, -104064784, 501816327, 1320182898, 1345528803, 1262206431, 567697681, 1208321048};

    ck_assert_ptr_null(slist_from_array(input, 0));

    struct slist *list = slist_from_array(input, SIZE);
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(slist_length(list), SIZE);

    struct slist *cell = list;
    for(int i = 0; i < SIZE && cell; i++, cell = cell->next) {
        ck_assert_int_eq(input[i], cell->value);
    }
    slist_destroy(list);
}
END_TEST

START_TEST(test_slist_length) {
    struct slist *list = slist_create(0);
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(slist_length(0), 0);
    ck_assert_int_eq(slist_length(list), 1);

    slist_append(list, 1);
    ck_assert_int_eq(slist_length(list), 2);

    for(int i = 2; i < 1000; i++) {
        slist_append(list, i);
    }
    ck_assert_int_eq(slist_length(list), 1000);
    slist_destroy(list);
}
END_TEST

START_TEST(test_slist_append) {
    struct slist *list = slist_create(0);
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(slist_length(list), 1);

    struct slist *cell1 = slist_append(list, 1);
    ck_assert_int_eq(cell1->value, 1);
    ck_assert_ptr_null(cell1->next);
    ck_assert(list->next == cell1);
    ck_assert_int_eq(slist_length(list), 2);

    struct slist *cell2 = slist_append(list, 2);
    ck_assert_int_eq(cell2->value, 2);
    ck_assert_ptr_null(cell2->next);
    ck_assert(cell1->next == cell2);

    ck_assert_int_eq(slist_length(list), 3);
    slist_destroy(list);
}
END_TEST

START_TEST(test_slist_insert) {
    struct slist *cell0 = slist_create(0);
    /* [0] */
    ck_assert_ptr_nonnull(cell0);
    ck_assert_int_eq(slist_length(cell0), 1);

    struct slist *cell1 = slist_insert(cell0, 0, 1);
    /* [1, 0] */
    ck_assert_int_eq(cell1->value, 1);
    ck_assert_ptr_nonnull(cell1->next);
    ck_assert_ptr_null(cell0->next);
    ck_assert(cell1->next == cell0);
    ck_assert_int_eq(slist_length(cell1), 2);

    struct slist *list = slist_insert(cell1, 10, 2);
    /* [1, 0, 2] */
    ck_assert_int_eq(slist_length(list), 3);

    struct slist *cell2 = list;
    while(cell2->next) {
        cell2 = cell2->next;
    }
    ck_assert_ptr_null(cell2->next);
    ck_assert_int_eq(cell2->value, 2);
    ck_assert(cell0->next == cell2);

    list = slist_insert(list, -1, 3);
    /* [1, 0, 3, 2] */
    ck_assert_int_eq(slist_length(list), 4);
    struct slist *cell3;
    for(cell3 = list; cell3->value != 3; cell3 = cell3->next) {
    }
    ck_assert_ptr_nonnull(cell3->next);
    ck_assert_int_eq(cell3->value, 3);
    ck_assert(cell0->next == cell3);
    ck_assert(cell3->next == cell2);
    ck_assert_ptr_null(cell2->next);

    slist_destroy(list);
}
END_TEST

START_TEST(test_slist_get) {
    struct slist *list = slist_create(0);
    ck_assert_ptr_nonnull(list);
    slist_append(list, 1);
    slist_append(list, 2);
    slist_append(list, 3);
    slist_append(list, 4);

    struct slist *cell = slist_get(list, 0);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 0);

    cell = slist_get(list, 1);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 1);

    cell = slist_get(list, 2);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 2);

    cell = slist_get(list, 3);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 3);

    cell = slist_get(list, 4);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 4);

    cell = slist_get(list, -5);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 0);

    cell = slist_get(list, -4);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 1);

    cell = slist_get(list, -3);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 2);

    cell = slist_get(list, -2);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 3);

    cell = slist_get(list, -1);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 4);

    /* Out of bounds */
    cell = slist_get(list, 5);
    ck_assert_ptr_null(cell);

    cell = slist_get(list, -6);
    ck_assert_ptr_null(cell);

    slist_destroy(list);
}
END_TEST

START_TEST(test_slist_find) {
    ck_assert_ptr_null(slist_find(0, 0));

    struct slist *list = slist_from_array((int[]){0, 1, 2, 3, 4}, 5);
    ck_assert_ptr_nonnull(list);

    struct slist *cell = slist_find(list, 0);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 0);

    cell = slist_find(list, 1);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 1);

    cell = slist_find(list, 2);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 2);

    cell = slist_find(list, 3);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 3);

    cell = slist_find(list, 4);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 4);

    cell = slist_find(list, 5);
    ck_assert_ptr_null(cell);

    cell = slist_find(list, -1);
    ck_assert_ptr_null(cell);

    slist_destroy(list);
}
END_TEST

START_TEST(test_slist_slice) {
    ck_assert_ptr_null(slist_slice(0, 0, 0));

    struct slist *list = slist_create(0);
    ck_assert_ptr_nonnull(list);
    slist_append(list, 1);
    slist_append(list, 2);
    slist_append(list, 3);
    slist_append(list, 4);

    /* Slices that include no cells. */
    ck_assert_ptr_null(slist_slice(list, 0, 0));
    ck_assert_ptr_null(slist_slice(list, 1, 0));
    ck_assert_ptr_null(slist_slice(list, -1, 0));
    ck_assert_ptr_null(slist_slice(list, 2, 1));
    ck_assert_ptr_null(slist_slice(list, 5, 6));
    ck_assert_ptr_null(slist_slice(list, -1, -2));

    struct slist *slice = slist_slice(list, 0, 1);
    ck_assert_ptr_nonnull(slice);
    ck_assert_int_eq(slist_length(slice), 1);
    ck_assert_int_eq(slice->value, 0);
    slist_destroy(slice);

    slice = slist_slice(list, 0, -1);
    ck_assert_ptr_nonnull(slice);
    ck_assert_int_eq(slist_length(slice), 4);
    ck_assert_int_eq(slice->value, 0);
    ck_assert_int_eq(slist_get(slice, 1)->value, 1);
    ck_assert_int_eq(slist_get(slice, 2)->value, 2);
    ck_assert_int_eq(slist_get(slice, 3)->value, 3);
    slist_destroy(slice);

    slice = slist_slice(list, -2, -1);
    ck_assert_ptr_nonnull(slice);
    ck_assert_int_eq(slist_length(slice), 1);
    ck_assert_int_eq(slice->value, 3);
    slist_destroy(slice);

    slist_destroy(list);
}
END_TEST

START_TEST(test_slist_delete) {
    ck_assert_ptr_null(slist_delete(0, 0));

    struct slist *list = slist_create(0);
    ck_assert_ptr_nonnull(list);
    slist_append(list, 1);
    slist_append(list, 2);
    slist_append(list, 3);
    slist_append(list, 4);

    list = slist_delete(list, 0);
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(list->value, 1);
    ck_assert_int_eq(slist_length(list), 4);

    /* Non-existent index: positive */
    ck_assert(list == slist_delete(list, 4));
    ck_assert_int_eq(slist_length(list), 4);

    /* Non-existent index: negative */
    ck_assert(list == slist_delete(list, -5));
    ck_assert_int_eq(slist_length(list), 4);

    ck_assert(list == slist_delete(list, 2));
    ck_assert_int_eq(slist_length(list), 3);

    ck_assert(list == slist_delete(list, 1));
    ck_assert_int_eq(slist_length(list), 2);

    ck_assert(list == slist_delete(list, -1));
    ck_assert_ptr_null(list->next);
    ck_assert_int_eq(list->value, 1);

    list = slist_delete(list, 0);
    ck_assert_ptr_null(list);
}
END_TEST

int mod2(int x) {
    return x % 2;
}

START_TEST(test_slist_map) {
    struct slist *source = slist_create(0);
    ck_assert_ptr_nonnull(source);
    struct slist *dest = slist_map(source, &mod2);
    ck_assert_ptr_nonnull(dest);
    ck_assert_int_eq(slist_length(dest), 1);
    ck_assert_int_eq(dest->value, 0);
    slist_destroy(dest);

    slist_append(source, 1);
    slist_append(source, 2);
    slist_append(source, 3);
    slist_append(source, -1);
    slist_append(source, -2);
    slist_append(source, -3);
    dest = slist_map(source, &mod2);
    ck_assert_ptr_nonnull(dest);
    ck_assert_int_eq(slist_length(dest), 7);
    ck_assert_int_eq(dest->value, 0);
    ck_assert_int_eq(slist_get(dest, 1)->value, 1);
    ck_assert_int_eq(slist_get(dest, 2)->value, 0);
    ck_assert_int_eq(slist_get(dest, 3)->value, 1);
    ck_assert_int_eq(slist_get(dest, 4)->value, -1);
    ck_assert_int_eq(slist_get(dest, 5)->value, 0);
    ck_assert_int_eq(slist_get(dest, 6)->value, -1);
    slist_destroy(dest);

    slist_destroy(source);
}
END_TEST

bool is_even(int x) {
    return (x % 2) == 0;
}

START_TEST(test_slist_filter) {
    struct slist *source = slist_create(0);
    ck_assert_ptr_nonnull(source);
    struct slist *dest = slist_filter(source, &is_even);
    ck_assert_ptr_nonnull(dest);
    ck_assert_int_eq(slist_length(dest), 1);
    ck_assert_int_eq(dest->value, 0);
    slist_destroy(dest);

    slist_append(source, 1);
    slist_append(source, 2);
    slist_append(source, 3);
    slist_append(source, -1);
    slist_append(source, -2);
    slist_append(source, -3);
    dest = slist_filter(source, &is_even);
    ck_assert_ptr_nonnull(dest);
    ck_assert_int_eq(slist_length(dest), 3);
    ck_assert_int_eq(dest->value, 0);
    ck_assert_int_eq(slist_get(dest, 1)->value, 2);
    ck_assert_int_eq(slist_get(dest, 2)->value, -2);
    slist_destroy(dest);

    slist_destroy(source);
}
END_TEST

START_TEST(test_slist_to_json) {
    char *json = slist_to_json(0);
    ck_assert_str_eq(json, "[]");
    free(json);

    struct slist *list = slist_create(0);
    json = slist_to_json(list);
    ck_assert_str_eq(json, "[0]");
    free(json);

    slist_append(list, 1);
    json = slist_to_json(list);
    ck_assert_str_eq(json, "[0,1]");
    free(json);
    slist_destroy(list);
    list = 0;

    const int SIZE = 10;
    int input[] = {-339477778, 1951527226, 1011318566, -104064784, 501816327, 1320182898, 1345528803, 1262206431, 567697681, 1208321048};
    list = slist_from_array(input, SIZE);
    json = slist_to_json(list);
    ck_assert_str_eq(json, "[-339477778,1951527226,1011318566,-104064784,501816327,1320182898,1345528803,1262206431,567697681,1208321048]");
    free(json);
    slist_destroy(list);
}
END_TEST

START_TEST(test_slist_from_json) {
    /* Various bogus inputs */
    ck_assert_ptr_null(slist_from_json(""));
    ck_assert_ptr_null(slist_from_json("{}"));
    ck_assert_ptr_null(slist_from_json("[]"));
    ck_assert_ptr_null(slist_from_json("[[1,2], [3,4]]"));
    ck_assert_ptr_null(slist_from_json("[0, 1, {}]"));

    struct slist *list = slist_from_json("[0]");
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(slist_length(list), 1);
    ck_assert_int_eq(list->value, 0);
    slist_destroy(list);

    list = slist_from_json("\n[\n  1,\n  -1\n] ");
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(slist_length(list), 2);
    ck_assert_int_eq(list->value, 1);
    ck_assert_int_eq(list->next->value, -1);
    slist_destroy(list);

    list = slist_from_json("[-339477778,1951527226,1011318566,-104064784,501816327,1320182898,1345528803,1262206431,567697681,1208321048]");
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(slist_length(list), 10);
    slist_destroy(list);
}
END_TEST

Suite *slist_suite(void) {
    Suite *s;
    TCase *tc;

    s = suite_create("Singly-linked list");
    tc = tcase_create("Core");

    tcase_add_test(tc, test_slist_create);
    tcase_add_test(tc, test_slist_from_array);
    tcase_add_test(tc, test_slist_length);
    tcase_add_test(tc, test_slist_append);
    tcase_add_test(tc, test_slist_insert);
    tcase_add_test(tc, test_slist_delete);
    tcase_add_test(tc, test_slist_get);
    tcase_add_test(tc, test_slist_find);
    tcase_add_test(tc, test_slist_slice);
    tcase_add_test(tc, test_slist_map);
    tcase_add_test(tc, test_slist_filter);
    tcase_add_test(tc, test_slist_to_json);
    tcase_add_test(tc, test_slist_from_json);
    suite_add_tcase(s, tc);

    return s;
}

int main(void) {
    int fails;
    Suite *s;
    SRunner *sr;

    s = slist_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    fails = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
