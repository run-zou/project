PWD=$(shell pwd)
SER_BIN=httpd
SER_SRC=httpd.c

CLI_BIN=demo_client
CLI_SRC=demo_client.c
INCLUDE=.
CC=gcc
FLAGS=-o
LDFLAGS=-lpthread#-static
LIB=


.PHONY:all
all:$(SER_BIN) $(CLI_BIN)

$(SER_BIN):$(SER_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS) -D_DEBUG

$(CLI_BIN):$(CLI_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS) -D_DEBUG

.PHONY:clean
clean:
	rm -f $(SER_BIN) $(CLI_BIN)

