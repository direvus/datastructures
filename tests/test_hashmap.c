#include <check.h>
#include <stdlib.h>
#include <stdio.h>
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

START_TEST(test_hashmap_copy) {
    /* Doesn't crash */
    hashmap_copy(0, 0);

    struct hashmap *src = hashmap_create();
    ck_assert_ptr_nonnull(src);
    struct hashmap *dst = hashmap_create();

    int *v;
    v = malloc(sizeof *v);
    *v = 0;
    hashmap_set(src, "in-source", v);

    v = malloc(sizeof *v);
    *v = 692844;
    hashmap_set(dst, "in-source", v);
    hashmap_set(dst, "not-in-source", v);

    char s[] = "a";
    for (int i = 1; i <= 26; i++) {
        v = malloc(sizeof *v);
        *v = i;
        hashmap_set(src, s, v);
        s[0]++;
    }

    hashmap_copy(dst, src);
    ck_assert_int_eq(dst->count, 28);

    s[0] = 'a';
    for (int i = 1; i <= 26; i++) {
        ck_assert_int_eq(*((int *) hashmap_get(dst, s)), i);
        s[0]++;
    }

    /* Keys in dst but not src are preserved. */
    ck_assert_int_eq(*((int *) hashmap_get(dst, "not-in-source")), 692844);

    /* Keys in both dst and src are overwritten. */
    ck_assert_int_eq(*((int *) hashmap_get(dst, "in-source")), 0);

    hashmap_destroy(src);
    hashmap_destroy(dst);
}
END_TEST

START_TEST(test_hashmap_resize) {
    struct hashmap *m = hashmap_create();
    unsigned int size = m->size;
    ck_assert_ptr_nonnull(m);

    const size_t KEYSIZE = 16;
    char k[KEYSIZE];
    int *v;
    for (unsigned int i = 0; i <= 128; i++) {
        v = malloc(sizeof *v);
        *v = i;
        sprintf(k, "%u", i);
        hashmap_set(m, k, v);
    }

    /* With that many keys, it really should have expanded at least once ... */
    ck_assert_int_gt(m->size, size);
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

    tc = tcase_create("Copy");
    tcase_add_test(tc, test_hashmap_copy);
    suite_add_tcase(s, tc);

    tc = tcase_create("Resize");
    tcase_add_test(tc, test_hashmap_resize);
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
