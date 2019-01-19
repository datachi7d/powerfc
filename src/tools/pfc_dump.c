#include <sys/poll.h>
#include <stdio.h>

#include "pfc_dump_cmdline.h"
#include "serial.h"
#include "pfc_memory.h"




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
    	Serial * serial = Serial_New(ai.serial_arg);

    	if(serial != NULL)
    	{
    		PFC_ID id = 0;
    		pfc_error error = PFC_ERROR_NONE;

    		for(id = 0x00; id < 0xff; id++)
    		{
    			if((error = Serial_WritePFCMessage(serial, id, NULL, 0)) == PFC_ERROR_NONE)
    			{
    				ReadResponse(serial, id);
    			}
    			else
    			{
    				printf("Error writing to serial %s (%d)\n", ai.serial_arg, error);
    				result = -1;
    				break;
    			}
    		}
    	}
    	else
    	{
    		printf("Error opening serial %s\n", ai.serial_arg);
    		result = -1;
    	}
    }

    return result;
}

