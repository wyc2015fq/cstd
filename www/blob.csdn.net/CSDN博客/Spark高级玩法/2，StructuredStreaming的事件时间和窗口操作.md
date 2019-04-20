# 2，StructuredStreaming的事件时间和窗口操作 - Spark高级玩法 - CSDN博客
2018年05月12日 08:18:55[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1475
推荐阅读：[1，StructuredStreaming简介](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484544&idx=1&sn=804700ecc8e47a03adf2367dcabe19f0&chksm=9f38e7a8a84f6ebe255725fb5a3f60f5e61682968979b0f2b3e825a4a76b369e14b0f976bd12&scene=21#wechat_redirect)
使用Structured Streaming基于事件时间的滑动窗口的聚合操作是很简单的，很像分组聚合。在一个分组聚合操作中，聚合值被唯一保存在用户指定的列中。在基于窗口的聚合的情况下，对于行的事件时间的每个窗口，维护聚合值。
如前面的例子，我们运行wordcount操作，希望以10min窗口计算，每五分钟滑动一次窗口。也即，12:00 - 12:10, 12:05 - 12:15, 12:10 - 12:20 这些十分钟窗口中进行单词统计。12:00 - 12:10意思是在12:00之后到达12:10之前到达的数据，比如一个单词在12:07收到。这个单词会影响12:00 - 12:10, 12:05 - 12:15两个窗口。
结果表将如下所示。
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUM7VjQtqKt8icdRaV4GkleXJaFpd1sfGicfs9rQ7GdDzgJUCJtd5fXrOicqic68ibVNVS3Fo3EJLlRSbA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
import org.apache.spark.sql.streaming.Trigger
import java.sql.Timestamp
import org.apache.spark.sql.functions._
import spark.implicits._
val lines = spark.readStream.format("socket").option("host", "127.0.0.1").option("port", 9999).option("includeTimestamp", true).load()
val words = lines.as[(String, Timestamp)].flatMap(line =>line._1.split(" ").map(word => (word, line._2))).toDF("word", "timestamp")
val windowedCounts = words.withWatermark("timestamp", "30 seconds").groupBy(window($"timestamp", "30 seconds", "15 seconds"), $"word").count()
val query = windowedCounts.writeStream.outputMode("Append").format("console").trigger(Trigger.ProcessingTime(5000)).option("truncate", "false").start()
query.awaitTermination()
推荐阅读：
[Spark Structured Streaming高级特性](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483938&idx=1&sn=0350e49c3f86f0ec2e196dfdf9506e22&chksm=9f38e10aa84f681c06d069690b8ddd949432d8e46817e5a6ff366ac128139b935b800238a9ad&scene=21#wechat_redirect)
[Spark Streaming 中管理 Kafka Offsets 的几种方式](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484684&idx=1&sn=3c8f42b750574ce296f4870c4f0f72aa&chksm=9f38e624a84f6f329823812eea4d2c419e53a8a73713730228a281252e2274d55416d0f9e726&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
