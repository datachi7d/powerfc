#ifndef PFC_REGISTRY_H_
#define PFC_REGISTRY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "pfc_types.h"
#include "pfc_memorytypes.h"

#define PFC_MAX_REGISTER_SIZE  0x7f

typedef struct _PFC_MemoryRegister PFC_MemoryRegister;
typedef struct _PFC_MemoryValue PFC_MemoryValue;
typedef struct _PFC_MemoryMap PFC_MemoryMap;
typedef struct _PFC_MemoryTable PFC_MemoryTable;
typedef struct _PFC_Memory PFC_Memory;

PFC_MemoryValue * PFC_MemoryRegister_AddValue(PFC_MemoryRegister * memoryRegister, pfc_memorytype Type, const char * Name);
PFC_MemoryValue * PFC_MemoryRegister_AddValueArray(PFC_MemoryRegister * memoryRegister, pfc_memorytype Type, const char * Name, int count);
const char * PFC_MemoryValue_GetName(PFC_MemoryValue * memoryValue);
pfc_size PFC_MemoryValue_GetSize(PFC_MemoryValue * memoryValue);
pfc_memorytype PFC_MemoryValue_GetType(PFC_MemoryValue * memoryValue);
PFC_MemoryValue * PFC_MemoryValue_GetFirst(PFC_MemoryValue * memoryValue);
int PFC_MemoryValue_GetIndex(PFC_MemoryValue * memoryValue);

PFC_MemoryValue * PFC_MemoryRegister_GetFirstValue(PFC_MemoryRegister * memoryRegister);
void PFC_MemoryRegister_DumpValue(PFC_MemoryRegister * MemoryRegister, PFC_Memory * Memory);
pfc_error PFC_MemoryRegister_GetNextValue(PFC_MemoryRegister * memoryRegister, PFC_MemoryValue ** value);
pfc_size PFC_MemoryRegister_GetSize(PFC_MemoryRegister * memoryRegister);
int PFC_MemoryRegister_GetCount(PFC_MemoryRegister * memoryRegister);
int PFC_MemoryRegister_GetOffsetOfValue(PFC_MemoryRegister * memoryRegister, PFC_MemoryValue * memoryValue);
int PFC_MemoryRegister_GetOffsetOfValueByName(PFC_MemoryRegister * memoryRegister, const char * name);
PFC_MemoryRegister * PFC_MemoryRegister_Get(PFC_Memory * Memory, PFC_ID RegisterID);
void PFC_MemoryRegister_SetFCPOffset(PFC_MemoryRegister * memoryRegister, uint16_t offset);
pfc_error PFC_MemoryRegister_SetFCPReorder(PFC_MemoryRegister * memoryRegister, uint8_t * reorder, pfc_size reorderSize);
pfc_size PFC_MemoryRegister_Malloc(PFC_MemoryRegister * memoryRegister);
void PFC_MemoryRegister_Free(PFC_MemoryRegister * memoryRegister);

PFC_MemoryRegister *  PFC_Memory_NewMirrorRegister(PFC_Memory * Memory, PFC_ID RegisterID, PFC_ID MirrorRegisterID, const char * name);
PFC_MemoryRegister * PFC_Memory_NewRegister(PFC_Memory * Memory, PFC_ID RegisterID, const char * name);
PFC_MemoryMap * PFC_Memory_NewMap(PFC_Memory * Memory, PFC_ID FirstRegisterID, PFC_ID LastRegisterID, pfc_memorytype cellType, uint8_t columns, uint8_t rows, const char * name);

PFC_MemoryTable * PFC_Memory_NewTable(PFC_Memory * Memory, PFC_ID registerID, pfc_memorytype Column1, pfc_memorytype Column2, uint8_t rows, const char * registerName, const char * Column1name, const char * Column2name);
PFC_MemoryRegister * PFC_MemoryTable_GetRegister(PFC_MemoryTable * Table);

PFC_Memory * PFC_Memory_New();
void PFC_Memory_Free(PFC_Memory * memory);
PFC_MemoryRegister * PFC_Memory_GetMemoryRegister(PFC_Memory * Memory, PFC_ID RegisterID);
void *  PFC_Memory_GetMemoryRegisterPointer(PFC_Memory * Memory, PFC_ID RegisterID);
pfc_error  PFC_Memory_UpdateMemoryRegisterPointer(PFC_Memory * Memory, PFC_ID RegisterID);
pfc_size  PFC_Memory_GetMemoryRegisterSize(PFC_Memory * Memory, PFC_ID RegisterID);
void PFC_Memroy_LoadFCPRO(PFC_Memory * Memory, const char * FileName);
void PFC_Memory_Dump(PFC_Memory * Memory);

pfc_size PFC_MemoryMap_Malloc(PFC_MemoryMap * MemoryMap);
PFC_MemoryValue * PFC_MemoryMap_GetMemoryValue(PFC_MemoryMap * MemoryMap, int Row, int Column);
void PFC_MemoryMap_SetFCPOffset(PFC_MemoryMap * MemoryMap, uint16_t FCPOffset);

#ifdef __cplusplus
}
#endif

#endif /* PFC_REGISTRY_H_ */
