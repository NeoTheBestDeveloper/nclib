#include <float.h>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "nclib/streams/mut_stream.h"

/* be and le payload layout:
 | 1 byte | 2 bytes   | 4 bytes | 8 bytes | ....
 |    u8  |   u16     |   u32   |   u64   | ....
  .... | 1 byte | 2 bytes | 4 bytes | 8 bytes | 4 bytes | 8 bytes | ....
  .... |   i8   |   i16   |   i32   |   i64   |    f32  |   f64   | ....
  .... | 1 byte | 1 byte  |
  .... |   bool |   bool  |
*/

u8 be_payload[] = {
    0x9c, 0xff, 0x9c, 0xff, 0xff, 0xff, 0x9c, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x9c, 0xe4, 0x80, 0x64, 0x80, 0x00, 0x00, 0x64,
    0x80, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x64, 0x7f, 0x7f, 0xff,
    0xff, 0x7f, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0,  0x1,
};
u8 le_payload[] = {
    0x9c, 0x9c, 0xff, 0x9c, 0xff, 0xff, 0xff, 0x9c, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xe4, 0x64, 0x80, 0x64, 0x0,  0x0,  0x80,
    0x64, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x80, 0xff, 0xff, 0x7f,
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0x7f, 0x0,  0x1,
};
u8 void_payload[] = { 0x2, 0x3, 0x20, 0x64, 0x80, 0x96 };

u8 le_addr[] = { 0x64, 0x0, 0x0, 0x0, 0xea, 0x0 };
u8 be_addr[] = { 0x0, 0x0, 0x0, 0x64, 0x0, 0xea };

const i32 page_id_expected = 100;
const i16 offset_expected = 234;

const u8 u8_expected = 156; // 2^8  - 100
const u16 u16_expected = 65436; // 2^16 - 100
const u32 u32_expected = 4294967196; // 2^32 - 100
const u64 u64_expected = 18446744073709551516u; // 2^64 - 100

const i8 i8_expected = -28; // -(2^8  / 2 - 100)
const i16 i16_expected = -32668; // -(2^16 / 2 - 100)
const i32 i32_expected = -2147483548; // -(2^32 / 2 - 100)
const i64 i64_expected = -9223372036854775708; // -(2^64 / 2 - 100)

const f32 f32_expected = FLT_MAX;
const f64 f64_expected = DBL_MAX;

const bool bool1_expected = false;
const bool bool2_expected = true;

const u64 u8_offset = 0;
const u64 u16_offset = 1;
const u64 u32_offset = 3;
const u64 u64_offset = 7;

const u64 i8_offset = 15;
const u64 i16_offset = 16;
const u64 i32_offset = 18;
const u64 i64_offset = 22;

const u64 f32_offset = 30;
const u64 f64_offset = 34;

const u64 bool1_offset = 42;
const u64 bool2_offset = 43;

Test(TestMutStream, test_read_be_u8)
{
    u8* payload_start = be_payload + u8_offset;
    const u64 payload_size = sizeof be_payload - u8_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    u8 res = mut_stream_read_u8(&s);
    cr_assert(eq(u8, res, u8_expected));
}

Test(TestMutStream, test_read_le_u8)
{
    u8* payload_start = le_payload + u8_offset;
    const u64 payload_size = sizeof le_payload - u8_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    u8 res = mut_stream_read_u8(&s);
    cr_assert(eq(u8, res, u8_expected));
}

Test(TestMutStream, test_read_be_i8)
{
    u8* payload_start = be_payload + i8_offset;
    const u64 payload_size = sizeof be_payload - i8_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    i8 res = mut_stream_read_i8(&s);
    cr_assert(eq(i8, res, i8_expected));
}

Test(TestMutStream, test_read_le_i8)
{
    u8* payload_start = le_payload + i8_offset;
    const u64 payload_size = sizeof le_payload - i8_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    i8 res = mut_stream_read_i8(&s);
    cr_assert(eq(i8, res, i8_expected));
}

Test(TestMutStream, test_read_be_u16)
{
    u8* payload_start = be_payload + u16_offset;
    const u64 payload_size = sizeof be_payload - u16_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    u16 res = mut_stream_read_u16(&s);
    cr_assert(eq(u16, res, u16_expected));
}

