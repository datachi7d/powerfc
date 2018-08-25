#include "pfc_memoryregistry.h"
#include "pfc_types.h"
#include "pfc_memory.h"
#include "pfc_list.h"

#include <stdio.h>
#include <string.h>

struct _PFC_Memory
{
    PFC_ValueList * MemoryMaps;
    PFC_ValueList * MemoryRegisters;
};

struct _PFC_MemoryRegister
{
    PFC_ID ID;
    PFC_ValueList * Values;
    uint8_t * Memory;
    uint16_t MemorySize;
    char * Name;
    int FCPRO_offset;
    uint8_t * FCPRO_reorder;
    bool Mirrored;
    PFC_MemoryRegister * MirroredRegister;

};

struct _PFC_MemoryValue
{
    char * Name;
    pfc_memorytype Type;
    int Row;
    int Column;
    bool ArrayItem;
    PFC_MemoryValue * firstItem;
};

struct _PFC_MemoryMap
{
    PFC_Memory * Memory;
    PFC_ID IDMin;
    PFC_ID IDMax;
    char * Name;
    pfc_memorytype Type;
    int Rows;
    int Columns;
};


struct _PFC_MemoryTable
{
    PFC_Memory * Memory;
    PFC_MemoryRegister * Register;
    char * Name;
    pfc_memorytype Column1Type;
    pfc_memorytype Column2Type;
    int Rows;
};

PFC_MemoryValue * PFC_MemoryValue_New(pfc_size Size)
{
    PFC_MemoryValue * memoryValue = NULL;
    if(Size != 0)
    {
        memoryValue = PFC_malloc(sizeof(*memoryValue));
    }

    return memoryValue;
}

void PFC_MemoryValue_Free(PFC_MemoryValue * ptr)
{
    if(ptr != NULL)
    {
        PFC_free(ptr->Name);
        PFC_free(ptr);
    }
}

const char * PFC_MemoryValue_GetName(PFC_MemoryValue * memoryValue)
{
    const char * result = NULL;

    if(memoryValue != NULL)
    {
        result = memoryValue->Name;
    }

    return result;
}

pfc_size PFC_MemoryValue_GetSize(PFC_MemoryValue * memoryValue)
{
    pfc_size result = 0;

    if(memoryValue != NULL)
    {
        result = PFC_Convert_PFCValueSize(memoryValue->Type);
    }

    return result;
}

pfc_memorytype PFC_MemoryValue_GetType(PFC_MemoryValue * memoryValue)
{
    pfc_memorytype result = PFC_MEMORYTYPE_LAST;

    if(memoryValue != NULL)
    {
        result = memoryValue->Type;
    }

    return result;
}

PFC_MemoryValue * PFC_MemoryValue_GetFirst(PFC_MemoryValue * memoryValue)
{
    PFC_MemoryValue * result = NULL;

    if(memoryValue != NULL)
    {
        result = memoryValue->firstItem;
    }

    return result;
}

int PFC_MemoryValue_GetIndex(PFC_MemoryValue * memoryValue)
{
    int result = 0;

    if(memoryValue != NULL)
    {
        if(memoryValue->ArrayItem)
        {
            result = memoryValue->Row;
        }
    }

    return result;
}


PFC_MemoryMap * PFC_MemoryMap_New(PFC_Memory * Memory, const char * Name, PFC_ID IDMin, PFC_ID IDMax, int Columns, int Rows)
{
    PFC_MemoryMap * memoryMap = NULL;
    if(Memory != NULL && Rows != 0 && Columns != 0 && IDMin < IDMax)
    {
        memoryMap = PFC_malloc(sizeof(*memoryMap));


        memoryMap->Memory = Memory;
        memoryMap->Name = PFC_strdup(Name);
        memoryMap->IDMin = IDMin;
        memoryMap->IDMax = IDMax;
        memoryMap->Rows = Rows;
        memoryMap->Columns = Columns;
    }

    return memoryMap;
}

void PFC_MemoryMap_Free(PFC_MemoryMap * ptr)
{
    if(ptr != NULL)
    {
        PFC_free(ptr->Name);
        PFC_free(ptr);
    }
}

