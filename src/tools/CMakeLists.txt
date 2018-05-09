INCLUDE(${CMAKE_SOURCE_DIR}/cmake/Gengetopt.cmake)

set(pfc_sim_SRC
	pfc_sim.c
)

add_gengetopt_files(pfc_sim_SRC pfc_sim_cmdline.ggo)

add_executable (pfc_sim ${pfc_sim_SRC})
target_link_libraries(pfc_sim pfclib_static)


set(pfc_sim_dump_SRC
	pfc_sim_dump.c
)

add_gengetopt_files(pfc_sim_dump_SRC pfc_sim_dump_cmdline.ggo)

add_executable (pfc_sim_dump ${pfc_sim_dump_SRC})
target_link_libraries(pfc_sim_dump pfclib_static)


include_directories(${CMAKE_CURRENT_BINARY_DIR})

install (TARGETS pfc_sim pfc_sim_dump
		RUNTIME DESTINATION bin)