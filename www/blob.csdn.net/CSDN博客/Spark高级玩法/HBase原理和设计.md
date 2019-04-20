# HBase原理和设计 - Spark高级玩法 - CSDN博客
2017年11月23日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：260
> 
**作者：Jiang Hongxiang**
**来源：比特科技**
**简介**
HBase —— Hadoop Database的简称，Google BigTable的另一种开源实现方式，从问世之初，就为了解决用大量廉价的机器高速存取海量数据、实现数据分布式存储提供可靠的方案。从功能上来讲，HBase不折不扣是一个数据库，与我们熟悉的Oracle、MySQL、MSSQL等一样，对外提供数据的存储和读取服务。而从应用的角度来说，HBase与一般的数据库又有所区别，HBase本身的存取接口相当简单，不支持复杂的数据存取，更不支持SQL等结构化的查询语言；HBase也没有除了rowkey以外的索引，所有的数据分布和查询都依赖rowkey。所以，HBase在表的设计上会有很严格的要求。架构上，HBase是分布式数据库的典范，这点比较像MongoDB的sharding模式，能根据键值的大小，把数据分布到不同的存储节点上，MongoDB根据configserver来定位数据落在哪个分区上，HBase通过访问Zookeeper来获取-ROOT-表所在地址，通过-ROOT-表得到相应.META.表信息，从而获取数据存储的region位置。
架构
上面提到，HBase是一个分布式的架构，除去底层存储的HDFS外，HBase本身从功能上可以分为三块：Zookeeper群、Master群和RegionServer群。
- 
Zookeeper群：HBase集群中不可缺少的重要部分，主要用于存储Master地址、协调Master和RegionServer等上下线、存储临时数据等等。
- 
Master群：Master主要是做一些管理操作，如：region的分配，手动管理操作下发等等，一般数据的读写操作并不需要经过Master集群，所以Master一般不需要很高的配置即可。
- 
RegionServer群：RegionServer群是真正数据存储的地方，每个RegionServer由若干个region组成，而一个region维护了一定区间rowkey值的数据，整个结构如下图：
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz/icNyEYk3VqGkokZMogdNSibkDh5XqzYFibjmct01zTnE1gFUDqcuhlSs6R2iaHuGicggPlvWkONSsWuaKpnFGWH6oCQ/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
上图中，Zookeeper(简称ZK)是一个集群，通常有奇数个ZK服务组成。Master为了服务可用性，也建议部署成集群方式，因为Master是整个管理操作的发起者，如果Master一旦发生意外停机，整个集群将会无法进行管理操作，所以Master也必须有多个，当然多个Master也有主从之分，如何区分哪个是主，哪个是从？关键看哪个Master能竞争到ZK上对应Master目录下的锁，持有该目录锁的Master为主Master，其他从Master轮询竞争该锁，所以一旦主Master发生意外停机，从Master很快会因为竞争到Master文件夹上的锁而接管服务。
RegionServer(简称RS)在非Replication模式下，整个系统中都是唯一的，也就是说，在整个非Replication的HBase集群中，每台RS上保存的数据都不一样，所以相对于前面两者，该模式下的RS并不是高可用的，至少RS可能存在单点故障的问题，但是由于HBase内部数据分region存储和region可以迁移的机制，RS服务的单点故障可能会在极小代价下很快恢复，但是一旦停掉的RS上有-ROOT-或者.META.表的region，那后果还是比较严重，因为数据节点的RS停机，只会在短时间内影响该台RS上的region不可访问，等到region迁移完成后即可恢复，如果是-ROOT-、.META.所在的RS停机，整个HBase的新的求情都将受到影响，因为需要通过.META.表来路由，从而寻找到region所在RS的地址。
**数据组织**
整个架构中，ZK用于服务协调和整个集群运行过程中部分信息的保存和-ROOT-表地址定位，Master用于集群内部管理，所以剩下的RS主要用于处理数据。
RS是处理数据的主要场所，那么在RS内部的数据是怎么分布的？其实RS本身只是一个容器，其定义了一些功能线程，比如：数据合并线程(compact thread)、storeFile分割线程(split thread)等等。容器中的主要对象就是region，region是一个表根据自身rowkey范围划分的一部分，一个表可以被划分成若干部分，也就是若干个region，region可以根据rowkey范围不同而被分布在不同的RS上(当然也可以在同一个RS上，但不建议这么做)。一个RS上可以包含多个表的region，也可以只包含一个表的部分region，RS和表是两个不同的概念。
这里还有一个概念——列簇。对HBase有一些了解的人，或多或少听说过：HBase是一个列式存储的数据库，而这个列式存储中的列，其实是区别于一般数据库的列，这里的列的概念，就是列簇，列簇，顾名思义就是很多列的集合，而在数据存储上来讲，不同列簇的数据，一定是分开存储的，即使是在同一个region内部，不同的列簇也存储在不同的文件夹中，这样做的好处是，一般我们定义列簇的时候，通常会把类似的数据放入同一个列簇，不同的列簇分开存储，有利于数据的压缩，并且HBase本身支持多种压缩方式。
**原理**
前面介绍了HBase的一般架构，我们知道了HBase有ZK、Master和RS等组成，本节我们来介绍下HBase的基本原理，从数据访问、RS路由到RS内部缓存、数据存储和刷写再到region的合并和拆分等等功能。
**RegionServer定位**
访问HBase通过HBase客户端(或API)进行，整个HBase提供给外部的地址，其实是ZK的入口，前面也介绍了，ZK中有保存-ROOT-所在的RS地址，从-ROOT-表可以获取.META.表信息，根据.META.表可以获取region在RS上的分布，整个region寻址过程大致如下：
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz/icNyEYk3VqGkokZMogdNSibkDh5XqzYFibjx3Xkx3I6ib48D4QQzhZcFTJmc2EO3wlvKZEY2kebZVNhq7ibOW9pJOaQ/0?wx_fmt=jpeg)
RS定位过程
- 
首先，Client通过访问ZK来请求目标数据的地址。
- 
ZK中保存了-ROOT-表的地址，所以ZK通过访问-ROOT-表来请求数据地址。
- 
同样，-ROOT-表中保存的是.META.的信息，通过访问.META.表来获取具体的RS。
- 
.META.表查询到具体RS信息后返回具体RS地址给Client。
- 
Client端获取到目标地址后，然后直接向该地址发送数据请求。
上述过程其实是一个三层索引结构，从ZK获取-ROOT-信息，再从-ROOT-获取.META.表信息，最后从.META.表中查到RS地址后缓存。这里有几个问题：
- 
既然ZK中能保存-ROOT-信息，那么为什么不把.META.信息直接保存在ZK中，而需要通过-ROOT-表来定位？
- 
Client查找到目标地址后，下一次请求还需要走ZK  —> -ROOT- —> .META.这个流程么？
先来回答第一个问题：为什么不直接把.META.表信息直接保存到ZK中？主要是为了保存的数据量考虑，ZK中不宜保存大量数据，而.META.表主要是保存Region和RS的映射信息，region的数量没有具体约束，只要在内存允许的范围内，region数量可以有很多，如果保存在ZK中，ZK的压力会很大。所以，通过一个-ROOT-表来转存到RS中是一个比较理想的方案，相比直接保存在ZK中，也就多了一层-ROOT-表的查询，对性能来说影响不大。
第二个问题：每次访问都需要走ZK –> -ROOT- —> .META.的流程么？当然不需要，Client端有缓存，第一次查询到相应region所在RS后，这个信息将被缓存到Client端，以后每次访问都直接从缓存中获取RS地址即可。当然这里有个意外：访问的region若果在RS上发生了改变，比如被balancer迁移到其他RS上了，这个时候，通过缓存的地址访问会出现异常，在出现异常的情况下，Client需要重新走一遍上面的流程来获取新的RS地址。总体来说，region的变动只会在极少数情况下发生，一般变动不会很大，所以在整个集群访问过程中，影响可以忽略。
**Region数据写入**
HBase通过ZK —> -ROOT-  —> .META.的访问获取RS地址后，直接向该RS上进行数据写入操作，整个过程如下图：
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz/icNyEYk3VqGkokZMogdNSibkDh5XqzYFibjMyPtZibc18DTzbBbb2VJia5ueIIsDlb0iczmoP2OpVzGvLc7pRLrBvk3Q/0?wx_fmt=jpeg)
RegionServer数据操作过程
Client通过三层索引获得RS的地址后，即可向指定RS的对应region进行数据写入，HBase的数据写入采用WAL(write ahead log)的形式，先写log，后写数据。HBase是一个append类型的数据库，没有关系型数据库那么复杂的操作，所以记录HLog的操作都是简单的put操作(delete/update操作都被转化为put进行)
**HLog**
HLog写入
HLog是HBase实现WAL方式产生的日志信息，其内部是一个简单的顺序日志，每个RS上的region都共享一个HLog，所有对于该RS上的region数据写入都被记录到该HLog中。HLog的主要作用就是在RS出现意外崩溃的时候，可以尽量多的恢复数据，这里说是尽量多，因为在一般情况下，客户端为了提高性能，会把HLog的auto flush关掉，这样HLog日志的落盘全靠操作系统保证，如果出现意外崩溃，短时间内没有被fsync的日志会被丢失。
HLog过期
HLog的大量写入会造成HLog占用存储空间会越来越大，HBase通过HLog过期的方式进行HLog的清理，每个RS内部都有一个HLog监控线程在运行，其周期可以通过hbase.master.cleaner.interval进行配置。
HLog在数据从memstore flush到底层存储上后，说明该段HLog已经不再被需要，就会被移动到.oldlogs这个目录下，HLog监控线程监控该目录下的HLog，当该文件夹下的HLog达到hbase.master.logcleaner.ttl设置的过期条件后，监控线程立即删除过期的HLog。
**Memstore**
数据存储
memstore是region内部缓存，其大小通过HBase参数hbase.hregion.memstore.flush.size进行配置。RS在写完HLog以后，数据写入的下一个目标就是region的memstore，memstore在HBase内部通过LSM-tree结构组织，所以能够合并大量对于相同rowkey上的更新操作。
正是由于memstore的存在，HBase的数据写入都是异步的，而且性能非常不错，写入到memstore后，该次写入请求就可以被返回，HBase即认为该次数据写入成功。这里有一点需要说明，写入到memstore中的数据都是预先按照rowkey的值进行排序的，这样有利于后续数据查找。
数据刷盘
memstore中的数据在一定条件下会进行刷写操作，使数据持久化到相应的存储设备上，触发memstore刷盘的操作有多种不同的方式如下图：
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz/icNyEYk3VqGkokZMogdNSibkDh5XqzYFibjhLhweiasnCUiaMnb73HXcRSWaVHa2q6JA5VP6DrA92RGZFdaS41z5wXQ/0?wx_fmt=jpeg)
Memstore刷写流程
以上1,2,3都可以触发memstore的flush操作，但是实现的方式不同：
- 
通过全局内存控制，触发memstore刷盘操作。memstore整体内存占用上限通过参数hbase.regionserver.global.memstore.upperLimit进行设置，当然在达到上限后，memstore的刷写也不是一直进行，在内存下降到hbase.regionserver.global.memstore.lowerLimit配置的值后，即停止memstore的刷盘操作。这样做，主要是为了防止长时间的memstore刷盘，会影响整体的性能。
在该种情况下，RS中所有region的memstore内存占用都没达到刷盘条件，但整体的内存消耗已经到一个非常危险的范围，如果持续下去，很有可能造成RS的OOM，这个时候，需要进行memstore的刷盘，从而释放内存。
- 
手动触发memstore刷盘操作
HBase提供API接口，运行通过外部调用进行memstore的刷盘
- 
memstore上限触发数据刷盘
前面提到memstore的大小通过hbase.hregion.memstore.flush.size进行设置，当region中memstore的数据量达到该值时，会自动触发memstore的刷盘操作。
刷盘影响
memstore在不同的条件下会触发数据刷盘，那么整个数据在刷盘过程中，对region的数据写入等有什么影响？memstore的数据刷盘，对region的直接影响就是：在数据刷盘开始到结束这段时间内，该region上的访问都是被拒绝的，这里主要是因为在数据刷盘结束时，RS会对改region做一个snapshot，同时HLog做一个checkpoint操作，通知ZK哪些HLog可以被移到.oldlogs下。从前面图上也可以看到，在memstore写盘开始，相应region会被加上UpdateLock锁，写盘结束后该锁被释放。
**StoreFile**
memstore在触发刷盘操作后会被写入底层存储，每次memstore的刷盘就会相应生成一个存储文件HFile，storeFile即HFile在HBase层的轻量级分装。数据量的持续写入，造成memstore的频繁flush，每次flush都会产生一个HFile，这样底层存储设备上的HFile文件数量将会越来越多。不管是HDFS还是Linux下常用的文件系统如Ext4、XFS等，对小而多的文件上的管理都没有大文件来的有效，比如小文件打开需要消耗更多的文件句柄；在大量小文件中进行指定rowkey数据的查询性能没有在少量大文件中查询来的快等等。
Compact
大量HFile的产生，会消耗更多的文件句柄，同时会造成RS在数据查询等的效率大幅度下降，HBase为解决这个问题，引入了compact操作，RS通过compact把大量小的HFile进行文件合并，生成大的HFile文件。
RS上的compact根据功能的不同，可以分为两种不同类型，即：minor compact和major compact。
- 
Minor Compact
minor compact又叫small compact，在RS运行过程中会频繁进行，主要通过参数hbase.hstore.compactionThreshold进行控制，该参数配置了HFile数量在满足该值时，进行minor compact，minor compact只选取region下部分HFile进行compact操作，并且选取的HFile大小不能超过hbase.hregion.max.filesize参数设置。
- 
Major Compact
相反major compact也被称之为large compact，major compact会对整个region下相同列簇的所有HFile进行compact，也就是说major compact结束后，同一个列簇下的HFile会被合并成一个。major compact是一个比较长的过程，对底层I/O的压力相对较大。
major compact除了合并HFile外，另外一个重要功能就是清理过期或者被删除的数据。前面提到过，HBase的delete操作也是通过append的方式写入，一旦某些数据在HBase内部被删除了，在内部只是被简单标记为删除，真正在存储层面没有进行数据清理，只有通过major compact对HFile进行重组时，被标记为删除的数据才能被真正的清理。
compact操作都有特定的线程进行，一般情况下不会影响RS上数据写入的性能，当然也有例外：在compact操作速度跟不上region中HFile增长速度时，为了安全考虑，RS会在HFile达到一定数量时，对写入进行锁定操作，直到HFile通过compact降到一定的范围内才释放锁。
Split
compact将多个HFile合并单个HFile文件，随着数据量的不断写入，单个HFile也会越来越大，大量小的HFile会影响数据查询性能，大的HFile也会，HFile越大，相对的在HFile中搜索的指定rowkey的数据花的时间也就越长，HBase同样提供了region的split方案来解决大的HFile造成数据查询时间过长问题。
一个较大的region通过split操作，会生成两个小的region，称之为Daughter，一般Daughter中的数据是根据rowkey的之间点进行切分的，region的split过程大致如下图：
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz/icNyEYk3VqGkokZMogdNSibkDh5XqzYFibjar0AZLmzib7O6GRbwg210Lrx6BUXXMKRSEm1fA6Sol7pUu7Or0MzfjA/0?wx_fmt=jpeg)
region split流程
- 
region先更改ZK中该region的状态为SPLITING。
- 
Master检测到region状态改变。
- 
region会在存储目录下新建.split文件夹用于保存split后的daughter region信息。
- 
Parent region关闭数据写入并触发flush操作，保证所有写入Parent region的数据都能持久化。
- 
在.split文件夹下新建两个region，称之为daughter A、daughter B。
- 
Daughter A、Daughter B拷贝到HBase根目录下，形成两个新的region。
- 
Parent region通知修改.META.表后下线，不再提供服务。
- 
Daughter A、Daughter B上线，开始向外提供服务。
- 
如果开启了balance_switch服务，split后的region将会被重新分布。
上面1 ~ 9就是region split的整个过程，split过程非常快，速度基本会在秒级内，那么在这么快的时间内，region中的数据怎么被重新组织的？
其实，split只是简单的把region从逻辑上划分成两个，并没有涉及到底层数据的重组，split完成后，Parent region并没有被销毁，只是被做下线处理，不再对外部提供服务。而新产生的region Daughter A和Daughter B，内部的数据只是简单的到Parent region数据的索引，Parent region数据的清理在Daughter A和Daughter B进行major compact以后，发现已经没有到其内部数据的索引后，Parent
 region才会被真正的清理。
