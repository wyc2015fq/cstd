
# Debian 平台下 Postgresql 数据库基本操作说明 - David Dai -- Focus on Oracle - CSDN博客


2015年02月11日 16:19:40[Dave](https://me.csdn.net/tianlesoftware)阅读数：11146





# 1  安装postgresql
--使用apt直接安装：
dave@dave:~/cndba$ sudo apt-get installpostgresql postgresql-client postgresql-server-dev-all -y
--查看数据库状态：
postgres@dave:~$ /etc/init.d/postgresql status
Running clusters: 9.1/main
--停止：
postgres@dave:~$ /etc/init.d/postgresql stop
[ ok ] Stopping PostgreSQL 9.1 databaseserver: main.
--启动：
postgres@dave:~$ /etc/init.d/postgresql start
[ ok ] Starting PostgreSQL 9.1 databaseserver: main.
postgres@dave:~$
--查看进程：
postgres@dave:~$ ps -ef|grep postgres
root     9502  9184  0 06:34 pts/2    00:00:00 su - postgres
postgres 9510  9502  0 06:34 pts/2    00:00:00 -su
postgres 9869     1  0 06:52 ?        00:00:00/usr/lib/postgresql/9.1/bin/postgres -D /var/lib/postgresql/9.1/main -cconfig_file=/etc/postgresql/9.1/main/postgresql.conf
postgres 9871  9869  0 06:52 ?        00:00:00 postgres: writer process
postgres 9872  9869  0 06:52 ?        00:00:00 postgres: wal writer process
postgres 9873  9869  0 06:52 ?        00:00:00 postgres: autovacuum launcherprocess
postgres 9874  9869  0 06:52 ?        00:00:00 postgres: stats collectorprocess
postgres 9905  9510  0 06:53 pts/2    00:00:00 ps -ef
postgres 9906  9510  0 06:53 pts/2    00:00:00 grep postgres
postgres@dave:~$
--设置开机自启动：
postgres@dave:~$ sudo update-rc.d postgresql start 88 2 3 4 5 . stop 88 0 1 6 .
update-rc.d: using dependency based bootsequencing
postgres@dave:~$
第一次安装后，默认生成一个名为postgres的数据库和一个名为postgres的数据库用户。同时还生成了一个名为postgres的Linux系统用户。
--修改postgres用户密码：
dave@dave:~$ sudo passwd postgres
Enter new UNIX password:
Retype new UNIX password:
passwd: password updated successfully
dave@dave:~$

# 2  查看数据库信息
--查看数据库信息：
dave@dave:~$ su - postgres
Password:
postgres@dave:~$ psql
psql (9.1.15)
Type "help" for help.
postgres=\# help
You are using psql, the command-lineinterface to PostgreSQL.
Type: \copyright for distribution terms
\h for help with SQL commands
\? for help with psql commands
\g or terminate with semicolon to execute query
\q to quit
postgres=\#
postgres-\# \l
List of databases
Name    | Owner   | Encoding |   Collate  |    Ctype    |  Access privileges
-----------+----------+----------+-------------+-------------+-----------------------
postgres  | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 |
template0 | postgres | UTF8     |en_US.UTF-8 | en_US.UTF-8 | =c/postgres         +
|          |          |             |             | postgres=CTc/postgres
template1 | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 |=c/postgres          +
|          |          |            |             |postgres=CTc/postgres
(3 rows)
postgres-\#
--连接到某个数据库：
postgres-\# \c postgres
You are now connected to database"postgres" as user "postgres".
postgres-\# \c template1
You are now connected to database"template1" as user "postgres".
template1-\#
postgres=\# \c postgres
You are now connected to database"postgres" as user "postgres".
--显示当前的数据库：
postgres=\# select current_database();
current_database
------------------
postgres
(1 row)
--该命令显示当前的userid：
postgres=\# select current_user;
current_user
--------------
postgres
(1 row)

--退出操作界面：
template1-\# \q
postgres@dave:~$

# 3  创建DB对象
默认的postgres用户是超级管理员，权限太大，所以一般建议创建一个独立的管理用户。
dave@dave:~$ su - postgres
Password:
postgres@dave:~$ psql
psql (9.1.15)
Type "help" for help.
--创建数据库用户、数据库，并赋予新用户新数据库的全部权限：
postgres=\# create user dave with password'dave';
CREATE ROLE
postgres=\# create database cndba;
CREATE DATABASE
postgres=\# grant all privileges on databasecndba to dave;
GRANT
postgres=\#
postgres=\# \l
List of databases
Name    | Owner   | Encoding |   Collate  |    Ctype    |  Access privileges
-----------+----------+----------+-------------+-------------+-----------------------
cndba     | postgres | UTF8     |en_US.UTF-8 | en_US.UTF-8 | =Tc/postgres        +
|          |          |             |             | postgres=CTc/postgres+
|          |          |             |             | dave=CTc/postgres
postgres  | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 |
template0 | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 |=c/postgres          +
|          |          |             |             | postgres=CTc/postgres
template1 | postgres | UTF8     | en_US.UTF-8 | en_US.UTF-8 |=c/postgres          +
|          |          |             |             | postgres=CTc/postgres
(4 rows)

--重置postgres账户密码：
postgres=\# alter user postgres withpassword 'dave';
ALTER ROLE
--修改用户认证配置文件pg_hba.conf：
postgres@dave:/etc/postgresql/9.1/main$ pwd
/etc/postgresql/9.1/main
postgres@dave:/etc/postgresql/9.1/main$ ls
environment pg_ctl.conf  pg_hba.conf  pg_ident.conf postgresql.conf  start.conf
\# Database administrative login by Unixdomain socket
local  all             postgrestrust
\# TYPE DATABASE        USER            ADDRESS                 METHOD
\# "local" is for Unix domainsocket connections only
local  all             allmd5
--重新加载postgresql：
postgres@dave:/etc/postgresql/9.1/main$/etc/init.d/postgresql reload
postgres@dave:~$ psql -d cndba -U dave
Password for user dave:
psql (9.1.15)
Type "help" for help.
cndba=> selectcurrent_user;
current_user
--------------
dave
(1 row)

--表的基本操作：
postgres-> \c cndba
You are now connected to database"cndba" as user "dave".
cndba->
cndba=> create table cndba(namevarchar(20),signupdate date);
CREATE TABLE
cndba=> insert into cndba(name, signupdate)values('dave', '2015-02-11');
INSERT 0 1
cndba=> select * from cndba;
name| signupdate
------+------------
dave| 2015-02-11
(1 row)
cndba=> update cndba set name ='tianlesoftware' where name = 'dave';
UPDATE 1
cndba=> alter table cndba add emailvarchar(40);
ALTER TABLE
cndba=> alter table cndba alter columnsignupdate set not null;
ALTER TABLE
cndba=> alter table cndba rename columnsignupdate to signup;
ALTER TABLE
cndba=> alter table cndba drop columnemail;
ALTER TABLE
cndba=> alter table cndba rename todave;
ALTER TABLE
cndba=> drop table if exists dave;
DROP TABLE
cndba=>

# 4  postgresql 查看数据库,表,索引,表空间以及大小
postgres=\# select pg_database.datname,pg_database_size(pg_database.datname) AS size from pg_database;
datname  |  size
-----------+---------
template1 | 6030136
template0 | 6030136
postgres | 6030136
cndba    | 6038328
(4 rows)
--以KB，MB，GB的方式来查看数据库大小
postgres=\# selectpg_size_pretty(pg_database_size('cndba'));
pg_size_pretty
----------------
5897kB
(1 row)
postgres=\# create table cndba(namevarchar(20),signupdate date);
CREATE TABLE
--查看多表：
postgres=\# \dt
List of relations
Schema | Name | Type  |  Owner
--------+-------+-------+----------
public | cndba | table | postgres
(1 row)
--查看单表：
postgres=\# \d cndba
Table "public.cndba"
Column   |         Type          | Modifiers
------------+-----------------------+-----------
name      | character varying(20) |
signupdate | date                  |

--查看表大小
postgres=\# select pg_relation_size('cndba');
pg_relation_size
------------------
8192
(1 row)
--以KB，MB，GB的方式来查看表大小
postgres=\# selectpg_size_pretty(pg_relation_size('cndba'));
pg_size_pretty
----------------
8192bytes
(1 row)

--查看索引信息：
postgres=\# create index idx_cndba oncndba(name);
CREATE INDEX
postgres=\# \di
List of relations
Schema |  Name    | Type  | Owner   | Table
--------+-----------+-------+----------+-------
public | idx_cndba | index | postgres | cndba
(1 row)
--查看索引大小：
postgres=\# select pg_size_pretty(pg_relation_size('idx_cndba'));
pg_size_pretty
----------------
16kB
(1 row)
--看表的总大小，包括索引大小
postgres=\# selectpg_size_pretty(pg_total_relation_size('cndba'));
pg_size_pretty
----------------
24kB
(1 row)
--查看所有表空间：
postgres=\# select spcname frompg_tablespace;
spcname
------------
pg_default
pg_global
(2 rows)
--查看表空间大小：
postgres=\# selectpg_size_pretty(pg_tablespace_size('pg_default'));
pg_size_pretty
----------------
23MB
(1 row)

# 5  PostgreSQL用户认证
PostgreSQL的配置文件在/etc/postgresql目录下：
dave@dave:/etc/postgresql/9.1/main$ pwd
/etc/postgresql/9.1/main
dave@dave:/etc/postgresql/9.1/main$ ls
environment pg_ctl.conf  pg_hba.conf  pg_ident.conf postgresql.conf  start.conf
postgresql.conf文件里保存的是数据库的相关的配置。
\# - Connection Settings -
listen_addresses = '*'          \# what IP address(es) to listen on;
\#comma-separated list of addresses;
\#defaults to 'localhost', '*' = all
\#(change requires restart)
port = 5432                             \# (change requiresrestart)
max_connections = 100                   \# (change requires restart)
\# Note: Increasing max_connections costs ~400 bytes of shared memory per
\# connection slot, plus lock space (seemax_locks_per_transaction).
\#superuser_reserved_connections = 3     \# (change requires restart)
unix_socket_directory ='/var/run/postgresql'           \# (changerequires restart)
\#unix_socket_group = ''                 \# (change requires restart)
\#unix_socket_permissions = 0777         \# begin with 0 to use octal notation
\#(change requires restart)
\#bonjour = off                          \# advertise servervia Bonjour
\#(change requires restart)
\#bonjour_name = ''                      \# defaults to thecomputer name
\#(change requires restart)
注意这里的端口信息，要添加的防火墙的策略里。

pg_hba.conf中保存基于主机的认证规则。每条规则会被逐条应用，直到找到一条符合的，就能通过认证;或者访问被reject方法显式拒绝。
postgres@dave:/etc/postgresql/9.1/main$ catpg_hba.conf |grep -v ^\# |grep -v ^$
local  all             postgres                               trust
local  all             all                                     md5
host   all             all             127.0.0.1/32            md5
host   all             all             ::1/128                 md5

带注释的如下：
\# Database administrative login by Unixdomain socket
local  all             postgres                               trust
\# TYPE DATABASE        USER            ADDRESS                 METHOD
\# "local" is for Unix domainsocket connections only
local  all             all                                     md5
\# IPv4 local connections:
host   all             all             127.0.0.1/32            md5

Type = host表示远程连接。
Database = all 表示所有数据库。
User = all 表示所有用户。
ADDRESS 由两部分组成，即IP地址/子网掩码。子网掩码规定了IP地址中前面哪些位表示网络编号。这里/0表示IP地址中没有表示网络编号的位，这样的话全部的IP地址都匹配，例如192.168.0.0/24表示匹配前24位，所以它匹配任何192.168.0.x形式的IP地址。
Method = trust 实际上表示无需认证。

--允许在本机上的任何身份连接任何数据库
TYPE DATABASE   USER    IP-ADDRESS    IP-MASK     METHOD
local all       all                                 trust(无条件进行连接)
--允许IP地址为192.168.1.x的任何主机与数据库sales连接
TYPE DATABASE   USER    IP-ADDRESS    IP-MASK     METHOD
host sales      all     192.168.1.0    255.255.255.0 identsameuser

# 6  远程访问数据库
postgresql默认情况下，远程访问不能成功，如果需要允许远程访问，需要修改两个配置文件，说明如下：
（1）postgresql.conf
将该文件中的listen_addresses项值设定为*。
（2）pg_hba.conf
在该配置文件的host allall 127.0.0.1/32 md5行下添加以下配置，或者直接将这一行修改为以下配置
host   all    all    0.0.0.0/0    md5
表示允许所有IP访问。
修改之后，Reload或者重启数据库让修改生效：
postgres@dave:/etc/postgresql/9.1/main$/etc/init.d/postgresql reload

直接使用Navicat链接：
![](https://img-blog.csdn.net/20150211161751956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20150211161739105?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






--------------------------------------------------------------------------------------------
版权所有，文章禁止转载，否则追究法律责任!
**AboutDave:**
--------------------------------------------------------------------------------------------
QQ:492913789
Email:ahdba@qq.com
Blog:http://www.cndba.cn/dave
Weibo:[http://weibo.com/tianlesoftware](http://weibo.com/tianlesoftware)
Twitter:[http://twitter.com/tianlesoftware](http://twitter.com/tianlesoftware)
Facebook:[http://www.facebook.com/tianlesoftware](http://www.facebook.com/tianlesoftware)
Linkedin:[http://cn.linkedin.com/in/tianlesoftware](http://cn.linkedin.com/in/tianlesoftware)
**Dave 的QQ群：**
--------------------------------------------------------------------------------------------
注意:加群必须注明表空间和数据文件关系 | 不要重复加群
CNDBA_1: 62697850 (空)    CNDBA_2: 62697716 (满)  CNDBA_3: 283816689
CNDBA_4: 391125754   CNDBA_5:104207940    CNDBA_6: 62697977   CNDBA_7: 142216823（满）

