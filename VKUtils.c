#include <memory.h>
#include "VKUtils.h"

#define MIN(a,b) (((a)<(b))?(a):(b))

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemoryLeak"
void* VKU_vec_alloc(size_t elem_size, size_t capacity) {
    size_t data_offset = offsetof(VKU_vec_t, data);
    char* vec_mem = malloc(elem_size * capacity + data_offset);
    if (vec_mem == NULL) {
        return NULL;
    }
    *((size_t*)((char*)vec_mem + offsetof(VKU_vec_t, elem_size))) = elem_size;
    *((size_t*)((char*)vec_mem + offsetof(VKU_vec_t, size))) = 0;
    *((size_t*)((char*)vec_mem + offsetof(VKU_vec_t, capacity))) = capacity;
    return vec_mem + data_offset;
}

void* VKU_vec_realloc(VKU_vec_t* vec, size_t new_capacity) {
    if (vec->capacity == new_capacity) {
        return vec->data;
    }
    VKU_vec_t* new_vec = (VKU_vec_t*)(VKU_vec_alloc(vec->elem_size, new_capacity) - offsetof(VKU_vec_t, data));
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
