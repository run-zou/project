#include "sql_connect.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void student_insert(char *str)
{
	if( str == NULL)
	{	return ; }

	char *begin=str;

	int  flag=0;	
	char *age=NULL;
	char *name=NULL;
	char *school=NULL;
	char *hobby=NULL;

	//获取学生的四个属性
	while(*begin != '\0')
	{

			if(*begin == '=')
			{
				switch(flag)
				{
					case 0:
						name = begin + 1;
						break;
					case 1:
						age = begin + 1;
							break;
					case 2:
						school  = begin + 1;
							break;
					case 3:
						hobby = begin + 1;
							break;
					default:
							break;

				}
				
				while(*begin != '&' && *begin != '\0')
				{ ++begin; }
					
				*begin = '\0';
				flag++;

				if(flag <= 3)
				{ begin++; }
			}

			if( *begin != '\0')
			begin++;
	}

#ifdef _DEBUG
	printf("name:%s\n",name);
	printf("age:%s\n",age);
	printf("hobby:%s\n",hobby);
	printf("school:%s\n",school);
#endif

	student_insert_sql(name, age, school, hobby);
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
	{//GET
		strcpy(query_string, getenv("QUERY_STRING"));

	//	printf("%s\n",query_string);

		if(query_string)
		{
			student_insert(query_string);
		}

	}
	else if( strcasecmp("POST", method) == 0)
	{//POST

			content_length = atoi(getenv("CONTENT_LENGTH"));
			
		//	printf("content-length=%d\n",content_length);
			
			int i = 0;
			char c = '\0';
	
			for(; i < content_length; ++i)
			{
				read(0, &post_data[i], 1);
			}
			post_data[i] = '\0';

			student_insert(post_data);
	}
	else
	{//do nothing
	}

	return 0;
}
