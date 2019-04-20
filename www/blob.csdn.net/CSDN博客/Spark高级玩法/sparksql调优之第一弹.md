# sparksql调优之第一弹 - Spark高级玩法 - CSDN博客
2018年01月18日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：4572
**1，jvm调优**
这个是扯不断，理还乱。建议能加内存就加内存，没事调啥JVM，你都不了解JVM和你的任务数据。
[spark调优系列之内存和GC调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483782&idx=1&sn=52059883b33cb97005186b0cba4e5f5a&chksm=9f38e2aea84f6bb8230189ee4e3de9cc7e11497f20f6cd498e540ef7f713478481c830106f51&scene=21#wechat_redirect)
**2，内存调优**
缓存表
spark2.+采用：
spark.catalog.cacheTable("tableName")缓存表，spark.catalog.uncacheTable("tableName")解除缓存。
spark 1.+采用：
采用 sqlContext.cacheTable("tableName")缓存，sqlContext.uncacheTable("tableName") 解除缓存
Sparksql仅仅会缓存必要的列，并且自动调整压缩算法来减少内存和GC压力。
|属性|默认值|介绍|
|----|----|----|
|spark.sql.inMemoryColumnarStorage.compressed|true|假如设置为true，SparkSql会根据统计信息自动的为每个列选择压缩方式进行压缩。|
|spark.sql.inMemoryColumnarStorage.batchSize|10000|控制列缓存的批量大小。批次大有助于改善内存使用和压缩，但是缓存数据会有OOM的风险|
**3，广播**
大小表进行join时，广播小表到所有的Worker节点，来提升性能是一个不错的选择。Spark提供了两个参数可以调整，不同版本会有些许不一样，本文以Spark2.2.1为例讲解。
|属性|默认值|描述|
|----|----|----|
|spark.sql.broadcastTimeout|300|广播等待超时时间，单位秒|
|spark.sql.autoBroadcastJoinThreshold|10485760 (10 MB)|最大广播表的大小。设置为-1可以禁止该功能。当前统计信息仅支持Hive Metastore表|
广播的变量的使用其实，有时候没啥用处。在任务超多，夸stage使用数据的时候才能凸显其真正作用。任务一趟跑完了，其实广播不广播无所谓了。。。
**4，分区数据的调控**
分区设置spark.sql.shuffle.partitions，默认是200.
对于有些公司来说，估计在用的时候会有Spark sql处理的数据比较少，然后资源也比较少，这时候这个shuffle分区数200就太大了，应该适当调小，来提升性能。
也有一些公司，估计在处理离线数据，数据量特别大，而且资源足，这时候shuffle分区数200，明显不够了，要适当调大。
适当，就完全靠经验。
**5，文件与分区**
这个总共有两个参数可以调整：
**一个是在读取文件的时候一个分区接受多少数据；**
**另一个是文件打开的开销，通俗理解就是小文件合并的阈值。**
文件打开是有开销的，开销的衡量，Spark 采用了一个比较好的方式就是打开文件的开销用，相同时间能扫描的数据的字节数来衡量。
参数介绍如下：
|属性名称|默认值|介绍|
|----|----|----|
|spark.sql.files.maxPartitionBytes|134217728 (128 MB)|打包传入一个分区的最大字节，在读取文件的时候。|
|spark.sql.files.openCostInBytes|4194304 (4 MB)|用相同时间内可以扫描的数据的大小来衡量打开一个文件的开销。当将多个文件写入同一个分区的时候该参数有用。该值设置大一点有好处，有小文件的分区会比大文件分区处理速度更快（优先调度）。|
spark.sql.files.maxPartitionBytes该值的调整要结合你想要的并发度及内存的大小来进行。
spark.sql.files.openCostInBytes说直白一些这个参数就是合并小文件的阈值，小于这个阈值的文件将会合并。
**6，文件格式**
建议parquet或者orc。Parquet已经可以达到很大的性能了。性能指标，网上一堆，在这里浪尖就不啰嗦了。
**7，sql调优**
听天由命吧。
**关于调优多说一句：**
对于Spark任务的调优，要深入了解的就是数据在整个spark计算链条中，在每个分区的分布情况。有了这点的了解，我们就会知道数据是否倾斜，在哪倾斜，然后在针对倾斜进行调优。
分区数该增大增大，该减少减少。
内存要尽可能大。
表别动不动就缓存，有时候重新加载比缓存速度都快。
该广播广播，不该广播的时候就别广播，就一个批次执行完的任务你广播毛线。
。。。。。
**Spark算子在使用的时候注意事项，容浪尖后续整理**。
**推荐阅读：**
1，[Spark调优系列之序列化方式调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483770&idx=1&sn=431258be59659cbe72a724984c5d3ccd&chksm=9f38e252a84f6b4446e94a5a4eede57933672a377364566519880ee7d9a1bc57c8a4c831e257&scene=21#wechat_redirect)
2，[spark调优系列之高层通用调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483790&idx=1&sn=8c9eac0e1c7f2633136a545f44be3d4e&chksm=9f38e2a6a84f6bb042b1e2fee9b3618e04fa2d7ca843e9cacb5d6bfc4afbc9fbdc78846efa9a&scene=21#wechat_redirect)
3，[Spark调优系列之硬件要求](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483786&idx=1&sn=a14c2853f16ff08e9d81eb51f0602741&chksm=9f38e2a2a84f6bb4027a1ff92108ee60a86f37de126ba50874e0f1422cd5ccab8d064f620a10&scene=21#wechat_redirect)
4，[spark调优系列之内存和GC调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483782&idx=1&sn=52059883b33cb97005186b0cba4e5f5a&chksm=9f38e2aea84f6bb8230189ee4e3de9cc7e11497f20f6cd498e540ef7f713478481c830106f51&scene=21#wechat_redirect)
![640.png?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png?)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg?)
**更多文章，敬请期待**
