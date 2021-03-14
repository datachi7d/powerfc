#include <sys/poll.h>
#include <stdio.h>

#include "pfc_read_cmdline.h"
#include "serial.h"
#include "pfc_memory.h"
#include "pfc_process.h"


void ReadResponse(Serial * serial, PFC_ID id)
{
	int pfdn = 0;
	struct pollfd * pfd;

	pfd = (struct pollfd *)PFC_malloc(sizeof(*pfd));

	pfd[0].fd = Serial_GetFD(serial);
	pfd[0].events = POLLIN;
	Serial_Reset(serial);

	poll(pfd, 1, 300);

	for(pfdn = 0; pfdn < 1; pfdn++)
	{
		if(pfd[pfdn].revents & POLLIN)
		{
		    if(Serial_GetFD(serial) == pfd[pfdn].fd)
		    {
				uint8_t data[256] = {0};
				pfc_size size = 0;
				PFC_ID read_id = 0;
				if(Serial_ReadPFCMessage(serial, &read_id, data, &size) == PFC_ERROR_NONE)
				{
					if(read_id == id)
					{
						int i = 0;
						printf("%02x", id);
						for(i = 0; i < size; i++)
						{
							printf("%02x", data[i]);
						}
						printf("\n");
					}
				}
		    }
		}
	}

	PFC_free(pfd);
}


int main(int argc, char * argv[])
{
	int result = 0;

    struct gengetopt_args_info ai;

    if (cmdline_parser(argc, argv, &ai) == 0)
    {


        PFC_Process * process = PFC_Process_NewFromConfig(ai.memory_config_arg);

        if(process != NULL)
        {
            if(PFC_Process_SetServer(process, ai.serial_arg) == PFC_ERROR_NONE)
            {
                pfc_error error = PFC_ERROR_NONE;
                
                error = PFC_Process_RequestServerRead(process, NULL, *ai.pfcid_arg);

                if(error == PFC_ERROR_NONE)
                {
                    PFC_Process_Run(process);
                }

                PFC_Process_DumpValue(process, *ai.pfcid_arg);
            }
        }
    }
    return result;
}