const char * PFC_MemoryMap_GetName(PFC_MemoryMap * memoryValue)
{
    const char * result = NULL;

    if(memoryValue != NULL)
    {
        result = memoryValue->Name;
    }

    return result;
}

pfc_memorytype PFC_MemoryMap_GetType(PFC_MemoryMap * memoryValue)
{
    pfc_memorytype result = PFC_MEMORYTYPE_LAST;

    if(memoryValue != NULL)
    {
        result = memoryValue->Type;
    }

    return result;
}

PFC_MemoryValue * PFC_MemoryMap_GetMemoryValue(PFC_MemoryMap * MemoryMap, int Row, int Column)
{
    PFC_MemoryValue * Result = NULL;

    if(MemoryMap != NULL)
    {
        PFC_ID ID = MemoryMap->IDMin;

        for(; ID <= MemoryMap->IDMax; ID++)
        {
            PFC_MemoryRegister * MemoryRegister = PFC_Memory_GetMemoryRegister(MemoryMap->Memory, ID);

            if(MemoryRegister != NULL)
            {
                PFC_MemoryValue * value = PFC_MemoryRegister_GetFirstValue(MemoryRegister);

                do
                {
                    if(value)
                    {
                        if(value->Row == Row && value->Column == Column)
                        {
                            Result = value;
                            break;
                        }
                    }
                } while((PFC_MemoryRegister_GetNextValue(MemoryRegister, &value)) == PFC_ERROR_NONE);


                if(Result != NULL)
                {
                    break;
                }
            }
        }

    }

    return Result;
}

pfc_size PFC_MemoryMap_Malloc(PFC_MemoryMap * MemoryMap)
{
    pfc_size Result = 0;

    if(MemoryMap != NULL)
    {
        PFC_ID ID = MemoryMap->IDMin;

        for(; ID <= MemoryMap->IDMax; ID++)
        {
            PFC_MemoryRegister * MemoryRegister = PFC_Memory_GetMemoryRegister(MemoryMap->Memory, ID);

            if(MemoryRegister != NULL)
            {
            	Result += PFC_MemoryRegister_Malloc(MemoryRegister);
            }
        }
    }

    return Result;
}


void PFC_MemoryMap_SetFCPOffset(PFC_MemoryMap * MemoryMap, uint16_t FCPOffset)
{

    if(MemoryMap != NULL)
    {
        PFC_ID ID = MemoryMap->IDMin;

        for(; ID <= MemoryMap->IDMax; ID++)
        {
            PFC_MemoryRegister * MemoryRegister = PFC_Memory_GetMemoryRegister(MemoryMap->Memory, ID);

            if(MemoryRegister != NULL)
            {
            	MemoryRegister->FCPRO_offset = FCPOffset;
            	FCPOffset += MemoryRegister->MemorySize;
            }
        }
    }
}


PFC_MemoryValue * MemoryRegister_AddValue(PFC_MemoryRegister * memoryRegister, pfc_memorytype Type, const char * Name, int Row, int Column)
{
    PFC_MemoryValue * memoryValue = NULL;
    pfc_error result = PFC_ERROR_UNSET;
    pfc_size Size = PFC_Convert_PFCValueSize(Type);

    if(memoryRegister != NULL && Size > 0)
    {
        if(memoryRegister->Memory == NULL)
        {
            memoryValue = PFC_MemoryValue_New(Size);

            if(memoryValue != NULL)
            {

                if ( (result = PFC_ValueList_AddItem(memoryRegister->Values, memoryValue)) == PFC_ERROR_NONE )
                {
                    memoryValue->Name = PFC_strdup(Name);
                    memoryValue->Type = Type;
                    memoryValue->Row = Row;
                    memoryValue->Column = Column;
                    memoryValue->ArrayItem = false;
                    memoryValue->firstItem = NULL;

                    memoryRegister->MemorySize += Size;
                }
                else
                {
                    PFC_MemoryValue_Free(memoryValue);
                    memoryValue = NULL;
                }
            }
        }
    }

    return memoryValue;
}

PFC_MemoryValue * PFC_MemoryRegister_AddValue(PFC_MemoryRegister * memoryRegister, pfc_memorytype Type, const char * Name)
{
    return MemoryRegister_AddValue(memoryRegister, Type, Name, -1, -1);
}

