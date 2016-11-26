set (pfclib_SOURCES
	pfc_list.c
	pfc_memory.c
	pfc_memorytypes.c
	pfc_memoryregistry.c)

set (pfclib_INCLUDE_DIRS
	${PFCLIB_INCLUDE_DIR})

add_library (pfclib_static STATIC ${pfclib_SOURCES})
target_include_directories (pfclib_static PUBLIC ${pfclib_INCLUDE_DIRS})
set_target_properties (pfclib_static PROPERTIES OUTPUT_NAME pfclib)


add_library (pfclib_shared SHARED ${pfclib_SOURCES})
target_include_directories (pfclib_shared PUBLIC ${pfclib_INCLUDE_DIRS})
set_target_properties (pfclib_shared PROPERTIES OUTPUT_NAME pfclib)
set_property (TARGET pfclib_shared PROPERTY POSITION_INDEPENDANT_CODE ON)

install (TARGETS pfclib_static pfclib_shared 
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib)

