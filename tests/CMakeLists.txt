# -----------------------------------------------------------------------------
# Tests

add_executable(${PROJECT_NAME}_test)
target_include_directories(
  ${PROJECT_NAME}_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(
  ${PROJECT_NAME}_test PRIVATE Catch2::Catch2 ${PROJECT_NAME}::${PROJECT_NAME}
)

enable_warnings(${PROJECT_NAME}_test)
target_enable_coverage(${PROJECT_NAME}_test)
add_sanitizers(${PROJECT_NAME}_test)

target_sources(
  ${PROJECT_NAME}_test
  PRIVATE test.cpp test_helpers.cpp test_loop_fusion_compiletime.cpp
          test_loop_fusion_runtime.cpp test_loop_fusion_simple.cpp
)

# If FUSION_ENABLE_COVERAGE is set, generate a coverage report for this test.
generate_coverage_report(${PROJECT_NAME}_test)

# Let Catch2 find all tests so that CTest can list all tests instead of just one
# executable.
catch_discover_tests(${PROJECT_NAME}_test)
