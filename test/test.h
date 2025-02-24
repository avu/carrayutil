#ifndef TEST_H
#define TEST_H

#include <stdlib.h>
#include <string.h>
#include "CArrayUtil.h"

typedef char* pchar;

extern int argc;
extern char **argv;
extern int test_nesting_level;

static void fail() {
    exit(1);
}

#define PRINT_INDENT(INDENT, ...) printf("%*s", (INDENT) + strlen(__VA_ARGS__), (__VA_ARGS__))
#define RUN_TEST(NAME) do { \
    if (argc < 2 || strncmp(argv[1], #NAME, CARR_MIN(strlen(#NAME), strlen(argv[1]))) == 0) { \
        PRINT_INDENT(test_nesting_level*2, "Start: " #NAME "\n");                             \
        test_nesting_level++;                                                                 \
        test_ ## NAME();                                                                      \
        test_nesting_level--;                                                                 \
        PRINT_INDENT(test_nesting_level*2, "End: " #NAME "\n");                               \
    }                                                                                         \
} while(0)

#define CONCATENATE_IMPL(A, B) A ## B
#define CONCATENATE(A, B) CONCATENATE_IMPL(A, B)

#endif //TEST_H
