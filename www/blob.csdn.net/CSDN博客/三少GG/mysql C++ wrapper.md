# mysql C++ wrapper - 三少GG - CSDN博客
2014年01月20日 00:04:57[三少GG](https://me.csdn.net/scut1135)阅读数：2068
前言：
I don't really understand why they made a C++ wrapper for the C API (MySQL++ = http://www.tangentsoft.net/mysql++/ ) because shouldn't the C API work in C++?
The mysql C api will work in C++ - I suppose someone just wanted to do a c++ wrapper.
库推荐：
1. SOCI  [http://soci.sourceforge.net/index.html](http://soci.sourceforge.net/index.html)
SOCI is a database access library for C++ that makes the illusion of embedding SQL queries in
 the regular C++ code, staying entirely within the Standard C++.
The idea is to provide C++ programmers a way to access SQL databases in the most natural and intuitive way. If you find existing libraries too difficult for
 your needs or just distracting, SOCI can be a good alternative.
2.  MySQL++[http://tangentsoft.net/mysql++/](http://tangentsoft.net/mysql++/)
MySQL++
 is a C++ wrapper for [MySQL](http://mysql.com/)’s
 C API. It is built around the same principles as the Standard C++ Library, to make dealing with the database as easy as dealing with STL containers. In addition, MySQL++ provides facilities that let you avoid the most repetitive sorts of SQL within your own
 code, providing native C++ interfaces for these common tasks.
个人编写：
1.  借助Boost库
[Lean and extensible mySQL C++
 wrapper](http://www.codeproject.com/Articles/17788/Lean-and-extensible-mySQL-C-wrapper)
This
 article present a lean C++ wrapper of mySQL client. All of the functions are defined in two header files. Users can build the file with mySQL C API library and header files of `boost::shared_ptr `which
 is used to manage the MYSQL connection and result set.
2.  [http://bogomip.net/blog/2009/08/28/new-c-mysql-wrapper/](http://bogomip.net/blog/2009/08/28/new-c-mysql-wrapper/)
3. 不借助外部库
```
#include <mysql.h>
#include <cstdio>
int main()
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *server = "server";
	char *user = "user";
	char *password = "password";  // got tot keep my data secret
	char *database = "cpp_test";
	conn = mysql_init(NULL);
	
	// connect to database
	if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		return -1;
	}
	
	// send SQL query
	if(mysql_query(conn, "select * from cpp_testTAB"))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		return -1;
	}
	
	res = mysql_use_result(conn);
	
	// output table name
	printf("MySQL Tables in mysql database:\n");
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		printf("%s %s %s %s\n", row[0], row[1], row[2], row[3]);
	}
	
	// close connection
	mysql_free_result(res);
	mysql_close(conn);
	
	return 0;
}
```
Output is:
MySQL Tables in mysql database:
1 John Bartholomew Pippin
2 Thomas Tiberius Tabernathy
3 Delta Echo Gamma
