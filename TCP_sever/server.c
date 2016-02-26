#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

static void usage()
{
	printf("Usage: ./server [ip] [port]\n");
}

int start(char* ip,short port) //duankou
{
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_fd == -1)
	{//error
		perror("socket");
		return -1;
	}

	struct sockaddr_in local; //IPv4结构
	//bzero(&local, sizeof(local)); //清0
	local.sin_family = AF_INET;  //address family
	local.sin_port = htons(port); //port : host -> net
	inet_aton(ip,&local.sin_addr.s_addr); //ip:xxx:xxx:xxx
	
	if( bind(socket_fd, (struct sockaddr*)&local, sizeof(local) ) == -1 )
	{
		perror("bind");
		return -1;
	}

	if( listen(socket_fd, 5) == -1)  //返回一个监听socket
	{//error
		perror("listen");
		return -1;
	}

	return socket_fd;
}


int main(int argc,char* argv[])
{
    if(argc != 3)
	{
		usage();
		return 1;	
	}

	short port = atoi(argv[2]); //string->int
	
	int listen_sock = start(argv[1],port);
	
	if(listen_sock == -1)
	{
		perror("start");
		return 1;
	}

	while(1)
	{
		struct sockaddr_in remote;
		memset(&remote, '\0' ,sizeof(remote));
		socklen_t len = sizeof(remote);

		int new_sock = accept(listen_sock, (struct sockaddr*)&remote, &len);  //accept出错的返回值为 -1     if error, return -1
		
		if(new_sock == -1) 
		{ //error
			perror("accept");
		}
		else
		{
			while(1)
			{
				char buf[1024];
				memset(buf, '\0',sizeof(buf));
				ssize_t sz = read(new_sock, buf , sizeof(buf)-1);

				if(sz > 0)
				{//read data and remote is exit ...
				
						buf[sz] = '\0';
						write(new_sock, buf, strlen(buf)); //回写
						printf("%s \n",buf);
				}
				else if(sz == 0)
				{//read the end
						printf("client is close\n");
						close(new_sock);
						break;
				}
				else
				{
					//do nothing
						break;
				}
			}
		}
	}

	return 0;
}
