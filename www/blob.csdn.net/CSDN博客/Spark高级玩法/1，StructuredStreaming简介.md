# 1，StructuredStreaming简介 - Spark高级玩法 - CSDN博客
2018年03月16日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：592
一，概述
Structured Streaming是一个可扩展和容错的流处理引擎，并且是构建于sparksql引擎之上。你可以用处理静态数据的方式去处理你的流计算。随着流数据的不断流入，Sparksql引擎会增量的连续不断的处理并且更新结果。可以使用DataSet/DataFrame的API进行 streaming aggregations, event-time windows, stream-to-batch joins等等。计算的执行也是基于优化后的sparksql引擎。通过checkpointing and Write Ahead Logs该系统可以保证点对点，一次处理，容错担保。
可以把输入的数据流当成一张表。数据流新增的每一条数据就像添加到该表的新增行数据。
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUicjyYcWLIb342Ue1Dp4oVoj8WXIEDD8X39gJ8oaP7GmibpfZTpaPR2J5klK47bgiae4chx4u0oiahSw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
在输入数据流上执行的query操作会生成一个结果表。每个触发间隔，比如1s，新的行都会被追加到输入表，最终更新结果表。结果表无论何时得到更新，都将会将变化的结果行写入外部的sink。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUicjyYcWLIb342Ue1Dp4oVoPYIQeibo64SnuJQG99o8plcV0qiaicWeS0UHUuTyK83er0ozalR8z3EIA/640?wx_fmt=png)
二，例子和概念
1， SocketSource-> console sink
下载安装nc，请不要用yum直接安装。
wget  http://vault.centos.org/6.6/os/x86_64/Packages/nc-1.84-22.el6.x86_64.rpm
rpm   -iUv nc-1.84-22.el6.x86_64.rpm
启动nc，然后准备写入数据到Structured Streaming
nc -lk 9999
启动nc之后，开始启动spark-shell
Spark-shell –master local[*]
执行如下代码：
val lines = spark.readStream.format("socket").option("host", "localhost").option("port", 9999).load()
val words = lines.as[String].flatMap(_.split(" "))
val wordCounts = words.withWatermark("timestamp", "30 seconds").groupBy("value").count()
val query = wordCounts.writeStream.outputMode("Update").format("console").start()
query.awaitTermination()
2，编程模型讲解
输入的第一行是生成了一个linesDataFrame，然后作为输入表。最终wordCounts DataFrame是结果表。基于linesDataFrame的查询跟静态的Dataframe查询时一样的。然而，当查询一旦启动，Spark会不停的检查Socket链接是否有新的数据。如果有新的数据，Spark将会在新数据上运行一个增量的查询，并且组合之前的counts结果，计算得到更新后的统计。
**3，**重点介绍的两个概念：source和sink。
3．1source
目前支持的source有三种：
File Sourcec:从给定的目录读取数据，目前支持的格式有text,csv,json,parquet。容错。
Kafka Source:从kafka拉取数据。仅兼容kafka 0.10.0或者更高版本。容错。
Socket Source(for testing):从一个连接中读取UTF8编码的文本数据。不容错。
3.2 output modes与查询类型
Append mode(default):仅仅从上次触发计算到当前新增的行会被输出到sink。仅仅支持行数据插入结果表后不进行更改的query操作。因此，这种方式能保证每行数据仅仅输出一次。例如，带有Select，where，map，flatmap，filter，join等的query操作支持append模式。
Complete mode:每次trigger都会将整个结果表输出到sink。这个是针对聚合操作的。
Updata mode:仅仅是自上次trigger之后结果表有变更的行会输出到sink。在以后的版本中会有更详细的信息。
不同类型的Streaming query支持不同的输出模式。
|Query Type|支持的输出模式|注释| |
|----|----|----|----|
|Queries with aggregation|Aggregation on event-time with watermark|Append, Update, Complete|Append mode和Update mode采用watermark来drop掉历史的聚合状态。Completemode不会删除历史聚合状态。|
|Other aggregations|Complete, Update|由于没有定义watermark，旧的聚合状态不会drop。Append mode不支持因为聚合操作是违反该模式的含义的。| |
|Queries with mapGroupsWithState|Update|| |
|Queries with flatMapGroupsWithState|Append operation mode|Append|flatMapGroupsWithState之后运行Aggregations|
|Update operation mode|Update|flatMapGroupsWithState之后不允许Aggregations| |
|Other queries|Append, Update|Complete mode不支持这种模式的原因是在结果表保留所有的非聚合的数据是不合适的。| |
3.3 sinks
FileSink:保存数据到指定的目录
noAggDF
  .writeStream
  .format("parquet")
  .option("checkpointLocation", "path/to/checkpoint/dir")
  .option("path", "path/to/destination/dir")
  .start()
