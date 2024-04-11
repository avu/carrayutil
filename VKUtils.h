#ifndef VKUTILS_H
#define VKUTILS_H

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

void* VKUtils_vec_alloc(size_t size_el, size_t capacity,
                        ptrdiff_t off_size,
                        ptrdiff_t off_capacity,
                        ptrdiff_t off_data);

#define VEC_DEF_TYPE(T) typedef struct { size_t size; size_t capacity; T* data;} vec_ ## T

#define VEC_ALLOC(T, SIZE) (T*)VKUtils_vec_alloc(sizeof(T), SIZE,              \
                                                 offsetof(vec_ ## T, size),    \
                                                 offsetof(vec_ ## T, capacity),\
                                                 offsetof(vec_ ## T, data))

#define VEC_SIZE(P, T) ((vec_ ## T*)((char*)P - offsetof(vec_ ## T, data)))->size
#define VEC_CAPACITY(P, T) ((vec_ ## T*)((char*)P - offsetof(vec_ ## T, data)))->capacity
#define VEC_FREE(P, T)  free(((vec_ ## T*)((char*)P - offsetof(vec_ ## T, data))))
#define VEC_ADD(P, D, T) do {                    \
    assert(VEC_SIZE(P, T) < VEC_CAPACITY(P, T)); \
    *(P + VEC_SIZE(P, T)) = (D);                 \
    VEC_SIZE(P, T)++;                            \
} while(0)

#endif // VKUTILS_H
