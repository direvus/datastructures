#include <check.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./util.h"
#include "../slisti.h"

START_TEST(test_slisti_create) {
    struct slisti *list = slisti_create(0);
    ck_assert_ptr_nonnull(list);
    ck_assert_ptr_null(list->next);
    ck_assert_int_eq(list->value, 0);
    free(list);
}
END_TEST

START_TEST(test_slisti_from_array) {
    const int SIZE = 10;
    int input[] = {-339477778, 1951527226, 1011318566, -104064784, 501816327, 1320182898, 1345528803, 1262206431, 567697681, 1208321048};

    ck_assert_ptr_null(slisti_from_array(input, 0));

    struct slisti *list = slisti_from_array(input, SIZE);
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(slisti_length(list), SIZE);

    struct slisti *cell = list;
    for(int i = 0; i < SIZE && cell; i++, cell = cell->next) {
        ck_assert_int_eq(input[i], cell->value);
    }
    slisti_destroy(list);
}
END_TEST

START_TEST(test_slisti_length) {
    struct slisti *list = slisti_create(0);
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(slisti_length(0), 0);
    ck_assert_int_eq(slisti_length(list), 1);

    slisti_append(list, 1);
    ck_assert_int_eq(slisti_length(list), 2);

    for(int i = 2; i < 1000; i++) {
        slisti_append(list, i);
    }
    ck_assert_int_eq(slisti_length(list), 1000);
    slisti_destroy(list);
}
END_TEST

START_TEST(test_slisti_append) {
    struct slisti *list = slisti_create(0);
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(slisti_length(list), 1);

    struct slisti *cell1 = slisti_append(list, 1);
    ck_assert_int_eq(cell1->value, 1);
    ck_assert_ptr_null(cell1->next);
    ck_assert(list->next == cell1);
    ck_assert_int_eq(slisti_length(list), 2);

    struct slisti *cell2 = slisti_append(list, 2);
    ck_assert_int_eq(cell2->value, 2);
    ck_assert_ptr_null(cell2->next);
    ck_assert(cell1->next == cell2);

    ck_assert_int_eq(slisti_length(list), 3);
    slisti_destroy(list);
}
END_TEST

START_TEST(test_slisti_insert) {
    struct slisti *cell0 = slisti_create(0);
    /* [0] */
    ck_assert_ptr_nonnull(cell0);
    ck_assert_int_eq(slisti_length(cell0), 1);

    struct slisti *cell1 = slisti_insert(cell0, 0, 1);
    /* [1, 0] */
    ck_assert_int_eq(cell1->value, 1);
    ck_assert_ptr_nonnull(cell1->next);
    ck_assert_ptr_null(cell0->next);
    ck_assert(cell1->next == cell0);
    ck_assert_int_eq(slisti_length(cell1), 2);

    struct slisti *list = slisti_insert(cell1, 10, 2);
    /* [1, 0, 2] */
    ck_assert_int_eq(slisti_length(list), 3);

    struct slisti *cell2 = list;
    while(cell2->next) {
        cell2 = cell2->next;
    }
    ck_assert_ptr_null(cell2->next);
    ck_assert_int_eq(cell2->value, 2);
    ck_assert(cell0->next == cell2);

    list = slisti_insert(list, -1, 3);
    /* [1, 0, 3, 2] */
    ck_assert_int_eq(slisti_length(list), 4);
    struct slisti *cell3;
    for(cell3 = list; cell3->value != 3; cell3 = cell3->next) {
    }
    ck_assert_ptr_nonnull(cell3->next);
    ck_assert_int_eq(cell3->value, 3);
    ck_assert(cell0->next == cell3);
    ck_assert(cell3->next == cell2);
    ck_assert_ptr_null(cell2->next);

    slisti_destroy(list);
}
END_TEST

START_TEST(test_slisti_get) {
    struct slisti *list = slisti_create(0);
    ck_assert_ptr_nonnull(list);
    slisti_append(list, 1);
    slisti_append(list, 2);
    slisti_append(list, 3);
    slisti_append(list, 4);

    struct slisti *cell = slisti_get(list, 0);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 0);

    cell = slisti_get(list, 1);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 1);

    cell = slisti_get(list, 2);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 2);

    cell = slisti_get(list, 3);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 3);

    cell = slisti_get(list, 4);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 4);

    cell = slisti_get(list, -5);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 0);

    cell = slisti_get(list, -4);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 1);

    cell = slisti_get(list, -3);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 2);

    cell = slisti_get(list, -2);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 3);

    cell = slisti_get(list, -1);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 4);

    /* Out of bounds */
    cell = slisti_get(list, 5);
    ck_assert_ptr_null(cell);

    cell = slisti_get(list, -6);
    ck_assert_ptr_null(cell);

    slisti_destroy(list);
}
END_TEST