Test(TestMutStream, test_read_le_u16)
{
    u8* payload_start = le_payload + u16_offset;
    const u64 payload_size = sizeof le_payload - u16_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_LITTLE_ENDIAN);
    u16 res = mut_stream_read_u16(&s);
    cr_assert(eq(u16, res, u16_expected));
}

Test(TestMutStream, test_read_be_i16)
{
    u8* payload_start = be_payload + i16_offset;
    const u64 payload_size = sizeof be_payload - i16_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    i16 res = mut_stream_read_i16(&s);
    cr_assert(eq(i16, res, i16_expected));
}

Test(TestMutStream, test_read_le_i16)
{
    u8* payload_start = le_payload + i16_offset;
    const u64 payload_size = sizeof le_payload - i16_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_LITTLE_ENDIAN);
    i16 res = mut_stream_read_i16(&s);
    cr_assert(eq(i16, res, i16_expected));
}

Test(TestMutStream, test_read_be_u32)
{
    u8* payload_start = be_payload + u32_offset;
    const u64 payload_size = sizeof be_payload - u32_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    u32 res = mut_stream_read_u32(&s);
    cr_assert(eq(u32, res, u32_expected));
}

Test(TestMutStream, test_read_le_u32)
{
    u8* payload_start = le_payload + u32_offset;
    const u64 payload_size = sizeof le_payload - u32_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_LITTLE_ENDIAN);
    u32 res = mut_stream_read_u32(&s);
    cr_assert(eq(u32, res, u32_expected));
}

Test(TestMutStream, test_read_be_i32)
{
    u8* payload_start = be_payload + i32_offset;
    const u64 payload_size = sizeof be_payload - i32_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    i32 res = mut_stream_read_i32(&s);
    cr_assert(eq(i32, res, i32_expected));
}

Test(TestMutStream, test_read_le_i32)
{
    u8* payload_start = le_payload + i32_offset;
    const u64 payload_size = sizeof le_payload - i32_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_LITTLE_ENDIAN);
    i32 res = mut_stream_read_i32(&s);
    cr_assert(eq(i32, res, i32_expected));
}

Test(TestMutStream, test_read_be_f32)
{
    u8* payload_start = be_payload + f32_offset;
    const u64 payload_size = sizeof be_payload - f32_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    f32 res = mut_stream_read_f32(&s);
    cr_assert(eq(flt, res, f32_expected));
}

Test(TestMutStream, test_read_le_f32)
{
    u8* payload_start = le_payload + f32_offset;
    const u64 payload_size = sizeof le_payload - f32_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_LITTLE_ENDIAN);
    f32 res = mut_stream_read_f32(&s);
    cr_assert(eq(flt, res, f32_expected));
}

Test(TestMutStream, test_read_be_f64)
{
    u8* payload_start = be_payload + f64_offset;
    const u64 payload_size = sizeof be_payload - f64_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    f64 res = mut_stream_read_f64(&s);
    cr_assert(eq(dbl, res, f64_expected));
}

Test(TestMutStream, test_read_le_f64)
{
    u8* payload_start = le_payload + f64_offset;
    const u64 payload_size = sizeof le_payload - f64_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_LITTLE_ENDIAN);
    f64 res = mut_stream_read_f64(&s);
    cr_assert(eq(dbl, res, f64_expected));
}

Test(TestMutStream, test_read_be_u64)
{
    u8* payload_start = be_payload + u64_offset;
    const u64 payload_size = sizeof be_payload - u64_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    u64 res = mut_stream_read_u64(&s);
    cr_assert(eq(u64, res, u64_expected));
}

Test(TestMutStream, test_read_le_u64)
{
    u8* payload_start = le_payload + u64_offset;
    const u64 payload_size = sizeof le_payload - u64_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_LITTLE_ENDIAN);
    u64 res = mut_stream_read_u64(&s);
    cr_assert(eq(u64, res, u64_expected));
}

Test(TestMutStream, test_read_be_i64)
{
    u8* payload_start = be_payload + i64_offset;
    const u64 payload_size = sizeof be_payload - i64_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    i64 res = mut_stream_read_i64(&s);
    cr_assert(eq(i64, res, i64_expected));
}

