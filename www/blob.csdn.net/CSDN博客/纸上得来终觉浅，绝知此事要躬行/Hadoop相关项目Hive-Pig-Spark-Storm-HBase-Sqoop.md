# Hadoop相关项目Hive-Pig-Spark-Storm-HBase-Sqoop - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年01月13日 16:53:55[boonya](https://me.csdn.net/boonya)阅读数：350








学习[Hadoop相关项目Hive-Pig-Spark-Storm-HBase-Sqoop](https://www.cnblogs.com/boonya/p/10263219.html)

**目录**

[Hive](#Hive)

[Pig](#Pig)

[Spark](#Spark)

[Storm](#Storm)

[HBase](#HBase)

[Sqoop](#Sqoop)

# Hive

[Pig和Hive的对比](https://www.cnblogs.com/ahu-lichang/p/7398299.html)

摘要: Pig Pig是一种编程语言，它简化了Hadoop常见的工作任务。Pig可加载数据、表达转换数据以及存储最终结果。Pig内置的操作使得半结构化数据变得有意义（如日志文件）。同时Pig可扩展使用Java中添加的自定义数据类型并支持数据转换。 Hive Hive在Hadoop中扮演数据仓库的角色。Hiv[阅读全文](https://www.cnblogs.com/ahu-lichang/p/7398299.html)



[hive的实现机制](https://www.cnblogs.com/ahu-lichang/p/6822481.html)

摘要: hive利用hdfs存储数据文件，利用MapReduce查询数据。 数据库：支持在线联机业务（实时、事务控制） 数据仓库：存储历史数据，面向主题的。主要用于离线数据分析的。[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6822481.html)



[hive1.2.1安装步骤（在hadoop2.6.4集群上）](https://www.cnblogs.com/ahu-lichang/p/6815933.html)

摘要: hive1.2.1在hadoop2.6.4集群上的安装 hive只需在一个节点上安装即可，这里再hadoop1上安装 1、上传hive安装包到/usr/local/目录下 2、解压 tar -zxvf /usr/local/hive-1.2.1.tar.gz 重命名　mv hive-1.2.1 hi[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6815933.html)



[Hive的安装和使用](https://www.cnblogs.com/ahu-lichang/p/6683352.html)

摘要: 1.Hive1.1 在hadoop生态圈中属于数据仓库的角色。他能够管理hadoop中的数据，同时可以查询hadoop中的数据。 本质上讲，hive是一个SQL解析引擎。Hive可以把SQL查询转换为MapReduce中的job来运行。 hive有一套映射工具，可以把SQL转换为MapReduce中[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6683352.html)

# Pig

[Pig和Hive的对比](https://www.cnblogs.com/ahu-lichang/p/7398299.html)

摘要: Pig Pig是一种编程语言，它简化了Hadoop常见的工作任务。Pig可加载数据、表达转换数据以及存储最终结果。Pig内置的操作使得半结构化数据变得有意义（如日志文件）。同时Pig可扩展使用Java中添加的自定义数据类型并支持数据转换。 Hive Hive在Hadoop中扮演数据仓库的角色。Hiv[阅读全文](https://www.cnblogs.com/ahu-lichang/p/7398299.html)



[Pig的安装和简单使用](https://www.cnblogs.com/ahu-lichang/p/6681430.html)

摘要: 1.Pig是基于hadoop的一个数据处理的框架。 MapReduce是使用java进行开发的，Pig有一套自己的数据处理语言，Pig的数据处理过程要转化为MR来运行。2.Pig的数据处理语言是数据流方式的，类似于初中做的数学题。3.Pig基本数据类型：int、long、float、double、c[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6681430.html)

# Spark

[win10下Spark的环境搭建](https://www.cnblogs.com/ahu-lichang/p/9502233.html)

摘要: win10下Spark的环境搭建 2018-08-19 18:36:45 一、jdk 1.8.0 安装与配置 二、scala 2.11.8 安装与配置http://www.scala-lang.org/download/2.11.8.html 上面两步见《win10下安装scala》 三、spark[阅读全文](https://www.cnblogs.com/ahu-lichang/p/9502233.html)



[Spark学习入门](https://www.cnblogs.com/ahu-lichang/p/8081596.html)

摘要: Spark 是一种“One Stack to rule them all”通用的大数据计算框架,期望使用一个技术栈就完美地 解决大数据领域的各种计算任务。 Spark特点：速度快、容易上手开发、超强的通用性、集成Hadoop、极高的活跃度。 Spark的速度比MapReduce快：MR计算模型太死板[阅读全文](https://www.cnblogs.com/ahu-lichang/p/8081596.html)

# Storm

[Flume+Kafka整合](https://www.cnblogs.com/ahu-lichang/p/7048757.html)

摘要: 脚本生产数据 >flume采集数据 >kafka消费数据 >storm集群处理数据 日志文件使用log4j生成，滚动生成！ 当前正在写入的文件在满足一定的数量阈值之后，需要重命名！！！ flume+Kafka整合步骤及相关配置：（先安装好zookeeper集群和Kafka集群） 配置flume： 1[阅读全文](https://www.cnblogs.com/ahu-lichang/p/7048757.html)



[Kafka集群环境搭建](https://www.cnblogs.com/ahu-lichang/p/6906378.html)

摘要: Kafka介绍 在流式计算中，Kafka一般用来缓存数据，Storm通过消费Kafka的数据进行计算。 KAFKA + STORM +REDIS 1、Apache Kafka是一个开源消息系统，用Scala写成。 2、Kafka是一个分布式消息队列：生产者、消费者的功能。它提供了类似于JMS的特性，[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6906378.html)



[Storm消息容错机制（ack-fail机制）](https://www.cnblogs.com/ahu-lichang/p/6904635.html)

摘要: storm消息容错机制（ack-fail） 1、介绍 在storm中，可靠的信息处理机制是从spout开始的。 一个提供了可靠的处理机制的spout需要记录他发射出去的tuple，当下游bolt处理tuple或者子tuple失败时spout能够重新发射。 Storm通过调用Spout的nextTup[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6904635.html)



[Storm通信机制（了解）](https://www.cnblogs.com/ahu-lichang/p/6901530.html)

摘要: Worker间的通信：经常需要通过网络跨节点进行，Storm使用ZeroMQ或Netty(0.9以后默认使用)作为进程间通信的消息框架。 Worker进程内部通信：不同worker的thread通信使用LMAX Disruptor来完成。 不同topologey之间的通信：Storm不负责，需要自己[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6901530.html)



[Storm程序的并发机制（重点掌握）](https://www.cnblogs.com/ahu-lichang/p/6899535.html)

摘要: 概念 Workers (JVMs): 在一个物理节点上可以运行一个或多个独立的JVM 进程。一个Topology可以包含一个或多个worker(并行的跑在不同的物理机上), 所以worker process就是执行一个topology的子集, 并且worker只能对应于一个topology Exec[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6899535.html)



[Storm目录树和任务提交过程](https://www.cnblogs.com/ahu-lichang/p/6898409.html)

摘要: Storm组件本地目录树 Storm zookeeper目录树 Storm任务提交的过程[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6898409.html)



[Storm集群启动流程分析](https://www.cnblogs.com/ahu-lichang/p/6881506.html)

摘要: Storm集群启动流程分析 程序员 1、客户端运行storm nimbus时，会调用storm的python脚本，该脚本中为每个命令编写了一个方法，每个方法都可以生成一条相应的Java命令。 命令格式：java -server xxx.ClassName -args nimbus >Running:[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6881506.html)



[Storm常用操作命令及WordCount](https://www.cnblogs.com/ahu-lichang/p/6871920.html)

摘要: Storm常用操作命令 1、任务提交命令：storm jar 【jar路径】 【拓扑包名.拓扑类名】 【拓扑名称】 storm jar /export/servers/storm/examples/storm-starter/storm-starter-topologies-1.0.3.jar or[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6871920.html)



[Storm1.0.3集群部署](https://www.cnblogs.com/ahu-lichang/p/6861916.html)

摘要: Storm集群部署 所有集群部署的基本流程都差不多：下载安装包并上传、解压安装包并配置环境变量、修改配置文件、分发安装包、启动集群、查看集群是否部署成功。 1、所有的集群上都要配置hosts vi /etc/hosts 192.168.33.201 storm1 hadoop1 zk1 192.16[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6861916.html)



[Storm介绍及核心组件和编程模型](https://www.cnblogs.com/ahu-lichang/p/6858807.html)

摘要: 离线计算 离线计算：批量获取数据、批量传输数据、周期性批量计算数据、数据展示 代表技术：Sqoop批量导入数据、HDFS批量存储数据、MapReduce批量计算数据、Hive批量计算数据、azkaban/oozie任务调度 流式计算 流式计算：数据实时产生、数据实时传输、数据实时计算、实时展示 代表[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6858807.html)



[JMS（Java消息服务）](https://www.cnblogs.com/ahu-lichang/p/6738829.html)

摘要: JMS即Java消息服务（Java Message Service）应用程序接口是一个Java平台中关于面向消息中间件（MOM：指的是利用高效可靠的消息传递机制进行平台无关的数据交流，并基于数据通信来进行分布式系统的集成。）的API， 用于在两个应用程序之间，或分布式系统中发送消息，进行异步通信。J[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6738829.html)

# HBase

[HBase0.99.2集群的搭建步骤（在hadoop2.6.4集群和zookeeper3.4.5集群上）](https://www.cnblogs.com/ahu-lichang/p/6849692.html)

摘要: HBase介绍（NoSql,不是关系型数据库） HBase是一个高可靠性、高性能、面向列、可伸缩的分布式存储系统，利用HBASE技术可在廉价PC Server上搭建起大规模结构化存储集群。 HBase利用hadoop hdfs作为其文件存储系统，利用hadoop MapReduce来处理HBase中[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6849692.html)



[HBase集群的搭建](https://www.cnblogs.com/ahu-lichang/p/6680455.html)

摘要: HBase集群的搭建（在《HBase伪分布式安装》基础上搭建） 1 集群结构，主节点(hmaster)是hadoop0，从节点(region server)是hadoop1和hadoop22 修改hadoop0上的hbase的几个文件 (1)修改hbase-env.sh的最后一行export HBA[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6680455.html)



[HBase结合MapReduce批量导入（HDFS中的数据导入到HBase）](https://www.cnblogs.com/ahu-lichang/p/6680213.html)

摘要: HBase结合MapReduce批量导入 在eclipse中将上面代码运行成功后，就可以去HBase shell中查看结果：[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6680213.html)



[HBase的JavaAPI操作](https://www.cnblogs.com/ahu-lichang/p/6680166.html)

摘要: 1 package hbase; 2 3 import org.apache.hadoop.conf.Configuration; 4 import org.apache.hadoop.hbase.HBaseConfiguration; 5 import org.apache.hadoop.hbase.HColumnDescriptor; 6 import org.apache.h...[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6680166.html)



[HBase shell操作](https://www.cnblogs.com/ahu-lichang/p/6677592.html)

摘要: HBase shell 进入到HBase shell中：/usr/local/hbase/bin/hbase shell HBase shell的DDL操作 HBase shell的DML操作[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6677592.html)



[HBase伪分布式安装](https://www.cnblogs.com/ahu-lichang/p/6675984.html)

摘要: HBase伪分布安装(前提条件：本机或集群环境下hadoop.1.1.2已经安装成功《hadoop集群的搭建（分布式安装）》 )1 解压缩、重命名、设置环境变量vi /etc/profile export HBASE_HOME=/usr/local/hbase export PATH=.:$HBAS[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6675984.html)



[HBase的基础知识](https://www.cnblogs.com/ahu-lichang/p/6675640.html)

摘要: 1.HBase(NoSQL：不是关系型数据库)的逻辑数据模型 HBase – Hadoop Database，是一个高可靠性、高性能、面向列、可伸缩的分布式存储系统，利用HBase技术可在廉价PC Server上搭建起大规模结构化存储集群。HBase利用Hadoop HDFS作为其文件存储系统，利用[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6675640.html)

# Sqoop

[sqoop1.4.6数据迁移](https://www.cnblogs.com/ahu-lichang/p/6840146.html)

摘要: sqoop介绍 sqoop是apache旗下一款“Hadoop和关系数据库服务器之间传送数据”的工具。 导入数据：MySQL，Oracle导入数据到Hadoop的HDFS、HIVE、HBASE等数据存储系统； 导出数据：从Hadoop的文件系统中导出数据到关系数据库 sqoop工作机制 将导入或导出[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6840146.html)



[Sqoop的安装及简单使用](https://www.cnblogs.com/ahu-lichang/p/6685861.html)

摘要: SQOOP是用于对数据进行导入导出的。 (1)把MySQL、Oracle等数据库中的数据导入到HDFS、Hive、HBase中 (2)把HDFS、Hive、HBase中的数据导出到MySQL、Oracle等数据库中 SQOOP的安装（在hadoop0上） 解压缩 tar -zxvf sqoop...[阅读全文](https://www.cnblogs.com/ahu-lichang/p/6685861.html)



