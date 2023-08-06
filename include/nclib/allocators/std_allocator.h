#pragma once

#include "nclib/allocators/allocator.h"

// Adopt standart malloc for AllocatorI interface.
typedef struct {
    AllocatorI methods;
} StdAllocator;

StdAllocator std_allocator_new();

// Work as standart malloc.
void* __attribute__((malloc)) std_allocator_alloc(StdAllocator*, u64 size);

// Do nothing.
void std_allocator_resize(StdAllocator*, u64 new_size);

// Do nothing.
void std_allocator_free(StdAllocator*);

// Real memory free.
void std_allocator_free_all(StdAllocator*);