Test(TestMutStream, test_read_le_i64)
{
    u8* payload_start = le_payload + i64_offset;
    const u64 payload_size = sizeof le_payload - i64_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_LITTLE_ENDIAN);
    i64 res = mut_stream_read_i64(&s);
    cr_assert(eq(i64, res, i64_expected));
}

Test(TestMutStream, test_read_be_bool1)
{
    u8* payload_start = be_payload + bool1_offset;
    const u64 payload_size = sizeof be_payload - bool1_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    bool flag = mut_stream_read_bool(&s);
    cr_assert(flag == bool1_expected);
}

Test(TestMutStream, test_read_le_bool1)
{
    u8* payload_start = le_payload + bool1_offset;
    const u64 payload_size = sizeof le_payload - bool1_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_LITTLE_ENDIAN);
    bool flag = mut_stream_read_bool(&s);
    cr_assert(flag == bool1_expected);
}

Test(TestMutStream, test_read_be_bool2)
{
    u8* payload_start = be_payload + bool2_offset;
    const u64 payload_size = sizeof be_payload - bool2_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);
    bool flag = mut_stream_read_bool(&s);
    cr_assert(flag == bool2_expected);
}

Test(TestMutStream, test_read_le_bool2)
{
    u8* payload_start = le_payload + bool2_offset;
    const u64 payload_size = sizeof le_payload - bool2_offset;

    MutStream s
        = mut_stream_new(payload_start, payload_size, STREAM_LITTLE_ENDIAN);

    bool flag = mut_stream_read_bool(&s);
    cr_assert(flag == bool2_expected);
}

Test(TestMutStream, test_read_void)
{
    MutStream s
        = mut_stream_new(void_payload, sizeof void_payload, STREAM_BIG_ENDIAN);

    u8 buf[sizeof void_payload];
    mut_stream_read_bytes(&s, buf, sizeof void_payload);
    cr_assert_arr_eq(buf, void_payload, sizeof void_payload);
}

Test(TestMutStream, test_mut_stream_tell)
{
    MutStream s
        = mut_stream_new(be_payload, sizeof be_payload, STREAM_BIG_ENDIAN);

    cr_assert(eq(u64, mut_stream_tell(&s), 0));

    mut_stream_read_u8(&s);
    cr_assert(eq(u64, mut_stream_tell(&s), 1));

    mut_stream_read_u16(&s);
    cr_assert(eq(u64, mut_stream_tell(&s), 3));

    mut_stream_read_u32(&s);
    cr_assert(eq(u64, mut_stream_tell(&s), 7));

    mut_stream_read_u64(&s);
    cr_assert(eq(u64, mut_stream_tell(&s), 15));
}

Test(TestMutStream, test_mut_stream_seek)
{
    MutStream s
        = mut_stream_new(void_payload, sizeof void_payload, STREAM_BIG_ENDIAN);
    mut_stream_seek(&s, 2, STREAM_START);

    u8 res = mut_stream_read_u8(&s);
    cr_assert(eq(u8, res, void_payload[2]));

    mut_stream_seek(&s, -2, STREAM_CURR);
    res = mut_stream_read_u8(&s);
    cr_assert(eq(u8, res, void_payload[1]));

    mut_stream_seek(&s, 1, STREAM_END);
    res = mut_stream_read_u8(&s);
    cr_assert(eq(u8, res, void_payload[(sizeof void_payload) - 1]));

    mut_stream_seek(&s, 2, STREAM_END);
    res = mut_stream_read_u8(&s);
    cr_assert(eq(u8, res, void_payload[(sizeof void_payload) - 2]));

    mut_stream_seek(&s, 1, STREAM_START);
    res = mut_stream_read_u8(&s);
    cr_assert(eq(u8, res, void_payload[1]));
}

Test(TestMutStream, test_mut_stream_write_be_u8)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_BIG_ENDIAN);

    mut_stream_write_u8(&s, u8_expected);
    cr_assert_arr_eq(be_payload + u8_offset, mut_stream_raw(&s),
                     sizeof u8_expected);
}

Test(TestMutStream, test_mut_stream_write_le_u8)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);

    mut_stream_write_u8(&s, u8_expected);
    cr_assert_arr_eq(le_payload + u8_offset, mut_stream_raw(&s),
                     sizeof u8_expected);
}

Test(TestMutStream, test_mut_stream_write_be_i8)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_BIG_ENDIAN);

    mut_stream_write_i8(&s, i8_expected);
    cr_assert_arr_eq(be_payload + i8_offset, mut_stream_raw(&s),
                     sizeof i8_expected);
}

