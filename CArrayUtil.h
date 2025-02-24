#ifndef C_ARRAY_UTIL_H
#define C_ARRAY_UTIL_H

#include <malloc.h>
#include <stdalign.h>

// C_ARRAY_UTIL_ALLOCATION_FAILED is called when allocation fails.
// Default implementation calls abort().
// Functions that can call C_ARRAY_UTIL_ALLOCATION_FAILED explicitly state
// this in the documentation. Functions with *_TRY_* return NULL on failure.
#ifndef C_ARRAY_UTIL_ALLOCATION_FAILED
#include <stdlib.h>
#define C_ARRAY_UTIL_ALLOCATION_FAILED() abort()
#endif

// === Allocation helpers ===

#define CARR_MAX(a,b) (((a)>(b))?(a):(b))

#define CARR_REALLOC(FUNC, P, CAPACITY) FUNC((P), alignof((P)[0]), sizeof((P)[0]), CAPACITY)

// === Arrays ===

#ifndef ARRAY_CAPACITY_GROW
#define ARRAY_CAPACITY_GROW(C) (((C) * 3 + 1) / 2) // 1.5 multiplier
#endif
#ifndef ARRAY_DEFAULT_CAPACITY
#define ARRAY_DEFAULT_CAPACITY 10
#endif
typedef struct {
    size_t size;
    size_t capacity;
    char data[];
} CARR_array_t;
void* CARR_array_realloc(void* old_data, size_t element_alignment, size_t element_size, size_t new_capacity);
// NULL unsafe!
#define ARRAY_T(P) ((CARR_array_t*)((char*)(P) - offsetof(CARR_array_t, data)))

/**
 * Allocate array. Returns NULL on allocation failure.
 * @param T type of elements
 * @param CAPACITY capacity of the array
 */
#define ARRAY_ALLOC(T, CAPACITY) (T*)CARR_array_realloc(NULL, alignof(T), sizeof(T), CAPACITY)

/**
 * @param P pointer to the first data element of the array
 * @return size of the array
 */
#define ARRAY_SIZE(P) ((P) == NULL ? (size_t) 0 : (ARRAY_T(P))->size)

/**
 * @param P pointer to the first data element of the array
 * @return capacity of the array
 */
#define ARRAY_CAPACITY(P) ((P) == NULL ? (size_t) 0 : (ARRAY_T(P))->capacity)

/**
 * @param P pointer to the first data element of the array
 * @return last element in the array
 */
#define ARRAY_LAST(P) ((P)[ARRAY_SIZE(P) - 1])

/**
 * Deallocate the vector
 * @param P pointer to the first data element of the array
 */
#define ARRAY_FREE(P) CARR_REALLOC(CARR_array_realloc, (P), 0)

/**
 * Apply function to the array elements
 * @param P pointer to the first data element of the array
 * @param F function to apply
 */
#define ARRAY_APPLY(P, F) do {                                   \
    for (size_t _i = 0; _i < ARRAY_SIZE(P); _i++) F(&((P)[_i])); \
} while(0)

/**
 * Apply function to the array elements, passing pointer to an element as first parameter
 * @param P pointer to the first data element of the array
 * @param F function to apply
 */
#define ARRAY_APPLY_LEADING(P, F, ...) do {                                   \
    for (size_t _i = 0; _i < ARRAY_SIZE(P); _i++) F(&((P)[_i]), __VA_ARGS__); \
} while(0)

/**
 * Apply function to the array elements, passing pointer to an element as last parameter
 * @param P pointer to the first data element of the array
 * @param F function to apply
 */
#define ARRAY_APPLY_TRAILING(P, F, ...) do {                                  \
    for (size_t _i = 0; _i < ARRAY_SIZE(P); _i++) F(__VA_ARGS__, &((P)[_i])); \
} while(0)

/**
 * Ensure array capacity. Array is implicitly initialized when necessary.
 * On allocation failure, array is unchanged.
 * @param P pointer to the first data element of the array
 * @param CAPACITY required capacity of the array
 */
#define ARRAY_TRY_ENSURE_CAPACITY(P, CAPACITY) do {                                      \
     (P) = CARR_REALLOC(CARR_array_realloc, (P), CARR_MAX(ARRAY_CAPACITY(P), CAPACITY)); \
} while(0)

