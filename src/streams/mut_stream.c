#include <string.h>

#include "nclib/streams/_streams_check_bound.h"
#include "nclib/streams/mutable_stream.h"

/********************************************
 *              DEFINES START.              *
 ********************************************/

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

/********************************************
 *              DEFINES END.                *
 ********************************************/

/****************************************************************
 *              PRIVATE METHODS SIGNATURE START.                *
 ****************************************************************/

static inline mut_stream_read_bytes_fn
_mut_stream_find_read_bytes_impl(StreamEndian endian);
static void _mut_stream_read_straight_bytes(MutableStream* stream, u8* dst,
                                            u64 size);
static void _mut_stream_read_reverse_bytes(MutableStream* stream, u8* dst,
                                           u64 size);

static inline mut_stream_write_bytes_fn
_mut_stream_find_write_bytes_impl(StreamEndian endian);
static void _mut_stream_write_straight_bytes(MutableStream* stream,
                                             const u8* src, u64 size);
static void _mut_stream_write_reverse_bytes(MutableStream* stream,
                                            const u8* src, u64 size);

static u64 _mut_stream_new_offset_from_start(i64 offset, u64 stream_size);
static u64 _mut_stream_new_offset_from_cur(i64 offset, u64 stream_size,
                                           u64 curr_offset);
static u64 _mut_stream_new_offset_from_end(i64 offset, u64 stream_size);
/************************************************************
 *              PRIVATE METHODS SIGNATURE END.              *
 ************************************************************/

/****************************************************
 *              PUBLIC METHODS START.               *
 ****************************************************/

MutableStream mut_stream_new(u8* buf, u64 buf_size, StreamEndian endian)
{
    return (MutableStream) {
        ._buf = buf,
        ._size = buf_size,
        ._offset = 0,

        ._stream_read_bytes_impl = _mut_stream_find_read_bytes_impl(endian),
        ._stream_write_bytes_impl = _mut_stream_find_write_bytes_impl(endian),
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

void mut_stream_read_bytes(MutableStream* stream, u8* bytes, u64 size)
{
    _mut_stream_read_straight_bytes(stream, bytes, size);
}

void mut_stream_write_bytes(MutableStream* stream, const u8* bytes, u64 size)
{
    _mut_stream_write_straight_bytes(stream, bytes, size);
}

u64 mut_stream_seek(MutableStream* stream, i64 offset, StreamWhence whence)
{
    if (whence == STREAM_START) {
        stream->_offset
            = _mut_stream_new_offset_from_start(offset, stream->_size);
    }
    else if (whence == STREAM_CURR) {
        stream->_offset = _mut_stream_new_offset_from_cur(
            offset, stream->_size, stream->_offset);
    }
    else {
        stream->_offset
            = _mut_stream_new_offset_from_end(offset, stream->_size);
    }

    return stream->_offset;
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

/****************************************************
 *              PUBLIC METHODS END.                 *
 ****************************************************/

/****************************************************
 *              PRIVATE METHODS START.              *
 ****************************************************/

static inline mut_stream_read_bytes_fn
_mut_stream_find_read_bytes_impl(StreamEndian endian)
{
    return endian == MACHINE_ENDIAN ? _mut_stream_read_straight_bytes
                                    : _mut_stream_read_reverse_bytes;
}

static void _mut_stream_read_straight_bytes(MutableStream* stream, u8* dst,
                                            u64 size)
{
    STREAM_CHECK_BOUND(stream, size);

    memcpy(dst, stream->_buf + stream->_offset, size);
    stream->_offset += size;
}

static void _mut_stream_read_reverse_bytes(MutableStream* stream, u8* dst,
                                           u64 size)
{
    STREAM_CHECK_BOUND(stream, size);

    for (u64 i = stream->_offset; i < stream->_offset + size; ++i) {
        dst[size - (i - stream->_offset) - 1] = stream->_buf[i];
    }
    stream->_offset += size;
}

static inline mut_stream_write_bytes_fn
_mut_stream_find_write_bytes_impl(StreamEndian endian)
{
    return endian == MACHINE_ENDIAN ? _mut_stream_write_straight_bytes
                                    : _mut_stream_write_reverse_bytes;
}

static void _mut_stream_write_straight_bytes(MutableStream* stream,
                                             const u8* src, u64 size)
{
    STREAM_CHECK_BOUND(stream, size);

    memcpy(stream->_buf + stream->_offset, src, size);
    stream->_offset += size;
}

static void _mut_stream_write_reverse_bytes(MutableStream* stream,
                                            const u8* src, u64 size)
{
    STREAM_CHECK_BOUND(stream, size);

    for (u64 i = stream->_offset; i < stream->_offset + size; ++i) {
        stream->_buf[i] = src[size - (i - stream->_offset) - 1];
    }
}

static u64 _mut_stream_new_offset_from_start(i64 offset, u64 stream_size)
{
    bool offset_negative = offset < 0;

    if (offset_negative) {
        return 0;
    }

    if ((u64)offset > stream_size) {
        return stream_size;
    }

    return (u64)offset;
}

static u64 _mut_stream_new_offset_from_cur(i64 offset, u64 stream_size,
                                           u64 curr_offset)
{
    bool offset_negative = offset < 0;

    if (offset_negative) {
        u64 offset_value = (u64)-offset;

        if (offset_value > curr_offset) {
            return 0;
        }

        return curr_offset - offset_value;
    }

    u64 offset_value = (u64)offset;

    if (offset_value + curr_offset > stream_size) {
        return stream_size;
    }

    return curr_offset + offset_value;
}

static u64 _mut_stream_new_offset_from_end(i64 offset, u64 stream_size)
{
    bool offset_negative = offset < 0;

    if (offset_negative) {
        return stream_size;
    }

    if ((u64)offset > stream_size) {
        return 0;
    }

    return stream_size - (u64)offset;
}

/****************************************************
 *              PRIVATE METHODS END.                *
 ****************************************************/
