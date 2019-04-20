# 经验|如何设置Spark资源 - Spark高级玩法 - CSDN博客
2018年08月20日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：2003
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
经常有人在微信群里问浪尖，到底应该如何配置yarn集群的资源，如何配置spark executor数目，内存及cpu。今天浪尖在这里大致聊聊这几个问题。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)资源调优
Spark和YARN管理的两个主要资源是CPU和内存。磁盘和网络I / O也会影响Spark性能，但Spark和YARN都不会主动管理它们。
应用程序中的每个Spark executor都具有相同的固定数量的核心和相同的固定堆大小。使用--executor-cores命令行参数或通过设置spark.executor.cores属性指定核心数。同样，使用--executor-memory参数或spark.executor.memory属性配置堆大小。 cores属性控制执行程序可以运行的并发任务数。例如，为每个执行程序设置--executor-cores 5，同时运行最多五个任务。 memory属性控制Spark可以缓存的数据量，以及用于分组，聚合和join的shuffle数据结构的内存的最大大小。
还要考虑Spark请求的资源如何适应YARN可用的资源。相关的YARN属性是：
- 
yarn.nodemanager.resource.memory-mb控制每个主机上container使用的最大内存总和。
- 
yarn.nodemanager.resource.cpu-vcores控制每个主机上container使用的最大内核总数。
申请五个executor核心意思就是向YARN请求五个核心。 YARN请求的内存更复杂，原因有两个：
- 
--executor-memory / spark.executor.memory属性控制executor堆大小，但executor也可能使用堆外内存，例如Java NIO直接缓冲区。 spark.yarn.executor.memoryOverhead属性值将添加到executor内存中，以确定每个executor对YARN的完整内存请求。默认为max（384，.07 * spark.executor.memory）。
- 
YARN请求的内存会稍微向上舍入。 yarn.scheduler.minimum-allocation-mb和yarn.scheduler.increment-allocation-mb属性分别控制最小和增量请求值。
下图（不按默认值缩放）显示Spark和YARN中内存属性的层次结构：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVMC94MGuM69RvUmPFIfbJajVhicM4SVHj0U5Uuyzftx42YdVnbSdNZjIeBjdkzsuytyacup6RzUFQ/640?wx_fmt=png)
调整Spark executor的内存大小时，请记住以下几点：
- 
ApplicationMaster是一个可以从yarn上申请container，但自身不会运行executor，它需要占用内存和CPU。在客户端部署模式下，它们默认为1024 MB和一个核心。在集群部署模式下，ApplicationMaster运行驱动程序，因此可考虑使用--driver-memory和--driver-cores配置其资源。
- 
运行具有太多内存的executor通常会导致过多的垃圾收集延迟。对于单个执行程序，请使用64 GB作为上限。
- 
HDFS客户端难以处理许多并发线程。每个executor最多有五个任务可以实现完全写入吞吐量，因此请将每个执行程序的核心数保持在该数量之下。
- 
运行微型executor（例如，使用单个核心和运行单个任务所需的足够内存）会抵消在单个JVM中运行多个任务的好处。例如，广播变量必须在每个执行程序上复制一次，因此许多小执行程序会导致更多的数据副本。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)资源调优示例
假设一个集群有六台机器运行nodemanager，每个主机配备16个内核和64 GB内存。
NodeManager容量yarn.nodemanager.resource.memory-mb和yarn.nodemanager.resource.cpu-vcores应分别设置为63 * 1024 = 64512（兆字节）和15。避免将100％的资源分配给YARN容器，因为主机需要一些资源来运行操作系统和Hadoop守护程序。在这种情况下，为这些系统进程留下一个GB和一个核心，对于有hbase共存的需要预留更多的内存和cpu。
可以考虑使用--num-executors 6 --executor-cores 15 --executor-memory 63G。但是，这种方法不行的：
- 
63 GB加上executor内存开销超出了NodeManagers的63 GB容量。
- 
ApplicationMaster在其中一个主机上占用一个核心，因此该主机上没有15核执行器的空间。
- 
每个执行程序15个核心可能导致较差的HDFS I / O吞吐量。
相反，使用--num-executors 17 --executor-cores 5 --executor-memory 19G：
- 
这导致所有主机上有三个executor，除了具有两个执行程序的ApplicationMaster。
- 
--executor-memory计算为（63/3 executor /每台主机）：
21 * 0.07 = 1.47GB。 21 - 1.47~19GB。
其实，资源配置调优全靠经验和自己对集群，分布式，数据量，业务及自己代码的理解，然后多加测试，这样形成自己的经验认知。
希望对大家有帮助。
更多大数据经验，技巧，案例，可以加入浪尖知识星球。
[完]
**推荐阅读：**
[戳破 | hive on spark 调优点](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485198&idx=1&sn=2ab4a6297443d0a6822d193d6ada0823&chksm=9f38e426a84f6d300a283df4a1d360bc8fc0b9b0e7965af46e039f34f46f6b9753d30d95a4d2&scene=21#wechat_redirect)
[关于Spark Streaming感知kafka动态分区的问题](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485078&idx=1&sn=5be1f2db95f2853651d8fe06cb78d637&chksm=9f38e5bea84f6ca8c9db462cbc1d85e75f0f2b72ec976e0af68cf1435a102261d8849bf96b5c&scene=21#wechat_redirect)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640?wx_fmt=jpeg)
