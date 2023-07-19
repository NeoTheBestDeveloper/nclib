#pragma once

#include "typedefs.h"

typedef enum {
    STREAM_BIG_ENDIAN = 0,
    STREAM_LITTLE_ENDIAN = 1,
} StreamEndian;

typedef struct {
    u8* buf;
    u64 size;
    u64 offset;
    StreamEndian endian;
} Stream;

typedef enum {
    STREAM_OK = 0,
    STREAM_OUT_OF_RANGE_ERROR = 1,
} StreamResultStatus;

typedef struct {
    u64 readen;
    StreamResultStatus status;
} StreamResult;

Stream stream_new(void* buf, u64 buf_size, StreamEndian buf_endian);

StreamResult stream_read_u8(Stream* stream, u8* num);
StreamResult stream_read_i8(Stream* stream, i8* num);
