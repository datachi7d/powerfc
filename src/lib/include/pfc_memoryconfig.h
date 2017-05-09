#ifndef SRC_LIB_INCLUDE_PFC_MEMORYCONFIG_H_
#define SRC_LIB_INCLUDE_PFC_MEMORYCONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pfc_types.h"
#include "pfc_memoryregistry.h"

#define XML_PFC_MEMORY_CONFIG "pfc_memory_config"
#define XML_PFC_MEMORY "pfc_memory"


#define XML_PFC_MEMORY_REGISTER "pfc_memory_register"
#define XML_PFC_MEMORY_VALUES "pfc_memory_values"
#define XML_PFC_MEMORY_VALUE "pfc_memory_value"
#define XML_PFC_MEMORY_VALUE_ARRAY "pfc_memory_value_array"
#define XML_PFC_MEMORY_MAP "pfc_memory_map"


#define XML_NAME "name"

#define XML_REGISTERID "registerid"

#define XML_REGISTERIDMIN "registeridmin"
#define XML_REGISTERIDMAX "registeridmax"

#define XML_SIZE "size"
#define XML_COUNT "count"

#define XML_MEMORY_TYPE "memorytype"

#define XML_ROWS "rows"
#define XML_COLUMNS "columns"


typedef struct _PFC_MemoryConfig PFC_MemoryConfig;
PFC_MemoryConfig * PFC_MemoryConfig_New(const char * fileName);
PFC_Memory * PFC_MemoryConfig_GetMemory(PFC_MemoryConfig * MemoryConfig);
void PFC_MemoryConfig_Free(PFC_MemoryConfig * MemoryConfig);

pfc_error PFC_MemoryConfig_LoadString(PFC_MemoryConfig * MemoryConfig, const char * string, uint32_t length);
pfc_error PFC_MemoryConfig_Load(PFC_MemoryConfig * MemoryConfig);

#ifdef __cplusplus
}
#endif


#endif /* SRC_LIB_INCLUDE_PFC_MEMORYCONFIG_H_ */
