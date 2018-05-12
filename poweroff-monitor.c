#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>

void poll_pin() {
    struct pollfd fdlist[1];
    int fd;

    fd = open("/sys/class/gpio/gpio26/value", O_RDONLY);
    fdlist[0].fd = fd;
    fdlist[0].events = POLLPRI;

    while (1) {
        int err;
        char buf[3];

        err = poll(fdlist, 1, -1);
        if (err == -1) {
            perror("poll");
            return;
        }

        err = read(fdlist[0].fd, buf, 2);
        printf("event on poweroff pin\n");
    }
}

int main()
{
    poll_pin();
}
