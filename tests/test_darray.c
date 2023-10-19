#include <stdlib.h>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "nclib/collections/darray.h"

darray_register_type_for_header(i32);
darray_register_type_for_translation_unit(i32);

Test(TestDarray, test_array_new)
{
    darray_i32 arr = darray_i32_new(10);

    cr_assert(eq(u64, arr._size, 0));
    cr_assert(eq(u64, arr._capacity, 10));
}

Test(TestDarray, test_array_len)
{
    darray_i32 arr = darray_i32_new(3);

    cr_assert(eq(u64, darray_i32_len(&arr), 0));
}

Test(TestDarray, test_array_capacity)
{
    darray_i32 arr = darray_i32_new(5);

    cr_assert(eq(u64, darray_i32_len(&arr), 0));
    cr_assert(eq(u64, darray_i32_capacity(&arr), 5));
}

Test(TestDarray, test_array_set_fail, .exit_code = EXIT_FAILURE)
{
    darray_i32 arr = darray_i32_new(5);

    darray_i32_set(&arr, 5, 1);
}

Test(TestDarray, test_array_set)
{
    darray_i32 arr = darray_i32_new(5);

    darray_i32_set(&arr, 0, 227);

    cr_assert(eq(i32, ((i32*)arr._buf)[0], 227));
}

Test(TestDarray, test_array_get_fail, .exit_code = EXIT_FAILURE)
{
    darray_i32 arr = darray_i32_new(5);

    i32 value = darray_i32_get(&arr, 5);
}

Test(TestDarray, test_array_get)
{
    darray_i32 arr = darray_i32_new(5);
    ((i32*)arr._buf)[0] = 337;

    cr_assert(eq(i32, darray_i32_get(&arr, 0), 337));
}
