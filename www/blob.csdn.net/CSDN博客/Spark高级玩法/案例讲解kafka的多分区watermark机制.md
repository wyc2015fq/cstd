# 案例讲解kafka的多分区watermark机制 - Spark高级玩法 - CSDN博客
2019年01月29日 22:28:05[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：67
最近还是有很多球友和微信群友反馈浪尖说watermark不理解，没办法浪尖又来了。
watermark依赖的背景，可以是：
- 
事件时间，必须要在事件时间这个概念之上理解watermark。
- 
watermark更形象的理解可以理解为时间序列心跳吧，驱动流进行，而不是延迟时间，比如6s，这个不是watermark。
watermark就像心跳一样，驱动着流程序基于事件时间进行处理，watermark之所以说类似于序列心跳，是因为它携带了时间戳 t。Watermark(t)意味着算子当前的事件时间已经到达了时间 t，任何时间戳小于t的事件都不会再发过来，也即是时间戳小于t的事件就应该丢弃了。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVlrgH0OCNtnVqFhYTS4sfjKJ2R23Gsk3CFg1PPYAenGzPiaCOvlKLtISknRIhgmCwxgpIPCSZxOQw/640?wx_fmt=png)
上面的可以说是针对单并行度的流，只有一个流水线，怎么着也玩不出彩来。watermark会在每个并行度的source处或者其他算子内部添加，然后继续向前流动，假如流程序不存在shuffle那就没啥好说的了，每个单独的实例单独进行，这个要想理解要结合前面的一篇文章：
[结合Spark讲一下Flink的runtime](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485964&idx=1&sn=224f6cc89d9e824055673f5ac313d28b&chksm=9f38e924a84f603203311d7c561bfcb3d5fca7241adf3540c4b50e0efca63040add04b2b64a1&scene=21#wechat_redirect)
存在shuffle的话，就是一个算子会存在多个输入的话，当前事件时间就采用最小的事件时间。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVlrgH0OCNtnVqFhYTS4sfjmZkSzRP6bh9DDeibd6NlKhlyMNlYxqrnT7tiaGkwW5notwqb6Yg6nrDg/640?wx_fmt=png)
kafkasource
kafka作为数据源的时候，假如消费的是多个topic或者多分区，那么由于分区消费是并行进行的，会打破数据在每个分区的数据性，这个是客户端性质决定的，除非你是一个消费者对应于一个分区。这种情况下，可以使用
Kafka-partition-aware watermark生成器，该生成器会在每个kafka消费者内部，为每个分区生成watermark，最后每个分区的watermark会想Stream shuffle watermark的合并机制一样合并。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVlrgH0OCNtnVqFhYTS4sfjLUsfqV0R9dIVZW3ib56kKD6uexzNib14zibMqAOXIuPE73m7OJEV2tWcw/640?wx_fmt=png)
代码示例
FlinkKafkaConsumer09<MyType> kafkaSource = new FlinkKafkaConsumer09<>("myTopic", schema, props);
kafkaSource.assignTimestampsAndWatermarks(new AscendingTimestampExtractor<MyType>() {
@Override
publiclongextractAscendingTimestamp(MyType element){
return element.eventTimestamp();
    }
});
DataStream<MyType> stream = env.addSource(kafkaSource);
flink与kafka整合的详细代码，关注公众号回复 **w** 即可获取。
推荐阅读：
[简单理解scala的闭包](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486351&idx=1&sn=547e825d363d5dafa5f84ef2ea098d36&chksm=9f38e8a7a84f61b17ed2f4925879f3498c79fe93101778f33ceb86d8e4c941b2705db3603861&scene=21#wechat_redirect)
[不懂watermark？来吧～](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486324&idx=1&sn=b1132367e26e6da1c1982de4af436e91&chksm=9f38e85ca84f614a201eb20c5b365cfd9e93b2c2b93f1906a12181e8720afc7d0541309c1dc4&scene=21#wechat_redirect)
[Flink迭代操作末文-迭代流](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486333&idx=1&sn=fbcaf4a19752e9fd5ab5ce284058322e&chksm=9f38e855a84f6143af8dfce3715ba6900a567c2c49dad140b4e118bd0f3b76abcf5db4350163&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWD0HKRNyZbjOPNVCKh0d98G5ydFibUfnEFa6LDKoUfXU5IYnFsBG8lhxmTFJtOrruqNl2NIFQAHhw/640?wx_fmt=png)
与500位好友一起学习～
