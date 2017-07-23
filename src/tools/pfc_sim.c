#include <stdio.h>

#include "pfc_process.h"

int main(int argc, char * argv[])
{
	if(argc == 4)
	{
		PFC_Process * process = PFC_Process_New(argv[1]);

		printf("memory config:	%s\n", argv[1]);
		printf("FCPRO file:		%s\n", argv[2]);
		printf("Serial port:	%s\n", argv[3]);

		if(process != NULL)
		{
			if(PFC_Process_LoadFCPro(process, argv[2]) == PFC_ERROR_NONE)
			{

				if(PFC_Process_AddClient(process, argv[3]) == PFC_ERROR_NONE)
				{
					while(1)
					{
						PFC_Process_Run(process);
					}
				}
				else
				{
					printf("Error opening serial\n");
					PFC_Process_Free(process);
				}
			}
			else
			{
				printf("Error loading FCPro\n");
				PFC_Process_Free(process);
			}
		}
		else
		{
			printf("Error loading memory config\n");
		}
	}
}
