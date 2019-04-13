
# Yarn在MapReduce中的工作机制 - Magician的博客 - CSDN博客


2018年03月28日 21:02:36[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：214所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)[前言:](#前言)
[1、YARN概述](#1yarn概述)
[2、mapreduce&yarn的工作机制](#2mapreduceyarn的工作机制)
[总结：](#总结)


# 目录
## 前言:
在了解Yarn在MR中的作用的时候需要先了解Yarn是什么。
## 1、YARN概述
Yarn是一个**资源调度平台**，负责为运算程序提供服务器运算资源，**相当于一个分布式的操作系统平台**，**而mapreduce等运算程序则相当于运行于操作系统之上的应用程序**
**YARN的重要概念**
1、  yarn并不清楚用户提交的程序的运行机制
2、  yarn只提供运算资源的调度（用户程序向yarn申请资源，yarn就负责分配资源）
3、  yarn中的主管角色叫**ResourceManager**
4、  yarn中具体提供运算资源的角色叫**NodeManager**
5、  这样一来，yarn其实就与运行的用户程序完全解耦，就意味着yarn上可以运行各种类型的分布式运算程序（mapreduce只是其中的一种），比如mapreduce、storm程序，spark程序，tez ……
6、  所以，spark、storm等运算框架都可以整合在yarn上运行，只要他们各自的框架中有符合yarn规范的资源请求机制即可
7、  Yarn就成为一个通用的资源调度平台，从此，企业中以前存在的各种运算集群都可以整合在一个物理集群上，提高资源利用率，方便数据共享。
## 2、mapreduce&yarn的工作机制
客户端提交程序到yarn的整个过程如下图所示：
![这里写图片描述](https://img-blog.csdn.net/2018032820513147?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**主要分为以下几个关键步骤：**
1、客户端发送Application的提交请求到ResourceManager。
2、客户端收到返回的存储位置信息后，将相关的文件存储到HDFS对应的位置（**实际就是mrAPPmaster所在的node上的位置**）
3、**向yarn申请启动mrappmaster。**
4、yarn将提交的申请信息进行封装转化为一个Task放进队列中等待处理。
5、相应的node领到task，运行mrappmaster，将相关的Job信息下载到本地。
6、mrappmaster向yarn申请运行maptask所需的node，各个node创建各自的容器container来处理maptask。
7、mrappmaster将job的相关信息发送的各个maptask所在的node上。
8、maptask的分区排序。
9、mrappmaster向yarn申请运行reducetask所需node
10、reducetask阶段
11、**mrappmaster向resourcemanager申请注销自己。**
**yarn有内存分离机制会给一个进程提供一块固定的资源空间(CPU+RAM)来运行，类似于一个容器，此处叫做container,一个节点包含多个container。**
## 总结：
最主要了解yarn在整个MR过程中起到的作用：即先通过Yarn来启动mrappmaster，再通过mrappmaster来申请对应的Maptask和ReduceTask的资源，（内部处理任务的单元是一个Container容器）任务执行完毕后mrappmaster向Yarn申请注销自己。

