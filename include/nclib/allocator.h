#pragma once

#include "nclib.h"

typedef struct AllocatorI AllocatorI;

typedef void* (*AllocatorI_alloc_type)(AllocatorI*, u64);
typedef void (*AllocatorI_resize_type)(AllocatorI*, u64);
typedef void (*AllocatorI_free_type)(AllocatorI*);
typedef void (*AllocatorI_free_all_type)(AllocatorI*);

struct AllocatorI {
    AllocatorI_alloc_type _alloc_impl;
    AllocatorI_resize_type _resize_impl;
    AllocatorI_free_type _free_impl;
    AllocatorI_free_all_type _free_all_impl;
};

inline void* allocator_alloc(AllocatorI* allocator, u64 size)
{
    return allocator->_alloc_impl(allocator, size);
}

inline void allocator_resize(AllocatorI* allocator, u64 new_size)
{
    allocator->_resize_impl(allocator, new_size);
}

inline void allocator_free(AllocatorI* allocator)
{
    allocator->_free_impl(allocator);
}

inline void allocator_free_all(AllocatorI* allocator)
{
    allocator->_free_all_impl(allocator);
}
