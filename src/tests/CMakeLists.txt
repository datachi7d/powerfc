set (test_pfclib_runner_SOURCES
	main.cc
	test_lists.cc
	test_memorytypes.cc
	test_memoryregistry.cc)

set (test_pfclib_runner_INCLUDE_DIRS
	${GTEST_INCLUDE_DIR}
	${PFCLIB_INCLUDE_DIR})

set (test_pfclib_runner_LIBRARIES
	gtest
	pthread
	pfclib_static)


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -g -std=c++11")

add_executable (test_pfclib_runner ${test_pfclib_runner_SOURCES})
target_include_directories (test_pfclib_runner PRIVATE ${test_pfclib_runner_INCLUDE_DIRS})
target_link_libraries (test_pfclib_runner ${test_pfclib_runner_LIBRARIES})

add_custom_command (
  OUTPUT test_pfclib_runner_out.xml
  COMMAND test_pfclib_runner --gtest_output=xml:test_pfclib_runner_out.xml
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  VERBATIM
)

if (RUN_TESTS)
  add_custom_target (
    test_pfclib_runner_TARGET ALL
    DEPENDS test_pfclib_runner_out.xml
  )
endif ()