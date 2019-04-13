
# Hbase简介及常用命令 - Magician的博客 - CSDN博客


2018年01月08日 14:45:13[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：217所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)
[前言：](#前言)
[1.Hbase简介](#1hbase简介)[1.1、什么是Hbase](#11什么是hbase)
[1.2、与传统数据库的对比](#12与传统数据库的对比)
[1.3、Hbase集群中的角色](#13hbase集群中的角色)
[2、Hbase数据模型](#2hbase数据模型)
[3、Hbase命令](#3hbase命令)
[总结：](#总结)

# 目录
# 前言：
对于Hbase来说，由于其是基于列的数据库，所以比传统的数据库快许多（每次查询只需要查询一个属性值，传统数据库则需要查询一行数据中的所有属性，因此比较慢。）以下只是对Hbase做了一个基本介绍，看完该博客后，希望能对Hbase有个基本的了解。
# 1.Hbase简介
## 1.1、什么是Hbase
HBASE是一个**高可靠性、高性能、面向列、可伸缩的分布式存储系统**，利用HBASE技术可在廉价PC Server上搭建起大规模结构化存储集群。
HBASE的**目标是存储并处理大型的数据**，更具体来说是仅需使用普通的硬件配置，就能够处理由成千上万的行和列所组成的大型数据。
HBASE是Google Bigtable的开源实现，但是也有很多不同之处。比如：Google Bigtable利用GFS作为其文件存储系统，**HBASE利用Hadoop HDFS作为其文件存储系统**；Google运行MAPREDUCE来处理Bigtable中的海量数据，**HBASE同样利用Hadoop MapReduce来处理HBASE中的海量数据**；Google Bigtable利用Chubby作为协同服务，**HBASE利用Zookeeper作为对应。**
## 1.2、与传统数据库的对比
1、传统数据库遇到的问题：
1）数据量很大的时候无法存储
2）没有很好的备份机制
3）数据达到一定数量开始缓慢，很大的话基本无法支撑
**2、HBASE优势：**
1）线性扩展，随着数据量增多可以通过节点扩展进行支撑
2）数据存储在hdfs上，备份机制健全
3）通过zookeeper协调查找数据，访问速度块。
## 1.3、Hbase集群中的角色
1、一个或者多个主节点，Hmaster
**Hmaster的职责**
1、管理用户对Table的增、删、改、查操作；
2、记录region在哪台Hregion server上；
3、在Region Split后，负责新Region的分配；
4、新机器加入时，管理HRegion Server的负载均衡，调整Region分布
5、在HRegion Server宕机后，负责失效HRegion Server 上的Regions迁移。
2、多个从节点，HregionServer
**HregionServer的职责**
HRegion Server主要负责响应用户I/O请求，向HDFS文件系统中读写数据，是HBASE中最核心的模块。
HRegion Server管理了很多table的分区，也就是region。
# 2、Hbase数据模型
![这里写图片描述](https://img-blog.csdn.net/20180108133840863?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**1**、**row key**（可看成是id,不可重复。时间戳可以看成是数据的版本号，每更新一次数据时间戳就会随之改变。存储数据是无类型、字节码存储方便传输）：
与nosql数据库们一样,**row key是用来检索记录的主键**。访问HBASE table中的行，只**有三种方式**：
1.通过单个row key访问
2.通过row key的range（正则）
3.全表扫描
**Row key行键 (Row key)可以是任意字符串**(最大长度 是 64KB，实际应用中长度一般为 10-100bytes)，在HBASE内部，row key保存为字节数组。**存储时，数据按照Row key的字典序(byte order)排序存储。设计key时，要充分排序存储这个特性，将经常一起读取的行存储放到一起。(位置相关性)**
**2**、**Columns Family**（列族是表结构，列是数据）：
列簇 ：**HBASE表中的每个列，都归属于某个列族。列族是表的schema的一部分(而列不是)，必须在使用表之前定义。**
列名都以列族作为前缀。例如 courses:history，courses:math都属于courses 这个列族。
**3**、**Cell**：
由{row key, columnFamily, version} 唯一确定的单元。cell中 的数据是没有类型的，全部是字节码形式存贮。
关键字：**无类型、字节码**
**4**、**Time Stamp**：
HBASE 中通过rowkey和columns确定的为一个存贮单元称为cell。**每个 cell都保存着同一份数据的多个版本。版本通过时间戳来索引**。时间戳的类型是 64位整型。时间戳可以由HBASE(在数据写入时自动 )赋值，此时时间戳是精确到毫秒 的当前系统时间。时间戳也可以由客户显式赋值。如果应用程序要避免数据版 本冲突，就必须自己生成具有唯一性的时间戳。每个 cell中，不同版本的数据按照时间倒序排序，即最新的数据排在最前面。
**为了避免数据存在过多版本造成的的管理 (包括存贮和索引)负担，HBASE提供 了两种数据版本回收方式。一是保存数据的最后n个版本，二是保存最近一段 时间内的版本（比如最近七天）。**用户可以针对每个列族进行设置。
# 3、Hbase命令
名称       命令表达式
创建表 create ‘表名’, ‘列族名1’,’列族名2’,’列族名N’
查看所有表   list
描述表 describe  ‘表名’
判断表存在   exists  ‘表名’
判断是否禁用启用表   is_enabled ‘表名’    is_disabled ‘表名’
添加记录        put  ‘表名’, ‘rowKey’, ‘列族 : 列‘  ,  ‘值’
查看记录rowkey下的所有数据    get  ‘表名’ , ‘rowKey’
查看表中的记录总数    count  ‘表名’
获取某个列族  get ‘表名’,’rowkey’,’列族’
获取某个列族的某个列  get ‘表名’,’rowkey’,’列族：列’
删除记录    delete  ‘表名’ ,‘行名’ , ‘列族：列’
删除整行    deleteall ‘表名’,’rowkey’
删除一张表   先要屏蔽该表，才能对该表进行删除
第一步 disable ‘表名’ ，第二步  drop ‘表名’
清空表 truncate ‘表名’
查看所有记录  scan “表名”
查看某个表某个列中所有数据   scan “表名” , {COLUMNS=>’列族名:列名’}
更新记录    就是重写一遍，进行覆盖，hbase没有修改，都是追加
# 总结：
对于Hbase来说你应该知道以下几点：
1、它是什么，能干什么；
2、Hbase集群中角色以及对应的职责；
3、Hbase数据模型以及读写数据的原理；
4、Hbase常用命令行
如果你能了解以下几个知识点，那么恭喜你已经达到“小菜鸟”的水平。后续如果想深入了解，则需要学习更多的相关知识。

