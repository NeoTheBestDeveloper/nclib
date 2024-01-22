project('nclib', 'c',
  version: '0.2.0',
  meson_version: '>=1.0.0',
  license: 'MIT',
  default_options: [
    'warning_level=everything',
    'c_std=c2x',
    'buildtype=debug',
    'optimization=0',
    'b_lundef=false',
    'strip=true',
    'default_library=both',
])

subdir('src')

cc = meson.get_compiler('c')
warning_flags = [
  '-Wfloat-equal',
  '-Wundef',
  '-Wshadow',
  '-Wpointer-arith',
  '-Wcast-align',
  '-Wwrite-strings',
  '-Wcast-qual',
  '-Wswitch-default',
  '-Wswitch-enum',
  '-Wconversion',
  '-Wformat=2',
  '-Wpedantic',
  '-Wformat-security',
  '-Wtype-limits',
  '-Wunused-result',
  '-Wvariadic-macros',
  '-Wno-format-extra-args',
  '-Wno-missing-prototypes',
  '-Wno-padded',
  '-Wno-missing-declarations',
  '-Wno-declaration-after-statement',
  '-Wno-suggest-attribute=const',
  '-Wno-c++-compat',
  '-Wno-extra-semi-stmt',
  '-Wno-c++98-compat'
]

add_project_arguments(cc.get_supported_arguments(warning_flags), language: 'c')

# Stream use information about endian.
if target_machine.endian() == 'big'
  add_project_arguments(cc.get_supported_arguments('-DMACHINE_ENDIAN=0'), language: 'c')
elif target_machine.endian() == 'little'
  add_project_arguments(cc.get_supported_arguments('-DMACHINE_ENDIAN=1'), language: 'c')
else 
  error('Unknown endian!')
endif


incdir = include_directories('include')
libnclib = both_libraries('nclib', 
                nclib_src, 
                include_directories: incdir
)

nclib = declare_dependency(
	include_directories: [incdir],
	link_with: [libnclib]
)

if get_option('buildtype') != 'release'
  libcriterion = subproject('libcriterion')
  subdir('tests')
endif
