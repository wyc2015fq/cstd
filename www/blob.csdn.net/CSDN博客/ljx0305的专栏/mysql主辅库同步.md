# mysql主辅库同步 - ljx0305的专栏 - CSDN博客
2009年07月22日 21:35:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1507
主库:192.168.40.4 DB:testsyn table:users 
辅库:192.168.40.5 
1.配置主库my.cnf 
- [mysqld]   
- log-bin=mysql-testsyn-bin --主库二进制日志文件名的前缀   
- binlog-**do**-db=testsyn      --要同步的数据库   
- server-id = 1             --serverid和辅库不同即可  
```java
[mysqld]
log-bin=mysql-testsyn-bin --主库二进制日志文件名的前缀
binlog-do-db=testsyn      --要同步的数据库
server-id = 1             --serverid和辅库不同即可
```
启动mysql 
创建用户testsyn.拥有replication slave权限
- grant replication slave on *.* to "testsyn"@"192.168.40.%" identified by 'testsyn' with grant option;  
```java
grant replication slave on *.* to "testsyn"@"192.168.40.%" identified by 'testsyn' with grant option;
```
查看主库状态： 
- show master status;  
```java
show master status;
```
2.配置辅库my.cnf 
- server-id = 2
- master-host = 192.168.40.4
- master-user = testsyn    
- master-password = testsyn   
- master-port = 3306
- master-connect-retry = 5  --重试间隔时间   
- replicate-**do**-db = testsyn    --同步db   
- #replicate-wild-**do**-table = user%.%   
- #replicate-**do**-db = itemcontent   
- #replicate-**do**-db = srcstore  
```java
server-id = 2
master-host = 192.168.40.4
master-user = testsyn 
master-password = testsyn
master-port = 3306
master-connect-retry = 5  --重试间隔时间
replicate-do-db = testsyn    --同步db
#replicate-wild-do-table = user%.%
#replicate-do-db = itemcontent
#replicate-do-db = srcstore
```
启动mysql 
查看辅库状态： 
- show slave status;  
```java
show slave status;
```
说明: 
Slave_IO_Running:连接到主库,并读取主库的日志到本地，生成本地日志文件 
Slave_SQL_Running：读取本地日志文件，并执行日志里的sql 
3.把主库上的数据dump到辅库上 
主库：192.168.40.4: 
- mysqldump --opt --master-data -uspider -pcrawler testsyn > /home/mahaibo/dump-testsyn &  
```java
mysqldump --opt --master-data -uspider -pcrawler testsyn > /home/mahaibo/dump-testsyn &
```
注意参数：--master-data
辅库:192.168.40.5 
先建立库:create database testsyn;
导入： 
- stop slave   
- 
- mysql -uspider -pcrawler testsyn < /home/mahaibo/dump-testsyn    
- 
- 
- start slave;  
```java
stop slave
mysql -uspider -pcrawler testsyn < /home/mahaibo/dump-testsyn 
start slave;
```
必须先停止stop slave，然后才能导入
4.在主库192.168.40.4上在users表里插入一条数据： 
insert into users (username,password) values ('test1','test1'); 
辅库上的users表里也相应的有了数据。 
5.查看主库二进制日志文件内容： 
show binlog events; 
注意： 
1.主辅库同步主要是通过二进制日志来实现同步的。 
2.在启动辅库的时候必须先把数据同步，并删除日志目录下的：master.info文件。因为master.info记录了上次要连接主库的信息，如果不删除，即使my.cnf里进行了修改，也不起作用。因为读取的还是master.info文件里的信息。
引用:http://www.javaeye.com/topic/171485
