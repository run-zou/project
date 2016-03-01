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

#define _BACK_LOG 5
#define _COMM_SIZE_ 1024
#define MAIN_PAGE "index.html"
#define HTTP_VERSION "HTTP/1.0"
#endif