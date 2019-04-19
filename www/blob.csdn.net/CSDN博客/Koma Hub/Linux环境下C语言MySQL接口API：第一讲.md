# Linux环境下C语言MySQL接口API：第一讲 - Koma Hub - CSDN博客
2019年03月11日 22:00:43[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：23
**Table of Contents**
[创建数据库](#%E5%88%9B%E5%BB%BA%E6%95%B0%E6%8D%AE%E5%BA%93)
[create_db.c](#create_db.c)
[编译：](#%E7%BC%96%E8%AF%91%EF%BC%9A)
[运行与结果查看：](#%E8%BF%90%E8%A1%8C%E4%B8%8E%E7%BB%93%E6%9E%9C%E6%9F%A5%E7%9C%8B%EF%BC%9A)
[查看MySQL版本](#%E6%9F%A5%E7%9C%8BMySQL%E7%89%88%E6%9C%AC)
[version.c](#version.c)
[编译运行：](#%E7%BC%96%E8%AF%91%E8%BF%90%E8%A1%8C%EF%BC%9A)
[显示一个数据库](#%E6%98%BE%E7%A4%BA%E4%B8%80%E4%B8%AA%E6%95%B0%E6%8D%AE%E5%BA%93)
[mysql_test.c](#mysql_test.c)
[连接主机上的数据库](#%E8%BF%9E%E6%8E%A5%E4%B8%BB%E6%9C%BA%E4%B8%8A%E7%9A%84%E6%95%B0%E6%8D%AE%E5%BA%93)
[connect.c](#connect.c)
[插入数据库、从数据库中删除](#%E6%8F%92%E5%85%A5%E6%95%B0%E6%8D%AE%E5%BA%93%E3%80%81%E4%BB%8E%E6%95%B0%E6%8D%AE%E5%BA%93%E4%B8%AD%E5%88%A0%E9%99%A4)
[insertDelMySQL.c](#insertDelMySQL.c)
[从数据库中查询](#%E4%BB%8E%E6%95%B0%E6%8D%AE%E5%BA%93%E4%B8%AD%E6%9F%A5%E8%AF%A2)
[searchMySQL.c](#searchMySQL.c)
[makefile](#makefile)
# 创建数据库
## create_db.c
```cpp
#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <stdio.h>
int main(int argc, char **argv)
{  
  MYSQL *con = mysql_init(NULL);
  if (con == NULL) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }
  if (mysql_real_connect(con, "localhost", "root", "root ", 
          NULL, 0, NULL, 0) == NULL) 
  {
      fprintf(stderr, "error:%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
  }  
  if (mysql_query(con, "CREATE DATABASE testdb")) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
  }
  mysql_close(con);
  exit(0);
}
```
## 编译：
`gcc createdb.c `mysql_config --libs --cflags``
## 运行与结果查看：
```
mysql> show databases
    -> ;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| sys                |
| testdb             |
+--------------------+
5 rows in set (0.00 sec)
mysql> use testdb
Database changed
mysql> show tables;
Empty set (0.00 sec)
mysql>
```
# 查看MySQL版本
## version.c
```cpp
#include <my_global.h>
#include <mysql.h>
int main(int argc, char **argv)
{
  printf("MySQL client version: %s\n", mysql_get_client_info());
  exit(0);
}
```
## 编译运行：
```
$ gcc -o a version.c `mysql_config --cflags --libs`
$ ./a
MySQL client version: 5.7.21
```
# 显示一个数据库
## mysql_test.c
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
int main(int argc, char * argv[])
{
    MYSQL * conn;
    MYSQL_RES * res;
    MYSQL_ROW row;
    char server[] = "localhost";
    char user[] = "root";
    char password[] = "root ";
    char database[] = "testdb";
    
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) 
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    if (mysql_query(conn, "show tables")) 
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    
    res = mysql_use_result(conn);
    
    printf("MySQL Tables in mysql database:\n");
    
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("%s \n", row[0]);
    }
    
    mysql_free_result(res);
    mysql_close(conn);
    
    printf("finish! \n");
    
    return 0;
}
```
# 连接主机上的数据库
## connect.c
```cpp
/*************************************
 * Programming Date   :2018年3月31日
 * Programming Content:linux环境实现用C语言连接MySql数据库连接操作-编译时加-lmysqlclient
 * Author             :Sun YiJie
**************************************/
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#define HOST_NAME   "localhost"
#define USER_NAME   "root"
#define PASSWORD    "root "
#define DATABASES   "testdb"
int main(int argc, char * argv[])
{
    MYSQL       mysql;                        //定义MYSQL结构体对象
    /*分配和初始化mysql对象: mysql_init()*/
    if (NULL == mysql_init(&mysql))     
    {
        printf("mysql_init(): %s\n", mysql_error(&mysql));
        exit(EXIT_FAILURE);
    }
    /*连接主机上的MYSQL数据库: mysql_real_connect()*/
    if (NULL == mysql_real_connect(
                &mysql,
                HOST_NAME,
                USER_NAME,
                PASSWORD,
                DATABASES,
                0,
                NULL,
                0
                 ))
    {
        printf("mysql_real_connect(): %s\n", mysql_error(&mysql));
        exit(EXIT_FAILURE);
    }
    printf("Connect to MySql successful!\n");
    /*关闭数据库*/
    mysql_close(&mysql);
    return 0;
}
```
# 插入数据库、从数据库中删除
## insertDelMySQL.c
```cpp
/*************************************
 * Programming Date   :2018年3月31日
 * Programming Content:linux环境实现用C语言插入和删除MySql数据库操作-编译时加-lmysqlclient
 * Author             :Sun YiJie
**************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#define HOST_NAME   "localhost"
#define USER_NAME   "root"
#define PASSWORD    "root "
#define DATABASES   "testdb"
int main(int argc, char * argv[])
{
    MYSQL       mysql;
    MYSQL_RES   * res = NULL;
    MYSQL_ROW   row;
    char        * query_str = NULL;
    int rc = 0, i = 0, fields = 0;
    int         rows = 0;
    /*分配和初始化mysql对象: mysql_init()*/
    if (NULL == mysql_init(&mysql))     
    {
        printf("mysql_init(): %s\n", mysql_error(&mysql));
        exit(EXIT_FAILURE);
    }
    /*连接主机上的MYSQL数据库: mysql_real_connect()*/
    if (NULL == mysql_real_connect(
                &mysql,
                HOST_NAME,
                USER_NAME,
                PASSWORD,
                DATABASES,
                0,
                NULL,
                0
                 ))
    {
        printf("1-mysql_real_connect(): %s\n", mysql_error(&mysql));
        exit(EXIT_FAILURE);
    }
    printf("---->Connect to MySql successful!<----\n");
    query_str = "insert into information values('005', 'Jerry', 25)";
    /*执行插入请求*/
    rc = mysql_real_query(&mysql, query_str, strlen(query_str));
    if (0 != rc)
    {
        printf("2-mysql_real_query(): %s\n", mysql_error(&mysql));
        exit(EXIT_FAILURE);
    }  
    query_str = "delete from information where name = 'Tom'";
    /*执行删除请求*/
    rc = mysql_real_query(&mysql, query_str, strlen(query_str));
    if (0 != rc)
    {
        printf("3-mysql_real_query(): %s\n", mysql_error(&mysql));
        exit(EXIT_FAILURE);
    }      
    query_str = "select * from information";
    /*执行数据库指令: mysql_real_query()*/
    rc = mysql_real_query(&mysql, query_str, strlen(query_str));
    if (0 != rc)
    {
        printf("4-mysql_real_query(): %s\n", mysql_error(&mysql));
        exit(EXIT_FAILURE);
    }
    /*得到数据库结果集合：mysql_store_result()*/
    res = mysql_store_result(&mysql);
    if (NULL == res)
    {
        printf("mysql_store_result(): %s\n", mysql_error(&mysql));
        exit(EXIT_FAILURE);       
    }
    /*统计数据库行的数量：mysql_num_rows()*/
    rows = mysql_num_rows(res);
    printf("This table has %d rows!\n", rows);
    /*统计数据库列的数量：mysql_num_fields()*/
    fields = mysql_num_fields(res);
    printf("This table has %d fields!\n", fields);
    /*打印每一行的元素*/
    while (row = (mysql_fetch_row(res)))
    {
        for (i = 0; i < fields; i++)
        {
            printf("%s\t", row[i]);
        }
        printf("\n");
    }
    /*释放数据库结果集和空间*/
    mysql_free_result(res);
    /*关闭数据库*/
    mysql_close(&mysql);
    return 0;
}
```
# 从数据库中查询
## searchMySQL.c
```cpp
/*************************************
 * Programming Date   :2018年3月31日
 * Programming Content:linux环境实现用C语言查询MySql数据库操作-编译时加-lmysqlclient
 * Author             :Sun YiJie
**************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#define HOST_NAME   "localhost"
#define USER_NAME   "root"
#define PASSWORD    "root "
#define DATABASES   "testdb"
int main(int argc, char * argv[])
{
    MYSQL       mysql;
    MYSQL_RES   * res = NULL;
    MYSQL_ROW   row;
    char        * query_str = NULL;
    int rc = 0, i = 0, fields = 0;
    int         rows = 0;
    /*分配和初始化mysql对象: mysql_init()*/
    if (NULL == mysql_init(&mysql))     
    {
        printf("mysql_init(): %s\n", mysql_error(&mysql));
        exit(EXIT_FAILURE);
    }
    /*连接主机上的MYSQL数据库: mysql_real_connect()*/
    if (NULL == mysql_real_connect(
                &mysql,
                HOST_NAME,
                USER_NAME,
                PASSWORD,
                DATABASES,
                0,
                NULL,
                0
                 ))
    {
        printf("mysql_real_connect(): %s\n", mysql_error(&mysql));
        exit(EXIT_FAILURE);
    }
    printf("---->Connect to MySql successful!<----\n");
    query_str = "select * from information";
    /*执行数据库指令: mysql_real_query()*/
    rc = mysql_real_query(&mysql, query_str, strlen(query_str));
    if (0 != rc)
    {
        printf("mysql_real_query(): %s\n", mysql_error(&mysql));
        exit(EXIT_FAILURE);
    }
    /*得到数据库结果集合：mysql_store_result()*/
    res = mysql_store_result(&mysql);
    if (NULL == res)
    {
        printf("mysql_store_result(): %s\n", mysql_error(&mysql));
        exit(EXIT_FAILURE);       
    }
    /*统计数据库行的数量：mysql_num_rows()*/
    rows = mysql_num_rows(res);
    printf("This table has %d rows!\n", rows);
    /*统计数据库列的数量：mysql_num_fields()*/
    fields = mysql_num_fields(res);
    printf("This table has %d fields!\n", fields);
    /*打印每一行的元素*/
    while (row = (mysql_fetch_row(res)))
    {
        for (i = 0; i < fields; i++)
        {
            printf("%s\t", row[i]);
        }
        printf("\n");
    }
    /*释放数据库结果集和空间*/
    mysql_free_result(res);    
    /*关闭数据库*/
    mysql_close(&mysql);
    return 0;
}
```
# makefile
```
files=`ls`
all: connectMySQL insertDelMySQL mysql_test searchMySQL
	#@echo $(suffix $(files))
	@echo $(files)
%:%.c
	gcc -o $@ $< `mysql_config --libs --cflags`
```
