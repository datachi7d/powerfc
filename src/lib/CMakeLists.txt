set (pfclib_SOURCES
	pfc_list.c
	pfc_memory.c
	pfc_memorytypes.c
	pfc_memoryregistry.c
	pfc_memoryconfig.c
	pfc_process.c
	pfc_recorder.c
	serial.c )

set (pfclib_INCLUDE_DIRS
	${PFCLIB_INCLUDE_DIR})

set (pfclib_LIBRARIES libxml_static m)

if (ENABLE_GCOV)

  #set(pfclib_LIBRARIES "${pfclib_LIBRARIES} gcov")
endif ()

set_source_files_properties(pfc_recorder.c PROPERTIES COMPILE_FLAGS -Wno-error=pedantic)


add_library (pfclib_static STATIC ${pfclib_SOURCES})
target_include_directories (pfclib_static PUBLIC ${pfclib_INCLUDE_DIRS})
set_target_properties (pfclib_static PROPERTIES OUTPUT_NAME pfclib)
target_link_libraries(pfclib_static ${pfclib_LIBRARIES})


add_library (pfclib_shared SHARED ${pfclib_SOURCES})
target_include_directories (pfclib_shared PUBLIC ${pfclib_INCLUDE_DIRS})
set_target_properties (pfclib_shared PROPERTIES OUTPUT_NAME pfclib)
set_property (TARGET pfclib_shared PROPERTY POSITION_INDEPENDANT_CODE ON)
target_link_libraries(pfclib_shared ${pfclib_LIBRARIES})


install (TARGETS pfclib_static pfclib_shared 
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib)