#include <stdbool.h>
#include <stdint.h>

static bool alloc_failed = false;
#define C_ARRAY_UTIL_ALLOCATION_FAILED() alloc_failed = true

#include "test.h"

typedef MAP(int, int) map_t;
static void safe_map_rehash(map_t* map) {
    bool result = HASH_MAP_TRY_REHASH(*map, linear_probing, NULL, NULL, 0, -1, 1.0);
    if (!result) fail();
}
static void safe_map_free(map_t* map) {
    MAP_FREE(*map);
    *map = NULL;
}

// Providing invalid alignment helps to make allocation fail.
#ifdef alignof
#undef alignof
#endif
#define alignof(...) SIZE_MAX

static void expect_alloc_fail(bool expected) {
    if (alloc_failed != expected) fail();
    alloc_failed = false;
}

static void test_alloc_fail_array() {
    ARRAY(int) a = NULL;
    bool try_result;

    try_result = ARRAY_TRY_ENSURE_CAPACITY(a, 1);
    if (try_result) fail();
    expect_alloc_fail(false);
    try_result = ARRAY_TRY_RESIZE(a, 1);
    if (try_result) fail();
    expect_alloc_fail(false);

    a = ARRAY_ALLOC(int, 1);
    expect_alloc_fail(false);
    ARRAY_ENSURE_CAPACITY(a, 1);
    expect_alloc_fail(true);
    ARRAY_SHRINK_TO_FIT(a);
    expect_alloc_fail(false);
    ARRAY_RESIZE(a, 1);
    expect_alloc_fail(true);
    ARRAY_PUSH_BACK(a); // Do not write into "pushed" value, it should be invalid.
    expect_alloc_fail(true);
}

static void test_alloc_fail_ring_buffer() {
    RING_BUFFER(int) b = NULL;
    bool try_result;

    try_result = RING_BUFFER_TRY_ENSURE_CAN_PUSH(b);
    if (try_result) fail();
    expect_alloc_fail(false);

    RING_BUFFER_ENSURE_CAN_PUSH(b);
    expect_alloc_fail(true);
    RING_BUFFER_PUSH_FRONT(b); // Do not write into "pushed" value, it should be invalid.
    expect_alloc_fail(true);
    RING_BUFFER_PUSH_BACK(b); // Do not write into "pushed" value, it should be invalid.
    expect_alloc_fail(true);
    RING_BUFFER_FREE(b);
    expect_alloc_fail(false);
}

static void test_alloc_fail_map() {
    map_t map = NULL;
    bool try_result;

    try_result = HASH_MAP_TRY_REHASH(map, linear_probing, NULL, NULL, 0, -1, 1.0);
    if (try_result) fail();
    expect_alloc_fail(false);
    HASH_MAP_REHASH(map, linear_probing, NULL, NULL, 0, -1, 1.0);
    expect_alloc_fail(true);

    safe_map_rehash(&map);
    try_result = MAP_TRY_ENSURE_EXTRA_CAPACITY(map, 1000);
    if (try_result) fail();
    expect_alloc_fail(false);
    MAP_ENSURE_EXTRA_CAPACITY(map, 1000);
    expect_alloc_fail(true);
    safe_map_free(&map);
}

void test_alloc_fail() {
    alloc_failed = false;
    RUN_TEST(alloc_fail_array);
    RUN_TEST(alloc_fail_ring_buffer);
    RUN_TEST(alloc_fail_map);
}
