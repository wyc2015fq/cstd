# 高并发MPP查询引擎Impala初识 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月05日 17:04:38[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5208
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









官网：http://impala.apache.org/



Apache Impala是高性能的专用SQL引擎，使用Impala SQL，因为Impala无需借助任何的框架，直接实现对数据块的查询，所以查询延迟毫秒级。还是得放到和Hive一起说说，Impala适用实时查询，因为比Hive查询快多了。Impala是Cloudera公司主导开发的新型查询系统，它提供SQL语义，能查询存储在Hadoop的HDFS和HBase中的PB级大数据。已有的Hive系统虽然也提供了SQL语义，但由于Hive底层执行使用的是MapReduce引擎，仍然是一个批处理过程，难以满足查询的交互性。

## 1、Impala即时查询原理

Impala是架构于Hadoop之上的开源、高并发的MPP查询引擎，被广泛应用于各种行业。Impala是完全集成的，用以平衡Hadoop的灵活性和可扩展性，为BI/数据分析师提供低延迟、高并发的以读为主的查询。

Impala将传统分析数据库的SQL支持和多用户性能与Hadoop的灵活性和可扩展性结合起来，它通过利用HDFS、HBase、Metastore、YARN、Sentry等标准组件能够读取大多数广泛使用的文件格式比如Parquet、Avro、RCFile来维护Hadoop的灵活性；为了降低延迟，避免利用MR或者读远程数据，基于负责查询执行所有方面、作为Hadoop基础设施一部分运行于各台服务器上的Deamon进程实现了一个分布式架构，在相同负载的情形下其性能相当或超过了商用MPP分析数据库。

在Cloudera的测试中，Impala的查询效率比Hive有数量级的提升。从技术角度上来看，Impala之所以能有好的性能，主要有以下几方面的原因。

1）  Impala不需要把中间结果写入磁盘，省掉了大量的I/O开销。

2）  省掉了MapReduce作业启动的开销。MapReduce启动task的速度很慢（默认每个心跳间隔是3秒钟），Impala直接通过相应的服务进程来进行作业调度，速度快了很多。

3）  Impala完全抛弃了MapReduce这个不太适合做SQL查询的范式，而是像Dremel一样借鉴了MPP并行数据库的思想另起炉灶，因此可做更多的查询优化，从而省掉不必要的shuffle、sort等开销。

4）  通过使用LLVM来统一编译运行时代码，避免了为支持通用编译而带来的不必要开销。

5）  用C++实现，做了很多有针对性的硬件优化，例如使用SSE指令。

6）  使用了支持Data locality的I/O调度机制，尽可能地将数据和计算分配在同一台机器上进行，减少了网络开销。

## 2、Impala架构

Impala是Cloudera在受到Google的Dremel启发下开发的实时交互SQL大数据查询工具（实时SQL查询引擎Impala），Impala没有再使用缓慢的Hive+MapReduce批处理，而是通过使用与商用并行关系数据库中类似的分布式查询引擎（由Query Planner、Query Coordinator和Query Exec Engine三部分组成），可以直接从HDFS或HBase中用SELECT、JOIN和统计函数查询数据，从而大大降低了延迟。

![](https://img-blog.csdn.net/20170705171204149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




Impala主要由Impalad， State Store和CLI组成。



1）Impalad: 与DataNode运行在同一节点上，由Impalad进程表示，它接收客户端的查询请求（接收查询请求的Impalad为Coordinator，Coordinator通过JNI调用java前端解释SQL查询语句，生成查询计划树，再通过调度器把执行计划分发给具有相应数据的其它Impalad进行执行），读写数据，并行执行查询，并把结果通过网络流式的传送回给Coordinator，由Coordinator返回给客户端。同时Impalad也与State Store保持连接，用于确定哪个Impalad是健康和可以接受新的工作。

在Impalad中启动三个ThriftServer:beeswax_server（连接客户端），hs2_server（借用Hive元数据）， be_server（Impalad内部使用）和一个ImpalaServer服务。

2）Impala State Store: 跟踪集群中的Impalad的健康状态及位置信息，由statestored进程表示，它通过创建多个线程来处理Impalad的注册订阅和与各Impalad保持心跳连接，各Impalad都会缓存一份State Store中的信息，当State Store离线后（Impalad发现State Store处于离线时，会进入recovery模式，反复注册，当State Store重新加入集群后，自动恢复正常，更新缓存数据）因为Impalad有State Store的缓存仍然可以工作，但会因为有些Impalad失效了，而已缓存数据无法更新，导致把执行计划分配给了失效的Impalad，导致查询失败。

3）CLI: 提供给用户查询使用的命令行工具（Impala Shell使用python实现），同时Impala还提供了Hue，JDBC，ODBC使用接口。



## 3、与Hive关系

Impala与Hive都是构建在Hadoop之上的数据查询工具各有不同的侧重适应面，但从客户端使用来看Impala与Hive有很多的共同之处，如数据表元数据、ODBC/JDBC驱动、SQL语法、灵活的文件格式、存储资源池等。Hive适合于长时间的批处理查询分析，而Impala适合于实时交互式SQL查询，Impala给数据分析人员提供了快速实验、验证想法的大数据分析工具。可以先使用hive进行数据转换处理，之后使用Impala在Hive处理后的结果数据集上进行快速的数据分析。Impala与Hive在Hadoop中的关系如下图。

![](https://img-blog.csdn.net/20170705171221934?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




## 4、Impala查询处理过程

Impalad分为Java前端与C++处理后端，接受客户端连接的Impalad即作为这次查询的Coordinator，Coordinator通过JNI调用Java前端对用户的查询SQL进行分析生成执行计划树，不同的操作对应不用的PlanNode, 如：SelectNode， ScanNode， SortNode， AggregationNode， HashJoinNode等等。



　　执行计划树的每个原子操作由一个PlanFragment表示，通常一条查询语句由多个Plan Fragment组成， Plan Fragment 0表示执行树的根，汇聚结果返回给用户，执行树的叶子结点一般是Scan操作，分布式并行执行。



     Java前端产生的执行计划树以Thrift数据格式返回给Impala C++后端（Coordinator）（执行计划分为多个阶段，每一个阶段叫做一个PlanFragment，每一个PlanFragment在执行时可以由多个Impalad实例并行执行(有些PlanFragment只能由一个Impalad实例执行,如聚合操作)，整个执行计划为一执行计划树），由Coordinator根据执行计划，数据存储信息（Impala通过libhdfs与HDFS进行交互。通过hdfsGetHosts方法获得文件数据块所在节点的位置信息），通过调度器（现在只有simple-scheduler,
 使用round-robin算法）Coordinator::Exec对生成的执行计划树分配给相应的后端执行器Impalad执行（查询会使用LLVM进行代码生成，编译，执行。对于使用LLVM如何提高性能这里有说明），通过调用GetNext()方法获取计算结果，如果是insert语句，则将计算结果通过libhdfs写回HDFS当所有输入数据被消耗光，执行结束，之后注销此次查询服务。　　



Impala与标准的BI环境集成，客户端通过ODBC或JDBC连接服务器、通过Kerberos或LDAP实现认证、遵循标准的SQL角色和权限实现权限管理。Impala的查询处理流程大概如图所示：

![](https://img-blog.csdn.net/20170705171233480?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



