#pragma once

#include "nclib/typedefs.h"

typedef struct Allocator Allocator;

typedef void* (*Allocator_alloc_type)(Allocator*, u64);
typedef void (*Allocator_resize_type)(Allocator*, u64);
typedef void (*Allocator_free_type)(Allocator*);
typedef void (*Allocator_free_all_type)(Allocator*);

struct Allocator {
    Allocator_alloc_type _alloc_impl;
    Allocator_resize_type _resize_impl;
    Allocator_free_type _free_impl;
    Allocator_free_all_type _free_all_impl;
};

inline void* allocator_alloc(Allocator* allocator, u64 size)
{
    return allocator->_alloc_impl(allocator, size);
}

inline void allocator_resize(Allocator* allocator, u64 new_size)
{
    allocator->_resize_impl(allocator, new_size);
}

inline void allocator_free(Allocator* allocator)
{
    allocator->_free_impl(allocator);
}

inline void allocator_free_all(Allocator* allocator)
{
    allocator->_free_all_impl(allocator);
}
