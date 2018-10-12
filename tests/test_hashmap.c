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
    ck_assert_int_eq(m->count, 1);

    /* Overwrite value for existing key */
    v = malloc(sizeof *v);
    *v = 1;
    ck_assert(hashmap_set(m, "a", v));
    ck_assert_int_eq(m->count, 1);

    /* Set values in colliding keys */
    ck_assert(hashmap_set(m, "anear", v));
    ck_assert_int_eq(m->count, 2);

    ck_assert(hashmap_set(m, "dicot", v));
    ck_assert_int_eq(m->count, 3);

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

START_TEST(test_hashmap_delete) {
    struct hashmap *m = hashmap_create();
    ck_assert_ptr_nonnull(m);

    /* Delete non-existent key */
    ck_assert(!hashmap_delete(m, "absent"));

    int *v;

    /* Set and delete in empty map */
    v = malloc(sizeof *v);
    *v = 0;
    hashmap_set(m, "a", v);
    ck_assert(hashmap_delete(m, "a"));
    ck_assert_int_eq(m->count, 0);
    ck_assert_ptr_null(hashmap_get(m, "a"));

    /* Set and delete colliding keys */
    v = malloc(sizeof *v);
    *v = 2;
    hashmap_set(m, "anear", v);
    v = malloc(sizeof *v);
    *v = 3;
    hashmap_set(m, "dicot", v);

    ck_assert(hashmap_delete(m, "anear"));
    ck_assert_ptr_null(hashmap_get(m, "anear"));
    ck_assert_int_eq(m->count, 1);

    ck_assert(hashmap_delete(m, "dicot"));
    ck_assert_ptr_null(hashmap_get(m, "dicot"));
    ck_assert_int_eq(m->count, 0);

    hashmap_destroy(m);
}
END_TEST

START_TEST(test_hashmap_exists) {
    struct hashmap *m = hashmap_create();
    ck_assert_ptr_nonnull(m);

    /* Check non-existent key */
    ck_assert(!hashmap_exists(m, "absent"));

    int *v;

    /* Set and check in empty map */
    v = malloc(sizeof *v);
    *v = 0;
    hashmap_set(m, "a", v);
    ck_assert(hashmap_exists(m, "a"));

    /* Set and check colliding keys */
    v = malloc(sizeof *v);
    *v = 2;
    hashmap_set(m, "anear", v);
    v = malloc(sizeof *v);
    *v = 3;
    hashmap_set(m, "dicot", v);

    ck_assert(hashmap_exists(m, "anear"));
    ck_assert(hashmap_exists(m, "dicot"));

    /* Delete and check */
    hashmap_delete(m, "anear");
    ck_assert(!hashmap_exists(m, "anear"));
    ck_assert(hashmap_exists(m, "dicot"));
    ck_assert(hashmap_exists(m, "a"));

    hashmap_delete(m, "dicot");
    ck_assert(!hashmap_exists(m, "anear"));
    ck_assert(!hashmap_exists(m, "dicot"));
    ck_assert(hashmap_exists(m, "a"));

    hashmap_delete(m, "a");
    ck_assert(!hashmap_exists(m, "anear"));
    ck_assert(!hashmap_exists(m, "dicot"));
    ck_assert(!hashmap_exists(m, "a"));

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

    tc = tcase_create("Set/get/delete");
    tcase_add_test(tc, test_hashmap_set);
    tcase_add_test(tc, test_hashmap_get);
    tcase_add_test(tc, test_hashmap_delete);
    suite_add_tcase(s, tc);

    tc = tcase_create("Existence");
    tcase_add_test(tc, test_hashmap_exists);
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
