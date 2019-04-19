# NoSql的易扩展性 - 左直拳的马桶_日用桶 - CSDN博客
2014年09月05日 17:24:39[左直拳](https://me.csdn.net/leftfist)阅读数：2524
NoSql现在很火很时髦，大家言必称NoSql，仿佛关系型数据库已成陈旧落后的代名词。
但依我看，真正理解NoSql的还不多，在实际项目中用过的应该就更少了。
我也还不理解，更没怎么应用过，所以现在要努力学习。
在学习过程中，常看到有吹嘘NoSql相比较关系型数据库而言，有一个优点是：易扩展。
这怎么理解呢？绝大部分的资料都没怎么讲，一味唾沫横飞地说：易扩展、高性能，高可用。。。
我看了一点MongoDB官网的文档，再结合自己的理解，解读如下：
**NoSql的易扩展性：**
关系型数据库的表之间，具有关联性，查询的时候往往需要join，因此所有的表需要存放在同一台服务器里。所以有人说（淘宝的数据库专家），目前的关系型数据库本质上是一个单机系统，而不是分布式的。
但NoSql就不同了：
1、表之间并没有什么关系（？），放哪都一样，特别适合分布式存储。
2、根据CAP理论，分布式系统中，一致性，可用性，分区容错性三者至多只能满足其二，NoSql牺牲了一致性，各个副本之间，仅追求的是最终一致性，这样也有利于分布式存储。
所以说它有好的扩展性（横向扩展）。
- Easy Scalability- Automatic [sharding](http://docs.mongodb.org/manual/core/sharded-clusters/) distributes collection data across machines.
- Eventually-consistent reads can be distributed over replicated servers.
