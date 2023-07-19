#include "nclib/stream.h"

Stream stream_new(void* buf, u64 buf_size, StreamEndian buf_endian)
{
    return (Stream) {
        .buf = (u8*)buf,
        .size = buf_size,
        .offset = 0,
        .endian = buf_endian,
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
