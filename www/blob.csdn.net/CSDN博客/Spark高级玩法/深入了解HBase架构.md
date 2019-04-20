# 深入了解HBase架构 - Spark高级玩法 - CSDN博客
2018年05月02日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：120
**HBase架构组件**
从物理结构上讲，HBase由三种类型的服务器构成主从式架构。Region Servers为数据的读取和写入提供服务。当访问数据时，客户端直接和Region Servers通信。Region的分配，DDL (create, delete tables)操作有HBase Master进程处理。Zookeeper是HDFS的一部分，维护着一个活动的集群。
Hadoop DataNode 存储着Region Server所管理的数据。所有的HBase数据存储在HDFS的文件中。Region  Server和HDfs DataNode并置在一起，这使得RegionServers所服务的数据具有数据局部性（使数据接近需要的位置）。HBase数据在写入时是本地数据，但是当Region移动时，在压实之前它不是本地数据。
NameNode维护构成文件的所有物理数据块的元数据信息。
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lbPnMOwDcrLAlgI0bma0JiaOAReOHxPxtOfASn9F11Tv0dKYLu4T0bxg/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**Regions**
HBase表是按照rowkey范围水平划分为“Regions”.Region包含表中start key和end  key之间的所有行。Region Server将Regions分配到集群的节点中，并对数据的读取和写入提供服务。单个Redion Server可服务大约1000个region。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3licmtZWv064GjnxMVF3TeiaYZDbaOuE8LDic9ibgPpL8JvIVdViajwJz0yzA/640?wx_fmt=png)
**HBase HMaster**
Region分配，DDL(create, delete tables)操作由HBase Master处理。
Mater的主要职责：
协调Region Servers
启动时分配Region，还原时重新分配Region或者负载均衡
监控集群中所有RegionServer实例（监听Zookeeper的消息）
管理员方法
提供创建，删除，更新表的接口。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lKr1vibDbJnTN3CepNCh8IE9fHChTd9ZiaiacYtOTANeOdeZH8ZcpJQxaw/640?wx_fmt=png)
**ZooKeeper: The Coordinator**
HBase使用Zookeeper做为分布式协调服务来维护及群众server的状态。Zookeeper维护处于活状态并可使用的Severs，并提供Server故障通知。Zookeeper使用共识来保证共同共享的状态。请注意，应该有三到五台机器达成共识。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lfrxN1x9TYxJJkgE4TMXqlKASX6lQRMSSMT8JibhNQjTrF86wNyXM2kA/640?wx_fmt=png)
**How the Components Work Together（组建如何协调工作）**
Zookeeper用于协调分布式系统成员的共享状态信息。Region Server和active HMaster通过会话链接到Zookeeper.ZooKeeper通过心跳维护会话活动的临时节点。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3llbG2pSCfNdtpDshPq3c2qDEK6fq7s4Su8JGKPULjlqNNT6rmOWtBcQ/640?wx_fmt=png)
每个Region Server创建一个临时节点。HMaster监控这些节点以发现可用的region servers，并监控这些节点的服务器故障。
HMaster监控这些节点以发现可用的区域服务器，并监控这些节点的服务器故障。HMasters争夺创造一个短暂的节点。Zookeeper确定第一个并使用它来确保只有一个主站处于活动状态。活动HMaster将心跳发送到Zookeeper，非活动HMaster将监听活动HMaster故障的通知。如果region server或者actice HMaster未能发送心跳信号，则会话过期并删除相应的临时节点。Listeners的更新在收到节点删除的通知后。Active HMaster监听region servers，并在region servers出现故障时进行恢复。Inactive HMaster监听active HMaster故障，并且如果active HMaster故障时，inactive HMaster编程active状态。
**Base First Read or Write**
HBase有一个叫做META的特殊的目录表，用于保存集群中regions的位置信息。Zookeeper存储着META表的位置。
以下是客户端第一次读取和写入HBase时发生的情况：
1. 客户端从zookeeper中META Table的位置.
2. 客户端查询.META。服务器获取客户端想要访问的并且是rowkey所相对应Region Server的信息。客户端会将META缓存带本地。
3. 从相应的Region Server获取行
在未来的读取操作过程中，客户端使用Meta Cache来检索META Table的位置和之前读取的Row Keys。随着时间的推移，不再需要查询META table了，除非应为一个region转移而错过,那么它将重新查询并更新Meta Cache。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3luGXOr37LxUwCHAia3atIfF7FC3oRbNv4asRu6xQYkbKib2NnuqiaBkicOA/640?wx_fmt=png)
**HBase Meta Table**
1. META表是一个保存的了系统中所有region列表的HBase表。
2. META表就像一颗B—tree
3. METa的结构如下：
Key: region start key,region id
Values: RegionServer
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lV85V9pPUgybLpSarvn44E7pibtWemNTbNKcdmSictic3zhEgMlhkH2Nhg/640?wx_fmt=png)
**Region Server Components**
Region Server运行在HDFS的DataNode,并且具备以下组件：
1. WAL：预写日志是分布式文件系统上的文件。WAL用于存储尚未被永久保存的新数据，用于故障情况下的恢复。
2. BlockCache：是读取缓存。在内存中存储频繁读取的数据，近期最少使用的数据在满时被删除。
3. MemStore：是写入缓存。存储尚未写入磁盘的数据。在写入磁盘之前进行排序，每个region的每个column family有一个MemStore。
4. 在磁盘上，Hfiles将行存储为已排序的KeyValues。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lJeFHw1b5sMc4vX7go7WpxnxuVYZaP3v7q7KHKXJ3g1xoeTgdvLUq9A/640?wx_fmt=png)
**HBase Write Steps (1)**
当客户端发起Put请求时，第一步是将数据写入于写日志，WAL：
1. 发布的内容将被添加到存储在磁盘上的WAL文件末尾。
2. WAL用于在服务器崩溃的情况下恢复尚未保存的数据。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3l8qA6I0or09LnYzZVgAyibhwyEPf2qUx25g1azeA7HxZJicIfo0ibFicwyA/640?wx_fmt=png)
**HBase Write Steps (2)**
一旦数据写入WAL，将会被写入MemStore中，然后放入Put请求确认信息返回给客户端。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3l5AynWtpibm2Ejoed6ict37618aHnaSYFib8tYHQg2lgdTQAmwicI2J7YEg/640?wx_fmt=png)
**HBase MemStore**
MemStore 将更新的内容排序并以KeyValues的形式存储到内存中，与将其存储在HFile中相同。每个列族只有一个MenStorre，更新内容按照列族排序。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lw5cJF0eQ0zlSFInCicjIeTmusVLKibTuP6aUqDyQgJhOryePGS6kZvicQ/640?wx_fmt=png)
**HBase Region Flush**
当MemStrore积聚了足够的数据，整个有序集合被写入到HDFS的HFile中。HBase每个列族使用多个HFile，其中包含真正的Cell或者KeyValue实例。随着时间的推移，在MenStore中跟据KeyValue排序，最终刷新到磁盘HFile文件中。
注意这也是HBase为什么限制列族数量的一个原因。每个列族只有一个MemStore；当一个MemStore数据满了，会刷新到磁盘文件中。它还保存了最近写入的序列号，以便让系统知道到目前为止持久化的情况。
高位序列号作为元字段存储在每个HFile中，以反映持久化结束位置以及继续执行的位置。在region启动时，序列号被读取后，然后最高位做为新编辑内容的序列号。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lZU3Gu1ju3FAG2erDVoMxoq0otiaEjf3KkFAhibYiayUibahwlpEbvwcVicg/640?wx_fmt=png)
**HBase HFile**
数据存储在HFile中，其中包含排序的Key/Value。当MemStore累积足够的数据时，整个已排序的KeyValue集将被写入HDFS中的新HFile。这是一个顺序写入。它速度非常快，因为它避免了移动磁盘驱动器磁头。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lZFia0337L2MXlLicPl4gWz23Jfh8kvncJm7YvzZBN12x2qmZDq4n3uuQ/640?wx_fmt=png)
**HBase HFile Structure**
HBase包含一个多层索引，是HBase不必读取整个文件的情况下查找定位数据。多级索引就像一颗b+tree：
1. 键值对按照升序存储
2. 索引通过RowKey指向内容为key/value的64kb大小的block中。
3. 每个block都有自己叶子索引
4. 每个block的最后的key放进intermediate index（中间索引）
5. 根索引指向中间索引
trailer指向元数据块，它写在文件中持久化数据的末尾。trailer也包含布隆过滤器和时间范围信息。布隆过滤器有助于跳过不包含某个row key的文件。如果时间范围信息不在读取的时间范围内，则时间范围信息对于跳过该文件非常有用。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lia71AuoofdQnnOnbbxibLypywRcibWEs7vNRS6s9ibZEAGxtlAmicswFUzw/640?wx_fmt=png)
**HFile Index**
我们刚才讨论的索引是在HFile打开并保存在内存中时加载的。这允许查找通过单个磁盘寻道来执行。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lwPibTd8M1bqvJHqelic2pt78d3jYOsKKXZbur0fKkyf6aJT33CicGXkkw/640?wx_fmt=png)
**HBase Read Merge**
我们已经看到，row对应的KeyValue cell可以在多个位置，row cell已经持久化到Hfile中，最近更新的cell在MemStore中，最近读取的cell在Block Cache中。因此，当读取一行数据时，系统是如何获得相应的cell并返回的？读取操作按照以下步骤从BlockCache，MemStore和HFile合并关键值：
首先，扫描器在BlockCache（读取缓存）中，查找Row Cells。最近读取的Key Values被缓存在这里，并且当需要内存时，最近最少使用的被清除。
其次，扫描器在MemStore中查找，内存写入缓存包含最近的写入。
如果扫描器在MenStore和BlockCache中没有找到Row  cells，然后HBase使用BLock Cache的HFile索引和布隆过滤器把可能存在目标Row  cells的HFile加载到内存中。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3l26FJY2Fxib8Z4npicAVSoB2x4BPIe5dPXuutAibz8JlTAepAcuQAlt6nQ/640?wx_fmt=png)
**HBase Read Merge**
正如前面所讨论的，每个MemStore可能有许多HFile，这意味着读取时可能需要检查多个HFile文件，这可能会影响性能。这被称为读取放大。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lvYH325byaAdoUBJDB9hvV50yDMaW6VxQmibqRt9FubYJGk9XzUBGqww/640?wx_fmt=png)
**HBase Minor Compaction**
HBase会自动选择一些较小的HFile，并将它们重写成更少且更大的Hfiles。这个过程称为Minor Compaction。Minor Compaction通过将较小的文件重写为较少但较大的文件来减少存储文件的数量，执行合并排序。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lQCC0R3Et0dxcoKQ4gbP8HicmDiaw8sVgnxuNpGfTiaUNVmSH9sqicZTib4g/640?wx_fmt=png)
**HBase Major Compaction**
Major compaction将region所有的HFile合并并重写到一个HFile中，每个列族对应这样的一个HFile。并在此过程中，删除已删除或过期的Cell。这样提升了读取性能，由于Major compaction重写了所有HFile文件，因此在此过程中可能会发生大量磁盘I/O和网络流量。这被称为写入放大。
Major compaction执行计划可以自动运行。由于写入放大，通常计划在周末或晚上进行Major compaction。由于服务器故障或负载平衡，Major compaction还会使任何远程数据文件成为本地服务器的本地数据文件。
**Region = Contiguous Keys**
快速回顾一下region：
表格可以水平划分为一个或者多个region。早start key和end key之间包含连续的，排序的行范围。
每个region小为1GB（默认）
表的region通过RegionServer为客户端提供服务。
RegionServer可以提供大约1,000个region（可能属于同一个表或不同的表）
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lv2yGNLlhd1AgniaFeTyZnTgCCnlIPzqZSibiajXyibXyXJv36RiasMdqspA/640?wx_fmt=png)
**Region Split**
最初每个表格有一个区域。当一个region变得太大时，它会分裂成两个子region。代表原region的一半的两个子region，在相同的RegionServer上并行打开，然后将分区报告给HMaster。由于负载平衡的原因，HMaster可以安排将新region移动到其他服务器。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lbTyDpOiaUxMqKCMlumGc2bbtvRzRdj9nic7mMOxaxia2HSlicuhSx5RJCQ/640?wx_fmt=png)
**Read Load Balancing**
分裂最初发生在同一个RegionServer上，由于负载平衡的原因，HMaster可以安排将新region移动到其他服务器。这导致新RegionServer提供服务的数据来自远程HDFS节点，直到Major compaction将数据文件移动到RegionServer的本地节点。HBase数据在写入时是本地数据，但当某个区域被移动时（为了负载平衡或恢复），在Major compaction之前它不是本地数据。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lI6ej6rjEuqGJxJqUhFuDTtBpT45uk5MGbOO8xkZZ4wI9WgJow4zWfg/640?wx_fmt=png)
**HDFS Data Replication**
所有的写入和读取都来自主节点。HDFS复制WAL和HFile块。HFile块复制自动发生。HBase依靠HDFS在存储文件时提供的数据安全性。在HDFS中写入数据时，本地写入一个副本，然后将其复制到第二个节点，并将第三个副本写入第三个节点。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3ltQQ5R7KXKEjbwvoIr00bbYZq1kmHR91B2GawayH24v8l87fbQ8DA5Q/640?wx_fmt=png)
**HDFS Data Replication (2)**
WAL文件和HFiles文件被持久化到磁盘并复制，那么HBase如何恢复没有报持久化到HFile的MemStore更新呢？请参阅下一节寻找答案。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lzGEsIiakMribQWGbd4jbibC3tibtNHmMy9BY3G0lyDWMmceJ4cFz74icFcg/640?wx_fmt=png)
**HBase Crash Recovery**
当RegionServer失败时，崩溃的region将不可用，直到检测和恢复步骤发生。 Zookeeper会在失去与RegionServer心跳信号时确定节点故障。HMaster将会被通知Region Server失败。HMaster将会被通知Region Server失败。
当HMaster确定RegionServer宕机时，HMaster重新分配宕机服务器的Region到活动的服务器。为了恢复宕机服务器未刷新到磁盘的memstore数据，HMaster将属于宕机RegionServer的WAL拆分成单独的文件并将这些文件存储在新RegionServer的数据节点中。每个Region Server然后进行重播WAL，从相应的WAL拆分文件，为region重建MemStore。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lZLvaLryibpjEDMIWtEngKGCicXiaSCrDzoMFPyw65aKgPZ8OGAlRBlVJQ/640?wx_fmt=png)
WAL的文件包含编辑列表，其中一个编辑表示单个放置或删除。一个编辑表示一个放置或删除。编辑按时间顺序编写，因此，对于持久化，添加内容将附加到存储在磁盘上的WAL文件的末尾。
如果数据仍在内存中并且未保存到HFile时发生故障会发生什么？WAL重播，重播WAL的过程是通过读取WAL，添加或者排序已知的编辑到当前MemStore。最后，Memtore将变化刷新到HFile。
**Data Recovery**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmib0j7eVicDwuibeusRDcFe3lTkzy3Mdg9Yg50ibNyUnicxtGELuJpyYIyD3buMjIAOdciaRetP93UY5rQ/640?wx_fmt=png)
**Apache HBase Architecture Benefits**
**HBase 优点：**
1. 强一致性模型
当写入返回时，所有读者将看到相同的值
2. 自动扩展
数据增长过大时分割region
使用HDFS传播和复制数据
3. 内置恢复机制
使用预写日志 (与文件系统上的日记类似)
4. 集成Hadoop
MHBase上的MapReduce很简单
**Apache HBase Has Problems Too…**
**Business continuity reliability：**
1. WAL 重播缓慢
2. 意外恢复缓慢
3. Major Compaction I/O 风暴
> 
**出处：http://www.uml.org.cn/bigdata/201804131.asp**
版权申明：内容来源网络，版权归原创者所有。除非无法确认，我们都会标明作者及出处，如有侵权烦请告知，我们会立即删除并表示歉意。谢谢。
**推荐阅读：**
1，[HBase高可用集群运维实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484508&idx=1&sn=690e313ab18b81198985e98a42084c17&chksm=9f38e774a84f6e62bcb93476f8a626bb651756f507e1ff8f3288424e04dab12888262bb33a89&scene=21#wechat_redirect)
2，[HBase在滴滴出行的应用场景和最佳实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484343&idx=1&sn=cd0c5bd1fbd8d55dd9779d336880bb3d&chksm=9f38e09fa84f6989105d027e60beb463332313724813ea114ec3f4e29707c8da75fc09434269&scene=21#wechat_redirect)
3，[Hbase源码系列之scan源码解析及调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483739&idx=1&sn=22647805307d21d6f893bea04c78153c&chksm=9f38e273a84f6b65be8cac2cc619a65e80f31235b43779b056db0b220b91559cf4a477dbb8bb&scene=21#wechat_redirect)
4，[从零开始 Spark 性能调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484670&idx=1&sn=f1401758c5d177da61bfcddb5fb0e67b&chksm=9f38e7d6a84f6ec0d6cdafe0bcba3353436ac8faef8a397c6cd1cdda2d1bd5e8ce8ec181f12c&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
