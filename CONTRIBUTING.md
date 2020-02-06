# Contributing

## Project Requirements
This project uses many (modern) tools, so you may need to install quite a lot first.

You need to install:

 - [CMake](https://cmake.org/)
 - Modern Compiler that supports C++17 or later

Following tools are optional but recommended:
 - [Doxygen](http://www.doxygen.nl/)
 - [`clang-tidy`](https://clang.llvm.org/extra/clang-tidy/)
 - [`clang-format`](https://clang.llvm.org/docs/ClangFormat.html)
 - [`cmake-format`](https://github.com/cheshirekow/cmake_format)
 - [Cppcheck](http://cppcheck.sourceforge.net/)

## Code Conventions
Please refer to [`docs/CodeConventions.md`](./docs/CodeConventions.md)

### Install Requirements on Ubuntu
These instructions were tested on Ubuntu 18.04:

```sh
pip3 install cmake_format
sudo apt install doxygen doxygen-doc
sudo apt install shellcheck
sudo apt install gcc g++
sudo apt install clang clang++ clang-format clang-tidy
```

## CMake Targets
Following build targets are available:

```sh
make              # Build all targets
make coverage     # Create code coverage
make doxygen      # Create developer documentation
make test         # Run all tests (`ctest` works as well)
```

## Building

### Linux & macOS

```sh
cd 11-loop-fusion-i
mkdir build && cd $_
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

## Testing

```sh
# Test
cmake .. -DFUSION_BUILD_TEST:BOOL=ON
# Note: CMake >= 3.12 supports -jN, so we don't need to pass to `make`
cmake --build . -j 2
ctest .

# Create coverage
cmake -DFUSION_ENABLE_COVERAGE:BOOL=ON ..
cmake --build . -j 2 -- coverage
```

## Sanitizers
This project can be built using multiple sanitizers.
Just pass one of these options to CMake.

```sh
cmake -DSANITIZE_ADDRESS=ON   ..
cmake -DSANITIZE_THREAD=ON    ..
cmake -DSANITIZE_UNDEFINED=ON ..
CXX=clang++ CC=clang cmake -DSANITIZE_MEMORY=ON ..
```

The memory sanitizer is as of now (2019-12-27) only available for clang.

## Branching policy
Currently, all work is done on the `master` branch.
For each change that you want to submit by creating a new pull requests,
please create a new branch that has a meaningful name, e.g. `fix-memory-leak`
or `fix-nnn` where `nnn` is the id of an GitLab issue.

## Submitting code

Submitted code should follow these quality guidelines:

 - all GitLab CI jobs pass
 - code should adhere to project standards

If anything breaks, you'll get notified on each pull request.
