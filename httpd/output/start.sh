#!/bin/bash

WORK_PATH=$(pwd)
HTTPD_CONF=$WORK_PATH/conf/httpd.conf

firewall_switch=$(grep -Ei 'FIREWALL_SWITACH' ${HTTP_CONF} | awk -F':' '{print $NF}')
httpd_port=$(grep -Ei 'HTTPD_PORT' ${HTTPD_CONF} | awk -F':' '{print $NF}')

echo $firewall_switch
echo $httpd_port

./httpd 8080
