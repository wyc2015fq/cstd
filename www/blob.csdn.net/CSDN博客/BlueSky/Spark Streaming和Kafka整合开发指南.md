# Spark Streaming和Kafka整合开发指南 - BlueSky - CSDN博客
2015年11月23日 09:42:32[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：667
转自：http://dataunion.org/15193.html
Apache [Kafka](http://www.iteblog.com/archives/tag/kafka)是一个分布式的消息发布-订阅系统。可以说，任何实时大数据处理工具缺少与[Kafka](http://www.iteblog.com/archives/tag/kafka)整合都是不完整的。本文将介绍如何使用[Spark](http://www.iteblog.com/archives/tag/spark) Streaming从[Kafka](http://www.iteblog.com/archives/tag/kafka)中接收数据，这里将会介绍两种方法：（1）、使用Receivers和Kafka高层次的API；（2）、使用Direct
 API，这是使用低层次的KafkaAPI，并没有使用到Receivers，是[Spark](http://www.iteblog.com/archives/tag/spark) 1.3.0中开始引入的。这两种方法有不同的编程模型，性能特点和语义担保。下文将会一一介绍。
## 基于Receivers的方法
这个方法使用了Receivers来接收数据。Receivers的实现使用到Kafka高层次的消费者API。对于所有的Receivers，接收到的数据将会保存在[Spark](http://www.iteblog.com/archives/tag/spark) executors中，然后由Spark
 Streaming启动的Job来处理这些数据。
然而，在默认的配置下，这种方法在失败的情况下会丢失数据，为了保证零数据丢失，你可以在Spark Streaming中使用WAL日志，这是在Spark 1.2.0才引入的功能，这使得我们可以将接收到的数据保存到WAL中（WAL日志可以存储在HDFS上），所以在失败的时候，我们可以从WAL中恢复，而不至于丢失数据。
下面，我将介绍如何使用这种方法来接收数据。
**1、引入依赖。**
对于Scala和Java项目，你可以在你的pom.xml文件引入以下依赖：
|`1`|`<dependency>`|
|`2`|`  ``<groupId>org.apache.spark</groupId>`|
|`3`|`  ``<artifactId>spark-streaming-kafka_2.``10``</artifactId>`|
|`4`|`  ``<version>``1.3``.``0``</version>`|
|`5`|`</dependency>`|
如果你是使用SBT，可以这么引入：
|`1`|```libraryDependencies +=````"org.apache.spark"``% ``"spark-streaming-kafka_2.10"``% ``"1.3.0"`|
**2、编程**
在Streaming程序中，引入KafkaUtils，并创建一个输入DStream：
|`1`|`import``org.apache.spark.streaming.kafka.``_`|
|`2`||
|`3`|`val``kafkaStream ``=``KafkaUtils.createStream(streamingContext,`|
|`4`|`    ````[ZK quorum], [consumer group id], [per-topic number of Kafka partitions to consume])```|
在创建DStream的时候，你也可以指定数据的Key和Value类型，并指定相应的解码类。
**需要注意的是：**
1、Kafka中Topic的分区和Spark Streaming生成的RDD中分区不是一个概念。所以，在`KafkaUtils.createStream()`增加特定主题分区数仅仅是增加一个receiver中消费Topic的线程数。并不增加Spark并行处理数据的数量；　　2、对于不同的Group和tpoic我们可以使用多个receivers创建不同的DStreams来并行接收数据；
3、如果你启用了WAL，这些接收到的数据将会被持久化到日志中，因此，我们需要将storage level 设置为`StorageLevel.MEMORY_AND_DISK_SER `,也就是：
|`1`|```KafkaUtils.createStream(..., StorageLevel.MEMORY````_``AND``_``DISK``_``SER)`|
**3、部署**
对应任何的Spark 应用，我们都是用`spark-submit`来启动你的应用程序，对于Scala和Java用户，如果你使用的是SBT或者是Maven，你可以将spark-streaming-kafka_2.10及其依赖打包进应用程序的Jar文件中，并确保spark-core_2.10和
 spark-streaming_2.10标记为provided，因为它们在Spark 安装包中已经存在：
|`01`|`<dependency>`|
|`02`|`          ``<groupId>org.apache.spark</groupId>`|
|`03`|`          ``<artifactId>spark-streaming``_``2.10``</artifactId>`|
|`04`|`          ``<version>``1.3``.``0``</version>`|
|`05`|`          ``<scope>provided</scope>`|
|`06`|`</dependency>`|
|`07`||
|`08`|`<dependency>`|
|`09`|`          ``<groupId>org.apache.spark</groupId>`|
|`10`|`          ``<artifactId>spark-core``_``2.10``</artifactId>`|
|`11`|`          ``<version>``1.3``.``0``</version>`|
|`12`|`          ``<scope>provided</scope>`|
|`13`|`</dependency>`|
然后使用spark-submit来启动你的应用程序。
　　当然，你也可以不在应用程序Jar文件中打包spark-streaming-kafka_2.10及其依赖，我们可以在spark-submit后面加上–jars参数也可以运行你的程序：
|`1`|```[iteblog@ spark]$ spark-1.3.0-bin-2.6.0/bin/spark-submit  --master yarn-cluster```|
|`2`|```--class iteblog.KafkaTest```|
|`3`|```--jars lib/spark-streaming-kafka_2.10-1.3.0.jar,```|
|`4`|`　　　　lib/spark-streaming_2.10-1.3.0.jar,`|
|`5`|`　　　　lib/kafka_2.10-0.8.1.1.jar,lib/zkclient-0.3.jar,`|
|`6`|```lib/metrics-core-2.2.0.jar ./iteblog-1.0-SNAPSHOT.jar```|
下面是一个完整的例子：
|`01`|`object````KafkaWordCount {```|
|`02`|`  ``def``main(args``:````Array[String]) {```|
|`03`|`    ``if````(args.length <````4````) {```|
|`04`|`      ``System.err.println(````"Usage: KafkaWordCount <zkQuorum> <group> <topics> <numThreads>"````)`|
|`05`|`      ``System.exit(``1``)`|
|`06`|`    ``}`|
|`07`||
|`08`|`    ``StreamingExamples.setStreamingLogLevels()`|
|`09`||
|`10`|`    ``val````Array(zkQuorum, group, topics, numThreads)````=``args`|
|`11`|`    ``val``sparkConf ``=``new``SparkConf().setAppName(``"KafkaWordCount"``)`|
|`12`|`    ``val``ssc ``=``new````StreamingContext(sparkConf, Seconds(````2``))`|
|`13`|`    ``ssc.checkpoint(``"checkpoint"``)`|
|`14`||
|`15`|`    ``val``topicMap ``=``topics.split(``","``).map((``_``,numThreads.toInt)).toMap`|
|`16`|`    ``val``lines ``=````KafkaUtils.createStream(ssc, zkQuorum, group, topicMap).map(````_``.``_``2``)`|
|`17`|`    ``val``words ``=``lines.flatMap(``_``.split(````" "````))`|
|`18`|`    ``val``wordCounts ``=``words.map(x ``=````> (x,````1``L))`|
|`19`|`      ``.reduceByKeyAndWindow(``_``+ ``_``, ``_``- ``_````, Minutes(````10````), Seconds(````2``), ``2``)`|
|`20`|`    ``wordCounts.print()`|
|`21`||
|`22`|`    ``ssc.start()`|
|`23`|`    ``ssc.awaitTermination()`|
|`24`|`  ``}`|
|`25`|`}`|
## 基于Direct API的方法
和基于Receiver接收数据不一样，这种方式定期地从Kafka的topic+partition中查询最新的偏移量，再根据定义的偏移量范围在每个batch里面处理数据。当作业需要处理的数据来临时，spark通过调用Kafka的简单消费者API读取一定范围的数据。这个特性目前还处于试验阶段，而且仅仅在Scala和Java语言中提供相应的API。
和基于Receiver方式相比，这种方式主要有一些几个优点：
（1）、**简化并行。**我们不需要创建多个Kafka 输入流，然后union他们。而使用directStream，[Spark](http://www.iteblog.com/archives/tag/spark) Streaming将会创建和Kafka分区一样的RDD分区个数，而且会从Kafka并行地读取数据，也就是说Spark分区将会和Kafka分区有一一对应的关系，这对我们来说很容易理解和使用；
（2）、**高效。**第一种实现零数据丢失是通过将数据预先保存在WAL中，这将会复制一遍数据，这种方式实际上很不高效，因为这导致了数据被拷贝两次：一次是被Kafka复制；另一次是写到WAL中。但是本文介绍的方法因为没有Receiver，从而消除了这个问题，所以不需要WAL日志；
（3）、**恰好一次语义（Exactly-once semantics）。**[《Spark Streaming和Kafka整合开发指南(一)》](http://www.iteblog.com/archives/1322)文章中通过使用Kafka高层次的API把偏移量写入Zookeeper中，这是读取Kafka中数据的传统方法。虽然这种方法可以保证零数据丢失，但是还是存在一些情况导致数据会丢失，因为在失败情况下通过Spark
 Streaming读取偏移量和Zookeeper中存储的偏移量可能不一致。而本文提到的方法是通过Kafka低层次的API，并没有使用到Zookeeper，偏移量仅仅被Spark Streaming保存在Checkpoint中。这就消除了Spark Streaming和Zookeeper中偏移量的不一致，而且可以保证每个记录仅仅被Spark Streaming读取一次，即使是出现故障。
但是本方法唯一的坏处就是没有更新Zookeeper中的偏移量，所以基于Zookeeper的Kafka监控工具将会无法显示消费的状况。然而你可以通过Spark提供的API手动地将偏移量写入到Zookeeper中。如何使用呢？其实和方法一差不多
**1、引入依赖。**
对于Scala和Java项目，你可以在你的pom.xml文件引入以下依赖：
|`1`|`<dependency>`|
|`2`|`  ``<groupId>org.apache.spark</groupId>`|
|`3`|`  ``<artifactId>spark-streaming-kafka_2.``10``</artifactId>`|
|`4`|`  ``<version>``1.3``.``0``</version>`|
|`5`|`</dependency>`|
如果你是使用SBT，可以这么引入：
|`1`|```libraryDependencies +=````"org.apache.spark"``% ``"spark-streaming-kafka_2.10"``% ``"1.3.0"`|
**2、编程**
在Streaming应用程序代码中，引入KafkaUtils ，并创建DStream输入流：
|`1`|`import``org.apache.spark.streaming.kafka.``_`|
|`2`||
|`3`|`val``directKafkaStream ``=``KafkaUtils.createDirectStream[`|
|`4`|`    ``[key ``class````], [value````class````], [key decoder````class````], [value decoder````class````] ](```|
|`5`|`    ````streamingContext, [map of Kafka parameters], [set of topics to consume])```|
在 Kafka parameters参数中，你必须指定 metadata.broker.list或者bootstrap.servers参数。在默认情况下，Spark Streaming将会使用最大的偏移量来读取Kafka每个分区的数据。如果你配置了auto.offset.reset为smallest，那么它将会从最小的偏移量开始消费。
当然，你也可以使用KafkaUtils.createDirectStream的另一个版本从任意的位置消费数据。如果你想回去每个batch中Kafka的偏移量，你可以如下操作：
|`1`|```directKafkaStream.foreachRDD { rdd````=``>`|
|`2`|`    ``val``offsetRanges ``=``rdd.asInstanceOf[HasOffsetRanges]`|
|`3`|`    ````// offsetRanges.length = # of Kafka partitions being consumed```|
|`4`|`    ``...`|
|`5`|`}`|
你可以通过这种方式来手动地更新Zookeeper里面的偏移量，使得基于Zookeeper偏移量的Kafka监控工具可以使用。
还有一点需要注意，因为这里介绍的方法没有使用到Receiver，所以Spark中关于spark.streaming.receiver.*相关的配置参数将不会对创建DStreams 有影响。我们可以使用spark.streaming.kafka.*参数进行配置。
**3、部署**
对应任何的Spark 应用，我们都是用`spark-submit`来启动你的应用程序，对于Scala和Java用户，如果你使用的是SBT或者是Maven，你可以将spark-streaming-kafka_2.10及其依赖打包进应用程序的Jar文件中，并确保spark-core_2.10和
 spark-streaming_2.10标记为provided，因为它们在Spark 安装包中已经存在：
|`01`|`<dependency>`|
|`02`|`          ``<groupId>org.apache.spark</groupId>`|
|`03`|`          ``<artifactId>spark-streaming``_``2.10``</artifactId>`|
|`04`|`          ``<version>``1.3``.``0``</version>`|
|`05`|`          ``<scope>provided</scope>`|
|`06`|`</dependency>`|
|`07`||
|`08`|`<dependency>`|
|`09`|`          ``<groupId>org.apache.spark</groupId>`|
|`10`|`          ``<artifactId>spark-core``_``2.10``</artifactId>`|
|`11`|`          ``<version>``1.3``.``0``</version>`|
|`12`|`          ``<scope>provided</scope>`|
|`13`|`</dependency>`|
然后使用spark-submit来启动你的应用程序。
