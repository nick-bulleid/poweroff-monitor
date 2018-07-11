#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NANOS 1000000000LL
#define DEBOUNCE_INTERVAL 100000L

#define GPIO_PATH "/sys/devices/platform/power_ctrl/poweroff-gpio"

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
    struct timespec ts;

    unsigned char state;
    long long now, down_time;

    buf[2] = '\0';
    state = 0;

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
            lseek(fdlist[0].fd, 0, SEEK_SET);
            read(fdlist[0].fd, buf, 2);

            // debounce input
            clock_gettime(CLOCK_MONOTONIC,  &ts);
            now = ts.tv_sec * NANOS + ts.tv_nsec;

            if ((state == 0) && (buf[0] == '1')) {
                down_time = now;
                state = 1;
            } else if ((state == 1) && (buf[0] == '0')) {
                if ((now - down_time) > DEBOUNCE_INTERVAL) {
                    state = 0;
                    system("shutdown -P now");
                }
            }
        }
    }
}

int main()
{
    set_edge();
    poll_pin();
}
