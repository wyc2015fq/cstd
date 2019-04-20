# kafka入门介绍 - Spark高级玩法 - CSDN博客
2018年05月26日 08:06:22[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：702
**背景：**
当今社会各种应用系统诸如商业、社交、搜索、浏览等像信息工厂一样不断的生产出各种信息，在大数据时代，我们面临如下几个挑战：
- 
如何收集这些巨大的信息
- 
如何分析它
- 
如何及时做到如上两点
以上几个挑战形成了一个业务需求模型，即生产者生产（produce）各种信息，消费者消费（consume）（处理分析）这些信息，而在生产者与消费者之间，需要一个沟通两者的桥梁-消息系统。
从一个微观层面来说，这种需求也可理解为不同的系统之间如何传递消息。
**Kafka诞生：**由 linked-in 开源
kafka-即是解决这类问题的一个框架，它实现了生产者和消费者之间的无缝连接。
kafka-高产出的分布式消息系统(A high-throughput distributed messaging system)
**Kafka特性：**它形容自己的设计是独一无二的，先看一下它有如何过人之处：
- 
快：单个kafka服务每秒可处理数以千计客户端发来的几百MB数据。
- 
可扩展性：一个单一集群可作为一个大数据处理中枢，集中处理各种类型业务
- 
持久化：消息被持久化到磁盘（可处理TB数据级别数据但仍保持极高数据处理效率），并且有备份容错机制
- 
分布式：着眼于大数据领域，支持分布式，集群可处理每秒百万级别消息
- 
实时性：生产出的消息可立即被消费者消费
**Kafka的组件：**
- 
topic：消息存放的目录即主题
- 
Producer：生产消息到topic的一方
- 
Consumer：订阅topic消费消息的一方
- 
Broker：Kafka的服务实例就是一个broker
如下图所示，Producer生产的消息通过网络发送给Kafka cluster，而Consumer从其中消费消息
**Topic 和Partition:**
消息发送时都被发送到一个topic，其本质就是一个目录，而topic由是由一些Partition Logs(分区日志)组成,其组织结构如下图所示：
(一个主题可以包含多个分区)
我们可以看到，每个Partition中的消息都是有序的，生产的消息被不断追加到Partition log上，其中的每一个消息都被赋予了一个唯一的offset值。
Kafka集群会保存所有的消息，不管消息有没有被消费；我们可以设定消息的过期时间，只有过期的数据才会被自动清除以释放磁盘空间。比如我们设置消息过期时间为2天，那么这2天内的所有消息都会被保存到集群中，数据只有超过了两天才会被清除。
Kafka需要维持的元数据只有一个--消费消息在Partition中的offset值，Consumer每消费一个消息，offset就会加1。其实消息的状态完全是由Consumer控制的，Consumer可以跟踪和重设这个offset值，这样的话Consumer就可以读取任意位置的消息。
把消息日志以Partition的形式存放有多重考虑，第一，方便在集群中扩展，每个Partition可以通过调整以适应它所在的机器，而一个topic又可以有多个Partition组成，因此整个集群就可以适应任意大小的数据了；第二就是可以提高并发，因为可以以Partition为单位读写了。
**分布式：**
(主从集群)
这些Partitions分布在集群的每一台server上，而每一个Partition在集群中都可以有多个备份，这个备份数量是可配置的。
每个Partition都有一个leader server，而其他备份的server都称为followers，只有leader服务器才会处理这个Partition上所有的读写请求，而其它followers则被动的复制leader上的数据。如果一个leader挂掉了，followers中的一个服务器则会自动升级为leader。因此，其实集群中的每个服务器都扮演着一个Partition的leader服务器，和其它Partition的follower服务器。
**Producers:**
Producer可以根据自己的选择发布消息到一个主题，Producer也可以自己决定把消息发布到这个主题的哪个Partition，当然我们可以选择API提供的简单的分区选择算法，也可以自己去实现一个分区选择算法。
**Consumers:**
消息传递通常由两种模式，queuing（队列）和publish-subscribe （发布-订阅）
- 
queuing：每个Consumer从消息队列中取走一个消息
- 
pub-scrib:消息被广播到每个Consumer
Kafka通过提供了一个对Consumer的抽象来同时实现这两种模式-ConsumerGroup。Consumer实例需要给自己指定一个ConsumerGroup的名字，如果所有的实例都用同一个ConsumerGroup名字，那么这些Consumer就会以queuing的模式工作；如果所有的实例分别用的不同的ConsumerGroup名字，那么它们就以public-subscribe模式工作。
(group的概念只针对于客户端，如果有多个客户端定义了多个组时，broker会以pub-scrib的形式将消息发送到每一个group上)
如下图所示：含两台server的集群一共有p0~p3四个Partition，两个Consumer Group，在Group内部是以queuing的模式消费Partition，在Group之间是以pub-scrib模式消费。
**消息顺序性：**
Kafka是如何确保消息消费的顺序性的呢？前面讲到过Partition，消息在一个Partition中的顺序是有序的，但是Kafka只保证消息在一个Partition中有序，如果要想使整个topic中的消息有序，那么一个topic仅设置一个Partition即可。
**推荐阅读：**
1，[Spark Streaming 中管理 Kafka Offsets 的几种方式](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484684&idx=1&sn=3c8f42b750574ce296f4870c4f0f72aa&chksm=9f38e624a84f6f329823812eea4d2c419e53a8a73713730228a281252e2274d55416d0f9e726&scene=21#wechat_redirect)
2，[开源消息中间件Kafka在华泰证券的探索与实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484654&idx=1&sn=50331e2b175e72d01de213e227f68e62&chksm=9f38e7c6a84f6ed0e4adab7ccb50de5b8b31d54ec45ae97d4418e100ff6f429060cf796d1b6d&scene=21#wechat_redirect)
3，[重磅：Flume1-7结合kafka讲解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484218&idx=1&sn=fe25af996f713b12359bd6e18cef5f31&chksm=9f38e012a84f69044ff9285b9f70e5e8aa6d6458faa306ac59645e307a8791b7acac77d156dc&scene=21#wechat_redirect)
4，[Kafka源码系列之实现自己的kafka监控](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483712&idx=1&sn=45d437ad71225d4e726b64852e315e03&chksm=9f38e268a84f6b7e1f070ad5664ca564ddb525c9dca15c13a0115fcd751bef8d84e23e3887e0&scene=21#wechat_redirect)
