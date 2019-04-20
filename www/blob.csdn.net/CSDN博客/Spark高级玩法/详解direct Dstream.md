# 详解direct Dstream - Spark高级玩法 - CSDN博客
2018年09月20日 07:01:50[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：205
**前言**
前面，有分享过基于receiver的，实际上，看到receiver based Dstream大家就对阅读提不起兴趣了，实际上这是错误的，基于receiver的才是spark streaming根本，虽然direct stream才更合适。但是，我们从基于receiver可以学到很多内容，最重要的spark streaming实现原理，数据本地性等。详细可以再去阅读一下：
[根本:详解receiver based Dstream](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485412&idx=1&sn=712f61b7aea0afa2a85fd8c2010e4e6c&chksm=9f38e4cca84f6dda3ac24e8bf3248d2b8b89315962fa71aaf13c2e98a436e10333c557b24404&scene=21#wechat_redirect)
**direct dstream运行架构图**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU8FeRab6Tibl3tJibGlfkBAR3q2YnoxoY8Cb3yn1Vomzr0dVJ67KTCzKpoXASJHKVtwvF9XkA1ZZmw/640)
**对比**
对比receiver based的Dstream和direct Dstream
**a **无需启动receiver，减少不必要的cpu占用
** b** 减少了receiver接收数据，写入blockmanager，然后运行时再通过blockid，网络传输，磁盘读区，来获取数据这个过程。提升了效率。
** c **无需wal，进一步减少磁盘读写。
**d **可以通过手动维护offset来实现精确的一次消费。
**e **Dstream中生成的RDD，并不是blockrdd，而是kafkardd，kafkardd是和kafka分区一一对应的，更便于我们把控并行度。
**  f **数据本地性的问题，导致receiver存在的机器会运行过多的任务，会导致有些executor空闲。
而kafkardd，在compute函数里，会使用simpleconsumer，根据指定的topic，分区，offset范围，去kafka读取数据。010版本以后，又存在假如kafka和spark运行于同一集群，会有数据本性的概念。
**数据本地性**
spark streaming与kafka 082结合生成的rdd，数据本地性计算方式如下：
override def getPreferredLocations(thePart: Partition): Seq[String] = {
val part = thePart.asInstanceOf[KafkaRDDPartition]
// TODO is additional hostname resolution necessary here
Seq(part.host)
}
spark streaming 与kafka 010结合生成的rdd，数据本地性计算方式如下：
override def getPreferredLocations(thePart: Partition): Seq[String] = {
// The intention is best-effort consistent executor for a given topicpartition,
  // so that caching consumers can be effective.
  // TODO what about hosts specified by ip vs name
val part = thePart.asInstanceOf[KafkaRDDPartition]
val allExecs = executors()
val tp = part.topicPartition
val prefHost = preferredHosts.get(tp)
val prefExecs = if (null == prefHost) allExecs else allExecs.filter(_.host == prefHost)
val execs = if (prefExecs.isEmpty) allExecs else prefExecs
if (execs.isEmpty) {
Seq.empty
  } else {
// execs is sorted, tp.hashCode depends only on topic and partition, so consistent index
val index = Math.floorMod(tp.hashCode, execs.length)
val chosen = execs(index)
Seq(chosen.toString)
  }
}
对于 与kafka010结合的注意事项，实际上以前浪尖也翻译过一篇文章。
[必读：Spark与kafka010整合](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484604&idx=1&sn=c7fb09731502041562894ad9ef06a8ca&chksm=9f38e794a84f6e82dddf1b6c0b081f7f3e0512b85557e7165f87c443ca804f7af6e11846cffa&scene=21#wechat_redirect)
**限速**
限速，很多人使用姿势不对，详细的原理可以参看
[Spark的PIDController源码赏析及backpressure详解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485119&idx=1&sn=8935368a8e39a6cb4895597104f93b91&chksm=9f38e597a84f6c81c5d54e034821c259af8808f82b5ca29d5e7b8801277132b316d5a97337c5&scene=21#wechat_redirect)
具体配置参数详解，可以参考：
- 
spark.streaming.backpressure.enabled
默认是false，设置为true，就开启了背压机制。
- 
spark.streaming.backpressure.initialRate
默认没设置，初始速率。第一次启动的时候每个receiver接受数据的最大值。
- 
spark.streaming.receiver.maxRate
默认值没设置。每个接收器将接收数据的最大速率（每秒记录数）。 
实际上，每个流每秒最多将消费此数量的记录。 将此配置设置为0或负数将不会对速率进行限制。
- 
spark.streaming.kafka.maxRatePerPartition
使用新Kafka direct API时从每个Kafka分区读取数据的最大速率（每秒记录数）。
详细的视频讲解，可以观看：
更多资料，可以加入知识星球获取
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640)
