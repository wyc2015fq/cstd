# 离线轻量级大数据平台Spark之单机部署及Java开发 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年10月27日 08:55:30[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3058
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









## 1、Spark平台基本介绍

Spark 由加州大学伯克利分校 AMP 实验室 (Algorithms, Machines, and PeopleLab) 开发，可用来构建大型的、低延迟的数据分析应用程序。

Spark 是在Scala 语言中实现的，它将 Scala 用作其应用程序框架。Scala 可以像操作本地集合对象一样轻松地操作分布式数据集。

Spark 是一种与Hadoop 相似的开源集群计算环境，不同之处使是Spark 启用了内存分布数据集，除了能够提供交互式查询外，它还可以优化迭代工作负载。尽管创建 Spark 是为了支持分布式数据集上的迭代作业，但是实际上它是对 Hadoop 的补充，可以在 Hadoo 文件系统中并行运行。通过名为 Mesos 的第三方集群框架可以支持此行为。

1）Spark集群计算架构

Spark 是为集群计算中的特定类型的工作负载而设计，即那些在并行操作之间重用工作数据集（比如机器学习算法）的工作负载。为了优化这些类型的工作负载，Spark 引进了内存集群计算的概念，可在内存集群计算中将数据集缓存在内存中，以缩短访问延迟。

Spark 还引进了名为弹性分布式数据集 (RDD) 的抽象。RDD 是分布在一组节点中的只读对象集合。这些集合是弹性的，如果数据集一部分丢失，则可以对它们进行重建。重建部分数据集的过程依赖于容错机制，该机制可以维护“血统”（即充许基于数据衍生过程重建部分数据集的信息）。RDD 被表示为一个 Scala 对象，并且可以从文件中创建它；一个并行化的切片（遍布于节点之间）；另一个 RDD 的转换形式；并且最终会彻底改变现有 RDD 的持久性，比如请求缓存在内存中。

Spark 中的应用程序称为驱动程序，这些驱动程序可实现在单一节点上执行的操作或在一组节点上并行执行的操作。与 Hadoop 类似，Spark 支持单节点集群或多节点集群。对于多节点操作，Spark 依赖于 Mesos 集群管理器。Mesos 为分布式应用程序的资源共享和隔离提供了一个有效平台。该设置充许 Spark 与 Hadoop 共存于节点的一个共享池中。

2）Spark编程模式

ResilientDistributed Datasets （RDD，）  弹性分布式数据集。 RDD是只读的、分区记录的集合。驱动程序可以在数据集上执行两种类型的操作：动作和转换。动作会在数据集上执行一个计算，并向驱动程序返回一个值；而转换会从现有数据集中创建一个新的数据集。动作的示例包括执行一个 Reduce 操作（使用函数）以及在数据集上进行迭代（在每个元素上运行一个函数，类似于Map 操作）。转换示例包括 Map 操作和 Cache 操作（它请求新的数据集存储在内存中）。

每个RDD有5个主要的属性：

一组分片（partition），即数据集的基本组成单位

一个计算每个分片的函数

对parentRDD的依赖，这个依赖描述了RDD之间的lineage

对于key-value的RDD，一个Partitioner

一个列表，存储存取每个partition的preferred位置。对于一个HDFS文件来说，存储每个partition所在的块的位置。

Scala 语言由EcolePolytechniqueFederale de Lausanne（瑞士洛桑市的两所瑞士联邦理工学院之一）开发。Scala 是一种多范式语言，它以一种流畅的、让人感到舒服的方法支持与命令式、函数式和面向对象的语言相关的语言特性。从面向对象的角度来看，Scala 中的每个值都是一个对象。同样，从函数观点来看，每个函数都是一个值。Scala也是属于静态类型，它有一个既有表现力又很安全的类型系统。

此外，Scala是一种虚拟机 (VM) 语言，并且可以通过 Scala 编译器生成的字节码，直接运行在使用 Java Runtime Environment V2 的 Java™ Virtual Machine (JVM) 上。该设置充许 Scala 运行在运行 JVM 的任何地方（要求一个额外的 Scala 运行时库）。它还充许 Scala 利用大量现存的 Java 库以及现有的 Java 代码。

最后，Scala具有可扩展性。该语言（它实际上代表了可扩展语言）被定义为可直接集成到语言中的简单扩展。

