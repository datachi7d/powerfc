#ifndef SERIAL_H_
#define SERIAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "pfc_types.h"

typedef struct _Serial Serial;

Serial * Serial_New(const char * path);
uint8_t Serial_Read(Serial * serial, uint8_t * buffer, uint8_t size);
uint8_t Serial_Write(Serial * serial, uint8_t * buffer, uint8_t size);
void Serial_Free(Serial * serial);

pfc_error Serial_ReadPFCMessage(Serial * serial, PFC_ID * ID, uint8_t * data, pfc_size * size);
pfc_error Serial_WritePFCMessage(Serial * serial, PFC_ID ID, uint8_t * data, pfc_size size);
pfc_error Serial_WritePFCAcknowledge(Serial * serial, PFC_ID ID);

#ifdef __cplusplus
}
#endif

#endif /* SERIAL_H_ */
