# Kafka源码系列之源码分析zookeeper在kafka的作用 - Spark高级玩法 - CSDN博客
2018年05月29日 00:26:11[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：758
浪尖的kafka源码系列以kafka0.8.2.2源码为例给大家进行讲解的。纯属个人爱好，希望大家对不足之处批评指正。
**一，zookeeper在分布式集群的作用**
**1，数据发布与订阅（配置中心）**
发布与订阅模型，即所谓的配置中心，顾名思义就是讲发布者将数据发布到zk节点上，共订阅者动态获取数据，实现配置的集中式管理和动态更新。例如，全局的配置信息，服务服务框架的地址列表就非常适合使用。
**2，负载均衡**
即软件负载均衡。最典型的是消息中间件的生产、消费者负载均衡。
**3，命名服务(Naming Service)**
常见的是发布者将自己的地址列表写到zookeeper的节点上，然后订阅者可以从固定名称的节点获取地址列表，链接到发布者进行相关通讯。
**4，分布式通知/协调**
这个利用的是zookeeper的watcher注册和异步通知机制，能够很好的实现分布式环境中不同系统间的通知与协调，实现对数据变更的实时处理。
**5，集群管理与Master选举**
集群管理，比如在线率，节点上线下线通知这些。Master选举可以使用临时顺序节点来实现。
**6，分布式锁**
分布式锁，这个主要得益于zookeeper数据的强一致性，利用的是临时节点。锁服务分为两类，一个是独占锁，另一个是控制时序。
独占，是指所有的客户端都来获取这把锁，最终只能有一个获取到。用的是临时节点。
控制时序，所有来获取锁的客户端，都会被安排得到锁，只不过要有个顺序。实际上是某个节点下的临时顺序子节点来实现的。
**7，分布式队列**
一种是FIFO，这个就是使用临时顺序节点实现的，和分布式锁服务控制时序一样。
第二种是等待队列的成员聚齐之后的才同意按序执行。实际上，是在队列的节点里首先创建一个/queue/num节点，并且赋值队列的大小。这样我们可以通过监控队列节点子节点的变动来感知队列是否已满或者条件已经满足执行的需要。这种，应用场景是有条件执行的任务，条件齐备了之后任务才能执行。
二，kafka 中的listener
1，kafka在zookeeper上的目录结构
**val ***ConsumersPath *= "/consumers"
**val ***BrokerIdsPath *= "/brokers/ids"
**val ***BrokerTopicsPath *= "/brokers/topics"
**val ***TopicConfigPath *= "/config/topics"
**val ***TopicConfigChangesPath *= "/config/changes"
**val ***ControllerPath *= "/controller"
**val ***ControllerEpochPath *= "/controller_epoch"
**val ***ReassignPartitionsPath *= "/admin/reassign_partitions"
**val ***DeleteTopicsPath *= "/admin/delete_topics"
**val ***PreferredReplicaLeaderElectionPath *= "/admin/preferred_replica_election"
下图是网络上最流行的kafka的zookeeper存储图。
**2，kafka的zkclient**
Kafka使用的是I0Itec.zkclient，https://github.com/sgroschupf/zkclient，github地址，与zk进行通信。
Zkclient对zookeeper的listener实现总共有四种：IZkStateListener(监听会话状态，是否进行了超时重连等)，IZkDataListener(监听节点数据的变动)，IZkChildListener(监听子节点的变动)，IZkConnection (监听链接)。
再赘述一下zookeeper的节点类型：持久节点（PERSISTENT），临时节点（EPHEMERAL）
持久顺序节点（PERSISTENT_SEQUENTIAL），临时顺序节点（EPHEMERAL_SEQUENTIAL）。
    下面对四种listener进行作用及在kafka中的实现进行详细讲解。
**1，IZkStateListener**
主要作用是**会话超时**的监控，需要在处理函数里重新注册临时节点。主要方法两个:
handleStateChanged,zookeeper的链接状态改变的时候调用
handleNewSession,与zookeeper的会话超时，导致断开并新连接建立的时候会调用。需要在此方法中实现临时节点的注册。在kafka中主要有以下四个实现:
**A),ZKSessionExpireListener**
是Kafka.consumer.ZookeeperConsumerConnector的内部类。所属对象为每个消费者的对象：ZookeeperConsumerConnector会话超时需要重新注册的临时节点为consumer的zknode临时节点：consumerGroupDir + "/ids"+consumerIdString。会导致消费者进行再平衡:loadBalancerListener.syncedRebalance()。
**B),SessionExpirationListener**
是kafka.controller.KafkaController的内部类。所属对象是每个Broker的Controller对象。维护的临时节点为"/controller"。会话超时会导致Crontroller再选举。
**C),ZkSessionExpireListener**
是kafka.consumer.ZookeeperTopicEventWatcher内部类。所属对象也是给ZookeeperTopicEventWatcher对象。作用是，每次会话超时事件触发后都会重新将ZkTopicEventListener和"/brokers/topics"的目进行绑定。ZkTopicEventListener负责会监控该目录下的子节点，也即topic的增删，最终调用WildcardStreamsHandler.handleTopicEvent。会在创建带topic过滤器的流的时候用到。createMessageStreamsByFilter具体请参考源码.
**D),SessionExpireListener**
Kafka.server.KafkaHealthcheck的内部类。监控的临时节点为"/brokers/ids"+brokerID。同时注册临时节点的时候会将advertisedHost:advertisedPort等信息写入该临时节点。是保障Broker存活在集群的关键。
**2，IZkDataListener**
该类及其实现，主要作用是监控zk节点**数据的变更**，来实现配置在集群中的更新。其子类要实现的方法有两个：
handleDataChange数据变动会调用该方法。
handleDataDeleted数据被删除会调用该方法。
A)**,ZKTopicPartitionChangeListener**
是Kafka.consumer.ZookeeperConsumerConnector的内部类。 "/brokers/topics"+topic监控的是消费者消费topic的节点。实现的方法handleDataChange，当topic节点存储的数据也即是partition信息(包括新增分区，分区迁移，leader变动)更新的时候，会调用该方法，并触发消费者的再平衡。
**B),PartitionsReassignedListener**
类的路径为Kafka.consumer.PartitionsReassignedListener。KafkaController的成员变量，监听的目录为"/admin/reassign_partitions"。通过管理命令往该节点写分区重分配策略，会触发分区的重分配，完成分区的迁移等动作。
**C),ReassignedPartitionsIsrChangeListener**
类的路径为Kafka.consumer.ReassignedPartitionsIsrChangeListener。监控的目录是每个partition的子节点state，如/brokers/topics/YourTopicName/partitions/21/state。假如我们需要将某些优先副本选为leader的时候会触发。
**D),PreferredReplicaElectionListener**
类的路径为Kafka.consumer.PreferredReplicaElectionListener。也是kafkaController内部对象。监控的目录是"/admin/preferred_replica_election"。根据给节点指定的优先副本列表，进行leader的重新选举。
**E),AddPartitionsListener**
该类是kafka.controller.PartitionStateMachine内部类及成员变量。监控的目录是每个topic的具体目录:"/brokers/topics/topic"，当新增分区的时候会触发该listener，然后做相关处理比如，让分区的leader上线等。
F),LeaderChangeListener
kafka.server.ZookeeperLeaderElector内部类及成员变量。Controller的选举的过程中用到了。监控的节点为/controller。
**3，IZkChildListener**
这种listener，主要是负责监听一个节点**子节点的变动**。只有一个要实现的方法:
handleChildChange(String parentPath, List<String> currentChilds)：
子节点变动的时候会调用该方法。我们可以在此实现自己的处理。在kafka中主要有以下几个实现:
**A),BrokerChangeListener**
是ReplicaStateMachine内部类及成员变量，监控的目录是"/brokers/ids"，当子节点有变动的时候会触发该listener，进而进行Broker故障处理和新Broker加入的处理。
**B),TopicChangeListener**
是PartitionStateMachine内部类及成员变量，监控的目录是"/brokers/topics"，当子节点变动的时候，会触发该函数，进而进入topic增加之后的相关处理。
**C),DeleteTopicsListener**
是PartitionStateMachine内部类及内部对象，监控的目录是" /admin/delete_topics"，当子节点变动的时候，会触发该函数，进而进入topic删除后相关处理操作。
**C),ZkTopicEventListener**
是kafka.consumer.ZookeeperTopicEventWatcher内部类及成员变量。也是在创建带topicfilter功能的消费流的时候用到，无论是删除topic还是增加topic都会重新初始化消费者。监控的目录是"/brokers/topics"。每个消费者维护一个。
**D),ConfigChangeListener**
TopicConfigManager的一个内部object和内部成员变量，监控的目录是"/config/changes"
方便用户进行topic的配置动态管理。
**E),ZKRebalancerListener**
是Kafka.consumer.ZookeeperConsumerConnector的内部类。每个消费者维护一个该对象，监控的目录是 /consumers/groupid/ids，当有新的conusmer加入或者有消费者死掉，都会触发该listener，进而会触发消费者再平衡。  
**4，IZkConnection **
Kafka里面没有用到。
**三，kafka用zookeeper实现的服务类型。**
**1，配置管理**
Topic的配置之所以能动态更新就是基于zookeeper做了一个动态全局配置管理。
**2，负载均衡**
基于zookeeper的消费者，实现了该特性，动态的感知分区变动，将负载使用既定策略分不到消费者身上。
**3，命名服务**
Broker将advertised.port和advertised.host.name，这两个配置发布到zookeeper上的zookeeper的节点上/brokers/ids/BrokerId(broker.id),这个是供生产者，消费者，其它Broker跟其建立连接用的。
**4，分布式通知**
比如分区增加，topic变动，Broker上线下线等均是基于zookeeper来实现的分布式通知。
**5，集群管理和master选举**
我们可以在通过命令行，对kafka集群上的topic partition分布，进行迁移管理，也可以对partition leader选举进行干预。
Master选举，要说有也是违反常规，常规的master选举，是基于临时顺序节点来实现的，序列号最小的作为master。而kafka的Controller的选举是基于临时节点来实现的，临时节点创建成功的成为Controller，更像一个独占锁服务。
**6，分布式锁**
独占锁，用于Controller的选举。
四，总结
    本文主要是结合kafka源码给大家讲解zookeeper的作用及kafka时怎么使用zookeeper的。希望会给大家带来对kafka的新的认识。并且希望能启发到大家，将zookeeper恰当的应用到我们自己的服务中去。
此文乃原创，部分摘取了网络，比如图片。浪尖在此谢谢了。
欢迎大家关注浪尖的公众号，一起奔赴分布式之旅。
