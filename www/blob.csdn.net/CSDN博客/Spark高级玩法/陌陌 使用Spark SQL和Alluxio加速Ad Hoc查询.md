# 陌陌:使用Spark SQL和Alluxio加速Ad Hoc查询 - Spark高级玩法 - CSDN博客
2018年08月15日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：203
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
星球里经常有人问，如何保存sparkstreaming状态，回答的时候也会说道Alluxio。可能很多公司并没有去做Alluxio相关的使用。希望通过本文，大家对Alluxio的使用场景更详细了解，后面结合spark streaming浪尖会制作一个demo。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)**数据分析趋势**
## Hadoop生态系统的广泛应用大幅度降低了企业启用分布式系统/算法的开发成本。但是资源的高效利用一直是企业和供应商的诉求目标，因此高效性能成为下一个议题。我们在综合考虑了几个选项来满足提升性能方面的需求后, 将精力集中在有智能缓存功能的Alluxio系统上。
Alluxio集群作为连接计算和存储的数据访问加速器。通过暂时将数据存储在内存或其它接近计算服务所属介质中的方法, 起到加速访问并提供远程存储本地化提升性能的能力。这种能力对于计算应用程序在云部署以及计算分离的对象存储场景中发生数据移动时负载优化尤为重要。缓存使用读/写缓冲保持持久存储的连续性以实现对用户的透明性。智能缓存管理利用可配置的策略来实现高效的数据放置（data placement），并且支持内存和磁盘（SSD/HDD）的分层存储。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)部署Alluxio的考虑
在大数据生态系统中, 主从架构是多数分布式系统的主流设计。这些具有中心化特点的系统有一个共同的问题，那就是主节点存储大量元信息数据和各种状态数据。因此，master节点可能会面临因负载过高而导致系统性能下降, 以及单点故障影响整体服务的风险，典型如HDFS中的NameNode。然而，如果一个服务是无状态或轻量级的，则它们的状态和存储的元数据可以很容易的恢复, 从系统维护的角度看， 这种设计能明显降低相关维护成本。 Alluxio是一个主从架构的服务，主节点也存储大量元信息数据和和多种状态数据，因此我们dev/ops团队尽可能让其不再成为下一个Namenode级别的服务。
Alluxio被设计为一个以内存为中心的架构。内存读写快是毋庸置疑的，但有几个问题需要多一些考虑。
- 
基于读场景考虑，由于冷读取会触发从远程数据源获取数据，所以在Alluxio上运行的任务性能仍然会优于同一个任务跑在线上环境吗？
- 
是否需要将从远程数据源获取的所有数据全部加载到Alluxio中？
- 
基于写场景考虑，如果最终仍将数据写入到远程存储（比如HDFS），为什么要写到Alluxio再写到HDFS呢，而不是直接写HDFS？前者显然增加了一些开销。
- 
如果先把数据写入Alluxio，那么当一个Alluxio master或worker节点失败的时候又会发生什么？
Alluxio分布式架构使得工作负载以一种向外扩展的方式散布到多个节点，以解决性能方面的问题。对于元数据，我们希望master节点能够按需进行扩展，但是在节点故障的情况下secondary master可以接管。即使一个master或worker节点发生故障，我们也可以通过格式化，重启集群并从远程重新加载数据的方式，来避免任何数据的丢失。
因为许多变量会影响写性能，所以很难预先设计出最佳方法来解决这个问题。对我们而言，在优先考虑稳定性的约束下，我们选择暂不将Alluxio应用于写密集场景。
Alluxio非常适合于有经常访问的热数据存在以及应用程序会利用内存缓存的场景。这既避免了从硬盘反复加载以及通过网络转移数据的开销，也避免了为很少访问的数据提供过多的内存资源而造成性能限制或资源浪费。
综上所述最适合我们基础设施的应用场景是Ad Hoc查询。部分热点数据经常被访问并且是读密集的，另外在必要时容易恢复。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)陌陌的架构
首先确定的是将HDFS Datanodes和Alluxio workers隔离部署，以解决如下问题：
- 
这两个进程都需要硬盘来存储数据，而大量的I/O操作可能会导致磁盘故障率的增加，这已经是生产中的一个问题了。
- 
Alluxio workers的分开提供了专用的HDD资源用于缓存，Datanodes上的HDD通常有超过80%的容量，因此这是一种有效的独立管理资源并提供最佳性能的方法。
此外，我们希望避免不能识别出Alluxio系统的在线任务被分配到Alluxio workers节点上。而yarn的节点标签特性解决了这个问题，通过将计算节点标记为“ad hoc”标签, 从而为Alluxio建立了一个独立专属的标签集群。下图显示了我们的架构。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFVxzkAV7dIEMbDMIXN9WGgJajoO5B4H3KVPJe48HS8n2ibkbvJCHhcvXXB0HLhVYRgl0Iu0Y85hFpQ/640?wx_fmt=jpeg)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)性能评估
我们抽取了四个不同大小的线上查询作实验，并且以四种不同环境运行这些查询, 后文以不同模式来区分这些环境：
- 
Yarn模式，是当前线上的生产环境。
- 
Spark模式，在标签集群上运行没有Alluxio作为中间层的Spark计算环境。
- 
Alluxio模式，在标签集群上运行配置了Alluxio 作为中间层, 并启用RAM和HDD层的 Spark 计算环境。
- 
Alluxio on Disk模式，和第三种模式很相似，但只使用HDD缓存，未启用RAM层缓存。
感兴趣的主要对比是生产环境的Yarn模式vs.使用RAM和HDD缓存的Alluxio模式。Spark模式和Alluxio on Disk模式是作为对照观察。由于在线模式存在资源竞争现象, 所以Spark模式可以理解为Yarn模式的空白对照, 即不存在资源竞争的情况下的在线模式表现。Alluxio on Disk是Alluxio模式的对照, 用于观察RAM层对效能的影响。
下面的表格显示了查询的输入大小信息，图表显示了性能结果。Y轴是以秒为单位的时间，更快显然更好。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFVxzkAV7dIEMbDMIXN9WGgJ9Vf3KA59hXwZg7X6H7p748vd9I1biadUEUtFCS9ibibapKePHQUbNb3Og/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFVxzkAV7dIEMbDMIXN9WGgJ0bbQdILgZ9Lf59e1srklnc290P24HtRzyHcCiafNln3AS1aIFdxIgOw/640?wx_fmt=jpeg)
从如上测试结果中可以得出以下几个推论：
- 
总的来说，Alluxio按照预期所想的取得了显著的性能提升，Alluxio 模式比Yarn模式优化了3-5倍时间开销，另外相较Spark模式也取得了1.5-3倍的时间开销优化。
- 
即使存在冷读场景下，多数情况Alluxio模式仍然有更好的表现。
- 
Alluxio workers越多，可获得的性能提升效果越明显。
- 
通过Alluxio模式和Alluxio on Disk模式的对比, 可以发现, RAM层带来的提升并没有一个数量级的差距, 在考虑到现在内存的成本约束下，Alluxio on Disk也是一个不错的折中。
- 
在某些小规模输入场景下，Spark模式和Alluixo模式取得了相近甚至优于Alluxio模式的时间开销, 这主要由于Spark具有自身的内存缓存管理机制。但是，一旦缓存数据量超过了JVM的内存，Spark就不能保持相应的性能效果了，而因为Alluxio使用的堆外内存技术所以不受相应限制。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)陌陌的部署方式
对于Spark的thrift server，我们开发了白名单特性，允许Alluxio加载指定表数据。采用这种方法可以在充分利用Alluxio 缓存能力的同时, 对缓存数据实现基本的管理，避免不必要的数据加载和回收。
此外，为了让Alluxio的启用对上层用户透明，还开发了无需修改用户侧任何业务代码即可自动切换对应模式的配置方法。
因此，如果SQL是一个涉及缓存白名单中表的数据查询,那么表的路径将会被转换为一个Alluxio的URI，这样应用程序就可以从Alluxio读取相关数据。如果SQL是一个DML 或 DCL 操作，它保持和原来一样，并直接写入远程文件系统（本例中既是HDFS）。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)总结和下一步工作
根据性能评估结果，大多数情况下Alluxio提高了计算性能，所以后续计划更广泛地部署相关服务并推广到更多的应用场景。此外，将更积极与社区保持同步, 更多关注系统在安全性、稳定性和任务监测相关问题。
下一步的考虑：
- 
从远程存储系统获取数据常常受到网络带宽的限制，从而限制了性能的提升。
- 
与由数千台机器组成的在线集群相比，标签集群的数量是有限制的，所以RAM层的上限很低。
- 
和业务方探寻适合Alluxio 写的应用场景。
本文作者richbird，原文地址：https://zhuanlan.zhihu.com/p/35523519
**推荐阅读：**
[Flink与Spark Streaming在与kafka结合的区别！](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485057&idx=1&sn=e82380e4c096ce60bb3cabe20d979afa&chksm=9f38e5a9a84f6cbf2ff343fd8ab7e6ddc58d2ed0628d5fdee029f7166a2e48413b6df9b799e5&scene=21#wechat_redirect)
[必会:关于SparkStreaming checkpoint那些事儿](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485157&idx=1&sn=6714f0579d230000336fac78fe9a730a&chksm=9f38e5cda84f6cdb2e924bcae2127f025cdef9757a611407eb2e028927d54a8ab02b752e5963&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
