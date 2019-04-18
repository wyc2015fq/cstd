# MYSQL 当前读和快照读 - z69183787的专栏 - CSDN博客
2018年08月15日 19:34:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1504
[https://www.cnblogs.com/cat-and-water/p/6427612.html](https://www.cnblogs.com/cat-and-water/p/6427612.html)
好的学习链接：[http://blog.csdn.net/taylor_tao/article/details/7063639](http://blog.csdn.net/taylor_tao/article/details/7063639)
innodb的默认事务隔离级别是rr（可重复读）。它的实现技术是mvcc。基于版本的控制协议。该技术不仅可以保证innodb的可重复读，而且可以防止幻读。但是它防止的是快照读，也就是读取的数据虽然是一致的，但是数据是历史数据。如何做到保证数据是一致的（也就是一个事务，其内部读取对应某一个数据的时候，数据都是一样的），同时读取的数据是最新的数据。innodb提供了一个间隙锁的技术。也就是结合grap锁与行锁，达到最终目的。当使用索引进行插入的时候，innodb会将当前的节点和上一个节点加锁。这样当进行select的时候，就不允许加x锁。那么在进行该事务的时候，读取的就是最新的数据。
实现：
1. 快照读(snapshot read)
简单的select操作(不包括 select ... lock in share mode, select ... for update)
2.当前读(current read)
select ... lock in share mode
select ... for update
insert
update
delete
在RR级别下，快照读是通过MVVC(多版本控制)和undo log来实现的，当前读是通过加record lock(记录锁)和gap lock(间隙锁)来实现的。
所以从上面的显示来看，如果需要实时显示数据，还是需要通过加锁来实现。这个时候会使用next-key技术来实现。
总结：在mysql中，提供了两种事务隔离技术，第一个是mvcc，第二个是next-key技术。这个在使用不同的语句的时候可以动态选择。不加lock inshare mode之类的就使用mvcc。否则使用next-key。mvcc的优势是不加锁，并发性高。缺点是不是实时数据。next-key的优势是获取实时数据，但是需要加锁。同时需要注意几点：1.事务的快照时间点是以第一个select来确认的。所以即便事务先开始。但是select在后面的事务的update之类的语句后进行，那么它是可以获取后面的事务的对应的数据。2.mysql中数据的存放还是会通过版本记录一系列的历史数据，这样，可以根据版本查找数据。
