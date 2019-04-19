# DataNode 工作机制 - Simple 专栏 - CSDN博客
2015年04月04日 10:45:55[Simple_Zz](https://me.csdn.net/love284969214)阅读数：551标签：[HDFS																[Hadoop																[大数据](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)](https://so.csdn.net/so/search/s.do?q=Hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=HDFS&t=blog)
个人分类：[hadoop生态圈](https://blog.csdn.net/love284969214/article/category/7975457)
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
## DataNode 工作机制
![DataNode 工作机制](https://img-blog.csdn.net/20180930101023127?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)DataNode 工作机制
- 一个数据块在 DataNode 上以文件形式存储在磁盘上，包括两个块，一个是数据本身，一个是元数据包括数据块的长度，块数据的校验和，以及时间戳。
- DataNode 启动后向 NameNode 注册。
- 注册通过后，每周期性（1小时）的向 NameNode 上报所有的块信息。
- NameNode 发起心跳每3秒一次，DataNode 返回心跳带有命令，（如复制块数据到另一节点，或删除某个数据块）。
- 如果超过10分钟没有收到某个 DataNode 的心跳，则认为该节点宕机。
- 集群运行中可以安全的加入和退出一些机器。
## 掉线时限参数设置
- DataNode 进程死亡或者网络故障造成 DataNode 无法与 NameNode 通信。
- NameNode不会立即把该节点判定为死亡，要经过一段时间，这段时间暂称作超时时长。
- HDFS默认的超时时长为10分钟 + 30秒。
- 如果定义超时时间为timeout，则超时时长的计算公式为：
- timeout  = 2 * dfs.namenode.heartbeat.recheck-interval + 10 * dfs.heartbeat.interval。
- 而默认的 dfs.namenode.heartbeat.recheck-interval 大小为5分钟，dfs.heartbeat.interval 默认为3秒。
- 需要注意的是 hdfs-site.xml 配置文件中的 heartbeat.recheck.interval 的单位为毫秒，dfs.heartbeat.interval 的单位为秒。
```
hdfs-site.xml
<property>
    <name>dfs.namenode.heartbeat.recheck-interval</name>
    <value>300000</value>
</property>
<property>
    <name> dfs.heartbeat.interval </name>
    <value>3</value>
</property>
```
## 服役新节点
1、在 NameNode 的 /soft/hadoop/etc/hadoop 目录下创建 dfs.hosts 文件。
```bash
[root@hadoop1 hadoop]$ pwd
/soft/hadoop/etc/hadoop
[root@hadoop1 hadoop]$ touch dfs.hosts
[root@hadoop1 hadoop]$ vi dfs.hosts
添加如下主机名称（包含新服役的节点）
hadoop1
hadoop2
hadoop3
hadoop4
```
2、在 NameNode 的 hdfs-site.xml 配置文件中增加 dfs.hosts 属性
```
hdfs-site.xml
<property>
    <name>dfs.hosts</name>
    <value>/soft/hadoop/etc/hadoop/dfs.hosts</value>
</property>
```
3、刷新 NameNode
```bash
[root@hadoop1 hadoop]$ hdfs dfsadmin -refreshNodes
Refresh nodes successful
```
4、更新 resourcemanager 节点
```bash
[root@hadoop1 hadoop]$ yarn rmadmin -refreshNodes
```
5、在 NameNode 的 slaves 文件中增加新主机名称
```
增加 hadoop4 不需要分发
hadoop1
hadoop2
hadoop3
hadoop4
```
6、单独命令启动新的数据节点和节点管理器
```
[root@hadoop1 hadoop]$ sbin/hadoop-daemon.sh start datanode
starting datanode, logging to /soft/hadoop/logs/hadoop-root-datanode-hadoop4.out
[root@hadoop1 hadoop]$ $ sbin/yarn-daemon.sh start nodemanager
starting nodemanager, logging to  /soft/hadoop/logs/yarn-root-nodemanager-hadoop4.out
```
7、在web浏览器上检查是否 ok
8、如果数据不均衡，可以用命令实现集群的再平衡。
```bash
[root@hadoop1 sbin]$ ./start-balancer.sh
starting balancer, logging to /soft/hadoop/logs/hadoop-root-balancer-hadoop1.out
Time Stamp Iteration#  Bytes Already Moved  Bytes Left To Move  Bytes Being Moved
```
## 退役旧节点
1、在 NameNode 的 /soft/hadoop/etc/hadoop 目录下创建 dfs.hosts.exclude 文件。
```bash
[root@hadoop1 hadoop]$ pwd
/soft/hadoop/etc/hadoop
[root@hadoop1 hadoop]$ touch dfs.hosts.exclude=
[root@hadoop1 hadoop]$ vi dfs.hosts.exclude
添加如下主机名称（要退役的节点）
hadoop4
```
2、在 NameNode 的 hdfs-site.xml 配置文件中增加 dfs.hosts.exclude 属性
```
<property>
    <name>dfs.hosts.exclude</name>
    <value>/soft/hadoop/etc/hadoop/dfs.hosts.exclude</value>
</property>
```
3、刷新 NameNode、刷新 resourcemanager。
```bash
[root@hadoop1 hadoop]$ hdfs dfsadmin -refreshNodes
Refresh nodes successful
[root@hadoop1 hadoop]$ yarn rmadmin -refreshNodes
```
4、检查web浏览器，退役节点的状态为 decommission in progress（退役中），说明数据节点正在复制块到其他节点。
5、等待退役节点状态为 decommissioned（所有块已经复制完成），停止该节点及节点资源管理器。
**      注意：如果副本数是3，服役的节点小于等于3，是不能退役成功的，需要修改副本数后才能退役。**
```
[root@hadoop4 hadoop]$ sbin/hadoop-daemon.sh stop datanode
stopping datanode
[root@hadoop4 hadoop]$ sbin/yarn-daemon.sh stop nodemanager
stopping nodemanager
```
6、从 include 文件中删除退役节点，再运行刷新节点的命令。
```
1.从 NameNode 的 dfs.hosts 文件中删除退役节点 hadoop4
hadoop1
hadoop2
hadoop3
2.刷新 NameNode 刷新 resourcemanager
[root@hadoop1 hadoop]$ hdfs dfsadmin -refreshNodes
Refresh nodes successful
[root@hadoop1 hadoop]$ yarn rmadmin -refreshNodes
```
7）从 NameNode 的 slave 文件中删除退役节点 hadoop4
```
hadoop1
hadoop2
hadoop3
```
8）如果数据不均衡，可以用命令实现集群的再平衡。
```bash
[root@hadoop1 sbin]$ ./start-balancer.sh
starting balancer, logging to /soft/hadoop/logs/hadoop-root-balancer-hadoop1.out
Time Stamp Iteration#  Bytes Already Moved  Bytes Left To Move  Bytes Being Moved
```
