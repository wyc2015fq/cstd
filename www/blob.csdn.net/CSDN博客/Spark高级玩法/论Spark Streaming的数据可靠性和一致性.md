# 论Spark Streaming的数据可靠性和一致性 - Spark高级玩法 - CSDN博客
2017年11月24日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：321
**摘要：**Spark Streaming自发布起就得到了广泛的关注，然而作为一个年轻的项目，需要提升的地方同样很多，比如1.2之前版本driver挂掉可能会丢失数据。这里将分析它的可靠性机制。
眼下大数据领域最热门的词汇之一便是流计算了，其中最耀眼的项目无疑是来自Spark社区的Spark Streaming项目，其从一诞生就受到广泛关注并迅速发展，目前已有追赶并超越Storm的架势。
对于流计算而言，毫无疑问最核心的特点是它的低时延能力，这主要是来自对数据不落磁盘就进行计算的内部机制，但这也带来了数据可靠性的问题，即有节点失效或者网络异常时，如何在节点间进行合适的协商来进行重传。更进一步的，若发生计划外的数据重传，怎么能保证没有产生重复的数据，所有数据都是精确一次的（Exact Once）？如果不解决这些问题，大数据的流计算将无法满足大多数企业级可靠性要求而流于徒有虚名。
本文将重点分析Spark Streaming是如何设计可靠性机制并实现数据一致性的。
### **Driver HA**
由于流计算系统是长期运行、数据不断流入的，因此其Spark守护进程（Driver）的可靠性是至关重要的，它决定了Streaming程序能否一直正确地运行下去。
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVou13r4rVNLVgdvILnwUPSvk1jlzicU8iarNDR3uDkbpqZxruDAVibXj7uV3A8usERj2h2Gys4JASuQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**图一 Driver数据持久化**
Driver实现HA的解决方案就是将元数据持久化，以便重启后的状态恢复。如图一所示，Driver持久化的元数据包括：
- 
**Block元数据（图一中的绿色箭头）**：Receiver从网络上接收到的数据，组装成Block后产生的Block元数据；
- 
**Checkpoint数据（图一中的橙色箭头）**：包括配置项、DStream操作、未完成的Batch状态、和生成的RDD数据等；
![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFVou13r4rVNLVgdvILnwUPSwM4aSV6WYY3ia1P5cwmephXJIKT6JpfsqI2p6icXrh7lJLKL2ot1icNdA/0?wx_fmt=jpeg)
**图二 Driver故障恢复**
Driver失败重启后：
- 
恢复计算（图二中的橙色箭头）：使用Checkpoint数据重启driver，重新构造上下文并重启接收器。
- 
恢复元数据块（图二中的绿色箭头）：恢复Block元数据。
- 
恢复未完成的作业（图二中的红色箭头）：使用恢复出来的元数据，再次产生RDD和对应的job，然后提交到Spark集群执行。
通过如上的数据备份和恢复机制，Driver实现了故障后重启、依然能恢复Streaming任务而不丢失数据，因此提供了系统级的数据高可靠。
### **可靠的上下游IO系统**
流计算主要通过网络socket通信来实现与外部IO系统的数据交互。由于网络通信的不可靠特点，发送端与接收端需要通过一定的协议来保证数据包的接收确认、和失败重发机制。
不是所有的IO系统都支持重发，这至少需要实现数据流的持久化，同时还要实现高吞吐和低时延。在Spark Streaming官方支持的data source里面，能同时满足这些要求的只有Kafka，因此在最近的Spark Streaming release里面，也是把Kafka当成推荐的外部数据系统。
除了把Kafka当成输入数据源（inbound data source）之外，通常也将其作为输出数据源（outbound data source）。所有的实时系统都通过Kafka这个MQ来做数据的订阅和分发，从而实现流数据生产者和消费者的解耦。
一个典型的企业大数据中心数据流向视图如下所示：
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVou13r4rVNLVgdvILnwUPSiccXpenK3tTNlOVbM441821rMyD6hzwnrobJeDUex4c9nFWrsmcBnQA/0?wx_fmt=png)
**图三 企业大数据中心数据流向视图**
除了从源头保证数据可重发之外，Kafka更是流数据Exact Once语义的重要保障。Kafka提供了一套低级API，使得client可以访问topic数据流的同时也能访问其元数据。Spark Streaming的每个接收任务可以从指定的Kafka topic、partition和offset去获取数据流，各个任务的数据边界很清晰，任务失败后可以重新去接收这部分数据而不会产生“重叠的”数据，因而保证了流数据“有且仅处理一次”。
### **可靠的接收器**
在Spark 1.3版本之前，Spark Streaming是通过启动专用的Receiver任务来完成从Kafka集群的数据流拉取。
Receiver任务启动后，会使用Kafka的高级API来创建topicMessageStreams对象，并逐条读取数据流缓存，每个batchInerval时刻到来时由JobGenerator提交生成一个spark计算任务。
由于Receiver任务存在宕机风险，因此Spark提供了一个高级的可靠接收器-ReliableKafkaReceiver类型来实现可靠的数据收取，它利用了Spark 1.2提供的WAL（Write Ahead Log）功能，把接收到的每一批数据持久化到磁盘后，更新topic-partition的offset信息，再去接收下一批Kafka数据。万一Receiver失败，重启后还能从WAL里面恢复出已接收的数据，从而避免了Receiver节点宕机造成的数据丢失（以下代码删除了细枝末节的逻辑）：
class ReliableKafkaReceiver{  private var topicPartitionOffsetMap: mutable.HashMap[TopicAndPartition, Long] = null  private var blockOffsetMap: ConcurrentHashMap[StreamBlockId, Map[TopicAndPartition, Long]] = null  override def onStart(): Unit = {    // Initialize the topic-partition / offset hash map.    topicPartitionOffsetMap = new mutable.HashMap[TopicAndPartition, Long]    // Initialize the block generator for storing Kafka message.    blockGenerator = new BlockGenerator(new GeneratedBlockHandler, streamId, conf)    messageHandlerThreadPool = Utils.newDaemonFixedThreadPool(      topics.values.sum, "KafkaMessageHandler")    blockGenerator.start()    val topicMessageStreams = consumerConnector.createMessageStreams(      topics, keyDecoder, valueDecoder)    topicMessageStreams.values.foreach { streams =>      streams.foreach { stream =>        messageHandlerThreadPool.submit(new MessageHandler(stream))      }    }  }
启用WAL后虽然Receiver的数据可靠性风险降低了，但却由于磁盘持久化带来的开销，系统整体吞吐率会有明显的下降。因此，在最新发布的Spark 1.3版本里，Spark Streaming增加了使用Direct API的方式来实现Kafka数据源的访问。
引入了Direct API后，Spark Streaming不再启动常驻的Receiver接收任务，而是直接分配给每个Batch及RDD最新的topic partition offset。job启动运行后Executor使用Kafka的simple consumer API去获取那一段offset的数据。
这样做的好处不仅避免了Receiver宕机带来的数据可靠性风险，同时也由于避免使用ZooKeeper做offset跟踪，而实现了数据的精确一次性（以下代码删除了细枝末节的逻辑）：
class DirectKafkaInputDStream{  protected val kc = new KafkaCluster(kafkaParams)  protected var currentOffsets = fromOffsets  override def compute(validTime: Time): Option[KafkaRDD[K, V, U, T, R]] = {    val untilOffsets = clamp(latestLeaderOffsets(maxRetries))    val rdd = KafkaRDD[K, V, U, T, R](      context.sparkContext, kafkaParams, currentOffsets, untilOffsets, messageHandler)    currentOffsets = untilOffsets.map(kv => kv._1 -> kv._2.offset)    Some(rdd)  }  
### **预写日志 Write Ahead Log**
Spark 1.2开始提供了预写日志能力，用于Receiver数据及Driver元数据的持久化和故障恢复。WAL之所以能提供持久化能力，是因为它利用了可靠的HDFS做数据存储。
Spark Streaming预写日志机制的核心API包括：
- 
管理WAL文件的WriteAheadLogManager
- 
读/写WAL的WriteAheadLogWriter和WriteAheadLogReader
- 
基于WAL的RDD：WriteAheadLogBackedBlockRDD
- 
基于WAL的Partition：WriteAheadLogBackedBlockRDDPartition
以上核心API在数据接收和恢复阶段的交互示意图如图四所示。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVou13r4rVNLVgdvILnwUPSkztf1ibM7I8Q0r1fK1xicxpYZeVelal4aPymAtvia4xtf3f9sVkFyW3fg/0?wx_fmt=png)
**图四 基于WAL的数据接收和恢复示意图**
从WriteAheadLogWriter的源码里可以清楚地看到，每次写入一块数据buffer到HDFS后都会调用flush方法去强制刷入磁盘，然后才去取下一块数据。因此receiver接收的数据是可以保证持久化到磁盘了，因而做到了较好的数据可靠性。
private[streaming] class WriteAheadLogWriter{  private lazy val stream = HdfsUtils.getOutputStream(path, hadoopConf)  def write(data: ByteBuffer): WriteAheadLogFileSegment = synchronized {    data.rewind() // Rewind to ensure all data in the buffer is retrieved    val lengthToWrite = data.remaining()    val segment = new WriteAheadLogFileSegment(path, nextOffset, lengthToWrite)    stream.writeInt(lengthToWrite)    if (data.hasArray) {      stream.write(data.array())    } else {      while (data.hasRemaining) {        val array = new Array[Byte](data.remaining)        data.get(array)        stream.write(array)      }    }    flush()    nextOffset = stream.getPos()    segment  }
### **结束语**
得益于Kafka这类可靠的data source、以及自身的checkpoint/WAL等机制，Spark Streaming的数据可靠性得到了很好的保证，数据能保证“至少一次”（at least once）被处理。但由于其outbound端的一致性实现还未完善，因此Exact once语义仍然不能端到端保证。Spark Streaming社区已经在跟进这个特性的实现（SPARK-4122），预计很快将合入trunk发布。
推荐阅读：
1，[金融反欺诈场景下的Spark实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484078&idx=1&sn=6740b1da86f8bda4157178ff9ec9d7db&chksm=9f38e186a84f689092aad7e9023fdca856abdada21b2a417870a075899384916f3809c3563be&scene=21#wechat_redirect)
2，[spark源码系列之内部通讯的三种机制](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483733&idx=1&sn=92e7bcf399a8aaa24f7b6c37b98b3b2e&chksm=9f38e27da84f6b6bcd9e26db1f11b83fd4dca9d4e3512a25fd3b4eae012065c939d6ed6ecaad&scene=21#wechat_redirect)
3，[Spark源码系列之spark2.2的StructuredStreaming使用及源码介绍](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483744&idx=1&sn=0b9908624245df2eaa24a88d2a04be58&chksm=9f38e248a84f6b5e206cb0bf3ade7378c00d1cf662125315341a143b63f9e8f72cdf91a0db17&scene=21#wechat_redirect)
4，[Spark调优系列之序列化方式调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483770&idx=1&sn=431258be59659cbe72a724984c5d3ccd&chksm=9f38e252a84f6b4446e94a5a4eede57933672a377364566519880ee7d9a1bc57c8a4c831e257&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**
