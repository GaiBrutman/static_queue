# Static Queue

An efficient , statically allocated queue

## Structure

```txt
src/                    All .c files
include/                All .h files. Used as "#include static_queue/header.h"
    static_queue/
tests/                  All test files
build/                  Build directory (gitignored)
conanfile.py            Conan package file
meson.build             Meson build file
requirements.txt        Python requirements
.gitignore              Git ignore file
README.md               This file
.clang-format           Clang formatting options
.pre-commit-config.yaml Pre-commit configuration
```

## Dependencies

- [meson](https://mesonbuild.com/) for building.
- [ninja](https://ninja-build.org/) for building.
- [conan](https://conan.io/) for package management.
- [Unity](https://www.throwtheswitch.org/unity) for testing.

## Usage

### Getting started

Set up a virtual environment.

```bash
python3 -m venv venv
. venv/bin/activate
```

Install pre-commit hooks.

```bash
pre-commit install
```

### Building

```bash
conan build .
```

### Testing

The Unity test framework is used for unit testing.

```bash
conan test . static_queue/latest
```

### VSCode Integration

Open static_queue.code-workspace in VSCode
