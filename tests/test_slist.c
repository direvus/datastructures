#include <check.h>
#include <stdlib.h>
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

Suite *slist_suite(void) {
    Suite *s;
    TCase *tc;

    s = suite_create("Singly-linked list");
    tc = tcase_create("Core");

    tcase_add_test(tc, test_slist_create);
    tcase_add_test(tc, test_slist_length);
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
