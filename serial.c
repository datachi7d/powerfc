/*
 * serial.c
 *
 *  Created on: 1/11/2016
 *      Author: sean
 */


#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>

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

static int serialfd = 0;


int serial_open(char * path)
{
    serialfd = open(path, O_RDWR | O_NOCTTY | O_SYNC);

    if (serialfd < 0)
    {
        printf("Error opening %s: %s\n", path, strerror(errno));
        return -1;
    }

    set_interface_attribs(serialfd, B19200);

    lseek(serialfd, 0, SEEK_END);

    return 0;
}

uint8_t serial_read(uint8_t * buffer, uint8_t size)
{
    size_t ret = 0;

    ret = read(serialfd, buffer, size);

    return ((ret < 0) | (ret != size)) ? 0 : size;
}

uint8_t serial_write(uint8_t * buffer, uint8_t size)
{
    size_t ret = 0;

    ret = write(serialfd, buffer, size);

    return ((ret < 0) | (ret != size)) ? 0 : size;
}

void serial_close()
{
    close(serialfd);
}

