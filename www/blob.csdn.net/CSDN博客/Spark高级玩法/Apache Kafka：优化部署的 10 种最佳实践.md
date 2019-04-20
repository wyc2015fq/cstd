# Apache Kafka：优化部署的 10 种最佳实践 - Spark高级玩法 - CSDN博客
2018年11月21日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：109
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWopqsJUpmTY8c6XqxEial1vnn1sgeHceL4lqDdY3Gx0EFmFsQ4VRKVMDVJWeJXoptdmVaEMBLPrrw/640)
		作者 | Ben Bromhead	
		译者 | 冬雨	转自 | info
Apache Kafka 肯定会像它的同名小说家一样不负众望，因为它能激奋新来者、挑战深度，若能更全面的理解它还会产生丰厚的回报。抛开文学，书归正传。遵循 kafka 最新的最佳实践，一定可以让这个强大的数据流平台的管理变得非常、非常容易，而且还会相当有效。
这里有 10 个具体的技巧，可以帮助您优化 Kafka 部署并更容易管理：
- 
设置日志配置参数以使日志易于管理
- 
了解 kafka 的 (低) 硬件需求
- 
充分利用 Apache ZooKeeper
- 
以正确的方式设置复制和冗余
- 
注意主题配置
- 
使用并行处理
- 
带着安全性思维配置和隔离 Kafka
- 
通过提高限制避免停机
- 
保持低网络延迟
- 
利用有效的监控和警报
让我们详细分析一下这些最佳实践。
1 设置日志配置参数以使日志易于管理	
Kafka 为用户提供了大量的日志配置选项，虽然默认设置是合理的，但定制日志行为以满足您的特定需求将确保它们不会成为长期的管理挑战。这包括设置日志保留策略、清理、压缩和压缩活动。
可以使用 Log.segment.bytes、log.segment.ms、log.cleanup.policy (或主题级等价参数) 来控制日志行为。如果在应用场景中您不需要以前的日志，那么您可以使用 Kafka 删除某个文件大小的日志文件，或者通过设置 cleanup.policy 在一段时间之后再“删除”。您还可以将其设置为“compact”，以便在需要时保留日志。注意，要了解运行日志清理会消耗 CPU 和 RAM 资源；在将 Kafka 用于任何时间长度的操作日志时，一定要平衡压缩的频率和维持性能的需要。
压缩是 Kafka 确保每个消息键 (在单个主题分区的数据日志中) 至少保留最后一个已知值的过程。压缩操作处理主题中的每个键，以保留其最后的值，清理所有其他重复项。在删除的情况下，该 键保存成“null”值 (它被称为“墓碑（tombstone）”，因为它能生动地表示已删除)。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YriaiaJPb26VMBNicd0ZAP7r5rKoh8U5NqdgB9SFPYiagiagZLXKr08nSRl8Aa9rSBV4VWenJfYP98tTsxoichujFS4A/640)
**图 1 Kafka 提交日志压缩过程**
请参考 Kafka 操作日志文档：
- 
日志：
https://kafka.apache.org/documentation/#log
- 
压缩基础知识：
https://kafka.apache.org/documentation/#design_compactionbasics
2 了解 kafka(低) 硬件需求	
虽然许多不熟悉 Kafka 的团队会高估它的硬件需求，但其实这个解决方案的设计初衷是低开销和友好地水平扩展。这使得使用廉价的商品硬件并仍可以保持成功运行 Kafka 成为可能：
- 
CPU：除非需要 SSL 和日志压缩，否则 Kafka 不需要强大的 CPU。而且，使用的内核越多，并行性越好。而且在大多数情况下，压缩也不会产生影响，应该使用 LZ4 编解码器来提供最佳性能。
- 
RAM：在大多数情况下，Kafka 可以以 6 GB 的内存运行堆空间。对于特别重的生产负载，使用 32 GB 以上的机器。额外的 RAM 将用于支持 OS 页面缓存和提高客户端吞吐量。虽然 Kafka 可以以更少的 RAM 运行，但当可用的内存较少时，它处理负载的能力就会受到限制。
- 
磁盘：如果在 RAID 设置中使用多个驱动器，就该 Kafka 大显身手了。由于 Kafka 的顺序磁盘 I/O 范式，所以 SSD 不会提供太多的优势，不应该使用 NAS。
- 
网络和文件系统：建议使用 XFS，如果条件允许，还可以将集群放在单个数据中心。同时，应尽可能提供更多的网络带宽。
Apache Kafka 网站还包含一个专门的硬件和操作系统配置部分，提供了有价值的建议。
关于 Kafka 负载 / 性能测试的其他有价值的链接：
- 
Apache Kafka 的基准测试：每秒 200 万次写 (在 3 台廉价的机器上)
https://engineering.linkedin.com/kafka/benchmarking-apache-kafka-2-million-writes-second-three-cheap-machines
- 
在 AWS 上的 Apache Kafka 负载测试
https://grey-boundary.io/load-testing-apache-kafka-on-aws/
- 
性能测试
https://cwiki.apache.org/confluence/display/KAFKA/Performance+testing
3 充分利用 Apache ZooKeeper	
Apache ZooKeeper 集群的运行是 Kafka 运行的关键依赖项。但是当你在 kafka 旁边使用 ZooKeeper 的时候，一定要记住一些重要的最佳实践。
ZooKeeper 节点的数量最大应该是五个。一个节点适合于开发环境，三个节点对于大多数产品 Kafka 集群来说就足够了。虽然一个大型 Kafka 环境可能需要五个 ZooKeeper 节点来减少延迟，但是必须考虑节点上的负载。如果有七个或更多节点同步并处理请求，负载将变得非常大，性能可能会受到明显的影响。还需要注意的是，与早期版本相比，近期版本的 Kafka 对 Zookeeper 的负载要低得多，早期版本使用 Zookeeper 来存储消费者偏移。
最后一点，就像 Kafka 的硬件需求一样，为 ZooKeeper 提供最强大的网络带宽。使用最好的磁盘、分别存储日志、隔离 ZooKeeper 进程、禁用交换，这些也会减少延迟。
下表重点显示了不同 Kafka 版本中依赖于 Zookeeper 的一些控制台操作。早期版本 0.8.0 在控制台没有提供很多功能。从 0.10.0.0 开始，我们可以看到一些主要功能与 Zookeeper 分离开了，这就降低了 Zookeeper 的使用率。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VMBNicd0ZAP7r5rKoh8U5NqdOV05ibn3E4VpeVBiaGfz94OURqwD65t5KVAicqiaMIY6sZBOqdtTib58u4g/640)
适当的管理意味着 kafka 部署的弹性。一个重要的实践是将 Kafka 的默认复制因子从两个增加到三个，这一条在大多数生产环境中都合适。这样做可以确保一个代理出现问题不要太要紧，甚至两个代理都出问题了也不会中断可用性，尽管这种情况不太可能发生。另一个需要考虑的问题是数据中心机架区域。例如，如果使用 AWS， Kafka 服务器应该位于同一个区域，但是利用多个可用性区域来实现冗余和弹性。以正确的方式设置复制和冗余。
机架部署要考虑的 Kafka 配置参数是：
`broker.rack=rack-id`
如 Apache Kafka 文档所述：
当一个主题被创建、修改或复制被重新分发时，将遵守机架约束，确保复制能够跨尽可能多的机架，分区将尽可能分布在不同的机架上，在此，机架即为复制因子。
举个例子：
假设，9 个 Kafka 代理 (B1-B9) 分布在三个货架上。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VMBNicd0ZAP7r5rKoh8U5NqdvEEOibYFJBmOzeFla9h6mCAutKG7yYpleE3ee7iaDmd9qaZJpB8OAOjw/640)
**图 2 带有机架感知的 kafka 集群**
在这里，一个具有三个分区 (P1、P2、P3) 和三个复制因子 (R1、R2、R3) 的单一主题将在每个机架中为一个节点分配一个分区。这个场景中每个分区有两个副本，以此提供高可用性，即使一个完整的机架发生故障 (如图所示) 也可以保持正常运行。
4 注意主题配置	
主题配置对 Kafka 集群的性能有巨大的影响。因为更改设置 (如复制因子或分区计数) 可能很困难，所以您需要在第一次以正确的方式设置这些配置，然后在需要更改时简单地创建一个新主题 (一定要在准生产环境中测试新主题)。
使用三个复制因子，并仔细思考大型消息的处理。如果可能的话，将大的消息分解成有序的块，或者使用指向数据的指针 (比如指向 S3 的链接)。如果这些方法不可选，则在生产者一方启用压缩。默认的日志段大小是 1 GB，如果您的消息更大，就应该仔细检查一下用例了。分区计数也是一个非常重要的设置，将在下一节详细讨论。
主题配置有一个“服务器默认”属性。可以在主题创建时或稍后进行重写，以便具有特定于主题的配置。
如上所述，最重要的配置之一是复制因子。以下例子演示了从控制台创建主题的过程，复制因子为 3 个分区和 3 个分区，以及其他“主题级别”配置：
> 
bin/kafka-topics.sh --zookeeper ip_addr_of_zookeeper：2181 --create --topic my-topic –partitions 3 --replication-factor 3 --config max.message.bytes=64000 --config flush.messages=1
有关主题级别配置的完整介绍，请参阅这里的内容:
https://kafka.apache.org/documentation/#topicconfigs
5 使用并行处理	
Kafka 是为并行处理而设计的，和并行操作本身一样，充分利用它需要操作的平衡。分区计数是一个主题级设置，分区越多，并行性和吞吐量就越大。然而，分区也意味着更多的复制延迟、重平衡和打开服务器文件。
找到您的最佳分区设置很简单，就像计算您希望为您的硬件实现的吞吐量，然后计算所需的分区数量就可以了。按照保守的估计，一个主题上的一个分区可以传递 10 MB/s，根据这个估计可以推断出您需要的总吞吐量。另一种直接进行测试的方法是对每个主题使用一个代理，然后看看结果，如果需要更高的吞吐量，则将分区加倍。
总的来说，这里有条规则值得一用：主题的总分区数要低于 10，集群的总分区数要低于 10，000。如果您不这样做，那么需具有很高的监控能力，并且准备好处理可能非常具有挑战性的重平衡和中断!
创建 Kafka 主题时设置了分区的数量，如下所示。
> 
bin/kafka-topics.sh --zookeeper ip_addr_of_zookeeper：2181 --create --topic my-topic –partitions 3 --replication-factor 3 --config max.message.bytes=64000 --config flush.messages=1
创建分区后可以增加分区计数。但它会影响消费者，因此建议在处理完所有结果后再执行此操作。
> 
bin/kafka-topics.sh --zookeeper zk_host：port/chroot --alter --topic topic_name –partitions new_number_of_partitions
6 出于安全性考虑配置和隔离 Kafka	
确保 Kafka 部署的两个主要关注点是 1)Kafka 的内部配置，2)Kafka 运行的基础设施。
Kafka 的.9 版本包含了许多有价值的安全特性，例如 Kafka/client 和 Kafka/ZooKeeper 认证支持，以及对具有公共互联网客户端的保护系统的 TLS 支持。虽然 TLS 确实为吞吐量和性能带来了成本，但它有效且有价值地隔离并保护了 Kafka 代理的流量。
隔离 kafka 和 ZooKeeper 对安全至关重要。除极为罕见的情况之外，ZooKeeper 不应该连接到公共互联网，而应该只与 kafka(或它所使用的其他解决方案) 交互。防火墙和安全组应该隔离 Kafka 和 ZooKeeper，让代理处于一个单独的私有网络中，拒绝外部连接。中间件或负载平衡层应该将 Kafka 与公共互联网客户端隔离。
Kafka 的安全选项和协议：
- 
SSL/SASL：客户端到代理、中介代理、代理到工具的身份验证。
- 
SSL：客户端到代理之间、代理到代理之间和工具到代理之间的数据加密
- 
SASL 类型：SASL/GSSAPI (Kerberos)， SASL/PLAIN
- 
Zookeeper 安全性：为客户端 (代理、工具、生产者、消费者) 进行身份验证，使用 ACL 进行授权。
- 
Kafka 代理客户端：生产者、消费者、其他工具。
- 
ZooKeeper 客户：kafka 代理、生产者、消费者、其他工具。
- 
授权是可插拔的。
一个使用 SASL_SSL 进行安全设置的配置示例：
```
#Broker configuration
      listeners=SSL://host.name:port,SASL_SSL://host.name:port 
      advertised.listeners=SSL://host.name:port,SASL_SSL://host.name:port
      security.protocol=SASL_SSL 
      security.inter.broker.protocol=SSL 
      listener.security.protocol.map=INTERBROKER\:SSL,PUBLIC_CLIENT\:
SASL_PLAINTEXT,PRIVATE_CLIENT\:SASL_PLAINTEXT
       ssl.keystore.location=/var/private/ssl/server.keystore.jks
       ssl.keystore.password=test1234
       ssl.key.password=test1234
       ssl.truststore.location=/var/private/ssl/server.truststore.jks
       ssl.truststore.password=test1234
       sasl.mechanism.inter.broker.protocol=PLAIN 
       sasl.enabled.mechanisms=PLAIN 
#Client Configuration (jaas file)
       sasl.mechanism=PLAIN
       sasl.jaas.config=org.apache.kafka.common.security.plain.PlainLoginModule 
       required \
       username="[USER NAME]" 
       password="[USER PASSWORD]";
```
7 通过提高 Ulimit 避免停机	
一种经常发生的情况是：代理看起来从过多的负载降下来了，但实际上是一个 (尽管仍然有压力)“打开的文件太多”的良性错误。编辑 /etc/sysctl.conf 文件，配置 Ulimit 以允许 128，000 或更多的打开文件，可以避免发生这个错误。
增加 CentOS 上限的一个例子：
- 
创建一个新的文件：/etc/security/limits.d/nofile.conf
- 
输入内容：
- 
soft nofile 128000
- 
hard nofile 128000
重新启动系统或重新登录。
通过以下命令来验证：
**ulimit - a**
> 
注意，有多种方法可以增加 ulimit。您可以按照任何适合您自己的 Linux 发行版的方法来修改。
8 保持低网络延迟	
为了实现 Kafka 部署的低延迟，请确保代理位于离客户端最近的区域，并在选择云提供商提供的实例类型时一定要考虑网络性能。如果带宽阻碍了您的发展，那么可能就值得考虑投资一个更大更强力的服务器了。
9 利用有效的监控和警报	
在创建 Kafka 集群时，按照上面的做法，您可以在以后的工作中避免很多问题，但是您仍然需要保持警惕，在出现问题之前，提前正确识别和处理任何小问题。
监视系统指标 (如网络吞吐量、打开的文件句柄、内存、负载、磁盘使用情况和其他因素) 是必不可少的，同时还要密切关注 JVM 统计数据，包括 GC 暂停和堆使用情况。仪表板和历史回溯工具能够加速调试过程，可以提供大量的价值。与此同时，应该配置 Nagios 或 PagerDuty 等警报系统，以便在出现延迟峰值或磁盘空间不足等症状时发出警告，从而在小问题如滚雪球般越滚越大之前就能解决。
通过 Instaclustr 控制台中显示的 Kafka 监控图示例：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VMBNicd0ZAP7r5rKoh8U5NqdjoNggXJlT0uJy59jpEfuiaGVTYN9qRuxiaYdewPYSNLD3LNicT1PI0HGw/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VMBNicd0ZAP7r5rKoh8U5NqdOzCwibhrZE8rnAmYl5tz1I3YFvoqQVInl6gra13B6LlvAo9OK7zNlGA/640)![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VMBNicd0ZAP7r5rKoh8U5NqdGASA9aokvzVBk7OMFCNxxfb7AeIDSjsHfbBhvJy4P6N5ojnkIibT5ZQ/640)
		英文原文	
https://www.infoq.com/articles/apache-kafka-best-practices-to-optimize-your-deployment
推荐阅读：
[kafka源码系列之mysql数据增量同步到kafka](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484962&idx=1&sn=e92a2521395d862c1e16efc266837152&chksm=9f38e50aa84f6c1cd72d24a86938a3b320a5e41ab8bdb6939de7381192c023b066da9c0ce305&scene=21#wechat_redirect)
[必读：Spark与kafka010整合](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484604&idx=1&sn=c7fb09731502041562894ad9ef06a8ca&chksm=9f38e794a84f6e82dddf1b6c0b081f7f3e0512b85557e7165f87c443ca804f7af6e11846cffa&scene=21#wechat_redirect)
[大数据基础系列之kafka011生产者缓存超时，幂等性和事务实现](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483812&idx=1&sn=d49338e7a38fdc30a8d20c054b61d3fd&chksm=9f38e28ca84f6b9aa0d8a4f42ca48fa8711d1653bf5f57a8948cf1cb8e1c5967c1b83e46516c&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
分享给你的小伙伴吧～
