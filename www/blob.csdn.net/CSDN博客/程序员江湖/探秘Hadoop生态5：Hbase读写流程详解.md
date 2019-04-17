# 探秘Hadoop生态5：Hbase读写流程详解 - 程序员江湖 - CSDN博客





2017年03月08日 09:41:03[黄小斜](https://me.csdn.net/a724888)阅读数：3041
所属专栏：[后端技术杂谈](https://blog.csdn.net/column/details/25481.html)









# [一条数据的HBase之旅，简明HBase入门教程-Write全流程](https://www.cnblogs.com/qwangxiao/p/8794672.html)

如果将上篇内容理解为一个冗长的"铺垫"，那么，从本文开始，剧情才开始正式展开。本文基于提供的样例数据，介绍了写数据的接口，RowKey定义，数据在客户端的组装，数据路由，打包分发，以及RegionServer侧将数据写入到Region中的全部流程。

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p1.pstatp.com/large/pgc-image/1521894193852160438d014)

NoSQL漫谈

# 本文整体思路
- 
前文内容回顾

- 
示例数据

- 
HBase可选接口介绍

- 
表服务接口介绍

- 
介绍几种写数据的模式

- 
如何构建Put对象(包含RowKey定义以及列定义)

- 
数据路由

- 
Client侧的分组打包

- 
Client发RPC请求到RegionServer

- 
安全访问控制

- 
RegionServer侧处理：Region分发

- 
Region内部处理：写WAL

- 
Region内部处理：写MemStore


为了保证"故事"的完整性，导致本文篇幅过长，非常抱歉，读者可以按需跳过不感兴趣的内容。

# 前文回顾

上篇文章《一条数据的HBase之旅，简明HBase入门教程-开篇》主要介绍了如下内容：
- 
HBase项目概况(搜索引擎热度/社区开发活跃度)

- 
HBase数据模型(RowKey，稀疏矩阵，Region，Column Family，KeyValue)

- 
基于HBase的数据模型，介绍了HBase的适合场景（以实体/事件为中心的简单结构的数据）

- 
介绍了HBase与HDFS的关系，集群关键角色以及部署建议

- 
写数据前的准备工作：建立连接，建表


# 示例数据

（上篇文章已经提及，这里再复制一次的原因，一是为了让下文内容更容易理解，二是个别字段名称做了调整）

给出一份我们日常都可以接触到的数据样例，先简单给出示例数据的字段定义：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p1.pstatp.com/large/pgc-image/1521892691690ed87b0dc6b)

示例数据字段定义

本文力求简洁，仅给出了最简单的几个字段定义。如下是”虚构”的样例数据：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p1.pstatp.com/large/pgc-image/1521892692243dd04dcc117)

示例数据

在本文大部分内容中所涉及的一条数据，是上面加粗的最后一行"Mobile1"为"13400006666"这行记录。在下面的流程图中，我们使用下面这样一个红色小图标来表示该数据所在的位置：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p3.pstatp.com/large/pgc-image/1521892691584bafa925637)

数据位置标记

# 可选接口

HBase中提供了如下几种主要的接口：
- 
Java Client API

HBase的基础API，应用最为广泛。

- 
HBase Shell

基于Shell的命令行操作接口，基于Java Client API实现。

- 
Restful API

Rest Server侧基于Java Client API实现。

- 
Thrift API

Thrift Server侧基于Java Client API实现。

- 
MapReduce Based Batch Manipulation API

基于MapReduce的批量数据读写API。


除了上述主要的API，HBase还提供了基于Spark的批量操作接口以及C++ Client接口，但这两个特性都被规划在了3.0版本中，当前尚在开发中。

无论是HBase Shell/Restful API还是Thrift API，都是基于Java Client API实现的。因此，接下来关于流程的介绍，都是基于Java Client API的调用流程展开的。

# 关于表服务接口的抽象

同步连接与异步连接，分别提供了不同的表服务接口抽象：
- 
Table 同步连接中的表服务接口定义

- 
AsyncTable 异步连接中的表服务接口定义


异步连接AsyncConnection获取AsyncTable实例的接口默认实现：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p3.pstatp.com/large/pgc-image/152189329545282daf32a47)

Create AsyncTable

同步连接ClusterConnection的实现类ConnectionImplementation中获取Table实例的接口实现：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p9.pstatp.com/large/pgc-image/1521893339494cbe4c43aff)

Create Table

# 写数据的几种方式
- 
Single Put

单条记录单条记录的随机put操作。Single Put所对应的接口定义如下：

在AsyncTable接口中的定义：
`CompletableFuture<Void> put(Put put);`
在Table接口中的定义：
`void put(Put put) throws IOException;`
- 
Batch Put

汇聚了几十条甚至是几百上千条记录之后的小批次随机put操作。

Batch Put只是本文对该类型操作的称法，实际的接口名称如下所示：

在AsyncTable接口中的定义：
`List<CompletableFuture<Void>> put(List<Put> puts);`
在Table接口中的定义：
`void put(List<Put> puts) throws IOException;`
- 
Bulkload

基于MapReduce API提供的数据批量导入能力，导入数据量通常在GB级别以上，Bulkload能够绕过Java Client API直接生成HBase的底层数据文件(HFile)。


# 构建Put对象

设计合理的RowKey

RowKey通常是一个或若干个字段的直接组合或经一定处理后的信息，因为一个表中所有的数据都是基于RowKey排序的，RowKey的设计对读写都会有直接的性能影响。

我们基于本文的样例数据，先给出两种RowKey的设计，并简单讨论各自的优缺点：

样例数据：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p1.pstatp.com/large/pgc-image/1521892692243dd04dcc117)

示例数据

RowKey Format 1： Mobile1 + StartTime

为了方便读者理解，我们在两个字段之间添加了连接符”^”。如下是RowKey以及相关排序结果：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p3.pstatp.com/large/pgc-image/152189269170657e3ba27c8)

RowKey Format 1

RowKey Format 2： StartTime + Mobile1

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p3.pstatp.com/large/pgc-image/152189269170517688dc3fa)

RowKey Format 2

从上面两个表格可以看出来，不同的字段组合顺序设计，带来截然不同的排序结果，我们将RowKey中的第一个字段称之为“先导字段”。第一种设计，有利于查询”手机号码XXX的在某时间范围内的数据记录”，但不利于查询”某段时间范围内有哪些手机号码拨出了电话？”，而第二种设计却恰好相反。

上面是两种设计都是两个字段的直接组合，这种设计在实际应用中，会带来读写热点问题，难以保障数据读写请求在所有Regions之间的负载均衡。避免热点的常见方法有如下几种：

Reversing

如果先导字段本身会带来热点问题，但该字段尾部的信息却具备良好的随机性，此时，可以考虑将先导字段做反转处理，将尾部几位直接提前到前面，或者直接将整个字段完全反转。

将先导字段Mobile1翻转后，就具备非常好的随机性。

例如：

13400001111^201803010800

将先导字段Mobile1反转后的RowKey变为：

11110000431^201803010800

Salting

Salting的原理是在RowKey的前面添加固定长度的随机Bytes，随机Bytes能保障数据在所有Regions间的负载均衡。

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p3.pstatp.com/large/pgc-image/1521892692958859ae6d101)

RowKey With Salting

Salting能很好的保障写入时将数据均匀分散到各个Region中，但对于读取却是不友好的，例如，如果读取Mobile1为”13400001111″在20180301这一天的数据记录时，因为Salting Bytes信息是随机选择添加的，查询时并不知道前面添加的Salting Bytes是”A”，因此{“A”, “B”, “C”}所关联的Regions都得去查看一下。

Hashing

Hashing是将一个RowKey通过一个Hash函数生成一组固定长度的bytes，Hash函数能保障所生成的随机bytes具备良好的离散度，从而也能够均匀打散到各个Region中。Hashing既有利于随机写入，又利于基于知道RowKey各字段的确切信息之后的随机读取操作，但如果是基于RowKey范围的Scan或者是RowKey的模糊信息进行查询的话，就会带来显著的性能问题，因为原来在字典顺序相邻的RowKey列表，通过Hashing打散后导致这些数据被分散到了多个Region中。

因此，RowKey的设计，需要充分考虑业务的读写特点。

本文内容假设RowKey设计：reversing(Mobile1) +StartTime

也就是说，RowKey由反转处理后的Mobile1与StartTime组成。对于我们所关注的这行数据：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p3.pstatp.com/large/pgc-image/15218926933622287604be7)

关注的数据记录

RowKey应该为： 66660000431^201803011300

因为创建表时预设的Region与RowKey强相关，我们现在才可以给出本文样例所需要创建的表的”Region分割点“信息：

假设，Region分割点为“1,2,3,4,5,6,7,8,9”，基于这9个分割点，可以预先创建10个Region，这10个Region的StartKey和StopKey如下所示：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p1.pstatp.com/large/pgc-image/152189269287467f4490b58)

Region划分信息
- 
第一个Region的StartKey为空，最后一个Region的StopKey为空

- 
每一个Region区间，都包含StartKey本身，但不包含StopKey

- 
由于Mobile1字段的最后一位是0~9之间的随机数字，因此，可以均匀打散到这10个Region中


# 定义列

每一个列在HBase中体现为一个KeyValue，而每一个KeyValue拥有特定的组成结构，这一点在上一篇文章的数据模型章节已经提到过。

所谓的定义列，就是需要定义出每一个列要存放的列族(Column Family)以及列标识(Qualifier)信息。

我们假设，存放样例数据的这个表名称为”TelRecords” ，为了简单起见，仅仅设置了1个名为”I”的列族。

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p1.pstatp.com/large/pgc-image/1521892692452fbffccb31f)

Column Family以及列标识定义

