# Hadoop2.x运行模式/部署/区别 - Simple 专栏 - CSDN博客
2015年01月16日 08:42:08[Simple_Zz](https://me.csdn.net/love284969214)阅读数：747标签：[Hadoop																[概念](https://so.csdn.net/so/search/s.do?q=概念&t=blog)](https://so.csdn.net/so/search/s.do?q=Hadoop&t=blog)
个人分类：[hadoop生态圈](https://blog.csdn.net/love284969214/article/category/7975457)
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
# 在做hadoop开发时，理解hadoop的运行模式尤为重要。
### 首先我们了解一下两个目录../hadoop/sbin与../hadoop/etc/hadoop
- sbin             用于启动hadoop的脚本文件目录
- etc/hadoop 用于配置启动文件，通过 **.xml 配置来确定hadoop的运行模式
## ![](https://img-blog.csdn.net/20180830145715653?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018083015220627?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## Hadoop的三种启动模式：
Hadoop部署方式分三种，Standalone mode、Pseudo-Distributed mode、Cluster mode，其中前两种都是在单机部署。
- 
### 单机模式（Standalone Mode）
- 默认模式
- 不对配置文件进行修改
- 使用本地文件系统，而不是分布式文件系统。
- 
### 伪分布模式（Pseudo-Distributed Mode）
- 在一台主机模拟一个小规模的集群，在这种模式下Hadoop使用的是分布式文件系统，一般用于程序调试与测试。也可以说 伪分布式 是 完全分布式 的一个特例。
- 在这种模式下，Hadoop使用的是分布式文件系统，各个 Job 由 Manager 来管理独立进程。在单机模式之上增加了代码调试功能，允许检查内存使用情况，HDFS输入输出，以及守护进程交互。Hadoop启动NameNode、DataNode、SecondaryNameNode、ResourceManager、NodeManager，这些守护进程都在同一台机器上运行，是相互独立的Java进程。
- 需要修改配置文件：core-site.xml、hdfs-site.xml、mapred-site.xml、yarn-site.xml。配置见下表
- 格式化文件系统
- 
### 完全分布式（Cluster Mode）
- Hadoop的运行在由多台主机搭建的集群上，是真正的生产环境。
- 在这种模式下，Hadoop在所有的主机上安装JDK、Hadoop、Zookeeper等软件，组成相互连通的网络。
- 主机间设置SSH免密码登录，把各从节点生成的公钥添加到主节点的信任列表中。
- 需要修改配置文件：core-site.xml、hdfs-site.xml、mapred-site.xml、yarn-site.xml。配置见下表
- 格式化文件系统。
|xml文件|属性名称|本地模式|伪分布式|完全分布式|
|----|----|----|----|----|
|core-site.xml|fs.defaultFs (fs.default.name)|file:///(默认)|hdfs://localhost/|hdfs://hadoop1|
|hdfs-site.xml|dfs.replication|N/A|1|3(默认)|
|mapred-site.xml|mapreduce.framework.name|local(默认）|yarn|yarn|
|yarn-site.xml|yarn.resoucemanager.hostnameyarn.nodemanager.auxservice|N/AN/A|localhostmapreduce_shuffle|resoucemanagermaperduce_shuffle|
### **注意：**
- 
### **在本地模式下，使用本地文件系统和本地MapReduce运行。**
- 
### **在分布式模式下，将启动HDFS和YARN守护进程运行。**
