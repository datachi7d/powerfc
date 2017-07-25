#include <sys/poll.h>
#include <stdio.h>
#include <string.h>

#include "pfc_memoryregistry.h"
#include "pfc_memorytypes.h"
#include "pfc_memoryconfig.h"
#include "pfc_memory.h"
#include "pfc_list.h"
#include "pfc_process.h"
#include "serial.h"

struct _PFC_Process
{
	PFC_MemoryConfig * MemoryConfig;
	PFC_ValueList * clients;
};

PFC_Process * PFC_Process_New(const char * memoryConfig)
{
	PFC_Process * Process = NULL;

	if(memoryConfig != NULL)
	{
		Process = (PFC_Process *)PFC_malloc(sizeof(PFC_Process));

		Process->MemoryConfig = PFC_MemoryConfig_New(memoryConfig);

		if(Process->MemoryConfig != NULL)
		{
			if(PFC_MemoryConfig_Load(Process->MemoryConfig) == PFC_ERROR_NONE)
			{

				Process->clients = PFC_ValueList_New();

				if(Process->clients == NULL)
				{
					PFC_Process_Free(Process);
					Process = NULL;
				}
			}
			else
			{
				PFC_Process_Free(Process);
				Process = NULL;
			}
		}
		else
		{
			PFC_Process_Free(Process);
			Process = NULL;
		}
	}

	return Process;
}

pfc_error PFC_Process_LoadFCPro(PFC_Process * process, const char * FCProFile)
{
	pfc_error result = PFC_ERROR_UNSET;

	if(process != NULL && FCProFile != NULL)
	{
		PFC_Memory * Memory = PFC_MemoryConfig_GetMemory(process->MemoryConfig);

		if(Memory != NULL)
		{
			PFC_Memroy_LoadFCPRO(Memory, FCProFile);
			result = PFC_ERROR_NONE;
		}
		else
		{
			result = PFC_ERROR_MEMORY;
		}
	}
	else
	{
		result = PFC_ERROR_NULL_PARAMETER;
	}

	return result;
}

pfc_error PFC_Process_AddClient(PFC_Process * process, const char * serialPath)
{
	pfc_error result = PFC_ERROR_UNSET;

	if(serialPath != NULL && process != NULL)
	{
		Serial * serial = Serial_New(serialPath);

		if(serial != NULL)
		{
			result = PFC_ValueList_AddItem(process->clients, serial);
		}
		else
		{
			result = PFC_ERROR_FILEIO;
		}
	}

	return result;
}

void Setup_Client_PollFD(PFC_Process * process, struct pollfd * pfd)
{
	if(process->clients != NULL)
	{
        PFC_ValueList * list = PFC_ValueList_GetFirst(process->clients);
        int pfdn = 0;

        if(list != NULL)
        {
        	Serial * value = PFC_ValueList_GetValue(list);
            do
            {
                if(value)
                {
                	int fd = Serial_GetFD(value);
                	if(fd >= 0)
                	{
                		pfd[pfdn].fd = fd;
                		pfd[pfdn].events = POLLIN;
                		pfdn++;
                	}
                }
                value = PFC_ValueList_NextItemValue(&list);
            }while( value != NULL );
        }
	}
}

Serial * GetClientFromFD(PFC_Process * process, int fd)
{
	Serial * serial = NULL;

	if(process->clients != NULL)
	{
        PFC_ValueList * list = PFC_ValueList_GetFirst(process->clients);
        int pfdn = 0;

        if(list != NULL)
        {
        	Serial * value = PFC_ValueList_GetValue(list);
            do
            {
                if(value)
                {
                	if(Serial_GetFD(value) == fd)
                	{
                		serial = value;
                		break;
                	}
                }
                value = PFC_ValueList_NextItemValue(&list);
            }while( value != NULL );
        }
	}

	return serial;
}

void PFC_Process_Run(PFC_Process * process)
{
	struct pollfd * pfd;

	if(process != NULL)
	{
		int pfd_count = PFC_ValueList_Count(process->clients);
		int pfdn = 0;
		pfd = (struct pollfd *)PFC_malloc(sizeof(*pfd) * pfd_count);

		Setup_Client_PollFD(process, pfd);

		poll(pfd, pfd_count, 100);

		for(pfdn = 0; pfdn < pfd_count; pfdn++)
		{
			if(pfd[pfdn].revents & POLLIN)
			{
				Serial * serial = GetClientFromFD(process, pfd[pfdn].fd);

				if(serial)
				{
					PFC_ID id = 0;
					pfc_size size = 0;
					uint8_t data[0xff] = {0};
					pfc_error result = PFC_ERROR_UNSET;

					result = Serial_ReadPFCMessage(serial, &id, data, &size);

					if(result == PFC_ERROR_NONE)
					{
						PFC_Memory * memory = PFC_MemoryConfig_GetMemory(process->MemoryConfig);
						uint8_t * memory_data = (uint8_t *)PFC_Memory_GetMemoryRegisterPointer(memory, id);
						pfc_size memory_size = PFC_Memory_GetMemoryRegisterSize(memory, id);

						if(size == 0)
						{
							printf("Read for %02x\n", id);

							if(memory_data != NULL && memory_size != 0)
							{
								Serial_WritePFCMessage(serial, id, memory_data, memory_size);
							}

						}
						else
						{
							printf("Write for %02x\n", id);

							if(size == memory_size)
							{
								memcpy(memory_data, data, size);

								Serial_WritePFCAcknowledge(serial, id);
							}
						}

						if(memory)
						{
							PFC_MemoryRegister * memoryRegister = PFC_Memory_GetMemoryRegister(memory, id);

							if(memoryRegister)
							{
								PFC_MemoryRegister_DumpValue(memoryRegister, memory);
							}
						}
					}
				}
			}
		}
	}
}

void PFC_Process_Free(PFC_Process * process)
{
	if(process != NULL)
	{
		if(process->MemoryConfig != NULL)
		{
			PFC_MemoryConfig_Free(process->MemoryConfig);
		}

		if(process->clients != NULL)
		{
	        PFC_ValueList * list = PFC_ValueList_GetFirst(process->clients);

	        if(list != NULL)
	        {
	        	Serial * value = PFC_ValueList_GetValue(list);
	            do
	            {
	                if(value)
	                {
	                	Serial_Free(value);
	                }
	                value = PFC_ValueList_NextItemValue(&list);
	            }while( value != NULL );
	        }

	       PFC_ValueList_Free(process->clients);
		}
	}
}
