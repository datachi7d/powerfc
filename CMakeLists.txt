cmake_minimum_required (VERSION 2.8.12.2)
project (pfclib)

enable_testing ()

find_package(GTest REQUIRED)

option (BUILD_TESTS "Build tests" ON)
option (RUN_TESTS "Enable tests" ON)
option (RUN_VALGRIND "Enable valgrind of tests" ON)
option (ENABLE_GCOV "Enable gcov" OFF)


set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Werror -Wno-pointer-sign -pedantic --std=gnu11 ")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Werror -std=c++11")



if (RUN_TESTS)
  set (BUILD_TESTS ON)
  if (RUN_VALGRIND)
    set (VALGRIND_COMMAND valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=./valgrind.suppress )
  else (RUN_VALGRIND)
    set (VALGRIND_COMMAND "")
  endif (RUN_VALGRIND)
endif (RUN_TESTS)

if (ENABLE_GCOV)
  set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -fprofile-arcs -ftest-coverage --coverage")
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fprofile-arcs -ftest-coverage --coverage")
  set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
endif ()

add_compile_options (-g)


add_subdirectory (lib)
add_subdirectory (src)