#pragma once

#include "nclib/typedefs.h"
#include "stream_endian.h"
#include "stream_whence.h"

typedef struct Stream Stream;
typedef void (*Stream_read_bytes_type)(Stream*, u8*, u64);

struct Stream {
    const u8* buf;
    u64 size;
    u64 offset;

    Stream_read_bytes_type _stream_read_bytes_impl;
};

Stream stream_new(const u8* buf, u64 buf_size, StreamEndian endian);

// Create stream with Big Endian buffer.
Stream stream_new_be(const u8* buf, u64 buf_size);

// Create stream with Little Endian buffer.
Stream stream_new_le(const u8* buf, u64 buf_size);

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
