# 转载和积累系列 - 大数据时代的 9 大Key-Value存储数据库 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年04月15日 11:33:57[initphp](https://me.csdn.net/initphp)阅读数：977








在过去的十年中，计算世界已经改变。现在不仅在大公司，甚至一些小公司也积累了TB量级的数据。各种规模的组织开始有了处理大数据的需求，而目前关系型数据库在可缩放方面几乎已经达到极限。 


一个解决方案是使用键值（Key-Value）存储数据库，这是一种NoSQL（非关系型数据库）模型，其数据按照键值对的形式进行组织、索引和存储。KV存储非常适合不涉及过多数据关系业务关系的业务数据，同时能有效减少读写磁盘的次数，比SQL数据库存储拥有更好的读写性能。 


本文就为你介绍9种用于大数据处理的免费键值存储数据库。




**1.  Aerospike 社区版**

****

![](https://img-blog.csdn.net/20150415112632495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




Aerospike是一个以分布式为核心基础，可基于行随机存取内存中索引、数据或SSD存储中数据的数据库。 

Aerospike主要用于广告业务，作为一个服务器端的cookie存储来使用，在这种场景下读取和写入性能是至关重要的。 

官网：[http://www.aerospike.com/press-releases/aerospike-launches-free-community-edition/](http://www.aerospike.com/press-releases/aerospike-launches-free-community-edition/)

相关资料：[Documentation](https://docs.aerospike.com/display/AS2/Aerospike+Documentation/)、[FAQ](https://docs.aerospike.com/display/AS2/FAQ)、[Blog](http://www.aerospike.com/aeroblog/)、[Forums](https://forums.aerospike.com/)





**2.  LevelDB**

****

![](https://img-blog.csdn.net/20150415112716331?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Leveldb是Google开发的一个非常高效的kv数据库，支持billion级别的数据量，在这个数量级别下还有着非常高的性能，主要归功于它的良好的设计，特别是LSM算法。Leveldb已经作为存储引擎被Riak和Kyoto
 Tycoon所支持，在国内淘宝的Tair开源key-value存储也已经将LevelDB作为其持久化存储引擎，并部署在线上使用。 

官网：[http://code.google.com/p/leveldb/](http://code.google.com/p/leveldb/)

相关资料：[Benchmarks](http://leveldb.googlecode.com/svn/trunk/doc/benchmark.html)、[Mailing
 List](https://groups.google.com/forum/?fromgroups)、[Twitter](https://twitter.com/LevelDB)

[](https://twitter.com/LevelDB)


**3.  Scalaris**

****

![](https://img-blog.csdn.net/20150415112837838?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Scalaris 是一个采用Erlang开发的分布式 key-value 存储系统，提供的 API 包括：Java、Python、Ruby和JSON。 

官网：[http://scalaris.googlecode.com/](http://scalaris.googlecode.com/)

相关资料：[Users
 and Developers Guide](https://code.google.com/p/scalaris/wiki/UsersDevelopersGuide)、[FAQ](https://code.google.com/p/scalaris/wiki/FAQ)、[Mailing
 List](https://groups.google.com/forum/?fromgroups)





**4.  Project Voldemort**

****

![](https://img-blog.csdn.net/20150415112838075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Voldemort是一个分布式键值存储系统，是Amazon's Dynamo的一个开源克隆。特性如下： 


- 支持自动复制数据到多个服务器上。
- 支持数据自动分割所以每个服务器只包含总数据的一个子集。
- 提供服务器故障透明处理功能。
- 支持可拨插的序化支持，以实现复杂的键-值存储，它能够很好的5.集成常用的序化框架如：Protocol Buffers、Thrift、Avro和Java Serialization。
- 数据项都被标识版本能够在发生故障时尽量保持数据的完整性而不会影响系统的可用性。
- 每个节点相互独立，互不影响。
- 支持可插拔的数据放置策略
官网：[http://project-voldemort.com/](http://project-voldemort.com/)

相关资料：[Wiki](https://github.com/voldemort/voldemort/wiki)、[Mailing
 List](https://groups.google.com/forum/?fromgroups)、[Github](https://github.com/voldemort/voldemort)、[Project
 Voldemort: Scaling Simple Storage](http://www.infoq.com/presentations/Project-Voldemort-Scaling-Simple-Storage)、[Serving
 Large-scale Batch Computed Data with Project Voldemort](http://static.usenix.org/events/fast12/tech/full_papers/Sumbaly.pdf)


[](http://static.usenix.org/events/fast12/tech/full_papers/Sumbaly.pdf)


**5.  Hyper****Dex**

****

![](https://img-blog.csdn.net/20150415112906686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

HyperDex是一个分布式、可搜索的键值存储系统，特性如下： 


- 分布式KV存储，系统性能能够随节点数目线性扩展
- 吞吐和延时都能秒杀现在风头正劲的MonogDB，吞吐甚至强于Redis
- 使用了hyperspace hashing技术，使得对存储的K-V的任意属性进行查询成为可能
官网：[http://hyperdex.org/](http://hyperdex.org/)

相关资料：[Documentation](http://hyperdex.org/doc/)、[Blog](http://hackingdistributed.com/)、[GitHub](https://github.com/rescrv/HyperDex)、[FAQ](http://hyperdex.org/FAQ/)、[Announcement
 Mailing List](https://groups.google.com/forum/?fromgroups)、[Discussion
 Mailing List](https://groups.google.com/forum/?fromgroups)


[](https://groups.google.com/forum/?fromgroups)


**6.  Berkeley DB**

****

![](https://img-blog.csdn.net/20150415112934766?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Berkeley DB是一个开源的文件数据库，介于关系数据库与内存数据库之间，使用方式与内存数据库类似，它提供的是一系列直接访问数据库的函数，而不是像关系数据库那样需要网络通讯、SQL解析等步骤。 

官网：[http://www.oracle.com/technetwork/products/berkeleydb/overview/index.html](http://www.oracle.com/technetwork/products/berkeleydb/overview/index.html)

相关资料：[Wiki](https://wikis.oracle.com/dashboard.action)、[Forums](https://forums.oracle.com/forums/main.jspa;jsessionid=8d92100c30d76261413ab9664fd79b25a158639b25f6.e34SbxmSbNyKai0Lc3mPbhmSc3mSe0?categoryID=84)、[Launchpad](https://launchpad.net/berkeley-db)





**7.  Apache Accumulo**

****

![](https://img-blog.csdn.net/20150415113103184?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Apache Accumulo 是一个可靠的、可伸缩的、高性能的排序分布式的 Key-Value 存储解决方案，基于单元访问控制以及可定制的服务器端处理。Accumulo使用 Google BigTable 设计思路，基于
 Apache Hadoop、Zookeeper 和 Thrift 构建。 

官网：[http://accumulo.apache.org/](http://accumulo.apache.org/)

相关资料：[Manual](http://accumulo.apache.org/1.4/user_manual/)、[Mailing
 Lists](http://accumulo.apache.org/mailing_list.html)、[Apache
 Accumulo Users Group](http://www.meetup.com/Accumulo-Users-DC/)





**8****.  Redis**

****

![](https://img-blog.csdn.net/20150415113042111?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Redis是一个高性能的key-value存储系统，和Memcached类似，它支持存储的value类型相对更多，包括string（字符串）、list（链表）、set（集合）和zset（有序集合）。与memcached一样，为了保证效率，数据都是缓存在内存中，区别的是Redis会周期性的把更新的数据写入磁盘或者把修改操作写入追加的记录文件，并且在此基础上实现了主从同步。 

Redis的出现，很大程度补偿了memcached这类key/value存储的不足，在部分场合可以对关系数据库起到很好的补充作用。它提供了Python、Ruby、Erlang、PHP客户端，使用很方便。 

官网：[http://redis.io/](http://redis.io/)

相关资料：[Documentation](http://redis.io/documentation)、[Tutorials](http://redis.io/documentation)、[FAQ](http://redis.io/topics/faq)、[Mailing
 List](http://groups.google.com/group/redis-db)、[Twitter](http://twitter.com/antirez)





**9.  Apache Cassandra**

****

![](https://img-blog.csdn.net/20150415113208454?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Apache Cassandra是一个混合型的非关系数据库，它最初由Facebook开发，用于储存特别大的数据。 

Cassandra的主要特点就是它不是一个数据库，而是由一堆数据库节点共同构成的一个分布式网络服务，对Cassandra 的一个写操作，会被复制到其它节点上去，对Cassandra的读操作，也会被路由到某个节点上面去读取。对于一个Cassandra群集来说，扩展性能是比较简单的事情，只管在群集里面添加节点就可以了。 

官网：[http://cassandra.apache.org/](http://cassandra.apache.org/)








来源：[http://www.iteye.com/news/27628](http://www.iteye.com/news/27628)




