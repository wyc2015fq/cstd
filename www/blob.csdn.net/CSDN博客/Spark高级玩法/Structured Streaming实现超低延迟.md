# Structured Streaming实现超低延迟 - Spark高级玩法 - CSDN博客
2018年10月19日 00:00:54[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：363
浪院长，最近忙死了，写文章的时间都没了。但是，都说时间就像海绵里的水，挤挤就有了。所以，今晚十点半开始整理这篇Structured streaming 相关的文章。
最近，忙于开发完善flink平台，并且使用我们的平台去支持一些复杂的业务，比如用户画像处理等。遇见了很多bug和性能点，后面陆续出文章给大家解析。
书归正传，大家都知道spark streaming是微批批处理，而Structured streaming在2.3以前也是批处理，在2.3引入了连续处理的概念，延迟大幅度降低值～1ms，但是还有诸多限制，这点比flink差了许多。
至于低延迟的测试，建议本文使用本文代码去测试，kafka source->kafka sink，这样便于观察延迟。
连续处理是Spark 2.3中引入的一种新的实验版本流执行模式，可实现极低（~1 ms）端到端延迟，并且具有至少一次处理容错保证。 structured streaming的连续处理模式与微批处理模式进行比较，微批处理引擎可以实现一次性保证，但微批处理最好仅可实现约100ms的延迟。 对于某些类型的查询（在下面讨论），可以选择在不修改应用代码的情况下运行该模式（即，不更改DataFrame / Dataset操作）。
要在连续处理模式下运行支持的查询，您只需指定一个连续触发器，并将所需的checkpoint间隔作为参数。 例如浪尖的demo如下：
object ContinuousProcessing {
def main(args: Array[String]): Unit = {
    val sparkConf = new SparkConf().setAppName(this.getClass.getName).setMaster("yarn-client")
      .set("yarn.resourcemanager.hostname", "mt-mdh.local")
      .set("spark.executor.instances","2")
      .set("spark.default.parallelism","4")
      .set("spark.sql.shuffle.partitions","4")
      .setJars(List("/Users/meitu/Desktop/sparkjar/bigdata.jar"
        ,"/opt/jars/spark-streaming-kafka-0-10_2.11-2.3.1.jar"
        ,"/opt/jars/kafka-clients-0.10.2.2.jar"
        ,"/opt/jars/kafka_2.11-0.10.2.2.jar"
        ,"/opt/jars/spark-sql-kafka-0-10_2.11-2.0.2.jar"))
    val spark = SparkSession
      .builder
      .appName("StructuredKafkaWordCount")
      .config(sparkConf)
      .getOrCreate()
    spark
      .readStream
      .format("kafka")
      .option("kafka.bootstrap.servers", "mt-mdh.local:9093")
      .option("subscribe", "StructuredSource")
      .load()
      .selectExpr("CAST(key AS STRING)", "CAST(value AS STRING)")
      .writeStream
      .format("kafka")
      .option("kafka.bootstrap.servers", "mt-mdh.local:9093")
      .option("topic", "StructuredSink")
      .option("checkpointLocation","/sql/checkpoint")
      .trigger(Trigger.Continuous("1 second"))  // only change in query
      .start()
      .awaitTermination()
  }
}
checkpoint 间隔为1秒意味着连续处理引擎将每秒记录查询的进度。 生成的checkpoint采用与微批处理引擎兼容的格式，因此可以使用任何触发器重新启动任何查询。 例如，假如查询支持微批处理和连续处理，那么实际上也可以用连续处理触发器去启动微批处理触发器，反之亦然。 
请注意，无论何时切换到连续模式，都将获得至少一次的容错保证。
**支持的查询**
从Spark 2.3开始，连续处理模式仅支持以下类型的查询。
- 
Operations：在连续模式下仅支持dataset/dataframe的类似于map的操作，即支持projection（select，map，flatMap，mapPartitions等）和selection（where，filter等）。
- 
除了聚合函数（因为尚不支持聚合），current_timestamp（）和current_date（）（使用时间的确定性计算具有挑战性）之外，支持所有SQL函数。
**Sources**
- 
Kafka Source：支持所有操作。
- 
Rate source：适合测试。只有连续模式支持的选项是numPartitions和rowsPerSecond。
**Sinks**
- 
Kafka sink：支持所有选项。
- 
Memory sink：适合调试。
- 
Console sink：适合调试。支持所有操作。请注意，控制台将打印你在连续触发器中指定的每个checkpoint间隔。
更详细的关于sink和source信息，请参阅输入源和输出接收器部分的官网。虽然控制台接收器非常适合测试，但是使用Kafka作为源和接收器可以最好地观察到端到端的低延迟处理。
**注意事项**
- 
连续处理引擎启动多个长时间运行的任务，这些任务不断从源中读取数据，处理数据并连续写入接收器。 查询所需的任务数取决于查询可以并行从源读取的分区数。 因此，在开始连续处理查询之前，必须确保群集中有足够的核心并行执行所有任务。 例如，如果您正在读取具有10个分区的Kafka主题，则群集必须至少具有10个核心才能使查询正常执行。
- 
停止连续处理流可能会产生虚假的任务终止警告。 这些可以安全地忽略。
- 
目前没有自动重试失败的任务。 任何失败都将导致查询停止，并且需要从检查点手动重新启动。(深受其害，kafka topic没数据流入也会挂掉的)
推荐阅读：
[推荐两个不错的flink项目](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485652&idx=1&sn=170ece64334979fc5ea5fc5628a33759&chksm=9f38ebfca84f62ea0c1aaaeb8410ec15aec31b1122558662175aa70a2d953f9a3f0e6e904c8f&scene=21#wechat_redirect)
[重要 : 优化flink的四种方式](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485344&idx=1&sn=cde8915c6983dcfe541043cd6db8dbe6&chksm=9f38e488a84f6d9eb7b961a2c4fa05f2bd85fa82df81e4d8e56d0c22d86ff413a0baaacea01e&scene=21#wechat_redirect)
[Spark SQL从入门到精通](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485504&idx=1&sn=e19460f43aa2fe388140a2241e9ea219&chksm=9f38eb68a84f627ee0b6dac115f0684dddba573be04ae7e4e54d17c084cf1946282f81b1fd43&scene=21#wechat_redirect)
[干货:Flink+Kafka 0.11端到端精确一次处理语义实现](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485150&idx=1&sn=ba0570a9f631501a189d8184807d518b&chksm=9f38e5f6a84f6ce0dd805eae87d6c62e2c96670f14c2bac1607961614ba49f310a4630e31a3b&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
