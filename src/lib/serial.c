#include "serial.h"
#include "pfc_memory.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define PFC_MAX_MESSAGE_LENGTH 0xff
#define PFC_MAX_DATA_LENGTH (PFC_MAX_MESSAGE_LENGTH - sizeof(PFC_Header) - 1)

struct _Serial
{
	int serialfd;
};

typedef struct __attribute__((__packed__))
{
    PFC_ID ID;
    pfc_size Length;
} PFC_Header;

uint8_t CheckSum(uint8_t * buffer, uint8_t length)
{
    uint8_t * ptr = buffer;
    uint8_t sum = 0xFF;

    for(; ptr < (buffer+length); ptr++)
        sum -= *ptr;

    return sum;
}

int SetInterfaceAttributes(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    //tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag |= PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    //tty.c_cflag |= CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */


    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;


    /* fetch bytes as they become available */
    //tty.c_cc[VMIN] = 0;
    //tty.c_cc[VTIME] = 2;

    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;


    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

Serial * Serial_New(const char * path)
{
	Serial * serial = NULL;

	if(path != NULL)
	{
		serial = PFC_malloc(sizeof(*serial));

		if(serial != NULL)
		{
			serial->serialfd = open(path, O_RDWR | O_NOCTTY | O_SYNC);

			if (serial->serialfd >= 0)
			{
				SetInterfaceAttributes(serial->serialfd, B19200);
				lseek(serial->serialfd, 0, SEEK_END);
			}
			else
			{
				printf("Error opening %s: %s\n", path, strerror(errno));
				Serial_Free(serial);
				serial = NULL;
			}
		}
	}

	return serial;
}

uint8_t Serial_Read(Serial * serial, uint8_t * buffer, uint8_t size)
{
    size_t ret = 0;

    if(serial != NULL)
    {
    	ret = read(serial->serialfd, buffer, size);
    }

    return ((ret < 0) | (ret != size)) ? 0 : size;
}

uint8_t Serial_Write(Serial * serial, uint8_t * buffer, uint8_t size)
{
    size_t ret = 0;

    if(serial != NULL)
    {
    	ret = write(serial->serialfd, buffer, size);
    }

    return ((ret < 0) | (ret != size)) ? 0 : size;
}

void Serial_Free(Serial * serial)
{

	if(serial != NULL)
	{
		if(serial->serialfd >= 0)
		{
			close(serial->serialfd);
		}

		PFC_free(serial);
	}
}

pfc_error Serial_ReadPFCMessage(Serial * serial, PFC_ID * ID, uint8_t * data, pfc_size * size)
{
	pfc_error result = PFC_ERROR_UNSET;

	if(serial != NULL && ID != NULL && data != NULL && size != NULL)
	{
		uint8_t data_buffer[PFC_MAX_MESSAGE_LENGTH] = {0};
		PFC_Header * header = (PFC_Header *)&data_buffer[0];
		int readLen = 0;

		readLen = Serial_Read(serial, (uint8_t *)header, sizeof(*header));

		if(readLen == sizeof(*header))
		{
			if(header->Length < PFC_MAX_MESSAGE_LENGTH)
			{
				*size = header->Length - sizeof(*header);
				uint8_t RecvChecksum = 0;

				if(header->Length > sizeof(*header))
				{
					readLen = Serial_Read(serial, &data_buffer[sizeof(*header)], *size);
				}
				else
				{
					readLen = 0;
				}

				if(readLen == *size
				   && Serial_Read(serial, &RecvChecksum, 1) == 1)
				{
					if(CheckSum(data_buffer, header->Length) == RecvChecksum)
					{
						*ID = header->ID;
						memcpy(data, &data_buffer[sizeof(*header)], *size);
						result = PFC_ERROR_NONE;
					}
					else
					{
						*size = 0;
						result = PFC_ERROR_CHECKSUM;
					}
				}
				else
				{
					*size = 0;
					result = PFC_ERROR_TIMEOUT;
				}

			}
			else
			{
				result = PFC_ERROR_MESSAGE_LENGTH;
			}
		}
		else
		{
			result = PFC_ERROR_TIMEOUT;
		}
	}
	else
	{
		result = PFC_ERROR_NULL_PARAMETER;
	}

	return result;
}


pfc_error Serial_WritePFCMessage(Serial * serial, PFC_ID ID, uint8_t * data, pfc_size size)
{
	pfc_error result = PFC_ERROR_UNSET;

	if(serial != NULL)
	{
		uint8_t data_buffer[PFC_MAX_MESSAGE_LENGTH] = {0};
		PFC_Header * header = (PFC_Header *)data_buffer;

	    header->ID = ID;
	    header->Length = sizeof(*header);

	    if(data != NULL
	       && size > 0
		   && size < PFC_MAX_MESSAGE_LENGTH)
	    {
	        header->Length += size;
	        memcpy(&data_buffer[sizeof(*header)], data, size);
	        result = PFC_ERROR_NONE;
	    }
	    else
	    {
	    	if(size != 0)
	    	{
	    		result = PFC_ERROR_LENGTH;
	    	}
	    	else
	    	{
	    		result = PFC_ERROR_NONE;
	    	}
	    }

	    if	(result == PFC_ERROR_NONE)
	    {
	    	data_buffer[header->Length+1] = CheckSum(data_buffer, header->Length);
	    	Serial_Write(serial, data_buffer, header->Length + 1);
	    }
	}
	else
	{
		result = PFC_ERROR_NULL_PARAMETER;
	}

	return result;
}

pfc_error Serial_WritePFCAcknowledge(Serial * serial, PFC_ID ID)
{
	return Serial_WritePFCMessage(serial, ID, NULL, 0);
}


