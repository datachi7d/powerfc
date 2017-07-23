set (PFCLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/include" CACHE INTERNAL "PFCLIB_INCLUDE_DIR")

add_subdirectory (lib)
add_subdirectory (tools)

if (BUILD_TESTS)
	add_subdirectory (tests)
endif ()
