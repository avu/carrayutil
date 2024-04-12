#include <stdlib.h>
#include <string.h>
#include "VKUtils.h"

void test_vec_int() {
    int* a = VEC_ALLOC(int, 10);

    if (VEC_CAPACITY(a) != 10) exit(1);

    VEC_ADD(a, 0);
    VEC_ADD(a, 1);
    VEC_ADD(a, 2);
    VEC_ADD(a, 3);

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

    VEC_ADD(a, "0");
    VEC_ADD(a, "1");
    VEC_ADD(a, "2");
    VEC_ADD(a, "3");

    if (VEC_SIZE(a) != 4) exit(1);

    for (int i = 0; i < VEC_SIZE(a); i++) {
        char str[10];
        sprintf(str, "%d", i);
        if (strcmp(a[i], str) != 0) exit(1);
    }

    VEC_FREE(a);
}

void test_vec_srink_to_fit() {
    pchar* a = VEC_ALLOC(pchar, 10);

    if (VEC_CAPACITY(a) != 10) exit(1);

    VEC_ADD(a, "0");
    VEC_ADD(a, "1");
    VEC_ADD(a, "2");
    VEC_ADD(a, "3");


    if (VEC_SIZE(a) != 4) exit(1);

    VEC_SHRINK_TO_FIT(a);

    if (VEC_CAPACITY(a) != 4) exit(1);
    if (VEC_SIZE(a) != 4) exit(1);

    for (int i = 0; i < VEC_SIZE(a); i++) {
        char str[10];
        sprintf(str, "%d", i);
        if (strcmp(a[i], str) != 0) exit(1);
    }

    VEC_FREE(a);
}

int main(int argc, char *argv[] ) {
    if (argc < 2 || strcmp("vec_int", argv[1]) == 0 ) test_vec_int();
    if (argc < 2 || strcmp("vec_pchar", argv[1]) == 0 ) test_vec_pchar();
    if (argc < 2 || strcmp("vec_srink_to_fit", argv[1]) == 0 ) test_vec_pchar();
    return 0;
}