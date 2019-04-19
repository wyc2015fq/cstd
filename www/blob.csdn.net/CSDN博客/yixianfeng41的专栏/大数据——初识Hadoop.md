# 大数据——初识Hadoop - yixianfeng41的专栏 - CSDN博客
2016年10月11日 11:26:43[第2梦](https://me.csdn.net/yixianfeng41)阅读数：3421
### **一、Hadoop生态系统云计算架构**
![](http://i.imgur.com/sdUyl7w.jpg)
Hadoop云计算架构
**主要包含如下核心组件：**
- HDFS。它是hadoop使用的分布式文件系统，提供了hadoop运算过程中的数据存储、数据备份、数据错误校验等功能。  
- MapReduce。 它是hadoop的并行计算框架。基于它写出来的应用程序能够运行在由上千个商用机器组成的大型集群上，并以一种可靠容错的方式并行处理上TB级别的数据集。
- HBSE。HBse是建立在Hadoop HDFS之上的可靠的、可扩展的分布式数据集库，是一个高可靠性、高性能、面向列（簇）、可伸缩的分布式数据库。
- Hive。Hive是基于Hadoop的一个数据仓库工具，处理能力强而且成本低廉。
- Zookeeper。Zookeeper是一个针对大型分布式的可靠协调系统，主要提供配置服务、名字服务、分布式同步、组服务等功能。它的目标是封装好复杂且容易出错的关键服务，提供给用户简洁易用的接口。
- Pig。Pig是基于hadoop的大规模数据分析平台，pig提供的SQL-Like语言叫做Pig Latin,该语言的编译器会将类sql的数据请求转换为一些列经过优化处理的mapreduce运算。
### **二、Hadoop集群架构**
![](http://i.imgur.com/qS1xneM.jpg)
Hadoop集群架构
Hadoop集群由一个Master主节点和若干个Slave节点组成。其中，Master节点上运行NameNode和JobTracker守护进程；Slave节点上运行DataNode和TaskTracker守护进程。 
Hadoop分别从三个角度将集群中的主机划分为两种角色：
![](http://i.imgur.com/ceKoNyL.jpg)
Hadoop集群主机角色划分
- 从主机服务角度。 
  从主机服务功能上将集群中的主机分为Master和Slave。Master主机负责分布式文件系统的元数据管理和分布式计算的作业调度，一般在一个集群中有一到数台Master主机；Slave主机负责具体的数据存储和计算。
- 从文件系统HDFS的角度 
  从文件系统HDFS的的角度将主机划分为NameNode和DatanNode。Namenode存储分布式文件系统中的元数据信息；DataNode存储文件系统中实际文件数据
- 从MapReduce角度 
  从MapReduce角度将主机划分为JobTracker和TaskTracker。JobTracker是作业的调度与管理者，属于master;TaskTracker是任务的实际执行者，属于slave。
### **三、HDFS**
#### **3.1、HDFS简介**
  HDFS是Hadoop使用的标准存储系统，是基于网络环境下的分布式文件系统。存储在HDFS上的数据文件首先进行分块，每个分块创建多个副本，并存储在集群的不同节点上，Hadoop MapReduce程序可以在所有节点上处理这些数据。HDFS上数据存储和处理过程如下所示。
![](http://i.imgur.com/3rqnwZF.jpg)
Hadoop上的数据存储和处理模型
#### **3.2、HDFS的两个主要组件**
  HDFS的设计遵从主从体系结构，每个HDFS集群都有一个名字节点（NameNode)和若干数据节点（DataNode)。HDFS存储文件的机制是将数据文件分块，然后将这些数据块按照一定的策略存放在数据节点上。NameNode节点管理文件系统的命名空间、文件、目录操作，同时也负责确定数据节点和文件块的映射关系。DataNode节点负责来自客户端的文件读/写请求，同时还要执行块的创建、删除以及来自名字节点的文件和块的操作命令。 
  NameNode和DataNode在功能上的区别：  
Hadoop上的数据存储和处理模型
![](http://i.imgur.com/uAlYr9k.jpg)
#### **3.3、HDFS的系统架构**
HDFS的架构如图所示:  
![](http://i.imgur.com/2nAHLQF.gif)
HDFS的架构
#### **3.4、HDFS读取文件流程**
HDFS中客户端读取文件的流程如下所示:
![](http://i.imgur.com/FiqhneR.jpg)
HDFS读取文件流程
**读取操作的具体流程如下：**
1. 客户端(client)用FileSystem的open()函数打开文件。 
2. DistributedFileSystem用RPC和NameNode通信，得到文件的所有数据块信息以及这些数据块所在节点的地址信息。 
3. DistributedFileSystem将获取的数据块信息保存在FSDataInputStream中，然后返回给客户端，用来读取数据。 
4. 客户端根据NameNode返回的信息，连接到存有数据块的数据节点，然后调用stream的read()函数开始读取数据。 
5. DFSInputStream连接保存此文件第一个数据块的最近的数据节点。 
6. Data从数据节点读到客户端(client)。 
7. 当此数据块读取完毕时，DFSInputStream关闭和此数据节点的连接，然后连接此文件下一个数据块的最近的数据节点。 
8. 当客户端读取完毕数据的时候，调用FSDataInputStream的close函数。 
9. 在读取数据的过程中，如果客户端在与数据节点通信出现错误，则尝试连接包含此数据块的下一个数据节点。 
10. 失败的数据节点将被记录，以后不再连接。
#### **3.5、HDFS写入文件流程**
HDFS写入文件流程如下所示。
![](http://i.imgur.com/Ec1MFlO.jpg)
HDFS写入文件流程
**写入操作的具体流程如下：**
1. 客户端调用create()来创建文件 
2. DistributedFileSystem用RPC调用元数据节点，在文件系统的命名空间中创建一个新的文件。 
3. 元数据节点首先确定文件原来不存在，并且客户端有创建文件的权限，然后创建新文件。 
4. DistributedFileSystem返回DFSOutputStream，客户端用于写数据。 
5. 客户端开始写入数据，DFSOutputStream将数据分成块，写入data queue。 
6. Data queue由Data Streamer读取，并通知元数据节点分配数据节点，用来存储数据块(每块默认复制3块)。分配的数据节点放在一个pipeline里。 
7. Data Streamer将数据块写入pipeline中的第一个数据节点。第一个数据节点将数据块发送给第二个数据节点。第二个数据节点将数据发送给第三个数据节点。 
8. DFSOutputStream为发出去的数据块保存了ack queue，等待pipeline中的数据节点告知数据已经写入成功。 
 如果数据节点在写入的过程中失败： 
 关闭pipeline，将ack queue中的数据块放入data queue的开始。 
 当前的数据块在已经写入的数据节点中被元数据节点赋予新的标示，则错误节点重启后能够察觉其数据块是过时的，会被删除。 
 失败的数据节点从pipeline中移除，另外的数据块则写入pipeline中的另外两个数据节点。 
 元数据节点则被通知此数据块是复制块数不足，将来会再创建第三份备份。 
9. 当客户端结束写入数据，则调用stream的close函数。此操作将所有的数据块写入pipeline中的数据节点，并等待ack queue返回成功。最后通知元数据节点写入完毕。
### **四、MapReduce**
#### **4.1、MapReduce作业执行过程中涉及到的实体**
- 客户端 
作用是提交MapReduce作业到集群的计算机主节点JobTracker上。客户端可以是集群上的节点也可以不是集群上的节点，从集群外的节点提交作业时需要制定JobTRacker的地址。
- 计算主节点JobTracker 
JobTracker 的功能主要是负责作业的任务分配，计算任务的管理和监控。
- 从节点TaskTracker 
运行JobTracker分配给他的任务。
- HDFS 
计算开始的时候，程序从HDFS中读入数据；计算结束后，将数据保存到HDFS中。
#### **4.2、MapReduce的工作流程**
整个MapReduce作业的工作工程，如下所示：
![](http://i.imgur.com/2yY8C6E.png)
MapReduce工作流程
**1. 作业的提交**
客户端调用JobClient的runjob()方法创建一个 JobClient实例，然后调用submitJob()方法提交作业到集群，其中主要包括以下步骤： 
1）通过JobTracker的getNewJobId()请求一个新的作业ID； 
2）检查作业的输出（比如没有指定输出目录或输出目录已经存在，就抛出异常），以免覆盖原有文件内容； 
3）计算作业的输入分片，为了高效，分片大小最好和HDFS块的大小相同（当分片无法计算时，比如输入路径不存在等原因，就抛出异常）； 
4）将运行作业所需的资源（比如作业Jar文件，配置文件，计算所得的输入分片等）复制到一个以作业ID命名的目录中。（集群中有多个副本可供TaskTracker访问）； 
5）通过调用JobTracker的submitJob()方法告知作业准备执行； 
6）JobTracker调度任务到工作节点上执行。runjob每隔1秒轮训一次节点，看看执行进度，指导任务执行完毕。  
**2. 作业的初始化**
1）JobTracker接收到对其submitJob()方法的调用后，就会把这个调用放入一个内部队列中，交由作业调度器（比如先进先出调度器，容量调度器，公平调度器等）进行调度； 
2）初始化主要是创建一个表示正在运行作业的对象——封装任务和记录信息，以便跟踪任务的状态和进程； 
3）为了创建任务运行列表，作业调度器首先从HDFS中获取JobClient已计算好的输入分片信息； 
4）然后为每个分片创建一个MapTask，并且创建ReduceTask。（Task在此时被指定ID，请区分清楚Job的ID和Task的ID）。 
**3. 任务的分配**
1）TaskTracker定期通过“心跳”与JobTracker进行通信，主要是告知JobTracker自身是否还存活，以及是否已经准备好运行新的任务等； 
2）JobTracker在为TaskTracker分配一个Task前，JobTracker需要按照优先级选择一个作业，在最高优先级的job中选择一个task。Hadoop默认的作业调度Map Task优先级比Reduce Task高； 
3）TaskTracker根据一定的策略运行一定数量的map task和reduce task。可运行的数量有TaskTracker的数量和内存大小决定
**4. 任务的执行**
1）TaskTracker分配到一个任务后，通过从HDFS把作业的Jar文件复制到TaskTracker所在的文件系统（Jar本地化用来启动JVM），同时TaskTracker将应用程序所需要的全部文件从分布式缓存复制到本地磁盘； 
2）TaskTracker为任务新建一个本地工作目录，并把Jar文件中的内容解压到这个文件夹中； 
3）TaskTracker启动一个新的JVM来运行每个Task（包括MapTask和ReduceTask），这样Client的MapReduce就不会影响TaskTracker守护进程（比如，导致崩溃或挂起等）。 
子进程通过umbilical接口与父进程进行通信，Task的子进程每隔几秒便告知父进程它的进度，直到任务完成。
**5. 进程和状态的更新**
一个作业和它的每个任务都有一个状态信息，包括作业或任务的运行状态，Map和Reduce的进度，计数器值，状态消息或描述（可以由用户代码来设置）。这些状态信息在作业期间不断改变，它们是如何与Client通信的呢？
![](http://i.imgur.com/1hTGxHj.png)
任务在运行时，对其进度（即任务完成的百分比）保持追踪。对于MapTask，任务进度是已处理输入所占的比例。对于ReduceTask，情况稍微有点复杂，但系统仍然会估计已处理Reduce输入的比例；
这些消息通过一定的时间间隔由Child JVM—>TaskTracker—>JobTracker汇聚。JobTracker将产生一个表明所有运行作业及其任务状态的全局视图。可以通过Web UI查看。同时JobClient通过每秒查询JobTracker来获得最新状态，并且输出到控制台上。
#### **4.3、Shuffle和Sort**
Shuffle阶段是指从Map的输出开始，包括系统执行排序以及传送Map输出到Reduce作为输入的过程。Sort阶段是指对Map端输出的Key进行排序的过程。不同的Map可能输出相同的Key，相同的Key必须发送到同一个Reduce端处理。Shuffle阶段可以分为Map端的Shuffle和Reduce端的Shuffle。Shuffle阶段和Sort阶段的工作过程，如下所示：
![](http://i.imgur.com/6HI2GjD.png)
**1、Map端的Shuffle**
Map函数开始产生输出时，并不是简单地把数据写到磁盘，因为频繁的磁盘操作会导致性能严重下降。它的处理过程更复杂，数据首先写到内存中的一个缓冲区，并做一些预排序，以提升效率；
每个MapTask都有一个用来写入输出数据的循环内存缓冲区（默认大小为100MB），当缓冲区中的数据量达到一个特定阈值时（默认是80%）系统将会启动一个后台线程把缓冲区中的内容写到磁盘（即spill阶段）。在写磁盘过程中，Map输出继续被写到缓冲区，但如果在此期间缓冲区被填满，那么Map就会阻塞直到写磁盘过程完成；
在写磁盘前，线程首先根据数据最终要传递到的Reducer把数据划分成相应的分区（partition）。在每个分区中，后台线程按Key进行排序（快速排序），如果有一个Combiner（即Mini Reducer）便会在排序后的输出上运行；
一旦内存缓冲区达到溢出写的阈值，就会创建一个溢出写文件，因此在MapTask完成其最后一个输出记录后，便会有多个溢出写文件。在在MapTask完成前，溢出写文件被合并成一个索引文件和数据文件（多路归并排序）（Sort阶段）；
溢出写文件归并完毕后，Map将删除所有的临时溢出写文件，并告知TaskTracker任务已完成，只要其中一个MapTask完成，ReduceTask就开始复制它的输出（Copy阶段）；
Map的输出文件放置在运行MapTask的TaskTracker的本地磁盘上，它是运行ReduceTaskTaskTracker所需要的输入数据，但是Reduce输出不是这样的，它一般写到HDFS中（Reduce阶段）。
**2、Reduce端的Shuffle**
Copy阶段：Reduce进程启动一些数据copy线程，通过HTTP方式请求MapTask所在的TaskTracker以获取输出文件。  
Merge阶段：将Map端复制过来的数据先放入内存缓冲区中，Merge有3种形式，分别是内存到内存，内存到磁盘，磁盘到磁盘。默认情况下第一种形式不启用，第二种Merge方式一直在运行（spill阶段）直到结束，然后启用第三种磁盘到磁盘的Merge方式生成最终的文件。  
Reduce阶段：最终文件可能存在于磁盘，也可能存在于内存中，但是默认情况下是位于磁盘中的。当Reduce的输入文件已定，整个Shuffle就结束了，然后就是Reduce执行，把结果放到HDFS中。
### ** 五、HBase**
#### **5.1、HBase表的特点**
1、大：一个表可以有数十亿行，上百万列； 
2、无模式：每行都有一个可排序的主键和任意多的列，列可以根据需要动态的增加，同一张表中不同的行可以有截然不同的列； 
3、面向列：面向列（族）的存储和权限控制，列（族）独立检索； 
稀疏：空（null）列并不占用存储空间，表可以设计的非常稀疏； 
4、数据多版本：每个单元中的数据可以有多个版本，默认情况下版本号自动分配，是单元格插入时的时间戳； 
5、数据类型单一：Hbase中的数据都是字符串，没有类型。
#### **5.2、HBase的物理集群架构**
HBase集群在物理组成上由一个Zookeeper集群、一个Master主服务器、多个RegionServer从服务器组成，同时，HBse在底层依赖于HDFS集群。整体系统架构如下所示：
![](http://i.imgur.com/zTfp6J6.png)
HBase的物理集群架构
HBase集群运行依赖于Zookeeper，默认情况下，它管理一个Zookeeper实例，作为集群“权威”。如果区域分配过程中有服务器崩溃，就需要Zookeeper来协调分配。客户端读写HBase中的数据时也需要先访问Zookeeper，了解集群属性。
在底层，HBase的所有信息都保存在HDFS中，HBase是构建在HDFS上的分布式数据库。
#### ** 5.3、HBase的存储架构**
HBase的存储架构如下所示：
![](http://i.imgur.com/ghzgGg7.jpg)
HBase的存储架构
HBASE集群种重要的角色就是其守护进程。包括运行在集群Master节点上的HMaster进程和运行在每个从节点RegionServer节点上的HRegionServer进程，初次之外，还包括若干Zookeeper进程。具体组件实体如下： 
**1、Client**
HBse Client使用HBase的RPC机制与HMaster（进行管理操作）和HRegionServer（进行读写操作）进行通信。  
**2、HMaster**
HMaster是集群Master节点的主进程，HMaster没有单点问题，HBase中可以启动多个HMaster，通过Zookeper的Master Election机制保证总有一个Master运行，HMaster在功能上主要负责Table和Region的管理工作。包括：Table的增删该查、Region分布等。 
**3、HRegionServer**
HRegionServer进程运行在集群从节点上，主要负责响应用户的IO请求，向HDFS系统中读写数据。其内部功能结构图如下：  
![](http://i.imgur.com/Jw0GtEH.jpg)
HRegionServer内部功能结构图
从图上可以看出，HRegionServer管理一系列HRegion对象，每一个HRegion又由多个HStore组成。HRegion对应Table中的一个Region;HStore对应Table中的一个列簇。每个列簇就是一个存储单元。 
HStore由两部分组成：MemStore和StoreFile。用户写入的数据首先会放入MemSore,MemStore满了之后，flush成一个StoreFile.
**4、Zookeeper **
Zookeeper中除了存储-ROOT表的地址和Master的地址，还会有HRegionServer的信息，使得Hmaster可以随时知道HRegionServer的状况。
**5、Hlog**
每一个HRegionServer都有一个Hlog对象，用户写入数据时，会备份一份到HLOG中，以便出现意外好恢复。
#### **5.4、HBase运行时特点**
**1、Hbase中的区域**
当HBase中数据记录数不断增多时，达到阈值后，就会从行的方向分裂成多份，每份是一个region,一个region由【startkey,endkey】表示。不同的region会被集群的Master分配给相应的RegionServer进行管理。  
![](http://i.imgur.com/Noke3dt.png)
![](http://i.imgur.com/eeZH7Eu.png)
**2、HBse中两种特殊表**
运行中的HBase内部保留了2中特殊的目录表：-ROOT- 和 .META. 。-ROOT-表包含了.META.表的所有区域列表的信息，而.META.表保存了所有用户空间的区域列表信息，以及RegionServer的服务器地址。
![](http://i.imgur.com/69ZqxCf.png)
HBse中两种特殊表
### ** 六、Hive**
#### **6.1、Hive简介**
Hive是建立在Hadoop之上的数据仓库软件工具，它提供了一系列的工具，帮助用户对大规模的数据进行提取、转换和加载，即ETL操作。Hive定义了简单的类SQL查询语言，称为HiveSql。从本质上讲，Hive其实就是一个SQL解释器，它能够将用户输入的HiveSql语句转换成MapReduce作业在Hadoop集群上执行。
#### **6.2、Hive的体系结构**
![](http://i.imgur.com/Nkz511l.png)
Hive的体系结构
### **七、参考资料**
1、解惑大数据 丁圣勇 编著 
2、大数据技术与应用实践指南 赵刚 著 
3、百度图片
