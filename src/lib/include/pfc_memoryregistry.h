/*
 * pfc_registry.h
 *
 *  Created on: 25/11/2016
 *      Author: sekelly
 */

#ifndef PFC_REGISTRY_H_
#define PFC_REGISTRY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pfc_types.h"
#include "pfc_memorytypes.h"

#define PFC_MAX_REGISTER_SIZE  0x7f

typedef struct _PFC_MemoryRegister PFC_MemoryRegister;
typedef struct _PFC_MemoryValue PFC_MemoryValue;
typedef struct _PFC_Memory PFC_Memory;

PFC_MemoryValue * PFC_MemoryRegister_AddValue(PFC_MemoryRegister * memoryRegister, pfc_memorytype Type, const char * Name);
PFC_MemoryValue * PFC_MemoryRegister_AddValueArray(PFC_MemoryRegister * memoryRegister, pfc_memorytype Type, const char * Name, int count);
const char * PFC_MemoryValue_GetName(PFC_MemoryValue * memoryValue);
pfc_size PFC_MemoryValue_GetSize(PFC_MemoryValue * memoryValue);
pfc_memorytype PFC_MemoryValue_GetType(PFC_MemoryValue * memoryValue);


PFC_MemoryValue * PFC_MemoryRegister_GetFirstValue(PFC_MemoryRegister * memoryRegister);
pfc_error PFC_MemoryRegister_GetNextValue(PFC_MemoryRegister * memoryRegister, PFC_MemoryValue ** value);
pfc_size PFC_MemoryRegister_GetSize(PFC_MemoryRegister * memoryRegister);
int PFC_MemoryRegister_GetCount(PFC_MemoryRegister * memoryRegister);
int PFC_MemoryRegister_GetOffsetOfValue(PFC_MemoryRegister * memoryRegister, PFC_MemoryValue * memoryValue);
PFC_MemoryRegister * PFC_MemoryRegister_Get(PFC_Memory * Memory, PFC_ID RegisterID);
void PFC_MemoryRegister_Free(PFC_MemoryRegister * memoryRegister);

PFC_MemoryRegister * PFC_Memory_NewRegister(PFC_Memory * Memory, PFC_ID RegisterID, pfc_size Size, const char * name);
pfc_error PFC_Memory_NewMap(PFC_Memory * Memory, PFC_ID FirstRegisterID, PFC_ID LastRegisterID, pfc_memorytype cellType, uint8_t columns, uint8_t rows, const char * name);

PFC_Memory * PFC_Memory_New();
void PFC_Memory_Free(PFC_Memory * memory);
PFC_MemoryRegister * PFC_Memory_GetMemoryRegister(PFC_Memory * Memory, PFC_ID RegisterID);
void *  PFC_Memory_GetMemoryRegisterPointer(PFC_Memory * Memory, PFC_ID RegisterID);
pfc_size  PFC_Memory_GetMemoryRegisterSize(PFC_Memory * Memory, PFC_ID RegisterID);

#ifdef __cplusplus
}
#endif

#endif /* PFC_REGISTRY_H_ */
