# Streams

This module contains useful collecions like Stream and MutStream. These collections
able to parse binary data from array to some types. Main features is convenience and endian independence.
You can read little endiang binary data at big endian machine without any problems. Endian of machine setted 
with compilation options. Endian of binary data setted with Stream constructor. Working with streams look like 
reading and writing to file, but using types.

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
