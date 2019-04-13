
# kafka架构浅显理解 - anlcy - 博客园






# [kafka架构浅显理解](https://www.cnblogs.com/camilla/p/8418370.html)
**Kafka的概念：**
**1、 AMQP协议**

Advanced Message Queuing Protocol （高级消息队列协议）
**The Advanced Message Queuing Protocol (AMQP)：**是一个标准开放的应用层的消息中间件（Message Oriented Middleware）协议。AMQP定义了通过网络发送的字节流的数据格式。因此兼容性非常好，任何实现AMQP协议的程序都可以和与AMQP协议兼容的其他程序交互，可以很容易做到跨语言，跨平台。
**2、**消费者：（Consumer）：从消息队列中请求消息的客户端应用程序
**3、**生产者：（Producer）  ：向broker发布消息的应用程序
**4、**AMQP服务端（broker）：用来接收生产者发送的消息并将这些消息路由给服务器中的队列，便于fafka将生产者发送的消息，动态的添加到磁盘并给每一条消息一个偏移量，所以对于kafka一个broker就是一个应用程序的实例
**kafka 设置auto.offset.reset**
smallest和largest才有效，如果smallest重新0开始读取，如果是largest从logfile的offset读取。一般情况下我们都是设置smallest例如：auto.offset.reset=smallest
**Kafka架构**
生产者生产消息、kafka集群、消费者获取消息这样一种架构，如下图：
![](https://images2015.cnblogs.com/blog/831021/201602/831021-20160222125238526-112967735.png)
kafka集群中的消息，是通过Topic（主题）来进行组织的，如下图：
![](https://images2015.cnblogs.com/blog/831021/201602/831021-20160222125343135-190706359.png)
1、主题（Topic）：一个主题类似新闻中的体育、娱乐、教育等分类概念，在实际工程中通常一个业务一个主题。
2、分区（Partition）：一个Topic中的消息数据按照多个分区组织，分区是kafka消息队列组织的最小单位，一个分区可以看作是一个FIFO（ First Input First Output的缩写，先入先出队列）的队列。
工作图：
![](https://images2015.cnblogs.com/blog/1185653/201707/1185653-20170716145456050-1787808842.png)
根据以上架构图代码演示：
1. 创建Topic:
./kafka-topics.sh --create --zookeeper hadoop1:2181,hadoop2:2181,hadoop3:2181 --replication-factor 2 --partitions 3 --topic partitionTopic
![](https://images2015.cnblogs.com/blog/1185653/201707/1185653-20170716145824441-101368709.png)
2. ./kafka-topics.sh --describe --zookeeper hadoop1:2181,hadoop2:2181,hadoop3:2181 --topic partitionTopic
可以看见我们一共有3个分区分别是0,1,2, replica 有2个。这和我们在上面命令设置的是一致的。
partition 0 的leader在broker1, follower在broker2
partition 1 的leader在broker2, follower在broker0
partition 2 的leader在broker0, follower在brokder1
一个patition只会在一个broker上，replica被均匀的分布在各个kafka server(broker)上
![](https://images2015.cnblogs.com/blog/1185653/201707/1185653-20170716145953363-668176743.png)
如果replicas 数设置大于 broker数会出现什么情况呢？
Kafka并不允许这种情况发生，因为在一个broker上如果有2个replica其实是没有意义的，因为再多的replica同时在一台broker上，随着该broker的crash，一起不可用。
![](https://images2015.cnblogs.com/blog/1185653/201707/1185653-20170716150805082-122060327.png)








