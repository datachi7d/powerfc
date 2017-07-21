#ifndef PFC_TYPES_H_
#define PFC_TYPES_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
  PFC_ERROR_NONE = 0,
  PFC_ERROR_MEMORY,
  PFC_ERROR_NULL_PARAMETER,
  PFC_ERROR_LENGTH,
  PFC_ERROR_NOT_FOUND,
  PFC_ERROR_LIST,
  PFC_ERROR_ID_CONFLICT,
  PFC_ERROR_FILEIO,
  PFC_ERROR_XML,
  PFC_ERROR_TIMEOUT,
  PFC_ERROR_CHECKSUM,
  PFC_ERROR_MESSAGE_LENGTH,
  PFC_ERROR_UNSET,
} pfc_error;

typedef uint8_t pfc_size;
typedef uint8_t PFC_ID;


#endif /* PFC_TYPES_H_ */
