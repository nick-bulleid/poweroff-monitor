#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void poll_pin() {
    struct pollfd fdlist[1];
    int fd;
    char buf[3];

    buf[2] = '\0';

    // TODO - set the edge of the gpio to "both"

    fd = open("/sys/class/gpio/gpio6/value", O_RDONLY);
    read(fd, buf, 2);
    fdlist[0].fd = fd;
    fdlist[0].events = POLLPRI;

    while (1) {
        int err;

        err = poll(fdlist, 1, -1);
        if (err == -1) {
            perror("poll");
            exit(1);
        }

        if (fdlist[0].revents & POLLPRI) {
            printf("event on poweroff pin\n");
            lseek(fdlist[0].fd, 0, SEEK_SET);
            read(fdlist[0].fd, buf, 2);

            // TODO - debounce input and call shutdown
        }
    }
}

int main()
{
    poll_pin();
}
