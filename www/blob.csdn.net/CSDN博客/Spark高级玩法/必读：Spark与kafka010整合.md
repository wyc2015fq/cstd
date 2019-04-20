# 必读：Spark与kafka010整合 - Spark高级玩法 - CSDN博客
2018年03月22日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1886
SparkStreaming与kafka010整合
读本文之前，请先阅读之前文章：
[必读：再讲Spark与kafka 0.8.2.1+整合](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484551&idx=1&sn=ee51a406c1fa975489b7f9758a9e8d2c&chksm=9f38e7afa84f6eb934bc8bd09b106db456b4146a663d4bff759c0e2f9c0a03f30c7479ece62b&scene=21#wechat_redirect)
Spark Streaming与kafka 0.10的整合，和0.8版本的direct Stream方式很像。Kafka的分区和spark的分区是一一对应的，可以获取offsets和元数据。API使用起来没有显著的区别。这个整合版本标记为experimental，所以API有可能改变。
工程依赖
首先，添加依赖。
groupId = org.apache.spark
artifactId = spark-streaming-kafka-0-10_2.11
version = 2.2.1
不要手动添加org.apache.kafka相关的依赖，如kafka-clients。spark-streaming-kafka-0-10已经包含相关的依赖了，不同的版本会有不同程度的不兼容。
**代码案例**
首先导入包正确的包org.apache.spark.streaming.kafka010
import org.apache.kafka.clients.consumer.ConsumerRecord
import org.apache.kafka.common.serialization.StringDeserializer
import org.apache.spark.streaming.kafka010._
import org.apache.spark.streaming.kafka010.LocationStrategies.PreferConsistent
import org.apache.spark.streaming.kafka010.ConsumerStrategies.Subscribe
ssc = new StreamingContext(sparkConf, Milliseconds(1000))
val preferredHosts = LocationStrategies.PreferConsistent
val kafkaParams = Map[String, Object](
"bootstrap.servers" -> "localhost:9092,anotherhost:9092",
"key.deserializer" -> classOf[StringDeserializer],
"value.deserializer" -> classOf[StringDeserializer],
"group.id" -> "use_a_separate_group_id_for_each_stream",
"auto.offset.reset" -> "latest",
"enable.auto.commit" -> (false: java.lang.Boolean)
)
val topics = Array("topicA", "topicB")
val stream = KafkaUtils.createDirectStream[String, String](
ssc,
preferredHosts,
Subscribe[String, String](topics, kafkaParams)
)
stream.map(record => (record.key, record.value))
kafka的参数，请参考kafka官网。如果，你的spark批次时间超过了kafka的心跳时间(30s)，需要增加heartbeat.interval.ms和session.timeout.ms。例如，批处理时间是5min，那么就需要调整group.max.session.timeout.ms。注意，例子中是将enable.auto.commit设置为了false。
**LocationStrategies(本地策略)**
新版本的消费者API会预取消息入buffer。因此，为了提升性能，在Executor端缓存消费者(而不是每个批次重新创建)是非常有必要的，优先调度那些分区到已经有了合适消费者主机上。
在很多情况下，你需要像上文一样使用LocationStrategies.PreferConsistent，这个参数会将分区尽量均匀地分配到所有的可以Executor上去。如果，你的Executor和kafka broker在同一台机器上，可以用PreferBrokers，这将优先将分区调度到kafka分区leader所在的主机上。最后，分区间负荷有明显的倾斜，可以用PreferFixed。这个允许你指定一个明确的分区到主机的映射（没有指定的分区将会使用连续的地址）。
消费者缓存的数目默认最大值是64。如果你希望处理超过（64*excutor数目）kafka分区，spark.streaming.kafka.consumer.cache.maxCapacity这个参数可以帮助你修改这个值。
如果你想禁止kafka消费者缓存，可以将spark.streaming.kafka.consumer.cache.enabled修改为false。禁止缓存缓存可能需要解决SPARK-19185描述的问题。一旦这个bug解决，这个属性将会在后期的spark版本中移除。
Cache是按照topicpartition和groupid进行分组的，所以每次调用creaDirectStream的时候要单独设置group.id。
**ConsumerStrategies(消费策略)**
新的kafka消费者api有多个不同的方法去指定消费者，其中有些方法需要考虑post-object-instantiation设置。ConsumerStrategies提供了一个抽象，它允许spark能够获得正确配置的消费者，即使从Checkpoint重启之后。
ConsumerStrategies.Subscribe，如上面展示的一样，允许你订阅一组固定的集合的主题。SubscribePattern允许你使用正则来指定自己感兴趣的主题。注意，跟0.8整合不同的是，使用subscribe或者subscribepattern在运行stream期间应对应到添加分区。其实，Assign运行你指定固定分区的集合。这三种策略都有重载构造函数，允许您指定特定分区的起始偏移量。
ConsumerStrategy是一个public类，允许你进行自定义策略。
**创建kafkaRDD**
类似于spark streaming的批处理，现在你可以通过指定自定义偏移范围自己创建kafkaRDD。
def getKafkaParams(extra: (String, Object)*): JHashMap[String, Object] = {
val kp = new JHashMap[String, Object]()
  kp.put("bootstrap.servers", kafkaTestUtils.brokerAddress)
  kp.put("key.deserializer", classOf[StringDeserializer])
  kp.put("value.deserializer", classOf[StringDeserializer])
  kp.put("group.id", s"test-consumer-${Random.nextInt}-${System.currentTimeMillis}")
  extra.foreach(e => kp.put(e._1, e._2))
  kp
}
val kafkaParams = getKafkaParams("auto.offset.reset" -> "earliest")
// Import dependencies and create kafka params as in Create Direct Stream above
val offsetRanges = Array(
// topic, partition, inclusive starting offset, exclusive ending offset
OffsetRange("test", 0, 0, 100),
OffsetRange("test", 1, 0, 100)
)
val rdd = KafkaUtils.createRDD[String, String](sparkContext, kafkaParams, offsetRanges, PreferConsistent)
注意，在这里是不能使用PreferBrokers的，因为不是流处理的话就没有driver端的消费者帮助你寻找元数据。必须使用PreferFixed,然后自己指定元数据
大家可以进入createRDD里面，看其源码，其实就是根据你的参数封装成了RDD，跟流式批处理是一致的。
def createRDD[K, V](
    sc: SparkContext,
kafkaParams: ju.Map[String, Object],
offsetRanges: Array[OffsetRange],
locationStrategy: LocationStrategy
  ): RDD[ConsumerRecord[K, V]] = {
val preferredHosts = locationStrategy match {
case PreferBrokers =>
throw new AssertionError(
"If you want to prefer brokers, you must provide a mapping using PreferFixed " +
"A single KafkaRDD does not have a driver consumer and cannot look up brokers for you.")
case PreferConsistent => ju.Collections.emptyMap[TopicPartition, String]()
case PreferFixed(hostMap) => hostMap
  }
val kp = new ju.HashMap[String, Object](kafkaParams)
fixKafkaParams(kp)
val osr = offsetRanges.clone()
new KafkaRDD[K, V](sc, kp, osr, preferredHosts, true)
}
**获取偏移**
Spark Streaming与kafka整合是运行你获取其消费的偏移的，具体方法如下：
stream.foreachRDD { rdd =>
val offsetRanges = rdd.asInstanceOf[HasOffsetRanges].offsetRanges
  rdd.foreachPartition { iter =>
val o: OffsetRange = offsetRanges(TaskContext.get.partitionId)
println(s"${o.topic} ${o.partition} ${o.fromOffset} ${o.untilOffset}")
  }
}
注意，HashOffsetRanges仅仅在spark计算链条的开始才能类型转换成功。要知道kafka分区和spark分区的一一对应关系在Shuffle后就会丧失，比如reduceByKey()或者window()。
**存储偏移**
Kafka在有可能存在任务失败的情况下的从消息传输语义（至少一次，最多一次，恰好一次）是取决于何时存储offset。Spark输出操作是至少一次传输语义。所以，如果你想实现仅仅一次的消费语义，你必须要么在密等输出后存储offset，要么就是offset的存储和结果输出是一次事务。
现在kafka有了3种方式，来提高可靠性（以及代码复杂性），用于存储偏移量。
**1， Checkpoint**
如果使能了Checkpoint，offset被存储到Checkpoint。这个虽然很容易做到，但是也有一些缺点。由于会多次输出结果，所以结果输出必须是满足幂等性。同时事务性不可选。另外，如果代码变更，你是不可以从Checkpoint恢复的。针对代码升级更新操作，你可以同时运行你的新任务和旧任务（因为你的输出结果是幂等性）。对于以外的故障，并且同时代码变更了，肯定会丢失数据的，除非另有方式来识别启动消费的偏移。
**2， Kafka自身**
Kafka提供的有api，可以将offset提交到指定的kafkatopic。默认情况下，新的消费者会周期性的自动提交offset到kafka。但是有些情况下，这也会有些问题，因为消息可能已经被消费者从kafka拉去出来，但是spark还没处理，这种情况下会导致一些错误。这也是为什么例子中stream将enable.auto.commit设置为了false。然而在已经提交spark输出结果之后，你可以手动提交偏移到kafka。相对于Checkpoint，offset存储到kafka的好处是：kafka既是一个容错的存储系统，也是可以避免代码变更带来的麻烦。提交offset到kafka和结果输出也不是一次事务，所以也要求你的输出结果是满足幂等性。
stream.foreachRDD { rdd =>
val offsetRanges = rdd.asInstanceOf[HasOffsetRanges].offsetRanges
// some time later, after outputs have completed
stream.asInstanceOf[CanCommitOffsets].commitAsync(offsetRanges)
}
由于带有HasOffsetRanges，到CanCommitOffsets的转换将会在刚执行createDirectStream之后成功，而不是经过各种操作算子后。commitAsync是线程安全的，必须在结果提交后进行执行。
**3， 自定义存储位置**
对于输出解雇支持事务的情况，可以将offset和输出结果在同一个事务内部提交，这样即使在失败的情况下也可以保证两者同步。如果您关心检测重复或跳过的偏移范围，回滚事务可以防止重复或丢失的消息。这相当于一次语义。也可以使用这种策略，甚至是聚合所产生的输出，聚合产生的输出通常是很难生成幂等的。代码示例
// The details depend on your data store, but the general idea looks like this
// begin from the the offsets committed to the database
val fromOffsets = selectOffsetsFromYourDatabase.map { resultSet =>
new TopicPartition(resultSet.string("topic"), resultSet.int("partition")) -> resultSet.long("offset")
}.toMap
val stream = KafkaUtils.createDirectStream[String, String](
  streamingContext,
PreferConsistent,
Assign[String, String](fromOffsets.keys.toList, kafkaParams, fromOffsets)
)
stream.foreachRDD { rdd =>
val offsetRanges = rdd.asInstanceOf[HasOffsetRanges].offsetRanges
val results = yourCalculation(rdd)
// begin your transaction
  // update results
  // update offsets where the end of existing offsets matches the beginning of this batch of offsets
  // assert that offsets were updated correctly
  // end your transaction
}
**SSL/TLS配置使用**
新的kafka消费者支持SSL。只需要在执行createDirectStream / createRDD之前设置kafkaParams。注意，这仅仅应用与Spark和kafkabroker之间的通讯；仍然负责分别确保节点间通信的安全。
val kafkaParams = Map[String, Object](
// the usual params, make sure to change the port in bootstrap.servers if 9092 is not TLS
"security.protocol" -> "SSL",
"ssl.truststore.location" -> "/some-directory/kafka.client.truststore.jks",
"ssl.truststore.password" -> "test1234",
"ssl.keystore.location" -> "/some-directory/kafka.client.keystore.jks",
"ssl.keystore.password" -> "test1234",
"ssl.key.password" -> "test1234"
)
Spark相关书籍，请进入浪尖微店。
**推荐阅读：**
1，[Hdfs的数据磁盘大小不均衡如何处理](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484536&idx=1&sn=1ae5ea300e0477273e6923cab2861425&chksm=9f38e750a84f6e46e54790ef6f4f777fb295998506fd6963fc484589081e1bfaf377342485d8&scene=21#wechat_redirect)
2，[数据科学的工作流程](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484519&idx=1&sn=7f57e0996d1a7c973c964c42bd9fdd98&chksm=9f38e74fa84f6e594cfbfc9de971e2b36a4df0ccb4bcdea2f1957fa505fc6f699accd89bfc48&scene=21#wechat_redirect)
3，[大数据基础系列之spark的监控体系介绍](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483792&idx=1&sn=2fe2364e08fec3fd9d57d2f5d1b99e2b&chksm=9f38e2b8a84f6baee19e8d430e522389cf8df33820d95dbfa4f0e4ef63f7661ca3bf41e73d7f&scene=21#wechat_redirect)
4，[金融反欺诈场景下的Spark实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484078&idx=1&sn=6740b1da86f8bda4157178ff9ec9d7db&chksm=9f38e186a84f689092aad7e9023fdca856abdada21b2a417870a075899384916f3809c3563be&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
