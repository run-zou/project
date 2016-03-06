#include "httpd.h"

void Usage(const char* proc)
{
	printf("%s [ip] [port]\n",proc);
}

void print_log(const char* fun, int line, int err_no, const char* err_str)
{
	printf("[%s: %d][%d][%s]\n",fun,line,err_no,err_str);
}

void print_debug(const char * str)
{
#ifdef _DEBUG_
	printf("%s\n",str);
#endif
}

void echo_error_to_client()
{

	return ;
}


void echo_html(int client, const char *path, unsigned int file_size )
{
		if(path == NULL)
		{ return;}

		int in_fd = open(path,O_RDONLY);
		if(in_fd < 0)
		{
				print_debug("open index.html failed");
			echo_error_to_client();
			return ;
		}
	
			print_debug("open index.html success");
		char echo_line[1024];
		memset(echo_line,'\0',sizeof(echo_line));

		strncpy(echo_line, HTTP_VERSION, strlen(HTTP_VERSION)+1);
		strcat(echo_line, " 200 Ok");
		strcat(echo_line, "\r\n\r\n");

		send(client, echo_line, strlen(echo_line),0);
			print_debug("send echo head success");


		//比write，send高效，减少了拷贝的次数（避免了从内核取到用户态的复杂拷贝过程）	
		if ( sendfile(client, in_fd, NULL, file_size) < 0 )	
		{
				print_debug("send_file failed");
			echo_error_to_client();
			close(in_fd);
			return;
		}
	
			print_debug("sendfile success");

		close(in_fd);
}


void clear_header(int client)
{
		char buf[1024];
		memset(buf, '\0', sizeof(buf));
		int ret = 0;
		do{
			ret = get_line(client,buf,sizeof(buf));
		}
		while(ret>0 && strcmp(buf,"\n") != 0);
}


//return > 0,success ;return <=0,failed
int get_line(int sock, char *buf, int max_len)
{

//print_debug("get line");
		if( !buf || max_len <=0 )
		{ return 0; }
		
		int i = 0;
		int n = 0;
		char c = '\0';
		while( i < (max_len-1) && c != '\n')
		{

//						print_debug("it is ok");

				n = recv(sock, &c, 1, 0);
//						printf("%c\n",c);	
//						printf("%d\n",n);
//						print_debug("recv success");

				if( n > 0)
				{//success,统一不同平台的回车换行符
					

//							print_debug("come in");
					if(c == '\r')
					{
					
//								print_debug("c == /r");
						
							n = recv(sock, &c, 1, MSG_PEEK); //嗅探(MSG_PEEK参数-- 只是探测一下并没有读出来)
						if(n > 0 && c == '\n')
						{//windows
				
//									print_debug("c == /n");
							recv(sock, &c, 1, 0);  //delete
							
						}
						else
						{//other platform
					
//									print_debug("other paltform");
							c = '\n';

						}
					}
						
//						print_debug("buf[i++]==c");
					//将值读到buf中
					buf[i++] = c;
				}
				else
				{//falied
					c = '\n';
					
//						print_debug("failed /n");
				}
	
		}

		buf[i]= '\0';


//print_debug("get line over");

		return i;  //返回字符个数
}


void exe_cgi(int sock_client, const char *path, const char *method, const char *query_string)
{//cgi

		char buf[_COMM_SIZE_];
		int numchars = 0;
		int content_length = -1;

		//pipe
		int cgi_input[2] = {0, 0};  
		int cgi_output[2] = {0, 0};

		pid_t id;

		if( strcasecmp(method,"GET") == 0 )	
		{//GET
				clear_header(sock_client);
		}
		else
		{//POST

			do{
					memset(buf, '\0', sizeof(buf));
					numchars = get_line(sock_client, buf, sizeof(buf));
					
					if(strncasecmp(buf, "Content-Length:",strlen("Content-Length:")) == 0 )
					{
						content_length = atoi(&buf[16]);  //拿出数据长度
					}

			  }while(numchars > 0 && strcmp(buf,"\n") != 0);	


			if( content_length == -1)
			{
				echo_error_to_client();
				return ;			
			}	
		}
		
		memset(buf, '\0', sizeof(buf));
		strcpy(buf,HTTP_VERSION);
		strcat(buf," 200 OK\r\n\r\n");
		send(sock_client, buf, strlen(buf), 0);

		if( pipe(cgi_input) < 0 )
		{//failed
			close(cgi_input[0]);
			close(cgi_input[1]);
			echo_error_to_client();
			return ;
		}

		if( pipe(cgi_output) < 0 )
		{
			close(cgi_output[0]);	
			close(cgi_output[1]);	
			echo_error_to_client();
			return ;
		}

	
		if( (id = fork()) < 0)
		{
			close(cgi_input[0]);
			close(cgi_input[1]);
			
			close(cgi_output[0]);	
			close(cgi_output[1]);	
			
			echo_error_to_client();
		}
		else if( id == 0)
		{//子进程读数据，

				//程序替换时，环境变量并不替换,可以将一些参数放入环境变量里面
				char method_env[_COMM_SIZE_];
				char query_env[_COMM_SIZE_ / 10];
				char content_len_env[_COMM_SIZE_ / 10];
				
				memset(method_env, '\0', sizeof(method_env));
				memset(query_env, '\0', sizeof(query_env));
				memset(content_len_env, '\0', sizeof(content_len_env));


				close(cgi_input[1]);  //close write
				close(cgi_output[0]); //close read
				

				//重定向
				dup2(cgi_input[0], 0); 
				dup2(cgi_output[1], 1);
				

				//将需要重用的参数传到环境变量里面
				sprintf(method_env, "REQUEST_METHOD=%s",method);
				putenv(method_env); //导入到环境变量里面

				if( strcasecmp("GET",method) == 0 )
				{
						sprintf(query_env, "QUERY_STRING=%s",query_string);
						putenv(query_env); //导入到环境变量里面
				}
				else
				{
						sprintf(content_len_env, "CONTENT_LENGTH=%d",content_length);
						putenv(content_len_env);
				}


				execl(path, path, NULL);  //没有返回值，若有则必定是失败了
				exit(1);
			
		}
		else
		{//父进程从socket里面拿出数据写到cgi_intput里面，让子进程读，然后再从另外一个管道cgi_output里面读出数据给浏览器
				
				close(cgi_input[0]);  //close read
				close(cgi_output[1]);//close write


				int i=0;
				char c='\0';
				if( strcasecmp("POST",method) == 0 )
				{//从sock里面拿数据，写到管道里面

						for(; i < content_length; ++i)
						{
							recv(sock_client, &c, 1, 0);
							write(cgi_input[1], &c, 1);
						}
				}

				while( read(cgi_output[0],&c,1) > 0 )
				{
					send(sock_client, &c, 1, 0);
				}

				close(cgi_input[1]);
				close(cgi_output[0]);

				waitpid(id, NULL, 0);
		}

}


