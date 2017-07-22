#ifndef SRC_LIB_INCLUDE_PFC_PROCESS_H_
#define SRC_LIB_INCLUDE_PFC_PROCESS_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _PFC_Process PFC_Process;

PFC_Process * PFC_Process_New();
void PFC_Process_Free(PFC_Process * process);

#ifdef __cplusplus
}
#endif

#endif /* SRC_LIB_INCLUDE_PFC_PROCESS_H_ */
