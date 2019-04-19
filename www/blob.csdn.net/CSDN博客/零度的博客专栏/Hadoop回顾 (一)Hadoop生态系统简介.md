# Hadoop回顾:(一)Hadoop生态系统简介 - 零度的博客专栏 - CSDN博客
2017年11月14日 14:09:20[零度anngle](https://me.csdn.net/zmx729618)阅读数：2467标签：[hadoop																[生态系统](https://so.csdn.net/so/search/s.do?q=生态系统&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)
个人分类：[Hadoop](https://blog.csdn.net/zmx729618/article/category/6452458)
首先我们先了解一下Hadoop的起源。然后介绍一些关于Hadoop生态系统中的具体工具的使用方法。如:HDFS、MapReduce、Yarn、Zookeeper、Hive、HBase、Oozie、Mahout、Pig、Flume、Sqoop。
# Hadoop的起源
Doug Cutting是Hadoop之父 ，起初他开创了一个开源软件Lucene（用Java语言编写，提供了全文检索引擎的架构，与Google类似），Lucene后来面临与Google同样的错误。于是，Doug Cutting学习并模仿Google解决这些问题的办法，产生了一个Lucene的微缩版Nutch。
后来，Doug Cutting等人根据2003-2004年Google公开的部分GFS和Mapreduce思想的细节，利用业余时间实现了GFS和Mapreduce的机制，从而提高了Nutch的性能。由此Hadoop产生了。
Hadoop于2005年秋天作为Lucene的子项目Nutch的一部分正式引入Apache基金会。2006年3月份，Map-Reduce和Nutch Distributed File System(NDFS)分别被纳入Hadoop的项目中。
关于Hadoop名字的来源，是Doug Cutting儿子的玩具大象。
# Hadoop是什么
Hadoop是一个开源框架，可编写和运行分布式应用处理大规模数据。 Hadoop框架的核心是HDFS和MapReduce。其中 HDFS 是分布式文件系统，MapReduce 是分布式数据处理模型和执行环境。
在一个宽泛而不断变化的分布式计算领域，Hadoop凭借什么优势能脱颖而出呢？ 
1. 运行方便：Hadoop是运行在由一般商用机器构成的大型集群上。Hadoop在云计算服务层次中属于PaaS(Platform-as-a- Service)：平台即服务。
2. 健壮性：Hadoop致力于在一般的商用硬件上运行，能够从容的处理类似硬件失效这类的故障。 
3. 可扩展性：Hadoop通过增加集群节点，可以线性地扩展以处理更大的数据集。 
4. 简单：Hadoop允许用户快速编写高效的并行代码。
# Hadoop的生态系统
![](http://i.imgur.com/MNHQQVG.jpg)
- 2) Nutch，互联网数据及Nutch搜索引擎应用
- 3) HDFS,Hadoop的分布式文件系统
- 5) MapReduce,分布式计算框架
- 6) Flume、Scribe，Chukwa数据收集，收集非结构化数据的工具。
- 7） Hiho、Sqoop,讲关系数据库中的数据导入HDFS的工具
- 8） Hive数据仓库，pig分析数据的工具
- 10）Oozie作业流调度引擎 
- 11）Hue，Hadoop自己的监控管理工具
- 12）Avro 数据序列化工具
- 13）mahout数据挖掘工具
- 14)Hbase分布式的面向列的开源数据库
# Hadoop生态系统的特点
- 源代码开源
- 社区活跃、参与者众多
- 涉及分布式存储和计算的方方面面
- 已得到企业界验证
# Hadoop生态系统的各组成部分详解
上面的图可能有些乱，下面我们用一个简易的Hadoop生态系统图谱来描述Hadoop生态系统中出现的各种数据工具。
Hadoop1.0时代的生态系统如下：
![](http://i.imgur.com/SYMxPmP.jpg)
Hadoop2.0时代的生态系统如下：
![](http://i.imgur.com/Dpz74XZ.jpg)
## Hadoop的核心
![](http://i.imgur.com/x40qGJk.jpg)
由上图可以看出Hadoop1.0与Hadoop2.0的区别。Hadoop1.0的核心由HDFS（Hadoop Distributed File System）和MapReduce(分布式计算框架)构成。而在Hadoop2.0中增加了Yarn(Yet Another Resource Negotiator),来负责集群资源的统一管理和调度。
### HDFS(分布式文件系统)
HDFS源自于Google发表于2003年10月的GFS论文，也即是说HDFS是GFS的克隆版。
此处只是HDFS的概述，如果想了解HDFS详情,请查看HDFS详解这篇文章。
#### HDFS具有如下特点：
- 良好的扩展性
- 高容错性
- 适合PB级以上海量数据的存储
#### HDFS的基本原理
- 将文件切分成等大的数据块，存储到多台机器上
- 将数据切分、容错、负载均衡等功能透明化
- 可将HDFS看成容量巨大、具有高容错性的磁盘
#### HDFS的应用场景
- 海量数据的可靠性存储
- 数据归档
### Yarn(资源管理系统)
Yarn是Hadoop2.0新增的系统，负责集群的资源管理和调度，使得多种计算框架可以运行在一个集群中。
此处只是Yarn的概述，如果想了解Yarn详情,请查看Yarn详解这篇文章。
#### Yarn具有如下特点：
- 良好的扩展性、高可用性
- 对多种数据类型的应用程序进行统一管理和资源调度
- 自带了多种用户调度器，适合共享集群环境
![](http://i.imgur.com/d70zXQK.jpg)
### MapReduce(分布式计算框架)
MapReduce源自于Google发表于2004年12月的MapReduce论文，也就是说，Hadoop MapReduce是Google MapReduce的克隆版。
此处只是MapReduce的概述，如果想了解MapReduce详情,请查看MapReduce详解这篇文章。
#### MapReduce具有如下特点：
- 良好的扩展性
- 高容错性
- 适合PB级以上海量数据的离线处理
![](http://i.imgur.com/wXUseY2.jpg)
## Hive(基于MR的数据仓库)
Hive由facebook开源，最初用于解决海量结构化的日志数据统计问题；是一种ETL(Extraction-Transformation-Loading)工具。它也是构建在Hadoop之上的数据仓库；数据计算使用MR,数据存储使用HDFS。
Hive定义了一种类似SQL查询语言的HiveQL查询语言，除了不支持更新、索引和实物，几乎SQL的其他特征都能支持。它通常用于离线数据处理（采用MapReduce);我们可以认为Hive的HiveQL语言是MapReduce语言的翻译器，把MapReduce程序简化为HiveQL语言。但有些复杂的MapReduce程序是无法用HiveQL来描述的。
Hive提供shell、JDBC/ODBC、Thrift、Web等接口。
此处只是Hive的概述，如果想了解Hive详情,请查看Hive详解这篇文章。
### Hive应用场景
- 日志分析：统计一个网站一个时间段内的pv、uv ；比如百度。淘宝等互联网公司使用hive进行日志分析
- 多维度数据分析
- 海量结构化数据离线分析
- 低成本进行数据分析（不直接编写MR） 
![](http://i.imgur.com/tGvdpMn.jpg)
## Pig(数据仓库)
Pig由yahoo!开源，设计动机是提供一种基于MapReduce的ad-hoc数据分析工具。它通常用于进行离线分析。
Pig是构建在Hadoop之上的数据仓库，定义了一种类似于SQL的数据流语言–Pig Latin,Pig Latin可以完成排序、过滤、求和、关联等操作，可以支持自定义函数。Pig自动把Pig Latin映射为MapReduce作业，上传到集群运行，减少用户编写Java程序的苦恼。
Pig有三种运行方式：Grunt shell、脚本方式、嵌入式。
此处只是Pig的概述，如果想了解Pig详情,请查看Pig详解这篇文章。
### Pig与Hive的比较
![](http://i.imgur.com/90OiAtM.jpg)
## Mahout(数据挖掘库)
Mahout是基于Hadoop的机器学习和数据挖掘的分布式计算框架。它实现了三大算法：推荐、聚类、分类。
![](http://i.imgur.com/2kMHImL.jpg)
## HBase(分布式数据库)
HBase源自Google发表于2006年11月的Bigtable论文。也就是说，HBase是Google Bigtable的克隆版。
HBase可以使用shell、web、api等多种方式访问。它是NoSQL的典型代表产品。
此处只是HBase的概述，如果想了解HBase详情,请查看HBase详解这篇文章。
### HBase的特点
- 高可靠性
- 高性能
- 面向列
- 良好的扩展性
### HBase的数据模型
![](http://i.imgur.com/4FdwBFG.jpg)
下面简要介绍一下：
- Table（表）：类似于传统数据库中的表
- Column Family(列簇)：Table在水平方向有一个或者多个Column Family组成；一个Column Family 中可以由任意多个Column组成。
- Row Key(行健)：Table的主键；Table中的记录按照Row Key排序。
- Timestamp（时间戳）：每一行数据均对应一个时间戳；也可以当做版本号。
## Zookeeper(分布式协作服务)
Zookeeper源自Google发表于2006年11月的Chubby论文，也就是说Zookeeper是Chubby的克隆版。
Zookeeper解决分布式环境下数据管理问题：
- 统一命名
- 状态同步
- 集群管理
- 配置同步
### Zookeeper的应用
- HDFS
- Yarn
- Storm
- HBase
- Flume
- Dubbo
- Metaq
## Sqoop（数据同步工具）
Sqoop是连接Hadoop与传统数据库之间的桥梁，它支持多种数据库，包括MySQL、DB2等；插拔式，用户可以根据需要支持新的数据库。
Sqoop实质上是一个MapReduce程序，充分利用MR并行的特点,充分利用MR的容错性。
![](http://i.imgur.com/aNVqEER.jpg)
此处只是Sqoop的概述，如果想了解Sqoop详情,请查看Sqoop详解这篇文章。
## Flume(日志收集工具)
Flume是Cloudera开源的日志收集系统。
### Flume的特点
- 分布式
- 高可靠性
- 高容错性
- 易于定制与扩展
### Flume OG与Flume NG的对比
- Flume OG：Flume original generation 即Flume 0.9.x版本，它由agent、collector、master等组件构成。
![](http://i.imgur.com/U7ryTV3.jpg)
- Flume NG：Flume next generation ，即Flume 1.x版本，它由Agent、Client等组件构成。一个Agent包含Source、Channel、Sink和其他组件。
![](http://i.imgur.com/hNyqbT1.jpg)
![](http://i.imgur.com/XafxurE.jpg)
## Oozie(作业流调度系统)
目前计算框架和作业类型种类繁多：如MapReduce、Stream、HQL、Pig等。这些作业之间存在依赖关系，周期性作业，定时执行的作业，作业执行状态监控与报警等。如何对这些框架和作业进行统一管理和调度？
解决方案有多种：
- Linux Crontab
- 自己设计调度系统（淘宝等公司）
- 直接使用开源系统（Oozie）
![](http://i.imgur.com/mbk9kJX.jpg)
# Hadoop发行版（开源版）介绍
- Apache Hadoop
推荐使用2.x.x版本
下载地址：[http://hadoop.apache.org/releases.html](http://hadoop.apache.org/releases.html)
- CDH(Cloudera Distributed Hadoop)
推荐使用CDH5版本
下载地址：[http://archive.cloudera.com/cdh5/cdh/5/](http://archive.cloudera.com/cdh5/cdh/5/)
详细安装步骤查看文章：**[搭建5个节点的hadoop集群环境（CDH5）](http://blog.csdn.net/u010270403/article/details/51446674)**
- HDP(HortonWorks Data Platform)
推荐使用HDP2.x版本
下载地址：[http://zh.hortonworks.com/downloads/#data-platform](http://zh.hortonworks.com/downloads/#data-platform)
最后建议：关于不同发行版在架构、部署和使用方法一致，不同之处仅在于内部实现。建议选择公司发行版，比如CDH或者HDP,因为它们经过集成测试，不会面临版本兼容性问题。

