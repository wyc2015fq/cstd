# Mysql 5.6主从同步配置与解决方案 - z69183787的专栏 - CSDN博客
2016年12月27日 18:24:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2806
主库IP：192.168.1.10
从库IP：192.168.1.11
1、主库配置编辑my.cnf：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
# 启用二进制日志
log_bin = mysql-bin
server-id = 111
log-bin-index=mysql-bin.index
sync_binlog=1
binlog_format=mixed
binlog-do-db = testdb //同步数据库
binlog-ignore-db = mysql
binlog-ignore-db = performance_schema
binlog-ignore-db = information_schema
binlog_checksum=NONE
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
2、创建同步账号
```
mysql> grant replication slave on *.* to slave@192.168.1.11 identified by '123456'
```
3、主库状态
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
mysql> flush privileges;
mysql> show master status;
+------------------+----------+--------------+------------------+
| File             | Position | Binlog_Do_DB | Binlog_Ignore_DB |
+------------------+----------+--------------+------------------+
| mysql-bin.000008 |      337 |              |                  |
+------------------+----------+--------------+------------------+
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
记录下二进制日志文件名和位置
4、从库配置文件配置
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
# [必须]启用二进制日志
log-bin=mysql-bin   
# [必须]服务器唯一ID，默认是1，一般取IP最后一段
server-id=222
relay-log-index = slave-relay-bin.index
relay-log = slave-relay-bin
sync_master_info = 1
sync_relay_log = 1
sync_relay_log_info = 1
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
5、配置连接主库
```
mysql> change master to master_host='192.168.1.10',master_user='slave',master_password='123456', master_log_file='mysql-bin.000008',master_log_pos=337;
```
6、开始同步
```
mysql> start slave;
mysql> show slave status\G;
```
7、正常状态
```
Slave_IO_Running: Yes
Slave_SQL_Running: Yes
```
8、解决主从不同步
先上Master库：
mysql>show processlist;   查看下进程是否Sleep太多。发现很正常。
show master status; 也正常。
mysql> show master status;
+-------------------+----------+--------------+-------------------------------+
| File              | Position | Binlog_Do_DB | Binlog_Ignore_DB              |
+-------------------+----------+--------------+-------------------------------+
| mysqld-bin.000001 |     3260 |              | mysql,test,information_schema |
+-------------------+----------+--------------+-------------------------------+
1 row in set (0.00 sec)
再到Slave上查看
mysql> show slave status\G                                                
Slave_IO_Running: Yes
Slave_SQL_Running: No
可见是Slave不同步
下面介绍两种解决方法：
方法一：忽略错误后，继续同步
该方法适用于主从库数据相差不大，或者要求数据可以不完全统一的情况，数据要求不严格的情况
解决： 
stop slave;
#表示跳过一步错误，后面的数字可变
set global sql_slave_skip_counter =1;
start slave;
之后再用mysql> show slave status\G  查看：
Slave_IO_Running: Yes
Slave_SQL_Running: Yes
ok，现在主从同步状态正常
方式二：重新做主从，完全同步
该方法适用于主从库数据相差较大，或者要求数据完全统一的情况
解决步骤如下：
1.先进入主库，进行锁表，防止数据写入
使用命令：
mysql> flush tables with read lock;
注意：该处是锁定为只读状态，语句不区分大小写
2.进行数据备份 
#把数据备份到mysql.bak.sql文件
[root@server01 mysql]#mysqldump -uroot -p -hlocalhost > mysql.bak.sql
这里注意一点：数据库备份一定要定期进行，确保数据万无一失
3.查看master 状态
mysql> show master status;
+-------------------+----------+--------------+-------------------------------+
| File              | Position | Binlog_Do_DB | Binlog_Ignore_DB              |
+-------------------+----------+--------------+-------------------------------+
| mysqld-bin.000001 |     3260 |              | mysql,test,information_schema |
+-------------------+----------+--------------+-------------------------------+
1 row in set (0.00 sec)
4.把mysql备份文件传到从库机器，进行数据恢复
#使用scp命令
[root@server01 mysql]# scp mysql.bak.sql root@192.168.128.11:/tmp/
5.停止从库的状态
mysql> stop slave;
6.然后到从库执行mysql命令，导入数据备份
mysql> source /tmp/mysql.bak.sql
7.设置从库同步，注意该处的同步点，就是主库show master status信息里的| File| Position两项
change master to master_host = '192.168.128.10', master_user = 'slave', master_port=3306, master_password='123456', master_log_file = 'mysqld-bin.000001', master_log_pos=3260;
8.重新开启从同步
mysql> start slave;
9.查看同步状态
mysql> show slave status\G  查看：
Slave_IO_Running: Yes
Slave_SQL_Running: Yes
