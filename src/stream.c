#include <string.h>

#include "nclib/stream.h"

#define CHECK_BOUND(_stream_, _size_)                                         \
    if (_stream_->offset + _size_ > _stream_->size) {                         \
        return STREAM_OUT_OF_RANGE_ERROR;                                     \
    }

// PRIVATE METHODS.
// READ.
static Stream_read_bytes_type
stream_find_read_bytes_impl(StreamEndian buf_endian, u64 size);

static inline StreamResult stream_read_straight_bytes(Stream* stream, u8* dst,
                                                      u64 size);
static inline StreamResult stream_read_reverse_bytes(Stream* stream, u8* dst,
                                                     u64 size);

static StreamResult stream_straight_read_two_bytes(void* stream, u8* dst);
static StreamResult stream_reverse_read_two_bytes(void* stream, u8* dst);

static StreamResult stream_straight_read_four_bytes(void* stream, u8* dst);
static StreamResult stream_reverse_read_four_bytes(void* stream, u8* dst);

static StreamResult stream_straight_read_eight_bytes(void* stream, u8* dst);
static StreamResult stream_reverse_read_eight_bytes(void* stream, u8* dst);

// WRITE.
static Stream_write_bytes_type
stream_find_write_bytes_impl(StreamEndian buf_endian, u64 size);

static inline StreamResult
stream_write_straight_bytes(Stream* stream, const u8* src, u64 size);
static inline StreamResult stream_write_reverse_bytes(Stream* stream,
                                                      const u8* src, u64 size);

static StreamResult stream_straight_write_two_bytes(void* stream,
                                                    const u8* src);
static StreamResult stream_reverse_write_two_bytes(void* stream,
                                                   const u8* src);

static StreamResult stream_straight_write_four_bytes(void* stream,
                                                     const u8* src);
static StreamResult stream_reverse_write_four_bytes(void* stream,
                                                    const u8* src);

static StreamResult stream_straight_write_eight_bytes(void* stream,
                                                      const u8* src);
static StreamResult stream_reverse_write_eight_bytes(void* stream,
                                                     const u8* src);

// PUBLIC METHODS.
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

        ._stream_write_two_bytes_impl
        = stream_find_write_bytes_impl(buf_endian, 2),
        ._stream_write_four_bytes_impl
        = stream_find_write_bytes_impl(buf_endian, 4),
        ._stream_write_eight_bytes_impl
        = stream_find_write_bytes_impl(buf_endian, 8),
    };
}

StreamResult stream_read_u8(Stream* stream, u8* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream_read_straight_bytes(stream, (u8*)num, 1);
}

StreamResult stream_read_i8(Stream* stream, i8* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream_read_straight_bytes(stream, (u8*)num, 1);
}

