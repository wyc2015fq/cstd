# POSTGRESQL 并发控制 - =朝晖= - 博客园
# [POSTGRESQL 并发控制](https://www.cnblogs.com/dhcn/p/7120895.html)
http://meidayhxp.blog.163.com/blog/static/117608156201210243837491/
这个内容是官方Doc中的一章，具体是那一版的，还未确认。
# 第九章 并发控制
本章介绍PostgreSQL的并发控制机制。当两个或多个用户同时访问同一个数据行时，需要使用并发控制机制来维护数据的完整性和一致性。
## 9.1 概述
PostgreSQL使用的是多版本并发控制机制(Multiversion Concurrency Control, MVCC)。多版本并发控制机制的意思是[数据库](http://lib.csdn.net/base/mysql)中的每个事务在查询数据时，看到的是数据的快照（一个历史版本），而不是数据的当前状态。在多版本并发控制机制中，读操作不用等待写操作，写操作不用等待读操作，只有在两个事务试图同时更新同一个数据行时，才会有等待出现。多版本并发控制机制可以减少数据库中的锁争用，减少用户的等待时间，提高数据库的性能。
PostgreSQL同时也提供了接口让应用程序显式对数据进行加锁操作。这些接口支持表级锁和行级锁。此外，还提供了建议锁（advisory lock）这样的锁机制，使得应用程序能够完全自主地控制得到和释放锁的时间。
## 9.2事务的隔离级别
SQL标准定义了四个事务隔离级别，表9-1列出了所有的隔离级别，事务在不同的隔离级别可以中看到的数据是不相同的。
**表9-1. SQL 事务隔离级别**
|**隔离级别**|**脏读（dirty read）**|**不可重复读（nonrepeatable read）**|**影子读（Phantom Read）**|
|----|----|----|----|
|Read uncommitted|可能|可能|可能|
|Read committed|不可能|可能|可能|
|Repeatable read|不可能|不可能|可能|
|Serializable|不可能|不可能|不可能|
脏读（dirty read）
一个事务可以读取其它还未提交的事务修改的数据。
不可重复读（nonrepeatable read）
一个事务重新读取以前读过的数据时，发现该数据被修改过。
影子读（phantom read）
一个事务两次执行同一个查询，发现第二次查询得到的数据行比第一次查询得到的数据行多。
PostgreSQL只提供了两种事务隔离级别，分别是Read Committed 和Serializable。使用命令SET TRANSACTION来设置事务的隔离级别。应用程序可以将事务的隔离级别设为Read Uncommitted，但系统会自动将隔离级别设为Read Committed。应用程序也可以将事务的隔离级别设为Repeatable Read，但系统会自动将隔离级别设为Serializable。这样的规则是符合SQL标准的。
### 9.2.1 Read Committed隔离级别
Read Committed是PostgreSQL的默认隔离级别。如果一个事务运行在这个隔离级别，事务发出的SELECT命令只能看见在SELECT命令开始执行以前提交的数据。同一个事务的两个先后执行的 SELECT命令可能会看到不同的数据，因为在其它并发执行的事务可能在第一个SELECT命令执行的过程中被提交。另外，事务发出的SELECT命令可以看到该事务以前发出的更新命令（包括INSERT、DELETE和UPDATE）修改过的数据。
命令UPDATE、 DELETE、SELECT FOR UPDATE和SELECT FOR SHARE可以看到的数据和SELECT命令是一样的。如果一个事务执行命令UPDATE（DELETE、SELECT FOR UPDATE或SELECT FOR SHARE），该命令发现一个数据行R1满足自己的搜索条件，同时有另外一个事务T2已经锁住了数据行T1（T2可能正在删除或更新R1），那么T1将进入等待状态，直到T2执行结束（回滚或提交），T1才能继续运行，根据T2的执行结果，T1有两种执行方式：
（1）如果T2被回滚，T2对R1做的更新将被取消，T1会使用R1原来的值继续运行命令。
（2）如果T2被提交，那要分两种情况：
（a）T2删除了数据行R1，那么T1继续运行时，将忽略R1, R1对T1正在执行的命令是不可见的。
（b）T2修改了数据行R1, 那么T1继续运行时，T1正在执行的命令将看见R1的新的值，同时会重新检查R1的新值是否符合该命令的WHERE子句的搜素条件, 如果符合，将使用R1的新值作为搜索结果，如果不符合，将忽略R1的新值。
Read Committed隔离级别值保证一个事务的单个命令看到的数据是一致的，不能保证一个事务发出的所有命令看到的数据都是一致的。
Read Committed隔离级别可以满足大部分应用程序的需要，它使用简单，比serializable隔离级别要快速。但那些使用复杂的查询和更新操作的应用可能需要数据库提供更加严格数据的一致性，这种情况下应该使用Serializable隔离级别。
### 9.2.2 Serializable隔离级别
Serializable是最严格的隔离级别。在这种隔离级别下，所有并发执行的事务的执行结果和单个事务一个一个地执行的结果是一样的。在这种隔离级别下运行的应用程序的逻辑要复杂一些，在事务不符合可串行化要求的而被终止的情况下，应用程序应该能够重新创建被终止的事务并再次请求数据库执行该事务。
在Serializable隔离级别下运行的应用程序，一个事务发出的SELECT命令只能看见事务开始运行以前已经提交的数据，看不见在事务开始运行以前没有提交的数据和在事务执行过程中其它并发执行的事务提交的数据。同一个事务的两个先后执行的 SELECT命令看到的数据是一样的。但事务的SELECT命令可以看到该事务以前发出的更新命令（包括INSERT、DELETE和UPDATE）修改过的数据。
命令UPDATE、 DELETE、SELECT FOR UPDATE和SELECT FOR SHARE可以看到的数据和SELECT命令是一样的。这些命令只能看到事务开始运行以前已经提交的数据。但是如果一个事务执行命令UPDATE（DELETE、SELECT FOR UPDATE或SELECT FOR SHARE），该命令发现一个数据行R1满足自己的搜索条件，同时有另外一个事务T2已经锁住了数据行T1（T2可能正在删除或更新R1），那么T1将进入等待状态，直到T2执行结束（回滚或提交），T1才能继续运行，根据T2的执行结果，T1有两种执行方式：
（1）如果T2被回滚，T2对R1做的更新将被取消，T1会使用R1原来的值继续运行命令。
（2）如果T2被提交，而且T2删除或者更新了R1，T1将被回滚，数据库会发出下面的提示信息：
错误: 当前事务运行在可串行化模式下，与其它并发执行的事务冲突，将被回滚。
如果应用程序收到上面的错误信息，应该终止执行当前事务，并重新从头开始执行这个事务。注意，只有含有更新操作的事务可能遇到上面的错误而被终止，只读事务永远都不会被被终止。
Serializable隔离级别保证每个事务在执行过程中看到完全一致的数据。但应用程序的逻辑会变得很复杂，多个事务若同时更新同一个数据行，其中有一个事务就可能失败，应用程序必须重新执行失败的事务，对数据库的压力就会变大。如果事务的更新逻辑非常复杂，导致在Read Committed隔离级别下可能出现不正确的结果，应该使用Serializable隔离级别。通常如果一个事务的几个连续执行的命令需要看到一样的数据，就应该使用Serializable隔离级别。
### 9.2.3 Serializable隔离级别和真正的可串行化
Serializable隔离级别并没有保证真正的数学上的可串行化，它只是保证事务在执行时不会出现脏读、不可重复读和Phantom Read**。**
例如，假设有一个叫mytab的表，它的内容如下：
 class | value -------+-------     1 |    10     1 |    20     2 |   100     2 |   200
有一个事务T1:
BEGIN
SELECT SUM(value) FROM mytab WHERE class = 1;INSERT INTO  mytab VALUES(2, SUM(value));  --注意SUM(value)表示将第一查询的结果插入到表中，这不是正确的INSERT命令语法。COMMIT
有另一个事务T2:
BEGIN
SELECT SUM(value) FROM mytab WHERE class = 2;INSERT INTO  mytab VALUES(1, SUM(value));  ));  --注意SUM(value)表示将第一查询的结果插入到表中，这不是正确的INSERT命令语法
COMMIT
假设T1和T2在Serializable隔离级别下被执行。
如果T1和T2并发执行，T1得到的结果是30，同时mytab中会多一个数据行(2,30)。T2得到的结果是300，同时mytab中会多一个数据行(1,30)。
如果T1先执行，T1结束后，T2再执行，T1得到的结果是30，同时mytab中会多一个数据行(2,30)。T2得到的结果是330，同时mytab中会多一个数据行(1,330)。
如果T2先执行，T2结束后，T1再执行，T1得到的结果是330，同时mytab中会多一个数据行(2,330)。T2得到的结果是300，同时mytab中会多一个数据行(1,300)。
从上面的例子可以看出在Serializable隔离级别下，并发执行的事务的执行结果与事务串行执行的结果并不相同。
如果想保证真正的数学上的可串行化，数据库必须使用谓词锁(predicate lock，SQL Server叫range lock)，意思是如果一个事务T1正在执行一个查询，该查询的的WHERE子句存在一个条件表达式E1，那么另外一个事务T2 就不能插入或删除任何满足E1的数据行。例如，一个事务A正在执行一个查询SELECT ... WHERE class = 1，另外一个事务就不能插入、更新或删除任何满足“class=1”的数据行。只有在A提交以后，B才能进行这样的操作。
实现谓词锁的代价非常高，而且事务会经常处理等待的状态，增加了查询的响应时间。而且绝大大不部分的应用都不需要数据库保证真正的数学上的可串行化。所以PostgreSQL并没有实现谓词锁，如果应用程序需要数据库保证真正的数学上的可串行化，可以使用PostgreSQL提供的命令对数据显示地进行加锁操作，后面的小节将会讨论如何对数据进行加锁操作。
## 9.3 显式加锁
PostgreSQL提供了多种锁模式来控制对表中的数据的并发访问。如果MVCC 不能满足应用的需求，可以使用这些锁来实现需要的功能。大部分的数据库命令在执行时，数据库会自动锁住相关的资源，不需要用户干预。可以查看视图pg_locks来得到系统中当前的锁信息。
### 9.3.1表级锁
下面列出了数据库使用的表级锁的模式和数据库在什么时候会使用这些锁。通常数据库在执行命令时会自动对表加上每个命令需要使用的锁。用户也可以使用命令LOCK对表进行加锁操作。
ACCESS SHARE
只与 ACCESS EXCLUSIVE模式冲突。
SELECT命令会在引用的表上加上这种模式的锁。 一般来说, 任何只是读取一个表中的数据而不修改表中的数据的查询都会表上加上这种模式的锁。
ROW SHARE
与EXCLUSIVE和ACCESS EXCLUSIVE模式冲突。
SELECT FOR UPDATE和SELECT FOR SHARE命令会在引用的表上加上这种模式的锁(同时会对查询引用的其它的表加ACCESS SHARE锁)。
ROW EXCLUSIVE
与SHARE, SHARE ROW EXCLUSIVE, EXCLUSIVE和ACCESS EXCLUSIVE模式冲突。
UPDATE, DELETE和INSERT命令会在引用的表上加上这种模式的锁((同时会对查询引用的其它的表加ACCESS SHARE锁)。 一般来说, 任何修改表中的数据的查询都会表上加上这种模式的锁。
SHARE UPDATE EXCLUSIVE
与SHARE UPDATE EXCLUSIVE, SHARE, SHARE ROW EXCLUSIVE, EXCLUSIVE和ACCESS EXCLUSIVE模式冲突。这个模式保证其它事务不能更新表的定义信息。VACUUM (不带FULL), ANALYZE和CREATE INDEX CONCURRENTLY命令会在引用的表上加上这种模式的锁。
SHARE
与 ROW EXCLUSIVE, SHARE UPDATE EXCLUSIVE, SHARE ROW EXCLUSIVE, EXCLUSIVE和ACCESS EXCLUSIVE模式冲突。 这个模式保证其它事务不能同时更新表中的数据。
CREATE INDEX (不带CONCURRENTLY) 命令会在引用的表上加上这种模式的锁。
SHARE ROW EXCLUSIVE
与ROW EXCLUSIVE, SHARE UPDATE EXCLUSIVE, SHARE, SHARE ROW EXCLUSIVE, EXCLUSIVE和ACCESS EXCLUSIVE模式冲突。
任何PostgreSQL命令都不会自动得到这种模式的锁。
EXCLUSIVE
与 ROW SHARE, ROW EXCLUSIVE, SHARE UPDATE EXCLUSIVE, SHARE, SHARE ROW EXCLUSIVE, EXCLUSIVE和ACCESS EXCLUSIVE模式冲突 。这种模式只允许其它的事务读表中的数据，不允许其它事务写表中的数据。PostgreSQL命令不会自动在任何用户创建的表上加这种模式的锁。只有在访问系统表时才会用到这种模式的锁。
ACCESS EXCLUSIVE
与所有的锁模式都冲突 (ACCESS SHARE, ROW SHARE, ROW EXCLUSIVE, SHARE UPDATE EXCLUSIVE, SHARE,SHARE ROW EXCLUSIVE, EXCLUSIVE, and ACCESS EXCLUSIVE)。这个锁模式保证只有拥有该锁的事务才能访问表，其它事务不能读也不能写这个表。ALTER TABLE, DROP TABLE, TRUNCATE, REINDEX, CLUSTER和VACUUM FULL命令会得到这种模式的锁。LOCK TABLE命令如果没有指定锁的模式，默认使用这种模式来锁住表。
**提示: **只有ACCESS EXCLUSIVE模式的锁才能阻止SELECT (不带FOR UPDATE/SHARE)命令被执行。
事务一旦得到某个模式的锁，该锁只有在事务结束以后才会被释放。但事务在创建了一个savepoint以后得到的锁会在事务被回滚该savepoint以后立即被释放。
如果两个锁模式互相冲突，那么两个不同的事务不能在同一个表上同时拥有这两种模式的锁。如果两个锁模式不是互相冲突的，那么两个不同的事务可以在同一个表上同时拥有这两种模式的锁。但同一个事务可以同时在同一个表上拥有两个互相冲突的锁模式。例如，ACCESS EXCLUSIVE与ACCESS SHARE是互相冲突的锁模式，但一个事务可以先在一个表上得到ACCESS EXCLUSIVE模式的锁，然后又可以在该表上得到ACCESS SHARE模式的锁。表9-2列出了锁冲突矩阵，其中的X表示冲突，空白表示不冲突。
注意，有些锁模式与自身是冲突的，例如，两个事务不能在同一表上同时拥有ACCESS EXCLUSIVE模式的锁。注意，有些锁模式与自身是不冲突的，例如，两个事务可以在同一表上同时拥有ACCESS SHARE模式的锁。
**表9-2. 锁冲突矩阵**
|**请求的锁模式**|**当前锁模式**| | | | | | | |
|----|----|----|----|----|----|----|----|----|
|**ACCESS SHARE**|**ROW SHARE**|**ROW EXCLUSIVE**|**SHARE UPDATE EXCLUSIVE**|**SHARE**|**SHARE ROW EXCLUSIVE**|**EXCLUSIVE**|**ACCESS EXCLUSIVE**| |
|ACCESS SHARE||||||||X|
|ROW SHARE|||||||X|X|
|ROW EXCLUSIVE|||||X|X|X|X|
|SHARE UPDATE EXCLUSIVE||||X|X|X|X|X|
|SHARE|||X|X||X|X|X|
|SHARE ROW EXCLUSIVE|||X|X|X|X|X|X|
|EXCLUSIVE||X|X|X|X|X|X|X|
|ACCESS EXCLUSIVE|X|X|X|X|X|X|X|X|
### 9.3.2行级锁
除了表级锁以外，还有行级锁。行级锁的模式有互斥和共享两种。当一个数据行被删除或更新时，事务会自动得到这个数据行上的互斥行级锁。一个事务获得的行级锁只有在事务提交或回滚以后才会被释放。行级锁不会影响查询操作，如果有其它事务企图更新同一个数据行，该事务将会进入等待状态，直到持有行级锁的事务被回滚或提交。如果一个命令只是想锁住数据行，而不是想更新数据行，可以使用SELECT FOR UPDATE，同一个事务以后发出的命令就可以随意地修改被锁住的数据行。
如果想得到一个数据行上的共享锁，使用命令SELECT FOR SHARE。多个不同的事务可以在同一个数据行上同时持有共享锁。如果一个事务已经在一个数据行上持有共享锁，那么其它的事务就不能更新和删除这个数据行，也不能在这个数据行上得到互斥锁。
PostgreSQL不会限制一个事务一次能同时锁住的数据行的最大数目，但每锁住一个数据行就会有一次写磁盘的操作。
除了表级锁和行级锁，数据库中还存在数据页级别的共享和互斥锁，数据页级别的锁用来协调对表的的某个数据页并发访问操作，它是由数据库自动得到和释放的。开发应用程序时不必关心数据页级别的锁，只要知道它存在就可以了。
### 9.3.3死锁
使用锁来协调多个事务对表中的数据的并发访问，很容易产生死锁，即两个或多个事务中的每个事务都拥有其它的某个或多个事务需要的锁资源，所有的事务都会无限期地等待下去。
系统会自动检测数据库中是否有死锁发生，如果有，会回滚一个造成死锁的事务，让其它的因死锁而等待的事务继续执行。
### 9.3.4建议锁（Advisory Locks）
PostgreSQL提供了一种被称为建议锁的锁机制。建议锁的含义是有应用程序自己特定的，何时得到和释放建议锁也是由应用程序自己控制的，数据库只是提供得到和释放建议锁的接口供用户使用。建议锁同事务没有任何关系，在一个事务中得到的建议锁，即使在事务被回滚以后仍然存在。建议锁的所有者是会话而非事务，一个会话可以多次得到同一个建议锁（如果一个会话执行了n次得到同一个建议锁的操作，那么它必须执行n次释放这个建议锁的操作，否则该建议锁不会被真正地释放，仍然被这个会话所有）。在一个会话结束后，该会话持有的所有建议锁都会被自动释放。
可以查看视图pg_locks得到当前会话持有的所有建议锁的列表。建议锁存放在数据库的共享内存中，数据库中的建议锁的最大数目受参数max_locks_per_transaction 和max_connections控制。
数据库提供的用于处理建议锁的函数参见第7.22节表7-50。
下面是一些使用建议锁的实例：
SELECT pg_advisory_lock(id) FROM foo WHERE id = 12345; -- ok
SELECT pg_advisory_lock(q.id) FROM
(
SELECT id FROM foo WHERE id > 12345 LIMIT 100;
) q;
## 9.4 在应用程序级别检查数据的一致性
因为PostgreSQL中的只读查询无论在任何隔离事务级别下都不会在读取的数据上加锁，一个事务读取的数据可能已被其它并发执行的事务修改。每个事务都会看到一个数据库内容的快照，并发执行的事务可能看到不同的数据库快照。如果事务想保证自己读取的数据在自己提交或回滚以前不被其它的事务修改，应该使用SELECT FOR UPDATE或SELECT FOR SHARE来读取数据（而不是只使用SELECT），也可以使用LOCK TABLE来锁住表，不让其它事务更新表中的数据。
如果应用程序希望一个事务在执行的过程中所引用的表的数据不能有任何变化，可以先用LOCK TABLE命令锁住所有的表，然后再执行其它的命令。
一般使用显示的加锁命令时，事务应该运行在Read Committed隔离级别下，也可以使用Serializable隔离级别。在Serializable隔离级别下使用LOCK TABLE命令时，注意LOCK TABLE命令应该作为事务发出的第一条命令。
## 9.5 锁与索引
PostgresSQL在访问索引时也会使用锁来控制对索引的并发访问，不同类型的索引的加锁模式如下：
B-tree 和GiST索引
使用短期的共享或互斥数据页级别的锁来控制对索引的并发访问。在每个索引数据行被读取或删除以后，锁会被立即释放。不会出现死锁的情况。
哈希索引
使用共享或互斥数据页级别的锁来控制对索引的并发访问。在每个哈希桶被处理以后，锁会被立即释放。可能会出现死锁。
GIN索引
使用短期的共享和互斥数据页级别的锁来控制对索引的并发访问。在每个索引数据行被读取或删除以后，锁会被立即释放 。GIN类型索引的插入操作可能素要比较大的工作量。
对标量数据类型建立索引时，应该使用B-tree索引。对非标量类型的数据建立索引，应该使用Gist或GIN索引。

