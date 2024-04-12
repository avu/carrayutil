#ifndef VKUTILS_H
#define VKUTILS_H

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#define VEC_CAPACITY_MULT 2
typedef struct {
    size_t elem_size;
    size_t size;
    size_t capacity;
    char data[];
} VKU_vec_t;

void* VKU_vec_alloc(size_t elem_size, size_t capacity);
void* VKU_vec_realloc(VKU_vec_t* vec, size_t new_capacity);

/**
 * Allocate vector
 * @param T type of elements
 * @param SIZE size of the vector
 */
#define VEC_ALLOC(T, SIZE) (T*)VKU_vec_alloc(sizeof(T), SIZE)
#define VEC_T(P) (VKU_vec_t*)((char*)P - offsetof(VKU_vec_t, data))

/**
 * @param P pointer to the first data element of the vector
 * @return size of the vector
 */
#define VEC_SIZE(P) (VEC_T(P))->size

/**
 * @param P pointer to the first data element of the vector
 * @return capacity of the vector
 */
#define VEC_CAPACITY(P) (VEC_T(P))->capacity

/**
 * Deallocate the vector
 * @param P pointer to the first data element of the vector
 */
#define VEC_FREE(P) free(VEC_T(P))

/**
 * Shrink capacity of the vector to its size
 * @param PP pointer to the pointer to the first data element of the vector
 */
#define VEC_SHRINK_TO_FIT(PP) do {                    \
    *PP = VKU_vec_realloc(VEC_T(*PP), VEC_SIZE(*PP)); \
} while(0)

/**
 * Add element to the end of the vector
 * @param PP pointer to the pointer to the first data element of the vector
 */
#define VEC_PUSH_BACK(PP, D) do {                                           \
    if (VEC_SIZE(*PP) >= VEC_CAPACITY(*PP)) {                               \
         *PP = VKU_vec_realloc(VEC_T(*PP), VEC_SIZE(*PP)*VEC_CAPACITY_MULT);\
    }                                                                       \
    *(*PP + VEC_SIZE(*PP)) = (D);                                           \
    VEC_SIZE(*PP)++;                                                        \
} while(0)

#endif // VKUTILS_H
