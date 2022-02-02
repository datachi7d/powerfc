#ifndef SRC_LIB_INCLUDE_PFC_RECORDER_H_
#define SRC_LIB_INCLUDE_PFC_RECORDER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pfc_types.h"
#include "pfc_memoryregistry.h"

typedef struct _PFC_Recording PFC_Recording;
typedef struct _PFC_Recording_Entry PFC_Recording_Entry;
typedef struct _PFC_Recording_Entry PFC_Recording_Entry;

void PFC_Recording_Free(PFC_Recording * ptr);
PFC_Recording * PFC_Recording_New(uint8_t * MemoryBuffer, uint32_t MemorySize);
pfc_error PFC_Recording_AddEntry(PFC_Recording * Recording, PFC_Memory * Memory, PFC_ID id, uint64_t timestamp);

pfc_error PFC_Recording_WriteToFile(PFC_Recording * Recording, const char * file);
PFC_Recording * PFC_Recording_New_FromFile(const char * file, uint8_t * MemoryBuffer, uint32_t MemorySize);
pfc_error PFC_Recording_Reset(PFC_Recording * Recording);
uint32_t PFC_Recording_GetSize(PFC_Recording * Recording);

PFC_Recording_Entry * PFC_Recording_GetEntry(PFC_Recording * Recording, uint16_t entryNumber);
uint64_t PFC_Recording_Entry_GetTimestamp(PFC_Recording_Entry * Entry);
PFC_ID PFC_Recording_Entry_GetID(PFC_Recording_Entry * Entry);
const uint8_t * PFC_Recording_Entry_GetData(PFC_Recording_Entry * Entry);
pfc_size PFC_Recording_Entry_GetDataLength(PFC_Recording_Entry * Entry);

#ifdef __cplusplus
}
#endif

#endif /* SRC_LIB_INCLUDE_PFC_RECORDER_H_ */
