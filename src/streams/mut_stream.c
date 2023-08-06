#include <string.h>

#include "nclib/streams/mutable_stream.h"

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

#define MUT_STREAM_READ_GEN(_type_)                                           \
    _type_ mut_stream_read_##_type_(MutableStream* stream)                    \
    {                                                                         \
        _type_ buf;                                                           \
        stream->_stream_read_bytes_impl(stream, (u8*)(&buf), sizeof buf);     \
        return buf;                                                           \
    }

#define MUT_STREAM_WRITE_GEN(_type_)                                          \
    void mut_stream_write_##_type_(MutableStream* stream, _type_ buf)         \
    {                                                                         \
        stream->_stream_write_bytes_impl(stream, (u8*)(&buf), sizeof buf);    \
    }

// PRIVATE METHODS.
// READ.
static MutableStream_read_bytes_type
mut_stream_find_read_bytes_impl(StreamEndian endian);
static inline void mut_stream_read_straight_bytes(MutableStream* stream,
                                                  u8* dst, u64 size);
static inline void mut_stream_read_reverse_bytes(MutableStream* stream,
                                                 u8* dst, u64 size);

// WRITE.
static MutableStream_write_bytes_type
mut_stream_find_write_bytes_impl(StreamEndian endian);
static inline void mut_stream_write_straight_bytes(MutableStream* stream,
                                                   const u8* src, u64 size);
static inline void mut_stream_write_reverse_bytes(MutableStream* stream,
                                                  const u8* src, u64 size);

// PUBLIC METHODS.
MutableStream mut_stream_new(u8* buf, u64 buf_size, StreamEndian endian)
{
    return (MutableStream) {
        .buf = buf,
        .size = buf_size,
        .offset = 0,

        ._stream_read_bytes_impl = mut_stream_find_read_bytes_impl(endian),
        ._stream_write_bytes_impl = mut_stream_find_write_bytes_impl(endian),
    };
}

MutableStream mut_stream_new_be(u8* buf, u64 buf_size)
{
    return mut_stream_new(buf, buf_size, STREAM_BIG_ENDIAN);
}

MutableStream mut_stream_new_le(u8* buf, u64 buf_size)
{
    return mut_stream_new(buf, buf_size, STREAM_LITTLE_ENDIAN);
}

MUT_STREAM_READ_GEN(u8)
MUT_STREAM_READ_GEN(i8)
MUT_STREAM_READ_GEN(u16)
MUT_STREAM_READ_GEN(i16)
MUT_STREAM_READ_GEN(u32)
MUT_STREAM_READ_GEN(i32)
MUT_STREAM_READ_GEN(u64)
MUT_STREAM_READ_GEN(i64)
MUT_STREAM_READ_GEN(f32)
MUT_STREAM_READ_GEN(f64)
MUT_STREAM_READ_GEN(bool)

void mut_stream_read_bytes(MutableStream* stream, u8* bytes, u64 size)
{
    mut_stream_read_straight_bytes(stream, bytes, size);
}

MUT_STREAM_WRITE_GEN(u8)
MUT_STREAM_WRITE_GEN(i8)
MUT_STREAM_WRITE_GEN(u16)
MUT_STREAM_WRITE_GEN(i16)
MUT_STREAM_WRITE_GEN(u32)
MUT_STREAM_WRITE_GEN(i32)
MUT_STREAM_WRITE_GEN(u64)
MUT_STREAM_WRITE_GEN(i64)
MUT_STREAM_WRITE_GEN(f32)
MUT_STREAM_WRITE_GEN(f64)
MUT_STREAM_WRITE_GEN(bool)

void mut_stream_write_bytes(MutableStream* stream, const u8* bytes, u64 size)
{
    mut_stream_write_straight_bytes(stream, bytes, size);
}

// PRIVATE METHODS.
static MutableStream_read_bytes_type
mut_stream_find_read_bytes_impl(StreamEndian endian)
{
    return endian == MACHINE_ENDIAN ? mut_stream_read_straight_bytes
                                    : mut_stream_read_reverse_bytes;
}

static inline void mut_stream_read_straight_bytes(MutableStream* stream,
                                                  u8* dst, u64 size)
{
    stream_check_bound(stream, size)

        memcpy(dst, stream->buf + stream->offset, size);
    stream->offset += size;
}

static inline void mut_stream_read_reverse_bytes(MutableStream* stream,
                                                 u8* dst, u64 size)
{
    stream_check_bound(stream, size);
    for (u64 i = stream->offset; i < stream->offset + size; ++i) {
        dst[size - (i - stream->offset) - 1] = stream->buf[i];
    }
    stream->offset += size;
}

static MutableStream_write_bytes_type
mut_stream_find_write_bytes_impl(StreamEndian endian)
{
    return endian == MACHINE_ENDIAN ? mut_stream_write_straight_bytes
                                    : mut_stream_write_reverse_bytes;
}

static inline void mut_stream_write_straight_bytes(MutableStream* stream,
                                                   const u8* src, u64 size)
{
    stream_check_bound(stream, size);
    memcpy(stream->buf + stream->offset, src, size);
    stream->offset += size;
}

static inline void mut_stream_write_reverse_bytes(MutableStream* stream,
                                                  const u8* src, u64 size)
{
    stream_check_bound(stream, size);
    for (u64 i = stream->offset; i < stream->offset + size; ++i) {
        stream->buf[i] = src[size - (i - stream->offset) - 1];
    }
}

u64 mut_stream_seek(MutableStream* stream, i64 offset, StreamWhence whence)
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
