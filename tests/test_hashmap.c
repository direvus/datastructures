#include <check.h>
#include <stdlib.h>
#include "./util.h"
#include "../hashmap.h"

START_TEST(test_hashmap_create) {
    struct hashmap *m = hashmap_create();
    ck_assert_ptr_nonnull(m);
    ck_assert_int_eq(m->size, HASHMAP_INIT_SIZE);
    ck_assert_int_eq(m->count, 0);
    for (int i = 0; i < HASHMAP_INIT_SIZE; i++) {
        ck_assert_ptr_null(m->buckets[i]);
    }
    free(m);
}
END_TEST

Suite *hashmap_suite(void) {
    Suite *s;
    TCase *tc;

    s = suite_create("Hashmap");
    tc = tcase_create("Core");

    tcase_add_test(tc, test_hashmap_create);
    suite_add_tcase(s, tc);

    return s;
}

int main(void) {
    int fails;
    Suite *s;
    SRunner *sr;

    s = hashmap_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    fails = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