PFC_MemoryValue * PFC_MemoryRegister_AddValueArray(PFC_MemoryRegister * memoryRegister, pfc_memorytype Type, const char * Name, int count)
{
    int i = 0;
    bool failed = false;
    PFC_MemoryValue * first = NULL;

    for(i = 0; i < count; i++)
    {
//        char valueName[256] = {0};
//        snprintf(valueName, sizeof(valueName), "%s[%d]", Name, i);
        PFC_MemoryValue * memoryValue = NULL;

        if((memoryValue = MemoryRegister_AddValue(memoryRegister, Type, Name, i, -1)) != NULL)
        {
            if(first == NULL)
            {
                first = memoryValue;
            }

            memoryValue->ArrayItem = true;
            memoryValue->firstItem = first;
        }
        else
        {
            failed = true;
            break;
        }
    }

    if(failed)
    {
        PFC_MemoryValue * next = first;

        while(first != NULL)
        {
            PFC_MemoryRegister_GetNextValue(memoryRegister, &next);
            if(PFC_ValueList_RemoveItem(memoryRegister->Values, first) == PFC_ERROR_NONE)
            {
                PFC_MemoryValue_Free(first);
                first = next;
            }
            else
            {
                first = NULL;
            }
        }

        first = NULL;
    }

    return first;
}

PFC_MemoryValue * PFC_MemoryRegister_AddValueXY(PFC_MemoryRegister * memoryRegister, pfc_memorytype Type, const char * Name,  int Row, int Column)
{
    return MemoryRegister_AddValue(memoryRegister, Type, Name, Row, Column);
}


PFC_MemoryValue * PFC_MemoryRegister_GetFirstValue(PFC_MemoryRegister * memoryRegister)
{
    PFC_MemoryValue * value = NULL;

    if(memoryRegister)
    {
        value = (PFC_MemoryValue *)PFC_ValueList_GetValue(PFC_ValueList_GetFirst(memoryRegister->Values));
    }

    return value;
}

void PFC_MemoryRegister_DumpValue(PFC_MemoryRegister * MemoryRegister, PFC_Memory * Memory)
{
	if(MemoryRegister != NULL && Memory != NULL)
	{
		PFC_MemoryValue * value = PFC_MemoryRegister_GetFirstValue(MemoryRegister);

		if(MemoryRegister->FCPRO_offset >= 0)
			printf("%s [%d-%d %02x]:\n", MemoryRegister->Name, MemoryRegister->FCPRO_offset, MemoryRegister->FCPRO_offset + MemoryRegister->MemorySize, MemoryRegister->ID);
		else
			printf("%s [%02x]:\n", MemoryRegister->Name, MemoryRegister->ID);


		do
		{
			if(value != NULL)
			{
				uint8_t * ptr = PFC_Memory_GetMemoryRegisterPointer(Memory,MemoryRegister->ID);

				if(ptr != NULL)
				{
					char valueBuffer[256] = {0};

					ptr += PFC_MemoryRegister_GetOffsetOfValue(MemoryRegister,value);

					PFC_Convert_PFCValueToString(PFC_MemoryValue_GetType(value), true, ptr, valueBuffer, sizeof(valueBuffer));

					if(value->ArrayItem)
					{
						printf("\t %s {%s}[%d]: %s\n", value->Name, PFC_MemoryType_ToString(value->Type), value->Row, valueBuffer);
					}
					else
					{
						printf("\t %s: %s\n", value->Name, valueBuffer);
					}
				}
			}
		} while((PFC_MemoryRegister_GetNextValue(MemoryRegister, &value)) == PFC_ERROR_NONE);
	}
}


pfc_error PFC_MemoryRegister_GetNextValue(PFC_MemoryRegister * memoryRegister, PFC_MemoryValue ** value)
{
    pfc_error result = PFC_ERROR_UNSET;

    if(memoryRegister && value && *value)
    {
        PFC_ValueList * list = PFC_ValueList_GetFirst(memoryRegister->Values);

        if(list != NULL)
        {
            PFC_MemoryValue * current = PFC_ValueList_GetValue(list);
            PFC_MemoryValue * previous = NULL;

            result = PFC_ERROR_NOT_FOUND;

            while( current != NULL )
            {
                previous = current;
                current = PFC_ValueList_NextItemValue(&list);

                if(previous == *value && current != NULL)
                {
                    *value = current;
                    result = PFC_ERROR_NONE;
                    break;
                }
            }


            if(result == PFC_ERROR_NOT_FOUND)
            {
                *value = NULL;
            }
        }
        else
        {
            result = PFC_ERROR_NULL_PARAMETER;
        }
    }
    else
    {
        result = PFC_ERROR_NULL_PARAMETER;
    }

    return result;
}


