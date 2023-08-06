#pragma once

#include "nclib/typedefs.h"
#include "stream_endian.h"
#include "stream_whence.h"

typedef struct MutableStream MutableStream;
typedef void (*MutableStream_read_bytes_type)(MutableStream*, u8*, u64);
typedef void (*MutableStream_write_bytes_type)(MutableStream*, const u8*, u64);

struct MutableStream {
    u8* buf;
    u64 size;
    u64 offset;

    MutableStream_read_bytes_type _stream_read_bytes_impl;
    MutableStream_write_bytes_type _stream_write_bytes_impl;
};

MutableStream mut_stream_new(u8* buf, u64 buf_size, StreamEndian endian);

// Create mutable stream with Big Endian buffer.
MutableStream mut_stream_new_be(u8* buf, u64 buf_size);

// Create mutable stream with Big Endian buffer.
MutableStream mut_stream_new_le(u8* buf, u64 buf_size);

u8 mut_stream_read_u8(MutableStream* stream);
i8 mut_stream_read_i8(MutableStream* stream);
u16 mut_stream_read_u16(MutableStream* stream);
i16 mut_stream_read_i16(MutableStream* stream);
u32 mut_stream_read_u32(MutableStream* stream);
i32 mut_stream_read_i32(MutableStream* stream);
u64 mut_stream_read_u64(MutableStream* stream);
i64 mut_stream_read_i64(MutableStream* stream);
f32 mut_stream_read_f32(MutableStream* stream);
f64 mut_stream_read_f64(MutableStream* stream);
bool mut_stream_read_bool(MutableStream* stream);
void mut_stream_read_bytes(MutableStream* stream, u8* buf, u64 size);

void mut_stream_write_u8(MutableStream* stream, u8 num);
void mut_stream_write_i8(MutableStream* stream, i8 num);
void mut_stream_write_u16(MutableStream* stream, u16 num);
void mut_stream_write_i16(MutableStream* stream, i16 num);
void mut_stream_write_u32(MutableStream* stream, u32 num);
void mut_stream_write_i32(MutableStream* stream, i32 num);
void mut_stream_write_u64(MutableStream* stream, u64 num);
void mut_stream_write_i64(MutableStream* stream, i64 num);
void mut_stream_write_f32(MutableStream* stream, f32 num);
void mut_stream_write_f64(MutableStream* stream, f64 num);
void mut_stream_write_bool(MutableStream* stream, bool flag);
void mut_stream_write_bytes(MutableStream* stream, const u8* bytes, u64 size);

u64 mut_stream_seek(MutableStream* stream, i64 offset, StreamWhence whence);

[[maybe_unused]] static inline u64 mut_stream_tell(const MutableStream* stream)
{
    return stream->offset;
}

[[maybe_unused]] static inline u64 mut_stream_size(const MutableStream* stream)
{
    return stream->size;
}

[[maybe_unused]] static inline const u8*
mut_stream_raw(const MutableStream* stream)
{
    return stream->buf;
}
