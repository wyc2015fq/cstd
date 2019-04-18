# SQLite小试 - Orisun - 博客园







# [SQLite小试](https://www.cnblogs.com/zhangchaoyang/articles/1933997.html)





1.安装

遵照不般的安装步骤：

$./configure

$make

$su -c "make install"

注意执行make install只有root权限才能把SQLite编译得到的文件安装到默认的目录/usr/local下。如果想更改安装路径可以用

$./configure --prefix=/opt/SQLite

其他的安装选项可以输入$./configure --help进行查看。

2.检查一下所安装的头文件、库、可执行程序

orisun@zcypc:~$ ll /usr/local/include/*sqlite*#Linux默认从/usr/local/include下去寻找头文件

-rw-r--r-- 1 root root  20686 2011-01-12 16:05 /usr/local/include/sqlite3ext.h

-rw-r--r-- 1 root root 295268 2011-01-12 16:05 /usr/local/include/sqlite3.h

orisun@zcypc:~$ ll /usr/local/lib/*sqlite*

-rw-r--r-- 1 root root 1891672 2011-01-12 16:05 /usr/local/lib/libsqlite3.a

-rwxr-xr-x 1 root root     828 2011-01-12 16:05 /usr/local/lib/libsqlite3.la*

lrwxrwxrwx 1 root root      19 2011-01-12 16:05 /usr/local/lib/libsqlite3.so -> libsqlite3.so.0.8.6*

lrwxrwxrwx 1 root root      19 2011-01-12 16:05 /usr/local/lib/libsqlite3.so.0 -> libsqlite3.so.0.8.6*

-rwxr-xr-x 1 root root 1650714 2011-01-12 16:05 /usr/local/lib/libsqlite3.so.0.8.6*

orisun@zcypc:~$ ll /usr/local/bin/*sqlite*

-rwxr-xr-x 1 root root 93138 2011-01-12 16:05 /usr/local/bin/sqlite3*

3.安装完成后输入$sqlite3 test.db进行测试

orisun@zcypc:~$ sqlite3 test.db

SQLite version 3.7.4

Enter ".help" for instructions

Enter SQL statements terminated with a ";"

sqlite> .help

.backup ?DB? FILE      Backup DB (default "main") to FILE

.bail ON|OFF           Stop after hitting an error.  Default OFF

.databases             List names and files of attached databases

.dump ?TABLE? ...      Dump the database in an SQL text format

... ...sqlite> .exit

4.简单的sqlite3数据库操作由3个函数组成:sqlite3_open(),sqlite3_exec(),sqlite3_close()

#include<sqlite3.h>
#include<stdio.h>
#include<stdlib.h>			//exit

static int select_callback(void *NotUsed,int argc,char **argv,char **azColName){
	int i;
	for(i=0;i<argc;i++){
		printf("%s=%s\n",azColName[i],argv[i]?argv[i]:"NULL");
	}
	printf("\n");
	return 0;
}
int main(){
	sqlite3 *db;
	char *zErrMsg=0;
	int rc;

	rc=sqlite3_open("test.db",&db);		//will create a new db if not existing
	if(rc!=SQLITE_OK){
		fprintf(stderr,"can't open database:%s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
	char *sql="create table T1(id integer primary key,name varchar(20))";
	rc=sqlite3_exec(db,sql,0,0,&zErrMsg);
	if(rc==SQLITE_OK){
		printf("Table T1 created.\n");
	}
	else{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
	}
	sql="insert into T1 values(101,'Tom')";
	rc=sqlite3_exec(db,sql,0,0,&zErrMsg);
	if(rc==SQLITE_OK){
		printf("One record inserted:(101,'Tom').\n");
	}
	else{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
	}
	sql="select id,name from T1";
	rc=sqlite3_exec(db,sql,select_callback,0,&zErrMsg);
	if(rc!=SQLITE_OK){
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
	}
	sqlite3_close(db);
	return 0;
}




sqlite3_exec(db,sql,select_callback,0,&zErrMsg);执行查询时每返回一条记录(注意并不是每执行一次查询)都会调用一次回调函数。

int (*callback)(void*,int,char**,char**)

第2个参数int查询所得到的列数numbers of columns

第3个参数是字符串数组，每一个字符串都是查询所得到的值

第4个参数是字符串数组，每一个字符串都是列名，即在创建表单时所指定的列名。

5.编译运行

由于SQLite默认安装到了/usr/local下，所以用gcc编译时头文件路径和库文件路径都不需要指定了，只需要用-l指定SQLite的库即可。

orisun@zcypc:~/Program$ gcc sqlite3.c -lsqlite3 -o sqlite3

orisun@zcypc:~/Program$ ./sqlite3

Table T1 created.

One record inserted:(101,'Tom').

id=101

name=Tom












