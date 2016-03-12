#include "sql_connect.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void student_insert(sql_connecter & _conn, char *str)
{
	if( str == NULL)
	{	
		return ; 
	}
	char name[32];
	memset(name,'\0',sizeof(name));
	char age[32];
	memset(age,'\0',sizeof(age));
	char school[32];
	memset(school,'\0',sizeof(school));
	char hobby[32];
	memset(hobby,'\0',sizeof(hobby));
	//str="name=a&age=12&school=beida&hobby=wrok"	
	//获取学生的四个属性，法一：
	//str是POST提交的表单，形式和str类似，要提取参数需要将‘=’和‘&’换成‘ ’以便sscanf函数处理
	char *tmp=str;
	while( *tmp != '\0')
	{
		if(*tmp == '=' || *tmp == '&')
		{
			*tmp = ' ';
		}
		tmp++;
	}
	sscanf(str,"%*s %s %*s %s %*s %s %*s %s",name,age,school,hobby);

	//获取学生的四个属性,法二
	//	char *begin=str;
	//	int  flag=0;	
	//	char *age=NULL;
	//	char *name=NULL;
	//	char *school=NULL;
	//	char *hobby=NULL;
	//	while(*begin != '\0')
	//	{
	//		if(*begin == '=')
	//		{
	//			switch(flag)
	//			{
	//				case 0:
	//					name = begin + 1;
	//					break;
	//				case 1:
	//					age = begin + 1;
	//					break;
	//				case 2:
	//					school  = begin + 1;
	//					break;
	//				case 3:
	//					hobby = begin + 1;
	//					break;
	//				default:
	//					break;
	//			}
	//				
	//		   while(*begin != '&' && *begin != '\0')
	//		   { ++begin; }
	//					
	//		   *begin = '\0';
	//			flag++;
	//
	//		   if(flag <= 3)
	//		   {
	//			   begin++; 
	//		   }
	//		}
	//
	//		if( *begin != '\0')
	//		{
	//			begin++;
	//		}
	//	}
#ifdef _DEBUG
	printf("name:%s\n",name);
	printf("age:%s\n",age);
	printf("school:%s\n",school);
	printf("hobby:%s\n",hobby);
#endif
	_conn.student_insert_sql(name, age, school, hobby);

}

int main()
{
	const string _host="127.0.0.1";
	const string _user="Zou";
	const string _password="";
	const string _db="test";
	sql_connecter conn(_host, _user, _password, _db); 
	//连结数据库
	conn.begin_connect();
	char method[1024]; //用于提取方法
	memset(method, '\0', sizeof(method));	
	char query_string[1024];//用于提取参数
	memset(query_string, '\0', sizeof(query_string));
	int content_length = -1;//提取数据长度
	char post_data[4096];
	memset(post_data, '\0', sizeof(post_data));
	//从环境变量里面拿出方法（GET，POST...）
	strcpy(method, getenv("REQUEST_METHOD"));
	if( strcasecmp("GET",method) == 0 ) 
	{//GET
		strcpy(query_string, getenv("QUERY_STRING"));
		printf("%s\n",query_string);
		if( strlen(query_string) != 0 )
		{//有参数则需要调用相应方法
			student_insert(conn,query_string);
		}
		else
		{//查看数据
			conn.select_sql();
		}
	}
	else if( strcasecmp("POST", method) == 0)
	{//POST表单提交
		content_length = atoi(getenv("CONTENT_LENGTH"));//获取数据长度
		//printf("content-length=%d\n",content_length);
		int i = 0;
		char c = '\0';
		for(; i < content_length; ++i)
		{
			read(0, &post_data[i], 1);
		}
		post_data[i] = '\0';
		//调用数据插入接口插入数据
		student_insert(conn, post_data);
	}
	else
	{//do nothing
	}

	return 0;
}
