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

typedef struct _PFC_MemoryRegister PFC_MemoryRegister;
typedef struct _PFC_MemoryValue PFC_MemoryValue;
typedef struct _PFC_Memory PFC_Memory;

PFC_MemoryValue * PFC_MemoryRegister_AddValue(PFC_MemoryRegister * memoryRegister, pfc_memorytype Type, const char * Name);
pfc_size PFC_MemoryRegister_GetSize(PFC_MemoryRegister * memoryRegister);
PFC_MemoryRegister * PFC_MemoryRegister_Get(PFC_Memory * Memory, PFC_ID RegisterID);
PFC_MemoryRegister * PFC_MemoryRegister_New(PFC_Memory * Memory, PFC_ID RegisterID, pfc_size Size, const char * name);
void PFC_MemoryRegister_Free(PFC_MemoryRegister * memoryRegister);

PFC_Memory * PFC_Memory_New();
void PFC_Memory_Free(PFC_Memory * memory);
PFC_MemoryRegister * PFC_Memory_GetMemoryRegister(PFC_Memory * Memory, PFC_ID RegisterID);
void *  PFC_Memory_GetMemoryRegisterPointer(PFC_Memory * Memory, PFC_ID RegisterID);
pfc_size  PFC_Memory_GetMemoryRegisterSize(PFC_Memory * Memory, PFC_ID RegisterID);

#ifdef __cplusplus
}
#endif

#endif /* PFC_REGISTRY_H_ */
