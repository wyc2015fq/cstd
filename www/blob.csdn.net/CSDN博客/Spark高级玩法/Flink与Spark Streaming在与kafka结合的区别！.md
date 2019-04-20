# Flink与Spark Streaming在与kafka结合的区别！ - Spark高级玩法 - CSDN博客
2018年07月13日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：2265
本文主要是想聊聊flink与kafka结合。当然，单纯的介绍flink与kafka的结合呢，比较单调，也没有可对比性，所以的准备顺便帮大家简单回顾一下Spark Streaming与kafka的结合。
看懂本文的前提是首先要熟悉kafka，然后了解spark Streaming的运行原理及与kafka结合的两种形式，然后了解flink实时流的原理及与kafka结合的方式。
**kafka**
kafka作为一个消息队列，在企业中主要用于缓存数据，当然，也有人用kafka做存储系统，比如存最近七天的数据。kafka的基本概念请参考：[kafka入门介绍](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484897&idx=1&sn=917255a95acee99e220644217003f2b2&chksm=9f38e6c9a84f6fdf77efa239be422c240a03b8124d6aef3e5c2c731a9635b2010a244eaeff43&scene=21#wechat_redirect)
更多kafka的文章请关注浪尖公众号，阅读。
首先，我们先看下图，这是一张生产消息到kafka，从kafka消费消息的结构图。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWibnCJh3av6TqibnicO5kfocxF7rmsxVIPfsVayfaoHJT81dhMDJROWAMpUyLK1pow465BOibJqaGEvQ/640?wx_fmt=png)
当然， 这张图很简单，拿这张图的目的是从中可以得到的跟本节文章有关的消息，有以下两个：
1，kafka中的消息不是kafka主动去拉去的，而必须有生产者往kafka写消息。
2，kafka是不会主动往消费者发布消息的，而必须有消费者主动从kafka拉取消息。
**spark Streaming结合kafka**
Spark Streaming现在在企业中流处理也是用的比较广泛，但是大家都知道其不是真正的实时处理，而是微批处理。
在spark 1.3以前，SPark Streaming与kafka的结合是基于Receiver方式，顾名思义，我们要启动1+个Receiver去从kafka里面拉去数据，拉去的数据会每隔200ms生成一个block，然后在job生成的时候，取出该job处理时间范围内所有的block，生成blockrdd，然后进入Spark core处理。
自Spark1.3以后，增加了direct Stream API，这种呢，主要特点是去掉了Receiver，在生成job，去取rdd的时候，计算每个partition要取数据的offset范围，然后生成一个kafkardd，该rdd特点是与kafka的分区是一一对应的。
有上面的特点可以看出，Spark Streaming是要生成rdd，然后进行处理的，rdd数据集我们可以理解为静态的，然每个批次，都会生成一个rdd，该过程就体现了批处理的特性，由于数据集时间段小，数据小，所以又称微批处理，那么就说明不是真正的实时处理。
还有一点，spark Streaming与kafka的结合是不会发现kafka动态增加的topic或者partition。
Spark的详细教程，请关注浪尖公众号，查看历史推文。
Spark Streaming与kafka结合源码讲解，请加入知识星球，获取。
**flink结合kafka**
大家都知道flink是真正的实时处理，他是基于事件触发的机制进行处理，而不是像spark Streaming每隔若干时间段，生成微批数据，然后进行处理。那么这个时候就有了个疑问，在前面kafka小节中，我们说到了kafka是不会主动往消费者里面吐数据的，需要消费者主动去拉去数据来处理。那么flink是如何做到基于事件实时处理kafka的数据呢？在这里浪尖带着大家看一下源码，flink1.5.0为例。
**1，flink与kafka结合的demo。**
val env = StreamExecutionEnvironment.getExecutionEnvironment
env.getConfig.disableSysoutLogging
env.getConfig.setRestartStrategy(RestartStrategies.fixedDelayRestart(4, 10000))
// create a checkpoint every 5 seconds
env.enableCheckpointing(5000)
// make parameters available in the web interface
env.getConfig.setGlobalJobParameters(params)
// create a Kafka streaming source consumer for Kafka 0.10.x
val kafkaConsumer = new FlinkKafkaConsumer010(
  params.getRequired("input-topic"),
new SimpleStringSchema,
params.getProperties)
val messageStream = env
  .addSource(kafkaConsumer)
  .map(in => prefix + in)
