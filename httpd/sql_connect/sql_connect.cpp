#include "sql_connect.h"

sql_connecter::sql_connecter(const string _host,  const string _user, const string _password,const string _db)
{
	this->res = NULL;
	this->mysql_base = mysql_init(NULL);
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

}

bool sql_connecter::close_connect()
{
		mysql_close(mysql_base);
		cout<<"connect close...\n";	
}

bool sql_connecter::begin_connect()
{
	if(	mysql_real_connect(mysql_base, host.c_str(), user.c_str(), password.c_str(), db.c_str(),3306,NULL, 0) == NULL)
	{
		cerr<<"connect error"<<endl;
		return false;
	}
	else
	{
		cerr<<"connect success!\n"
	}

	return true;
}

bool sql_connecter::select_sql()
{
	string sql_ "select * from student";
	
	if(	mysql_query(mysql_base, sql.c_str()) == 0)
	{
		cout<<"query success"<<endl;
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


bool sql_connecter::insert_sql()
{
	string sql = "INSET INTO student (name,age,school,hobby) VALUES";
	sql += "(";
	sql += data;
	sql +=	")";

	if(	mysql_query(mysql_base, sql.c_str()) == 0)
	{
		cout<<"query success"<<endl;
		return true;
	}
	else
	{
		cerr<<"query failed"<<endl;
		return false;
	}
}



int main()
{
	const string _host="127.0.0.1";
	const string _user="Zou";
	const string _password="";
	const string _db="student";
    const string data= "'tom','19','beida','sleep'";

	sql_connecter conn(_host, _user, _password, _db);

   return 0;
}
