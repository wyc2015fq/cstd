
# Redis 集合(Set) -  芝麻软件工作室的专栏 - CSDN博客


2016年07月27日 10:21:43[seven-soft](https://me.csdn.net/softn)阅读数：158


Redis的Set是string类型的无序集合。集合成员是唯一的，这就意味着集合中不能出现重复的数据。
Redis 中 集合是通过哈希表实现的，所以添加，删除，查找的复杂度都是O(1)。
集合中最大的成员数为 232 - 1(4294967295, 每个集合可存储40多亿个成员)。
### 实例
redis127.0.0.1:6379>SADD w3ckey redis(integer)1redis127.0.0.1:6379>SADD w3ckey mongodb(integer)1redis127.0.0.1:6379>SADD w3ckey mysql(integer)1redis127.0.0.1:6379>SADD w3ckey mysql(integer)0redis127.0.0.1:6379>SMEMBERS w3ckey1)"mysql"2)"mongodb"3)"redis"在以上实例中我们通过**SADD**命令向名为 w3ckey 的集合插入的三个元素。
---

## Redis 集合命令
下表列出了 Redis 集合基本命令：
|序号|命令及描述|
|1|[SADD key member1 [member2]](http://www.runoob.com/redis/sets-sadd.html)
|向集合添加一个或多个成员|
|2|[SCARD key](http://www.runoob.com/redis/sets-scard.html)
|获取集合的成员数|
|3|[SDIFF key1 [key2]](http://www.runoob.com/redis/sets-sdiff.html)
|返回给定所有集合的差集|
|4|[SDIFFSTORE destination key1 [key2]](http://www.runoob.com/redis/sets-sdiffstore.html)
|返回给定所有集合的差集并存储在 destination 中|
|5|[SINTER key1 [key2]](http://www.runoob.com/redis/sets-sinter.html)
|返回给定所有集合的交集|
|6|[SINTERSTORE destination key1 [key2]](http://www.runoob.com/redis/sets-sinterstore.html)
|返回给定所有集合的交集并存储在 destination 中|
|7|[SISMEMBER key member](http://www.runoob.com/redis/sets-sismember.html)
|判断 member 元素是否是集合 key 的成员|
|8|[SMEMBERS key](http://www.runoob.com/redis/sets-smembers.html)
|返回集合中的所有成员|
|9|[SMOVE source destination member](http://www.runoob.com/redis/sets-smove.html)
|将 member 元素从 source 集合移动到 destination 集合|
|10|[SPOP key](http://www.runoob.com/redis/sets-spop.html)
|移除并返回集合中的一个随机元素|
|11|[SRANDMEMBER key [count]](http://www.runoob.com/redis/sets-srandmember.html)
|返回集合中一个或多个随机数|
|12|[SREM key member1 [member2]](http://www.runoob.com/redis/sets-srem.html)
|移除集合中一个或多个成员|
|13|[SUNION key1 [key2]](http://www.runoob.com/redis/sets-sunion.html)
|返回所有给定集合的并集|
|14|[SUNIONSTORE destination key1 [key2]](http://www.runoob.com/redis/sets-sunionstore.html)
|所有给定集合的并集存储在 destination 集合中|
|15|[SSCAN key cursor [MATCH pattern] [COUNT count]](http://www.runoob.com/redis/sets-sscan.html)|

