#include "client.h"

void Usage(const char * proc)
{
	printf("Usage:%s [ip][port]\n");
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		Usage(argv[0]);
	}
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		perror("socket");
		exit(1);
	}
	short port = atoi(argv[2]);
	struct sockaddr_in remote_server;
	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons(port);
	remote_server.sin_addr.s_addr = inet_addr(argv[1]);
	//connect
	if( connect(sock, (struct sockaddr*)&remote_server, sizeof(remote_server)) < 0 )
	{
		perror("connect");
		exit(2);
	}
	printf("connect success!\n");
	char cmd[64] = "GET / HTTP/1.0\r\n";
	//printf("please int your cmd!\n");
	//gets(cmd);
	int len = strlen(cmd);
	printf("cmd:%s\n",cmd);
	ssize_t size = send(sock, cmd, len, 0);
	if(size <= 0)
	{
		perror("send");
		close(sock);
		return 1;
	}
	printf("send success!\n");
	char buf[1024];
	memset(buf, '\0', sizeof(buf));
	while( recv(sock, buf, sizeof(buf)-1, 0) > 0)
	{
		printf("recv success! \n");
		printf("%s",buf);
		fflush(stdout);
		memset(buf, '\0', sizeof(buf));
	}
	printf("\n");
	close(sock);
	return 0;
}
