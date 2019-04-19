# Spark-概述、目录文件构成、spark shell、SparkUI - shine302的博客 - CSDN博客
置顶2018年04月17日 22:05:15[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：299
1）Spark是什么？
Spark 是一个用来实现快速而通用的集群计算的平台。
在速度方面，Spark 扩展了广泛使用的MapReduce 计算模型，而且高效地支持更多计算模
式，包括交互式查询和流处理。
Spark 的一个主要特点就是能够在内存中进行计算，因而更快。不过即使是必须在磁盘上
进行的复杂计算，Spark 依然比MapReduce 更加高效。
2）spark软件栈
![](https://img-blog.csdn.net/20180417220217929?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Spark Core：Spark Core 实现了Spark 的基本功能，包含任务调度、内存管理、错误恢复、与存储系统 交互等模块。Spark Core 中还包含了对弹性分布式数据集（resilient distributed dataset，简称RDD）的API 定义
Spark SQL 是Spark 用来操作结构化数据的程序包。
Spark Streaming 是Spark 提供的对实时数据进行流式计算的组件。
Spark 中还包含一个提供常见的机器学习（ML）功能的程序库，叫作MLlib。
GraphX 是用来操作图（比如社交网络的朋友关系图）的程序库，可以进行并行的图计算。
集群调度器：Spark 设计为可以高效地在一个计算节点到数千个计算节点之间伸缩计算。为了实现这样的要求，同时获得最大灵活性，Spark 支持在各种集群管理器（clustermanager）上运行，包括Hadoop YARN、Apache Mesos，以及Spark 自带的一个简易调度器，叫作独立调度器。
3）Spark目录文件构成
bin：用来和Spark进行交互的一系列可执行文件
core、streaming：Spark项目的主要组件的源代码
4） Spark Shell 短日志输出模式
在conf 目录下创建一个名为log4j.properties 的文件来管理日志设置。Spark
开发者们已经在Spark 中加入了一个日志设置文件的模版，叫作log4j.properties.template。要让日志看起来不那么啰嗦，可以先把这个日志设置模版文件复制一份到conf/log4j.properties 来作为日志设置文件，接下来找到下面这一行：
log4j.rootCategory=INFO, console
然后通过下面的设定降低日志级别，只显示警告及更严重的信息：
log4j.rootCategory=WARN, console
这时再打开shell，你就会看到输出大大减少
5）SparkUI ：4040端口
