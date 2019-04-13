
# 为什么选择RocketMQ以及Confluent公司对Kafka的分区设计的说明 - 无界 - CSDN博客

2019年03月19日 01:27:33[21aspnet](https://me.csdn.net/21aspnet)阅读数：188


《[为什么选择RocketMQ](https://rocketmq.apache.org/docs/motivation/)》
根据我们的研究，随着使用的队列和虚拟主题的增加，ActiveMQ IO模块遇到了瓶颈。我们尽力通过节流，断路器或降级解决这个问题，但效果不佳。因此，我们开始关注当时流行的消息传递解决方案Kafka。不幸的是，Kafka无法满足我们的要求，特别是在低延迟和高可靠性方面，详见[此处](https://rocketmq.apache.org/rocketmq/how-to-support-more-queues-in-rocketmq/)。
《[如何在RocketMQ中支持更多队列？](https://rocketmq.apache.org/rocketmq/how-to-support-more-queues-in-rocketmq/)》
Kafka是一个分布式流媒体平台，它源于[日志聚合案例](https://engineering.linkedin.com/distributed-systems/log-what-every-software-engineer-should-know-about-real-time-datas-unifying)。它不需要太高的并发性。在阿里巴巴的一些大型案例中，我们发现原始模型无法满足我们的实际需求。因此，我们开发了一个名为RocketMQ的消息传递中间件，它可以处理广泛的用例，从传统的发布/订阅场景到要求不容许消息丢失的高要求的大容量实时事务系统。现在，在阿里巴巴，RocketMQ集群每天处理超过5000亿个事件，为超过3000个核心应用程序提供服务。
**Kafka的分区设计**
生产者写作的并行性受分区数量的限制。
消费者消费并行度的程度也受到消费的分区数量的限制。假设分区数为20，则最大并发消费消费者数为20。
每个主题都包含固定数量的分区。分区编号确定单个代理可能具有的最大主题数，而不会显着影响性能。
更多细节请参考[这里](http://www.confluent.io/blog/how-to-choose-the-number-of-topicspartitions-in-a-kafka-cluster/)。
**为什么Kafka不能支持更多分区**
每个分区都存储整个消息数据。尽管每个分区都按顺序写入磁盘，但随着并发写入分区数量的增加，从操作系统的角度来看，写入变得随机。
由于分散的数据文件，很难使用Linux IO组提交机制。
《[如何选择Kafka群集中的主题/分区数](https://www.confluent.io/blog/how-choose-number-topics-partitions-kafka-cluster)》
[https://www.confluent.io/blog/author/jun-rao/](https://www.confluent.io/blog/author/jun-rao/)强烈推荐看看
### 更多分区可提高吞吐量
首先要理解的是，主题分区是Kafka中并行性的单位。在生产者和代理端，对不同分区的写入可以完全并行完成。如此昂贵的操作（如压缩）可以利用更多的硬件资源。在消费者方面，Kafka总是将一个分区的数据提供给一个消费者线程。因此，消费者（在消费者群体内）的并行度受到消费的分区数量的限制。因此，通常，Kafka群集中的分区越多，可以实现的吞吐量越高。
选择分区数的粗略公式基于吞吐量。您可以在单个分区上测量您可以实现的生产（称为*p*）和消耗（称之为*c*）。假设您的目标吞吐量为*t*。然后你需要至少有*max（t / p，t / c）*分区。可以在生产者上实现的每分区吞吐量取决于配置，例如批处理大小，压缩编解码器，确认类型，复制因子等。但是，通常，只需要一个就可以产生10秒MB /秒单一分区如此[基准测试中](https://engineering.linkedin.com/kafka/benchmarking-apache-kafka-2-million-writes-second-three-cheap-machines)所示。消费者吞吐量通常取决于应用程序，因为它对应于消费者逻辑处理每条消息的速度。所以，你真的需要测量它。
虽然可以随着时间的推移增加分区数量，但是如果使用密钥生成消息，则必须要小心。在发布加密消息时，Kafka会根据密钥的哈希确定性地将消息映射到分区。这可以保证具有相同密钥的消息始终路由到同一分区。这种保证对于某些应用程序很重要，因为分区内的消息总是按照消费者的要求传递。如果分区数量发生变化，则此类保证可能不再成立。为避免这种情况，通常的做法是过度分区。基本上，您可以根据未来的目标吞吐量确定分区数，例如一年或两年后。最初，您可以根据当前吞吐量拥有一个小型Kafka群集。随着时间的推移，您可以向集群添加更多代理，并按比例将现有分区的子集移动到新代理（可以在线完成）。这样，您可以在不使用密钥时破坏应用程序中的语义的情况下跟上吞吐量的增长。
除了吞吐量之外，在选择分区数量时还有一些其他因素值得考虑。正如您将看到的，在某些情况下，分区太多也可能产生负面影响。
更多分区需要更多打开文件句柄
更多分区可能会增加不可用性
**更多分区可能会增加端到端延迟**
更多分区可能需要在客户端中有更多内存
《[Apache Kafka支持每个群集20万个分区](https://www.confluent.io/blog/apache-kafka-supports-200k-partitions-per-cluster)》
在Kafka中，主题可以包含分发记录的多个分区。分区是并行的单位。通常，更多分区会导致更高的吞吐量。但是，在Kafka群集中有更多分区时，应考虑一些因素。我很高兴地报告说，最近的Apache Kafka 1.1.0版本显着增加了单个Kafka集群可以从部署和可用性角度支持的分区数量。
通过这些改进，可以在Kafka中支持多少个分区？确切的数量取决于诸如可容忍的不可用性窗口，ZooKeeper延迟，代理存储类型等因素。根据经验，我们建议每个代理最多包含4,000个分区，每个群集最多包含200,000个分区。后一个群集范围限制的主要原因是为了适应控制器硬故障的罕见事件，如前所述。请注意，与分区相关的其他注意事项仍然适用，并且可能需要使用更多分区进行一些其他配置调
我们在1.1.0中所做的改进只是朝着实现Kafka无限可扩展的最终目标迈出的一步。我们还在1.1.0中使用更多分区对延迟进行了改进，并将在单独的博客中进行讨论。在不久的将来，我们计划进一步改进以支持Kafka集群中的数百万个分区。
《[Apache Kafka在大数据生态系统中的价值](https://www.confluent.io/blog/the-value-of-apache-kafka-in-big-data-ecosystem/)》
# 如何在RocketMQ中支持更多分区？
![](https://img-blog.csdnimg.cn/2019031901092928.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
所有消息数据都存储在提交日志文件中。所有写入都是完全顺序的，而读取是随机的。
ConsumeQueue存储实际的用户消费位置信息，这些信息也以顺序方式刷新到磁盘。
> 优点：
每个使用队列都是轻量级的，并且包含有限数量的元数据。
对磁盘的访问是完全顺序的，这可以避免磁盘锁争用，并且在创建大量队列时不会导致高磁盘IO等待。
> 缺点：
消息消耗将首先读取消耗队列，然后提交日志。在最坏的情况下，该过程会带来一定的成本。
提交日志和使用队列需要在逻辑上保持一致，这为编程模型带来了额外的复杂性。
> 设计动机：
随机读。尽可能多地读取以提高页面缓存命中率，并减少读取IO操作。如此大的内存仍然是可取的。如果累积大量消息，读取性能是否会严重降低？答案是否定的，原因如下：即使消息的大小仅为1KB，系统也会提前读取更多数据，请参阅[PAGECACHE预取](https://en.wikipedia.org/wiki/Cache_prefetching)以供参考。这意味着对于续集数据读取，它将访问将执行的主存储器而不是慢速磁盘IO读取。
从磁盘随机访问CommitLog。如果在SSD的情况下将I / O调度程序设置为NOOP，则读取qps将大大加速，因此比其他电梯调度程序算法快得多。
鉴于ConsumeQueue仅存储固定大小的元数据，主要用于记录消费进度，因此可以很好地支持随机读取。利用页面缓存预取，访问ConsumeQueue与访问主内存一样快，即使是在大量消息累积的情况下也是如此。因此，ConsumeQueue不会对读取性能带来明显的损失。
CommitLog几乎存储所有信息，包括消息数据。与关系数据库的重做日志类似，只要提交日志存在，就可以完全恢复消耗队列，消息密钥索引和所有其他所需数据。
|消息产品|客户端SDK|协议和规范|订购消息|预定消息|批量留言|广播消息|邮件过滤器|服务器触发重新传递|消息存储|消息追溯|消息优先级|高可用性和故障转移|消息跟踪|组态|管理和操作工具|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|ActiveMQ的|Java，.NET，C ++等|推模型，支持OpenWire，STOMP，AMQP，MQTT，JMS|独家消费者或独家排队可以确保订购|支持的|不支持|支持的|支持的|不支持|使用JDBC和高性能日志（如levelDB，kahaDB）支持非常快速的持久性|支持的|支持的|支持，根据存储，如果使用kahadb，则需要ZooKeeper服务器|不支持|默认配置为低级别，用户需要优化配置参数|支持的|
|Kafka|Java，Scala等|拉模型，支持TCP|确保在分区内对消息进行排序|不支持|支持，使用异步生产者|不支持|支持，您可以使用Kafka Streams过滤邮件|不支持|高性能文件存储|支持的偏移量表示|不支持|支持，需要ZooKeeper服务器|不支持|Kafka使用键值对格式进行配置。可以从文件或以编程方式提供这些值。|支持，使用terminal命令公开核心指标|
|RocketMQ|Java，C ++，Go|拉模型，支持TCP，JMS，OpenMessaging|确保严格的消息排序，并可以优雅地扩展|支持的|支持，使用同步模式以避免消息丢失|支持的|支持的基于SQL92的属性过滤器表达式|支持的|高性能和低延迟的文件存储|支持的时间戳和偏移量2表示|不支持|支持的Master-Slave模型，没有其他套件|支持的|开箱即用，用户只需要注意一些配置|支持的富Web和终端命令，用于公开核心指标|
《[Kafka的生成者、消费者、broker的基本概念](https://blog.csdn.net/u010020099/article/details/82290403)》![](https://img-blog.csdnimg.cn/20190319012058328.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

《[Confluent介绍（一）](https://www.cnblogs.com/zdfjf/p/5646525.html)》
最开始接触confluent是通过这篇博客，[How to Build a Scalable ETL Pipeline with Kafka Connect](http://www.confluent.io/blog/how-to-build-a-scalable-etl-pipeline-with-kafka-connect)，对于做大数据的，数据的ETL(抽取，转换，装载）是必不可少的。例如，要把传统的关系型数据库中的数据导入到HDFS里，或者导入到Hive中，进一步对数据进行分析，或者把json或者文本文件中的数据导入到大数据数据仓库中进行分析。这都需要ETL。这篇文章介绍了如何利用confluent的相关组件(Kafka Connect，构建一个ETL pipeline.下图来自于这篇博客。有兴趣的可以看一下这篇博客。
# confluent介绍：
LinkedIn有个三人小组出来创业了—正是当时开发出Apache Kafka实时信息列队技术的团队成员，基于这项技术Jay Kreps带头创立了新公司[Confluent](http://confluent.io/)。Confluent的产品围绕着Kafka做的。
### 什么是Confluent Platform?
Confluent Platform 是一个流数据平台，能够组织管理来自不同数据源的数据，拥有稳定高效的系统。
![](https://img-blog.csdnimg.cn/20190319012455722.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
Confluent Platform 不仅提供数据传输的系统， 还提供所有的工具：连接数据源的工具，应用， 以及数据接收。
### Confluent Platform 都包括什么？
Confluent Platform 很容易的建立实时数据管道和流应用。通过将多个来源和位置的数据集成到公司一个中央数据流平台，Confluent Platform使您可以专注于如何从数据中获得商业价值而不是担心底层机制，如数据是如何被运输或不同系统间摩擦。具体来说，Confluent Platform简化了连接数据源到Kafka，用Kafka构建应用程序，以及安全，监控和管理您的Kafka的基础设施。
![](https://img-blog.csdnimg.cn/20190319012518847.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
《[如何使用Kafka Connect构建可扩展的ETL管道](https://www.confluent.io/blog/how-to-build-a-scalable-etl-pipeline-with-kafka-connect/)》
在这篇博客中，我们使用Kafka Connect构建了一个ETL管道，它结合了JDBC和HDFS连接器。此管道捕获数据库中的更改并将更改历史记录加载到数据仓库，在本例中为Hive。
我们还演示了JDBC Kafka连接器和HDFS Kafka连接器提供的一些有用功能，例如更改捕获，模式迁移和自定义分区。要了解有关Kafka Connect的更多信息，包括设计，架构和用法，请访问[http://docs.confluent.io/current/connect/index.html](http://docs.confluent.io/current/connect/index.html?_ga=2.59104366.2134401213.1552923575-1497421650.1552923575)。
如果要开发新连接器，请访问[http://docs.confluent.io/current/connect/devguide.html上](http://docs.confluent.io/current/connect/devguide.html?_ga=2.59104366.2134401213.1552923575-1497421650.1552923575)的开发人员指南。
=======================
下面是阿里巴巴中间件团队的比较分析，
**这是几年前的，现在变化很快，不过其中的测试方法和一些理论性的东西值得学习。**
## 《
## [RocketMQ与kafka对比（18项差异）](http://jm.taobao.org/2016/03/24/rmq-vs-kafka/)
## 》
发表于 2016-04-20
2011年初，Linkin开源了Kafka这个优秀的消息中间件，淘宝中间件团队在对Kafka做过充分Review之后，Kafka无限消息堆积，高效的持久化速度吸引了我们，但是同时发现这个消息系统主要定位于日志传输，对于使用在淘宝交易、订单、充值等场景下还有诸多特性不满足，为此我们重新用Java语言编写了RocketMQ，定位于非日志的可靠消息传输（日志场景也OK），目前RocketMQ在阿里集团被广泛应用在订单，交易，充值，流计算，消息推送，日志流式处理，binglog分发等场景。
### 数据可靠性
RocketMQ支持异步实时刷盘，同步刷盘，同步复制，异步复制
卡夫卡使用异步刷盘方式，异步复制/同步复制总结：RocketMQ的同步刷盘在单机可靠性上比Kafka更高，不会因为操作系统Crash，导致数据丢失。

### 性能对比
Kafka[单机写入TPS约在百万条/秒，消息大小10个字节](http://engineering.linkedin.com/kafka/benchmarking-apache-kafka-2-million-writes-second-three-cheap-machines)
RocketMQ单机写入TPS单实例约7万条/秒，单机部署3个Broker，可以跑到最高12万条/秒，消息大小10个字节

### 消息投递实时性
Kafka使用短轮询方式，实时性取决于轮询间隔时间，0.8以后版本支持长轮询。
RocketMQ使用长轮询，同Push方式实时性一致，消息的投递延时通常在几个毫秒。

### 消费失败重试
Kafka消费失败不支持重试。
RocketMQ消费失败支持定时重试，每次重试间隔时间顺延

### 严格的消息顺序
Kafka支持消息顺序，但是一台代理宕机后，就会产生消息乱序
RocketMQ支持严格的消息顺序，在顺序消息场景下，一台Broker宕机后，发送消息会失败，但是不会乱序MySQL的二进制日志分发需要严格的消息顺序

### 定时消息
Kafka不支持定时消息
RocketMQ支持两类定时消息

### 消息查询

### 消息回溯
卡夫卡不支持消息查询
RocketMQ支持根据消息标识查询消息，也支持根据消息内容查询消息（发送消息时指定一个消息密钥，任意字符串，例如指定为订单编号）
总结：消息查询对于定位消息丢失问题非常有帮助，例如某个订单处理失败，是消息没收到还是收到处理出错了。
卡夫卡理论上可以按照偏移来回溯消息
RocketMQ支持按照时间来回溯消息，精度毫秒，例如从一天之前的某时某分某秒开始重新消费消息总结：典型业务场景如consumer做订单分析，但是由于程序逻辑或者依赖的系统发生故障等原因，导致今天消费的消息全部无效，需要重新从昨天零点开始消费，那么以时间为起点的消息重放功能对于业务非常有帮助。

### 消息消费并行度
Kafka的消费并行度依赖Topic配置的分区数，如分区数为10，那么最多10台机器来并行消费（每台机器只能开启一个线程），或者一台机器消费（10个线程并行消费）。即消费并行度和分区数一致。
RocketMQ消费并行度分两种情况
顺序消费方式并行度同卡夫卡完全一致
乱序方式并行度取决于Consumer的线程数，如Topic配置10个队列，10台机器消费，每台机器100个线程，那么并行度为1000。

### 消息堆积能力
理论上Kafka要比RocketMQ的堆积能力更强，不过RocketMQ单机也可以支持亿级的消息堆积能力，我们认为这个堆积能力已经完全可以满足业务需求。

## [Kafka、RabbitMQ、RocketMQ消息中间件的对比 —— 消息发送性能](http://jm.taobao.org/2016/04/01/kafka-vs-rabbitmq-vs-rocketmq-message-send-performance/)
发表于 2016-04-20
RabbitMQ是使用Erlang语言开发的开源消息队列系统，基于AMQP协议来实现。AMQP的主要特征是面向消息、队列、路由（包括点对点和发布/订阅）、可靠性、安全。AMQP协议更多用在企业系统内，对数据一致性、稳定性和可靠性要求很高的场景，对性能和吞吐量的要求还在其次。
对比Kafka、RabbitMQ、RocketMQ发送小消息(124字节)的性能。这次压测我们只关注服务端的性能指标,所以压测的标准是:
**不断增加发送端的压力,直到系统吞吐量不再上升,而响应时间拉长。这时服务端已出现性能瓶颈,可以获得相应的系统最佳吞吐量。**
![](https://img-blog.csdnimg.cn/20190327173002103.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
Kafka的吞吐量高达**17.3w/s**，不愧是高吞吐量消息中间件的行业老大。这主要取决于它的队列模式保证了写磁盘的过程是**线性IO**。此时broker磁盘IO已达瓶颈。
RocketMQ也表现不俗，吞吐量在**11.6w/s**，磁盘IO %util已接近100%。RocketMQ的消息写入内存后即返回ack，由**单独的线程**专门做**刷盘**的操作，所有的消息均是**顺序写文件**。
RabbitMQ的吞吐量5.95w/s，CPU资源消耗较高。它支持AMQP协议，实现非常重量级，为了保证消息的可靠性在吞吐量上做了取舍。我们还做了RabbitMQ在消息持久化场景下的性能测试，吞吐量在2.6w/s左右。

## [Kafka vs RocketMQ——单机系统可靠性 ](http://jm.taobao.org/2016/04/28/kafka-vs-rocktemq-4/)
发表于 2016-04-20
分别模拟Broker服务进程被Kill、物理机器掉电的异常场景，多次实验，查看极端情况下消息系统的可靠性。
使用多个发送端向一个Topic发送消息，发送方式为同步发送，分区数为8，只启动一个订阅者。
### 场景1.
### 模拟进程退出
在Broker进程被终止重启，Kafka和RMQ都能保证同步发送的消息不丢，因为进程退出后操作系统能确保将该进程遗留在内存的数据刷到磁盘上。实验中，Kafka出现了极少量的消息重复。再次可以确定此场景中，二者的可靠性都很高。
### 场景2.
### 模拟机器掉电
即使在并发很低的情况下，Kafka和RMQ都无法保证掉电后不丢消息。这个时候，就需要改变刷盘策略了。我们把刷盘策略由“异步刷盘”变更为“同步刷盘”，就是说，让每一条消息都完成存储后才返回，以保证消息不丢失。
设置同步刷盘时，二者都没出现消息丢失的情况。限于我们使用的是普通PC机器，两者吞吐量都不高。此时Kafka的最高TPS仅有500条/秒，RMQ可以达到4000条/秒，已经是Kafka的8倍。
为什么Kafka的吞吐量如此低呢？因为Kafka本身是没有实现任何同步刷盘机制的，就是说在这种场景下测试，Kafka注定是要丢消息的。但要想做到每一条消息都在落盘后才返回，我们可以通过修改异步刷盘的频率来实现。设置参数log.flush.interval.messages=1，即每条消息都刷一次磁盘。这样的做法，Kafka也不会丢消息了，但是频繁的磁盘读写直接导致性能的下降。
另外，二者在服务恢复后，均出现了消息重复消费的情况，这说明消费位点的提交并不是同步落盘的。不过，幸好Kafka和RMQ都提供了自定义消费位点的接口，来避免大量的重复消费。
## 测试结论
在Broker进程被Kill的场景， Kafka和RocketMQ都能在保证吞吐量的情况下，不丢消息，可靠性都比较高。
在宿主机掉电的场景，Kafka与RocketMQ均能做到不丢消息，此时Kafka的吞吐量会急剧下跌，几乎不可用。RocketMQ则仍能保持较高的吞吐量。
在单机可靠性方面，RocketMQ综合表现优于Kafka。

### 同步刷盘和异步刷盘的区别
![](https://img-blog.csdnimg.cn/201903271735480.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
同步刷盘是在每条消息都确认落盘了之后才向发送者返回响应；而异步刷盘中，只要消息保存到Broker的内存就向发送者返回响应，Broker会有专门的线程对内存中的消息进行批量存储。所以异步刷盘的策略下，当机器突然掉电时，Broker内存中的消息因无法刷到磁盘导致丢失。

## [Kafka vs RocketMQ—— Topic数量对单机性能的影响 ](http://jm.taobao.org/2016/04/07/kafka-vs-rocketmq-topic-amout/)
发表于 2016-04-20
Topic是消息中间件里一个重要的概念，每一个Topic代表了一类消息，有了多个Topic，就可以对消息进行**归类与隔离**。
Kafka和RocketMQ都是磁盘消息队列的模式，对于**同一个消费组**，**一个分区只支持一个消费线程来**消费消息。过少的分区，会导致消费速度大大落后于消息的生产速度。所以在实际生产环境中，一个Topic会设置成多分区的模式，来支持多个消费者。
默认每个Topic的分区数为8，每个Topic对应一个订阅者，逐步增加Topic数量。
可以看到，不论Topic数量是多少，Kafka和RocketMQ均能保证发送端和消费端的TPS持平，就是说，保证了消息没有累积。
根据Topic数量的变化，画出二者的消息处理能力的对比曲线如下图：
![](https://img-blog.csdnimg.cn/20190327174245994.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
从图上可以看出：
Kafka在Topic数量由64增长到256时，吞吐量下降了98.37%。
RocketMQ在Topic数量由64增长到256时，吞吐量只下降了16%。
为什么两个产品的表现如此悬殊呢？这是因为Kafka的每个Topic、每个分区都会对应一个物理文件。当Topic数量增加时，消息分散的落盘策略会导致磁盘IO竞争激烈成为瓶颈。而RocketMQ所有的消息是保存在同一个物理文件中的，Topic和分区数对RocketMQ也只是逻辑概念上的划分，所以Topic数的增加对RocketMQ的性能不会造成太大的影响。
## 测试结论
**在消息发送端，消费端共存的场景下，随着Topic数的增加Kafka吞吐量会急剧下降，而RocketMQ则表现稳定。因此Kafka适合Topic和消费端都比较少的业务场景，而RocketMQ更适合多Topic，多消费端的业务场景。**

## [Kafka vs RocketMQ——多Topic对性能稳定性的影响 ](http://jm.taobao.org/2016/04/20/kafka-vs-rocketmq-3/)
发表于 2016-04-20
稳定性测试：测试系统的长期稳定运行能力。在系统运行过程中，对系统施压，观察系统的各种性能指标以及服务器的负载指标。
默认每个Topic的分区数为8，每个Topic对应一个订阅者，逐步增加Topic的数量，这里性能是否抖动根据趋势图做直观的判断。
Kafka在32和64个Topic时，就已经出现了不稳定的情况。下面看一下32和64个Topic的详细数据，如下图所示：
![](https://img-blog.csdnimg.cn/20190327174555264.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
蓝色Kafka的TPS曲线在18分钟以后，就开始上下波动，毫无规律，而RocketMQ则表现稳定。
下面再看64个Topic的情况，如下图所示：
![](https://img-blog.csdnimg.cn/20190327174631160.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
Kafka的TPS在前20分钟保持稳定，并大幅度领先RocketMQ。20分钟后又开始出现不规则波动，这些波动直接导致响应时间的变化（图4），某个时刻Kafka的客户端响应时间会达到25毫秒，而RocketMQ全程都是5毫秒。![](https://img-blog.csdnimg.cn/20190327174742534.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
这次的对比3个场景中， Kafka胜出一个，就是8个Topic的场景，如图5所示，由于Topic个数和分区数的限制，导致Kafka只适合小规模的业务系统。
![](https://img-blog.csdnimg.cn/20190327174826613.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
## 测试结论
Topic数的增加对RocketMQ无影响，长时间运行服务非常稳定。
Kafka 的Topic数量建议不要超过8个。8个以上的Topic会导致Kafka响应时间的剧烈波动，造成部分客户端的响应时间过长，影响客户端投递的实时性以及客户端的业务吞吐量。
=======================
## 《
## [分布式开放消息系统(RocketMQ)的原理与实践](https://www.jianshu.com/p/453c6e7ff81c)
## 》
关键特性以及其实现原理
## 一、顺序消息
RocketMQ通过轮询所有队列的方式来确定消息被发送到哪一个队列（负载均衡策略）。比如下面的示例中，**订单号相同**的消息会被先后发送到同一个队列中：
```python
// RocketMQ通过MessageQueueSelector中实现的算法来确定消息发送到哪一个队列上
// RocketMQ默认提供了两种MessageQueueSelector实现：随机/Hash
// 当然你可以根据业务实现自己的MessageQueueSelector来决定消息按照何种策略发送到消息队列中
SendResult sendResult = producer.send(msg, new MessageQueueSelector() {
    @Override
    public MessageQueue select(List<MessageQueue> mqs, Message msg, Object arg) {
        Integer id = (Integer) arg;
        int index = id % mqs.size();
        return mqs.get(index);
    }
}, orderId);
```
在获取到路由信息以后，会根据`MessageQueueSelector`实现的算法来选择一个队列，同一个OrderId获取到的肯定是同一个队列。
```python
private SendResult send()  {
    // 获取topic路由信息
    TopicPublishInfo topicPublishInfo = this.tryToFindTopicPublishInfo(msg.getTopic());
    if (topicPublishInfo != null && topicPublishInfo.ok()) {
        MessageQueue mq = null;
        // 根据我们的算法，选择一个发送队列
        // 这里的arg = orderId
        mq = selector.select(topicPublishInfo.getMessageQueueList(), msg, arg);
        if (mq != null) {
            return this.sendKernelImpl(msg, mq, communicationMode, sendCallback, timeout);
        }
    }
}
```

## 二、消息重复
上面在解决消息顺序问题时，引入了一个新的问题，就是消息重复。那么RocketMQ是怎样解决消息重复的问题呢？还是“恰好”不解决。
造成消息重复的根本原因是：网络不可达。只要通过网络交换数据，就无法避免这个问题。所以解决这个问题的办法就是绕过这个问题。那么问题就变成了：如果消费端收到两条一样的消息，应该怎样处理？
> 消费端处理消息的业务逻辑保持幂等性

> 保证每条消息都有唯一编号且保证消息处理成功与去重表的日志同时出现
**RocketMQ不保证消息不重复，如果你的业务需要保证严格的不重复消息，需要你自己在业务端去重。**

## 三、事务消息
同样是一个转账的业务，在集群环境下，耗时居然成倍的增长，这显然是不能够接受的。那如何来规避这个问题？
> 大事务 = 小事务 + 异步
将大事务拆分成多个小事务异步执行。这样基本上能够将跨机事务的执行效率优化到与单机一致。转账的事务就可以分解成如下两个小事务。
RocketMQ支持事务消息，下面来看看RocketMQ是怎样来实现的。
RocketMQ第一阶段发送`Prepared消息`时，会拿到消息的地址，第二阶段执行本地事务，第三阶段通过第一阶段拿到的地址去访问消息，并修改消息的状态。
如果确认消息发送失败了，RocketMQ会定期扫描消息集群中的事务消息，如果发现了`Prepared消息`，它会向消息发送端(生产者)确认，RocketMQ会根据发送端设置的策略来决定是回滚还是继续发送确认消息。这样就保证了消息发送与本地事务同时成功或同时失败。
消费端开始消费这条消息，这个时候就会出现消费失败和消费超时两个问题，解决超时问题的思路就是一直重试，直到消费端消费消息成功，整个过程中有可能会出现消息重复的问题，按照前面的思路解决即可。
这样基本上可以解决消费端超时问题，但是如果消费失败怎么办？阿里提供给我们的解决方法是：**人工解决**。大家可以考虑一下，按照事务的流程，因为某种原因失败，那么需要回滚整个流程。如果消息系统要实现这个回滚流程的话，系统复杂度将大大提升，且很容易出现Bug，估计出现Bug的概率会比消费失败的概率大很多。这也是RocketMQ目前暂时没有解决这个问题的原因，在设计实现消息系统时，我们需要衡量是否值得花这么大的代价来解决这样一个出现概率非常小的问题，这也是大家在解决疑难问题时需要多多思考的地方。
> 在3.2.6版本中移除了事务消息的实现，所以此版本不支持事务消息
----------
《[消息中间件—RocketMQ消息存储（二）](https://www.jianshu.com/p/6d0c118c17de)》
作者对MQ源码分析很到位，画图很好：
![](https://img-blog.csdnimg.cn/20190329161912233.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

