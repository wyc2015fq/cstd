# Spark SQL的几个里程碑！ - Spark高级玩法 - CSDN博客
2018年11月12日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：436
本文讲讲Spark SQL的几个里程碑的更新升级。
**1. spark 1.0.0诞生了Spark SQL**
官方版本是spark 1.0.0引入的Spark SQL模块。当时这个模块的核心实际上就是一种新类型的RDD，叫做SchemaRDD。SchemaRDD就是类型为ROW的RDD，但同时又包含了一个描述每一列数据类型的schema信息。SchemRDD也可类似于传统数据库的一张表。SchemaRDD可以从已有的RDD创建，可以是Parquet文件，json数据集或则HiveQL生成。该版本引入是在2014年五月30日。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVvRRHZ0jLDYwW9nRVQmTafibZmHXAHTcRktvBLtZSeMGIdqBB0h5an7ERUlo799A1aQnaovCBpriaQ/640)
**2. Spark 1.2.0诞生了ML机器学习库**
Ml机器学习库是基于SchemaRDD的，后来的版本是基于Dataframe的，可以直接与Spark SQL进行交互。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVvRRHZ0jLDYwW9nRVQmTafM8jZa9rcFEVm4AmnXZSZjANMzAlxo3xep2ia9FIMicRkgp223dGLQ6pQ/640)
**3. Spark 1.3.0 诞生了Dataframe**
Spark 1.3的时候做了一个重大变革。就是将SchemaRDD重命名为了DataFrame，主要原因是DataFrame不再直接继承自RDD，而是自己维护和实现了自己的功能函数。但是DataFrame可以通过调用 .rdd 转化为RDD。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVvRRHZ0jLDYwW9nRVQmTafCJVEkmVZ3JfefTJpHT8opALYtGz76VicJ96fA1fK2kQWIGFf7ULasibQ/640)
**4. spark 1.6.0诞生了Dataset和SparkSession**
Spark 1.6的时候也是有了重大调整，增加了Dataset的概念,类似RDD，在享受Spark SQL执行引擎性能优化的同时允许用户使用自定义对象和lambda函数。
在引入Dataset的同时，也引入了SparkSession，也即是会话管理功能，允许不同用户可以在使用不同配置和临时表的情况下共享统一的集群。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVvRRHZ0jLDYwW9nRVQmTafcCf68KohTcExI5iacpgHe0FjjujgLNXWdY95LD5Yrc0OgMW9U8Xl5iaA/640)
**5. Spark 2.0.0诞生了Strcutured Streaming**
Spark 2.0开始，Dataset API和Dataframe API统一了。Scala版本，DataFrame被类型定义成Dataset[Row]，java版本必须是要Dataset[Row]代替Dataframe。
SparkSession已经完全替换掉了旧的SQLContext和HiveContext。SQLContext和HiveContext为了保持兼容还在被保留。
上线了Structured Streaming。这个是Spark 流处理发展的主要方向，底层是基于Spark SQL 和 Catalyst 优化器，让用户像使用静态Dataset开发离线处理任务一样使用流Dataset开发流处理业务，这个就是依赖于Catalyst 优化器自动增量的查询计划。
从自Spark 2.x依赖的更新状态来看，Spark SQL及Catalyst 优化器已经成为Spark框架努力的方向，主要体现在：
1). 逐步废弃掉基于RDD的mllib机器学习库，着重发展基于DataFrame的ml库。目前是，基于RDD的机器学习库处于保留状态，后期会废弃。
2). Spark2.4.0 未对Spark Streaming(RDD-based)做进一步更新。Structured Streaming(dataframe-based)被大力优化更新，也有取代Spark Streaming之势头。
所以，spark 使用及爱好者要大力掌握好Spark SQL和Structured Streaming。
**那么是不是就不要深入学习Spark Core和Spark Streaming了呢？**
答案是否定的！
Spark Core是Spark SQL的基石，所以很有必要掌握好Spark Core。
Spark SQL和Structured Streaming处理的是结构化数据，非结构化数据，还是需要Spark Core和Spark Streaming进行解析处理。
Structured Streaming 的功能还不够完善，限制颇多，比如多流join之后不能聚合等，所以Spark Streaming的给用户以灵活处理的接口还是有用武之地的。
推荐阅读：
[大数据啊大数据！](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485810&idx=1&sn=178ce450fef932d27026dc5e2951ac3a&chksm=9f38ea5aa84f634c91b518671e135a4761e9d0a0a0912488944653aac2baf66f83579540078b&scene=21#wechat_redirect)
[如何成为一个优秀的工程师？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485792&idx=1&sn=89df7bad8652930a01256fd4acce02ea&chksm=9f38ea48a84f635edcd52427da14466bc0224cce3c025206c616b15cb02c3fb94eb0bcdc8f49&scene=21#wechat_redirect)
[解惑:这个SPARK任务是数据倾斜了吗？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485791&idx=1&sn=52ab8a2ab64c82f1c70287d0a69bf604&chksm=9f38ea77a84f636155f724ad9a94486ede70ffbc3ff4f3de4a99cb8092a797a9e2fa91df707b&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
感谢点赞，转发！
