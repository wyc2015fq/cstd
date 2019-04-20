# Spark SQL用UDF实现按列特征重分区 - Spark高级玩法 - CSDN博客
2018年11月15日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：222
欢迎关注，浪尖公众号，**bigdatatip**，建议置顶。
这两天，球友又问了我一个比较有意思的问题:
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUxSPIibibOd4NhB1eXQbLaRyNQ2D4EdlRFs6Xge7HaZNstkAmdycCIfapcXWJ2AAQ2a2s9zVkSa1uw/640)
解决问题之前，要先了解一下Spark 原理，要想进行相同数据归类到相同分区，肯定要有产生shuffle步骤。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUxSPIibibOd4NhB1eXQbLaRyX9OSiboyJmibDod7DfHekQP9Ev83kEaLplHUzsf7gsmYZhbFoLL7aQrw/640)
比如，F到G这个shuffle过程，那么如何决定数据到哪个分区去的呢？这就有一个分区器的概念，默认是hash分区器。
假如，我们能在分区这个地方着手的话肯定能实现我们的目标。
那么，在没有看Spark Dataset的接口之前，浪尖也不知道Spark Dataset有没有给我门提供这种类型的API，抱着试一试的心态，可以去Dataset类看一下，这个时候会发现有一个函数叫做repartition。
/**
   * Returns a new Dataset partitioned by the given partitioning expressions, using
   * `spark.sql.shuffle.partitions` as number of partitions.
   * The resulting Dataset is hash partitioned.
   *
   * This is the same operation as "DISTRIBUTE BY" in SQL (Hive QL).
   *
   * @group typedrel
   * @since 2.0.0
   */
  @scala.annotation.varargs
  def repartition(partitionExprs: Column*): Dataset[T] = {
repartition(sparkSession.sessionState.conf.numShufflePartitions, partitionExprs: _*)
  }
可以传入列表达式来进行重新分区，产生的新的Dataset的分区数是由参数spark.sql.shuffle.partitions决定，那么是不是可以满足我们的需求呢？
明显，直接用是不行的，可以间接使用UDF来实现该功能。
**方式一-简单重分区**
首先，实现一个UDF截取列值共同前缀，当然根据业务需求来写该udf
val substring = udf{(str: String) => {
      str.substring(0,str.length-1)
    }}
注册UDF
spark.udf.register("substring",substring)
创建Dataset
val sales = spark.createDataFrame(Seq(
      ("Warsaw1", 2016, 100),
      ("Warsaw2", 2017, 200),
      ("Warsaw3", 2016, 100),
      ("Warsaw4", 2017, 200),
      ("Beijing1", 2017, 200),
      ("Beijing2", 2017, 200),
      ("Warsaw4", 2017, 200),
      ("Boston1", 2015, 50),
      ("Boston2", 2016, 150)
    )).toDF("city", "year", "amount")
执行充分去操作
val res = sales.repartition(substring(col("city")))
打印分区ID及对应的输出结果
res.foreachPartition(partition=>{
println("---------------------> Partition start ")
println("partitionID is "+TaskContext.getPartitionId())
      partition.foreach(println)
println("=====================> Partition stop ")
    })
浪尖这里spark.sql.shuffle.partitions设置的数值为10.
输出结果截图如下：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUxSPIibibOd4NhB1eXQbLaRyNmM1WnPu6UzpT0fOr1r5OF9FppBic8FJFufPib5m6T0qKo9S6Kiau0kbQ/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUxSPIibibOd4NhB1eXQbLaRyGB464ACCj61nv8LfSZib1SPKxia2btPetE1RtjEZSHZTrd0Cvic3q3zEA/640)
**方式二-SQL实现**
对于Dataset的repartition产生的shuffle是不需要进行聚合就可以产生shuffle使得按照字段值进行归类到某些分区。
SQL的实现要实现重分区要使用group by，然后udf跟上面一样，需要进行聚合操作。
完整代码如下：
val sales = spark.createDataFrame(Seq(
      ("Warsaw1", 2016, 100),
      ("Warsaw2", 2017, 200),
      ("Warsaw3", 2016, 100),
      ("Warsaw4", 2017, 200),
      ("Beijing1", 2017, 200),
      ("Beijing2", 2017, 200),
      ("Warsaw4", 2017, 200),
      ("Boston1", 2015, 50),
      ("Boston2", 2016, 150)
    )).toDF("city", "year", "amount")
    sales.registerTempTable("temp");
    val substring = udf{(str: String) => {
      str.substring(0,str.length-1)
    }}
    spark.udf.register("substring",substring)
    val res = spark.sql("select sum(amount) from temp group by substring(city)")
//
    res.foreachPartition(partition=>{
      println("---------------------> Partition start ")
      println("partitionID is "+TaskContext.getPartitionId())
      partition.foreach(println)
      println("=====================> Partition stop ")
    })
输出结果如下：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUxSPIibibOd4NhB1eXQbLaRyyqRSiazfq0ia1qTtFo5OEBtazFQbWPh6VicXtDkAwUvg2fQEBIPJSibtMg/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUxSPIibibOd4NhB1eXQbLaRyW3acm9SzWfRXcy45Iwmz3r3EYX91jsTgMDzTicRkNJwBUB9JkHJZYqA/640)
由上面的结果也可以看到task执行结束时间是无序的。
浪尖在这里主要是讲了Spark SQL 如何实现按照自己的需求对某列重分区。
那么，浪尖在这里就顺带问一下，如何用Spark Core实现该功能呢？
推荐阅读：
[spark源码导读一](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485775&idx=1&sn=cfe2375410c242ff06512d0586b239b0&chksm=9f38ea67a84f6371fb5989b0dd3c3957fbe0a0a5d7ad58a2717264d9771c81693b9ac023132c&scene=21#wechat_redirect)
[Spark SQL从入门到精通](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485504&idx=1&sn=e19460f43aa2fe388140a2241e9ea219&chksm=9f38eb68a84f627ee0b6dac115f0684dddba573be04ae7e4e54d17c084cf1946282f81b1fd43&scene=21#wechat_redirect)
[kafka连接器两种部署模式详解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484229&idx=1&sn=d5e87da8a05c40b617f343044ba6d808&chksm=9f38e06da84f697b5181c1aedbeb9205e3af0a81dde6d19487cbde803e01d86fb9367b3a3b3b&scene=21#wechat_redirect)
[Kafka源码系列之0.10版本的Producer源码解析及性能点讲解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483747&idx=1&sn=ba7bb28c7b8eacd607ffe6f9afbe7e2b&chksm=9f38e24ba84f6b5dd752e7fced79ad6cad9370e4356a39524272a0805eef5f50674534cca31c&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
欢迎点赞，转发，星球入口点击原文阅读。
