# 探秘Hadoop生态4：HBase无伤入门 - 程序员江湖 - CSDN博客





2017年03月08日 09:40:58[黄小斜](https://me.csdn.net/a724888)阅读数：749标签：[Hadoop](https://so.csdn.net/so/search/s.do?q=Hadoop&t=blog)
个人分类：[Hadoop](https://blog.csdn.net/a724888/article/category/7779280)

所属专栏：[后端技术杂谈](https://blog.csdn.net/column/details/25481.html)









本文由毕杰山同学贡献 ，原文首发于公众号：NoSQL漫谈  链接：https://mp.weixin.qq.com/s/CXsGcbbsKTMXotlwRFQ5xw



一些常见的HBase新手问题



- 
什么样的数据适合用HBase来存储？

- 
既然HBase也是一个数据库，能否用它将现有系统中昂贵的Oracle替换掉？

- 
存放于HBase中的数据记录，为何不直接存放于HDFS之上？

- 
能否直接使用HBase来存储文件数据？

- 
Region(HBase中的数据分片)迁移后，数据是否也会被迁移？

- 
为何基于Spark/Hive分析HBase数据时性能较差？

![](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/BmfThWjVGAQNP2lyZ3KNPRGVt9G5AEfYdicp3KRkY44ChznXLyHI7hpI2MyFVARTgVYqdH58VlkoX4XAHWR9bJA/640?wxfrom=5&wx_lazy=1)



开篇




用惯了Oracle/MySQL的同学们，心目中的数据表，应该是长成这样的：

![](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/licvxR9ib9M6AvECce69uqxAGjO2tFu95JMiaeRR1mCZWq05KI1M6eTu5BXic7sN8M0uPKUIxicFc13WNliaJic6GVAdQ/640?wxfrom=5&wx_lazy=1)

这种表结构规整，每一行都有固定的列构成，因此，非常适合结构化数据的存储。但在NoSQL领域，数据表的模样却往往换成了另外一种"画风"：


![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/licvxR9ib9M6AvECce69uqxAGjO2tFu95JpZnOXttW9e3Gm1fvl2FN8xaaQpVxfeFcCApyT8v3S5JfTXmkosGCpA/640?)




行由看似"杂乱无章"的列组成，行与行之间也无须遵循一致的定义，而这种定义恰好符合半结构化数据或非结构化数据的特点。本文所要讲述的HBase，就属于该派系的一个典型代表。这些"杂乱无章"的列所构成的多行数据，被称之为一个"稀疏矩阵"，而上图中的每一个"黑块块"，在HBase中称之为一个KeyValue。



Apache HBase官方给出了这样的定义：




> 
Apache HBase™ is the Hadoop database, adistributed,scalable,big data store.





即：Apache HBase是基于Hadoop构建的一个分布式的、可伸缩的海量数据存储系统。




HBase常被用来存放一些海量的(通常在TB级别以上)结构比较简单的数据，如历史订单记录，日志数据，监控Metris数据等等，HBase提供了简单的基于Key值的快速查询能力。




HBase在国内市场已经取得了非常广泛的应用，在搜索引擎中，也可以看出来，HBase在国内呈现出了逐年上升的势态：

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/licvxR9ib9M6AvECce69uqxAGjO2tFu95JXgm7CPcGyFMmTcoBf95DIBF0nlkwYdP1rohQ3cpnldCHe5dicCZgdGg/640?)




从Apache HBase所关联的github项目的commits统计信息来看，也可以看出来该项目非常活跃：

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/licvxR9ib9M6AvECce69uqxAGjO2tFu95J1nNO4XYKGpMqnjib5Lm8DKyukvuBoIQLZm3265Vd3bAo5NnXuQ4Lm3w/640?)

(需要说明的一点：HBase中的每一次commit，都已经过社区Commiter成员严格的Review，在commit之前，一个Patch可能已经被修改了几十个版本)




令人欣喜的是，国内的开发者也积极参与到了HBase社区贡献中，而且被社区接纳了多名PMC以及Committer成员。




本文将以一条数据在HBase中的“旅程”为线索，介绍HBase的核心概念与流程，几乎每一部分都可以展开成一篇独立的长文，但本文旨在让读者能够快速的了解HBase的架构轮廓，所以很多特性/流程被被一言带过，但这些特性在社区中往往经历了漫长的开发过程。至于讲什么以及讲到什么程度，本文都做了艰难的取舍，在讲解的过程中，将会穿插解答本文开始所提出的针对初学者的一些常见问题。




本文适用于HBase新手，而对于具备一定经验的HBase开发人员，相信本文也可以提供一些有价值的参考。本文内容基于HBase 2.0 beta 2版本，对比于1.0甚至是更早期的版本，2.0出现了大量变化，下面这些问题的答案将揭示部分关键的变化（新手可以直接跳过这些问题）：




> - 
HBasemetaRegion在哪里提供服务？

- 
HBase是否可以保证单行操作的原子性？

- 
Region中写WAL与写MemStore的顺序是怎样的？

- 
你是否遇到过Region长时间处于RIT的状态？ 你认为旧版本中Assignment Manager的主要问题是什么？

- 
在面对Full GC问题时，你尝试做过哪些优化？

- 
你是否深究过HBase Compaction带来的“写放大”有多严重？

- 
HBase的RPC框架存在什么问题？

- 
导致查询时延毛刺的原因有哪些？






本系列文章的整体行文思路如下：
- 
介绍HBase数据模型

- 
基于数据模型介绍HBase的适用场景

- 
快速介绍集群关键角色以及集群部署建议

- 
示例数据介绍

- 
写数据流程

- 
读数据流程

- 
数据更新

- 
负载均衡机制

- 
HBase如何存储小文件数据





这些内容将会被拆成几篇文章。至于集群服务故障的处理机制，集群工具，周边生态，性能调优以及最佳实践等进阶内容，暂不放在本系列文章范畴内。




约定




1. 本文范围内针对一些关键特性/流程，使用了加粗以及加下划线的方式做了强调，如"ProcedureV2"。这些特性往往在本文中仅仅被粗浅提及，后续计划以独立的文章来介绍这些特性/流程。

2. 术语缩写：对于一些进程/角色名称，在本文范围内可能通过缩写形式来表述：




![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/licvxR9ib9M6AvECce69uqxAGjO2tFu95JsBx8WbK0szGbpOTjF62Av6QOP9B4lOSoYPaga0icvtGfOuRXPoV2VQw/640?)




数据模型

### RowKey




用来表示唯一一行记录的主键，HBase的数据是按照RowKey的字典顺序进行全局排序的，所有的查询都只能依赖于这一个排序维度。




> 
通过下面一个例子来说明一下"字典排序"的原理：

RowKey列表{"abc", "a", "bdf", "cdf", "defg"}按字典排序后的结果为{"a", "abc", "bdf", "cdf", "defg"}

也就是说，当两个RowKey进行排序时，先对比两个RowKey的第一个字节，如果相同，则对比第二个字节，依次类推...如果在对比到第M个字节时，已经超出了其中一个RowKey的字节长度，那么，短的RowKey要被排在另外一个RowKey的前面。


### 稀疏矩阵




参考了Bigtable，HBase中一个表的数据是按照稀疏矩阵的方式组织的，"开篇"部分给出了一张关于HBase数据表的抽象图，我们再结合下表来加深大家关于"稀疏矩阵"的印象：




![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/licvxR9ib9M6AvECce69uqxAGjO2tFu95JwRSQibooSA48r8eibQyfsGnUByzvyVZb5IOynYeicibO3eyFQQsRjJjia3A/640?)




看的出来：每一行中，列的组成都是灵活的，行与行之间并不需要遵循相同的列定义， 也就是HBase数据表"schema-less"的特点。

### Region




区别于Cassandra/DynamoDB的"Hash分区"设计，HBase中采用了"Range分区"，将Key的完整区间切割成一个个的"Key Range" ，每一个"Key Range"称之为一个Region。




也可以这么理解：将HBase中拥有数亿行的一个大表，横向切割成一个个"子表"，这一个个"子表"就是Region：

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/licvxR9ib9M6AvECce69uqxAGjO2tFu95JaGQREjtjRR7nnRsD19t6sFf5RUlR7alw6ztacjNU5tDbYsQcxuOKbA/640?)

