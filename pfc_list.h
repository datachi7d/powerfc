/*
 * pfc_list.h
 *
 *  Created on: 25/11/2016
 *      Author: sekelly
 */

#ifndef PFC_LIST_H_
#define PFC_LIST_H_

typedef struct _PFC_ValueList PFC_ValueList;

PFC_ValueList * PFC_ValueList_New();
PFC_ValueList * PFC_ValueList_GetFirst(PFC_ValueList * List);
PFC_ValueList * PFC_ValueList_GetLast(PFC_ValueList * List);
void * PFC_ValueList_NextItemValue(PFC_ValueList ** List);

#endif /* PFC_LIST_H_ */
