#ifndef C_ARRAY_UTIL_H
#define C_ARRAY_UTIL_H

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#define ARRAY_CAPACITY_MULT 2
typedef struct {
    size_t elem_size;
    size_t size;
    size_t capacity;
    char data[];
} CARR_array_t;

void* CARR_array_alloc(size_t elem_size, size_t capacity);
void* CARR_array_realloc(CARR_array_t* vec, size_t new_capacity);

/**
 * Allocate vector
 * @param T type of elements
 * @param SIZE size of the vector
 */
#define ARRAY_ALLOC(T, SIZE) (T*)CARR_array_alloc(sizeof(T), SIZE)
#define ARRAY_T(P) (CARR_array_t *)((char*)P - offsetof(CARR_array_t, data))

/**
 * @param P pointer to the first data element of the vector
 * @return size of the vector
 */
#define ARRAY_SIZE(P) (ARRAY_T(P))->size

/**
 * @param P pointer to the first data element of the vector
 * @return capacity of the vector
 */
#define ARRAY_CAPACITY(P) (ARRAY_T(P))->capacity

/**
 * Deallocate the vector
 * @param P pointer to the first data element of the vector
 */
#define ARRAY_FREE(P) free(ARRAY_T(P))

/**
 * Shrink capacity of the vector to its size
 * @param PP pointer to the pointer to the first data element of the vector
 */
#define ARRAY_SHRINK_TO_FIT(PP) do {                    \
    *PP = CARR_array_realloc(ARRAY_T(*PP), ARRAY_SIZE(*PP)); \
} while(0)

/**
 * Add element to the end of the vector
 * @param PP pointer to the pointer to the first data element of the vector
 */
#define ARRAY_PUSH_BACK(PP, D) do {                                           \
    if (ARRAY_SIZE(*PP) >= ARRAY_CAPACITY(*PP)) {                               \
         *PP = CARR_array_realloc(ARRAY_T(*PP), ARRAY_SIZE(*PP)*ARRAY_CAPACITY_MULT);\
    }                                                                       \
    *(*PP + ARRAY_SIZE(*PP)) = (D);                                           \
    ARRAY_SIZE(*PP)++;                                                        \
} while(0)

#endif // CARRAYUTILS_H
