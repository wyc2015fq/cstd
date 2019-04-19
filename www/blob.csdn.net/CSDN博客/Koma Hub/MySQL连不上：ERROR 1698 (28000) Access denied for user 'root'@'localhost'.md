# MySQL连不上：ERROR 1698 (28000): Access denied for user 'root'@'localhost' - Koma Hub - CSDN博客
2019年03月13日 22:32:19[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：8
个人分类：[MySQL																[Linux](https://blog.csdn.net/Rong_Toa/article/category/7156254)](https://blog.csdn.net/Rong_Toa/article/category/7349084)
**Table of Contents**
[修改文件：mysqld.cnf](#%E4%BF%AE%E6%94%B9%E6%96%87%E4%BB%B6%EF%BC%9Amysqld.cnf%C2%A0)
[重启mysql](#%E9%87%8D%E5%90%AFmysql)
[再次连接](#%E5%86%8D%E6%AC%A1%E8%BF%9E%E6%8E%A5)
[修改密码](#%E4%BF%AE%E6%94%B9%E5%AF%86%E7%A0%81)
[将密码方式改为常规](#%E5%B0%86%E5%AF%86%E7%A0%81%E6%96%B9%E5%BC%8F%E6%94%B9%E4%B8%BA%E5%B8%B8%E8%A7%84)
今天在linuxmint上安装了一个mysql
```
$ mysql --version
mysql  Ver 14.14 Distrib 5.7.25, for Linux (x86_64) using  EditLine wrapper
```
但是出现了登录不上的问题。
```
Failed to start mysqld.service: Unit mysqld.service not found.
$ mysql -uroot -p
Enter password: 
ERROR 1698 (28000): Access denied for user 'root'@'localhost'
$ systemctl start mysql 
$ mysql -uroot -p
Enter password: 
ERROR 1698 (28000): Access denied for user 'root'@'localhost'
```
## 修改文件：mysqld.cnf 
```
$ sudo vim /etc/mysql/mysql.conf.d/mysqld.cnf 
 38 lc-messages-dir = /usr/share/mysql
 39 skip-external-locking
 40 #toa 2019.03.13 10:16
 41 skip-grant-tables
 42 #
```
## 重启mysql
`$ service mysql restart`
## 再次连接
```
$ mysql -uroot -p
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 2
Server version: 5.7.25-0ubuntu0.18.04.2 (Ubuntu)
Copyright (c) 2000, 2019, Oracle and/or its affiliates. All rights reserved.
Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.
Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.
mysql>
```
登陆上了，然后我quit，将上面修改后的文件注释掉
```
$ sudo vim /etc/mysql/mysql.conf.d/mysqld.cnf 
 38 lc-messages-dir = /usr/share/mysql
 39 skip-external-locking
 40 #toa 2019.03.13 10:16
 41 #skip-grant-tables
 42 #
$ service mysql restart
$ mysql -uroot -p
Enter password: 
ERROR 1698 (28000): Access denied for user 'root'@'localhost'
```
发现又登录不上去了，然后。。。重复上面的过程，进去MySQL。
## 修改密码
```
mysql> use mysql
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A
Database changed
mysql> update user set authentication_string=password("root ") where user="root";
Query OK, 1 row affected, 1 warning (0.01 sec)
Rows matched: 1  Changed: 1  Warnings: 1
mysql> flush privileges;
Query OK, 0 rows affected (0.00 sec)
mysql> quit
Bye
```
## 将密码方式改为常规
```
mysql> select user,plugin from user;
+------------------+-----------------------+
| user             | plugin                |
+------------------+-----------------------+
| root             | auth_socket           |
| mysql.session    | mysql_native_password |
| mysql.sys        | mysql_native_password |
| debian-sys-maint | mysql_native_password |
+------------------+-----------------------+
4 rows in set (0.00 sec)
mysql> update user set authentication_string=password("root "),plugin='mysql_native_password' where user='root'
    -> ;
Query OK, 1 row affected, 1 warning (0.00 sec)
Rows matched: 1  Changed: 1  Warnings: 1
mysql> select user,plugin from user;
+------------------+-----------------------+
| user             | plugin                |
+------------------+-----------------------+
| root             | mysql_native_password |
| mysql.session    | mysql_native_password |
| mysql.sys        | mysql_native_password |
| debian-sys-maint | mysql_native_password |
+------------------+-----------------------+
4 rows in set (0.00 sec)
mysql> exit
Bye
```
然后退出，将修改后的文件注释后，重启mysql。发现可以了，正常登入
```
$ mysql -uroot -p
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
...
mysql>
```
> 
原文参考：[https://www.cnblogs.com/cpl9412290130/p/9583868.html](https://www.cnblogs.com/cpl9412290130/p/9583868.html)