pfc_size PFC_MemoryRegister_GetSize(PFC_MemoryRegister * memoryRegister)
{
    pfc_size Size = 0;

    if(memoryRegister)
    {
        PFC_ValueList * list = PFC_ValueList_GetFirst(memoryRegister->Values);
        PFC_MemoryValue * value = PFC_ValueList_GetValue(list);

        while( value != NULL )
        {
            Size += PFC_Convert_PFCValueSize(value->Type);
            value = PFC_ValueList_NextItemValue(&list);
        }
    }

    return Size;
}

int PFC_MemoryRegister_GetCount(PFC_MemoryRegister * memoryRegister)
{
    int Count = 0;

    if(memoryRegister)
    {
        PFC_ValueList * list = PFC_ValueList_GetFirst(memoryRegister->Values);
        PFC_MemoryValue * value = PFC_ValueList_GetValue(list);

        while( value != NULL )
        {
            Count += 1;
            value = PFC_ValueList_NextItemValue(&list);
        }
    }

    return Count;
}


int PFC_MemoryRegister_GetOffsetOfValue(PFC_MemoryRegister * memoryRegister, PFC_MemoryValue * memoryValue)
{
    int Size = 0;
    bool found = false;

    if(memoryRegister)
    {
        PFC_ValueList * list = PFC_ValueList_GetFirst(memoryRegister->Values);
        PFC_MemoryValue * value = PFC_ValueList_GetValue(list);

        while( value != NULL )
        {
            if(value == memoryValue)
            {
                found = true;
                break;
            }

            Size += PFC_Convert_PFCValueSize(value->Type);
            value = PFC_ValueList_NextItemValue(&list);
        }
    }

    return found ? Size : -1;
}

int PFC_MemoryRegister_GetOffsetOfValueByName(PFC_MemoryRegister * memoryRegister, const char * name)
{
    int Size = 0;
    bool found = false;

    if(memoryRegister)
    {
        PFC_ValueList * list = PFC_ValueList_GetFirst(memoryRegister->Values);
        PFC_MemoryValue * value = PFC_ValueList_GetValue(list);

        while( value != NULL )
        {
            if(strcmp(value->Name, name) == 0)
            {
                found = true;
                break;
            }

            Size += PFC_Convert_PFCValueSize(value->Type);
            value = PFC_ValueList_NextItemValue(&list);
        }
    }

    return found ? Size : -1;
}

void PFC_MemoryRegister_SetFCPOffset(PFC_MemoryRegister * memoryRegister, uint16_t offset)
{
    if(memoryRegister)
    {
    	memoryRegister->FCPRO_offset = offset;
    }
}

pfc_error PFC_MemoryRegister_SetFCPReorder(PFC_MemoryRegister * memoryRegister, uint8_t * reorder, pfc_size reorderSize)
{
	pfc_error result = PFC_ERROR_UNSET;

    if(memoryRegister && reorderSize == memoryRegister->MemorySize)
    {
    	memoryRegister->FCPRO_reorder = (uint8_t *)PFC_malloc(memoryRegister->MemorySize);

    	if(memoryRegister->FCPRO_reorder)
    	{
    		memcpy(memoryRegister->FCPRO_reorder, reorder, reorderSize);
    	}
    	else
    	{
    		result = PFC_ERROR_MEMORY;
    	}
    }
    else
    {
    	result = reorderSize == memoryRegister->MemorySize ? PFC_ERROR_LENGTH : PFC_ERROR_NULL_PARAMETER;
    }

    return result;
}

pfc_size PFC_MemoryRegister_Malloc(PFC_MemoryRegister * memoryRegister)
{
	pfc_size Result = 0;
	if(memoryRegister != NULL)
	{
		memoryRegister->Memory = PFC_malloc(memoryRegister->MemorySize);

		if(memoryRegister->Memory)
			Result = memoryRegister->MemorySize;
	}

	return Result;
}

