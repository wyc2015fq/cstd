# 初步认知MySQL metadata lock(MDL) - weixin_33985507的博客 - CSDN博客
2016年07月17日 12:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
http://blog.itpub.net/26515977/viewspace-1208250/
概述
随着5.5.3引入MDL，更多的Query被“Waiting for table metadata lock”给\'炕\'了
SHOW PROCESSLIST的输出也有之前的"Locked"变得粒度更加细的\'Waiting for table metadata lock\'
引入MDL，当需要访问、修改表结构时，都需要对元数据上锁(读/写)
MDL在Server层，保护表数据结构，而非数据本身。Server比之前版本繁忙多了
MDL意味着DDL，一旦DDL被阻塞，那么面向该表的所有Query都会被挂起，包括Select，不过5.6作了改进，5.5可通过参数控制
假如没有MDL
在5.1版本，MDL的生命随语句的结束而释放，并非在事务提交后结束，意味着事务不是真正的隔离
因为同一条Query在2次执行期间倘若被插入了DDL，那么得到的结果将会不同，下面是个例子
```
会话1：
mysql> select version();
+------------+
| version()  |
+------------+
| 5.1.72-log |
+------------+
1 row in set (0.00 sec)
mysql> select @@tx_isolation;
+-----------------+
| @@tx_isolation  |
+-----------------+
| REPEATABLE-READ |
+-----------------+
1 row in set (0.00 sec)
mysql> begin;
Query OK, 0 rows affected (0.00 sec)
mysql> select * from t where id=1;
+----+--------+
| id | name   |
+----+--------+
|  1 | python |
+----+--------+
1 row in set (0.04 sec)
会话2：
mysql> alter table t add column comment varchar(200) default \'I use Python\';
Query OK, 3 rows affected (0.02 sec)
Records: 3  Duplicates: 0  Warnings: 0
会话1：
mysql> select * from t where id=1;
Empty set (0.00 sec)
mysql> rollback;
Query OK, 0 rows affected (0.00 sec)
mysql> begin;
Query OK, 0 rows affected (0.00 sec)
mysql> select * from t where id=1;
+----+--------+--------------+
| id | name   | comment      |
+----+--------+--------------+
|  1 | python | I use Python |
+----+--------+--------------+
1 row in set (0.00 sec)
```
与上面的不同，在5.5 MDL拉长了生命长度，与事务同生共死，只要事务还在，MDL就在，由于事务持有MDL锁，任何DDL在事务期间都休息染指，下面是个例子
```
会话1：
mysql> select version();
+------------+
| version()  |
+------------+
| 5.5.16-log |
+------------+
1 row in set (0.01 sec)
mysql> begin;
Query OK, 0 rows affected (0.00 sec)
mysql> select * from t order by id;
+----+------+
| id | name |
+----+------+
|  1 | a    |
|  2 | e    |
|  3 | c    |
+----+------+
3 rows in set (0.00 sec)
会话2：
mysql> alter table t add column cc char(10) default \'c lang\'; <<===Hangs
会话3：
mysql> show processlist;
+----+------+-----------+------+---------+------+---------------------------------+-------------------------------------------------------+
| Id | User | Host      | db   | Command | Time | State                           | Info                                                  |
+----+------+-----------+------+---------+------+---------------------------------+-------------------------------------------------------+
|  2 | root | localhost | db1  | Sleep   |  191 |                                 | NULL                                                  |
|  3 | root | localhost | db1  | Query   |  125 | Waiting for table metadata lock | alter table t add column cc char(10) default \'c lang\' |
|  4 | root | localhost | NULL | Query   |    0 | NULL                            | show processlist                                      |
+----+------+-----------+------+---------+------+---------------------------------+-------------------------------------------------------+
```
回滚了会话1的事务后，我们看下alter主要被谁炕了
```
mysql> show profiles;
+----------+---------------+-------------------------------------------------------+
| Query_ID | Duration      | Query                                                 |
+----------+---------------+-------------------------------------------------------+
|        1 | 1263.64100500 | alter table t add column dd char(10) default \' Elang\' |
+----------+---------------+-------------------------------------------------------+
1 row in set (0.00 sec)
mysql> show profile for query 1;
+------------------------------+------------+
| Status                       | Duration   |
+------------------------------+------------+
| starting                     |   0.000124 |
| checking permissions         |   0.000015 |
| checking permissions         |   0.000010 |
| init                         |   0.000023 |
| Opening tables               |   0.000063 |
| System lock                  |   0.000068 |
| setup                        |   0.000082 |
| creating table               |   0.034159 |
| After create                 |   0.000185 |
| copy to tmp table            |   0.000309 |
| rename result table          | 999.999999 |
| end                          |   0.004457 |
| Waiting for query cache lock |   0.000024 |
| end                          |   0.000029 |
| query end                    |   0.000009 |
| closing tables               |   0.000030 |
| freeing items                |   0.000518 |
| cleaning up                  |   0.000015 |
+------------------------------+------------+
18 rows in set (0.00 sec)
```
显然，在"rename result table"处发生了等待，在我们alter一张大表的时候，真正触礁的地方也就这里，这也呼应下面的案例1
案例
案例 1：
5.5下给大表加主键时会锁住读的问题
加锁策略：
1）上MDL读锁
2）操作数据，最耗时，需copy data，简易流程如下：
       a) 创建临时表A，重定义A为修改后的表结构
       b) 从原表读取数据插入到A表
