# 消息队列服务Kafka揭秘：痛点、优势以及适用场景 - Spark高级玩法 - CSDN博客
2018年08月12日 00:10:35[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1702
**摘要**：消息队列Kafka是一个分布式的、高吞吐量、高可扩展性消息队列服务，广泛用于日志收集、监控数据聚合、流式数据处理、在线和离线分析等，是大数据生态中不可或缺的产品之一。本文就将带你走进消息队列Kafka。本文摘自阿里云栖社区。
**以下内容根据演讲视频以及PPT整理而成。**
视频分享
http://click.aliyun.com/m/1000012118/
PPT下载链接
http://click.aliyun.com/m/1000012119/
**消息队列Kafka**
消息队列Kafka是一个分布式的、高吞吐量、高可扩展性消息队列服务。相比于Apache Kafka，消息队列Kafka所提供的是全托管的服务。这里也简单地介绍一下Apache Kafka，Apache Kafka是一个分布式的基于push-subscribe的消息系统，它具备快速、可扩展、可持久化的特点。它现在是Apache旗下的一个开源系统，作为hadoop生态系统的一部分，目前被广泛使用在大数据场景中。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUMBgnATnUmVHh6elwfAOcvDZiaiaW4QgJANQKO3Hw6sGE58mH25asCXkFvBDemXqImNWM30N8kFJ1w/640?wx_fmt=png)
而消息队列 Kafka 针对Apache Kafka提供全托管服务，彻底解决开源产品长期以来的痛点。用户只需专注于业务开发，无需部署运维，低成本、更弹性、更可靠。消息队列产品最大的特点就是全托管服务，这里主要涉及到两个特性：兼容性和便捷性。首先，对于兼容性，消息队列Kafka能够100%兼容Apache Kafka，对于用户而言，可使用各种语言的开源客户端进行无缝接入，目前使用开源Kafka的用户，只需要更改一个接入点就可以使用消息队列Kafka产品。同时，消息队列Kafka兼容Apache Kafka的所有生态。其次，对于便捷性而言，消息队列Kafka不需要部署，用户只要在购买消息队列Kafka后，填入实例信息，15分钟内就能使用消息队列Kafka的服务了，因此是非常便捷易用的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUMBgnATnUmVHh6elwfAOcvZnAquWL8Iqic20er6xxepkjlcQ21STvGZJG6c5Xj5B95LiaeqSoy4nZA/640?wx_fmt=png)
上述是对于消息队列Kafka的整体介绍，接下来将分为痛点、优势以及场景这三个模块与大家进行较为细节的分享。首先将与大家分享目前阿里云针对于消息队列服务所收集到的用户痛点，以及根据这些痛点来解决问题，消息队列Kafka所具备的优势又是什么，最后还将与大家介绍一下消息队列Kafka所适用的场景。
**痛点：自建Kafka的烦恼**
**Apache Kafka运维难度大**
对于Kafka而言，从用户视角来看，是一个非常简单的产品，其所提供的是发布与订阅模型。那么，在对于Kafka进行运维方面而言，其难度又会非常大，这是因为它不仅仅需要关注整个集群之内像broker、controller类似的角色，还需要关注其所依赖的一些产品像ZooKeeper等。所以对于以上这些模块的运维不仅仅涉及到参数的调优，同时随着业务的增长，还会面临扩缩容等问题。此外，还需要关注磁盘以及网络情况。因此，综上所述，自建Kafka的运维成本和运维难度都是非常大的。接下来就为大家分享一些具体的例子。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUMBgnATnUmVHh6elwfAOcvuf8jgph6Mw2TjHyfFOoFMeyBrc0hguopWaCtedibx18plHWlewewOxA/640?wx_fmt=png)
**数据混乱**
一些用户反馈在自己使用Kafka集群的时候出现了数据混乱的问题。大家都知道，在Kafka集群里面存在Controller和Broker两种角色，在Controller出现异常的情况之下，会从Broker里面自动地选择一个Broker成为新的Controller。但是由于网络等异常情况，最开始挂掉的Controller可能重新复活了，那么在复活之后，对于整个集群而言就会出现“脑裂”的情况。因为Controller的主要职责是管理整个集群的分区和副本的状态，而当出现“脑裂”就会造成数据混乱的问题，而这对于用户而言，是不可接受的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUMBgnATnUmVHh6elwfAOcvq6e1EBVnXibia1zGXia6O4VvNv7Iib5q0UFRKaEpk1MWoLptNZdyYKcBFA/640?wx_fmt=png)
**ZooKeeper不可用**
整个Kafka集群对于ZK是强依赖的，而ZooKeeper的运维工作也是庞大而复杂的。比如在运维人员对于ZooKeeper不是非常了解的情况之下，可能不知道如何部署ZooKeeper，不知道如何保证ZK在同机房或者多机房的情况下保证其一定可用，而这些往往需要运维人员的思考和权衡。而ZooKeeper上面会存储Kafka的重要数据，当ZK不可用的情况下，整个集群的灾备选组以及存储的数据都会受到影响。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUMBgnATnUmVHh6elwfAOcveV7Nz2IqkKqFvuG6124AOqeRtylWeJn4HYpiaUL28zhib17DPCWSqIKw/640?wx_fmt=png)
**带宽关注**
对于用户而言，自建Kafka时不仅仅需要关注其外围的依赖产品，其实还需要关注一个在集群内部经常会遇到的问题——带宽。站在用户的使用角度来看，经常需要做出对于副本数的权衡。而为了提升可靠性以及容灾能力，集群往往需要三副本，而当副本数量一多，那么就会涉及到机器之间的数据复制，这种情况就会增加网络的带宽。同时，由于Broker之间是对等的，并且需要从Controller里面同步数据。这样一来，Controller不仅仅需要承担自己的本身的任务，还需要对外提供服务，而就其本身的设计而言，这两部分任务是没有优先级先后的，所以在集群规模大的情况之下，就会引发网络带宽的拥堵问题。而阿里云消息队列Kafka就已经帮助用户解决了上述问题了，用户不需要去做备份之间的权衡，阿里云会帮助用户实现数据的三副本存储，并且使得服务可用性能够达到99.9%
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUMBgnATnUmVHh6elwfAOcvsbBkLYicdShg1D2ibnaGF5ymfkAAuMNVksdVUUuAeic0sDMw1vQPC8AOw/640?wx_fmt=png)
**磁盘运维**
用户自建Kafka还会遇到其他的一些问题，比如磁盘的运维问题。从0.110版本之后，Consumer offsets不仅仅存储在ZK端，其可以作为一个普通的Topic存储在Kafka集群里面。而整个Consumer offsets的留存策略决定了磁盘的占用情况，因此有可能因为设置了错误的参数导致磁盘的占用过高。同时，用户经常看到的情况是：自己的集群有100T的磁盘，仅仅使用了几十T就已经出现了不可写的情况。大家都知道，在Producer里面可以通过两种方式对于数据进行分区，通过Hash可能会造成Hash的倾斜，而使用RoundBobin的方式也可能导致磁盘占用不均。对于用户而言，其可能看到的情况是用户明明购买了很多的磁盘，磁盘也没有被占满，但是Producer却已经不可写了。而关于磁盘运维的细节问题，消息队列Kafka就已经帮助用户解决掉了。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUMBgnATnUmVHh6elwfAOcvAM69AfHBjGpGRI9lEoR1ee7Wibq2IX3XQibg95VZOdQtdXHOG6YsHnibA/640?wx_fmt=png)
**数据丢失**
其实，对于用户而言，最苦恼的就是数据丢失问题。Kafka为用户提供了三种数据存储策略，第一种可以认为是OneWay方式，第二种相当于将一个备份的数据落盘，最后一种相当于将所有备份数据落盘才能成功。对于这三种方式的选择过程，其实就是可用性与性能之间的博弈。在网络负载很高或者磁盘很难写入的情况下，就可能造成磁盘写入失败。同时，Kafka的数据最开始是存储在PageCache上面的，并且会定时地刷到磁盘上，但是并不是每条消息发送成功都会存储在磁盘上的。如果出现断电或者机器故障的情况，存储在内存中的数据就会丢失。此外，还有一种情况就是当单批数据量超过限制也会丢失数据。而使用消息队列Kafka，用户就不需要去做这些数据上面的选型的博弈和考虑，因为只要消息队列Kafka发送数据成功，那么这些数据就会被持久化，保证了数据不会丢失。因为消息队列Kafka做了这些优化，数据的可靠性就能够达到8个9(即99.999999%)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUMBgnATnUmVHh6elwfAOcv7PCvm9Kfh4os5Ql4a0U4rsia3xpeTQlKTNrxlLicTE1PehC6bRsXsStQ/640?wx_fmt=png)
**消息队列Kafka的优势**
上述与大家分享的就是消息队列Kafka的优势，再来总结一下。消息队列Kafka是完全兼容Apache Kafka的，Apache Kafka所能够用到的整个生态的产品，比如上端的Flume等产品和下端的Spark、Storm、Flink以及ES等，对于消息队列Kafka而言也是完全兼容的。其次，消息队列Kafka所提供的是全托管的服务，也就是说无论集群中出现的是磁盘问题、网络问题也好，无论是Kafka本身的还是其所依赖的产品所出现的任何问题，都是有专业团队来解决的。对于用户而言，所能够看到的是产品99.9%的可用性，并且能够为用户带来非常稳定的状态，而底层的技术细节则是由阿里云专业的团队来处理的。对于高可用以及高可靠这部分而言，其与全托管是存在强关联的。对于数据的可靠性而言，都是每一个产品所最为重视的，因为当发生了数据的丢失，就可能使得整个的业务逻辑出现错误，进而引发一些重大的故障。而阿里云所承诺的是当用户使用消息队列Kafka发送消息，只要返回所发送的消息是成功的，那么这个数据的可靠性就能够达到8个9，这一点也是用户所无需担心的。同时，阿里云消息队列为用户提供了非常实用的业务报表以及灵活全面的业务监控体系，并且业务的监控和报表是基于用户业务维度的，包括整个集群的磁盘水位、Topic以及Consumer Group在内的所有的用户所关心的业务相关指标，这些内容都会沉淀在消息队列Kafka的控制台里面，用户直接登录控制台就能够看到整体业务的运行情况。最后一点，运行在消息队列Kafka上的数据是非常安全的，通过VPC网络的隔离、鉴权、加密以及黑白名单这一系列的保障能够保证用户的数据是非常安全的，同时消息队列Kafka所具有的一个巨大优势就是其购买的每一个实例都是用户购买所独享的，用户之间不会因为相互影响导致整个系统出现不稳定的情况。
**场景**
以上为大家介绍了消息队列Kafka的优势，接下来为大家分享其所适用的场景。其实，可以认为消息队列Kafka与开源的Apache Kafka所适用的场景是一样的，不同之处在于消息队列Kafka具有更高的可靠性以及可用性，同时不需要用户自己进行运维。
**构建日志分析平台**
淘宝、天猫平台等公司每天都会产生大量的日志。运营、运维团队以及一些决策人员需要对于整个的日志数据进行分析与统计。而Kafka本身的性能是非常高效的，同时Kafka的特性决定它非常适合作为"日志收集中心"，这是因为Kafka在采集日志的时候业务是无感知的，其能够兼容自己的上游，能够直接地通过配置加密消息。当日志数据发送到Kafka集群里面，其实对于业务而言是完全无侵入的。同时其在下游又能够直接地对接Hadoop/ODPS等离线仓库存储和Strom/Spark等实现实时在线分析。在这样的情况之下，使用Kafka，只需要用户去关注整个流程里面的业务逻辑，而无需做更多的开发就能够实现统计、分析以及报表。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUMBgnATnUmVHh6elwfAOcvmkqIibTWuMvSxH5ee2nIicfnfic2GnGKFr8Gmq5nxsRibsMEqMqsCk13Fg/640?wx_fmt=png)
**网站活动跟踪场景**
除了实现数据分析形成报表之外，Kafka还可以实现网站活动跟踪场景。通过Kafka可以实时地收集到网站的活动数据，比如用户对于页面的浏览、搜索以及行为等。消息队列Kafka可以通过Topic来对于业务上面不同的数据模型进行切分的。那么，用户可以按照注册或者登录以及购买等进行切分，对于下游所需要跟踪的场景的不同，可以对接不同的处理系统，比如实时处理、实时监控以及离线处理，Kafka在这个场景里面是非常便捷易用的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUMBgnATnUmVHh6elwfAOcvYR3977u6iabG8ibfzQQ91SazLOkcq7yxTtRVyU4ich9aRTcYHj4t9kyYw/640?wx_fmt=png)
**数据在流动中产生价值**
前面两个例子是将消息队列Kafka在整个解决方案里面承担的是数据输入流的角色，而Kafka却不仅仅可以充当数据的输入流，还可以做流计算处理，比如股市走向分析、气象数据测控、网站用户行为分析等领域，由于在这些领域中数据产生快、实时性强、数据量大，所以很难统一采集并入库存储后再做处理，这便导致传统的数据处理架构不能满足需求。而Kafka Stream以及Storm/Samza/Spark等流计算引擎的出现，可以根据业务需求对数据进行计算分析，最终把结果保存或者分发给需要的组件。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUMBgnATnUmVHh6elwfAOcvmqqRu9ygUN58c1bXyo7ccT0zTkEzpeibfHY9icNick32rbGaMiccn0CiahQ/640?wx_fmt=png)
**多路转发**
大家经常会遇到的场景就是对于不同的业务维度而言，需要不同的计算方式，比如对于对账系统而言，可能需要实时的流处理方式；对于统计分析而言，可以使用批计算方式。而使用Kafka能够实现多路转发，上游生产一份数据，多个下游节点都能够获取这份数据并做出相应的处理，因此Kafka可以完成数据多路转发的功能。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUMBgnATnUmVHh6elwfAOcvRUvSJtSPD6QH5Ub1ibAsstPIk7icNwpicVaHibXWol0gqQibAiaslCTR0z8g/640?wx_fmt=png)
**推荐阅读:**
[关于Spark Streaming感知kafka动态分区的问题](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485078&idx=1&sn=5be1f2db95f2853651d8fe06cb78d637&chksm=9f38e5bea84f6ca8c9db462cbc1d85e75f0f2b72ec976e0af68cf1435a102261d8849bf96b5c&scene=21#wechat_redirect)
[kafka源码系列之mysql数据增量同步到kafka](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484962&idx=1&sn=e92a2521395d862c1e16efc266837152&chksm=9f38e50aa84f6c1cd72d24a86938a3b320a5e41ab8bdb6939de7381192c023b066da9c0ce305&scene=21#wechat_redirect)
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/z9433rAGTDdpO0B0FqCuTT6IVUeJeupR8DrCPat3Iqu9rfDbpxOwwsZrzLecngibudlf85XCJnDHVMtk9Xw9pzw/640?wx_fmt=gif)戳下面的**原文阅读，更有料**！
