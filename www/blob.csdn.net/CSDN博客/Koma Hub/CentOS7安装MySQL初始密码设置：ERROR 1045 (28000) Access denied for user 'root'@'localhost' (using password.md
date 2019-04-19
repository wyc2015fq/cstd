# CentOS7安装MySQL初始密码设置：ERROR 1045 (28000): Access denied for user 'root'@'localhost' (using password: - Koma Hub - CSDN博客
2019年03月11日 21:27:36[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：20
个人分类：[MySQL																[CentOS](https://blog.csdn.net/Rong_Toa/article/category/8122413)](https://blog.csdn.net/Rong_Toa/article/category/7349084)
晕，找了好半天终于找到了，在这里做个笔记。
> 
解决错误：ERROR 1045 (28000): Access denied for user 'root'@'localhost' (using password: YES)
**Table of Contents**
[1.安装](#1.%E5%AE%89%E8%A3%85)
[2.查找初始密码](#2.%E6%9F%A5%E6%89%BE%E5%88%9D%E5%A7%8B%E5%AF%86%E7%A0%81)
[3.密码修改](#3.%E5%AF%86%E7%A0%81%E4%BF%AE%E6%94%B9)
[4.其他设置](#4.%E5%85%B6%E4%BB%96%E8%AE%BE%E7%BD%AE)
感谢：[CentOs安装Mysql和配置初始密码](https://www.cnblogs.com/FlyingPuPu/p/7783735.html)
# 1.安装
首先使用yum安装MySQL就不用过多说明了。
# 2.查找初始密码
查找方式：
```
$ grep 'temporary pass' /var/log/mysqld.log 
2018-02-04T06:08:19.158928Z 1 [Note] A temporary password is generated for root@localhost: l#()FqWfy9ih
```
这里注意，我的密码里带有()括号，那么这样用就不对了
```
$ mysql -uroot -pl#()FqWfy9ih
bash: 未预期的符号 `(' 附近有语法错误
```
需要这样用：
```
$ mysql -uroot -pl#\(\)FqWfy9ih
mysql: [Warning] Using a password on the command line interface can be insecure.
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 31
Server version: 5.7.24
Copyright (c) 2000, 2018, Oracle and/or its affiliates. All rights reserved.
Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.
Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.
mysql>
```
成功进入MySQL的命令行。
# 3.密码修改
```
mysql> set password for 'root'@'localhost' = password('root');
ERROR 1819 (HY000): Your password does not satisfy the current policy requirements
mysql> set global validate_password_length=0;
Query OK, 0 rows affected (0.00 sec)
mysql> set global validate_password_policy=0;
Query OK, 0 rows affected (0.00 sec)
mysql>  set password for 'root'@'localhost' = password('root ');
Query OK, 0 rows affected, 1 warning (0.00 sec)
mysql> exit
Bye
$
```
以上命令依次为：
- 修改：密码最小长度策略
- 修改：密码强度检查等级策略，0/LOW、1/MEDIUM、2/STRONG
- 修改密码
# 4.其他设置
```
mysql> GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY 'root ' WITH GRANT OPTION;
Query OK, 0 rows affected, 1 warning (0.00 sec)
mysql> flush privileges;
Query OK, 0 rows affected (0.00 sec)
```
依次为：
- 开启mysql的root用户远程连接服务(%号即远程连接，IDENTIFIED BY后面跟的密码)
- 刷新
