# 大数据测试学习笔记之hadoop家族 - zhusongziye的博客 - CSDN博客





2018年03月12日 20:44:39[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：167








## 前言

在进行大数据测试之前，我们必须了解下大数据处理的的相关技术体系，今天主要学习和了解了hadoop家族，这里记录下来分享给大家。




### hadoop家族产品

hadoop项目地址： http://hadoop.apache.org/

![](https://img-blog.csdn.net/20180312204358114?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Hadoop是项目的总称。

主要是由HDFS和MapReduce组成。

HDFS是Google File System（GFS）的开源实现。

MapReduce是Google MapReduce的开源实现。

一句话介绍hadoop家族产品：
- 
Hadoop Common: 在0.20及以前的版本中，包含HDFS、MapReduce和其他项目公共内容，从0.21开始HDFS和MapReduce被分离为独立的子项目，其余内容为Hadoop Common




- 
HDFS: Hadoop分布式文件系统(Distributed File System) － HDFS (Hadoop Distributed File System)




- 
MapReduce：并行计算框架，0.20前使用 org.apache.hadoop.mapred 旧接口，0.20版本开始引入org.apache.hadoop.mapreduce的新API




- 
HBase: 类似Google BigTable的分布式NoSQL列数据库。（HBase和Avro已经于2010年5月成为顶级 Apache 项目）




- 
Hive：数据仓库工具，由Facebook贡献。




- 
Zookeeper：分布式锁设施，提供类似Google Chubby的功能，由Facebook贡献。




- 
Ambari: 是一种基于Web的工具，支持Hadoop集群的供应、管理和监控。




- 
Avro：新的数据序列化格式与传输工具，将逐步取代Hadoop原有的IPC机制。




- 
Cassandra:是一套开源分布式NoSQL数据库系统。它最初由Facebook开发，用于储存简单格式数据，集Google BigTable的数据模型与Amazon Dynamo的完全分布式的架构于一身。




- 
Chukwa: 是一个开源的用于监控大型分布式系统的数据收集系统，它可以将各种各样类型的数据收集成适合Hadoop 处理的文件保存在HDFS中供Hadoop进行各种 MapReduce 操作。




- 
Pig: 大数据分析平台，为用户提供多种接口。




- 
Ambari：Hadoop管理工具，可以快捷的监控、部署、管理集群。




- 
Sqoop：于在HADOOP与传统的数据库间进行数据的传递。




- 
Flume: 是一个分布的、可靠的、高可用的海量日志聚合的系统，可用于日志数据收集，日志数据处理，日志数据传输。




- 
Bigtop: 是一个对Hadoop及其周边生态进行打包，分发和测试的工具。




- 
HCatalog: 是基于Hadoop的数据表和存储管理，实现中央的元数据和模式管理，跨越Hadoop和RDBMS，利用Pig和Hive提供关系视图。




- 
Cloudera Hue: 是一个基于WEB的监控和管理系统，实现对HDFS，MapReduce/YARN, HBase, Hive, Pig的web化操作和管理。





当然整个hadoop生态不仅仅上述这些产品，还有更多的，你可以自己多去了解下。

### 总结

面对hadoop复杂、多样的产品，如何才能有效的学习掌握呢？
- 
了解基于hadoop家族产品的具体大数据的应用场景

- 
深入学习HDFS和MapReduce工作原理

- 
尝试安装、部署hadoop家族系列产品，并试用

- 
与从事大数据开发的人员聊聊具体的技术落地过程





对于大数据的技术体系中涉及的各种技术的学习是一个长期的过程，但光看书是不够的，还需要去实践。

作为软件测试，更需要在实践的过程中结合测试理念，从细节做起，从解决小问题做起，逐步建立系一套符合企业需要的大数据测试体系和积累起自己的大数据相关的技术体系。



