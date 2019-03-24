# Kafka入门教程其二 生产与消费详解

2019年02月17日 22:33:13

小鹅鹅

阅读数：63

 								标签： 																[Kafka																](http://so.csdn.net/so/search/s.do?q=Kafka&t=blog)[大数据																](http://so.csdn.net/so/search/s.do?q=大数据&t=blog)[MQ																](http://so.csdn.net/so/search/s.do?q=MQ&t=blog)[消息队列																](http://so.csdn.net/so/search/s.do?q=消息队列&t=blog) 							更多

 								个人分类： 																[Kafka																](https://blog.csdn.net/asd136912/article/category/8645155) 							

 									

​                   					 					版权声明：本文为博主原创文章，转载请附http://blog.csdn.net/asd136912。					https://blog.csdn.net/asd136912/article/details/86986164				



### 文章目录

- - - [1. 概述](https://blog.csdn.net/asd136912/article/details/86986164#1__1)

    - [2. 生产](https://blog.csdn.net/asd136912/article/details/86986164#2__7)

    - - [2.1 partition分配与Leader选举](https://blog.csdn.net/asd136912/article/details/86986164#21_partitionLeader_19)

      - - [2.1.1 partition分配](https://blog.csdn.net/asd136912/article/details/86986164#211_partition_26)
        - [2.1.2 Leader选举](https://blog.csdn.net/asd136912/article/details/86986164#212_Leader_31)

      - [2.2 多副本同步](https://blog.csdn.net/asd136912/article/details/86986164#22__38)

    - [3. 消费](https://blog.csdn.net/asd136912/article/details/86986164#3__58)

    - - [3.1 offset保存](https://blog.csdn.net/asd136912/article/details/86986164#31_offset_63)
      - [3.2 分配partition--reblance](https://blog.csdn.net/asd136912/article/details/86986164#32_partitionreblance_75)
      - [3.2.1 选coordinator](https://blog.csdn.net/asd136912/article/details/86986164#321_coordinator_83)
      - [3.2.2 交互流程](https://blog.csdn.net/asd136912/article/details/86986164#322__89)
      - [3.2.3 reblance流程](https://blog.csdn.net/asd136912/article/details/86986164#323_reblance_96)

    - [4. 消息投递语义](https://blog.csdn.net/asd136912/article/details/86986164#4__114)

    - - [4.1 At least once](https://blog.csdn.net/asd136912/article/details/86986164#41_At_least_once_122)
      - [4.2 At most once](https://blog.csdn.net/asd136912/article/details/86986164#42_At_most_once_127)
      - [4.3 Exactly once](https://blog.csdn.net/asd136912/article/details/86986164#43_Exactly_once_132)

    - [Ref](https://blog.csdn.net/asd136912/article/details/86986164#Ref_149)



### 1. 概述

接着上一篇博客，本篇主要介绍Kafka的生产与消费的过程。Producers往Brokers里面的指定Topic中写消息，Consumers从Brokers里面拉去指定Topic的消息。
 ![img](https://img-blog.csdnimg.cn/20190210155647337.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

图中有两个topic，topic 0有两个partition，topic 1有一个partition，三副本备份。

### 2. 生产

![img](https://img-blog.csdnimg.cn/20190210160032172.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)
 创建一条记录，记录中一个要指定对应的**topic**和**value**，**key**和**partition**可选。 先序列化，然后按照topic和partition，放进对应的发送队列中。kafka produce都是批量请求，会积攒一批，然后一起发送，不是调send()就进行立刻进行网络发包。

如果partition没填，那么情况会是这样的：

- key有填
   按照key进行哈希，相同key去一个partition。（如果扩展了partition的数量那么就不能保证了）
- key没填
   round-robin来选partition

这些要发往同一个partition的请求按照配置，攒一批然后由一个单独的线程一次性发过去。

#### 2.1 partition分配与Leader选举

当存在多副本的情况下，会尽量把多个副本，分配到不同的broker上。kafka会为partition选出一个leader，之后所有该partition的请求，实际操作的都是leader，然后再同步到其他的follower。当一个broker歇菜后，所有leader在该broker上的partition都会重新选举，选出一个leader。（这里不像分布式文件存储系统那样会自动进行复制保持副本数）

然后这里就涉及两个细节：怎么分配partition，怎么选leader。

关于partition的分配，还有leader的选举，总得有个执行者。在kafka中，这个执行者就叫**controller**。kafka使用zk在broker中选出一个controller，用于partition分配和leader选举。

##### 2.1.1 partition分配

1. 将所有Broker（假设共n个Broker）和待分配的Partition排序
2. 将第i个Partition分配到第（i mod n）个Broker上 （这个就是leader）
3. 将第i个Partition的第j个Replica分配到第（(i + j) mode n）个Broker上

##### 2.1.2 Leader选举

controller会在Zookeeper的/brokers/ids节点上注册Watch，一旦有broker宕机，它就能知道。当broker宕机后，controller就会给受到影响的partition选出新leader。controller从zk 的`/brokers/topics/[topic]/partitions/[partition]/state`中，读取对应partition的ISR（in-sync replica已同步的副本）列表，选一个出来做leader。

选出leader后，更新zk，然后发送LeaderAndISRRequest给受影响的broker。这里不是使用zk通知，而是直接给broker发送rpc请求。

如果ISR列表是空，那么会根据配置，随便选一个replica做leader，或者干脆这个partition就是歇菜。如果ISR列表的有机器，但是也歇菜了，那么还可以等ISR的机器活过来。

#### 2.2 多副本同步

这里的策略，服务端这边的处理是follower从leader批量拉取数据来同步。但是具体的可靠性，是由生产者来决定的。

生产者生产消息的时候，通过request.required.acks参数来设置数据的可靠性。

| acks | what happen                                                  |
| ---- | ------------------------------------------------------------ |
| 0    | which means that the producer never waits for an acknowledgement from the broker.发过去就完事了，不关心broker是否处理成功，可能丢数据。 |
| 1    | which means that the producer gets an acknowledgement after the  leader replica has received the data.  当写Leader成功后就返回,其他的replica都是通过fetcher去同步的,所以kafka是异步写，主备切换可能丢数据。 |
| -1   | which means that the producer gets an acknowledgement after all  in-sync replicas have received the data.  要等到isr里所有机器同步成功，才能返回成功，延时取决于最慢的机器。强一致，不会丢数据。 |

在acks=-1的时候，如果ISR少于`min.insync.replicas`指定的数目，那么就会返回不可用。

这里ISR列表中的机器是会变化的，根据配置 `replica.lag.time.max.ms`，多久没同步，就会从ISR列表中剔除。以前还有根据落后多少条消息就踢出ISR，在`1.0`版本后就去掉了，因为这个值很难取，在高峰的时候很容易出现节点不断的进出ISR列表。

从ISA中选出leader后，follower会从把自己日志中上一个高水位后面的记录去掉，然后去和leader拿新的数据。因为新的leader选出来后，follower上面的数据，可能比新leader多，所以要截取。这里高水位的意思，对于partition和leader，就是所有ISR中都有的最新一条记录。消费者最多只能读到高水位；

从leader的角度来说高水位的更新会延迟一轮，例如写入了一条新消息，ISR中的broker都fetch到了，但是ISR中的broker只有在下一轮的fetch中才能告诉leader。

也正是由于这个高水位延迟一轮，在一些情况下，kafka会出现丢数据和主备数据不一致的情况，`0.11`开始，使用leader epoch来代替高水位。

### 3. 消费

订阅topic是以一个消费组来订阅的，一个消费组里面可以有多个消费者。同一个消费组中的两个消费者，不会同时消费一个partition。换句话来说，就是一个partition，只能被消费组里的一个消费者消费，但是可以同时被多个消费组消费。因此，如果消费组内的消费者如果比partition多的话，那么就会有个别消费者一直空闲。

![img](https://img-blog.csdnimg.cn/20190210162646705.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

#### 3.1 offset保存

一个消费组消费partition，需要保存offset记录消费到哪，以前保存在zk中，由于zk的写性能不好，以前的解决方法都是consumer每隔一分钟上报一次。这里zk的性能严重影响了消费的速度，而且很容易出现重复消费。

在`0.10`版本后，kafka把这个offset的保存，从zk总剥离，保存在一个名叫`__consumeroffsets`   topic的topic中。写进消息的key由groupid、topic、partition组成，value是偏移量offset。topic配置的清理策略是compact。总是保留最新的key，其余删掉。一般情况下，每个key的offset都是缓存在内存中，查询的时候不用遍历partition，如果没有缓存，第一次就会遍历partition建立缓存，然后查询返回。

确定consumer group位移信息写入`__consumers_offsets`的哪个partition，具体计算公式：

```java
__consumers_offsets partition =
	           Math.abs(groupId.hashCode() % groupMetadataTopicPartitionCount)   
	//groupMetadataTopicPartitionCount由offsets.topic.num.partitions指定，默认是50个分区。
123
```

#### 3.2 分配partition–reblance

生产过程中broker要分配partition，消费过程这里，也要分配partition给消费者。类似broker中选了一个controller出来，消费也要从broker中选一个coordinator，用于分配partition。

下面从顶向下，分别阐述一下

1. 怎么选coordinator
2. 交互流程
3. reblance的流程

#### 3.2.1 选coordinator

1. 看offset保存在那个partition
2. 该partition leader所在的broker就是被选定的coordinator

这里我们可以看到，consumer group的coordinator，和保存consumer group offset的partition leader是同一台机器。

#### 3.2.2 交互流程

把coordinator选出来之后，就是要分配了
 整个流程是这样的：

1. consumer启动、或者coordinator宕机了，consumer会任意请求一个broker，发送ConsumerMetadataRequest请求，broker会按照上面说的方法，选出这个consumer对应coordinator的地址。
2. consumer  发送heartbeat请求给coordinator，返回IllegalGeneration的话，就说明consumer的信息是旧的了，需要重新加入进来，进行reblance。返回成功，那么consumer就从上次分配的partition中继续执行。

#### 3.2.3 reblance流程

1. consumer给coordinator发送JoinGroupRequest请求。
2. 这时其他consumer发heartbeat请求过来时，coordinator会告诉他们，要reblance了。
3. 其他consumer发送JoinGroupRequest请求。
4. 所有记录在册的consumer都发了JoinGroupRequest请求之后，coordinator就会在这里consumer中随便选一个leader。然后回JoinGroupRespone，这会告诉consumer你是follower还是leader，对于leader，还会把follower的信息带给它，让它根据这些信息去分配partition
5. consumer向coordinator发送SyncGroupRequest，其中leader的SyncGroupRequest会包含分配的情况。
6. coordinator回包，把分配的情况告诉consumer，包括leader。
    当partition或者消费者的数量发生变化时，都得进行reblance。

列举一下会reblance的情况：

1. 增加partition
2. 增加消费者
3. 消费者主动关闭
4. 消费者宕机
5. coordinator自己也宕机

### 4. 消息投递语义

kafka支持3种消息投递语义

- At most once：最多一次，消息可能会丢失，但不会重复
- At least once：最少一次，消息不会丢失，可能会重复
- Exactly once：只且一次，消息不丢失不重复，只且消费一次（0.11中实现，仅限于下游也是kafka）

在业务中，常常都是使用At least once的模型，如果需要可重入的话，往往是业务自己实现。

#### 4.1 At least once

先获取数据，再进行业务处理，业务处理成功后commit offset。

1. 生产者生产消息异常，消息是否成功写入不确定，重做，可能写入重复的消息
2. 消费者处理消息，业务处理成功后，更新offset失败，消费者重启的话，会重复消费

#### 4.2 At most once

先获取数据，再commit offset，最后进行业务处理。

1. 生产者生产消息异常，不管，生产下一个消息，消息就丢了
2. 消费者处理消息，先更新offset，再做业务处理，做业务处理失败，消费者重启，消息就丢了

#### 4.3 Exactly once

思路是这样的，首先要保证消息不丢，再去保证不重复。所以盯着At least once的原因来搞。 首先想出来的：

生产者重做导致重复写入消息----生产保证幂等性
 消费者重复消费—消灭重复消费，或者业务接口保证幂等性重复消费也没问题

由于业务接口是否幂等，不是kafka能保证的，所以kafka这里提供的exactly  once是有限制的，消费者的下游也必须是kafka。所以一下讨论的，没特殊说明，消费者的下游系统都是kafka（注:使用kafka  conector，它对部分系统做了适配，实现了exactly once）。
 生产者幂等性好做，没啥问题。
 解决重复消费有两个方法：

1. 下游系统保证幂等性，重复消费也不会导致多条记录。
2. 把commit offset和业务处理绑定成一个事务。

本来exactly once实现第1点就ok了。
 但是在一些使用场景下，我们的数据源可能是多个topic，处理后输出到多个topic，这时我们会希望输出时要么全部成功，要么全部失败。这就需要实现事务性。既然要做事务，那么干脆把重复消费的问题从根源上解决，把commit  offset和输出到其他topic绑定成一个事务。

### Ref

1. <https://www.jianshu.com/p/d3e963ff8b70>