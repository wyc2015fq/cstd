# 银行系统中的消息分发利器Kafka（三） - 知乎
# 

建议从头阅读：

[GRAYLAMB：银行系统中的消息分发利器Kafka（一）](https://zhuanlan.zhihu.com/p/54160439)

[GRAYLAMB：银行系统中的消息分发利器Kafka（二）](https://zhuanlan.zhihu.com/p/54763452)

6、Partition

上次我们说到，Kafka可以存储数据，而且数据按照Topic进行分类。

这些存储的数据可能会很大，这可能会给Kafka的Broker带来很大的存储压力。

一个好的解决办法就是把这些数据拆成一个或多个Partition：
![](https://pic1.zhimg.com/v2-6a94509ddf7bfa21a30b41bc6210e04c_b.jpg)
然后，把这多个Partition分发到不同的服务器上。

Kafka是一个分布式系统，所以对数据文件的Partition进行分布式管理是很方便的。

随之，另外一个问题来了，我们要把数据分成多少个Partition呢？
![](https://pic2.zhimg.com/v2-b3eed3c56a7aff2275094eaf78270f09_b.jpg)
这个问题不是Kafka来解答的，而是Kafka的管理员，他们会确定一项数据分成多少个Partition，然后Kafka再按照他们的指令来处理分区。

每一个Partition就存在一个服务器上，也不能再将他们进一步拆分。所以管理员可以根据服务器情况计算出Partition的数量。

7、Offset

我们知道Kafka上会接受大量的消息（Message），然后这样一个一个消息在Kafka中会形成一个顺序的队列。

而Offset是就是“消息”在队列中的位置。
![](https://pic3.zhimg.com/v2-0416cc980e3d94ddd85fff1bba3ac256_b.jpg)
在每一个Partition 中，第一个消息的Offset就是0，第二个就是1，以此类推。另外，Offset并不是一个全局的ID，它只作用于所属的Partition。所以，在同一个Partition中，不会有相同的Offset。

结合上面的知识，我们可以知道，如果要在Kafka中定位一个消息信息，就是先找到Topic，然后找到Partition，最后找到Offset。
![](https://pic1.zhimg.com/v2-14b0fc4ea6d49093333132495a2c1f6c_b.jpg)
8、Consumer Group

先把前面的场景复习一下。

首先我们有很多节点的数据要收集，于是我们通过Kafka来实现：
![](https://pic1.zhimg.com/v2-48297192e7ddd0ff972c0e16e164ef6c_b.jpg)
然后我们为每一个节点创建一个Producer：
![](https://pic2.zhimg.com/v2-968fc095a3e32d7e0ca0587eacaefc21_b.jpg)
现在我们为Kafka创建Consumer，Consumer将从Kafka上获取数据，并将数据存入DataCenter：
![](https://pic2.zhimg.com/v2-9cc959c202d491d2bc7ccdaca7beb5a1_b.jpg)
这时，你有成百上千的Producer同时将数据放入同一个Topic，Kafka怎么能hold住呢？我们前面说过Kafka是一个分布式系统，于是我们利用很多的Kafka Broker来分散处理压力，并且将数据分Partition：
![](https://pic2.zhimg.com/v2-fcf1244567fe310d0a98fe372e47804d_b.jpg)
这时你会发现，处理压力跑到Conumser那里了，于是我们就需要一个Consumer Group了。
![](https://pic1.zhimg.com/v2-9826a2cc99378f77838bbacf2e33d8fc_b.jpg)
Consumer Group中的Consumer要与Partition一一对应。因此，一个Topic有多少Partition，一个Consumer Group就有多少Consumer。Kafka不允许两个或两个以上的Consumer同时读取一个Partition的数据，以避免double reading的问题。

Kafka的几个重要的概念就介绍完了。后面我会逐步深入的介绍Kafka的一些细节，欢迎关注～
![](https://pic4.zhimg.com/v2-01cdb54b14088b5977e10b275943d877_b.jpg)

