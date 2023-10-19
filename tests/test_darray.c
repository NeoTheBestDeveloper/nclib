#include <stdlib.h>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "nclib/collections/darray.h"

darray_register_type_for_header(i32);
darray_register_type_for_translation_unit(i32);

Test(TestDarray, test_darray_new)
{
    darray_i32 arr = darray_i32_new(10);

    cr_assert(eq(i64, arr._size, 10));
    cr_assert(eq(i64, arr._capacity, _CAPACITY_FACTOR * 10));

    darray_i32_free(&arr);
}

Test(TestDarray, test_darray_new_with_capacity)
{
    darray_i32 arr = darray_i32_new_with_capacity(10);

    cr_assert(eq(i64, arr._size, 0));
    cr_assert(eq(i64, arr._capacity, 10));

    darray_i32_free(&arr);
}

Test(TestDarray, test_darray_len)
{
    darray_i32 arr = darray_i32_new(3);

    cr_assert(eq(i64, darray_i32_size(&arr), 3));

    darray_i32_free(&arr);
}

Test(TestDarray, test_darray_capacity)
{
    darray_i32 arr = darray_i32_new(5);

    cr_assert(eq(i64, darray_i32_size(&arr), 5));
    cr_assert(eq(i64, darray_i32_capacity(&arr), _CAPACITY_FACTOR * 5));

    darray_i32_free(&arr);
}

Test(TestDarray, test_darray_set)
{
    darray_i32 arr = darray_i32_new(5);

    darray_i32_set(&arr, 0, 227);

    cr_assert(eq(i32, ((i32*)arr._buf)[0], 227));

    darray_i32_free(&arr);
}

Test(TestDarray, test_darray_set_negative)
{
    darray_i32 arr = darray_i32_new(5);

    darray_i32_set(&arr, -3, 227);

    cr_assert(eq(i32, ((i32*)arr._buf)[2], 227));

    darray_i32_free(&arr);
}

Test(TestDarray, test_darray_set_fail, .exit_code = EXIT_FAILURE)
{
    darray_i32 arr = darray_i32_new(5);

    darray_i32_set(&arr, 5, 1);

    darray_i32_free(&arr);
}

Test(TestDarray, test_darray_negative_set_fail, .exit_code = EXIT_FAILURE)
{
    darray_i32 arr = darray_i32_new(5);

    darray_i32_set(&arr, -6, 1);

    darray_i32_free(&arr);
}

Test(TestDarray, test_darray_get)
{
    darray_i32 arr = darray_i32_new(5);
    ((i32*)arr._buf)[2] = 337;

    cr_assert(eq(i32, darray_i32_get(&arr, 2), 337));

    darray_i32_free(&arr);
}

Test(TestDarray, test_darray_get_negative)
{
    darray_i32 arr = darray_i32_new(5);
    ((i32*)arr._buf)[3] = 337;

    cr_assert(eq(i32, darray_i32_get(&arr, -2), 337));

    darray_i32_free(&arr);
}

Test(TestDarray, test_darray_get_fail, .exit_code = EXIT_FAILURE)
{
    darray_i32 arr = darray_i32_new(5);

    i32 value = darray_i32_get(&arr, 5);

    darray_i32_free(&arr);
}

Test(TestDarray, test_darray_negative_get_fail, .exit_code = EXIT_FAILURE)
{
    darray_i32 arr = darray_i32_new(5);

    i32 value = darray_i32_get(&arr, -12345);

    darray_i32_free(&arr);
}

Test(TestDarray, test_darray_push_without_new_allocation)
{
    darray_i32 arr = darray_i32_new_with_capacity(5);

    darray_i32_push(&arr, 1);
    darray_i32_push(&arr, 2);
    darray_i32_push(&arr, 3);
    darray_i32_push(&arr, 9328);
    darray_i32_push(&arr, 1234);

    cr_assert(eq(i32, ((i32*)arr._buf)[0], 1));
    cr_assert(eq(i32, ((i32*)arr._buf)[1], 2));
    cr_assert(eq(i32, ((i32*)arr._buf)[2], 3));
    cr_assert(eq(i32, ((i32*)arr._buf)[3], 9328));
    cr_assert(eq(i32, ((i32*)arr._buf)[4], 1234));

    darray_i32_free(&arr);
}

Test(TestDarray, test_darray_push_with_new_allocation)
{
    darray_i32 arr = darray_i32_new_with_capacity(2);

    darray_i32_push(&arr, 1);
    darray_i32_push(&arr, 2);
    darray_i32_push(&arr, 3);

    cr_assert(eq(i32, ((i32*)arr._buf)[0], 1));
    cr_assert(eq(i32, ((i32*)arr._buf)[1], 2));
    cr_assert(eq(i32, ((i32*)arr._buf)[2], 3));

    cr_assert(eq(i64, arr._size, 3));
    cr_assert(eq(i64, arr._capacity, 2 * _CAPACITY_FACTOR));
}
