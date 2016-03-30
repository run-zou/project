#ifndef _HTTPD_H_
#define _HTTPD_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define _BACK_LOG 5                //最大TCP链接数目
#define _COMM_SIZE_ 1024           //
#define MAIN_PAGE "index.html"     //主页
#define HTTP_VERSION "HTTP/1.0"    //协议版本
#define MAX_EVENT_NUM 1024         //最大就绪数目

#endif //_HTTPD_H_