START_TEST(test_slisti_find) {
    ck_assert_ptr_null(slisti_find(0, 0));

    struct slisti *list = slisti_from_array((int[]){0, 1, 2, 3, 4}, 5);
    ck_assert_ptr_nonnull(list);

    struct slisti *cell = slisti_find(list, 0);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 0);

    cell = slisti_find(list, 1);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 1);

    cell = slisti_find(list, 2);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 2);

    cell = slisti_find(list, 3);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 3);

    cell = slisti_find(list, 4);
    ck_assert_ptr_nonnull(cell);
    ck_assert_int_eq(cell->value, 4);

    cell = slisti_find(list, 5);
    ck_assert_ptr_null(cell);

    cell = slisti_find(list, -1);
    ck_assert_ptr_null(cell);

    slisti_destroy(list);
}
END_TEST

START_TEST(test_slisti_slice) {
    ck_assert_ptr_null(slisti_slice(0, 0, 0));

    struct slisti *list = slisti_create(0);
    ck_assert_ptr_nonnull(list);
    slisti_append(list, 1);
    slisti_append(list, 2);
    slisti_append(list, 3);
    slisti_append(list, 4);

    /* Slices that include no cells. */
    ck_assert_ptr_null(slisti_slice(list, 0, 0));
    ck_assert_ptr_null(slisti_slice(list, 1, 0));
    ck_assert_ptr_null(slisti_slice(list, -1, 0));
    ck_assert_ptr_null(slisti_slice(list, 2, 1));
    ck_assert_ptr_null(slisti_slice(list, 5, 6));
    ck_assert_ptr_null(slisti_slice(list, -1, -2));

    struct slisti *slice = slisti_slice(list, 0, 1);
    ck_assert_ptr_nonnull(slice);
    ck_assert_int_eq(slisti_length(slice), 1);
    ck_assert_int_eq(slice->value, 0);
    slisti_destroy(slice);

    slice = slisti_slice(list, 0, -1);
    ck_assert_ptr_nonnull(slice);
    ck_assert_int_eq(slisti_length(slice), 4);
    ck_assert_int_eq(slice->value, 0);
    ck_assert_int_eq(slisti_get(slice, 1)->value, 1);
    ck_assert_int_eq(slisti_get(slice, 2)->value, 2);
    ck_assert_int_eq(slisti_get(slice, 3)->value, 3);
    slisti_destroy(slice);

    slice = slisti_slice(list, -2, -1);
    ck_assert_ptr_nonnull(slice);
    ck_assert_int_eq(slisti_length(slice), 1);
    ck_assert_int_eq(slice->value, 3);
    slisti_destroy(slice);

    slisti_destroy(list);
}
END_TEST

START_TEST(test_slisti_delete) {
    ck_assert_ptr_null(slisti_delete(0, 0));

    struct slisti *list = slisti_create(0);
    ck_assert_ptr_nonnull(list);
    slisti_append(list, 1);
    slisti_append(list, 2);
    slisti_append(list, 3);
    slisti_append(list, 4);

    list = slisti_delete(list, 0);
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(list->value, 1);
    ck_assert_int_eq(slisti_length(list), 4);

    /* Non-existent index: positive */
    ck_assert(list == slisti_delete(list, 4));
    ck_assert_int_eq(slisti_length(list), 4);

    /* Non-existent index: negative */
    ck_assert(list == slisti_delete(list, -5));
    ck_assert_int_eq(slisti_length(list), 4);

    ck_assert(list == slisti_delete(list, 2));
    ck_assert_int_eq(slisti_length(list), 3);

    ck_assert(list == slisti_delete(list, 1));
    ck_assert_int_eq(slisti_length(list), 2);

    ck_assert(list == slisti_delete(list, -1));
    ck_assert_ptr_null(list->next);
    ck_assert_int_eq(list->value, 1);

    list = slisti_delete(list, 0);
    ck_assert_ptr_null(list);
}
END_TEST

int mod2(int x) {
    return x % 2;
}

START_TEST(test_slisti_map) {
    struct slisti *source = slisti_create(0);
    ck_assert_ptr_nonnull(source);
    struct slisti *dest = slisti_map(source, &mod2);
    ck_assert_ptr_nonnull(dest);
    ck_assert_int_eq(slisti_length(dest), 1);
    ck_assert_int_eq(dest->value, 0);
    slisti_destroy(dest);

    slisti_append(source, 1);
    slisti_append(source, 2);
    slisti_append(source, 3);
    slisti_append(source, -1);
    slisti_append(source, -2);
    slisti_append(source, -3);
    dest = slisti_map(source, &mod2);
    ck_assert_ptr_nonnull(dest);
    ck_assert_int_eq(slisti_length(dest), 7);
    ck_assert_int_eq(dest->value, 0);
    ck_assert_int_eq(slisti_get(dest, 1)->value, 1);
    ck_assert_int_eq(slisti_get(dest, 2)->value, 0);
    ck_assert_int_eq(slisti_get(dest, 3)->value, 1);
    ck_assert_int_eq(slisti_get(dest, 4)->value, -1);
    ck_assert_int_eq(slisti_get(dest, 5)->value, 0);
    ck_assert_int_eq(slisti_get(dest, 6)->value, -1);
    slisti_destroy(dest);

    slisti_destroy(source);
}
END_TEST

