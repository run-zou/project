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
#	rm ${BIN} 


#1.注意开启 mysql 服务
#2.注意添加动态库的查找路径
# 导入动态库寻找路径,环境变量在每次重新打开终端的时候都需要export一下,或者写进ld.so.conf文件里面
#export LD_LIBRARY_PATH=/home/admin/mycode/project/httpd/sql_connect/mysql_lib/lib
