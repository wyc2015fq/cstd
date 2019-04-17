# 在线实时大数据平台Storm本地模式运行的一个小发现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月28日 17:26:09[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2645
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、现象：生产中分别部署了两台服务器，独立运行storm，然后拓扑程序提交是本地模式，发现不用启动storm和zookeeper也可以运行；

#jps  没有下面进程

 QuorumPeerMain  //zookeeper进程

 supervisor  

 nimbus





2、验证：在拓扑程序中增加参数传递，1代表本地模式，2代表集群模式

     1）关闭集群后，传参1运行本地模式成功，传参2运行集群模式失败；

      2）启动集群后，传参2集群模式提交运行，成功；

    本地模式：不用启动storm和zookeeper集群

  storm jar /tmp/glabeling.jar com.glabeling.GlabelingInfoTopology -N 1-C /tmp/glabeling.xml > /tmp/storm.logs

   集群模式：需启动storm和zookeeper集群

storm jar /tmp/glabeling.jar com.glabeling.GlabelingInfoTopology -N 2 -C /tmp/glabeling.xml > /tmp/storm.logs





3、过程中，对zookeeper作用记录下：

Storm中使用Zookeeper主要用于Storm集群各节点的分布式协调工作，具体功能如下：

（1）存储客户端提供的topology任务信息，nimbus负责将任务分配信息写入Zookeeper，supervisor从Zookeeper上读取任务分配信息；

（2）存储supervisor和worker的心跳（包括它们的状态），使得nimbus可以监控整个集群的状态， 从而重启一些挂掉的worker；

（3）存储整个集群的所有状态信息和配置信息。




