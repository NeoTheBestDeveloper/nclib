setup:
	meson setup --wipe build

test:
	meson test nclib: -C build

test-mingw:
	meson setup --cross-file x86_64-w64-mingw32.txt --wipe build-mingw
	meson test nclib: -C build-mingw

clean:
	rm -rf build build-mingw build-sanitizer build-release .cache

release:
	meson setup --buildtype=release --wipe build-release
	meson compile -j 8 -C build-release
