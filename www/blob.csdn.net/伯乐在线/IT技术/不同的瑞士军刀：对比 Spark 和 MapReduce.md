# 不同的瑞士军刀：对比 Spark 和 MapReduce - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [古月水语](http://www.jobbole.com/members/hbdt08) 翻译，[顾星竹](http://www.jobbole.com/members/harjoogoo) 校稿。未经许可，禁止转载！
英文出处：[Saggi Neumann](https://www.xplenty.com/blog/2014/11/apache-spark-vs-hadoop-mapreduce/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
Apache 基金会下的 Spark 再次引爆了大数据的话题。带着比 Hadoop MapReduce 速度要快 100 倍的承诺以及更加灵活方便的 API，一些人认为这或许预示着 Hadoop MapReduce 的终结。
作为一个开源的数据处理框架，Spark 是如何做到如此迅速地处理数据的呢？秘密就在于它是运行在集群的内存上的，而且不受限于 MapReduce 的二阶段范式。这大大加快了重复访问同一数据的速度。
Spark 既可以单独运行，也可以运行在 Hadoop YARN 上（注：Hadoop第二代框架中的改进框架，用于将资源管理和处理组件分开，基于YARN的结构不受 MapReduce 约束），此时 Spark 可以直接从 HDFS （Hadoop Distributed File System 分布式文件系统）中读取数据。 诸如  Yahoo（雅虎）、Intel（因特尔）、Baidu（百度）、Trend Micro（趋势科技）和 Groupon（高朋）等公司已经在使用 Spark 了。
听上去好像 Spark 已经注定要取代 Hadoop MapReduce 了。但真的是这样吗？本文我们将对比这两个平台来看看是否 Spark 真的技高一筹。
![](http://ww1.sinaimg.cn/large/7cc829d3gw1ezx5tcb4xfj20hz0ciwfg.jpg)
### 性能
Spark 在内存中处理数据，而 Hadoop MapReduce 是通过 map 和 reduce 操作在磁盘中处理数据。因此从这个角度上讲 Spark 的性能应该是超过 Hadoop MapReduce 的。
然而，既然在内存中处理，Spark 就需要很大的内存容量。就像一个标准的数据库系统操作一样， Spark 每次将处理过程加载到内存之中，然后该操作作为缓存一直保持在内存中直到下一步操作。如果 Spark 与其它资源需求型服务一同运行在 Hadoop YARN 上，又或者数据块太大以至于不能完全读入内存，此时 Spark 的性能就会有很大的降低。
与此相反， MapReduce 会在一个工作完成的时候立即结束该进程，因此它可以很容易的和其它服务共同运行而不会产生明显的性能降低。
当涉及需要重复读取同样的数据进行迭代式计算的时候，Spark 有着自身优势。 但是当涉及单次读取、类似 ETL （抽取、转换、加载）操作的任务，比如数据转化、数据整合等时，MapReduce 绝对是不二之选，因为它就是为此而生的。
小结：当数据大小适于读入内存，尤其是在专用集群上时，Spark 表现更好；Hadoop MapReduce 适用于那些数据不能全部读入内存的情况，同时它还可以与其它服务同时运行。
### 使用难度
Spark 有着灵活方便的Java，Scala和 Python 的API，同时对已经熟悉 SQL 的技术员工来说， Spark 还适用 Spark SQL（也就是之前被人熟知的 Shark）。多亏了 Spark 提供的简单易用的构造模块，我们可以很容易的编写自定义函数。它甚至还囊括了可以即时反馈的交互式命令模式。
Hadoop MapReduce 是用 Java 编写的，但由于其难于编程而备受诟病。尽管需要一定时间去学习语法，Pig 还是在一定程度上简化了这个过程， Hive也为平台提供了 SQL 的兼容。一些 Hadoop 工具也可以无需编程直接运行 MapReduce 任务。Xplenty 就是一个基于 Hadoop 的数据整合服务，而且也不需要进行任何编程和部署。
尽管 Hive 提供了命令行接口，但 MapReduce 并没有交互式模式。诸如 Impala，Presto 和 Tez 等项目都在尝试希望为 Hadoop 提供全交互式查询模式。
安装与维护方面， Spark 并不绑定在 Hadoop 上，虽然 在 Hortonworks（HDP 2.2 版） 和 Cloudera（CDH 5 版） 的产品中 Spark 和 Hadoop MapReduce 都包含在其分布式系统中。（注： Cloudera， Hortonworks 及 MapR 是 Hadoop 领域三大知名的初创公司，致力于打造更好的 Hadoop 企业版应用）。
小结：Spark 更易于编程，同时也包含交互式模式；Hadoop MapReduce 不易编程但是现有的很多工具使其更易于使用。
### 成本
Spark 和 Hadoop MapReduce 都是开源的，但是机器和人工的花费仍是不可避免的。
这两个框架既可以在商用服务器上也可以运行在云端，下表可以看到它们有着相似的硬件需求：
框架                 Apache Spark                        Apache Hadoop balanced workload slaves
内核                      8–16                                                           4
内存             8 GB 到数百GB                                                24 GB
硬盘                      4–8                                                        4–6 1TB
网络                10 GB 或更多                                           1 GB 以太网
Spark 集群的内存至少要和需要处理的数据块一样大，因为只有数据块和内存大小合适才能发挥出其最优的性能。所以如果真的需要处理非常大的数据，Hadoop 绝对是合适之选，毕竟硬盘的费用要远远低于内存的费用。
考虑到 Spark 的性能标准，在执行相同的任务的时候，需要的硬件更少而运行速度却更快，因此应该是更合算的，尤其是在云端的时候，此时只需要即用即付。
在技术人员方面，即使 Hadoop 从 2005 年就开始普及，但是 MapReduce 方面的专家仍然存在着短缺。而对于从 2010 年才开始普及的 Spark ，这又意味着什么呢？ 或许投身 Spark 学习的人正在快速增加，但是相比于 Hadoop MapReduce 仍然存在着更大的技术人才的缺口。
进一步讲，现存了大量的 Hadoop 即服务的资料和基于 Hadoop 的服务（比如我们 Xplenty 的数据整合服务），这些都降低对技术人员能力和底层硬件知识的要求。相比之下，几乎没有现有可选的 Spark 服务，仅有的那些也是新产品。
小结：根据基准要求， Spark 更加合算， 尽管人工成本会很高。依靠着更多熟练的技术人员和 Hadoop 即服务的供给， Hadoop MapReduce 可能更便宜。
### 兼容性
Spark 既可以单独运行，也可以在 Hadoop YARN 上，或者在预置 Mesos 上以及云端。它支持实现 Hadoop 输入范式的数据源，所以可以整合所有 Hadoop 支持的数据源和文件格式。 根据 Spark 官方教程， 它还可以通过 JDBC 和 ODBC 同 BI（商业智能） 工具一起运行。 Hive 和 Pig 也在逐步实现这样的功能。
小结： Spark 和 Hadoop MapReduce 具有相同的数据类型和数据源的兼容性。
### 数据处理
除了平常的数据处理，Spark 可以做的远不止这点：它还可以处理图和利用现有的机器学习库。高性能也使得 Spark 在实时处理上的表现和批处理上的表现一样好。这也催生了一个更好的机遇，那就是用一个平台解决所有问题而不是只能根据任务选取不同的平台，毕竟所有的平台都需要学习和维护。
Hadoop MapReduce 在批处理上表现卓越。如果需要进行实时处理，可以利用另外的平台比如  Storm 或者 Impala，而图处理则可以用 Giraph。MapReduce 过去是用 Mahout 做机器学习的，但其负责人已经将其抛弃转而支持 Spark 和 h2o（机器学习引擎）。
小结：Spark 是数据处理的瑞士军刀；Hadoop MapReduce 是批处理的突击刀。
### 容错
和 MapReduce 一样， Spark 会重试每个任务并进行预测执行。然而，MapReduce 是依赖于硬盘驱动器的，所以如果一项处理中途失败，它可以从失败处继续执行，而 Spark 则必须从头开始执行，所以 MapReduce 这样节省了时间。
小结：Spark 和 Hadoop MapReduce 都有着较好的容错能力，但是 Hadoop MapReduce 要稍微更好一点。
### 安全性
在安全性上， 此时的 Spark 还略显不足。 授权验证由共享秘钥机制支持，网络用户接口则通过 servlet 过滤器和事件日志保护。Spark 可以运行在 YARN 上并配合使用 HDFS， 这也就意味着它同时还拥有 Kerberos 认证授权验证，HDFS 文件许可机制和节点间的加密机制。
Hadoop MapReduce 拥有所有 Hadoop 支持的安全机制，同时也整合了其它基于 Hadoop 的安全项目， 比如  Knox 网关和  Sentry。志在解决 Hadoop 安全的  Rhino 项目也只是在添加 Sentry 支持时添加了 Spark 支持。否则 Spark 开发者们只能自己去提升其安全性了。
小结： Spark 的安全机制仍处在发展期。 Hadoop MapReduce 拥有更多安全控制机制和项目。
## 总结
Spark 是大数据领域冉冉升起的新星，但是 Hadoop MapReduce 仍有着较广的应用领域。
在内存中进行数据处理使得 Spark 具有较好的性能表现，也比较高效合算。它兼容所有 Hadoop 的数据源和文件格式， 支持多种语言的简单易用的 API 也使人们更快速的可以上手。 Spark 甚至实现了图处理和机器学习工具。
Hadoop MapReduce 是一个更加成熟的平台，为进行批处理而生。当遇到确实非常大的数据以至于无法完全读入内存，又或是依靠着大量对该平台有经验的技术人员，它可能会比 Spark 更加合算。 而且围绕 Hadoop MapReduce 的衍生系统正在依靠着更多的支撑项目、工具和云服务而更加壮大。
但是即使看上去 Spark 像是最终的赢家，问题在于我们永远不会单独使用它—我们需要 HDFS 存储数据，或许还会需要用到 HBase，Hive，Pig，Impala 或其他 Hadoop 项目。这意味着在处理非常大的数据的时候，Spark 仍然需要同 Hadoop 和 MapReduce 共同运行。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/9c903122240ecf2f55fe1500d5b1abb1.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/13d1510be0f79679c7789f5ee8889910.jpg)
