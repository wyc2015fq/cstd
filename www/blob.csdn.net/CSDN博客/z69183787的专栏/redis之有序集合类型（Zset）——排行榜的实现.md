# redis之有序集合类型（Zset）——排行榜的实现 - z69183787的专栏 - CSDN博客
2018年07月17日 11:32:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1048
[https://blog.csdn.net/loophome/article/details/50373202](https://blog.csdn.net/loophome/article/details/50373202)
Redis有序集合和Redis集合类似，是不包含 相同字符串的合集。它们的差别是，每个有序集合 的成员都关联着一个评分，这个评分用于把有序集 合中的成员按最低分到最高分排列。
使用有序集合，你可以非常快地（O(log(N))）完成添加，删除和更新元素的操作。 因为元素是在插入时就排好序的，所以很快地通过评分(score)或者 位次(position)获得一个范围的元素。 访问有序集合的中间元素同样也是非常快的，因此你可以使用有序集合作为一个没用重复成员的智能列表。 在这个列表中， 你可以轻易地访问任何你需要的东西: 有序的元素，快速的存在性测试，快速访问集合中间元素！
简而言之，使用有序集合你可以很好地完成 很多在其他数据库中难以实现的任务。
at first，我们先添加一下数据：
- 
`> zadd member_list 10 a 3 b 1 c 4 d 7 e`
- 
`(integer) 5`
- 
`#返回5，即成功加了5个进入集合。现在试试添加重复的元素`
- 
`> zadd member_list 9 a 8 f`
- 
`(integer) 1`
- 
`#这个步骤，返回了1，而不是2，是因为a这个元素已经存在于集合当中了，不会添加成功。`
- 
`> zrange member_list 0 6 WITHSCORES`
- 
`1) "c"`
- 
`2) 1.0`
- 
`3) "b"`
- 
`4) 3.0`
- 
`5) "d"`
- 
`6) 4.0`
- 
`7) "e"`
- 
`8) 7.0`
- 
`9) "f"`
- 
`10) 8.0`
- 
`11) "a"`
- 
`12) 9.0`
可以看到，有序集合排序的规则是根据分母的大小，分母越小排在集合的前面
如下表显示与​​排序集的一些基本命令：
|S.N.|命令& 描述|
|----|----|
|1|[ZADD key score1 member1 [score2 member2]](http://www.yiibai.com/redis/sorted_sets_zadd.html)			添加一个或多个成员到有序集合，或者如果它已经存在更新其分数|
|2|[ZCARD key](http://www.yiibai.com/redis/sorted_sets_zcard.html)			得到的有序集合成员的数量|
|3|[ZCOUNT key min max](http://www.yiibai.com/redis/sorted_sets_zcount.html)			计算一个有序集合成员与给定值范围内的分数|
|4|[ZINCRBY key increment member](http://www.yiibai.com/redis/sorted_sets_zincrby.html)			在有序集合增加成员的分数|
|5|[ZINTERSTORE destination numkeys key [key ...]](http://www.yiibai.com/redis/sorted_sets_zinterstore.html)			多重交叉排序集合，并存储生成一个新的键有序集合。|
|6|[ZLEXCOUNT key min max](http://www.yiibai.com/redis/sorted_sets_zlexcount.html)			计算一个给定的字典范围之间的有序集合成员的数量|
|7|[ZRANGE key start stop [WITHSCORES]](http://www.yiibai.com/redis/sorted_sets_zrange.html)			由索引返回一个成员范围的有序集合。|
|8|[ZRANGEBYLEX key min max [LIMIT offset count]](http://www.yiibai.com/redis/sorted_sets_zrangebylex.html)			返回一个成员范围的有序集合（由字典范围）|
|9|[ZRANGEBYSCORE key min max [WITHSCORES] [LIMIT]](http://www.yiibai.com/redis/sorted_sets_zrangebyscore.html)			按分数返回一个成员范围的有序集合。|
|10|[ZRANK key member](http://www.yiibai.com/redis/sorted_sets_zrank.html)			确定成员的索引中有序集合|
|11|[ZREM key member [member ...]](http://www.yiibai.com/redis/sorted_sets_zrem.html)			从有序集合中删除一个或多个成员|
|12|[ZREMRANGEBYLEX key min max](http://www.yiibai.com/redis/sorted_sets_zremrangebylex.html)			删除所有成员在给定的字典范围之间的有序集合|
|13|[ZREMRANGEBYRANK key start stop](http://www.yiibai.com/redis/sorted_sets_zremrangebyrank.html)			在给定的索引之内删除所有成员的有序集合|
|14|[ZREMRANGEBYSCORE key min max](http://www.yiibai.com/redis/sorted_sets_zremrangebyscore.html)			在给定的分数之内删除所有成员的有序集合|
|15|[ZREVRANGE key start stop [WITHSCORES]](http://www.yiibai.com/redis/sorted_sets_zrevrange.html)			返回一个成员范围的有序集合，通过索引，以分数排序，从高分到低分|
|16|[ZREVRANGEBYSCORE key max min [WITHSCORES]](http://www.yiibai.com/redis/sorted_sets_zrevrangebyscore.html)			返回一个成员范围的有序集合，按分数，以分数排序从高分到低分|
|17|[ZREVRANK key member](http://www.yiibai.com/redis/sorted_sets_zrevrank.html)			确定一个有序集合成员的索引，以分数排序，从高分到低分|
|18|[ZSCORE key member](http://www.yiibai.com/redis/sorted_sets_zscore.html)			获取给定成员相关联的分数在一个有序集合|
|19|[ZUNIONSTORE destination numkeys key [key ...]](http://www.yiibai.com/redis/sorted_sets_zunionstore.html)			添加多个集排序，所得排序集合存储在一个新的键|
|20|[ZSCAN key cursor [MATCH pattern] [COUNT count]](http://www.yiibai.com/redis/sorted_sets_zscan.html)			增量迭代排序元素集和相关的分数|
参考链接：（[http://www.yiibai.com/redis/redis_sorted_sets.html](http://www.yiibai.com/redis/redis_sorted_sets.html)）
我们可以实现的事情：
1.排行榜
排行榜是非常常见的实现，比如收入排行榜，积分排行榜。在某种情况下，可以直接缓存整个排行榜，然后设置定时任务，在某个时间点更新。但对于一些实时性比较强的，需要及时更新数据，可以利用redis的有序队列实现，原理是利用有序队列的关联评分。
PS：如果要自己实现，可以考虑使用二叉堆实现，效率能到到**O(log(N))**
- 
`#假定有一个实时的竞技场评分，在玩家结束操作后打分，并动态显示玩家的排行榜`
- 
`> zadd rank 100 biki 87 zhibin 72 ming 64 fen 98 cat`
- 
`(integer) 5`
- 
`#显示得分最高的前三名玩家`
- 
`> ZREVRANGE rank 0 2 WITHSCORES`
- 
`1) "biki"`
- 
`2) 100.0`
- 
`3) "cat"`
- 
`4) 98.0`
- 
`5) "zhibin"`
- 
`6) 87.0`
- 
`#此时aaa玩家结束游戏得分为90`
- 
`>zadd rank 90 aaa`
- 
`#插入到有序队列，在进行查询`
- 
`> zadd rank 90 aaa`
- 
`(integer) 1`
- 
`> ZREVRANGE rank 0 2 WITHSCORES`
- 
`1) "biki"`
- 
`2) 100.0`
- 
`3) "cat"`
- 
`4) 98.0`
- 
`5) "aaa"`
- 
`6) 90.0`
排名已经更新了