/**
 * Ensure array capacity. Array is implicitly initialized when necessary.
 * On allocation failure, C_ARRAY_UTIL_ALLOCATION_FAILED is called.
 * @param P pointer to the first data element of the array
 * @param CAPACITY required capacity of the array
 */
#define ARRAY_ENSURE_CAPACITY(P, CAPACITY) do { \
    ARRAY_TRY_ENSURE_CAPACITY(P, CAPACITY);     \
    if (ARRAY_CAPACITY(P) < (CAPACITY))         \
        C_ARRAY_UTIL_ALLOCATION_FAILED();       \
} while(0)

/**
 * Shrink capacity of the array to its size.
 * On allocation failure, array is unchanged.
 * @param P pointer to the first data element of the array
 */
#define ARRAY_SHRINK_TO_FIT(P) do {                             \
    (P) = CARR_REALLOC(CARR_array_realloc, (P), ARRAY_SIZE(P)); \
} while(0)

/**
 * Resize an array. Array is implicitly initialized when necessary.
 * On allocation failure, array is unchanged.
 * @param P pointer to the first data element of the array
 * @param SIZE required size of the array
 */
#define ARRAY_TRY_RESIZE(P, SIZE) do {            \
    ARRAY_TRY_ENSURE_CAPACITY(P, SIZE);           \
    if ((P) != NULL && ARRAY_CAPACITY(P) >= SIZE) \
        ARRAY_T(P)->size = (SIZE);                \
} while(0)

/**
 * Resize an array. Array is implicitly initialized when necessary.
 * On allocation failure, C_ARRAY_UTIL_ALLOCATION_FAILED is called.
 * @param P pointer to the first data element of the array
 * @param SIZE required size of the array
 */
#define ARRAY_RESIZE(P, SIZE) do {        \
    ARRAY_TRY_RESIZE(P, SIZE);            \
    if (ARRAY_SIZE(P) != (SIZE))          \
        C_ARRAY_UTIL_ALLOCATION_FAILED(); \
} while(0)

/**
 * Add element to the end of the array. Array is implicitly initialized when necessary.
 * On allocation failure, C_ARRAY_UTIL_ALLOCATION_FAILED is called.
 * @param P pointer to the first data element of the array
 */
#define ARRAY_PUSH_BACK(P, ...) do {                                               \
    if (ARRAY_SIZE(P) >= ARRAY_CAPACITY(P)) {                                      \
         (P) = CARR_REALLOC(CARR_array_realloc, (P), (P) == NULL ?                 \
                ARRAY_DEFAULT_CAPACITY : ARRAY_CAPACITY_GROW(ARRAY_SIZE(P)));      \
         if (ARRAY_SIZE(P) >= ARRAY_CAPACITY(P)) C_ARRAY_UTIL_ALLOCATION_FAILED(); \
    }                                                                              \
    *((P) + ARRAY_SIZE(P)) = (__VA_ARGS__);                                        \
    ARRAY_T(P)->size++;                                                            \
} while(0)

#define SARRAY_COUNT_OF(STATIC_ARRAY) (sizeof(STATIC_ARRAY)/sizeof((STATIC_ARRAY)[0]))

// === Ring buffers ===

typedef struct {
    size_t head;
    size_t tail;
    size_t capacity;
    char data[];
} CARR_ring_buffer_t;
void* CARR_ring_buffer_realloc(void* old_data, size_t element_alignment, size_t element_size, size_t new_capacity);
// NULL unsafe!
#define RING_BUFFER_T(P) ((CARR_ring_buffer_t *)((char*)(P) - offsetof(CARR_ring_buffer_t, data)))

/**
 * @param P pointer to the first data element of the ring buffer
 * @return size of the ring buffer
 */
#define RING_BUFFER_SIZE(P) ((P) == NULL ? (size_t) 0 : \
    (RING_BUFFER_T(P)->capacity + RING_BUFFER_T(P)->tail - RING_BUFFER_T(P)->head) % RING_BUFFER_T(P)->capacity)

/**
 * @param P pointer to the first data element of the ring buffer
 * @return capacity of the ring buffer
 */