**HBase设计**
HBase是一个分布式数据库，其性能的好坏主要取决于内部表的设计和资源的分配是否合理。
**Rowkey设计**
rowkey是HBase实现分布式的基础，HBase通过rowkey范围划分不同的region，分布式系统的基本要求就是在任何时候，系统的访问都不要出现明显的热点现象，所以rowkey的设计至关重要，一般我们建议rowkey的开始部分以hash或者MD5进行散列，尽量做到rowkey的头部是均匀分布的。禁止采用时间、用户id等明显有分段现象的标志直接当作rowkey来使用。
**列簇设计**
HBase的表设计时，根据不同需求有不同选择，需要做在线查询的数据表，尽量不要设计多个列簇，我们知道，不同的列簇在存储上是被分开的，多列簇设计会造成在数据查询的时候读取更多的文件，从而消耗更多的I/O。
**TTL设计**
选择合适的数据过期时间也是表设计中需要注意的一点，HBase中允许列簇定义数据过期时间，数据一旦超过过期时间，可以被major compact进行清理。大量无用历史数据的残余，会造成region体积增大，影响查询效率。
**Region设计**
一般地，region不宜设计成很大，除非应用对阶段性性能要求很多，但是在将来运行一段时间可以接受停服处理。region过大会导致major compact调用的周期变长，而单次major compact的时间也相应变长。major compact对底层I/O会造成压力，长时间的compact操作可能会影响数据的flush，compact的周期变长会导致许多删除或者过期的数据不能被及时清理，对数据的读取速度等都有影响。
相反，小的region意味着major compact会相对频繁，但是由于region比较小，major compact的相对时间较快，而且相对较多的major compact操作，会加速过期数据的清理。
当然，小region的设计意味着更多的region split风险，region容量过小，在数据量达到上限后，region需要进行split来拆分，其实split操作在整个HBase运行过程中，是被不怎么希望出现的，因为一旦发生split，涉及到数据的重组，region的再分配等一系列问题。所以我们在设计之初就需要考虑到这些问题，尽量避免region的运行过程中发生split。
HBase可以通过在表创建的时候进行region的预分配来解决运行过程中region的split产生，在表设计的时候，预先分配足够多的region数，在region达到上限前，至少有部分数据会过期，通过major compact进行清理后， region的数据量始终维持在一个平衡状态。
region数量的设计还需要考虑内存上的限制，通过前面的介绍我们知道每个region都有memstore，memstore的数量与region数量和region下列簇的数量成正比,一个RS下memstore内存消耗：
> 
Memory = memstore大小 * region数量 * 列簇数量
如果不进行前期数据量估算和region的预分配，通过不断的split产生新的region，容易导致因为内存不足而出现OOM现象。
**推荐阅读：**
1，[大数据查询——HBase读写设计与实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484239&idx=1&sn=93c55d3a91f6ff9646389755844cabe7&chksm=9f38e067a84f697182d978ba8c3d89a4b3b7349beccbf070a9dc958b5738a83e6cd9bf24b49d&scene=21#wechat_redirect)
2，[HBase的安装部署](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484117&idx=2&sn=b5cbd15e1259d5727f7c36ee83c60989&chksm=9f38e1fda84f68ebf4a0e52b888dc7b8885d1c899426067a6fa4b2ca3b903b1594d566efab2f&scene=21#wechat_redirect)
3，[浅谈数据分库分表之道](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484095&idx=1&sn=d0e5b171478d7022062099ff22934308&chksm=9f38e197a84f688149649d0a9492ba9f9f886de2fa89defdac732708cbe550ff8c4cbc165cc9&scene=21#wechat_redirect)
4，[Spark源码系列之spark2.2的StructuredStreaming使用及源码介绍](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483744&idx=1&sn=0b9908624245df2eaa24a88d2a04be58&chksm=9f38e248a84f6b5e206cb0bf3ade7378c00d1cf662125315341a143b63f9e8f72cdf91a0db17&scene=21#wechat_redirect)
5，[Hbase源码系列之源码前奏hbase:meta表相关详细介绍](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483719&idx=1&sn=0234932b3ded7ae249d6eb31ea5605f6&chksm=9f38e26fa84f6b7922d8efa10c086e28cb0d7187a5b8f91143d9a252caf3f4fa1d0704abfc77&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

