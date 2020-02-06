# CMake >= 3.12 has some new features like `HOMEPAGE_URL`. It also supports
# C++20 using `cxx_std_20` in `target_compile_features`.
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(
  "loop_fusion"
  VERSION 0.1.0
  DESCRIPTION "C++ Loop Fusion"
  HOMEPAGE_URL "https://github.com/loop-fusion/loop-fusion"
  LANGUAGES CXX
)

message("=> Project: ${PROJECT_NAME}")

# -----------------------------------------------------------------------------
# Project configuration options. Sanitizer options are defined in the
# correspondig FindXX modules.
# cmake-format: off
option(FUSION_ENABLE_COVERAGE   "Add coverage information to binaries."        OFF)
option(FUSION_BUILD_TEST        "Build loop_fusion Catch2 tests"                ON)
# cmake-format: on

# -----------------------------------------------------------------------------
# CMake modules and other cmake related (third-party) scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(MiscFunctions)
check_no_in_source_build()
set_default_build_type("RelWithDebInfo")

find_package(Sanitizers) # for Thread/Address/Memory/UB Sanitizers
include(GNUInstallDirs) # For ${CMAKE_INSTALL_<DIR>} variables that are
                        # standarized
include(CMakePackageConfigHelpers)

include(coverage) # for lcov and gcov code coverage
activate_coverage(FUSION_ENABLE_COVERAGE)

# -----------------------------------------------------------------------------
# Loop Fusion Header Only Library

add_library(${PROJECT_NAME} INTERFACE)
# add alias so the project can be uses with add_subdirectory
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# -----------------------------------------------------------------------------

add_subdirectory(third_party)
add_subdirectory(docs)

if(FUSION_BUILD_TEST)
  include(CTest)
  include(Catch)
  # Per CMake documentation, enable_testing() must be called in the root
  # directory.
  enable_testing()
  add_subdirectory(tests)
  add_subdirectory(benchmarks)
endif()

# -----------------------------------------------------------------------------
# Installation (locations are provided by GNUInstallDirs) See:
# https://dominikberner.ch/cmake-interface-lib/

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}_Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" INSTALL_DESTINATION
  ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(
  EXPORT ${PROJECT_NAME}_Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.txt")
