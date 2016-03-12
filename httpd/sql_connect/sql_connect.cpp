#include "sql_connect.h"

sql_connecter::sql_connecter(const string _host,  const string _user, const string _password,const string _db)
{
	this->res = NULL;
	this->mysql_base = mysql_init(NULL);
	this->host = _host;
	this->user = _user;
	this->password = _password;
	this->db = _db;
}

sql_connecter::~sql_connecter()
{
	if(mysql_base)
	{
		close_connect();
	}
	if(res != NULL)
	{
		free(res);
	}
#ifdef _DEBUG__
	char c;
	cout<<"did you want to delete tables:y?n :";
	scanf("%c",&c);
	if( c == 'y' || c == 'Y' )
	{
		clear_sql_data(); //清除数据库内容，慎用
	}
#endif
}

bool sql_connecter::begin_connect()
{//和mysql建立连结
	if(	mysql_real_connect(mysql_base, host.c_str(), user.c_str(), password.c_str(), db.c_str(),3306,NULL, 0) == NULL)
	{
		cerr<<"connect error"<<endl;
		return false;
	}
	else
	{
	#ifdef _DEBUG__
		cerr<<"connect success!\n";
	#endif
	}
	return true;
}

bool sql_connecter::close_connect()
{//断开连结
		mysql_close(mysql_base);
	#ifdef _DEBUG__
		cout<<"connect close...\n";	
	#endif
}

bool sql_connecter::insert_sql(const string data)
{//向mysql中insert into数据
	string sql = "INSERT INTO student (name,age,school,hobby) VALUES";
	sql += "(";
	sql += data;
	sql +=	")";
	//cout<<sql<<endl;
	if(	mysql_query(mysql_base, sql.c_str()) == 0)
	{
	//#ifdef _DEBUG__
		cout<<"insert success"<<endl;
	//#endif
		return true;
	}
	else
	{
		cerr<<"insert failed"<<endl;
		return false;
	}
}

bool sql_connecter::select_sql()
{
	string sql = "select * from student";
	MYSQL_ROW row;
	MYSQL_FIELD *fd;
	char buf[64][1024];
	if(	mysql_query(mysql_base, sql.c_str()) == 0)
	{
	#ifdef _DEBUG__
		cout<<"query success"<<endl;
	#endif
		res=mysql_store_result(mysql_base); //将数据读取到MYSQL_RES中
		int i=0,j=0;
		for(; fd = mysql_fetch_field(res); ++i)
		{
			bzero(buf[i], sizeof(buf[i]));
			strcpy(buf[i],fd->name);
		}
	    //将输出的数据制成表格形式
	    printf("<table border=\"1\">\n");	
	    printf("<tr>\n");	
	    j=mysql_num_fields(res); //列
	    for(i=0; i<j; ++i)
		{
			printf("<th>%s </th>\t",buf[i]);
		}
	    printf("</tr>\n");	
		
		while( row=mysql_fetch_row(res) )
		{
			printf("<tr>\n");	
			for(i=0; i<j; ++i)
			{//输出一行的所有列
				printf("<td>%s </td>\t",row[i]);
			}
			printf("</tr>\n");	
		}
		printf("</table>\n");
		return true;
	}
	else
	{
		cerr<<"query failed"<<endl;
	}
	
	return false;
}

void sql_connecter::show_info()
{
	cout<<mysql_get_client_info()<<endl;
}

void sql_connecter::student_insert_sql(char *name, char *age, char *school, char *hobby)
{
#ifdef _DEBUG__
	const string data = "name,age,school,hobby";
#endif
	char buf[64];
	memset(buf, '\0', sizeof(buf));
	strcpy(buf,"'");
	strcat(buf,name);
	strcat(buf,"','");
	strcat(buf,age);
	strcat(buf,"','");
	strcat(buf,school);
	strcat(buf,"','");
	strcat(buf,hobby);
	strcat(buf,"'");
	insert_sql(buf);
	//select_sql();
}

void sql_connecter::clear_sql_data()
{
	//清空表结构，慎用
	string clear = "delete from student";
#ifdef _DEBUG__
	printf("clear!\n");
#endif
	if(	mysql_query(mysql_base, clear.c_str()) == 0)
	{
		printf("clear success");
		select_sql();
	}
}

#ifdef _DEBUG__
int main()
{
	const string _host="127.0.0.1";
	const string _user="Zou";
	const string _password="";
	const string _db="test";
	//要插入的数据
	const std::string data = "'tomcat','19','beida','sleep'";
	sql_connecter conn(_host, _user, _password, _db); 
	conn.begin_connect();
	//conn.insert_sql(data);
	conn.select_sql();
    return 0;
}
#endif
