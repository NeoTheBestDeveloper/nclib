#include <float.h>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "criterion/assert.h"
#include "criterion/internal/assert.h"
#include "nclib.h"

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
    0xf3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3e, 0x7f, 0x7f, 0xff,
    0xff, 0x7f, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0,  0x1,
};
u8 le_payload[] = {
    0x9c, 0x9c, 0xff, 0x9c, 0xff, 0xff, 0xff, 0x9c, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xe4, 0x64, 0x80, 0x64, 0x0,  0x0,  0x80,
    0x3e, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xf3, 0xff, 0xff, 0x7f,
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0x7f, 0x0,  0x1,
};
u8 void_payload[] = {
    0x2, 0x3, 0x20, 0x64, 0x80, 0x96,
};

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

Test(TestStream, test_read_be_u8)
{
    u8* payload_start = be_payload + u8_offset;
    const u64 payload_size = sizeof be_payload - u8_offset;

    Stream s = stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);

    u8 num;
    StreamResult res = stream_read_u8(&s, &num);

    cr_assert(eq(u32, res.status, STREAM_OK));
    cr_assert(eq(u64, res.readen, sizeof num));
    cr_assert(eq(u8, num, u8_expected));
}

Test(TestStream, test_read_le_u8)
{
    u8* payload_start = le_payload + u8_offset;
    const u64 payload_size = sizeof le_payload - u8_offset;

    Stream s = stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);

    u8 num;
    StreamResult res = stream_read_u8(&s, &num);

    cr_assert(eq(u32, res.status, STREAM_OK));
    cr_assert(eq(u64, res.readen, sizeof num));
    cr_assert(eq(u8, num, u8_expected));
}

Test(TestStream, test_read_be_i8)
{
    u8* payload_start = be_payload + i8_offset;
    const u64 payload_size = sizeof be_payload - i8_offset;

    Stream s = stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);

    i8 num;
    StreamResult res = stream_read_i8(&s, &num);

    cr_assert(eq(u32, res.status, STREAM_OK));
    cr_assert(eq(u64, res.readen, sizeof num));
    cr_assert(eq(i8, num, i8_expected));
}

Test(TestStream, test_read_le_i8)
{
    u8* payload_start = le_payload + i8_offset;
    const u64 payload_size = sizeof le_payload - i8_offset;

    Stream s = stream_new(payload_start, payload_size, STREAM_BIG_ENDIAN);

    i8 num;
    StreamResult res = stream_read_i8(&s, &num);

    cr_assert(eq(u32, res.status, STREAM_OK));
    cr_assert(eq(u64, res.readen, sizeof num));
    cr_assert(eq(i8, num, i8_expected));
}

Test(TestStream, test_read_out_of_range)
{
    Stream s = stream_new(be_payload, sizeof be_payload, STREAM_BIG_ENDIAN);

    for (u64 i = 0; i < (sizeof be_payload) + 1; ++i) {
        u8 num;
        StreamResult res = stream_read_u8(&s, &num);
        if (i < sizeof be_payload) {
            cr_assert(eq(u32, res.status, STREAM_OK));
            cr_assert(eq(u64, res.readen, sizeof num));
        }
        else {
            cr_assert(eq(u32, res.status, STREAM_OUT_OF_RANGE_ERROR));
            cr_assert(eq(u64, res.readen, 0));
        }
    }

    // TODO: Check with u64 num
}