Test(TestMutStream, test_mut_stream_write_le_i8)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);

    mut_stream_write_i8(&s, i8_expected);
    cr_assert_arr_eq(le_payload + i8_offset, mut_stream_raw(&s),
                     sizeof i8_expected);
}

Test(TestMutStream, test_mut_stream_write_be_u16)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_BIG_ENDIAN);

    mut_stream_write_u16(&s, u16_expected);
    cr_assert_arr_eq(be_payload + u16_offset, mut_stream_raw(&s),
                     sizeof u16_expected);
}

Test(TestMutStream, test_mut_stream_write_le_u16)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);

    mut_stream_write_u16(&s, u16_expected);
    cr_assert_arr_eq(le_payload + u16_offset, mut_stream_raw(&s),
                     sizeof u16_expected);
}

Test(TestMutStream, test_mut_stream_write_be_i16)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_BIG_ENDIAN);

    mut_stream_write_i16(&s, i16_expected);
    cr_assert_arr_eq(be_payload + i16_offset, mut_stream_raw(&s),
                     sizeof i16_expected);
}

Test(TestMutStream, test_mut_stream_write_le_i16)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);

    mut_stream_write_i16(&s, i16_expected);
    cr_assert_arr_eq(le_payload + i16_offset, mut_stream_raw(&s),
                     sizeof i16_expected);
}

Test(TestMutStream, test_mut_stream_write_be_u32)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_BIG_ENDIAN);

    mut_stream_write_u32(&s, u32_expected);
    cr_assert_arr_eq(be_payload + u32_offset, mut_stream_raw(&s),
                     sizeof u32_expected);
}

Test(TestMutStream, test_mut_stream_write_le_u32)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);

    mut_stream_write_u32(&s, u32_expected);
    cr_assert_arr_eq(le_payload + u32_offset, mut_stream_raw(&s),
                     sizeof u32_expected);
}

Test(TestMutStream, test_mut_stream_write_be_i32)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_BIG_ENDIAN);

    mut_stream_write_i32(&s, i32_expected);
    cr_assert_arr_eq(be_payload + i32_offset, mut_stream_raw(&s),
                     sizeof i32_expected);
}

Test(TestMutStream, test_mut_stream_write_le_i32)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);

    mut_stream_write_i32(&s, i32_expected);
    cr_assert_arr_eq(le_payload + i32_offset, mut_stream_raw(&s),
                     sizeof i32_expected);
}

Test(TestMutStream, test_mut_stream_write_be_f32)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_BIG_ENDIAN);

    mut_stream_write_f32(&s, f32_expected);
    cr_assert_arr_eq(be_payload + f32_offset, mut_stream_raw(&s),
                     sizeof f32_expected);
}

Test(TestMutStream, test_mut_stream_write_le_f32)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);

    mut_stream_write_f32(&s, f32_expected);
    cr_assert_arr_eq(le_payload + f32_offset, mut_stream_raw(&s),
                     sizeof f32_expected);
}

Test(TestMutStream, test_mut_stream_write_be_f64)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_BIG_ENDIAN);

    mut_stream_write_f64(&s, f64_expected);
    cr_assert_arr_eq(be_payload + f64_offset, mut_stream_raw(&s),
                     sizeof f64_expected);
}

Test(TestMutStream, test_mut_stream_write_le_f64)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);

    mut_stream_write_f64(&s, f64_expected);
    cr_assert_arr_eq(le_payload + f64_offset, mut_stream_raw(&s),
                     sizeof f64_expected);
}

Test(TestMutStream, test_mut_stream_write_be_u64)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_BIG_ENDIAN);

    mut_stream_write_u64(&s, u64_expected);
    cr_assert_arr_eq(be_payload + u64_offset, mut_stream_raw(&s),
                     sizeof u64_expected);
}

Test(TestMutStream, test_mut_stream_write_le_u64)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);

    mut_stream_write_u64(&s, u64_expected);
    cr_assert_arr_eq(le_payload + u64_offset, mut_stream_raw(&s),
                     sizeof u64_expected);
}

