# Flink on yarn初步讲解 - Spark高级玩法 - CSDN博客
2018年07月02日 00:18:28[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1379
**前情回顾**
对于flink的基本概念和基本运行模式讲解的内容请参考这篇文章《[Flink流式处理概念简介](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483910&idx=1&sn=9e40343a26760d32c49715e626b0db1f&chksm=9f38e12ea84f683846694f50505e8cb73a4298d22812a0f601edf0b260622e24d324b0540809&scene=21#wechat_redirect)》。本文主要是讲解flink on yarn的运行原理及基本使用，后面会再出一篇文章将flink on yarn的部署及各种调优。
Flink运行时由两种类型的进程组成：
1),JobManager也叫master协调分布式执行。他们调度任务，协调checkpoints，协调故障恢复等。至少有一个JobManager。高可用情况下可以启动多个JobManager，其中一个选举为leader，其余为standby。
2),TaskManager也叫worker，负责执行具体的tasks，缓存，交换数据流。至少有一个TaskManager。
JobManager和TaskManager可以用很多种方式启动：可以直接作为Standalone集群启动，也可以被yarn或者Mesos管理。TaskManager连接到JobManager，宣布自己可用，并接受分配的工作。
客户端不是runtime 和程序执行的一部分，而是用于准备并发送数据流到JobManager。
之后，客户端可以断开连接或保持连接以接收进度报告。客户端作为触发执行的Java / Scala程序的一部分运行，或在命令行进程中运行./bin/flink运行。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWR5SD1zqHaUDX5VSllV5a19dhuFtT1Qdicgibd9EsVc9vqkyl1kjlh5HnXrIbWLBt92SIic3aOFqf6A/640?wx_fmt=png)
**Flink on yarn**
根据前面的讲解的实际上，就很容易可以理解flink on yarn的部署了。
首先，我们要启动一套jobmanager和taskmanager集群
然后，我们可以向该集群提交我们的应用程序，由上个步骤启动的jobmanager和TaskManager运行我们的应用。
有上面两个步骤，我们可以先给出flink on yarn的交互图，如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWR5SD1zqHaUDX5VSllV5a1ib2ibBsMaS2UzdO2zXWNDspTqib3RDaqKuO8f6UYXRJ8dI1X0CBZX6E3g/640?wx_fmt=png)
当启动新的Flink YARN会话时，客户端首先检查请求的资源（容器和内存）是否可用。之后，它将包含flink的jar和配置上传到HDFS（步骤1）。
客户端的下一步是请求（步骤2）YARN容器以启动ApplicationMaster（步骤3）。由于客户端将配置和jar文件注册为容器的资源，因此在该特定机器上运行的YARN的NodeManager将负责准备容器（例如，下载文件）。一旦完成，ApplicationMaster（AM）就会启动。
该JobManager和AM在同一容器中运行。成功启动后，AM就很容易知道JobManager的地址（它自己的主机）。它为TaskManagers生成一个新的Flink配置文件（以便它们可以连接到JobManager）。该文件也被上传到HDFS。此外，AM容器还提供Flink的Web界面。YARN代码分配的所有端口都是临时端口。这允许用户并行执行多个Flink YARN会话。
之后，AM开始为Flink的TaskManagers分配容器，它将从HDFS下载jar文件和修改后的配置。完成这些步骤后，Flink即会设置并准备接受作业。
**测试flink on yarn**
首先，启动一个yarn session，并且给其分配4个taskmanager，每个taskmanager 4GB内存。
```bash
# get the hadoop2 package from the Flink download page at# http://flink.apache.org/downloads.htmlcurl -O <flink_hadoop2_download_url>tar xvzf flink-1.5.0-bin-hadoop2.tgzcd flink-1.5.0/
./bin/yarn-session.sh -n 4 -jm 1024 -tm 4096
```
-jm代表jobmanager的内存大小，-tm代表的是TaskManager的内存大小，-n代表的是n个taskmanager。那么可能会有人问，还有个slot的概念，如何通过参数设置呢，其实只需要-s参数，就可以设置每个TaskManager所包含的slot数。至于slot数的设置技巧我们后面会详细介绍，推荐做法是每个taskmanager包含的slot数呢是处理器数。
接着就是运行一个flink job到yarn上
```bash
# get the hadoop2 package from the Flink download page at# http://flink.apache.org/downloads.htmlcurl -O <flink_hadoop2_download_url>tar xvzf flink-1.5.0-bin-hadoop2.tgzcd flink-1.5.0/
./bin/flink run -m yarn-cluster -yn 4 -yjm 1024 -ytm 4096 ./examples/batch/WordCount.jar
```
可能有人会问，咋没有指定master呢，其实，他会自动找到flink集群并提交job。
前提是，要提前设置hadoop的yarn相关配置，
YARN_CONF_DIR, HADOOP_CONF_DIR or HADOOP_CONF_PATH这几个变量任意一个配置了，都可以让flink读取到yarn的配置文件。
本文翻译整理自flink官网。
