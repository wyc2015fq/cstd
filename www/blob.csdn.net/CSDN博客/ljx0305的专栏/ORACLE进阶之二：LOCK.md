# ORACLE进阶之二：LOCK - ljx0305的专栏 - CSDN博客
2008年10月08日 14:07:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：555标签：[oracle																[数据库																[session																[insert																[dictionary																[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=dictionary&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=session&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[Oracle体系结构](https://blog.csdn.net/ljx0305/article/category/428729)
数据库是一个多用户使用的共享资源。当多个用户并发地存取数据时，在数据库中就会产生多个事务同时存取同一数据的情况。若对并发操作不加控制就可能会读取和存储不正确的数据，破坏数据库的一致性。 
加锁是实现数据库并发控制的一个非常重要的技术。当事务在对某个数据对象进行操作前，先向系统发出请求，对其加锁。加锁后事务就对该数据对象有了一定的控制，在该事务释放锁之前，其他的事务不能对此数据对象进行更新操作。 
在数据库中有两种基本的锁类型：排它锁（Exclusive Locks，即X锁）和共享锁（Share Locks，即S锁）。当数据对象被加上排它锁时，其他的事务不能对它读取和修改。加了共享锁的数据对象可以被其他事务读取，但不能修改。数据库利用这两种基本的锁类型来对数据库的事务进行并发控制。 
根据保护的对象不同，Oracle数据库锁可以分为以下几大类：
 DML锁（data locks，数据锁），用于保护数据的完整性；
 DDL锁（dictionary locks，字典锁），用于保护数据库对象的结构，如表、索引等的结构定义；
 内部锁和闩（internal locks and latches），保护数据库的内部结构，应用于SGA；
在我们实际应用开发中涉及较多的是DML锁，其他两种的话DBA会更加关心点；
DML锁的目的在于保证并发情况下的数据完整性，主要包括TM锁和TX锁，其中TM锁称为表级锁，TX锁称为事务锁或行级锁。 
当Oracle执行DML语句时，系统自动在所要操作的表上申请TM类型的锁。当TM锁获得后，系统再自动申请TX类型的锁，并将实际锁定的数据行的锁标志位进行置位。这样在事务加锁前检查TX锁相容性时就不用再逐行检查锁标志，而只需检查TM锁模式的相容性即可，大大提高了系统的效率。TM锁包括了SS、SX、S、X等多种模式，在数据库中用0－6来表示。不同的SQL操作产生不同类型的TM锁。如下图所示：  
|值|锁模式|锁描述|SQL|
|----|----|----|----|
|0|NONE|||
|1|NULL|空|SELECT|
|2|SS(ROW-S)|行级共享锁其他对象只能查询这些数据行|SELECT FOR UPDATE、LOCK FOR UPDATE、LOCK ROW SHARE|
|3|SX(ROW-X)|行级排它锁在提交前不允许做DML操作|INSERT、UPDATE、DELETE、LOCK ROW SHARE|
|4|S(SHARE)|共享锁|CREATE INDEX、LOCK SHARE|
|5|SSX(S/ROW-X)|共享行级排它锁|LOCK SHARE ROW EXCLUSIVE|
|6|X(eXclusive)|排它锁|ALTER TABLE、DROP TABLE、DROP INDEX、TRUNCATE TABLE、LOCK EXCLUSIVE|
在数据行上只有X锁（排他锁）。在 Oracle数据库中，当一个事务首次发起一个DML语句时就获得一个TX锁，该锁保持到事务被提交或回滚。当两个或多个会话在表的同一条记录上执行DML语句时，第一个会话在该条记录上加锁，其他的会话处于等待状态。当第一个会话提交后，TX锁被释放，其他会话才可以加锁。 
在大概了解oracle的锁机制之后，我们来解决几个基本的问题：
1.UPDATE/DELETE操作会将RS锁定，直至操作被COMMIT或者ROLLBACK；
 若操作未COMMIT之前其他session对同样的RS做变更操作，则操作会被hold，直至前session的UPDATE/DELETE操作被COMMIT；
2.session内外SELECT的RS范围
 前提：INSERT、UPDATE操作未COMMIT之前进行SELECT；
 若在同一session内，SELECT出来的RS会包括之前INSERT、UPDATE影响的记录；
 若不在同一session内，SELECT出来的RS不会包括未被COMMIT的记录；
3.SELECT.... FOR UPDATE [OF cols] [NOWAIT/WAIT] [SKIP LOCKED]
 OF cols：只锁定指定字段所在表的RS，而没有指定的表则不会锁定，只会在多表联合查询时出现；
 NOWAIT：语句不会hold，而是直接返回错误ORA-00054: resource busy and acquire with NOWAIT specified；
 WAIT N：语句被hold N秒，之后返回错误ORA-30006: resource busy; acquire with WAIT timeout expired；
 SKIP LOCKED：不提示错误，而是直接返回no rows selected；
 以上几个选项可以联合使用的，比较推荐的有：
 SELECT.... FOR UPDATE NOWAIT：对同一RS执行该SQL时，直接返回错误；
 SELECT.... FOR UPDATE NOWAIT SKIP LOCKED：对同一RS执行该SQL时，直接返回空行；
 PS：当RS被LOCK住之后，只对同样请求LOCK的语句有效，对无需LOCK的SELECT语句并没有任何影响； 
引用:http://blog.csdn.net/sfdev/archive/2008/01/13/2042325.aspx
