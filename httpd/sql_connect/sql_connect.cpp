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

//1.增
bool sql_connecter::insert_sql(char *name, char *age, char *school, char *hobby)
{//向mysql中insert into数据
	string sql = "INSERT INTO student (name,age,school,hobby) VALUES";
	sql+="('";
	sql+=name;
	sql+="',";
	sql+=age;
	sql+=",'";
	sql+=school;
	sql+="','";
	sql+=hobby;
	sql+="')";
#ifdef _DEBUG__
	cout<<sql<<endl;
#endif
	if(	mysql_query(mysql_base, sql.c_str()) == 0)
	{
		cout<<"<p> insert success </p>"<<endl;
	}
	else
	{
		cerr<<"<p> insert failed </p>"<<endl;
		return false;
	}
	cout<<"<a href=\"../index.html\">return home </a><br/>"<<endl; //超链接
	return true;
}
//2.删
bool sql_connecter::delete_sql(const string type,const string val)//age,10...
{
	//delete from student where (name=x,age=x,school=x,hobby=x)
	string sql = "delete from student where(";
	sql+=type;
	sql+="='";
	sql+=val;
	sql+="')";

	cout<<"<p>"<<sql<<"</p>"<<endl;
	
	if(mysql_query(mysql_base, sql.c_str()) == 0)
	{
	#ifdef _DEBUG__
		cout<<"<p> delete success </p>"<<endl;
	#endif
	}
	else
	{
		cerr<<"<p> delete failed </p>"<<endl;
		return false;
	}
	cout<<"<a href=\"../index.html\">return home </a><br/>"<<endl; //超链接
	return true;
}
//3.改
bool sql_connecter::update_sql(const string _type,const string new_val, const string type, const string val)
{//update stduent set age=x where(age=x,...)
	string sql = "update student set ";
	sql+=_type;
	sql+="='";
	sql+=new_val;
	sql+="' ";
	sql+="where(";
	sql+=type;
	sql+="='";
	sql+=val;
	sql+="')";
	cout<<"<p>"<<sql<<"</p>"<<endl;
	if(mysql_query(mysql_base, sql.c_str()) == 0)
	{
	#ifdef _DEBUG__
		cout<<"<p> update success </p>"<<endl;
	#endif
	}
	else
	{
		cerr<<"<p> update failed </p>"<<endl;
		return false;
	}
	cout<<"<a href=\"../index.html\">return home </a><br/>"<<endl; //超链接
	return true;
}
//4.查
bool sql_connecter::select_sql()
{
	string sql = "select * from student";
	MYSQL_ROW row;
	MYSQL_FIELD *fd;
	char buf[64][1024];
	if(	mysql_query(mysql_base, sql.c_str()) == 0)
	{
	#ifdef _DEBUG__
		cout<<"<p> query success </p>"<<endl;
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
		{//输出第一行的每一列信息
			printf("<th>%s </th>\t",buf[i]);
		}
	    printf("</tr>\n");	
		
		while( row=mysql_fetch_row(res) )
		{
			printf("<tr>\n");	
			for(i=0; i<j; ++i)
			{//一次输出每一行的所有列
				printf("<td>%s </td>\t",row[i]);
			}
			printf("</tr>\n");	
		}
		printf("</table>\n");
		cout<<"<a href=\"../index.html\">return home </a><br/>"<<endl; //超链接
		return true;
	}
	else
	{
		cerr<<"<p> query failed </p>"<<endl;
	}
	return false;
}

void sql_connecter::show_info()
{
	cout<<mysql_get_client_info()<<endl;
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
	sql_connecter conn(_host, _user, _password, _db); 
	conn.begin_connect();
	conn.insert_sql("tomcat","19","beida","sleep");
	conn.select_sql();
	conn.update_sql("name","mack","age","3");
	conn.select_sql();
	conn.delete_sql("name","mac");
	conn.select_sql();
    return 0;
}
#endif
