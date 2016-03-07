#ifndef _SQL_CONNET_
#define _SQL_CONNET_

#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>
using namespace std;

class sql_connecter
{
public:

	sql_connecter(const string _host,const string _user,const string _password,const string _db);
	
	~sql_connecter();

	bool begin_connect(); 
	bool close_connect();	
	bool insert_sql(const string data);
	bool select_sql();
	void show_info();
	void clear_sql_data();

private:
	MYSQL_RES  *res;
	MYSQL  	   *mysql_base;
	string 		host;
	string 		user;
	string 		password;
	string 		db;
	
};

#endif
