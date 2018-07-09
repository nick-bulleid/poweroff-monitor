poweroff-monitor: poweroff-monitor.o
	gcc -o poweroff-monitor poweroff-monitor.o

poweroff-monitor.o: poweroff-monitor.c
	gcc -c poweroff-monitor.c

.PHONY: install
install: poweroff-monitor
	mkdir -p /usr/local/sbin
	cp  $< /usr/local/sbin/poweroff-monitor
	mkdir -p /etc/systemd/system
	cp poweroff-monitor.service /etc/systemd/system/poweroff-monitor.service

.PHONY: uninstall
uninstall:
	rm -f /usr/local/sbin/poweroff-monitor
	rm -f /etc/systemd/system/poweroff-monitor.service