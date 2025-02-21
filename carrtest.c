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

    ARRAY_PUSH_BACK(a, 0);
    ARRAY_PUSH_BACK(a, 1);
    ARRAY_PUSH_BACK(a, 2);
    ARRAY_PUSH_BACK(a, 3);

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

    ARRAY_PUSH_BACK(a, "0");
    ARRAY_PUSH_BACK(a, "1");
    ARRAY_PUSH_BACK(a, "2");
    ARRAY_PUSH_BACK(a, "3");

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

    ARRAY_PUSH_BACK(a, "0");
    ARRAY_PUSH_BACK(a, "1");
    ARRAY_PUSH_BACK(a, "2");
    ARRAY_PUSH_BACK(a, "3");


    if (ARRAY_SIZE(a) != 4) exit(1);

    ARRAY_SHRINK_TO_FIT(a);

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

    ARRAY_PUSH_BACK(a, "0");
    ARRAY_PUSH_BACK(a, "1");
    ARRAY_PUSH_BACK(a, "2");
    ARRAY_PUSH_BACK(a, "3");

    if (ARRAY_SIZE(a) != 4) exit(1);
    if (ARRAY_CAPACITY(a) <= 3) exit(1);

    for (int i = 0; i < ARRAY_SIZE(a); i++) {
        char str[10];
        sprintf(str, "%d", i);
        if (strcmp(a[i], str) != 0) exit(1);
    }

    ARRAY_FREE(a);
}

#define TEST_USER_DATA1 123
#define TEST_USER_DATA2 321
static void pchar_apply_callback(pchar* c) {
    if (c == NULL) exit(1);
}
static void pchar_apply_leading_callback(pchar* c, int user_data1, int user_data2) {
    if (c == NULL) exit(1);
    if (user_data1 != TEST_USER_DATA1) exit(1);
    if (user_data2 != TEST_USER_DATA2) exit(1);
}
static void pchar_apply_trailing_callback(int user_data1, int user_data2, pchar* c) {
    if (c == NULL) exit(1);
    if (user_data1 != TEST_USER_DATA1) exit(1);
    if (user_data2 != TEST_USER_DATA2) exit(1);
}

void test_array_null_safe() {
    pchar* a = NULL;

    if (ARRAY_SIZE(a) != 0) exit(1);
    if (ARRAY_CAPACITY(a) != 0) exit(1);
    ARRAY_FREE(a);

    ARRAY_APPLY(a, pchar_apply_callback);
    ARRAY_APPLY_LEADING(a, pchar_apply_leading_callback, TEST_USER_DATA1, TEST_USER_DATA2);
    ARRAY_APPLY_TRAILING(a, pchar_apply_trailing_callback, TEST_USER_DATA1, TEST_USER_DATA2);

    ARRAY_PUSH_BACK(a, "test");
    if (ARRAY_SIZE(a) != 1) exit(1);
    if (ARRAY_CAPACITY(a) < 1) exit(1);
    if (strcmp(a[0], "test") != 0) exit(1);

    ARRAY_FREE(a);
}

void test_array_ensure_capacity() {
    pchar* a = NULL;

    ARRAY_ENSURE_CAPACITY(a, 1);
    if (ARRAY_CAPACITY(a) < 1) exit(1);

    size_t expanded_capacity = ARRAY_CAPACITY(a) + 1;
    ARRAY_ENSURE_CAPACITY(a, expanded_capacity);
    if (ARRAY_CAPACITY(a) < expanded_capacity) exit(1);

    ARRAY_FREE(a);
}

void test_array_resize() {
    pchar* a = NULL;

    ARRAY_RESIZE(a, 10);
    if (ARRAY_SIZE(a) != 10) exit(1);
    if (ARRAY_CAPACITY(a) < 10) exit(1);

    ARRAY_RESIZE(a, 20);
    if (ARRAY_SIZE(a) != 20) exit(1);
    if (ARRAY_CAPACITY(a) < 20) exit(1);

    ARRAY_FREE(a);
}

void test_ring_buffer_null_safe() {
    pchar* b = NULL;

    if (RING_BUFFER_SIZE(b) != 0) exit(1);
    if (RING_BUFFER_CAPACITY(b) != 0) exit(1);
    RING_BUFFER_FREE(b);

    RING_BUFFER_PUSH_BACK(b, "test");
    if (RING_BUFFER_SIZE(b) != 1) exit(1);
    if (RING_BUFFER_CAPACITY(b) < 1) exit(1);

    RING_BUFFER_FREE(b);
}

void test_ring_buffer_wrap() {
    const int EXPAND_COUNT = 1000;
    const int INNER_COUNT  = 1000;
    int* b = NULL;

    int read = 0;
    int write = 0;
    for (int i = 0; i < EXPAND_COUNT; i++) {
        for (int j = 0; j < INNER_COUNT; j++) {
            RING_BUFFER_PUSH_BACK(b, write);
            write++;
            int* value = RING_BUFFER_FRONT(b);
            if (value == NULL) exit(1);
            if (*value != read) exit(1);
            read++;
            RING_BUFFER_POP_FRONT(b);
        }
        RING_BUFFER_PUSH_BACK(b, write);
        write++;
    }
    if (RING_BUFFER_SIZE(b) != EXPAND_COUNT) exit(1);

    for (int i = 0; i < EXPAND_COUNT; i++) {
        int* value = RING_BUFFER_FRONT(b);
        if (value == NULL) exit(1);
        if (*value != read) exit(1);
        read++;
        RING_BUFFER_POP_FRONT(b);
    }
    if (RING_BUFFER_FRONT(b) != NULL) exit(1);
    if (RING_BUFFER_SIZE(b) != 0) exit(1);

    RING_BUFFER_FREE(b);
}

int main(int argc, char *argv[] ) {
    RUN_TEST(array_int);
    RUN_TEST(array_pchar);
    RUN_TEST(array_shrink_to_fit);
    RUN_TEST(array_expand);
    RUN_TEST(array_null_safe);
    RUN_TEST(array_ensure_capacity);
    RUN_TEST(array_resize);
    RUN_TEST(ring_buffer_null_safe);
    RUN_TEST(ring_buffer_wrap);
    return 0;
}