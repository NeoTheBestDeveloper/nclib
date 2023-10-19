#pragma once

#define _CAPACITY_FACTOR (2)

#include "nclib/allocators/allocator.h"
#include "nclib/allocators/std_allocator.h"
#include "nclib/panic.h"
#include "nclib/typedefs.h"

typedef struct {
    u64 _size;
    u64 _capacity;
    u8* _buf;
    Allocator* _allocator;
} _Darray;

#define _darray_gen_new(_type_)                                               \
    darray_##_type_ darray_##_type_##_new(u64 size)                           \
    {                                                                         \
        _Darray array = {                                                     \
            ._size = size,                                                    \
            ._allocator = (Allocator*)std_allocator_new(),                    \
            ._capacity = size * _CAPACITY_FACTOR,                             \
        };                                                                    \
        array._buf = allocator_alloc(array._allocator,                        \
                                     array._capacity * sizeof(_type_));       \
        memset(array._buf, 0, array._capacity);                               \
        return array;                                                         \
    }

#define _darray_gen_new_with_capacity(_type_)                                 \
    darray_##_type_ darray_##_type_##_new_with_capacity(u64 capacity)         \
    {                                                                         \
        _Darray array = {                                                     \
            ._size = 0,                                                       \
            ._allocator = (Allocator*)std_allocator_new(),                    \
            ._capacity = capacity,                                            \
        };                                                                    \
        array._buf = allocator_alloc(array._allocator,                        \
                                     array._capacity * sizeof(_type_));       \
        memset(array._buf, 0, array._capacity);                               \
        return array;                                                         \
    }

#define _darray_gen_len(_type_)                                               \
    static inline u64 darray_##_type_##_len(darray_##_type_* arr)             \
    {                                                                         \
        return arr->_size;                                                    \
    }

#define _darray_gen_capacity(_type_)                                          \
    static inline u64 darray_##_type_##_capacity(darray_##_type_* arr)        \
    {                                                                         \
        return arr->_capacity;                                                \
    }

#define _darray_gen_set(_type_)                                               \
    void darray_##_type_##_set(darray_##_type_* arr, u64 idx, _type_ obj)     \
    {                                                                         \
        if (idx >= arr->_size) {                                              \
            panic("Error: darray index out of bound at %s:%d. idx=%lu, "      \
                  "capacity=%lu\n.",                                          \
                  __FILE__, __LINE__, idx, arr->_capacity);                   \
        }                                                                     \
                                                                              \
        ((_type_*)(arr->_buf))[idx] = obj;                                    \
    }

#define _darray_gen_get(_type_)                                               \
    _type_ darray_##_type_##_get(const darray_##_type_* arr, u64 idx)         \
    {                                                                         \
        if (idx >= arr->_size) {                                              \
            panic("Error: darray index out of bound at %s:%d. idx=%lu, "      \
                  "capacity=%lu\n.",                                          \
                  __FILE__, __LINE__, idx, arr->_capacity);                   \
        }                                                                     \
                                                                              \
        return ((_type_*)(arr->_buf))[idx];                                   \
    }

#define darray_register_type_for_header(_type_)                               \
    typedef _Darray darray_##_type_;                                          \
    darray_##_type_ darray_##_type_##_new(u64 size);                          \
    darray_##_type_ darray_##_type_##_new_with_capacity(u64 capacity);        \
    void darray_##_type_##_set(darray_##_type_* arr, u64 idx, _type_ obj);    \
    _type_ darray_##_type_##_get(const darray_##_type_* arr, u64 idx);        \
                                                                              \
    _darray_gen_len(_type_);                                                  \
    _darray_gen_capacity(_type_);

#define darray_register_type_for_translation_unit(_type_)                     \
    _darray_gen_new(_type_);                                                  \
    _darray_gen_new_with_capacity(_type_);                                    \
    _darray_gen_set(_type_);                                                  \
    _darray_gen_get(_type_);
