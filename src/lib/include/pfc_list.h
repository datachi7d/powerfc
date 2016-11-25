/*
 * pfc_list.h
 *
 *  Created on: 25/11/2016
 *      Author: sekelly
 */

#ifndef PFC_LIST_H_
#define PFC_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pfc_types.h"

typedef struct _PFC_ValueList PFC_ValueList;

PFC_ValueList * PFC_ValueList_New();
void PFC_ValueList_Free(PFC_ValueList * list);
PFC_ValueList * PFC_ValueList_GetFirst(PFC_ValueList * List);
PFC_ValueList * PFC_ValueList_GetLast(PFC_ValueList * List);
void * PFC_ValueList_NextItemValue(PFC_ValueList ** List);
pfc_error PFC_ValueList_AddItem(PFC_ValueList * List, void * Value);
void * PFC_ValueList_GetValue(PFC_ValueList * List);

#ifdef __cplusplus
}
#endif

#endif /* PFC_LIST_H_ */
