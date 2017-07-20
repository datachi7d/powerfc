#include "serial.h"
#include "pfc_memory.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

struct _Serial
{
	int serialfd;
};


int set_interface_attribs(int fd, int speed)
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
				set_interface_attribs(serial->serialfd, B19200);
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

