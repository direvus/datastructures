#include <check.h>
#include <stdlib.h>
#include "./util.h"
#include "../hash.h"

START_TEST(test_hash_shimmy2_zero) {
    ck_assert_int_eq(hash_shimmy2(NULL), 0);
    ck_assert_int_eq(hash_shimmy2(""), 0);
    ck_assert_int_eq(hash_shimmy2("\0"), 0);
}
END_TEST

START_TEST(test_hash_shimmy2_nonzero) {
    ck_assert_int_gt(hash_shimmy2("a"), 0);
    ck_assert_int_gt(hash_shimmy2(" "), 0);
}
END_TEST

START_TEST(test_hash_shimmy2_variance) {
    ck_assert_int_ne(hash_shimmy2("a"), hash_shimmy2("aa"));
    ck_assert_int_ne(hash_shimmy2("aaa"), hash_shimmy2("aa"));
}
END_TEST

Suite *hash_suite(void) {
    Suite *s;
    TCase *tc;

    s = suite_create("Hash functions");
    tc = tcase_create("Shimmy2");

    tcase_add_test(tc, test_hash_shimmy2_zero);
    tcase_add_test(tc, test_hash_shimmy2_nonzero);
    tcase_add_test(tc, test_hash_shimmy2_variance);
    suite_add_tcase(s, tc);

    return s;
}

int main(void) {
    int fails;
    Suite *s;
    SRunner *sr;

    s = hash_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    fails = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
