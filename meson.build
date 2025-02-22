project(
  'libpkt',
  'c',
  version : '1.0.0',
)
project_description = 'High-performance C library designed for efficient handling and manipulation of .pkt files, a custom binary file format intended for lightweight packet storage.'

project_headers = [
    'include/libpkt.h'
]

project_source_files = [
    'src/libpkt.c'
]

# project_test_files = [
#     'tests/test_libpkt.c'
# ]

build_args = [
]


public_headers = include_directories('include')
build_args += [
  '-DPROJECT_NAME=' + meson.project_name(),
  '-DPROJECT_VERSION=' + meson.project_version(),
]

# if target_machine.system() == 'windows' or target_machine.system() == 'cygwin'
#   build_args += '-DLIBPKT_PUBLIC="__declspec(dllexport)"'
# else
#   build_args += '-DLIBPKT_PUBLIC=__attribute__((visibility("default")))'
# endif


project_target = shared_library(
  meson.project_name(),
  project_source_files,
  install : true,
  c_args : build_args,
  gnu_symbol_visibility : 'default',
  include_directories : public_headers,
)

project_dep = declare_dependency(
  include_directories: public_headers,
  link_with : project_target
)
set_variable(meson.project_name() + '_dep', project_dep)

install_headers(project_headers, subdir : meson.project_name())

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : meson.project_name(),
  libraries : project_target,
)