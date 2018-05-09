#ifndef SRC_LIB_INCLUDE_PFC_PROCESS_H_
#define SRC_LIB_INCLUDE_PFC_PROCESS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pfc_types.h"

typedef struct _PFC_Process PFC_Process;

PFC_Process * PFC_Process_NewFromConfig();
pfc_error PFC_Process_LoadFCPro(PFC_Process * process, const char * FCProFile);
pfc_error PFC_Process_AddClient(PFC_Process * process, const char * serialPath);
pfc_error PFC_Process_SetServer(PFC_Process * process, const char * serialPath);
void PFC_Process_Run(PFC_Process * process);
void PFC_Process_Free(PFC_Process * process);

#ifdef __cplusplus
}
#endif

#endif /* SRC_LIB_INCLUDE_PFC_PROCESS_H_ */
