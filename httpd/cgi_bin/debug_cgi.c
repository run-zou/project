#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{

	char method[1024];
	char query_string[1024];
	int content_length = -1;
	char post_data[4096];

	memset(method, '\0', sizeof(method));
	memset(query_string, '\0', sizeof(query_string));
	memset(post_data, '\0', sizeof(post_data));


	printf("<html>\n");
	printf("\t<h>hello world</h>\n");

	strcpy(method, getenv("REQUEST_METHOD"));
	if( strcasecmp("GET",method) == 0 ) 
	{
			strcpy(query_string, getenv("QUERY_STRING"));
			
			printf("<p>method:%s</p><br/>\n",method);
			printf("<p>query_string:%s </p><br/>\n",query_string);
			printf("<p>you are so handsome!</p>");
	}
	else if( strcasecmp("POST", method) == 0)
	{
			content_length = atoi(getenv("CONTENT_LENGTH"));

			int i = 0;
			char c = '\0';
	
			for(; i < content_length; ++i)
			{
				read(0, &post_data[i], 1);
			}
	
			post_data[i] = '\0';

			printf("<p>post data: %s</p><br/>\n",post_data);
	}
	else
	{}


	printf("</html>\n");
	return 0;
}
