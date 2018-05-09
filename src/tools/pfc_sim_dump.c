#include <stdio.h>

#include "pfc_sim_dump_cmdline.h"
#include "pfc_process.h"

int main(int argc, char * argv[])
{

    struct gengetopt_args_info ai;

    if (cmdline_parser(argc, argv, &ai) == 0)
    {
        PFC_Process * process = PFC_Process_NewFromDump(ai.data_arg);

        if(process != NULL)
        {
            if(PFC_Process_AddClient(process, ai.serial_arg) == PFC_ERROR_NONE)
            {
                while(1)
                {
                    PFC_Process_Run(process);
                }
            }
            else
            {
                printf("Error opening serial: %s\n", ai.serial_arg);
                PFC_Process_Free(process);
            }
        }
        else
        {
            printf("Error loading memory dump: %s\n", ai.data_arg);
        }
    }

    return 0;
}
