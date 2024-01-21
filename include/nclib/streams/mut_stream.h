#pragma once

#include "nclib/typedefs.h"
#include "stream_endian.h"
#include "stream_whence.h"

typedef struct MutStream MutStream;
typedef void (*MutStreamReadBytesFn)(MutStream*, u8*, u64);
typedef void (*MutStreamWriteBytesFn)(MutStream*, u8 const*, u64);

struct MutStream {
    u8* _buf;
    u64 _size;
    u64 _offset;

    MutStreamReadBytesFn _read_bytes_impl;
    MutStreamWriteBytesFn _write_bytes_impl;
};

MutStream mut_stream_new(u8* buf, u64 size, StreamEndian endian);
MutStream mut_stream_new_be(u8* buf, u64 size);
MutStream mut_stream_new_le(u8* buf, u64 size);

u8 mut_stream_read_u8(MutStream* stream);
i8 mut_stream_read_i8(MutStream* stream);
u16 mut_stream_read_u16(MutStream* stream);
i16 mut_stream_read_i16(MutStream* stream);
u32 mut_stream_read_u32(MutStream* stream);
i32 mut_stream_read_i32(MutStream* stream);
u64 mut_stream_read_u64(MutStream* stream);
i64 mut_stream_read_i64(MutStream* stream);
f32 mut_stream_read_f32(MutStream* stream);
f64 mut_stream_read_f64(MutStream* stream);
bool mut_stream_read_bool(MutStream* stream);
void mut_stream_read_bytes(MutStream* stream, u8* buf, u64 size);

void mut_stream_write_u8(MutStream* stream, u8 num);
void mut_stream_write_i8(MutStream* stream, i8 num);
void mut_stream_write_u16(MutStream* stream, u16 num);
void mut_stream_write_i16(MutStream* stream, i16 num);
void mut_stream_write_u32(MutStream* stream, u32 num);
void mut_stream_write_i32(MutStream* stream, i32 num);
void mut_stream_write_u64(MutStream* stream, u64 num);
void mut_stream_write_i64(MutStream* stream, i64 num);
void mut_stream_write_f32(MutStream* stream, f32 num);
void mut_stream_write_f64(MutStream* stream, f64 num);
void mut_stream_write_bool(MutStream* stream, bool flag);
void mut_stream_write_bytes(MutStream* stream, u8 const* buf, u64 size);

u64 mut_stream_seek(MutStream* stream, i64 offset, StreamWhence whence);

[[maybe_unused]] static inline u64 mut_stream_tell(MutStream const* stream)
{
    return stream->_offset;
}

[[maybe_unused]] static inline u64 mut_stream_size(MutStream const* stream)
{
    return stream->_size;
}

[[maybe_unused]] static inline u8 const*
mut_stream_raw(MutStream const* stream)
{
    return stream->_buf;
}
