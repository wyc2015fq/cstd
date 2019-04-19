# Centos7安装mysql - fanyun的博客 - CSDN博客
2016年12月13日 09:01:15[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：447
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
### 查看版本
```
# cat /etc/redhat-release 
CentOS Linux release 7.2.1511 (Core)
```
- 1
- 2
- 1
- 2
### 安装mysql
```
# yum install mysql
# yum install mysql-devel
```
安装完，会出现这样的报错
# MariaDB的目的是完全兼容MySQL，包括API和命令行，使之能轻松成为MySQL的代替品。在存储引擎方面，10.0.9版起使用XtraDB（名称代号为Aria）来代替MySQL的InnoDB
# yum install mysql-y
# mysql
ERROR 2002 (HY000):Can't connect to local MySQL server through socket '/var/lib/mysql/mysql.sock'(111)
试着去启动mysql服务，仍然不行
# systemctl startmysql
Failed to issuemethod call: Unit mysql.service failed to load: No such file or directory.
# systemctl start  mysql.service
Failed to issuemethod call: Unit mysql.service failed to load: No such file or directory
# systemctl enablemysql.service
Failed to issuemethod call: Access denied
下面讲正确的步骤：
# yum installmariadb-server -y
# systemctl startmariadb.service
# systemctl enablemariadb.service
# mysql
Welcome to theMariaDB monitor.  Commands end with ; or\g.
Your MariaDBconnection id is 10
Server version:5.5.44-MariaDB MariaDB Server
Copyright (c) 2000,2015, Oracle, MariaDB Corporation Ab and others.
Type 'help;' or '\h'for help. Type '\c' to clear the current input statement.
MariaDB [(none)]>
搞定!
如何设置mysql root密码
#mysql_secure_installation
创建普通用户并授权：
`mysql > use mysql;`
[?](http://my.oschina.net/fhd/blog/383847#)
```
`＃创建root用户与密码并设置为从安装mysql服务的机器本地访问`
`mysql > grant all on root.* to ``'root'``@``'localhost'`
`            ``identified by ``'wwc111'``;`
```
[?](http://my.oschina.net/fhd/blog/383847#)
```
`#设置openscanner用户与密码，并从任何机器都可以访问mysql`
`mysql > grant all on root.* to ``'root'``@``'%'``identified by ``'wwc111'``;`
```
[?](http://my.oschina.net/fhd/blog/383847#)
```
`mysql > flush privileges;            ``#刷新才会生效`
```
### 查看版本
```
# cat /etc/redhat-release 
CentOS Linux release 7.2.1511 (Core)
```
- 1
- 2
- 1
- 2
### 安装mysql
```
# yum install mysql
# yum install mysql-devel
```
安装完，会出现这样的报错
# MariaDB的目的是完全兼容MySQL，包括API和命令行，使之能轻松成为MySQL的代替品。在存储引擎方面，10.0.9版起使用XtraDB（名称代号为Aria）来代替MySQL的InnoDB
# yum install mysql-y
# mysql
ERROR 2002 (HY000):Can't connect to local MySQL server through socket '/var/lib/mysql/mysql.sock'(111)
试着去启动mysql服务，仍然不行
# systemctl startmysql
Failed to issuemethod call: Unit mysql.service failed to load: No such file or directory.
# systemctl start  mysql.service
Failed to issuemethod call: Unit mysql.service failed to load: No such file or directory
# systemctl enablemysql.service
Failed to issuemethod call: Access denied
下面讲正确的步骤：
# yum installmariadb-server -y
# systemctl startmariadb.service
# systemctl enablemariadb.service
# mysql
Welcome to theMariaDB monitor.  Commands end with ; or\g.
Your MariaDBconnection id is 10
Server version:5.5.44-MariaDB MariaDB Server
Copyright (c) 2000,2015, Oracle, MariaDB Corporation Ab and others.
Type 'help;' or '\h'for help. Type '\c' to clear the current input statement.
MariaDB [(none)]>
搞定!
如何设置mysql root密码
#mysql_secure_installation
创建普通用户并授权：
`mysql > use mysql;`
[?](http://my.oschina.net/fhd/blog/383847#)
```
`＃创建root用户与密码并设置为从安装mysql服务的机器本地访问`
`mysql > grant all on root.* to ``'root'``@``'localhost'`
`            ``identified by ``'wwc111'``;`
```
[?](http://my.oschina.net/fhd/blog/383847#)
```
`#设置openscanner用户与密码，并从任何机器都可以访问mysql`
`mysql > grant all on root.* to ``'root'``@``'%'``identified by ``'wwc111'``;`
```
[?](http://my.oschina.net/fhd/blog/383847#)
```
`mysql > flush privileges;            ``#刷新才会生效`
```
