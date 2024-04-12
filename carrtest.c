#include <stdlib.h>
#include <string.h>
#include "CArrayUtil.h"

#define RUN_TEST(NAME) do { \
    if (argc < 2 || strcmp( #NAME, argv[1]) == 0 ) { \
        printf("Start: " #NAME "\n");                \
        test_ ## NAME();                             \
        printf("End: " #NAME "\n");                  \
    }                                                \
} while(0)

void test_array_int() {
    int* a = ARRAY_ALLOC(int, 10);

    if (ARRAY_CAPACITY(a) != 10) exit(1);

    ARRAY_PUSH_BACK(&a, 0);
    ARRAY_PUSH_BACK(&a, 1);
    ARRAY_PUSH_BACK(&a, 2);
    ARRAY_PUSH_BACK(&a, 3);

    if (ARRAY_SIZE(a) != 4) exit(1);

    for (int i = 0; i < ARRAY_SIZE(a); i++) {
        if (a[i] != i) exit(1);
    }
    ARRAY_FREE(a);
}

typedef char* pchar;

void test_array_pchar() {
    pchar* a = ARRAY_ALLOC(pchar, 10);

    if (ARRAY_CAPACITY(a) != 10) exit(1);

    ARRAY_PUSH_BACK(&a, "0");
    ARRAY_PUSH_BACK(&a, "1");
    ARRAY_PUSH_BACK(&a, "2");
    ARRAY_PUSH_BACK(&a, "3");

    if (ARRAY_SIZE(a) != 4) exit(1);

    for (int i = 0; i < ARRAY_SIZE(a); i++) {
        char str[10];
        sprintf(str, "%d", i);
        if (strcmp(a[i], str) != 0) exit(1);
    }

    ARRAY_FREE(a);
}

void test_array_shrink_to_fit() {
    pchar* a = ARRAY_ALLOC(pchar, 10);
    pchar* aa = a;

    if (ARRAY_CAPACITY(a) != 10) exit(1);

    ARRAY_PUSH_BACK(&a, "0");
    ARRAY_PUSH_BACK(&a, "1");
    ARRAY_PUSH_BACK(&a, "2");
    ARRAY_PUSH_BACK(&a, "3");


    if (ARRAY_SIZE(a) != 4) exit(1);

    ARRAY_SHRINK_TO_FIT(&a);

    if (a == aa) exit(1);

    if (ARRAY_CAPACITY(a) != 4) exit(1);
    if (ARRAY_SIZE(a) != 4) exit(1);

    for (int i = 0; i < ARRAY_SIZE(a); i++) {
        char str[10];
        sprintf(str, "%d", i);
        if (strcmp(a[i], str) != 0) exit(1);
    }

    ARRAY_FREE(a);
}

void test_array_expand() {
    pchar* a = ARRAY_ALLOC(pchar, 3);

    ARRAY_PUSH_BACK(&a, "0");
    ARRAY_PUSH_BACK(&a, "1");
    ARRAY_PUSH_BACK(&a, "2");
    ARRAY_PUSH_BACK(&a, "3");

    if (ARRAY_SIZE(a) != 4) exit(1);
    if (ARRAY_CAPACITY(a) <= 3) exit(1);

    for (int i = 0; i < ARRAY_SIZE(a); i++) {
        char str[10];
        sprintf(str, "%d", i);
        if (strcmp(a[i], str) != 0) exit(1);
    }

    ARRAY_FREE(a);
}

int main(int argc, char *argv[] ) {
    RUN_TEST(array_int);
    RUN_TEST(array_pchar);
    RUN_TEST(array_shrink_to_fit);
    RUN_TEST(array_expand);
    return 0;
}