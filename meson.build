project('nclib', 'c',
  version: '0.1.0',
  meson_version: '>=1.0.0',
  license: 'MIT',
  default_options: [
    'warning_level=everything',
    'c_std=c2x',
    'buildtype=debug',
    'optimization=2',
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

add_project_arguments(cc.get_supported_arguments('-DCHECK_BOUND'), language: 'c')

incdir = include_directories('include')
nclib = library('nclib', 
                nclib_src, 
                include_directories: incdir)


if get_option('buildtype') != 'release'
  libcriterion = subproject('libcriterion')
  subdir('tests')
endif
