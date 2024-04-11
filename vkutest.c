#include <stdlib.h>
#include <string.h>
#include "VKUtils.h"

VEC_DEF_TYPE(int);

void test_vec_int() {
    int* a = VEC_ALLOC(int, 10);

    if (VEC_CAPACITY(a, int) != 10) exit(1);

    VEC_ADD(a, 0, int);
    VEC_ADD(a, 1, int);
    VEC_ADD(a, 2, int);
    VEC_ADD(a, 3, int);

    if (VEC_SIZE(a, int) != 4) exit(1);

    for (int i = 0; i < VEC_SIZE(a, int); i++) {
        if (a[i] != i) exit(1);
    }
    VEC_FREE(a, int);
}

typedef char* pchar;
VEC_DEF_TYPE(pchar);

void test_vec_pchar() {
    pchar* a = VEC_ALLOC(pchar, 10);

    if (VEC_CAPACITY(a, int) != 10) exit(1);

    VEC_ADD(a, "0", int);
    VEC_ADD(a, "1", int);
    VEC_ADD(a, "2", int);
    VEC_ADD(a, "3", int);

    if (VEC_SIZE(a, int) != 4) exit(1);

    for (int i = 0; i < VEC_SIZE(a, int); i++) {
        char str[10];
        sprintf(str, "%d", i);
        if (strcmp(a[i], str) != 0) exit(1);
    }

    VEC_FREE(a, pchar);
}


int main(int argc, char *argv[] ) {
    if (argc < 2 || strcmp("vec_int", argv[1]) == 0 ) test_vec_int();
    if (argc < 2 || strcmp("vec_pchar", argv[1]) == 0 ) test_vec_pchar();
    return 0;
}