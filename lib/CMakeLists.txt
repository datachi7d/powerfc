if (BUILD_TESTS)
  add_subdirectory (gtest)
#  set (GTEST_INCLUDE_DIR ${gtest_SOURCE_DIR}/include CACHE INTERNAL "GTEST_INCLUDE_DIR")  
endif ()