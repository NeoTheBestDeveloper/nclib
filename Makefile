test:
	meson test nclib: -C build

dev-mingw:
	meson setup --cross-file x86_64-w64-mingw32.txt --wipe build-mingw
	meson compile -j 8 -C build-mingw

test-mingw:
	meson test nclib: -C build-mingw
