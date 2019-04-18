# 【Linux】Ubuntu下C语言访问MySQL数据库入门 - 江南烟雨 - CSDN博客
2013年01月15日 16:22:25[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：10162
使用的系统是Ubuntu 11.10.数据库是MySQL。
## MySQL数据库环境配置
首先需要安装MySQL客户端和服务器，命令行安装方式为：
```cpp
sudo apt-get install mysql-server mysql-client
```
然后，要使用C语言编程访问数据库，需要另外安装一个开发包：
```cpp
sudo apt-get install libmysqlclient15-dev
```
![](https://img-my.csdn.net/uploads/201301/15/1358235590_6323.png)
## 在MySQL中建立相应数据库
首先以用户rick登录MySQL数据库（用户rick已经被root权限用户赋予了创建数据库等等的权限）：
![](https://img-my.csdn.net/uploads/201301/15/1358235716_9826.png)
然后创建一个名为foo的数据库：
```cpp
CREATE DATABASE foo;
```
然后利用如下SQL语句创建表及插入数据：
```cpp
CREATE TABLE children(
    childno int(11) NOT NULL auto_increment,
    fname varchar(30),
    age int(11),
    PRIMARY KEY (childno)
    );
INSERT INTO children(childno,fname,age) VALUES(1,'Jenny',21);
INSERT INTO children(childno,fname,age) VALUES(2,'Andrew',17);
INSERT INTO children(childno,fname,age) VALUES(3,'Gavin',8);
INSERT INTO children(childno,fname,age) VALUES(4,'Duncan',6);
INSERT INTO children(childno,fname,age) VALUES(5,'Emma',4);
INSERT INTO children(childno,fname,age) VALUES(6,'Alex',15);
INSERT INTO children(childno,fname,age) VALUES(7,'Adrian',9);
```
在MySQL命令行模式中执行方法如下：
![](https://img-my.csdn.net/uploads/201301/15/1358235892_6713.png)
## MySQL数据库连接测试
然后采用如下C语言进行数据库连接测试connect1.c：
```cpp
#include <stdlib.h>
#include <stdio.h>
#include "mysql.h"
int main(int argc,char *argv[])
{
  MYSQL *conn_ptr;
  conn_ptr = mysql_init(NULL);
  if(!conn_ptr)
  {
    fprintf(stderr,"mysql_init failed\n");
    return EXIT_FAILURE;
  }
  conn_ptr = mysql_real_connect(conn_ptr,"localhost","rick","rick","foo",0,NULL,0);
  if(conn_ptr)
    printf("Connection success\n");
  else
    printf("Connection failed\n");
  mysql_close(conn_ptr);
  return EXIT_SUCCESS;
}
```
执行结果：
![](https://img-my.csdn.net/uploads/201301/15/1358236013_6570.png)
注意的是：需要指定include库和库文件的路径名，以及指定链接的库模块mysqlclient。
如果不在开始的时候安装开发包，就会产生如下错误：
![](https://img-my.csdn.net/uploads/201301/15/1358236075_1571.png)
## 执行SQL语句进行数据操作
向数据库表children中插入一行：
```cpp
#include <stdlib.h>
#include <stdio.h>
#include "mysql.h"
int main()
{
  MYSQL my_connecyion;
  int res;
  mysql_init(&my_connecyion);
  if(mysql_real_connect(&my_connecyion,"localhost","rick","rick","foo",0,NULL,0))
  {
    printf("Connection success\n");
    //执行SQL语句
    res = mysql_query(&my_connecyion,"INSERT INTO children(fname,age) VALUES('Ann',3)");
    if(!res)
      printf("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(&my_connecyion));
    else
      fprintf(stderr,"Insert error %d : %s \n",mysql_errno(&my_connecyion),mysql_error(&my_connecyion));
    mysql_close(&my_connecyion);
  }
  else{
    fprintf(stderr,"Connection failed\n");
    if(mysql_errno(&my_connecyion))
      fprintf(stderr,"Connection error %d : %s\n",mysql_errno(&my_connecyion),mysql_error(&my_connecyion));
  }
  return EXIT_SUCCESS;
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201301/15/1358238313_3067.png)
![](https://img-my.csdn.net/uploads/201301/15/1358238279_4230.png)
在这里特别需要注意的是：
函数mysql_affected_rows返回的是被一个更新操作修改的行数，而不是满足where子句的行数。
