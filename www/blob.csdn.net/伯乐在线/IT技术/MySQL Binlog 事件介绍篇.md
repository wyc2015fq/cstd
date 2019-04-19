# MySQL Binlog 事件介绍篇 - 文章 - 伯乐在线
原文出处： [zhaohui](http://codingo.xyz/index.php/2017/11/13/mysql-events/)
**前言**
上一篇文件[MySql Binlog初识](http://blog.jobbole.com/113073/)，对Binlog的参数，格式以及个别事件做了详细介绍，但是Binlog事件数量比较多，上篇文章中没有对所有事件进行介绍；本文将对Binlog的事件进行简单说明，必要的时候通过SQL触发相关的事件，以下基于Mysql5.5，5.0以前的版本不考虑。
**Binlog事件**
1.UNKNOWN_EVENT
此事件从不会被触发，也不会被写入binlog中；发生在当读取binlog时，不能被识别其他任何事件，那被视为UNKNOWN_EVENT。
2.START_EVENT_V3
每个binlog文件开始的时候写入的事件，此事件被用在MySQL3.23 – 4.1，MYSQL5.0以后已经被FORMAT_DESCRIPTION_EVENT取代。
3.QUERY_EVENT
执行更新语句时会生成此事件，包括：create，insert，update，delete；
手动触发：


```
insert into btest values(1,100,'zhaohui');
 
| bin-log.000001 | 432 | Query       |         1 |         536 | use `test`; insert into btest values(1,100,'zhaohui')                                                                                                                                                          |
| bin-log.000001 | 536 | Xid         |         1 |         563 | COMMIT /* xid=30 */
```
4.STOP_EVENT
当mysqld停止时生成此事件
可以手动停止mysql，生成的事件：


```
| bin-log.000001 | 563 | Stop        |         1 |         582 |
```
5.ROTATE_EVENT
当mysqld切换到新的binlog文件生成此事件，切换到新的binlog文件可以通过执行flush logs命令或者binlog文件大于max_binlog_size参数配置的大小；
手动触发：


```
mysql> flush logs;
Query OK, 0 rows affected (0.24 sec)
 
mysql> show binlog events in 'bin-log.000002';
+----------------+-----+-------------+-----------+-------------+---------------------------------------+
| Log_name       | Pos | Event_type  | Server_id | End_log_pos | Info                                  |
+----------------+-----+-------------+-----------+-------------+---------------------------------------+
| bin-log.000002 |   4 | Format_desc |         1 |         107 | Server ver: 5.5.29-log, Binlog ver: 4 |
| bin-log.000002 | 107 | Rotate      |         1 |         148 | bin-log.000003;pos=4                  |
+----------------+-----+-------------+-----------+-------------+---------------------------------------+
```
6.INTVAR_EVENT
当sql语句中使用了AUTO_INCREMENT的字段或者LAST_INSERT_ID()函数；此事件没有被用在binlog_format为ROW模式的情况下。


```
insert into btest (age,name)values(100,'zhaohui');
 
mysql> show binlog events in 'bin-log.000003';
+----------------+-----+-------------+-----------+-------------+---------------------------------------------------------------+
| Log_name       | Pos | Event_type  | Server_id | End_log_pos | Info                                                          |
+----------------+-----+-------------+-----------+-------------+---------------------------------------------------------------+
| bin-log.000003 |   4 | Format_desc |         1 |         107 | Server ver: 5.5.29-log, Binlog ver: 4                         |
| bin-log.000003 | 107 | Query       |         1 |         175 | BEGIN                                                         |
| bin-log.000003 | 175 | Intvar      |         1 |         203 | INSERT_ID=2                                                   |
| bin-log.000003 | 203 | Query       |         1 |         315 | use `test`; insert into btest (age,name)values(100,'zhaohui') |
| bin-log.000003 | 315 | Xid         |         1 |         342 | COMMIT /* xid=32 */                                           |
+----------------+-----+-------------+-----------+-------------+---------------------------------------------------------------+
5 rows in set (0.00 sec)
```
btest表中的id为AUTO_INCREMENT，所以产生了INTVAR_EVENT
7.LOAD_EVENT
执行LOAD DATA INFILE 语句时产生此事件，在MySQL 3.23版本中使用；
8.SLAVE_EVENT
未使用的
9.CREATE_FILE_EVENT
执行LOAD DATA INFILE 语句时产生此事件，在MySQL4.0和4.1版本中使用；
10.APPEND_BLOCK_EVENT
执行LOAD DATA INFILE 语句时产生此事件，在MySQL4.0版本中使用；
11.EXEC_LOAD_EVENT
执行LOAD DATA INFILE 语句时产生此事件，在MySQL4.0和4.1版本中使用；
12.DELETE_FILE_EVENT
执行LOAD DATA INFILE 语句时产生此事件，在MySQL4.0版本中使用；
13.NEW_LOAD_EVENT
执行LOAD DATA INFILE 语句时产生此事件，在MySQL4.0和4.1版本中使用；
14.RAND_EVENT
执行包含RAND()函数的语句产生此事件，此事件没有被用在binlog_format为ROW模式的情况下；


```
mysql> insert into btest (age,name)values(rand(),'zhaohui');
 
mysql> show binlog events in 'bin-log.000003';
+----------------+-----+-------------+-----------+-------------+------------------------------------------------------------------+
| Log_name       | Pos | Event_type  | Server_id | End_log_pos | Info                                                             |
+----------------+-----+-------------+-----------+-------------+------------------------------------------------------------------+
......
| bin-log.000003 | 342 | Query       |         1 |         410 | BEGIN                                                            |
| bin-log.000003 | 410 | Intvar      |         1 |         438 | INSERT_ID=3                                                      |
| bin-log.000003 | 438 | RAND        |         1 |         473 | rand_seed1=223769196,rand_seed2=1013907192                       |
| bin-log.000003 | 473 | Query       |         1 |         588 | use `test`; insert into btest (age,name)values(rand(),'zhaohui') |
| bin-log.000003 | 588 | Xid         |         1 |         615 | COMMIT /* xid=48 */                                              |
+----------------+-----+-------------+-----------+-------------+------------------------------------------------------------------+
10 rows in set (0.00 sec)
```
15.USER_VAR_EVENT
执行包含了用户变量的语句产生此事件，此事件没有被用在binlog_format为ROW模式的情况下；


```
mysql> set @age=50;
Query OK, 0 rows affected (0.00 sec)
 
mysql> insert into btest (age,name)values(@age,'zhaohui');
Query OK, 1 row affected (0.12 sec)
 
mysql> show binlog events in 'bin-log.000003';
+----------------+-----+-------------+-----------+-------------+------------------------------------------------------------------+
| Log_name       | Pos | Event_type  | Server_id | End_log_pos | Info                                                             |
+----------------+-----+-------------+-----------+-------------+------------------------------------------------------------------+
......                                          |
| bin-log.000003 | 615 | Query       |         1 |         683 | BEGIN                                                            |
| bin-log.000003 | 683 | Intvar      |         1 |         711 | INSERT_ID=4                                                      |
| bin-log.000003 | 711 | User var    |         1 |         756 | @`age`=50                                                        |
| bin-log.000003 | 756 | Query       |         1 |         869 | use `test`; insert into btest (age,name)values(@age,'zhaohui')   |
| bin-log.000003 | 869 | Xid         |         1 |         896 | COMMIT /* xid=70 */                                              |
+----------------+-----+-------------+-----------+-------------+------------------------------------------------------------------+
15 rows in set (0.00 sec)
```
16.FORMAT_DESCRIPTION_EVENT
描述事件，被写在每个binlog文件的开始位置，用在MySQL5.0以后的版本中，代替了START_EVENT_V3


```
mysql> show binlog events in 'bin-log.000003';
+----------------+-----+-------------+-----------+-------------+---------------------------------------------------------------+
| Log_name       | Pos | Event_type  | Server_id | End_log_pos | Info                                                          |
+----------------+-----+-------------+-----------+-------------+---------------------------------------------------------------+
| bin-log.000003 |   4 | Format_desc |         1 |         107 | Server ver: 5.5.29-log, Binlog ver: 4                         |
......
```
17.XID_EVENT
支持XA的存储引擎才有，本地测试的数据库存储引擎是innodb，所有上面出现了XID_EVENT；innodb事务提交产生了QUERY_EVENT的BEGIN声明，QUERY_EVENT以及COMMIT声明，
如果是myIsam存储引擎也会有BEGIN和COMMIT声明，只是COMMIT类型不是XID_EVENT；
18.BEGIN_LOAD_QUERY_EVENT和EXECUTE_LOAD_QUERY_EVENT
执行LOAD DATA INFILE 语句时产生此事件，在MySQL5.0版本中使用；


```
mysql> LOAD DATA INFILE "D:/btest.sql" INTO TABLE test.btest FIELDS TERMINATED BY ',';
Query OK, 1 row affected (0.11 sec)
Records: 1  Deleted: 0  Skipped: 0  Warnings: 0
 
mysql> show binlog events in 'bin-log.000003';
+----------------+------+--------------------+-----------+-------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Log_name       | Pos  | Event_type         | Server_id | End_log_pos | Info                                                                                                                                                                             |
+----------------+------+--------------------+-----------+-------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
......                                                                                                                                                          |
| bin-log.000003 |  896 | Query              |         1 |         964 | BEGIN                                                                                                                                                                            |
| bin-log.000003 |  964 | Begin_load_query   |         1 |        1008 | ;file_id=3;block_len=21                                                                                                                                                          |
| bin-log.000003 | 1008 | Execute_load_query |         1 |        1237 | use `test`; LOAD DATA INFILE 'D:/btest.sql' INTO TABLE `btest` FIELDS TERMINATED BY ',' ENCLOSED BY '' ESCAPED BY '\\' LINES TERMINATED BY '\n' (`id`, `age`, `name`) ;file_id=3 |
| bin-log.000003 | 1237 | Xid                |         1 |        1264 | COMMIT /* xid=148 */                                                                                                                                                             |
+----------------+------+--------------------+-----------+-------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
19 rows in set (0.00 sec)
```
btest.sql内容如下：


```
999, 101, 'zhaohui'
```
19.TABLE_MAP_EVENT
用在binlog_format为ROW模式下，将表的定义映射到一个数字，在行操作事件之前记录（包括：WRITE_ROWS_EVENT，UPDATE_ROWS_EVENT，DELETE_ROWS_EVENT）；


```
mysql> insert into btest values(998,88,'zhaohui');
Query OK, 1 row affected (0.09 sec)
 
mysql> show binlog events in 'bin-log.000004';
+----------------+-----+-------------+-----------+-------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Log_name       | Pos | Event_type  | Server_id | End_log_pos | Info                                                                                                                                                                                                                           |
+----------------+-----+-------------+-----------+-------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
......                                                                                                                                                                                                         |
| bin-log.000004 | 776 | Query       |         1 |         844 | BEGIN                                                                                                                                                                                                                          |
| bin-log.000004 | 844 | Table_map   |         1 |         892 | table_id: 33 (test.btest)                                                                                                                                                                                                      |
| bin-log.000004 | 892 | Write_rows  |         1 |         943 | table_id: 33 flags: STMT_END_F                                                                                                                                                                                                 |
| bin-log.000004 | 943 | Xid         |         1 |         970 | COMMIT /* xid=20 */                                                                                                                                                                                                            |
+----------------+-----+-------------+-----------+-------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
14 rows in set (0.00 sec)
```
20.PRE_GA_WRITE_ROWS_EVENT,PRE_GA_UPDATE_ROWS_EVENT和PRE_GA_DELETE_ROWS_EVENT
以上三个事件已经过期，被其他事件代替；
PRE_GA_WRITE_ROWS_EVENT被WRITE_ROWS_EVENT代替；
PRE_GA_UPDATE_ROWS_EVENT被UPDATE_ROWS_EVENT代替；
PRE_GA_DELETE_ROWS_EVENT被DELETE_ROWS_EVENT代替；
21.WRITE_ROWS_EVENT、UPDATE_ROWS_EVENT和DELETE_ROWS_EVENT
以上三个事件都被用在binlog_format为ROW模式下，分别对应inset，update和delete操作；


```
mysql> insert into btest values(997,88,'zhaohui');
mysql> update btest set age=89 where id=997;
mysql> delete from btest where id=997;
 
mysql> show binlog events in 'bin-log.000004';
+----------------+------+-------------+-----------+-------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Log_name       | Pos  | Event_type  | Server_id | End_log_pos | Info                                                                                                                                                                                                                           |
+----------------+------+-------------+-----------+-------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
......                                                                                                                                                                                                          |
| bin-log.000004 | 1164 | Query       |         1 |        1232 | BEGIN                                                                                                                                                                                                                          |
| bin-log.000004 | 1232 | Table_map   |         1 |        1280 | table_id: 33 (test.btest)                                                                                                                                                                                                      |
| bin-log.000004 | 1280 | Write_rows  |         1 |        1331 | table_id: 33 flags: STMT_END_F                                                                                                                                                                                                 |
| bin-log.000004 | 1331 | Xid         |         1 |        1358 | COMMIT /* xid=24 */                                                                                                                                                                                                            |
| bin-log.000004 | 1358 | Query       |         1 |        1426 | BEGIN                                                                                                                                                                                                                          |
| bin-log.000004 | 1426 | Table_map   |         1 |        1474 | table_id: 33 (test.btest)                                                                                                                                                                                                      |
| bin-log.000004 | 1474 | Update_rows |         1 |        1548 | table_id: 33 flags: STMT_END_F                                                                                                                                                                                                 |
| bin-log.000004 | 1548 | Xid         |         1 |        1575 | COMMIT /* xid=25 */                                                                                                                                                                                                            |
| bin-log.000004 | 1575 | Query       |         1 |        1643 | BEGIN                                                                                                                                                                                                                          |
| bin-log.000004 | 1643 | Table_map   |         1 |        1691 | table_id: 33 (test.btest)                                                                                                                                                                                                      |
| bin-log.000004 | 1691 | Delete_rows |         1 |        1742 | table_id: 33 flags: STMT_END_F                                                                                                                                                                                                 |
| bin-log.000004 | 1742 | Xid         |         1 |        1769 | COMMIT /* xid=27 */                                                                                                                                                                                                            |
+----------------+------+-------------+-----------+-------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
```
22.INCIDENT_EVENT
主服务器发生了不正常的事件，通知从服务器并告知可能会导致数据处于不一致的状态；
23.HEARTBEAT_LOG_EVENT
主服务器告诉从服务器，主服务器还活着，不写入到日志文件中；
**总结**
本文对Binlog的所有事件进行了大体的介绍，必要的时候也介绍了触发事件的条件；但是并没有深入介绍事件的fix data和variable data，后续文章会继续介绍这一块。
**参考：**
[https://dev.mysql.com/doc/internals/en/event-meanings.html](https://dev.mysql.com/doc/internals/en/event-meanings.html)