3）将MDL读锁升级为写锁
       c) 删除原表，将A重命名为原表名
4）释放MDL写锁
5.5必须在打开了old_alter_table后才会采取上述策略，但事实上，无论其是否打开
对主键操作都必须copy data，基于此5.6已经改进，从而把MDL写锁仅圈在重命名的操作
这样阻塞时间非常之短，几乎可以认为不阻塞读，在5.5时做alter建议将其打开，减小持锁时间
案例 2：
mysqldump备份不一致导致备库延迟问题
由于5.1中没有引入MDL，所有在mysqldump备份过程中，并发DDL都会对其产生影响，导致备份集不一致
最终表现是使用此备份集恢复的备库在relay主库binlog会出现slave error，造成在备库上的读与主库数据不一致
因为5.5有了MDL，所以–single-transaction时，事务内操作过的表都会持有MDL，因此不会被DDL破坏
例如，mysqldump已经备份了a，b，c表，因为它们在事务内，事务还没提交，它们的MDL不会释放
因此另外的线程如果做a,b,c中任意一张表的DDL操作，都会出现Waiting for table metadata lock，而还没备份到的表不会持有MDL，因此还可以做DDL
监控
lock_wait_timeout
```
mysql> show variables like \'lock_wait_timeout\';
+-------------------+----------+
| Variable_name     | Value    |
+-------------------+----------+
| lock_wait_timeout | 31536000 |
+-------------------+----------+
1 row in set (0.00 sec)
```
官方文档解释：
```
This variable specifies the timeout in seconds for attempts to acquire metadata locks. 
The permissible values range from 1 to 31536000 (1 year). The default is 31536000
```
5.5默认是1年！太长了，个人认为不是很合理，这个需要我们DBA自己调整，50秒会比较好
另外，这个既然针对MDL的超时设置，那为何不命名为"metadata_lock_wait_timeout"呢？
show open tables：当前打开的表，与flush tables结合，能判断表的活性
show status like \'Open%tables\';是上面的简化输出
诊断
对于MyISAM表：
当发现Query由于“Waiting for table metadata lock”而挂起等待MyISAM表，这是因为该表已经被包含在一个未提交的事务，无论其是否有意
而且常见的锁追踪技巧此时也没有作用，比如InnoDB Lock Monitor, InnoDB Status, mysqladmin debug output, INFORMATION_SCHEMA等
事实上，在select一张表之前先SET AUTOCOMMIT=0，那么之后，直到该事务提交或结束后其它DDL才能够得到该表的控制权
测试：
```
Connection #1:
create table t1 (id int) engine=myisam;
set @@autocommit=0;
select * from t1;
Connection #2:
alter table t1 rename to t2; <-- Hangs
```
对于InnoDB表：
倘若在InnoDB表遇见此问题，我们能够用SHOW ENGINE INNODB STATUS，优先查看transactions部分
测试：
```
create table t3 (id int) engine=innodb;
create table t4 (id int) engine=innodb;
delimiter |
CREATE TRIGGER t3_trigger AFTER INSERT ON t3
  FOR EACH ROW BEGIN
    INSERT INTO t4 SET id = NEW.id;
  END;
|
delimiter ;
```
```
Connection #1:
begin;
insert into t3 values (1);
```
```
Connection #2:
drop trigger if exists t3_trigger; <-- Hangs
mysql> SHOW ENGINE INNODB STATUS\G;
....
....
....
------------
TRANSACTIONS
------------
Trx id counter BF03
Purge done for trx\'s n:o < BD03 undo n:o < 0
History list length 82
LIST OF TRANSACTIONS FOR EACH SESSION:
---TRANSACTION 0, not started
MySQL thread id 4, OS thread handle 0xa7d3fb90, query id 40 localhost root
show engine innodb status
---TRANSACTION BF02, ACTIVE 38 sec
2 lock struct(s), heap size 320, 0 row lock(s), undo log entries 2
MySQL thread id 2, OS thread handle 0xa7da1b90, query id 37 localhost root
...
...
...
```
官方文档对 TRANSACTIONS解释：
```
TRANSACTIONS
If this section reports lock waits, your applications might have lock contention. 
The output can also help to trace the reasons for transaction deadlocks.
```
除此之外，我们还可以利用information_schema，以下是几个有用SQL：
① 锁等待
`SELECT * FROM INNODB_LOCK_WAITS`
② 被阻塞的事务
```
SELECT * 
FROM INNODB_LOCKS 
WHERE LOCK_TRX_ID IN (SELECT BLOCKING_TRX_ID FROM INNODB_LOCK_WAITS)
```
或者
```
SELECT INNODB_LOCKS.* 
FROM INNODB_LOCKS
JOIN INNODB_LOCK_WAITS
  ON (INNODB_LOCKS.LOCK_TRX_ID = INNODB_LOCK_WAITS.BLOCKING_TRX_ID)
```
③ 指定表上的锁
```
SELECT * FROM INNODB_LOCKS 
WHERE LOCK_TABLE = db_name.table_name
```
④ 事务与锁
```
SELECT TRX_ID, TRX_REQUESTED_LOCK_ID, TRX_MYSQL_THREAD_ID, TRX_QUERY
FROM INNODB_TRX
WHERE TRX_STATE = \'LOCK WAIT\'
```
当然，最好的办法还是综合运用前面2种
与table cache的关系
先明白table cache所解决的问题域：fd(文件描述符)打开/关闭太过频繁导致资源消耗
那么它是如何解决的？tc通过cache所有打开的fd，当有新的连接请求时不需重新打开，结束后也不用关闭
DDL操作终究会被阻塞，即使table cache含有其所需的fd。MySQL认为，tc旧条目必须失效
访问该表的DDL操作必须重新打开fd，下面是个测试
```
会话1：
mysql> show status like \'Open%tables\';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| Open_tables   | 26    |  <==当前打开的表数量
| Opened_tables | 2     |  <==已经打开的表数量
+---------------+-------+
2 rows in set (0.00 sec)
会话2：
mysql> alter table t add column Oxx char(20) default \'ORACLE\';
Query OK, 3 rows affected (0.05 sec)
Records: 3  Duplicates: 0  Warnings: 0
会话1：
mysql> select * from t order by id;
+----+------+--------+--------+---------+---------+-------+--------+--------+--------+--------+
| id | name | cc     | dd     | EE      | ff      | OO    | OE     | OF     | OX     | Oxx    |
+----+------+--------+--------+---------+---------+-------+--------+--------+--------+--------+
|  1 | a    | c lang |  Elang |  Golang |  Golang | MySQL | ORACLE | ORACLE | ORACLE | ORACLE |
|  2 | e    | c lang |  Elang |  Golang |  Golang | MySQL | ORACLE | ORACLE | ORACLE | ORACLE |
|  3 | c    | c lang |  Elang |  Golang |  Golang | MySQL | ORACLE | ORACLE | ORACLE | ORACLE |
+----+------+--------+--------+---------+---------+-------+--------+--------+--------+--------+
3 rows in set (0.00 sec)
mysql> show status like \'Open%tables\';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| Open_tables   | 27    |
| Opened_tables | 3     |
+---------------+-------+
2 rows in set (0.00 sec)
会话2：
mysql> alter table t add column Oxf char(20) default \'ORACLE\';
Query OK, 3 rows affected (0.06 sec)
Records: 3  Duplicates: 0  Warnings: 0
会话1：
mysql> show status like \'Open%tables\';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| Open_tables   | 26    |
| Opened_tables | 3     |
+---------------+-------+
2 rows in set (0.00 sec)
```
结论：
当需要对"热表"做DDL，需要特别谨慎，否则，容易造成MDL等待，导致连接耗尽或者拖垮Server
参考资料：
http://dinglin.iteye.com/blog/1884696
http://www.xaprb.com/blog/2012/08/28/debugging-metadata-locking-in-mysql-5-5/
http://blog.chinaunix.net/uid-28212952-id-3400571.html
http://www.chriscalender.com/?p=1189
http://www.mysqlperformanceblog.com/2013/02/01/implications-of-metadata-locking-changes-in-mysql-5-5/
By 迦叶
2013-12-17
Good Luck
