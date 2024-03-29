#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h> 
#include <string.h>
#include <errno.h>


static int set_interface_attribs(int fd, int speed, int parity)
{
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        printf("error %d from tcgetattr\n", errno);
        return -1;
    }
    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("error %d from tcsetattr\n", errno);
        return -1;
    }
    return 0;
}

static void set_blocking(int fd, int should_block)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0) {
        printf("error %d from tggetattr\n", errno);
        return;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("error %d setting term attributes\n", errno);
    }
}


static char *portname = "/dev/ttyACM0";


int main (int argc, char **argv)
{
    int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("error %d opening %s: %s\n", errno, portname, strerror(errno));
        exit(1);
    }
    set_interface_attribs(fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking(fd, 0);                    // set no blocking
    write(fd, "version\n", 8);             
    usleep((8 + 25) * 100);
    char buf [100];
    int n = read(fd, buf, sizeof(buf) - 1);     // read up to 100 characters if ready to read
    if (n <= 0) {
        printf("error got no answer\n");
        exit(1);
    }
    buf[n - 1] = '\0';
    printf("got: '%s' (%i)\n", buf, n);
    close(fd);
    return 0;
}
