#pragma once

#include "typedefs.h"

typedef enum {
    STREAM_BIG_ENDIAN = 0,
    STREAM_LITTLE_ENDIAN = 1,
} StreamEndian;

typedef enum {
    STREAM_START = 0,
    STREAM_CURR = 1,
    STREAM_END = 2,
} StreamPosistion;

typedef enum {
    STREAM_OK = 0,
    STREAM_OUT_OF_RANGE_ERROR = 1,
} StreamResult;

typedef StreamResult (*Stream_read_bytes_type)(void*, u8*);
typedef StreamResult (*Stream_write_bytes_type)(void*, const u8*);

typedef struct {
    u8* buf;
    u64 size;
    u64 offset;

    Stream_read_bytes_type _stream_read_two_bytes_impl;
    Stream_read_bytes_type _stream_read_four_bytes_impl;
    Stream_read_bytes_type _stream_read_eight_bytes_impl;

    Stream_write_bytes_type _stream_write_two_bytes_impl;
    Stream_write_bytes_type _stream_write_four_bytes_impl;
    Stream_write_bytes_type _stream_write_eight_bytes_impl;
} Stream;

Stream stream_new(void* buf, u64 buf_size, StreamEndian buf_endian);

StreamResult stream_read_u8(Stream* stream, u8* num);
StreamResult stream_read_i8(Stream* stream, i8* num);
StreamResult stream_read_u16(Stream* stream, u16* num);
StreamResult stream_read_i16(Stream* stream, i16* num);
StreamResult stream_read_u32(Stream* stream, u32* num);
StreamResult stream_read_i32(Stream* stream, i32* num);
StreamResult stream_read_u64(Stream* stream, u64* num);
StreamResult stream_read_i64(Stream* stream, i64* num);
StreamResult stream_read_f32(Stream* stream, f32* num);
StreamResult stream_read_f64(Stream* stream, f64* num);
StreamResult stream_read_bool(Stream* stream, bool* flag);
StreamResult stream_read_void(Stream* stream, void* bytes, u64 size);

StreamResult stream_write_u8(Stream* stream, const u8* num);
StreamResult stream_write_i8(Stream* stream, const i8* num);
StreamResult stream_write_u16(Stream* stream, const u16* num);
StreamResult stream_write_i16(Stream* stream, const i16* num);
StreamResult stream_write_u32(Stream* stream, const u32* num);
StreamResult stream_write_i32(Stream* stream, const i32* num);
StreamResult stream_write_u64(Stream* stream, const u64* num);
StreamResult stream_write_i64(Stream* stream, const i64* num);
StreamResult stream_write_f32(Stream* stream, const f32* num);
StreamResult stream_write_f64(Stream* stream, const f64* num);
StreamResult stream_write_bool(Stream* stream, const bool* flag);
StreamResult stream_write_void(Stream* stream, const void* bytes, u64 size);

StreamResult stream_seek(Stream* stream, i64 offset, StreamPosistion whence);

u64 stream_pos(const Stream* stream);
u64 stream_size(const Stream* stream);
const u8* stream_raw(const Stream* stream);
