#include "VKUtils.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemoryLeak"
void* VKUtils_vec_alloc(size_t size_el, size_t capacity,
                        ptrdiff_t off_size,
                        ptrdiff_t off_capacity,
                        ptrdiff_t off_data)
{
    char* vec_mem = malloc(size_el * capacity + off_data);
    if (vec_mem == NULL) {
        return NULL;
    }
    *((size_t*)((char*)vec_mem + off_size)) = 0;
    *((size_t*)((char*)vec_mem + off_capacity)) = capacity;
    return vec_mem + off_data;
}
#pragma clang diagnostic pop
