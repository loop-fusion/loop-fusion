# -----------------------------------------------------------------------------
# Benchmarks

add_executable(${PROJECT_NAME}_benchmarks)
target_link_libraries(
  ${PROJECT_NAME}_benchmarks PRIVATE Catch2::Catch2
                                     ${PROJECT_NAME}::${PROJECT_NAME}
)
target_sources(
  ${PROJECT_NAME}_benchmarks PRIVATE benchmarks.cpp benchmark_loop_fusion.cpp
                                     benchmark_loop_union.cpp
)
target_compile_definitions(
  ${PROJECT_NAME}_benchmarks PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING
)

enable_warnings(${PROJECT_NAME}_benchmarks)
add_sanitizers(${PROJECT_NAME}_benchmarks)
