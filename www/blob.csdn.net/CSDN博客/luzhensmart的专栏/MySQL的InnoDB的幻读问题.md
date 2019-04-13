
# MySQL的InnoDB的幻读问题 - luzhensmart的专栏 - CSDN博客


2019年03月08日 16:21:39[芦震](https://me.csdn.net/luzhensmart)阅读数：9


# 快照读历史数据－mvcc
innodb的默认事务隔离级别是rr（可重复读）。**它的实现技术是mvcc。基于版本的控制协议。该技术不仅可以保证innodb的可重复读，而且可以防止****[幻读](https://www.baidu.com/s?wd=%E5%B9%BB%E8%AF%BB&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)****。（这也就是是此前以rr隔离级别实践时，不仅可以防止可重复读，也防止了****[幻读](https://www.baidu.com/s?wd=%E5%B9%BB%E8%AF%BB&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)****）但是它防止的是快照读，也就是读取的数据虽然是一致的，但是数据是历史数据。（http://blog.sina.com.cn/s/blog_499740cb0100ugs7.html这个帖子里面就有一个实例）**
一些文章写到InnoDB的可重复读避免了“幻读”（phantom read），这个说法并不准确。
做个试验：(以下所有试验要注意存储引擎和隔离级别)
```python
mysql> show create table t_bitfly\G;
CREATE TABLE `t_bitfly` (
`id` bigint(20) NOT NULL default '0',
`value` varchar(32) default NULL,
PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk
mysql> select @@global.tx_isolation, @@tx_isolation;
+-----------------------+-----------------+
| @@global.tx_isolation | @@tx_isolation  |
+-----------------------+-----------------+
| REPEATABLE-READ       | REPEATABLE-READ |
+-----------------------+-----------------+
```
### 试验一：
```python
t Session A                   Session B
|
| START TRANSACTION;          START TRANSACTION;
|
| SELECT * FROM t_bitfly;
| empty set
|                             INSERT INTO t_bitfly
|                             VALUES (1, 'a');
|
| SELECT * FROM t_bitfly;
| empty set
|                             COMMIT;
|
| SELECT * FROM t_bitfly;
| empty set
|
| INSERT INTO t_bitfly VALUES (1, 'a');
| ERROR 1062 (23000):
| Duplicate entry '1' for key 1
v (shit, 刚刚明明告诉我没有这条记录的)
```
**如此就出现了幻读，以为表里没有数据，其实数据已经存在了，傻乎乎的提交后，才发现数据冲突了。**
### 试验二：
```python
t Session A                  Session B
|
| START TRANSACTION;         START TRANSACTION;
|
| SELECT * FROM t_bitfly;
| +------+-------+
| | id   | value |
| +------+-------+
| |    1 | a     |
| +------+-------+
|                            INSERT INTO t_bitfly
|                            VALUES (2, 'b');
|
| SELECT * FROM t_bitfly;
| +------+-------+
| | id   | value |
| +------+-------+
| |    1 | a     |
| +------+-------+
|                            COMMIT;
|
| SELECT * FROM t_bitfly;
| +------+-------+
| | id   | value |
| +------+-------+
| |    1 | a     |
| +------+-------+
|
| UPDATE t_bitfly SET value='z';
| Rows matched: 2  Changed: 2  Warnings: 0
| (怎么多出来一行)
|
| SELECT * FROM t_bitfly;
| +------+-------+
| | id   | value |
| +------+-------+
| |    1 | z     |
| |    2 | z     |
| +------+-------+
|
v
```
**本事务中第一次读取出一行，做了一次更新后，另一个事务里提交的数据就出现了。也可以看做是一种幻读。**
------
那么，InnoDB指出的可以避免幻读是怎么回事呢？
> [http://dev.mysql.com/doc/refman/5.0/en/innodb-record-level-locks.html](http://dev.mysql.com/doc/refman/5.0/en/innodb-record-level-locks.html)

> By default, InnoDB operates in REPEATABLE READ transaction isolation level and with the innodb_locks_unsafe_for_binlog system variable disabled. In this case, InnoDB uses next-key locks for searches and index scans, which prevents phantom rows (see Section 13.6.8.5, “Avoiding the Phantom Problem Using Next-Key Locking”).
准备的理解是，当隔离级别是可重复读，且禁用innodb_locks_unsafe_for_binlog的情况下，在搜索和扫描index的时候使用的next-key locks可以避免幻读。
关键点在于，是InnoDB默认对一个普通的查询也会加next-key locks，还是说需要应用自己来加锁呢？如果单看这一句，可能会以为InnoDB对普通的查询也加了锁，如果是，那和序列化（SERIALIZABLE）的区别又在哪里呢？
MySQL manual里还有一段：
> 13.2.8.5. Avoiding the Phantom Problem Using Next-Key Locking (
> [http://dev.mysql.com/doc/refman/5.0/en/innodb-next-key-locking.html](http://dev.mysql.com/doc/refman/5.0/en/innodb-next-key-locking.html)
> )

> To prevent phantoms,
> InnoDB
> uses an algorithm called
> next-key locking
> that combines index-row locking with gap locking.

> You can use next-key locking to implement a uniqueness check in your application: If you read your data in share mode and do not see a duplicate for a row you are going to insert, then you can safely insert your row and know that the next-key lock set on the successor of your row during the read prevents anyone meanwhile inserting a duplicate for your row. Thus, the next-key locking enables you to “lock” the nonexistence of something in your table.
我的理解是说，InnoDB提供了next-key locks，但需要应用程序自己去加锁。manual里提供一个例子：
```python
SELECT * FROM child WHERE id > 100 FOR UPDATE;
```
这样，InnoDB会给id大于100的行（假如child表里有一行id为102），以及100-102，102+的gap都加上锁。
可以使用show innodb status来查看是否给表加上了锁。
### 再看一个实验，要注意，表t_bitfly里的id为主键字段。实验三：
```python
t Session A                 Session B
|
| START TRANSACTION;        START TRANSACTION;
|
| SELECT * FROM t_bitfly
| WHERE id&lt;=1
| FOR UPDATE;
| +------+-------+
| | id   | value |
| +------+-------+
| |    1 | a     |
| +------+-------+
|                           INSERT INTO t_bitfly
|                           VALUES (2, 'b');
|                           Query OK, 1 row affected
|
| SELECT * FROM t_bitfly;
| +------+-------+
| | id   | value |
| +------+-------+
| |    1 | a     |
| +------+-------+
|                           INSERT INTO t_bitfly
|                           VALUES (0, '0');
|                           (waiting for lock ...
|                           then timeout)
|                           ERROR 1205 (HY000):
|                           Lock wait timeout exceeded;
|                           try restarting transaction
|
| SELECT * FROM t_bitfly;
| +------+-------+
| | id   | value |
| +------+-------+
| |    1 | a     |
| +------+-------+
|                           COMMIT;
|
| SELECT * FROM t_bitfly;
| +------+-------+
| | id   | value |
| +------+-------+
| |    1 | a     |
| +------+-------+
v
```
**可以看到，用id<=1加的锁，只锁住了id<=1的范围，可以成功添加id为2的记录，添加id为0的记录时就会等待锁的释放。**
MySQL manual里对可重复读里的锁的详细解释：
> [http://dev.mysql.com/doc/refman/5.0/en/set-transaction.html\#isolevel_repeatable-read](http://dev.mysql.com/doc/refman/5.0/en/set-transaction.html#isolevel_repeatable-read)

> For locking reads (
> SELECT
> with
> FOR UPDATE
> or
> LOCK IN SHARE MODE
> ),
> UPDATE
> , and
> DELETE
> statements, locking depends on whether the statement uses a unique index with a unique search condition, or a range-type search condition. For a unique index with a unique search condition,
> InnoDB
> locks only the index record found, not the gap before it. For other search conditions,
> InnoDB
> locks the index range scanned, using gap locks or next-key (gap plus index-record) locks to block insertions by other sessions into the gaps covered by the range.
------
### 一致性读和提交读，先看实验，实验四：

```python
t Session A                      Session B
|
| START TRANSACTION;             START TRANSACTION;
|
| SELECT * FROM t_bitfly;
| +----+-------+
| | id | value |
| +----+-------+
| |  1 | a     |
| +----+-------+
|                                INSERT INTO t_bitfly
|                                VALUES (2, 'b');
|                                COMMIT;
|
| SELECT * FROM t_bitfly;
| +----+-------+
| | id | value |
| +----+-------+
| |  1 | a     |
| +----+-------+
|
| SELECT * FROM t_bitfly LOCK IN SHARE MODE;
| +----+-------+
| | id | value |
| +----+-------+
| |  1 | a     |
| |  2 | b     |
| +----+-------+
|
| SELECT * FROM t_bitfly FOR UPDATE;
| +----+-------+
| | id | value |
| +----+-------+
| |  1 | a     |
| |  2 | b     |
| +----+-------+
|
| SELECT * FROM t_bitfly;
| +----+-------+
| | id | value |
| +----+-------+
| |  1 | a     |
| +----+-------+
v
```
如果使用普通的读，会得到一致性的结果，如果使用了加锁的读，就会读到“最新的”“提交”读的结果。
本身，可重复读和提交读是矛盾的。在同一个事务里，如果保证了可重复读，就会看不到其他事务的提交，违背了提交读；如果保证了提交读，就会导致前后两次读到的结果不一致，违背了可重复读。
可以这么讲，InnoDB提供了这样的机制，在默认的可重复读的隔离级别里，可以使用加锁读去查询最新的数据。
> [http://dev.mysql.com/doc/refman/5.0/en/innodb-consistent-read.html](http://dev.mysql.com/doc/refman/5.0/en/innodb-consistent-read.html)

> If you want to see the “freshest” state of the database, you should use either the READ COMMITTED isolation level or a locking read:

> SELECT * FROM t_bitfly LOCK IN SHARE MODE;
------
结论：MySQL InnoDB的可重复读并不保证避免幻读，需要应用使用加锁读来保证。而这个加锁度使用到的机制就是next-key locks。

# 当前读最新数据－next key
如何做到保证数据是一致的（可重复、幻）（也就是一个事务，其内部读取对应某一个数据的时候，数据都是一样的），同时读取的数据是最新的数据。innodb提供了一个间隙锁的技术，**即next－key，也就是结合grap锁与行锁，**达到最终目的。当使用索引进行插入（或select，update等）的时候，innodb会将当前的节点（record_lock）和上一个节点（gap lock）加锁？。这样当进行select的时候，就不允许加x锁。那么在进行该事务的时候，读取的就是最新的数据。（同样的，反过来看，当select in share mode时加next－key锁，视查询索引是否唯一而定一片数据的insert和update阻塞），当两个当前读在一个事务中，别的update insert delete就无法打断，也就不会出现重复读和幻读。
实现：
1. 快照读(snapshot read)
> 简单的select操作(不包括 select ... lock in share mode, select ... for update)
2.当前读(current read)
> select ... lock in share mode

> select ... for update

> insert

> update

> delete
**在RR级别下，快照读是通过MVVC(多版本控制)和undo log来实现的，当前读是通过加record lock(记录锁)和gap lock(间隙锁)来实现的。（u  无锁读   rc mvcc rr mvcc s 自动升级为当前读）**
所以从上面的显示来看，**如果需要实时显示数据，还是需要通过手动加锁来实现。这个时候会使用next-key技术来实现**。
总结：在mysql中，提供了两种事务隔离技术，**第一个是mvcc，第二个是next-key技术。**这个在使用不同的语句的时候可以动态选择。**不加lock inshare mode之类的快照读就使用mvcc。否则 当前读使用next-key。**mvcc的优势是不加锁，并发性高。缺点是不是实时数据。next-key的优势是获取实时数据，但是需要加锁。
**另外，重要：**
**在rr级别下，mvcc完全解决了重复读，但并不能真正的完全避免幻读，只是在部分场景下利用历史数据规避了幻读**
**对于快照读，mysql使用mvcc利用历史数据部分避免了幻读（在某些场景看上去规避了幻读）**
**要完全避免，需要手动加锁将快照读调整为当前读（mysql不会自动加锁），然后mysql使用next-key完全避免了幻读，比如rr下，锁1（0，2，3，4），另一个线程的insert 3即被阻塞****，在rc下，另一个线程仍然可以****[大摇大摆](https://www.baidu.com/s?wd=%E5%A4%A7%E6%91%87%E5%A4%A7%E6%91%86&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)****的插入，如本线程再次查询比如count，则会不一致**
参考：https://www.cnblogs.com/cat-and-water/p/6429268.html  MySQL的InnoDB的幻读问题
同时需要注意几点：
1.**事务的快照时间点是以第一个select来确认的**。所以即便事务先开始。但是select在后面的事务的update之类的语句后进行，那么它是可以获取后面的事务的对应的数据。
2.mysql[中数据](https://www.baidu.com/s?wd=%E4%B8%AD%E6%95%B0%E6%8D%AE&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)的存放还是会通过版本记录一系列的历史数据，这样，可以根据版本查找数据。

