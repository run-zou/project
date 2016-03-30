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

static void Bad_request(client)
{
	print_debug("enter our fault...\n");
	char buf[1024];
	sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>your enter message is a bad request</p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}
static void Unauthorized(client)
{}
static void Not_found(client)
{
	char buf[1024];
	sprintf(buf, "HTTP/1.0 404 NOT_FOUND\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>source is not found </p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}
static void Forbidden(client)
{
	char buf[1024];
	sprintf(buf, "HTTP/1.0 403 Forbidden\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>server Forbidden </p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}
static void Server_error(client)
{
	char buf[1024];
	sprintf(buf, "HTTP/1.0 500 Server error\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>server server error </p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}
static void Unavailable(client)
{
	char buf[1024];
	sprintf(buf, "HTTP/1.0 503 Unavailable\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>server unavailable </p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}
	
void echo_error_to_client(int client, int error_status)
{
	switch(error_status)
	{
		case 200: //ok 客户端请求成功
				break;
		case 400: //Bad Request  客户端请求有语法错误，不能被服务器所理解
			Bad_request(client);
				break;
		case 401: //Unauthorized 请求未经授权
			Unauthorized(client);
				break;
		case 403: // Forbidden  服务器收到请求，但是拒绝提供服务
			Forbidden(client);
				break;
		case 404: // Not Found  请求资源不存在，eg：输入了错误的URL
			Not_found(client);
				break;
		case 405: //用户在Request-Line字段定义的方法不允许
				break;
		case 406: //根据用户发送的Accept拖，请求资源不可访问
				break;
		case 407: //类似401，用户必须首先在代理服务器上得到授权
				break;
		case 408: //客户端没有在用户指定的饿时间内完成请求
				break;
		case 409: //对当前资源状态，请求不能完成
				break;
		case 410: //服务器上不再有此资源且无进一步的参考地址
				break;
		case 411: //服务器拒绝用户定义的Content-Length属性请求
				break;
		case 412: //一个或多个请求头字段在当前请求中错误
				break;
		case 413: //请求的资源大于服务器允许的大小
				break;
		case 414: //请求的资源URL长于服务器允许的长度
				break;
		case 415: //请求资源不支持请求项目格式
				break;
		case 416: //请求中包含Range请求头字段，在当前请求资源范围内没有range指示值，请求也不包含If-Range请求头字段
				break;
		case 417: //服务器不满足请求Expect头字段指定的期望值，如果是代理服务器，可能是下一级服务器不能满足请求长。
				break;
		case 500: // Internal Server Error 服务器发生不可预期的错误
			Server_error(client);
				break;
		case 501: // Error 未实现
				break;
		case 502: // HTTP 网关错误
				break;
		case 503: // Server Unavailable 服务器当前不能处理客户端的请求，一段时间后可能恢复正常
			Unavailable(client);
				break;
		default:
				break;
	}
}

//访问html
void echo_html(int client, const char *path, unsigned int file_size )
{
		if(path == NULL)
		{ return;}
		//以只读方式打开url指定的资源 
		int in_fd = open(path,O_RDONLY);
		if(in_fd < 0)
		{
			print_debug("open index.html failed");
			perror("open");
			return ;
		}
		print_debug("open index.html success");
		//给浏览器发送响应报文,这样浏览器就可以解释浏览器页面
		char echo_line[1024];
		memset(echo_line,'\0',sizeof(echo_line));
		strncpy(echo_line, HTTP_VERSION, strlen(HTTP_VERSION)+1);
		strcat(echo_line, " 200 Ok");
		strcat(echo_line, "\r\n\r\n");
		send(client, echo_line, strlen(echo_line), 0);
		print_debug("send echo head success");
		//sendfile函数比write，send高效，减少了拷贝的次数（避免了从内核取到用户态的复杂拷贝过程) 注意第二个参数
		if ( sendfile(client, in_fd, NULL, file_size) < 0 )	
		{//将资源发送给客户端
			print_debug("send_file failed");
			echo_error_to_client(client, 400);
			close(in_fd);
			return;
		}
		print_debug("sendfile success");
		close(in_fd);
}

void clear_header(int client)
{ //清除一行消息
		char buf[1024];
		memset(buf, '\0', sizeof(buf));
		int ret = 0;	
		do
		{
			ret = get_line(client,buf,sizeof(buf));

		}while(ret>0 && strcmp(buf,"\n") != 0);
}

int get_line(int sock, char *buf, int max_len)
{//获取一行信息
	if( !buf || max_len <=0 )
	{ 
		return 0; 
	}
	int i = 0;
	int n = 0;
	char c = '\0';	
	while( i < (max_len-1) && c != '\n')
	{
		n = recv(sock, &c, 1, 0);
		if( n > 0)
		{//success,统一不同平台的回车换行符
			if(c == '\r')
			{
				n = recv(sock, &c, 1, MSG_PEEK); //嗅探(MSG_PEEK参数-- 只是探测一下并没有将数据读出来)
				if(n > 0 && c == '\n')
				{//windows平台下风格
					print_debug("c == /n");
					recv(sock, &c, 1, 0);  //delete					
				}
				else
				{//其他平台风格
					print_debug("other paltform");
					c = '\n';
				}
			}
			print_debug("buf[i++]==c");
			buf[i++] = c;//将值读到buf中
		}
		else
		{//falied
			print_debug("failed /n");
			c = '\n';
		}
		buf[i]= '\0'; //结尾补上‘\0’，标志结束
	}
	printf("%s",buf);
	return i;  //返回字符个数
}

//执行cgi
void exe_cgi(int sock_client, const char *path, const char *method, const char *query_string)
{//cgi
	char buf[_COMM_SIZE_];
	int numchars = 0;
	int content_length = -1;//存放数据长度
	pid_t id;  //fork

	//将cgi程序的输出显示到浏览器上(重定向可以完成)，利用管道提供可靠服务
	int cgi_input[2] = {0, 0};  
	int cgi_output[2] = {0, 0};

	if( strcasecmp(method,"GET") == 0 )	
	{//GET方法
		clear_header(sock_client);
	}
	else
	{//POST方法
		do
		{	
			//读出Content-length	
			memset(buf, '\0', sizeof(buf));
			numchars = get_line(sock_client, buf, sizeof(buf));
			if(strncasecmp(buf, "Content-Length:",strlen("Content-Length:")) == 0 )
			{
				content_length = atoi(&buf[16]);  //拿出数据长度
			}

		}while( numchars > 0 && strcmp(buf,"\n") != 0 );	
			
		if( content_length == -1)
		{//content-length字段出错
			echo_error_to_client(sock_client, 500);
			return ;			
		}	
	}
	//给浏览器发送响应报文	
	memset(buf, '\0', sizeof(buf));
	strcpy(buf,HTTP_VERSION);
	strcat(buf," 200 OK\r\n\r\n");
	send(sock_client, buf, strlen(buf), 0);

	//创建input管道
	if( pipe(cgi_input) < 0 )
	{//failed
		close(cgi_input[0]);
		close(cgi_input[1]);
		echo_error_to_client(sock_client,500);
		return ;
	}
	//创建output管道
	if( pipe(cgi_output) < 0 )
	{
		close(cgi_output[0]);	
		close(cgi_output[1]);	
		echo_error_to_client(sock_client,500);
		return ;
	}

	//下面代码的逻辑大概是这样的:
	//1.创建双向通信管道(两个匿名管道)，子进程关闭input的写端,output的读端；父进程关闭input的读端,output的写端;
	//2.在子进程中还将inuput管道的读端重定向为标准输出，将output的写端重定向为标准输入；
	//3.重定向后父进程从套接字读取的数据被子进程(浏览器)读走,而浏览器输入的数据则交给父进程(服务器),这样就实现了服务器与浏览器的通信
	if( (id = fork()) < 0)
	{
		close(cgi_input[0]);
		close(cgi_input[1]);		
		close(cgi_output[0]);	
		close(cgi_output[1]);	
		echo_error_to_client(sock_client,500);
	}
	else if( id == 0)
	{//子进程
		//程序替换时，环境变量并不替换,可以将一些以后需要使用的数据导入环境变量里面
		char method_env[_COMM_SIZE_];
		memset(method_env, '\0', sizeof(method_env));
		char query_env[_COMM_SIZE_ / 10];
		memset(query_env, '\0', sizeof(query_env));
		char content_len_env[_COMM_SIZE_ / 10];
		memset(content_len_env, '\0', sizeof(content_len_env));

		close(cgi_input[1]);  //关闭写端--》标准输入
		close(cgi_output[0]); //关闭读端--》标准输出
		//重定向
		dup2(cgi_input[0], 0); 
		dup2(cgi_output[1], 1);
		//将需要重用的参数传到环境变量里面,因为程序替换时环境变量没有被替换
		sprintf(method_env, "REQUEST_METHOD=%s",method);
		putenv(method_env); //将‘方法’导入到环境变量里面
				
		if( strcasecmp("GET",method) == 0 )
		{//GET
			sprintf(query_env, "QUERY_STRING=%s",query_string);
			putenv(query_env); //将‘参数’导入到环境变量里面
		}
		else
		{//POST
			sprintf(content_len_env, "CONTENT_LENGTH=%d",content_length);
			putenv(content_len_env);//将‘长度’导入环境变量
		}

		//程序替换
		execl(path, path, NULL);  //exec函数通常情况没有返回值，若有则必定是失败了
		exit(1);
	}
	else
	{//父进程				
		close(cgi_input[0]);  //关闭读端
		close(cgi_output[1]);//关闭写端
		int i=0;
		char c='\0';
		if( strcasecmp("POST",method) == 0 )
		{//从sock里面拿数据，写到管道里面
			for(; i < content_length; ++i)
			{//从套接字里面读出数据，将它写到cgi_intput里面
				recv(sock_client, &c, 1, 0);
				write(cgi_input[1], &c, 1);
			}
		}
		
		while( read(cgi_output[0],&c,1) > 0 )
		{//从output里面读取数据，输出(重定向)
			send(sock_client, &c, 1, 0);
		}
		//完成后关闭管道剩余开口
		close(cgi_input[1]);
		close(cgi_output[0]);
		waitpid(id, NULL, 0);
	}
}

void* accept_request(void *arg)
{
	pthread_detach(pthread_self()); //线程分离
	int cgi = 0; //cgi
	int sock_client = (int)arg; //sock
	char * query_string = NULL; //用于提取参数
	char method[_COMM_SIZE_ / 10]; //用于提取方法
	memset(method, '\0', sizeof(method));
	char url[_COMM_SIZE_]; //用于提取url
	memset(url, '\0', sizeof(url));
	char buffer[_COMM_SIZE_/10]; //读取数据
	memset(buffer, '\0', sizeof(buffer));
	char path[_COMM_SIZE_]; //路径
	memset(path, '\0', sizeof(path));
#ifdef _DEBUG_ 
	while( get_line(sock_client, buffer, sizeof(buffer)) > 0 )
	{
		printf("%s",buffer);
		fflush(stdout);
		printf("\n");
	}
#endif
	if( get_line(sock_client, buffer, sizeof(buffer)) < 0 )
	{
		echo_error_to_client(sock_client,500);
		return NULL;
	}
	//http请求的状态行被读取到buf数组里面，接下来可以从中取出‘方法’，‘url’,以及‘参数(query)'
	print_debug("data exit!");
	int i = 0;
	int j = 0;
	//提取方法
	while( !isspace(buffer[j]) && i<sizeof(method)-1 && j<sizeof(buffer) )
	{
		method[i] = buffer[j];
		i++;
		j++;
	}
	//检查提取的方法是否合理
	if( strcasecmp(method,"GET") && strcasecmp(method,"POST") )
	{
		echo_error_to_client(sock_client, 500);
		return NULL;
	}
	//清除多余空格
	while( isspace(buffer[j]) && j<sizeof(buffer))
	{
		j++;
	}
	//提取url
	i=0;
	while( !isspace(buffer[j]) && i<sizeof(url)-1 && j<sizeof(buffer) )
	{
		url[i++] = buffer[j++];
	}
	print_debug(method);
	print_debug(url);
	if( strcasecmp(method,"POST") == 0 )
	{//POST方法，需要cgi
		cgi = 1;
	}
	if( strcasecmp(method,"GET") == 0 )
	{//GET方法
		query_string = url;
		while(*query_string != '?' &&  *query_string != '\0' )
		{
			query_string++;
		}
		if( *query_string == '?')
		{//url = /xxx/xxx?val1=xxx&val2=xxx   //说明有参数，提取参数
			*query_string = '\0';
			 query_string++;	
		     cgi = 1;
		}
	}
	//将url拼接到path后面
	sprintf(path, "htdocs%s", url);

	if(path[strlen(path)-1] == '/')
	{//说明访问的是主页, 和 GET / HTTP/1.0 相似
		strcat(path, MAIN_PAGE);	
	}
	print_debug(path);
	//定义struct stat结构，用于测试path路径下文件的属性
	struct stat st; 
	if( stat(path, &st) < 0) 
	{//失败，不存在(需要清除资源)
		 print_debug("stat faild");
		 clear_header(sock_client);
		 echo_error_to_client(sock_client,403);
	}
	else
	{//存在
		print_debug("stat success");
		if(S_ISDIR(st.st_mode))
		{//目录，则将index.html拼接到path后面
			print_debug("isdir");
			strcat(path, "/");
			strcat(path, MAIN_PAGE);	
		}
		else if(st.st_mode & S_IXUSR || st.st_mode & S_IXGRP || st.st_mode & S_IXOTH)
		{//可执行的，有三种可执行权限中的一种，则需要交给cgi取执行
			cgi = 1;
		}
		else
		{
			//do nothing 
		}
		//根据cgi的值确定行为（采取何种方式）
		if(cgi)
		{//需要交给cgi执行
			print_debug("exe_cgi:");
			exe_cgi(sock_client, path, method, query_string);
		}
		else
		{//不需要执行cgi
			print_debug("echo_html");
			clear_header(sock_client);
			echo_html( sock_client, path, st.st_size);
		}
	}
	//结束后需要关闭套接字
	close(sock_client);
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
	//设置为ip地址可复用
	int flag = 1;
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &flag,sizeof(flag));
	//完成sockaddr_in参数
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

	return listen_sock; //返回一个监听套接字
}

void setnoblock(int newsock)
{//设置sock为非阻塞式
	int old = fcntl(newsock, F_GETFL); //获取文件描述符状态标志
	if(old < 0)
	{
		perror("tcntl");
		exit(1);
	}
	fcntl(newsock, F_SETFL, old | O_NONBLOCK); //置为非阻塞的
}


int main(int argc, char* argv[])
{
	if( argc != 2)
	{//exe + port
		Usage(argv[0]);
		exit(1);
	}
	short port = atoi(argv[1]); //port
	short sock = start(port); //创建一个监听套接字
	
	//使用多线程版本	
#ifdef UES_THREAD
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
		 //链接建立成功,创建线程执行
		 pthread_t new_thread;
		 pthread_create(&new_thread, NULL, accept_request, (void*)new_sock);
    }
#endif

   	//使用epoll多路复用处理就绪，使用多线程处理请求
#ifdef USE_EPOLL
	struct epoll_event ready_event[MAX_EVENT_NUM];
	struct epoll_event _event;
	_event.events = EPOLLIN;
	_event.data.fd = sock;
	int epollfd = epoll_create(MAX_EVENT_NUM);
	//添加sock上的注册事件
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &_event) < 0)
	{
		perror("epoll_ctl");
		exit(1);
	}
	setnoblock(sock);
	//开始处理事件
    while(1) 
    {
		//int timeout =8000; //-1时表示永不超时
		//等待注册事件的就绪
		int ready_num = epoll_wait(epollfd, ready_event, MAX_EVENT_NUM, -1); //永不超时（阻塞式等待）
		if(ready_num == 0)
		{
			print_debug("timeout!\n");
			break;
		}
		else if(ready_num == -1)
		{
			perror("epoll_wait");
			break;
		}
	
		int i=0;	
		for(; i<ready_num;++i)
		{
			int fd = ready_event[i].data.fd;
			if(fd == sock)
			{
				struct sockaddr_in client;
				socklen_t len =0;
				int new_sock = accept(sock, (struct sockaddr*)&client, &len);
				if(new_sock < 0)
				{
					perror("accept");
					continue;
				}
				print_debug("accept success\n");
	
				setnoblock(new_sock);//设置为非阻塞式，因为event使用了ET模式
				_event.events = EPOLLIN | EPOLLOUT | EPOLLET;
				_event.data.fd = new_sock;
				if(epoll_ctl(epollfd, EPOLL_CTL_ADD, new_sock, &_event) < 0)
				{//添加new_sock注册事件
					perror("epoll_ctl");
					close(new_sock);
					continue;
				}
	
				print_debug("re_epoll_ctl  success\n");
			}
			else if( (ready_event[i].events & EPOLLIN) && (ready_event[i].events & EPOLLOUT))
			{
				//printf("join this patr,fd is %d,ready_fd is %d \n",fd,ready_event[i].data.fd);
				pthread_t new_thread;
				if(pthread_create(&new_thread, NULL, accept_request, (void*)ready_event[i].data.fd) == 0)
				{
					print_debug("pthread success\n");
				}
				//删除fd上的注册事件	
				epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);	
			}
		}
    }
#endif

   return 0;
}

