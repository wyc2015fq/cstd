# 最常见的Kafka面试题及答案 - Spark高级玩法 - CSDN博客
2018年09月28日 00:01:47[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：684
本文为您盘点最常见的Kafka面试题，同时也是对Apache Kafka初学者必备知识点的一个整理与介绍。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFVJQsKVMzaiawcYSQRxPoUMaib2kefLic8pMRqrxgZnKxr5LIWHa4CFCWBvYktO8cWRDFOHUTtkhuaKA/640)
1、请说明什么是Apache Kafka?
Apache Kafka是由Apache开发的一种发布订阅消息系统，它是一个分布式的、分区的和重复的日志服务。
2、请说明什么是传统的消息传递方法?
传统的消息传递方法包括两种：
- 
排队：在队列中，一组用户可以从服务器中读取消息，每条消息都发送给其中一个人。
- 
发布-订阅：在这个模型中，消息被广播给所有的用户。
3、请说明Kafka相对传统技术有什么优势?
Apache Kafka与传统的消息传递技术相比优势之处在于：
快速:单一的Kafka代理可以处理成千上万的客户端，每秒处理数兆字节的读写操作。
可伸缩:在一组机器上对数据进行分区和简化，以支持更大的数据
持久:消息是持久性的，并在集群中进行复制，以防止数据丢失。
设计:它提供了容错保证和持久性
4、在Kafka中broker的意义是什么?
在Kafka集群中，broker术语用于引用服务器。
5、Kafka服务器能接收到的最大信息是多少?
Kafka服务器可以接收到的消息的最大大小由参数message.max.bytes决定，010版本默认值是1000012，可以配置为broker级别或者topic级别。
6、解释Kafka的Zookeeper是什么?我们可以在没有Zookeeper的情况下使用Kafka吗?
Zookeeper是一个开放源码的、高性能的协调服务，它用于Kafka的分布式应用。
不，不可能越过Zookeeper，直接联系Kafka broker。一旦Zookeeper停止工作，它就不能服务客户端请求。
- 
Zookeeper主要用于在集群中不同节点之间进行通信
- 
在Kafka中，它被用于提交偏移量，因此如果节点在任何情况下都失败了，它都可以从之前提交的偏移量中获取
- 
除此之外，它还执行其他活动，如: leader检测、分布式同步、配置管理、识别新节点何时离开或连接、集群、节点实时状态等等。
[Kafka源码系列之源码分析zookeeper在kafka的作用](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484900&idx=1&sn=95577024ba531e6743d06e8c550662a3&chksm=9f38e6cca84f6fda5a37dd6a4f8d858a108a5bbfb802eeb4ed24f1a152a5f60b600e0fca125d&scene=21#wechat_redirect)
7、解释Kafka的用户如何消费信息?
在Kafka中传递消息是通过使用sendfile API完成的。它支持将字节从套接口转移到磁盘，通过内核空间保存副本，并在内核用户之间调用内核。
消费者消费有各种客户端：
010: http://kafka.apache.org/0102/javadoc/index.html?org/apache/kafka/clients/consumer/KafkaConsumer.html
082 分高阶API和低阶API：
https://cwiki.apache.org/confluence/display/KAFKA/Consumer+Group+Example
https://cwiki.apache.org/confluence/display/KAFKA/0.8.0+SimpleConsumer+Example
8、解释如何提高远程用户的吞吐量?
如果用户位于与broker不同的数据中心，则可能需要调优套接口缓冲区大小，以对长网络延迟进行摊销。
9、解释一下，在数据制作过程中，你如何能从Kafka得到准确的信息?
在数据中，为了精确地获得Kafka的消息，你必须遵循两件事: 在数据消耗期间避免重复，在数据生产过程中避免重复。
这里有两种方法，可以在数据生成时准确地获得一个语义:
- 
每个分区使用一个单独的写入器，每当你发现一个网络错误，检查该分区中的最后一条消息，以查看您的最后一次写入是否成功
- 
在消息中包含一个主键(UUID或其他)，并在用户中进行反复制
10、解释如何减少ISR中的扰动?broker什么时候离开ISR?
ISR是一组与leaders完全同步的消息副本，也就是说ISR中包含了所有提交的消息。ISR应该总是包含所有的副本，直到出现真正的故障。如果一个副本从leader中脱离出来，将会从ISR中删除。
11、Kafka为什么需要复制?
Kafka的信息复制确保了任何已发布的消息不会丢失，并且可以在机器错误、程序错误或更常见些的软件升级中使用。
12、如果副本在ISR中停留了很长时间表明什么?
如果一个副本在ISR中保留了很长一段时间，那么它就表明，跟踪器无法像在leader收集数据那样快速地获取数据。
13、请说明如果首选的副本不在ISR中会发生什么?
如果首选的副本不在ISR中，控制器将无法将leadership转移到首选的副本。
14、有可能在生产后发生消息偏移吗?
在大多数队列系统中，作为生产者的类无法做到这一点，它的作用是触发并忘记消息。broker将完成剩下的工作，比如使用id进行适当的元数据处理、偏移量等。
作为消息的用户，你可以从Kafka broker中获得补偿。如果你注视SimpleConsumer类，你会注意到它会获取包括偏移量作为列表的MultiFetchResponse对象。此外，当你对Kafka消息进行迭代时，你会拥有包括偏移量和消息发送的MessageAndOffset对象。
15、kafka提高吞吐量的配置
最基础的配置是
batch.size 默认是单批次最大16384字节，超过该值就立即发送。
linger.ms 默认是0ms，超过该时间就立即发送。
上面两个条件满足其一，就立即发送消息否则等待。
本文由浪尖修改完善自http://www.toutiao.com/i6456660580726997517
推荐阅读：
[Spark SQL从入门到精通](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485504&idx=1&sn=e19460f43aa2fe388140a2241e9ea219&chksm=9f38eb68a84f627ee0b6dac115f0684dddba573be04ae7e4e54d17c084cf1946282f81b1fd43&scene=21#wechat_redirect)
[如何获取kafka的broker保存的消费者信息？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485462&idx=1&sn=4cf0c42c5091f16a89294971c9cbd840&chksm=9f38eb3ea84f62281f6881007a3b76acd001bd61331420d7e12a11b0b7837cceaca6d17ef88c&scene=21#wechat_redirect)
[干货:Flink+Kafka 0.11端到端精确一次处理语义实现](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485150&idx=1&sn=ba0570a9f631501a189d8184807d518b&chksm=9f38e5f6a84f6ce0dd805eae87d6c62e2c96670f14c2bac1607961614ba49f310a4630e31a3b&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
