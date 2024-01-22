# General documentation

## Headers

- "nclib.h" contains all library.
- "nclib/panic.h" contains panic function.
- "nclib/typedefs.h" contains better c types.
- "nclib/streams/streams.h" contains all [streams](./streams.md) logic.

## Compilation options

### MACHINE_ENDIAN

Set endian for target machine which will execute this library. 

Values:
"0" - big endian.
"1" - little endian.
default: none


### CHECK_BOUND

Pass it if you want check bound of library collections. For example, if you want 
access to stream index 5, but it size 3 with this option library raise panic and tell
you what was wrong instead of some "segmentation fold" error.

default: none
