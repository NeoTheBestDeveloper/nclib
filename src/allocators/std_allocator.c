#include <stdlib.h>

#include "nclib/allocators/std_allocator.h"

StdAllocator* std_allocator_new()
{
    StdAllocator* allocator = malloc(sizeof(StdAllocator));

    allocator->methods._alloc_impl = (Allocator_alloc_type)std_allocator_alloc;
    allocator->methods._resize_impl
        = (Allocator_resize_type)std_allocator_resize;
    allocator->methods._free_all_impl
        = (Allocator_free_all_type)std_allocator_free_all;
    allocator->methods._free_impl = (Allocator_free_type)std_allocator_free;

    return allocator;
}

void* __attribute__((malloc)) std_allocator_alloc(StdAllocator*, u64 size)
{
    return malloc(size);
}
void std_allocator_free_all(StdAllocator*) { }

void std_allocator_resize(StdAllocator*, u64) { return; }
void std_allocator_free(StdAllocator*) { return; }
