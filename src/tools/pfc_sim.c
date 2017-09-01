#include <stdio.h>

#include "pfc_sim_cmdline.h"
#include "pfc_process.h"

int main(int argc, char * argv[])
{

    struct gengetopt_args_info ai;

    if (cmdline_parser(argc, argv, &ai) == 0)
    {
          PFC_Process * process = PFC_Process_New(ai.memory_config_arg);

          if(process != NULL)
          {
              if(ai.fcpro_given ? PFC_Process_LoadFCPro(process, ai.fcpro_arg) == PFC_ERROR_NONE : true)
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
                  printf("Error loading FCPro file: %s\n", ai.fcpro_arg);
                  PFC_Process_Free(process);
              }
          }
          else
          {
              printf("Error loading memory config: %s\n", ai.memory_config_arg);
          }
    }

    return 0;
}
