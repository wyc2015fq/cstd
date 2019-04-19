# mysql事务 - 建建的博客 - CSDN博客
2017年03月15日 11:04:15[纪建](https://me.csdn.net/u013898698)阅读数：175
什么是事务
一系列有序的数据库操作：
- 要么全部成功
- 要么全部回退到操作前状态
- 中间状态对其他连接不可见
事务的基本操作
|基本操作|说明|
|----|----|
|start transaction；|开始事务|
|commit；|提交（全部完成）|
|rollback；|回滚（回到初始状态）|
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719222949122-1580507276.png)
图中便是数据库中的事务操作；及 开始事务-->SQL操作-->commit/rollback；
savepoint，保存点，在事务回滚时，可以回滚到保存点；
而提交只能全部提交不能仅提交保存点；
```
savepoint  savepoint_name;
rollback to savepoint_name;
commit;
```
事务开始也可以用 begin；或者 start transaction；
清空表数据：
```
mysql> select * from ttt;
+---+------+------+
| a | b    | c    |
+---+------+------+
| **1** |    **1** |    **1** |
+---+------+------+
**1** row in set (**0.00** sec)
mysql> truncate table ttt;
Query OK, **0** rows affected (**0.01** sec)
mysql> select * from ttt;
Empty set (**0.00** sec)
```
当多个连接同时连在一个数据库中时，truncate 命令无法使用
自动提交（默认开启）
即在我们数据语句后，系统自动给我们加上了COMMIT；
- autocommit可以在session级别设置
- 每个DML操作都自动提交
- DDL永远是自动提交，无法通过rollback回滚
```
mysql> show  variables like '%autocommit%';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| autocommit    | ON    |
+---------------+-------+
**1** row in set (**0.00** sec)
 
mysql> set session autocommit = **0**;
Query OK, **0** rows affected (**0.00** sec)
 
mysql> show  variables like '%autocommit%';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| autocommit    | OFF   |
+---------------+-------+
**1** row in set (**0.00** sec)
```
此时，在这个session中将DML将不会自动提交；
而如果在DML语句后我们进行了一句 DDL语句：
alter table ttt add new int(11);增加了一个字段；此时之前的DML也会自动提交，
因为DDL永远都会在语句后面自动COMMIT，而也顺带把之前的DML语句也提交了；
基本属性
事务的四个基本属性（ACID）酸
- 原子性（Atomicity）
- 一致性（Consistency）
- 隔离性（Isolation）
- 持久性（Durability）
事务的原子性（事务被打包一起执行或者一起撤销）
- 包含在事务中的操作要么全部被执行，要么都不执行
- 中途数据库或者应用发生异常，未提交的事务都应该被回滚
事务的一致性（各种约束条件，保证事务的正确合理，完成性）
数据正确性，合理性，完整性
数据一致性应该符合应用需要规则
-      余额不能为负数
-      交易对象必须先有账号
-      用户账号不能重复
事务的结果需要满足数据的一致性约束
事务的持久性(提交后的事务持久化到磁盘)
-     提交完成的事务会数据库的影响必须是永久性的
-      数据库异常不会丢失事务更新
-      通常认为成功写入磁盘的数据即为持久化成功
事务的持久化实现
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719223011294-1243444483.png)
机械硬盘对随机的读写非常慢，所以数据并不是同步刷新的。当update set a =3 ,commit ;
顺序写到事务日志中去，然后在空闲时，顺序的异步刷新到磁盘中去；这样速度回比较快，
因为机械硬盘对顺序读写操作的速度还是很快的。但如果数据库发现异常宕机了，
此时磁盘上的记录并没有得到修改，这时候需要通过回放事务日志的方式，也就是通过事务日志，重新将事务，更新到磁盘中去。
事务的隔离性
     数据库事务在提交完成前，中间的任何数据变化对其他的事务都是不可见的。
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719223021310-1205471309.png)
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719223027904-1432717069.png)
隔离现象
- 脏读：Dirty Read  
- 不可重复读：NonRepeatable Read
- 幻读：Phantom Read
隔离等级
- 未提交读：Read uncommitted
- 已提交读：Read committed
- 可重复读：Repeatable  Read
- 可串行化：Seriallizable
mysql的事务隔离级别
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719223043357-380180549.png)
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719223052138-1510514611.png)
tx_isolation 设置隔离级别；
数据库的隔离现象与隔离级别的演示
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719223105076-2134111793.png)
隔离现象
- 脏读：Dirty Read  
- 不可重复读：NonRepeatable Read
- 幻读：Phantom Read
隔离等级
- 未提交读：Read uncommitted
- 已提交读：Read committed
- 可重复读：Repeatable  Read
- 可串行化：Seriallizable
默认情况下：
查看隔离级别：
```
mysql> show global variables like "%iso%";
+---------------+-----------------+
| Variable_name | Value           |
+---------------+-----------------+
| tx_isolation  | REPEATABLE-READ |
+---------------+-----------------+
```
修改隔离级别：
```
set global  tx_isolation = 'READ-COMMITTED ';
```
READ UNCOMMITTED (未提交读) 的 dirty read（脏读）现象：
脏读现象：即A连接，未提交的事务，B连接的事务可以看到；
READ COMMITTED(已提交读)的NONREPEATABLE READ(不可重复读)现象：
A连接，提交的事务，B连接的事务中可以看到，这样在B连接中的事务，就可以看到A连接事务提交前，和提交后两种状态；PS：叫可重复读不是更贴切？
注意：不可重复读针对update，delete
READ COMMITTED(已提交读)的PHANTOM READ(幻读)现象：
A连接，提交的事务，B连接的事务可以看到，这样在B连接中的事务，就可以看到A连接事务提交前，和提交后两种状态；
注意：幻读针对insert；
REPEATABLE-READ(可重复读)的PHANTOM READ(幻读)现象：
但是在mysql中innodb事务引擎，将可重复读的幻读现象也消除掉了；
通过gap lock  间隙锁来 粗暴的实现
可重复读：即消除了不可重复读的现象；
事务与并发写
某个正在更新的记录在提交或回滚前不能被其他事务同时更新；
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719223126591-964153166.png)
引入锁的概念；在innodb中会具体讲解；
并发写是不允许的，而如果A事务写，B事务读是可以的。这个读受到数据库隔离级别的影响；
innodb 使用MVCC非锁定读技术，从undo log中读取行记录的快照；这样读行就不需要等待锁资源，提高了并发；
数据库回滚的实现
回滚段（rollback segment）与数据前像
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160719223134310-102989237.png)- 回滚段也帮助实现事务的隔离性
- InnoDB存储引擎异常恢复过程中需要用到回滚段..
总结
- savepoint：可以回滚到保存点，不可以提交保存点
- 清空表数据：truncate table tbl
- autocommit：DML可以设置不自动提交，DDL永远自动提交
- ACID
- 　　事务的原子性（事务被打包一起执行或者一起撤销）
- 　　事务的一致性（各种约束条件，保证事务的正确合理，完成性）
- 　　事务的持久性(提交后的事务持久化到磁盘)
- 　　事务的隔离性：
- - 　　隔离现象：脏读，不可重复读--update,delete，幻读--insert；
- 　　隔离等级：未提交读，已提交读，可重复读，可串行化
- innodb默认repeatable read
- innodb的repeatable read 消除了幻读现象通过gap lock 间隙锁粗暴实现；
- 非锁定读技术：MVCC，从undo log读取行记录快照
- 数据库回滚的实现：回滚段与数据前像；
