project(
  'static_queue',
  'c',
  version: '0.3.1',
  meson_version: '>=1.1.0',
  default_options: ['buildtype=release', 'warning_level=3'],
  license: 'MIT license',
)

project_headers = include_directories('include')
project_source_files = [
  'src/static_queue.c',
  'src/ring_buffer.c',
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
  objects: project_target.extract_objects(project_source_files),
)

if not meson.is_subproject()
  subdir('tests')

  test(
    'test_ring_buffer',
    executable(
      'test_ring_buffer',
      ['tests/test_ring_buffer.c'],
      dependencies: [project_dep, test_dep],
      install: false,
    ),
  )

  test(
    'test_static_queue',
    executable(
      'test_static_queue',
      ['tests/test_static_queue.c'],
      dependencies: [project_dep, test_dep],
      install: false,
    ),
  )
endif