//此文件用于实现学生信息的注册，查找，删除和更新
#include "sql_connect.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void student_insert(sql_connecter &_conn, char *str)
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
	//获取学生的四个属性
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
#ifdef _DEBUG
	printf("name:%s\n",name);
	printf("age:%s\n",age);
	printf("school:%s\n",school);
	printf("hobby:%s\n",hobby);
#endif
	_conn.insert_sql(name, age, school, hobby);
}

void student_del_upd(sql_connecter &_conn, char *str)
{	
	//获取参数
	char *begin=str;
	int  flag=0;	
	char *type1=NULL; //参数一
	char *val1=NULL;  //参数二
	char *type2=NULL; //参数三
	char *val2=NULL;  //参数四
	while(*begin != '\0')
	{
		if(*begin == '=')
		{
			switch(flag)
			{
				case 0:
					type1 = begin + 1;
					break;
				case 1:
					val1 = begin + 1;
					break;
				case 2:
					type2  = begin + 1;
					break;
				case 3:
					val2 = begin + 1;
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
		{ begin++; }
	}
#ifdef _DEBUG
  printf("type1:%s\n",type1);
  printf("val1:%s\n",val1);
  printf("type2:%s\n",type2);
  printf("val2:%s\n",val2);
#endif
	if(type1 == NULL)
	{//说明输入错误
		printf("<p> not recv data,intput again </p>\n");
		return ;
	}

	if( type2 != NULL  && val2 != NULL)
	{//3--如果有两对参数，则表示update
		_conn.update_sql(type1,val1,type2,val2);
	}
	else if( type1 != NULL && val1 != NULL)
	{//2--delete方法
		_conn.delete_sql(type1,val1);	
	}
}

int main()
{
	const string _host="127.0.0.1";
	const string _user="Zou";
	const string _password="";
	const string _db="test";
	//连接数据库（可连结远程数据库）
	sql_connecter conn(_host, _user, _password, _db); 
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
#ifdef _DEBUG
		printf("<p> %s </p>\n",query_string);
#endif
		if( strlen(query_string) != 0 )
		{//有参数则需要调用相应方法
			student_del_upd(conn,query_string);
		}
		else
		{//4--数据库信息的查看
			conn.select_sql();
		}
	}
	else if( strcasecmp("POST", method) == 0)
	{//POST表单提交
		content_length = atoi(getenv("CONTENT_LENGTH"));//获取数据长度
#ifdef _DEBUG
		printf("content-length=%d\n",content_length);
#endif
		int i = 0;
		char c = '\0';
		for(; i < content_length; ++i)
		{
			read(0, &post_data[i], 1);
		}
		post_data[i] = '\0';
		//1--调用数据插入接口插入数据
		student_insert(conn, post_data);
	}
	else
	{//do nothing
	}

	return 0;
}
