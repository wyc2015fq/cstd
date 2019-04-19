# PostgreSQL的登录、创建用户、数据库并赋权 - Koma Hub - CSDN博客
2019年03月30日 19:39:27[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：13
个人分类：[PostgreSQL](https://blog.csdn.net/Rong_Toa/article/category/7718480)
**Table of Contents**
[一、在默认配置条件下，本机访问PostgreSQL](#%E4%B8%80%E3%80%81%E5%9C%A8%E9%BB%98%E8%AE%A4%E9%85%8D%E7%BD%AE%E6%9D%A1%E4%BB%B6%E4%B8%8B%EF%BC%8C%E6%9C%AC%E6%9C%BA%E8%AE%BF%E9%97%AEPostgreSQL)
[二、创建新用户来访问PostgreSQL](#%E4%BA%8C%E3%80%81%E5%88%9B%E5%BB%BA%E6%96%B0%E7%94%A8%E6%88%B7%E6%9D%A5%E8%AE%BF%E9%97%AEPostgreSQL)
[1、如上所述，先切换到Linux用户postgres，并执行psql：](#1%E3%80%81%E5%A6%82%E4%B8%8A%E6%89%80%E8%BF%B0%EF%BC%8C%E5%85%88%E5%88%87%E6%8D%A2%E5%88%B0Linux%E7%94%A8%E6%88%B7postgres%EF%BC%8C%E5%B9%B6%E6%89%A7%E8%A1%8Cpsql%EF%BC%9A)
[2、创建数据库新用户，如 test：](#2%E3%80%81%E5%88%9B%E5%BB%BA%E6%95%B0%E6%8D%AE%E5%BA%93%E6%96%B0%E7%94%A8%E6%88%B7%EF%BC%8C%E5%A6%82%20test%EF%BC%9A)
[3、创建用户数据库，如testdb：](#3%E3%80%81%E5%88%9B%E5%BB%BA%E7%94%A8%E6%88%B7%E6%95%B0%E6%8D%AE%E5%BA%93%EF%BC%8C%E5%A6%82testdb%EF%BC%9A)
[4、将testdb数据库的所有权限都赋予test：](#4%E3%80%81%E5%B0%86testdb%E6%95%B0%E6%8D%AE%E5%BA%93%E7%9A%84%E6%89%80%E6%9C%89%E6%9D%83%E9%99%90%E9%83%BD%E8%B5%8B%E4%BA%88test%EF%BC%9A)
[5、使用命令 \q 退出psql：](#5%E3%80%81%E4%BD%BF%E7%94%A8%E5%91%BD%E4%BB%A4%20%5Cq%20%E9%80%80%E5%87%BApsql%EF%BC%9A)
[6、创建Linux普通用户，与刚才新建的数据库用户同名，如 test：](#6%E3%80%81%E5%88%9B%E5%BB%BALinux%E6%99%AE%E9%80%9A%E7%94%A8%E6%88%B7%EF%BC%8C%E4%B8%8E%E5%88%9A%E6%89%8D%E6%96%B0%E5%BB%BA%E7%9A%84%E6%95%B0%E6%8D%AE%E5%BA%93%E7%94%A8%E6%88%B7%E5%90%8C%E5%90%8D%EF%BC%8C%E5%A6%82%20test%EF%BC%9A)
[7、以test的身份连接数据库testdb：](#7%E3%80%81%E4%BB%A5test%E7%9A%84%E8%BA%AB%E4%BB%BD%E8%BF%9E%E6%8E%A5%E6%95%B0%E6%8D%AE%E5%BA%93testdb%EF%BC%9A)
[三、最简单的做法](#%C2%A0%E4%B8%89%E3%80%81%E6%9C%80%E7%AE%80%E5%8D%95%E7%9A%84%E5%81%9A%E6%B3%95)
[四、开启远程访问](#%E5%9B%9B%E3%80%81%E5%BC%80%E5%90%AF%E8%BF%9C%E7%A8%8B%E8%AE%BF%E9%97%AE)
[1、编辑配置文件](#1%E3%80%81%E7%BC%96%E8%BE%91%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6)
[2、重启数据库服务：](#2%E3%80%81%E9%87%8D%E5%90%AF%E6%95%B0%E6%8D%AE%E5%BA%93%E6%9C%8D%E5%8A%A1%EF%BC%9A)
[3、此后即可从其它机器上登录，例如用Navicat for PostgreSQL：](#3%E3%80%81%E6%AD%A4%E5%90%8E%E5%8D%B3%E5%8F%AF%E4%BB%8E%E5%85%B6%E5%AE%83%E6%9C%BA%E5%99%A8%E4%B8%8A%E7%99%BB%E5%BD%95%EF%BC%8C%E4%BE%8B%E5%A6%82%E7%94%A8Navicat%20for%20PostgreSQL%EF%BC%9A)
# 一、在默认配置条件下，本机访问PostgreSQL
切换到Linux用户postgres，然后执行psql：
```
$ sudo su - postgres 
[sudo] password for rongtao:      
postgres@rongtao:~$ psql
psql (10.6 (Ubuntu 10.6-0ubuntu0.18.04.1))
Type "help" for help.
postgres=#
```
此时就在数据库postgres中了。使用 psql -d [databasename] 可登录其他数据库。注意：执行psql命令时，必须确认已配置postgresql的环境变量。
# 二、创建新用户来访问PostgreSQL
## 1、如上所述，先切换到Linux用户postgres，并执行psql：
```
$ sudo su - postgres 
[sudo] password for rongtao:      
postgres@rongtao:~$ psql
psql (10.6 (Ubuntu 10.6-0ubuntu0.18.04.1))
Type "help" for help.
postgres=#
```
现在位于数据库提示符下。
## 2、创建数据库新用户，如 test：
```
postgres=# create user test with password 'rong ';
CREATE ROLE
```
注意：语句要以分号结尾。密码要用单引号括起来。
## 3、创建用户数据库，如testdb：
`postgres=# CREATE DATABASE testdb OWNER test;`
## 4、将testdb数据库的所有权限都赋予test：
`postgres=# GRANT ALL PRIVILEGES ON DATABASE testdb TO test;`
## 5、使用命令 \q 退出psql：
`postgres=# \q`
## 6、创建Linux普通用户，与刚才新建的数据库用户同名，如 test：
```
$ sudo adduser test
$ sudo passwd test
```
## 7、以test的身份连接数据库testdb：
```
$ su - test
Password: 
[test@master ~]$ psql -d testdb
```
#  三、最简单的做法
以你的普通Linux用户名，在数据库中创建同名的用户和数据库，如xxf，然后就可以本机直接连接到数据库xxf了。
```
$ psql -d testdb
psql (10.6 (Ubuntu 10.6-0ubuntu0.18.04.1))
Type "help" for help.
testdb=> select * from goods;
 name | price 
------+-------
(0 rows)
testdb=> grant all privileges on database testdb to test;
GRANT
testdb=> \q
rongtao@rongtao:~$ exit
logout
$ psql
```
# 四、开启远程访问
## 1、编辑配置文件
文件：postgresql.conf
位置：/var/lib/pgsql/data/postgresql.conf
添加/修改：在所有IP地址上监听，从而允许远程连接到数据库服务器：
listening_address: '*'
文件：pg_hba.conf
位置：/var/lib/pgsql/data/pg_hba.conf
添加/修改：允许任意用户从任意机器上以密码方式访问数据库，把下行添加为第一条规则：
host    all             all             0.0.0.0/0               md5
## 2、重启数据库服务：
$ sudo systemctl restart postgresql
## 3、此后即可从其它机器上登录，例如用Navicat for PostgreSQL：
主机名或IP： 172.*.*.*
端口：          5432
初始数据库： xxf
用户：          xxf
密码：          ******  (数据库用户xxf的密码，不是Linux用户xxf的密码)
