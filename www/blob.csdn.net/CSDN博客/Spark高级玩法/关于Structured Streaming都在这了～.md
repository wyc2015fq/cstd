# 关于Structured Streaming都在这了～ - Spark高级玩法 - CSDN博客
2019年01月04日 23:28:53[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：134
最近私聊浪尖的粉丝好多都在问：流处理学flink还是spark？
**显而易见，都要学～**
**以前发过一篇对比flink和spark的文章，这周末再重新整理一下，给大家更加清晰的对比。**
Spark的流式处理总共分两个模块Spark Streaming 和 Structured Streaming。
Spark Streaming 是基于RDD的微批处理(>200ms)，数据可以是结构化的也可以是非结构化的，虽然是微批处理但是他也提供了有状态的算子，比如upstatebykey，这个会维护中间值在内存中。Spark Streaming也允许你自己管理状态的存储位置，常用的有redis，alluxio，这样你可以管理地更加精细。
**Spark 2.4未对spark streaming更新，重心移到了Structured streaming。**
Structured streaming也是微批处理，只是其批次可以更小(~1ms),底层依赖于Spark SQL，主要针对结构化数据。
Structured streaming状态管理，比如聚合操作，join操作等都已经帮你做了，当然你也可以用flatMapGroupsWithState和mapGroupsWithState来进行一些状态操作。
对于处理的触发执行，Structured Streaming支持仅一次出发，周期性触发和连续不断触发。
对于处理时间的概念，更是增加了事件时间和窗口时间，同时支持窗口操作按照事件时间执行，支持watermark机制，以便删除过期数据。
可以像传统数据库那样根据主键删除重复数据。
*等等。。。*
浪尖前段时间整理了一些列的Structured Streaming文章和源码，今天已经分享到知识星球。
球友可以去星球获取～
欢迎加入浪尖知识星球，与450+球友一起学习。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVNctsRdhXREZicwK2pphjoShx62P6YoG0h0CU7l8jJ0FltzqPRDicy9zXR4dJqJtn0FEIich3fl8UvA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVNctsRdhXREZicwK2pphjoSU6EGZIuDaMwQg65B9lkBLKBQ3z2DKNlGnGgR9mSPPuNcsynNX6j64A/640?wx_fmt=png)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUibDu2gaX89M600Ip3MDrVY0ibqsWg1gHuTdGCX1QfbyyCPlf31gLQna9OvNyRsBaByXsZrZeqibtVQ/640?wx_fmt=jpeg)
