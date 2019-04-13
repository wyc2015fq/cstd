
# Spark Streaming - anlcy - 博客园






# [Spark Streaming](https://www.cnblogs.com/camilla/p/8295341.html)
Spark Streaming基于Spark处理流式数据的框架，在MapReduce中，由于其分布式特性——所有数据需要读写磁盘、启动job耗时较大，难以满足时效性要求。而Streaming能够在Spark上生根发芽的原因是因为其内存特性、低延时的执行引擎和高速的执行效率。
Streaming的原理是将Stream数据分成小的时间间隔（比如几秒），即将其离散化（Discretized）并转换成一个一个数据集（RDD），然后分批处理处理这小的RDD。所以Streaming很容易很mlib，Spark SQL等进行结合，做到实时的数据分析处理。此外，Streaming也继承了RDD的容错特性。如果RDD 的某些 partition 丢失了 , 可以通过 lineage 信息重新计算恢复。
Streaming的数据源主要分下面两类：
**·**外部文件系统 , 如 HDFS，Streaming可以监控一个目录中新产生的数据，并及时处理。如果出现fail，可以通过重新读取数据来恢复 , 绝对不会有数据丢失。
**·**网络系统：如MQ系统（Kafka、ZeroMQ、Flume等）。Streaming会默认会在两个不同节点加载数据到内存 , 一个节点 fail 了 , 系统可以通过另一个节点的数据重算。假设正在运行 InputReceiver 的节点 fail 了 , 可能会丢失一部分数据。
![streaming-arch](http://debugo.com/wp-content/uploads/2014/09/streaming-arch-640x239.png)
## 使用Spark Streaming
一个简单的基于Streaming的workCount代码如下：
package cn.com.gf.spark.demo.test
import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.streaming.{Seconds, StreamingContext}
import org.apache.spark.streaming.StreamingContext._
object HelloWorld {
def main(args: Array[String]): Unit ={
val sparkConf = new SparkConf().setAppName("HDFSWordCount").setMaster("spark://172.19.1.232:7077")
//create the streaming context
val  ssc = new StreamingContext(sparkConf, Seconds(30))
//process file when new file be found.
val lines = ssc.textFileStream("file:///home/spark/data")
val words = lines.flatMap(_.split(" "))
val wordCounts = words.map(x => (x, 1)).reduceByKey(_ + _)
wordCounts.print()
ssc.start()
ssc.awaitTermination()
}
}
这段代码实现了当指定的路径有新文件生成时，就会对这些文件执行wordcount，并把结果print。具体流程如下：
![wordcount](http://debugo.com/wp-content/uploads/2014/09/wordcount.png)
#### (1). 创建StreamingContext
使用Spark Streaming就需要创建StreamingContext对象（类似SparkContext）。创建StreamingContext对象所需的参数与SparkContext基本一致，包括设定Master节点(setMaster），设定应用名称(setAppName)。第二个参数Seconds(30)，指定了Spark Streaming处理数据的时间间隔为30秒。需要根据具体应用需要和集群处理能力进行设置。
#### (2). 创建InputDStream
Spark Streaming它支持并使用的数据流叫Dstream，类似于Spark使用RDD，直接支持Kafka、Flume的数据源。Dstream是一种连续的RDDs。Spark Streaming需要指明数据源。如上例所示的textFileStream，Spark Streaming以文件系统作为输入流。这里有3个要点：
**（1）**dataDirectory下的文件格式都是一样
**（2）**在这个目录下创建文件都是通过移动或者重命名的方式创建的
**（3）**一旦文件进去之后就不能再改变
Spark Streaming支持多种不同数据源，它们对应的DStream同样不同(如kafkaStream，flumeStream， networkStream等）。
#### (3). 操作DStream
对于从数据源得到的DStream，用户可以在其基础上进行各种操作，如上例所示的操作就是一个典型的word count执行流程：对于当前时间窗口内从数据源得到的数据首先进行 flatMap，然后进行map和reduceByKey，并在最后使用print()输出。
Streaming中的Dstream支持两种操作：Transformation和output。Streaming中的Transformation和RDD的Transformation相类似，都是对其离散化数据集进行处理。
|Transformation|Meaning|
|window|(|windowLength|,|slideInterval|)|Return a new DStream which is computed based on windowed batches of the source DStream.|
|countByWindow|(|windowLength|,|slideInterval|)|Return a sliding window count of elements in the stream.|
|reduceByWindow|(|func|,|windowLength|,|slideInterval|)|Return a new single-element stream, created by aggregating elements in the stream over a sliding interval using|func|. The function should be associative so that it can be computed correctly in parallel.|
|reduceByKeyAndWindow|(|func|,|windowLength|,|slideInterval|, [|numTasks|])|When called on a DStream of (K, V) pairs, returns a new DStream of (K, V) pairs where the values for each key are aggregated using the given reduce function|func|over batches in a sliding window.|Note:|By default, this uses Spark’s default number of parallel tasks (2 for local mode, and in cluster mode the number is determined by the config property|spark|.|default|.|parallelism|) to do the grouping. You can pass an optional|numTasks|argument to set a different number of tasks.|
|reduceByKeyAndWindow|(|func|,|invFunc|,|windowLength|,|slideInterval|, [|numTasks|])|A more efficient version of the above|reduceByKeyAndWindow|()|where the reduce value of each window is calculated incrementally using the reduce values of the previous window. This is done by reducing the new data that enter the sliding window, and “inverse reducing” the old data that leave the window. An example would be that of “adding” and “subtracting” counts of keys as the window slides. However, it is applicable to only “invertible reduce functions”, that is, those reduce functions which have a corresponding “inverse reduce” function (taken as parameter|invFunc|. Like in|reduceByKeyAndWindow|, the number of reduce tasks is configurable through an optional argument.|
|countByValueAndWindow|(|windowLength|,|slideInterval|, [|numTasks|])|When called on a DStream of (K, V) pairs, returns a new DStream of (K, Long) pairs where the value of each key is its frequency within a sliding window. Like in|reduceByKeyAndWindow|, the number of reduce tasks is configurable through an optional argument.|
### Output操作：
|Output Operation|Meaning|
|print|()|Prints first ten elements of every batch of data in a DStream on the driver.|
|foreachRDD|(|func|)|The fundamental output operator. Applies a function,|func|, to each RDD generated from the stream. This function should have side effects, such as printing output, saving the RDD to external files, or writing it over the network to an external system.|
|saveAsObjectFiles|(|prefix|, [|suffix|])|Save this DStream’s contents as a|SequenceFile|of serialized objects. The file name at each batch interval is generated based on|prefix|and|suffix|:|“prefix-TIME_IN_MS[.suffix]“|.|
|saveAsTextFiles|(|prefix|, [|suffix|])|Save this DStream’s contents as a text files. The file name at each batch interval is generated based on|prefix|and|suffix|:|“prefix-TIME_IN_MS[.suffix]“|.|
|saveAsHadoopFiles|(|prefix|, [|suffix|])|Save this DStream’s contents as a Hadoop file. The file name at each batch interval is generated based on|prefix|and|suffix|:|“prefix-TIME_IN_MS[.suffix]“|.|
#### (4). 启动Spark Streaming
上述只是定义好了应用拓扑结构，当启动Spark Streaming (ssc.start())时，当ssc.start()启动后程序才真正进行所有预期的操作。才会按既定的interval来执行这一系列job。
运行这个spark-streaming代码：
spark-submit --master spark://debugo:7077 --class MyExample.WordCountStreaming spark0_2.10-1.0.jar
......
(discretized,1)
(stream,3)
(created,1)
(are,1)
(this,2)
(Kafka,1)
(is,1)
(provides,1)
(guide,2)
(Internally,,1)
...
/09/03 10:37:31 INFO MapPartitionsRDD: Removing RDD 20 from persistence list
/09/03 10:37:31 INFO BlockManager: Removing RDD 20
/09/03 10:37:31 INFO MappedRDD: Removing RDD 17 from persistence list
/09/03 10:37:31 INFO BlockManager: Removing RDD 17
/09/03 10:37:31 INFO FlatMappedRDD: Removing RDD 16 from persistence list
/09/03 10:37:31 INFO BlockManager: Removing RDD 16
/09/03 10:37:31 INFO MappedRDD: Removing RDD 15 from persistence list
/09/03 10:37:31 INFO BlockManager: Removing RDD 15
/09/03 10:37:31 INFO UnionRDD: Removing RDD 14 from persistence list
/09/03 10:37:31 INFO BlockManager: Removing RDD 14
stateful操作
上面的例子是一种最简单的应用场景——无状态更新。大多数时候，这种流式处理模型很难满足需求——比如我们需要累计统计单词的数量，或者计算一个信号的频域参数（需要以滑动窗口的方式处理一个间隔的数据），这就要使用另外两种应用场景——状态操作和窗口操作。状态(stateful)操作中，主要使用到下面一个transformation方法：
UpdateStateByKey
我们希望保存它状态的信息时，使用这个操作，并持续更新状态数值。使用它有两个步骤：
（1）定义状态(state)，这个状态可以是任意的数据类型
（2）定义状态更新函数，从前一个状态更改新的状态
下面展示一个例子：
def updateFunction(newValues: Seq[Int], runningCount: Option[Int]): Option[Int] = {
val newCount = … // add the new values with the previous running count to get the new count Some(newCount)
}
它会针对里面的每个元素（如wordCount中的word）调用一下更新函数，newValues是最新的值，runningCount是之前的值。
下面是一个stateful的wordCount实例（修改自rg.apache.spark.examples.streaming.kafkaWordCount）：
/**
* Created by debugo on 2014/9/4.
*/
package com.debugo.example
import java.util.Properties
import kafka.producer._
object WordCountProducer {
def main(args: Array[String]) {
if (args.length &lt; 4) {
System.err.println("Usage: KafkaWordCountProducer " + " ")
System.exit(1)
}
val Array(brokers, topic, messagesPerSec, wordsPerMessage) = args
// Zookeper connection properties
val props = new Properties()
props.put("metadata.broker.list", brokers)
props.put("serializer.class", "kafka.serializer.StringEncoder")
val config = new ProducerConfig(props)
val producer = new Producer[String, String](config)
// Send some messages
while(true) {
val messages = (1 to messagesPerSec.toInt).map { messageNum =&gt;
val str = (1 to wordsPerMessage.toInt).map(x =&gt; scala.util.Random.nextInt(10).toString).mkString(" ")
new KeyedMessage[String, String](topic, str)}.toArray
producer.send(messages: _*)
Thread.sleep(100)
}
}
}
/**
* Created by debugo on 2014/9/3.
*/
package com.debugo.example
import org.apache.spark.SparkConf
import org.apache.spark.streaming.{Seconds, StreamingContext}
import org.apache.spark.streaming.StreamingContext._
import org.apache.spark.streaming.kafka._
object WordCountStateful {
def main(args: Array[String]): Unit = {
val updateFunc = (values: Seq[Int], state: Option[Int]) =&gt; {
val currentCount = values.foldLeft(0)(_ + _)
val previousCount = state.getOrElse(0) //None to 0
Some(currentCount + previousCount)
}
if (args.length &lt; 4) {
System.err.println("Usage: KafkaWordCount ")
System.exit(1)
}
val Array(zkQuorum, group, topics, numThreads) = args
val conf = new SparkConf().setAppName("WordCountStateful")
val ssc = new StreamingContext(conf, Seconds(20))
ssc.checkpoint(".")
//get data
val topicsMap = topics.split(",").map((_,numThreads.toInt)).toMap
val lines = KafkaUtils.createStream(ssc, zkQuorum, group, topicsMap).map(_._2)
val words = lines.flatMap(_.split(","))
val wordCounts = words.map(x =&gt; (x, 1))
//using updateStateByKey to update state.
val stateDstream = wordCounts.updateStateByKey[Int](updateFunc)
stateDstream.print()
ssc.start()
ssc.awaitTermination()
}
}
完成后compile&package。下面是启动kafka服务的流程。
**下载并配置kafka**
**wget http://mirrors.hust.edu.cn/apache/kafka/0.8.1.1/kafka_2.10-0.8.1.1.tgz**
**tar xzvf kafka*.tgz**
**vim conf/server.properties**
**broker.id=0        \# [0-2] for hdp0[2-4]**
**host.name=hdp03**
**advertised.host.name=hdp03**
**zookeeper.connect=hdp02:2181,hdp03:2181,hdp04:2181**
此时，zookeeper服务已经在hdp02,hdp03,hdp04上启动。然后在hdp02,hdp03,hdp04上分别启动kafka服务
/root/kafka_2.10-0.8.1.1/bin/kafka-server-start.sh /root/kafka_2.10-0.8.1.1/config/server.properties
[2014-09-04 15:15:39,784] INFO Opening socket connection to server hdp03/172.19.17.233:2181 (org.apache.zookeeper.ClientCnxn)
[2014-09-04 15:15:39,795] INFO Socket connection established to hdp03/172.19.17.233:2181, initiating session (org.apache.zookeeper.ClientCnxn)
[2014-09-04 15:15:39,819] INFO Session establishment complete on server hdp03/172.19.17.233:2181, sessionid = 0x3483b071fa006a7, negotiated timeout = 6000 (org.apache.zookeeper.ClientCnxn)
[2014-09-04 15:15:39,825] INFO zookeeper state changed (SyncConnected) (org.I0Itec.zkclient.ZkClient)
**新建一个TOPIC**(replication-factor=num of brokers)
/root/kafka_2.10-0.8.1.1/bin/kafka-topics.sh--create--topic test--replication-factor2--partitions2--zookeeper hdp03:2181Createdtopic"test".
**模拟producer进行测试**
/root/kafka_2.10-0.8.1.1/bin/kafka-console-producer.sh--broker-list hdp03:9092--sync--topic test
输入一系列文本消息：
HelloKafka
HiKafka
**打开console consumer**
/root/kafka_2.10-0.8.1.1/bin/kafka-console-consumer.sh--zookeeper hdp03:2181--topic test--from-beginning
HelloKafka
HiKafka
**运行example的producer**
$SPARK_HOME/bin/run-example org.apache.spark.examples.streaming.KafkaWordCountProducerhdp02:9092test35
再submit刚刚编译的jar，这里需要添加kafka和streaming-kafka的jar
spark-submit --master=spark://hdp02:7077 --class=com.debugo.example.WordCountStateful --jars=/usshare/java/spark-streaming-kafka_2.10-1.0.2.jar,/usr/share/java/kafka_2.10-0.8.0.jar spark0_2.10-1.0.jar hdp02:2181,hdp03:2181,hdp04:2181 test-consumer-group test 1
这样就得到了累计统计的效果。需要注意的是，consumer-group要和配置文件config/consumer.properties中的配置一致。
## Window Operations
前面的word count的例子，当我们想要每隔10秒计算一下最近30秒的单词总数。可以使用reduceByKeyAndWindow方法
windowedWordCounts=pairs.reduceByKeyAndWindow(_+_,Seconds(30),Seconds(10))
这里面提到了windows的两个参数：
**（1）**window length：window的长度是30秒，最近30秒的数据
**（2）**slice interval：计算的时间间隔
通过这个例子，我们大概能够窗口的意思了，定期计算滑动的数据。
除此之外，Window操作还有下面操作：
|Transformation|Meaning|
|window|(|windowLength|,|slideInterval|)|Return a new DStream which is computed based on windowed batches of the source DStream.|
|countByWindow|(|windowLength|,|slideInterval|)|Return a sliding window count of elements in the stream.|
|reduceByWindow|(|func|,|windowLength|,|slideInterval|)|Return a new single-element stream, created by aggregating elements in the stream over a sliding interval using|func|. The function should be associative so that it can be computed correctly in parallel.|
|reduceByKeyAndWindow|(|func|,|windowLength|,|slideInterval|, [|numTasks|])|When called on a DStream of (K, V) pairs, returns a new DStream of (K, V) pairs where the values for each key are aggregated using the given reduce function|func|over batches in a sliding window.|Note:|By default, this uses Spark’s default number of parallel tasks (2 for local mode, and in cluster mode the number is determined by the config property|spark|.|default|.|parallelism|) to do the grouping. You can pass an optional|numTasks|argument to set a different number of tasks.|
|reduceByKeyAndWindow|(|func|,|invFunc|,|windowLength|,|slideInterval|, [|numTasks|])|A more efficient version of the above|reduceByKeyAndWindow|()|where the reduce value of each window is calculated incrementally using the reduce values of the previous window. This is done by reducing the new data that enter the sliding window, and “inverse reducing” the old data that leave the window. An example would be that of “adding” and “subtracting” counts of keys as the window slides. However, it is applicable to only “invertible reduce functions”, that is, those reduce functions which have a corresponding “inverse reduce” function (taken as parameter|invFunc|. Like in|reduceByKeyAndWindow|, the number of reduce tasks is configurable through an optional argument.|
|countByValueAndWindow|(|windowLength|,|slideInterval|, [|numTasks|])|When called on a DStream of (K, V) pairs, returns a new DStream of (K, Long) pairs where the value of each key is its frequency within a sliding window. Like in|reduceByKeyAndWindow|, the number of reduce tasks is configurable through an optional argument.|
## 性能调优(摘自参考2)
#### 优化运行时间
**增加并行度**。确保使用整个集群的资源，而不是把任务集中在几个特定的节点上。对于包含shuffle的操作，增加其并行度以确保更为充分地使用集群资源。
**减少数据序列化、反序列化的负担**。Spark Streaming默认将接收到的数据序列化后存储以减少内存的使用。但序列化和反序列化需要更多的CPU时间，因此更加高效的序列化方式（Kryo）和自定义的序列化接口可以更高效地使用CPU。
**设置合理的batch窗口**。在Spark Streaming中，Job之间有可能存在着依赖关系，后面的Job必须确保前面的Job执行结束后才能提交。若前面的Job执行时间超出了设置的batch窗口，那么后面的Job就无法按时提交，这样就会进一步拖延接下来的Job，造成后续Job的阻塞。因此，设置一个合理的batch窗口确保Job能够在这个batch窗口中结束是必须的。
**减少任务提交和分发所带来的负担**。通常情况下Akka框架能够高效地确保任务及时分发，但当batch窗口非常小（500ms）时，提交和分发任务的延迟就变得不可接受了。使用Standalone模式和Coarse-grained Mesos模式通常会比使用Fine-Grained Mesos模式有更小的延迟。
#### 优化内存使用
**控制batch size**。Spark Streaming会把batch窗口内接收到的所有数据存放在Spark内部的可用内存区域中，因此必须确保当前节点Spark的可用内存至少能够容纳这个batch窗口内所有的数据，否则必须增加新的资源以提高集群的处理能力。
**及时清理不再使用的数据**。上面说到Spark Streaming会将接收到的数据全部存储于内部的可用内存区域中，因此对于处理过的不再需要的数据应及时清理以确保Spark Streaming有富余的可用内存空间。通过设置合理的spark.cleaner.ttl时长来及时清理超时的无用数据。
**观察及适当调整GC策略**。GC会影响Job的正常运行，延长Job的执行时间，引起一系列不可预料的问题。观察GC的运行情况，采取不同的GC策略以进一步减小内存回收对Job运行的影响。
## 参考
http://spark.apache.org/docs/latest/streaming-programming-guide.html
http://www.csdn.net/article/2014-01-28/2818282-Spark-Streaming-big-data
http://jerryshao.me/architecture/2013/04/02/spark-streaming-introduction/





