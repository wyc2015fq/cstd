# 探秘Hadoop生态6：Hive技术初探与实践入门 - 程序员江湖 - CSDN博客





2017年03月08日 09:41:08[黄小斜](https://me.csdn.net/a724888)阅读数：1042标签：[Hadoop](https://so.csdn.net/so/search/s.do?q=Hadoop&t=blog)
个人分类：[Hadoop](https://blog.csdn.net/a724888/article/category/7779280)

所属专栏：[后端技术杂谈](https://blog.csdn.net/column/details/25481.html)













数据仓库概念总结

 发表于 2018-03-05 |  分类于 [数据仓库](http://www.mdjs.info/categories/%E6%95%B0%E6%8D%AE%E4%BB%93%E5%BA%93/)

**0x00 前言**

整理一些数据仓库中的常用概念。大部分概念不是照搬书上的准确定义，会加入很多自己的理解。

**0x01 概念**

**数据仓库（Data Warehouse）**

数据仓库，英文名称为Data Warehouse，可简写为DW或DWH。数据仓库，是为企业所有级别的决策制定过程，提供所有类型数据支持的战略集合。

*个人理解，数据仓库不单单是一个概念，其实算是对数据管理和使用的一种方法论，它包括了如何合理地收集数据、如何规范的管理数据、如何优雅地使用数据，以及任务调度、数据血统分析等一系列内容。 在大数据时代这些概念依旧没有过时，相反，它更加重要。*

利用数据仓库的方式存放的资料，具有一旦存入，便不会随时间发生变动的特性，此外，存入的资料必定包含时间属性，通常一个数据仓库中会含有大量的历史性资料，并且它可利用特定的分析方式，从其中发掘出特定的资讯。

**联机分析处理（OLAP, Online Analytical Process）**

OLAP（Online Analytical Process），联机分析处理，以多维度的方式分析数据，而且能够弹性地提供 **上卷（Roll-up）**、**下钻（Drill-down）** 和透视分析（Pivot）等操作，它是呈现集成性决策信息的方法，多用于决策支持系统、商务智能或数据仓库。其主要的功能在于方便大规模数据分析及统计计算，可对决策提供参考和支持。与之相区别的是联机交易处理（OLTP），联机交易处理，更侧重于基本的、日常的事务处理，包括数据的增删改查。

OLAP需要以大量历史数据为基础，再配合上时间点的差异，对多维度及汇整型的信息进行复杂的分析。

OLAP的概念，在实际应用中存在广义和狭义两种不同的理解方式。广义上的理解与字面上的意思相同，泛指一切不会对数据进行更新的分析处理。但更多的情况下OLAP被理解为其狭义上的含义，即与多维分析相关，基于立方体（Cube）计算而进行的分析。

**商务智能（BI, Business Intelligence）**

BI（Business Intelligence），即商务智能，指用现代数据仓库技术、在线分析技术、数据挖掘和数据展现技术进行数据分析以实现商业价值。

*大致上来讲，BI就是利用各种技术来辅助于商业决策，它需要以数据仓库的数据为基础，通过Olap系统来做分析，必要时还需要一些数据挖掘的方法来挖掘更深层次的价值。*

**元数据（Metadata）**

管理元数据的系统。网上没找到定义，个人对它的理解如下：

一个管理元数据信息的系统

能够提供方便的元数据的操作和查询操作

详细的内容请参照这篇博客[别人家的元数据系统是怎么设计的](http://www.mdjs.info/2017/05/30/data-warehouse/metadata/)

**数据分层**

其实数据分层的意思就是对数据按照一定的层级来存储，这样做的好处很多，在下面列了几个，详细的请参考这篇博客：[如何优雅地设计数据分层](http://www.mdjs.info/2017/05/14/data-warehouse/data-layer/)
- 清晰数据结构：每一个数据分层都有它的作用域，这样我们在使用表的时候能更方便地定位和理解。
- 数据血缘追踪：简单来讲可以这样理解，我们最终给业务诚信的是一能直接使用的张业务表，但是它的来源有很多，如果有一张来源表出问题了，我们希望能够快速准确地定位到问题，并清楚它的危害范围。
- 减少重复开发：规范数据分层，开发一些通用的中间层数据，能够减少极大的重复计算。
- 把复杂问题简单化。讲一个复杂的任务分解成多个步骤来完成，每一层只处理单一的步骤，比较简单和容易理解。而且便于维护数据的准确性，当数据出现问题之后，可以不用修复所有的数据，只需要从有问题的步骤开始修复。
- 屏蔽原始数据的异常。
- 屏蔽业务的影响，不必改一次业务就需要重新接入数据。

**维度建模**

*维度建模是一种数据仓库的建模方法，这样讲吧，它的作用就是帮你更好的组织和使用数据。* 详细的讲解请看这篇博客：[详解维度建模](http://www.mdjs.info/2017/01/05/data-warehouse/dimensona-modeling/)

维度模型是数据仓库领域大师Ralph Kimall所倡导，他的《The DataWarehouse Toolkit-The Complete Guide to Dimensona Modeling，中文名《数据仓库工具箱》，是数据仓库工程领域最流行的数仓建模经典。维度建模以分析决策的需求出发构建模型，构建的数据模型为分析需求服务，因此它重点解决用户如何更快速完成分析需求，同时还有较好的大规模复杂查询的响应性能。

典型的代表是我们比较熟知的星形模型，以及在一些特殊场景下适用的雪花模型。

**ETL （Extract-Transform-Load）**

*ETL 在数据开发的工作中主要是数据清洗，它包括数据的接入，初步的清洗，数据导入Hive或者Mysql中等一系列操作，目前比较火的大数据技术在很大程度上就是解决了大数据量下的数据清洗工作。*

**另外，很多写sql的任务也可以理解是数据清洗，比如使用sql对原始数据做一部分的业务处理、过滤掉一些特殊记录等，因此ETL的范围相对来讲比较广，很多数据开发的工作都可以归结到ETL中。**

ETL，是英文 Extract-Transform-Load 的缩写，用来描述将数据从来源端经过抽取（extract）、转换（transform）、加载（load）至目的端的过程。ETL一词较常用在数据仓库，但其对象并不限于数据仓库。

ETL是构建数据仓库的重要一环，用户从数据源抽取出所需的数据，经过数据清洗,最终按照预先定义好的数据仓库模型，将数据加载到数据仓库中去。

**参考**
- [https://research.google.com/pubs/pub45390.html](https://research.google.com/pubs/pub45390.html)
- [http://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E4%BB%93%E5%BA%93](http://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E4%BB%93%E5%BA%93)
- [http://www.mdjs.info/categories/Data-Warehouse-in-Action/](http://www.mdjs.info/categories/Data-Warehouse-in-Action/)
- [http://www.mdjs.info/2017/05/14/data-warehouse/data-layer/](http://www.mdjs.info/2017/05/14/data-warehouse/data-layer/)
- [http://www.mdjs.info/2017/06/17/data-warehouse/data-quality-monitor/](http://www.mdjs.info/2017/06/17/data-warehouse/data-quality-monitor/)
- [https://www.jianshu.com/p/a8a9e3ada6d3](https://www.jianshu.com/p/a8a9e3ada6d3)





![](https://www.songma.com/wenzhang/20180702/ex53wijsbff921.png)

HIVE技术初探

## 1 概述

### 1.1 基本详情
![](https://www.songma.com/wenzhang/20180702/xb5ffaxgpag922.png)1.1 Hive概述之基本详情.png
### 1.2 技术比较
- 
Hive与传统数据库比较

![](https://www.songma.com/wenzhang/20180702/xs5es1vzksz923.jpg)Hive与传统数据库比较
- 
Hive与Hbase比较
Hive 是个SQL引擎，HBase是个存储引擎，相似MySql, 有个SQL引擎解析解决SQL语句，另外使用Innodb,Myisam,Ndb做数据存储引擎。


### 1.3 优缺点
![](https://www.songma.com/wenzhang/20180702/5nrypql1fkp924.png)1.3 Hive概述之优缺点.png
### 1.4 应使用场景
![](https://www.songma.com/wenzhang/20180702/ig0rzbtakmq925.png)1.4 Hive概述之应使用场景.png
## 2 体系结构

### 2.1 服务端组件
![](https://www.songma.com/wenzhang/20180702/bjzn4onl5ou926.png)2.1 Hive体系结构之服务端组件.png![](https://www.songma.com/wenzhang/20180702/uvn0eb3tl34927.png)技术架构图
### 2.2 工作流程
- 
基本流程

![](https://www.songma.com/wenzhang/20180702/aldfum23j3w928.png)基本流程
- 
Hive HSQL执行流程

![](https://www.songma.com/wenzhang/20180702/d3kk4rl1wkk929.png)HSQL执行流程
- 
流程说明

![](https://www.songma.com/wenzhang/20180702/now1w1kqbio930.png)流程说明.png

### 2.3 元数据
![](https://www.songma.com/wenzhang/20180702/jefx2ukfjxx931.png)2.3 Hive体系结构之元数据.png
## 3 安装模式
![](https://www.songma.com/wenzhang/20180702/kdc2norxzji933.png)3 Hive安装模式.png
## 4 数据类型
![](https://www.songma.com/wenzhang/20180702/dmwxuqynzpa934.png)4 Hive数据类型.png
## 5 数据模型

### 5.1 数据存储
![](https://www.songma.com/wenzhang/20180702/uylxzbrdhee935.png)5.1 Hive数据模型之数据存储.png
### 5.2 四种表类型和视图
![](https://www.songma.com/wenzhang/20180702/t4tymfunf5b937.png)5.2 Hive数据模型之四种表类型和视图.png
参考
Hive Wiki: https://cwiki.apache.org/confluence/display/Hive/Home
Hive Design Docs: https://cwiki.apache.org/confluence/display/Hive/DesignDocs
Hive基础架构:http://willgo.cc/blog/2015/02/01/Hive-structure.html
Hive技术原了解析:https://blog.csdn.net/qq_36864672/article/details/78648248








Hive入门

上节课我们一起学习了用Java来对HBase的表进行增、删、改、查操作。这节课我们开始步入Hive的学习阶段

       我们先来看一下Hive的简介，如下图所示，Hive是一个数据仓库，它部署在Hadoop集群上，它的数据是存储在HDFS上的，Hive所建的表在HDFS上对应的是一个文件夹，表的内容对应的是一个文件。它不仅可以存储大量的数据而且可以对存储的数据进行分析，但它有个缺点就是不能实时的更新数据，无法直接修改和删除数据，如果想要修改数据需要先把数据所在的文件下载下来，修改完之后再上传上去。Hive的语法非常类似于我们的MySQL语句，所以上起手来特别容易。HIve特别神奇的地方是我们只需写一条SQL语句它就会自动转换为MapReduce任务去执行，不用我们再手动去写MapReduce了。

![](https://img-blog.csdn.net/20161105101123301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

                                                                                           图一  Hive简介

![](https://img-blog.csdn.net/20161105102649894?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

                                                                                                 图二  Hive简介

         接下来我们一起来看一下Hive的系统架构图，如下图所示，这张图还是比较老的图，不过还是可以拿来学习的，我们看到Hive的最上方是CLI、JDBC/ODBC、WebUI。CLI的意思是Command Line Interface（命令行接口）,这意味着我们可以通过三种方式来操作我们的Hive，我们发送命令用CLI，尽量不要使用JDBC/ODBC因为事实证明它是有很多问题的，我们可以通过Web来浏览我们的Hive表及数据。元数据我们一般存储在mysql当中（Hive默认的数据库是derby），元数据是指表的信息，比如表的名字，表有哪些列等等描述信息。并不是我们要计算的数据。我们向表中插入的数据是保存在HDFS上的。

![](https://img-blog.csdn.net/20161105105956712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

                                                                                                  图三  Hive架构图

![](https://img-blog.csdn.net/20161105112510830?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

                                                                                              图四  Hive系统架构说明。

        上面说了些理论，接下来我们一起来真正操作一下我们的Hive。当然在操作之前请确保您的Hadoop集群是正常的。如果还没有搭建好集群请参考：http://blog.csdn.net/u012453843/article/details/52829830这篇博客进行搭建。

        首先我们需要下载Hive的安装包，我们使用以前的版本apache-hive-0.13.0-bin.tar.gz，因为我发现最新的版本放在我们的hadoop-2.2.0版本上执行会有问题。我们可以从http://download.csdn.net/detail/u012453843/9673736这个网址下载0.13.0版本的开发包。

        下载完开发包之后上传到itcast01上，然后把它解压到/itcast/目录下。

[root@itcast01 ~]# ls
anaconda-ks.cfg           apache-maven-3.3.9-bin.tar.gz  Documents  glibc-2.14         hadoop-2.2.0-src     hbase-0.96.2-hadoop2-bin.tar.gz  install.log.syslog     Music     Public     Videos   apache-hive-0.13.0-bin.tar.gz  Desktop        Downloads  glibc-2.14.tar.gz  hadoop-2.2.0.tar.gz  install.log      jdk-7u80-linux-x64.gz  Pictures  Templates
[root@itcast01 ~]# tar -zxvf apache-hive-0.13.0-bin.tar.gz -C /itcast/

解压完之后我们到/itcast/目录下查看一下hive包，在它的bin目录下是一些可执行的脚本。如下所示。

[root@itcast01 ~]# cd /itcast/
[root@itcast01 itcast]# ls
apache-hive-0.13.0-bin  hadoop-2.2.0  hbase-0.96.2-hadoop2
[root@itcast01 itcast]# cd apache-hive-0.13.0-bin/
[root@itcast01 apache-hive-0.13.0-bin]# ls
bin  conf  examples  hcatalog  lib  LICENSE  NOTICE  README.txt  RELEASE_NOTES.txt  scripts
[root@itcast01 apache-hive-0.13.0-bin]# cd bin

      我们来查看一下bin目录下的脚本有哪些，发现hive脚本当前所属组是502，所属用户是games，我们把它修改一下。


[root@itcast01 bin]# ls -ls
total 40
4 -rwxr-xr-x. 1  502 games  881 Mar  5  2014 beeline
4 -rw-r--r--. 1 root root   690 Nov  5 13:29 derby.log
4 drwxr-xr-x. 3 root root  4096 Nov  5 13:28 ext
8 -rwxr-xr-x. 1  502 games 7164 Mar 13  2014 hive
4 -rwxr-xr-x. 1  502 games 1900 Mar  5  2014 hive-config.sh
4 -rwxr-xr-x. 1  502 games  885 Mar  5  2014 hiveserver2
4 drwxr-xr-x. 5 root root  4096 Nov  5 13:29 metastore_db
4 -rwxr-xr-x. 1  502 games  832 Mar  5  2014 metatool
4 -rwxr-xr-x. 1  502 games  884 Mar  5  2014 schematool
[root@itcast01 bin]# cd /itcast/
[root@itcast01 itcast]# ls
apache-hive-0.13.0-bin  hadoop-2.2.0  hbase-0.96.2-hadoop2

       使用chown -R root:root apache-hive-0.13.0-bin/来改变用户组和所属用户都改成root，改完之后再查看一下。
[root@itcast01 itcast]# chown -R root:root apache-hive-0.13.0-bin/
[root@itcast01 itcast]# cd apache-hive-0.13.0-bin/
[root@itcast01 apache-hive-0.13.0-bin]# ls -ls
total 304
  4 drwxr-xr-x. 4 root root   4096 Nov  5 13:29 bin
  4 drwxr-xr-x. 2 root root   4096 Nov  5 13:28 conf
  4 drwxr-xr-x. 4 root root   4096 Nov  5 13:28 examples
  4 drwxr-xr-x. 7 root root   4096 Nov  5 13:28 hcatalog
  4 drwxr-xr-x. 4 root root   4096 Nov  5 13:28 lib
 24 -rw-r--r--. 1 root root  23828 Mar  5  2014 LICENSE
  4 -rw-r--r--. 1 root root    277 Mar  5  2014 NOTICE
  4 -rw-r--r--. 1 root root   3838 Mar  5  2014 README.txt
248 -rw-r--r--. 1 root root 251948 Apr 16  2014 RELEASE_NOTES.txt
  4 drwxr-xr-x. 3 root root   4096 Nov  5 13:28 scripts

      conf目录下是它的配置文件。
[root@itcast01 apache-hive-0.13.0-bin]#
[root@itcast01 bin]# cd ..
[root@itcast01 apache-hive-0.13.0-bin]# cd conf
[root@itcast01 conf]# ls
hive-default.xml.template  hive-env.sh.template  hive-exec-log4j.properties.template  hive-log4j.properties.template
[root@itcast01 conf]#

        我们不用对配置做任何修改便可以使用Hive的功能，我们先到bin目录下，然后使用./hive命令启动我们的hive，然后我们可以使用命令show tables查看当前都有哪些表，使用show databases查看都有哪些数据库，如下所示。我们在关系型数据库中如果想查看有哪些表必须先指定用哪个数据库，但是我们的Hive就不用，它会默认使用default数据库。

[root@itcast01 bin]# ls
beeline  ext  hive  hive-config.sh  hiveserver2  metatool  schematool
[root@itcast01 bin]# ./hive
16/11/05 13:29:16 INFO Configuration.deprecation: mapred.reduce.tasks is deprecated. Instead, use mapreduce.job.reduces
16/11/05 13:29:16 INFO Configuration.deprecation: mapred.min.split.size is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize
16/11/05 13:29:16 INFO Configuration.deprecation: mapred.reduce.tasks.speculative.execution is deprecated. Instead, use mapreduce.reduce.speculative
16/11/05 13:29:16 INFO Configuration.deprecation: mapred.min.split.size.per.node is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize.per.node
16/11/05 13:29:16 INFO Configuration.deprecation: mapred.input.dir.recursive is deprecated. Instead, use mapreduce.input.fileinputformat.input.dir.recursive
16/11/05 13:29:16 INFO Configuration.deprecation: mapred.min.split.size.per.rack is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize.per.rack
16/11/05 13:29:16 INFO Configuration.deprecation: mapred.max.split.size is deprecated. Instead, use mapreduce.input.fileinputformat.split.maxsize
16/11/05 13:29:16 INFO Configuration.deprecation: mapred.committer.job.setup.cleanup.needed is deprecated. Instead, use mapreduce.job.committer.setup.cleanup.needed

Logging initialized using configuration in jar:file:/itcast/apache-hive-0.13.0-bin/lib/hive-common-0.13.0.jar!/hive-log4j.properties
hive> show tables;
OK
Time taken: 0.608 seconds
hive> show databases;
OK
default
Time taken: 0.017 seconds, Fetched: 1 row(s)
hive>       

       接下来我们就创建一张student表，有两列，分别是id和name，如下所示，创建完表之后我们查看一下当前都有哪些表，发现有我们刚才创建的表student。

hive> create table student(id int,name string);
OK
Time taken: 0.84 seconds
hive> show tables;
OK
student
Time taken: 0.069 seconds, Fetched: 1 row(s)
hive>

       我们还可以查看创建表的信息，如下所示，可以看到有建表语句，有student表存储的位置'hdfs://ns1/user/hive/warehouse/student'。

hive> show create table student;
OK
CREATE  TABLE `student`(
  `id` int, 
  `name` string)
ROW FORMAT SERDE 
  'org.apache.hadoop.hive.serde2.lazy.LazySimpleSerDe' 
STORED AS INPUTFORMAT 
  'org.apache.hadoop.mapred.TextInputFormat' 
OUTPUTFORMAT 
  'org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat'
LOCATION
 'hdfs://ns1/user/hive/warehouse/student'
TBLPROPERTIES (
  'transient_lastDdlTime'='1478326020')
Time taken: 0.799 seconds, Fetched: 13 row(s)
hive>

        既然student表是存储在hdfs上的，那么我们便到hdfs系统去查看一下，我们HDFS根目录下点击user--->点击hive---->点击warehouse就会看到如下图所示的界面，可以看到这里确实有我们刚才创建的表，它是一个文件夹，我们以后向student表插入数据就是把一个文件放到student目录下。

![](https://img-blog.csdn.net/20161105141756049?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

         接下来我们便创建一个student.txt文件并在里面添加几条数据，创建student.txt直接使用命令vim student.txt便可以，然后在里面输入三行内容，然后保存退出。

[root@itcast01 ~]# vim student.txt
1   zhangsan
2   lisi
3   wangwu

         接着我们把student.txt文件上传到HDFS系统student目录下，如下所示。

hive> load data local inpath '/root/student.txt' into table student;
Copying data from file:/root/student.txt
Copying file: file:/root/student.txt
Loading data to table default.student
Table default.student stats: [numFiles=1, numRows=0, totalSize=33, rawDataSize=0]
OK
Time taken: 1.796 seconds
hive>

         操作完毕之后我们到HDFS文件系统查看一下，如下图所示，发现已经有student.txt文件并且文件中的内容与我们输入的内容一致。

![](https://img-blog.csdn.net/20161105142747350?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        我们从hive视图查询一下student表的内容，如下所示。发现查询出来的内容都是NULL，这是因为我们创建表的时候没有指定列与列之间是以什么分割的。有两列NULL是因为我们创建了两个字段。

hive> select * from student;
OK
NULL    NULL
NULL    NULL
NULL    NULL
Time taken: 1.058 seconds, Fetched: 3 row(s)
hive>

       我们不妨来试试查询student表中插入元素的数量，如下所示。我们从执行信息中可以看到Hive自动去调用MapReduce去统计我们的数量了，我们根本就没做什么事情。是不是很神奇呢。最后查询的结果是3，完全正确。

hive> select count(*) from student;
Total jobs = 1
Launching Job 1 out of 1
Number of reduce tasks determined at compile time: 1
In order to change the average load for a reducer (in bytes):
  set hive.exec.reducers.bytes.per.reducer=<number>
In order to limit the maximum number of reducers:
  set hive.exec.reducers.max=<number>
In order to set a constant number of reducers:
  set mapreduce.job.reduces=<number>
Starting Job = job_1478323409873_0001, Tracking URL = http://itcast03:8088/proxy/application_1478323409873_0001/
Kill Command = /itcast/hadoop-2.2.0/bin/hadoop job  -kill job_1478323409873_0001
Hadoop job information for Stage-1: number of mappers: 1; number of reducers: 1
2016-11-05 14:35:51,757 Stage-1 map = 0%,  reduce = 0%
2016-11-05 14:36:01,163 Stage-1 map = 100%,  reduce = 0%, Cumulative CPU 1.66 sec
2016-11-05 14:36:11,584 Stage-1 map = 100%,  reduce = 100%, Cumulative CPU 2.7 sec
MapReduce Total cumulative CPU time: 2 seconds 700 msec
Ended Job = job_1478323409873_0001
MapReduce Jobs Launched: 
Job 0: Map: 1  Reduce: 1   Cumulative CPU: 2.7 sec   HDFS Read: 237 HDFS Write: 2 SUCCESS
Total MapReduce CPU Time Spent: 2 seconds 700 msec
OK
3
Time taken: 35.475 seconds, Fetched: 1 row(s)
hive>

       既然我们建的student表没有指定列与列的分割符，那么我们接下来再建一张表并且指定分隔符。我们就建一张teacher表吧，建表语句如下。

hive> create table teacher(id bigint,name string)row format delimited fields terminated by '\t';
OK
Time taken: 0.545 seconds
hive> show create table teacher;
OK
CREATE  TABLE `teacher`(
  `id` bigint, 
  `name` string)
ROW FORMAT DELIMITED 
  FIELDS TERMINATED BY '\t' 
STORED AS INPUTFORMAT 
  'org.apache.hadoop.mapred.TextInputFormat' 
OUTPUTFORMAT 
  'org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat'
LOCATION
  'hdfs://ns1/user/hive/warehouse/teacher'
TBLPROPERTIES (
  'transient_lastDdlTime'='1478344693')
Time taken: 0.309 seconds, Fetched: 13 row(s)

       建好了teacher表，我们再来给teacher表中插入一些数据。我们的做法是新建一个teacher.txt文件并在里面输入一些数据，如下所示，输入完毕后保存退出该文件。


[root@itcast01 ~]# vim teacher.txt
1       赵老师
2       王老师
3       刘老师
4       邓老师

       接下来我们把teacher.txt插入到teacher表当中。如下所示。

hive> load data local inpath '/root/teacher.txt' into table teacher;
Copying data from file:/root/teacher.txt
Copying file: file:/root/teacher.txt
Loading data to table default.teacher
Table default.teacher stats: [numFiles=1, numRows=0, totalSize=48, rawDataSize=0]
OK
Time taken: 0.764 seconds
hive>

      上传完后，我们到HDFS文件系统中去看看我们的teacher表及数据。如下图所示，发现我们的表及teacher.txt文件都存在于HDFS中。

![](https://img-blog.csdn.net/20161105204534970?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

         我们再通过shell命令来查看teacher表的信息，如下所示，可以看到我们查询到了teacher表中的数据。

hive> select * from teacher;
OK
1       赵老师
2       王老师
3       刘老师
4       邓老师
Time taken: 0.971 seconds, Fetched: 4 row(s)
hive>

       我们以前学MapReduce的时数据的排序都需要我们手动写MapReduce程序来完成，现在有了Hive，我们只需要一条order by语句便可以搞定，假如我们想让teacher表中的数据降序排列，我们可以使用语句select * from teacher order by desc;我们并没有写任何MapReduce语句，接下来我们执行这条语句，信息如下，可以发现它会自动启用MapReduce来帮我们完成排序功能，真的是省去了我们很多麻烦。但是这也有个问题就是延迟性比较高，因为它要启动MapReduce，小弟要领取jar包领取任务，有可能还要执行多个MapReducer，因此延迟性便会比较多。

hive> select * from teacher order by id desc;
Total jobs = 1
Launching Job 1 out of 1
Number of reduce tasks determined at compile time: 1
In order to change the average load for a reducer (in bytes):
  set hive.exec.reducers.bytes.per.reducer=<number>
In order to limit the maximum number of reducers:
  set hive.exec.reducers.max=<number>
In order to set a constant number of reducers:
  set mapreduce.job.reduces=<number>
Starting Job = job_1478323409873_0002, Tracking URL = http://itcast03:8088/proxy/application_1478323409873_0002/
Kill Command = /itcast/hadoop-2.2.0/bin/hadoop job  -kill job_1478323409873_0002
Hadoop job information for Stage-1: number of mappers: 1; number of reducers: 1
2016-11-05 20:52:32,598 Stage-1 map = 0%,  reduce = 0%
2016-11-05 20:52:40,933 Stage-1 map = 100%,  reduce = 0%, Cumulative CPU 1.4 sec
2016-11-05 20:52:47,147 Stage-1 map = 100%,  reduce = 100%, Cumulative CPU 2.43 sec
MapReduce Total cumulative CPU time: 2 seconds 430 msec
Ended Job = job_1478323409873_0002
MapReduce Jobs Launched: 
Job 0: Map: 1  Reduce: 1   Cumulative CPU: 2.43 sec   HDFS Read: 252 HDFS Write: 48 SUCCESS
Total MapReduce CPU Time Spent: 2 seconds 430 msec
OK
4       邓老师
3       刘老师
2       王老师
1       赵老师
Time taken: 25.794 seconds, Fetched: 4 row(s)
hive>

      下面我们来说一下数据库的问题，我们在上面Hive架构图的时候提到了metadata元数据库，它里面有表的名字，表的列和分区及其属性，表的属性（是否为外部表等），表的数据所在目录等。Hive默认使用的元数据库是derby数据库，但是这个数据库有它致命的缺陷，那就是它仅支持单连接，这在公司的开发中简直就是恶梦。我下面为大家证明一下derby数据库仅支持单连接。

       我们先hive的bin目录下启动hive并且建一个数据库itcast，如下所示。

启动hive



[root@itcast01 bin]# ./hive
16/11/05 21:25:38 INFO Configuration.deprecation: mapred.reduce.tasks is deprecated. Instead, use mapreduce.job.reduces
16/11/05 21:25:38 INFO Configuration.deprecation: mapred.min.split.size is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize
16/11/05 21:25:38 INFO Configuration.deprecation: mapred.reduce.tasks.speculative.execution is deprecated. Instead, use mapreduce.reduce.speculative
16/11/05 21:25:38 INFO Configuration.deprecation: mapred.min.split.size.per.node is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize.per.node
16/11/05 21:25:38 INFO Configuration.deprecation: mapred.input.dir.recursive is deprecated. Instead, use mapreduce.input.fileinputformat.input.dir.recursive
16/11/05 21:25:38 INFO Configuration.deprecation: mapred.min.split.size.per.rack is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize.per.rack
16/11/05 21:25:38 INFO Configuration.deprecation: mapred.max.split.size is deprecated. Instead, use mapreduce.input.fileinputformat.split.maxsize
16/11/05 21:25:38 INFO Configuration.deprecation: mapred.committer.job.setup.cleanup.needed is deprecated. Instead, use mapreduce.job.committer.setup.cleanup.needed

Logging initialized using configuration in jar:file:/itcast/apache-hive-0.13.0-bin/lib/hive-common-0.13.0.jar!/hive-log4j.properties

创建itcast数据库
hive> create database itcast;
OK
Time taken: 0.579 seconds

查看当前所有的数据库
hive> show databases;
OK
default
itcast
Time taken: 0.08 seconds, Fetched: 2 row(s)

查看当前所有的表（默认是在default数据库下）
hive> show tables;
OK
student
teacher
Time taken: 0.037 seconds, Fetched: 2 row(s)

我们切换到itcast数据库
hive> use itcast;
OK
Time taken: 0.012 seconds

查看itcast数据库下所有的表，当然，现在我们还没建任何表，因此没有任何表信息
hive> show tables;
OK
Time taken: 0.017 seconds

我们创建一张user表，列与列之间用'\t'分割。
hive> create table user(id int,name string) row format delimited fields terminated by '\t';
OK
Time taken: 0.229 seconds

我们再来看看itcast数据库下都有哪些表，发现已经有一张user表了。
hive> show tables;
OK
user
Time taken: 0.016 seconds, Fetched: 1 row(s)
hive>

     我们到HDFS文件系统查看一下我们刚才建的itcast数据库，看它是怎样存储的，如下图所示，我们发现我们新建的itcast数据库在HDFS上的名字叫itcast.db，它是一个文件夹，之或以以.db作为后缀，就是告诉我们它是一个数据库。

![](https://img-blog.csdn.net/20161105214305759?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       我们点击itcast.db文件夹进去，可以看到有一个名叫user的文件夹，如下图所示，这就是我们刚才在itcast数据库中创建的user表，当然，现在我们还没有给user表中存放任何东西，因此现在user文件夹下面是空的。

![](https://img-blog.csdn.net/20161105214620214?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       我们接下来再打开一个itcast01的连接，并在hive的bin目录下启动hive，如下图所示，发现启动的时候报错了，提示另一个实例占用了metastore_db文件了。

![](https://img-blog.csdn.net/20161105215838265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

     那么metastore_db文件在哪里呢？我们在hive的bin目录下使用ls命令就可以看到metastore_db文件。这个数据库文件是我们启动hive后生成的文件，它是derby数据库文件。刚才报错就是这个文件不能同时被两个实例使用，意味着同一时间只能有一个人操作数据库，这绝对是我们难以忍受的。

[root@itcast01 bin]# ls
beeline  derby.log  ext  hive  hive-config.sh  hiveserver2  metastore_db  metatool  schematool
[root@itcast01 bin]#

     那么，是不是我们就真的无法在第二个实例启动hive呢？也不是，只是我们启动时所在的目录不能在bin目录下了，我们假如在bin的上一级目录启动hive，我们在启动hive之前，先查看bin的上级目录下是否有metastore_db文件，如下所示，发现没有metastore_db文件。

[root@itcast01 bin]# cd ..
[root@itcast01 apache-hive-0.13.0-bin]# ls
bin  conf  examples  hcatalog  lib  LICENSE  NOTICE  README.txt  RELEASE_NOTES.txt  scripts
[root@itcast01 apache-hive-0.13.0-bin]#

      接下来我们便在bin的上级目录下启动hive，只是我们的启动命令改成bin/hive了而已。发现启动成功了，说明第二个实例也能启动hive，但是事情还没完。


[root@itcast01 apache-hive-0.13.0-bin]# bin/hive
16/11/05 22:04:23 INFO Configuration.deprecation: mapred.reduce.tasks is deprecated. Instead, use mapreduce.job.reduces
16/11/05 22:04:23 INFO Configuration.deprecation: mapred.min.split.size is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize
16/11/05 22:04:23 INFO Configuration.deprecation: mapred.reduce.tasks.speculative.execution is deprecated. Instead, use mapreduce.reduce.speculative
16/11/05 22:04:23 INFO Configuration.deprecation: mapred.min.split.size.per.node is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize.per.node
16/11/05 22:04:23 INFO Configuration.deprecation: mapred.input.dir.recursive is deprecated. Instead, use mapreduce.input.fileinputformat.input.dir.recursive
16/11/05 22:04:23 INFO Configuration.deprecation: mapred.min.split.size.per.rack is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize.per.rack
16/11/05 22:04:23 INFO Configuration.deprecation: mapred.max.split.size is deprecated. Instead, use mapreduce.input.fileinputformat.split.maxsize
16/11/05 22:04:23 INFO Configuration.deprecation: mapred.committer.job.setup.cleanup.needed is deprecated. Instead, use mapreduce.job.committer.setup.cleanup.needed

Logging initialized using configuration in jar:file:/itcast/apache-hive-0.13.0-bin/lib/hive-common-0.13.0.jar!/hive-log4j.properties
hive>


      我们看一下bin的上级目录下现在有没有metastore_db文件，如下所示，发现这时在bin的上级目录下已经新生成了一个metastore_db文件。还没完。

[root@itcast01 apache-hive-0.13.0-bin]# ls
bin  conf  derby.log  examples  hcatalog  lib  LICENSE  metastore_db  NOTICE  README.txt  RELEASE_NOTES.txt  scripts
[root@itcast01 apache-hive-0.13.0-bin]#

      我们再在bin的上级目录启动的hive下查看databases，如下所示，可以看到只有default数据库，根本就没有我们刚才创建的itcast数据库，而且我们查看表也没有任何表信息。这是为什么？？？其实原因在于我们现在查询的数据库其实是bin的上级目录下的metastore_db数据库，而不是bin目录下的metastore_db数据库，由于bin的上级目录下的metastore_db数据库是新建的，还没有任何表，也没有建过数据库，因此我们看到的都是空的。这说明，如果我们想同时操作derby数据库的话得在不同的目录启动hive而且新启动的hive没有任何数据，我们还需要重新建数据库，建表，无法公用，这是我们不敢想象的。因此derby数据库有它致命的缺点，我们在实际开发中用的是mysql数据库而不是derby数据库。

![](https://img-blog.csdn.net/20161105221752352?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       既然derby数据库不靠谱，我们便安装一下mysql数据库，在Linux上安装mysql大家可以参考：http://blog.csdn.net/u012453843/article/details/53048412这篇文章进行安装。

       安装好了mysql，我们把原来用derby数据库在HDFS上生成的hive目录删掉，目的是不与我们用mysql数据库产生冲突，如下所示。

[root@itcast03 bin]# hadoop fs -rm -r /user/hive
16/11/06 00:53:09 INFO fs.TrashPolicyDefault: Namenode trash configuration: Deletion interval = 0 minutes, Emptier interval = 0 minutes.
Deleted /user/hive
[root@itcast03 bin]#

        接下来我们来修改配置文件，我们先到conf目录下看都有哪些配置文件，比较重要的文件是hive-default.xml.template和hive-env.sh.template，hive-default.xml.template是我们需要配置的。

[root@itcast03 bin]# cd /itcast/
[root@itcast03 itcast]# ls
apache-hive-0.13.0-bin  hadoop-2.2.0  hbase-0.96.2-hadoop2  sqoop-1.4.6  sqoop-1.4.6.bin__hadoop-2.0.4-alpha
[root@itcast03 itcast]# cd apache-hive-0.13.0-bin/
[root@itcast03 apache-hive-0.13.0-bin]# ls
bin  conf  examples  hcatalog  lib  LICENSE  NOTICE  README.txt  RELEASE_NOTES.txt  scripts
[root@itcast03 apache-hive-0.13.0-bin]# cd conf
[root@itcast03 conf]# ls
hive-default.xml.template  hive-env.sh.template  hive-exec-log4j.properties.template  hive-log4j.properties.template
[root@itcast03 conf]#

         要修改hive-default.xml.template，我们需要先把它的名字修改一下，将其改为hive-site.xml，如下所示。

[root@itcast03 conf]# ls
hive-default.xml.template  hive-env.sh.template  hive-exec-log4j.properties.template  hive-log4j.properties.template
[root@itcast03 conf]# mv hive-default.xml.template hive-site.xml
[root@itcast03 conf]# ls
hive-env.sh.template  hive-exec-log4j.properties.template  hive-log4j.properties.template hive-site.xml
[root@itcast03 conf]#      

         接下来我们使用NodePad++来编辑我们的配置文件，我们先把<configuration>原来的配置内容都删掉，然后粘贴进去我们下面的配置内容，大家注意配置文件的格式。

<configuration>
 <!--配置连接的URL，现在我们的mysql安装在了itcast05上了，因此我们改成itcast05，如果hive库不存在我们创建它-->
 <property>
   <name>javax.jdo.option.ConnectionURL</name>
   <value>jdbc:mysql://itcast05:3306/hive?createDatabaseIfNotExist=true</value>
   <description>JDBC connect string for a JDBC metastore</description>
 </property>
 <!--连接驱动-->
 <property>
   <name>javax.jdo.option.ConnectionDriverName</name>
   <value>com.mysql.jdbc.Driver</value>
   <description>Driver class name for a JDBC metastore</description>
 </property>
 <!--连接数据库的用户名-->
 <property>
   <name>javax.jdo.option.ConnectionUserName</name>
   <value>root</value>
   <description>username to use against metastore database</description>
 </property>
 <!--连接数据库的密码-->
 <property>
   <name>javax.jdo.option.ConnectionPassword</name>
   <value>root</value>
   <description>password to use against metastore database</description>
 </property>
</configuration>

     配置好了之后我们还需要在itcast03的hive的lib包下放mysql的驱动包，大家如果没有驱动包的话，可以到http://download.csdn.net/detail/u012453843/9667329这个地址进行下载。

[root@itcast03 ~]# ls
anaconda-ks.cfg  Documents  hbase-0.96.2-hadoop2-bin.tar.gz  install.log.syslog     logs  mysql-connector-java-5.1.40-bin.jar  Public                                      Templates  time    wc.txt
Desktop          Downloads  install.log                      jdk-7u80-linux-x64.gz  Music  Pictures                             sqoop-1.4.6.bin__hadoop-2.0.4-alpha.tar.gz  test.sh    Videos
[root@itcast03 ~]# cp mysql-connector-java-5.1.40-bin.jar /itcast/apache-hive-0.13.0-bin/lib/
[root@itcast03 ~]#

     好，现在我们开始在itcast03上启动我们的hive，我们发现出错了，提示我们的itcast03没有权限访问itcast05上的Mysql Server服务。


[root@itcast03 ~]# cd /itcast/apache-hive-0.13.0-bin/bin
[root@itcast03 bin]# ./hive
16/11/06 01:18:07 INFO Configuration.deprecation: mapred.reduce.tasks is deprecated. Instead, use mapreduce.job.reduces
16/11/06 01:18:07 INFO Configuration.deprecation: mapred.min.split.size is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize
16/11/06 01:18:07 INFO Configuration.deprecation: mapred.reduce.tasks.speculative.execution is deprecated. Instead, use mapreduce.reduce.speculative
16/11/06 01:18:07 INFO Configuration.deprecation: mapred.min.split.size.per.node is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize.per.node
16/11/06 01:18:07 INFO Configuration.deprecation: mapred.input.dir.recursive is deprecated. Instead, use mapreduce.input.fileinputformat.input.dir.recursive
16/11/06 01:18:07 INFO Configuration.deprecation: mapred.min.split.size.per.rack is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize.per.rack
16/11/06 01:18:07 INFO Configuration.deprecation: mapred.max.split.size is deprecated. Instead, use mapreduce.input.fileinputformat.split.maxsize
16/11/06 01:18:07 INFO Configuration.deprecation: mapred.committer.job.setup.cleanup.needed is deprecated. Instead, use mapreduce.job.committer.setup.cleanup.needed

Logging initialized using configuration in jar:file:/itcast/apache-hive-0.13.0-bin/lib/hive-common-0.13.0.jar!/hive-log4j.properties
Exception in thread "main" java.lang.RuntimeException: java.lang.RuntimeException: Unable to instantiate org.apache.hadoop.hive.metastore.HiveMetaStoreClient
        at org.apache.hadoop.hive.ql.session.SessionState.start(SessionState.java:344)
        at org.apache.hadoop.hive.cli.CliDriver.run(CliDriver.java:681)
        at org.apache.hadoop.hive.cli.CliDriver.main(CliDriver.java:625)
        at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
        at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
        at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
        at java.lang.reflect.Method.invoke(Method.java:606)
        at org.apache.hadoop.util.RunJar.main(RunJar.java:212)


Caused by: java.sql.SQLException: null,  message from server: "Host 'itcast03' is not allowed to connect to this MySQL server"
        at com.mysql.jdbc.SQLError.createSQLException(SQLError.java:964)
        at com.mysql.jdbc.SQLError.createSQLException(SQLError.java:897)
        at com.mysql.jdbc.SQLError.createSQLException(SQLError.java:886)
        at com.mysql.jdbc.MysqlIO.doHandshake(MysqlIO.java:1040)

       既然没有权限，那么我们便来增加权限

首先我们以root身份登录itcast05上的mysql


[root@itcast05 ~]# mysql -uroot -p
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 10
Server version: 5.1.73 MySQL Community Server (GPL)

Copyright (c) 2000, 2013, Oracle and/or its affiliates. All rights reserved.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

执行下面的语句  *.*:所有库下的所有表   %：任何IP地址或主机都可以连接

mysql> GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY 'root' WITH GRANT OPTION;
Query OK, 0 rows affected (0.00 sec)

刷新一下权限

mysql> FLUSH PRIVILEGES;
Query OK, 0 rows affected (0.00 sec)

mysql>

        接下来我们再在itcast03上启动hive，我们发现我们可以成功启动hive了。


[root@itcast03 bin]# ./hive
16/11/06 01:27:20 INFO Configuration.deprecation: mapred.reduce.tasks is deprecated. Instead, use mapreduce.job.reduces
16/11/06 01:27:20 INFO Configuration.deprecation: mapred.min.split.size is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize
16/11/06 01:27:20 INFO Configuration.deprecation: mapred.reduce.tasks.speculative.execution is deprecated. Instead, use mapreduce.reduce.speculative
16/11/06 01:27:20 INFO Configuration.deprecation: mapred.min.split.size.per.node is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize.per.node
16/11/06 01:27:20 INFO Configuration.deprecation: mapred.input.dir.recursive is deprecated. Instead, use mapreduce.input.fileinputformat.input.dir.recursive
16/11/06 01:27:20 INFO Configuration.deprecation: mapred.min.split.size.per.rack is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize.per.rack
16/11/06 01:27:20 INFO Configuration.deprecation: mapred.max.split.size is deprecated. Instead, use mapreduce.input.fileinputformat.split.maxsize
16/11/06 01:27:20 INFO Configuration.deprecation: mapred.committer.job.setup.cleanup.needed is deprecated. Instead, use mapreduce.job.committer.setup.cleanup.needed

Logging initialized using configuration in jar:file:/itcast/apache-hive-0.13.0-bin/lib/hive-common-0.13.0.jar!/hive-log4j.properties

       我们来查看一下当前有哪些表，发现一张表都没有，因为我们还没有进行过任何操作。
hive> show tables;
OK
Time taken: 0.619 seconds

       我们来创建一张people表。
hive> create table people (id int,name string);
OK
Time taken: 0.396 seconds
hive>


      我们利用SQLyog工具登录mysql，如下图所示，如果我们上面设置的mysql允许远程访问有问题的话（要特别注意root用户的密码，别输错了，我这里用的是root密码，你如果设置的不一样的话一定要用你设置的密码，否则就会有问题），SQLyog是无法登陆的，提示Access denied，因此mysql访问权限一定要设置正确并且刷新权限。

![](https://img-blog.csdn.net/20161106111206377?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

         脸上mysql之后，如下图所示，可以看到mysql数据库自动多了一个hive库，这里面大概有二十几张表，这里面存放的是元数据信息。

![](https://img-blog.csdn.net/20161106014109717?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        那么我们刚才创建的people表存放在哪儿呢？它其实存放在TBLS里面，如下图所示。

![](https://img-blog.csdn.net/20161106014429332?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       那么表有哪些字段在哪儿保存呢？它其实保存在COLUMNS_V2中，如下图所示。那么它怎么知道这两个字段是属于people表的呢，它其实是有外键的。

![](https://img-blog.csdn.net/20161106014616427?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      那么在哪儿存放着数据的存放路径呢，如下图所示，可以看到在SDS当中存放着我们的数据在HDFS上的存放路径。

![](https://img-blog.csdn.net/20161106015042163?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      我们到HDFS上看下people表的路径，如下图所示，发现确实是正确的。

![](https://img-blog.csdn.net/20161106015340258?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       这里我们还需要验证一下我们使用mysql之后是否真的解决了Derby数据库仅支持单连接的问题。我们已经打开了一个itcast03的实例，我们再打开一个itcast03的实例，并且在hive的bin目录下启动hive，看是否能启动成功。发现在itcast03的hive的bin目录下启动hive是没有问题的，我们查看表(show tables)也是有值的。

![](https://img-blog.csdn.net/20161106094503828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      我们再在别的目录下启动一下hive，看是否还在那个目录下生成metastore_db文件，如下图所示

启动前先查看当前目录下有哪些文件，如下


[root@itcast03 ~]# ls
anaconda-ks.cfg  Documents  hbase-0.96.2-hadoop2-bin.tar.gz  install.log.syslog     logs   mysql-connector-java-5.1.40-bin.jar  Public       Templates  time    wc.txt
Desktop          Downloads  install.log           jdk-7u80-linux-x64.gz  Music  Pictures        sqoop-1.4.6.bin__hadoop-2.0.4-alpha.tar.gz  test.sh    Videos

启动hive
[root@itcast03 ~]# /itcast/apache-hive-0.13.0-bin/bin/hive
16/11/06 09:48:18 INFO Configuration.deprecation: mapred.reduce.tasks is deprecated. Instead, use mapreduce.job.reduces
16/11/06 09:48:18 INFO Configuration.deprecation: mapred.min.split.size is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize
16/11/06 09:48:18 INFO Configuration.deprecation: mapred.reduce.tasks.speculative.execution is deprecated. Instead, use mapreduce.reduce.speculative
16/11/06 09:48:18 INFO Configuration.deprecation: mapred.min.split.size.per.node is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize.per.node
16/11/06 09:48:18 INFO Configuration.deprecation: mapred.input.dir.recursive is deprecated. Instead, use mapreduce.input.fileinputformat.input.dir.recursive
16/11/06 09:48:18 INFO Configuration.deprecation: mapred.min.split.size.per.rack is deprecated. Instead, use mapreduce.input.fileinputformat.split.minsize.per.rack
16/11/06 09:48:18 INFO Configuration.deprecation: mapred.max.split.size is deprecated. Instead, use mapreduce.input.fileinputformat.split.maxsize
16/11/06 09:48:18 INFO Configuration.deprecation: mapred.committer.job.setup.cleanup.needed is deprecated. Instead, use mapreduce.job.committer.setup.cleanup.needed

Logging initialized using configuration in jar:file:/itcast/apache-hive-0.13.0-bin/lib/hive-common-0.13.0.jar!/hive-log4j.properties

启动成功后查看表，可以查到people表
hive> show tables;
OK
people
Time taken: 0.401 seconds, Fetched: 1 row(s)

退出hive视图
hive> exit;

再次查看当前目录下的文件，发现跟启动hive前是一样的！说明我们配置的Mysql完全解决了Derby数据库的缺陷！！
[root@itcast03 ~]# ls
anaconda-ks.cfg  Documents  hbase-0.96.2-hadoop2-bin.tar.gz  install.log.syslog     logs   mysql-connector-java-5.1.40-bin.jar  Public      Templates  time    wc.txt
Desktop          Downloads  install.log          jdk-7u80-linux-x64.gz  Music  Pictures       sqoop-1.4.6.bin__hadoop-2.0.4-alpha.tar.gz  test.sh    Videos
[root@itcast03 ~]#





      好了，本小节我们便一起学习到这里。




