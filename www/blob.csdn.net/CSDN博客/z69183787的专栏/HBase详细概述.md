# HBase详细概述 - z69183787的专栏 - CSDN博客
2017年10月10日 17:40:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：461
个人分类：[大数据-Hbase](https://blog.csdn.net/z69183787/article/category/5568489)
[http://m.blog.csdn.net/u010270403/article/details/51648462](http://m.blog.csdn.net/u010270403/article/details/51648462)
本文首先简单介绍了HBase,然后重点讲述了HBase的高并发和实时处理数据 、HBase数据模型、HBase物理存储、HBase系统架构，HBase调优、HBase Shell访问等。
不过在此之前，你可以先了解 [Hadoop生态系统](http://blog.csdn.net/u010270403/article/details/51493191) ，若想运行HBase，则需要先搭建好Hadoop集群环境，可以参考此文[搭建5个节点的hadoop集群环境（CDH5）](http://blog.csdn.net/u010270403/article/details/51446674) 。
好了，让我们来学习HBase吧！
# HBase简介
## HBase的发展史
2006年底由PowerSet 的Chad Walters和Jim Kellerman 发起，2008年成为Apache Hadoop的一个子项目。现已作为产品在多家企业被使用，如：
- WorldLingo
- Streamy.com
- OpenPlaces
- Yahoo!
- Adobe
- 淘宝
- Facebook
- Twitter
- Trend Micro
## Hbase是什么
HBase是一种构建在HDFS之上的分布式、面向列的存储系统。在需要实时读写、随机访问超大规模数据集时，可以使用HBase。
尽管已经有许多数据存储和访问的策略和实现方法，但事实上大多数解决方案，特别是一些关系类型的，在构建时并没有考虑超大规模和分布式的特点。许多商家通过复制和分区的方法来扩充数据库使其突破单个节点的界限，但这些功能通常都是事后增加的，安装和维护都和复杂。同时，也会影响RDBMS的特定功能，例如联接、复杂的查询、触发器、视图和外键约束这些操作在大型的RDBMS上的代价相当高，甚至根本无法实现。
HBase从另一个角度处理伸缩性问题。它通过线性方式从下到上增加节点来进行扩展。HBase不是关系型数据库，也不支持SQL，但是它有自己的特长，这是RDBMS不能处理的，HBase巧妙地将大而稀疏的表放在商用的服务器集群上。
HBase 是Google Bigtable 的开源实现，与Google Bigtable 利用GFS作为其文件存储系统类似， HBase 利用Hadoop HDFS 作为其文件存储系统；Google 运行MapReduce 来处理Bigtable中的海量数据， HBase 同样利用Hadoop MapReduce来处理HBase中的海量数据；Google Bigtable 利用Chubby作为协同服务， HBase 利用Zookeeper作为对应。
## HBase的特点
- 大：一个表可以有上亿行，上百万列。
- 面向列：面向列表（簇）的存储和权限控制，列（簇）独立检索。
- 稀疏：对于为空（NULL）的列，并不占用存储空间，因此，表可以设计的非常稀疏。
- 无模式：每一行都有一个可以排序的主键和任意多的列，列可以根据需要动态增加，同一张表中不同的行可以有截然不同的列。
- 数据多版本：每个单元中的数据可以有多个版本，默认情况下，版本号自动分配，版本号就是单元格插入时的时间戳。
- 数据类型单一：HBase中的数据都是字符串，没有类型。
# HBase的高并发和实时处理数据
Hadoop是一个高容错、高延时的分布式文件系统和高并发的批处理系统，不适用于提供实时计算；HBase是可以提供实时计算的分布式数据库，数据被保存在HDFS分布式文件系统上，由HDFS保证期高容错性，但是再生产环境中，HBase是如何基于hadoop提供实时性呢？ HBase上的数据是以StoreFile(HFile)二进制流的形式存储在HDFS上block块儿中；但是HDFS并不知道的hbase存的是什么，它只把存储文件是为二进制文件，也就是说，hbase的存储数据对于HDFS文件系统是透明的。下面是HBase文件在HDFS上的存储示意图。
HBase HRegion servers集群中的所有的region的数据在服务器启动时都是被打开的，并且在内冲初始化一些memstore，相应的这就在一定程度上加快系统响 应；而Hadoop中的block中的数据文件默认是关闭的，只有在需要的时候才打开，处理完数据后就关闭，这在一定程度上就增加了响应时间。
![](http://i.imgur.com/hFAUWwJ.png)
从根本上说，HBase能提供实时计算服务主要原因是由其架构和底层的数据结构决定的，即由LSM-Tree + HTable(region分区) + Cache决定——客户端可以直接定位到要查数据所在的HRegion server服务器，然后直接在服务器的一个region上查找要匹配的数据，并且这些数据部分是经过cache缓存的。具体查询流程如下图所示：
![](http://i.imgur.com/v0nBB38.png)
具体数据访问流程如下：
- Client会通过内部缓存的相关的-ROOT-中的信息和.META.中的信息直接连接与请求数据匹配的HRegion server；
- 然后直接定位到该服务器上与客户请求对应的Region，客户请求首先会查询该Region在内存中的缓存——Memstore(Memstore是一个按key排序的树形结构的缓冲区)；
- 如果在Memstore中查到结果则直接将结果返回给Client；
- 在Memstore中没有查到匹配的数据，接下来会读已持久化的StoreFile文件中的数据。前面的章节已经讲过，StoreFile也是按 key排序的树形结构的文件——并且是特别为范围查询或block查询优化过的，；另外HBase读取磁盘文件是按其基本I/O单元(即 HBase Block)读数据的。
具体就是过程就是：
如果在BlockCache中能查到要造的数据则这届返回结果，否则就读去相应的StoreFile文件中读取一block的数据，如果还没有读到要查的 数据，就将该数据block放到HRegion Server的blockcache中，然后接着读下一block块儿的数据，一直到这样循环的block数据直到找到要请求的数据并返回结果；如果将该 Region中的数据都没有查到要找的数据，最后接直接返回null，表示没有找的匹配的数据。当然blockcache会在其大小大于一的阀值（heapsize * hfile.block.cache.size
 * 0.85）后启动基于LRU算法的淘汰机制，将最老最不常用的block删除。
# HBase数据模型
HBase 以表的形式存储数据。表由行和列组成。列划分为若干个列族（row family），如下图所示。
![](http://i.imgur.com/GFvzuzp.png)
HBase的逻辑数据模型：
![](http://i.imgur.com/URUpR4i.png)
HBase的物理数据模型（实际存储的数据模型）：
![](http://i.imgur.com/f2FSBal.png)
逻辑数据模型中空白cell在物理上是不存储的，因为根本没有必要存储，因此若一个请求为要获取t8时间的contents:html，他的结果就是空。相似的，若请求为获取t9时间的anchor:my.look.ca，结果也是空。但是，如果不指明时间，将会返回最新时间的行，每个最新的都会返回
## Row Key
与 NoSQL 数据库一样，Row Key 是用来检索记录的主键。访问 HBase table 中的行，只有三种方式：
1)通过单个 Row Key 访问。
2)通过 Row Key 的 range 全表扫描。
3)Row Key 可以使任意字符串（最大长度是64KB，实际应用中长度一般为 10 ~ 100bytes），在HBase 内部，Row Key 保存为字节数组。
在存储时，数据按照* Row Key 的字典序（byte order）排序存储*。设计 Key 时，要充分排序存储这个特性，将经常一起读取的行存储到一起（位置相关性）。
注意 字典序对 int 排序的结果是 1，10,100,11,12,13,14,15,16,17,18,19,20,21，…， 9，91,92,93,94,95,96,97,98,99。要保存整形的自然序，Row Key 必须用 0 进行左填充。
行的一次读写是原子操作（不论一次读写多少列）。这个设计决策能够使用户很容易理解程序在对同一个行进行并发更新操作时的行为。
## 列族
HBase 表中的每个列都归属于某个列族。列族是表的 Schema 的一部分（而列不是），必须在使用表之前定义。列名都以列族作为前缀，例如 courses:history、courses:math 都属于 courses 这个列族。
访问控制、磁盘和内存的使用统计都是在列族层面进行的。在实际应用中，列族上的控制权限能帮助我们管理不同类型的应用， 例如，允许一些应用可以添加新的基本数据、一些应用可以读取基本数据并创建继承的列族、 
一些应用则只允许浏览数据（甚至可能因为隐私的原因不能浏览所有数据）。
## 时间戳
HBase 中通过 Row 和 Columns 确定的一个存储单元称为 Cell。每个 Cell 都保存着同一份数据的多个版本。 版本通过时间戳来索引，时间戳的类型是 64 位整型。时间戳可以由HBase（在数据写入时自动）赋值， 
此时时间戳是精确到毫秒的当前系统时间。时间戳也 可以由客户显示赋值。如果应用程序要避免数据版本冲突，就必须自己生成具有唯一性的时间戳。每个 Cell 中，不同版本的数据按照时间倒序排序，即最新的数据排在最前面。
为了避免数据存在过多版本造成的管理（包括存储和索引）负担，HBase 提供了两种数据版本回收方式。 一是保存数据的最后 n 个版本，二是保存最近一段时间内的版本（比如最近七天）。用户可以针对每个列族进行设置。
## Cell
Cell 是由 {row key，column(=< family> + < label>)，version} 唯一确定的单元。Cell 中的数据是没有类型的，全部是字节码形式存储。
# HBase物理存储
Table 在行的方向上分割为多个HRegion，每个HRegion分散在不同的RegionServer中。
![](http://i.imgur.com/8l78wwK.png)
每个HRegion由多个Store构成，每个Store由一个memStore和0或多个StoreFile组成，每个Store保存一个Columns Family
![](http://i.imgur.com/Kffn381.png)
# HBase系统架构
HBase架构图如下：
![](http://i.imgur.com/rMljD33.jpg)
从HBase的架构图上可以看出，HBase中的组件包括Client、Zookeeper、HMaster、HRegionServer、HRegion、Store、MemStore、StoreFile、HFile、HLog等，接下来介绍他们的作用。
HBase中的每张表都通过行键按照一定的范围被分割成多个子表（HRegion），默认一个HRegion超过256M就要被分割成两个，这个过程由HRegionServer管理，而HRegion的分配由HMaster管理。
## Client
包含访问HBase的接口，并维护cache来加快对HBase的访问。
## Zookeeper的作用
HBase依赖Zookeeper，默认情况下HBase管理Zookeeper实例(启动或关闭Zookeeper),Master与RegionServers启动时会向Zookeeper注册。
![](http://i.imgur.com/5tqPWmW.jpg)
Zookeeper的作用：
- 保证任何时候，集群中只有一个master
- 存储所有Region的寻址入口
- 实时监控Region server的上线和下线信息。并实时通知给master
- 存储HBase的schema和table元数据
## HMaster的作用
- 为Region server分配region
- 负责Region server的负载均衡
- 发现失效的Region server并重新分配其上的region。
- HDFS上的垃圾文件回收。
- 处理schema更新请求。
## HRegionServer的作用
- 维护master分配给他的region，处理对这些region的io请求。
- 负责切分正在运行过程中变的过大的region。
注意：client访问hbase上的数据时不需要master的参与，因为数据寻址访问zookeeper和region server，而数据读写访问region server。master仅仅维护table和region的元数据信息，而table的元数据信息保存在zookeeper上，因此master负载很低。
## HRegion
table在行的方向上分隔为多个Region。Region是HBase中分布式存储和负载均衡的最小单元，即不同的region可以分别在不同的Region Server上，但同一个Region是不会拆分到多个server上。
Region按大小分隔，每个表一般是只有一个region。随着数据不断插入表，region不断增大，当region的某个列族达到一个阈值（默认256M）时就会分成两个新的region。
每个region由以下信息标识：
- < 表名,startRowkey,创建时间>
- 由目录表(-ROOT-和.META.)记录该region的endRowkey
Region被分配给哪个Region Server是完全动态的，所以需要机制来定位Region具体在哪个region server。 
下面我们来看看Region是如何被定位的。
### HRegion定位
![](http://i.imgur.com/yONmFt9.jpg)
- 通过zk里的文件/hbase/rs得到-ROOT-表的位置。-ROOT-表只有一个region。
- 通过-ROOT-表查找.META.表的第一个表中相应的region的位置。其实-ROOT-表是.META.表的第一个region；.META.表中的每一个region 
在-ROOT-表中都是一行记录。
- 通过.META.表找到所要的用户表region的位置。用户表中的每个region在.META.表中都是一行记录。
-ROOT-表永远不会被分隔为多个region，保证了最多需要三次跳转，就能定位到任意的region。client会将查询的位置 信息保存缓存起来，缓存不会主动失效，因此如果client上的缓存全部失效，则需要进行6次网络来回，才能定位到正确的region，其中三次用来发现 缓存失效，另外三次用来获取位置信息。
提示：
-ROOT-表：表包含.META.表所在的region列表，该表只有一个Region;Zookeeper中记录了-ROOT-表的location
.META.表：表包含所有的用户空间region列表，以及Region Server的服务器地址
## Store
每一个region由一个或多个store组成，至少是一个store，hbase会把一起访问的数据放在一个store里面，即为每个 ColumnFamily建一个store，如果有几个ColumnFamily，也就有几个Store。一个Store由一个memStore和0或者 多个StoreFile组成。 HBase以store的大小来判断是否需要切分region
## MemStore
memStore 是放在内存里的。保存修改的数据即keyValues。当memStore的大小达到一个阀值（默认64MB）时，memStore会被flush到文 件，即生成一个快照。目前hbase 会有一个线程来负责memStore的flush操作。
## StoreFile
memStore内存中的数据写到文件后就是StoreFile，StoreFile底层是以HFile的格式保存。
## HLog
HLog(WAL log)：WAL意为write ahead log，用来做灾难恢复使用，HLog记录数据的所有变更，一旦region server 宕机，就可以从log中进行恢复。
HLog文件就是一个普通的Hadoop Sequence File， Sequence File的value是key时HLogKey对象，其中记录了写入数据的归属信息，除了table和region名字外，还同时包括sequence number和timestamp，timestamp是写入时间，sequence number的起始值为0，或者是最近一次存入文件系统中的sequence number。 Sequence File的value是HBase的KeyValue对象，即对应HFile中的KeyValue。
## LogFlusher
前面提到，数据以KeyValue形式到达HRegionServer，将写入WAL之后，写入一个SequenceFile。看过去没问题，但是因为数 据流在写入文件系统时，经常会缓存以提高性能。这样，有些本以为在日志文件中的数据实际在内存中。 
这里，我们提供了一个LogFlusher的类。它调用 HLog.optionalSync(),后者根据 hbase.regionserver.optionallogflushinterval (默认是10秒)，定期调用Hlog.sync()。另外，HLog.doWrite()也会根据 
hbase.regionserver.flushlogentries (默认100秒)定期调用Hlog.sync()。Sync() 本身调用HLog.Writer.sync()，它由SequenceFileLogWriter实现。
## LogRoller
Log的大小通过$HBASE_HOME/conf/hbase-site.xml 的 hbase.regionserver.logroll.period 限制，默认是一个小时。所以每60分钟，会打开一个新的log文件。久而久之，会有一大堆的文件需要维护。首先，LogRoller调用 HLog.rollWriter()，定时滚动日志，之后，利用HLog.cleanOldLogs()可以清除旧的日志。它首先取得存储文件中的最大的 sequence number，之后检查是否存在一个log所有的条目的“sequence
 number”均低于这个值，如果存在，将删除这个log。 每个region server维护一个HLog，而不是每一个region一个，这样不同region（来自不同的table）的日志会混在一起，这样做的目的是不断追加 单个文件相对于同时写多个文件而言，可以减少磁盘寻址次数，因此可以提高table的写性能。带来麻烦的时，如果一个region server下线，为了恢复其上的region，需要将region server上的log进行拆分，然后分发到其他region server上进行恢复。
# HBase工作流程
目前为止，相信你已经了解了HBase组件的使用和作用，但你可能还不太清楚上面提及的这些HBase组件间是如何运作的，下面我们来看看HBase的工作流程。
![](http://i.imgur.com/66IsIM3.png)
## Client
首先当一个请求产生时，HBase Client使用RPC(远程过程调用)机制与HMaster和HRegionServer进行通信，对于管理类操作，Client与HMaster进行RPC;对于数据读写操作，Client与HRegionServer进行RPC。
## Zookeeper
HBase Client使用RPC(远程过程调用)机制与HMaster和HRegionServer进行通信，但如何寻址呢？由于Zookeeper中存储了-ROOT-表的地址和HMaster的地址，所以需要先到Zookeeper上进行寻址。
HRegionServer也会把自己以Ephemeral方式注册到Zookeeper中，使HMaster可以随时感知到各个HRegionServer的健康状态。此外，Zookeeper也避免了HMaster的单点故障。
## HMaster
当用户需要进行Table和Region的管理工作时，就需要和HMaster进行通信。HBase中可以启动多个HMaster,通过Zookeeper的Master Eletion机制保证总有一个Master运行。
- 管理用户对Table的增删改查操作
- 管理HRegionServer的负载均衡，调整Region的分布
- 在Region Split后，负责新Region的分配
- 在HRegionServer停机后，负责失效HRegionServer上的Regions迁移
## HRegionServer
当用户需要对数据进行读写操作时，需要访问HRegionServer。HRegionServer存取一个子表时，会创建一个HRegion对象，然后对表的每个列族创建一个Store实例，每个Store都会有一个 MemStore和0个或多个StoreFile与之对应，每个StoreFile都会对应一个HFile， HFile就是实际的存储文件。因此，一个HRegion有多少个列族就有多少个Store。 一个HRegionServer会有多个HRegion和一个HLog。
当HStore存储是HBase的核心了，其中由两部分组成：MemStore和StoreFiles。 MemStore是Sorted Memory Buffer,用户 
写入数据首先 会放在MemStore,当MemStore满了以后会Flush成一个 StoreFile（实际存储在HDHS上的是HFile），当StoreFile文件数量增长到一定阀值，就会触发Compact合并操作，并将多个StoreFile合并成一个StoreFile，合并过程中会进行版本合并和数据删除，因此可以看出HBase其实只有增加数据，所有的更新和删除操作都是在后续的compact过程中进行的，这使得用户的 读写操作只要进入内存中就可以立即返回，保证了HBase
 I/O的高性能。
下面以一个具体数据Put的流程，让我们加深对HBase工作流程的认识。
### HBase Put流程
下面是put流程的时序图：
![](http://i.imgur.com/Nqa4LYq.jpg)
客户端：
- 客户端发起Put写请求，讲put写入writeBuffer，如果是批量提交，写满缓存后自动提交
- 根据rowkey将put吩咐给不同regionserver
服务端：
- RegionServer将put按rowkey分给不同的region
- Region首先把数据写入wal
- wal写入成功后，把数据写入memstore
- Memstore写完后，检查memstore大小是否达到flush阀值
- 如果达到flush阀值，将memstore写入HDFS，生成HFile文件
## HBase Compact &&Split
当StoreFile文件数量增长到一定阀值，就会触发Compact合并操作，并将多个StoreFile合并成一个StoreFile，当这个StoreFile大小超过一定阀值后，会触发Split操作，同时把当前Region Split成2个Region,这是旧的Region会下线，新Split出的2个Region会被HMaster分配到相应的HregionServer上，使得原先1个Region的压力得以分散到2个Region上。
如下图四个Storefile文件（从memstore文件经过flush而得到，默认64M的storefile文件）经过Compact合并成一个大的256M storefile文件，当设定的Region阀值为128M时，就会Split为两个128M的Storefile文件，然后HMaster再把这两个storefile文件分配到不停地Regionserver上。
![](http://i.imgur.com/cg3ZTgr.jpg)
## HFile
HBase中所有的数据文件都存储在Hadoop HDFS上，主要包括两种文件类型：
- Hfile:HBase中KeyValue数据的存储格式，HFile是Hadoop的 二进制格式文件，实际上StoreFile就是对Hfile做了轻量级包装，即StoreFile底层就是HFile
- HLog File:HBase中WAL(write ahead log)的存储格式，物理上是Hadoop的Sequence File
HFile的存储格式如下：
![](http://i.imgur.com/SHEVXNr.png)
HFile文件是不定长的，长度固定的只有其中的两块：Trailer和FileInfo。
Trailer中有指针指向其他数据块的起始点，FileInfo记录了文件的一些meta信息。 Data Block是hbase io的基本单元，为了提高效率，HRegionServer中有基于LRU的block cache机制。 
每个Data块的大小可以在创建一个Table的时候通过参数指定（默认块大小64KB），大号的Block有利于顺序Scan，小号的 Block利于随机查询。 
每个Data块除了开头的Magic以外就是一个个KeyValue对拼接而成，Magic内容就是一些随机数字，目的是防止数 据损坏，结构如下。
![](http://i.imgur.com/UHhLMR7.png)
上图可知，开始是两个固定长度的数值，分别表示key的长度和alue的长度。紧接着是Key,开始是固定长度的数值，表示RowKey的长度，紧接着是RowKey，然后是固定长度的数值，表示Family的长度，然后是Family,接着是Qualifier，然后是两个固定长度的数值，表示Time Stamp和Key Type(Put/Delete)。Value部分没有那么复杂的结构，就是纯粹的二进制数据。
## HBase的三维有序（即字典顺序）存储
Hfile是HBase中KeyValue数据的存储格式。从上面的 HBase物理数据模型中可以看出，HBase是面向列表（簇）的存储。每个Cell由 {row key，column(=< family> + < label>)，version} 唯一确定的单元，他们组合在一起就是一个KeyValue。根据上述描述，这个KeyValue中的key就是{row key，column(=< family> + < label>)，version} ，而value就是cell中的值。
HBase的三维有序存储中的三维是指：rowkey（行主键），column key(columnFamily+< label>)，timestamp(时间戳或者版本号)三部分组成的三维有序存储。
- rowkey
rowkey是行的主键，它是以字典顺序排序的。所以 rowkey的设计是至关重要的，关系到你应用层的查询效率。我们在根据rowkey范围查询的时候，我们一般是知道startRowkey，如果我们通过scan只传startRowKey ： d开头的，那么查询的是所有比d大的都查了，而我们只需要d开头的数据，那就要通过endRowKey来限制。我们可以通过设定endRowKey为：d 开头，后面的根据你的rowkey组合来设定，一般是加比startKey大一位。
- column key
column key是第二维，数据按rowkey字典排序后，如果rowkey相同，则是根据column key来排序的，也是按字典排序。 
我们在设计table的时候要学会利用这一点。比如我们的收件箱。我们有时候需要按主题排序，那我们就可以把主题这设置为我们的column key，即设计为columnFamily+主题.,这样的设计。
- timestamp
timestamp 时间戳，是第三维，这是个按降序排序的，即最新的数据排在最前面。这个就没有什么说的了。网上其他的博客也提到比较多。
## HLog Replay
根据以上的叙述，我们已经了解了关于HStore的基本原理，但我们还必须要了解一下HLog的功能，因为上述的HStore在系统正常工作的前提下是没问题的，但是在分布式 系统环境中，无法避免系统出错或者宕机，因为一旦HRegionServer意外退出，MemStore中的内存数据将会丢失，这就需要引入HLog。每个HRegionServer中都有一个HLog对象，HLog是一个实现Write Ahead Log的类，在每一次用户操作写入MemStore的同时，也会写一份数据到HLog文件中，HLog文件定期(当文件已持久化到StoreFile中的数据)会滚出新的，并且删除旧的文件。当HRegionServer意外终止
 后，HMaster会通过Zookeeper感知到，HMaster首先会处理遗留的Hlog文件，将其中不同Region的Log数据进行拆分，分别放到相应Region的目录下，然后再将失效的Region重新分配，领取到这些Region的Regionserver在Load Region的过程中，会发现历史HLog需要处理，因此Replay HLog中的数据到MemStore中，然后flush到StoreFiles,完成数据恢复。
## HLog存储格式
WAL(Write Ahead Log):RegionServer在处理插入和删除过程中，用来记录操作内容的日志，只有日志写入成功，才会通知客户端操作成功。
![](http://i.imgur.com/k9D7z8y.jpg)
上图中是HLog文件的结构，其实HLog文件就是一个普通的Hadoop Sequence File，Sequence File的Key是HLogKey对象，HLogKey中记录了写入数据的归属信息，除了table和Region名字外，同时还包括sequence number和timestamp,timestamp是”写入时间”，sequence number 的起始值为0，或者是最近一次存入文件系统中的sequence number。
HLog Sequence File 的Value是HBase的KeyValue对象昂，即对应HFile中的KeyValue。
# HBase的高可用
![](http://i.imgur.com/SZ5OpkQ.jpg)
当出现上图三种情况的高可用策略：
- HDFS机架识别策略：当数据文件损坏时，会找相同机架上备份的数据文件，如果相同机架上的数据文件也损坏会找不同机架备份数据文件。
- HBase的Region快速恢复：当节点损坏时，节点上的丢失region,会在其他节点上均匀快速恢复。
- Master节点的HA机制：Master为一主多备。当Master主节点宕机后，剩下的备节点通过选举，产生主节点。
# HBase性能和优化
## 影响HBase性能的因素
![](http://i.imgur.com/DQYQgKT.jpg)
上图中，从HDFS以下都属于HBase的支撑系统。
从构成集群机器的底层硬件到把硬件和操作系统(尤其是文件系统)，JVM,HDFS连接起来的网络之间的所有部件都会影响到HBase的性能。HBase系统的状态也会影响到HBase的系统。例如，在集群中执行合并的时候或者Memstore刷写的时候与什么都没有做的时候相比，其性能表现是不同的。应用系统的性能还取决于它和HBase的交互方式，所以模式设计和其他环节一样起到了必不可少的作用。
在评判HBase性能时，所有这些因素都有影响；在优化集群时，需要查看所有这些因素。
## 优化HBase支撑系统
(1)硬件选择
总的原则是，根据业务情况和集群规模大小选择合理的硬件。
(2)网络配置
基于当前阶段硬件的典型分布式系统都会受到网络的限制，HBase也不例外。在节点和机架顶置交换机之间建议采用10Gb以太网交换机。千万不要过于满额配置地使用网络，否则在高负载时，会影响HBase应用系统的性能。
(3)操作系统
一般情况下，只要使用Hadoop和HBase，操作系统通常选择Linux。可以选择Red Hat Enterprise Linux，CentOS，也可以选择成功部署过的其他操作系统。
(4)本地文件系统
本地Linux文件系统在HBase集群体系里起到了重要作用，并且严重影响到HBase的性能。虽然EXT4是推荐使用的本地文件系统，但没有大规模使用，相反EXT3和XFS已经在生产系统里得到成功使用，建议使用EXT3和XFS作为本地文件系统。
(5)HDFS
## 根据业务访问特点优化
根据业务访问特点，将Hbase的工作负载大致分为以下四类：
(1)随机读密集型
对于随机读密集型工作负载，高效利用缓存和更好地索引会给HBase系统带来更高的性能。
(2)顺序读密集型
对于顺序读密集型工作负载，读缓存不会带来太多好处；除非顺序读的规模很小并且限定在一个特定的行键范围内，否则很可能使用缓存会比不使用缓存需要更频繁地访问硬盘。
(3)写密集型
写密集型工作负载的优化方法需要有别于读密集型负载。缓存不再起到重要作用。写操作总是进入MemStore，然后被刷写生成新的Hfile，以后再被合并。获得更好写性能的办法是不要太频繁刷写、合并或者拆分，因为在这段时间里IO压力上升，系统会变慢。
(4)混合型
对于完全混合型工作负载，优化方法会变得复杂些。优化时，需要混合调整多个参数来得到一个最优的组合。
## 其它角度来优化HBase性能
(1)JVM垃圾回收优化
(2)本地memstore分配缓存优化
(3)Region拆分优化
(4**)Region合并优化**
(5)Region预先加载优化
(6)负载均衡优化
(7)启用压缩
(8)GZIP、snappy、lzo，推荐snappy,压缩比稍差于lzo；但是压缩速度高于lzo,并且与lzo有差不多的 解压缩速度。
（9）进行预分区，从而避免自动split，提高hbase响应速度
（10）避免出现region热点现象，启动按照table级别进行balance
## HBase常见的调优参数
![](http://i.imgur.com/CMJchjF.jpg)
![](http://i.imgur.com/CjRYD2o.jpg)
![](http://i.imgur.com/h9175Pe.jpg)
# HBase shell访问
HBase Shell 提供了大多数的 HBase 命令, 通过 HBase Shell 用户可以方便地创建、删除及修改表, 还可以向表中添加数据、列出表中的相关信息等。
在启动 HBase 之后,用户可以通过下面的命令进入 HBase Shell 之中,命令如下所示:
```
[hadoop@CDHNode1 ~]$hbase shell
```
输入 help 可以看到命令分组。（注意命令都是小写，有大小写的区分）
![](http://i.imgur.com/fUJoJHf.jpg)
部分命令
![](http://i.imgur.com/LYj2b0Q.jpg)
下边分组举例 Shell 的各种操作。
## general操作
查询 HBase 服务器状态 status。
查询hbase版本 version
## ddl操作
1、 创建一个表
create ‘table1’, ‘tab1_id’, ‘tab1_add’, ‘tab1_info’
2、 列出所有的表
![](http://i.imgur.com/7lLGBhR.jpg)
3、 获得表的描述
describe “table1”
![](http://i.imgur.com/UVTvRoz.jpg)
4、 删除一个列族 disable alter enable 注意删除前，需要先把表disable
disable ‘table1’
alter ‘table1’, {NAME=>’tab1_add’, METHOD=>’delete’}
enable ‘table1’
5、 查看表是否存在 
exists ‘table2’
![](http://i.imgur.com/NkHeLQm.jpg)
6、 判断表是否为‘enable’
is_enabled ‘table1’
7、 删除一个表
disable ‘table1’
drop ‘table1’
## dml操作
1、 插入几条记录
put ‘member’, ‘scutshuxue’, ‘info:age’, ‘24’
put ‘member’, ‘scutshuxue’, ‘info:birthday’, ‘1987-06-17’
put ‘member’, ‘scutshuxue’, ‘info:company’, ‘alibaba’
put ‘member’, ‘scutshuxue’, ‘address:contry’, ‘china’
put ‘member’, ‘scutshuxue’, ‘address:province’, ‘zhejiang’
put ‘member’, ‘scutshuxue’, ‘address:city’, ‘hangzhou’
put命令比较简单，只有这一种用法： 
hbase> put ‘t1′, ‘r1′, ‘c1′, ‘value’, ts1
t1指表名，r1指行键名，c1指列名，value指单元格值。ts1指时间戳，一般都省略掉了。
2、 全表扫描 scan
![](http://i.imgur.com/85BuoSl.jpg)
3、 获得数据 get
1) 获得一行的所有数据
![](http://i.imgur.com/wC9i6f3.jpg)
2) 获得某行，某列族的所有数据
![](http://i.imgur.com/0eS9Oy0.jpg)
3) 获得某行，某列族，某列的所有数据
![](http://i.imgur.com/5dhf6ub.jpg)
4、 更新一条记录 put（把scutshuxue年龄改为99）
put ‘member’, ‘scutshuxue’, ‘info:age’, 99
5、 删除 delete、 deleteall
1) 删除行’scutshuxue’, 列族为‘info’ 中age的值
delete ‘member’, ‘scutshuxue’, ‘info:age’
2) 删除整行
deleteall ‘member’, ‘scutshuxue’
6、 查询表中有多少行
count ‘member’
7、 给‘xiaoming’这个id增加’info:age’字段，并使用counter实现递增
incr ‘member’, ‘xiaoming’, ‘info:age’
8、 将整个表清空
truncate ‘member’
![](http://i.imgur.com/bdsEKMn.jpg)
可以看出，HBase 是通过先对表执行 disable，然后再执行 drop 操作后重建表来实现 truncate 的功能的。
以上是学习HBase后，整理的HBase概述，如有错误，请指正！