void PFC_MemoryRegister_Free(PFC_MemoryRegister * memoryRegister)
{
    if(memoryRegister != NULL)
    {
        PFC_ValueList * list = PFC_ValueList_GetFirst(memoryRegister->Values);

        if(list != NULL)
        {
            PFC_MemoryValue * value = PFC_ValueList_GetValue(list);
            do
            {
                if(value)
                {
                    PFC_MemoryValue_Free(value);
                }
                value = PFC_ValueList_NextItemValue(&list);
            }while( value != NULL );
        }


        PFC_ValueList_Free(memoryRegister->Values);

        if(memoryRegister->Memory)
            PFC_free(memoryRegister->Memory);

        if(memoryRegister->Name)
            PFC_free(memoryRegister->Name);

        if(memoryRegister->FCPRO_reorder)
            PFC_free(memoryRegister->FCPRO_reorder);

        PFC_free(memoryRegister);
    }
}

PFC_Memory * PFC_Memory_New()
{
    PFC_Memory * result = PFC_malloc(sizeof(*result));

    if(result != NULL)
    {
        result->MemoryRegisters = PFC_ValueList_New();

        if(result->MemoryRegisters != NULL)
        {
            result->MemoryMaps = PFC_ValueList_New();

            if(result->MemoryMaps == NULL)
            {
                PFC_ValueList_Free(result->MemoryRegisters);
                PFC_free(result);
                result = NULL;
            }
        }
        else
        {
            PFC_free(result);
            result = NULL;
        }
    }

    return result;
}

void PFC_Memory_Free(PFC_Memory * memory)
{
    if(memory != NULL)
    {
        PFC_ValueList * list = PFC_ValueList_GetFirst(memory->MemoryRegisters);

        if(list != NULL)
        {
            PFC_MemoryRegister * value = PFC_ValueList_GetValue(list);
            do
            {
                if(value)
                {
                    PFC_MemoryRegister_Free(value);
                }
                value = PFC_ValueList_NextItemValue(&list);
            }while( value != NULL );
        }

        PFC_ValueList_Free(memory->MemoryRegisters);

        list = PFC_ValueList_GetFirst(memory->MemoryMaps);

        if(list != NULL)
        {
            PFC_MemoryMap * value = PFC_ValueList_GetValue(list);
            do
            {
                if(value)
                {
                    PFC_MemoryMap_Free(value);
                }
                value = PFC_ValueList_NextItemValue(&list);
            }while( value != NULL );
        }

        PFC_free(memory);
    }
}

PFC_MemoryRegister *  PFC_Memory_NewMirrorRegister(PFC_Memory * Memory, PFC_ID MirrorRegisterID, PFC_ID RegisterID, const char * name)
{
    PFC_MemoryRegister * memoryRegister = NULL;
    PFC_MemoryRegister * memoryMirrorRegister = NULL;

    if (Memory != NULL && Memory->MemoryRegisters != NULL)
    {
        if((memoryRegister = PFC_Memory_GetMemoryRegister(Memory, MirrorRegisterID)) != NULL)
        {
            if(memoryRegister->Memory != NULL && memoryRegister->MemorySize != 0)

                if((memoryMirrorRegister = PFC_Memory_NewRegister(Memory, RegisterID, name)) != NULL)
                {
                    memoryMirrorRegister->Mirrored = true;
                    memoryMirrorRegister->MirroredRegister = memoryRegister;
                }
        }
    }

    return memoryMirrorRegister;
}

PFC_MemoryRegister * PFC_Memory_NewRegister(PFC_Memory * Memory, PFC_ID RegisterID, const char * name)
{
    PFC_MemoryRegister * memoryRegister = NULL;

    if (Memory != NULL && Memory->MemoryRegisters != NULL)
    {
        if(PFC_Memory_GetMemoryRegister(Memory, RegisterID) == NULL)
        {
            memoryRegister = PFC_malloc(sizeof(*memoryRegister));

            if (memoryRegister != NULL)
            {
				pfc_error result;

				if ( (result = PFC_ValueList_AddItem(Memory->MemoryRegisters, memoryRegister)) == PFC_ERROR_NONE)
				{
					if ( (memoryRegister->Values = PFC_ValueList_New()) != NULL)
					{
						memoryRegister->ID = RegisterID;
						memoryRegister->MemorySize = 0;
						memoryRegister->Name = PFC_strdup(name);
						memoryRegister->FCPRO_offset = -1;
						memoryRegister->Mirrored = false;
						memoryRegister->MirroredRegister = NULL;
					}
					else
					{
						PFC_MemoryRegister_Free(memoryRegister);
						memoryRegister = NULL;
					}
				}
				else
				{
					PFC_MemoryRegister_Free(memoryRegister);
					memoryRegister = NULL;
				}
            }
        }
    }

    return memoryRegister;
}


