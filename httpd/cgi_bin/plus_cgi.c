#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void plus(char *str)
{
	if( str == NULL)
	{	return ; }

	char *begin=str;
	char *data1=NULL;
	char *data2=NULL;

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

	int dataA = atoi(data1);
	int dataB = atoi(data2);
	printf("%d\n",dataA+dataB);

}

int main()
{

	char method[1024];
	char query_string[1024];
	int content_length = -1;
	char post_data[4096];

	memset(method, '\0', sizeof(method));
	memset(query_string, '\0', sizeof(query_string));
	memset(post_data, '\0', sizeof(post_data));


	strcpy(method, getenv("REQUEST_METHOD"));
	if( strcasecmp("GET",method) == 0 ) 
	{
			strcpy(query_string, getenv("QUERY_STRING"));

//			printf("<p>method: %s<br/>",method);
//			printf("query_string: %s </p<br/>",query_string);
	
			if(query_string)
			{
				plus(query_string);	
			}

	}
	else if( strcasecmp("POST", method) == 0)
	{
		//	printf("hello world\n");

			content_length = atoi(getenv("CONTENT_LENGTH"));
			
		//	printf("content-length=%d\n",content_length);
			
			int i = 0;
			char c = '\0';
	
			for(; i < content_length; ++i)
			{
				read(0, &post_data[i], 1);
			}
	
			post_data[i] = '\0';

			plus(post_data);
			
		//	printf("post data: %s\n",post_data);
	}
	else
	{}

//	printf("</html>\n");
	return 0;
}
