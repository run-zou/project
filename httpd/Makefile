PWD=$(shell pwd)
SER_BIN=httpd
CLI_BIN=demo_client
SER_SRC=httpd.c
CLI_SRC=demo_client.c
INCLUDE=.
CC=gcc
FLAGS=-o
LDFLAGS=#-static
LIB=


.PHONY:all
all:$(SER_BIN) $(CLIENT_BIN)

$(SER_BIN):$(SER_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS)

$(CLI_BIN):$(CLI_SRC)
	$(CC) $(FLAGS) $@ $^ $(LDFLAGS)

.PHONY:clean
clean:
	rm -f $(SER_BIN) $(CLI_BIN)

