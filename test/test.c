#include "test.h"
int argc;
char **argv;
int test_nesting_level;

void test_array();
void test_ring_buffer();
void test_map();
void test_alloc_fail();

int main(int my_argc, char *my_argv[]) {
    argc = my_argc;
    argv = my_argv;
    test_nesting_level = 0;
    RUN_TEST(array);
    RUN_TEST(ring_buffer);
    RUN_TEST(map);
    RUN_TEST(alloc_fail);
    return 0;
}
