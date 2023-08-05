#include <string.h>

#include "nclib/stream.h"

#ifdef CHECK_BOUND

#include <stdio.h>
#include <stdlib.h>
#undef stream_check_bound

#define stream_check_bound(_stream_, _offset_differance_)                     \
    if (_stream_->offset + _offset_differance_ > _stream_->size) {            \
        fprintf(                                                              \
            stderr,                                                           \
            "Error: stream access out of bound at %s:%d. Size=%lu, access "   \
            "by index=%lu.\n",                                                \
            __FILE__, __LINE__, _stream_->size,                               \
            _stream_->offset + _offset_differance_);                          \
        exit(EXIT_FAILURE);                                                   \
    }

#else

#define stream_check_bound(_stream, _offset_differance_)

#endif // endif !CHECK_BOUND

#define STREAM_READ_GEN(_type_)                                               \
    _type_ stream_read_##_type_(Stream* stream)                               \
    {                                                                         \
        _type_ buf;                                                           \
        stream->_stream_read_bytes_impl(stream, (u8*)(&buf), sizeof buf);     \
        return buf;                                                           \
    }

#define STREAM_WRITE_GEN(_type_)                                              \
    void stream_write_##_type_(Stream* stream, _type_ buf)                    \
    {                                                                         \
        stream->_stream_write_bytes_impl(stream, (u8*)(&buf), sizeof buf);    \
    }

// PRIVATE METHODS.
// READ.
static Stream_read_bytes_type stream_find_read_bytes_impl(StreamEndian endian);
static inline void stream_read_straight_bytes(Stream* stream, u8* dst,
                                              u64 size);
static inline void stream_read_reverse_bytes(Stream* stream, u8* dst,
                                             u64 size);

// WRITE.
static Stream_write_bytes_type
stream_find_write_bytes_impl(StreamEndian endian);
static inline void stream_write_straight_bytes(Stream* stream, const u8* src,
                                               u64 size);
static inline void stream_write_reverse_bytes(Stream* stream, const u8* src,
                                              u64 size);

// PUBLIC METHODS.
Stream stream_new(u8* buf, u64 buf_size, StreamEndian endian)
{
    return (Stream) {
        .buf = (u8*)buf,
        .size = buf_size,
        .offset = 0,

        ._stream_read_bytes_impl = stream_find_read_bytes_impl(endian),
        ._stream_write_bytes_impl = stream_find_write_bytes_impl(endian),
    };
}

Stream stream_new_be(u8* buf, u64 buf_size)
{
    return stream_new(buf, buf_size, STREAM_BIG_ENDIAN);
}

Stream stream_new_le(u8* buf, u64 buf_size)
{
    return stream_new(buf, buf_size, STREAM_LITTLE_ENDIAN);
}

STREAM_READ_GEN(u8)
STREAM_READ_GEN(i8)
STREAM_READ_GEN(u16)
STREAM_READ_GEN(i16)
STREAM_READ_GEN(u32)
STREAM_READ_GEN(i32)
STREAM_READ_GEN(u64)
STREAM_READ_GEN(i64)
STREAM_READ_GEN(f32)
STREAM_READ_GEN(f64)
STREAM_READ_GEN(bool)

void stream_read_bytes(Stream* stream, u8* bytes, u64 size)
{
    stream_read_straight_bytes(stream, bytes, size);
}

STREAM_WRITE_GEN(u8)
STREAM_WRITE_GEN(i8)
STREAM_WRITE_GEN(u16)
STREAM_WRITE_GEN(i16)
STREAM_WRITE_GEN(u32)
STREAM_WRITE_GEN(i32)
STREAM_WRITE_GEN(u64)
STREAM_WRITE_GEN(i64)
STREAM_WRITE_GEN(f32)
STREAM_WRITE_GEN(f64)
STREAM_WRITE_GEN(bool)

void stream_write_bytes(Stream* stream, const u8* bytes, u64 size)
{
    stream_write_straight_bytes(stream, bytes, size);
}

// PRIVATE METHODS.
static Stream_read_bytes_type stream_find_read_bytes_impl(StreamEndian endian)
{
    return endian == MACHINE_ENDIAN ? stream_read_straight_bytes
                                    : stream_read_reverse_bytes;
}

static inline void stream_read_straight_bytes(Stream* stream, u8* dst,
                                              u64 size)
{
    stream_check_bound(stream, size)

        memcpy(dst, stream->buf + stream->offset, size);
    stream->offset += size;
}

static inline void stream_read_reverse_bytes(Stream* stream, u8* dst, u64 size)
{
    stream_check_bound(stream, size);
    for (u64 i = stream->offset; i < stream->offset + size; ++i) {
        dst[size - (i - stream->offset) - 1] = stream->buf[i];
    }
    stream->offset += size;
}

static Stream_write_bytes_type
stream_find_write_bytes_impl(StreamEndian endian)
{
    return endian == MACHINE_ENDIAN ? stream_write_straight_bytes
                                    : stream_write_reverse_bytes;
}

static inline void stream_write_straight_bytes(Stream* stream, const u8* src,
                                               u64 size)
{
    stream_check_bound(stream, size);
    memcpy(stream->buf + stream->offset, src, size);
    stream->offset += size;
}

static inline void stream_write_reverse_bytes(Stream* stream, const u8* src,
                                              u64 size)
{
    stream_check_bound(stream, size);
    for (u64 i = stream->offset; i < stream->offset + size; ++i) {
        stream->buf[i] = src[size - (i - stream->offset) - 1];
    }
}

u64 stream_seek(Stream* stream, i64 offset, StreamWhence whence)
{
    if (whence == STREAM_START) {
        stream->offset = (u64)offset;
    }
    else if (whence == STREAM_CURR) {
        stream->offset = (u64)((i64)stream->offset + offset);
    }
    else {
        stream->offset = (u64)((i64)stream->size - offset);
    }

    return stream->offset;
}
