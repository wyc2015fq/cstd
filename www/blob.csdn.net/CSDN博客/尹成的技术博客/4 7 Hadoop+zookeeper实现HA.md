
# 4.7 Hadoop+zookeeper实现HA - 尹成的技术博客 - CSDN博客

2018年11月15日 14:33:34[尹成](https://me.csdn.net/yincheng01)阅读数：50


集群规划
	zk01——zk05，5台centos
	zk01——zk05部署了5个zookeeper
	zk01为namenode节点
	zk05为namenode的HA节点
	zk02、zk03、zk04为datanode节点
搭建过程
	修改主机名，hosts
	搭建5台zookeeper，[启动后5台机器都要zkServer.sh](http://xn--5zkServer-fy5oz6iikhou9xoj2eqvygvu0a.sh)status查看状态，如果有启动没成功的节点，先解决这里的问题，再继续向下进行
	配置环境变量
	在主节点配置hadoop，其他和以前步骤一样，只有core-site.xml和hdfs-site.xml有变化，如下：
core-site.xml中，fs.defaultFS的值修改为hdfs://ns    ns为自定义的名称，与hdfs.site.xml中对应
core-site.xml中，增加以下配置：
<property>    
    <name>dfs.nameservices</name>    
    <value>ns</value>    
</property>  
<!-- ns下面有两个NameNode，分别是nn1，nn2 -->
<property>
  <name>dfs.ha.namenodes.ns</name>
  <value>nn1,nn2</value>
</property>
<!-- nn1的RPC通信地址 -->
<property>
  <name>dfs.namenode.rpc-address.ns.nn1</name>
  <value>zk01:9000</value>
</property>
<!-- nn1的http通信地址 -->
<property>
    <name>dfs.namenode.http-address.ns.nn1</name>
    <value>zk01:50070</value>
</property>
<!-- nn2的RPC通信地址 -->
<property>
    <name>dfs.namenode.rpc-address.ns.nn2</name>
    <value>zk05:9000</value>
</property>
<!-- nn2的http通信地址 -->
<property>
    <name>dfs.namenode.http-address.ns.nn2</name>
    <value>zk05:50070</value>
</property>
<!-- 指定NameNode的元数据在JournalNode上的存放位置 -->
<property>
    <name>dfs.namenode.shared.edits.dir</name>qjournal://zk01:8485;zk02:8485;zk03:8485;zk04:8485;zk05:8485/ns

dfs.journalnode.edits.dir
/usr/local/hadoop/

dfs.ha.automatic-failover.enabled
true

dfs.client.failover.proxy.provider.ns
org.apache.hadoop.hdfs.server.namenode.ha.ConfiguredFailoverProxyProvider

dfs.ha.fencing.methods
shell(/bin/true)
	在zk01节点输入 hdfs zkfc -formatZK 格式化zookeeper集群，此时会在zookeeper建立HA的相应节点
	所有节点hadoop的sbin目录下，执行 ./hadoop-daemon.sh start journalnode 启动JournalNode集群
	在所有节点hadoop的sbin目录执行 ./hadoop-daemon.sh start zkfc 启动ZooKeeperFailoverCotroller
	在zk01节点输入 hdfs namenode -format -clusterId ss 格式化
	在zk01节点hadoop的sbin目录输入 ./hadoop-daemon.sh start namenode 启动namenode
	在zk05节点hadoop的bin目录输入 ./hdfs namenode -bootstrapStandby 将格式化同步到备机
	在zk05节点hadoop的sbin目录输入 ./hadoop-daemon.sh start namenode 启动备机的namenode
	在zk01节点hadoop的sbin目录输入 ./hadoop-daemons.sh start datanode 启动datanode
	在zk01节点hadoop的sbin目录输入 ./start-yarn.sh
	下图是两个启动后的namenode节点
	此时 kill -9 zk01节点namenode的进程
	查看zk05会自动切换为active，HA配置成功
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

