poweroff-monitor: poweroff-monitor.o
	gcc -o poweroff-monitor poweroff-monitor.o

poweroff-monitor.o: poweroff-monitor.c
	gcc -c poweroff-monitor.c
