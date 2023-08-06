#pragma once

#include "nclib/typedefs.h"

typedef struct {
    u8* buf;
    u64 size;
    u64 pos;
} Arena;

Arena arena_new(u64 size);

// Set pos to zero, don't free memory.
void arena_clear(Arena*);

// Free memory.
void arena_free(Arena*);

void* arena_alloc(Arena*, u64 size);
void arena_resize(Arena*, u64 new_size);
