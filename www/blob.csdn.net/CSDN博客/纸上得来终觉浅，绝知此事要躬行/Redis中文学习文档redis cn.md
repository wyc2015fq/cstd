# Redis中文学习文档redis.cn - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月20日 15:45:16[boonya](https://me.csdn.net/boonya)阅读数：785








# 文档

注意: Redis的文档在 [redis-doc github repository](http://github.com/antirez/redis-doc)同样也有提供。

## Redis 使用
- [Redis命令](http://www.redis.cn/commands.html) redis完整的命令列表，以及他们的说明文档。
- [管道（Pipelining）](http://www.redis.cn/topics/pipelining.html)：学习如何一次发送多个命令，节省往返时间。
- [Redis 发布/订阅（Pub/Sub）](http://www.redis.cn/topics/pubsub.html)：redis是一个快速、稳定的发布/订阅的信息系统。
- [Redis Lua 脚本](http://www.redis.cn/commands/eval.html)：Redis 2.6 Lua 脚本相关文档。
- [内存优化](http://www.redis.cn/topics/memory-optimization.html)：了解如何使用内存和学习一些使用技巧。
- [过期（Expires）](http://www.redis.cn/commands/expire.html)：Redis允许为每一个key设置不同的过期时间，当它们到期时将自动从服务器上删除。
- [将Redis当做使用LRU算法的缓存来使用](http://www.redis.cn/topics/lru-cache.html)：如何配置并且将Redis当做缓存来使用，通过限制内存及自动回收键。
- [Redis 事务](http://www.redis.cn/topics/transactions.html)：将一组命令放在同一个事务中进行处理。
- [大量插入数据](http://www.redis.cn/topics/mass-insert.html)：如何在短时间里向Redis写入大量数据。
- [从文件中批量插入数据](http://www.redis.cn/topics/batch-insert.html)：将文件中的指令批量执行。
- [分区（Partitioning）](http://www.redis.cn/topics/partitioning.html)：如何将你的数据分布在多个Redis里面。

## Redis 管理
- [配置（Configuration）](http://www.redis.cn/topics/config.html)：怎么配置 redis。
- [复制（Replication）](http://www.redis.cn/topics/replication.html)：你需要知道怎么设置主从复制。
- [持久化（Persistence）](http://www.redis.cn/topics/persistence.html)：了解如何配置redis的持久化。
- [Redis 管理（Redis Administration）](http://www.redis.cn/topics/admin.html)：学习redis管理方面的知识。
- [安全性（Security）](http://www.redis.cn/topics/security.html)：概述Redis的安全。
- [信号处理（Signals Handling）](http://www.redis.cn/topics/signals.html)：如何处理Redis信号。
- [连接处理（Connections Handling）](http://www.redis.cn/topics/clients.html)：如何处理Redis客户端连接。
- [高可用性（High Availability）](http://www.redis.cn/topics/sentinel.html)：Redis Sentinel是Redis官方的高可用性解决方案。目前工作进展情况（beta阶段，积极发展），已经可用。
- [基准（Benchmarks）](http://www.redis.cn/topics/benchmarks.html)：看看Redis在不同平台上跑得有多快。

## 故障排除
- [Redis 的问题?](http://www.redis.cn/topics/problems.html): 错误（Bugs）? 高延迟（High latency）? 其他问题? 使用
[我们的故障排除页面](http://www.redis.cn/topics/problems.html) 作为出发点，了解更多的信息。

## 非稳定版的功能文档
- [Redis的密钥空间通知（Redis keyspace notifications）](http://www.redis.cn/topics/notifications.html)：获得通过的Pub / Sub的密钥空间活动的通知。最近不稳定的分支合并到了此功能。这个实现的第一个稳定版本将在Redis的2.8。

## 产品规格（Specifications）
- [Redis的协议规范（Redis Protocol specification）](http://www.redis.cn/topics/protocol.html)如果你正在使用redis，或者对它好奇，这里将简单的介绍如何与redis交互。
- [Redis的RDB格式规范](https://github.com/sripathikrishnan/redis-rdb-tools/wiki/Redis-RDB-Dump-File-Format)，与
[RDB的版本历史记录](https://github.com/sripathikrishnan/redis-rdb-tools/blob/master/docs/RDB_Version_History.textile)。
- [内部机制（Internals）](http://www.redis.cn/topics/internals.html):详细学习Redis内部引擎是如何实现的。

## 教程 & FAQ
- [FAQ](http://www.redis.cn/topics/faq.html)：关于Redis的一些常见问题。
- [数据类型（Data types）](http://www.redis.cn/topics/data-types.html)：Redis支持不同类型值的摘要。
- [15分钟快速了解Redis的数据结构](http://www.redis.cn/topics/data-types-intro.html)
- [用PHP+Redis编写一个简单的Twitter](http://www.redis.cn/topics/twitter-clone.html)
- [用Redis实现自动完成](http://antirez.com/post/autocomplete-with-redis.html)

## 简报（Presentations）
- Salvatore Sanfilippo: [Redis的集群概述](http://redis.io/presentation/Redis_Cluster.pdf)
- Pieter Noordhuis: [Redis 2.2的新特性](http://redis.io/presentation/Pnoordhuis_whats_new_in_2_2.pdf)

如果你想在演示文稿中使用本标识（logo），请使用[高分辨率的版本](http://redis.io/images/redis-300dpi.png)。
[SVG版本](http://redis.io/images/redis-logo.svg)也是可用的。

## 使用案例（Use cases）
- [谁正在使用 Redis](http://www.redis.cn/topics/whos-using-redis.html)

## 书籍

以下是已经发布的Redis书籍的列表：
- 由 [Karl Seguin](http://twitter.com/karlseguin) 编写的
[《The Little Redis Book》](http://openmymind.net/2012/1/23/The-Little-Redis-Book/) 是一本非常简洁和开放的Reids入门书籍。
- [Josiah L. Carlson](http://twitter.com/dr_josiah) 编写的
[《Redis in Action》](http://www.manning.com/carlson/) 是早期的阅读版本。
- 还有 [Redis Cookbook (O'Reilly Media, 2011)](http://shop.oreilly.com/product/0636920020127.do)

下列书籍有Redis的相关内容，但不是专门关于Redis的：
- [Seven databases in seven weeks](http://pragprog.com/book/rwdata/seven-databases-in-seven-weeks) (Note: Redis chapter still coming soon).
- [Mining the Social Web (O'Reilly Media, 2011)](http://shop.oreilly.com/product/0636920010203.do)
- [Professional NoSQL (Wrox, 2011)](http://www.wrox.com/WileyCDA/WroxTitle/Professional-NoSQL.productCd-047094224X.html)

## 保荐人

[Salvatore Sanfilippo](http://antirez.com) 和 [Pieter Noordhuis](http://twitter.com/pnoordhuis) 发展redis的工作是由 [VMware](http://vmware.com)发起的。在过去Redis接受过的捐赠中，将在[赞助商的页面](http://www.redis.cn/topics/sponsors.html)中列出的其他公司。



关于[Redis 文档（documentation）互动](http://bbs.redis.cn/forum.php?mod=viewthread&tid=852)的最新评论



geelou 发布于 2016-8-31 17:31:41

Redis 文档（documentation）中心




