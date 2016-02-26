#include "httpd.h"

void Usage(const char* proc)
{
	printf("%s [ip] [port]\n",proc);
}

void print_log(const char* fun, int line, int err_no, const char* err_str)
{

}

// 
int start(short port)
{
	
	int listen_sock	 = socket(AF_INET, SOCK_STREAM, 0);	
	if(listen_sock == -1)
	{
		print_log(__FUNCTION__, __LINE__, errno, strerror(errno)) ;
		exit(1);
	}

	//sucess
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	loacl.sin_port = htons(port);
	loacl.sin_addr = INADDR_ANY; //

  //bind
	socklen_t len =sizeof(loacl);
	if( bind(listen_sock, (struct sockaddr*)&local, len) == -1 )
	{
		print_log(__FUNCTION__, __LINE__, errno, strerror(errno)) ;
		exit(2);
	}
	
  //listen
	if( listen(listen_sock, _BACK_LOG) == -1 )
	{
		print_log(__FUNCTION__, __LINE__, errno, strerror(errno)) ;
		exit(3);	
	}

	return listen_sock;

}


int main(int argc, char* argv[])
{

	if( argc != 3)
	{
		Usage(argv[0]);
		exit(1);
	}

	short port = atoi(argv[2])
	//printf("prot:%d\n",prot);

	int sock=start(port);



	return 0;
}
