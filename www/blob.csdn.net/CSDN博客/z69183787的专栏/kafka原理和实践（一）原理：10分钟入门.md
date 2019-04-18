# kafka原理和实践（一）原理：10分钟入门 - z69183787的专栏 - CSDN博客
2018年05月15日 17:16:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：803
[http://www.cnblogs.com/dennyzhangdd/p/7759869.html](http://www.cnblogs.com/dennyzhangdd/p/7759869.html)
**目录**
- [一、kafka拓扑图](http://www.cnblogs.com/dennyzhangdd/p/7759869.html#_label0)
- [二、名词解释](http://www.cnblogs.com/dennyzhangdd/p/7759869.html#_label1)
- [三、生产模型](http://www.cnblogs.com/dennyzhangdd/p/7759869.html#_label2)
- [四、消费模型](http://www.cnblogs.com/dennyzhangdd/p/7759869.html#_label3)
**正文**
系列目录
[kafka原理和实践（一）原理：10分钟入门](http://www.cnblogs.com/dennyzhangdd/p/7759869.html)
[kafka原理和实践（二）spring-kafka简单实践](http://www.cnblogs.com/dennyzhangdd/p/7759875.html)
[kafka原理和实践（三）spring-kafka生产者源码](http://www.cnblogs.com/dennyzhangdd/p/7827564.html)
[kafka原理和实践（四）spring-kafka消费者源码](http://www.cnblogs.com/dennyzhangdd/p/7759876.html)
[kafka原理和实践（五）spring-kafka配置详解](http://www.cnblogs.com/dennyzhangdd/p/7834143.html)
[kafka原理和实践（六）总结升华](http://www.cnblogs.com/dennyzhangdd/p/7759878.html)
=========正文分割线===============
关于kafka原理网上文章很多，但能真正快速入门的几乎没有（要不就是原文翻译官网...实在枯燥无味之极），本章尽量采用图文的方式，提纲挈领的阐述原理。
[回到顶部](http://www.cnblogs.com/dennyzhangdd/p/7759869.html#_labelTop)
## 一、kafka拓扑图
大家都知道kafka是依赖zookeeper集群的，一般最少也要三台服务器来实现HA。拓扑图如下，分三层：
1.Producers:消息生产者，push消息给Brokers.发送时根据不同topic选择不同分区（在Broker上）。
2.Brokers：注册在zookeeper节点上。
3.Consumers：消息消费者，从brokers上根据订阅的topic选择不同分区，poll数据，执行消费。
![](https://images2017.cnblogs.com/blog/584866/201711/584866-20171117093650374-1834235910.png)
[回到顶部](http://www.cnblogs.com/dennyzhangdd/p/7759869.html#_labelTop)
## 二、名词解释
1.producer：消息生产者，发布消息到 kafka 集群的终端或服务。
2.broker：kafka 集群中包含的服务器。
3.topic：每条发布到 kafka 集群的消息属于的类别，即 kafka 是面向 topic 的。
4.partition：partition 是物理上的概念，每个 topic 包含一个或多个 partition。kafka 分配的单位是 partition。
5.consumer：从 kafka 集群中消费消息的终端或服务。
6.Consumer group：high-level consumer API 中，每个 consumer 都属于一个 consumer group，每条消息只能被 consumer group 中的一个 Consumer 消费，但可以被多个 consumer group 消费。
7.replica：partition 的副本，保障 partition 的高可用。
8.leader：replica 中的一个角色， producer 和 consumer 只跟 leader 交互。
9.follower：replica 中的一个角色，从 leader 中复制数据。
10.controller：kafka 集群中的其中一个服务器，用来进行 leader election 以及 各种 failover。
11.zookeeper：kafka 通过 zookeeper 来存储集群的 meta 信息。
[回到顶部](http://www.cnblogs.com/dennyzhangdd/p/7759869.html#_labelTop)
## 三、生产模型
kafka生产者多线程异步发送模型如下图，主要包含2个流程：1）数据批量存储,批量发送2）Netty NIO 发送数据
大概看一下流程，理解个大概即可，具体见后续源码分析章节。
![](https://images2017.cnblogs.com/blog/584866/201711/584866-20171122161814993-837212513.png)
[回到顶部](http://www.cnblogs.com/dennyzhangdd/p/7759869.html#_labelTop)
## 四、消费模型
根据xml配置的不同启动不同的容器（ConcurrentMessageListenerContainer/MessageListenerContainer）,下图为并发消息监听器容器启动流程，主要包含2个主流程：
1.从cluster拉取消息
2.消费消息
![](https://images2017.cnblogs.com/blog/584866/201711/584866-20171123123259383-2005482311.png)