Region是HBase中负载均衡的基本单元，当一个Region增长到一定大小以后，会自动分裂成两个。




### Column Family




如果将Region看成是一个表的横向切割，那么，一个Region中的数据列的纵向切割，称之为一个Column Family。每一个列，都必须归属于一个Column Family，这个归属关系是在写数据时指定的，而不是建表时预先定义。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/licvxR9ib9M6AvECce69uqxAGjO2tFu95JZzZNC0tZEsNgKY8x3Wt5x0AdHmSIjacvxBQX8DEkJBfjREUibibqdic2w/640?)

### KeyValue




KeyValue的设计不是源自Bigtable，而是要追溯至论文"The log-structured merge-tree(LSM-Tree)"。每一行中的每一列数据，都被包装成独立的拥有特定结构的KeyValue，KeyValue中包含了丰富的自我描述信息:

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/licvxR9ib9M6AvECce69uqxAGjO2tFu95JSNy7zECmCYtYYPSoj1ZPK7tVDHVia5mRKDyEj2eTUfuP3U8iar5u6HZw/640?)

看的出来，KeyValue是支撑"稀疏矩阵"设计的一个关键点：一些Key相同的任意数量的独立KeyValue就可以构成一行数据。但这种设计带来的一个显而易见的缺点：每一个KeyValue所携带的自我描述信息，会带来显著的数据膨胀。




