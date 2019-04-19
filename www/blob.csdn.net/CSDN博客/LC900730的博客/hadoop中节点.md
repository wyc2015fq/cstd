# hadoop中节点 - LC900730的博客 - CSDN博客
2017年11月22日 14:13:38[lc900730](https://me.csdn.net/LC900730)阅读数：1335
## namenode主要功能
- namenode提供名称查询服务
- namenode保存metadate信息。(包括oweership和permissions)。文件包含哪些块；Block保存在哪个Datanode(由DataNode启动时候上报)
- NameNode的metadate信息启动后会加载到内存。
## datanode主要功能
Hadoop 集群包含一个NameNode和大量的DataNode
- 保存block，每个块对应一个元数据信息文件。这个文件主要描述这个块属于哪个文件、第几个块等
- 启动Datanode时候会向namenode汇报block信息
- 心跳保持联系(3s一次)，如果10分钟namenode没有收到datanode心跳，认为lost，并将其上的block复制到其他datanode。
## 辅助元数据节点
- 周期将editslog文件中记录对hdfs的操作合并到一个FsImage中文件中，然后清空EditsLog文件。NameNode重启就会加载最新的一个FsImage文件 ，并创建EditsLog文件来记录HDFS操作。
