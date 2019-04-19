# ERROR 1045 (28000):Access denied for user 'root'@'node1' (using password: YES) - sxf_123456的博客 - CSDN博客
2017年11月11日 00:45:56[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：144
个人分类：[mysql](https://blog.csdn.net/sxf_123456/article/category/7122238)

在node1节点上安装了mysql，启动Hive时报错： 
Caused by: java.sql.SQLException: Access denied for user ‘root’@’node1’ (using password: YES)
测试登录MySQL 
1. 登录node1(本地节点)节点上的mysql
```
[root@node1 ~]#  mysql -h node1 -uroot -p123456
ERROR 1045 (28000): Access denied for user 'root'@'node1' (using password: YES)
```
- 1
- 2
- 登录本地数据库
```
[root@node1 ~]#  mysql -uroot -p123456
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 9
Server version: 5.1.73 Source distribution
Copyright (c) 2000, 2013, Oracle and/or its affiliates. All rights reserved.
Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.
Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.
mysql>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 查询用户表
```
mysql> select user,host,password from mysql.user;
+------+-----------+-------------------------------------------+
| user | host      | password                                  |
+------+-----------+-------------------------------------------+
| root | localhost | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| root | node1     |                                           |
| root | 127.0.0.1 |                                           |
|      | localhost |                                           |
|      | node1     |                                           |
| root | %         | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
+------+-----------+-------------------------------------------+
6 rows in set (0.00 sec)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
发现问题了。
先删除node1记录
```
mysql> delete from mysql.user where host='node1';
Query OK, 2 rows affected (0.05 sec)
mysql> select user,host,password from mysql.user;
+------+-----------+-------------------------------------------+
| user | host      | password                                  |
+------+-----------+-------------------------------------------+
| root | localhost | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| root | 127.0.0.1 |                                           |
|      | localhost |                                           |
| root | %         | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
+------+-----------+-------------------------------------------+
4 rows in set (0.00 sec)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
再删除密码为空的记录
```
mysql> delete from mysql.user where user is null;
Query OK, 0 rows affected (0.01 sec)
mysql> delete from mysql.user where password='';
Query OK, 2 rows affected (0.00 sec)
mysql> select user,host,password from mysql.user;
+------+-----------+-------------------------------------------+
| user | host      | password                                  |
+------+-----------+-------------------------------------------+
| root | localhost | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| root | %         | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
+------+-----------+-------------------------------------------+
2 rows in set (0.00 sec)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
最后对root授权
```
mysql> grant all privileges on *.* to root@'%' identified by '123456' with grant option;
Query OK, 0 rows affected (0.00 sec)
mysql> flush privileges;
Query OK, 0 rows affected (0.00 sec)
mysql> exit
Bye
[root@node1 ~]# mysql -h node1 -uroot -p123456
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 7
Server version: 5.1.73 Source distribution
Copyright (c) 2000, 2013, Oracle and/or its affiliates. All rights reserved.
Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.
Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.
mysql>
```
