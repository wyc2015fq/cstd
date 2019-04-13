
# mysql导入source数据库sql的C++实现和封装 - 高科的专栏 - CSDN博客

2015年02月05日 08:48:55[高科](https://me.csdn.net/pbymw8iwm)阅读数：1663


之前有好多人在为这件事情头疼不已：
想有一个不需要安装mysql客户端就可以导入数据库脚本，但找不到对应的api调用。所以得需要自己去实现导入数据库的实现方法：
common.h

```python
#ifndef _COMMON_H
#define _COMMON_H
#ifdef WIN32
	#include <winsock2.h>
	typedef __int8					int8_t;
	typedef __int16					int16_t;
	typedef __int32					int32_t;
	typedef __int64					int64_t;
	typedef unsigned __int8			uint8_t;
	typedef unsigned __int16		uint16_t;
	typedef unsigned __int32		uint32_t;
	typedef unsigned __int64		uint64_t;
#define atoll(_String) \
	_atoi64(_String)
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <stdint.h>
	#include<linux/string.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
using namespace std;
enum SERVER_ACTION{
	SERVER_STARTNEW = 0,//开新服
};
#ifdef WIN32
#define  PATH_DELIMTER "\\"
#else
#define  PATH_DELIMTER "/"
#endif
#endif
```
dbmanager.h

```python
#ifndef _DBMANAGER_H
#define _DBMANAGER_H
#include <string>
using namespace std;
#include "common.h"
#include <mysql.h>
//数据库配置信息
struct DBInfo
{
	string host;
	string user;
	string passwd;
	string db;
	uint16_t port;
};
class DBManager
{
public:
	DBManager();
	~DBManager();
	bool SelectDB(string dbName);
	bool ConnectDB(DBInfo &dbInfo);
	MYSQL_RES* ExeSql(const char * sql, int len);
	bool readFromSql(string fileName,vector<string>& sql);
	bool sourceSql(string fileName);
private:
	MYSQL *mysqlInit(DBInfo &info);
	void mysqlClose();
private:
	MYSQL *m_mysqlConn;
	DBInfo m_dbConfig;
};

extern DBManager g_DBManager;




#endif
```
dbmanager.cpp

```python
#include "dbmanager.h"
#include <fstream>
DBManager g_DBManager;
DBManager::DBManager()
{
}
DBManager::~DBManager()
{
}
bool DBManager::ConnectDB(DBInfo &dbInfo)
{
	m_dbConfig = dbInfo; 
	m_mysqlConn = mysqlInit(dbInfo);
	if (!m_mysqlConn) 
	{
		return false;
	}
	return true;
}
MYSQL *DBManager::mysqlInit(DBInfo &info)
{
	MYSQL *mysql = mysql_init(NULL);
	if (!mysql)  
		return NULL;
	
	if (!mysql_real_connect(mysql,
								info.host.c_str(),
								info.user.c_str(),
								info.passwd.c_str(),
								info.db.c_str(),
								info.port, NULL, 0)) 
	{
		int ret = mysql_errno(mysql); 
		
		mysql_close(mysql);
		return NULL;
	}
	
#if MYSQL_VERSION_ID >= 50013
	my_bool reconnect = 1;
	if (mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect))
	{
		int ret = mysql_errno(mysql); 
		 
		mysql_close(mysql);
		return NULL;
	}
#else
	mysql->reconnect = 1;
#endif
	
	return mysql;
} 
void DBManager::mysqlClose()
{
	if (m_mysqlConn)
	{
		mysql_close(m_mysqlConn);
		m_mysqlConn = NULL;
	}
} 
/************************************************************************/
/* 执行SQL语句                                                          */
/************************************************************************/
MYSQL_RES* DBManager::ExeSql(const char * sql, int len)
{
	MYSQL_RES* res = NULL;
	int ret = mysql_real_query(m_mysqlConn, sql, len);
	if (ret == 0)
	{
		res = mysql_store_result(m_mysqlConn);
	}else{
		printf("mysql query %s return errorcode:%d\n",sql, mysql_errno(m_mysqlConn));
	}
	return res;
}
/************************************************************************/
/* 选择数据库                                                           */
/************************************************************************/
bool DBManager::SelectDB(string dbName)  
{  
	if(mysql_select_db(m_mysqlConn,dbName.c_str()))  
		return false;  
	else  
		return true;  
}
/************************************************************************/
/*fileName是sql文件的路径，  解析出fileName中的每一条sql语句，放入到sql容器中    */
/************************************************************************/
bool DBManager::readFromSql(string fileName,vector<string>& sql){
	ifstream in(fileName.c_str(), ios::in);//linux
	string signalSql,s;  
	if(!in){
		return false;
	}  
	while(getline(in,s)){
		int pos = s.find(";");
		signalSql += s;
		if(pos != s.npos){//找到了一条语句的结束位
			sql.push_back(signalSql);
			signalSql.clear();
		}
		s.clear();
	}
	in.close();
	return true;
}
/************************************************************************/
/* 导入数据库sql                                                        */
/************************************************************************/
bool DBManager::sourceSql(string fileName){
	vector<string> vecSql;
	bool ret = readFromSql(fileName,vecSql);
	if(ret == false){
		printf("导入gamedb.sql失败");
		return false;
	}
	for (vector<string>::iterator it = vecSql.begin(); it != vecSql.end(); it++)
	{
		ExeSql((*it).c_str(), (*it).length());
	}
	return true;
}
```
我这里的source实现其实就是把sql脚本里的记录按行读取，如果遇到了一个分号，表示已经是一个语句了，把它放到一个sql语句的容器中，不知道还有没有别人有更好的方法，希望可以在这里交流，并留下你们的代码

