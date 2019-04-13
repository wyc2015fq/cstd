
# Redis 有序集合(sorted set) -  芝麻软件工作室的专栏 - CSDN博客


2016年07月27日 10:22:29[seven-soft](https://me.csdn.net/softn)阅读数：286个人分类：[Redis																](https://blog.csdn.net/softn/article/category/6332853)



Redis 有序集合和集合一样也是string类型元素的集合,且不允许重复的成员。
不同的是每个元素都会关联一个double类型的分数。redis正是通过分数来为集合中的成员进行从小到大的排序。
有序集合的成员是唯一的,但分数(score)却可以重复。
集合是通过哈希表实现的，所以添加，删除，查找的复杂度都是O(1)。 集合中最大的成员数为 232 - 1(4294967295, 每个集合可存储40多亿个成员)。
### 实例
redis127.0.0.1:6379>ZADD w3ckey1redis(integer)1redis127.0.0.1:6379>ZADD w3ckey2mongodb(integer)1redis127.0.0.1:6379>ZADD w3ckey3mysql(integer)1redis127.0.0.1:6379>ZADD w3ckey3mysql(integer)0redis127.0.0.1:6379>ZADD w3ckey4mysql(integer)0redis127.0.0.1:6379>ZRANGE w3ckey010WITHSCORES1)"redis"2)"1"3)"mongodb"4)"2"5)"mysql"6)"4"在以上实例中我们通过命令**ZADD**向 redis 的有序集合中添加了三个值并关联上分数。
---

## Redis 有序集合命令
下表列出了 redis 有序集合的基本命令:
|序号|命令及描述|
|1|[ZADD key score1 member1 [score2 member2]](http://www.runoob.com/redis/sorted-sets-zadd.html)
|向有序集合添加一个或多个成员，或者更新已存在成员的分数|
|2|[ZCARD key](http://www.runoob.com/redis/sorted-sets-zcard.html)
|获取有序集合的成员数|
|3|[ZCOUNT key min max](http://www.runoob.com/redis/sorted-sets-zcount.html)
|计算在有序集合中指定区间分数的成员数|
|4|[ZINCRBY key increment member](http://www.runoob.com/redis/sorted-sets-zincrby.html)
|有序集合中对指定成员的分数加上增量 increment|
|5|[ZINTERSTORE destination numkeys key [key ...]](http://www.runoob.com/redis/sorted-sets-zinterstore.html)
|计算给定的一个或多个有序集的交集并将结果集存储在新的有序集合 key 中|
|6|[ZLEXCOUNT key min max](http://www.runoob.com/redis/sorted-sets-zlexcount.html)
|在有序集合中计算指定字典区间内成员数量|
|7|[ZRANGE key start stop [WITHSCORES]](http://www.runoob.com/redis/sorted-sets-zrange.html)
|通过索引区间返回有序集合成指定区间内的成员|
|8|[ZRANGEBYLEX key min max [LIMIT offset count]](http://www.runoob.com/redis/sorted-sets-zrangebylex.html)
|通过字典区间返回有序集合的成员|
|9|[ZRANGEBYSCORE key min max [WITHSCORES] [LIMIT]](http://www.runoob.com/redis/sorted-sets-zrangebyscore.html)
|通过分数返回有序集合指定区间内的成员|
|10|[ZRANK key member](http://www.runoob.com/redis/sorted-sets-zrank.html)
|返回有序集合中指定成员的索引|
|11|[ZREM key member [member ...]](http://www.runoob.com/redis/sorted-sets-zrem.html)
|移除有序集合中的一个或多个成员|
|12|[ZREMRANGEBYLEX key min max](http://www.runoob.com/redis/sorted-sets-zremrangebylex.html)
|移除有序集合中给定的字典区间的所有成员|
|13|[ZREMRANGEBYRANK key start stop](http://www.runoob.com/redis/sorted-sets-zremrangebyrank.html)
|移除有序集合中给定的排名区间的所有成员|
|14|[ZREMRANGEBYSCORE key min max](http://www.runoob.com/redis/sorted-sets-zremrangebyscore.html)
|移除有序集合中给定的分数区间的所有成员|
|15|[ZREVRANGE key start stop [WITHSCORES]](http://www.runoob.com/redis/sorted-sets-zrevrange.html)
|返回有序集中指定区间内的成员，通过索引，分数从高到底|
|16|[ZREVRANGEBYSCORE key max min [WITHSCORES]](http://www.runoob.com/redis/sorted-sets-zrevrangebyscore.html)
|返回有序集中指定分数区间内的成员，分数从高到低排序|
|17|[ZREVRANK key member](http://www.runoob.com/redis/sorted-sets-zrevrank.html)
|返回有序集合中指定成员的排名，有序集成员按分数值递减(从大到小)排序|
|18|[ZSCORE key member](http://www.runoob.com/redis/sorted-sets-zscore.html)
|返回有序集中，成员的分数值|
|19|[ZUNIONSTORE destination numkeys key [key ...]](http://www.runoob.com/redis/sorted-sets-zunionstore.html)
|计算给定的一个或多个有序集的并集，并存储在新的 key 中|
|20|[ZSCAN key cursor [MATCH pattern] [COUNT count]](http://www.runoob.com/redis/sorted-sets-zscan.html)
|迭代有序集合中的元素（包括元素成员和元素分值）|