PFC_MemoryTable * PFC_Memory_NewTable(PFC_Memory * Memory, PFC_ID registerID, pfc_memorytype Column1, pfc_memorytype Column2, uint8_t rows, const char * registerName, const char * Column1name, const char * Column2name)
{
    PFC_MemoryTable * Result = NULL;

    if(Memory != NULL)
    {
        int column1Size = PFC_Convert_PFCValueSize(Column1);
        int column2Size = PFC_Convert_PFCValueSize(Column2);

        if(column1Size > 0 && column2Size > 0)
        {
            int TotalSize = ((int)rows) * (column1Size + column2Size);
            int X = 0;
            int Y = 0;

            if(TotalSize <= PFC_MAX_REGISTER_SIZE)
            {
                int registerCount = 0;
                PFC_MemoryRegister * registerN = PFC_Memory_NewRegister(Memory, registerID, registerName);

                if(registerN != NULL)
                {
                    bool failed = false;


                    for(registerCount = 0; registerCount < rows; registerCount++)
                    {
                        char valueName1[256] = {0};
                        char valueName2[256] = {0};

                        snprintf(valueName1, sizeof(valueName1), "%s[%d]", Column1name, registerCount);
                        snprintf(valueName2, sizeof(valueName2), "%s[%d]", Column2name, registerCount);

                        if(PFC_MemoryRegister_AddValueXY(registerN, Column1, valueName1, 1, registerCount) == NULL ||
                                PFC_MemoryRegister_AddValueXY(registerN, Column2, valueName2, 2, registerCount) == NULL)
                        {
                            failed = true;
                            break;
                        }
                    }

                    if(failed == false)
                    {
                    	Result =  (PFC_MemoryTable*)PFC_malloc(sizeof(*Result));
                    	Result->Register = registerN;
                    }
                }
            }
        }
    }


    return Result;
}

PFC_MemoryRegister * PFC_MemoryTable_GetRegister(PFC_MemoryTable * Table)
{
	PFC_MemoryRegister * Result = NULL;
	if(Table != NULL && Table->Register != NULL)
	{
		Result = Table->Register;
	}

	return Result;
}


PFC_MemoryMap * PFC_Memory_NewMap(PFC_Memory * Memory, PFC_ID FirstRegisterID, PFC_ID LastRegisterID, pfc_memorytype cellType, uint8_t columns, uint8_t rows, const char * name)
{
    PFC_MemoryMap * Result = NULL;

    if(Memory != NULL)
    {
        int cellSize = PFC_Convert_PFCValueSize(cellType);

        if(cellSize)
        {
            int TotalSize = ((int)rows) * ((int)columns) * (int)PFC_Convert_PFCValueSize(cellType);
            int Registers = (LastRegisterID - FirstRegisterID) + 1;
            int RegisterSize = (TotalSize/Registers);
            int ValuesPerRegister =  RegisterSize % cellSize == 0 ? RegisterSize/cellSize : 0;
            int X = 0;
            int Y = 0;

            if(RegisterSize <= PFC_MAX_REGISTER_SIZE && ValuesPerRegister > 0)
            {
                PFC_ID registerID = FirstRegisterID;
                bool failed = false;

                for(registerID = FirstRegisterID; registerID <= LastRegisterID; registerID++)
                {
                    int registerCount = 0;
                    char registerName[256] = {0};
                    PFC_MemoryRegister * registerN = NULL;
                    snprintf(registerName, sizeof(registerName), "%s[%d]", name, registerID - FirstRegisterID);

                    registerN = PFC_Memory_NewRegister(Memory, registerID, registerName);

                    if(registerN != NULL)
                    {
                        failed = false;


                        for(registerCount = 0; registerCount < (RegisterSize/cellSize); registerCount++)
                        {
                            char valueName[256] = {0};
                            snprintf(valueName, sizeof(valueName), "%s[%d][%d]", name, X, Y);
                            if(PFC_MemoryRegister_AddValueXY(registerN, cellType, valueName, X, Y) == NULL)
                            {
                                failed = true;
                                break;
                            }

                            if(Y < (rows-1))
                            {
                                Y++;
                            }
                            else if(X < (columns-1))
                            {
                                Y = 0;
                                X++;
                            }
                        }

                        if(failed)
                        {
                            break;
                        }
                    }
                    else
                    {
                        failed = true;
                        break;
                    }
                }

                if(!failed)
                {
                    if(Y == rows-1 && X == columns-1)
                    {
                        Result = PFC_MemoryMap_New(Memory, name, FirstRegisterID, LastRegisterID, columns, rows);


                    }
                }
            }
        }
    }


    return Result;

}


