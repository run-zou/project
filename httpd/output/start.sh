#!/bin/bash

WORK_PATH=$(pwd)
HTTPD_CONF=$WORK_PATH/conf/httpd.conf
HTTPD=$WORK_PATH/httpd

firewall_switch=$(grep -Ei 'FIREWALL_SWITCH' ${HTTP_CONF} | awk -F':' '{print $NF}' | tr -d ' ')
httpd_port=$(grep -Ei 'HTTPD_PORT' ${HTTPD_CONF} | awk -F':' '{print $NF}' | tr -d ' ')

if [ "X$firewall_switch" == "XYES"];then
		service iptables stop
else
		service iptables start
fi

echo $firewall_switch
echo $httpd_port

./httpd 8080
