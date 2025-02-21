#include <memory.h>
#include "CArrayUtil.h"

#if defined(_MSC_VER)
#   include <malloc.h>
#   define ALIGNED_ALLOC(ALIGNMENT, SIZE) _aligned_malloc((SIZE), (ALIGNMENT))
#   define ALIGNED_FREE(PTR) _aligned_free(PTR)
#else
#   include <stdlib.h>
#   define ALIGNED_ALLOC(ALIGNMENT, SIZE) aligned_alloc((ALIGNMENT), (SIZE))
#   define ALIGNED_FREE(PTR) free(PTR)
#endif

// === Allocation helpers ===

/// old_data may be NULL, new_data cannot be NULL.
typedef void (*CARR_copy_data_fp)(void* old_data, void* new_data, size_t element_size);

/**
 * Reallocate struct with the new capacity.
 * Accepts and returns pointers (nullable) to data, not the beginning of struct.
 *
 * Data can never be lost, but underlying allocation may fail,
 * so check the capacity of the returned struct when necessary.
 */
static void* CARR_realloc(void* old_data, CARR_copy_data_fp copy_data_fp,
                          size_t header_alignment, size_t header_size,
                          size_t element_alignment, size_t element_size, size_t new_capacity) {
    // assert capacity field is just before the data
    if (old_data != NULL && ((size_t*) old_data)[-1] == new_capacity) {
        return old_data;
    }

    // assert header_alignment and data_alignment are powers of 2
    size_t total_alignment = CARR_MAX(header_alignment, element_alignment);
    size_t alignment_mask = total_alignment - 1;
    // assert header_size is multiple of header_alignment
    size_t aligned_header_size = (header_size + alignment_mask) & ~alignment_mask;

    void* new_data;
    if (new_capacity == 0) {
        new_data = NULL;
    } else {
        void* block = ALIGNED_ALLOC(total_alignment, aligned_header_size + element_size * new_capacity);
        if (block == NULL) return old_data;
        new_data = (char*)block + aligned_header_size;
        ((size_t*) new_data)[-1] = new_capacity;
        copy_data_fp(old_data, new_data, element_size);
    }

    if (old_data != NULL) {
        void* old_block = (char*)old_data - aligned_header_size;
        ALIGNED_FREE(old_block);
    }

    return new_data;
}

// === Arrays ===

static void CARR_array_copy(void* old_data, void* new_data, size_t element_size) {
    if (old_data == NULL) {
        ARRAY_T(new_data)->size = 0;
    } else {
        ARRAY_T(new_data)->size = ARRAY_T(old_data)->size;
        memcpy(new_data, old_data, ARRAY_T(new_data)->size * element_size);
    }
}
void* CARR_array_realloc(void* old_data, size_t element_alignment, size_t element_size, size_t new_capacity) {
    return CARR_realloc(old_data, &CARR_array_copy, alignof(CARR_array_t), offsetof(CARR_array_t, data),
                        element_alignment, element_size, new_capacity);
}

// === Ring buffers ===

static void CARR_ring_buffer_copy(void* old_data, void* new_data, size_t element_size) {
    CARR_ring_buffer_t* new_buf = RING_BUFFER_T(new_data);
    new_buf->head = new_buf->tail = 0;
    if (old_data != NULL) {
        CARR_ring_buffer_t* old_buf = RING_BUFFER_T(old_data);
        if (old_buf->tail > old_buf->head) {
            new_buf->tail = old_buf->tail - old_buf->head;
            memcpy(new_data, old_data + old_buf->head*element_size, new_buf->tail*element_size);
        } else if (old_buf->tail < old_buf->head) {
            new_buf->tail = old_buf->capacity + old_buf->tail - old_buf->head;
            memcpy(new_data, old_data + old_buf->head*element_size, (old_buf->capacity-old_buf->head)*element_size);
            memcpy(new_data + (new_buf->tail-old_buf->tail)*element_size, old_data, old_buf->tail*element_size);
        }
    }
}
void* CARR_ring_buffer_realloc(void* old_data, size_t element_alignment, size_t element_size, size_t new_capacity) {
    return CARR_realloc(old_data, &CARR_ring_buffer_copy,
                        alignof(CARR_ring_buffer_t), offsetof(CARR_ring_buffer_t, data),
                        element_alignment, element_size, new_capacity);
}
