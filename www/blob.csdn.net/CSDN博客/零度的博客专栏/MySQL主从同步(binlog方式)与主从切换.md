# MySQL主从同步(binlog方式)与主从切换 - 零度的博客专栏 - CSDN博客
2017年04月20日 15:19:49[零度anngle](https://me.csdn.net/zmx729618)阅读数：2269
### 1 主从同步配置
#### 1、部署环境：
     机器A：192.168.2.63（主） 
     机器B：192.168.2.94（从） 
[MySQL](http://lib.csdn.net/base/14)：5.6.*以上
### 2、主（master）配置：
修改mysql安装目录下my.ini配置文件
``
log_bin=mysql-bin-1#文件名mysql-bin-1
server_id=1#服务ID，用于区分服务，范围1~2^32-1#MySQL 磁盘写入策略以及数据安全性#每次事务提交时MySQL都会把log buffer的数据写入log file，并且flush(刷到磁盘)中去
innodb_flush_log_at_trx_commit=1#当sync_binlog =N (N>0) ，MySQL 在每写 N次 二进制日志binary log时，会使用fdatasync()函数将它的写二进制日志binary log同步到磁盘中去。
#sync_binlog 的默认值是0，像操作系统刷其他文件的机制一样，MySQL不会同步到磁盘中去而是依赖操作系统来刷新binary log。
sync_binlog=1
binlog-do-db=db1 #同步数据库
binlog-do-db=db2
#mysql复制模式，三种：SBR（基于sql语句复制），RBR（基于行的复制），MBR（混合模式复制）
binlog_format=MIXED #混合模式复制
expire_logs_days=7#binlog过期清理时间
max_binlog_size=20M #binlog每个日志文件大小
![](http://static.blog.csdn.net/images/save_snippets.png)
配置后，重启mysql服务。
#### 3、从（slave）配置
修改mysql安装目录下my.ini配置文件
``
log_bin=mysql-bin-2#文件名mysql-bin-2
server_id=2#服务ID，用于区分服务，范围1~2^32-1#MySQL 磁盘写入策略以及数据安全性#每次事务提交时MySQL都会把log buffer的数据写入log file，并且flush(刷到磁盘)中去
innodb_flush_log_at_trx_commit=1#当sync_binlog =N (N>0) ，MySQL 在每写 N次 二进制日志binary log时，会使用fdatasync()函数将它的写二进制日志binary log同步到磁盘中去。
#sync_binlog 的默认值是0，像操作系统刷其他文件的机制一样，MySQL不会同步到磁盘中去而是依赖操作系统来刷新binary log。
sync_binlog=1
binlog-do-db=db1 #同步数据库
binlog-do-db=db2
#mysql复制模式，三种：SBR（基于sql语句复制），RBR（基于行的复制），MBR（混合模式复制）
binlog_format=MIXED #混合模式复制
expire_logs_days=7#binlog过期清理时间
max_binlog_size=20M #binlog每个日志文件大小
![](http://static.blog.csdn.net/images/save_snippets.png)
从（slave）配置跟主配置类似，server-id需要区别开来。重启mysql服务。
slave上同样配置了log_bin以及磁盘写入策略等，目的是为了主从切换，下面会继续讲解。
#### 4、创建同步账号
主（master）创建repl账号，从（slave）通过此账号访问主（master）同步数据：
 mysql> grant replication slave on *.* to'repl'@'%' identified by'repl';
 格式：mysql> GRANT REPLICATION SLAVE ON *.* TO'帐号'@'从服务器IP或主机名' IDENTIFIED BY'密码';
![](http://static.blog.csdn.net/images/save_snippets_01.png)
## 5、 开启主从同步
主（master）上查看binlog日志文件，以及坐标。
```
mysql> show master status;
+------------------+----------+--------------+------------------+
| File       | Position | Binlog_Do_DB | Binlog_Ignore_DB |
+------------------+----------+--------------+------------------+
| mysql-bin-1.000001 |    70 | db1,db2   |     |
+------------------+----------+--------------+------------------+
1 row in set (0.00 sec)
```
![](http://static.blog.csdn.net/images/save_snippets.png)
从（slave）配置访问信息，开启同步。
```
mysql> change master to
    -> master_host='192.168.2.63',
    -> master_port=3306,
    -> master_user='repl',
    -> master_password='repl',
    -> master_log_file='mysql-bin-1.000001',
    -> master_log_pos=76;
Query OK, 0 rows affected, 2 warnings (0.34 sec)
mysql> start slave;
Query OK, 0 rows affected (0.04 sec)
mysql> show slave status\G
*************************** 1. row ***************************
               Slave_IO_State: Waiting for master to send event
                  Master_Host: 192.168.2.63
                  Master_User: repl
                  Master_Port: 3306
                Connect_Retry: 60
              Master_Log_File: mysql-bin-1.000001
          ...........
             Slave_IO_Running: Yes
            Slave_SQL_Running: Yes
          ...........
      Slave_SQL_Running_State: Slave has read all relay log; waiting for the slave I/O
          ...........
1 row in set (0.00 sec)
```
### 2、主从切换
#### 1、停止从库线程
从库状态确认，确保从库执行完了relay log更新后，停止IO_THREAD线程。
```
mysql> show slave status\G
*************************** 1. row ***************************
               Slave_IO_State: Waiting for master to send event
          ...........
             Slave_IO_Running: Yes
            Slave_SQL_Running: Yes
          ...........
      Slave_SQL_Running_State: Slave has read all relay log; waiting for the slave I/O
          ...........
```
![](http://static.blog.csdn.net/images/save_snippets.png)
## 停止从库IO_THREAD线程
```
mysql> stop slave io_thread;
Query OK, 0 rows affected (0.06 sec)
```
![](http://static.blog.csdn.net/images/save_snippets_01.png)
#### 2、从库上升为主库
```
mysql> grant replication slave on *.* to 'repl'@'%' identified by 'repl';
mysql> stop slave;
Query OK, 0 rows affected (0.01 sec)
mysql> reset master;
Query OK, 0 rows affected (0.06 sec)
mysql> reset slave all;
mysql>show slave status\G
Empty set (0.00 sec)
mysql>show master status;
+------------------+----------+--------------+------------------+
| File       | Position | Binlog_Do_DB | Binlog_Ignore_DB |
+------------------+----------+--------------+------------------+
| mysql-bin-2.000001 |    639| db1,db2   |     |
+------------------+----------+--------------+------------------+
```
![](http://static.blog.csdn.net/images/save_snippets.png)
reset slave all 命令会删除从库的 replication 参数，之后 show slave status\G 的信息返回为空。
## 3、主库下降为从库
```
mysql> change master to
    -> master_host='192.168.2.94',
    -> master_port=3306,
    -> master_user='repl',
    -> master_password='repl',
    -> master_log_file='mysql-bin-2.000001',
    -> master_log_pos=639;
Query OK, 0 rows affected, 2 warnings (0.34 sec)
mysql> start slave;
Query OK, 0 rows affected (0.04 sec)
```
