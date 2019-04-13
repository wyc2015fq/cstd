
# Hive的相关介绍 - Magician的博客 - CSDN博客


2017年12月29日 17:32:02[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：414标签：[hive																](https://so.csdn.net/so/search/s.do?q=hive&t=blog)[简介																](https://so.csdn.net/so/search/s.do?q=简介&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=hive&t=blog)个人分类：[Hive																](https://blog.csdn.net/qq_16633405/article/category/7234365)[大数据																](https://blog.csdn.net/qq_16633405/article/category/6905043)[
							](https://blog.csdn.net/qq_16633405/article/category/7234365)
所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)[前言：](#前言)
[1、Hive简介](#1hive简介)
[2、Hive架构](#2hive架构)
[3、Hive与Hadoop的关系](#3hive与hadoop的关系)
[4、Hive与传统数据库对比](#4hive与传统数据库对比)
[5、Hive的数据存储](#5hive的数据存储)
[总结：](#总结)


# 目录
## 前言：
**为什么使用Hive**
**直接使用hadoop所面临的问题**
人员学习成本太高
项目周期要求太短
MapReduce实现复杂查询逻辑开发难度太大
**使用Hive的优点：**
操作接口采用类SQL语法，提供快速开发的能力。
避免了去写MapReduce，减少开发人员的学习成本。
扩展功能很方便。
## 1、Hive简介
**1.1、什么是Hive**
Hive是基于Hadoop的一个数据仓库工具，可以将结构化的数据文件映射为一张数据库表，并提供类SQL查询功能。
**1.2、Hive的特点**
可扩展 ：
Hive可以自由的扩展集群的规模，一般情况下不需要重启服务。
延展性：
Hive支持用户自定义函数，用户可以根据自己的需求来实现自己的函数。
容错：
良好的容错性节点出现问题SQL仍可完成执行。
2、Hive架构
2.1、架构图
![这里写图片描述](https://img-blog.csdn.net/20171229172350169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171229172350169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.2、基本组成
[
](https://img-blog.csdn.net/20171229172350169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)用户接口：包括 CLI、JDBC/ODBC、WebGUI。
元数据存储：通常是存储在关系数据库如 mysql , derby中。
解释器、编译器、优化器、执行器。
[
](https://img-blog.csdn.net/20171229172350169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)2.3、各组件的基本功能
[
](https://img-blog.csdn.net/20171229172350169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)用户接口主要由三个：CLI、JDBC/ODBC和WebGUI。其中，CLI为shell命令行；JDBC/ODBC是Hive的JAVA实现，与传统数据库JDBC类似；WebGUI是通过浏览器访问Hive。
元数据存储：Hive 将元数据存储在数据库中。Hive
中的元数据包括表的名字，表的列和分区及其属性，表的属性（是否为外部表等），表的数据所在目录等。
解释器、编译器、优化器完成 HQL 查询语句从词法分析、语法分析、编译、优化以及查询计划的生成。生成的查询计划存储在 HDFS中，并在随后有 MapReduce 调用执行。
3、Hive与Hadoop的关系
Hive利用HDFS存储数据，利用MapReduce查询数据
![这里写图片描述](https://img-blog.csdn.net/20171229172721678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4、Hive与传统数据库对比
![这里写图片描述](https://img-blog.csdn.net/20171229172757518?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
总结：hive具有sql数据库的外表，但应用场景完全不同，hive只适合用来做批量数据统计分析。
5、Hive的数据存储
1、Hive中所有的数据都存储在 HDFS 中，没有专门的数据存储格式（可支持Text，SequenceFile，ParquetFile，RCFILE等）
2、只需要在创建表的时候告诉 Hive 数据中的列分隔符和行分隔符，Hive 就可以解析数据。
3、Hive 中包含以下数据模型：DB、Table，External Table，Partition，Bucket。
db：在hdfs中表现为${hive.metastore.warehouse.dir}目录下一个文件夹。
table：在hdfs中表现所属db目录下一个文件夹。
external table：外部表,与table类似，不过其数据存放位置可以在任意指定路径 普通表: 删除表后, hdfs上的文件都删了 External外部表删除后,hdfs上的文件没有删除, 只是把文件删除了。
partition：在hdfs中表现为table目录下的子目录 。
bucket：桶, 在hdfs中表现为同一个表目录下根据hash散列之后的多个文件, 会根据不同的文件把数据放到不同的文件中。
总结：
对于Hive来说，个人感觉只要知道以下几个知识点就算是达到和博主一样的小菜鸟的境界，后期可以通过专项学习来进化为“大鹏”：
1、Hive是什么，干什么用的。
2、理解Hive的工作原理。
3、熟练掌握Hql的编写，能够运用Hql进行简单的数据分析。
目前只想到了这些点，如有遗漏后续会接着补充。