因为Mobile1与StartTime都已经被包含在RowKey中，所以，不需要再在列中存储一份。关于列族名称与列标识名称，建议应该简短一些，因为这些信息都会被包含在KeyValue里面，过长的名称会导致数据膨胀。

基于RowKey和列定义信息，就可以组建HBase的Put对象，一个Put对象用来描述待写入的一行数据，一个Put可以理解成与某个RowKey关联的1个或多个KeyValue的集合。

至此，这条数据已经转变成了Put对象，如下图所示：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p1.pstatp.com/large/pgc-image/15218936468202d2ba7eea0)

Put

# 数据路由

初始化ZooKeeper Session

因为meta Region存放于ZooKeeper中，在第一次从ZooKeeper中读取META Region的地址时，需要先初始化一个ZooKeeper Session。ZooKeeper Session是ZooKeeper Client与ZooKeeper Server端所建立的一个会话，通过心跳机制保持长连接。

获取Region路由信息

通过前面建立的连接，从ZooKeeper中读取meta Region所在的RegionServer，这个读取流程，当前已经是异步的。获取了meta Region的路由信息以后，再从meta Region中定位要读写的RowKey所关联的Region信息。如下图所示：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p1.pstatp.com/large/pgc-image/15218926937472890130879)

Region Routing

因为每一个用户表Region都是一个RowKey Range，meta Region中记录了每一个用户表Region的路由以及状态信息，以RegionName(包含表名，Region StartKey，Region ID，副本ID等信息)作为RowKey。基于一条用户数据RowKey，快速查询该RowKey所属的Region的方法其实很简单：只需要基于表名以及该用户数据RowKey，构建一个虚拟的Region Key，然后通过Reverse Scan的方式，读到的第一条Region记录就是该数据所关联的Region。如下图所示：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p3.pstatp.com/large/pgc-image/1521892695296b1b33c00ea)

Location User Region

Region只要不被迁移，那么获取的该Region的路由信息就是一直有效的，因此，HBase Client有一个Cache机制来缓存Region的路由信息，避免每次读写都要去访问ZooKeeper或者meta Region。

> 
进阶内容1：meta Region究竟在哪里？

meta Region的路由信息存放在ZooKeeper中，但meta Region究竟在哪个RegionServer中提供读写服务？

在1.0版本中，引入了一个新特性，使得Master可以”兼任”一个RegionServer角色(可参考HBASE-5487, HBASE-10569)，从而可以将一些系统表的Region分配到Master的这个RegionServer中，这种设计的初衷是为了简化/优化Region Assign的流程，但这依然带来了一系列复杂的问题，尤其是Master初始化和RegionServer初始化之间的Race，因此，在2.0版本中将这个特性暂时关闭了。详细信息可以参考：HBASE-16367，HBASE-18511，HBASE-19694，HBASE-19785，HBASE-19828


# 客户端侧的数据分组“打包”

如果这条待写入的数据采用的是Single Put的方式，那么，该步骤可以略过（事实上，单条Put操作的流程相对简单，就是先定位该RowKey所对应的Region以及RegionServer信息后，Client直接发送写请求到RegionServer侧即可）。

但如果这条数据被混杂在其它的数据列表中，采用Batch Put的方式，那么，客户端在将所有的数据写到对应的RegionServer之前，会先分组”打包”，流程如下：
- 
按Region分组：遍历每一条数据的RowKey，然后，依据meta表中记录的Region信息，确定每一条数据所属的Region。此步骤可以获取到Region到RowKey列表的映射关系。

- 
按RegionServer”打包”：因为Region一定归属于某一个RegionServer（注：本文内容中如无特殊说明，都未考虑Region Replica特性），那属于同一个RegionServer的多个Regions的写入请求，被打包成一个MultiAction对象，这样可以一并发送到每一个RegionServer中。


![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p1.pstatp.com/large/pgc-image/152189373724025942eacb1)

数据分组与打包

# Client发送写数据请求到RegionServer

类似于Client发送建表到Master的流程，Client发送写数据请求到RegionServer，也是通过RPC的方式。只是，Client到Master以及Client到RegionServer，采用了不同的RPC服务接口。

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p1.pstatp.com/large/pgc-image/15218926943380ab68bc0b4)

Client Send Request To RegionServer

single put请求与batch put请求，两者所调用的RPC服务接口方法是不同的，如下是Client.proto中的定义：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p3.pstatp.com/large/pgc-image/15218937851044f18b96944)

Client Proto定义

# 安全访问控制

如何保障UserA只能写数据到UserA的表中，以及禁止UserA改写其它User的表的数据，HBase提供了ACL机制。ACL通常需要与Kerberos认证配合一起使用，Kerberos能够确保一个用户的合法性，而ACL确保该用户仅能执行权限范围内的操作。

HBase将权限分为如下几类：
- 
READ(‘R’)

- 
WRITE(‘W’)

- 
EXEC(‘X’)

- 
CREATE(‘C’)

- 
ADMIN(‘A’)


可以为一个用户/用户组定义整库级别的权限集合，也可以定义Namespace、表、列族甚至是列级别的权限集合。

# RegionServer端处理：Region分发

RegionServer的RPC Server侧，接收到来自Client端的RPC请求以后，将该请求交给Handler线程处理。

如果是single put，则该步骤比较简单，因为在发送过来的请求参数MutateRequest中，已经携带了这条记录所关联的Region，那么直接将该请求转发给对应的Region即可。

如果是batch puts，则接收到的请求参数为MultiRequest，在MultiRequest中，混合了这个RegionServer所持有的多个Region的写入请求，每一个Region的写入请求都被包装成了一个RegionAction对象。RegionServer接收到MultiRequest请求以后，遍历所有的RegionAction，而后写入到每一个Region中，此过程是串行的:

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p9.pstatp.com/large/pgc-image/1521893818377870dca9599)

Write Per Region

从这里可以看出来，并不是一个batch越大越好，大的batch size甚至可能导致吞吐量下降。

# Region内部处理：写WAL

HBase也采用了LSM-Tree的架构设计：LSM-Tree利用了传统机械硬盘的“顺序读写速度远高于随机读写速度”的特点。随机写入的数据，如果直接去改写每一个Region上的数据文件，那么吞吐量是非常差的。因此，每一个Region中随机写入的数据，都暂时先缓存在内存中(HBase中存放这部分内存数据的模块称之为MemStore，这里仅仅引出概念，下一章节详细介绍)，为了保障数据可靠性，将这些随机写入的数据顺序写入到一个称之为WAL(Write-Ahead-Log)的日志文件中，WAL中的数据按时间顺序组织：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p3.pstatp.com/large/pgc-image/152189386163085da58b57b)

MemStore And WAL

如果位于内存中的数据尚未持久化，而且突然遇到了机器断电，只需要将WAL中的数据回放到Region中即可:

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p1.pstatp.com/large/pgc-image/152189388650404ff90fe90)

WAL Replay

在HBase中，默认一个RegionServer只有一个可写的WAL文件。WAL中写入的记录，以Entry为基本单元，而一个Entry中，包含：
- 
WALKey 包含{Encoded Region Name，www.365soke.cn  Table Name，www.hbs90.cn Sequence ID，Timestamp}等关键信息，其中，Sequence ID在维持数据一致性方面起到了关键作用，可以理解为一个事务ID。

- 
WALEdit WALEdit中直接保存待写入数据的所有的KeyValues，www.boshenyl.cn  而这些KeyValues可能来自一个Region中的多行数据。


也就是说，通常，一个Region中的一个batch put请求，会被组装成一个Entry，写入到WAL中：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p3.pstatp.com/large/pgc-image/15218939416076742076116)

Write into WAL

将Entry写到文件中时是支持压缩的，但该特性默认未开启。

# WAL进阶内容

WAL Roll and Archive

当正在写的WAL文件达到一定大小以后，会创建一个新的WAL文件，上一个WAL文件依然需要被保留，因为这个WAL文件中所关联的Region中的数据，尚未被持久化存储，因此，该WAL可能会被用来回放数据。

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p9.pstatp.com/large/pgc-image/1521893984553cc3544aa39)

Roll WAL

如果一个WAL中所关联的所有的Region中的数据，都已经被持久化存储了，那么，这个WAL文件会被暂时归档到另外一个目录中：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p3.pstatp.com/large/pgc-image/15218940064321094236f76)

WAL Archive

注意，这里不是直接将WAL文件删除掉，这是一种稳妥且合理的做法，原因如下：
- 
避免因为逻辑实现上的问题导致WAL被误删，暂时归档到另外一个目录，为错误发现预留了一定的时间窗口

- 
按时间维度组织的WAL数据文件还可以被用于其它用途，如增量备份，跨集群容灾等等，因此，这些WAL文件通常不允许直接被删除，至于何时可以被清理，还需要额外的控制逻辑


另外，如果对写入HBase中的数据的可靠性要求不高，那么，HBase允许通过配置跳过写WAL操作。

> 
思考：put与batch put的性能为何差别巨大？

在网络分发上，batch put已经具备一定的优势，因为batch put是打包分发的。

而从写WAL这块，看的出来，www.taohuayuan178.com batch put写入的一小批次Put对象，可以通过一次sync就持久化到WAL文件中了，有效减少了IOPS。

但前面也提到了，batch size并不是越大越好，因为每一个batch在RegionServer端是被串行处理的。


利用Disruptor提升写并发性能

在高并发随机写入场景下，会带来大量的WAL Sync操作，HBase中采用了Disruptor的RingBuffer来减少竞争，思路是这样：如果将瞬间并发写入WAL中的数据，合并执行Sync操作，可以有效降低Sync操作的次数，来提升写吞吐量。

Multi-WAL

