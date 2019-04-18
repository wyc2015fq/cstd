# HDFS safemode - Soul Joy Hub - CSDN博客

2016年07月14日 14:17:53[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：544


# 问题一

> 
HDFS在什么情况下会进入safemode？safemode是怎样一种工作模式？

- namenode所使用的存放editlog和fsimage的目录满了的时候（分区满，无空间）
- namenode 启动过程中（仅仅启动过程中），如果数据块汇报数量不够，就会在safemode中（不是副本数哦，为什么？副本数可以不满，因为可以启动了之后再复制满）
- 手动进入（比如维护升级）。

处于safemode的集群是无法接收任何写操作的，包括创建目录、删除文件、修改文件、上传文件等等。hdfs集群在启动和关闭的时候一般会有一段时间处于safemode，如果集群中出现了大量的block副本数量低于配置的副本数据量（这个副本数量的配置并不一定是在hdfs的配置文件中配的，配置文件中的只是默认值，在创建文件的时候，客户端可以给文件指定一个副本数量），或者有大量的节点出现故障时，集群可能会长时间处于safemode不能自拔。当系统处于安全模式时，会检查数据块的完整性。总体来说，安全模式是hadoop的一种保护机制，用于保证集群中数据块的安全性的。

# 问题二

> 
某天，一童鞋说某HDFS集群出故障了，于是有了如下对话： 

  A：故障是什么现象？ 

  B：不能上传文件 

  A： 是不是在Safemode？ 

  B看了一下HDFS的Namenode页面 [http://namenode:50070](http://namenode:50070)。 

  B: 还真是safemode，好像少了数据块，还缺xx block可退出safemode 

  A: 这个集群总共有几台datanode啊？现在活着的有几台？ 

  B：3台，你看live nodes有3台，没有dead nodes。 

  A： (心想没有道理啊)看看管理界面([http://manager-ip:8180](http://manager-ip:8180))上到底几台datanode。 

  B:  这里有8台啊

为什么50070页面只看到了live nodes 3台，deadnode 0台？这个合理吗？

若块的备份不够，Namenode将其复制到其它的datanode，而不需要知道配置了多少台datanode。

## 关于配置

```xml
<property>
  <name>dfs.namenode.safemode.min.datanodes</name>
  <value>0</value>
  <description>
    Specifies the number of datanodes that must be considered alive
    before the name node exits safemode.
    Values less than or equal to 0 mean not to take the number of live
    datanodes into account when deciding whether to remain in safe mode
    during startup.
    Values greater than the number of datanodes in the cluster
    will make safe mode permanent.
  </description>
</property>
<property>
  <name>dfs.namenode.safemode.extension</name>
  <value>30000</value>
  <description>
    Determines extension of safe mode in milliseconds
    after the threshold level is reached.
  </description>
</property>
```

dfs.namenode.safemode.min.datanodes的意思指namenode退出安全模式之前有效的（活着的）datanode的数量。这个值小等于0表示在退出安全模式之前无须考虑有效的datanode节点个数，值大于集群中datanode节点总数则表示永远处于安全模式。