bool is_even(int x) {
    return (x % 2) == 0;
}

START_TEST(test_slisti_filter) {
    struct slisti *source = slisti_create(0);
    ck_assert_ptr_nonnull(source);
    struct slisti *dest = slisti_filter(source, &is_even);
    ck_assert_ptr_nonnull(dest);
    ck_assert_int_eq(slisti_length(dest), 1);
    ck_assert_int_eq(dest->value, 0);
    slisti_destroy(dest);

    slisti_append(source, 1);
    slisti_append(source, 2);
    slisti_append(source, 3);
    slisti_append(source, -1);
    slisti_append(source, -2);
    slisti_append(source, -3);
    dest = slisti_filter(source, &is_even);
    ck_assert_ptr_nonnull(dest);
    ck_assert_int_eq(slisti_length(dest), 3);
    ck_assert_int_eq(dest->value, 0);
    ck_assert_int_eq(slisti_get(dest, 1)->value, 2);
    ck_assert_int_eq(slisti_get(dest, 2)->value, -2);
    slisti_destroy(dest);

    slisti_destroy(source);
}
END_TEST

int add(int x, int y) {
    return x + y;
}

START_TEST(test_slisti_reduce) {
    struct slisti *source = slisti_create(0);
    ck_assert_ptr_nonnull(source);
    int result = slisti_reduce(source, &add);
    ck_assert_int_eq(result, 0);

    slisti_append(source, 1);
    slisti_append(source, 2);
    slisti_append(source, 3);
    slisti_append(source, -1);
    result = slisti_reduce(source, &add);
    ck_assert_int_eq(result, 5);

    slisti_destroy(source);
}
END_TEST

START_TEST(test_slisti_to_json) {
    char *json = slisti_to_json(0);
    ck_assert_str_eq(json, "[]");
    free(json);

    struct slisti *list = slisti_create(0);
    json = slisti_to_json(list);
    ck_assert_str_eq(json, "[0]");
    free(json);

    slisti_append(list, 1);
    json = slisti_to_json(list);
    ck_assert_str_eq(json, "[0,1]");
    free(json);
    slisti_destroy(list);
    list = 0;

    const int SIZE = 10;
    int input[] = {-339477778, 1951527226, 1011318566, -104064784, 501816327, 1320182898, 1345528803, 1262206431, 567697681, 1208321048};
    list = slisti_from_array(input, SIZE);
    json = slisti_to_json(list);
    ck_assert_str_eq(json, "[-339477778,1951527226,1011318566,-104064784,501816327,1320182898,1345528803,1262206431,567697681,1208321048]");
    free(json);
    slisti_destroy(list);
}
END_TEST

START_TEST(test_slisti_from_json) {
    /* Various bogus inputs */
    ck_assert_ptr_null(slisti_from_json(""));
    ck_assert_ptr_null(slisti_from_json("{}"));
    ck_assert_ptr_null(slisti_from_json("[]"));
    ck_assert_ptr_null(slisti_from_json("[[1,2], [3,4]]"));
    ck_assert_ptr_null(slisti_from_json("[0, 1, {}]"));

    struct slisti *list = slisti_from_json("[0]");
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(slisti_length(list), 1);
    ck_assert_int_eq(list->value, 0);
    slisti_destroy(list);

    list = slisti_from_json("\n[\n  1,\n  -1\n] ");
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(slisti_length(list), 2);
    ck_assert_int_eq(list->value, 1);
    ck_assert_int_eq(list->next->value, -1);
    slisti_destroy(list);

    list = slisti_from_json("[-339477778,1951527226,1011318566,-104064784,501816327,1320182898,1345528803,1262206431,567697681,1208321048]");
    ck_assert_ptr_nonnull(list);
    ck_assert_int_eq(slisti_length(list), 10);
    slisti_destroy(list);
}
END_TEST

Suite *slisti_suite(void) {
    Suite *s;
    TCase *tc;

    s = suite_create("Singly-linked list");
    tc = tcase_create("Core");

    tcase_add_test(tc, test_slisti_create);
    tcase_add_test(tc, test_slisti_from_array);
    tcase_add_test(tc, test_slisti_length);
    tcase_add_test(tc, test_slisti_append);
    tcase_add_test(tc, test_slisti_insert);
    tcase_add_test(tc, test_slisti_delete);
    tcase_add_test(tc, test_slisti_get);
    tcase_add_test(tc, test_slisti_find);
    tcase_add_test(tc, test_slisti_slice);
    tcase_add_test(tc, test_slisti_map);
    tcase_add_test(tc, test_slisti_filter);
    tcase_add_test(tc, test_slisti_reduce);
    tcase_add_test(tc, test_slisti_to_json);
    tcase_add_test(tc, test_slisti_from_json);
    suite_add_tcase(s, tc);

    return s;
}

int main(void) {
    int fails;
    Suite *s;
    SRunner *sr;

    s = slisti_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    fails = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
