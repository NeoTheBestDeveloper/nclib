#pragma once

#define _CAPACITY_FACTOR (2)

#include "nclib/panic.h"
#include "nclib/typedefs.h"

typedef struct {
    i64 _size;
    i64 _capacity;
    u8* _buf;
} _Darray;

#define CHECK_BOUND(_arr_, _idx_)                                             \
    if ((_idx_ >= 0 && _idx_ >= _arr_->_size)                                 \
        || (_idx_ < 0 && -_idx_ > _arr_->_size)) {                            \
        panic("Error: darray index out of bound at %s:%d. idx=%li, "          \
              "size=%li\n.",                                                  \
              __FILE__, __LINE__, _idx_, _arr_->_size);                       \
    }

#define _gen_darray_new(_type_)                                               \
    darray_##_type_ darray_##_type_##_new(i64 size)                           \
    {                                                                         \
        _Darray array = {                                                     \
            ._size = size,                                                    \
            ._capacity = size * _CAPACITY_FACTOR,                             \
        };                                                                    \
        array._buf = calloc(array._capacity, sizeof(_type_));                 \
        return array;                                                         \
    }

#define _gen_darray_new_with_capacity(_type_)                                 \
    darray_##_type_ darray_##_type_##_new_with_capacity(i64 capacity)         \
    {                                                                         \
        _Darray array = {                                                     \
            ._size = 0,                                                       \
            ._capacity = capacity,                                            \
        };                                                                    \
        array._buf = calloc(array._capacity, sizeof(_type_));                 \
        return array;                                                         \
    }

#define _gen_darray_size(_type_)                                              \
    static inline i64 darray_##_type_##_size(darray_##_type_* arr)            \
    {                                                                         \
        return arr->_size;                                                    \
    }

#define _gen_darray_capacity(_type_)                                          \
    static inline i64 darray_##_type_##_capacity(darray_##_type_* arr)        \
    {                                                                         \
        return arr->_capacity;                                                \
    }

#define _gen_darray_set(_type_)                                               \
    void darray_##_type_##_set(darray_##_type_* arr, i64 idx, _type_ obj)     \
    {                                                                         \
        CHECK_BOUND(arr, idx);                                                \
        if (idx < 0) {                                                        \
            ((_type_*)(arr->_buf))[arr->_size + idx] = obj;                   \
        }                                                                     \
        else {                                                                \
            ((_type_*)(arr->_buf))[idx] = obj;                                \
        }                                                                     \
    }

#define _gen_darray_get(_type_)                                               \
    _type_ darray_##_type_##_get(const darray_##_type_* arr, i64 idx)         \
    {                                                                         \
        CHECK_BOUND(arr, idx);                                                \
        if (idx < 0) {                                                        \
            return ((_type_*)(arr->_buf))[arr->_size + idx];                  \
        }                                                                     \
        return ((_type_*)(arr->_buf))[idx];                                   \
    }

#define darray_register_type_for_header(_type_)                               \
    typedef _Darray darray_##_type_;                                          \
    darray_##_type_ darray_##_type_##_new(i64 size);                          \
    darray_##_type_ darray_##_type_##_new_with_capacity(i64 capacity);        \
                                                                              \
    _gen_darray_size(_type_);                                                 \
    _gen_darray_capacity(_type_);                                             \
    void darray_##_type_##_set(darray_##_type_* arr, i64 idx, _type_ obj);    \
    _type_ darray_##_type_##_get(const darray_##_type_* arr, i64 idx);

#define darray_register_type_for_translation_unit(_type_)                     \
    _gen_darray_new(_type_);                                                  \
    _gen_darray_new_with_capacity(_type_);                                    \
    _gen_darray_set(_type_);                                                  \
    _gen_darray_get(_type_);
