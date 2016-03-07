WORK_PATH=${shell pwd}

BIN=sql_connect
CC=g++
INCLUDE=-I${WORK_PATH}/mysql_lib/include  -L${WORK_PATH}/mysql_lib/lib -lmysqlclient
#LIB=-lmysqlclient
#LIB_PATH=-L ${WORK_PATH}/mysql_lib/lib
SRC=sql_connect.cpp


${BIN}:${SRC}
	${CC} -o $@ $^ ${INCLUDE} ${LIB_PATH} ${LIB} -g

.PHONY:clean
clean:
	rm ${BIN} 

#导入动态库寻找路径
#export LD_LIBRARY_PATH=/home/admin/mycode/project/httpd/sql_connect/mysql_lib/lib

