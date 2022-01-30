#include <sys/poll.h>
#include <stdio.h>

#include "pfc_read_cmdline.h"
#include "serial.h"
#include "pfc_memory.h"
#include "pfc_process.h"


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

