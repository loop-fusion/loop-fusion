# Set the default build type if not set by the user.
function(set_default_build_type buildType)
  if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(
      STATUS "Setting build type to '${buildType}' as none was specified."
    )
    set(CMAKE_BUILD_TYPE
        "${buildType}"
        CACHE STRING "Choose the type of build." FORCE
    )
    # Set the possible values of build type for cmake-gui
    set_property(
      CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel"
                                      "RelWithDebInfo"
    )
  endif()
endfunction()

function(check_no_in_source_build)
  if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(
      FATAL_ERROR
        "Building in-source is not supported! Create a build dir and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt"
    )
  endif()
endfunction()

function(enable_warnings warning_target)
  if(NOT TARGET ${warning_target})
    message(WARNING "loop-fusion warnings: ${warning_target} is not a target.")
    return()
  endif()

  message(
    DEBUG
    "Enable ${CMAKE_CXX_COMPILER_ID} compiler warnings for target ${warning_target}"
  )

  if(MSVC)
    target_compile_options(${warning_target} PRIVATE /W3)

  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(
      ${warning_target}
      PRIVATE -Wall
              -Wextra
              -pedantic
              # Warnings that are not enabled but -Wall/-Wextra See
              # https://kristerw.blogspot.com/2017/09/useful-gcc-warning-
              # options-not-enabled.html
              -Wunknown-pragmas
              -Wundef
              -Wold-style-cast # warn for c-style casts (e.g. `(int) 3.0`)
              -Wuseless-cast
              -Wdisabled-optimization
              -Wstrict-overflow=4
              -Winit-self
              -Wpointer-arith
              -Wduplicated-cond
              -Wdouble-promotion
              -Wshadow # warn the user if a variable declaration shadows one
                       # from a parent context
              -Wduplicated-branches
              -Wrestrict
              -Wnull-dereference # warn if a null dereference is detected
              -Wlogical-op
              -Wunsafe-loop-optimizations
              -Wno-error=unsafe-loop-optimizations
              -Wformat=2
              -Wmissing-field-initializers
    )

  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang|AppleClang")
    target_compile_options(
      ${warning_target}
      PRIVATE -Wall -Wextra -pedantic -Wdocumentation # Warns about doxygen
              # variable name mismatches, etc.
    )

  else()
    message(
      WARNING "Unknown compiler '${CMAKE_CXX_COMPILER_ID}'. It may not work."
    )
    target_compile_options(${warning_target} PRIVATE -Wall -Wextra -pedantic)

  endif()
endfunction()
