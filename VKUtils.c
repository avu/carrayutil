#include <memory.h>
#include "VKUtils.h"

#define MIN(a,b) (((a)<(b))?(a):(b))

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemoryLeak"
void* VKU_vec_alloc(size_t elem_size, size_t capacity) {
    VKU_vec_t *pvec = malloc(elem_size * capacity + offsetof(VKU_vec_t, data));
    if (pvec == NULL) {
        return NULL;
    }
    pvec->elem_size = elem_size;
    pvec->size = 0;
    pvec->capacity = capacity;
    return pvec->data;
}

void* VKU_vec_realloc(VKU_vec_t* vec, size_t new_capacity) {
    if (vec->capacity == new_capacity) {
        return vec->data;
    }
    VKU_vec_t* new_vec = (VKU_vec_t*)(VKU_vec_alloc(vec->elem_size, new_capacity) - offsetof(VKU_vec_t, data));
    if (new_vec == NULL) {
        return NULL;
    }
    new_vec->capacity = new_capacity;
    new_vec->size = MIN(vec->size, new_capacity);
    new_vec->elem_size = vec->elem_size;
    memcpy(new_vec->data, vec->data, new_vec->size*new_vec->elem_size);
    free(vec);
    return new_vec->data;
}

void* VKU_vec_shrink_to_fit(VKU_vec_t* vec) {
    return VKU_vec_realloc(vec, vec->size);
}

#pragma clang diagnostic pop
