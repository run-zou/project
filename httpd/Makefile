PWD=$(shell pwd)
CGI_PATH=$(PWD)/cgi_bin ${PWD}/sql_connect
SER_BIN=httpd
SER_SRC=httpd.c
CLI_BIN=client
CLI_SRC=client.c
POOL_SRC=pthread_pool.c
INCLUDE= -I /usr/libevent/include -I./pool
LIB=-L /usr/libevent/lib/ 
CC=gcc
FLAGS=-o
LDFLAGS=-lpthread -levent #-static

#使用libevent版本，epoll版本或者多线程版本
#USE=USE_LIBEVENT
USE=USE_EPOLL 
#USE=UES_THREAD    

.PHONY:all
all:$(SER_BIN) $(CLI_BIN) cgi
$(SER_BIN):$(SER_SRC) $(POOL_SRC)
	@$(CC) $(FLAGS) $@ $^ $(MYINCLUDE) $(MYLIB) $(MYFLAG) $(LDFLAGS) -D${USE} -g  #-D_DEBUG_ -g
$(CLI_BIN):$(CLI_SRC)
	@$(CC) $(FLAGS) $@ $^ $(LDFLAGS) #-D_DEBUG_

.PHONY:cgi
cgi:
	@for name in `echo $(CGI_PATH)`;\
		do\
		cd $$name;\
		make;\
		cd -;\
		done

.PHONY:output
output:all
	@mkdir  output
	@mkdir -p output/htdocs/cgi_bin
	@cp httpd output
	@cp client output
	@cp -rf conf output
	@cp -rf log output
	@cp start.sh output
	@cp -rf cgi_bin output
	@cp -rf htdocs/* output/htdocs
	@for name in `echo $(CGI_PATH)`;\
			do\
			cd $$name;\
			make output;\
			cd -;\
			done

PHONY:clean
clean:
	rm -rf $(SER_BIN) $(CLI_BIN) output
	@for name in `echo $(CGI_PATH)`;\
			do\
			cd $$name;\
			make clean;\
			cd -;\
			done
