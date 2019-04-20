# 必读：再讲Spark与kafka 0.8.2.1+整合 - Spark高级玩法 - CSDN博客
2018年03月17日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1412
Kafka在0.8和0.10版本引入了新的消费者API，所以sparkStreaming与kafka的整合提供了两个包。  请根据你的集群选用正确的包。注意， 0.8和后期的版本0.9及0.10是兼容的，但是0.10整合是不兼容之前的版本的。
包与版本特性之间的对应关系如下：
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVYk5bdDTpE1ibEQo6B6Dg6HqBwNRM2A7ZKl6Wsl9iaxKdiaunuauSzUoxDoOrNIBZA8U2DCsV4lSP6A/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
本文主要讲述sparkStreaming与kafka 0.8.2.1+版本整合，要求kafka集群的版本是0.8.2.1或者更高版本。
**基于Receiver的方式**
这种方式使用一个Receiver来接受数据。Receiver是使用kafka的高级消费者API来实现的。所有的Receiver从kafka里面接受数据，然后存储于Executors，sparkStreaming再生成任务来处理数据。
然而，默认配置的情况，这种方式在失败的情况下有可能丢失数据，为了确保零数据丢失，可以配置预写日志(WAL，从spark1.2引入)。这会将Receiver接收到的数据写入分布式文件系统，如hdfs，所以所有的数据可以在从失败恢复运行的时候加载到。
**导包(MVN或者sbt)：**
groupId = org.apache.spark
 artifactId = spark-streaming-kafka-0-8_2.11
 version = 2.2.1
**测试代码如下：**
val sparkConf = new SparkConf().setAppName("KafkaWordCount")
val ssc = new StreamingContext(sparkConf, Seconds(2))
ssc.checkpoint("checkpoint")
val topics = "topic1,topic2 1"
val topicMap = topics.split(",").map((_, numThreads.toInt)).toMap
val lines = KafkaUtils.createStream(ssc, zkQuorum, group, topicMap).map(_._2)
val words = lines.flatMap(_.split(" "))
val wordCounts = words.map(x => (x, 1L))
  .reduceByKeyAndWindow(_ + _, _ - _, Minutes(10), Seconds(2), 2)
wordCounts.print()
ssc.start()
ssc.awaitTermination()
注意事项：
1，打包的时候spark-streaming-kafka-0-8对应的jar包一定要带上。
2，消费的kafka分区和生成的RDD分区并不是一一对应的。所以，增加KafkaUtils.createStream()命令中topic指定的分区，也即map里面topic名字对应的value，只会增加消费该命令创建的Receiver的内部消费者线程数目，不会增加spark处理数据的并行度，恰当线程数会增加Receiver的接收数据的速度。
3，KafkaUtils.createStream()命令执行只会创建一个Receiver，我们可以结合消费的topic分区和group名称来多创建几个Receiver，来增加接收数据的并行度。
4，如果你启动了预写日志，日志存储系统时hdfs，日志已经会被存储副本。所以，可以设置存储等级为StorageLevel.MEMORY_AND_DISK_SER.
5，要配置该机制，首先要调用 StreamingContext 的 checkpoint ( ) 方法设置一个 checkpoint 目录，然后需要将 spark.streaming.receiver.writeAheadLog.enable 参数设置为 true。
**Direct Approach**
在spark 1.3以后引入了一种新的sparkStreamingapi，新的api回自己在driver内部维护一个偏移，然后自动计算指定的topic+partition该批次需要拉去数据的范围，然后从kafka拉去数据来计算。不同于基于Receiver的方式，direct模式不会将偏移记录到Zookeeper，以保证故障恢复从上次偏移处消费消息。Direct模式你可以通过Checkpoint或者自己编写工具来实现偏移的维护，保证数据消费不丢失。
这种方式相比于基于Receiver的方式有以下优势：
1， **简化并行度：**不需要创建多个kafkastream，然后union他们。使用directStream，sparkstreaming生成的RDD分区和kafka的分区是一一对应的，这种方式理解起来更简单而且便于调优。
2， **高效：**基于Receiver的方式要保证数据不丢失，必须启用预写日志。这个行为实际上是非常抵消的，数据会被复制两次，一次是kafka集群，一次是预写日志。Direct方式解决了这个问题，由于没有Receiver，故而也不需要预写日志。只要你kafka里面存有数据，那么消息就可以从kafka里面恢复。
3， **仅一次消费语义：**基于Receiver的会把偏移提交到Zookeeper。这种方式结合预写日志能保证数据不丢失，也即是最少一次消费语义，但是有几率导致消费者在存在失败的情况下消费消息两次。比如，消息处理并经过存储之后，但是偏移并没有提交到Zookeeper，这个时候发生故障了，那么恢复之后，就会按照Zookeeper上的偏移再一次消费数据并处理，导致消息重复处理。但是direct方式偏移不会提交到Zookeeper，是sparkstreaming在driver使用内存变量加Checkpoint进行追踪的，所以尽管会存在任务失败，但是仍然能保证消费的一次处理。
注意，由于direct方式不会提交偏移到Zookeeper，所以，基于Zookeeper的kafka监控工具就不能监控到sparkstreaming的消费情况。然而，你可以自己讲偏移提交道Zookeeper，来满足你的需求。
**导包(MVN或者sbt)：**
groupId = org.apache.spark
 artifactId = spark-streaming-kafka-0-8_2.11
 version = 2.2.1
