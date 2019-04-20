# 请别再问我Spark的MLlib和ML库的区别 - Spark高级玩法 - CSDN博客
2017年12月01日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：16261
# **机器学习库（MLlib）指南**
MLlib是Spark的机器学习（ML）库。其目标是使实际的机器学习可扩展和容易。在高层次上，它提供了如下工具：
- 
ML算法：通用学习算法，如分类，回归，聚类和协同过滤
- 
特征提取，特征提取，转换，降维和选择
- 
管道：用于构建，评估和调整ML管道的工具
- 
持久性：保存和加载算法，模型和管道
- 
实用程序：线性代数，统计，数据处理等
# **公告：基于DataFrame的API是主要的API**
**MLlib基于RDD的API现在处于维护模式。**
从Spark 2.0开始，包中的基于RDD的API `spark.mllib`已进入维护模式。Spark的主要机器学习API现在是包中的基于DataFrame的API `spark.ml`。
***有什么影响？***
- 
MLlib将仍然支持基于RDD的API `spark.mllib`并修复错误。
- 
MLlib不会将新功能添加到基于RDD的API。
- 
在Spark 2.x版本中，MLlib将向基于DataFrame的API添加功能，以便与基于RDD的API达成功能对等。
- 
达到功能对等（大致估计为Spark 2.2）后，基于RDD的API将被弃用。
- 
基于RDD的API预计将在Spark 3.0中被删除。
***为什么MLlib切换到基于DataFrame的API？***
- 
DataFrames提供比RDD更友好的API。DataFrame的许多优点包括Spark数据源，SQL / DataFrame查询，Tungsten和Catalyst优化以及跨语言的统一API。
- 
MLlib的基于DataFrame的API提供跨ML算法和跨多种语言的统一API。
- 
数据框便于实际的ML管线，特别是功能转换。
***什么是“Spark ML”？***
- 
“Spark ML”不是一个正式的名字，偶尔用于指代基于MLlib DataFrame的API。这主要是由于`org.apache.spark.ml`基于DataFrame的API所使用的Scala包名以及我们最初用来强调管道概念的“Spark
 ML Pipelines”术语。
***MLlib是否被弃用？***
- 
编号MLlib包括基于RDD的API和基于DataFrame的API。基于RDD的API现在处于维护模式。
# **依赖**
MLlib使用线性代数包Breeze，它依赖于 netlib-java进行优化的数值处理。如果本机库在运行时不可用，您将看到一条警告消息，而将使用纯JVM实现。
由于运行时专有二进制文件的授权问题，`netlib-java`默认情况下，我们不包含本地代理。要配置`netlib-java`/
 Breeze以使用系统优化的二进制文件，请包括 `com.github.fommil.netlib:all:1.1.2`（或者构建Spark `-Pnetlib-lgpl`）作为项目的依赖项，并阅读netlib-java文档以获取平台的其他安装说明。
要在Python中使用MLlib，您将需要NumPy 1.4或更高版本。
**推荐阅读：**
1，[Spark的Ml
 pipeline](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484270&idx=1&sn=69153063accd105649d4466d938012da&chksm=9f38e046a84f69505d954726c65d940f9a370342755ae2ce7456134ef4b9a6eafb1350a4dfb6&scene=21#wechat_redirect)
2，[干货：基于Spark
 Mllib的SparkNLP库。](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484265&idx=1&sn=670a359265edd9ae6eee9f358df35495&chksm=9f38e041a84f695749d1f54971874f10c7912840b73ab1f48af8c443b6c5ca1d806e3f3e2388&scene=21#wechat_redirect)
3，[phoenix二级索引](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484260&idx=1&sn=07a7670a2a95c3e03f988d11fdc3037e&chksm=9f38e04ca84f695ad162a8d17bbd88dcf062ca867e35177ef143c67a23c0617c8cb49eff5243&scene=21#wechat_redirect)
4，[Kafka源码系列之topic创建分区分配及leader选举](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483707&idx=1&sn=b878694a7745e1afb6bc4a5dc4529cd6&chksm=9f38e213a84f6b05f08f2665ab260570a56d96b736fef2af050e6a597f435b7b1f1da59b918f&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

