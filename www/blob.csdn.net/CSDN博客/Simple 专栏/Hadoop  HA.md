# Hadoop  HA - Simple 专栏 - CSDN博客
2016年05月13日 18:01:33[Simple_Zz](https://me.csdn.net/love284969214)阅读数：738
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
# Hadoop  HA 是什么？
hadoop2.x版本中可以有多个namenode结点，一个是活跃（active）状态，一个是准备（standby）状态。
## HDFS HA背景
HDFS集群中NameNode 存在单点故障（SPOF）。对于只有一个NameNode的集群，如果NameNode机器出现意外情况，将导致整个集群无法使用，直到NameNode 重新启动。
影响HDFS集群不可用主要包括以下两种情况：一是NameNode机器宕机，将导致集群不可用，重启NameNode之后才可使用；二是计划内的NameNode节点软件或硬件升级，导致集群在短时间内不可用。
为了解决上述问题，Hadoop给出了HDFS的高可用HA方案：HDFS通常由两个NameNode组成，一个处于active状态，另一个处于standby状态。Active NameNode对外提供服务，比如处理来自客户端的RPC请求，而Standby NameNode则不对外提供服务，仅同步Active NameNode的状态，以便能够在它失败时快速进行切换。
## HDFS HA架构
一个典型的HA集群，NameNode会被配置在两台独立的机器上，在任何时间上，一个NameNode处于活动状态，而另一个NameNode处于备份状态，活动状态的NameNode会响应集群中所有的客户端，备份状态的NameNode只是作为一个副本，保证在必要的时候提供一个快速的转移。
为了让Standby Node与Active Node保持同步，这两个Node都与一组称为JNS的互相独立的进程保持通信(Journal Nodes)。当Active Node上更新了namespace，它将记录修改日志发送给JNS的多数派。Standby noes将会从JNS中读取这些edits，并持续关注它们对日志的变更。Standby Node将日志变更应用在自己的namespace中，当failover发生时，Standby将会在提升自己为Active之前，确保能够从JNS中读取所有的edits，即在failover发生之前Standy持有的namespace应该与Active保持完全同步。
为了支持快速failover，Standby node持有集群中blocks的最新位置是非常必要的。为了达到这一目的，DataNodes上需要同时配置这两个Namenode的地址，同时和它们都建立心跳链接，并把block位置发送给它们。
任何时刻，只有一个Active NameNode是非常重要的，否则将会导致集群操作的混乱，那么两个NameNode将会分别有两种不同的数据状态，可能会导致数据丢失，或者状态异常，这种情况通常称为“split-brain”（脑裂，三节点通讯阻断，即集群中不同的Datanodes却看到了两个Active NameNodes）。对于JNS而言，任何时候只允许一个NameNode作为writer；在failover期间，原来的Standby Node将会接管Active的所有职能，并负责向JNS写入日志记录，这就阻止了其他NameNode基于处于Active状态的问题。
![](https://img-blog.csdn.net/20160809103017606)
基于QJM的HDFS HA方案如上图所示，其处理流程为：集群启动后一个NameNode处于Active状态，并提供服务，处理客户端和DataNode的请求，并把editlog写到本地和share editlog（这里是QJM）中。另外一个NameNode处于Standby状态，它启动的时候加载fsimage，然后周期性的从share editlog中获取editlog，保持与Active节点的状态同步。为了实现Standby在Active挂掉后迅速提供服务，需要DataNode同时向两个NameNode汇报，使得Stadnby保存block to DataNode信息，因为NameNode启动中最费时的工作是处理所有DataNode的blockreport。为了实现热备，增加FailoverController和Zookeeper，FailoverController与Zookeeper通信，通过Zookeeper选举机制，FailoverController通过RPC让NameNode转换为Active或Standby。
## HDFS HA配置要素
NameNode机器：两台配置对等的物理机器，它们分别运行Active和Standby Node。
JouralNode机器：运行JouralNodes的机器。JouralNode守护进程相当的轻量级，可以和Hadoop的其他进程部署在一起，比如NameNode、DataNode、ResourceManager等，至少需要3个且为奇数，如果你运行了N个JNS，那么它可以允许(N-1)/2个JNS进程失效并且不影响工作。
在HA集群中，Standby NameNode还会对namespace进行checkpoint操作（继承Backup Namenode的特性），因此不需要在HA集群中运行SecondaryNameNode、CheckpointNode或者BackupNode。
## HDFS自动故障转移
HDFS的自动故障转移主要由Zookeeper（Zookeeper很重要啊）和ZKFC两个组件组成。
Zookeeper集群作用主要有：
一：是故障监控。每个NameNode将会和Zookeeper建立一个持久session，如果NameNode失效，那么此session将会过期失效，此后Zookeeper将会通知另一个Namenode，然后触发Failover；
二：是NameNode选举。ZooKeeper提供了简单的机制来实现Acitve Node选举，如果当前Active失效，Standby将会获取一个特定的排他锁，那么获取锁的Node接下来将会成为Active。
ZKFC是一个Zookeeper的客户端，它主要用来监测和管理NameNodes的状态，每个NameNode机器上都会运行一个ZKFC程序，它的职责主要有：一是健康监控。ZKFC间歇性的ping NameNode，得到NameNode返回状态，如果NameNode失效或者不健康，那么ZKFS将会标记其为不健康；二是Zookeeper会话管理。当本地NaneNode运行良好时，ZKFC将会持有一个Zookeeper session，如果本地NameNode为Active，它同时也持有一个“排他锁”znode，如果session过期，那么次lock所对应的znode也将被删除；三是选举。当集群中其中一个NameNode宕机，Zookeeper会自动将另一个激活。
## YARN HA架构
![](https://img-blog.csdn.net/20160809103116029)
YARN的HA架构和HDFS HA类似，需要启动两个ResourceManager，这两个ResourceManager会向ZooKeeper集群注册，通过ZooKeeper管0理它们的状态（Active或Standby）并进行自动故障转移。
## 高可用集群规划
根据Hadoop的HA架构分析，规划整个集群由5台主机组成，具体情况如下表所示：
|主机名|IP地址|安装的软件|JPS|
|----|----|----|----|
|hadoop-master1|172.16.20.81|Jdk/hadoop|Namenode/zkfc/resourcemanager/JobHistoryServer|
|hadoop-master2|172.16.20.82|Jdk/hadoop|Namenode/zkfc/resourcemanager/WebProxyServer|
|hadoop-slave1|172.16.20.83|Jkd/hadoop/zookeepe|Datanode/journalnode/nodemanager/quorumPeerMain|
|hadoop-slave2|172.16.20.84|Jkd/hadoop/zookeeper|Datanode/journalnode/nodemanager/quorumPeerMain|
|hadoop-slave3|172.16.20.85|Jkd/hadoop/zookeeper|Datanode/journalnode/nodemanager/quorumPeerMain|
需要说明以下几点：
HDFS HA通常由两个NameNode组成，一个处于Active状态，另一个处于Standby状态。Active NameNode对外提供服务，而Standby NameNode则不对外提供服务，仅同步Active NameNode的状态，以便能够在它失败时快速进行切换。
Hadoop 2.0官方提供了两种HDFS HA的解决方案，一种是NFS，另一种是QJM。这里我们使用简单的QJM。在该方案中，主备NameNode之间通过一组JournalNode同步元数据信息，一条数据只要成功写入多数JournalNode即认为写入成功。通常配置奇数个JournalNode，这里还配置了一个Zookeeper集群，用于ZKFC故障转移，当Active NameNode挂掉了，会自动切换Standby NameNode为Active状态。
YARN的ResourceManager也存在单点故障问题，这个问题在hadoop-2.4.1得到了解决：有两个ResourceManager，一个是Active，一个是Standby，状态由zookeeper进行协调。
YARN框架下的MapReduce可以开启JobHistoryServer来记录历史任务信息，否则只能查看当前正在执行的任务信息。
Zookeeper的作用是负责HDFS中NameNode主备节点的选举，和YARN框架下ResourceManaer主备节点的选举。