默认情形下，一个RegionServer只有一个被写入的WAL Writer，尽管WAL Writer依靠顺序写提升写吞吐量，在基于普通机械硬盘的配置下，此时只能有单块盘发挥作用，其它盘的IOPS能力并没有被充分利用起来，这是Multi-WAL设计的初衷。Multi-WAL可以在一个RegionServer中同时启动几个WAL Writer，可按照一定的策略，将一个Region与其中某一个WAL Writer绑定，这样可以充分发挥多块盘的性能优势。

关于WAL的未来

WAL是基于机械硬盘的IO模型设计的，而对于新兴的非易失性介质，如3D XPoint，WAL未来可能会失去存在的意义，关于这部分内容，请参考文章《从HBase中移除WAL？3D XPoint技术带来的变革》。

# Region内部处理：写MemStore

每一个Column Family，在Region内部被抽象为了一个HStore对象，而每一个HStore拥有自身的MemStore，用来缓存一批最近被随机写入的数据，这是LSM-Tree核心设计的一部分。

MemStore中用来存放所有的KeyValue的数据结构，称之为CellSet，而CellSet的核心是一个ConcurrentSkipListMap，我们知道，ConcurrentSkipListMap是Java的跳表实现，数据按照Key值有序存放，而且在高并发写入时，性能远高于ConcurrentHashMap。

因此，写MemStore的过程，事实上是将batch put提交过来的所有的KeyValue列表，写入到MemStore的以ConcurrentSkipListMap为组成核心的CellSet中：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p1.pstatp.com/large/pgc-image/15218940464954288f80897)

Write Into MemStore

MemStore因为涉及到大量的随机写入操作，会带来大量Java小对象的创建与消亡，会导致大量的内存碎片，给GC带来比较重的压力，HBase为了优化这里的机制，借鉴了操作系统的内存分页的技术，增加了一个名为MSLab的特性，通过分配一些固定大小的Chunk，来存储MemStore中的数据，这样可以有效减少内存碎片问题，降低GC的压力。当然，ConcurrentSkipListMap本身也会创建大量的对象，这里也有很大的优化空间，去年阿里的一篇文章透露了阿里如何通过优化ConcurrentSkipListMap的结构来有效减少GC时间。

> 
进阶内容2：先写WAL还是先写MemStore?

在0.94版本之前，Region中的写入顺序是先写WAL再写MemStore，这与WAL的定义也相符。

但在0.94版本中，将这两者的顺序颠倒了，当时颠倒的初衷，是为了使得行锁能够在WAL sync之前先释放，从而可以提升针对单行数据的更新性能。详细问题单，请参考HBASE-4528。

在2.0版本中，这一行为又被改回去了，原因在于修改了行锁机制以后(下面章节将讲到)，发现了一些性能下降，而HBASE-4528中的优化却无法再发挥作用，详情请参考HBASE-15158。改动之后的逻辑也更简洁了。


> 
进阶内容3：关于行级别的ACID

在之前的版本中，行级别的任何并发写入/更新都是互斥的，由一个行锁控制。但在2.0版本中，这一点行为发生了变化，多个线程可以同时更新一行数据，这里的考虑点为：
- 
如果多个线程写入同一行的不同列族，是不需要互斥的

- 
多个线程写同一行的相同列族，也不需要互斥，即使是写相同的列，也完全可以通过HBase的MVCC机制来控制数据的一致性

- 
当然，CAS操作(如checkAndPut)或increment操作，依然需要独占的行锁


更多详细信息，可以参考HBASE-12751。


至此，这条数据已经被同时成功写到了WAL以及MemStore中：

