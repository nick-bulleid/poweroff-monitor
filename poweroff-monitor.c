#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GPIO_PATH "/sys/class/gpio/gpio6"

void set_edge() {
    FILE *fp;

    fp = fopen(GPIO_PATH "/edge", "w");
    if (!fp)
    {
        perror("set_edge");
        exit(1);
    }

    fprintf(fp, "both\n");
    fclose(fp);
}

void poll_pin() {
    struct pollfd fdlist[1];
    int fd;
    char buf[3];

    buf[2] = '\0';

    fd = open(GPIO_PATH "/value", O_RDONLY);
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
    set_edge();
    poll_pin();
}
