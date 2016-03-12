#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void plus(char *str)
{//简单加法器
	if( str == NULL)
	{	return ; }

	char *begin=str;
	char *data1=NULL;
	char *data2=NULL;
	//提取参数?val1=xx&val2=xx
	while(*begin != '\0')
	{
		if(*begin == '=' && data1==NULL)
		{
			data1=begin+1;
			begin++;
			if(*begin == '&')
			{
				*begin = '\0';
			}
		}
		else if(*begin == '=' && data1 != NULL && data2 == NULL)
		{
			data2=begin+1;
			begin++;
		}
		else
		{
			begin++;
		}
	}
	int val1 = atoi(data1);
	int val2 = atoi(data2);
	printf("%d\n",val1+val2);
}

int main()
{
	char method[1024];
	memset(method, '\0', sizeof(method));
	char query_string[1024];
	memset(query_string, '\0', sizeof(query_string));
	char post_data[4096];
	memset(post_data, '\0', sizeof(post_data));
	int content_length = -1;
	//导入环境变量，从里面获取所需要信息
	strcpy(method, getenv("REQUEST_METHOD"));
	if( strcasecmp("GET",method) == 0 ) 
	{//GET
		strcpy(query_string, getenv("QUERY_STRING"));
		if(query_string)
		{
			printf("<HTML><br/>\n");
			printf("<h2>sum:</h2>");
			printf("<p>");
			plus(query_string);
			printf("</p>\n");	
			printf("</HTML>\n");
		}
	}
	else if( strcasecmp("POST", method) == 0)
	{//POST
			content_length = atoi(getenv("CONTENT_LENGTH"));	
			//printf("content-length=%d\n",content_length);
			int i = 0;
			char c = '\0';
			for(; i < content_length; ++i)
			{
				read(0, &post_data[i], 1);
			}
			post_data[i] = '\0';
			plus(post_data);
			//printf("post data: %s\n",post_data);
	}
	else
	{}

	return 0;
}
