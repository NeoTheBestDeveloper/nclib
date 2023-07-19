#include "nclib/stream.h"

// PRIVATE METHODS
static inline StreamResult stream_read_straight_bytes(Stream* stream, u8* dst,
                                                      u64 size);
static inline StreamResult stream_read_reverse_bytes(Stream* stream, u8* dst,
                                                     u64 size);

static Stream_read_two_bytes_type
stream_find_read_two_bytes_impl(StreamEndian buf_endian);
static StreamResult stream_straight_read_two_bytes(void* stream, u8* num);
static StreamResult stream_reverse_read_two_bytes(void* stream, u8* num);

static Stream_read_four_bytes_type
stream_find_read_four_bytes_impl(StreamEndian buf_endian);
static StreamResult stream_straight_read_four_bytes(void* stream, u8* num);
static StreamResult stream_reverse_read_four_bytes(void* stream, u8* num);

Stream stream_new(void* buf, u64 buf_size, StreamEndian buf_endian)
{
    return (Stream) {
        .buf = (u8*)buf,
        .size = buf_size,
        .offset = 0,

        ._stream_read_two_bytes_impl
        = stream_find_read_two_bytes_impl(buf_endian),
        ._stream_read_four_bytes_impl
        = stream_find_read_four_bytes_impl(buf_endian),
    };
}

StreamResult stream_read_u8(Stream* stream, u8* num)
{
    if (stream->offset + sizeof *num > stream->size) {
        return (StreamResult) {
            .readen = 0,
            .status = STREAM_OUT_OF_RANGE_ERROR,
        };
    }

    *num = stream->buf[stream->offset];
    stream->offset += 1;
    return (StreamResult) { .readen = 1, .status = STREAM_OK };
}

StreamResult stream_read_i8(Stream* stream, i8* num)
{
    if (stream->offset + sizeof *num > stream->size) {
        return (StreamResult) {
            .readen = 0,
            .status = STREAM_OUT_OF_RANGE_ERROR,
        };
    }

    *num = (i8)stream->buf[stream->offset];
    stream->offset += 1;
    return (StreamResult) { .readen = 1, .status = STREAM_OK };
}

StreamResult stream_read_u16(Stream* stream, u16* num)
{
    if (stream->offset + sizeof *num > stream->size) {
        return (StreamResult) {
            .readen = 0,
            .status = STREAM_OUT_OF_RANGE_ERROR,
        };
    }
    return stream->_stream_read_two_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_i16(Stream* stream, i16* num)
{
    if (stream->offset + sizeof *num > stream->size) {
        return (StreamResult) {
            .readen = 0,
            .status = STREAM_OUT_OF_RANGE_ERROR,
        };
    }
    return stream->_stream_read_two_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_u32(Stream* stream, u32* num)
{
    if (stream->offset + sizeof *num > stream->size) {
        return (StreamResult) {
            .readen = 0,
            .status = STREAM_OUT_OF_RANGE_ERROR,
        };
    }
    return stream->_stream_read_four_bytes_impl(stream, (u8*)num);
}

StreamResult stream_read_i32(Stream* stream, i32* num)
{
    if (stream->offset + sizeof *num > stream->size) {
        return (StreamResult) {
            .readen = 0,
            .status = STREAM_OUT_OF_RANGE_ERROR,
        };
    }
    return stream->_stream_read_four_bytes_impl(stream, (u8*)num);
}

static Stream_read_two_bytes_type
stream_find_read_two_bytes_impl(StreamEndian buf_endian)
{
    if (buf_endian == MACHINE_ENDIAN) {
        return stream_straight_read_two_bytes;
    }
    return stream_reverse_read_two_bytes;
}

static StreamResult stream_straight_read_two_bytes(void* stream, u8* num)
{
    return stream_read_straight_bytes(stream, num, 2);
}

static StreamResult stream_reverse_read_two_bytes(void* stream, u8* num)
{
    return stream_read_reverse_bytes(stream, num, 2);
}

static inline StreamResult stream_read_straight_bytes(Stream* stream, u8* dst,
                                                      u64 size)
{
    for (u64 i = stream->offset; i < stream->offset + size; ++i) {
        dst[i - stream->offset] = stream->buf[i];
    }
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

static Stream_read_four_bytes_type
stream_find_read_four_bytes_impl(StreamEndian buf_endian)
{
    if (buf_endian == MACHINE_ENDIAN) {
        return stream_straight_read_four_bytes;
    }
    return stream_reverse_read_four_bytes;
}

static StreamResult stream_straight_read_four_bytes(void* stream, u8* num)
{
    return stream_read_straight_bytes(stream, num, 4);
}

static StreamResult stream_reverse_read_four_bytes(void* stream, u8* num)
{
    return stream_read_reverse_bytes(stream, num, 4);
}
