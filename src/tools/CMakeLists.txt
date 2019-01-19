INCLUDE(${CMAKE_SOURCE_DIR}/cmake/Gengetopt.cmake)


#PFC Simulation application

set(pfc_sim_SRC
	pfc_sim.c
)

add_gengetopt_files(pfc_sim_SRC pfc_sim_cmdline.ggo)

add_executable (pfc_sim ${pfc_sim_SRC})
target_link_libraries(pfc_sim pfclib_static)

#PFC Simulation dump application

set(pfc_sim_dump_SRC
	pfc_sim_dump.c
)

add_gengetopt_files(pfc_sim_dump_SRC pfc_sim_dump_cmdline.ggo)

add_executable (pfc_sim_dump ${pfc_sim_dump_SRC})
target_link_libraries(pfc_sim_dump pfclib_static)

#PFC read application

set(pfc_read_SRC
	pfc_read.c
)

add_gengetopt_files(pfc_read_SRC pfc_read_cmdline.ggo)

add_executable (pfc_read ${pfc_read_SRC})
target_link_libraries(pfc_read pfclib_static)

#PFC dump application

set(pfc_dump_SRC
	pfc_dump.c
)

add_gengetopt_files(pfc_dump_SRC pfc_dump_cmdline.ggo)

add_executable (pfc_dump ${pfc_dump_SRC})
target_link_libraries(pfc_dump pfclib_static)


set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-error=unused-but-set-variable")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

install (TARGETS pfc_sim pfc_sim_dump
		RUNTIME DESTINATION bin)