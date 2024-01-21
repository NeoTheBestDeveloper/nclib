#pragma once

#include "nclib/typedefs.h"
#include "stream_endian.h"
#include "stream_whence.h"

typedef struct Stream Stream;
typedef void (*StreamReadBytesFn)(Stream*, u8*, u64);

struct Stream {
    u8 const* _buf;
    u64 _size;
    u64 _offset;

    StreamReadBytesFn _read_bytes_impl;
};

Stream stream_new(u8 const* buf, u64 size, StreamEndian endian);
Stream stream_new_be(u8 const* buf, u64 size);
Stream stream_new_le(u8 const* buf, u64 size);

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

[[maybe_unused]] static inline u64 stream_tell(Stream const* stream)
{
    return stream->_offset;
}

[[maybe_unused]] static inline u64 stream_size(Stream const* stream)
{
    return stream->_size;
}

[[maybe_unused]] static inline u8 const* stream_raw(Stream const* stream)
{
    return stream->_buf;
}
