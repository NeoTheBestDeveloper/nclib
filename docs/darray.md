# Darray

Darray is a dynamic array which support "templates" like C++. It can generate
methods for types in compile time.

## Base methods (For i32 type)

darray_i32 darray_i32_new(i64 size) - create new empty darray with entered capacity. 
darray_i32 darray_i32_new_with_capacity(i64 capacity) - create new empty darray with entered capacity. 
Capacity is a count of preallocated in memory objects.

void darray_i32_free(darray_i32 *arr) - free array.

i32 darray_i32_get(darray_i32 *arr, i64 idx) - get element by index. Also support negative indexes.
void darray_i32_set(darray_i32 *arr, i64 idx, i32 value) - set element by index. 

void darray_i32_push(darray_i32 *arr, i32 value) - add obj at the end of array.
i32 darray_i32_pop(darray_i32 *arr) - delete obj from the end of array.
void darray_i32_insert(darray_i32 *arr, i64 idx, i32 value)

i64 darray_i32_find(darray_i32 *arr, i32 value) - find obj in array and return idx, if not found, then return -1.
i32 darray_i32_remove_by_idx(darray_i32 *arr, i64 idx) - delete obj by idx, return deleted object.
void darray_i32_remove(darray_i32 *arr, i32 value) - delete obj by value

i64 darray_i64_size(darray_i32 *arr) - return arr size.
i64 darray_i64_capacity(darray_i32 *arr) - return arr capacity.