void* accept_request(void *arg)
{
	pthread_detach(pthread_self()); //线程分离

	int sock_client = (int)arg;	

	int cgi = 0;

	char * query_string = NULL;
	
	char method[_COMM_SIZE_ / 10];   //用于提取方法
	memset(method, '\0', sizeof(method));

	char url[_COMM_SIZE_];      //用于提取url
	memset(url, '\0', sizeof(url));


	char buffer[_COMM_SIZE_/10];  
	memset(buffer, '\0', sizeof(buffer));

	char path[_COMM_SIZE_];
	memset(path, '\0', sizeof(path));

//#ifdef _DEBUG_ 
//	while( get_line(sock_client, buffer, sizeof(buffer)) > 0 )
//	{
//		printf("%s",buffer);
//		fflush(stdout);
//		printf("\n");
//	}
//#endif

	if( get_line(sock_client, buffer, sizeof(buffer)) < 0 )
	{
		echo_error_to_client();
		return NULL;
	}

//print_debug("data exit!");

	int i = 0;
	int j = 0;
	while( !isspace(buffer[j]) && i<sizeof(method)-1 && j<sizeof(buffer) )
	{//提取方法
		method[i] = buffer[j];
		i++;
		j++;
	}


	if( strcasecmp(method,"GET") && strcasecmp(method,"POST") )
	{//发放是否合理
		echo_error_to_client();
		return NULL;
	}

	while( isspace(buffer[j]) && j<sizeof(buffer))
	{//清除多余空格
		j++;
	}

	i=0;
	while( !isspace(buffer[j]) && i<sizeof(url)-1 && j<sizeof(buffer) )
	{//提取url
		url[i++] = buffer[j++];
	}

	print_debug(method);
	print_debug(url);

	if( strcasecmp(method,"POST") == 0 )
	{//POST方法，需要cgi
		cgi = 1;
	}

	if(strcasecmp(method,"GET") == 0)
	{//提取参数
		query_string = url;

		while(*query_string != '?' &&  *query_string != '\0' )
		{
			query_string++;
		}

		if( *query_string == '?')
		{//url == /xxx/xxx/ + arg   //说明有参数
			*query_string = '\0';
			 query_string++;	
		 	 cgi = 1;
		}
	}

	sprintf(path, "htdocs%s", url);
	if(path[strlen(path)-1] == '/')
	{//根目录
		strcat(path, MAIN_PAGE);	
	}

	print_debug(path);

	struct stat st; 

	if( stat(path, &st) < 0) 
	{//失败，不存在(需要清除资源)

			print_debug("stat faild");

			clear_header(sock_client);
			echo_error_to_client();
	}
	else
	{//存在

		print_debug("stat success");

		if(S_ISDIR(st.st_mode))
		{//目录
			
			print_debug("isdir");

			strcat(path, "/");
			strcat(path, MAIN_PAGE);	
		}
		else if(st.st_mode & S_IXUSR || st.st_mode & S_IXGRP || st.st_mode & S_IXOTH)
		{//可执行的
			cgi = 1;
		}
		else
		{}


		if(cgi)
		{
			print_debug("exe_cgi:");

			exe_cgi(sock_client, path, method, query_string);
		}
		else
		{
			clear_header(sock_client);
			
//			print_debug("echo_html");
			echo_html( sock_client, path, st.st_size);
		}
	}

	close(sock_client);

print_debug("pthread over");

	return NULL;
}



int start(short port)
{
	
	int listen_sock	 = socket(AF_INET, SOCK_STREAM, 0);	
	if(listen_sock == -1)
	{
		print_log(__FUNCTION__, __LINE__, errno, strerror(errno)) ;
		exit(1);
	}

	int flag = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &flag,sizeof(flag));  //端口复用（timewait）

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = htonl(INADDR_ANY); //自动绑定ip

	socklen_t len =sizeof(local);
  //bind
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
	if( argc != 2)
	{
		Usage(argv[0]);
		exit(1);
	}


	short port = atoi(argv[1]);


	short sock = start(port);

	struct sockaddr_in client;
	socklen_t len =0;


    while(1) 
    {
   		 int new_sock = accept(sock, (struct sockaddr*)&client, &len);	
		 if( new_sock < 0)
		 {

	 			 print_log(__FUNCTION__, __LINE__, errno, strerror(errno)) ;	 	
				 continue;
		 }

		print_debug("accept success!");

		 //sucess
		 pthread_t new_thread;
		 pthread_create(&new_thread, NULL, accept_request, (void*)new_sock);

    }
	
   
   return 0;
}
