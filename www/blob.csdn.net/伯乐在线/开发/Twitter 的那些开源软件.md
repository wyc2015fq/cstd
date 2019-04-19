# Twitter 的那些开源软件 - 文章 - 伯乐在线
原文出处： [oschina](http://www.oschina.net/news/74962/twitter-open-source-software-recommendation)
从Twitter的GitHub账户中可以看到，Twitter已经开源的开源项目有近200个，领域涉及分布式架构、大数据、异步网络传输（客户端、服务端）、Web、工具等。Twitter可以称为构建于开源项目之上，该公司开源负责人Chris Aniszczyk表示，如果没有开源软件，Twitter将不会存在，用户在移动端和PC端发送和接收的每一条推文都会需要开源软件。
### Typeahead.js——自动文字补齐jQuery插件
这款jQuery插件来自于Twitter的一个新的项目，支持远程和本地的数据集。比较有特色的地方在于你可以将数据集使用本地存储(local storage)来保存在本地，有效的提高用户体验。同时也拥有很多远程数据集的处理选项，例如（请求频率，最大的并发请求数，等等）。
主要特性：
- 支持数据本地保存，客户端加载，优化加载速度
- 支持多语言，并且支持阿拉伯文
- 支持Hogan.js模板引擎整合
- 支持多数据集拼装
- 支持本地和远程的数据集
**源码地址：**[**https://github.com/twitter/typeahead.js**](https://github.com/twitter/typeahead.js)
### Twemoji——Twitter 的 Emoji 表情
TTwemoji 是 Twitter 开源的其完整的 Emoji 表情图片。开发者可以去GitHub下载完整的表情库，并把这些表情加入到自己的应用或网页中。
![](http://jbcdn2.b0.upaiyun.com/2016/07/49582ea6b99c215a27a86fc26487db53.png)
**源码地址：[https://github.com/twitter/twemoji](https://github.com/twitter/twemoji)**
### **Hogan.js****——JS模板引擎**
Hogan.js是Twitter团队所制作的一个针对mustache模板的语法解析器。Hogan.js不依赖其他任何库或框架，同时保证了高效率的模板解析，而其体积却仅有2.5K。用它作为你的一部分资产打包编译模板提前或将它包括在你的浏览器来处理动态模板。
**源码地址：[http://twitter.github.com/hogan.js](http://twitter.github.com/hogan.js)**
### Effective Scala——Scala语言
Scala是Twitter的主要应用编程语言之一，大部分基础架构是使用Scala编写，有几个大型库包在支持应用，Scala是一种大型高效语言，在实践中要谨慎使用。它的陷阱在哪里，哪个特性我们很喜欢，另外哪些应该注意回避？当在实现“纯函数风格”时，又要注意些什么呢？Scala主要是创建大量形成分布式系统的服务。
Scala提供了需要工具用于简化表达，少打字代表少阅读，少阅读代表能快速阅读，简洁能够增加清晰度（大道至简）。但是简洁也是一种双刃剑，会导致其反面效果，导致阅读者的正确理解度不够。
**源码地址：[https://github.com/twitter/effectivescala](https://github.com/twitter/effectivescala)**
### Finagle——RPC框架
Finagle 是一个容错的、与协议无关的用于JVM 的RPC系统。Finagle 使用 sbt 进行构建。Finagle 来自 Twitter ！它使得在 Java、Scala 或任何基于 JVM 的语言重构建鲁棒的客户端和服务器非常容易。Finagle 支持广泛的基于请求/答复的 RPC 协议和很多类型的流协议
使用 Finagle 可以快速实现异步的远程方法调用 RPC 客户端和服务器端，本身足够灵活支持多种 RPC 变种，包括请求响应式、流和管道模式，如 HTTP 管道和 Redis 管道，也可轻松的有状态的 RPC 一起运行，例如那些需要认证的 RPC 服务。
![](http://jbcdn2.b0.upaiyun.com/2016/07/f612bd9fa0956a3a9a5a39afd0430243.png)
![](http://jbcdn2.b0.upaiyun.com/2016/07/089c454c5781229053a08862ca733fa7.png)
**源码地址：[https://github.com/twitter/finagle](https://github.com/twitter/finagle)**
### FlockDB——分布式图形数据库
FlockDB将图存储为一个边的集合，每条边用两个代表顶点的64位整数表示。对于一个社会化网络图，这些顶点ID即用户ID，但是对于“收藏”推文这 样的边，其目标顶点(destination id)则是一条推文的ID。每一条边都被一个64位的位置信息标识，用于排序。(Twitter在“关注”类的边上用了时间戳标识，所以你的关注者列表时 按时间排序的，最新的在最前面。）
**源码地址：[https://github.com/twitter/flockdb](https://github.com/twitter/flockdb)**
### Snowflake——分布式自增ID算法
Twitter在把存储系统从MySQL迁移到Cassandra的过程中，由于Cassandra没有顺序ID生成机制，于是自己开发了一套全局唯一ID生成服务：Snowflake。优点是：高性能，低延迟；独立的应用；按时间有序。缺点是：需要独立的开发和部署。
41位的时间序列（精确到毫秒，41位的长度可以使用69年）；
10位的机器标识（10位的长度最多支持部署1024个节点）；
12位的计数顺序号（12位的计数顺序号支持每个节点每毫秒产生4096个ID序号）最高位是符号位，始终为0。
高效很方便的GUID产生算法，一个int64_t字段就可以胜任，不像现在主流128bit的GUID算法，即使无法保证严格的ID序列性，但是对于特定的业务，比如用做游戏服务器端的GUID产生会很方便。另外，在多线程的环境下，序列号使用Atomic可以在代码实现上有效减少锁的密度。
源码地址：[https://github.com/twitter/snowflake](https://github.com/twitter/snowflake)
### Diffy——自动化测试工具
Diffy是一个开源的自动化测试工具，它能够自动检测基于Apache Thrift或者基于HTTP的服务。使用Diffy，只需要进行简单的配置，之后不需要再编写测试代码。
Diffy主要基于稳定版本和它的副本的输出，对候选版本的输出进行比较，以检查候选版本是否正确。因此，Diffy首先假设候选版本应该和稳定版本有“相似”的输出。即不论候选版本和稳定版本系统模块是否相同，他们的最终输出应该是“相似”的。这里一直使用“相似”，而不是使用相同，是因为相同请求可能会有一些Diffy不需要关心的干扰，比如：
- 响应中包含服务器生成的时间戳；
- 代码中使用了随机数；
- 系统服务间有条件竞争。
**源码地址：****[https://github.com/twitter/diffy](https://github.com/twitter/diffy)**
### Scalding——Scala库
Scalding 是一个 Scala 库简化了 Hadoop MapReduce 作业开发。基于 Cascading 构建。Scalding 跟 Pig 类似，但提供更紧密的 Scala 集成。
Hadoop 是一个统计词（counting words）的分布式系统。
**源码地址：****[https://github.com/twitter/scalding](https://github.com/twitter/scalding)**
### Gizzard——通用数据切分中间件
Gizzard是Twitter在2011年4月份新推出的一个通用数据切分中间件，在Twitter的架构中占重要的作用。Twitter还公布了Gizzard的完整代码。有了Gizzard，初创公司和小公司就可以更好更快地处理大量数据，从而利用更少的资源满足用户需求。Gizzard的主要功能如下：
- 支持不同的底层数据存储，Redis/Memcache/Mysql等都支持，原则上只要写操作幂等（也就是写操作与顺序无关）则都可以支持；
- 通用数据拆分支持，支持一致性hash、主键mod、自定义拆分函数等多种方式；
- 通过replication tree实现不同节点数据的备份机制；
- 容错机制，在一台机器出问题后，会自动保存更新延迟队列，在恢复后重新执行，从而保证一致性；
- 快速迁移。
![](http://jbcdn2.b0.upaiyun.com/2016/07/3df7f290c2a9834747147271f0a72968.jpg)
**源码地址：**[**https://github.com/twitter/gizzard**](https://github.com/twitter/gizzard)
### Summingbird——流处理框架
Summingbird是MapReduce流处理框架，一个大规模数据处理系统，支持开发者以批处理模式（基于Hadoop/MapReduce）或流处理模式（基于Storm）或混合模式（即组合前两种模式）以统一的方式执行代码。它基于Apache 2许可发布，用于解决工程师使用现有方法遇到的实际问题：
- 两个不同系统中的两组聚合逻辑必须保持同步；
- 在每个系统和客户端之间，键和值必须一致地进行序列化；
- 客户端要负责从两个数据存储读取数据、执行最后的聚合并提供合并结果。
**源码地址：**[**https://github.com/twitter/summingbird**](https://github.com/twitter/summingbird)
### Algebird——Scala的抽象代数工具
Algebird是用于Scala的抽象代数。这些代码主要是用于建立聚合系统（通过Scalding或Storm）。Algebird跟Summingbird这个组件相关：利用一些概率算法HyperLogLog来提高计算速度。
**源码地址：[https://github.com/twitter/algebird](https://github.com/twitter/algebird)**
### Iago——网站负载测试工具
Iago是一个网站负载测试工具，Iago针对一个给定的网站进行访问录制并合成流量数据。它不同于其他的负载生成工具，它试图保持恒定的请求率。例如，如果你想按每分钟100K来请求你的服务，Iago会试图保持这个速度进行测试。
**源码地址：**[**https://github.com/twitter/iagox**](https://github.com/twitter/iagox)
### Heron——数据实时分析平台
2016年5月25日，Twitter正式宣布Heron开源。Heron的基本原理和方法：实时流系统是在大规模数据分析的基础上实现系统性的分析。另外，它还需要：每分钟处理数十亿事件的能力、有秒级延迟，和行为可预见；在故障时保证数据的准确性，在达到流量峰值时是弹性的，并且易于调试和在共享的基础设施上实现简单部署。
为了满足这些需求，Twitter讨论出了几种方案，包括：扩展Storm、使用其他的开源系统、开发一个全新的平台。因为有几个需求是要求改变 Storm的核心架构，所以对它进行扩展需要一个很长的开发周期。其他的开源流处理框架并不能完美满足Twitter对于规模、吞吐量和延迟的需求。而且，这些系统也不能兼容Storm API——适应一个新的API需要重写几个topologies和修改高级的abstractions，这会导致一个很长的迁移过程。所以，Twitter决定建立 一个新的系统来满足以上提到需求和兼容Storm API。
在Twitter，Heron作为主要的流媒体系统，运行数以百万计的开发和生产topologies。由于Heron可高效使用资源，在迁移Twitter所有的topologies后，整体硬件减少了3倍，导致Twitter的基础设置效率有了显著的提升。
**源码地址：**[**https://github.com/twitter/heron**](https://github.com/twitter/heron)
### DistributedLog——分布式日志复制服务
DistributedLog（DL）是一个高性能的日志复制服务，提供了持久化、复制以及强一致性的功能，这对于构建可靠的分布式系统都是至关重要的，如复制状态机（replicated-state-machines）、通用的发布/订阅系统、分布式数据库以及分布式队列。DL会分类维护记录的序列（sequences of records），并将其称为Log（又叫做Log Stream），将记录写入到DL Log的进程称之为Writer，从Log中读取并处理记录的进程称之为Reader。DL的优势可以总结为:
- 高性能：面对大量的并发日志时，在可持久化的Writer上DL能够提供毫秒级的延迟，同时还能应对上千客户端每秒大量的读取和写入操作；
- 持久化和一致性：消息会持久化到磁盘上，并且以副本的形式存储多份，从而避免丢失。通过严格的顺序，保证Writer和Reader之间的一致性；
- 各种工作负载：DL支持各种负载，包括延迟敏感的在线事务处理（OLTP）应用（如分布式数据库的WAL和基于内存的复制状态机）、实时的流提取和计算以及分析处理；
- 多租户：针对实际的工作负载，DL的设计是I/O隔离的，从而支持多租户的大规模日志；
- 分层架构：DL有一个现代化的分层设计，它将有状态的存储层与无状态的服务提供层进行了分离，能够使存储的扩展独立于CPU和内存，因此支持大规模的写入fan-in和读取fan-out。
**源码地址：[https://github.com/twitter/distributedlog](https://github.com/twitter/distributedlog)**
### Ambrose——可视化监视系统
Ambrose是Twitter发布的开源MapReduce可视化监视系统。 它可以监视Hadoop集群上（目前只限于Apache Pig）的MapReduce任务。Ambrose计划支持：
- Pig（已实现）
- Cascading
- Scalding
- Cascalog
- Hive
![](http://ww2.sinaimg.cn/large/7cc829d3gw1f5jygcua8hg20op0kb7wh.gif)
**源码地址：**[**https://github.com/twitter/ambrose**](https://github.com/twitter/ambrose)
### SecureHeaders——Web安全开发工具
SecureHeaders是Twitter送给Web开发者的一份大礼，作为一款Web安全开发工具，Secureheaders能够自动实施安全相关的header规则，包括内容安全政策（CSP），防止XSS、HSTS等攻击，防止火绵羊（Firesheep）攻击以及XFO点击劫持等。
**源码地址：**[**https://github.com/twitter/secureheaders**](https://github.com/twitter/secureheaders)
### Activerecord-Reputation-System——活动记录信誉系统
Activerecord-Reputation-System系统基于Rails开发，可以根据网络中的评价自动对应用程序进行信誉估值，帮助开发者发现更多的应用程序相关信息，从而指导下一步决策。Twitter称，开发者可以很容易地在Rails应用程序中集成该系统，或者从主应用程序中分离该系统，以便做出更好的设计。
该系统是一个信誉网络，网络中的数据根据评价进行更新，然后计算信誉值并通过网络进行传播。在该网络中，直接根据评价计算的信誉值称为原始信誉（primary reputations），间接计算的称为非原始信誉（non-primary reputations）。
![Alt text](http://jbcdn2.b0.upaiyun.com/2016/07/20b8c5cec6be79e933c61ba2503dda9e.png)
**源码地址：**[**https://github.com/twitter/activerecord-reputation-system**](https://github.com/twitter/activerecord-reputation-system)
### CocoaSPDY——SPDY框架
CocoaSPDY是一个面向OS X（Cocoa）和iOS（Cocoa Touch）的SPDY框架，基于他们先前对Netty的贡献，同时，他们更新了其iOS应用程序，使用SPDY代替了纯HTTP。Twitter已经注意到，通信延迟降低了多达30%，当“用户的网络状况变得更糟”时，改善效果更明显。
SPDY有另外一个优点：“多路复用请求”—— 一项在单个TCP会话中连续不断地发送请求并接收乱序响应的能力、从服务器向客户端推送消息，以及压缩请求和响应的头信息。
**源码地址：**[**https://github.com/twitter/CocoaSPDY**](https://github.com/twitter/CocoaSPDY)
### TwUI——UI框架
TwUI是一个支持硬件加速的Mac的UI框架：
- 使用CoreAnimation实现的GPU加速；
- 简单的MVC开发。
与UIKit不同的地方：
- 简化Table View单元；
- 基于块的布局和drawRect；
- 一个统一的坐标系统（bottom left origin）；
- Sub-pixel文本绘制。
![](http://jbcdn2.b0.upaiyun.com/2016/07/fc6d731e13f0811110cd40fc4a91b55b.png)
**源码地址：**[**https://github.com/twitter/twui**](https://github.com/twitter/twui)
### Twemproxy——代理服务器
Twemproxy是一个快速的单线程代理程序，支持Memcached ASCII协议和更新的Redis协议。它全部用C写成，使用Apache 2.0 License授权。Twemproxy的强大之处在于可以通过配置的方式让它禁用掉失败的结点，同时还能在一段时间后进行重试，抑或使用指定的键->服务器映射。这意味着在将Redis用作数据存储时，它可以对Redis数据集进行分片（禁用掉结点驱逐）；在将Redis用作缓存时，它可以启用结点驱逐以实现简单的高可用性。它的特性是：
- 通过代理的方式减少缓存服务器的连接数；
- 自动在多台缓存服务器间共享数据；
- 通过不同的策略与散列函数支持一致性散列；
- 通过配置的方式禁用失败的结点；
- 运行在多个实例上，客户端可以连接到首个可用的代理服务器；
- 支持请求的流式与批处理，因而能够降低来回的消耗；
- 速度快；
- 轻量级。
**源码地址：**[**https://github.com/twitter/fatcache**](https://github.com/twitter/fatcache)
### Fatcache——缓存服务
Fatcache可以让你在SSD上运行memcached，你可以把它当作是大数据中的缓存。它的一些性能数据如下：
- 单节点可每秒可处理10万set操作，每个数据包是100字节；
- 单节点可每秒处理4.5k get操作，每个数据100字节；
- 8 个Fatcache实例可处理32k get操作/每秒到一个单600GB的SSD存储；
- 你可以通过将多个SSD连接到单个机器来提升IO性能。
**源码地址：**[**https://github.com/twitter/fatcache**](https://github.com/twitter/fatcache)
### AnomalyDetection——自动侦测时间序列异常点的R包
AnomalyDetection是一个R语言程序包，Twitter通常会在重大新闻和体育赛事期间用AnomalyDetection扫描入站流量，发现那些使用僵尸账号发送大量垃圾（营销）信息的机器人。
![流量异常侦测figure_localglobal_anomalies](http://jbcdn2.b0.upaiyun.com/2016/07/7173c8f928ac8e5a7f8edcbf0b2cf3fd.png)
*AnomalyDetection异常扫描*
据Twitter透露，AnomalyDetection与Twitter去年10月开源的BreakoutDetection存在互补关系。
流量异常侦测对于号称“地球脉搏”的Twitter来说非常具有挑战性，因为对流量进行长周期跨度（例如一年）的扫描分析时，一些异常活动往往会掩 盖掉。此外，流量异常的原因也各有不同，有的是健康的，例如重大新闻事件导致的流量异常，而有的是不好的，例如QPS（每秒查询量）中point-in- time实时性下降可能意味着硬件或数据采集方面出了问题。
![Twitter流量异常侦测-长期figure_longterm](http://jbcdn2.b0.upaiyun.com/2016/07/1188169d17c9a8cb78c7f3b8cfa9714c.png)
*长周期流量异常侦测*
**源码地址：****[https://github.com/twitter/AnomalyDetection](https://github.com/twitter/AnomalyDetection)**
