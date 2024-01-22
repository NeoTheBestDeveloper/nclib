# Streams

This module contains useful collecions like Stream and MutStream. These collections
able to parse binary data from array to some types. Main features is convenience and endian independence.
You can read little endiang binary data at big endian machine without any problems. Endian of machine setted 
with compilation options. Endian of binary data setted with Stream constructor. Working with streams look like 
reading and writing to file, but using types.

## Constants 

- StreamWhence::STREAM_START - mean start of stream.
- StreamWhence::STREAM_CURR - mean current position of stream.
- StreamWhence::STREAM_END - mean last index of stream.

- StreamEndian::STREAM_BIG_ENDIAN - for big endian.
- StreamEndian::STREAM_LITTLE_ENDIAN - for little endian.

## Stream methods.

Constructors:
```c
Stream stream_new(u8 const* buf, u64 size, StreamEndian endian); // Create Stream with selected endian.
Stream stream_new_be(u8 const* buf, u64 size); // Create Stream with big endian.
Stream stream_new_le(u8 const* buf, u64 size); // Create Stream with little endian.
```

Methods for reading base types:
```c 
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

```
Getters:
```c 
u64 stream_tell(Stream const* stream); // Tell current position inside stream.
u64 stream_size(Stream const* stream); // Return size of stream data.
u8 const* stream_raw(Stream const* stream); // Return const pointer to stream data.
```

Other:
```c 
u64 stream_seek(Stream* stream, i64 offset, StreamWhence whence); // Work like seek for files.
```

Examples:
```c
u8 data[4] = {0, 1, 25, 36};
Stream stream = stream_new_le(data, 4);

bool first_flag = stream_read_bool(&stream); // first_flag=false
bool second_flag = stream_read_bool(&stream); // second_flag=true
u8 first_number = stream_read_u8(&stream); // first_number=25 

stream_seek(&stream, 0, STREAM_START); // Start reading from start.
first_flag = stream_read_bool(&stream); // first_flag=false

stream_seek(&stream, 1, STREAM_END); // Set offset to last byte.
u8 second_number = stream_read_u8(&stream); // first_number=25 
```

## MutStream methods.

Constructors:
```c 
MutStream mut_stream_new(u8* buf, u64 size, StreamEndian endian); // Create endian with selected endian.
MutStream mut_stream_new_be(u8* buf, u64 size); // Create big endian stream.
MutStream mut_stream_new_le(u8* buf, u64 size); // Create little endian stream.
```

Methods for reading base types:
```c 
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
```

Methods for writing base types:
```c 
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
```

Getters:
```c 
u64 mut_stream_tell(MutStream const* stream); // Tell current position inside stream.
u64 mut_stream_size(MutStream const* stream); // Return size of stream data.
u8 const* mut_stream_raw(MutStream const* stream); // Return const pointer to stream data.
```

Other:
```c
u64 mut_stream_seek(MutStream* stream, i64 offset, StreamWhence whence); // Change current position of MutStream. It works like lseek.
```

Examples:
```c
u8 data[4] = {0, 1, 25, 36};
MutStream stream = mut_stream_new_le(data, 4);

bool first_flag = mut_stream_read_bool(&stream); // first_flag=false
bool second_flag = mut_stream_read_bool(&stream); // second_flag=true

mut_stream_write_u8(&stream, 46); // Rewrite 25 to 46.
mut_stream_seek(&stream, -1, STREAM_CURR); // Step backward.
u8 first_number = mut_stream_read_u8(&stream); // first_number=46.
```

## How to read/write my own type.

### For Stream
```c 
typedef struct {
    i32 page_id;
    i16 offset;
} Addr;

Addr stream_read_addr(Stream* stream)
{
    Addr res;
    stream->_read_bytes_impl(stream, (u8*)(&res.page_id), sizeof res.page_id);
    stream->_read_bytes_impl(stream, (u8*)(&res.offset), sizeof res.offset);
    return res;
}


// Addr {.page_id = 100, .offset=234}
u8 le_addr[] = { 0x64, 0x0, 0x0, 0x0, 0xea, 0x0 };
u8 be_addr[] = { 0x0, 0x0, 0x0, 0x64, 0x0, 0xea };

Stream be_stream = stream_new_be(be_addr, 6);
Addr addr = stream_read_addr(&be_stream); // addr = {.page_id = 100, .offset=234}

Stream le_stream = stream_new_le(le_addr, 6);
addr = stream_read_addr(&le_stream); // addr = {.page_id = 100, .offset=234}
```
### For MutStream 
```c
typedef struct {
    i32 page_id;
    i16 offset;
} Addr;

Addr mut_stream_read_addr(MutStream* stream)
{
    Addr res;
    stream->_read_bytes_impl(stream, (u8*)(&res.page_id), sizeof res.page_id);
    stream->_read_bytes_impl(stream, (u8*)(&res.offset), sizeof res.offset);
    return res;
}

void mut_stream_write_addr(MutStream* stream, Addr addr)
{
    stream->_write_bytes_impl(stream, (u8*)(&addr.page_id),
                              sizeof addr.page_id);
    stream->_write_bytes_impl(stream, (u8*)(&addr.offset), sizeof addr.offset);
}

// Addr {.page_id = 100, .offset=234}
u8 le_addr[] = { 0x64, 0x0, 0x0, 0x0, 0xea, 0x0 };
u8 be_addr[] = { 0x0, 0x0, 0x0, 0x64, 0x0, 0xea };

MutStream be_stream = mut_stream_new_be(be_addr, 6);
Addr addr = mut_stream_read_addr(&be_stream); // addr = {.page_id = 100, .offset=234}

MutStream le_stream = mut_stream_new_le(le_addr, 6);
addr = mut_stream_read_addr(&le_stream); // addr = {.page_id = 100, .offset=234}

u8 buf[sizeof le_addr];
MutStream stream = mut_stream_new(buf, sizeof buf, STREAM_LITTLE_ENDIAN);
mut_stream_write_addr(&stream, addr); // buf arr will the same like a le_addr arr
```
