#include "test.h"

#define TYPE uint32_t
#include "test_ring_buffer.h"
#define TYPE uint64_t
#include "test_ring_buffer.h"

static void test_ring_buffer_null_safe() {
    RING_BUFFER(pchar) b = NULL;

    if (RING_BUFFER_SIZE(b) != 0) fail();
    if (RING_BUFFER_CAPACITY(b) != 0) fail();
    RING_BUFFER_FREE(b);

    RING_BUFFER_PUSH_BACK(b) = "test";
    if (RING_BUFFER_SIZE(b) != 1) fail();
    if (RING_BUFFER_CAPACITY(b) < 1) fail();

    RING_BUFFER_FREE(b);
}

static void test_ring_buffer_struct() {
    typedef struct {
        size_t data[123];
    } struct_t;
    RING_BUFFER(struct_t) b = NULL;

    for (size_t i = 0; i < 1000; i++) {
        RING_BUFFER_PUSH_BACK(b) = (struct_t){i};
    }
    if (RING_BUFFER_SIZE(b) != 1000) fail();
    for (size_t i = 0;; i++) {
        struct_t* s = RING_BUFFER_FRONT(b);
        if (s == NULL) {
            if (i != 1000) fail();
            else break;
        }
        if (s->data[0] != i) fail();
        RING_BUFFER_POP_FRONT(b);
    }

    RING_BUFFER_FREE(b);
}

static void test_ring_buffer_struct_align() {
    typedef struct {
        alignas(256) size_t data[123];
    } struct_t;
    RING_BUFFER(struct_t) b = NULL;

    for (size_t i = 0; i < 1000; i++) {
        RING_BUFFER_PUSH_BACK(b) = (struct_t){i};
        if ((uintptr_t)RING_BUFFER_BACK(b) % 256 != 0) fail();
    }
    if (RING_BUFFER_SIZE(b) != 1000) fail();
    for (size_t i = 0;; i++) {
        struct_t* s = RING_BUFFER_FRONT(b);
        if (s == NULL) {
            if (i != 1000) fail();
            else break;
        }
        if (s->data[0] != i) fail();
        if ((uintptr_t)s % 256 != 0) fail();
        RING_BUFFER_POP_FRONT(b);
    }

    RING_BUFFER_FREE(b);
}

void test_ring_buffer() {
    RUN_TEST(ring_buffer_wrap_uint32_t);
    RUN_TEST(ring_buffer_wrap_uint64_t);
    RUN_TEST(ring_buffer_null_safe);
    RUN_TEST(ring_buffer_struct);
    RUN_TEST(ring_buffer_struct_align);
}
