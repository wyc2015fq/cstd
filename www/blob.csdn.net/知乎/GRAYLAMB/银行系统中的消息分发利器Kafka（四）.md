# 银行系统中的消息分发利器Kafka（四） - 知乎
# 

建议从头阅读：

[GRAYLAMB：银行系统中的消息分发利器Kafka（一）](https://zhuanlan.zhihu.com/p/54160439)

[GRAYLAMB：银行系统中的消息分发利器Kafka（二）](https://zhuanlan.zhihu.com/p/54763452)

[GRAYLAMB：银行系统中的消息分发利器Kafka（三）](https://zhuanlan.zhihu.com/p/55303792)

这次我们来介绍一下Kafka内部是如何实现高可用的。

上次我们介绍到，Kafka会把数据拆分到不同的Partition中。以达到分布式集群管理的目的。
![](https://pic3.zhimg.com/v2-debb79cf5574351d15551365c7b7be22_b.jpg)

但当集群中的部分服务器故障时，怎么办？
![](https://pic3.zhimg.com/v2-8dc99d9b9d7273a8997f9d568c584012_b.jpg)


为了解决每一个节点的单点问题，Kafka内部会把数据拷贝多份，存储在不同的服务器中。下图展示了对于Partition1（P1）的数据，我在P2、P5的服务器上分别建立了1份拷贝。
![](https://pic4.zhimg.com/v2-096b5389b923fd6a4b52de64ed86ab5f_b.jpg)


当所有拷贝过P1的节点，只要期中有一个是正常工作的，我们就可以正常读取到数据。
![](https://pic2.zhimg.com/v2-16bfe09e39617ab08a4e7500194c4ce1_b.jpg)


在Kafka中，有一个专门的参数代表数据拷贝的数量——replication factor。如果replication factor为3，就意味着在整个集群中，数据有3份拷贝。
![](https://pic2.zhimg.com/v2-4f8c60ee944b214a683d78437fef365d_b.jpg)
一般而言，会把replication factor设置为3，这是一个比较合理的参数，可以充分的容灾，也不会浪费太多的服务器。**有一点需要说明的是，replication factor不是针对Partition的，而是针对Topic的，因此如果一个Topic的replication factor为3，那就意味着Topic下所有Partition的拷贝数量都为3。**

下面详细解释一下实现机制：Kafka内，每一个partition内部会选出一个Broker作为Leader，然后Leader负责一切与Producer与Consumer的交互。
![](https://pic1.zhimg.com/v2-276f7527a4ad29b58b19b68ae3891ce4_b.jpg)


当我们把replication factor设置为3时，Kafka就知道每个Partition的数据还要有2份拷贝，因此会为每个Partition找到另外两个Broker做数据备份。同时，数据会先存在Leader上，然后同步机制会把数据从Leader不断同步到备份用的Broker。
![](https://pic1.zhimg.com/v2-9bd18017df1ea7a051c9f0de72ef4c04_b.jpg)


我们在服务终端的命令行下可以调出目前的Topic、Partition，以及副本配置情况。
![](https://pic3.zhimg.com/v2-65f0ccd70405fa33d904f89c483f5646_b.png)
我们看到，上面示例中：

Topic是TestTopicXYZ

PartitionCount是这个Topic共有多少个分区，示例中共有2个分区，下面列出了两个分区的情况：一个是Partition0，一个是Partition1。

Replicationfactor是3，说明这个Topic的所有分区都有3个副本，下面列出了Leader节点是谁：Partition0的Leader节点是1，Partition1的leader节点是2。

最后一列下面展现了整个Partition的节点分布情况：Replicas是说明有3个节点，第一、第二、第三分别是谁。可以看到第一节点与前面的leader节点是对应上的。因为leader节点就是第一节点，数据最先记录在Leader节点，然后同步给第二、第三节点。Isr则是说明三个节点中有几个节点目前数据已经同步了，示例中的两个Partition，每个Partition的3个节点都展现出来了，说明他们上面的数据都已经同步了。


![](https://pic4.zhimg.com/v2-01cdb54b14088b5977e10b275943d877_b.jpg)