适用场景




在介绍完了HBase的数据模型以后，我们可以回答本文一开始的前两个问题：




> - 
什么样的数据适合用HBase来存储？

- 
既然HBase也是一个数据库，能否用它将现有系统中昂贵的Oracle替换掉？






HBase的数据模型比较简单，数据按照RowKey排序存放，适合HBase存储的数据，可以简单总结如下：



- 
以实体为中心的数据

实体可以包括但不限于如下几种：
- 
自然人／账户／手机号／车辆相关数据

- 
用户画像数据（含标签类数据）

- 
图数据（关系类数据）



描述这些实体的，可以有基础属性信息、实体关系(图数据)、所发生的事件(如交易记录、车辆轨迹点)等等。



- 
以事件为中心的数据
- 
监控数据

- 
时序数据

- 
实时位置类数据

- 
消息/日志类数据






上面所描述的这些数据，有的是结构化数据，有的是半结构化或非结构化数据。HBase的“稀疏矩阵”设计，使其应对非结构化数据存储时能够得心应手，但在我们的实际用户场景中，结构化数据存储依然占据了比较重的比例。由于HBase仅提供了基于RowKey的单维度索引能力，在应对一些具体的场景时，依然还需要基于HBase之上构建一些专业的能力，如：



- 
OpenTSDB时序数据存储，提供基于Metrics+时间+标签的一些组合维度查询与聚合能力

- 
GeoMesa时空数据存储，提供基于时间+空间范围的索引能力

- 
JanusGraph图数据存储，提供基于属性、关系的图索引能力





HBase擅长于存储结构简单的海量数据但索引能力有限，而Oracle等传统关系型数据库(RDBMS)能够提供丰富的查询能力，但却疲于应对TB级别的海量数据存储，HBase对传统的RDBMS并不是取代关系，而是一种补充。




HBase与HDFS





我们都知道HBase的数据是存储于HDFS里面的，相信大家也都有这么的认知：




> 
HBase是一个分布式数据库，HDFS是一个分布式文件系统





理解了这一点，我们先来粗略回答本文已开始提出的其中两个问题：




> - 
HBase中的数据为何不直接存放于HDFS之上？


HBase中存储的海量数据记录，通常在几百Bytes到KB级别，如果将这些数据直接存储于HDFS之上，会导致大量的小文件产生，为HDFS的元数据管理节点(NameNode)带来沉重的压力。



- 
文件能否直接存储于HBase里面？


如果是几MB的文件，其实也可以直接存储于HBase里面，我们暂且将这类文件称之为小文件，HBase提供了一个名为MOB的特性来应对这类小文件的存储。但如果是更大的文件，强烈不建议用HBase来存储，关于这里更多的原因，希望你在详细读完本系列文章所有内容之后能够自己解答。





集群角色







