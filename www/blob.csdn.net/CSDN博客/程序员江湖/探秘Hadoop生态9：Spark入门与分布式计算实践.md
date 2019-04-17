# 探秘Hadoop生态9：Spark入门与分布式计算实践 - 程序员江湖 - CSDN博客





2017年03月08日 09:46:06[黄小斜](https://me.csdn.net/a724888)阅读数：780
所属专栏：[后端技术杂谈](https://blog.csdn.net/column/details/25481.html)









# [](http://blog.csdn.net/u013189988/article/details/52804092)



# Spark 介绍


由 yiyohunter 创建，小路依依 最后一次修改 2016-12-12





行业广泛使用Hadoop来分析他们的数据集。原因是Hadoop框架基于一个简单的编程模型（MapReduce），它支持可扩展，灵活，容错和成本有效的计算解决方案。这里，主要关注的是在处理大型数据集时在查询之间的等待时间和运行程序的等待时间方面保持速度。
Spark由Apache Software Foundation引入，用于加速Hadoop计算软件过程。
对于一个普遍的信念，Spark不是Hadoop的修改版本，并不是真的依赖于Hadoop，因为它有自己的集群管理。 Hadoop只是实现Spark的方法之一。
Spark以两种方式使用Hadoop - 一个是存储，另一个是处理。由于Spark具有自己的集群管理计算，因此它仅使用Hadoop进行存储。

## Apache Spark简介
Apache Spark是一种快速的集群计算技术，专为快速计算而设计。它基于Hadoop MapReduce，它扩展了MapReduce模型，以有效地将其用于更多类型的计算，包括交互式查询和流处理。 Spark的主要特性是它的内存中集群计算，提高了应用程序的处理速度。
Spark旨在涵盖各种工作负载，如批处理应用程序，迭代算法，交互式查询和流式处理。除了在相应系统中支持所有这些工作负载之外，它还减少了维护单独工具的管理负担。

## Apache Spark的演变
Spark是Hadoop在2009年在加州大学伯克利分校的Matei Zaharia的AMPLab开发的子项目之一。它是在2010年根据BSD许可开放。它在2013年捐赠给Apache软件基金会，现在Apache Spark已经成为2014年2月的顶级Apache项目。

## Apache Spark的特性
Apache Spark具有以下功能。

速度

Spark有助于在Hadoop集群中运行应用程序，在内存中速度提高100倍，在磁盘上运行时提高10倍。这可以通过减少对磁盘的读/写操作的数量来实现。它将中间处理数据存储在存储器中。

支持多种语言

Spark在Java，Scala或Python中提供了内置的API。因此，您可以使用不同的语言编写应用程序。 Spark提供了80个高级操作符进行交互式查询。

高级分析

Spark不仅支持“Map”和“reduce”。它还支持SQL查询，流数据，机器学习（ML）和图算法。

## Spark基于Hadoop
下图显示了如何使用Hadoop组件构建Spark的三种方式。


![星火内置在Hadoop](https://7n.w3cschool.cn/attachments/tuploads/spark_sql/spark_built_on_hadoop.jpg)


Spark部署有三种方式，如下所述。
Standalone- Spark独立部署意味着Spark占据HDFS（Hadoop分布式文件系统）顶部的位置，并明确为HDFS分配空间。 这里，Spark和MapReduce将并行运行以覆盖集群上的所有spark作业。
Hadoop Yarn- Hadoop Yarn部署意味着，spark只需运行在Yarn上，无需任何预安装或根访问。 它有助于将Spark集成到Hadoop生态系统或Hadoop堆栈中。 它允许其他组件在堆栈顶部运行。
Spark in MapReduce (SIMR)- MapReduce中的Spark用于在独立部署之外启动spark job。 使用SIMR，用户可以启动Spark并使用其shell而无需任何管理访问。

## Spark的组件
下图说明了Spark的不同组件。
![星火组件](https://7n.w3cschool.cn/attachments/tuploads/spark_sql/components_of_spark.jpg)

### Apache Spark Core
Spark Core是spark平台的基础通用执行引擎，所有其他功能都是基于。它在外部存储系统中提供内存计算和引用数据集。
Spark SQL
Spark SQL是Spark Core之上的一个组件，它引入了一个称为SchemaRDD的新数据抽象，它为结构化和半结构化数据提供支持。

Spark Streaming
Spark Streaming利用Spark Core的快速调度功能来执行流式分析。它以小批量获取数据，并对这些小批量的数据执行RDD（弹性分布式数据集）转换。

MLlib (Machine Learning Library)
MLlib是Spark之上的分布式机器学习框架，因为基于分布式内存的Spark架构。根据基准，它是由MLlib开发人员针对交替最小二乘法（ALS）实现完成的。 Spark MLlib是基于Hadoop磁盘的Apache Mahout版本的9倍（在Mahout获得了Spark接口之前）。
GraphX
GraphX是Spark上的一个分布式图形处理框架。它提供了一个用于表达图形计算的API，可以通过使用Pregel抽象API为用户定义的图形建模。它还为此抽象提供了一个优化的运行时。




# Spark RDD


由 yiyohunter 创建，youj 最后一次修改 2016-12-12




## 弹性分布式数据集

弹性分布式数据集(RDD)是Spark的基本数据结构。它是一个不可变的分布式对象集合。 RDD中的每个数据集划分为逻辑分区，可以在集群的不同节点上计算。 RDD可以包含任何类型的Python，Java或Scala对象，包括用户定义的类。

形式上，RDD是只读的，分区的记录集合。 RDD可以通过对稳定存储器或其他RDD上的数据的确定性操作来创建。 RDD是可以并行操作的元件的容错集合。

有两种方法来创建RDD - 并行化驱动程序中的现有集合，或引用外部存储系统中的数据集，例如共享文件系统，HDFS，HBase或提供Hadoop输入格式的任何数据源。

Spark使用RDD的概念来实现更快和更高效的MapReduce操作。让我们首先讨论MapReduce操作是如何发生的，以及为什么它们不那么高效。

## MapReduce中的数据共享缓慢

MapReduce被广泛应用于在集群上使用并行的分布式算法来处理和生成大型数据集。它允许用户使用一组高级操作符来编写并行计算，而不必担心工作分布和容错。

不幸的是，在大多数当前框架中，在计算之间重复使用数据(例如：两个MapReduce作业之间)的唯一方法是将其写入外部稳定存储系统(例如：HDFS)。虽然这个框架提供了访问集群的计算资源的许多抽象，用户仍然想要更多。

迭代和交互应用程序都需要跨并行作业更快的数据共享。由于复制，序列化和磁盘IO，MapReduce中的数据共享速度很慢。关于存储系统，大多数Hadoop应用程序，他们花费90%以上的时间做HDFS读写操作。

## MapReduce迭代运算

在多阶段应用程序中跨多个计算重用中间结果。下图说明了当前框架如何工作，同时对MapReduce执行迭代操作。这会由于数据复制，磁盘I / O和序列化而招致大量开销，这使得系统变慢。



![MapReduce的迭代操作](https://7n.w3cschool.cn/attachments/tuploads/spark_sql/iterative_operations_on_mapreduce.jpg)

# MapReduce上的交互操作
用户对同一数据子集运行即席查询。 每个查询将对稳定存储执行磁盘I / O，这可以支配应用程序执行时间。
下图说明了当在MapReduce上执行交互式查询时当前框架如何工作。


![MapReduce的交互式操作](https://7n.w3cschool.cn/attachments/tuploads/spark_sql/interactive_operations_on_mapreduce.jpg)

## 使用Spark RDD进行数据共享
由于复制，序列化和磁盘IO，MapReduce中的数据共享速度很慢。大多数Hadoop应用程序，他们花费90％以上的时间做HDFS读写操作。
认识到这个问题，研究人员开发了一个名为Apache Spark的专门框架。 spark的关键思想是弹性分布式数据集（RDD）;它支持内存中处理计算。这意味着，它将存储器的状态存储为作业中的对象，并且对象可以在这些作业之间共享。内存中的数据共享比网络和磁盘快10到100倍。
让我们现在尝试找出Spark RDD中如何进行迭代和交互操作。

## Spark RDD的迭代操作
下图给出了Spark RDD的迭代操作。它将中间结果存储在分布式存储器中，而不是稳定存储（磁盘），并使系统更快。
注意 - 如果分布式内存（RAM）不足以存储中间结果（JOB的状态），则它将这些结果存储在磁盘上


![在星火RDD迭代操作](https://7n.w3cschool.cn/attachments/tuploads/spark_sql/iterative_operations_on_spark_rdd.jpg)

## 在星火RDD交互式操作

此图显示星火RDD交互式操作。如果不同的查询在同一组数据的反复运行，该特定数据可被保存在内存中获得更好的执行时间。

![在星火RDD交互式操作](https://7n.w3cschool.cn/attachments/tuploads/spark_sql/interactive_operations_on_spark_rdd.jpg)

默认情况下，每次对其执行操作时，都可以重新计算每个已转换的RDD。 但是，您还可以在内存中保留RDD，在这种情况下，Spark将保持集群上的元素更快的访问，下次查询它。 还支持在磁盘上持久存储RDD，或者跨多个节点进行复制。








# Spark SQL简介


由 yiyohunter 创建，youj 最后一次修改 2016-12-12






Spark为结构化数据处理引入了一个称为Spark SQL的编程模块。它提供了一个称为DataFrame的编程抽象，并且可以充当分布式SQL查询引擎。

## Spark SQL的特性

以下是Spark SQL的功能 

集成

无缝地将SQL查询与Spark程序混合。 Spark SQL允许您将结构化数据作为Spark中的分布式数据集(RDD)进行查询，在Python，Scala和Java中集成了API。这种紧密的集成使得可以轻松地运行SQL查询以及复杂的分析算法。

统一数据访问

加载和查询来自各种来源的数据。 Schema-RDDs提供了一个有效处理结构化数据的单一接口，包括Apache Hive表，镶木地板文件和JSON文件。

Hive兼容性

在现有仓库上运行未修改的Hive查询。 Spark SQL重用了Hive前端和MetaStore，为您提供与现有Hive数据，查询和UDF的完全兼容性。只需将其与Hive一起安装即可。

标准连接

通过JDBC或ODBC连接。 Spark SQL包括具有行业标准JDBC和ODBC连接的服务器模式。

可扩展性

对于交互式查询和长查询使用相同的引擎。 Spark SQL利用RDD模型来支持中查询容错，使其能够扩展到大型作业。不要担心为历史数据使用不同的引擎。

## Spark SQL架构

下图说明了Spark SQL的体系结构



![星火SQL架构](https://7n.w3cschool.cn/attachments/tuploads/spark_sql/spark_sql_architecture.jpg)


此架构包含三个层，即Language API，Schema RDD和数据源。

语言API

Spark与不同的语言和Spark SQL兼容。 它也是由这些语言支持的API（python，scala，java，HiveQL）。

模式RDD

Spark Core是使用称为RDD的特殊数据结构设计的。 通常，Spark SQL适用于模式，表和记录。 因此，我们可以使用Schema RDD作为临时表。 我们可以将此Schema RDD称为数据帧。

数据源

通常spark-core的数据源是文本文件，Avro文件等。但是，Spark SQL的数据源不同。 这些是Parquet文件，JSON文档，HIVE表和Cassandra数据库。
我们将在随后的章节中更多地讨论这些。





# Spark SQL 数据帧


由 yiyohunter 创建，youj 最后一次修改 2016-12-12






DataFrame是一个分布式数据集合，它被组织成命名列。从概念上讲，它相当于具有良好优化技术的关系表。

DataFrame可以从不同来源的数组构造，例如Hive表，结构化数据文件，外部数据库或现有RDD。这个API是为现代大数据和数据科学应用程序设计的，从Python的R ProgrammingandPandas中的DataFrame中获得灵感。

## DataFrame的特性

下面是一些DataFrame的一些特征：

能够将单个节点集群上的大小为Kilobytes到Petabytes的数据处理为大型集群。

支持不同的数据格式(Avro，csv，弹性搜索和Cassandra)和存储系统(HDFS，HIVE表，mysql等)。

通过Spark SQL Catalyst优化器(树变换框架)的最先进的优化和代码生成。

可以通过Spark-Core轻松地与所有大数据工具和框架集成。

提供用于Python，Java，Scala和R编程的API。

## SQLContext

SQLContext是一个类，用于初始化Spark SQL的功能。初始化SQLContext类对象需要SparkContext类对象(sc)。

以下命令用于通过spark-shell初始化SparkContext。


$ spark-shell


默认情况下，SparkContext对象在spark-shell启动时用namesc初始化。
使用以下命令创建SQLContext。

scala> val sqlcontext = new org.apache.spark.sql.SQLContext(sc)

例

让我们考虑一个名为employeesee.json的JSON文件中的员工记录示例。 使用以下命令创建DataFrame(df)并读取名为employee.json的JSON文档，并具有以下内容。

employee.json-将此文件放在currentscala>指针所在的目录中。


{
   {"id" : "1201", "name" : "satish", "age" : "25"}
   {"id" : "1202", "name" : "krishna", "age" : "28"}
   {"id" : "1203", "name" : "amith", "age" : "39"}
   {"id" : "1204", "name" : "javed", "age" : "23"}
   {"id" : "1205", "name" : "prudvi", "age" : "23"}
}

## DataFrame操作

DataFrame为结构化数据操作提供了一个领域特定的语言。 这里，我们包括使用DataFrames的结构化数据处理的一些基本示例。

按照以下步骤执行DataFrame操作 

阅读JSON文档

首先，我们要读取JSON文档。 基于此，生成名为(dfs)的DataFrame。

使用以下命令读取JSON文档namedemployee.json。 数据显示为带有字段id，name和age的表。


scala> val dfs = sqlContext.read.json("employee.json")

输出：字段名称从employee.json自动获取。
dfs: org.apache.spark.sql.DataFrame = [age: string, id: string, name: string]

### 显示数据

如果你想看到的数据框的数据，然后使用以下命令。
scala> dfs.show()

输出：您可以以表格格式查看员工数据。
<console>:22, took 0.052610 s
+----+------+--------+
|age | id   |  name  |
+----+------+--------+
| 25 | 1201 | satish |
| 28 | 1202 | krishna|
| 39 | 1203 | amith  |
| 23 | 1204 | javed  |
| 23 | 1205 | prudvi |
+----+------+--------+

### 使用方法printSchema

如果要查看DataFrame的Structure（Schema），请使用以下命令。
scala> dfs.printSchema()

输出：
root
   |-- age: string (nullable = true)
   |-- id: string (nullable = true)
   |-- name: string (nullable = true)
使用选择方法
使用以下命令从DataFrame的三个列中获取fetchname-column。

scala> dfs.select("name").show()

输出：您可以查看名称列的值。
<console>:22, took 0.044023 s
+--------+
|  name  |
+--------+
| satish |
| krishna|
| amith  |
| javed  |
| prudvi |
+--------+
使用年龄过滤器
使用以下命令查找年龄大于23（age> 23）的雇员。

scala> dfs.filter(dfs("age") > 23).show()

输出
<console>:22, took 0.078670 s
+----+------+--------+
|age | id   | name   |
+----+------+--------+
| 25 | 1201 | satish |
| 28 | 1202 | krishna|
| 39 | 1203 | amith  |
+----+------+--------+
使用groupBy方法
使用以下命令计算同一年龄的员工人数。

scala> dfs.groupBy("age").count().show()

输出：这两个雇员有23岁。
<console>:22, took 5.196091 s
+----+-----+
|age |count|
+----+-----+
| 23 |  2  |
| 25 |  1  |
| 28 |  1  |
| 39 |  1  |
+----+-----+

## 运行SQL查询
SQLContext使应用程序能够在运行SQL函数时以编程方式运行SQL查询，并将结果作为DataFrame返回。
通常，在后台，SparkSQL支持两种不同的方法将现有的RDD转换为DataFrames

|编号|方法和说明|
|----|----|
|1|[使用反射来推断模式](https://www.w3cschool.cn/spark_sql/fanshe.html)此方法使用反射来生成包含特定类型的对象的RDD的模式。|
|2|[以编程方式指定模式](https://www.w3cschool.cn/spark_sql/bianchengfangshi.html)第二种创建DataFrame的方法是通过编程接口，它允许您构造一个模式，然后将其应用到现有的RDD。|






## 搭建开发环境
- 安装 Scala IDE
搭建 Scala 语言开发环境很容易，[Scala IDE 官网](http://scala-ide.org/download/current.html) 下载合适的版本并解压就可以完成安装，本文使用的版本是 4.1.0。

- 安装 Scala 语言包
如果下载的 Scala IDE 自带的 Scala 语言包与 Spark 1.3.1 使用的 Scala 版本 (2.10.x) 不一致，那么就需要下载和本文所使用的 Spark 所匹配的版本，以确保实现的 Scala 程序不会因为版本问题而运行失败。

请下载并安装 [Scala 2.10.5 版本](http://www.scala-lang.org/download/2.10.5.html)

- 安装 JDK
如果您的机器上没有安装 JDK，请下载并安装 1.6 版本以上的 JDK。

- 创建并配置 Spark 工程
打开 Scala IDE，创建一个名称为 spark-exercise 的 Scala 工程。


##### 图 1. 创建 scala 工程
![图 1. 创建 scala 工程](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img01.jpg)
在工程目录下创建一个 lib 文件夹，并且把您的 Spark 安装包下的 spark-assembly jar 包拷贝到 lib 目录下。

##### 图 2. Spark 开发 jar 包
![图 2. Spark 开发 jar 包](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img02.jpg)
并且添加该 jar 包到工程的 classpath 并配置工程使用刚刚安装的 Scala 2.10.5 版本.，工程目录结构如下。

##### 图 3. 添加 jar 包到 classpath
![图 3. 添加 jar 包到 classpath](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img03.jpg)


[回页首](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/#ibm-pcon)

## 运行环境介绍

为了避免读者对本文案例运行环境产生困惑，本节会对本文用到的集群环境的基本情况做个简单介绍。
- 本文所有实例数据存储的环境是一个 8 个机器的 Hadoop 集群，文件系统总容量是 1.12T，NameNode 叫 hadoop036166, 服务端口是 9000。读者可以不关心具体的节点分布，因为这个不会影响到您阅读后面的文章。
- 本文运行实例程序使用的 Spark 集群是一个包含四个节点的 Standalone 模式的集群, 其中包含一个 Master 节点 (监听端口 7077) 和三个 Worker 节点，具体分布如下：
|Server Name|Role|
|----|----|
|hadoop036166|Master|
|hadoop036187|Worker|
|hadoop036188|Worker|
|hadoop036227|Worker|
- Spark 提供一个 Web UI 去查看集群信息并且监控执行结果，默认地址是:http://<spark_master_ip>:8080 ，对于该实例提交后我们也可以到 web 页面上去查看执行结果，当然也可以通过查看日志去找到执行结果。

##### 图 4. Spark 的 web console
![图 4. Spark 的 web console](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img04.jpg)


[回页首](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/#ibm-pcon)

## 案例分析与编程实现

### 案例一

a. 案例描述

提起 Word Count(词频数统计)，相信大家都不陌生，就是统计一个或者多个文件中单词出现的次数。本文将此作为一个入门级案例，由浅入深的开启使用 Scala 编写 Spark 大数据处理程序的大门。

b．案例分析

对于词频数统计，用 Spark 提供的算子来实现，我们首先需要将文本文件中的每一行转化成一个个的单词, 其次是对每一个出现的单词进行记一次数，最后就是把所有相同单词的计数相加得到最终的结果。

对于第一步我们自然的想到使用 flatMap 算子把一行文本 split 成多个单词，然后对于第二步我们需要使用 map 算子把单个的单词转化成一个有计数的 Key-Value 对，即 word -> (word,1). 对于最后一步统计相同单词的出现次数，我们需要使用 reduceByKey 算子把相同单词的计数相加得到最终结果。
c. 编程实现

##### 清单 1.SparkWordCount 类源码

import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._

object SparkWordCount {
 def FILE_NAME:String = "word_count_results_";
 def main(args:Array[String]) {
 if (args.length < 1) {
 println("Usage:SparkWordCount FileName");
 System.exit(1);
 }
 val conf = new SparkConf().setAppName("Spark Exercise: Spark Version Word Count Program");
 val sc = new SparkContext(conf);
 val textFile = sc.textFile(args(0));
 val wordCounts = textFile.flatMap(line => line.split(" ")).map(
                                        word => (word, 1)).reduceByKey((a, b) => a + b)
 //print the results,for debug use.
 //println("Word Count program running results:");
 //wordCounts.collect().foreach(e => {
 //val (k,v) = e
 //println(k+"="+v)
 //});
 wordCounts.saveAsTextFile(FILE_NAME+System.currentTimeMillis());
 println("Word Count program running results are successfully saved.");
 }
}

d. 提交到集群执行

本实例中, 我们将统计 HDFS 文件系统中/user/fams 目录下所有 txt 文件中词频数。其中 spark-exercise.jar 是 Spark 工程打包后的 jar 包，这个 jar 包执行时会被上传到目标服务器的/home/fams 目录下。运行此实例的具体命令如下：

##### 清单 2.SparkWordCount 类执行命令

 ./spark-submit \
--class com.ibm.spark.exercise.basic.SparkWordCount \
--master spark://hadoop036166:7077 \
--num-executors 3 \
--driver-memory 6g --executor-memory 2g \
--executor-cores 2 \
/home/fams/sparkexercise.jar \
hdfs://hadoop036166:9000/user/fams/*.txt

e. 监控执行状态

该实例把最终的结果存储在了 HDFS 上，那么如果程序运行正常我们可以在 HDFS 上找到生成的文件信息

##### 图 5. 案例一输出结果
![图 5. 案例一输出结果](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img05.jpg)
打开 Spark 集群的 Web UI, 可以看到刚才提交的 job 的执行结果。

##### 图 6. 案例一完成状态
![图 6. 案例一完成状态](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img06.jpg)
如果程序还没运行完成，那么我们可以在 Running Applications 列表里找到它。

### 案例二

a. 案例描述

该案例中，我们将假设我们需要统计一个 1000 万人口的所有人的平均年龄，当然如果您想测试 Spark 对于大数据的处理能力，您可以把人口数放的更大，比如 1 亿人口，当然这个取决于测试所用集群的存储容量。假设这些年龄信息都存储在一个文件里，并且该文件的格式如下，第一列是 ID，第二列是年龄。

##### 图 7. 案例二测试数据格式预览
![图 7. 案例二测试数据格式预览](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img07.jpg)
现在我们需要用 Scala 写一个生成 1000 万人口年龄数据的文件，源程序如下：

##### 清单 3. 年龄信息文件生成类源码

 import java.io.FileWriter
 import java.io.File
 import scala.util.Random

 object SampleDataFileGenerator {
 
 def main(args:Array[String]) {
 val writer = new FileWriter(new File("C: \\sample_age_data.txt"),false)
 val rand = new Random()
 for ( i <- 1 to 10000000) {
 writer.write( i + " " + rand.nextInt(100))
 writer.write(System.getProperty("line.separator"))
 }
 writer.flush()
 writer.close()
 }
 }

b. 案例分析

要计算平均年龄，那么首先需要对源文件对应的 RDD 进行处理，也就是将它转化成一个只包含年龄信息的 RDD，其次是计算元素个数即为总人数，然后是把所有年龄数加起来，最后平均年龄=总年龄/人数。

对于第一步我们需要使用 map 算子把源文件对应的 RDD 映射成一个新的只包含年龄数据的 RDD，很显然需要对在 map 算子的传入函数中使用 split 方法，得到数组后只取第二个元素即为年龄信息；第二步计算数据元素总数需要对于第一步映射的结果 RDD 使用 count 算子；第三步则是使用 reduce 算子对只包含年龄信息的 RDD 的所有元素用加法求和；最后使用除法计算平均年龄即可。

由于本例输出结果很简单，所以只打印在控制台即可。

c. 编程实现

##### 清单 4.AvgAgeCalculator 类源码

import org.apache.spark.SparkConf
import org.apache.spark.SparkContext
object AvgAgeCalculator {
 def main(args:Array[String]) {
 if (args.length < 1){
 println("Usage:AvgAgeCalculator datafile")
 System.exit(1)
 }
 val conf = new SparkConf().setAppName("Spark Exercise:Average Age Calculator")
 val sc = new SparkContext(conf)
 val dataFile = sc.textFile(args(0), 5);
 val count = dataFile.count()
 val ageData = dataFile.map(line => line.split(" ")(1))
 val totalAge = ageData.map(age => Integer.parseInt(
                                String.valueOf(age))).collect().reduce((a,b) => a+b)
 println("Total Age:" + totalAge + ";Number of People:" + count )
 val avgAge : Double = totalAge.toDouble / count.toDouble
 println("Average Age is " + avgAge)
 }
}

d. 提交到集群执行

要执行本实例的程序，需要将刚刚生成的年龄信息文件上传到 HDFS 上，假设您刚才已经在目标机器上执行生成年龄信息文件的 Scala 类，并且文件被生成到了/home/fams 目录下。

那么您需要运行一下 HDFS 命令把文件拷贝到 HDFS 的/user/fams 目录。

##### 清单 5. 年龄信息文件拷贝到 HDFS 目录的命令

hdfs dfs –copyFromLocal /home/fams /user/fams

##### 清单 6.AvgAgeCalculator 类的执行命令

 ./spark-submit \
 --class com.ibm.spark.exercise.basic.AvgAgeCalculator \
 --master spark://hadoop036166:7077 \
 --num-executors 3 \
 --driver-memory 6g \
 --executor-memory 2g \
 --executor-cores 2 \
 /home/fams/sparkexercise.jar \
 hdfs://hadoop036166:9000/user/fams/inputfiles/sample_age_data.txt

e. 监控执行状态

在控制台您可以看到如下所示信息：

##### 图 8. 案例二输出结果
![图 8. 案例二输出结果](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img08.jpg)
我们也可以到 Spark Web Console 去查看 Job 的执行状态

##### 图 9. 案例二完成状态
![图 9. 案例二完成状态](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img09.jpg)
### 案例三

a. 案例描述

本案例假设我们需要对某个省的人口 (1 亿) 性别还有身高进行统计，需要计算出男女人数，男性中的最高和最低身高，以及女性中的最高和最低身高。本案例中用到的源文件有以下格式, 三列分别是 ID，性别，身高 (cm)。

##### 图 10. 案例三测试数据格式预览
![图 10. 案例三测试数据格式预览](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img10.jpg)
我们将用以下 Scala 程序生成这个文件，源码如下：

##### 清单 7. 人口信息生成类源码

import java.io.FileWriter
import java.io.File
import scala.util.Random

object PeopleInfoFileGenerator {
 def main(args:Array[String]) {
 val writer = new FileWriter(new File("C:\\LOCAL_DISK_D\\sample_people_info.txt"),false)
 val rand = new Random()
 for ( i <- 1 to 100000000) {
 var height = rand.nextInt(220)
 if (height < 50) {
 height = height + 50
 }
 var gender = getRandomGender
 if (height < 100 && gender == "M")
 height = height + 100
 if (height < 100 && gender == "F")
 height = height + 50
 writer.write( i + " " + getRandomGender + " " + height)
 writer.write(System.getProperty("line.separator"))
 }
 writer.flush()
 writer.close()
 println("People Information File generated successfully.")
 }
 
 def getRandomGender() :String = {
 val rand = new Random()
 val randNum = rand.nextInt(2) + 1
 if (randNum % 2 == 0) {
 "M"
 } else {
 "F"
 }
 }
}

b. 案例分析

对于这个案例，我们要分别统计男女的信息，那么很自然的想到首先需要对于男女信息从源文件的对应的 RDD 中进行分离，这样会产生两个新的 RDD，分别包含男女信息；其次是分别对男女信息对应的 RDD 的数据进行进一步映射，使其只包含身高数据，这样我们又得到两个 RDD，分别对应男性身高和女性身高；最后需要对这两个 RDD 进行排序，进而得到最高和最低的男性或女性身高。

对于第一步，也就是分离男女信息，我们需要使用 filter 算子，过滤条件就是包含”M” 的行是男性，包含”F”的行是女性；第二步我们需要使用 map 算子把男女各自的身高数据从 RDD 中分离出来；第三步我们需要使用 sortBy 算子对男女身高数据进行排序。

c. 编程实现

在实现上，有一个需要注意的点是在 RDD 转化的过程中需要把身高数据转换成整数，否则 sortBy 算子会把它视为字符串，那么排序结果就会受到影响，例如 身高数据如果是：123,110,84,72,100，那么升序排序结果将会是 100,110,123,72,84，显然这是不对的。

##### 清单 8.PeopleInfoCalculator 类源码

object PeopleInfoCalculator {
 def main(args:Array[String]) {
 if (args.length < 1){
 println("Usage:PeopleInfoCalculator datafile")
 System.exit(1)
 }
 val conf = new SparkConf().setAppName("Spark Exercise:People Info(Gender & Height) Calculator")
 val sc = new SparkContext(conf)
 val dataFile = sc.textFile(args(0), 5);
 val maleData = dataFile.filter(line => line.contains("M")).map(
                              line => (line.split(" ")(1) + " " + line.split(" ")(2)))
 val femaleData = dataFile.filter(line => line.contains("F")).map(
                              line => (line.split(" ")(1) + " " + line.split(" ")(2)))
 //for debug use
 //maleData.collect().foreach { x => println(x)}
 //femaleData.collect().foreach { x => println(x)}
 val maleHeightData = maleData.map(line => line.split(" ")(1).toInt)
 val femaleHeightData = femaleData.map(line => line.split(" ")(1).toInt)
 //for debug use
 //maleHeightData.collect().foreach { x => println(x)}
 //femaleHeightData.collect().foreach { x => println(x)}
 val lowestMale = maleHeightData.sortBy(x => x,true).first()
 val lowestFemale = femaleHeightData.sortBy(x => x,true).first()
 //for debug use
 //maleHeightData.collect().sortBy(x => x).foreach { x => println(x)}
 //femaleHeightData.collect().sortBy(x => x).foreach { x => println(x)}
 val highestMale = maleHeightData.sortBy(x => x, false).first()
 val highestFemale = femaleHeightData.sortBy(x => x, false).first()
 println("Number of Male Peole:" + maleData.count())
 println("Number of Female Peole:" + femaleData.count())
 println("Lowest Male:" + lowestMale)
 println("Lowest Female:" + lowestFemale)
 println("Highest Male:" + highestMale)
 println("Highest Female:" + highestFemale)
 }
}

d. 提交到集群执行

在提交该程序到集群执行之前，我们需要将刚才生成的人口信息数据文件上传到 HDFS 集群，具体命令可以参照上文。

##### 清单 9.PeopleInfoCalculator 类的执行命令

 ./spark-submit \
 --class com.ibm.spark.exercise.basic.PeopleInfoCalculator \
 --master spark://hadoop036166:7077 \
 --num-executors 3 \
 --driver-memory 6g \
 --executor-memory 3g \
 --executor-cores 2 \
 /home/fams/sparkexercise.jar \
 hdfs://hadoop036166:9000/user/fams/inputfiles/sample_people_info.txt

e. 监控执行状态

对于该实例，如程序中打印的一样，会在控制台显示如下信息：

##### 图 11. 案例三输出结果
![图 11. 案例三输出结果](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img11.jpg)
在 Spark Web Console 里可以看到具体的执行状态信息

##### 图 12. 案例三完成状态
![图 12. 案例三完成状态](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img12.jpg)
### 案例四

a. 案例描述

该案例中我们假设某搜索引擎公司要统计过去一年搜索频率最高的 K 个科技关键词或词组，为了简化问题，我们假设关键词组已经被整理到一个或者多个文本文件中，并且文档具有以下格式。

##### 图 13. 案例四测试数据格式预览
![图 13. 案例四测试数据格式预览](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img13.jpg)
我们可以看到一个关键词或者词组可能出现多次，并且大小写格式可能不一致。

b. 案例分析

要解决这个问题，首先我们需要对每个关键词出现的次数进行计算，在这个过程中需要识别不同大小写的相同单词或者词组，如”Spark”和“spark” 需要被认定为一个单词。对于出现次数统计的过程和 word count 案例类似；其次我们需要对关键词或者词组按照出现的次数进行降序排序，在排序前需要把 RDD 数据元素从 (k,v) 转化成 (v,k)；最后取排在最前面的 K 个单词或者词组。

对于第一步，我们需要使用 map 算子对源数据对应的 RDD 数据进行全小写转化并且给词组记一次数，然后调用 reduceByKey 算子计算相同词组的出现次数；第二步我们需要对第一步产生的 RDD 的数据元素用 sortByKey 算子进行降序排序；第三步再对排好序的 RDD 数据使用 take 算子获取前 K 个数据元素。

c. 编程实现

##### 清单 10.TopKSearchKeyWords 类源码

import org.apache.spark.SparkConf
import org.apache.spark.SparkContext

object TopKSearchKeyWords {
 def main(args:Array[String]){
 if (args.length < 2) {
 println("Usage:TopKSearchKeyWords KeyWordsFile K");
 System.exit(1)
 }
 val conf = new SparkConf().setAppName("Spark Exercise:Top K Searching Key Words")
 val sc = new SparkContext(conf)
 val srcData = sc.textFile(args(0))
 val countedData = srcData.map(line => (line.toLowerCase(),1)).reduceByKey((a,b) => a+b)
 //for debug use
 //countedData.foreach(x => println(x))
 val sortedData = countedData.map{ case (k,v) => (v,k) }.sortByKey(false)
 val topKData = sortedData.take(args(1).toInt).map{ case (v,k) => (k,v) }
 topKData.foreach(println)
 }
}

d. 提交到集群执行

##### 清单 11.TopKSearchKeyWords 类的执行命令

 ./spark-submit \
 --class com.ibm.spark.exercise.basic.TopKSearchKeyWords \
 --master spark://hadoop036166:7077 \
 --num-executors 3 \
 --driver-memory 6g \
 --executor-memory 2g \
 --executor-cores 2 \
 /home/fams/sparkexercise.jar \
 hdfs://hadoop036166:9000/user/fams/inputfiles/search_key_words.txt

e. 监控执行状态

如果程序成功执行，我们将在控制台看到以下信息。当然读者也可以仿照案例二和案例三那样，自己尝试使用 Scala 写一段小程序生成此案例需要的源数据文件，可以根据您的 HDFS 集群的容量，生成尽可能大的文件，用来测试本案例提供的程序。

##### 图 14. 案例四输出结果
![图 14. 案例四输出结果](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img14.jpg)
##### 图 15. 案例四完成状态
![图 15. 案例四完成状态](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/img15.jpg)


[回页首](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/#ibm-pcon)

## Spark job 的执行流程简介

我们可以发现，Spark 应用程序在提交执行后，控制台会打印很多日志信息，这些信息看起来是杂乱无章的，但是却在一定程度上体现了一个被提交的 Spark job 在集群中是如何被调度执行的，那么在这一节，将会向大家介绍一个典型的 Spark job 是如何被调度执行的。

我们先来了解以下几个概念：

DAG: 即 Directed Acyclic Graph，有向无环图，这是一个图论中的概念。如果一个[有向图](http://baike.baidu.com/view/807915.htm)无法从某个顶点出发经过若干条边回到该点，则这个图是一个有向无环图。

Job：我们知道，Spark 的计算操作是 lazy 执行的，只有当碰到一个动作 (Action) 算子时才会触发真正的计算。一个 Job 就是由动作算子而产生包含一个或多个 Stage 的计算作业。

Stage：Job 被确定后,Spark 的调度器 (DAGScheduler) 会根据该计算作业的计算步骤把作业划分成一个或者多个 Stage。Stage 又分为 ShuffleMapStage 和 ResultStage，前者以 shuffle 为输出边界，后者会直接输出结果，其边界可以是获取外部数据，也可以是以一个 ShuffleMapStage 的输出为边界。每一个 Stage 将包含一个 TaskSet。

TaskSet： 代表一组相关联的没有 shuffle 依赖关系的任务组成任务集。一组任务会被一起提交到更加底层的 TaskScheduler。

Task：代表单个数据分区上的最小处理单元。分为 ShuffleMapTask 和 ResultTask。ShuffleMapTask 执行任务并把任务的输出划分到 (基于 task 的对应的数据分区) 多个 bucket(ArrayBuffer) 中,ResultTask 执行任务并把任务的输出发送给驱动程序。

Spark 的作业任务调度是复杂的，需要结合源码来进行较为详尽的分析，但是这已经超过本文的范围，所以这一节我们只是对大致的流程进行分析。

Spark 应用程序被提交后，当某个动作算子触发了计算操作时，SparkContext 会向 DAGScheduler 提交一个作业，接着 DAGScheduler 会根据 RDD 生成的依赖关系划分 Stage，并决定各个 Stage 之间的依赖关系，Stage 之间的依赖关系就形成了 DAG。Stage 的划分是以 ShuffleDependency 为依据的，也就是说当某个 RDD 的运算需要将数据进行 Shuffle 时，这个包含了 Shuffle 依赖关系的 RDD 将被用来作为输入信息，进而构建一个新的 Stage。我们可以看到用这样的方式划分 Stage，能够保证有依赖关系的数据可以以正确的顺序执行。根据每个 Stage 所依赖的 RDD 数据的 partition 的分布，会产生出与 partition 数量相等的 Task，这些 Task 根据 partition 的位置进行分布。其次对于 finalStage 或是 mapStage 会产生不同的 Task，最后所有的 Task 会封装到 TaskSet 内提交到 TaskScheduler 去执行。有兴趣的读者可以通过阅读 DAGScheduler 和 TaskScheduler 的源码获取更详细的执行流程。



[回页首](https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice1/#ibm-pcon)

## 结束语

通过本文，相信读者对如何使用 Scala 编写 Spark 应用程序处理大数据已经有了较为深入的了解。当然在处理实际问题时，情况可能比本文举得例子复杂很多，但是解决问题的基本思想是一致的。在碰到实际问题的时候，首先要对源数据结构格式等进行分析，然后确定如何去使用 Spark 提供的算子对数据进行转化，最终根据实际需求选择合适的算子操作数据并计算结果。本文并未介绍其它 Spark 模块的知识，显然这不是一篇文章所能完成的，希望以后会有机会总结更多的 Spark 应用程序开发以及性能调优方面的知识，写成文章与更多的 Spark 技术爱好者分享，一起进步。由于时间仓促并且本人知识水平有限，文章难免有未考虑周全的地方甚至是错误，希望各位朋友不吝赐教。有任何问题，都可以在文末留下您的评论，我会及时回复。












