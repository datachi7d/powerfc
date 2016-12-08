/*
 * pfc_registry.c
 *
 *  Created on: 25/11/2016
 *      Author: sekelly
 */

#include "pfc_memoryregistry.h"
#include "pfc_types.h"
#include "pfc_memory.h"
#include "pfc_list.h"

#include <stdio.h>

struct _PFC_Memory
{
    PFC_ValueList * MemoryRegisters;
};

struct _PFC_MemoryRegister
{
    PFC_ID ID;
    PFC_ValueList * Values;
    uint8_t * Memory;
    uint16_t MemorySize;
    char * Name;
};

struct _PFC_MemoryValue
{
    char * Name;
    pfc_memorytype Type;
    int Row;
    int Column;
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


PFC_MemoryValue * MemoryRegister_AddValue(PFC_MemoryRegister * memoryRegister, pfc_memorytype Type, const char * Name, int Row, int Column)
{
    PFC_MemoryValue * memoryValue = NULL;
    pfc_error result = PFC_ERROR_UNSET;
    pfc_size Size = PFC_Convert_PFCValueSize(Type);

    if(memoryRegister != NULL && Size > 0)
    {
        if(PFC_MemoryRegister_GetSize(memoryRegister) + Size <= memoryRegister->MemorySize)
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
        char valueName[256] = {0};
        snprintf(valueName, sizeof(valueName), "%s[%d]", Name, i);
        PFC_MemoryValue * memoryValue = NULL;

        if((memoryValue = MemoryRegister_AddValue(memoryRegister, Type, valueName, i, -1)) == NULL)
        {
            failed = true;
            break;
        }
        else if(first == NULL)
        {
            first = memoryValue;
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

        PFC_free(memoryRegister);
    }
}

PFC_Memory * PFC_Memory_New()
{
    PFC_Memory * result = PFC_malloc(sizeof(*result));

    if(result != NULL)
    {
        result->MemoryRegisters = PFC_ValueList_New();

        if(result->MemoryRegisters == NULL)
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

        PFC_free(memory);
    }
}

PFC_MemoryRegister * PFC_Memory_NewRegister(PFC_Memory * Memory, PFC_ID RegisterID, pfc_size Size, const char * name)
{
    PFC_MemoryRegister * memoryRegister = NULL;

    if (Memory != NULL && Memory->MemoryRegisters != NULL)
    {
        if(PFC_Memory_GetMemoryRegister(Memory, RegisterID) == NULL)
        {
            memoryRegister = PFC_malloc(sizeof(*memoryRegister));

            if (memoryRegister != NULL)
            {
                memoryRegister->Memory = PFC_malloc(Size);
                if(memoryRegister->Memory != NULL)
                {
                    pfc_error result;

                    if ( (result = PFC_ValueList_AddItem(Memory->MemoryRegisters, memoryRegister)) == PFC_ERROR_NONE)
                    {
                        if ( (memoryRegister->Values = PFC_ValueList_New()) != NULL)
                        {
                            memoryRegister->ID = RegisterID;
                            memoryRegister->MemorySize = Size;
                            memoryRegister->Name = PFC_strdup(name);
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

pfc_error PFC_Memory_NewMap(PFC_Memory * Memory, PFC_ID FirstRegisterID, PFC_ID LastRegisterID, pfc_memorytype cellType, uint8_t columns, uint8_t rows, const char * name)
{
    pfc_error Result = PFC_ERROR_UNSET;

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

                    registerN = PFC_Memory_NewRegister(Memory, registerID, RegisterSize, registerName);

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
                        Result = PFC_ERROR_NONE;
                    }
                    else
                    {
                        Result = PFC_ERROR_NULL_PARAMETER;
                    }
                }
                else
                {
                    Result = PFC_ERROR_NULL_PARAMETER;
                }
            }
            else
            {
                Result = PFC_ERROR_NULL_PARAMETER;
            }
        }
        else
        {
            Result = PFC_ERROR_NULL_PARAMETER;
        }
    }
    else
    {
        Result = PFC_ERROR_NULL_PARAMETER;
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
                    }
                }
                value = PFC_ValueList_NextItemValue(&list);
            }while( value != NULL );
        }
    }

    return result;
}

void *  PFC_Memory_GetMemoryRegisterPointer(PFC_Memory * Memory, PFC_ID RegisterID)
{
    void * ptr = NULL;
    PFC_MemoryRegister * memoryRegister = PFC_Memory_GetMemoryRegister(Memory, RegisterID);

    if(memoryRegister != NULL)
    {
        ptr = memoryRegister->Memory;
    }

    return ptr;
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

