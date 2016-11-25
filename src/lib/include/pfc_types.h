/*
 * pfc_types.h
 *
 *  Created on: 25/11/2016
 *      Author: sekelly
 */

#ifndef PFC_TYPES_H_
#define PFC_TYPES_H_

#include <stddef.h>
#include <stdint.h>

typedef enum
{
  PFC_ERROR_NONE = 0,
  PFC_ERROR_MEMORY,
  PFC_ERROR_NULL_PARAMETER,

  PFC_ERROR_UNSET,
} pfc_error;


typedef uint8_t pfc_size;

typedef uint8_t PFC_ID;


#endif /* PFC_TYPES_H_ */
