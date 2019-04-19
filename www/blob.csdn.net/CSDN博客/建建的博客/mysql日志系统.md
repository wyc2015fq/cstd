# mysql日志系统 - 建建的博客 - CSDN博客
2017年03月15日 10:54:51[纪建](https://me.csdn.net/u013898698)阅读数：111
个人分类：[Mysql](https://blog.csdn.net/u013898698/article/category/6790249)

mysql日志的分类：
- 服务器日志
- - 记录进程启动运行过程中的特殊事件，帮助分析mysql服务遇到的问题。
- 根据需求抓取特定的SQL语句，追踪性能可能存在的问题的业务SQL。
- 事务日志--记录内容不记录状态
- - 记录应用程序对数据的所有更改
- 可用于数据恢复
- 可用于实例间数据同步
服务器日志：
- 服务错误日志--log_error
- 慢查询日志
- 综合查询日志
事务日志：
- 存储引擎事务日志
- 二进制日志
服务器错误日志：
- 记录实例启动运行过程中重要消息
- 配置参数
- - log_error=/data/mysql_data/node-1/mysqld.log
- 内容并非全是错误消息
- 如果mysqld进程无法正常启动首先查看错误日志
```
mysql> show global variables like "log_error";
+---------------+--------------------------+
| Variable_name | Value                    |
+---------------+--------------------------+
| log_error     | /var/log/mysql/error.log |
+---------------+--------------------------+
**1** row in set (**0.00** sec)
 
##tail -f  error.log 实时监控这个文件中的；
```
tail -f  error.log 实时监控这个文件中的；
慢查询日志
- 记录执行时间超过一定阈值的SQL语句
- 配置参数
- - slow_query_log =1-----是否打开
- slow_query_log_file = /data/mysql_data/node-1/mysql-slow.log --位置
- long_query_time=5-----阈值时间
- 用于分析系统中可能存在性能问题的SQL
#查看慢日志是否打开以及日志路径
```
mysql> show global variables like '%slow%';
+---------------------+----------------------------------+
| Variable_name       | Value                            |
+---------------------+----------------------------------+
| log_slow_queries    | OFF                              |
| slow_launch_time    | **2**                                |
| slow_query_log      | OFF                              |
| slow_query_log_file | /var/lib/mysql/itcast01-slow.log |
+---------------------+----------------------------------+
**4** rows in set (**0.00** sec)
```
#打开慢查询日志
```
mysql> set global slow_query_log =**1**;
Query OK, **0** rows affected (**0.00** sec)
 
mysql> show global variables like '%slow%';
+---------------------+----------------------------------+
| Variable_name       | Value                            |
+---------------------+----------------------------------+
| log_slow_queries    | ON                               |
| slow_launch_time    | **2**                                |
| slow_query_log      | ON                               |
| slow_query_log_file | /var/lib/mysql/itcast01-slow.log |
+---------------------+----------------------------------+
**4** rows in set (**0.00** sec)
```
#查看慢查询日志的阈值
```
mysql> show global variables like 'long_query_time';
+-----------------+-----------+
| Variable_name   | Value     |
+-----------------+-----------+
| long_query_time | **10.000000** |
+-----------------+-----------+
```
设置阈值，通常5秒；
```
mysql> set global  long_query_time = **1**;
Query OK, **0** rows affected (**0.00** sec)
 
mysql> show global variables like 'long_query_time';
+-----------------+----------+
| Variable_name   | Value    |
+-----------------+----------+
| long_query_time | **1.000000** |
+-----------------+----------+
**1** row in set (**0.00** sec)
```
综合查询日志
- 如果开启将会记录系统中所有SQL语句
- 配置参数
- - general_log = 1
- general_log_file =  /data/mysql_data/node-1/mysql-gen.log
- 偶尔用于帮助分析系统问题，对性能有影响
```
mysql> show global variables like '%general%';
+------------------+-----------------------------+
| Variable_name    | Value                       |
+------------------+-----------------------------+
| general_log      | OFF                         |
| general_log_file | /var/lib/mysql/itcast01.log |
+------------------+-----------------------------+
**2** rows in set (**0.00** sec)
```
查询日志的输出与文件切换
- 日志 输出参数
- - log_output = {file|table|none}
- 如果日志文件过大，可以定期截断并切换新文件
- - flush logs；
```
mysql> show global variables like '%log_output%';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| log_output    | FILE  |
+---------------+-------+
**1** row in set (**0.00** sec)
```
存储引擎事务日志---只记录数据变化；
- 部分存储引擎拥有重做日志(redo log)
- 如innodb，TokuDB等WAL(Write Ahead Log)机制存储引擎
- 日志随着事务commit优先持久化，确保异常恢复不丢数据。
- 日志顺序写性能较好
innodb事务日志重用机制
- innodb事务日志采用两组文件交替重用
     情况一：
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160723101929029-562959658.png)
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160723101940013-237362181.png)
情况二：
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160723101950451-1725762570.png)
所以在写入频繁的线上业务中，建议将ib_logfile的大小调大；这样有利于并发；
二进制日志binlog
- binlog(binary log)
- 记录数据引起数据变化的SQL语句或数据逻辑变化的内容
- mysql服务层记录，无关存储引擎
- binlog的主要作用：
- - 基于备份恢复数据
- 数据库主从同步
- 挖掘分析SQL语句
开启binlog
- 参数
- - log_bin = c:/tmp/mylog/mysql-bin(静态参数，设置为非0，就认为binlog打开；设置为1，则目录在mysql数据目录中)
- sql_log_bin = 1（mysql选择session是否记录binlog ）
- sync_binlog = 1(mysql，binlog持久化的方式；0，不主动刷新到磁盘，而是由脏数据刷新时来刷；1，则每一条就主动刷新到磁盘；100，则是每100条才主动刷新到磁盘)
```
mysql> show global variables like 'log_bin';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| log_bin       | OFF   |
+---------------+-------+
**1** row in set (**0.00** sec)
```
binlog管理
- 主要参数
- - max_binlog_size  = 100MB
- expire_logs_days  =7---------自动保存多少天的binlog文件
- binlog始终生成新文件，不会重用
- 手工清理binlog
- - purge binary logs to 'mysql-bin.000009'
- purge binary logs before '2015-06-01 22:45:34'
查看binlog内容
- 日志(log)
- - show binlog events in 'mysql-bin.000011'
- show binlog events in 'mysql-bin.000011' from 60(position) limit 3;
- mysqlbinlog工具
- - mysqlbinlog c:/tmp/mylog/mysql-bin.000001 --start-datetime| --stop--datetime --start-position|--stop-position
binlog格式
- 主要参数
- - binlog_format={row|satement|mixed}
- 查看row模式的binlog内容
- - mysqlbinlog --base64-output=decode-rows -v c:/tmp/mylog/mysql-bin.000001
```
show binary logs;
show binlog events in 'mysql-bin.000001'
```
binlog 只记录对数据产生变化的 语句；
但是有局限：
如果使用uuid(),每次产生不一样的值；来插入数据，
binlog,记录的是 insert into tab values （uuid()）;
此时通过回放，binlog不能得到原数据；
因为每次uuid()产生不一样的值。
所以mysql允许使用另一种格式记录binlog，只记录数据变化值；
可以将binlog_format 设置成row;
而binlog_format 设置成satement,记录具体操作语句；
而binlog_format 设置成mixed,mysql自动判断使用row,还是satement;
总结
- 服务日志：
- - log_error
- - 记录实例启动运行过程中重要消息，
- 内容并非全是错误消息，
- mysqld无法启动首先查看错误日志；
- 慢查询日志；
- 综合查询日志
- - 会记录系统中所有SQL语句，
- 偶尔用于帮助分析系统问题，对性能有影响；
- 日志的输出：
- - log_output=file,table,none;
- 如果日志文件过大，可以定期截断并切换新文件，flush logs;
- 事务日志：
- - redo log
- - 只记录数据变化
- innodb事务日志重用机制，ib_logfile大会利于写多业务；
- bin log
- - 记录引起数据变化的SQL语句或数据变化的内容，
- 在mysql服务层记录，
- 主要用于：
- - 基于备份恢复数据，
- 数据库主从同步，
- 挖掘分析SQL；
- 主要参数：
- - row 只记录数据变化，
- satement 记录引起数据变化的SQL，
- max混合两种；
- 
