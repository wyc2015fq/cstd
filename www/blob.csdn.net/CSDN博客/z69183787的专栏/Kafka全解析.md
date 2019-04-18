# Kafka全解析 - z69183787的专栏 - CSDN博客
2018年05月15日 15:10:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：101
个人分类：[Mq-Kafka](https://blog.csdn.net/z69183787/article/category/7665372)
[https://blog.csdn.net/vinfly_li/article/details/79397201](https://blog.csdn.net/vinfly_li/article/details/79397201)
## 一. Concepts
Kafka is used for building real-time data pipelines and streaming apps
- 分布式消息传递
- 网站活跃数据跟踪
- 日志聚合
- 流式数据处理
- 数据存储
- 事件源
- ……
![image_1c41ppi4cefs1s25sg7aosjb79.png-90.5kB](http://static.zybuluo.com/vin123456/bmoyo3qo6p5my1lwixidqdan/image_1c41ppi4cefs1s25sg7aosjb79.png)
### Kafka terminology 术语
#### 1.Topics
Kafka maintains feeds of messages in categories called topics. 
消息都归属于一个类别成为topic,在物理上不同Topic的消息分开存储,逻辑上一个Topic的消息对使用者透明 
![image_1c41qimir1kmhh85pg5pnk3g16.png-88.5kB](http://static.zybuluo.com/vin123456/moymsc9l0gz3gpxznffs1jvz/image_1c41qimir1kmhh85pg5pnk3g16.png)
#### 2.Partitions
Topics are broken up into ordered commit logs called partitions 
每个Topics划分为一个或者多个Partition,并且Partition中的每条消息都被标记了一个sequential id ,也就是offset,并且存储的数据是可配置存储时间的 
![image_1c41qsc4d1tr5gum1rg3s314991j.png-45.8kB](http://static.zybuluo.com/vin123456/isr66cz0getpcplu0brixhqm/image_1c41qsc4d1tr5gum1rg3s314991j.png)
#### 3.Message Ordering
消息只保证在同一个Partition中有序,所以,如果要保证从Topic中拿到的数据有序,则需要做到:
- Group messages in a partition by key(producer)
- Configure exactly one consumer instance per partition within a consumer group
kafka能保证的是:
- Message sent by a producer to a particular topic partition will be appended in the order they are sent
- A consumer instance sees messages in the order they are stored in the log
- For a topic with replication factor N, kafka can tolerate up to N-1 server failures without “losing” any messages committed to the log
#### 4.Log
Partition对应逻辑上的Log
#### 5.Replication 副本
- Topics can (and should) be replicated
- The unit of replication is the partition
- Each partition in a topic has 1 leader and 0 or more replicas
- 
A replica is deemed to be “in-sync” if
- The replica can communicate with Zookeeper
- The replica is not “too far” behind the leader(configurable)
- 
The group of in-sync replicas for a partition is called the ISR(In-Sync-Replicas)
- The Replication factor cannot be lowered
#### 6.kafka durability 可靠性
Durability can be configured with the producer configuration `request.required.acks`
- 0 : The producer never waits for an ack
- 1 : The producer gets an ack after the leader replica has received the data
- -1 : The producer gets an ack after all ISRs receive the data
Minimum available ISR can also be configured such that an error is returned if enough replicas are not available to replicate data
所以,kafka可以选择不同的durability来换取不同的吞吐量
|Durability|Behaviour|Per Event Latency|Required Acknowledgements(request.required.acks)|
|----|----|----|----|
|Hignest|ACK all ISRs have received|Higest|-1|
|Medium|ACK once the leader has received|Medium|1|
|Lowest|No ACKs required|Lowest|0|
通用,kafka可以通过增加更多的Broker来提升吞吐量 
一个推荐的配置:|Property|Value|
|----|----|
|replication|3|
|min.insync.replicas|2|
|request.required.acks|-1|
7.Broker
Kafka is run as a cluster comparised of one or more servers each of which is called broker 
![image_1c43kfoau1r6lqq8k46moo1ijm9.png-97.7kB](http://static.zybuluo.com/vin123456/me363cp6t3db90wsju5am9rr/image_1c43kfoau1r6lqq8k46moo1ijm9.png)
#### 8.Producer
Processes that publish messages to a kafka topic are called producers
- Producers publish to a topic of their choosing(push) 
数据载入kafka可以是分布式的,通常是通过”Round-Robin”算法策略,也可以根据message中的key来进行语义分割”semantic partitioning”来分布式载入,Brokers 通过分区来均衡载入
- kafka支持异步发送async,异步发送消息是less durable的,但是是高吞吐的
- Producer的载入平衡和ISRs 
![image_1c43llqh611la1mkn18ir9tb1uh4m.png-48.7kB](http://static.zybuluo.com/vin123456/axdch9v7jp3n9h4e93le6toe/image_1c43llqh611la1mkn18ir9tb1uh4m.png)
#### 9.Consumer
Processes that subscribe(订阅) to tpics and process the feed of published messages are called consumers
- Multiple Consumer can read from the same topic
- Each Consumer is responsible for managing it’s own offset
- Message stay on kafka… they are not removed after they consumed 
![image_1c43mc7fncp5ecstopm851fn913.png-29.2kB](http://static.zybuluo.com/vin123456/nv9uamqjno0d893c9ixagac0/image_1c43mc7fncp5ecstopm851fn913.png)
Consumer可以从任一地方开始消费,然后又回到最大偏移量处,Consumers又可以被划分为Consumer Group
#### 10.Consumer Group
Consumer Group是显式分布式,多个Consumer构成组结构,Message只能传输给某个Group中的某一个Consumer
- 
常用的Consumer Group模式:
- All consumer instances in one group 
Acts like a traditional queue with load balancing
- All consumer instances in different groups 
All messages are broadcast to all consumer instances
- “Logical Subscriber” - Many consumer instances in a group 
Consumers are added for scalability and fault tolerance,Each consumer instance reads from one or more partitions for a topic ,There cannot be more consumer instances than partitions
![image_1c43tgqo01278rom1p8t1hbi123h2t.png-36.2kB](http://static.zybuluo.com/vin123456/iuf3xoayxmuatoqff3l46jpv/image_1c43tgqo01278rom1p8t1hbi123h2t.png)
Consumer Groups 提供了topics和partitions的隔离 
![image_1c43tioh07thbap1mnlphdscj3a.png-44.7kB](http://static.zybuluo.com/vin123456/k1xj39melbfgricvmf1h53wb/image_1c43tioh07thbap1mnlphdscj3a.png)
并且当某个Consumer挂掉后能够重新平衡
- 
Consumer Group的应用场景
- 点对点 
将所有消费者放到一个Consumer Group
- 广播 
将每个消费者单独放到一个Consumer Group
- 水平扩展 
向Consumer Group中添加消费者并进行Rebalance
- 故障转移 
当某个Consumer发生故障时,Consumer Group重新分配分区
## 二. Kafka 核心原理
### Kafka设计思想
- 可持久化Message 
持久化本地文件系统,设置有效期
- 支持高流量处理 
面向特定的使用场景而不是通用功能
- 消费状态保存在消费端而不是服务端 
减轻服务器负担和交互
- 支持分布式 
生产者/消费者透明异步
- 依赖磁盘文件系统做消息缓存 
不消耗内存
- 高效的磁盘存取 
复杂度为O(1)
- 强调减少数据的序列化和拷贝开销 
批量存储和发送、zero-copy
- 支持数据并行加载到Hadoop 
集成Hadoop
### Kafka原理分析
#### 1.存储
- 
Partition 
topic物理上的分组，一个topic可以分为多个partition，每个partition是一个有序的队列。 
在Kafka文件存储中，同一个topic下有多个不同partition，每个partition为一个目录，partiton命名规则为topic名称+有序序号，第一个partiton序号从0开始，序号最大值为partitions数量减1 
![image_1c4480qvd11df1j1k8fhbeq9mm.png-228kB](http://static.zybuluo.com/vin123456/vtquk7n1ciwtb5hkltra0kvp/image_1c4480qvd11df1j1k8fhbeq9mm.png)
每个partion(目录)相当于一个巨型文件被平均分配到多个大小相等segment(段)数据文件中。但每个段segment file消息数量不一定相等，这种特性方便old segment file快速被删除。 
每个partiton只需要支持顺序读写就行了，segment文件生命周期由服务端配置参数决定。 
这样做的好处就是能快速删除无用文件，有效提高磁盘利用率。
- 
segment file
- segment file组成：由2大部分组成，分别为index file和data file，此2个文件一一对应，成对出现，后缀”.index”和“.log”分别表示为segment索引文件、数据文件.
- segment文件命名规则：partion全局的第一个segment从0开始，后续每个segment文件名为上一个segment文件最后一条消息的offset值。数值最大为64位long大小，19位数字字符长度，没有数字用0填充。
![image_1c4486sih13qm937f0umam1trt13.png-34.2kB](http://static.zybuluo.com/vin123456/zs0593gs569ajoj8rt7hmf0d/image_1c4486sih13qm937f0umam1trt13.png)
其中`.index`索引文件存储大量元数据，`.log`数据文件存储大量消息，索引文件中元数据指向对应数据文件中message的物理偏移地址。他们两个是一一对应的,对应关系如下 
![image_1c448a5mqdom1rnk1ejcakj1c3k1g.png-106.1kB](http://static.zybuluo.com/vin123456/dxcyzqfbfo56p2w1hpx7d682/image_1c448a5mqdom1rnk1ejcakj1c3k1g.png)
- Message 
segment data file由许多message组成，message物理结构如下 
![image_1c448cn3rd81h8krbp1753h4m1t.png-62.3kB](http://static.zybuluo.com/vin123456/ouqzcve82su479hzesj830th/image_1c448cn3rd81h8krbp1753h4m1t.png)
参数说明:
|关键字|解释说明|
|----|----|
|8 byte offset|在parition(分区)内的每条消息都有一个有序的id号，这个id号被称为偏移(offset),它可以唯一确定每条消息在parition(分区)内的位置。即offset表示partiion的第多少message|
|4 byte message size|message大小|
|4 byte CRC32|用crc32校验message|
|1 byte “magic”|表示本次发布Kafka服务程序协议版本号|
|1 byte “attributes”|表示为独立版本、或标识压缩类型、或编码类型。|
|4 byte key length|表示key的长度,当key为-1时，K byte key字段不填|
|K byte key|可选|
|value bytes payload|表示实际消息数据。|
2. Consumer- High Level Consumer 
消费者保存消费状态：将从某个Partition读取的最后一条消息的offset存于ZooKeeper中
- Low Level Consumer：更好的控制数据的消费 
同一条消息读多次 
只读取某个Topic的部分Partition 
管理事务，从而确保每条消息被处理一次，且仅被处理一次 
大量额外工作 
必须在应用程序中跟踪offset，从而确定下一条应该消费哪条消息 
应用程序需要通过程序获知每个Partition的Leader是谁 
必须处理Leader的变化
#### 3.消息传递语义Delivery Semantics
- At least once 
kafka的默认设置 
Messages are never lost but maybe redelivered
- At most once 
Messages are lost but never redelivered
- Exactly once 
比较难实现 
Messages are delivered once and only once 
实现Exactly Once需要考虑: 
- Must consider two components 
Durability guarantees when publishing a message 
Durability guarantees when consuming a message
- Producer 
What happens when a produce request was sent but a network error returned before an ack ? 
RE:Use a single writer per partition and check the latest committed value after network errors
- Consumer 
include a unique ID(e.g.UUID) and de-duplicate 
Consider storing offsets with data
解释:
- 
消息传递语义: Producer 角度 
当Producer向broker发送消息时，一旦这条消息被commit，因为replication的存在，它就不会丢,但是如果Producer发送数据给broker后，遇到网络问题而造成通信中断，那Producer就无法判断该条消息是否已经commit 
理想的解决方案：Producer可以生成一种类似于主键的东西，发生故障时幂等性的重试多次，这样就做到了Exactly once,目前默认情况下一条消息从Producer到broker是确保了At least once
- 
消息传递语义: Consumer : High Level API 
Consumer在从broker读取消息后，可以选择commit，该操作会在Zookeeper中保存该Consumer在该Partition中读取的消息的offset 
该Consumer下一次再读该Partition时会从下一条开始读取；如未commit，下一次读取的开始位置会跟上一次commit之后的开始位置相同 
现实的问题：到底是先处理消息再commit，还是先commit再处理消息？
- 先处理消息再commit 
如果在处理完消息再进行commit之前Consumer发生宕机，下次重新开始工作时还会处理刚刚未commit的消息，实际上该消息已经被处理过了。这就对应于At least once 
业务场景使用幂等性：消息都有一个主键，所以消息的处理往往具有幂等性，即多次处理这一条消息跟只处理一次是等效的，那就可以认为是Exactly once。
- 先commit再处理消息 
如果Consumer在commit后还没来得及处理消息就宕机了，下次重新开始工作后就无法读到刚刚已提交而未处理的消息，这就对应于At most once
- 
消息传递语义: Consumer : Lower Level API 
Exactly once的实现思想：协调offset和消息数据 
经典做法是引入两阶段提交 
offset和消息数据放在同一个地方：Consumer拿到数据后可能把数据放到共享空间中，如果把最新的offset和数据本身一起写到共享空间，那就可以保证数据的输出和offset的更新要么都完成，要么都不完成，间接实现Exactly once 
High level API而言，offset是存于Zookeeper中的，无法获取，而Low level API的offset是由自己去维护的，可以实现以上方案
#### 4.高可用性
同一个Partition可能会有多个Replica，需要保证同一个Partition的多个Replica之间的数据一致性 
而这时需要在这些Replication之间选出一个Leader，Producer和Consumer只与这个Leader交互，其它Replica作为Follower从Leader中复制数据
- 
副本与高可用性：副本Leader Election算法
- Zookeeper中的选举算法回顾 
少数服从多数：确保集群中一半以上的机器得到同步 
适合共享集群配置的系统中，而并不适合需要存储大量数据的系统，因为需要大量副本集。f个Replica失败情况下需要2f+1个副本
- Kafka的做法 
ISR(in-sync replicas)，这个ISR里的所有副本都跟上了Leader，只有ISR里的成员才有被选为Leader的可能 
在这种模式下，对于f+1个副本，一个Partition能在保证不丢失已经commit的消息的前提下容忍f个副本的失败 
ISR需要的总的副本的个数几乎是“少数服从多数”的一半
- 
副本与高可用性：Replica分配算法 
将所有Replica均匀分布到整个集群 
将所有n个Broker和待分配的Partition排序 
将第i个Partition分配到第(i mod n)个Broker上 
将第i个Partition的第j个Replica分配到第((i + j) mode n)个Broker上
![image_1c4493m12109413lledr5st8qo26.png-97.5kB](http://static.zybuluo.com/vin123456/aiutgg34qf6lg0bggpsc6opp/image_1c4493m12109413lledr5st8qo26.png)
#### 5. 零拷贝
Kafka通过Message分组和Sendfile系统调用实现了zero-copy
- 
传统的socket发送文件拷贝 
![image_1c4498jqa15nphsndjb1dtcn02j.png-32kB](http://static.zybuluo.com/vin123456/4gez1ywmgv07clx9hvzl40lm/image_1c4498jqa15nphsndjb1dtcn02j.png)
1.内核态 
2.用户态 
3.内核态 
4.网卡缓存 
经历了内核态和用户态的拷贝
- 
sendfile系统调用 
![image_1c449ampeh081628cd7ad717bb3g.png-28.7kB](http://static.zybuluo.com/vin123456/qih69uks7babu5dul6ucoxh2/image_1c449ampeh081628cd7ad717bb3g.png)
避免了内核态与用户态的上下文切换动作
> 
sendfile 
优点：大块文件传输效率高 
缺点：小块文件效率较低、BIO而不是NIO 
mmap+write 
优点：使用小块文件传输时效率高 
缺点：比sendfile多消耗CPU、内存安全控制复杂 
应用 
Kafka使用第一种，大块数据传输 
RocketMQ使用第二种，小快数据传输
## 三. Use Cases
- Real-Time Stream Processing(combined with Spark Streaming)
- General purpose Message Bus
- Collecting User Activity Data
- Collecting Operational Metrics from applications,servers or devices
- Log Aggregation
- Change Data Capture
- Commit Log for distributed systems
![image_1c449qinbe82ha61tu21mmnn7p4q.png-112.5kB](http://static.zybuluo.com/vin123456/f2hmy13xmu1z3pg9bfb1jqe1/image_1c449qinbe82ha61tu21mmnn7p4q.png)
## 四. Development with Kafka
![image_1c449sb6f16cm1ei5jt1mafhkf57.png-110.8kB](http://static.zybuluo.com/vin123456/pamuvdhu1irjd0mi1kny56ly/image_1c449sb6f16cm1ei5jt1mafhkf57.png)
## 五. Administration
- list && describe
```
echo "此Kafka集群所有的Topic : "
kafka-topics --list --zookeeper dc226.dooioo.cn:2181, dc227.dooioo.cn:2181,dc229.dooioo.cn:2181/kafka
echo "您要查看的Topic详细 : "
kafka-topics --describe --zookeeper dc226.dooioo.cn:2181, dc227.dooioo.cn:2181,dc229.dooioo.cn:2181/kafka --topic $topicName
```
- 1
- 2
- 3
- 4
- 5
- 6
- create topic
```
kafka-topics --create --zookeeper dc226.dooioo.cn:2181,dc227.dooioo.cn:2181,dc229.dooioo.cn:2181/kafka --replication-factor 1 --pa
rtitions 1 --topic $topicName
```
- 1
- 2
- open producer
`kafka-console-producer --broker-list 10.22.253.227:9092 --topic $topicName `- 1
- open consumer
`kafka-console-consumer --zookeeper 10.22.253.226:2181,10.22.253.227:2181,10.22.253.229:2181/kafka --topic $topicName --from-beginning`- 1
## 六. Cluster Planning
## 七. Compare
![image_1c44a7mec16tck74h6crf9ob5k.png-78.8kB](http://static.zybuluo.com/vin123456/xawzcsa99x1qi70y2eyya2jz/image_1c44a7mec16tck74h6crf9ob5k.png)
![image_1c44a8aca12eb1ku713ra1lee183661.png-72.7kB](http://static.zybuluo.com/vin123456/5vnh3k6sktoaym61whcvogvj/image_1c44a8aca12eb1ku713ra1lee183661.png)
![image_1c44a93vu9fhibm1np41jdd1qto6e.png-171.7kB](http://static.zybuluo.com/vin123456/09rekrb768bum0ji5on8ip78/image_1c44a93vu9fhibm1np41jdd1qto6e.png)
