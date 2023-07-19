#pragma once

#include "typedefs.h"

typedef enum {
    STREAM_BIG_ENDIAN = 0,
    STREAM_LITTLE_ENDIAN = 1,
} StreamEndian;

typedef enum {
    STREAM_OK = 0,
    STREAM_OUT_OF_RANGE_ERROR = 1,
} StreamResultStatus;

typedef struct {
    u64 readen;
    StreamResultStatus status;
} StreamResult;

typedef StreamResult (*Stream_read_two_bytes_type)(void*, u8*);
typedef StreamResult (*Stream_read_four_bytes_type)(void*, u8*);

typedef struct {
    u8* buf;
    u64 size;
    u64 offset;

    Stream_read_two_bytes_type _stream_read_two_bytes_impl;
    Stream_read_four_bytes_type _stream_read_four_bytes_impl;
} Stream;

Stream stream_new(void* buf, u64 buf_size, StreamEndian buf_endian);

StreamResult stream_read_u8(Stream* stream, u8* num);
StreamResult stream_read_i8(Stream* stream, i8* num);

StreamResult stream_read_u16(Stream* stream, u16* num);
StreamResult stream_read_i16(Stream* stream, i16* num);

StreamResult stream_read_u32(Stream* stream, u32* num);
StreamResult stream_read_i32(Stream* stream, i32* num);
