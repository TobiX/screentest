project(
  'screentest',
  'c',
  license: 'GPL-2.0-only',
  license_files: ['COPYING'],
  meson_version: '>= 1.1',
  version: '2.2-dev',
)

i18n = import('i18n')

gmodule_dep = dependency('gmodule-2.0', version: '>= 1.1.3')
gtk_dep = dependency('gtk+-2.0', version: '>= 2.24')
screentest_deps = [
  gmodule_dep,
  gtk_dep,
]

prefix = get_option('prefix')
datadir = join_paths(prefix, get_option('datadir'))
localdir = join_paths(datadir, 'local')

config_h = configuration_data()
config_h.set_quoted('DATADIR', datadir)
config_h.set_quoted('LOCALEDIR', localdir)
config_h.set_quoted('PACKAGE', meson.project_name())
config_h.set_quoted('PACKAGE_NAME', meson.project_name())
config_h.set_quoted('VERSION', meson.project_version())

configure_file(
  configuration: config_h,
  output: 'config.h',
)

subdir('po')

screentest_srcs = [
  'callbacks.c',
  'test_basic.c',
  'test_blink.c',
  'test_bright_pixels.c',
  'test_grid.c',
  'test_horizontal.c',
  'test_lcdalign.c',
  'test_text.c',
  'test_vertical.c',
]

executable(
  meson.project_name(),
  'main.c',
  dependencies: screentest_deps,
  install: true,
  sources: screentest_srcs,
)

uidir = datadir / meson.project_name()
install_data(
  'screentest.ui',
  install_dir: uidir,
)