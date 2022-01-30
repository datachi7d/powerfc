idf_component_register(SRCS 
                        "../../lib/xml/xmlparser.c"
                        "../../lib/xml/xmltree.c"
                        "../../src/lib/pfc_list.c"
                        "../../src/lib/pfc_memory.c"
                        "../../src/lib/pfc_memoryconfig.c"
                        "../../src/lib/pfc_memoryregistry.c"
                        "../../src/lib/pfc_memorytypes.c"
                        "../../src/lib/pfc_process.c"
                        "../../src/lib/serial.c"
                       INCLUDE_DIRS 
                        "../../lib/xml"
                        "../../src/lib/include"
)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error=pointer-sign)