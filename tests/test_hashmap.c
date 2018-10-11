#include <check.h>
#include <stdlib.h>
#include "./util.h"
#include "../hashmap.h"

START_TEST(test_hashmap_create) {
    struct hashmap *m = hashmap_create();
    ck_assert_ptr_nonnull(m);
    ck_assert_int_gt(m->size, 0);
    ck_assert_int_eq(m->count, 0);
    for (size_t i = 0; i < m->size; i++) {
        ck_assert_ptr_null(m->buckets[i]);
    }
    free(m);
}
END_TEST

START_TEST(test_hashmap_destroy) {
    struct hashmap *m = hashmap_create();
    /* Just checking it doesn't crash */
    hashmap_destroy(m);
}
END_TEST

START_TEST(test_hashmap_set) {
    struct hashmap *m = hashmap_create();
    ck_assert_ptr_nonnull(m);

    /* NULL value */
    ck_assert(!hashmap_set(m, "a", 0));

    int *v;

    /* Set value in empty map */
    v = malloc(sizeof *v);
    *v = 0;
    ck_assert(hashmap_set(m, "a", v));

    /* Overwrite value for existing key */
    v = malloc(sizeof *v);
    *v = 1;
    ck_assert(hashmap_set(m, "a", v));

    /* Set values in colliding keys */
    ck_assert(hashmap_set(m, "anear", v));
    ck_assert(hashmap_set(m, "dicot", v));

    hashmap_destroy(m);
}
END_TEST

START_TEST(test_hashmap_get) {
    struct hashmap *m = hashmap_create();
    ck_assert_ptr_nonnull(m);

    int *v, *p;

    /* Non-existent key */
    p = hashmap_get(m, "absent");
    ck_assert_ptr_null(p);

    /* Value in empty map */
    v = malloc(sizeof *v);
    *v = 0;
    hashmap_set(m, "a", v);
    p = (int *) hashmap_get(m, "a");
    ck_assert_ptr_nonnull(p);
    ck_assert_int_eq(*p, *v);

    /* Overwrite value for existing key */
    v = malloc(sizeof *v);
    *v = 1;
    hashmap_set(m, "a", v);
    p = (int *) hashmap_get(m, "a");
    ck_assert_ptr_nonnull(p);
    ck_assert_int_eq(*p, *v);

    /* Set values in colliding keys */
    v = malloc(sizeof *v);
    *v = 2;
    hashmap_set(m, "anear", v);
    v = malloc(sizeof *v);
    *v = 3;
    hashmap_set(m, "dicot", v);

    p = (int *) hashmap_get(m, "anear");
    ck_assert_ptr_nonnull(p);
    ck_assert_int_eq(*p, 2);

    p = (int *) hashmap_get(m, "dicot");
    ck_assert_ptr_nonnull(p);
    ck_assert_int_eq(*p, 3);

    hashmap_destroy(m);
}
END_TEST

Suite *hashmap_suite(void) {
    Suite *s;
    TCase *tc;

    s = suite_create("Hashmap");
    tc = tcase_create("Create/destroy");
    tcase_add_test(tc, test_hashmap_create);
    tcase_add_test(tc, test_hashmap_destroy);
    suite_add_tcase(s, tc);

    tc = tcase_create("Set/get");
    tcase_add_test(tc, test_hashmap_set);
    tcase_add_test(tc, test_hashmap_get);
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
