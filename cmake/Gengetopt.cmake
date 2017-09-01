macro(find_gengetopt)
    if(NOT gengetopt_executable)
        find_program(gengetopt_executable gengetopt)
        if (NOT gengetopt_executable)
          message(fatal_error "gengetopt not found - aborting")
        endif (NOT gengetopt_executable)
    endif(NOT gengetopt_executable)
endmacro(find_gengetopt)

macro(add_gengetopt_files _sources )
    find_gengetopt()

    foreach (_current_file ${ARGN})
        get_filename_component(_in ${_current_file} ABSOLUTE)
        get_filename_component(_basename ${_current_file} NAME_WE)

      set(_out ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.c)
      set(_header ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.h)

        add_custom_command(
           OUTPUT ${_out} ${_header}
           COMMAND ${gengetopt_executable}
           ARGS
           --input=${_in} --file-name=${_basename}
           DEPENDS ${_in}
        )
    
        set(${_sources} ${${_sources}} ${_out} )
        set(${_sources} ${${_sources}} ${_header} )
        message("in: " ${_in})
    endforeach (_current_file)
endmacro(add_gengetopt_files)