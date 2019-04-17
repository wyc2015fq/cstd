# 探秘Hadoop生态13：初探Storm和入门实例 - 程序员江湖 - CSDN博客





2017年03月08日 20:43:26[黄小斜](https://me.csdn.net/a724888)阅读数：1282
所属专栏：[后端技术杂谈](https://blog.csdn.net/column/details/25481.html)









# **这位大侠，这是我的公众号：程序员江湖。****分享程序员面试与技术的那些事。 干货满满，关注就送。**![这里写图片描述](https://img-blog.csdn.net/20180827211353568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E3MjQ4ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# [Storm：最火的流式处理框架](https://www.cnblogs.com/langtianya/p/5199529.html)

伴随着信息科技日新月异的发展，信息呈现出爆发式的膨胀，人们获取信息的途径也更加多样、更加便捷，同时对于信息的时效性要求也越来越高。举个搜索场景中的例子，当一个卖家发布了一条宝贝信息时，他希望的当然是这个宝贝马上就可以被卖家搜索出来、点击、购买啦，相反，如果这个宝贝要等到第二天或者更久才可以被搜出来，估计这个大哥就要骂娘了。再举一个推荐的例子，如果用户昨天在淘宝上买了一双袜子，今天想买一副泳镜去游泳，但是却发现系统在不遗余力地给他推荐袜子、鞋子，根本对他今天寻找泳镜的行为视而不见，估计这哥们心里就会想推荐你妹呀。其实稍微了解点背景知识的码农们都知道，这是因为后台系统做的是每天一次的全量处理，而且大多是在夜深人静之时做的，那么你今天白天做的事情当然要明天才能反映出来啦。
- 实现一个实时计算系统

全量数据处理使用的大多是鼎鼎大名的hadoop或者hive，作为一个批处理系统，hadoop以其吞吐量大、自动容错等优点，在海量数据处理上得到了广泛的使用。但是，hadoop不擅长实时计算，因为它天然就是为批处理而生的，这也是业界一致的共识。否则最近这两年也不会有s4,storm,puma这些实时计算系统如雨后春笋般冒出来啦。先抛开s4,storm,puma这些系统不谈，我们首先来看一下，如果让我们自己设计一个实时计算系统，我们要解决哪些问题。
- 低延迟。都说了是实时计算系统了，延迟是一定要低的。
- 高性能。性能不高就是浪费机器，浪费机器是要受批评的哦。
- 分布式。系统都是为应用场景而生的，如果你的应用场景、你的数据和计算单机就能搞定，那么不用考虑这些复杂的问题了。我们所说的是单机搞不定的情况。
- 可扩展。伴随着业务的发展，我们的数据量、计算量可能会越来越大，所以希望这个系统是可扩展的。
- 容错。这是分布式系统中通用问题。一个节点挂了不能影响我的应用。

好，如果仅仅需要解决这5个问题，可能会有无数种方案，而且各有千秋，随便举一种方案，使用消息队列+分布在各个机器上的工作进程就ok啦。我们再继续往下看。
- 容易在上面开发应用程序。亲，你设计的系统需要应用程序开发人员考虑各个处理组件的分布、消息的传递吗？如果是，那有点麻烦啊，开发人员可能会用不好，也不会想去用。
- 消息不丢失。用户发布的一个宝贝消息不能在实时处理的时候给丢了，对吧？更严格一点，如果是一个精确数据统计的应用，那么它处理的消息要不多不少才行。这个要求有点高哦。
	
# 诞 生

 在2011年Storm开源之前，由于Hadoop的火红，整个业界都在喋喋不休地谈论大数据。Hadoop的高吞吐，海量数据处理的能力使得人们可以方便地处理海量数据。但是，Hadoop的缺点也和它的优点同样鲜明——延迟大，响应缓慢，运维复杂。

有需求也就有创造，在Hadoop基本奠定了大数据霸主地位的时候，很多的开源项目都是以弥补Hadoop的实时性为目标而被创造出来。而在这个节骨眼上Storm横空出世了。

Storm带着流式计算的标签华丽丽滴出场了，看看它的一些卖点：
- 分布式系统：可横向拓展,现在的项目不带个分布式特性都不好意思开源。
- 运维简单：Storm的部署的确简单。虽然没有Mongodb的解压即用那么简单，但是它也就是多安装两个依赖库而已。
- 高度容错：模块都是无状态的，随时宕机重启。
- 无数据丢失：Storm创新性提出的ack消息追踪框架和复杂的事务性处理,能够满足很多级别的数据处理需求。不过，越高的数据处理需求，性能下降越严重。
- 多语言：实际上，Storm的多语言更像是临时添加上去似的。因为，你的提交部分还是要使用Java实现。

下面，我们简单地认识一下Storm这个产品。

# 认 识

    Storm是一个免费开源、分布式、高容错的实时计算系统。Storm令持续不断的流计算变得容易，弥补了Hadoop批处理所不能满足的实时要求。Storm经常用于在实时分析、在线机器学习、持续计算、分布式远程调用和ETL等领域。Storm的部署管理非常简单，而且，在同类的流式计算工具，Storm的性能也是非常出众的。

    Storm主要分为两种组件Nimbus和Supervisor。这两种组件都是快速失败的，没有状态。任务状态和心跳信息等都保存在Zookeeper上的，提交的代码资源都在本地机器的硬盘上。
- Nimbus负责在集群里面发送代码，分配工作给机器，并且监控状态。全局只有一个。
- Supervisor会监听分配给它那台机器的工作，根据需要启动/关闭工作进程Worker。每一个要运行Storm的机器上都要部署一个，并且，按照机器的配置设定上面分配的槽位数。
- Zookeeper是Storm重点依赖的外部资源。Nimbus和Supervisor甚至实际运行的Worker都是把心跳保存在Zookeeper上的。Nimbus也是根据Zookeerper上的心跳和任务运行状况，进行调度和任务分配的。
- Storm提交运行的程序称为Topology。
- Topology处理的最小的消息单位是一个Tuple，也就是一个任意对象的数组。
- Topology由Spout和Bolt构成。Spout是发出Tuple的结点。Bolt可以随意订阅某个Spout或者Bolt发出的Tuple。Spout和Bolt都统称为component。

下图是一个Topology设计的逻辑图的例子。

![topology01](http://jbcdn2.b0.upaiyun.com/2013/09/topology01.jpg)

下图是Topology的提交流程图。

![topology02](http://jbcdn2.b0.upaiyun.com/2013/09/topology021.jpg)

下图是Storm的数据交互图。可以看出两个模块Nimbus和Supervisor之间没有直接交互。状态都是保存在Zookeeper上。Worker之间通过ZeroMQ传送数据。

![topology03](http://jbcdn2.b0.upaiyun.com/2013/09/topology03.png)

虽然，有些地方做得还是不太好，例如，底层使用的ZeroMQ不能控制内存使用(下个release版本，引入了新的消息机制使用netty代替ZeroMQ），多语言支持更多是噱头，Nimbus还不支持HA。但是，就像当年的Hadoop那样，很多公司选择它是因为它是唯一的选择。而这些先期使用者，反过来促进了Storm的发展。

# 发 展

Storm已经发展到0.8.2版本了，看一下两年多来，它取得的成就：
- 有50个大大小小的公司在使用Storm，相信更多的不留名的公司也在使用。这些公司中不乏淘宝，百度，Twitter，Groupon，雅虎等重量级公司。
- 从开源时候的0.5.0版本，到现在的0.8.0+，和即将到来的0.9.0+。先后添加了以下重大的新特性：
		- 使用kryo作为Tuple序列化的框架（0.6.0）
- 添加了Transactional topologies（事务性拓扑）的支持（0.7.0）
- 添加了Trident的支持（0.8.0）
- 引入netty作为底层消息机制（0.9.0）


> 
Transactional topologies和Trident都是针对实际应用中遇到的重复计数问题和应用性问题的解决方案。可以看出，实际的商用给予了Storm很多良好的反馈。

- 在GitHub上超过4000个项目负责人。Storm集成了许多库，支持包括Kestrel、Kafka、JMS、Cassandra、Memcached以及更多系统。随着支持的库越来越多，Storm更容易与现有的系统协作。Storm的拥有一个活跃的社区和一群热心的贡献者。过去两年，Storm的发展是成功的。

# 当 前

Storm被广泛应用于实时分析，在线机器学习，持续计算、分布式远程调用等领域。来看一些实际的应用:
- 一淘-[实时分析系统pora](http://www.searchtb.com/2012/11/pora.html)：实时分析用户的属性，并反馈给搜索引擎。最初，用户属性分析是通过每天在云梯上定时运行的MR job来完成的。为了满足实时性的要求，希望能够实时分析用户的行为日志，将最新的用户属性反馈给搜索引擎，能够为用户展现最贴近其当前需求的结果。
- 携程-[网站性能监控](http://www.programmer.com.cn/14601/)：实时分析系统监控携程网的网站性能。利用HTML5提供的performance标准获得可用的指标，并记录日志。Storm集群实时分析日志和入库。使用DRPC聚合成报表，通过历史数据对比等判断规则，触发预警事件。
		
> 
如果，业务场景中需要低延迟的响应，希望在秒级或者毫秒级完成分析、并得到响应，而且希望能够随着数据量的增大而拓展。那就可以考虑下，使用Storm了。


- 试想下，如果，一个游戏新版本上线，有一个实时分析系统，收集游戏中的数据，运营或者开发者可以在上线后几秒钟得到持续不断更新的游戏监控报告和分析结果，然后马上针对游戏的参数和平衡性进行调整。这样就能够大大缩短游戏迭代周期，加强游戏的生命力（实际上，zynga就是这么干的！虽然使用的不是Storm……[Zynga研发之道探秘：用数据说话](http://www.csdn.net/article/2011-08-26/303631)）。
- 除了低延迟，Storm的Topology灵活的编程方式和分布式协调也会给我们带来方便。用户属性分析的项目，需要处理大量的数据。使用传统的MapReduce处理是个不错的选择。但是，处理过程中有个步骤需要根据分析结果，采集网页上的数据进行下一步的处理。这对于MapReduce来说就不太适用了。但是，Storm的Topology就能完美解决这个问题。基于这个问题，我们可以画出这样一个Storm的Topology的处理图。

![topology04](http://jbcdn2.b0.upaiyun.com/2013/09/topology04.jpg)



我们只需要实现每个分析的过程，而Storm帮我们把消息的传送和接受都完成了。更加激动人心的是，你只需要增加某个Bolt的并行度就能够解决掉某个结点上的性能瓶颈。

# 未 来

在流式处理领域里，Storm的直接对手是S4。不过，S4冷淡的社区、半成品的代码，在实际商用方面输给Storm不止一条街。

如果把范围扩大到实时处理，Storm就一点都不寂寞了。
- Puma：Facebook使用puma和Hbase相结合来处理实时数据,使批处理 计算平台具备一定实时能力。 不过这不算是一个开源的产品。只是内部使用。
- HStreaming：尝试为Hadoop环境添加一个实时的组件HStreaming能让一个Hadoop平台在几天内转为一个实时系统。分商业版和免费版。也许HStreaming可以借Hadoop的东风，撼动Storm。
- Spark Streaming：作为UC Berkeley云计算software stack的一部分，Spark Streaming是建立在Spark上的应用框架，利用Spark的底层框架作为其执行基础，并在其上构建了DStream的行为抽象。利用DStream所提供的api，用户可以在数据流上实时进行count，join，aggregate等操作。

当然，Storm也有Yarn-Storm项目，能让Storm运行在Hadoop2.0的Yarn框架上，可以让Hadoop的MapReduce和Storm共享资源。

# 总 结

知乎上有一个挺好的问答： 问：实时处理系统（类似s4, storm）对比直接用MQ来做好处在哪里？  答：好处是它帮你做了： 1) 集群控制。2) 任务分配。3) 任务分发 4) 监控 等等。

需要知道Storm不是一个完整的解决方案。使用Storm你需要加入消息队列做数据入口，考虑如何在流中保存状态，考虑怎样将大问题用分布式去解决。解决这些问题的成本可能比增加一个服务器的成本还高。但是，一旦下定决定使用了Storm并解决了那些恼人的细节，你就能享受到Storm给你带来的简单，可拓展等优势了。

技术的发展日新月异，数据处理领域越来越多优秀的开源产品。Storm的过去是成功的，将来会如何发展，我们拭目以待吧。

### 后记

本文的重点是描述Storm的应用场景和未来的发展前景，让大家对Storm有一个初步的印象。如果，要落地使用的朋友，在网上可以找到很多优秀的Storm的技术文章。例如：Storm的核心贡献者[徐明明](http://xumingming.sinaapp.com/)的博客和淘宝关于storm的文章。


Storm和Spark Streaing的区别：

### Storm和Spark Streaming都是分布式流处理的开源框架，但是它们之间还是有一些区别的，这里将进行比较并指出它们的重要的区别。

1.     处理模型以及延迟

虽然这两个框架都提供可扩展性(Scalability)和可容错性(Fault Tolerance),但是它们的处理模型从根本上说是不一样的。Storm处理的是每次传入的一个事件，而Spark Streaming是处理某个时间段窗口内的事件流。因此，Storm处理一个事件可以达到亚秒级的延迟，而Spark Streaming则有秒级的延迟。

2.     容错和数据保证

在容错数据保证方面的权衡方面，Spark Streaming提供了更好的支持容错状态计算。在Storm中，当每条单独的记录通过系统时必须被跟踪，所以Storm能够至少保证每条记录将被处理一次，但是在从错误中恢复过来时候允许出现重复记录，这意味着可变状态可能不正确地被更新两次。而Spark Streaming只需要在批处理级别对记录进行跟踪处理，因此可以有效地保证每条记录将完全被处理一次，即便一个节点发生故障。虽然Storm的 Trident library库也提供了完全一次处理的功能。但是它依赖于事务更新状态，而这个过程是很慢的，并且通常必须由用户实现。

简而言之,如果你需要亚秒级的延迟，Storm是一个不错的选择，而且没有数据丢失。如果你需要有状态的计算，而且要完全保证每个事件只被处理一次，Spark Streaming则更好。Spark Streaming编程逻辑也可能更容易，因为它类似于批处理程序，特别是在你使用批次(尽管是很小的)时。

3.     实现和编程API

Storm主要是由Clojure语言实现，Spark Streaming是由Scala实现。如果你想看看这两个框架是如何实现的或者你想自定义一些东西你就得记住这一点。Storm是由BackType和 Twitter开发，而Spark Streaming是在UC Berkeley开发的。

Storm提供了Java API，同时也支持其他语言的API。 Spark Streaming支持Scala和Java语言(其实也支持Python)。另外Spark Streaming的一个很棒的特性就是它是在Spark框架上运行的。这样你就可以想使用其他批处理代码一样来写Spark Streaming程序，或者是在Spark中交互查询。这就减少了单独编写流批量处理程序和历史数据处理程序。

4.     生产支持

Storm已经出现好多年了，而且自从2011年开始就在Twitter内部生产环境中使用，还有其他一些公司。而Spark Streaming是一个新的项目，并且在2013年仅仅被Sharethrough使用(据作者了解)。

Storm是 Hortonworks Hadoop数据平台中流处理的解决方案，而Spark Streaming出现在 MapR的分布式平台和Cloudera的企业数据平台中。除此之外，Databricks是为Spark提供技术支持的公司，包括了Spark Streaming。

5.     集群管理集成

尽管两个系统都运行在它们自己的集群上，Storm也能运行在Mesos，而Spark Streaming能运行在YARN 和Mesos上。



# Storm 基础知识

由 小路依依 创建， 最后一次修改 2016-08-12

## 基础知识

Storm 是一个分布式的，可靠的，容错的数据流处理系统。它会把工作任务委托给不同类型的组件，每个组件负责处理一项简单特定的任务。Storm 集群的输入流由一个被称作 spout 的组件管理，spout 把数据传递给 bolt， bolt 要么把数据保存到某种存储器，要么把数据传递给其它的 bolt。你可以想象一下，一个 Storm 集群就是在一连串的 bolt 之间转换 spout 传过来的数据。

这里用一个简单的例子来说明这个概念。昨晚我在新闻节目里看到主持人在谈论政治人物和他们对于各种政治话题的立场。他们一直重复着不同的名字，而我开始考虑这些名字是否被提到了相同的次数，以及不同次数之间的偏差。

想像播音员读的字幕作为你的数据输入流。你可以用一个 spout 读取一个文件（或者 socket，通过 HTTP，或者别的方法）。文本行被 spout 传给一个 bolt，再被 bolt 按单词切割。单词流又被传给另一个 bolt，在这里每个单词与一张政治人名列表比较。每遇到一个匹配的名字，第二个 bolt 为这个名字在数据库的计数加1。你可以随时查询数据库查看结果， 而且这些计数是随着数据到达实时更新的。所有组件（spouts和bolts）及它们之间的关系请参考拓扑图1-1

![](https://7n.w3cschool.cn/attachments/image/wk/storm/01.png)

现在想象一下，很容易在整个 Storm 集群定义每个 bolt 和 spout 的并行性级别，因此你可以无限的扩展你的拓扑结构。很神奇，是吗？尽管这是个简单例子，你也可以看到 Storm 的强大。

有哪些典型的 Storm 应用案例？

数据处理流

正如上例所展示的，不像其它的流处理系统，Storm 不需要中间队列。

连续计算

连续发送数据到客户端，使它们能够实时更新并显示结果，如网站指标。

分布式远程过程调用

频繁的 CPU 密集型操作并行化。

Storm 组件

对于一个Storm集群，一个连续运行的主节点组织若干节点工作。

在 Storm 集群中，有两类节点：主节点 master node 和工作节点 worker nodes。主节点运行着一个叫做 Nimbus 的守护进程。这个守护进程负责在集群中分发代码，为工作节点分配任务，并监控故障。Supervisor守护进程作为拓扑的一部分运行在工作节点上。一个 Storm 拓扑结构在不同的机器上运行着众多的工作节点。

因为 Storm 在 Zookeeper 或本地磁盘上维持所有的集群状态，守护进程可以是无状态的而且失效或重启时不会影响整个系统的健康（见图1-2）

![](https://7n.w3cschool.cn/attachments/image/wk/storm/02.png)

在系统底层，Storm 使用了 zeromq(0mq, zeromq([http://www.zeromq.org](http://www.zeromq.org/)))。这是一种先进的，可嵌入的网络通讯库，它提供的绝妙功能使 Storm 成为可能。下面列出一些 zeromq 的特性。
- 一个并发架构的 Socket 库
- 对于集群产品和超级计算，比 TCP 要快
- 可通过 inproc（进程内）, IPC（进程间）, TCP 和 multicast(多播协议)通信
- 异步 I / O 的可扩展的多核消息传递应用程序
- 利用扇出(fanout), 发布订阅（PUB-SUB）,管道（pipeline）, 请求应答（REQ-REP），等方式实现 N-N 连接

NOTE: Storm 只用了 push/pull sockets

## Storm 的特性

在所有这些设计思想与决策中，有一些非常棒的特性成就了独一无二的 Storm。
- 简化编程：如果你曾试着从零开始实现实时处理，你应该明白这是一件多么痛苦的事情。使用 Storm，复杂性被大大降低了。
- 使用一门基于 JVM 的语言开发会更容易，但是你可以借助一个小的中间件，在 Storm 上使用任何语言开发。有现成的中间件可供选择，当然也可以自己开发中间件。
- 容错：Storm 集群会关注工作节点状态，如果宕机了必要的时候会重新分配任务。
- 可扩展：所有你需要为扩展集群所做的工作就是增加机器。Storm 会在新机器就绪时向它们分配任务。
- 可靠的：所有消息都可保证至少处理一次。如果出错了，消息可能处理不只一次，不过你永远不会丢失消息。
- 快速：速度是驱动 Storm 设计的一个关键因素
- 事务性：You can get exactly once messaging semantics for pretty much any computation. 你可以为几乎任何计算得到恰好一次消息语义。



# Storm 起步

由 小路依依 创建， 最后一次修改 2016-08-12

## 准备开始

## 准备开始

在本章，我们要创建一个 Storm 工程和我们的第一个 Storm 拓扑结构。

NOTE: 下面假设你的 JRE 版本在 1.6 以上。我们推荐 Oracle 提供的 JRE。你可以到 [http://www.java.com/downloads/](http://www.java.com/downloads/) 下载。

## 操作模式

开始之前，有必要了解一下 Storm 的操作模式。有下面两种方式。

### 本地模式

在本地模式下，Storm 拓扑结构运行在本地计算机的单一 JVM 进程上。这个模式用于开发、测试以及调试，因为这是观察所有组件如何协同工作的最简单方法。在这种模式下，我们可以调整参数，观察我们的拓扑结构如何在不同的 Storm 配置环境下运行。要在本地模式下运行，我们要下载 Storm 开发依赖，以便用来开发并测试我们的拓扑结构。我们创建了第一个 Storm 工程以后，很快就会明白如何使用本地模式了。

NOTE: 在本地模式下，跟在集群环境运行很像。不过很有必要确认一下所有组件都是线程安全的，因为当把它们部署到远程模式时它们可能会运行在不同的 JVM 进程甚至不同的物理机上，这个时候它们之间没有直接的通讯或共享内存。

我们要在本地模式运行本章的所有例子。

### 远程模式

在远程模式下，我们向 Storm 集群提交拓扑，它通常由许多运行在不同机器上的流程组成。远程模式不会出现调试信息， 因此它也称作生产模式。不过在单一开发机上建立一个 Storm 集群是一个好主意，可以在部署到生产环境之前，用来确认拓扑在集群环境下没有任何问题。

你将在第六章学到更多关于远程模式的内容，并在附录B学到如何安装一个 Storm 集群。

## Hello World

我们在这个工程里创建一个简单的拓扑，数单词数量。我们可以把这个看作 Storm 的 “Hello World”。不过，这是一个非常强大的拓扑，因为它能够扩展到几乎无限大的规模，而且只需要做一些小修改，就能用它构建一个统计系统。举个例子，我们可以修改一下工程用来找出 Twitter 上的热点话题。

要创建这个拓扑，我们要用一个 spout 读取文本，第一个 bolt 用来标准化单词，第二个 bolt 为单词计数，如图2-1所示。

![](https://7n.w3cschool.cn/attachments/image/wk/storm/03.png)

你可以从这个网址下载源码压缩包， [https://github.com/storm-book/examples-ch02-getting_started/zipball/master](https://github.com/storm-book/examples-ch02-getting_started/zipball/master)。

NOTE: 如果你使用 [git](http://git-scm.com/)（一个分布式版本控制与源码管理工具），你可以执行 `git clone [git@github.com](git@github.com):storm-book/examples-ch02-getting_started.git`，把源码检出到你指定的目录。

## Java 安装检查

构建 Storm 运行环境的第一步是检查你安装的 Java 版本。打开一个控制台窗口并执行命令：java -version。控制台应该会显示出类似如下的内容：

```
**java** **-version**

    **java** **version** "1.6.0_26"
    **Java**(**TM**) **SE** **Runtime** **Enviroment** (**build** 1.6.0_26-b03)

    **Java** **HotSpot**(**TM**) **Server** **VM** (**build** 20.1-b02, **mixed** **mode**)
```

如果不是上述内容，检查你的 Java 安装情况。（参考 [http://www.java.com/download/](http://www.java.com/download/)）

## 创建工程

开始之前，先为这个应用建一个目录（就像你平常为 Java 应用做的那样）。这个目录用来存放工程源码。

接下来我们要下载 Storm 依赖包，这是一些 jar 包，我们要把它们添加到应用类路径中。你可以采用如下两种方式之一完成这一步：
- 下载所有依赖，解压缩它们，把它 们添加到类路径
- 使用 Apache Maven

NOTE: Maven 是一个软件项目管理的综合工具。它可以用来管理项目的开发周期的许多方面，从包依赖到版本发布过程。在这本书中，我们将广泛使用它。如果要检查是否已经安装了maven，在命令行运行 mvn。如果没有安装你可以从 [http://maven.apache.org/download.html](http://maven.apache.org/download.html)下载。

没有必要先成为一个 Maven 专家才能使用 Storm，不过了解一下关于 Maven 工作方式的基础知识仍然会对你有所帮助。你可以在 Apache Maven 的网站上找到更多的信息（[http://maven.apache.org/](http://maven.apache.org/)）。

NOTE: Storm 的 Maven 依赖引用了运行 Storm 本地模式的所有库。

要运行我们的拓扑，我们可以编写一个包含基本组件的 pom.xml 文件。

```
<**project** xmlns="http://maven.apache.org/POM/4.0.0"
             xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
             xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
             http://maven.apache.org/xsd/maven-4.0.0.xsd">
             <**modelVersion**>4.0.0</**modelVersion**>
             <**groupId**>storm.book</**groupId**>
             <**artifactId**>Getting-Started</**artifactId**>
             <**version**>0.0.1-SNAPSHOT</**version**>
             <**build**>
                 <**plugins**>
                     <**plugin**>
                         <**groupId**>org.apache.maven.plugins</**groupId**>
                         <**artifactId**>maven-compiler-plugin</**artifactId**>
                         <**version**>2.3.2</**version**>
                         <**configuration**>
                             <**source**>1.6</**source**>
                             <**target**>1.6</**target**>
                             <**compilerVersion**>1.6</**compilerVersion**>
                         </**configuration**>
                     </**plugin**>
                 </**plugins**>
             </**build**>
             <**repositories**>
                 <!-- Repository where we can found the storm dependencies -->
                 <**repository**>
                     <**id**>clojars.org</**id**>
                     <**url**>http://clojars.org/repo</**url**>
                 </**repository**>
             </**repositories**>
             <**dependencies**>
                 <!-- Storm Dependency -->
                 <**dependency**>
                     <**groupId**>storm</**groupId**>
                     <**artifactId**>storm</**artifactId**>
                     <**version**>0.6.0</**version**>
                 </**dependency**>
             </**dependencies**>
    </**project**>
```

开头几行指定了工程名称和版本号。然后我们添加了一个编译器插件，告知 Maven 我们的代码要用 Java1.6 编译。接下来我们定义了 Maven 仓库（Maven 支持为同一个工程指定多个仓库）。clojars 是存放 Storm 依赖的仓库。Maven 会为运行本地模式自动下载必要的所有子包依赖。

一个典型的 Maven Java 工程会拥有如下结构：

```
我们的应用目录/
         ├── pom.xml
         └── src
               └── main
                  └── java
               |  ├── spouts
               |  └── bolts
               └── resources
```

java 目录下的子目录包含我们的代码，我们把要统计单词数的文件保存在 resource 目录下。

NOTE：命令 mkdir -p 会创建所有需要的父目录。

## 创建我们的第一个 Topology

我们将为运行单词计数创建所有必要的类。可能这个例子中的某些部分，现在无法讲的很清楚，不过我们会在随后的章节做进一步的讲解。

## Spout

pout WordReader 类实现了 IRichSpout 接口。我们将在第四章看到更多细节。WordReader负责从文件按行读取文本，并把文本行提供给第一个 bolt。

NOTE: 一个 spout 发布一个定义域列表。这个架构允许你使用不同的 bolts 从同一个spout 流读取数据，它们的输出也可作为其它 bolts 的定义域，以此类推。

例2-1包含 WordRead 类的完整代码（我们将会分析下述代码的每一部分）。

```
/**
         *  例2-1.src/main/java/spouts/WordReader.java
         */
        **package** spouts;

        **import** java.io.BufferedReader;
        **import** java.io.FileNotFoundException;
        **import** java.io.FileReader;
        **import** java.util.Map;
        **import** backtype.storm.spout.SpoutOutputCollector;
        **import** backtype.storm.task.TopologyContext;
        **import** backtype.storm.topology.IRichSpout;
        **import** backtype.storm.topology.OutputFieldsDeclarer;
        **import** backtype.storm.tuple.Fields;
        **import** backtype.storm.tuple.Values;

        **public** **class** **WordReader** **implements** **IRichSpout** {
            **private** SpoutOutputCollector collector;
            **private** FileReader fileReader;
            **private** **boolean** completed = **false**;
            **private** TopologyContext context;
            **public** **boolean** **isDistributed**() {**return** **false**;}
            **public** **void** **ack**(Object msgId) {
                    System.out.println("OK:"+msgId);
            }
            **public** **void** **close**() {}
            **public** **void** **fail**(Object msgId) {
                 System.out.println("FAIL:"+msgId);
            }
            /**
             * 这个方法做的惟一一件事情就是分发文件中的文本行
             */
            **public** **void** **nextTuple**() {
            /**
             * 这个方法会不断的被调用，直到整个文件都读完了，我们将等待并返回。
             */
                 **if**(completed){
                     **try** {
                         Thread.sleep(1000);
                     } **catch** (InterruptedException e) {
                         //什么也不做
                     }
                    **return**;
                 }
                 String str;
                 //创建reader
                 BufferedReader reader = **new** BufferedReader(fileReader);
                 **try**{
                     //读所有文本行
                    **while**((str = reader.readLine()) != **null**){
                     /**
                      * 按行发布一个新值
                      */
                         **this**.collector.emit(**new** Values(str),str);
                     }
                 }**catch**(Exception e){
                     **throw** **new** RuntimeException("Error reading tuple",e);
                 }**finally**{
                     completed = **true**;
                 }
             }
             /**
              * 我们将创建一个文件并维持一个collector对象
              */
             **public** **void** **open**(Map conf, TopologyContext context, SpoutOutputCollector collector) {
                     **try** {
                         **this**.context = context;
                         **this**.fileReader = **new** FileReader(conf.get("wordsFile").toString());
                     } **catch** (FileNotFoundException e) {
                         **throw** **new** RuntimeException("Error reading file ["+conf.get("wordFile")+"]");
                     }
                     **this**.collector = collector;
             }
             /**
              * 声明输入域"word"
              */
             **public** **void** **declareOutputFields**(OutputFieldsDeclarer declarer) {
                 declarer.declare(**new** Fields("line"));
             }
        }
```

第一个被调用的 spout 方法都是 public void open(Map conf, TopologyContext context, SpoutOutputCollector collector)。它接收如下参数：配置对象，在定义topology 对象是创建；TopologyContext 对象，包含所有拓扑数据；还有SpoutOutputCollector 对象，它能让我们发布交给 bolts 处理的数据。下面的代码主是这个方法的实现。

```
**public** **void** **open**(Map conf, TopologyContext context,
        SpoutOutputCollector collector) {
        **try** {
            **this**.context = context;
            **this**.fileReader = **new** FileReader(conf.**get**("wordsFile").toString());
        } **catch** (FileNotFoundException e) {
            **throw** **new** RuntimeException("Error reading file ["+conf.**get**("wordFile")+"]");
        }
        **this**.collector = collector;
    }
```

我们在这个方法里创建了一个 FileReader 对象，用来读取文件。接下来我们要实现 public void nextTuple()，我们要通过它向 bolts 发布待处理的数据。在这个例子里，这个方法要读取文件并逐行发布数据。

```
**public** **void** **nextTuple**() {
        **if**(completed){
            **try** {
                Thread.sleep(1);
            } **catch** (InterruptedException e) {
                //什么也不做
            }
            **return**;
        }
        String str;
        BufferedReader reader = **new** BufferedReader(fileReader);
        **try**{
            **while**((str = reader.readLine()) != **null**){
                **this**.collector.emit(**new** Values(str));
            }
        }**catch**(Exception e){
            **throw** **new** RuntimeException("Error reading tuple",e);
        }**finally**{
            completed = **true**;
        }
    }
```

NOTE: Values 是一个 ArrarList 实现，它的元素就是传入构造器的参数。

nextTuple() 会在同一个循环内被 ack() 和 fail() 周期性的调用。没有任务时它必须释放对线程的控制，其它方法才有机会得以执行。因此 nextTuple 的第一行就要检查是否已处理完成。如果完成了，为了降低处理器负载，会在返回前休眠一毫秒。如果任务完成了，文件中的每一行都已被读出并分发了。

NOTE:元组(tuple)是一个具名值列表，它可以是任意 java 对象（只要它是可序列化的）。默认情况，Storm 会序列化字符串、字节数组、ArrayList、HashMap 和 HashSet 等类型。

## Bolts

现在我们有了一个 spout，用来按行读取文件并每行发布一个元组，还要创建两个 bolts，用来处理它们（看图2-1）。bolts 实现了接口 backtype.storm.topology.IRichBolt。

bolt最重要的方法是void execute(Tuple input)，每次接收到元组时都会被调用一次，还会再发布若干个元组。

NOTE: 只要必要，bolt 或 spout 会发布若干元组。当调用 nextTuple 或 execute 方法时，它们可能会发布0个、1个或许多个元组。你将在第五章学习更多这方面的内容。

第一个 bolt，WordNormalizer，负责得到并标准化每行文本。它把文本行切分成单词，大写转化成小写，去掉头尾空白符。

首先我们要声明 bolt 的出参：

```
**public** **void** **declareOutputFields**(OutputFieldsDeclarer declarer){
        declarer.declare(**new** Fields("word"));
    }
```

这里我们声明 bolt 将发布一个名为 “word” 的域。

下一步我们实现 public void execute(Tuple input)，处理传入的元组：

```
public **void** execute(Tuple input){
        String sentence=input.getString(0);
        String[] words=sentence.split(" ");
        **for**(String word : words){
            word=word.trim();
            **if**(!word.isEmpty()){
                word=word.toLowerCase();
                //发布这个单词
                collector.emit(**new** Values(word));
            }
        }
        //对元组做出应答
        collector.ack(input);
    }
```

第一行从元组读取值。值可以按位置或名称读取。接下来值被处理并用collector对象发布。最后，每次都调用collector 对象的 ack() 方法确认已成功处理了一个元组。

例2-2是这个类的完整代码。

```
//例2-2 src/main/java/bolts/WordNormalizer.java
    **package** bolts;
    **import** java.util.ArrayList;
    **import** java.util.List;
    **import** java.util.Map;
    **import** backtype.storm.task.OutputCollector;
    **import** backtype.storm.task.TopologyContext;
    **import** backtype.storm.topology.IRichBolt;
    **import** backtype.storm.topology.OutputFieldsDeclarer;
    **import** backtype.storm.tuple.Fields;
    **import** backtype.storm.tuple.Tuple;
    **import** backtype.storm.tuple.Values;
    **public** **class** **WordNormalizer** **implements** **IRichBolt**{
        **private** OutputCollector collector;
        **public** **void** **cleanup**(){}
        /**
          * *bolt*从单词文件接收到文本行，并标准化它。
          * 文本行会全部转化成小写，并切分它，从中得到所有单词。
         */
        **public** **void** **execute**(Tuple input){
            String sentence = input.getString(0);
            String[] words = sentence.split(" ");
            **for**(String word : words){
                word = word.trim();
                **if**(!word.isEmpty()){
                    word=word.toLowerCase();
                    //发布这个单词
                    List a = **new** ArrayList();
                    a.add(input);
                    collector.emit(a,**new** Values(word));
                }
            }
            //对元组做出应答
            collector.ack(input);
        }
        **public** **void** **prepare**(Map stormConf, TopologyContext context, OutputCollector collector) {
            **this**.collector=collector;
        }

        /**
          * 这个*bolt*只会发布“word”域
          */
        **public** **void** **declareOutputFields**(OutputFieldsDeclarer declarer) {
            declarer.declare(**new** Fields("word"));
        }
    }
```

NOTE:通过这个例子，我们了解了在一次 execute 调用中发布多个元组。如果这个方法在一次调用中接收到句子 “This is the Storm book”，它将会发布五个元组。

下一个bolt，WordCounter，负责为单词计数。这个拓扑结束时（cleanup() 方法被调用时），我们将显示每个单词的数量。

NOTE: 这个例子的 bolt 什么也没发布，它把数据保存在 map 里，但是在真实的场景中可以把数据保存到数据库。

```
**package** bolts;

**import** java.util.HashMap;
**import** java.util.Map;
**import** backtype.storm.task.OutputCollector;
**import** backtype.storm.task.TopologyContext;
**import** backtype.storm.topology.IRichBolt;
**import** backtype.storm.topology.OutputFieldsDeclarer;
**import** backtype.storm.tuple.Tuple;

**public** **class** **WordCounter** **implements** **IRichBolt**{
    Integer id;
    String name;
    Map<String,Integer> counters;
    **private** OutputCollector collector;

    /**
      * 这个spout结束时（集群关闭的时候），我们会显示单词数量
      */
    @Override
    **public** **void** **cleanup**(){
        System.out.println("-- 单词数 【"+name+"-"+id+"】 --");
        **for**(Map.Entry<String,Integer> entry : counters.entrySet()){
            System.out.println(entry.getKey()+": "+entry.getValue());
        }
    }

    /**
     *  为每个单词计数
     */
    @Override
    **public** **void** **execute**(Tuple input) {
        String str=input.getString(0);
        /**
         * 如果单词尚不存在于map，我们就创建一个，如果已在，我们就为它加1
         */
        **if**(!counters.containsKey(str)){
            conters.put(str,1);
        }**else**{
            Integer c = counters.get(str) + 1;
            counters.put(str,c);
        }
        //对元组作为应答
        collector.ack(input);
    }

    /**
     * 初始化
     */
    @Override
    **public** **void** **prepare**(Map stormConf, TopologyContext context, OutputCollector collector){
        **this**.counters = **new** HashMap<String, Integer>();
        **this**.collector = collector;
        **this**.name = context.getThisComponentId();
        **this**.id = context.getThisTaskId();
    }

    @Override
    **public** **void** **declareOutputFields**(OutputFieldsDeclarer declarer) {}
}
```

execute 方法使用一个 map 收集单词并计数。拓扑结束时，将调用 clearup() 方法打印计数器 map。（虽然这只是一个例子，但是通常情况下，当拓扑关闭时，你应当使用 cleanup() 方法关闭活动的连接和其它资源。）

## 主类

你可以在主类中创建拓扑和一个本地集群对象，以便于在本地测试和调试。LocalCluster 可以通过 Config 对象，让你尝试不同的集群配置。比如，当使用不同数量的工作进程测试你的拓扑时，如果不小心使用了某个全局变量或类变量，你就能够发现错误。（更多内容请见第三章）

NOTE：所有拓扑节点的各个进程必须能够独立运行，而不依赖共享数据（也就是没有全局变量或类变量），因为当拓扑运行在真实的集群环境时，这些进程可能会运行在不同的机器上。

接下来，TopologyBuilder 将用来创建拓扑，它决定 Storm 如何安排各节点，以及它们交换数据的方式。

```
TopologyBuilder builder = **new** TopologyBuilder();
    builder.setSpout("word-reader", **new** WordReader());
    builder.setBolt("word-normalizer", **new** WordNormalizer()).shuffleGrouping("word-reader");
    builder.setBolt("word-counter", **new** WordCounter()).shuffleGrouping("word-normalizer");
```

在 spout 和 bolts 之间通过 shuffleGrouping 方法连接。这种分组方式决定了 Storm 会以随机分配方式从源节点向目标节点发送消息。

下一步，创建一个包含拓扑配置的 Config 对象，它会在运行时与集群配置合并，并通过prepare 方法发送给所有节点。

```
Config conf = **new** Config();
    conf.put("wordsFile", args[0]);
    conf.setDebug(true);
```

由 spout 读取的文件的文件名，赋值给 wordFile 属性。由于是在开发阶段，设置 debug 属性为 true，Strom 会打印节点间交换的所有消息，以及其它有助于理解拓扑运行方式的调试数据。

正如之前讲过的，你要用一个 LocalCluster 对象运行这个拓扑。在生产环境中，拓扑会持续运行，不过对于这个例子而言，你只要运行它几秒钟就能看到结果。

```
LocalCluster cluster = **new** LocalCluster();
    cluster.submitTopology("Getting-Started-Topologie", conf, builder.createTopology());
    Thread.sleep(2000);
    cluster.shutdown();
```

调用 createTopology 和 submitTopology，运行拓扑，休眠两秒钟（拓扑在另外的线程运行），然后关闭集群。

例2-3是完整的代码

```
//例2-3 src/main/java/TopologyMain.java
    **import** spouts.WordReader;
    **import** backtype.storm.Config;
    **import** backtype.storm.LocalCluster;
    **import** backtype.storm.topology.TopologyBuilder;
    **import** backtype.storm.tuple.Fields;
    **import** bolts.WordCounter;
    **import** bolts.WordNormalizer;

    **public** **class** **TopologyMain** {
        **public** **static** **void** **main**(String[] args) **throws** InterruptedException {
        //定义拓扑
            TopologyBuilder builder = **new** TopologyBuilder());
            builder.setSpout("word-reader", **new** WordReader());
            builder.setBolt("word-normalizer", **new** WordNormalizer()).shuffleGrouping("word-reader");
            builder.setBolt("word-counter", **new** WordCounter(),2).fieldsGrouping("word-normalizer", **new** Fields("word"));

        //配置
            Config conf = **new** Config();
            conf.put("wordsFile", args[0]);
            conf.setDebug(**false**);

        //运行拓扑
             conf.put(Config.TOPOLOGY_MAX_SPOUT_PENDING, 1);
            LocalCluster cluster = **new** LocalCluster();
            cluster.submitTopology("Getting-Started-Topologie", conf, builder.createTopology();
            Thread.sleep(1000);
            cluster.shutdown();
        }
    }
```

观察运行情况

你已经为运行你的第一个拓扑准备好了。在这个目录下面创建一个文件，/src/main/resources/words.txt，一个单词一行，然后用下面的命令运行这个拓扑：mvn exec:java -Dexec.mainClass=”TopologyMain” -Dexec.args=”src/main/resources/words.txt。举个例子，如果你的 words.txt 文件有如下内容：Storm test are great is an Storm simple application but very powerful really Storm is great 你应该会在日志中看到类似下面的内容： is: 2 application: 1 but: 1 great: 1 test: 1 simple: 1 Storm: 3 really: 1 are: 1 great: 1 an: 1 powerful: 1 very: 1 在这个例子中，每类节点只有一个实例。但是如果你有一个非常大的日志文件呢？你能够很轻松的改变系统中的节点数量实现并行工作。这个时候，你就要创建两个 WordCounter** 实例。
`    builder.setBolt("word-counter", **new** WordCounter(),2).shuffleGrouping("word-normalizer");  `
程序返回时，你将看到： — 单词数 【word-counter-2】 — application: 1 is: 1 great: 1 are: 1 powerful: 1 Storm: 3 — 单词数 [word-counter-3] — really: 1 is: 1 but: 1 great: 1 test: 1 simple: 1 an: 1 very: 1 棒极了！修改并行度实在是太容易了（当然对于实际情况来说，每个实例都会运行在单独的机器上）。不过似乎有一个问题：单词 is 和 great 分别在每个 WordCounter 各计数一次。怎么会这样？当你调用shuffleGrouping 时，就决定了 Storm 会以随机分配的方式向你的 bolt 实例发送消息。在这个例子中，理想的做法是相同的单词问题发送给同一个 WordCounter 实例。你把shuffleGrouping(“word-normalizer”) 换成 fieldsGrouping(“word-normalizer”, new Fields(“word”)) 就能达到目的。试一试，重新运行程序，确认结果。 你将在后续章节学习更多分组方式和消息流类型。

## 结论

我们已经讨论了 Storm 的本地和远程操作模式之间的不同，以及 Storm 的强大和易于开发的特性。你也学习了一些 Storm 的基本概念，我们将在后续章节深入讲解它们。





