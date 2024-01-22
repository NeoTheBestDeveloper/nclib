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

default: meson choose endian of your machine.

Examples:
```sh
meson setup --buildtype=release build-release -Dc_args="-DMACHINE_ENDIAN=1" # For little endian
# OR
meson setup --buildtype=release build-release -Dc_args="-DMACHINE_ENDIAN=0" # For big endian
```

### CHECK_BOUND

Pass it if you want check bound of library collections. For example, if you want 
access to stream index 5, but it size 3 with this option library raise panic and tell
you what was wrong instead of some "segmentation fold" error.

default: don't check bound.

Examples:
```sh
meson setup --buildtype=release build-release -Dc_args="-DCHECK_BOUND" # For checking collections bound.
# OR
meson setup --buildtype=release build-release # Don't check collections bound.