Foreach sink:在输出的数据上做任何操作。
writeStream
  .foreach(...)
.start()
Console sink(for debugging)：每次trigger都会讲结果输出到console或stdout。
aggDF
  .writeStream
  .outputMode("complete")
  .format("console")
  .start()
memory sink
// Have all the aggregates in an in-memory table
aggDF
  .writeStream
  .queryName("aggregates")    // this query name will be the table name
  .outputMode("complete")
  .format("memory")
  .start()
spark.sql("select * from aggregates").show()
kafkasink
支持stream和batch数据写入kafka
valds = df
  .selectExpr("CAST(key AS STRING)", "CAST(value AS STRING)")
  .writeStream
  .format("kafka")
  .option("kafka.bootstrap.servers", "host1:port1,host2:port2")
  .option("topic", "topic1")
  .start()
Sink支持的输出模式
|Sink|Outputmode|Options|容错|注释|
|----|----|----|----|----|
|FileSink|Append|path:输出路径，必须指定|Yes|支持写入分区的tables。按照时间分区或许是有用的。|
|ForeachSink|Append，Update，Complete|None|依赖于于ForeachWriter的实现|细节请看官网|
|ConsoleSink|Append，Complete，Update|NumRows：每个trigger显示的行数。Truncate：假如太长是否删除，默认是true|No||
|MemorySink|Append,Complete|None|No.但是在Completemode 重新query就会导致重新创建整张表|后续sql使用的表明就是queryName|
以上是全部概念。
三 注意事项
StructuredStreaming不会管理整个输入表。它会从Streaming数据源中读取最近的可用数据，然后增量的处理它并更新结果，最后废弃源数据。它仅仅会保留很小更新结果必要的中间状态数据。
这种模型更很多其他的流处理引擎不一样。很多其他流处理系统需要用户自己保持聚合状态，所以还需要考虑容错和数据一致性(at-least-once, or at-most-once, or exactly-once)。在这种模型里面，在有新数据的时候spark负责更新结果表。
**推荐阅读：**
1，[案例：Spark基于用户的协同过滤算法](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484291&idx=1&sn=4599b4e31c2190e363aa379a92794ace&chksm=9f38e0aba84f69bd5b78b48e31b3f5b3792ec40e2d25fdbe6bc735f9c98ceb4584462b08e439&scene=21#wechat_redirect)
2，[Spark度量系统相关讲解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483941&idx=1&sn=aa6e5490363db569222806af563c2940&chksm=9f38e10da84f681b34cb4bebf769e3fa8f711475aa7345d2f9fdd867e8f5b1f1a5fa0a172f3f&scene=21#wechat_redirect)
3，[kafka连接器两种部署模式详解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484229&idx=1&sn=d5e87da8a05c40b617f343044ba6d808&chksm=9f38e06da84f697b5181c1aedbeb9205e3af0a81dde6d19487cbde803e01d86fb9367b3a3b3b&scene=21#wechat_redirect)
4，[基于zookeeper leader选举方式一](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483890&idx=1&sn=95938c9d25e2273129798acefa08f0c1&chksm=9f38e2daa84f6bcc7212262354735617c1a5f14eb9738e7a658930a261e068b1f97cd2dff27c&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
