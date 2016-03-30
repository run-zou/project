//分别利用GET方法和POST方法执行加法计算并将结果输出到浏览器

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void plus(char *str)
{//简单加法器
	if( str == NULL)
	{	
		return ; 
	}
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
	if(data1 == NULL || data2==NULL)
	{//falied
		return ;
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
	//将环境变量里面的支取出来拷贝到数组中
	strcpy(method, getenv("REQUEST_METHOD"));
	if( strcasecmp("GET",method) == 0 ) 
	{//GET
		//取出GET方法的参数
		strcpy(query_string, getenv("QUERY_STRING"));
		if(query_string)
		{//构造成html文档格式
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
			//取出POST表单长度
			content_length = atoi(getenv("CONTENT_LENGTH"));	
			//printf("<p>content-length=%d</p>\n",content_length);
			int i = 0;
			char c = '\0';
			for(; i < content_length; ++i)
			{
				read(0, &post_data[i], 1);
			}
			post_data[i] = '\0';
			printf("<HTML><br/>\n");
			printf("<h2>sum:</h2>");
			printf("<p>");
			plus(post_data);
			printf("</p>");
			printf("</HTML>\n");
			//printf("<p>post data: %s</p>\n",post_data);
	}
	else
	{}

	return 0;
}
