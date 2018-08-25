cmake_minimum_required (VERSION 2.8.12.2)
project (pfclib)

enable_testing ()

option (BUILD_TESTS "Build tests" ON)
option (RUN_TESTS "Enable tests" ON)
option (RUN_VALGRIND "Enable valgrind of tests" ON)

if (RUN_TESTS)
  set (BUILD_TESTS ON)
  if (RUN_VALGRIND)
    set (VALGRIND_COMMAND valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=./valgrind.suppress )
  else (RUN_VALGRIND)
    set (VALGRIND_COMMAND "")
  endif (RUN_VALGRIND)
endif (RUN_TESTS)

add_compile_options (-g)

set(CMAKE_C_FLAGS_DEBUG "-Wall -Werror -Wno-pointer-sign -g -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Werror -g -std=c++11")

add_subdirectory (lib)
add_subdirectory (src)