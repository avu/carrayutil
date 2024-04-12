#ifndef VKUTILS_H
#define VKUTILS_H

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

typedef struct {
    size_t elem_size;
    size_t size;
    size_t capacity;
    void* data;
} VKU_vec_t;

void* VKU_vec_alloc(size_t elem_size, size_t capacity);
void* VKU_vec_realloc(VKU_vec_t* vec, size_t new_capacity);


#define VEC_ALLOC(T, SIZE) (T*)VKU_vec_alloc(sizeof(T), SIZE)
#define VEC_T(P) (VKU_vec_t*)((char*)P - offsetof(VKU_vec_t, data))
#define VEC_SIZE(P) (VEC_T(P))->size
#define VEC_CAPACITY(P) (VEC_T(P))->capacity
#define VEC_FREE(P)  free(VEC_T(P))
#define VEC_SHRINK_TO_FIT(P) VKU_vec_realloc(VEC_T(P), VEC_SIZE(P))

#define VEC_ADD(P, D) do {                       \
    assert(VEC_SIZE(P) < VEC_CAPACITY(P));       \
    *(P + VEC_SIZE(P)) = (D);                    \
    VEC_SIZE(P)++;                               \
} while(0)

#endif // VKUTILS_H
