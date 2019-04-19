# Hadoop2.X架构浅析 - 零度的博客专栏 - CSDN博客
2017年05月31日 10:49:20[零度anngle](https://me.csdn.net/zmx729618)阅读数：1554
                
Hadoop2.X核心包括两个module： 
**HDFS** – Hadoop File System。 
**YARN** – Yet Another Resource Negotiator 也称为MapReduce2.0，即MPv2
其中HDFS是底层的存储系统。不仅Hadoop的底层存储可以使用HDFS，其他分布式计算系统也可以使用HDFS作为底层存储系统。HDFS与在Hadoop1.0中有一些大的变化。而Hadoop2.0最重要的变化是新抽象出来的YARN，即独立的分布式资源管理与调度系统。YARN的出现主要为了解决Hadoop1.0中NameNode的瓶颈问题。了解Hadoop1.0的同学知道，在1.0中NameNode既是HDFS的主控服务器，也是JobTracker之所在，因此负载很高且容易单点失败。另外MapReduce作为分布式批处理计算范型，在2.0中没有什么改变。
下面我们分别就HDFS和YARN做进一步讨论。
**一、HDFS**
首先我们来看HDFS的架构： 
![这里写图片描述](https://img-blog.csdn.net/20160108161256106)
大概描述是：HDFS仍然采用master/slave模式。主控节点仍然是NameNode，从节点仍然是多个DataNode。NameNode记录数据集的元数据。由于每个大文件load到HDFS时，都会被分割成默认64MB的数据块(Block)，且这些数据块被分散到多个DataNode中做并行处理，因此NameNode需要管理一个文件分成了哪些Block，这些Block又分散在哪些DataNode上。这些映射关系就是元数据。当DataNode上的Block发生变化时，需向NameNode报告更新元数据。客户端操作数据时，需向NameNode查询元数据，在查询到数据所在的DataNode后，直接与DataNode交互，执行读/写操作。不同的数据块Block会有多个副本(主要是为了数据安全)。Rack是机架，一份数据的多个副本可能存在不同机架的服务器上。
在Hadoop2.0中，HDFS有两个大的改变：
- HA方案 
即High Availability。Hadoop1.0中的NameNode为单节点，而second NameNode并非是NameNode的热备机。因此在2.0中增加了stand-by NameNode(SNN)，而主节点称为active NameNode(ANN)。ANN和SNN共享第三方存储，是热备方案，可自动failover。
- NameNode联盟 
HA解决了单节点失败的问题，但是NameNode的扩展性差的问题仍然没有解决。由于NameNode在内存中管理元数据，因此由于内存限制，Hadoop1.0能管理的机器数最多为4000~5000台。同时在多租户环境下(即一个集群运行完全不相关的多个应用)，单一的NameNode无法实现租户间的有效隔离。NameNode联盟使用多个NameNode来解决扩展性问题，可管理1w台机器以上的集群。
NameNode联盟工作方式如下：将多个DataNode存储的Block的元数据分成多个Block Pool(并非是一对一的关系)，而一个NameNode可以管理多个Block Pool。由此每个NameNode管理一部分元数据，且相互独立，不需要任何协调工作。
另外目前不能单独安装HDFS。也就是说如果你想用HDFS，需要直接安装Hadoop。
**二、YARN**
在Hadoop1.0中NameNode同时运行JobTracker，其作用是将用户提交到Hadoop的job分解成map或reduce task，并在各个DataNode上并发运行这些task。每个DataNode上都运行一个TaskTracker进程，跟踪报告task的运行情况并向JobTracker汇报。Hadoop2.0中这些功能被抽象成一个独立的模块，称为YARN。YARN本质上是一个分布式资源管理与调度系统。它包括：
唯一的资源管理器(Resource Manager) 
每个作业一个的应用管理器(Application Master) 
每个机器一个的节点管理器(Node Manager) 
![这里写图片描述](https://img-blog.csdn.net/20160108172045423)
The ResourceManager has two main components: Scheduler and ApplicationsManager.
The ApplicationsManager is responsible for accepting job-submissions, negotiating the first container for executing the application specific ApplicationMaster and provides the service for restarting the ApplicationMaster container on failure.AM的功能和MPV1中的JobTracker类似。
The NodeManager is the per-machine framework agent who is responsible for containers, monitoring their resource usage (cpu, memory, disk, network) and reporting the same to the ResourceManager/Scheduler.
The per-application ApplicationMaster has the responsibility of negotiating appropriate resource containers from the Scheduler, tracking their status and monitoring for progress.
YARN是一个典型的二级调度器。RM管理全局资源与调度，为一级调度器，将资源分配给每个Application Master。而Application Master为二级调度器，负责在不同的Node间分配协调资源，在container运行子任务。
