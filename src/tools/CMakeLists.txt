INCLUDE(${CMAKE_SOURCE_DIR}/cmake/Gengetopt.cmake)

set(pfc_sim_SRC
	pfc_sim.c
)

add_gengetopt_files(pfc_sim_SRC pfc_sim_cmdline.ggo)

add_executable (pfc_sim ${pfc_sim_SRC})
target_link_libraries(pfc_sim pfclib_static)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

install (TARGETS pfc_sim
		RUNTIME DESTINATION bin)