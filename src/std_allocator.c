#include <stdlib.h>

#include "nclib/std_allocator.h"

StdAllocator std_allocator_new()
{
    return (StdAllocator){
        .methods = {
            ._alloc_impl = (AllocatorI_alloc_type)std_allocator_alloc,
            ._resize_impl = (AllocatorI_resize_type)std_allocator_resize,
            ._free_all_impl = (AllocatorI_free_all_type)std_allocator_free_all,
            ._free_impl = (AllocatorI_free_type)std_allocator_free,
        },
    };
}

void* __attribute__((malloc)) std_allocator_alloc(StdAllocator*, u64 size)
{
    return malloc(size);
}
void std_allocator_free_all(StdAllocator*) { }

void std_allocator_resize(StdAllocator*, u64) { return; }
void std_allocator_free(StdAllocator*) { return; }
