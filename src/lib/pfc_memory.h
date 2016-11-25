/*
 * pfc_memory.h
 *
 *  Created on: 25/11/2016
 *      Author: sekelly
 */

#ifndef PFC_MEMORY_H_
#define PFC_MEMORY_H_

void * PFC_malloc(uint16_t size);
void PFC_free(void * ptr);

char * PFC_strdup(const char * string);

#endif /* PFC_MEMORY_H_ */
