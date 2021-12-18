add_executable(bench-svec 
    bench_svec.cpp)
target_link_libraries(bench-svec
  PRIVATE
    doux::core
    benchmark::benchmark)

# Disable clang-tidy and cppcheck for benchmarks
set_target_properties(bench-svec
  PROPERTIES
    CXX_CLANG_TIDY ""
    CXX_CPPCHECK "")

# --------------------------------------------------------------------

add_executable(bench-cons 
    bench_constraint.cpp)
target_link_libraries(bench-cons
  PRIVATE
    doux::core
    doux::pd
    benchmark::benchmark)

# Disable clang-tidy and cppcheck for benchmarks
set_target_properties(bench-cons
  PROPERTIES
    CXX_CLANG_TIDY ""
    CXX_CPPCHECK "")