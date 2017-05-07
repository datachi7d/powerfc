#include "pfc_memoryconfig.h"
#include "pfc_memory.h"

#include <stddef.h>

struct _PFC_MemoryConfig
{
	char * FileName;
};

PFC_MemoryConfig * PFC_MemoryConfig_New(char * fileName)
{
    PFC_MemoryConfig * memoryConfig = NULL;

    if(fileName != NULL)
    {
    	if((memoryConfig = (PFC_MemoryConfig *)PFC_malloc(sizeof(*memoryConfig))) != NULL)
    	{
    		memoryConfig->FileName = (char *)PFC_strdup(fileName);

    		if(memoryConfig->FileName == NULL)
    		{
    			PFC_free(memoryConfig);
    			memoryConfig = NULL;
    		}
    	}
    }

    return memoryConfig;
}
