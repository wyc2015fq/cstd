# 测试运维工程师&基础知识必备之ＭongｏDB篇 - zhusongziye的博客 - CSDN博客





2018年11月10日 22:22:09[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：62








**Part1:**

**NO1****我怎么查看 Mongo 正在使用的链接？**

db._adminCommand("connPoolStats");



**NO2mongodb的结构介绍**

数据库中存储的对象设计bson，一种类似json的二进制文件，由键值对组成



**NO3为什么MongoDB的数据文件很大？**

MongoDB采用的预分配空间的方式来防止文件碎片



**NO4MongoDB在A:{B,C}上建立索引，查询A:{B,C}和A:{C,B}都会使用索引吗？**

不会，只会在A:{B,C}上使用索引



**NO5非关系型数据库有哪些？**

Membase、MongoDB、Hypertable



**NO6如何理解MongoDB中的GridFS机制，MongoDB为何使用GridFS来存储文件？**

GridFS是一种将大型文件存储在MongoDB中的文件规范。使用GridFS可以将大文件分隔成多个小文档存放，这样我们能够有效的保存大文档，而且解决了BSON对象有限制的问题。



**NO7数据在什么时候才会扩展到多个分片（shard）里？**

mongodb分片是基于区域的，所以一个集合的所有对象都放置在同一个块中，只有当存在多余一个块的时候，才会有多个分片获取数据的选项



**NO8当我试图更新一个正在被迁移的块（chunk）上的文档时会发生什么？**

会立即更新旧的分片，然后更改才会在所有权转移前复制到新的分片上



**NO9分片（sharding）和复制（replication）是怎样工作的？**

分片可能是单一的服务器或者集群组成，推荐使用集群



**NO10分析器在MongoDB中的作用是什么?**

分析器就是explain 显示每次操作性能特点的数据库分析器。通过分析器可能查找比预期慢的操作



**Part 2:**



**NO1**MySQL和MongoDB之间最基本的区别是什么？****

关系型数据库与非关系型数据库的区别，即数据存储结构的不同。



**NO2**MongoDB的特点是什么？****

面向文档

高性能

高可用

易扩展

丰富的查询语言



**NO3MongoDB支持存储过程吗？如果支持的话，怎么用？**

MongoDB支持存储过程，它是javascript写的，保存在db.system.js表中。



**NO4**数据库三范式?****

1NF 属性不可分 

2NF 非主键属性，完全依赖于主键属性 

3NF 非主键属性无传递依赖



**NO5**死锁怎么解决?****

找到进程号，kill 进程。



**NO6有哪些锁,select时怎么加排它锁**

乐观锁：自己实现，通过版本号 

悲观锁：共享锁，多个事务，只能读不能写，加 lock in share mode 

排它锁：一个事务，只能写，for update 

行锁 

表锁 



**NO7**使用mongodb的优点？****

面向文件

高性能

高可用

易扩展

可分片

对数据存储友好



** NO8分析器在MongoDB中的作用是什么?**

分析器就是explain 显示每次操作性能特点的数据库分析器。通过分析器可能查找比预期慢的操作



