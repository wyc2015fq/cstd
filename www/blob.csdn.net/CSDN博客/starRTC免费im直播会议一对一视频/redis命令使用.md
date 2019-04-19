# redis命令使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年01月29日 17:18:00[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：168
set key value
get key
删除key (返回被移除key的数量。):del
key
检查给定key是否存在(若key存在，返回1，否则返回0。):exists key
> expire key 5(integer) 1> get key (immediately)"some-value"> get key (after some
 time)(nil)
使用EXPIRE来设置超时时间(也可以再次调用这个命令来改变超时时间，使用PERSIST命令去除超时时间
 )。我们也可以在创建值的时候设置超时时间:
> set key 100 ex 10OK>
 ttl key(integer) 9
TTL命令用来查看key对应的值剩余存活时间。封禁用户可以用
list:
[LPUSH](http://www.redis.cn/commands/lpush.html) 命令可向list的左边（头部）添加一个新元素，而[RPUSH](http://www.redis.cn/commands/rpush.html)命令可向list的右边（尾部）添加一个新元素。最后[LRANGE](http://www.redis.cn/commands/lrange.html)命令可从list中取出一定范围的元素:
还有一个重要的命令是pop,它从list中删除元素并同时返回删除的值。可以在左边或右边操作。

> AUTH mypass
[http://www.redis.cn/topics/data-types-intro.html#strings](http://www.redis.cn/topics/data-types-intro.html#strings)
[http://redisdoc.com/](http://redisdoc.com/)
[https://github.com/phpredis/phpredis#exists](https://github.com/phpredis/phpredis#exists)