Test(TestMutStream, test_mut_stream_write_be_i64)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_BIG_ENDIAN);

    mut_stream_write_i64(&s, i64_expected);
    cr_assert_arr_eq(be_payload + i64_offset, mut_stream_raw(&s),
                     sizeof i64_expected);
}

Test(TestMutStream, test_mut_stream_write_le_i64)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);

    mut_stream_write_i64(&s, i64_expected);
    cr_assert_arr_eq(le_payload + i64_offset, mut_stream_raw(&s),
                     sizeof i64_expected);
}

Test(TestMutStream, test_mut_stream_write_le_bool1)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);

    mut_stream_write_bool(&s, bool1_expected);
    cr_assert_arr_eq(le_payload + bool1_offset, mut_stream_raw(&s),
                     sizeof bool1_expected);
}

Test(TestMutStream, test_mut_stream_write_be_bool1)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_BIG_ENDIAN);

    mut_stream_write_bool(&s, bool1_expected);
    cr_assert_arr_eq(be_payload + bool1_offset, mut_stream_raw(&s),
                     sizeof bool1_expected);
}

Test(TestMutStream, test_mut_stream_write_le_bool2)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);

    mut_stream_write_bool(&s, bool2_expected);
    cr_assert_arr_eq(le_payload + bool2_offset, mut_stream_raw(&s),
                     sizeof bool2_expected);
}

Test(TestMutStream, test_mut_stream_write_be_bool2)
{
    u8 buf[100];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_BIG_ENDIAN);

    mut_stream_write_bool(&s, bool2_expected);
    cr_assert_arr_eq(be_payload + bool2_offset, mut_stream_raw(&s),
                     sizeof bool2_expected);
}

Test(TestMutStream, test_mut_stream_write_void)
{
    u8 buf[sizeof void_payload];
    MutStream s = mut_stream_new(buf, sizeof buf, STREAM_BIG_ENDIAN);

    mut_stream_write_bytes(&s, void_payload, sizeof void_payload);
    cr_assert_arr_eq(void_payload, mut_stream_raw(&s), sizeof void_payload);
}

typedef struct {
    i32 page_id;
    i16 offset;
} Addr;

Addr mut_stream_read_addr(MutStream* stream)
{
    Addr res;
    stream->_read_bytes_impl(stream, (u8*)(&res.page_id), sizeof res.page_id);
    stream->_read_bytes_impl(stream, (u8*)(&res.offset), sizeof res.offset);
    return res;
}

void mut_stream_write_addr(MutStream* stream, Addr addr)
{
    stream->_write_bytes_impl(stream, (u8*)(&addr.page_id),
                              sizeof addr.page_id);
    stream->_write_bytes_impl(stream, (u8*)(&addr.offset), sizeof addr.offset);
}

Test(TestMutStream, test_stream_extend_types)
{
    MutStream s = mut_stream_new(be_addr, sizeof be_addr, STREAM_BIG_ENDIAN);

    Addr res = mut_stream_read_addr(&s);

    cr_assert(eq(i32, res.page_id, page_id_expected));
    cr_assert(eq(i16, res.offset, offset_expected));

    s = mut_stream_new(le_addr, sizeof le_addr, STREAM_LITTLE_ENDIAN);

    res = mut_stream_read_addr(&s);

    cr_assert(eq(i32, res.page_id, page_id_expected));
    cr_assert(eq(i16, res.offset, offset_expected));

    u8 buf[sizeof le_addr];
    s = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);
    mut_stream_write_addr(&s, res);
    cr_assert_arr_eq(buf, le_addr, sizeof le_addr);
}

Test(TestMutStream, test_documentation_example)
{
    u8 data[4] = { 0, 1, 25, 36 };
    MutStream stream = mut_stream_new_le(data, 4);

    bool first_flag = mut_stream_read_bool(&stream); // first_flag=false
    cr_assert(eq(u8, first_flag, data[0]));
    bool second_flag = mut_stream_read_bool(&stream); // second_flag=true
    cr_assert(eq(u8, second_flag, data[1]));

    mut_stream_write_u8(&stream, 46); // Rewrite 25 to 46.
    cr_assert(eq(u8, data[2], 46));
    mut_stream_seek(&stream, -1, STREAM_CURR); // Step backward.

    u8 first_number = mut_stream_read_u8(&stream); // first_number=46.
    cr_assert(eq(u8, first_number, data[2]));
}
