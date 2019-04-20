# Hadoop3.0稳定版的安装部署 - Spark高级玩法 - CSDN博客
2017年12月15日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：11903
**号外：本公众号改名为Spark学习技巧了。**
Apache Hadoop 3.0.0在前一个主要发行版本（hadoop-2.x）中包含了许多重要的增强功能。
这个版本通常是可用的（GA），api稳定性和质量已经到了可以再生成中使用的步骤。
**一 概览**
**1，java8是必须**
所有hadoop 的jar都是利用java8的运行时版本进行编译的。依然在使用java7或者更低版本的用户必须升级到Java8.
**2，HDFS支持纠删码(Erasure Coding)**
与副本相比纠删码是一种更节省空间的数据持久化存储方法。标准编码(比如Reed-Solomon(10,4))会有1.4 倍的空间开销；然而HDFS副本则会有3倍的空间开销。因为纠删码额外开销主要是在重建和执行远程读，它传统用于存储冷数据，即不经常访问的数据。当部署这个新特性时用户应该考虑纠删码的网络和CPU 开销。
**3，MapReduce任务级本地优化**
MapReduce添加了Map输出collector的本地实现。对于shuffle密集型的作业来说，这将会有30%以上的性能提升。更多内容请参见 MAPREDUCE-2841
**4，支持多于2个的NameNodes**
最初的HDFS NameNode high-availability实现仅仅提供了一个active NameNode和一个Standby NameNode；并且通过将编辑日志复制到三个JournalNodes上，这种架构能够容忍系统中的任何一个节点的失败。然而，一些部署需要更高的容错度。我们可以通过这个新特性来实现，其允许用户运行多个Standby
 NameNode。比如通过配置三个NameNode和五个JournalNodes，这个系统可以容忍2个节点的故障，而不是仅仅一个节点。HDFS high-availability文档已经对这些信息进行了更新，我们可以阅读这篇文档了解如何配置多于2个NameNodes。,
**5，多个服务的默认端口被改变**
在此之前，多个Hadoop服务的默认端口都属于Linux的临时端口范围（32768-61000）。这就意味着我们的服务在启动的时候可能因为和其他应用程序产生端口冲突而无法启动。现在这些可能会产生冲突的端口已经不再属于临时端口的范围，这些端口的改变会影响NameNode, Secondary NameNode, DataNode以及KMS。与此同时，官方文档也进行了相应的改变，具体可以参见
 HDFS-9427以及HADOOP-12811。下面表格列出了端口变化的情况
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX8oN62EaCOiamlv0Df7bibYMvfED9hemIuH6hjCIY6FKrZ705XBVmHfBJ48qft4GianZ5ZJ0oibfkX5A/?wx_fmt=png)
**6，Intra-datanode均衡器**
一个DataNode可以管理多个磁盘，正常写入操作，各磁盘会被均匀填满。然而，当添加或替换磁盘时可能导致此DataNode内部的磁盘存储的数据严重内斜。这种情况现有的HDFS balancer是无法处理的。这种情况是由新intra-DataNode平衡功能来处理，通过hdfs diskbalancer CLI来调用。更多请参考HDFS
 Commands Guide,
**7，重写守护进程以及任务的堆内存管理**
Hadoop守护进程和MapReduce任务的堆内存管理发生了一系列变化。
HADOOP-10950：介绍了配置守护集成heap大小的新方法。主机内存大小可以自动调整，HADOOP_HEAPSIZE 已弃用。
MAPREDUCE-5785：map和reduce task堆大小的配置方法，所需的堆大小不再需要通过任务配置和Java选项实现。已经指定的现有配置不受此更改影响。
**8，HDFS Router-Based Federation**
HDFS Router-Based Federation 添加了一个 RPC路由层，提供了多个 HDFS 命名空间的联合视图。与现有 ViewFs 和 HDFS Federation 功能类似，不同之处在于挂载表（mount table）由服务器端（server-side）的路由层维护，而不是客户端。这简化了现有 HDFS客户端
 对 federated cluster 的访问。 详细请参见：HDFS-10467,
**9，YARN Resource Types**
YARN 资源模型（YARN resource model）已被推广为支持用户自定义的可数资源类型（support user-defined countable resource types），不仅仅支持 CPU 和内存。比如集群管理员可以定义诸如 GPUs、软件许可证（software licenses）或本地附加存储器（locally-attached
 storage）之类的资源。YARN 任务可以根据这些资源的可用性进行调度。详细请参见： YARN-3926。
