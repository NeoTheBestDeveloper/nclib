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
} StreamWhence;

typedef struct Stream Stream;
typedef void (*Stream_read_bytes_type)(Stream*, u8*, u64);
typedef void (*Stream_write_bytes_type)(Stream*, const u8*, u64);

struct Stream {
    u8* buf;
    u64 size;
    u64 offset;

    Stream_read_bytes_type _stream_read_bytes_impl;
    Stream_write_bytes_type _stream_write_bytes_impl;
};

Stream stream_new(void* buf, u64 buf_size, StreamEndian endian);

Stream stream_new_be(void* buf, u64 buf_size);
Stream stream_new_le(void* buf, u64 buf_size);

u8 stream_read_u8(Stream* stream);
i8 stream_read_i8(Stream* stream);
u16 stream_read_u16(Stream* stream);
i16 stream_read_i16(Stream* stream);
u32 stream_read_u32(Stream* stream);
i32 stream_read_i32(Stream* stream);
u64 stream_read_u64(Stream* stream);
i64 stream_read_i64(Stream* stream);
f32 stream_read_f32(Stream* stream);
f64 stream_read_f64(Stream* stream);
bool stream_read_bool(Stream* stream);
void stream_read_bytes(Stream* stream, u8* buf, u64 size);

void stream_write_u8(Stream* stream, u8 num);
void stream_write_i8(Stream* stream, i8 num);
void stream_write_u16(Stream* stream, u16 num);
void stream_write_i16(Stream* stream, i16 num);
void stream_write_u32(Stream* stream, u32 num);
void stream_write_i32(Stream* stream, i32 num);
void stream_write_u64(Stream* stream, u64 num);
void stream_write_i64(Stream* stream, i64 num);
void stream_write_f32(Stream* stream, f32 num);
void stream_write_f64(Stream* stream, f64 num);
void stream_write_bool(Stream* stream, bool flag);
void stream_write_bytes(Stream* stream, const u8* bytes, u64 size);

u64 stream_seek(Stream* stream, i64 offset, StreamWhence whence);

[[maybe_unused]] static inline u64 stream_tell(const Stream* stream)
{
    return stream->offset;
}

[[maybe_unused]] static inline u64 stream_size(const Stream* stream)
{
    return stream->size;
}

[[maybe_unused]] static inline const u8* stream_raw(const Stream* stream)
{
    return stream->buf;
}
