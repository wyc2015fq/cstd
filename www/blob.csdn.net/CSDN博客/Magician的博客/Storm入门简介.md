
# Storm入门简介 - Magician的博客 - CSDN博客


2018年01月11日 14:23:46[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：182标签：[Storm																](https://so.csdn.net/so/search/s.do?q=Storm&t=blog)[流式计算																](https://so.csdn.net/so/search/s.do?q=流式计算&t=blog)[编程模型																](https://so.csdn.net/so/search/s.do?q=编程模型&t=blog)[核心组件																](https://so.csdn.net/so/search/s.do?q=核心组件&t=blog)[简介																](https://so.csdn.net/so/search/s.do?q=简介&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=核心组件&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=编程模型&t=blog)个人分类：[Storm																](https://blog.csdn.net/qq_16633405/article/category/7395540)[大数据																](https://blog.csdn.net/qq_16633405/article/category/6905043)[
							](https://blog.csdn.net/qq_16633405/article/category/7395540)
所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)
[
																	](https://so.csdn.net/so/search/s.do?q=编程模型&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=流式计算&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=流式计算&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=Storm&t=blog)

[目录](#目录)
[前言：](#前言)[1、Storm简介](#1storm简介)
[2、Storm与Hadoop的区别](#2storm与hadoop的区别)
[3、Storm核心组件](#3storm核心组件)
[4、Storm编程模型](#4storm编程模型)
[5、流式计算一般架构图（重要）](#5流式计算一般架构图重要)
[总结：](#总结)


# 目录
# 前言：
在介绍Storm之前，先介绍下离线计算。
**离线计算：批量获取数据、批量传输数据、周期性批量计算数据、数据展示**
**代表技术：Sqoop批量导入数据、HDFS批量存储数据、MapReduce批量计算数据、Hive批量计算数据、azkaban任务调度**
与之对应的就是流式计算即Storm的应用场景——流式计算。那么流式计算是什么呢？
**流式计算：**数据实时产生、数据实时传输、数据实时计算、实时展示。
**代表技术：Flume实时采集数据、kafka实时数据存储、Storm实时计算数据、redius实时结果缓存、持久化存储（Mysql）。**
一句话总结：**将源源不断产生的数据实时收集并实时计算，尽可能快的得到计算结果。**
## 1、Storm简介
**Storm用来实时处理数据，特点：低延迟、高可用、分布式、可扩展、数据不丢失。提供简单容易理解的接口，便于开发。**
## 2、Storm与Hadoop的区别
•**Storm处理的数据保存在内存中**，**源源不断**；**Hadoop处理的数据保存在文件系统中，一批一批**。
•**Storm的数据通过网络传输进来；Hadoop的数据保存在磁盘中。**
•**Storm与Hadoop的编程模型相似**
![这里写图片描述](https://img-blog.csdn.net/20180111135322674?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 3、Storm核心组件
![这里写图片描述](https://img-blog.csdn.net/20180111135857504?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20180111135857504?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
•   Nimbus：负责资源分配和任务调度。
•   Supervisor：负责接受nimbus分配的任务，启动和停止属于自己管理的worker进程。—通过配置文件设置当前supervisor上启动多少个worker。
•   Worker：运行具体处理组件逻辑的进程。Worker运行的任务类型只有两种，一种是Spout任务，一种是Bolt任务。
•   Task：worker中每一个spout或bolt的线程称为一个task. 在storm0.8之后，task不再与物理线程对应，不同spout/bolt的task可能会共享一个物理线程，该线程称为executor。Task=线程=executor
---
**补充知识：**
Zookeeper：保存任务分配的信息、心跳信息、元数据信息。
并发度:用户指定的一个任务，可以被多个线程执行，并发度的数量等于线程的数量
Worker与topology：
一个worker只属于一个topology,每个worker中运行的task只能属于这个topology。 反之，一个topology包含多个worker，其实就是这个topology运行在多个worker上。
一个topology要求的worker数量如果不被满足，集群在任务分配时，根据现有的worker先运行topology。**如果当前集群中worker数量为0，那么最新提交的topology将只会被标识active，不会运行，只有当集群有了空闲资源之后，才会被运行。**
## 4、Storm编程模型
![这里写图片描述](https://img-blog.csdn.net/20180111140824055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20180111140824055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
•   Topology：Storm中运行的一个实时应用程序的名称。（拓扑）
•   Spout：在一个topology中获取源数据流的组件。接受外部数据源的组件，将外部数据源转化成Storm内部的数据，以Tuple为基本的传输单元下发给Bolt
•   Bolt：接受数据然后执行处理的组件,用户可以在其中执行自己想要的操作。接受Spout发送的数据，或上游的bolt的发送的数据。根据业务逻辑进行处理。发送给下一个Bolt或者是存储到某种介质上。介质可以是Redis可以是mysql，或者其他。
•   Tuple：一次消息传递的基本单元，里面封装了一个List对象，用来保存数据,理解为一组消息就是一个Tuple。
•   Stream：表示数据的流向。
•   通常情况下spout会从外部数据源中读取数据，然后转换为topology内部的源数据。
## 5、流式计算一般架构图（重要）
![这里写图片描述](https://img-blog.csdn.net/20180406163633340?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180406163633340?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其中
[
](https://img-blog.csdn.net/20180406163633340?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)flume用来获取数据。
Kafka用来临时保存数据。
Strom用来计算数据。
Redis是个内存数据库，用来保存数据。
[

](https://img-blog.csdn.net/20180406163633340?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 总结：
[
](https://img-blog.csdn.net/20180406163633340?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)对于Storm来说首先你要了解以下几个知识点：
1、离线计算和流式计算是什么以及对应的区别。
2、Storm是什么以及是干什么用的。
3、Storm与Hadoop的区别。
4、Storm的核心组件。（重点）
5、Storm的编程模型。（在新的文章中详细介绍）
6、流式计算的一般架构图。(重点)
7、Storm的应用场景及行业案例。
了解了以上的知识点，你就算是和博主一样刚刚入门。下面将会围绕上面的一些重要的知识点就行详细的介绍（如编程模型以及对应的WC案例的分析：详见：[https://blog.csdn.net/qq_16633405/article/details/79835012](https://blog.csdn.net/qq_16633405/article/details/79835012)）

