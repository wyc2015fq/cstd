# 3，Structured Streaming使用checkpoint进行故障恢复 - Spark高级玩法 - CSDN博客
2018年05月13日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：868
使用checkpoint进行故障恢复
如果发生故障或关机，可以恢复之前的查询的进度和状态，并从停止的地方继续执行。这是使用Checkpoint和预写日志完成的。您可以使用检查点位置配置查询，那么查询将将所有进度信息（即，每个触发器中处理的偏移范围）和运行聚合（例如，示例中的wordcount）保存到检查点位置。此检查点位置必须是HDFS兼容文件系统中的路径，并且可以在启动查询时将其设置为DataStreamWriter中的选项。
aggDF
  .writeStream
  .outputMode("complete")
  .option("checkpointLocation", "path/to/HDFS/dir")
  .format("memory")
  .start()
具体测试代码如下：
val lines = spark.readStream.format("socket").option("host", "localhost").option("port", 9999).load()
val words = lines.as[String].flatMap(_.split(" "))
val wordCounts = words.groupBy("value").count()
val query = wordCounts.writeStream.queryName("aggregates").outputMode("complete").option("checkpointLocation", "memory/").format("memory").start()
spark.sql("select * from aggregates").show()
kill掉submit进行测试
**推荐阅读：**
1，[2，StructuredStreaming的事件时间和窗口操作](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484821&idx=1&sn=5a1df85158c2aaf521f69080b174e9d9&chksm=9f38e6bda84f6fabda5a3e168a404eb83ba0ac39d31c6d3fc91a9976eef0310f2549c6f373d5&scene=21#wechat_redirect)
2，[译：Tensorflow实现的CNN文本分类](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484852&idx=1&sn=413e1a19896ee48cf3822fcfef466c29&chksm=9f38e69ca84f6f8af6f953c45254382cc0298764b6cf19e473b8c74c6c5e210ce4a77a945af3&scene=21#wechat_redirect)
3，[机器学习之学习率 Learning Rate](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484813&idx=1&sn=9992148e6669fb9ee205b2ee3b726623&chksm=9f38e6a5a84f6fb376b0bdffefe26790662d44a2830d1b708ff48872a431565520b5d1ea7f15&scene=21#wechat_redirect)
