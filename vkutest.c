#include <stdlib.h>
#include <string.h>
#include "VKUtils.h"

#define RUN_TEST(NAME) do { \
    if (argc < 2 || strcmp( #NAME, argv[1]) == 0 ) { \
        printf("Start: " #NAME "\n");                \
        test_ ## NAME();                             \
        printf("End: " #NAME "\n");                  \
    }                                                \
} while(0)

void test_vec_int() {
    int* a = VEC_ALLOC(int, 10);

    if (VEC_CAPACITY(a) != 10) exit(1);

    VEC_PUSH_BACK(&a, 0);
    VEC_PUSH_BACK(&a, 1);
    VEC_PUSH_BACK(&a, 2);
    VEC_PUSH_BACK(&a, 3);

    if (VEC_SIZE(a) != 4) exit(1);

    for (int i = 0; i < VEC_SIZE(a); i++) {
        if (a[i] != i) exit(1);
    }
    VEC_FREE(a);
}

typedef char* pchar;

void test_vec_pchar() {
    pchar* a = VEC_ALLOC(pchar, 10);

    if (VEC_CAPACITY(a) != 10) exit(1);

    VEC_PUSH_BACK(&a, "0");
    VEC_PUSH_BACK(&a, "1");
    VEC_PUSH_BACK(&a, "2");
    VEC_PUSH_BACK(&a, "3");

    if (VEC_SIZE(a) != 4) exit(1);

    for (int i = 0; i < VEC_SIZE(a); i++) {
        char str[10];
        sprintf(str, "%d", i);
        if (strcmp(a[i], str) != 0) exit(1);
    }

    VEC_FREE(a);
}

void test_vec_shrink_to_fit() {
    pchar* a = VEC_ALLOC(pchar, 10);
    pchar* aa = a;

    if (VEC_CAPACITY(a) != 10) exit(1);

    VEC_PUSH_BACK(&a, "0");
    VEC_PUSH_BACK(&a, "1");
    VEC_PUSH_BACK(&a, "2");
    VEC_PUSH_BACK(&a, "3");


    if (VEC_SIZE(a) != 4) exit(1);

    VEC_SHRINK_TO_FIT(&a);

    if (a == aa) exit(1);

    if (VEC_CAPACITY(a) != 4) exit(1);
    if (VEC_SIZE(a) != 4) exit(1);

    for (int i = 0; i < VEC_SIZE(a); i++) {
        char str[10];
        sprintf(str, "%d", i);
        if (strcmp(a[i], str) != 0) exit(1);
    }

    VEC_FREE(a);
}

void test_vec_expand() {
    pchar* a = VEC_ALLOC(pchar, 3);

    VEC_PUSH_BACK(&a, "0");
    VEC_PUSH_BACK(&a, "1");
    VEC_PUSH_BACK(&a, "2");
    VEC_PUSH_BACK(&a, "3");

    if (VEC_SIZE(a) != 4) exit(1);
    if (VEC_CAPACITY(a) <= 3) exit(1);

    for (int i = 0; i < VEC_SIZE(a); i++) {
        char str[10];
        sprintf(str, "%d", i);
        if (strcmp(a[i], str) != 0) exit(1);
    }

    VEC_FREE(a);
}

int main(int argc, char *argv[] ) {
    RUN_TEST(vec_int);
    RUN_TEST(vec_pchar);
    RUN_TEST(vec_shrink_to_fit);
    RUN_TEST(vec_expand);
    return 0;
}