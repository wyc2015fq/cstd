# MySQL集群系列1：2台机器搭建双主集群 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月11日 21:17:22[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：294
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)
## 先配置静态IP
2台机器mysql密码一样，最好在同一局域网内，最好在mysql刚安装时就配置好，后面有数据了不好同步。
本文实现了2台机器mysql数据同步成功；
## 配置my.cnf
先关闭防火墙
service iptables status
2台机器除了server-id不同之外，配置项基本一样（把a改为b）。
 vim /etc/my.cnf 
在[mysqld]部分添加如下：
*#add*
log-bin=master-a-bin 
server-id=1
relay-log=master-a-relay-bin
log_slave_updates=1sync_binlog=1*#add*
### 配置项说明
两台MySQL均要开启binlog日志功能
log-bin  //[必须]启用二进制日志
server-id=1       //[必须]默认是1，一般取IP最后一段
两台MySQL的server-ID不能一样，需将其中一台修改为2
log_slave_updates表示slave将复制事件写进自己的二进制日志
sync_binlog=1。每进行1次事务提交之后，MySQL将进行一次fsync之类的磁盘同步指令来将binlog_cache中的数据强制写入磁盘。
修改配置后重启mysqld
service mysqld restart
其它配置项：待查
binlog-do-db =new_test (要记录的数据库,多个可换行多次设置)
replicate-do-db =new_test (要复制的数据库，多个可换行过个设置)
binlog-ignore-db=mysql //不对mysql库进行日志记录操作 如下意思雷同
binlog-ignore-db=test
binlog-ignore-db=information_schema
binlog-ignore-db=performance_schema
replicate-ignore-db=test  //不对test进行复制操作 如下意思雷同
replicate-ignore-db=mysql
replicate-ignore-db=information_schema
replicate-ignore-db=performance_schema
bing-address=0.0.0.0  //意思是允许所有 机器 服务器安全起见可设置为指定的服务器IP地址 
主从复制的时候时间必须要保持一致
iptables  -I  INPUT  -p  tcp  --dport  3306  -j ACCEP
## 1.主服务器A（123.1.1.123）创建用户并清空日志
mysql -uroot -p
grant replication slave on *****.***** to 'root'**@**'%' identified by 'pwd';   
flush privileges;
为了安全，最好将“%”改为具体的IP地址（如123.1.1.124），使该帐户只能在指定的从服务器上使用。
flush tables with **read** lock;
show binary logs;
reset master;
show binary logs;
unlock tables;
记录下 FILE 及 Position 的值，在后面进行从服务器操作的时候需要用到。
### 从服务器B清空日志
show binary logs;
reset master;
show binary logs;
从服务器通过change master to命令修改设置
change master to 
master_host='123.1.1.123',
master_user='root',
master_password='pwd',master_port=3306,
master_log_file='master-a-bin.000001',master_log_pos=106;
 以上全部复制后修改并粘贴
show slave status \G;
在输出的信息中：
Slave_IO_Running: No
Slave_SQL_Running: No
需要使其值为Yes
start slave;
show slave status \G;
错误：
Last_IO_Error: error connecting to master '[root@123.1.1.123](mailto:root@123.1.1.123):3306'
mysql -h 10.24.54.18 -ureplication -p
ERROR 1130 (HY000): Host '123.103.58.124' is not allowed to connect to this MySQL server
ERROR 1045 (28000): Access denied for user 'root'@'123.103.58.124' (using password: YES)
最后发现是密码不对。
## 2.设置从服务器B
在“从机”B上设置复制帐号
grant replication slave on *****.***** to 'root'**@**'%' identified by 'pwd';flush privileges;
清空“从机”日志：
show binary logs;
reset master;
show binary logs;
在原主服务器A上通过change master to命令修改设置，可以全部复制并粘贴：
change master to 
master_host='123.103.*.124',
master_user='root',
master_password='pwd',
master_port=3306,
master_log_file='master-b-bin.000001',
master_log_pos=106;
先
show slave status \G;
看看
Slave_IO_Running: No
Slave_SQL_Running: No
需要使其值为Yes
start slave;
show slave status \G;
应该有
Slave_IO_Running: Yes
Slave_SQL_Running: Yes
设置完后重启：service mysqld restart
如果数据不匹配
reset slave;
## 测试
show databases;
create database a1;
use a1;
create table t1**(****id** int auto_increment primary key,name varchar**(**30**)****)**;
insert into t1**(**name**)** values**(**'appBox1'**)**;
可以发现数据库已同步，但2台皆可写。
下一节将实现读写分离。
经测试，如果设置之前有数据不会同步，只能同步之后的
show variables like 'server_id';
其它：
RESET MASTER 
删除所有index file 中记录的所有binlog 文件，将日志索引文件清空，创建一个新的日志文件，这个命令通常仅仅用于第一次用于搭建主从关系的时的主库，
reset slave 将使slave 忘记主从复制关系的位置信息。该语句将被用于干净的启动, 它删除master.info文件和relay-log.info 文件以及所有的relay log 文件并重新启用一个新的relaylog文件。
使用reset slave之前必须使用stop slave 命令将复制进程停止。
