WORK_PATH=${shell pwd}

BIN=sql_connect
CC=g++
INCLUDE=-I ${WORK_PATH}/mysql_lib/include -I.
LIB_PATH=-L ${WORK_PATH}/mysql_lib/lib
SRC=sql_connect.cpp


${BIN}:${SRC}
	${CC} -o $@ $^ ${INLUCDE} ${LIB_PATH}

.PHONY:clean
clean:
	rm ${BIN} 
