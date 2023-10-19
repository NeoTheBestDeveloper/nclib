#pragma once

#include "nclib/panic.h"
#include "nclib/typedefs.h"

#define _CAPACITY_FACTOR (2)

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

#define GET_ARR_ELEM(_type_, _arr_, _idx_) (((_type_*)(_arr_->_buf))[_idx_])
#define SET_ARR_ELEM(_type_, _arr_, _idx_, _value_)                           \
    (((_type_*)(_arr_->_buf))[_idx_] = _value_)

static inline i64 _calculate_positive_idx(i64 idx, i64 size)
{
    return (idx < 0) ? size + idx : idx;
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

#define _gen_darray_free(_type_)                                              \
    darray_##_type_ darray_##_type_##_free(darray_##_type_* arr)              \
    {                                                                         \
        free(arr->_buf);                                                      \
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
        i64 positive_idx = _calculate_positive_idx(idx, arr->_size);          \
        SET_ARR_ELEM(_type_, arr, positive_idx, obj);                         \
    }

#define _gen_darray_get(_type_)                                               \
    _type_ darray_##_type_##_get(const darray_##_type_* arr, i64 idx)         \
    {                                                                         \
        CHECK_BOUND(arr, idx);                                                \
        i64 positive_idx = _calculate_positive_idx(idx, arr->_size);          \
        return GET_ARR_ELEM(_type_, arr, positive_idx);                       \
    }

#define _gen_darray_push(_type_)                                              \
    void darray_##_type_##_push(darray_##_type_* arr, _type_ obj)             \
    {                                                                         \
        if (arr->_size >= arr->_capacity) {                                   \
            arr->_buf = realloc(arr->_buf, arr->_size * _CAPACITY_FACTOR);    \
            arr->_capacity = arr->_size * _CAPACITY_FACTOR;                   \
        }                                                                     \
        SET_ARR_ELEM(_type_, arr, arr->_size, obj);                           \
        arr->_size += 1;                                                      \
    }

#define _DARRAY_LEFT_SHIFT(_type_, _arr_)                                     \
    for (i64 i = 0; i < _arr_->_size - 1; ++i) {                              \
        _type_ tmp = GET_ARR_ELEM(_type_, _arr_, i + 1);                      \
        SET_ARR_ELEM(_type_, _arr_, i, tmp);                                  \
    }

#define _gen_darray_pop(_type_)                                               \
    _type_ darray_##_type_##_pop(darray_##_type_* arr)                        \
    {                                                                         \
        arr->_size -= 1;                                                      \
        return GET_ARR_ELEM(_type_, arr, arr->_size);                         \
    }

#define darray_register_type_for_header(_type_)                               \
    typedef _Darray darray_##_type_;                                          \
    darray_##_type_ darray_##_type_##_new(i64 size);                          \
    darray_##_type_ darray_##_type_##_new_with_capacity(i64 capacity);        \
    darray_##_type_ darray_##_type_##_free(darray_##_type_* arr);             \
                                                                              \
    _gen_darray_size(_type_);                                                 \
    _gen_darray_capacity(_type_);                                             \
    void darray_##_type_##_set(darray_##_type_* arr, i64 idx, _type_ obj);    \
    _type_ darray_##_type_##_get(const darray_##_type_* arr, i64 idx);        \
    void darray_##_type_##_push(darray_##_type_* arr, _type_ obj);            \
    _type_ darray_##_type_##_pop(darray_##_type_* arr);

#define darray_register_type_for_translation_unit(_type_)                     \
    _gen_darray_new(_type_);                                                  \
    _gen_darray_new_with_capacity(_type_);                                    \
    _gen_darray_set(_type_);                                                  \
    _gen_darray_get(_type_);                                                  \
    _gen_darray_free(_type_);                                                 \
    _gen_darray_push(_type_);                                                 \
    _gen_darray_pop(_type_);
