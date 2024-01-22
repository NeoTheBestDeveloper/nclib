# Library for me which implement base data structures in C. 

## Goals

- [x] Better type aliases
- [x] Stream
- [x] Panic
- [ ] Arena Allocator
- [ ] Str
- [ ] Hash Table
- [ ] Darray with "templates"

## Docs

If you want read documention read this [docs](./docs/README.md).

## How to build 

Before install meson.

```sh
git clone https://github.com/NeoTheBestDeveloper/nclib
cd nclib

meson setup --buildtype=release build-release
meson compile -j 8 -C build-release
```