![一条数据的HBase之旅，简明HBase入门教程-Write全流程](http://p3.pstatp.com/large/pgc-image/152189407088168954805b2)

Data Written In HBase

# 总结

本文主要内容总结如下：
- 
介绍HBase写数据可选接口以及接口定义。

- 
通过一个样例，介绍了RowKey定义以及列定义的一些方法，以及如何组装Put对象

- 
数据路由，数据分发、打包，以及Client通过RPC发送写数据请求至RegionServer

- 
RegionServer接收数据以后，将数据写到每一个Region中。写数据流程先写WAL再写MemStore，这里展开了一些技术细节

- 
简单介绍了HBase权限控制模型


需要说明的一点，本文所讲到的MemStore其实是一种"简化"后的模型，在2.0版本中，这里已经变的更加复杂，这些内容将在下一篇介绍Flush与Compaction的流程中详细介绍。







这是《一条数据的HBase之旅》系列文章的第4篇，介绍HBase Get/Scan的核心实现思路。

系列文章

在阅读本文之前，希望你已经读过本系列文章的前面几篇内容：**开篇内容**介绍HBase的数据模型、适用场景、集群关键角色、建表流程以及所涉及的HBase基础概念。**Writer全流程**介绍了写数据的接口，RowKey定义，数据在客户端的组装，数据路由，打包分发，以及RegionServer侧将数据写入到Region中的全部流程。**Flush与Compaction**阐述了Flush与Compaction流程，讲述了Compaction所面临的本质问题，介绍了HBase现有的几种Compaction策略以及各自的适用场景。

本文思路

**1.介绍HBase的两种读取模式：Get与Scan**

如何发起一次Get请求，Get有哪些关键参数

如何发起一次Scan请求，Scan有哪些关键参数

**2.Client如何发送请求到对应的RegionServer**

**3.RegionServer侧如何处理一次读取请求**

关于Scan的命题定义

如何处理Get请求

合理组织所有的"KeyValue数据源"

读取KeyValue的基础Scanner接口

RegionScanner的初始化

通过next请求读取一行行数据

**4.本文内容总结，并列出了关于Scan流程的更多细节问题**

**硬广植入：关于公有云HBase服务**点击本文末尾处的"**阅读原文**"链接，可了解**华为云**上的**全托管式HBase服务CloudTable**，目前已集成了**时序数据库****OpenTSDB**与**时空数据库****GeoMesa**。

HBase的两种读取模式

**Get**

Get是指基于确切的RowKey去获取一行数据，通常被称之为**随机点查**，这正是HBase所擅长的读取模式。一次Get操作，包含两个主要步骤：

1.构建Get

基于RowKey构建Get对象的最简单示例代码如下：

可以为构建的Get对象指定返回的列族：

![](http://t12.baidu.com/it/u=2283916718,1529889208&fm=173&app=25&f=JPG?w=640&h=135&s=0070E4324D624D205E5521DA000080B2)

也可以直接指定返回某列族中的指定列：

![](http://t11.baidu.com/it/u=2226856123,200254633&fm=173&app=25&f=JPG?w=639&h=160&s=85706532C5644D224EF521DA000080B2)

2.发送Get请求并且获取对应的记录

与写数据类似，发送Get请求的接口也是由Table提供的，获取到的一行记录，被封装成一个Result对象。也可以这么理解一个Result对象：

- 关联一行数据，一定不可能包含跨行的结果

- 包含一个或多个被请求的列(可能包含所有列，也可能仅包含部分列)

示例代码如下：

![](http://t10.baidu.com/it/u=1536674601,639474352&fm=173&app=25&f=JPG?w=640&h=353&s=05B0E532118A45490E75E4DA0000C0B2)

上面给出的是一次随机获取一行记录的例子，但事实上，一次获取多行记录的需求也是普遍存在的，Table中也定义了Batch Get的接口，这样可以在一次网络请求中同时获取多行数据。示例代码如下：

![](http://t12.baidu.com/it/u=2220994271,2339948197&fm=173&app=25&f=JPG?w=640&h=575&s=8070E53213BFC1CE4CFD70DE0000C0B2)

关于Batch Get需要补充说明一点信息：获取到的Result列表中的结果的顺序，与给定的RowKey顺序是一致的。

**Scan**

HBase中的数据表通过划分成一个个的Region来实现数据的分片，每一个Region关联一个RowKey的范围区间，而每一个Region中的数据，按RowKey的字典顺序进行组织。

正是基于这种设计，使得HBase能够轻松应对这类查询："指定一个RowKey的范围区间，获取该区间的所有记录"， 这类查询在HBase被称之为Scan。

一次Scan操作，包括如下几个关键步骤：

1.构建Scan

最简单也最常用的构建Scan对象的方法，就是仅仅指定Scan的**StartRow**与**StopRow**。示例如下：

![](http://t12.baidu.com/it/u=1100565521,4293251215&fm=173&app=25&f=JPG?w=640&h=304&s=A57065321B4A4549085DA4DA0000C0B3)

如果StartRow未指定，则本次Scan将从表的第一行数据开始读取。

如果StopRow未指定，而且在不主动停止本次Scan操作的前提下，本次Scan将会一直读取到表的最后一行记录。



![](http://t10.baidu.com/it/u=2988498042,1501992141&fm=173&app=25&f=JPG?w=640&h=371&s=C8A03C7297DE45C85E5D90DE000050B1)



如果StartRow与StopRow都未指定，那本次Scan就是一次全表扫描操作。

同Get类似，Scan也可以**主动指定返回的列族或列**:

![](http://t10.baidu.com/it/u=3694233891,1092309985&fm=173&app=25&f=JPG?w=640&h=265&s=857265321FC84C431ED5C5DA000050B2)

2.获取ResultScanner

3.遍历查询结果

![](http://t10.baidu.com/it/u=3488717334,906400371&fm=173&app=25&f=JPG?w=640&h=209&s=8172653287744C224CE565DE000080B2)

4.关闭ResultScanner

通过下面的方法可以关闭一个ResultScanner:

如果基于Java传统的try-catch-finally语法，上述close方式需要在finally模块显式调用。但如果是是基于try-with-resource语法，则由Java框架自动调用。

将上面1~4步骤联合起来的示例代码如下：

![](http://t12.baidu.com/it/u=14428460,814575584&fm=173&app=25&f=JPG?w=640&h=414&s=81706D32118EC54D0AF585DA0000C0B1)

Scan的其它重要参数

**a) Caching: 设置一次RPC请求批量读取的Results数量**

下面的示例代码设定了一次读取回来的Results数量为100：

scan.setCaching(100);Client每一次往RegionServer发送scan请求，都会批量拿回一批数据(由Caching决定过了每一次拿回的Results数量)，然后放到本次的Result Cache中：

![](http://t11.baidu.com/it/u=2576618722,169399618&fm=173&app=25&f=JPG?w=640&h=291&s=5BD4E60ACDF5788A58D9997103004071)



应用每一次读取数据时，都是从本地的Result Cache中获取的。如果Result Cache中的数据读完了，则Client会再次往RegionServer发送scan请求获取更多的数据。

**b) Batch: 设置每一个Result中的列的数量**

下面的示例代码设定了每一个Result中的列的数量的限制值为3：

scan.setBatch(3);该参数适用于一行数据过大的场景，这样，一行数据被请求的列会被拆成多个Results返回给Client。

**举例说明如下：**

假设一行数据中共有十个列：{Col01，Col02，Col03，Col04，Col05，Col06，Col07，Col08，Col09, Col10} 假设Scan中设置的Batch为3，那么，这一行数据将会被拆成4个Results返回：

Result1 -> {Col01，Col02，Col03}

Result2 -> {Col04，Col05，Col06}

Result3 -> {Col07，Col08，Col09}

Result4 -> {Col10}

关于Caching参数，我们说明了是Client每一次从RegionServer侧获取到的Results的数量，上例中，一行数据被拆成了4个Results，这将会导致Caching中的计数器被减了4次。结合Caching与Batch，我们再列举一个稍复杂的例子：

假设，Scan的参数设置如下：

final byte[] start = Bytes.toBytes("Row1");final byte[] stop = Bytes.toBytes("Row5");Scan scan = new Scan();scan.withStartRow(start).withStopRow(stop);scan.setCaching(10);scan.setBatch(3);待读取的数据RowKey与所关联的列集如下所示：

**Row1**: {Col01，Col02，Col03，Col04，Col05，Col06，Col07，Col08，Col09，Col10} **Row2**: {Col01，Col02，Col03，Col04，Col05，Col06，Col07，Col08，Col09，Col10，Col11} **Row3**: {Col01，Col02，Col03，Col04，Col05，Col06，Col07，Col08，Col09，Col10}

再回顾一下Caching与Batch的定义：

**Caching**: 影响一次读取返回的Results数量。

**Batch**: 限定了一个Result中所包含的列的数量，如果一行数据被请求的列的数量超出Batch限制，那么这行数据会被拆成多个Results。

那么， Client往RegionServer第一次请求所返回的结果集如下所示：

Result1 -> Row1: {Col01，Col02，Col03}

Result2 -> Row1: {Col04，Col05，Col06}

Result3 -> Row1: {Col07，Col08，Col09}

Result4 -> Row1: {Col10}

Result5 -> Row2: {Col01，Col02，Col03}

Result6 -> Row2: {Col04，Col05，Col06}

Result7 -> Row2: {Col07，Col08，Col09}

Result8 -> Row2: {Col10，Col11}

Result9 -> Row3: {Col01，Col02，Col03}

Result10 -> Row3: {Col04，Col05，Col06}

**c) Limit: 限制一次Scan操作所获取的行的数量**

同SQL语法中的limit子句，限制一次Scan操作所获取的行的总量：

scan.setLimit(10000);**注意：**Limit参数是在2.0版本中新引入的。但在2.0.0版本中，当Batch与Limit同时设置时，似乎还存在一个BUG，初步分析问题原因应该与BatchScanResultCache中的numberOfCompletedRows计数器逻辑处理有关。因此，暂时不建议同时设置这两个参数。

**d) CacheBlock: RegionServer侧是否要缓存本次Scan所涉及的HFileBlocks**scan.setCacheBlocks(true);**e) Raw Scan: 是否可以读取到删除标识以及被删除但尚未被清理的数据**

scan.setRaw(true);**f) MaxResultSize: 从内存占用量的维度限制一次Scan的返回结果集**

下面的示例代码将返回结果集的最大值设置为5MB：

scan.setMaxResultSize(5 * 1024 * 1024);**g) Reversed Scan: 反向扫描**

普通的Scan操作是按照字典顺序从小到大的顺序读取的，而Reversed Scan则恰好相反：

scan.setReversed(true);**h) 带Filter的Scan**

Filter可以在Scan的结果集基础之上，对返回的记录设置更多条件值，这些条件可以与RowKey有关，可以与列名有关，也可以与列值有关，还可以将多个Filter条件组合在一起，等等。

最常用的Filter是SingleColumnValueFilter，基于它，可以实现如下类似的查询：

"返回满足条件{列I:D的值大于等于10}的所有行"

示例代码如下：

![](http://t11.baidu.com/it/u=3627502744,1630473609&fm=173&app=25&f=JPG?w=640&h=167&s=84F26532DF624D220C7DE5DA000010B2)

Filter丰富了HBase的查询能力，但使用Filter之前，需要注意一点：Filter可能会导致查询响应时延变的不可控制。**因为我们无法预测，为了找到一条符合条件的记录，背后需要扫描多少数据量**，如果在有效限制了Scan范围区间(通过设置StartRow与StopRow限制)的前提下，该问题能够得到有效的控制。这些信息都要求使用Filter之前应该详细调研自己的业务数据模型。

Client发送读取请求到RegionServer

无论是Get，还是Scan，Client在发送请求到RegionServer之前，也需要先获取路由信息：

**1.定位该请求所关联的Region**

因为Get请求仅关联一个RowKey，所以，直接定位该RowKey所关联的Region即可。对于Scan请求，先定位Scan的StartRow所关联的Region。

**2.往RegionServer发送读取请求**

该过程与《一条数据的HBase之旅，简明HBase入门教程 - Write全流程》的"数据路由"章节所描述的流程类似，不再赘述。

如果一次Scan涉及到跨Region的读取，读完一个Region的数据以后，需要继续读取下一个Region的数据，这需要在Client侧不断记录和刷新Scan的进展信息。如果一个Region中已无更多的数据，在scan请求的响应结果中会带有提示信息，这样可以让Client侧切换到下一个Region继续读取。

RegionServer如何处理读取请求

关于Read的命题

通过前面的文章我们已经了解了如下信息：

**1.一个表可能包含一个或多个Region**

将HBase中拥有数亿行的一个大表，**横向切割**成一个个"**子表**"，这一个个"**子表**"就是**Region**。

![](http://t12.baidu.com/it/u=217271552,153452792&fm=173&app=25&f=JPG?w=640&h=285&s=9EA87C2393B84588446DC0D700009033)



**2.每一个Region中关联一个或多个列族**

如果将Region看成是一个表的**横向切割**，那么，一个Region中的数据列的**纵向切割**，称之为一个**Column Family**。每一个列，都必须归属于一个Column Family，这个归属关系是在写数据时指定的，而不是建表时预先定义。

![](http://t12.baidu.com/it/u=539761502,2874194498&fm=173&app=25&f=JPG?w=639&h=252&s=613AA0739C685D095265046B0300B072)



**3.每一个列族关联一个MemStore，以及一个或多个HFiles文件**

上面的关于“Region与多列族”的图中，泛化了Column Family的内部结构。下图是包含MemStore与HFile的Column Family组成结构：

![](http://t12.baidu.com/it/u=3513783008,578106265&fm=173&app=25&f=JPG?w=640&h=367&s=1B2C742211CE4CEA12EDAD7900004073)



HFile数据文件存在于底层的HDFS中，上图中只是为了方便阐述HFile与Column Family之间的关系。

在HBase的源码实现中，将一个Column Family抽象成一个Store对象。可以这么简单理解Column Family与Store的概念差异：Column Family更多的是面向用户层可感知的逻辑概念，而Store则是源码实现中的概念，是关于一个Column Family的抽象。

**4.每一个MemStore中可能涉及一个Active Segment，以及一个或多个Immutable Segments**

![](http://t12.baidu.com/it/u=3889206521,77538979&fm=173&app=25&f=JPG?w=640&h=352&s=123CE6221E885EC80556C0660200D063)



扩展到一个Region包含两个Column Family的情形：

![](http://t12.baidu.com/it/u=840951311,436012947&fm=173&app=25&f=JPG?w=640&h=299&s=121CE62217304D845650517202005073)



**5.HFile由Block构成，默认地，用户数据被按序组织成一个个64KB的Block**

HFile V1的结构虽已过时，但非常有助于你理解HFile的核心设计思想：

![](http://t12.baidu.com/it/u=632165252,4022730357&fm=173&app=25&f=JPEG?w=559&h=164&s=E7E8B053CCE9D90160E8E1DE0300D0B3)



**- Data Block**(上图中左侧的Data块)：保存了实际的KeyValue数据。

**- Data Index**：关于Data Block的索引信息。

HFile V2只不过在HFile V1基础上做的演进，将Data Index信息以及BloomFilter的数据也分成了多层。

当前阶段，你只需要了解到：**基于一个给定的RowKey，HFile中提供的索引信息能够快速查询到对应的Data Block**。



在重新温习了上述内容以后，我们也大致了解了关于HBase读取我们所面临的问题是什么。关于HBase Read的命题可以定义为：如何从1个或多个列族(1个或多个MemStore Segments+1个或多个HFiles)所构成的Region中读取用户所期望的数据？这些数据默认必须是**未被标记删除**的、**未过期**的而且是**最新版本**的数据。

将Get看作一类特殊的Scan

无论是读取一行数据，还是读取指定RowKey范围的读取一系列数据，所面临的问题其实是类似的，因此，可以将Get看作是一种特殊的Scan，只不过它的StartRow与StopRow重叠，事实上，RegionServer侧处理Get请求时的确先将Get先转换成了一个Scan操作。

合理组织所有的KeyValue数据源

在Store/Column Family内部，KeyValue可能存在于MemStore的Segment中，也可能存在于HFile文件中，无论是Segment还是HFile，我们统称为**KeyValue数据源**。

在本文的第一部分介绍如何执行Scan操作时，我们讲到了Client侧使用一个**ResultScanner**来抽象地描述一次Scan操作，ResultScanner屏蔽掉了往RegionServer发送请求以及一个Region读取完成以后切换到下一个Region等细节信息。

初次阅读RegionServer/Region的读取流程所涉及的源码时，会被各色各样的Scanner类整的晕头转向，HBase使用了各种Scanner来抽象每一层/每一类KeyValue数据源的Scan操作：

- 关于一个Region的读取，被封装成一个RegionScanner对象。

- 每一个Store/Column Family的读取操作，被封装在一个StoreScanner对象中。

- SegmentScanner与StoreFileScanner分别用来描述关于MemStore中的Segment以及HFile的读取操作。

- StoreFileScanner中关于HFile的实际读取操作，由HFileScanner完成。

RegionScanner的构成如下图所示：

![](http://t11.baidu.com/it/u=1225929859,609378391&fm=173&app=25&f=JPG?w=639&h=349&s=1A2076221FA8648E4B71AD700000C073)



在StoreScanner内部，多个SegmentScanner与多个StoreFileScanner被组织在一个称之为KeyValueHeap的对象中：

![](http://t10.baidu.com/it/u=1045175574,1882223833&fm=173&app=25&f=JPG?w=640&h=257&s=5B54E6029EF01E885A17CB6E0300F06F)



每一个Scanner内部有一个指针指向当前要读取的KeyValue，KeyValueHeap的核心是一个**优先级队列**(**PriorityQueue**)，在这个PriorityQueue中，按照每一个Scanner当前指针所指向的KeyValue进行排序：

![](http://t10.baidu.com/it/u=4032268815,3429847031&fm=173&app=25&f=JPG?w=639&h=195&s=85726D3297744C215EFD01DB000080B2)

同样的，RegionScanner中的多个StoreScanner，也被组织在一个KeyValueHeap对象中：

![](http://t10.baidu.com/it/u=3593993529,4196481930&fm=173&app=25&f=JPG?w=640&h=188&s=4AAC3C628D206D0358D411CA0000E0B1)



KeyValueScanner接口

KeyValueScanner定义了读取KeyValue的基础接口：

![](http://t10.baidu.com/it/u=2412247461,4092950561&fm=173&app=25&f=JPG?w=640&h=778&s=8CA87432193EC5CC5E7D45DA0000D0B2)

实现了KeyValueScanner接口类的主要Scanner包括：

StoreFileScannerSegmentScannerStoreScannerRegionScanner初始化

RegionScanner初始化过程，包括几个关键操作：

**1.获取ReadPoint**

ReadPoint决定了此次Scan操作能看到哪些数据。Scan过程中新写入的数据，对此次Scan是不可见的。

**2.按需选择对应的Store，并初始化对应的StoreScanner**

StoreScanner在初始化的时候，也会按需选择对应的SegmentScanner以及StoreFileScanner，筛选规则包括：

- 如果一次Scan操作指定了Time Range，则只选择与该Time Range有关的Scanners。

- 对于Get操作，可以通过BloomFilter过滤掉不符合条件的Scanners。

StoreScanner中筛选除了Scanner以后，会将每一个Scanner seek到Scan的StartRow位置：

![](http://t12.baidu.com/it/u=3502292069,1341029486&fm=173&app=25&f=JPG?w=640&h=299&s=9232EE228EDC45CA5CDC7D5A02003070)



通过next请求读取一个个KeyValue

如果将RegionScanner理解成一个内部构造复杂的机器，而驱动这个机器运转的动力源自Client侧的一次次scan请求，scan请求通过调用RegionScanner的next方法来获取一个个KeyValue。

为了简单的解释该流程，我们先假定一个RegionScanner中仅包含一个StoreScanner，那么，这个RegionScanner中的核心读取操作，是由StoreScanner完成的，我们进一步假定StoreScanner由4个Scanners组成（我们泛化了SegmentScanner与StoreFileScanner的区别，统称为Scanner），直观起见，在下图中我们使用了四种不同的颜色（ScannerA~ScannerD为随机名称，请忽略它们在名称上的顺序）：

每一个Scanner中都有一个current指针指向下一个即将要读取的KeyValue，**KeyValueHeap中的PriorityQueue正是按照每一个Scanner的current所指向的KeyValue进行排序**。

第一次next请求，将会返回ScannerA中的**Row01:FamA:Col1**，而后ScannerA的指针移动到下一个KeyValue **Row01:FamA:Col2**，PriorityQueue中的Scanners排序依然不变：



第二次next请求，依然返回ScannerA中的**Row01:FamA:Col2**，ScannerA的指针移动到下一个KeyValue Row02:FamA:Col1，此时，PriorityQueue中的Scanners排序发生了变化：



下一次next请求，将会返回ScannerB中的KeyValue.....周而复始，直到某一个Scanner所读取的数据耗尽，该Scanner将会被close，不再出现在上面的PriorityQueue中。

SegmentScanner/StoreFileScanner中返回的KeyValue，包含了各种类型的KeyValue：

已被更新过的旧KeyValue已被标记删除但尚未被及时清理的KeyValue已过期的尚未被及时清理的KeyValue用来描述一次删除操作的KeyValue(删除还包含了多种类型)承载最新用户数据的普通KeyValue因此，在StoreScanner层，需要对这些KeyValue做更复杂的逻辑校验，这些校验由**ScanQueryMatcher**完成。**默认地**，可作为返回数据的KeyValue，应该满足如下条件：

KeyValue类型为PutKeyValue所关联的列为用户Scan所涉及的列KeyValue的时间戳符合Scan的TimeRange要求版本最新未被标记删除通过了Filter的过滤条件上述条件，只针对一些普通的Scan，不同的Scan参数配置，可能会导致条件集发生变化，如Scan启用了Raw Scan模式时，Delete类型的KeyValue也会被返回。另外，上面的这些条件所罗列的顺序，也未遵循实际的检查顺序，而实际的检查顺序也是严格的，如果颠倒就可能会导致Bug。小米的同学就曾发现了这样的一个Bug:

假设某一个列共有T1~T5五个版本， ColumnFamily中设置的MaxVersions为3(即最大允许保留的版本数)T5 -> Value=5 T4 -> Value=4 T3 -> Value=3 T2 -> Value=2 T1 -> Value=1如果Scan中采用了一个SingleColumnValueFilter，要求返回满足Value<=3的所有结果。因为MaxVersions为3，我们**所期望的返回结果**应该为：T5 -> Value=5 （Value不满足条件）T4 -> Value=4 （Value不满足条件） T3 -> Value=3T2 -> Value=2 （Version不满足条件）T1 -> Value=1 （Version不满足条件）关于多版本检查以及Filter检查，这里有两种可能的顺序：**Opt 1**：先检查Filter，再检查多版本。这种情况下的返回结果为：T5 -> Value=5 （Value不满足条件）T4 -> Value=4 （Value不满足条件） T3 -> Value=3 T2 -> Value=2 T1 -> Value=1 这种情况的返回结果就是错误的。**Opt 2**: 先检查多版本，再检查Filter。这种情况下的返回结果才是预期的。



在Scanner中，如果允许读取多个版本（由Scan#readVersions配置），那正常的读取顺序应该为：

![](http://t11.baidu.com/it/u=4202344471,190308071&fm=173&app=25&f=JPG?w=640&h=531&s=DA203C62F2237BA972E525DB0000A0A1)



上面这种读取的顺序与实际存在的数据的逻辑顺序也是相同的。

由于不同的Scan所读取的每一行中的数据不同，有的限定了列的数量，有的限定了版本的数量，这使得读取时可以通过一些优化，减少不必要的数据扫描。如某次Scan在允许读多个版本的同时，限定了只读取C1~C3，那么，读取顺序应该为：

![](http://t10.baidu.com/it/u=3426600870,583086292&fm=173&app=25&f=JPG?w=639&h=531&s=DA283C62F2235AA972E525DB0000A0A1)



最普通的Scan，其实只需要读取每一列的最新版本即可，那读取的顺序应该为：



通过上面几张图，我们其实是想说明在Scanner内部需要具备这样的一些基础能力：

- 如果只需要当前列的最新版本，那么Scanner应该可以跳过当前列的其它版本，而且将指针移到下一列的开始位置。

- 如果当前行的所要读取的列都已读完，那么，Scanner应该可以跳过该行剩余的列，将指针移动到下一行的开始位置。

我们知道KeyValueScanner定义了基础的seek/reseek/requestSeek等接口，可以将指针移动到指定KeyValue位置。但关于指针如何移动的决策信息，由谁来提供？

这些信息也是由ScanQueryMatcher提供的。ScanQueryMatcher对每一个KeyValue的逻辑检查结果称之为MatchCode，MatchCode不仅包含了是否应该返回该KeyValue的结果，还可能给出了Scanner的下一步操作的提示信息。关于它的枚举值，简单举例如下：

**INCLUDE_AND_SEEK_NEXT_ROW**

包含当前KeyValue，并提示Scanner当前行已无需继续读取，请Seek到下一行。

**INCLUDE_AND_SEEK_NEXT_COL**

包含当前KeyValue，并提示Scanner当前列已无需继续读取，请Seek到下一列。

无论是StoreScanner还是RegionScanner，返回的都是符合条件的KeyValue列表。这些KeyValues在RSRpcServices层被进一步组装成Results响应给Client侧。

总结

Scan涉及了太多的细节内容，本文只粗略介绍了Scan的一些核心思路，这与本系列文章最初的定位有关，当然也受限于本文的篇幅。 本文主要介绍了如下内容：

**介绍HBase的两种读取模式：Get与Scan**

1.Client如何发起一次Get请求，Get的关键参数

2.Client如何发起一次Scan请求，Scan的关键参数

**重点介绍了RegionServer侧关于Scan的处理流程：**

1.如何用Scanner来抽象描述关于Region的读取操作

2.关于读取KeyValue的基础Scanner接口定义 3.RegionScanner初始化时的关键操作

4.Client侧的一次次scan请求如何驱动RegionScanner内部的读取操作

5.从StoreFileScanner/SegmentScanner中读取出来的原始KeyValue如何被合理的校验

6.Scanner读取时如何跳过一些不必要的数据



关于Scan的更多细节，感兴趣的同学可以自己去源码中探寻答案：

1.如果第一次scan请求不能取回所有的数据，下一次scan如何快速有效继承上一次的进度？2.Get/Small Scan/Large Scan在实现上有哪些本质的区别？

3.ScanQueryMatcher中校验KeyValue的详细逻辑以及校验的顺序

4.关于Filter涉及多步校验，每一步校验是在什么地方完成的？

5.MinVersion与MaxVersion的定义是什么？

6.ScanQueryMatcher中关于多种删除类型的语义是如何定义的？

7.如何限制一次Scan所占用的内存大小以及执行的时间？

8.BloomFilter在Get/Scan流程中是如何被应用的？

9.Scan过程中如果正在读取的HFile文件被Compaction合并了，如何处理？

10.正在Scan的Region突然被迁移到其它的RegionServer中，如何继续原来的进度继续读取？

11.Reverse Scan与普通Scan在实现上有何不同？



HFile的内容在本文只粗略提及，在RegionServer侧的处理流程中，关于BlockCache部分更是只字未提。本文将重点放在介绍Scan的核心思路上。**下篇文章将介绍HFile的核心原理。**

本文由百家号作者上传并发布，百家号仅提供信息发布平这是《一条数据的HBase之旅》系列文章的第4篇，介绍HBase Get/Scan的核心实现思路。

系列文章

在阅读本文之前，希望你已经读过本系列文章的前面几篇内容：**开篇内容**介绍HBase的数据模型、适用场景、集群关键角色、建表流程以及所涉及的HBase基础概念。**Writer全流程**介绍了写数据的接口，RowKey定义，数据在客户端的组装，数据路由，打包分发，以及RegionServer侧将数据写入到Region中的全部流程。**Flush与Compaction**阐述了Flush与Compaction流程，讲述了Compaction所面临的本质问题，介绍了HBase现有的几种Compaction策略以及各自的适用场景。

本文思路

**1.介绍HBase的两种读取模式：Get与Scan**

如何发起一次Get请求，Get有哪些关键参数

如何发起一次Scan请求，Scan有哪些关键参数

**2.Client如何发送请求到对应的RegionServer**

**3.RegionServer侧如何处理一次读取请求**

关于Scan的命题定义

如何处理Get请求

合理组织所有的"KeyValue数据源"

读取KeyValue的基础Scanner接口

RegionScanner的初始化

通过next请求读取一行行数据

**4.本文内容总结，并列出了关于Scan流程的更多细节问题**

**硬广植入：关于公有云HBase服务**点击本文末尾处的"**阅读原文**"链接，可了解**华为云**上的**全托管式HBase服务CloudTable**，目前已集成了**时序数据库****OpenTSDB**与**时空数据库****GeoMesa**。

HBase的两种读取模式

**Get**

Get是指基于确切的RowKey去获取一行数据，通常被称之为**随机点查**，这正是HBase所擅长的读取模式。一次Get操作，包含两个主要步骤：

1.构建Get

基于RowKey构建Get对象的最简单示例代码如下：

可以为构建的Get对象指定返回的列族：

![](http://t12.baidu.com/it/u=2283916718,1529889208&fm=173&app=25&f=JPG?w=640&h=135&s=0070E4324D624D205E5521DA000080B2)

也可以直接指定返回某列族中的指定列：

![](http://t11.baidu.com/it/u=2226856123,200254633&fm=173&app=25&f=JPG?w=639&h=160&s=85706532C5644D224EF521DA000080B2)

2.发送Get请求并且获取对应的记录

与写数据类似，发送Get请求的接口也是由Table提供的，获取到的一行记录，被封装成一个Result对象。也可以这么理解一个Result对象：

- 关联一行数据，一定不可能包含跨行的结果

- 包含一个或多个被请求的列(可能包含所有列，也可能仅包含部分列)

示例代码如下：

![](http://t10.baidu.com/it/u=1536674601,639474352&fm=173&app=25&f=JPG?w=640&h=353&s=05B0E532118A45490E75E4DA0000C0B2)

上面给出的是一次随机获取一行记录的例子，但事实上，一次获取多行记录的需求也是普遍存在的，Table中也定义了Batch Get的接口，这样可以在一次网络请求中同时获取多行数据。示例代码如下：

![](http://t12.baidu.com/it/u=2220994271,2339948197&fm=173&app=25&f=JPG?w=640&h=575&s=8070E53213BFC1CE4CFD70DE0000C0B2)

关于Batch Get需要补充说明一点信息：获取到的Result列表中的结果的顺序，与给定的RowKey顺序是一致的。

**Scan**

HBase中的数据表通过划分成一个个的Region来实现数据的分片，每一个Region关联一个RowKey的范围区间，而每一个Region中的数据，按RowKey的字典顺序进行组织。

正是基于这种设计，使得HBase能够轻松应对这类查询："指定一个RowKey的范围区间，获取该区间的所有记录"， 这类查询在HBase被称之为Scan。

一次Scan操作，包括如下几个关键步骤：

1.构建Scan

最简单也最常用的构建Scan对象的方法，就是仅仅指定Scan的**StartRow**与**StopRow**。示例如下：

![](http://t12.baidu.com/it/u=1100565521,4293251215&fm=173&app=25&f=JPG?w=640&h=304&s=A57065321B4A4549085DA4DA0000C0B3)

如果StartRow未指定，则本次Scan将从表的第一行数据开始读取。

如果StopRow未指定，而且在不主动停止本次Scan操作的前提下，本次Scan将会一直读取到表的最后一行记录。



![](http://t10.baidu.com/it/u=2988498042,1501992141&fm=173&app=25&f=JPG?w=640&h=371&s=C8A03C7297DE45C85E5D90DE000050B1)



如果StartRow与StopRow都未指定，那本次Scan就是一次全表扫描操作。

同Get类似，Scan也可以**主动指定返回的列族或列**:

![](http://t10.baidu.com/it/u=3694233891,1092309985&fm=173&app=25&f=JPG?w=640&h=265&s=857265321FC84C431ED5C5DA000050B2)

2.获取ResultScanner

3.遍历查询结果

![](http://t10.baidu.com/it/u=3488717334,906400371&fm=173&app=25&f=JPG?w=640&h=209&s=8172653287744C224CE565DE000080B2)

4.关闭ResultScanner

通过下面的方法可以关闭一个ResultScanner:

如果基于Java传统的try-catch-finally语法，上述close方式需要在finally模块显式调用。但如果是是基于try-with-resource语法，则由Java框架自动调用。

将上面1~4步骤联合起来的示例代码如下：

![](http://t12.baidu.com/it/u=14428460,814575584&fm=173&app=25&f=JPG?w=640&h=414&s=81706D32118EC54D0AF585DA0000C0B1)

Scan的其它重要参数

**a) Caching: 设置一次RPC请求批量读取的Results数量**

下面的示例代码设定了一次读取回来的Results数量为100：

scan.setCaching(100);Client每一次往RegionServer发送scan请求，都会批量拿回一批数据(由Caching决定过了每一次拿回的Results数量)，然后放到本次的Result Cache中：

![](http://t11.baidu.com/it/u=2576618722,169399618&fm=173&app=25&f=JPG?w=640&h=291&s=5BD4E60ACDF5788A58D9997103004071)



应用每一次读取数据时，都是从本地的Result Cache中获取的。如果Result Cache中的数据读完了，则Client会再次往RegionServer发送scan请求获取更多的数据。

**b) Batch: 设置每一个Result中的列的数量**

下面的示例代码设定了每一个Result中的列的数量的限制值为3：

scan.setBatch(3);该参数适用于一行数据过大的场景，这样，一行数据被请求的列会被拆成多个Results返回给Client。

**举例说明如下：**

假设一行数据中共有十个列：{Col01，Col02，Col03，Col04，Col05，Col06，Col07，Col08，Col09, Col10} 假设Scan中设置的Batch为3，那么，这一行数据将会被拆成4个Results返回：

Result1 -> {Col01，Col02，Col03}

Result2 -> {Col04，Col05，Col06}

Result3 -> {Col07，Col08，Col09}

Result4 -> {Col10}

关于Caching参数，我们说明了是Client每一次从RegionServer侧获取到的Results的数量，上例中，一行数据被拆成了4个Results，这将会导致Caching中的计数器被减了4次。结合Caching与Batch，我们再列举一个稍复杂的例子：

假设，Scan的参数设置如下：

final byte[] start = Bytes.toBytes("Row1");final byte[] stop = Bytes.toBytes("Row5");Scan scan = new Scan();scan.withStartRow(start).withStopRow(stop);scan.setCaching(10);scan.setBatch(3);待读取的数据RowKey与所关联的列集如下所示：

**Row1**: {Col01，Col02，Col03，Col04，Col05，Col06，Col07，Col08，Col09，Col10} **Row2**: {Col01，Col02，Col03，Col04，Col05，Col06，Col07，Col08，Col09，Col10，Col11} **Row3**: {Col01，Col02，Col03，Col04，Col05，Col06，Col07，Col08，Col09，Col10}

再回顾一下Caching与Batch的定义：

**Caching**: 影响一次读取返回的Results数量。

**Batch**: 限定了一个Result中所包含的列的数量，如果一行数据被请求的列的数量超出Batch限制，那么这行数据会被拆成多个Results。

那么， Client往RegionServer第一次请求所返回的结果集如下所示：

Result1 -> Row1: {Col01，Col02，Col03}

Result2 -> Row1: {Col04，Col05，Col06}

Result3 -> Row1: {Col07，Col08，Col09}

Result4 -> Row1: {Col10}

Result5 -> Row2: {Col01，Col02，Col03}

Result6 -> Row2: {Col04，Col05，Col06}

Result7 -> Row2: {Col07，Col08，Col09}

Result8 -> Row2: {Col10，Col11}

Result9 -> Row3: {Col01，Col02，Col03}

Result10 -> Row3: {Col04，Col05，Col06}

**c) Limit: 限制一次Scan操作所获取的行的数量**

同SQL语法中的limit子句，限制一次Scan操作所获取的行的总量：

scan.setLimit(10000);**注意：**Limit参数是在2.0版本中新引入的。但在2.0.0版本中，当Batch与Limit同时设置时，似乎还存在一个BUG，初步分析问题原因应该与BatchScanResultCache中的numberOfCompletedRows计数器逻辑处理有关。因此，暂时不建议同时设置这两个参数。

**d) CacheBlock: RegionServer侧是否要缓存本次Scan所涉及的HFileBlocks**scan.setCacheBlocks(true);**e) Raw Scan: 是否可以读取到删除标识以及被删除但尚未被清理的数据**

scan.setRaw(true);**f) MaxResultSize: 从内存占用量的维度限制一次Scan的返回结果集**

下面的示例代码将返回结果集的最大值设置为5MB：

scan.setMaxResultSize(5 * 1024 * 1024);**g) Reversed Scan: 反向扫描**

普通的Scan操作是按照字典顺序从小到大的顺序读取的，而Reversed Scan则恰好相反：

scan.setReversed(true);**h) 带Filter的Scan**

Filter可以在Scan的结果集基础之上，对返回的记录设置更多条件值，这些条件可以与RowKey有关，可以与列名有关，也可以与列值有关，还可以将多个Filter条件组合在一起，等等。

最常用的Filter是SingleColumnValueFilter，基于它，可以实现如下类似的查询：

"返回满足条件{列I:D的值大于等于10}的所有行"

示例代码如下：

![](http://t11.baidu.com/it/u=3627502744,1630473609&fm=173&app=25&f=JPG?w=640&h=167&s=84F26532DF624D220C7DE5DA000010B2)

Filter丰富了HBase的查询能力，但使用Filter之前，需要注意一点：Filter可能会导致查询响应时延变的不可控制。**因为我们无法预测，为了找到一条符合条件的记录，背后需要扫描多少数据量**，如果在有效限制了Scan范围区间(通过设置StartRow与StopRow限制)的前提下，该问题能够得到有效的控制。这些信息都要求使用Filter之前应该详细调研自己的业务数据模型。

Client发送读取请求到RegionServer

无论是Get，还是Scan，Client在发送请求到RegionServer之前，也需要先获取路由信息：

**1.定位该请求所关联的Region**

因为Get请求仅关联一个RowKey，所以，直接定位该RowKey所关联的Region即可。对于Scan请求，先定位Scan的StartRow所关联的Region。

**2.往RegionServer发送读取请求**

该过程与《一条数据的HBase之旅，简明HBase入门教程 - Write全流程》的"数据路由"章节所描述的流程类似，不再赘述。

如果一次Scan涉及到跨Region的读取，读完一个Region的数据以后，需要继续读取下一个Region的数据，这需要在Client侧不断记录和刷新Scan的进展信息。如果一个Region中已无更多的数据，在scan请求的响应结果中会带有提示信息，这样可以让Client侧切换到下一个Region继续读取。

RegionServer如何处理读取请求

关于Read的命题

通过前面的文章我们已经了解了如下信息：

**1.一个表可能包含一个或多个Region**

将HBase中拥有数亿行的一个大表，**横向切割**成一个个"**子表**"，这一个个"**子表**"就是**Region**。

![](http://t12.baidu.com/it/u=217271552,153452792&fm=173&app=25&f=JPG?w=640&h=285&s=9EA87C2393B84588446DC0D700009033)



**2.每一个Region中关联一个或多个列族**

如果将Region看成是一个表的**横向切割**，那么，一个Region中的数据列的**纵向切割**，称之为一个**Column Family**。每一个列，都必须归属于一个Column Family，这个归属关系是在写数据时指定的，而不是建表时预先定义。

![](http://t12.baidu.com/it/u=539761502,2874194498&fm=173&app=25&f=JPG?w=639&h=252&s=613AA0739C685D095265046B0300B072)



**3.每一个列族关联一个MemStore，以及一个或多个HFiles文件**

上面的关于“Region与多列族”的图中，泛化了Column Family的内部结构。下图是包含MemStore与HFile的Column Family组成结构：

![](http://t12.baidu.com/it/u=3513783008,578106265&fm=173&app=25&f=JPG?w=640&h=367&s=1B2C742211CE4CEA12EDAD7900004073)



HFile数据文件存在于底层的HDFS中，上图中只是为了方便阐述HFile与Column Family之间的关系。

在HBase的源码实现中，将一个Column Family抽象成一个Store对象。可以这么简单理解Column Family与Store的概念差异：Column Family更多的是面向用户层可感知的逻辑概念，而Store则是源码实现中的概念，是关于一个Column Family的抽象。

**4.每一个MemStore中可能涉及一个Active Segment，以及一个或多个Immutable Segments**

![](http://t12.baidu.com/it/u=3889206521,77538979&fm=173&app=25&f=JPG?w=640&h=352&s=123CE6221E885EC80556C0660200D063)



扩展到一个Region包含两个Column Family的情形：

![](http://t12.baidu.com/it/u=840951311,436012947&fm=173&app=25&f=JPG?w=640&h=299&s=121CE62217304D845650517202005073)



**5.HFile由Block构成，默认地，用户数据被按序组织成一个个64KB的Block**

HFile V1的结构虽已过时，但非常有助于你理解HFile的核心设计思想：

![](http://t12.baidu.com/it/u=632165252,4022730357&fm=173&app=25&f=JPEG?w=559&h=164&s=E7E8B053CCE9D90160E8E1DE0300D0B3)



**- Data Block**(上图中左侧的Data块)：保存了实际的KeyValue数据。

**- Data Index**：关于Data Block的索引信息。

HFile V2只不过在HFile V1基础上做的演进，将Data Index信息以及BloomFilter的数据也分成了多层。

当前阶段，你只需要了解到：**基于一个给定的RowKey，HFile中提供的索引信息能够快速查询到对应的Data Block**。



在重新温习了上述内容以后，我们也大致了解了关于HBase读取我们所面临的问题是什么。关于HBase Read的命题可以定义为：如何从1个或多个列族(1个或多个MemStore Segments+1个或多个HFiles)所构成的Region中读取用户所期望的数据？这些数据默认必须是**未被标记删除**的、**未过期**的而且是**最新版本**的数据。

将Get看作一类特殊的Scan

无论是读取一行数据，还是读取指定RowKey范围的读取一系列数据，所面临的问题其实是类似的，因此，可以将Get看作是一种特殊的Scan，只不过它的StartRow与StopRow重叠，事实上，RegionServer侧处理Get请求时的确先将Get先转换成了一个Scan操作。

合理组织所有的KeyValue数据源

在Store/Column Family内部，KeyValue可能存在于MemStore的Segment中，也可能存在于HFile文件中，无论是Segment还是HFile，我们统称为**KeyValue数据源**。

在本文的第一部分介绍如何执行Scan操作时，我们讲到了Client侧使用一个**ResultScanner**来抽象地描述一次Scan操作，ResultScanner屏蔽掉了往RegionServer发送请求以及一个Region读取完成以后切换到下一个Region等细节信息。

初次阅读RegionServer/Region的读取流程所涉及的源码时，会被各色各样的Scanner类整的晕头转向，HBase使用了各种Scanner来抽象每一层/每一类KeyValue数据源的Scan操作：

- 关于一个Region的读取，被封装成一个RegionScanner对象。

- 每一个Store/Column Family的读取操作，被封装在一个StoreScanner对象中。

- SegmentScanner与StoreFileScanner分别用来描述关于MemStore中的Segment以及HFile的读取操作。

- StoreFileScanner中关于HFile的实际读取操作，由HFileScanner完成。

RegionScanner的构成如下图所示：

![](http://t11.baidu.com/it/u=1225929859,609378391&fm=173&app=25&f=JPG?w=639&h=349&s=1A2076221FA8648E4B71AD700000C073)



在StoreScanner内部，多个SegmentScanner与多个StoreFileScanner被组织在一个称之为KeyValueHeap的对象中：

![](http://t10.baidu.com/it/u=1045175574,1882223833&fm=173&app=25&f=JPG?w=640&h=257&s=5B54E6029EF01E885A17CB6E0300F06F)



每一个Scanner内部有一个指针指向当前要读取的KeyValue，KeyValueHeap的核心是一个**优先级队列**(**PriorityQueue**)，在这个PriorityQueue中，按照每一个Scanner当前指针所指向的KeyValue进行排序：

![](http://t10.baidu.com/it/u=4032268815,3429847031&fm=173&app=25&f=JPG?w=639&h=195&s=85726D3297744C215EFD01DB000080B2)

同样的，RegionScanner中的多个StoreScanner，也被组织在一个KeyValueHeap对象中：

![](http://t10.baidu.com/it/u=3593993529,4196481930&fm=173&app=25&f=JPG?w=640&h=188&s=4AAC3C628D206D0358D411CA0000E0B1)



KeyValueScanner接口

KeyValueScanner定义了读取KeyValue的基础接口：

![](http://t10.baidu.com/it/u=2412247461,4092950561&fm=173&app=25&f=JPG?w=640&h=778&s=8CA87432193EC5CC5E7D45DA0000D0B2)

实现了KeyValueScanner接口类的主要Scanner包括：

StoreFileScannerSegmentScannerStoreScannerRegionScanner初始化

RegionScanner初始化过程，包括几个关键操作：

**1.获取ReadPoint**

ReadPoint决定了此次Scan操作能看到哪些数据。Scan过程中新写入的数据，对此次Scan是不可见的。

**2.按需选择对应的Store，并初始化对应的StoreScanner**

StoreScanner在初始化的时候，也会按需选择对应的SegmentScanner以及StoreFileScanner，筛选规则包括：

- 如果一次Scan操作指定了Time Range，则只选择与该Time Range有关的Scanners。

- 对于Get操作，可以通过BloomFilter过滤掉不符合条件的Scanners。

StoreScanner中筛选除了Scanner以后，会将每一个Scanner seek到Scan的StartRow位置：

![](http://t12.baidu.com/it/u=3502292069,1341029486&fm=173&app=25&f=JPG?w=640&h=299&s=9232EE228EDC45CA5CDC7D5A02003070)



通过next请求读取一个个KeyValue

如果将RegionScanner理解成一个内部构造复杂的机器，而驱动这个机器运转的动力源自Client侧的一次次scan请求，scan请求通过调用RegionScanner的next方法来获取一个个KeyValue。

为了简单的解释该流程，我们先假定一个RegionScanner中仅包含一个StoreScanner，那么，这个RegionScanner中的核心读取操作，是由StoreScanner完成的，我们进一步假定StoreScanner由4个Scanners组成（我们泛化了SegmentScanner与StoreFileScanner的区别，统称为Scanner），直观起见，在下图中我们使用了四种不同的颜色（ScannerA~ScannerD为随机名称，请忽略它们在名称上的顺序）：

每一个Scanner中都有一个current指针指向下一个即将要读取的KeyValue，**KeyValueHeap中的PriorityQueue正是按照每一个Scanner的current所指向的KeyValue进行排序**。

第一次next请求，将会返回ScannerA中的**Row01:FamA:Col1**，而后ScannerA的指针移动到下一个KeyValue **Row01:FamA:Col2**，PriorityQueue中的Scanners排序依然不变：



第二次next请求，依然返回ScannerA中的**Row01:FamA:Col2**，ScannerA的指针移动到下一个KeyValue Row02:FamA:Col1，此时，PriorityQueue中的Scanners排序发生了变化：



下一次next请求，将会返回ScannerB中的KeyValue.....周而复始，直到某一个Scanner所读取的数据耗尽，该Scanner将会被close，不再出现在上面的PriorityQueue中。

SegmentScanner/StoreFileScanner中返回的KeyValue，包含了各种类型的KeyValue：

已被更新过的旧KeyValue已被标记删除但尚未被及时清理的KeyValue已过期的尚未被及时清理的KeyValue用来描述一次删除操作的KeyValue(删除还包含了多种类型)承载最新用户数据的普通KeyValue因此，在StoreScanner层，需要对这些KeyValue做更复杂的逻辑校验，这些校验由**ScanQueryMatcher**完成。**默认地**，可作为返回数据的KeyValue，应该满足如下条件：

KeyValue类型为PutKeyValue所关联的列为用户Scan所涉及的列KeyValue的时间戳符合Scan的TimeRange要求版本最新未被标记删除通过了Filter的过滤条件上述条件，只针对一些普通的Scan，不同的Scan参数配置，可能会导致条件集发生变化，如Scan启用了Raw Scan模式时，Delete类型的KeyValue也会被返回。另外，上面的这些条件所罗列的顺序，也未遵循实际的检查顺序，而实际的检查顺序也是严格的，如果颠倒就可能会导致Bug。小米的同学就曾发现了这样的一个Bug:

假设某一个列共有T1~T5五个版本， ColumnFamily中设置的MaxVersions为3(即最大允许保留的版本数)T5 -> Value=5 T4 -> Value=4 T3 -> Value=3 T2 -> Value=2 T1 -> Value=1如果Scan中采用了一个SingleColumnValueFilter，要求返回满足Value<=3的所有结果。因为MaxVersions为3，我们**所期望的返回结果**应该为：T5 -> Value=5 （Value不满足条件）T4 -> Value=4 （Value不满足条件） T3 -> Value=3T2 -> Value=2 （Version不满足条件）T1 -> Value=1 （Version不满足条件）关于多版本检查以及Filter检查，这里有两种可能的顺序：**Opt 1**：先检查Filter，再检查多版本。这种情况下的返回结果为：T5 -> Value=5 （Value不满足条件）T4 -> Value=4 （Value不满足条件） T3 -> Value=3 T2 -> Value=2 T1 -> Value=1 这种情况的返回结果就是错误的。**Opt 2**: 先检查多版本，再检查Filter。这种情况下的返回结果才是预期的。



在Scanner中，如果允许读取多个版本（由Scan#readVersions配置），那正常的读取顺序应该为：

![](http://t11.baidu.com/it/u=4202344471,190308071&fm=173&app=25&f=JPG?w=640&h=531&s=DA203C62F2237BA972E525DB0000A0A1)



上面这种读取的顺序与实际存在的数据的逻辑顺序也是相同的。

由于不同的Scan所读取的每一行中的数据不同，有的限定了列的数量，有的限定了版本的数量，这使得读取时可以通过一些优化，减少不必要的数据扫描。如某次Scan在允许读多个版本的同时，限定了只读取C1~C3，那么，读取顺序应该为：

![](http://t10.baidu.com/it/u=3426600870,583086292&fm=173&app=25&f=JPG?w=639&h=531&s=DA283C62F2235AA972E525DB0000A0A1)



最普通的Scan，其实只需要读取每一列的最新版本即可，那读取的顺序应该为：



通过上面几张图，我们其实是想说明在Scanner内部需要具备这样的一些基础能力：

- 如果只需要当前列的最新版本，那么Scanner应该可以跳过当前列的其它版本，而且将指针移到下一列的开始位置。

- 如果当前行的所要读取的列都已读完，那么，Scanner应该可以跳过该行剩余的列，将指针移动到下一行的开始位置。

我们知道KeyValueScanner定义了基础的seek/reseek/requestSeek等接口，可以将指针移动到指定KeyValue位置。但关于指针如何移动的决策信息，由谁来提供？

这些信息也是由ScanQueryMatcher提供的。ScanQueryMatcher对每一个KeyValue的逻辑检查结果称之为MatchCode，MatchCode不仅包含了是否应该返回该KeyValue的结果，还可能给出了Scanner的下一步操作的提示信息。关于它的枚举值，简单举例如下：

**INCLUDE_AND_SEEK_NEXT_ROW**

包含当前KeyValue，并提示Scanner当前行已无需继续读取，请Seek到下一行。

**INCLUDE_AND_SEEK_NEXT_COL**

包含当前KeyValue，并提示Scanner当前列已无需继续读取，请Seek到下一列。

无论是StoreScanner还是RegionScanner，返回的都是符合条件的KeyValue列表。这些KeyValues在RSRpcServices层被进一步组装成Results响应给Client侧。

总结

Scan涉及了太多的细节内容，本文只粗略介绍了Scan的一些核心思路，这与本系列文章最初的定位有关，当然也受限于本文的篇幅。 本文主要介绍了如下内容：

**介绍HBase的两种读取模式：Get与Scan**

1.Client如何发起一次Get请求，Get的关键参数

2.Client如何发起一次Scan请求，Scan的关键参数

**重点介绍了RegionServer侧关于Scan的处理流程：**

1.如何用Scanner来抽象描述关于Region的读取操作

2.关于读取KeyValue的基础Scanner接口定义 3.RegionScanner初始化时的关键操作

4.Client侧的一次次scan请求如何驱动RegionScanner内部的读取操作

5.从StoreFileScanner/SegmentScanner中读取出来的原始KeyValue如何被合理的校验

6.Scanner读取时如何跳过一些不必要的数据



关于Scan的更多细节，感兴趣的同学可以自己去源码中探寻答案：

1.如果第一次scan请求不能取回所有的数据，下一次scan如何快速有效继承上一次的进度？2.Get/Small Scan/Large Scan在实现上有哪些本质的区别？

3.ScanQueryMatcher中校验KeyValue的详细逻辑以及校验的顺序

4.关于Filter涉及多步校验，每一步校验是在什么地方完成的？

5.MinVersion与MaxVersion的定义是什么？

6.ScanQueryMatcher中关于多种删除类型的语义是如何定义的？

7.如何限制一次Scan所占用的内存大小以及执行的时间？

8.BloomFilter在Get/Scan流程中是如何被应用的？

9.Scan过程中如果正在读取的HFile文件被Compaction合并了，如何处理？

10.正在Scan的Region突然被迁移到其它的RegionServer中，如何继续原来的进度继续读取？

11.Reverse Scan与普通Scan在实现上有何不同？



HFile的内容在本文只粗略提及，在RegionServer侧的处理流程中，关于BlockCache部分更是只字未提。本文将重点放在介绍Scan的核心思路上。**下篇文章将介绍HFile的核心原理。**

本文由百家号作者上传并发布，百家号仅提供信息发布平

**微信公众号【Java技术江湖】一位阿里 Java 工程师的技术小站。作者黄小斜，专注 Java 相关技术：SSM、SpringBoot、MySQL、分布式、中间件、集群、Linux、网络、多线程，偶尔讲点Docker、ELK，同时也分享技术干货和学习经验，致力于Java全栈开发！**

![](https://img-blog.csdnimg.cn/20190319165741304.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E3MjQ4ODg=,size_16,color_FFFFFF,t_70)



