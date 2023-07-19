#include <string.h>

#include "nclib/stream.h"

#define CHECK_BOUND(stream, num)                                              \
    if (stream->offset + sizeof *num > stream->size) {                        \
        return (StreamResult) {                                               \
            .readen = 0,                                                      \
            .status = STREAM_OUT_OF_RANGE_ERROR,                              \
        };                                                                    \
    }

// PRIVATE METHODS
static inline StreamResult stream_read_straight_bytes(Stream* stream, u8* dst,
                                                      u64 size);
static inline StreamResult stream_read_reverse_bytes(Stream* stream, u8* dst,
                                                     u64 size);

static Stream_read_bytes_type
stream_find_read_bytes_impl(StreamEndian buf_endian, u64 size);

static StreamResult stream_straight_read_two_bytes(void* stream, u8* num);
static StreamResult stream_reverse_read_two_bytes(void* stream, u8* num);

static StreamResult stream_straight_read_four_bytes(void* stream, u8* num);
static StreamResult stream_reverse_read_four_bytes(void* stream, u8* num);

static StreamResult stream_straight_read_eight_bytes(void* stream, u8* num);
static StreamResult stream_reverse_read_eight_bytes(void* stream, u8* num);

Stream stream_new(void* buf, u64 buf_size, StreamEndian buf_endian)
{
    return (Stream) {
        .buf = (u8*)buf,
        .size = buf_size,
        .offset = 0,

        ._stream_read_two_bytes_impl
        = stream_find_read_bytes_impl(buf_endian, 2),
        ._stream_read_four_bytes_impl
        = stream_find_read_bytes_impl(buf_endian, 4),
        ._stream_read_eight_bytes_impl
        = stream_find_read_bytes_impl(buf_endian, 8),
    };
}

StreamResult stream_read_u8(Stream* stream, u8* num)
{
    CHECK_BOUND(stream, num);
    return stream_read_straight_bytes(stream, (u8*)num, 1);
}

StreamResult stream_read_i8(Stream* stream, i8* num)
{
    CHECK_BOUND(stream, num);
    return stream_read_straight_bytes(stream, (u8*)num, 1);
}

StreamResult stream_read_u16(Stream* stream, u16* num)
{
    CHECK_BOUND(stream, num);
    return stream->_stream_read_two_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_i16(Stream* stream, i16* num)
{
    CHECK_BOUND(stream, num);
    return stream->_stream_read_two_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_u32(Stream* stream, u32* num)
{
    CHECK_BOUND(stream, num);
    return stream->_stream_read_four_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_i32(Stream* stream, i32* num)
{
    CHECK_BOUND(stream, num);
    return stream->_stream_read_four_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_f32(Stream* stream, f32* num)
{
    CHECK_BOUND(stream, num);
    return stream->_stream_read_four_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_f64(Stream* stream, f64* num)
{
    CHECK_BOUND(stream, num);
    return stream->_stream_read_eight_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_u64(Stream* stream, u64* num)
{
    CHECK_BOUND(stream, num);
    return stream->_stream_read_eight_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_i64(Stream* stream, i64* num)
{
    CHECK_BOUND(stream, num);
    return stream->_stream_read_eight_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_bool(Stream* stream, bool* flag)
{
    CHECK_BOUND(stream, flag);
    return stream_read_straight_bytes(stream, (u8*)flag, 1);
}

StreamResult stream_read_void(Stream* stream, void* bytes, u64 size)
{
    if (stream->offset + size > stream->size) {
        return (StreamResult) {
            .readen = 0,
            .status = STREAM_OUT_OF_RANGE_ERROR,
        };
    }
    return stream_read_straight_bytes(stream, bytes, size);
}

static Stream_read_bytes_type
stream_find_read_bytes_impl(StreamEndian buf_endian, u64 size)
{
    if (size == 2) {
        return buf_endian == MACHINE_ENDIAN ? stream_straight_read_two_bytes
                                            : stream_reverse_read_two_bytes;
    }

    if (size == 4) {
        return buf_endian == MACHINE_ENDIAN ? stream_straight_read_four_bytes
                                            : stream_reverse_read_four_bytes;
    }

    return buf_endian == MACHINE_ENDIAN ? stream_straight_read_eight_bytes
                                        : stream_reverse_read_eight_bytes;
}

static StreamResult stream_straight_read_two_bytes(void* stream, u8* num)
{
    return stream_read_straight_bytes((Stream*)stream, num, 2);
}

static StreamResult stream_reverse_read_two_bytes(void* stream, u8* num)
{
    return stream_read_reverse_bytes((Stream*)stream, num, 2);
}

static inline StreamResult stream_read_straight_bytes(Stream* stream, u8* dst,
                                                      u64 size)
{
    memcpy(dst, stream->buf + stream->offset, size);
    stream->offset += size;

    return (StreamResult) { .readen = size, .status = STREAM_OK };
}

static inline StreamResult stream_read_reverse_bytes(Stream* stream, u8* dst,
                                                     u64 size)
{
    for (u64 i = stream->offset; i < stream->offset + size; ++i) {
        dst[size - (i - stream->offset) - 1] = stream->buf[i];
    }
    stream->offset += size;

    return (StreamResult) { .readen = size, .status = STREAM_OK };
}

static StreamResult stream_straight_read_four_bytes(void* stream, u8* num)
{
    return stream_read_straight_bytes((Stream*)stream, num, 4);
}

static StreamResult stream_reverse_read_four_bytes(void* stream, u8* num)
{
    return stream_read_reverse_bytes((Stream*)stream, num, 4);
}

static StreamResult stream_straight_read_eight_bytes(void* stream, u8* num)
{
    return stream_read_straight_bytes((Stream*)stream, num, 8);
}

static StreamResult stream_reverse_read_eight_bytes(void* stream, u8* num)
{
    return stream_read_reverse_bytes((Stream*)stream, num, 8);
}
