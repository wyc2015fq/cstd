# HBase体系结构(转) - z69183787的专栏 - CSDN博客
2016年10月22日 21:33:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：483
个人分类：[分布式-Zookeeper2](https://blog.csdn.net/z69183787/article/category/6476923)

HBase的服务器体系结构遵循简单的主从服务器架构，它由HRegion服务器（HRegion Server）群和HBase Master服务器（HBase Master Server)构成。HBase Master服务器负责管理所有的HRegion服务器，而HBase中所有的服务器都是通过ZooKeeper来进行协调，并处理HBase服务器运行期间可能遇到的错误。HBase Master Server本身不存储HBase中的任何数据，HBase逻辑上的表可能会被划分为多个HRegion，然后存储到HRegion
 Server群中，HBase Master Server中存储的是从数据到HRegion Server中的映射。
![](http://images.cnitblog.com/blog/360373/201310/21103715-5416c6300c7944f098e49f7723ec7e36.png)
## Client
HBase Client使用HBase的RPC机制与HMaster和HRegionServer进行通信，对于管理类操作，Client与HMaster进行RPC；对于数据读写类操作，Client与HRegionServer进行RPC
## Zookeeper
Zookeeper Quorum中除了存储了-ROOT-表的地址和HMaster的地址，HRegionServer也会把自己以Ephemeral方式注册到Zookeeper中，使得HMaster可以随时感知到各个HRegionServer的健康状态。此外，Zookeeper也避免了HMaster的单点问题，见下文描述
## HBase Master服务器
　　每台HRegion服务器都会和HMaster服务器通信，HMaster的主要任务就是要告诉每台HRegion服务器它要维护哪些HRegion。
　　当一台新的HRegion服务器登录到HMaster服务器时，HMaster会告诉它先等待分配数据。而当一台HRegion死机时，HMaster会把它负责的HRegion标记为未分配，然后再把它们分配到其他HRegion服务器中。
## HRegion服务器
HRegionServer主要负责响应用户I/O请求，向HDFS文件系统中读写数据，是HBase中最核心的模块。
    所有的数据库数据一般是保存在Hadoop分布式文件系统上面的，用户通过一系列HRegion服务器来获取这些数据，一台机器上面一般只运行一个HRegion服务器，且每一个区段的HRegion也只会被一个HRegion服务器维护。
　　当用户需要更新数据的时候，他会被分配到对应的HRegion服务器上提交修改，这些修改显示被写到Hmemcache（内存中的缓存，保存最近更新的数据）缓存和服务器的Hlog（磁盘上面的记录文件，他记录着所有的更新操作）文件里面。在操作写入Hlog之后，commit()调用才会将其返回给客户端。
　　在读取数据的时候，HRegion服务器会先访问Hmemcache缓存，如果缓存里没有改数据，才会回到Hstores磁盘上面寻找，每一个列族都会有一个HStore集合，每一个HStore集合包含很多HstoreFile文件，如下图：
![](http://images.cnitblog.com/blog/360373/201310/21103732-a31b01647de0446a90ca4138d4d22eea.png)
    HStore存储是HBase存储的核心了，其中由两部分组成，一部分是MemStore，一部分是StoreFiles。MemStore是Sorted Memory Buffer，用户写入的数据首先会放入MemStore，当MemStore满了以后会Flush成一个StoreFile（底层实现是HFile），当StoreFile文件数量增长到一定阈值，会触发Compact合并操作，将多个StoreFiles合并成一个StoreFile，合并过程中会进行版本合并和数据删除，因此可以看出HBase其实只有增加数据，所有的更新和删除操作都是在后续的compact过程中进行的，这使得用户的写操作只要进入内存中就可以立即返回，保证了HBase
 I/O的高性能。当StoreFiles Compact后，会逐步形成越来越大的StoreFile，当单个StoreFile大小超过一定阈值后，会触发Split操作，同时把当前Region Split成2个Region，父Region会下线，新Split出的2个孩子Region会被HMaster分配到相应的HRegionServer上，使得原先1个Region的压力得以分流到2个Region上。
## HRegion
　　当表的大小超过设置值的是偶，HBase会自动地将表划分为不同的区域，每个区域包含所有行的一个子集。对用户来说，每个表是一堆数据的集合，靠主键来区分。从物理上来说，一张表被拆分成了多块，每一块就是一个HRegion。我们用表名+开始/结束主键来区分每一个HRegion，一个HRegion会保存一个表里某段连续的数据，从开始主键到结束主键，一张完整的表是保存在多个HRegion上面的。
## HBase存储格式
HBase中的所有数据文件都存储在Hadoop HDFS文件系统上，主要包括上述提出的两种文件类型：
1.       HFile， HBase中KeyValue数据的存储格式，HFile是Hadoop的二进制格式文件，实际上StoreFile就是对HFile做了轻量级包装，即StoreFile底层就是HFile
2.       HLog File，HBase中WAL（Write Ahead Log） 的存储格式，物理上是Hadoop的Sequence File
HFile已经介绍了，接下来就是介绍HLog File。
在分布式系统环境中，无法避免系统出错或者宕机，因此一旦HRegionServer意外退出，MemStore中的内存数据将会丢失，这就需要引入HLog了。每个HRegionServer中都有一个HLog对象，HLog是一个实现Write Ahead Log的类，在每次用户操作写入MemStore的同时，也会写一份数据到HLog文件中（HLog文件格式见后续），HLog文件定期会滚动出新的，并删除旧的文件（已持久化到StoreFile中的数据）。当HRegionServer意外终止后，HMaster会通过Zookeeper感知到，HMaster首先会处理遗留的
 HLog文件，将其中不同Region的Log数据进行拆分，分别放到相应region的目录下，然后再将失效的region重新分配，领取 到这些region的HRegionServer在Load Region的过程中，会发现有历史HLog需要处理，因此会Replay HLog中的数据到MemStore中，然后flush到StoreFiles，完成数据恢复。
### HFile
下图是HFile的存储格式：
![](http://images.cnitblog.com/blog/360373/201310/21110934-a7927f19d9bc4be186a2bace0fe815a5.jpg)
首先HFile文件是不定长的，长度固定的只有其中的两块：Trailer和FileInfo。正如图中所示的，Trailer中有指针指向其他数据块的起始点。File Info中记录了文件的一些Meta信息，例如：AVG_KEY_LEN, AVG_VALUE_LEN, LAST_KEY, COMPARATOR, MAX_SEQ_ID_KEY等。Data Index和Meta Index块记录了每个Data块和Meta块的起始点。
Data Block是HBase I/O的基本单元，为了提高效率，HRegionServer中有基于LRU的Block Cache机制。每个Data块的大小可以在创建一个Table的时候通过参数指定，大号的Block有利于顺序Scan，小号Block利于随机查询。每个Data块除了开头的Magic以外就是一个个KeyValue对拼接而成, Magic内容就是一些随机数字，目的是防止数据损坏。后面会详细介绍每个KeyValue对的内部构造。
HFile里面的每个KeyValue对就是一个简单的byte数组。但是这个byte数组里面包含了很多项，并且有固定的结构。我们来看看里面的具体结构：
![](http://images.cnitblog.com/blog/360373/201310/21110957-bf7f56599e954cd6ab806e163812a801.jpg)
开始是两个固定长度的数值，分别表示Key的长度和Value的长度。紧接着是Key，开始是固定长度的数值，表示RowKey的长度，紧接着是RowKey，然后是固定长度的数值，表示Family的长度，然后是Family，接着是Qualifier，然后是两个固定长度的数值，表示Time Stamp和Key Type（Put/Delete）。Value部分没有这么复杂的结构，就是纯粹的二进制数据了。
### HLogFile
![](http://images.cnitblog.com/blog/360373/201310/21110948-12fbf40567ac44388e3102260f0e7b82.jpg)
上图中示意了HLog文件的结构，其实HLog文件就是一个普通的Hadoop Sequence File，Sequence File 的Key是HLogKey对象，HLogKey中记录了写入数据的归属信息，除了table和region名字外，同时还包括 sequence number和timestamp，timestamp是“写入时间”，sequence number的起始值为0，或者是最近一次存入文件系统中sequence number。
HLog Sequece File的Value是HBase的KeyValue对象，即对应HFile中的KeyValue，可参见上文描述。
参考  [http://www.cnblogs.com/NicholasLee/archive/2012/09/13/2683223.html](http://www.cnblogs.com/NicholasLee/archive/2012/09/13/2683223.html)
[http://www.searchtb.com/2011/01/understanding-hbase.html](http://www.searchtb.com/2011/01/understanding-hbase.html)
