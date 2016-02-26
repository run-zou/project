#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

static void usage()
{
	printf("./client [remote_ip] [remote_port]\n");
}



int main(int argc ,char* argv[])
{
	if( argc != 3)
	{
		usage();
		return 1;
	}
	short port =atoi(argv[2]);

	int conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(conn_fd < 0)
	{
		perror("sock");
		return 1;
	}
	
	
	struct sockaddr_in remote;
	remote.sin_family= AF_INET;
	remote.sin_port = htons(port);
	inet_aton(argv[1],&remote.sin_addr);

	if( connect(conn_fd, (struct sockaddr*)&remote, sizeof(remote)) == -1)
	{
		perror("connect");
		close(conn_fd);
		return 0;
	}

	printf("connect success ...Tcp link is bulied...\n");
	char buf[1024];
	while(1)
	{

		memset(buf, '\0' ,sizeof(buf));
		printf("please Enter:>");
		
		gets(buf);
		if(strncasecmp("quit", buf, 4)==0 )
		{
			break;
		}
		fflush(stdout);

		write(conn_fd, buf, strlen(buf));

		ssize_t sz = read(conn_fd, buf, sizeof(buf));
		if(sz > 0)
		{
			buf[sz] = '\0';
			printf("echo data: %s\n",buf);
		}
	}

	return 0;	
}