#define RING_BUFFER_CAPACITY(P) ((P) == NULL ? (size_t) 0 : RING_BUFFER_T(P)->capacity)

/**
 * Ensure enough capacity to push an element into ring buffer. Implicitly initializes when buffer is NULL.
 * On allocation failure, C_ARRAY_UTIL_ALLOCATION_FAILED is called.
 * @param P pointer to the first data element of the buffer
 */
#define RING_BUFFER_ENSURE_CAN_PUSH(P) do {                                            \
    if (RING_BUFFER_SIZE(P) + 1 >= RING_BUFFER_CAPACITY(P))                            \
        (P) = CARR_REALLOC(CARR_ring_buffer_realloc, (P), (P) == NULL ?                \
            ARRAY_DEFAULT_CAPACITY : ARRAY_CAPACITY_GROW(RING_BUFFER_T(P)->capacity)); \
    if ((P) == NULL) C_ARRAY_UTIL_ALLOCATION_FAILED();                                 \
} while(0)

/**
 * Add element to the beginning of the ring buffer. Implicitly initializes when buffer is NULL.
 * On allocation failure, C_ARRAY_UTIL_ALLOCATION_FAILED is called.
 * @param P pointer to the first data element of the buffer
 */
#define RING_BUFFER_PUSH_FRONT(P, ...) do {                                                                          \
    RING_BUFFER_ENSURE_CAN_PUSH(P);                                                                                  \
    RING_BUFFER_T(P)->head = (RING_BUFFER_T(P)->head + RING_BUFFER_T(P)->capacity - 1) % RING_BUFFER_T(P)->capacity; \
    (P)[RING_BUFFER_T(P)->head] = (__VA_ARGS__);                                                                     \
} while(0)

/**
 * Add element to the end of the ring buffer. Implicitly initializes when buffer is NULL.
 * On allocation failure, C_ARRAY_UTIL_ALLOCATION_FAILED is called.
 * @param P pointer to the first data element of the buffer
 */
#define RING_BUFFER_PUSH_BACK(P, ...) do {                                              \
    RING_BUFFER_ENSURE_CAN_PUSH(P);                                                     \
    (P)[RING_BUFFER_T(P)->tail] = (__VA_ARGS__);                                        \
    RING_BUFFER_T(P)->tail = (RING_BUFFER_T(P)->tail + 1) % RING_BUFFER_T(P)->capacity; \
} while(0)

/**
 * Get pointer to the first element of the ring buffer.
 * @param P pointer to the first data element of the buffer
 */
#define RING_BUFFER_FRONT(P) ((P) == NULL || RING_BUFFER_T(P)->head == RING_BUFFER_T(P)->tail ? NULL : &(P)[RING_BUFFER_T(P)->head])

/**
 * Get pointer to the last element of the ring buffer.
 * @param P pointer to the first data element of the buffer
 */
#define RING_BUFFER_BACK(P) ((P) == NULL || RING_BUFFER_T(P)->head == RING_BUFFER_T(P)->tail ? NULL : \
    &(P)[(RING_BUFFER_T(P)->tail + RING_BUFFER_T(P)->capacity - 1) % RING_BUFFER_T(P)->capacity])

/**
 * Move beginning of the ring buffer forward (remove first element).
 * @param P pointer to the first data element of the buffer
 */
#define RING_BUFFER_POP_FRONT(P) RING_BUFFER_T(P)->head = (RING_BUFFER_T(P)->head + 1) % RING_BUFFER_T(P)->capacity

/**
 * Move end of the ring buffer backward (remove last element).
 * @param P pointer to the first data element of the buffer
 */
#define RING_BUFFER_POP_BACK(P) \
    RING_BUFFER_T(P)->tail = (RING_BUFFER_T(P)->tail + RING_BUFFER_T(P)->capacity - 1) % RING_BUFFER_T(P)->capacity

/**
 * Deallocate the ring buffer
 * @param P pointer to the first data element of the buffer
 */
#define RING_BUFFER_FREE(P) CARR_REALLOC(CARR_ring_buffer_realloc, (P), 0)

#endif // C_ARRAY_UTIL_H
