WORK_PATH=${shell pwd}

BIN=sql_connect
CGI_STU=cgi_stu
SRC=sql_connect.cpp
CGI_SRC=cgi_student.cpp

CC=g++
INCLUDE=-I${WORK_PATH}/mysql_lib/include # -L${WORK_PATH}/mysql_lib/lib -lmysqlclient
LIB=-lmysqlclient
LIB_PATH=-L ${WORK_PATH}/mysql_lib/lib
DEBUG=-D_DEBUG__

#.PHONY:all
#all:${BIN} ${CGI_STU}

#${BIN}:${SRC}
#	${CC} -o $@ $^ ${INCLUDE} ${LIB_PATH} ${LIB} ${DEBUG} -g

${CGI_STU}:${CGI_SRC} ${SRC}
	${CC} -o $@ $^	${INCLUDE} ${LIB_PATH} ${LIB} #${DEBUG} 

.PHONY:output
output:
	cp cgi_stu ../output/htdocs/cgi_bin

.PHONY:clean
clean:
	rm ${CGI_STU}
	#rm ${BIN} 

#导入动态库寻找路径,环境变量在每次重新打开终端的时候都需要export一下
#export LD_LIBRARY_PATH=/home/admin/mycode/project/httpd/sql_connect/mysql_lib/lib


#注意添加动态库的查找路径
#注意开启 mysql 服务