> 
关于集群环境，你可以使用国内外大数据厂商的平台，如Cloudera，Hontonworks以及国内的华为，都发行了自己的企业版大数据平台，另外，华为云、阿里云中也均推出了全托管式的HBase服务。





我们假设集群环境已经Ready了，先来看一下集群中的关键角色：

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/licvxR9ib9M6AvECce69uqxAGjO2tFu95JHgpTicaT3tFMw0TkUIw9NgibnHv4dibickJ9JYsTGRsbpst2CO79JAaHGQ/640?)

相信大部分人对这些角色都已经有了一定程度的了解，我们快速的介绍一下各个角色在集群中的主要职责：



- 
ZooKeeper

在一个拥有多个节点的分布式系统中，假设，只能有一个节点是主节点，如何快速的选举出一个主节点而且让所有的节点都认可这个主节点？这就是HBase集群中存在的一个最基础命题。

利用ZooKeeper就可以非常简单的实现这类"仲裁"需求，ZooKeeper还提供了基础的事件通知机制，所有的数据都以 ZNode的形式存在，它也称得上是一个"微型数据库"。

- 
NameNode

HDFS作为一个分布式文件系统，自然需要文件目录树的元数据信息，另外，在HDFS中每一个文件都是按照Block存储的，文件与Block的关联也通过元数据信息来描述。NameNode提供了这些元数据信息的存储。

- 
DataNode

HDFS的数据存放节点。

- 
RegionServer

HBase的数据服务节点。

- 
Master

HBase的管理节点，通常在一个集群中设置一个主Master，一个备Master，主备角色的"仲裁"由ZooKeeper实现。 Master主要职责：

①负责管理所有的RegionServer。

②建表/修改表/删除表等DDL操作请求的服务端执行主体。

③管理所有的数据分片(Region)到RegionServer的分配。

④如果一个RegionServer宕机或进程故障，由Master负责将它原来所负责的Regions转移到其它的RegionServer上继续提供服务。

⑤Master自身也可以作为一个RegionServer提供服务，该能力是可配置的。





集群部署建议




如果基于物理机/虚拟机部署，通常建议：




1. RegionServer与DataNode联合部署，RegionServer与DataNode按1:1比例设置。




这种部署的优势在于，RegionServer中的数据文件可以存储一个副本于本机的DataNode节点中，从而在读取时可以利用HDFS中的"短路径读取(Short Circuit)"来绕过网络请求，降低读取时延。




![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/licvxR9ib9M6AvECce69uqxAGjO2tFu95JfUghsw54IPAck3f1TV6CqibP5EC5ibibVck6s6hfWibTjZtZjvZWSCtjhA/640?)




2. 管理节点独立于数据节点部署




如果是基于物理机部署，每一台物理机节点上可以设置几个RegionServers/DataNodes来提升资源使用率。




也可以选择基于容器来部署，如在HBaseCon Asia 2017大会知乎的演讲主题中，就提到了知乎基于Kubernetes部署HBase服务的实践。

对于公有云HBase服务而言，为了降低总体拥有成本(TCO)，通常选择"计算与存储物理分离"的方式，从架构上来说，可能导致平均时延略有下降，但可以借助于共享存储底层的IO优化来做一些"弥补"。




HBase集群中的RegionServers可以按逻辑划分为多个Groups，一个表可以与一个指定的Group绑定，可以将RegionServer Group理解成将一个大的集群划分成了多个逻辑子集群，借此可以实现多租户间的隔离，这就是HBase中的RegionServer Group特性。




示例数据




给出一份我们日常都可以接触到的数据样例，先简单给出示例数据的字段定义：



![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/pJE78AtczrlH9POIcXO5ssMNoibLXEQdzySszk5QJ3ia4Kh8A8gzayFMdLNYfbOnsYSCo06MnVV4A1CdYotK6diag/640?wx_fmt=jpeg)



本文力求简洁，仅给出了最简单的示例。如下是"虚构"的样例数据：






![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/pJE78AtczrlH9POIcXO5ssMNoibLXEQdzztPmSoTS9Co5I1ICdiaalUu77V6fLAyAG7zg66qVK89g9vXpc5VgS3w/640?wx_fmt=jpeg)







在本文大部分内容中所涉及的一条数据，是上面加粗的最后一行"Mobile1"为"13400006666"这行记录。




