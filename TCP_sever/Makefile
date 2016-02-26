CLI=client
SER=server

.PHONY:all
all:$(SER) client
$(SER):server.c
	gcc  -o $@ $^ -g
$(CLI):client.c
	gcc  -o $@ $^ -g

.PHONY:clean
clean:
	rm $(CLI) $(SER)