**10，基于API来配置 Capacity Scheduler 队列的配置**
OrgQueue 扩展了 capacity scheduler ，通过 REST API 提供了以编程的方式来改变队列的配置，This enables automation of queue configuration management by administrators in the queue’s administer_queue
 ACL.。详细请参见：YARN-5734
**二 hadoop3.0伪分布式安装部署**
**1，配置core-site.xml**
<property>
               <name>hadoop.tmp.dir</name>
               <value>/opt/modules/hadoop-3.0.0/data/tmp</value>
       </property>
       <property>
               <name>fs.defaultFS</name>
               <value>hdfs://Luffy.OnePiece.com:8020</value>
       </property>
**2，配置hdfs-site.xml**
<property>
   <name>dfs.replication</name>
<value>1</value>
</property>
<property>
<name>dfs.permissions</name>
<value>false</value>
</property>
**3，配置yarn-site.xml**
<property>
     <name>yarn.nodemanager.aux-services</name>
     <value>mapreduce_shuffle</value>
</property>
<property>  
   <name>yarn.nodemanager.vmem-check-enabled</name>  
   <value>false</value>  
</property>  
**4，配置mapred-site.xml**
<property>
       <name>mapreduce.framework.name</name>
       <value>yarn</value>
</property>
<property>
 <name>yarn.app.mapreduce.am.env</name>
 <value>HADOOP_MAPRED_HOME=/opt/modules/hadoop-3.0.0</value>
</property>
<property>
 <name>mapreduce.map.env</name>
 <value>HADOOP_MAPRED_HOME=/opt/modules/hadoop-3.0.0</value>
</property>
<property>
 <name>mapreduce.reduce.env</name>
 <value>HADOOP_MAPRED_HOME=/opt/modules/hadoop-3.0.0</value>
</property>
5，配置/etc/profile
export JAVA_HOME=/opt/modules/jdk1.8.0_121
export PATH=$PATH:$JAVA_HOME/bin
export HADOOP_HOME=/opt/modules/hadoop-3.0.0
export HADOOP_PREFIX=$HADOOP_HOME
export HADOOP_MAPRED_HOME=$HADOOP_HOME
export HADOOP_YARN_HOME=$HADOOP_HOME
export HADOOP_MAPRED_HOME=$HADOOP_HOME
export HADOOP_COMMON_HOME=$HADOOP_HOME
export HADOOP_HDFS_HOME=$HADOOP_HOME
export YARN_HOME=$HADOOP_HOME
export HADOOP_CONF_DIR=$HADOOP_HOME/etc/hadoop
export YARN_CONF_DIR=$HADOOP_HOME/etc/hadoop
export PATH=$PATH:$HADOOP_HOME/bin
export PATH=$PATH:$HADOOP_HOME/sbin
**6，格式化namenode**
hdfsnamenode -format
**7，启动**
可以使用sbin目录下的启动命令，hadoop3.0更推荐如下启动命令
bin/hdfs --daemon start namenode
bin/hdfs --daemon start
datanode
bin/yarn --daemon start resourcemanager
bin/yarn --daemon start nodemanager
将start换成stop，即可编程停止命令。启动后jps查看
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX8oN62EaCOiamlv0Df7bibYMn2OUsmM5ZhqGoB6ryibw5TKloAaBdiakV9K3l3ze8A1MdtkIKalKC1OA/?wx_fmt=png)
**8，上传如下数据入hdfs **
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX8oN62EaCOiamlv0Df7bibYM7ugN01wS5zVzZI7ZkWZXkMfG1xYzuGR7vnz8IKPc3a7A1pwZwDmlkg/?wx_fmt=png)
**9，运行自带的wordcount命令**
bin/yarn jar share/hadoop/mapreduce/hadoop-mapreduce-examples-3.0.0.jar wordcount /input /output
执行结束后：
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX8oN62EaCOiamlv0Df7bibYM6jqLCVdt6ibESuXa8e8QC18pshFTdEzN5KJTDuzthaOdN4Z0beO5dvA/?wx_fmt=png)
**推荐阅读：**
1，[kylin集群Nginx负载均衡](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484336&idx=1&sn=46ba9859f0ce8cd7714ca6fdac959ae3&chksm=9f38e098a84f698edd76f4d82e32f09dd6c2c4f351ead7cf7c260c9554c230394b1d86af190e&scene=21#wechat_redirect)
2，[Hadoop伪分布式集群安装部署](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484091&idx=1&sn=00d93b91642a6aaa103de2731066410f&chksm=9f38e193a84f68858e1d906967f0b7b3221320d82fa95905b9a854ebe39fc96837820a42bc22&scene=21#wechat_redirect)
**号外：本公众号改名为Spark学习技巧了。**
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