写数据之前：创建连接





### Login




在启用了安全特性的前提下，Login阶段是为了完成用户认证(确定用户的合法身份)，这是后续一切安全访问控制的基础。

当前Hadoop/HBase仅支持基于Kerberos的用户认证，ZooKeeper除了Kerberos认证，还能支持简单的用户名/密码认证，但都基于静态的配置，无法动态新增用户。如果要支持其它第三方认证，需要对现有的安全框架做出比较大的改动。




### 创建Connection




Connection可以理解为一个HBase集群连接的抽象，建议使用ConnectionFactory提供的工具方法来创建。因为HBase当前提供了两种连接模式：同步连接，异步连接，这两种连接模式下所创建的Connection也是不同的。我们给出ConnectionFactory中关于获取这两种连接的典型方法定义：




![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/licvxR9ib9M6AvECce69uqxAGjO2tFu95Jl0ic76Ywmf8NNFpg5LDlgGDbddWKLFicN9hia4ejia7kcav9DgqaTgNe7A/640?)




Connection中主要维护着两类共享的资源：



- 
线程池

- 
Socket连接





这些资源都是在真正使用的时候才会被创建，因此，此时的连接还只是一个"虚拟连接"。




写数据之前：创建数据表

### DDL操作的抽象接口 - Admin




Admin定义了常规的DDL接口，列举几个典型的接口：




![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/licvxR9ib9M6AvECce69uqxAGjO2tFu95JMwTyQMLuMxQPALNPZaAmrQ82yMicw80ztmW1gsEPgZCWicm6UWWWOlZA/640?)




预设合理的数据分片 - Region




分片数量会给读写吞吐量带来直接的影响，因此，建表时通常建议由用户主动指定划分Region分割点，来设定Region的数量。

HBase中数据是按照RowKey的字典顺序排列的，为了能够划分出合理的Region分割点，需要依据如下几点信息：



- 
Key的组成结构

- 
Key的数据分布预估

如果不能基于Key的组成结构来预估数据分布的话，可能会导致数据在Region间的分布不均匀

- 
读写并发度需求

依据读写并发度需求，设置合理的Region数量


### 为表定义合理的Schema




既然HBase号称"schema-less"的数据存储系统，那何来的是"schema "？的确，在数据库范式的支持上，HBase非常弱，这里的"schema"，主要指如下一些信息的设置：




1. NameSpace设置

2. Column Family的数量

3. 每一个Column Family中所关联的一些关键配置：

① Compression

HBase当前可以支持Snappy，GZ，LZO，LZ4，Bzip2以及ZSTD压缩算法

②DataBlock Encoding

HBase针对自身的特殊数据模型所做的一种压缩编码

③ BloomFilter

可用来协助快速判断一条记录是否存在

④ TTL

指定数据的过期时间

⑤StoragePolicy

指定Column Family的存储策略，可选项有："ALL_SSD"，"ONE_SSD"，"HOT"，"WARM"，"COLD"，"LAZY_PERSIST"




HBase中并不需要预先设置Column定义信息，这就是HBase schema less设计的核心。




Client发送建表请求到Master




建表的请求是通过RPC的方式由Client发送到Master:



- 
RPC接口基于Protocol Buffer定义

- 
建表相关的描述参数，也由Protocol Buffer进行定义及序列化





Client端侧调用了Master服务的什么接口，参数是什么，这些信息都被通过RPC通信传输到Master侧，Master再依据这些接口\参数描述信息决定要执行的操作。2.0版本中，HBase目前已经支持基于Netty的异步RPC框架。




> 
关于HBase RPC框架

早期的HBase RPC框架，完全借鉴了Hadoop中的实现，那时，Netty项目尚不盛行。





Master侧接收到Client侧的建表请求以后，一些主要操作包括：




1. 生成每一个Region的描述信息对象HRegionInfo，这些描述信息包括：Region ID, Region名称，Key范围，表名称等信息。

2. 生成每一个Region在HDFS中的文件目录。

3. 将HRegionInfo信息写入到记录元数据的hbase:meta表中。




> 
说明

meta表位于名为"hbase"的namespace中，因此，它的全称为"hbase:meta"。但在本系列文章范畴内，常将其缩写为"meta"。