PFC_MemoryRegister * PFC_Memory_GetMemoryRegister(PFC_Memory * Memory, PFC_ID RegisterID)
{
    PFC_MemoryRegister * result = NULL;

    if(Memory != NULL)
    {
        PFC_ValueList * list = PFC_ValueList_GetFirst(Memory->MemoryRegisters);

        if(list != NULL)
        {
            PFC_MemoryRegister * value = PFC_ValueList_GetValue(list);
            do
            {
                if(value)
                {
                    if(value->ID == RegisterID)
                    {
                        result = value;
                        break;
                    }
                }
                value = PFC_ValueList_NextItemValue(&list);
            }while( value != NULL );
        }
    }

    return result;
}

pfc_error MemoryRegister_MirrorUpdate(PFC_Memory * Memory, PFC_ID RegisterID, bool MirrorUpdate)
{
    pfc_error error = PFC_ERROR_UNSET;
    PFC_MemoryRegister * memoryRegister = PFC_Memory_GetMemoryRegister(Memory, RegisterID);

    if(memoryRegister->Mirrored )
    {
        if(memoryRegister->MirroredRegister != NULL && memoryRegister->MirroredRegister->MemorySize >= memoryRegister->MemorySize)
        {
            int Count = PFC_MemoryRegister_GetCount(memoryRegister);
            int i = 0;

            PFC_MemoryValue * value = PFC_MemoryRegister_GetFirstValue(memoryRegister);

            do
            {
                int dataMirrorOffset = PFC_MemoryRegister_GetOffsetOfValue(memoryRegister,value);
                int dataOffset = PFC_MemoryRegister_GetOffsetOfValueByName(memoryRegister->MirroredRegister, value->Name);
                pfc_size dataSize = PFC_MemoryValue_GetSize(value);


                if(dataMirrorOffset >= 0 && dataOffset >= 0 &&
                        dataMirrorOffset + dataSize <= memoryRegister->MemorySize &&
                        dataOffset + dataSize <= memoryRegister->MirroredRegister->MemorySize)
                {
                    uint8_t * dataMirror = memoryRegister->Memory + dataMirrorOffset;
                    uint8_t * data = memoryRegister->MirroredRegister->Memory + dataOffset;

                    if(MirrorUpdate)
                        memcpy(dataMirror, data, dataSize);
                    else
                        memcpy(data, dataMirror, dataSize);
                }
                else
                {
                    error = PFC_ERROR_MEMORY;
                    break;
                }
                i++;

            } while((error = PFC_MemoryRegister_GetNextValue(memoryRegister, &value)) == PFC_ERROR_NONE);

            if(i == Count)
            {
                error = PFC_ERROR_NONE;

                //PFC_MemoryRegister_DumpValue(memoryRegister->MirroredRegister, Memory);
            }

        }
        else
        {
            error = PFC_ERROR_MEMORY;
        }
    }

    return error;
}

void *  PFC_Memory_GetMemoryRegisterPointer(PFC_Memory * Memory, PFC_ID RegisterID)
{
    void * ptr = NULL;
    PFC_MemoryRegister * memoryRegister = PFC_Memory_GetMemoryRegister(Memory, RegisterID);

    if(memoryRegister != NULL)
    {
        ptr = memoryRegister->Memory;

        if(memoryRegister->Mirrored)
        {
            MemoryRegister_MirrorUpdate(Memory, RegisterID, true);
        }
    }

    return ptr;
}


