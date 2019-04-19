# 《ZeroC Ice 权威指南》笔记 - 建建的博客 - CSDN博客
2017年05月02日 17:46:42[纪建](https://me.csdn.net/u013898698)阅读数：752
Ice介绍
        Ice（Internet Communications Engine）是ZeroC公司的杰作，继承了CORBA的血统，是新一代的面向对象的分布式系统中间件。Ice是RPC通讯领域里最稳定、强大、高性能、跨平台、多语言支持的老牌开源中间件，特别适合于当前互联网领域中一个平台存在多种开发语言编程，以及网站和app应用并存的复杂大型项目。
        RPC（Remote Procedure Call Protocol 远程过程调用协议），是一种通过网络从远程计算机程序上请求服务，而不需要了解底层网络技术的协议。RPC假定某些传输协议的存在，如TCP或UDP，为通讯程序之间携带信息数据；在OSI网络通讯模型中，RPC跨越了传输层和应用层。
        Ice通过具体编程语言物管的中立语言Slice（Specification Language fro Ice）来描述服务的接口，从而达到对象接口与其实现想分离的目的。
        目前Ice平台支持客户端API的语言有C++、.NET、Java、Python、Object-C、Ruby、PHP、JavaScript等。在服务器可以使用C、.NET、Java、Python等来开发。
关键特性
    1. 支持多语言之间的RPC互通。
    2. 高性能的RPC调用。
    3. 支持传统的RPC调用、异步调用、One-Way调用、批量发起请求，支持TCP通信、UDP通信等。
    4. 多平台支持。
    5. 不断更新，与时俱进。
Java在分布式系统的核心技术
- JNDI：Java命名和目录服务，类似zooKeeper
- JMS：Java消息服务 如LinkeIn 来源的Kafka ，ZeroMQ，nanomsg都值得研究
- RMI：Java远程通信框架，替代它的有很多，比如Ice、Dubbo、Avro、Thrift、Protocol Buffers协议等。
几个常用来源中间件介绍
其实，一个复杂的分布式系统可以通过搭积木的方式来建立，每个积木就是一个中间件，可通过适当的代码将其黏合在一起。
ZooKeeper 
    ZooKeeper 提供基础的目录/名字服务、配置管理服务。并且在此基础上还能提供分布式锁、leader选举等高级功能。网上有一段有趣生动的介绍:“ZooKeeper，顾名思义就是动物园里大象(hadoop)、蜜蜂(Hive)、小猪(Pig)、和我的猫(MyCat)的管理员”。
    客户端建议采用Apache Curator这个二次封装的客户端来进行客户端代码的编写，它封装和简化了很多于业务无关的逻辑，使用简单，质量可靠。
Apache Kafka
    Kafka是一个高吞吐量的分布式消息系统，由LinkIn开源，被描述为LinkeIn的“中枢神经系统”。Kafka管理从各个应用程序汇聚到此的信息流，这些数据经过处理后再被分发到何处。
    Kafka使用Scala开发，而Scala又是JVM上运行的动态需要，因此对会Java的同学来说学习难度并不大，其客户端也支持Java语言，比较容易部署在本机上进行学习研究。
Facebook Thrift
    Facebook Thrift是最新一代高性能、跨需要的RPC通信框架，支持多种语言。
    Facebook Thrift与Ice类似，不过没有Ice完整和成熟。与Facebook Thrift类似的还有Apache Avro。
Apache Storm、Spark Streaming 、Samza
    与Hadoop相比Storm是个实时的高容错的分布式计算系统。Storm也可以处理批量数据，但其在保证高可靠性的前提下还可以让处理进行得更加实时，所有的信息都会被处理。Storm同样还具备容错和分布计算的特性，这让Storm可以扩展到不同的机器上进行大批量的数据处理。
Storm的主要开发语言为Java，并且包括了Clojure这种Lisp语言，对于Java工程师来说，学习难度并不大。与Strom类似的还有Spark Streaming、LinkIn的Samza，它们都是最近开源的热门项目。
    Spark Streaming是Spark中新的流式计算框架。Spark并不会像Strom那样一次处理一个数据流，而是在处理前按时间间隔预先将其分为一段段的批处理作业。
    而Samza处理数据流时，会依次处理收到的每条消息。Samza的流单位既不是元组，也不是Dstream而是一条条消息。
    Strom、Spark Streming、Samza这三种实时流计算系统都是分布式系统，具有低延迟、可扩展和容错性等诸多优点。它们的共同特同特色在于：允许你在运行数据流代码时，将任务分配到一系列具有容错能力的计算机上并行运行，此外，它们都提供了简单的API来简化底层实现的复杂程度。
MyCat
    MyCat是一个基于MySQL的分表分库的中间件软件，也可以说是一个分布式数据库，其前身为阿里巴巴开源的Cobar，但由于阿里巴巴后来发展云平台并提供收费RDS服务，因此最终关闭了这个开源项目。然后国内一帮技术大拿一起发起了MyCat开源项目 ( http : / / www my cat org. cn ），目标是成为Apache旗下的顶级中国开源项目。
章节笔记
3.1 看不大明白
3.2-3.5  有实现
3.6 IcePatch2介绍使用-部署、同步。
4.1 改造Ice日志，将ice日志以logback的方式保存。
4.2 封装ice客户端，很实用，已经实现demo。
4.3 集成Ice与Spring框架
4.4 实现Ice服务质量监控，通过代码拦截采集信息、JRDS监控等。
45. 使用Maven标准化项目
5.1 深入解析Ice异步调用，怎么结合Ice本事实现异步
5.2 Ice附在均衡机制研究，多负载均衡深入研究
5.3 Ice SLL实践。加密 数字证书 CA（收费）  自己做CA证书，需要注意，Tcp调用和SLL调用100次性能下降4倍。这就是为什么Ice默认采用TCP通信和大多数系统不使用SSL加密的方式进行接口调用的原因。
5.4 PHP优化建议。
5.5 Ice性能调优
    需要注意Ice默认线程池大小是1 。一般我们要设置成CPU核心数的4-16倍，以支持更多的并发性和防止请求阻塞。
    Ice for Java 是运行在JVM上面的，JVM调优的几个原则如下
        1. 尽量使用稳定版本的JVM，因为每次JVM的升级都带来了性能的升级。
        2. 由于会在一个服务器上启动多个Ice Node实例，因此每个Ice Node上的IceBox不要分配过大的内存以造成浪费和竞争。观察GC的波动，在性能测试过程中监控JVM堆内存的峰值，来设定一个合理的堆大小。
        3. 不建议设置不确定的优化参数，以免造成困惑。
        4. 调优的目标是GC总体时间减少，内存使用稳定，避免大幅波动。
5.6 Ice部署方案
    要满足以下条件
        1. 避免单点故障
            将Ice的注册表服务Registry部署为主从模式，主节点宕机时，从节点继续工作，为了可靠需要在两台不同的服务器上分别启动进程；
        2. 提供服务的负载均衡
            每个Ice服务部署在至少两台不同的服务器上并启动负载均衡机制；
        3. 尽可能充分利用硬件资源
            通过将CPU密集型的服务与I/O密集型服务交叉部署在一台服务器上来达到目标；
    实际部署案例，6台X86服务器组成IceGrid集群，如图：
![](https://img-blog.csdn.net/20160307192518532?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        Ice-Reg-M是Ice的注册表主节点，部署和启动注册表服务，并启动一个Ice Node，在此Node上根据情况部署访问频率和服务压力（主要是CPU）不是很高的一组服务。
        另外，Ice-Reg-M和Ice-Reg-S上同时提供Ice服务部署同步(3.6介绍过)的Server端进程，用于Ice服务的二进制文件的同步更新操作，其余节点全部安装同步客户端命令，默认连接Ice-Reg-M，实现版本更新和部署升级。
        其余四台各产生两组互为主备的结构。
6.1 Ice For JavaScript Ice3.6才支持，文档和demo还不是很完善。是基于WebSocket技术。
6.2 Ice对移动设备的支持，支持Android（Java）和IOS（Object-C）
第7 章 互联网架构师深造之路
还是有收获的，里面有些理念还是比较独特的。但是整体来说还是不能看懂看透，只能大概了解下，写些demo。感觉书本作者也不是很用心，代码格式有点乱~ ，此书的精华还未完全吸收。等我在修炼一段时间再过来看看。
试读章节：[http://pan.baidu.com/s/1sk6ffSL](http://pan.baidu.com/s/1sk6ffSL)
ice概念和原理：[http://share.weiyun.com/aed5e6b95bac127fec852206625ec69e](http://share.weiyun.com/aed5e6b95bac127fec852206625ec69e)
