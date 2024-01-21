#include <stdio.h>
#include <string.h>

#include "nclib/streams/_streams_check_bound.h"
#include "nclib/streams/stream.h"

/********************************************
 *              DEFINES START.              *
 ********************************************/

#define GEN_READ_METHOD_FOR(_type_)                                           \
    _type_ stream_read_##_type_(Stream* stream)                               \
    {                                                                         \
        _type_ buf;                                                           \
        stream->_read_bytes_impl(stream, (u8*)(&buf), sizeof buf);            \
        return buf;                                                           \
    }

/********************************************
 *              DEFINES END.                *
 ********************************************/

/****************************************************************
 *              PRIVATE METHODS SIGNATURE START.                *
 ****************************************************************/

static inline StreamReadBytesFn
_stream_find_read_bytes_impl(StreamEndian endian);
static void _stream_read_straight_bytes(Stream* stream, u8* dst, u64 size);
static void _stream_read_reverse_bytes(Stream* stream, u8* dst, u64 size);

static u64 _stream_new_offset_from_start(i64 offset, u64 stream_size);
static u64 _stream_new_offset_from_cur(i64 offset, u64 stream_size,
                                       u64 curr_offset);
static u64 _stream_new_offset_from_end(i64 offset, u64 stream_size);

/************************************************************
 *              PRIVATE METHODS SIGNATURE END.              *
 ************************************************************/

/****************************************************
 *              PUBLIC METHODS START.               *
 ****************************************************/

Stream stream_new(const u8* buf, u64 buf_size, StreamEndian endian)
{
    return (Stream) {
        ._buf = buf,
        ._size = buf_size,
        ._offset = 0,
        ._read_bytes_impl = _stream_find_read_bytes_impl(endian),
    };
}

Stream stream_new_be(const u8* buf, u64 buf_size)
{
    return stream_new(buf, buf_size, STREAM_BIG_ENDIAN);
}

Stream stream_new_le(const u8* buf, u64 buf_size)
{
    return stream_new(buf, buf_size, STREAM_LITTLE_ENDIAN);
}

void stream_read_bytes(Stream* stream, u8* bytes, u64 size)
{
    _stream_read_straight_bytes(stream, bytes, size);
}

u64 stream_seek(Stream* stream, i64 offset, StreamWhence whence)
{
    if (whence == STREAM_START) {
        stream->_offset = _stream_new_offset_from_start(offset, stream->_size);
    }
    else if (whence == STREAM_CURR) {
        stream->_offset = _stream_new_offset_from_cur(offset, stream->_size,
                                                      stream->_offset);
    }
    else {
        stream->_offset = _stream_new_offset_from_end(offset, stream->_size);
    }

    return stream->_offset;
}

GEN_READ_METHOD_FOR(u8)
GEN_READ_METHOD_FOR(i8)
GEN_READ_METHOD_FOR(u16)
GEN_READ_METHOD_FOR(i16)
GEN_READ_METHOD_FOR(u32)
GEN_READ_METHOD_FOR(i32)
GEN_READ_METHOD_FOR(u64)
GEN_READ_METHOD_FOR(i64)
GEN_READ_METHOD_FOR(f32)
GEN_READ_METHOD_FOR(f64)
GEN_READ_METHOD_FOR(bool)

/****************************************************
 *              PUBLIC METHODS END.                 *
 ****************************************************/

/****************************************************
 *              PRIVATE METHODS START.              *
 ****************************************************/

static inline StreamReadBytesFn
_stream_find_read_bytes_impl(StreamEndian endian)
{
    return endian == MACHINE_ENDIAN ? _stream_read_straight_bytes
                                    : _stream_read_reverse_bytes;
}

static void _stream_read_straight_bytes(Stream* stream, u8* dst, u64 size)
{
    STREAM_CHECK_BOUND(stream, size);

    memcpy(dst, stream->_buf + stream->_offset, size);
    stream->_offset += size;
}

static void _stream_read_reverse_bytes(Stream* stream, u8* dst, u64 size)
{
    STREAM_CHECK_BOUND(stream, size);

    for (u64 i = (u64)stream->_offset; i < (u64)(stream->_offset + size);
         ++i) {
        dst[size - (i - stream->_offset) - 1] = stream->_buf[i];
    }
    stream->_offset += size;
}

static u64 _stream_new_offset_from_start(i64 offset, u64 stream_size)
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

static u64 _stream_new_offset_from_cur(i64 offset, u64 stream_size,
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

static u64 _stream_new_offset_from_end(i64 offset, u64 stream_size)
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
