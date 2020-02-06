# -----------------------------------------------------------------------------
# Example Executable
#
# This example shows how to use loop_fusion with CMake. loop_fusion has to be
# installed on your system in order for this to work.
#
# See `examples/README.md` for instructions on how to compile this project.
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(
  loop_example_compiletime_basic
  VERSION 0.0.1
  DESCRIPTION "Example for C++ loop fusion"
  LANGUAGES CXX
)

# Dependencies
find_package(loop_fusion REQUIRED)

# Main executable
add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE loop_fusion::loop_fusion)
target_compile_options(
  ${PROJECT_NAME}
  PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall>
          $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wextra>
          $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-pedantic>
          $<$<CXX_COMPILER_ID:MSVC>:/W3>
)

target_sources(${PROJECT_NAME} PRIVATE loops.cpp)
