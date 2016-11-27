/*
 * pfc_memory.c
 *
 *  Created on: 25/11/2016
 *      Author: sekelly
 */

#include <string.h>
#include <stdlib.h>

#include "pfc_types.h"
#include "pfc_memory.h"


void * PFC_malloc(uint16_t size)
{
    void * ptr = malloc(size);
    memset(ptr, 0, size);
    return ptr;
}

void PFC_free(void * ptr)
{
    free(ptr);
}

char * PFC_strdup(const char * string)
{
    return strdup(string);
}