整个过程中，新表的状态也是记录在hbase:meta表中的，而不用再存储在ZooKeeper中。




如果建表执行了一半，Master进程故障，如何处理？这里是由HBase自身提供的一个名为Procedure(V2)的框架来保障操作的事务性的，备Master接管服务以后，将会继续完成整个建表操作。




一个被创建成功的表，还可以被执行如下操作：



- 
Disable  将所有的Region下线，该表暂停读写服务

- 
Enable  将一个Disable过的表重新Enable，也就是上线所有的Region来正常提供读写服务

- 
Alter  更改表或列族的描述信息


Master分配Regions




新创建的所有的Regions，通过AssignmentManager将这些Region按照轮询(Round-Robin)的方式分配到每一个RegionServer中，具体的分配计划是由LoadBalancer来提供的。




AssignmentManager负责所有Regions的分配/迁移操作，Master中有一个定时运行的线程，来检查集群中的Regions在各个RegionServer之间的负载是否是均衡的，如果不均衡，则通过LoadBalancer生成相应的Region迁移计划，HBase中支持多种负载均衡算法，有最简单的仅考虑各RegionServer上的Regions数目的负载均衡算法，有基于迁移代价的负载均衡算法，也有数据本地化率优先的负载均衡算法，因为这一部分已经提供了插件化机制，用户也可以自定义负载均衡算法。




总结




本文主要介绍了如下内容：




1. HBase项目概述，呈现了HBase社区的活跃度以及搜索引擎热度等信息

2. HBase数据模型部分，讲到了RowKey，稀疏矩阵，Region，Column Family，KeyValue等概念

3. 基于HBase的数据模型，介绍了HBase的适合场景（以实体/事件为中心的简单结构的数据）

4. 介绍了HBase与HDFS的关系

5. 介绍了集群的关键角色：ZooKeeper, Master, RegionServer，NameNode, DataNode

6. 集群部署建议

7. 给出了一些示例数据

8. 写数据之前的准备工作：建立集群连接，建表（建表时应该定义合理的Schema以及设置合理的Region数量），建表由Master处理，新创建的Regions由Region AssignmentManager负责分配到各个RegionServer。









Hbase使用教程



## 1     基本介绍

## 1.1 前言

