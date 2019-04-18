# 二、MySQL主从切换 - z69183787的专栏 - CSDN博客
2017年04月15日 11:40:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：371
个人分类：[数据库-主从同步](https://blog.csdn.net/z69183787/article/category/6863121)

参考上一篇博客 MySQL主从同步[http://blog.csdn.net/juded/article/details/54600294](http://blog.csdn.net/juded/article/details/54600294)
## 从库状态确认
确保从库执行完了relay log更新后，停止IO_THREAD线程。
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
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
## 停止从库IO_THREAD线程
```
mysql> stop slave io_thread;
Query OK, 0 rows affected (0.06 sec)
```
- 1
- 2
## 从库上升为主库
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
reset slave all 命令会删除从库的 replication 参数，之后 show slave status\G 的信息返回为空。
## 主库上升为从库
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
