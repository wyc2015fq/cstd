# mongodb TTL collections - aisoo的专栏 - CSDN博客
2013年02月26日 10:58:05[aisoo](https://me.csdn.net/aisoo)阅读数：2191
从2.2开始，mongodb又引入TTL(time to live),之前有capped collections，特性已经介绍过，这里先了解一下TTL的特性，就是可以在建立索引的时候指定多久后删除对应的记录，建立索引的字段必须是date类型，这样我就更加灵活的控制过期数据。
条件限制：
1.创建索引的字段必须是一个date BSON type，如果不是时间字段，那么不会过期
2.不能再_id字段上创建，如果一个字段有索引，那么不能创建
3.TTL不支持复合索引
4.capped collection不可以使用TTL，因为capped collection不支持remove操作
5.如果一个字段是数组，而且里面有多个数据类型，那么TTL将会按最早的匹配过期阀值。
处理这个操作的后台任务一分钟运行一次
语法：db.collection.ensureIndex( { "date_field": 1 }, { expireAfterSeconds: 3600 } )
这里我设置了1小时，如果设置小于一分钟，由于后台处理进程是每分钟执行一次，所以不要设置小于1分钟，毫无意义
如果是replica sets那么只能在主节点运行，所有的删除操作会同步到其他节点
另外这里提一下，2.2以后mongodb增强了并发操作，大家知道以前mongodb是全局锁，也就是整个server的锁，现在细化到db级别
2.2之前的版本统计聚合操作都是用mapreduce，现在可以用[Aggregation Framework](http://docs.mongodb.org/manual/applications/aggregation/) ，这个里面包含了很多的函数可以实现我们需要的结果
具体详见[http://docs.mongodb.org/manual/applications/aggregation/](http://docs.mongodb.org/manual/applications/aggregation/)
总之2.2还是给大家带来不少惊喜，可以参阅[http://docs.mongodb.org/manual/release-notes/2.2/](http://docs.mongodb.org/manual/release-notes/2.2/)
TTL参阅[http://docs.mongodb.org/manual/tutorial/expire-data/](http://docs.mongodb.org/manual/tutorial/expire-data/)
另外sharding加入的打标签的功能，也就是可以控制某个范围能的数据指定到一个分片节点上，可参阅[http://www.mongodb.org/display/DOCS/Tag+Aware+Sharding](http://www.mongodb.org/display/DOCS/Tag+Aware+Sharding)
