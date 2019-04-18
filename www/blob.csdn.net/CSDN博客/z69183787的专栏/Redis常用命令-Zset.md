# Redis常用命令-Zset - z69183787的专栏 - CSDN博客
2018年07月17日 11:25:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：66
[https://blog.csdn.net/xiaouncle/article/details/62236593](https://blog.csdn.net/xiaouncle/article/details/62236593)
Redis是一个开源的速度非常快的非关系数据库，它可以存储键（key）与5种不同类型的值（value）之间的映射（mapping），可以将存储在内存的键值对数据持久化到硬盘，可以使用复制特性来扩展读性能，还可以使用客户端分片来扩展写性能。、 
请注意：存储在Redis中的数据并不会由于计算机的关机而从内存中消失，必须手动删除数据才会消失。
## 一、Zset键值结构
![这里写图片描述](https://img-blog.csdn.net/20170416133414710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb3VuY2xl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
有序集合中元素的特点：有序、无重复、可进行集合间操作
## 二、重要API
|参数|解释|
|----|----|
|Zadd|Zadd key-name score member [score member…]——将带有给定分值的成员添加到有序列表里面|
|Zrem|Zrem key-name member [member…]——从有序集合里面移除给定的成员，并返回被移除成员的数量|
|Zcard|Zcard key-name——返回有序集合包含的成员数量|
|Zincrby|Zincrby key-name increment member——将member成员的分值加上increment|
|Zcount|Zcount key-name min max——返回分值介于min和max之间的成员数量，包括min和max在内|
|Zrank|Zrank key-name member——返回成员member在有序集合中的排名，成员按照分值从小到大排列|
|Zrevrank|Zrevrank key-name member——返回成员member在有序集合中的排名 ，成员按照分值从大到小排列|
|Zscore|Zscore key-name member——返回成员member的分值|
|Zrange|Zrange key-name start stop [WITHSCORES]——返回有序集合中排名介于start和stop之间的成员，包括start和stop在内，如果给定了可选的WITHSCORES选项，那么命令会将成员的分值一并返回，成员按照分值从小到大排列|
|Zrevrange|Zrevrange key-name start stop [WITHSCORES]——返回有序集合中排名介于start和stop之间的成员，包括start和stop在内，如果给定了可选的WITHSCORES选项，那么命令会将成员的分值一并返回，成员按照分值从大到小排列|
|Zrangebyscore|Zrangebyscore key-name min max [WITHSCORES] [LIMIT offset count]——返回有序集合中分值介于min和max之间的所有成员，包括min和max在内，并按照分值从小到大的排序来返回他们|
|Zrevrangebyscore|Zrevrangebyscore key-name min max [WITHSCORES] [LIMIT offset count]——返回有序集合中分值介于min和max之间的所有成员，包括min和max在内，并按照分值从大到小的排序来返回他们|
|Zremrangebyrank|Zremrangebyrank key-name start stop——移除有序集合中排名介于start和stop之间的所有成员，包括start和stop在内|
|Zremrangebyscore|Zremrangebyscore key-name key min max——移除有序集合中分值介于min和max之间的所有成员，包括min和max在内|
|Zinterstore|Zinterstore dest-key key-count key [key…][WEIGHTS weight [weight…]] [AGGREGATE SUM|MIN|MAX]——对给定的有序集合执行类似于集合的交集运算|
|Zunionstore|Zunionstore dest-key key-count key [key…][WEIGHTS weight [weight…]] [AGGREGATE SUM|MIN|MAX]——对给定的有序集合执行类似于集合的并集运算|
实例演示： 
zset-key1中的元素： 
val5　　　　10 
member3　 32 
member5　 40 
member6　 50
zset-key2中的元素： 
val1　　　　10 
val2　　　　20 
val3　　　　30 
val4　　　　40 
val5　　　　50 
member3　 60 
member5　 70
Zinterstore zset-key3 2 zset-key1 zset-key2 
zset-key3中的元素： 
val5　　　　50 
member3　 60 
member5　 70
Zunionstore zset-key4 2 zset-key1 zset-key2 
zset-key4中的元素： 
val1　　　　10 
val2　　　　20 
val3　　　　30 
val4　　　　40 
member6　 50 
val5　　　　60（50+10） 
member3　 92（32+60） 
member5　 110（40+70）
