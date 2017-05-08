#ifndef SRC_LIB_INCLUDE_PFC_MEMORYCONFIG_H_
#define SRC_LIB_INCLUDE_PFC_MEMORYCONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pfc_types.h"

#define XML_PFC_MEMORY_CONFIG "pfc_memory_config"
#define XML_PFC_MEMORY "pfc_memory"


#define XML_PFC_MEMORY_REGISTER "pfc_memory_register"
#define XML_PFC_MEMORY_VALUES "pfc_memory_values"
#define XML_PFC_MEMORY_VALUE "pfc_memory_value"
#define XML_PFC_MEMORY_MAP "pfc_memory_map"




#define XML_NAME "Name"

#define XML_REGISTERID "RegisterID"

#define XML_SIZE "Size"

#define XML_MEMORY_TYPE "MemoryType"

#define XML_ROWS "Rows"
#define XML_COLUMNS "COLUMNS"


typedef struct _PFC_MemoryConfig PFC_MemoryConfig;
PFC_MemoryConfig * PFC_MemoryConfig_New(const char * fileName);
void PFC_MemoryConfig_Free(PFC_MemoryConfig * MemoryConfig);

pfc_error PFC_MemoryConfig_Load(PFC_MemoryConfig * MemoryConfig);

#ifdef __cplusplus
}
#endif


#endif /* SRC_LIB_INCLUDE_PFC_MEMORYCONFIG_H_ */
