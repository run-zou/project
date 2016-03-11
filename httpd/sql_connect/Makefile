WORK_PATH=${shell pwd}

BIN=sql_connect
CGI_BIN=cgi_stu
SRC=sql_connect.cpp
CGI_SRC=cgi_student.cpp

CC=g++
INCLUDE=-I${WORK_PATH}/mysql_lib/include 
LIB_PATH=-L ${WORK_PATH}/mysql_lib/lib
LIB=-lmysqlclient
DEBUG=-D_DEBUG__

#.PHONY:all
#all:${BIN} ${CGI_BIN}

#${BIN}:${SRC}
#	${CC} -o $@ $^ ${INCLUDE} ${LIB_PATH} ${LIB} ${DEBUG} -g

${CGI_BIN}:${CGI_SRC} ${SRC}
	${CC} -o $@ $^	${INCLUDE} ${LIB_PATH} ${LIB} #${DEBUG} -g

.PHONY:output
output:
	cp cgi_stu ../output/htdocs/cgi_bin

.PHONY:clean
clean:
	rm ${CGI_BIN}
	#rm ${BIN} 






