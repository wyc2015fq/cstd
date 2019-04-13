
# 大数据之Kafka内部原理详细介绍 - Magician的博客 - CSDN博客


2018年04月07日 15:47:00[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：788所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)[前言：](#前言)
[1、Kafka整体结构](#1kafka整体结构)
[2、Consumer与topic关系](#2consumer与topic关系)
[3、Kafka消息的分发](#3kafka消息的分发)
[4、Consumer的负载均衡](#4consumer的负载均衡)
[5、kafka文件存储机制](#5kafka文件存储机制)
[总结：](#总结)


# 目录
## 前言：
本篇文章所介绍的内容还是以了解为主，主要目的还是为了对Kafka有一个更深入的理解。主要介绍了以下几个知识点：Kafka的负载均衡、Producer生产数据、Kafka文件存储机制等（不过总感觉这篇文章总结的乱七八糟。。请大家多多包涵，随着后面的学习，自己会再回过头来进行修改）
## 1、Kafka整体结构
**Kafka名词解释和工作方式**
kafka：是一个生产-消费模型。类JMS消息队列，结合JMS中的两种模式，可以有多个消费者主动拉取数据，在JMS中只有点对点模式才有消费者主动拉取数据。
Producer ：消息生产者，就是向kafka broker发消息的客户端。数据的分发策略由producer决定，默认是defaultPartition  Utils.abs(key.hashCode) % numPartitions
Consumer ：消息消费者，向kafka broker取消息的客户端
Topic ：可以理解为一个队列或者目标发送的目的地，这是一个逻辑上的概念，落到磁盘上是一个partition的目录。partition的目录中有多个segment组合(index,log)，一个Topic对应多个partition[0,1,2,3]，一个partition对应多个segment组合。一个segment有默认的大小是1G。每个partition可以设置多个副本(replication-factor 1),会从所有的副本中选取一个leader出来。所有读写操作都是通过leader来进行的。特别强调，和mysql中主从有区别，mysql做主从是为了读写分离，在kafka中读写操作都是leader。
Consumer Group（CG）：数据消费者组，ConsumerGroup可以有多个，每个ConsumerGroup消费的数据都是一样的。可以把多个consumer线程划分为一个组，组里面所有成员共同消费一个topic的数据，组员之间不能重复消费。
Broker ：一台kafka服务器就是一个broker。一个集群由多个broker组成。一个broker可以容纳多个topic。只管数据存储，不管是谁生产，不管是谁消费。
Partition：为了实现扩展性，一个非常大的topic可以分布到多个broker（即服务器）上，一个topic可以分为多个partition，每个partition是一个有序的队列。partition中的每条消息都会被分配一个有序的id（offset）。kafka只保证按一个partition中的顺序将消息发给consumer，不保证一个topic的整体（多个partition间）的顺序。
Offset：kafka的存储文件都是按照offset.kafka来命名，用offset做名字的好处是方便查找。例如你想找位于2049的位置，只要找到2048.kafka的文件即可。当然the first offset就是00000000000.kafka。
## 2、Consumer与topic关系
**本质上kafka只支持Topic；**
每个group中可以有多个consumer，每个consumer属于一个consumer group；
通常情况下，一个group中会包含多个consumer，这样不仅可以提高topic中消息的并发消费能力，而且还能提高”故障容错”性，如果group中的某个consumer失效那么其消费的partitions将会有其他consumer自动接管。
对于Topic中的一条特定的消息，只会被订阅此Topic的每个group中的其中一个consumer消费，此消息不会发送给一个group的多个consumer；那么一个group中所有的consumer将会交错的消费整个Topic，每个group中consumer消息消费互相独立，我们可以认为一个group是一个”订阅”者。
在kafka中,一个partition中的消息只会被group中的一个consumer消费(同一时刻)；
一个Topic中的每个partions，只会被一个”订阅者”中的一个consumer消费，不过一个consumer可以同时消费多个partitions中的消息。
kafka的设计原理决定,对于一个topic，同一个group中不能有多于partitions个数的consumer同时消费，否则将意味着某些consumer将无法得到消息。
kafka只能保证一个partition中的消息被某个consumer消费时是顺序的；事实上，从Topic角度来说,当有多个partitions时,消息仍不是全局有序的。
3、Kafka消息的分发
Producer客户端负责消息的分发
kafka集群中的任何一个broker都可以向producer提供metadata信息,这些metadata中包含”集群中存活的servers列表”/”partitions leader列表”等信息；
当producer获取到metadata信息之后, producer将会和Topic下所有partition
leader保持socket连接；
消息由producer直接通过socket发送到broker，中间不会经过任何”路由层”，事实上，消息被路由到哪个partition上由producer客户端决定；
比如可以采用”random”“key-hash”“轮询”等,如果一个topic中有多个partitions,那么在producer端实现”消息均衡分发”是必要的。
在producer端的配置文件中,开发者可以指定partition路由的方式。
Producer消息发送的应答机制
ack机制：broker表示发来的数据已确认接收无误，表示数据已经保存到磁盘。
设置发送数据是否需要服务端的反馈,有三个值0,1,-1
0: producer不会等待broker发送ack
1: 当leader接收到消息之后发送ack
-1: 当所有的follower都同步消息成功后发送ack
request.required.acks=0
4、Consumer的负载均衡
当一个group中,有consumer加入或者离开时,会触发partitions均衡.均衡的最终目的,是提升topic的并发消费能力，步骤如下：
假如topic1,具有如下partitions: P0,P1,P2,P3
加入group中,有如下consumer: C1,C2
首先根据partition索引号对partitions排序: P0,P1,P2,P3
根据consumer.id排序: C0,C1
计算倍数: M = [P0,P1,P2,P3].size / [C0,C1].size,本例值M=2(向上取整)
然后依次分配partitions: C0 = [P0,P1],C1=[P2,P3],即Ci = [P(i * M),P((i + 1)
M -1)]
![这里写图片描述](https://img-blog.csdn.net/20180407150443123?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5、kafka文件存储机制
5.1、Kafka文件存储基本结构
在Kafka文件存储中，同一个topic下有多个不同partition，每个partition为一个目录，partiton命名规则为topic名称+有序序号，第一个partiton序号从0开始，序号最大值为partitions数量减1。
每个partition(目录)相当于一个巨型文件被平均分配到多个大小相等segment(段)数据文件中。但每个段segment file消息数量不一定相等，这种特性方便old segment file快速被删除。默认保留7天的数据。
![这里写图片描述](https://img-blog.csdn.net/20180407151306484?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180407151306484?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 每个partiton只需要支持顺序读写就行了，segment文件生命周期由服务端配置参数决定。（什么时候创建，什么时候删除）
![这里写图片描述](https://img-blog.csdn.net/20180407151031284?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5.2、Kafka Partition Segment
Segment file组成：由2大部分组成，分别为index file和data file，此2个文件一一对应，成对出现，后缀”.index”和“.log”分别表示为segment索引文件、数据文件。
![这里写图片描述](https://img-blog.csdn.net/20180407151415380?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Segment文件命名规则：partion全局的第一个segment从0开始，后续每个segment文件名为上一个segment文件最后一条消息的offset值。数值最大为64位long大小，19位数字字符长度，没有数字用0填充。
索引文件存储大量元数据，数据文件存储大量消息，索引文件中元数据指向对应数据文件中message的物理偏移地址。
![3，497：当前log文件中的第几条信息，存放在磁盘上的那个地方](https://img-blog.csdn.net/20180407151552541?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
上述图中索引文件存储大量元数据，数据文件存储大量消息，索引文件中元数据指向对应数据文件中message的物理偏移地址。
其中以索引文件中元数据3,497为例，依次在数据文件中表示第3个message(在全局partiton表示第368772个message)、以及该消息的物理偏移地址为497。
5.3、Kafka 查找message
读取offset=368776的message，需要通过下面2个步骤查找。
![这里写图片描述](https://img-blog.csdn.net/20180407152055560?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[       ](https://img-blog.csdn.net/20180407152055560?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5.3.1、查找segment file
00000000000000000000.index表示最开始的文件，起始偏移量(offset)为0
00000000000000368769.index的消息量起始偏移量为368770 = 368769 + 1
00000000000000737337.index的起始偏移量为737338=737337 + 1
其他后续文件依次类推。
以起始偏移量命名并排序这些文件，只要根据offset二分查找文件列表，就可以快速定位到具体文件。当offset=368776时定位到00000000000000368769.index和对应log文件。
5.3.2、通过segment file查找message
当offset=368776时，依次定位到00000000000000368769.index的元数据物理位置和00000000000000368769.log的物理偏移地址
然后再通过00000000000000368769.log顺序查找直到offset=368776为止。
总结：
需要了解的知识点有以下几个：
1、Kafka的内部名词及各个名词的含义及作用。
2、Kafka消息分发的机制。
3、Consumer的负载均衡机制。
4、Kafka文件存储机制。
总感觉这篇文章总结的比较乱，也许可以把其中的模块拆开来写吧。Anyway，就先总结成这样吧，以后有机会会再修改。

