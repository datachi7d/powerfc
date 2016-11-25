option (BUILD_TESTS "Build tests" ON)
option (RUN_TESTS "Enable tests" ON)

if (RUN_TESTS)
  set (BUILD_TESTS ON)
endif ()

add_subdirectory (lib)
add_subdirectory (src)
