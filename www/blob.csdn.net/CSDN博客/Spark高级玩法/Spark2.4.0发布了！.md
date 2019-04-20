# Spark2.4.0发布了！ - Spark高级玩法 - CSDN博客
2018年11月10日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：725
Spark2.4.0 今天官网发布，这是一个大好消息。
Spark 2.4.0是2.x的第五个发型版本。官方发布消息链接如下：
http://spark.apache.org/releases/spark-release-2-4-0.html
此版本继续关注可用性，稳定性和优化，浪尖在这里摘要翻译一下，主要的关注点：
**SparkCore ****和 SQL **
- 
增加了Barrier ExecutionMode，可以更好的和深度学习的框架整合
- 
同时引入了 30+ 内置函数和 higher-order函数可以处理更复杂的数据类型
- 
改善了与k8s的整合
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVvDUwRxgObLdfLPncPugYF96KCRxlBG7uuNN6LtqEz7CfmCU9ezKYHHTZFkFt7ZzMwEbDZaH9lLw/640)
- 
支持scala 2.12
- 
内置支持了Avro格式数据源，这个感情好，后面浪尖给出测试案例，以后有pb的支持那就更好了。
**性能和稳定性**
这个优化比较多，值得关注
- 
Connector优化
这个优化主要是Parquet，orc，csv及avro等的优化升级
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVvDUwRxgObLdfLPncPugYFcrrEe8qoChtevRNjn9Vfm25nhj5N4p1lnYVR0jon48HoH1Pcg4e5RQ/640)
**MLlib**
- 
MLlib支持了图像格式的数据源
**StructuredStreaming**
- 
使用foreachBatch（支持Python，Scala和Java）将每个微批的输出行暴露为DataFrame。
- 
为Python API 增加了foreach 和 ForeachWriter
- 
支持使用“kafka.isolation.level”读取使用事务的生产者生产到kafka topic的已提交消息。
Spark SQL的升级页面里也有对Spark 2.4 在 SQL 方面的调整优化，大家有兴趣也可以看看，有没有自己关注的bug被修复了。
http://spark.apache.org/docs/latest/sql-migration-guide-upgrade.html#upgrading-from-spark-sql-23-to-24
细心的同学注意到了，Spark Streaming已经稳定到不用更新了，，，还是说Spark Streaming已经凉凉了。
推荐阅读：
[spark源码导读一](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485775&idx=1&sn=cfe2375410c242ff06512d0586b239b0&chksm=9f38ea67a84f6371fb5989b0dd3c3957fbe0a0a5d7ad58a2717264d9771c81693b9ac023132c&scene=21#wechat_redirect)
[解惑:这个SPARK任务是数据倾斜了吗？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485791&idx=1&sn=52ab8a2ab64c82f1c70287d0a69bf604&chksm=9f38ea77a84f636155f724ad9a94486ede70ffbc3ff4f3de4a99cb8092a797a9e2fa91df707b&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
**欢迎转发，点赞**