HBase – Hadoop Database，是一个分布式的、面向列的开源数据库，该技术来源于 Fay Chang 所撰写的Google论文“Bigtable：一个结构化数据的 [分布式存储系统](http://baike.baidu.com/view/1911305.htm) ”。就像Bigtable利用了Google文件系统（File System）所提供的分布式数据存储一样，HBase在Hadoop之上提供了类似于Bigtable的能力。HBase是Apache的Hadoop项目的子项目。

HBase不同于一般的关系数据库，它是一个适合于非结构化数据存储的数据库。另一个不同的是HBase基于列的而不是基于行的模式。

HBase是一个高 [可靠](http://baike.baidu.com/view/190360.htm) 性、高性能、面向列、可伸缩的 [分布式存储系统](http://baike.baidu.com/view/1911305.htm) ，利用HBase技术可在廉价PC Server上搭建起大规模 [结构](http://baike.baidu.com/view/160039.htm) 化 [存储](http://baike.baidu.com/view/87682.htm) 集群。

## 2     安装和使用

## 2.1 下载

HBase的官方网站http://www.apache.org/dyn/closer.cgi/hbase/上面可以下载到各种版本。目前用最新版本是0.98.2，建议下载stable目录下的稳定版本。

## 2.2 安装

安装依赖基础要求

1.   Linux操作系统

根据HBase的官方介绍，HBase没有在windows下测试过，因而，我们都是将HBase安装在Linux操作系统上。我本机安装的Ubuntu 12.04的虚拟机。

2.   Jdk

HBase需要jdk支持其运行，jdk版本要求是1.6及其以上。

这里暂且把Linux虚拟机的安装和虚拟机上jdk的安装过程跳过，可以参照网上其他相关资料执行。

HBase的安装方法比较简单，将我们下载的HBase的安装包hbase-0.94.20.tar.gz拷贝到Linux的根目录下。

接着执行以下命令和配置，之后启动HBase：

1.   解压缩安装包


root@ubuntu:/# tar xfz hbase-0.94.20.tar.gz

root@ubuntu:/# cd hbase-0.94.20


2.   配置数据存储目录

正如官方文档描述的那样，这时我们可以直接启动HBase，这样的话，使用的数据存储目录为 /tmp/hbase-${user.name}，也就意味着，我们一旦重启Linux，我们先前存储的数据就将丢失。

Linux下执行以下命令：


root@ubuntu:/# cd /hbase-0.94.20/conf/

root@ubuntu:/hbase-0.94.20/conf# vi hbase-site.xml


之后，修改配置文件内容为：


<?xml version="1.0"?>

<?xml-stylesheet type="text/xsl"href="configuration.xsl"?>

<configuration>

  <property>

   <name>hbase.rootdir</name>

    <value>file:///hbase_data/hbase</value>

  </property>

</configuration>


3.   启动HBase


root@ubuntu:/hbase-0.94.20/conf# ../bin/start-hbase.sh

starting master, logging to/hbase-0.94.20/bin/../logs/hbase-root-master-ubuntu.out


至此，单机模式启动HBase已经完成了。HBase的停止脚本是相同目录下的stop-hbase.sh。

## 2.3 HBase安装模式

在上一节中我们提到，我们安装的是单机模式。单机模式表示，我们所有的服务都运行在一个JVM上，包括HBase和Zookeeper。

另外，HBase还有两种安装模式：伪分布式模式和分布式模式。

伪分布式模式是把进程运行在一台机器上，但不是一个JVM。

完全分布式模式就是把整个服务被分布在各个节点上了 。

伪分布式模式和分布式模式依赖安装较多其他组件和服务，安装过程较为复杂，将会在另一篇文章中专门介绍。

## 3     开始一个例子

大多数技术人员happy的时候开始了。我们开始一个简单的Helloworld。

## 3.1 使用HBase shell连接HBase

使用HBase自带的客户端连接工具，连接到HBase：

![](https://img1.tuicool.com/ba2Ive.jpg!web)

## 3.2 创建User表

输入以下命令并执行：

![](https://img1.tuicool.com/vueqAj.jpg!web)

## 3.3 对User表简单地增删改查

往User表中插入一条信息：

![](https://img0.tuicool.com/yumemq.jpg!web)

查询刚才插入的信息：

![](https://img1.tuicool.com/RRr2m2.jpg!web)

## 3.4 检查数据存储目录

我们看一下之前我们配置的数据存储目录的变化：

![](https://img2.tuicool.com/NvaEvq.jpg!web)

我们可以看到，在之前配置的数据存储目录下，已经新添加了一些用于存储我们刚才存入的数据的文件了。

## 4     HBase基础定义和概念

## 4.1 表

HBase是一个数据库，数据以表的形式存储在Hbase中。

正如我们在hello world中定义中的User表类似，HBase的表的结构如下所示：
|Row Key|Time Stamp|ColumnFamily contents|ColumnFamily anchor|
|----|----|----|----|
|"com.cnn.www"|t9||anchor:cnnsi.com = "CNN"|
|"com.cnn.www"|t8||anchor:my.look.ca = "CNN.com"|
|"com.cnn.www"|t6|contents:html = "<html>..."||
|"com.cnn.www"|t5|contents:html = "<html>..."||
|"com.cnn.www"|t3|contents:html = "<html>..."||
4.2 行、列族、列
行以rowkey作为唯一标示。Rowkey是一段字节数组，这意味着，任何东西都可以保存进去，例如字符串、或者数字。行是按字典的排序由低到高存储在表中。

列族是列的集合。要准确表示一个列，需要“列族：列名”的方式。例如Hello world中的name列，应该被表示为“personalinfo:name”。

值得注意的是，列族被要求在创建表时指定，但列不需要，可以随时使用的时候创建。另外，一个列族的成员在文件系统上都存储在一起，因而列族中的所有列的存取方式都是一致的。HBase的存储优化就都针对列族级别，例如，我们可以考虑将经常需要一起取出来分析的信息，都存储在一个列族上。

## 5     HBase常用的操作

为了方便大家开发过程中快速查询，这里分类介绍最常见的HBase命令。HBase shell中支持的所有命令，可以通过help命令来列举出来。如下所示：

![](https://img0.tuicool.com/IvuEBz2.jpg!web)

这里只是截取了前部分命令，尚有部分命令不能再上图中显示。

## 5.1 一般命令

### 5.1.1 status

功能：查询服务器状态

使用：

![](https://img2.tuicool.com/Y32QFry.jpg!web)

### 5.1.2 version

功能：查询HBase版本信息

使用：

![](https://img2.tuicool.com/bIvEri.jpg!web)

### 5.1.3 whoami

功能：查看连接的用户

使用：

![](https://img1.tuicool.com/EVRniem.jpg!web)

## 5.2 DDL命令

### 5.2.1 Create创建表

功能：创建一个表。正如之前提到的，创建一个表时，不指定具体的列名，但要指定列族名。

使用：create ‘表名’,’列族名1’,’列族名2’

![](https://img2.tuicool.com/FBB3ui.jpg!web)

### 5.2.2 disable失效表

功能：失效一个表。当需要修改表结构、删除表时，需要先执行此命令。

使用：

![](https://img1.tuicool.com/fyiemm.jpg!web)

### 5.2.3 enable使失效表有效

功能：使表有效。在失效表以后，需要执行此命令，以使得表可用。

使用：

![](https://img2.tuicool.com/V7rmUj.jpg!web)

### 5.2.4 alter修改表结构

功能：修改表结构，包括新增列族、删除列族等

使用：

新增列族（记得在执行alter之前，要先disable表）

![](https://img1.tuicool.com/mAJjAv.jpg!web)

删除列族

![](https://img2.tuicool.com/r6Rjye.jpg!web)

重命名列族

列族不能被重命名。重命名一个列族的通常途径是使用API创建一个有着期望名称的新的列族，然后将数据复制过去，最后再删除旧的列族。

### 5.2.5 describe查看表结构

功能：查看表结构

使用：

![](https://img2.tuicool.com/neyYZb.jpg!web)

### 5.2.6 list列举数据库中的所有表

功能：查看数据库中所有的表

使用：

![](https://img1.tuicool.com/yM7Jrm.jpg!web)

### 5.2.7 drop删除表

功能：删除指定的表

使用：

![](https://img1.tuicool.com/UBRRV3.jpg!web)

## 5.3 DML命令

### 5.3.1 put插入数据

功能：插入一条数据到指定的表中。对于同一个rowkey，如果执行两次put，则第二次被认为是更新操作。

使用：put ‘表名’,’列族名1:列名1’,’值’

![](https://img2.tuicool.com/2INfEn.jpg!web)

### 5.3.2 get获取数据

功能：获取数据

使用：

获取指定rowkey的指定列族指定列的数据

![](https://img0.tuicool.com/7BfyAzF.jpg!web)

获取指定rowkey的指定列族所有的数据

![](https://img1.tuicool.com/fuaiIb.jpg!web)

获取指定rowkey的所有数据

![](https://img1.tuicool.com/Un2uIj.jpg!web)

获取指定时间戳的数据

![](https://img2.tuicool.com/euQZnu.jpg!web)

### 5.3.3 Count计算表的行数

功能：计算表的行数

使用：

![](https://img0.tuicool.com/2IfMzy.jpg!web)

### 5.3.4 put更新数据

详见5.3.1

### 5.3.5 scan全表扫描数据

功能：扫描全表所有数据

使用：

![](https://img2.tuicool.com/iURJnq.jpg!web)

### 5.3.6 delete删除数据

功能：删除表中的数据

使用：

删除指定rowkey的指定列族的列名的数据

![](https://img1.tuicool.com/JnEVJz.jpg!web)

删除指定rowkey的指定列族的数据

![](https://img2.tuicool.com/JbEZZr.jpg!web)

### 5.3.7 deleteall删除整行数据

功能：删除整行数据

使用：

![](https://img0.tuicool.com/mye6r2.jpg!web)

### 5.3.8 truncate删除全表数据

功能：删除表中所有的数据。正如你看到的，在HBase的help命令里并没有

使用：

![](https://img2.tuicool.com/Zjqme2.jpg!web)