## 2、Spark单机模式部署

**1）基本环境**

centos6.2 64位

#java-version //1.8版本

2）安装spark

官网https://spark.apache.org/downloads.html选择最新的 Spark 版本和 Hadoop 版本（暂不用 Hadoop，所以任何版本都行），然后下载压缩包。

解压到spark-2.0.1-bin-hadoop2.7.tgz到/home/spark路径下。

#cd /home

#tar -zxvfspark-2.0.1-bin-hadoop2.7.tgz

#mv spark-2.0.1-bin-hadoop2.7.tgzspark

#cd /spark

#bin/spark-shell

进入scale命令行。Spark 主要使用 Scala 来进行开发，要最大程度地发挥 Spark 的性能，最好掌握scale语言。Spark 还支持 Java 和Python 的接口，但 Java 的语法没有 Scala 简洁，Python 的性能没有 Scala 的高。

3）spark-Shell

Scale> valtextFile = sc.textFile("README.md")

textFile:org.apache.spark.rdd.RDD[String] = README.md MapPartitionsRDD[1] at textFile at<console>:24

RDD对象集合有转换和行动两类操作，转化操作返回新的RDD，行动操作返回具体数据类型。

先看看两个具体的行动操作：统计和取第一行

Scale> textFile.count()

Res1: Long =99

scala>textFile.first()

res2: String= # Apache Spark

在看看转化操作：

Scale>vallinesWithSpark = textFile.filter(line => line.contains("Spark"))

linesWithSpark:org.apache.spark.rdd.RDD[String] = MapPartitionsRDD[2] at filter at <console>:26

Scale> textFile.filter(line=> line.contains("Spark")).count()

res3: Long =19

简单操作可参考：https://spark.apache.org/docs/latest/quick-start.htmlb

## 3、Java开发Spark

为避免学习Scale语言，在Eclipse上用java开发。

1）Maven要升级到3.3.9版本。

卸载原来的版本：

#apt-getautoremove maven 

安装新版本：

cd~/Downloads

wget http://mirrors.tuna.tsinghua.edu.cn/apache/maven/maven-3/3.3.9/binaries/apache-maven-3.3.9-bin.tar.gz

sudo mkdir-p /usr/local/apache-maven

sudo mvapache-maven-3.3.9-bin.tar.gz /usr/local/apache-maven

cd/usr/local/apache-maven

sudo tar-xzvf apache-maven-3.3.9-bin.tar.gz

配置环境变量/etc/profile

#vi/etc/profile

尾部加入：

    export M2_HOME=/usr/local/apache-maven/apache-maven-3.3.9

exportM2=$M2_HOME/bin

exportMAVEN_OPTS="-Xms256m -Xmx512m"

exportPATH=$M2:$PATH

#source /etc/profile

#mvn --version

Apache Maven 3.3.9

2）编译spark的jar包

官网下载源码spark-2.0.1.tgz并解压到spark-2.0.1。在ubuntu14用Maven编译。

进入spark-2.0.1目录执行mvn package编译生成jar文件。

#build/mvn-DskipTests clean package

编译后进入core/target目录提取jar包spark-core_2.11-2.0.1.jar，开发时需要其他功能再引入spark相关jar包。暂不使用HDFS集群，所以不添加一个hadoop-client依赖。

下载已编译spark-2.0.1-bin-hadoop2.7.tgz内jars目录内也可以找到相关jar包。

3）工程实例

Java工程中引入spark-core_2.11-2.0.1.jar、scala-library-2.11.8.jar、spark-sql_2.11-2.0.1.jar、scala-reflect-2.11.8.jar，实现wordcount案例。代码可参考examples里，eclipse里export输出skwc.jar。

提交执行：

bin/spark-submit--class sk.rdd.JavaWordCount --master local /mnt/skwc.jar/home/spark/README.md



## 4、总结




初步了解Spark平台，后续根据需要：

1）深入学习RDD操作和Spark SQL及所支持的各类数据输入和输出格式；

2）部署Spark分布式环境并调优性能，独立资源管理器和Meso、Yarn管理器；

3）掌握Spark Streaming实时流式处理框架；

4）Hadoop集成，含hafs文件系统和Yarn集群管理器集成；

5）应用上需要重点掌握MLib机器学习库。



