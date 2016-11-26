cmake_minimum_required (VERSION 2.8.12.2)
project (pfclib)

enable_testing ()

option (BUILD_TESTS "Build tests" ON)
option (RUN_TESTS "Enable tests" ON)

if (RUN_TESTS)
  set (BUILD_TESTS ON)
endif ()

add_compile_options (-g)

add_subdirectory (lib)
add_subdirectory (src)
