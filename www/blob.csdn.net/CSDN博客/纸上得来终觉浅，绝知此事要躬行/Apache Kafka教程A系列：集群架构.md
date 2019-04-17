# Apache Kafka教程A系列：集群架构 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月02日 18:09:38[boonya](https://me.csdn.net/boonya)阅读数：128








原文地址：[https://www.tutorialspoint.com/apache_kafka/apache_kafka_cluster_architecture.htm](https://www.tutorialspoint.com/apache_kafka/apache_kafka_cluster_architecture.htm)

看看下面的插图， 它展示了Kafka的集群图。

![](https://img-blog.csdn.net/20180402180549300)


下表描述了上图中显示的每个组件。



|序列号|组件和描述|
|----|----|
|1|BrokerKafka集群通常由多个代理组成，以保持负载平衡。 卡夫卡代理者是无状态的，所以他们使用ZooKeeper维护他们的集群状态。 一个Kafka代理实例可以处理每秒数十万次的读写操作，每个Broker都可以处理TB消息，而不会影响性能。 卡夫卡代理者领导者选举可以由ZooKeeper完成。|
|2|ZooKeeperZooKeeper用于管理和协调Kafka代理者。 ZooKeeper服务主要用于通知生产者和消费者有关Kafka系统中任何新代理者的存在或Kafka系统中代理者的失败。 根据Zookeeper收到的有关代理者存在或失败的通知，生产者和消费者就会做出决定，并开始与其他代理者协调他们的任务。|
|3|Producers生产者将数据推送给代理者。 新代理启动后，所有生产者都会搜索它并自动向该新代理发送消息。 卡夫卡生产者不会等待代理者的确认，并且可以像代理者处理的那样快地发送消息。|
|4|Consumers由于Kafka代理是无状态的，这意味着消费者必须通过使用分区偏移量来维护消耗了多少消息。 如果消费者确认特定的消息偏移量，则意味着消费者已经消费了所有先前的消息。 消费者向代理发出一个异步拉取请求，以准备消耗字节缓冲区。 消费者可以简单地通过提供偏移值来倒回或跳到分区中的任何点。 消费者补偿值由ZooKeeper通知。|



