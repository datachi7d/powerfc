#include <string.h>

#include "pfc_types.h"
#include "pfc_list.h"
#include "pfc_memory.h"

struct _PFC_ValueList
{
    PFC_ValueList * next;
    PFC_ValueList * previous;
    void * value;
};

PFC_ValueList * PFC_ValueList_New()
{
    PFC_ValueList * result = PFC_malloc(sizeof(*result));

    return result;
}

void PFC_ValueList_Free(PFC_ValueList * list)
{

    if(list != NULL)
    {
        PFC_ValueList * listItem = list;
        while( listItem != NULL )
        {
            PFC_ValueList * nextItem = listItem->next;
            PFC_free(listItem);
            listItem = nextItem;
        }
    }
}

PFC_ValueList * PFC_ValueList_GetFirst(PFC_ValueList * List)
{
    PFC_ValueList * item = List;

    if(item != NULL && item->value != NULL)
    {
        while(item->previous != NULL && item->value != NULL)
            item = item->previous;
    }
    else
    {
        item = NULL;
    }

    return item;
}

PFC_ValueList * PFC_ValueList_GetLast(PFC_ValueList * List)
{
    PFC_ValueList * item = List;

    if(item != NULL)
    {
        while(item->next != NULL && item->value != NULL)
            item = item->next;
    }

    return item;
}

pfc_error PFC_ValueList_AddItem(PFC_ValueList * List, void * Value)
{
    pfc_error Result = PFC_ERROR_UNSET;

    if(List != NULL && Value != NULL)
    {
        PFC_ValueList * lastListItem = PFC_ValueList_GetLast(List);
        PFC_ValueList * newListItem = lastListItem->value == NULL ? lastListItem : PFC_malloc(sizeof(*newListItem));

        if(lastListItem != NULL && newListItem != NULL)
        {
            if(lastListItem->value != NULL)
            {
                newListItem->previous = lastListItem;
                lastListItem->next = newListItem;
            }
            newListItem->value = Value;
            Result = PFC_ERROR_NONE;
        }
        else
        {
            PFC_free(newListItem);
            Result = PFC_ERROR_MEMORY;
        }
    }
    else
    {
        Result = PFC_ERROR_NULL_PARAMETER;
    }

    return Result;
}

pfc_error PFC_ValueList_RemoveItem(PFC_ValueList * List, void * Value)
{
    pfc_error Result = PFC_ERROR_UNSET;

    if(List != NULL && Value != NULL)
    {
        PFC_ValueList * valueListItem = List;

        Result = PFC_ERROR_NOT_FOUND;

        do
        {

            if(valueListItem->value == Value)
            {
                if(valueListItem->previous != NULL)
                    valueListItem->previous->next = valueListItem->next;

                if(valueListItem->next != NULL)
                    valueListItem->next->previous = valueListItem->previous;

                PFC_free(valueListItem);
                valueListItem = NULL;

                Result = PFC_ERROR_NONE;
            }
            else
            {
                if(PFC_ValueList_NextItemValue(&valueListItem) == NULL)
                {
                    valueListItem = NULL;
                }
            }

        }while(valueListItem != NULL);

    }
    else
    {
        Result = PFC_ERROR_NULL_PARAMETER;
    }

    return Result;
}


void * PFC_ValueList_NextItemValue(PFC_ValueList ** List)
{
    void * nextValue = NULL;

    if(List != NULL && *List != NULL)
    {
        if((*List)->next != NULL && (*List)->next->value != NULL)
        {
            *List = (*List)->next;
            nextValue = (*List)->value;
        }
    }

    return nextValue;
}

void * PFC_ValueList_GetValue(PFC_ValueList * List)
{
    void * Value = NULL;

    if(List != NULL)
    {
        Value = List->value;
    }

    return Value;
}

int PFC_ValueList_Count(PFC_ValueList * List)
{
	int count = 0;

	if(List)
	{
		PFC_ValueList * item = List;
        while(item->next != NULL && item->value != NULL)
        {
        	item = item->next;
        	count++;
        }
	}

	return count;
}
