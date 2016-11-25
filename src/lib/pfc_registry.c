/*
 * pfc_registry.c
 *
 *  Created on: 25/11/2016
 *      Author: sekelly
 */

#include "pfc_types.h"
#include "pfc_registry.h"
#include "pfc_memory.h"
#include "pfc_list.h"

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
    pfc_size Size;
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

PFC_MemoryValue * PFC_MemoryRegister_AddValue(PFC_MemoryRegister * memoryRegister, pfc_size Size, const char * Name)
{
    PFC_MemoryValue * memoryValue = NULL;

    if(memoryRegister != NULL && Size > 0)
    {
        memoryValue = PFC_MemoryValue_New(Size);

        if(memoryValue != NULL)
        {
            if(PFC_MemoryRegister_GetSize(memoryRegister) <= memoryRegister->MemorySize + Size)
            {
                pfc_error result = PFC_ERROR_UNSET;

                if ( (result = PFC_ValueList_AddItem(memoryRegister->Values, memoryValue)) == PFC_ERROR_NONE )
                {
                    memoryValue->Name = PFC_strdup(Name);
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

pfc_size PFC_MemoryRegister_GetSize(PFC_MemoryRegister * memoryRegister)
{
    pfc_size Size = 0;

    if(memoryRegister)
    {
        PFC_ValueList * list = PFC_ValueList_GetFirst(memoryRegister->Values);

        if(list != NULL)
        {
            PFC_MemoryValue * value = PFC_ValueList_GetValue(list);
            do
            {
                if(value)
                {
                    Size += value->Size;
                }
                value = PFC_ValueList_NextItemValue(&list);
            }while( value != NULL );
        }
    }

    return Size;
}


PFC_MemoryRegister * PFC_MemoryRegister_New(PFC_Memory * Memory, PFC_ID RegisterID, pfc_size Size, const char * name)
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
    }
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

