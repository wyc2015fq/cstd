# Kafka入门教程其一 消息队列基本概念与学习笔记

 																				2019年01月27日 18:18:19 					[小鹅鹅](https://me.csdn.net/asd136912) 						阅读数：42 						 																															

 									

​                   					 					版权声明：本文为博主原创文章，转载请附http://blog.csdn.net/asd136912。					https://blog.csdn.net/asd136912/article/details/86668160				



### 文章目录

- - - [1. 综述](https://blog.csdn.net/asd136912/article/details/86668160#1__1)

    - [2. 消息队列(Message Queue)](https://blog.csdn.net/asd136912/article/details/86668160#2_Message_Queue_8)

    - - [2.1 点对点](https://blog.csdn.net/asd136912/article/details/86668160#21__10)
      - [2.2 发布/订阅(pub-sub)](https://blog.csdn.net/asd136912/article/details/86668160#22_pubsub_17)

    - [3. Kafka基础术语解释](https://blog.csdn.net/asd136912/article/details/86668160#3_Kafka_22)

    - - [3.1 Broker](https://blog.csdn.net/asd136912/article/details/86668160#31_Broker_38)
      - [3.2 Partitions](https://blog.csdn.net/asd136912/article/details/86668160#32_Partitions_52)
      - [3.3 Message](https://blog.csdn.net/asd136912/article/details/86668160#33_Message_59)

    - [4. Kafka持久化](https://blog.csdn.net/asd136912/article/details/86668160#4_Kafka_72)

    - [5. Kafka 作为消息/存储系统及流处理](https://blog.csdn.net/asd136912/article/details/86668160#5_Kafka__83)

    - - [5.1 消息系统](https://blog.csdn.net/asd136912/article/details/86668160#51__84)
      - [5.2 存储系统](https://blog.csdn.net/asd136912/article/details/86668160#52__90)
      - [5.3 流处理](https://blog.csdn.net/asd136912/article/details/86668160#53__114)

    - [6. 常用配置项](https://blog.csdn.net/asd136912/article/details/86668160#6__123)

    - - [6.1 broker配置](https://blog.csdn.net/asd136912/article/details/86668160#61_broker_125)
      - [6.2 topic配置](https://blog.csdn.net/asd136912/article/details/86668160#62_topic_131)

    - [Ref](https://blog.csdn.net/asd136912/article/details/86668160#Ref_140)



### 1. 综述

Apache Kafka是基于发布/订阅的容错消息系统，由Scala和Java编写，是一个分布式消息队列，具有高性能、持久化、多副本备份、横向扩展能力。

与其他消息传递系统相比，Kafka具有更好的吞吐量，内置分区，复制和固有的容错能力，这使得它非常适合大规模消息处理应用程序。

Kafka适合离线和在线消息消费。 Kafka消息保留在磁盘上，并在群集内复制以防止数据丢失。 Kafka构建在ZooKeeper同步服务之上。 它与Apache Storm和Spark非常好地集成，用于实时流式数据分析。

### 2. 消息队列(Message Queue)

Message Queue消息传送系统提供传送服务。消息传送依赖于大量支持组件，这些组件负责处理连接服务、消息的路由和传送、持久性、安全性以及日志记录。消息服务器可以使用一个或多个代理实例。消息队列分为两种：**点对点**与**发布/订阅(pub-sub)**

#### 2.1 点对点

消息生产者生产消息发送到queue中，然后消息消费者从queue中取出并且消费消息。

消息被消费以后，queue中不再有存储，所以消息消费者不可能消费到已经被消费的消息。Queue支持存在多个消费者，但是对一个消息而言，只会有一个消费者可以消费。

该系统的典型示例是订单处理系统，其中每个订单将由一个订单处理器处理，但多个订单处理器也可以同时工作。
 ![img](https://img-blog.csdnimg.cn/20190127172000814.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

#### 2.2 发布/订阅(pub-sub)

消息生产者（发布）将消息发布到topic中，同时有多个消息消费者（订阅）消费该消息。和点对点方式不同，发布到topic的消息会被所有订阅者消费。

现实生活的例子是电视，它发布不同的频道，如运动，电影，音乐等，任何人都可以订阅自己的频道集。
 ![img](https://img-blog.csdnimg.cn/20190127172007979.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

### 3. Kafka基础术语解释

![img](https://img-blog.csdnimg.cn/20190127175858556.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

- **生产者Producer:** 是消息的产生的源头，负责生成消息并发送到Kafka服务器上。
- **消费者Consumer:** 消息的使用方，负责消费Kafka服务器上的消息。
- **主题Topic:** 由用户定义并配置在Kafka服务器，用于建立生产者和消息者之间的订阅关系：生产者发送消息到指定的Topic下，消息者从这个Topic下消费消息。
- **分区Partition:** 一个Topic下面会分为很多分区，例如：“kafka-test”这个Topic下可以分为6个分区，分别由两台服务器提供，那么通常可以配置为让每台服务器提供3个分区，假如服务器ID分别为0、1，则所有的分区为0-0、0-1、0-2和1-0、1-1、1-2。
   ![img](https://img-blog.csdnimg.cn/20190127202255507.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)
   Topic物理上的分组，一个 topic可以分为多个 partition，每个 partition 是一个有序的队列。partition中的每条消息都会被分配一个有序的 id（offset）
- **Broker:** 即Kafka的服务器，用户存储消息，Kafka集群中的一台或多台服务器统称为 broker。
- **消费者分组Group:** 用于归组同类消费者，在Kafka中，多个消费者可以共同消息一个Topic下的消息，每个消费者消费其中的部分消息，这些消费者就组成了一个分组，拥有同一个分组名称，通常也被称为消费者集群。
- **偏移量Offset:** 消息存储在Kafka的Broker上，消费者拉取消息数据的过程中需要知道消息在文件中的偏移量，这个偏移量就是所谓的Offset。
- **领导者Leader:** 负责给定分区的所有读取和写入的节点。 每个分区都有一个服务器充当Leader。
- **追随者Follower:** 跟随领导者指令的节点被称为Follower。 如果领导失败，一个追随者将自动成为新的领导者。 跟随者作为正常消费者，拉取消息并更新其自己的数据存储。

#### 3.1 Broker

- Message在Broker中通Log追加的方式进行持久化存储。并进行分区（patitions)。
- 为了减少磁盘写入的次数,broker会将消息暂时buffer起来,当消息的个数(或尺寸)达到一定阀值时,再flush到磁盘,这样减少了磁盘IO调用的次数。
- Broker没有副本机制，一旦broker宕机，该broker的消息将都不可用。Message消息是有多份的。
- Broker不保存订阅者的状态，由订阅者自己保存。
- 无状态导致消息的删除成为难题（可能删除的消息正在被订阅），kafka采用基于时间的SLA(服务水平保证)，消息保存一定时间（通常为7天）后会被删除。
- 消息订阅者可以rewind back到任意位置重新进行消费，当订阅者故障时，可以选择最小的offset(id)进行重新读取消费消息。

#### 3.2 Partitions

- Kafka基于文件存储.通过分区，可以将日志内容分散到多个server上,来避免文件尺寸达到单机磁盘的上限，每个partiton都会被当前server(kafka实例)保存。
- 可以将一个topic切分多任意多个partitions，来消息保存/消费的效率。
- 越多的partitions意味着可以容纳更多的consumer，有效提升并发消费的能力。

#### 3.3 Message

- Message消息：是通信的基本单位，每个 producer 可以向一个 topic（主题）发布一些消息。
- Kafka中的Message是以topic为基本单位组织的，不同的topic之间是相互独立的。每个topic又可以分成几个不同的partition(每个topic有几个partition是在创建topic时指定的)，每个partition存储一部分Message。
- partition中的每条Message包含了以下三个属性：
  - offset: 消息唯一标识, 对应类型long
  - MessageSize: 对应类型int32
  - data: message的具体内容。

### 4. Kafka持久化

1. 一个Topic可以认为是一类消息，每个topic将被分成多partition(区),每个partition在存储层面是append  log文件。任何发布到此partition的消息都会被直接追加到log文件的尾部，每条消息在文件中的位置称为offset（偏移量），partition是以文件的形式存储在文件系统中。
2. Logs文件根据broker中的配置要求,保留一定时间后删除来释放磁盘空间。

![img](https://img-blog.csdnimg.cn/20190127211739737.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

1. 为数据文件建索引：稀疏存储，每隔一定字节的数据建立一条索引。下图为一个partition的索引示意图：
    ![img](https://img-blog.csdnimg.cn/20190127211907230.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

### 5. Kafka 作为消息/存储系统及流处理

#### 5.1 消息系统

kafka有比传统的消息系统更强的顺序保证。
 传统的消息系统按顺序保存数据，如果多个消费者从队列消费，则服务器按存储的顺序发送消息，但是，尽管服务器按顺序发送，消息异步传递到消费者，因此消息可能乱序到达消费者。这意味着消息存在并行消费的情况，顺序就无法保证。消息系统常常通过仅设1个消费者来解决这个问题，但是这意味着没用到并行处理。

kafka做的更好。通过并行topic的parition ——  kafka提供了顺序保证和负载均衡。每个partition仅由同一个消费者组中的一个消费者消费到。并确保消费者是该partition的唯一消费者，并按顺序消费数据。每个topic有多个分区，则需要对多个消费者做负载均衡，但请注意，**相同的消费者组中不能有比分区更多的消费者，否则多出的消费者一直处于空等待，不会收到消息。**

#### 5.2 存储系统

所有发布消息到消息队列和消费分离的系统，实际上都充当了一个存储系统（发布的消息先存储起来）。Kafka比别的系统的优势是它是一个非常高性能的**存储系统**。

写入到kafka的数据将写到磁盘并复制到集群中保证容错性。并允许生产者等待消息应答，直到消息完全写入。

kafka的磁盘结构 - 无论你服务器上有50KB或50TB，执行是相同的。

client来控制读取数据的位置。你还可以认为kafka是一种专用于高性能，低延迟，提交日志存储，复制，和传播特殊用途的**分布式文件系统**。

以下是详细说明：可以认为topic下有partition，partition下有segment，segment是实际的一个个文件，topic和partition都是抽象概念。

在目录`/${topicName}-{$partitionid}/`下，存储着实际的log文件（即segment），还有对应的索引文件。

每个segment文件大小相等，文件名以这个segment中最小的offset命名，文件扩展名是.log；segment对应的索引的文件名字一样，扩展名是.index。有两个index文件，一个是offset  index用于按offset去查message，一个是time index用于按照时间去查，其实这里可以优化合到一起，下面只说offset  index。总体的组织是这样的：
 ![img](https://img-blog.csdnimg.cn/20190217175050189.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)
 为了减少索引文件的大小，降低空间使用，方便直接加载进内存中，这里的索引使用稀疏矩阵，不会每一个message都记录下具体位置，而是**每隔一定的字节数，再建立一条索引**。 索引包含两部分，分别是**baseOffset**，还有**position**。

- **baseOffset**：意思是这条索引对应segment文件中的第几条message。这样做方便使用数值压缩算法来节省空间。例如kafka使用的是varint。
- **position**：在segment中的绝对位置。

查找offset对应的记录时，会先用二分法，找出对应的offset在哪个segment中，然后使用索引，在定位出offset在segment中的大概位置，再遍历查找message。

#### 5.3 流处理

在kafka中，流处理持续获取`输入topic`的数据，进行处理加工，然后写入`输出topic`。

可以直接使用producer和consumer API进行简单的处理。对于复杂的转换，Kafka提供了更强大的Streams API。可构建聚合计算或连接流到一起的复杂应用程序。

助于解决此类应用面临的硬性问题：处理无序的数据，代码更改的再处理，执行状态计算等。

前面的博客[Spark Structured Streaming + Kafka使用笔记](https://blog.csdn.net/asd136912/article/details/82913264)有详细介绍Spark+Kafka的使用。

### 6. 常用配置项

#### 6.1 broker配置

| 配置项                        | 作用                                                         |
| ----------------------------- | ------------------------------------------------------------ |
| [broker.id](http://broker.id) | broker的唯一标识                                             |
| auto.create.topics.auto       | 设置成true，就是遇到没有的topic自动创建topic。               |
| log.dirs                      | log的目录数，目录里面放partition，当生成新的partition时，会挑目录里partition数最少的目录放。 |

#### 6.2 topic配置

| 配置项                                      | 作用                                                         |
| ------------------------------------------- | ------------------------------------------------------------ |
| num.partitions                              | 新建一个topic，会有几个partition。                           |
| [log.retention.ms](http://log.retention.ms) | 对应的还有minutes，hours的单位。日志保留时间，因为删除是文件维度而不是消息维度，看的是日志文件的mtime。 |
| log.retention.bytes                         | partion最大的容量，超过就清理老的。注意这个是partion维度，就是说如果你的topic有8个partition，配置1G，那么平均分配下，topic理论最大值8G。 |
| log.segment.bytes                           | 一个segment的大小。超过了就滚动。                            |
| [log.segment.ms](http://log.segment.ms)     | 一个segment的打开时间，超过了就滚动。                        |
| message.max.bytes                           | message最大多大                                              |

### Ref

1. <https://www.jianshu.com/p/d3e963ff8b70>
2. <https://www.w3cschool.cn/apache_kafka/apache_kafka_introduction.html>
3. <https://blog.csdn.net/dapeng1995/article/details/81536862>
4. <http://orchome.com/5>