set (test_pfclib_runner_SOURCES
	main.cc
	test_lists.cc
	test_memorytypes.cc
	test_memoryregistry.cc
	test_memoryconfig.cc
	test_serial.cc
	test_process.cc
	support/process.cc)

set (test_pfclib_runner_INCLUDE_DIRS
	support
	${GTEST_INCLUDE_DIR}
	${PFCLIB_INCLUDE_DIR})

set (test_pfclib_runner_LIBRARIES
	gtest
	pthread
	pfclib_static)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -g -std=c++11 -Wno-narrowing")

add_definitions (-D__STDC_FORMAT_MACROS)

add_executable (test_pfclib_runner ${test_pfclib_runner_SOURCES})
target_include_directories (test_pfclib_runner PRIVATE ${test_pfclib_runner_INCLUDE_DIRS})
target_link_libraries (test_pfclib_runner ${test_pfclib_runner_LIBRARIES})

configure_file(test_memory_config.xml test_memory_config.xml COPYONLY)
configure_file(test_memory_dump.dat test_memory_dump.dat COPYONLY)
configure_file(FCPRO.hd1 FCPRO.hd1 COPYONLY)
configure_file(valgrind.suppress valgrind.suppress COPYONLY)

if (ENABLE_GCOV)
  include(${CMAKE_SOURCE_DIR}/cmake/CodeCoverage.cmake)
  setup_target_for_coverage(test_pfclib_runner_gcov_TARGET test_pfclib_runner coverage)
endif ()



add_custom_command (
  OUTPUT test_pfclib_runner_out.xml
  COMMAND ${VALGRIND_COMMAND} ${CMAKE_CURRENT_BINARY_DIR}/test_pfclib_runner --gtest_output=xml:test_pfclib_runner_out.xml
  COMMAND rm test_pfclib_runner_out.xml
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
)

if (RUN_TESTS)
  add_custom_target (test_pfclib_runner_TARGET ALL DEPENDS test_pfclib_runner_out.xml)
endif ()