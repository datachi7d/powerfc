#ifndef PFC_MEMORY_H_
#define PFC_MEMORY_H_

#include <stdint.h>

void * PFC_malloc(uint32_t size);
void PFC_free(void * ptr);

char * PFC_strdup(const char * string);

#endif /* PFC_MEMORY_H_ */
