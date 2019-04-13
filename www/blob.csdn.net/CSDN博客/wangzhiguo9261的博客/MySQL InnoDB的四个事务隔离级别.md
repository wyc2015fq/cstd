
# MySQL InnoDB的四个事务隔离级别 - wangzhiguo9261的博客 - CSDN博客


2018年07月11日 13:59:45[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：254个人分类：[MySQL](https://blog.csdn.net/wangzhiguo9261/article/category/7454395)


望通过本文，可以加深读者对MySQL InnoDB的四个事务隔离级别，以及脏读、不重复读、幻读的理解。

MySQL InnoDB事务的隔离级别有四级，默认是“可重复读”（REPEATABLE READ）。
·未提交读（READUNCOMMITTED）。另一个事务修改了数据，但尚未提交，而本事务中的SELECT会读到这些未被提交的数据（脏读）。
·提交读（READCOMMITTED）。本事务读取到的是最新的数据（其他事务提交后的）。问题是，在同一个事务里，前后两次相同的SELECT会读到不同的结果（不重复读）。
·可重复读（REPEATABLEREAD）。在同一个事务里，SELECT的结果是事务开始时时间点的状态，因此，同样的SELECT操作读到的结果会是一致的。但是，会有幻读现象（稍后解释）。
·串行化（SERIALIZABLE）。读操作会隐式获取共享锁，可以保证不同事务间的互斥。
四个级别逐渐增强，每个级别解决一个问题。
·脏读，最容易理解。另一个事务修改了数据，但尚未提交，而本事务中的SELECT会读到这些未被提交的数据。
·不重复读。解决了脏读后，会遇到，同一个事务执行过程中，另外一个事务提交了新数据，因此本事务先后两次读到的数据结果会不一致。
·幻读。解决了不重复读，保证了同一个事务里，查询的结果都是事务开始时的状态（一致性）。但是，如果另一个事务同时提交了新数据，本事务再更新时，就会“惊奇的”发现了这些新数据，貌似之前读到的数据是“鬼影”一样的幻觉。


MySQL InnoDB事务隔离级别可设置为global和session级别。

事务隔离级别查看

查看当前session的事务隔离级别：
**[sql]**[view plain](http://blog.csdn.net/seven_3306/article/details/27085275#)[copy](http://blog.csdn.net/seven_3306/article/details/27085275#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
mysql> show variableslike'%tx_isolation%';
+---------------+--------------+
| Variable_name | Value        |
+---------------+--------------+
| tx_isolation  |SERIALIZABLE|
+---------------+--------------+
设置事务隔离级别：
设置global事务隔离级别：
set global isolation level read committed;
注意一点的设置global并不会对当前session生效。
设置session事务隔离级别sql脚本：
**[sql]**[view plain](http://blog.csdn.net/seven_3306/article/details/27085275#)[copy](http://blog.csdn.net/seven_3306/article/details/27085275#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
setsessiontransactionisolationlevelreaduncommitted;
setsessiontransactionisolationlevelreadcommitted;
setsessiontransactionisolationlevelREPEATABLEREAD;
setsessiontransactionisolationlevelSERIALIZABLE;


上面的文字，读起来并不是那么容易让人理解，以下用几个实验对InnoDB的四个事务隔离级别做详细的解释，希望通过实验来加深大家对InnoDB的事务隔离级别理解。

**[sql]**[view plain](http://blog.csdn.net/seven_3306/article/details/27085275#)[copy](http://blog.csdn.net/seven_3306/article/details/27085275#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
CREATETABLE`t` (
`a`INT(11)NOTNULLPRIMARYKEY
) ENGINE = INNODBDEFAULTCHARSET = UTF8;
INSERTINTOt (a)VALUES(1),(2),(3);


实验一：解释脏读、可重复读问题
|更新事务
|事务A READ-UNCOMMITTED
|事务B READ-COMMITTED,
|事务C-1 REPEATABLE-READ
|事务C-2 REPEATABLE-READ
|事务D SERIALIZABLE
|
|set autocommit =0;


|
|start transaction ;

|start transaction;
|
|insert into t(a)values(4);


|

|select * from t;
|1,2,3,4(|脏读：读取到了未提交的事务中的数据|)
|select * from t;
|1,2,3（解决脏读）
|select * from t;
|1,2,3
|select * from t;
|1,2,3
|select * from t;
|1,2,3
|



|



|
|commit;


|
|select * from t:
|1,2,3,4
|select * from t:
|1,2,3,4
|select * from t:
|1,2,3,4 (与上面的不在一个事务中，所以读到为事务提交后最新的，所以可读到4)
|select * from t:
|1,2,3|（重复读：由于与上面的在一个事务中，所以只读到事务开始事务的数据，也就是重复读）
|select * from t:
|1,2,3,4
|


|commit|（提交事务，下面的就是一个新的事务，所以可以读到事务提交以后的最新数据）
|


|select * from t:
|1,2,3,4
|
|READ-UNCOMMITTED 会产生脏读，基本很少适用于实际场景，所以基本不使用。
|

实验二：测试READ-COMMITTED与REPEATABLE-READ
|事务A
|事务B READ-COMMITTED
|事务C REPEATABLE-READ
|
|set autocommit =0;

|
|start transaction ;
|start transaction;
|start transaction;
|
|insert into t(a)values(4);

|

|select * from t;
|1,2,3
|select * from t;
|1,2,3
|

|

|
|commit;

|
|select * from t:
|1,2,3,4
|select * from t:
|1,2,3|（重复读：由于与上面的在一个事务中，所以只读到事务开始事务的数据，也就是重复读）
|

|commit|（提交事务，下面的就是一个新的事务，所以可以读到事务提交以后的最新数据）
|

|select * from t:
|1,2,3,4
|
|REPEATABLE-READ可以确保一个事务中读取的数据是可重复的，也就是相同的读取（第一次读取以后，即使其他事务已经提交新的数据，同一个事务中再次select也并不会被读取）。
|READ-COMMITTED只是确保读取最新事务已经提交的数据。
|
当然数据的可见性都是对不同事务来说的，同一个事务，都是可以读到此事务中最新数据的。
**[sql]**[view plain](http://blog.csdn.net/seven_3306/article/details/27085275#)[copy](http://blog.csdn.net/seven_3306/article/details/27085275#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
starttransaction;
insertintot(a)values(4);
select*fromt;
1,2,3,4;
insertintot(a)values(5);
select*fromt;
1,2,3,4,5;


实验三：测试SERIALIZABLE事务对其他的影响
|事务A SERIALIZABLE
|事务B READ-UNCOMMITTED
|事务C READ-COMMITTED,
|事务D REPEATABLE-READ
|事务E SERIALIZABLE
|
|set autocommit =0;


|
|start transaction ;

|start transaction;
|
|select a from t union all select sleep(1000) from dual;


|

|insert into t(a)values(5);
|insert into t(a)values(5);
|insert into t(a)values(5);
|insert into t(a)values(5);
|
|ERROR 1205 (HY000): Lock wait timeout exceeded; try restarting transaction
|ERROR 1205 (HY000): Lock wait timeout exceeded; try restarting transaction
|ERROR 1205 (HY000): Lock wait timeout exceeded; try restarting transaction
|ERROR 1205 (HY000): Lock wait timeout exceeded; try restarting transaction
|
|SERIALIZABLE 串行化执行，导致所有其他事务不得不等待事务A结束才行可以执行，这里特意使用了sleep函数，直接导致事务B,C,D,E等待事务A持有释放的锁。由于我sleep了1000秒，而innodb_lock_wait_timeout为120s。所以120s到了就报错HY000错误。
|
|SERIALIZABLE是相当严格的串行化执行模式，不管是读还是写，都会影响其他读取相同的表的事务。是严格的表级读写排他锁。也就失去了innodb引擎的优点。实际应用很少。
|

实验四：幻读
一些文章写到InnoDB的可重复读避免了“幻读”（phantom read），这个说法并不准确。
做个试验：(以下所有试验要注意存储引擎和隔离级别)
**[sql]**[view plain](http://blog.csdn.net/seven_3306/article/details/27085275#)[copy](http://blog.csdn.net/seven_3306/article/details/27085275#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
mysql>showcreatetablet_bitfly\G;
CREATETABLE`t_bitfly` (
`id`bigint(20)NOTNULLdefault'0',
`value`varchar(32)defaultNULL,
PRIMARYKEY(`id`)
) ENGINE=InnoDBDEFAULTCHARSET=gbk
mysql>select@@global.tx_isolation, @@tx_isolation;
+-----------------------+-----------------+
| @@global.tx_isolation | @@tx_isolation  |
+-----------------------+-----------------+
|REPEATABLE-READ|REPEATABLE-READ|
+-----------------------+-----------------+

试验4-1：


**[sql]**[view plain](http://blog.csdn.net/seven_3306/article/details/27085275#)[copy](http://blog.csdn.net/seven_3306/article/details/27085275#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
SessionA                                                                            Session B
STARTTRANSACTION;                                                                  STARTTRANSACTION;
SELECT*FROMt_bitfly;
emptyset
INSERTINTOt_bitflyVALUES(1,'a');

SELECT*FROMt_bitfly;
emptyset
COMMIT;
SELECT*FROMt_bitfly;
emptyset
INSERTINTOt_bitflyVALUES(1,'a');
ERROR 1062 (23000):
Duplicate entry'1'forkey1


v (shit,刚刚明明告诉我没有这条记录的)
如此就出现了幻读，以为表里没有数据，其实数据已经存在了，傻乎乎的提交后，才发现数据冲突了。
试验4-2：

**[sql]**[view plain](http://blog.csdn.net/seven_3306/article/details/27085275#)[copy](http://blog.csdn.net/seven_3306/article/details/27085275#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
SessionA                                                                         Session B
STARTTRANSACTION;                                                               STARTTRANSACTION;
SELECT*FROMt_bitfly;
+------+-------+
| id   | value |
+------+-------+
|    1 |a     |
+------+-------+
INSERTINTOt_bitflyVALUES(2,'b');

SELECT*FROMt_bitfly;
+------+-------+
| id   | value |
+------+-------+
|    1 |a     |
+------+-------+
COMMIT;
SELECT*FROMt_bitfly;
+------+-------+
| id   | value |
+------+-------+
|    1 |a     |
+------+-------+
UPDATEt_bitflySETvalue='z';
Rowsmatched: 2  Changed:2  Warnings: 0
(怎么多出来一行)
SELECT*FROMt_bitfly;
+------+-------+
| id   | value |
+------+-------+
|    1 |z     |
|    2 |z     |
+------+-------+


本事务中第一次读取出一行，做了一次更新后，另一个事务里提交的数据就出现了。也可以看做是一种幻读。
------
那么，InnoDB指出的可以避免幻读是怎么回事呢？
http://dev.mysql.com/doc/refman/5.0/en/innodb-record-level-locks.html
By default, InnoDB operatesin REPEATABLE READ transaction isolation level and with the innodb_locks_unsafe_for_binlogsystem variable disabled. In this case, InnoDB uses next-key locks for searchesand index scans, which prevents phantom rows (see Section 13.6.8.5, “Avoidingthe Phantom Problem Using Next-Key Locking”).
准备的理解是，当隔离级别是可重复读，且禁用innodb_locks_unsafe_for_binlog的情况下，在搜索和扫描index的时候使用的next-keylocks可以避免幻读。
关键点在于，是InnoDB默认对一个普通的查询也会加next-key locks，还是说需要应用自己来加锁呢？如果单看这一句，可能会以为InnoDB对普通的查询也加了锁，如果是，那和序列化（SERIALIZABLE）的区别又在哪里呢？
MySQL manual里还有一段：
13.2.8.5. Avoiding the PhantomProblem Using Next-Key Locking (http://dev.mysql.com/doc/refman/5.0/en/innodb-next-key-locking.html)
Toprevent phantoms,`InnoDB`usesan algorithm called*next-key locking*that combinesindex-row locking with gap locking.
Youcan use next-key locking to implement a uniqueness check in your application:If you read your data in share mode and do not see a duplicate for a row youare going to insert, then you can safely insert your row and know that thenext-key lock set on the successor of your row during the read prevents anyonemeanwhile inserting a duplicate for your row. Thus, the next-key lockingenables you to “lock” the nonexistence of something in your table.
我的理解是说，InnoDB提供了next-key locks，但需要应用程序自己去加锁。manual里提供一个例子：
SELECT * FROM child WHERE id> 100 FOR UPDATE;
这样，InnoDB会给id大于100的行（假如child表里有一行id为102），以及100-102，102+的gap都加上锁。
可以使用showinnodb status来查看是否给表加上了锁。
再看一个实验，要注意，表t_bitfly里的id为主键字段。
实验4-3：

**[sql]**[view plain](http://blog.csdn.net/seven_3306/article/details/27085275#)[copy](http://blog.csdn.net/seven_3306/article/details/27085275#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
Session A                               Session B
STARTTRANSACTION;                      STARTTRANSACTION;
SELECT*FROMt_bitfly
WHEREid<=1
FORUPDATE;
+------+-------+
| id   | value |
+------+-------+
|    1 | a     |
+------+-------+
INSERTINTOt_bitfly
VALUES(2,'b');
Query OK, 1 row affected
SELECT*FROMt_bitfly;
+------+-------+
| id   | value |
+------+-------+
|    1 | a     |
+------+-------+
INSERTINTOt_bitfly
VALUES(0,'0');
(waitingforlock ...
thentimeout)
ERROR 1205 (HY000):
Lock wait timeout exceeded;
try restartingtransaction
SELECT*FROMt_bitfly;
+------+-------+
| id   | value |
+------+-------+
|    1 | a     |
+------+-------+
COMMIT;
SELECT*FROMt_bitfly;
+------+-------+
| id   | value |
+------+-------+
|    1 | a     |
+------+-------+




可以看到，用id<=1加的锁，只锁住了id<=1的范围，可以成功添加id为2的记录，添加id为0的记录时就会等待锁的释放。
MySQL manual里对可重复读里的锁的详细解释：
http://dev.mysql.com/doc/refman/5.0/en/set-transaction.html\#isolevel_repeatable-read
Forlocking reads (`SELECT`with`FORUPDATE`or`LOCK IN SHARE MODE`),`UPDATE`, and`DELETE`statements, lockingdepends on whether the statement uses a unique index with a unique searchcondition, or a range-type search condition. For a unique index with a uniquesearch condition,`InnoDB`locksonly the index record found, not the gap before it. For other searchconditions,`InnoDB`locksthe index range scanned, using gap locks or next-key (gap plus index-record)locks to block insertions by other sessions into the gaps covered by the range.
------
一致性读和提交读，先看实验，
实验4-4：
**[sql]**[view plain](http://blog.csdn.net/seven_3306/article/details/27085275#)[copy](http://blog.csdn.net/seven_3306/article/details/27085275#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
SessionA                                                                  Session B
STARTTRANSACTION;                                                         STARTTRANSACTION;
SELECT*FROMt_bitfly;
+----+-------+
| id | value |
+----+-------+
|  1 |a     |
+----+-------+
INSERTINTOt_bitflyVALUES(2,'b');
COMMIT;
SELECT*FROMt_bitfly;
+----+-------+
| id | value |
+----+-------+
|  1 |a     |
+----+-------+
SELECT*FROMt_bitfly LOCKINSHARE MODE;
+----+-------+
| id | value |
+----+-------+
|  1 |a     |
|  2 |b     |
+----+-------+
SELECT*FROMt_bitflyFORUPDATE;
+----+-------+
| id | value |
+----+-------+
|  1 |a     |
|  2 |b     |
+----+-------+
SELECT*FROMt_bitfly;
+----+-------+
| id | value |
+----+-------+
|  1 |a     |
+----+-------+



如果使用普通的读，会得到一致性的结果，如果使用了加锁的读，就会读到“最新的”“提交”读的结果。
本身，可重复读和提交读是矛盾的。在同一个事务里，如果保证了可重复读，就会看不到其他事务的提交，违背了提交读；如果保证了提交读，就会导致前后两次读到的结果不一致，违背了可重复读。
可以这么讲，InnoDB提供了这样的机制，在默认的可重复读的隔离级别里，可以使用加锁读去查询最新的数据。
http://dev.mysql.com/doc/refman/5.0/en/innodb-consistent-read.html
Ifyou want to see the “freshest” state of the database, you should use either theREAD COMMITTED isolation level or a locking read:
SELECT * FROM t_bitfly LOCK IN SHARE MODE;
------
结论：MySQLInnoDB的可重复读并不保证避免幻读，需要应用使用加锁读来保证。而这个加锁度使用到的机制就是next-keylocks。
====================结尾====================

文章幻读部分直接转载了bitfly的文章：http://blog.bitfly.cn/post/mysql-innodb-phantom-read/

转载自[utf7](http://my.csdn.net/chenyechao)的文章：http://blog.csdn.net/seven_3306/article/details/27085275

转载请说明出处，包括参考文章出处。

