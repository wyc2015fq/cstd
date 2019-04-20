# spark 流式去重 - Spark高级玩法 - CSDN博客
2019年02月23日 22:42:07[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：160
大数据去重本身很蛋疼，针对个别数据去重更是不可理喻但是spark的Structured Streaming就很容易能实现这个功能。听浪尖给你娓娓道来～
数据从采集到最终处理结束是会存在一条数据在某一个点被重复接收处理的情况。如 kafka支持的是至少一次写语义，也即是当写数据到kafka的时候，有些记录可能重复，例如如果消息已经被broker接收并写入文件但是并没有应答，这时生产者向kafka重发一个消息，就可能重复。由于kafka的至少一次的写语义，structured streaming不能避免这种类型数据重复。所以一旦写入成功，可以假设structured Streaming的查询输出是以至少一次语义写入kafka的。一个可行去除重复记录的解决方案是数据中引入一个primary(unique)key，这样就可以在读取数据的时候实行去重。
structured streaming是可以使用事件中的唯一标识符对数据流中的记录进行重复数据删除。这与使用唯一标识符列的静态重复数据删除完全相同。该查询将存储来自先前记录的一定量的数据，以便可以过滤重复的记录。与聚合类似，可以使用带有或不带有watermark 的重复数据删除功能。
A),带watermark：如果重复记录可能到达的时间有上限，则可以在事件时间列上定义watermark，并使用guid和事件时间列进行重复数据删除。
B),不带watermark：由于重复记录可能到达时间没有界限，所以查询将来自所有过去记录的数据存储为状态。
源代码，已测试通过～
```
package bigdata.spark.StructuredStreaming.KafkaSourceOperator
import org.apache.spark.SparkConf
import org.apache.spark.sql.SparkSession
import org.apache.spark.sql.functions.get_json_object
import org.apache.spark.sql.streaming.{OutputMode, Trigger}
object KafkaDropDuplicate {
  def main(args: Array[String]): Unit = {
    val sparkConf = new SparkConf().setAppName(this.getClass.getName).setMaster("local[*]")
      .set("yarn.resourcemanager.hostname", "mt-mdh.local")
      .set("spark.executor.instances","2")
      .set("spark.default.parallelism","4")
      .set("spark.sql.shuffle.partitions","4")
      .setJars(List("/opt/sparkjar/bigdata.jar"
        ,"/opt/jars/spark-streaming-kafka-0-10_2.11-2.3.1.jar"
        ,"/opt/jars/kafka-clients-0.10.2.2.jar"
        ,"/opt/jars/kafka_2.11-0.10.2.2.jar"
        ,"/opt/jars/spark-sql-kafka-0-10_2.11-2.0.2.jar"))
    val spark = SparkSession
      .builder
      .appName("StructuredKafkaWordCount")
      .config(sparkConf)
      .getOrCreate()
    import spark.implicits._
    val df = spark
      .readStream
      .format("kafka")
      .option("kafka.bootstrap.servers","mt-mdh.local:9093")
      .option("subscribe", "jsontest")
      .load()
    val words = df.selectExpr("CAST(value AS STRING)")
    val fruit = words.select(
      get_json_object($"value", "$.time").alias("timestamp").cast("long")
      , get_json_object($"value", "$.fruit").alias("fruit"))
    val fruitCast = fruit
      .select(fruit("timestamp")
        .cast("timestamp"),fruit("fruit"))
      .withWatermark("timestamp", "10 Seconds")
      .dropDuplicates("fruit")
      .groupBy("fruit").count()
    fruitCast.writeStream
      .outputMode(OutputMode.Complete())
      .format("console")
      .trigger(Trigger.ProcessingTime(5000))
      .option("truncate","false")
      .start()
      .awaitTermination()
  }
}
```
疯狂的2019，欢迎和570位球友一起学习～
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWD0HKRNyZbjOPNVCKh0d98G5ydFibUfnEFa6LDKoUfXU5IYnFsBG8lhxmTFJtOrruqNl2NIFQAHhw/640?wx_fmt=png)
