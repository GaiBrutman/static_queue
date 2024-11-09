project(
  'static_queue',
  'c',
  version: '0.1.0',
  meson_version: '>=1.1.0',
  default_options: ['buildtype=release', 'warning_level=3'],
  license: 'MIT license',
)

project_headers = include_directories('include')
project_source_files = [
  'src/static_queue.c',
]

project_target = library(
  meson.project_name(),
  project_source_files,
  include_directories: project_headers,
  install: true,
)


# Make objects usable as a static library.
project_dep = declare_dependency(
  include_directories: project_headers,
  objects: project_target.extract_objects(source_files),
)

if not meson.is_subproject()
  subdir('tests')

  test(
    'test_sanity',
    executable(
      'test_sanity',
      ['tests/test_sanity.c'],
      dependencies: [project_dep, test_dep],
      install: false,
    ),
  )
endif