**测试代码如下：**
val Array(brokers, topics) = args
// Create context with 2 second batch interval
val sparkConf = new SparkConf().setAppName("DirectKafkaWordCount")
val ssc = new StreamingContext(sparkConf, Seconds(2))
// Create direct kafka stream with brokers and topics
val topicsSet = topics.split(",").toSet
val kafkaParams = Map[String, String]("metadata.broker.list" -> brokers)
val messages = KafkaUtils.createDirectStream[String, String, StringDecoder, StringDecoder](
  ssc, kafkaParams, topicsSet)
// Get the lines, split them into words, count the words and print
val lines = messages.map(_._2)
val words = lines.flatMap(_.split(" "))
val wordCounts = words.map(x => (x, 1L)).reduceByKey(_ + _)
wordCounts.print()
// Start the computation
ssc.start()
ssc.awaitTermination()
关于自己编代码提交到Zookeeper，限于篇幅的原因，不在这里啰嗦。
**调优限速**
现实系统中会有流量尖峰，比如淘宝的双十一，那一秒钟的流量，大的吓人，假如有sparkstreaming处理的话，会有可能导致消息不能及时处理，甚至出现故障，应对这种流量尖峰，sparkstreaming内部实现了一个控制器，基于PID，具体PID的概念是啥，请自行百度。
这里只是想介绍两个主要的参数：
基于Receiver的要配置的参数是spark.streaming.receiver.maxRate
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVYk5bdDTpE1ibEQo6B6Dg6HnvaWODdxtWG58LlhicHypxN4JHjomxNSibjWL2BMibmesqypxDdhgia3XQ/640?wx_fmt=png)
基于direct的要配置的参数是spark.streaming.kafka.maxRatePerPartition
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVYk5bdDTpE1ibEQo6B6Dg6H6A3Moic29ffz1mLIEvSZcE6I0gOBZicaL7zepejUvutR8M9d6ZNoqh9g/640?wx_fmt=png)
通过我们压测我们的sparkstreaming任务每秒钟最大消费处理的消息数，然后使用这两个参数限消费消息的速率，来避免高峰期一批次消费过量消息导致应用不正常执行。
**推荐阅读：**
1，[论Spark Streaming的数据可靠性和一致性](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484247&idx=1&sn=f9a65c44ea7186012d72dee25526ec77&chksm=9f38e07fa84f69699c3599dbf06753b7b4b99148fc076b1ea6896aef466731c012a4831a4607&scene=21#wechat_redirect)
2，[Spark Structured Streaming高级特性](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483938&idx=1&sn=0350e49c3f86f0ec2e196dfdf9506e22&chksm=9f38e10aa84f681c06d069690b8ddd949432d8e46817e5a6ff366ac128139b935b800238a9ad&scene=21#wechat_redirect)
3，[构建Flink工程及demo演示](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483921&idx=1&sn=faa61778f81e6eeeafb58dca21c7d798&chksm=9f38e139a84f682fbb09227b899c16225c89074488596664f6de281224785df26ba5290dc497&scene=21#wechat_redirect)
4，[Flink系列之时间](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484055&idx=1&sn=4415fc918b0313002303fd9e999a476a&chksm=9f38e1bfa84f68a917fed17ebb28db0951c0c00740d30cae54ee56c24986e4e8b6b373febcd8&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