StreamResult stream_read_u16(Stream* stream, u16* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_read_two_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_i16(Stream* stream, i16* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_read_two_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_u32(Stream* stream, u32* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_read_four_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_i32(Stream* stream, i32* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_read_four_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_f32(Stream* stream, f32* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_read_four_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_f64(Stream* stream, f64* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_read_eight_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_u64(Stream* stream, u64* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_read_eight_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_i64(Stream* stream, i64* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_read_eight_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_bool(Stream* stream, bool* flag)
{
    CHECK_BOUND(stream, sizeof *flag);
    return stream_read_straight_bytes(stream, (u8*)flag, 1);
}

StreamResult stream_read_void(Stream* stream, void* bytes, u64 size)
{
    CHECK_BOUND(stream, size);
    return stream_read_straight_bytes(stream, (u8*)bytes, size);
}

u64 stream_pos(const Stream* stream) { return stream->offset; }
u64 stream_size(const Stream* stream) { return stream->size; }
const u8* stream_raw(const Stream* stream) { return stream->buf; }

StreamResult stream_write_u8(Stream* stream, const u8* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream_write_straight_bytes(stream, num, 1);
}

StreamResult stream_write_i8(Stream* stream, const i8* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream_write_straight_bytes(stream, (const u8*)num, 1);
}

StreamResult stream_write_u16(Stream* stream, const u16* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_write_two_bytes_impl((Stream*)stream,
                                                (const u8*)num);
}

StreamResult stream_write_i16(Stream* stream, const i16* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_write_two_bytes_impl((Stream*)stream,
                                                (const u8*)num);
}

StreamResult stream_write_u32(Stream* stream, const u32* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_write_four_bytes_impl((Stream*)stream,
                                                 (const u8*)num);
}

StreamResult stream_write_i32(Stream* stream, const i32* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_write_four_bytes_impl((Stream*)stream,
                                                 (const u8*)num);
}

StreamResult stream_write_f32(Stream* stream, const f32* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_write_four_bytes_impl((Stream*)stream,
                                                 (const u8*)num);
}

StreamResult stream_write_f64(Stream* stream, const f64* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_write_eight_bytes_impl((Stream*)stream,
                                                  (const u8*)num);
}

StreamResult stream_write_u64(Stream* stream, const u64* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_write_eight_bytes_impl((Stream*)stream,
                                                  (const u8*)num);
}

StreamResult stream_write_i64(Stream* stream, const i64* num)
{
    CHECK_BOUND(stream, sizeof *num);
    return stream->_stream_write_eight_bytes_impl((Stream*)stream,
                                                  (const u8*)num);
}

StreamResult stream_write_bool(Stream* stream, const bool* flag)
{
    CHECK_BOUND(stream, sizeof *flag);
    return stream_write_straight_bytes(stream, (const u8*)flag, 1);
}

StreamResult stream_write_void(Stream* stream, const void* bytes, u64 size)
{
    CHECK_BOUND(stream, size);
    return stream_write_straight_bytes(stream, bytes, size);
}

// PRIVATE METHODS.
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

    return STREAM_OK;
}

static inline StreamResult stream_read_reverse_bytes(Stream* stream, u8* dst,
                                                     u64 size)
{
    for (u64 i = stream->offset; i < stream->offset + size; ++i) {
        dst[size - (i - stream->offset) - 1] = stream->buf[i];
    }
    stream->offset += size;

    return STREAM_OK;
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

static Stream_write_bytes_type
stream_find_write_bytes_impl(StreamEndian buf_endian, u64 size)
{
    if (size == 2) {
        return buf_endian == MACHINE_ENDIAN ? stream_straight_write_two_bytes
                                            : stream_reverse_write_two_bytes;
    }

    if (size == 4) {
        return buf_endian == MACHINE_ENDIAN ? stream_straight_write_four_bytes
                                            : stream_reverse_write_four_bytes;
    }

    return buf_endian == MACHINE_ENDIAN ? stream_straight_write_eight_bytes
                                        : stream_reverse_write_eight_bytes;
}

static inline StreamResult stream_write_straight_bytes(Stream* stream,
                                                       const u8* src, u64 size)
{
    memcpy(stream->buf + stream->offset, src, size);
    stream->offset += size;
    return STREAM_OK;
}

static inline StreamResult stream_write_reverse_bytes(Stream* stream,
                                                      const u8* src, u64 size)
{
    for (u64 i = stream->offset; i < stream->offset + size; ++i) {
        stream->buf[i] = src[size - (i - stream->offset) - 1];
    }
    return STREAM_OK;
}

static StreamResult stream_straight_write_two_bytes(void* stream,
                                                    const u8* src)
{
    return stream_write_straight_bytes((Stream*)stream, src, 2);
}

static StreamResult stream_reverse_write_two_bytes(void* stream, const u8* src)
{
    return stream_write_reverse_bytes((Stream*)stream, src, 2);
}

static StreamResult stream_straight_write_four_bytes(void* stream,
                                                     const u8* src)
{
    return stream_write_straight_bytes((Stream*)stream, src, 4);
}

static StreamResult stream_reverse_write_four_bytes(void* stream,
                                                    const u8* src)
{
    return stream_write_reverse_bytes((Stream*)stream, src, 4);
}

static StreamResult stream_straight_write_eight_bytes(void* stream,
                                                      const u8* src)
{
    return stream_write_straight_bytes((Stream*)stream, src, 8);
}

static StreamResult stream_reverse_write_eight_bytes(void* stream,
                                                     const u8* src)
{
    return stream_write_reverse_bytes((Stream*)stream, src, 8);
}
