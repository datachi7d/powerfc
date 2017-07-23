add_executable (pfc_sim pfc_sim.c)
target_link_libraries(pfc_sim pfclib_static)

install (TARGETS pfc_sim
		RUNTIME DESTINATION bin)