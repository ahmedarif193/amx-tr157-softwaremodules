#!/bin/sh
[ -f /etc/environment ] && source /etc/environment
ulimit -c ${ULIMIT_CONFIGURATION:0}
name="tr181-softwaremodules"

case $1 in
    start|boot)
	${name} -D
        ;;
    stop|shutdown)
        if [ -f /var/run/${name}.pid ]; then
            kill `cat /var/run/${name}.pid`
        else
            killall ${name}
	fi
        ;;
    debuginfo)
        ubus-cli "SoftwareModules.?"
        ;;
    restart)
        $0 stop
        $0 start
        ;;
    *)
        echo "Usage : $0 [start|boot|stop|shutdown|debuginfo|restart]"
        ;;
esac