// create a Kafka producer for Kafka 0.10.x
val kafkaProducer = new FlinkKafkaProducer010(
  params.getRequired("output-topic"),
new SimpleStringSchema,
params.getProperties)
// write data into Kafka
messageStream.addSink(kafkaProducer)
env.execute("Kafka 0.10 Example")
从上面的demo可以看出，数据源的入口就是FlinkKafkaConsumer010，当然这里面只是简单的构建了一个对象，并进行了一些配置的初始化，真正source的启动是在其run方法中run方法的调用过程在这里不讲解，后面会出教程讲解。
**首先看一下类的继承关系**
public class FlinkKafkaConsumer010<T> extends FlinkKafkaConsumer09<T>
public class FlinkKafkaConsumer09<T> extends FlinkKafkaConsumerBase<T>
其中，run方法就在FlinkKafkaConsumerBase里，当然其中open方法里面对kafka相关内容进行里初始化。
从输入到计算到输出完整的计算链条的调用过程，后面浪尖会出文章介绍。在这里只关心flink如何从主动消费数据，然后变成事件处理机制的过程。
由于其FlinkKafkaConsumerBase的run比较长，我这里只看重要的部分，首先是会**创建Kafka09Fetcher**。
this.kafkaFetcher = createFetcher(
      sourceContext,
subscribedPartitionsToStartOffsets,
periodicWatermarkAssigner,
punctuatedWatermarkAssigner,
(StreamingRuntimeContext) getRuntimeContext(),
offsetCommitMode,
getRuntimeContext().getMetricGroup().addGroup(KAFKA_CONSUMER_METRICS_GROUP),
useMetrics);
接着下面有段神器，flink严重优越于Spark Streaming的，代码如下：
final AtomicReference<Exception> discoveryLoopErrorRef = new AtomicReference<>();
this.discoveryLoopThread = new Thread(new Runnable() {
@Override
public void run() {
try {
// --------------------- partition discovery loop ---------------------
List<KafkaTopicPartition> discoveredPartitions;
// throughout the loop, we always eagerly check if we are still running before
         // performing the next operation, so that we can escape the loop as soon as possible
while (running) {
if (LOG.isDebugEnabled()) {
LOG.debug("Consumer subtask {} is trying to discover new partitions ...", getRuntimeContext().getIndexOfThisSubtask());
}
try {
               discoveredPartitions = partitionDiscoverer.discoverPartitions();
} catch (AbstractPartitionDiscoverer.WakeupException | AbstractPartitionDiscoverer.ClosedException e) {
// the partition discoverer may have been closed or woken up before or during the discovery;
               // this would only happen if the consumer was canceled; simply escape the loop
break;
}
// no need to add the discovered partitions if we were closed during the meantime
if (running && !discoveredPartitions.isEmpty()) {
kafkaFetcher.addDiscoveredPartitions(discoveredPartitions);
}
// do not waste any time sleeping if we're not running anymore
if (running && discoveryIntervalMillis != 0) {
try {
                  Thread.sleep(discoveryIntervalMillis);
} catch (InterruptedException iex) {
// may be interrupted if the consumer was canceled midway; simply escape the loop
break;
}
            }
         }
      } catch (Exception e) {
discoveryLoopErrorRef.set(e);
} finally {
// calling cancel will also let the fetcher loop escape
         // (if not running, cancel() was already called)
if (running) {
            cancel();
}
      }
   }
}, "Kafka Partition Discovery for " + getRuntimeContext().getTaskNameWithSubtasks());
它定义了一个线程池对象，去动态发现kafka新增的topic（支持正则形式指定消费的topic），或者动态发现kafka新增的分区。
接着肯定是启动动态发现分区或者topic线程，并且启动kafkaFetcher。
discoveryLoopThread.start();
kafkaFetcher.runFetchLoop();
// --------------------------------------------------------------------
// make sure that the partition discoverer is properly closed
partitionDiscoverer.close();
discoveryLoopThread.join();
接着，我们进入kafkaFetcher的runFetchLoop方法，映入眼帘的是
// kick off the actual Kafka consumer
consumerThread.start();
这个线程是在构建kafka09Fetcher的时候创建的
this.consumerThread = new KafkaConsumerThread(
LOG,
handover,
kafkaProperties,
unassignedPartitionsQueue,
createCallBridge(),
getFetcherName() + " for " + taskNameWithSubtasks,
pollTimeout,
useMetrics,
consumerMetricGroup,
subtaskMetricGroup);
KafkaConsumerThread 继承自Thread，然后在其run方法里，首先看到的是
// this is the means to talk to FlinkKafkaConsumer's main thread
final Handover handover = this.handover;
这个handover的作用呢暂且不提，接着分析run方法里面内容
**1，获取消费者**
try {
this.consumer = getConsumer(kafkaProperties);
}
**2，检测分区并且会重分配新增的分区**
try {
if (hasAssignedPartitions) {
      newPartitions = unassignedPartitionsQueue.pollBatch();
}
else {
// if no assigned partitions block until we get at least one
      // instead of hot spinning this loop. We rely on a fact that
      // unassignedPartitionsQueue will be closed on a shutdown, so
      // we don't block indefinitely
newPartitions = unassignedPartitionsQueue.getBatchBlocking();
}
if (newPartitions != null) {
      reassignPartitions(newPartitions);
}
**3，消费数据**
// get the next batch of records, unless we did not manage to hand the old batch over
if (records == null) {
try {
      records = consumer.poll(pollTimeout);
}
catch (WakeupException we) {
continue;
}
}
**4，通过handover将数据发出去**
try {
   handover.produce(records);
records = null;
}
由于被kafkaConsumerThread打断了kafkaFetcher的runFetchLoop方法的分析，我们在这里继续
**1，拉取handover.producer生产的数据**
while (running) {
// this blocks until we get the next records
   // it automatically re-throws exceptions encountered in the consumer thread
final ConsumerRecords<byte[], byte[]> records = handover.pollNext();
**2，数据格式整理**，并将数据整理好后，逐个Record发送，将循环主动批量拉取kafka数据，转化为事件触发。
// get the records for each topic partition
for (KafkaTopicPartitionState<TopicPartition> partition : subscribedPartitionStates()) {
   List<ConsumerRecord<byte[], byte[]>> partitionRecords =
         records.records(partition.getKafkaPartitionHandle());
   for (ConsumerRecord<byte[], byte[]> record : partitionRecords) {
final T value = deserializer.deserialize(
            record.key(), record.value(),
record.topic(), record.partition(), record.offset());
      if (deserializer.isEndOfStream(value)) {
// end of stream signaled
running = false;
         break;
}
// emit the actual record. this also updates offset state atomically
      // and deals with timestamps and watermark generation
emitRecord(value, partition, record.offset(), record);
}
}
肯定会注意到这行代码emitRecord(value, partition, record.offset(), record);，从这里开始flink变成事件触发的流引擎。
**handover-枢纽**
handover是在构建kafkaFetcher的时候构建的
this.handover = new Handover();
handover是一个工具，将一组数据或者异常从生产者线程传输到消费者线程。它高效的扮演了一个阻塞队列的特性。该类运行于flink kafka consumer，用来在kafkaConsumer 类和主线程之间转移数据和异常。
handover有两个重要方法，分别是：
1，producer
producer是将kafkaConusmer获取的数据发送出去，在KafkaConsumerThread中调用。代码如上
2，pollnext
从handover里面拉去下一条数据，会阻塞的，行为很像是从一个阻塞队列里面拉去数据。
综述
kafkaConsumer批量拉去数据，flink将其经过整理之后变成，逐个Record发送的事件触发式的流处理。这就是flink与kafka结合事件触发时流处理的基本思路。
重要的事情再说一遍：Flink支持动态发现新增topic或者新增partition哦。具体实现思路，前面有代码为证，后面会对比spark Streaming的这块（不支持动态发现新增kafka topic或者partition），来详细讲解。
**推荐阅读：**
[Flink on yarn初步讲解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485017&idx=1&sn=77a48922ccddfa84f6ef25b395cf084e&chksm=9f38e571a84f6c67d6906b53770a5bfa99a1fcd9ee818c21acc8ed72cc7e8707287235836465&scene=21#wechat_redirect)
[Flink并行度](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485003&idx=1&sn=823c50f422cdcce1b6e1b5cf608a882c&chksm=9f38e563a84f6c756b37d4f68812e7b783105b66aafa87529038da71dc7e1a626b2542ef4338&scene=21#wechat_redirect)