pfc_error  PFC_Memory_UpdateMemoryRegisterPointer(PFC_Memory * Memory, PFC_ID RegisterID)
{
    pfc_error error = PFC_ERROR_UNSET;
    PFC_MemoryRegister * memoryRegister = PFC_Memory_GetMemoryRegister(Memory, RegisterID);

    if(memoryRegister != NULL)
    {
        if(memoryRegister->Mirrored)
        {
            error = MemoryRegister_MirrorUpdate(Memory, RegisterID, false);
        }
        else
        {
            error = PFC_ERROR_NONE;
        }
    }
    else
    {
        error = PFC_ERROR_NOT_FOUND;
    }

    return error;
}

pfc_size  PFC_Memory_GetMemoryRegisterSize(PFC_Memory * Memory, PFC_ID RegisterID)
{
    return PFC_MemoryRegister_GetSize(PFC_Memory_GetMemoryRegister(Memory, RegisterID));
}

PFC_MemoryValue *  PFC_Memory_GetFirstMemoryValue(PFC_Memory * Memory, PFC_ID RegisterID)
{
    PFC_MemoryValue * memoryValue = NULL;
    PFC_MemoryRegister * memoryRegister = PFC_Memory_GetMemoryRegister(Memory, RegisterID);

    if(memoryRegister != NULL)
    {
        memoryValue = (PFC_MemoryValue *)PFC_ValueList_GetValue(PFC_ValueList_GetFirst(memoryRegister->Values));
    }

    return memoryValue;
}

void PFC_Memroy_LoadFCPRO(PFC_Memory * Memory, const char * FileName)
{
	if(Memory != NULL)
	{
		FILE * FileP;
		FileP = fopen(FileName, "rb");

		if(FileP != NULL)
		{
			char * file_buffer = NULL;

			if(fseek(FileP, 0L, SEEK_END) >= 0)
			{
				long file_buffer_size = ftell(FileP);

				if(file_buffer_size >= 0)
				{
					rewind(FileP);
					file_buffer = PFC_malloc((uint32_t)file_buffer_size);

					if(file_buffer != NULL)
					{
	                    if(fread(file_buffer, file_buffer_size, 1, FileP) > 0)
	                    {
							PFC_ValueList * list = PFC_ValueList_GetFirst(Memory->MemoryRegisters);

							if(list != NULL)
							{
								PFC_MemoryRegister * MemoryRegister = PFC_ValueList_GetValue(list);
								do
								{
									if(MemoryRegister)
									{
										if(MemoryRegister->Memory != NULL && MemoryRegister->FCPRO_offset >= 0)
										{
											if(MemoryRegister->FCPRO_reorder != NULL)
											{
												int i = 0;
												for(i = 0; i < MemoryRegister->MemorySize; i++)
												{
													int calcFCPOffset = MemoryRegister->FCPRO_offset + MemoryRegister->FCPRO_reorder[i];
													MemoryRegister->Memory[i] = file_buffer[calcFCPOffset];
												}
											}
											else
											{
												memcpy(MemoryRegister->Memory, &file_buffer[MemoryRegister->FCPRO_offset], MemoryRegister->MemorySize);
											}
										}
									}
									MemoryRegister = PFC_ValueList_NextItemValue(&list);
								}while( MemoryRegister != NULL );
							}
	                    }
					}
				}
			}
		}
	}
}


void PFC_Memory_Dump(PFC_Memory * Memory)
{
    if(Memory != NULL)
    {
        PFC_ValueList * list = PFC_ValueList_GetFirst(Memory->MemoryRegisters);

        if(list != NULL)
        {
            PFC_MemoryRegister * MemoryRegister = PFC_ValueList_GetValue(list);
            do
            {
                if(MemoryRegister)
                {
                    uint8_t * RegisterMemory = MemoryRegister->Memory;

                    PFC_MemoryRegister_DumpValue(MemoryRegister, Memory);

                    MemoryRegister = PFC_ValueList_NextItemValue(&list);
                }
            }while( MemoryRegister != NULL );
        }
    }
}


