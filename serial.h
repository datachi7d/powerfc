/*
 * serial.h
 *
 *  Created on: 1/11/2016
 *      Author: sean
 */

#ifndef SERIAL_H_
#define SERIAL_H_

int serial_open(char * path);
uint8_t serial_read(uint8_t * buffer, uint8_t size);
uint8_t serial_write(uint8_t * buffer, uint8_t size);
void serial_close();

#endif /* SERIAL_H_ */
