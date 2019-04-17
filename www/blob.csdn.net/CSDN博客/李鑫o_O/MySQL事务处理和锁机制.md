# MySQL事务处理和锁机制 - 李鑫o_O - CSDN博客





2016年05月16日 16:33:12[hustlx](https://me.csdn.net/HUSTLX)阅读数：1679










# MySQL事务处理和锁机制



1.隔离级别

(1)读不提交(ReadUncommited，RU)

这种隔离级别下，事务间完全不隔离，会产生脏读，可以读取未提交的记录，实际情况下不会使用。

(2)读提交(Readcommited，RC)



仅能读取到已提交的记录，这种隔离级别下，会存在幻读现象，所谓幻读是指在同一个事务中，多次执行同一个查询，返回的记录不完全相同的现象。幻读产生的根本原因是，在RC隔离级别下，每条语句都会读取已提交事务的更新，若两次查询之间有其他事务提交，则会导致两次查询结果不一致。虽然如此，读提交隔离级别在生产环境中使用很广泛。

(3)可重复读(RepeatableRead, RR)

可重复读解决了幻读问题。不是所有的数据库都实现了该隔离级别，后面会简单介绍下mysql是如何实现可重复读隔离级别的。

(4)串行化(Serializable)

在串行化隔离模式下，消除了脏读，幻象，但事务并发度急剧下降，事务的隔离级别与事务的并发度成反比，隔离级别越高，事务的并发度越低。实际生产环境下，dba会在并发和满足业务需求之间作权衡，选择合适的隔离级别。

2.并发调度方式

与隔离级别紧密联系的另外一个东西是并发调度，通过并发调度实现隔离级别。对于并发调度，不同的数据库厂商有不同的实现机制，但基本原理类似，都是通过加锁来保护数据对象不同时被多个事务修改。多版本的并发控制(MVCC)相对于传统的基于锁的并发控制主要特点是读不上锁，这种特性对于读多写少的场景，大大提高了系统的并发度，因此大部分关系型数据库都实现了MVCC。

3.两阶段锁协议

两阶段锁协议的含义是，事务分为两个阶段，第一个阶段是获得封锁，第二个阶段是释放封锁。两阶段封锁保证并发调度的正确性。两阶段封锁相对于一阶段封锁（一次性获得事务需要的所有锁），提高了并发度，但同时也带来了死锁的可能。

4.死锁

所谓死锁是指两个或多个事务，各自占有对方的期望获得的资源，形成的循环等待，彼此无法继续执行的一种状态。

5.锁类型

根据锁的类型分，可以分为共享锁，排他锁，意向共享锁和意向排他锁。根据锁的粒度分，又可以分为行锁，表锁。对于mysql而言，事务机制更多是靠底层的存储引擎来实现，因此，mysql层面只有表锁，而支持事务的innodb存储引擎则实现了行锁(记录锁)，gap锁，next-key锁。Mysql的记录锁实质是索引记录的锁，因为innodb是索引组织表；gap锁是索引记录间隙的锁，这种锁只在RR隔离级别下有效；next-key锁是记录锁加上记录之前gap锁的组合。mysql通过gap锁和next-key锁实现RR隔离级别。

说明：

1.  对于更新操作(读不上锁)，只有走索引才可能上行锁；否则会对聚簇索引的每一行上写锁，实际等同于对表上写锁。

2.  若多个物理记录对应同一个索引，若同时访问，也会出现锁冲突；

3.  当表有多个索引时，不同事务可以用不同的索引锁住不同的行，另外innodb会同时用行锁对数据记录(聚簇索引)加锁。

4.  MVCC并发控制机制下，任何操作都不会阻塞读操作，读操作也不会阻塞任何操作，只因为读不上锁。


|[6. 锁互斥与兼容关系矩阵](http://shop111432812.taobao.com/)|[](http://shop111432812.taobao.com/)|[X (排他锁)](http://shop111432812.taobao.com/)|[S (共享锁)](http://shop111432812.taobao.com/)|[IX (意向排他锁)](http://shop111432812.taobao.com/)|[IS(意向共享锁)](http://shop111432812.taobao.com/)||----|----|----|----|----||[X](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)||[S](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)||[IX](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)||[IS](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[(y表示兼容，n表示不兼容):](http://shop111432812.taobao.com/)|[](http://shop111432812.taobao.com/)|[X (排他锁)](http://shop111432812.taobao.com/)|[S (共享锁)](http://shop111432812.taobao.com/)|[IX (意向排他锁)](http://shop111432812.taobao.com/)|[IS(意向共享锁)](http://shop111432812.taobao.com/)|[X](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[S](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[IX](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[IS](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|[](http://shop111432812.taobao.com/)|[X (排他锁)](http://shop111432812.taobao.com/)|[S (共享锁)](http://shop111432812.taobao.com/)|[IX (意向排他锁)](http://shop111432812.taobao.com/)|[IS(意向共享锁)](http://shop111432812.taobao.com/)| | | | | | | | | | | | | | | | | | | | | |
|[X](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)| | | | | | | | | | | | | | | | | | | | | |
|[S](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)| | | | | | | | | | | | | | | | | | | | | |
|[IX](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)| | | | | | | | | | | | | | | | | | | | | |
|[IS](http://shop111432812.taobao.com/)|[n](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)|[y](http://shop111432812.taobao.com/)| | | | | | | | | | | | | | | | | | | | | |

[(y表示兼容，n表示不兼容):](http://shop111432812.taobao.com/)

7.mysql锁相关操作

(1).查看最近一次死锁信息

show innodbengine status;

查看LATEST DETECTED DEADLOCK这一栏的相关信息。

(2)查看正在使用的锁

SELECT r.trx_idwaiting_trx_id, 

r.trx_querywaiting_query, 

b.trx_idblocking_trx_id, 

b.trx_queryblocking_query, 

b.trx_mysql_thread_idblocking_thread, 

b.trx_started,

b.trx_wait_started

FROMinformation_schema.innodb_lock_waits w 

INNER JOINinformation_schema.innodb_trx b 

ON b.trx_id= w.blocking_trx_id 

INNER JOINinformation_schema.innodb_trx r 

ON r.trx_id= w.requesting_trx_id

(3)查看事务的隔离级别

showvariables like 'tx_isolation';

8.典型SQL语句锁分析

(1).select......lock in share mode 获得共享锁。【对于表上意向共享锁；对于读取的每一个行，上行级共享锁】

(2).select...... for update 获得排他锁【对于表上意向排他锁；对于读取的每一个行，会上行级排他锁】

(3).insertinto target_tab select * from source_tab where ...

(4).createtable new_tab as select ... From source_tab where ...

(3)和(4)在RR隔离级别下，会对source_tab上锁，防止出现幻读；RC隔离级别下，不上锁。

(5).FLUSHTABLES WITH READ LOCK

全局读锁定，锁定数据库中的所有库中的所有表，mysqldump会用到这个命令。

9、死锁及处理方式

所谓死锁<DeadLock>: 是指两个或两个以上的进程在执行过程中,

因争夺资源而造成的一种互相等待的现象,若无外力作用,它们都将无法推进下去.

此时称系统处于死锁状态或系统产生了死锁,这些永远在互相等竺的进程称为死锁进程.

表级锁不会产生死锁.所以解决死锁主要还是真对于最常用的InnoDB.

遇到死锁的处理方式

mysql -uxxx-pxxx -h服务器ip --port=服务器端口;（如果服务器设置了ip和端口访问的话，一定要带ip和端口）

mysql>show processlist; #查看正在执行的sql （show full processlist;查看全部sql）

mysql>kill id #杀死sql进程；

如果进程太多找不到，就重启mysql吧

/ect/init.d/mysqlrestart 

或/ect/init.d/mysqlstop（如果关不掉就直接kill -9 进程id）  再/ect/init.d/mysqlstart 

去看看mysql日志文件是否保存死锁日志：

常用目录：/var/log/mysqld.log；（该目录还有其它相关日志文件就都看看）

怎么解决还是要看具体什么问题.





1.      事务处理和并发性

1.1.        基础知识和相关概念

1 ）全部的表类型都可以使用锁，但是只有 InnoDB 和 BDB 才有内置的事务功能。

2 ）使用 begin 开始事务，使用 commit 结束事务，中间可以使用 rollback 回滚事务。

3 ）在默认情况下， InnoDB 表支持一致读。

SQL 标准中定义了 4 个隔离级别： read uncommited ， read commited ， repeatable read ， serializable 。

readuncommited 即脏读，一个事务修改了一行，另一个事务也可以读到该行。

如果第一个事务执行了回滚，那么第二个事务读取的就是从来没有正式出现过的值。 ?

readcommited 即一致读，试图通过只读取提交的值的方式来解决脏读的问题，但是这又引起了不可重复读取的问题。

一个事务执行一个查询，读取了大量的数据行。在它结束读取之前，另一个事务可能完成了对数据行的更改。当第一个事务试图再次执行同一个查询，服务器就会返回不同的结果。

repeatableread 即可重复读，在一个事务对数据行执行读取或写入操作时锁定了这些数据行。

但是这种方式又引发了幻想读的问题。

因为只能锁定读取或写入的行，不能阻止另一个事务插入数据，后期执行同样的查询会产生更多的结果。

serializable模式中，事务被强制为依次执行。这是 SQL 标准建议的默认行为。

4 ）如果多个事务更新了同一行，就可以通过回滚其中一个事务来解除死锁。

5 ） MySQL 允许利用 set transaction 来设置隔离级别。

6 ）事务只用于 insert 和 update 语句来更新数据表，不能用于对表结构的更改。执行一条更改表结构或begin 则会立即提交当前的事务。

7 ）所有表类型都支持表级锁，但是 MyISAM 只支持表级锁。

8 ）有两种类型的表级锁：读锁和写锁。

读锁是共享锁，支持并发读，写操作被锁。

写锁是独占锁，上锁期间其他线程不能读表或写表。

8 ）如果要支持并发读写，建议采用 InnoDB 表，因为它是采用行级锁，可以获得更多的更新性能。

9 ）很多时候，可以通过经验来评估什么样的锁对应用程序更合适，不过通常很难说一个锁比别的更好，这全都要依据应用程序来决定，不同的地方可能需要不同的锁。当前 MySQL 已经支持 ISAM, MyISAM, MEMORY (HEAP) 类型表的表级锁了， BDB 表支持页级锁， InnoDB 表支持行级锁。

10 ） MySQL 的表级锁都是写锁优先，而且是采用排队机制，这样不会出现死锁的情况。对于 InnoDB 和 BDB 存储引擎来说，是可能产生死锁的。这是因为 InnoDB 会自动捕获行锁， BDB 会在执行 SQL 语句时捕获页锁的，而不是在事务的开始就这么做。

1.2.        不同锁的优缺点及选择

行级锁的优点及选择 ：

1 ）在很多线程请求不同记录时减少冲突锁。

2 ）事务回滚时减少改变数据。

3 ）使长时间对单独的一行记录加锁成为可能。

行级锁的缺点 ：

1 ）比页级锁和表级锁消耗更多的内存。

2 ）当在大量表中使用时，比页级锁和表级锁更慢，因为他需要请求更多的所资源。

3 ）当需要频繁对大部分数据做 GROUP BY 操作或者需要频繁扫描整个表时，就明显的比其它锁更糟糕。

4 ）使用更高层的锁的话，就能更方便的支持各种不同的类型应用程序，因为这种锁的开销比行级锁小多了。

5 ）可以用应用程序级锁来代替行级锁，例如 MySQL 中的 GET_LOCK() 和 RELEASE_LOCK() 。但它们是劝告锁（原文： These are advisory locks ），因此只能用于安全可信的应用程序中。

6 ）对于 InnoDB 和 BDB 表， MySQL 只有在指定用LOCK TABLES 锁表时才使用表级锁。在这两种表中，建议最好不要使用 LOCK TABLES ，因为 InnoDB 自动采用行级锁， BDB 用页级锁来保证事务的隔离。

表锁的优点及选择：

1 ）很多操作都是读表。

2 ）在严格条件的索引上读取和更新，当更新或者删除可以用单独的索引来读取得到时： UPDATE tbl_name SET column=value WHEREunique_key_col=key_value;DELETE FROM tbl_name WHERE unique_key_col=key_value;

3 ） SELECT 和 INSERT 语句并发的执行，但是只有很少的 UPDATE 和 DELETE 语句。

4 ）很多的扫描表和对全表的 GROUP BY 操作，但是没有任何写表。

表锁的缺点：

1 ）一个客户端提交了一个需要长时间运行的 SELECT 操作。

2 ）其他客户端对同一个表提交了 UPDATE 操作，这个客户端就要等到 SELECT 完成了才能开始执行。

3 ）其他客户端也对同一个表提交了 SELECT 请求。由于 UPDATE 的优先级高于 SELECT ，所以 SELECT 就会先等到 UPDATE 完成了之后才开始执行，它也在等待第一个 SELECT 操作。

1.3.        如何避免锁的资源竞争

1 ）让 SELECT 速度尽量快，这可能需要创建一些摘要表。

2 ）启动 mysqld 时使用参数 --low-priority-updates 。这就会让更新操作的优先级低于SELECT 。

这种情况下，在上面的假设中，第二个 SELECT 就会在 INSERT 之前执行了，而且也无需等待第一个 SELECT 了。

3 ）可以执行 SETLOW_PRIORITY_UPDATES=1 命令，指定所有的更新操作都放到一个指定的链接中去完成。

4 ）用 LOW_PRIORITY 属性来降低 INSERT ， UPDATE ，DELETE 的优先级。

5 ）用 HIGH_PRIORITY 来提高 SELECT 语句的优先级。

6 ）从 MySQL 3.23.7 开始，可以在启动 mysqld 时指定系统变量 max_write_lock_count 为一个比较低的值，它能强制临时地提高表的插入数达到一个特定值后的所有 SELECT 操作的优先级。它允许在 WRITE 锁达到一定数量后有 READ 锁。

7 ）当 INSERT 和 SELECT 一起使用出现问题时，可以转而采用 MyISAM 表，它支持并发的 SELECT 和 INSERT 操作。

8 ）当在同一个表上同时有插入和删除操作时， INSERTDELAYED 可能会很有用。

9 ）当 SELECT 和 DELETE 一起使用出现问题时， DELETE 的 LIMIT 参数可能会很有用。

10 ）执行 SELECT 时使用 SQL_BUFFER_RESULT 有助于减短锁表的持续时间。

11 ）可以修改源代码 `mysys/thr_lock.c' ，只用一个所队列。这种情况下，写锁和读锁的优先级就一样了，这对一些应用可能有帮助。






