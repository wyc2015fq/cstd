# Apache Kafka教程A系列：与Spark整合 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月09日 16:00:27[boonya](https://me.csdn.net/boonya)阅读数：168








原文地址：[https://www.tutorialspoint.com/apache_kafka/apache_kafka_integration_spark.htm](https://www.tutorialspoint.com/apache_kafka/apache_kafka_integration_spark.htm)

在本章中，我们将讨论如何将Apache Kafka与Spark Streaming API集成。


#### 关于Spark
Spark Streaming API支持实时数据流的可扩展，高吞吐量，容错流处理。数据可以从Kafka，Flume，Twitter等许多来源获取，并且可以使用复杂算法进行处理，例如地图，缩小，连接和窗口等高级功能。最后，处理后的数据可以推送到文件系统，数据库和现场仪表板上。弹性分布式数据集（RDD）是Spark的基础数据结构。它是一个不可变的分布式对象集合。 RDD中的每个数据集都被划分为逻辑分区，这些逻辑分区可以在集群的不同节点上进行计算。


#### 与Spark整合

Kafka是Spark流媒体的潜在消息传递和集成平台。 Kafka充当实时数据流的中心枢纽，并使用Spark Streaming中的复杂算法进行处理。数据处理完成后，Spark Streaming可以将结果发布到HDFS，数据库或仪表板中的另一个Kafka主题或商店中。下图描述了概念流程。

![](https://img-blog.csdn.net/201804091554584)


现在，让我们详细介绍Kafka-Spark API。


##### SparkConf API
它代表Spark应用程序的配置。 用于将各种Spark参数设置为键值对。
SparkConf类具有以下方法 - 
- set(string key, string value) - 设置配置变量。
- remove(string key) - 从配置中删除键。
- setAppName(string name) - 为应用程序设置应用程序名称。
- get(string key) - 获取密钥


##### StreamingContext API

这是Spark功能的主要入口点。 SparkContext表示与Spark群集的连接，并且可用于在群集上创建RDD，累加器和广播变量。 签名的定义如下所示。


publicStreamingContext(String master,String appName,Duration batchDuration,String sparkHome, scala.collection.Seq<String> jars, 
   scala.collection.Map<String,String> environment)- master − 要连接的群集URL(e.g. mesos://host:port, spark://host:port, local[4]).
- appName −作业名称，以显示在集群Web UI上
- batchDuration − 流数据将被分成批次的时间间隔
public StreamingContext(SparkConf conf, Duration batchDuration)
通过提供新的SparkContext所需的配置来创建StreamingContext。


- conf − Spark 参数
- batchDuration − 流数据将被分成批次的时间间隔

##### KafkaUtils API

KafkaUtils API用于将Kafka集群连接到Spark流。 该API具有如下定义的重要方法createStream签名。


public static ReceiverInputDStream<scala.Tuple2<String,String>> createStream(
   StreamingContext ssc, String zkQuorum, String groupId,
   scala.collection.immutable.Map<String,Object> topics, StorageLevel storageLevel)
上面显示的方法用于创建从Kafka Brokers中提取消息的输入流。



- ssc - StreamingContext对象。
- zkQuorum - Zookeeper仲裁。
- groupId - 此消费者的组ID。
- topics - 返回要消费的主题地图。
- storageLevel - 用于存储接收对象的存储级别。

KafkaUtils API还有另一种方法createDirectStream，它用于创建一个输入流，直接从Kafka Brokers中提取消息而不使用任何接收器。 此流可以保证来自Kafka的每条消息都只包含在一次转换中。




示例应用程序在Scala中完成。 要编译应用程序，请下载并安装sbt，scala构建工具（与maven类似）。 主应用程序代码如下所示。


import java.util.HashMapimport org.apache.kafka.clients.producer.{KafkaProducer,ProducerConfig,Produc-erRecord}import org.apache.spark.SparkConfimport org.apache.spark.streaming._
import org.apache.spark.streaming.kafka._

objectKafkaWordCount{def main(args:Array[String]){if(args.length <4){System.err.println("Usage: KafkaWordCount <zkQuorum><group> <topics> <numThreads>")System.exit(1)}

      val Array(zkQuorum,group, topics, numThreads)= args
      val sparkConf =newSparkConf().setAppName("KafkaWordCount")
      val ssc =newStreamingContext(sparkConf,Seconds(2))
      ssc.checkpoint("checkpoint")

      val topicMap = topics.split(",").map((_, numThreads.toInt)).toMap
      val lines =KafkaUtils.createStream(ssc, zkQuorum,group, topicMap).map(_._2)
      val words = lines.flatMap(_.split(" "))
      val wordCounts = words.map(x =>(x,1L)).reduceByKeyAndWindow(_ + _, _ - _,Minutes(10),Seconds(2),2)
      wordCounts.print()

      ssc.start()
      ssc.awaitTermination()}}
##### 构建脚本

spark-kafka集成取决于spark，spark流和spakKafka集成jar。 创建一个新的文件build.sbt并指定应用程序的详细信息及其依赖关系。 sbt将在编译和打包应用程序时下载必要的jar。


name :="Spark Kafka Project"
version :="1.0"
scalaVersion :="2.10.5"

libraryDependencies +="org.apache.spark"%%"spark-core"%"1.6.0"
libraryDependencies +="org.apache.spark"%%"spark-streaming"%"1.6.0"
libraryDependencies +="org.apache.spark"%%"spark-streaming-kafka"%"1.6.0"
##### 编译/打包

运行以下命令来编译和打包应用程序的jar文件。 我们需要将jar文件提交到spark控制台来运行应用程序。


sbt package
##### 提交给Spark

启动Kafka Producer CLI（在前一章中介绍），创建一个名为my-first-topic的新主题，并提供一些示例消息，如下所示。


Another spark test message
运行以下命令将应用程序提交到spark控制台。


/usr/local/spark/bin/spark-submit --packages org.apache.spark:spark-streaming
-kafka_2.10:1.6.0--class"KafkaWordCount"--master local[4] target/scala-2.10/spark
-kafka-project_2.10-1.0.jar localhost:2181<group name><topic name><number of threads>
这个应用程序的输出示例如下所示。


spark console messages ..
(Test,1)
(spark,1)
(another,1)
(message,1)
spark console message ..



