# 配置Hadoop2.0HDFS的HA以及联邦使用QJM - 零度的博客专栏 - CSDN博客
2016年10月09日 15:16:13[零度anngle](https://me.csdn.net/zmx729618)阅读数：1448
## HA和联邦解释 
       相比于Hadoop1.0，Hadoop 2.0中的HDFS增加了两个重大特性，HA和Federaion。HA即为High Availability，用于解决NameNode单点故障问题，该特性通过热备的方式为主NameNode提供一个备用者，一旦主NameNode出现故障，可以迅速切换至备NameNode，从而实现不间断对外提供服务。Federation即为“联邦”，该特性允许一个HDFS集群中存在多个NameNode同时对外提供服务，这些NameNode分管一部分目录（水平切分），彼此之间相互隔离，但共享底层的DataNode存储资源。
        在一个典型的HDFSHA场景中，通常由两个NameNode组成，一个处于active状态，另一个处于standby状态。Active NameNode对外提供服务，比如处理来自客户端的RPC请求，而Standby NameNode则不对外提供服务，仅同步active namenode的状态，以便能够在它失败时快速进行切换。
        为了能够实时同步Active和Standby两个NameNode的元数据信息（实际上editlog），需提供一个共享存储系统，可以是NFS、QJM（Quorum Journal Manager）或者Bookeeper，Active Namenode将数据写入共享存储系统，而Standby监听该系统，一旦发现有新数据写入，则读取这些数据，并加载到自己内存中，以保证自己内存状态与Active NameNode保持基本一致，如此这般，在紧急情况下standby便可快速切为active
 namenode。
## HA和联邦配置过程详述
    大家从官网下载的apache hadoop2.2.0的代码是32位操作系统下编译的，不能使用64位的jdk。我下面部署的hadoop代码是自己的64位机器上重新编译过的。服务器都是64位的，本配置尽量模拟真实环境。大家可以以32位的操作系统做练习，这是没关系的。
    在这里我们选用4台机器进行示范，各台机器的职责如下表格所示
||hadoop101|hadoop102|hadoop103|hadoop104|
|----|----|----|----|----|
|是NameNode吗?|是，属集群c1|是，属集群c1|是，属集群c2|是，属集群c2|
|是DataNode吗？|是|是|是|是|
|是JournalNode吗？|是|是|是|不是|
|是ZooKeeper吗？|是|是|是|不是|
|是ZKFC吗?|是|是|是|是|
    配置文件一共包括6个，分别是hadoop-env.sh、core-site.xml、hdfs-site.xml、mapred-site.xml、yarn-site.xml和slaves。除了hdfs-site.xml文件在不同集群配置不同外，其余文件在四个节点的配置是完全一样的，可以复制。
## 文件hadoop-env.sh
    就是修改这一行内容，修改后的结果如下
export JAVA_HOME=/usr/local/jdk   
【这里的JAVA_HOME的值是jdk的安装路径。如果你那里不一样，请修改为自己的地址】
## 文件core-site.xml
<configuration> 
< property> 
  <name>fs.defaultFS</name> 
  <value>hdfs://cluster1</value> 
< /property>
【这里的值指的是默认的HDFS路径。当有多个HDFS集群同时工作时，用户如果不写集群名称，那么默认使用哪个哪？在这里指定！该值来自于hdfs-site.xml中的配置】 
< property> 
  <name>hadoop.tmp.dir</name> 
  <value>/usr/local/hadoop/tmp</value> 
< /property>
【这里的路径默认是NameNode、DataNode、JournalNode等存放数据的公共目录。用户也可以自己单独指定这三类节点的目录。】 
< property> 
  <name>ha.zookeeper.quorum</name> 
  <value>hadoop101:2181,hadoop102:2181,hadoop103:2181</value> 
< /property>
【这里是ZooKeeper集群的地址和端口。注意，数量一定是奇数，且不少于三个节点】 
< /configuration>
## 集群c1的文件hdfs-site.xml
该文件只配置在hadoop101和hadoop102上。
<configuration> 
    <property> 
        <name>dfs.replication</name> 
        <value>2</value> 
    </property>
【指定DataNode存储block的副本数量。默认值是3个，我们现在有4个DataNode，该值不大于4即可。】 
    <property> 
        <name>dfs.nameservices</name> 
        <value>cluster1,cluster2</value> 
    </property>
【使用federation时，使用了2个HDFS集群。这里抽象出两个NameService实际上就是给这2个HDFS集群起了个别名。名字可以随便起，相互不重复即可】 
    <property> 
        <name>dfs.ha.namenodes.cluster1</name> 
        <value>hadoop101,hadoop102</value> 
    </property>
【指定NameService是cluster1时的namenode有哪些，这里的值也是逻辑名称，名字随便起，相互不重复即可】 
    <property> 
        <name>dfs.namenode.rpc-address.cluster1.hadoop101</name> 
        <value>hadoop101:9000</value> 
    </property>
【指定hadoop101的RPC地址】 
    <property> 
        <name>dfs.namenode.http-address.cluster1.hadoop101</name> 
        <value>hadoop101:50070</value> 
    </property>
【指定hadoop101的http地址】 
    <property> 
        <name>dfs.namenode.rpc-address.cluster1.hadoop102</name> 
        <value>hadoop102:9000</value> 
    </property>
【指定hadoop102的RPC地址】
    <property> 
        <name>dfs.namenode.http-address.cluster1.hadoop102</name> 
        <value>hadoop102:50070</value> 
    </property>
【指定hadoop102的http地址】 
    <property> 
        <name>dfs.namenode.shared.edits.dir</name> 
        <value>qjournal://hadoop101:8485;hadoop102:8485;hadoop103:8485/cluster1</value> 
    </property> 
【指定cluster1的两个NameNode共享edits文件目录时，使用的JournalNode集群信息】
         <property> 
        <name>dfs.ha.automatic-failover.enabled.cluster1</name> 
        <value>true</value> 
    </property> 
【指定cluster1是否启动自动故障恢复，即当NameNode出故障时，是否自动切换到另一台NameNode】
    <property> 
        <name>dfs.client.failover.proxy.provider.cluster1</name> 
        <value>org.apache.hadoop.hdfs.server.namenode.ha.ConfiguredFailoverProxyProvider</value> 
    </property>
【指定cluster1出故障时，哪个实现类负责执行故障切换】 
    <property> 
        <name>dfs.ha.namenodes.cluster2</name> 
        <value>hadoop103,hadoop104</value> 
    </property>
【指定NameService是cluster2时，两个NameNode是谁，这里是逻辑名称，不重复即可。以下配置与cluster1几乎全部相似，不再添加注释】 
    <property> 
        <name>dfs.namenode.rpc-address.cluster2.hadoop103</name> 
        <value>hadoop103:9000</value> 
    </property> 
    <property> 
        <name>dfs.namenode.http-address.cluster2.hadoop103</name> 
        <value>hadoop103:50070</value> 
    </property> 
    <property> 
        <name>dfs.namenode.rpc-address.cluster2.hadoop104</name> 
        <value>hadoop104:9000</value> 
    </property>
    <property> 
        <name>dfs.namenode.http-address.cluster2.hadoop104</name> 
        <value>hadoop104:50070</value> 
    </property> 
    <!-- 
    <property> 
        <name>dfs.namenode.shared.edits.dir</name> 
        <value>qjournal://hadoop101:8485;hadoop102:8485;hadoop103:8485/cluster2</value> 
    </property>
【这段代码是注释掉的，不要打开】 
    --> 
    <property> 
        <name>dfs.ha.automatic-failover.enabled.cluster2</name> 
        <value>true</value> 
    </property> 
    <property> 
        <name>dfs.client.failover.proxy.provider.cluster2</name> 
        <value>org.apache.hadoop.hdfs.server.namenode.ha.ConfiguredFailoverProxyProvider</value> 
    </property> 
    <property> 
        <name>dfs.journalnode.edits.dir</name> 
        <value>/usr/local/hadoop/tmp/journal</value> 
    </property>
【指定JournalNode集群在对NameNode的目录进行共享时，自己存储数据的磁盘路径】 
    <property> 
        <name>dfs.ha.fencing.methods</name> 
        <value>sshfence</value> 
    </property>
【一旦需要NameNode切换，使用ssh方式进行操作】 
    <property> 
        <name>dfs.ha.fencing.ssh.private-key-files</name> 
        <value>/root/.ssh/id_rsa</value> 
    </property>
【如果使用ssh进行故障切换，使用ssh通信时用的密钥存储的位置】
</configuration>
## 集群c2的文件hdfs-site.xml
该文件只配置在hadoop103和hadoop104上。
该文件与c1中的hdfs-site.xml配置内容完全相同，只有注释位置不一样，一定要注意，不要随便改
<configuration> 
    <property> 
        <name>dfs.replication</name> 
        <value>2</value> 
    </property> 
    <property> 
        <name>dfs.nameservices</name> 
        <value>cluster1,cluster2</value> 
    </property> 
    <property> 
        <name>dfs.ha.namenodes.cluster1</name> 
        <value>hadoop101,hadoop102</value> 
    </property> 
    <property> 
        <name>dfs.namenode.rpc-address.cluster1.hadoop101</name> 
        <value>hadoop101:9000</value> 
    </property> 
    <property> 
        <name>dfs.namenode.http-address.cluster1.hadoop101</name> 
        <value>hadoop101:50070</value> 
    </property> 
    <property> 
        <name>dfs.namenode.rpc-address.cluster1.hadoop102</name> 
        <value>hadoop102:9000</value> 
    </property>
    <property> 
        <name>dfs.namenode.http-address.cluster1.hadoop102</name> 
        <value>hadoop102:50070</value> 
    </property> 
    <!--    
    <property> 
        <name>dfs.namenode.shared.edits.dir</name> 
        <value>qjournal://hadoop101:8485;hadoop102:8485;hadoop103:8485/cluster1</value> 
    </property>
【这段代码是注释掉的，不要打开】 
    --> 
    <property> 
        <name>dfs.ha.automatic-failover.enabled.cluster1</name> 
        <value>true</value> 
    </property> 
    <property> 
        <name>dfs.client.failover.proxy.provider.cluster1</name> 
        <value>org.apache.hadoop.hdfs.server.namenode.ha.ConfiguredFailoverProxyProvider</value> 
    </property> 
    <property> 
        <name>dfs.ha.namenodes.cluster2</name> 
        <value>hadoop103,hadoop104</value> 
    </property> 
    <property> 
        <name>dfs.namenode.rpc-address.cluster2.hadoop103</name> 
        <value>hadoop103:9000</value> 
    </property> 
    <property> 
        <name>dfs.namenode.http-address.cluster2.hadoop103</name> 
        <value>hadoop103:50070</value> 
    </property> 
    <property> 
        <name>dfs.namenode.rpc-address.cluster2.hadoop104</name> 
        <value>hadoop104:9000</value> 
    </property>
    <property> 
        <name>dfs.namenode.http-address.cluster2.hadoop104</name> 
        <value>hadoop104:50070</value> 
    </property>
    <property> 
        <name>dfs.namenode.shared.edits.dir</name> 
        <value>qjournal://hadoop101:8485;hadoop102:8485;hadoop103:8485/cluster2</value> 
    </property>
    <property> 
        <name>dfs.ha.automatic-failover.enabled.cluster2</name> 
        <value>true</value> 
    </property> 
    <property> 
        <name>dfs.client.failover.proxy.provider.cluster2</name> 
        <value>org.apache.hadoop.hdfs.server.namenode.ha.ConfiguredFailoverProxyProvider</value> 
    </property> 
    <property> 
        <name>dfs.journalnode.edits.dir</name> 
        <value>/usr/local/hadoop/tmp/journal</value> 
    </property> 
    <property> 
        <name>dfs.ha.fencing.methods</name> 
        <value>sshfence</value> 
    </property> 
    <property> 
        <name>dfs.ha.fencing.ssh.private-key-files</name> 
        <value>/root/.ssh/id_rsa</value> 
    </property>
</configuration>
## 文件mapred-site.xml
<configuration> 
< property> 
  <name>mapreduce.framework.name</name> 
  <value>yarn</value> 
< /property>
【指定运行mapreduce的环境是yarn，与hadoop1截然不同的地方】 
< /configuration>
## 文件yarn-site.xml
<configuration> 
  <property> 
    <name>yarn.resourcemanager.hostname</name> 
    <value>hadoop101</value> 
  </property>   
【自定ResourceManager的地址，还是单点，这是隐患】
  <property> 
    <name>yarn.nodemanager.aux-services</name> 
    <value>mapreduce_shuffle</value> 
  </property> 
< /configuration>
## 文件slaves
hadoop101 
hadoop102 
hadoop103 
hadoop104
【指定所有的DataNode节点列表，每行一个节点名称】
注意：以上配置中c1中的hdfs-site.xml文件配置在hadoop101和hadoop102中，c2中的hdfs-site.xml文件配置在hadoop103和hadoop104中。其余文件在各个节点都相同。
# 启动过程
    启动时，要非常小心，请严格按照我这里描述的步骤做，每一步要检查自己的操作是否正确。
## 1.首先检查各个节点的配置文件是否正确
特别要注意hdfs-site.xml文件，在c1和c2中是不同的。
## 2.启动ZooKeeper集群
关于ZooKeeper的集群配置和启动描述见[吴超沉思录](http://www.superwu.cn/)，这里不再详述。
在hadoop101、hadoop102、hadoop103上分别执行命令：zkServer.sh start
命令输出(以hadoop101为例)：
[root@hadoop101 hadoop]# zkServer.sh status 
JMX enabled by default 
Using config: /usr/local/zookeeper/bin/../conf/zoo.cfg 
Mode: follower
三个节点都执行完启动命令后，在hadoop101执行以下验证。
验证：
[root@hadoop101 hadoop]# zkCli.sh 
Connecting to localhost:2181 
2014-02-12 07:20:35,509 [myid:] - INFO  [main:Environment@100] - Client environment:zookeeper.version=3.4.5-1392090, built on 09/30/2012 17:52 GMT 
2014-02-12 07:20:35,523 [myid:] - INFO  [main:Environment@100] - Client environment:host.name=hadoop101 
2014-02-12 07:20:35,524 [myid:] - INFO  [main:Environment@100] - Client environment:java.version=1.7.0_45 
2014-02-12 07:20:35,525 [myid:] - INFO  [main:Environment@100] - Client environment:java.vendor=Oracle Corporation 
2014-02-12 07:20:35,525 [myid:] - INFO  [main:Environment@100] - Client environment:java.home=/usr/local/jdk/jre 
2014-02-12 07:20:35,526 [myid:] - INFO  [main:Environment@100] - Client environment:java.class.path=/usr/local/zookeeper/bin/../build/classes:/usr/local/zookeeper/bin/../build/lib/*.jar:/usr/local/zookeeper/bin/../lib/slf4j-log4j12-1.6.1.jar:/usr/local/zookeeper/bin/../lib/slf4j-api-1.6.1.jar:/usr/local/zookeeper/bin/../lib/netty-3.2.2.Final.jar:/usr/local/zookeeper/bin/../lib/log4j-1.2.15.jar:/usr/local/zookeeper/bin/../lib/jline-0.9.94.jar:/usr/local/zookeeper/bin/../zookeeper-3.4.5.jar:/usr/local/zookeeper/bin/../src/java/lib/*.jar:/usr/local/zookeeper/bin/../conf:/usr/local/hadoop 
2014-02-12 07:20:35,526 [myid:] - INFO  [main:Environment@100] - Client environment:java.library.path=/usr/java/packages/lib/amd64:/usr/lib64:/lib64:/lib:/usr/lib 
2014-02-12 07:20:35,527 [myid:] - INFO  [main:Environment@100] - Client environment:java.io.tmpdir=/tmp 
2014-02-12 07:20:35,528 [myid:] - INFO  [main:Environment@100] - Client environment:java.compiler=<NA> 
2014-02-12 07:20:35,528 [myid:] - INFO  [main:Environment@100] - Client environment:os.name=Linux 
2014-02-12 07:20:35,529 [myid:] - INFO  [main:Environment@100] - Client environment:os.arch=amd64 
2014-02-12 07:20:35,529 [myid:] - INFO  [main:Environment@100] - Client environment:os.version=2.6.32-431.el6.x86_64 
2014-02-12 07:20:35,530 [myid:] - INFO  [main:Environment@100] - Client environment:user.name=root 
2014-02-12 07:20:35,530 [myid:] - INFO  [main:Environment@100] - Client environment:user.home=/root 
2014-02-12 07:20:35,531 [myid:] - INFO  [main:Environment@100] - Client environment:user.dir=/usr/local/hadoop 
2014-02-12 07:20:35,533 [myid:] - INFO  [main:ZooKeeper@438] - Initiating client connection, connectString=localhost:2181 sessionTimeout=30000 watcher=org.apache.zookeeper.ZooKeeperMain$MyWatcher@10636a7e 
Welcome to ZooKeeper! 
2014-02-12 07:20:35,569 [myid:] - INFO  [main-SendThread(127.0.0.1:2181):ClientCnxn$SendThread@966] - Opening socket connection to server 127.0.0.1/127.0.0.1:2181. Will not attempt to authenticate using SASL (unknown error) 
2014-02-12 07:20:35,587 [myid:] - INFO  [main-SendThread(127.0.0.1:2181):ClientCnxn$SendThread@849] - Socket connection established to 127.0.0.1/127.0.0.1:2181, initiating session 
JLine support is enabled 
2014-02-12 07:20:35,687 [myid:] - INFO  [main-SendThread(127.0.0.1:2181):ClientCnxn$SendThread@1207] - Session establishment complete on server 127.0.0.1/127.0.0.1:2181, sessionid = 0x654423404e530000, negotiated timeout = 30000
WATCHER::
WatchedEvent state:SyncConnected type:None path:null 
[zk: localhost:2181(CONNECTED) 0] ls / 
[zookeeper] 
[zk: localhost:2181(CONNECTED) 1]
【可以看到ZK集群 中只有一个节点zookeeper】
## 3.格式化ZooKeeper集群，目的是在ZooKeeper集群上建立HA的相应节点。
在hadoop101上执行命令：/usr/local/hadoop/bin/hdfs zkfc –formatZK
命令输出：
[root@hadoop101 hadoop]# /usr/local/hadoop/bin/hdfs zkfc -formatZK 
14/02/12 07:28:56 INFO tools.DFSZKFailoverController: Failover controller configured for NameNode NameNode at hadoop101/192.168.80.101:9000 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:zookeeper.version=3.4.5-1392090, built on 09/30/2012 17:52 GMT 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:host.name=hadoop101 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:java.version=1.7.0_45 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:java.vendor=Oracle Corporation 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:java.home=/usr/local/jdk/jre 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:java.class.path=/usr/local/hadoop/etc/hadoop:/usr/local/hadoop/share/hadoop/common/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/common/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jsch-0.1.42.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/slf4j-log4j12-1.7.5.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-digester-1.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-cli-1.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-el-1.0.jar:/usr/local/hadoop/share/hadoop/common/lib/jets3t-0.6.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/slf4j-api-1.7.5.jar:/usr/local/hadoop/share/hadoop/common/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-lang-2.5.jar:/usr/local/hadoop/share/hadoop/common/lib/jasper-runtime-5.5.23.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-xc-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-codec-1.4.jar:/usr/local/hadoop/share/hadoop/common/lib/activation-1.1.jar:/usr/local/hadoop/share/hadoop/common/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-httpclient-3.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-beanutils-1.7.0.jar:/usr/local/hadoop/share/hadoop/common/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/lib/jsp-api-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-beanutils-core-1.8.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-configuration-1.6.jar:/usr/local/hadoop/share/hadoop/common/lib/jaxb-impl-2.2.3-1.jar:/usr/local/hadoop/share/hadoop/common/lib/zookeeper-3.4.5.jar:/usr/local/hadoop/share/hadoop/common/lib/hadoop-auth-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/common/lib/stax-api-1.0.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-collections-3.2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/servlet-api-2.5.jar:/usr/local/hadoop/share/hadoop/common/lib/junit-4.8.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jetty-util-6.1.26.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/jsr305-1.3.9.jar:/usr/local/hadoop/share/hadoop/common/lib/mockito-all-1.8.5.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-logging-1.1.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-math-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jaxb-api-2.2.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-json-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-net-3.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jetty-6.1.26.jar:/usr/local/hadoop/share/hadoop/common/lib/jasper-compiler-5.5.23.jar:/usr/local/hadoop/share/hadoop/common/lib/guava-11.0.2.jar:/usr/local/hadoop/share/hadoop/common/lib/xmlenc-0.52.jar:/usr/local/hadoop/share/hadoop/common/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/common/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-jaxrs-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/jettison-1.1.jar:/usr/local/hadoop/share/hadoop/common/hadoop-common-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/common/hadoop-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/hadoop-nfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/hdfs:/usr/local/hadoop/share/hadoop/hdfs/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-cli-1.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-el-1.0.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-daemon-1.0.13.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-lang-2.5.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jasper-runtime-5.5.23.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-codec-1.4.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jsp-api-2.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/servlet-api-2.5.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jetty-util-6.1.26.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jsr305-1.3.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-logging-1.1.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jetty-6.1.26.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/guava-11.0.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/xmlenc-0.52.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-nfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/guice-3.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/hamcrest-core-1.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/yarn/lib/aopalliance-1.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/javax.inject-1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/guice-servlet-3.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/yarn/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/yarn/lib/junit-4.10.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-guice-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/yarn/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-api-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-tests-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-site-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-client-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-nodemanager-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-applications-distributedshell-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-resourcemanager-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-web-proxy-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-applications-unmanaged-am-launcher-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/guice-3.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/hamcrest-core-1.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/aopalliance-1.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/javax.inject-1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/guice-servlet-3.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/junit-4.10.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-guice-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-plugins-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-app-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-core-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-shuffle-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-common-2.2.0.jar:/usr/local/hadoop/contrib/capacity-scheduler/*.jar 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:java.library.path=/usr/local/hadoop/lib/native 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:java.io.tmpdir=/tmp 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:java.compiler=<NA> 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:os.name=Linux 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:os.arch=amd64 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:os.version=2.6.32-431.el6.x86_64 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:user.name=root 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:user.home=/root 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Client environment:user.dir=/usr/local/hadoop 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Initiating client connection, connectString=hadoop101:2181,hadoop102:2181,hadoop103:2181 sessionTimeout=5000 watcher=org.apache.hadoop.ha.ActiveStandbyElector$WatcherWithClientRef@3e9c00ea 
14/02/12 07:28:57 INFO zookeeper.ClientCnxn: Opening socket connection to server hadoop102/192.168.80.102:2181. Will not attempt to authenticate using SASL (unknown error) 
14/02/12 07:28:57 INFO zookeeper.ClientCnxn: Socket connection established to hadoop102/192.168.80.102:2181, initiating session 
14/02/12 07:28:57 INFO zookeeper.ClientCnxn: Session establishment complete on server hadoop102/192.168.80.102:2181, sessionid = 0x6644234039710000, negotiated timeout = 5000 
14/02/12 07:28:57 INFO ha.ActiveStandbyElector: Session connected. 
14/02/12 07:28:57 INFO ha.ActiveStandbyElector: Successfully created /hadoop-ha/cluster1 in ZK. 
14/02/12 07:28:57 INFO zookeeper.ZooKeeper: Session: 0x6644234039710000 closed 
14/02/12 07:28:57 INFO zookeeper.ClientCnxn: EventThread shut down 
[root@hadoop101 hadoop]#
验证：
[root@hadoop101 hadoop]# zkCli.sh 
Connecting to localhost:2181 
2014-02-12 07:30:24,355 [myid:] - INFO  [main:Environment@100] - Client environment:zookeeper.version=3.4.5-1392090, built on 09/30/2012 17:52 GMT 
2014-02-12 07:30:24,373 [myid:] - INFO  [main:Environment@100] - Client environment:host.name=hadoop101 
2014-02-12 07:30:24,374 [myid:] - INFO  [main:Environment@100] - Client environment:java.version=1.7.0_45 
2014-02-12 07:30:24,375 [myid:] - INFO  [main:Environment@100] - Client environment:java.vendor=Oracle Corporation 
2014-02-12 07:30:24,376 [myid:] - INFO  [main:Environment@100] - Client environment:java.home=/usr/local/jdk/jre 
2014-02-12 07:30:24,376 [myid:] - INFO  [main:Environment@100] - Client environment:java.class.path=/usr/local/zookeeper/bin/../build/classes:/usr/local/zookeeper/bin/../build/lib/*.jar:/usr/local/zookeeper/bin/../lib/slf4j-log4j12-1.6.1.jar:/usr/local/zookeeper/bin/../lib/slf4j-api-1.6.1.jar:/usr/local/zookeeper/bin/../lib/netty-3.2.2.Final.jar:/usr/local/zookeeper/bin/../lib/log4j-1.2.15.jar:/usr/local/zookeeper/bin/../lib/jline-0.9.94.jar:/usr/local/zookeeper/bin/../zookeeper-3.4.5.jar:/usr/local/zookeeper/bin/../src/java/lib/*.jar:/usr/local/zookeeper/bin/../conf:/usr/local/hadoop 
2014-02-12 07:30:24,378 [myid:] - INFO  [main:Environment@100] - Client environment:java.library.path=/usr/java/packages/lib/amd64:/usr/lib64:/lib64:/lib:/usr/lib 
2014-02-12 07:30:24,379 [myid:] - INFO  [main:Environment@100] - Client environment:java.io.tmpdir=/tmp 
2014-02-12 07:30:24,380 [myid:] - INFO  [main:Environment@100] - Client environment:java.compiler=<NA> 
2014-02-12 07:30:24,381 [myid:] - INFO  [main:Environment@100] - Client environment:os.name=Linux 
2014-02-12 07:30:24,382 [myid:] - INFO  [main:Environment@100] - Client environment:os.arch=amd64 
2014-02-12 07:30:24,382 [myid:] - INFO  [main:Environment@100] - Client environment:os.version=2.6.32-431.el6.x86_64 
2014-02-12 07:30:24,383 [myid:] - INFO  [main:Environment@100] - Client environment:user.name=root 
2014-02-12 07:30:24,383 [myid:] - INFO  [main:Environment@100] - Client environment:user.home=/root 
2014-02-12 07:30:24,384 [myid:] - INFO  [main:Environment@100] - Client environment:user.dir=/usr/local/hadoop 
2014-02-12 07:30:24,387 [myid:] - INFO  [main:ZooKeeper@438] - Initiating client connection, connectString=localhost:2181 sessionTimeout=30000 watcher=org.apache.zookeeper.ZooKeeperMain$MyWatcher@10636a7e 
Welcome to ZooKeeper! 
2014-02-12 07:30:24,422 [myid:] - INFO  [main-SendThread(127.0.0.1:2181):ClientCnxn$SendThread@966] - Opening socket connection to server 127.0.0.1/127.0.0.1:2181. Will not attempt to authenticate using SASL (unknown error) 
2014-02-12 07:30:24,462 [myid:] - INFO  [main-SendThread(127.0.0.1:2181):ClientCnxn$SendThread@849] - Socket connection established to 127.0.0.1/127.0.0.1:2181, initiating session 
JLine support is enabled 
2014-02-12 07:30:24,494 [myid:] - INFO  [main-SendThread(127.0.0.1:2181):ClientCnxn$SendThread@1207] - Session establishment complete on server 127.0.0.1/127.0.0.1:2181, sessionid = 0x654423404e530001, negotiated timeout = 30000
WATCHER::
WatchedEvent state:SyncConnected type:None path:null 
[zk: localhost:2181(CONNECTED) 0] ls / 
[hadoop-ha, zookeeper] 
[zk: localhost:2181(CONNECTED) 1] ls /hadoop-ha 
[cluster1] 
[zk: localhost:2181(CONNECTED) 2]
【格式化操作的目的是在ZK集群中建立一个节点，用于保存集群c1中NameNode的状态数据】
在hadoop103上执行命令：/usr/local/hadoop/bin/hdfs zkfc –formatZK
命令输出：
[root@hadoop103 hadoop]# /usr/local/hadoop/bin/hdfs zkfc -formatZK 
14/02/12 07:32:14 WARN util.NativeCodeLoader: Unable to load native-hadoop library for your platform... using builtin-java classes where applicable 
14/02/12 07:32:14 INFO tools.DFSZKFailoverController: Failover controller configured for NameNode NameNode at hadoop103/192.168.80.103:9000 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:zookeeper.version=3.4.5-1392090, built on 09/30/2012 17:52 GMT 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:host.name=hadoop103 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:java.version=1.7.0_45 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:java.vendor=Oracle Corporation 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:java.home=/usr/local/jdk/jre 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:java.class.path=/usr/local/hadoop/etc/hadoop:/usr/local/hadoop/share/hadoop/common/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/common/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jsch-0.1.42.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/slf4j-log4j12-1.7.5.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-digester-1.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-cli-1.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-el-1.0.jar:/usr/local/hadoop/share/hadoop/common/lib/jets3t-0.6.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/slf4j-api-1.7.5.jar:/usr/local/hadoop/share/hadoop/common/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-lang-2.5.jar:/usr/local/hadoop/share/hadoop/common/lib/jasper-runtime-5.5.23.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-xc-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-codec-1.4.jar:/usr/local/hadoop/share/hadoop/common/lib/activation-1.1.jar:/usr/local/hadoop/share/hadoop/common/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-httpclient-3.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-beanutils-1.7.0.jar:/usr/local/hadoop/share/hadoop/common/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/lib/jsp-api-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-beanutils-core-1.8.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-configuration-1.6.jar:/usr/local/hadoop/share/hadoop/common/lib/jaxb-impl-2.2.3-1.jar:/usr/local/hadoop/share/hadoop/common/lib/zookeeper-3.4.5.jar:/usr/local/hadoop/share/hadoop/common/lib/hadoop-auth-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/common/lib/stax-api-1.0.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-collections-3.2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/servlet-api-2.5.jar:/usr/local/hadoop/share/hadoop/common/lib/junit-4.8.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jetty-util-6.1.26.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/jsr305-1.3.9.jar:/usr/local/hadoop/share/hadoop/common/lib/mockito-all-1.8.5.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-logging-1.1.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-math-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jaxb-api-2.2.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-json-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-net-3.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jetty-6.1.26.jar:/usr/local/hadoop/share/hadoop/common/lib/jasper-compiler-5.5.23.jar:/usr/local/hadoop/share/hadoop/common/lib/guava-11.0.2.jar:/usr/local/hadoop/share/hadoop/common/lib/xmlenc-0.52.jar:/usr/local/hadoop/share/hadoop/common/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/common/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-jaxrs-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/jettison-1.1.jar:/usr/local/hadoop/share/hadoop/common/hadoop-common-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/common/hadoop-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/hadoop-nfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/hdfs:/usr/local/hadoop/share/hadoop/hdfs/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-cli-1.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-el-1.0.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-daemon-1.0.13.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-lang-2.5.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jasper-runtime-5.5.23.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-codec-1.4.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jsp-api-2.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/servlet-api-2.5.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jetty-util-6.1.26.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jsr305-1.3.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-logging-1.1.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jetty-6.1.26.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/guava-11.0.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/xmlenc-0.52.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-nfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/guice-3.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/hamcrest-core-1.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/yarn/lib/aopalliance-1.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/javax.inject-1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/guice-servlet-3.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/yarn/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/yarn/lib/junit-4.10.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-guice-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/yarn/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-api-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-tests-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-site-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-client-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-nodemanager-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-applications-distributedshell-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-resourcemanager-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-web-proxy-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-applications-unmanaged-am-launcher-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/guice-3.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/hamcrest-core-1.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/aopalliance-1.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/javax.inject-1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/guice-servlet-3.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/junit-4.10.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-guice-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-plugins-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-app-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-core-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-shuffle-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-common-2.2.0.jar:/usr/local/hadoop/contrib/capacity-scheduler/*.jar 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:java.library.path=/usr/java/packages/lib/amd64:/usr/lib64:/lib64:/lib:/usr/lib 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:java.io.tmpdir=/tmp 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:java.compiler=<NA> 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:os.name=Linux 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:os.arch=amd64 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:os.version=2.6.32-431.el6.x86_64 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:user.name=root 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:user.home=/root 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Client environment:user.dir=/usr/local/hadoop 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Initiating client connection, connectString=hadoop101:2181,hadoop102:2181,hadoop103:2181 sessionTimeout=5000 watcher=org.apache.hadoop.ha.ActiveStandbyElector$WatcherWithClientRef@91d3b18 
14/02/12 07:32:14 INFO zookeeper.ClientCnxn: Opening socket connection to server hadoop102/192.168.80.102:2181. Will not attempt to authenticate using SASL (unknown error) 
14/02/12 07:32:14 INFO zookeeper.ClientCnxn: Socket connection established to hadoop102/192.168.80.102:2181, initiating session 
14/02/12 07:32:14 INFO zookeeper.ClientCnxn: Session establishment complete on server hadoop102/192.168.80.102:2181, sessionid = 0x6644234039710001, negotiated timeout = 5000 
14/02/12 07:32:14 INFO ha.ActiveStandbyElector: Session connected. 
14/02/12 07:32:14 INFO ha.ActiveStandbyElector: Successfully created /hadoop-ha/cluster2 in ZK. 
14/02/12 07:32:14 INFO zookeeper.ZooKeeper: Session: 0x6644234039710001 closed 
14/02/12 07:32:14 INFO zookeeper.ClientCnxn: EventThread shut down
验证：
[root@hadoop103 hadoop]# zkCli.sh 
Connecting to localhost:2181 
2014-02-12 07:32:21,770 [myid:] - INFO  [main:Environment@100] - Client environment:zookeeper.version=3.4.5-1392090, built on 09/30/2012 17:52 GMT 
2014-02-12 07:32:21,786 [myid:] - INFO  [main:Environment@100] - Client environment:host.name=hadoop103 
2014-02-12 07:32:21,788 [myid:] - INFO  [main:Environment@100] - Client environment:java.version=1.7.0_45 
2014-02-12 07:32:21,789 [myid:] - INFO  [main:Environment@100] - Client environment:java.vendor=Oracle Corporation 
2014-02-12 07:32:21,789 [myid:] - INFO  [main:Environment@100] - Client environment:java.home=/usr/local/jdk/jre 
2014-02-12 07:32:21,790 [myid:] - INFO  [main:Environment@100] - Client environment:java.class.path=/usr/local/zookeeper/bin/../build/classes:/usr/local/zookeeper/bin/../build/lib/*.jar:/usr/local/zookeeper/bin/../lib/slf4j-log4j12-1.6.1.jar:/usr/local/zookeeper/bin/../lib/slf4j-api-1.6.1.jar:/usr/local/zookeeper/bin/../lib/netty-3.2.2.Final.jar:/usr/local/zookeeper/bin/../lib/log4j-1.2.15.jar:/usr/local/zookeeper/bin/../lib/jline-0.9.94.jar:/usr/local/zookeeper/bin/../zookeeper-3.4.5.jar:/usr/local/zookeeper/bin/../src/java/lib/*.jar:/usr/local/zookeeper/bin/../conf:/usr/local/hadoop 
2014-02-12 07:32:21,791 [myid:] - INFO  [main:Environment@100] - Client environment:java.library.path=/usr/java/packages/lib/amd64:/usr/lib64:/lib64:/lib:/usr/lib 
2014-02-12 07:32:21,792 [myid:] - INFO  [main:Environment@100] - Client environment:java.io.tmpdir=/tmp 
2014-02-12 07:32:21,793 [myid:] - INFO  [main:Environment@100] - Client environment:java.compiler=<NA> 
2014-02-12 07:32:21,793 [myid:] - INFO  [main:Environment@100] - Client environment:os.name=Linux 
2014-02-12 07:32:21,794 [myid:] - INFO  [main:Environment@100] - Client environment:os.arch=amd64 
2014-02-12 07:32:21,794 [myid:] - INFO  [main:Environment@100] - Client environment:os.version=2.6.32-431.el6.x86_64 
2014-02-12 07:32:21,795 [myid:] - INFO  [main:Environment@100] - Client environment:user.name=root 
2014-02-12 07:32:21,796 [myid:] - INFO  [main:Environment@100] - Client environment:user.home=/root 
2014-02-12 07:32:21,796 [myid:] - INFO  [main:Environment@100] - Client environment:user.dir=/usr/local/hadoop 
2014-02-12 07:32:21,801 [myid:] - INFO  [main:ZooKeeper@438] - Initiating client connection, connectString=localhost:2181 sessionTimeout=30000 watcher=org.apache.zookeeper.ZooKeeperMain$MyWatcher@10636a7e 
Welcome to ZooKeeper! 
2014-02-12 07:32:21,850 [myid:] - INFO  [main-SendThread(127.0.0.1:2181):ClientCnxn$SendThread@966] - Opening socket connection to server 127.0.0.1/127.0.0.1:2181. Will not attempt to authenticate using SASL (unknown error) 
JLine support is enabled 
2014-02-12 07:32:21,868 [myid:] - INFO  [main-SendThread(127.0.0.1:2181):ClientCnxn$SendThread@849] - Socket connection established to 127.0.0.1/127.0.0.1:2181, initiating session 
2014-02-12 07:32:21,906 [myid:] - INFO  [main-SendThread(127.0.0.1:2181):ClientCnxn$SendThread@1207] - Session establishment complete on server 127.0.0.1/127.0.0.1:2181, sessionid = 0x6744234039810002, negotiated timeout = 30000
WATCHER::
WatchedEvent state:SyncConnected type:None path:null 
[zk: localhost:2181(CONNECTED) 0] ls / 
[hadoop-ha, zookeeper] 
[zk: localhost:2181(CONNECTED) 1] ls /hadoop-ha 
[cluster2, cluster1] 
[zk: localhost:2181(CONNECTED) 2]
【集群c2也格式化，产生一个新的ZK节点cluster2】
## 4.启动JournalNode集群
在hadoop101、hadoop102、hadoop103上分别执行命令：/usr/local/hadoop/sbin/hadoop-daemon.sh start journalnode
命令输出(以hadoop101为例)：
[root@hadoop101 hadoop]# /usr/local/hadoop/sbin/hadoop-daemon.sh start journalnode 
starting journalnode, logging to /usr/local/hadoop/logs/hadoop-root-journalnode-hadoop101.out 
[root@hadoop101 hadoop]#
在每个节点执行完启动命令后，每个节点都执行以下验证。
验证(以hadoop101为例)：
[root@hadoop101 hadoop]# jps 
23396 JournalNode 
23598 Jps 
22491 QuorumPeerMain 
[root@hadoop101 hadoop]#
【产生一个java进程JournalNode】
查看一下目录结构
[root@hadoop101 hadoop]# jps 
23396 JournalNode 
22491 QuorumPeerMain 
23445 Jps
[root@hadoop101 hadoop]# pwd 
/usr/local/hadoop 
[root@hadoop101 hadoop]# ls tmp/ 
journal 
[root@hadoop101 hadoop]#
【启动JournalNode后，会在本地磁盘产生一个目录，用户保存NameNode的edits文件的数据】
## 5.格式化集群c1的一个NameNode
从hadoop101和hadoop102中任选一个即可，这里选择的是hadoop101
在hadoop101执行以下命令：/usr/local/hadoop/bin/hdfs namenode -format -clusterId c1
命令输出：
[root@hadoop101 hadoop]# /usr/local/hadoop/bin/hdfs namenode -format -clusterId c1 
14/02/12 08:07:59 INFO namenode.NameNode: STARTUP_MSG: 
/************************************************************ 
STARTUP_MSG: Starting NameNode 
STARTUP_MSG:   host = hadoop101/192.168.80.101 
STARTUP_MSG:   args = [-format, -clusterId, c1] 
STARTUP_MSG:   version = 2.2.0 
STARTUP_MSG:   classpath = /usr/local/hadoop/etc/hadoop:/usr/local/hadoop/share/hadoop/common/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/common/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jsch-0.1.42.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/slf4j-log4j12-1.7.5.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-digester-1.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-cli-1.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-el-1.0.jar:/usr/local/hadoop/share/hadoop/common/lib/jets3t-0.6.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/slf4j-api-1.7.5.jar:/usr/local/hadoop/share/hadoop/common/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-lang-2.5.jar:/usr/local/hadoop/share/hadoop/common/lib/jasper-runtime-5.5.23.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-xc-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-codec-1.4.jar:/usr/local/hadoop/share/hadoop/common/lib/activation-1.1.jar:/usr/local/hadoop/share/hadoop/common/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-httpclient-3.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-beanutils-1.7.0.jar:/usr/local/hadoop/share/hadoop/common/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/lib/jsp-api-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-beanutils-core-1.8.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-configuration-1.6.jar:/usr/local/hadoop/share/hadoop/common/lib/jaxb-impl-2.2.3-1.jar:/usr/local/hadoop/share/hadoop/common/lib/zookeeper-3.4.5.jar:/usr/local/hadoop/share/hadoop/common/lib/hadoop-auth-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/common/lib/stax-api-1.0.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-collections-3.2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/servlet-api-2.5.jar:/usr/local/hadoop/share/hadoop/common/lib/junit-4.8.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jetty-util-6.1.26.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/jsr305-1.3.9.jar:/usr/local/hadoop/share/hadoop/common/lib/mockito-all-1.8.5.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-logging-1.1.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-math-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jaxb-api-2.2.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-json-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-net-3.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jetty-6.1.26.jar:/usr/local/hadoop/share/hadoop/common/lib/jasper-compiler-5.5.23.jar:/usr/local/hadoop/share/hadoop/common/lib/guava-11.0.2.jar:/usr/local/hadoop/share/hadoop/common/lib/xmlenc-0.52.jar:/usr/local/hadoop/share/hadoop/common/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/common/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-jaxrs-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/jettison-1.1.jar:/usr/local/hadoop/share/hadoop/common/hadoop-common-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/common/hadoop-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/hadoop-nfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/hdfs:/usr/local/hadoop/share/hadoop/hdfs/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-cli-1.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-el-1.0.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-daemon-1.0.13.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-lang-2.5.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jasper-runtime-5.5.23.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-codec-1.4.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jsp-api-2.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/servlet-api-2.5.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jetty-util-6.1.26.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jsr305-1.3.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-logging-1.1.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jetty-6.1.26.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/guava-11.0.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/xmlenc-0.52.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-nfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/guice-3.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/hamcrest-core-1.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/yarn/lib/aopalliance-1.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/javax.inject-1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/guice-servlet-3.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/yarn/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/yarn/lib/junit-4.10.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-guice-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/yarn/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-api-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-tests-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-site-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-client-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-nodemanager-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-applications-distributedshell-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-resourcemanager-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-web-proxy-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-applications-unmanaged-am-launcher-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/guice-3.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/hamcrest-core-1.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/aopalliance-1.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/javax.inject-1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/guice-servlet-3.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/junit-4.10.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-guice-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-plugins-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-app-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-core-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-shuffle-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-common-2.2.0.jar:/contrib/capacity-scheduler/*.jar 
STARTUP_MSG:   build = Unknown -r Unknown; compiled by 'root' on 2013-12-26T08:50Z 
STARTUP_MSG:   java = 1.7.0_45 
************************************************************/ 
14/02/12 08:07:59 INFO namenode.NameNode: registered UNIX signal handlers for [TERM, HUP, INT] 
Formatting using clusterid: c1 
14/02/12 08:08:01 INFO namenode.HostFileManager: read includes: 
HostSet( 
) 
14/02/12 08:08:01 INFO namenode.HostFileManager: read excludes: 
HostSet( 
) 
14/02/12 08:08:01 INFO blockmanagement.DatanodeManager: dfs.block.invalidate.limit=1000 
14/02/12 08:08:01 INFO util.GSet: Computing capacity for map BlocksMap 
14/02/12 08:08:01 INFO util.GSet: VM type       = 64-bit 
14/02/12 08:08:01 INFO util.GSet: 2.0% max memory = 966.7 MB 
14/02/12 08:08:01 INFO util.GSet: capacity      = 2^21 = 2097152 entries 
14/02/12 08:08:01 INFO blockmanagement.BlockManager: dfs.block.access.token.enable=false 
14/02/12 08:08:01 INFO blockmanagement.BlockManager: defaultReplication         = 2 
14/02/12 08:08:01 INFO blockmanagement.BlockManager: maxReplication             = 512 
14/02/12 08:08:01 INFO blockmanagement.BlockManager: minReplication             = 1 
14/02/12 08:08:01 INFO blockmanagement.BlockManager: maxReplicationStreams      = 2 
14/02/12 08:08:01 INFO blockmanagement.BlockManager: shouldCheckForEnoughRacks  = false 
14/02/12 08:08:01 INFO blockmanagement.BlockManager: replicationRecheckInterval = 3000 
14/02/12 08:08:01 INFO blockmanagement.BlockManager: encryptDataTransfer        = false 
14/02/12 08:08:01 INFO namenode.FSNamesystem: fsOwner             = root (auth:SIMPLE) 
14/02/12 08:08:01 INFO namenode.FSNamesystem: supergroup          = supergroup 
14/02/12 08:08:01 INFO namenode.FSNamesystem: isPermissionEnabled = true 
14/02/12 08:08:01 INFO namenode.FSNamesystem: Determined nameservice ID: cluster1 
14/02/12 08:08:01 INFO namenode.FSNamesystem: HA Enabled: true 
14/02/12 08:08:01 INFO namenode.FSNamesystem: Append Enabled: true 
14/02/12 08:08:01 INFO util.GSet: Computing capacity for map INodeMap 
14/02/12 08:08:01 INFO util.GSet: VM type       = 64-bit 
14/02/12 08:08:01 INFO util.GSet: 1.0% max memory = 966.7 MB 
14/02/12 08:08:01 INFO util.GSet: capacity      = 2^20 = 1048576 entries 
14/02/12 08:08:01 INFO namenode.NameNode: Caching file names occuring more than 10 times 
14/02/12 08:08:01 INFO namenode.FSNamesystem: dfs.namenode.safemode.threshold-pct = 0.9990000128746033 
14/02/12 08:08:01 INFO namenode.FSNamesystem: dfs.namenode.safemode.min.datanodes = 0 
14/02/12 08:08:01 INFO namenode.FSNamesystem: dfs.namenode.safemode.extension     = 30000 
14/02/12 08:08:01 INFO namenode.FSNamesystem: Retry cache on namenode is enabled 
14/02/12 08:08:01 INFO namenode.FSNamesystem: Retry cache will use 0.03 of total heap and retry cache entry expiry time is 600000 millis 
14/02/12 08:08:01 INFO util.GSet: Computing capacity for map Namenode Retry Cache 
14/02/12 08:08:01 INFO util.GSet: VM type       = 64-bit 
14/02/12 08:08:01 INFO util.GSet: 0.029999999329447746% max memory = 966.7 MB 
14/02/12 08:08:01 INFO util.GSet: capacity      = 2^15 = 32768 entries 
14/02/12 08:08:03 INFO common.Storage: Storage directory /usr/local/hadoop/tmp/dfs/name has been successfully formatted. 
14/02/12 08:08:04 INFO namenode.FSImage: Saving image file /usr/local/hadoop/tmp/dfs/name/current/fsimage.ckpt_0000000000000000000 using no compression 
14/02/12 08:08:04 INFO namenode.FSImage: Image file /usr/local/hadoop/tmp/dfs/name/current/fsimage.ckpt_0000000000000000000 of size 196 bytes saved in 0 seconds. 
14/02/12 08:08:04 INFO namenode.NNStorageRetentionManager: Going to retain 1 images with txid >= 0 
14/02/12 08:08:04 INFO util.ExitUtil: Exiting with status 0 
14/02/12 08:08:04 INFO namenode.NameNode: SHUTDOWN_MSG: 
/************************************************************ 
SHUTDOWN_MSG: Shutting down NameNode at hadoop101/192.168.80.101 
************************************************************/ 
[root@hadoop101 hadoop]#
验证：
[root@hadoop101 hadoop]# ls tmp/ 
dfs  journal 
[root@hadoop101 hadoop]# ls tmp/dfs/ 
name
【格式化NameNode会在磁盘产生一个目录，用于保存NameNode的fsimage、edits等文件】
## 6.启动c1中刚才格式化的NameNode
在hadoop101上执行命令：/usr/local/hadoop/sbin/hadoop-daemon.sh start namenode
命令输出：
[root@hadoop101 hadoop]# /usr/local/hadoop/sbin/hadoop-daemon.sh start namenode 
starting namenode, logging to /usr/local/hadoop/logs/hadoop-root-namenode-hadoop101.out
验证：
[root@hadoop101 hadoop]# jps 
23396 JournalNode 
23598 Jps 
23558 NameNode 
22491 QuorumPeerMain 
[root@hadoop101 hadoop]#
【启动后，产生一个新的java进程NameNode】
通过浏览器访问，也可以看到下图所示
![image](http://images.cnitblog.com/blog/346286/201402/121441441135426.png)
## 7.把NameNode的数据从hadoop101同步到hadoop102中
在hadoop102上执行命令：/usr/local/hadoop/bin/hdfs namenode –bootstrapStandby
命令输出：
[root@hadoop102 hadoop]# /usr/local/hadoop/bin/hdfs namenode -bootstrapStandby 
14/02/12 08:17:44 INFO namenode.NameNode: STARTUP_MSG: 
/************************************************************ 
STARTUP_MSG: Starting NameNode 
STARTUP_MSG:   host = hadoop102/192.168.80.102 
STARTUP_MSG:   args = [-bootstrapStandby] 
STARTUP_MSG:   version = 2.2.0 
STARTUP_MSG:   classpath = /usr/local/hadoop/etc/hadoop:/usr/local/hadoop/share/hadoop/common/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/common/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jsch-0.1.42.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/slf4j-log4j12-1.7.5.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-digester-1.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-cli-1.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-el-1.0.jar:/usr/local/hadoop/share/hadoop/common/lib/jets3t-0.6.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/slf4j-api-1.7.5.jar:/usr/local/hadoop/share/hadoop/common/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-lang-2.5.jar:/usr/local/hadoop/share/hadoop/common/lib/jasper-runtime-5.5.23.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-xc-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-codec-1.4.jar:/usr/local/hadoop/share/hadoop/common/lib/activation-1.1.jar:/usr/local/hadoop/share/hadoop/common/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-httpclient-3.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-beanutils-1.7.0.jar:/usr/local/hadoop/share/hadoop/common/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/lib/jsp-api-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-beanutils-core-1.8.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-configuration-1.6.jar:/usr/local/hadoop/share/hadoop/common/lib/jaxb-impl-2.2.3-1.jar:/usr/local/hadoop/share/hadoop/common/lib/zookeeper-3.4.5.jar:/usr/local/hadoop/share/hadoop/common/lib/hadoop-auth-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/common/lib/stax-api-1.0.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-collections-3.2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/servlet-api-2.5.jar:/usr/local/hadoop/share/hadoop/common/lib/junit-4.8.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jetty-util-6.1.26.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/jsr305-1.3.9.jar:/usr/local/hadoop/share/hadoop/common/lib/mockito-all-1.8.5.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-logging-1.1.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-math-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jaxb-api-2.2.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-json-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-net-3.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jetty-6.1.26.jar:/usr/local/hadoop/share/hadoop/common/lib/jasper-compiler-5.5.23.jar:/usr/local/hadoop/share/hadoop/common/lib/guava-11.0.2.jar:/usr/local/hadoop/share/hadoop/common/lib/xmlenc-0.52.jar:/usr/local/hadoop/share/hadoop/common/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/common/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-jaxrs-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/jettison-1.1.jar:/usr/local/hadoop/share/hadoop/common/hadoop-common-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/common/hadoop-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/hadoop-nfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/hdfs:/usr/local/hadoop/share/hadoop/hdfs/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-cli-1.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-el-1.0.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-daemon-1.0.13.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-lang-2.5.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jasper-runtime-5.5.23.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-codec-1.4.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jsp-api-2.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/servlet-api-2.5.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jetty-util-6.1.26.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jsr305-1.3.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-logging-1.1.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jetty-6.1.26.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/guava-11.0.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/xmlenc-0.52.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-nfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/guice-3.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/hamcrest-core-1.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/yarn/lib/aopalliance-1.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/javax.inject-1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/guice-servlet-3.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/yarn/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/yarn/lib/junit-4.10.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-guice-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/yarn/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-api-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-tests-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-site-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-client-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-nodemanager-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-applications-distributedshell-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-resourcemanager-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-web-proxy-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-applications-unmanaged-am-launcher-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/guice-3.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/hamcrest-core-1.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/aopalliance-1.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/javax.inject-1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/guice-servlet-3.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/junit-4.10.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-guice-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-plugins-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-app-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-core-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-shuffle-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-common-2.2.0.jar:/usr/local/hadoop/contrib/capacity-scheduler/*.jar 
STARTUP_MSG:   build = Unknown -r Unknown; compiled by 'root' on 2013-12-26T08:50Z 
STARTUP_MSG:   java = 1.7.0_45 
************************************************************/ 
14/02/12 08:17:44 INFO namenode.NameNode: registered UNIX signal handlers for [TERM, HUP, INT] 
===================================================== 
About to bootstrap Standby ID hadoop102 from: 
           Nameservice ID: cluster1 
        Other Namenode ID: hadoop101 
  Other NN's HTTP address: hadoop101:50070 
  Other NN's IPC  address: hadoop101/192.168.80.101:9000 
             Namespace ID: 1496717450 
            Block pool ID: BP-2022554027-192.168.80.101-1392164061887 
               Cluster ID: c1 
           Layout version: -47 
===================================================== 
14/02/12 08:17:48 INFO common.Storage: Storage directory /usr/local/hadoop/tmp/dfs/name has been successfully formatted. 
14/02/12 08:17:48 INFO namenode.TransferFsImage: Opening connection to [http://hadoop101:50070/getimage?getimage=1&txid=0&storageInfo=-47:1496717450:0:c1](http://hadoop101:50070/getimage?getimage=1&txid=0&storageInfo=-47:1496717450:0:c1)
14/02/12 08:17:48 INFO namenode.TransferFsImage: Transfer took 0.18s at 0.00 KB/s 
14/02/12 08:17:48 INFO namenode.TransferFsImage: Downloaded file fsimage.ckpt_0000000000000000000 size 196 bytes. 
14/02/12 08:17:48 INFO util.ExitUtil: Exiting with status 0 
14/02/12 08:17:48 INFO namenode.NameNode: SHUTDOWN_MSG: 
/************************************************************ 
SHUTDOWN_MSG: Shutting down NameNode at hadoop102/192.168.80.102 
************************************************************/ 
[root@hadoop102 hadoop]#
验证：
[root@hadoop102 hadoop]# ls tmp/ 
dfs  journal 
[root@hadoop102 hadoop]# ls tmp/dfs/ 
name 
[root@hadoop102 hadoop]#
【在tmp目录下产生一个目录name】
## 8.启动c1中另一个Namenode
在hadoop102上执行命令：/usr/local/hadoop/sbin/hadoop-daemon.sh start namenode
命令输出：
[root@hadoop102 hadoop]# /usr/local/hadoop/sbin/hadoop-daemon.sh start namenode 
starting namenode, logging to /usr/local/hadoop/logs/hadoop-root-namenode-hadoop102.out
验证：
[root@hadoop102 hadoop]# jps 
12355 JournalNode 
12611 Jps 
12573 NameNode 
12081 QuorumPeerMain 
[root@hadoop102 hadoop]#
【产生java进程NameNode】
通过浏览器访问，也可以看到下图所示
![image](http://images.cnitblog.com/blog/346286/201402/121441521662588.png)
## 9.格式化集群c2的一个NameNode
从hadoop103和hadoop104中任选一个即可，这里选择的是hadoop103
在hadoop103执行以下命令：/usr/local/hadoop/bin/hdfs namenode -format -clusterId c2
命令输出：
[root@hadoop103 hadoop]# /usr/local/hadoop/bin/hdfs namenode -format -clusterId c2 
14/02/12 08:23:28 INFO namenode.NameNode: STARTUP_MSG: 
/************************************************************ 
STARTUP_MSG: Starting NameNode 
STARTUP_MSG:   host = hadoop103/192.168.80.103 
STARTUP_MSG:   args = [-format, -clusterId, c2] 
STARTUP_MSG:   version = 2.2.0 
STARTUP_MSG:   classpath = /usr/local/hadoop/etc/hadoop:/usr/local/hadoop/share/hadoop/common/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/common/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jsch-0.1.42.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/slf4j-log4j12-1.7.5.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-digester-1.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-cli-1.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-el-1.0.jar:/usr/local/hadoop/share/hadoop/common/lib/jets3t-0.6.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/slf4j-api-1.7.5.jar:/usr/local/hadoop/share/hadoop/common/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-lang-2.5.jar:/usr/local/hadoop/share/hadoop/common/lib/jasper-runtime-5.5.23.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-xc-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-codec-1.4.jar:/usr/local/hadoop/share/hadoop/common/lib/activation-1.1.jar:/usr/local/hadoop/share/hadoop/common/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-httpclient-3.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-beanutils-1.7.0.jar:/usr/local/hadoop/share/hadoop/common/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/lib/jsp-api-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-beanutils-core-1.8.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-configuration-1.6.jar:/usr/local/hadoop/share/hadoop/common/lib/jaxb-impl-2.2.3-1.jar:/usr/local/hadoop/share/hadoop/common/lib/zookeeper-3.4.5.jar:/usr/local/hadoop/share/hadoop/common/lib/hadoop-auth-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/common/lib/stax-api-1.0.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-collections-3.2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/servlet-api-2.5.jar:/usr/local/hadoop/share/hadoop/common/lib/junit-4.8.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jetty-util-6.1.26.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/jsr305-1.3.9.jar:/usr/local/hadoop/share/hadoop/common/lib/mockito-all-1.8.5.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-logging-1.1.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-math-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jaxb-api-2.2.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-json-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-net-3.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jetty-6.1.26.jar:/usr/local/hadoop/share/hadoop/common/lib/jasper-compiler-5.5.23.jar:/usr/local/hadoop/share/hadoop/common/lib/guava-11.0.2.jar:/usr/local/hadoop/share/hadoop/common/lib/xmlenc-0.52.jar:/usr/local/hadoop/share/hadoop/common/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/common/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-jaxrs-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/jettison-1.1.jar:/usr/local/hadoop/share/hadoop/common/hadoop-common-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/common/hadoop-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/hadoop-nfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/hdfs:/usr/local/hadoop/share/hadoop/hdfs/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-cli-1.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-el-1.0.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-daemon-1.0.13.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-lang-2.5.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jasper-runtime-5.5.23.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-codec-1.4.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jsp-api-2.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/servlet-api-2.5.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jetty-util-6.1.26.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jsr305-1.3.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-logging-1.1.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jetty-6.1.26.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/guava-11.0.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/xmlenc-0.52.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-nfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/guice-3.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/hamcrest-core-1.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/yarn/lib/aopalliance-1.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/javax.inject-1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/guice-servlet-3.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/yarn/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/yarn/lib/junit-4.10.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-guice-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/yarn/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-api-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-tests-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-site-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-client-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-nodemanager-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-applications-distributedshell-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-resourcemanager-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-web-proxy-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-applications-unmanaged-am-launcher-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/guice-3.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/hamcrest-core-1.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/aopalliance-1.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/javax.inject-1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/guice-servlet-3.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/junit-4.10.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-guice-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-plugins-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-app-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-core-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-shuffle-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-common-2.2.0.jar:/usr/local/hadoop/contrib/capacity-scheduler/*.jar 
STARTUP_MSG:   build = Unknown -r Unknown; compiled by 'root' on 2013-12-26T08:50Z 
STARTUP_MSG:   java = 1.7.0_45 
************************************************************/ 
14/02/12 08:23:28 INFO namenode.NameNode: registered UNIX signal handlers for [TERM, HUP, INT] 
14/02/12 08:23:30 WARN util.NativeCodeLoader: Unable to load native-hadoop library for your platform... using builtin-java classes where applicable 
Formatting using clusterid: c2 
14/02/12 08:23:31 INFO namenode.HostFileManager: read includes: 
HostSet( 
) 
14/02/12 08:23:31 INFO namenode.HostFileManager: read excludes: 
HostSet( 
) 
14/02/12 08:23:31 INFO blockmanagement.DatanodeManager: dfs.block.invalidate.limit=1000 
14/02/12 08:23:31 INFO util.GSet: Computing capacity for map BlocksMap 
14/02/12 08:23:31 INFO util.GSet: VM type       = 64-bit 
14/02/12 08:23:31 INFO util.GSet: 2.0% max memory = 966.7 MB 
14/02/12 08:23:31 INFO util.GSet: capacity      = 2^21 = 2097152 entries 
14/02/12 08:23:31 INFO blockmanagement.BlockManager: dfs.block.access.token.enable=false 
14/02/12 08:23:31 INFO blockmanagement.BlockManager: defaultReplication         = 2 
14/02/12 08:23:31 INFO blockmanagement.BlockManager: maxReplication             = 512 
14/02/12 08:23:31 INFO blockmanagement.BlockManager: minReplication             = 1 
14/02/12 08:23:31 INFO blockmanagement.BlockManager: maxReplicationStreams      = 2 
14/02/12 08:23:31 INFO blockmanagement.BlockManager: shouldCheckForEnoughRacks  = false 
14/02/12 08:23:31 INFO blockmanagement.BlockManager: replicationRecheckInterval = 3000 
14/02/12 08:23:31 INFO blockmanagement.BlockManager: encryptDataTransfer        = false 
14/02/12 08:23:31 INFO namenode.FSNamesystem: fsOwner             = root (auth:SIMPLE) 
14/02/12 08:23:31 INFO namenode.FSNamesystem: supergroup          = supergroup 
14/02/12 08:23:31 INFO namenode.FSNamesystem: isPermissionEnabled = true 
14/02/12 08:23:31 INFO namenode.FSNamesystem: Determined nameservice ID: cluster2 
14/02/12 08:23:31 INFO namenode.FSNamesystem: HA Enabled: true 
14/02/12 08:23:31 INFO namenode.FSNamesystem: Append Enabled: true 
14/02/12 08:23:31 INFO util.GSet: Computing capacity for map INodeMap 
14/02/12 08:23:31 INFO util.GSet: VM type       = 64-bit 
14/02/12 08:23:31 INFO util.GSet: 1.0% max memory = 966.7 MB 
14/02/12 08:23:31 INFO util.GSet: capacity      = 2^20 = 1048576 entries 
14/02/12 08:23:31 INFO namenode.NameNode: Caching file names occuring more than 10 times 
14/02/12 08:23:31 INFO namenode.FSNamesystem: dfs.namenode.safemode.threshold-pct = 0.9990000128746033 
14/02/12 08:23:31 INFO namenode.FSNamesystem: dfs.namenode.safemode.min.datanodes = 0 
14/02/12 08:23:31 INFO namenode.FSNamesystem: dfs.namenode.safemode.extension     = 30000 
14/02/12 08:23:31 INFO namenode.FSNamesystem: Retry cache on namenode is enabled 
14/02/12 08:23:31 INFO namenode.FSNamesystem: Retry cache will use 0.03 of total heap and retry cache entry expiry time is 600000 millis 
14/02/12 08:23:31 INFO util.GSet: Computing capacity for map Namenode Retry Cache 
14/02/12 08:23:31 INFO util.GSet: VM type       = 64-bit 
14/02/12 08:23:31 INFO util.GSet: 0.029999999329447746% max memory = 966.7 MB 
14/02/12 08:23:31 INFO util.GSet: capacity      = 2^15 = 32768 entries 
14/02/12 08:23:33 INFO common.Storage: Storage directory /usr/local/hadoop/tmp/dfs/name has been successfully formatted. 
14/02/12 08:23:33 INFO namenode.FSImage: Saving image file /usr/local/hadoop/tmp/dfs/name/current/fsimage.ckpt_0000000000000000000 using no compression 
14/02/12 08:23:33 INFO namenode.FSImage: Image file /usr/local/hadoop/tmp/dfs/name/current/fsimage.ckpt_0000000000000000000 of size 196 bytes saved in 0 seconds. 
14/02/12 08:23:33 INFO namenode.NNStorageRetentionManager: Going to retain 1 images with txid >= 0 
14/02/12 08:23:33 INFO util.ExitUtil: Exiting with status 0 
14/02/12 08:23:33 INFO namenode.NameNode: SHUTDOWN_MSG: 
/************************************************************ 
SHUTDOWN_MSG: Shutting down NameNode at hadoop103/192.168.80.103 
************************************************************/ 
[root@hadoop103 hadoop]#
【上面的输出可以看到/usr/local/hadoop/tmp/dfs/name 被成功格式化了】
验证：
[root@hadoop103 hadoop]# ls tmp/ 
dfs  journal 
[root@hadoop103 hadoop]# ls tmp/dfs/ 
name 
[root@hadoop103 hadoop]#
## 10.启动c2中刚才格式化的NameNode
在hadoop103上执行命令：/usr/local/hadoop/sbin/hadoop-daemon.sh start namenode
命令输出：
[root@hadoop103 hadoop]# /usr/local/hadoop/sbin/hadoop-daemon.sh start namenode 
starting namenode, logging to /usr/local/hadoop/logs/hadoop-root-namenode-hadoop103.out 
[root@hadoop103 hadoop]#
验证：
[root@hadoop103 hadoop]# jps 
11290 JournalNode 
11560 NameNode 
10972 QuorumPeerMain 
11600 Jps 
[root@hadoop103 hadoop]#
也可以通过浏览器访问[http://hadoop103:50070](http://hadoop103:50070/)，可以看到如上图页面，此处省略截图。
## 11.把NameNode的数据从hadoop103同步到hadoop104中
在hadoop104上执行命令：/usr/local/hadoop/bin/hdfs namenode –bootstrapStandby
命令输出：
[root@hadoop104 hadoop]# /usr/local/hadoop/bin/hdfs namenode -bootstrapStandby 
14/02/12 08:28:30  INFO namenode.NameNode: STARTUP_MSG: 
/************************************************************ 
STARTUP_MSG: Starting NameNode 
STARTUP_MSG:   host = hadoop104/192.168.80.104 
STARTUP_MSG:   args = [-bootstrapStandby] 
STARTUP_MSG:   version = 2.2.0 
STARTUP_MSG:   classpath = /usr/local/hadoop/etc/hadoop:/usr/local/hadoop/share/hadoop/common/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/common/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jsch-0.1.42.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/slf4j-log4j12-1.7.5.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-digester-1.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-cli-1.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-el-1.0.jar:/usr/local/hadoop/share/hadoop/common/lib/jets3t-0.6.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/slf4j-api-1.7.5.jar:/usr/local/hadoop/share/hadoop/common/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-lang-2.5.jar:/usr/local/hadoop/share/hadoop/common/lib/jasper-runtime-5.5.23.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-xc-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-codec-1.4.jar:/usr/local/hadoop/share/hadoop/common/lib/activation-1.1.jar:/usr/local/hadoop/share/hadoop/common/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-httpclient-3.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-beanutils-1.7.0.jar:/usr/local/hadoop/share/hadoop/common/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/lib/jsp-api-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-beanutils-core-1.8.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-configuration-1.6.jar:/usr/local/hadoop/share/hadoop/common/lib/jaxb-impl-2.2.3-1.jar:/usr/local/hadoop/share/hadoop/common/lib/zookeeper-3.4.5.jar:/usr/local/hadoop/share/hadoop/common/lib/hadoop-auth-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/common/lib/stax-api-1.0.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-collections-3.2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/servlet-api-2.5.jar:/usr/local/hadoop/share/hadoop/common/lib/junit-4.8.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jetty-util-6.1.26.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/jsr305-1.3.9.jar:/usr/local/hadoop/share/hadoop/common/lib/mockito-all-1.8.5.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-logging-1.1.1.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-math-2.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jaxb-api-2.2.2.jar:/usr/local/hadoop/share/hadoop/common/lib/jersey-json-1.9.jar:/usr/local/hadoop/share/hadoop/common/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/common/lib/commons-net-3.1.jar:/usr/local/hadoop/share/hadoop/common/lib/jetty-6.1.26.jar:/usr/local/hadoop/share/hadoop/common/lib/jasper-compiler-5.5.23.jar:/usr/local/hadoop/share/hadoop/common/lib/guava-11.0.2.jar:/usr/local/hadoop/share/hadoop/common/lib/xmlenc-0.52.jar:/usr/local/hadoop/share/hadoop/common/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/common/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/common/lib/jackson-jaxrs-1.8.8.jar:/usr/local/hadoop/share/hadoop/common/lib/jettison-1.1.jar:/usr/local/hadoop/share/hadoop/common/hadoop-common-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/common/hadoop-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/common/hadoop-nfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/hdfs:/usr/local/hadoop/share/hadoop/hdfs/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-cli-1.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-el-1.0.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-daemon-1.0.13.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-lang-2.5.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jasper-runtime-5.5.23.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-codec-1.4.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jsp-api-2.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/servlet-api-2.5.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jetty-util-6.1.26.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jsr305-1.3.9.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/commons-logging-1.1.1.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/jetty-6.1.26.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/guava-11.0.2.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/xmlenc-0.52.jar:/usr/local/hadoop/share/hadoop/hdfs/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-nfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/hdfs/hadoop-hdfs-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/guice-3.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/hamcrest-core-1.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/yarn/lib/aopalliance-1.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/javax.inject-1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/guice-servlet-3.0.jar:/usr/local/hadoop/share/hadoop/yarn/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/yarn/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/yarn/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/yarn/lib/junit-4.10.jar:/usr/local/hadoop/share/hadoop/yarn/lib/jersey-guice-1.9.jar:/usr/local/hadoop/share/hadoop/yarn/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/yarn/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-api-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-tests-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-site-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-client-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-nodemanager-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-applications-distributedshell-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-resourcemanager-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-web-proxy-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-applications-unmanaged-am-launcher-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-server-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/yarn/hadoop-yarn-common-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/protobuf-java-2.5.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/snappy-java-1.0.4.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-server-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/guice-3.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jackson-core-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-core-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/xz-1.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/hamcrest-core-1.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/avro-1.7.4.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/aopalliance-1.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/hadoop-annotations-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/commons-compress-1.4.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/javax.inject-1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/guice-servlet-3.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/commons-io-2.1.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/netty-3.6.2.Final.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jackson-mapper-asl-1.8.8.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/asm-3.2.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/junit-4.10.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/jersey-guice-1.9.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/log4j-1.2.17.jar:/usr/local/hadoop/share/hadoop/mapreduce/lib/paranamer-2.3.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-plugins-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-2.2.0-tests.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-app-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-core-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-shuffle-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-2.2.0.jar:/usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-client-common-2.2.0.jar:/usr/local/hadoop/contrib/capacity-scheduler/*.jar 
STARTUP_MSG:   build = Unknown -r Unknown; compiled by 'root' on 2013-12-26T08:50Z 
STARTUP_MSG:   java = 1.7.0_45
************************************************************/ 
14/02/12 08:28:35 INFO namenode.NameNode: registered UNIX signal handlers for [TERM, HUP, INT] 
===================================================== 
About to bootstrap Standby ID hadoop104 from: 
           Nameservice ID: cluster2 
        Other Namenode ID: hadoop103 
  Other NN's HTTP address: hadoop103:50070 
  Other NN's IPC  address: hadoop103/192.168.80.103:9000 
             Namespace ID: 698609742 
            Block pool ID: BP-1304582337-192.168.80.103-1392164613254 
               Cluster ID: c2 
           Layout version: -47 
===================================================== 
14/02/12 08:28:39 INFO common.Storage: Storage directory /usr/local/hadoop/tmp/dfs/name has been successfully formatted. 
14/02/12 08:28:39 INFO namenode.TransferFsImage: Opening connection to [http://hadoop103:50070/getimage?getimage=1&txid=0&storageInfo=-47:698609742:0:c2](http://hadoop103:50070/getimage?getimage=1&txid=0&storageInfo=-47:698609742:0:c2)
14/02/12 08:28:40 INFO namenode.TransferFsImage: Transfer took 0.67s at 0.00 KB/s 
14/02/12 08:28:40 INFO namenode.TransferFsImage: Downloaded file fsimage.ckpt_0000000000000000000 size 196 bytes. 
14/02/12 08:28:40 INFO util.ExitUtil: Exiting with status 0 
14/02/12 08:28:40 INFO namenode.NameNode: SHUTDOWN_MSG: 
/************************************************************ 
SHUTDOWN_MSG: Shutting down NameNode at hadoop104/192.168.80.104 
************************************************************/
验证：
[root@hadoop104 hadoop]# pwd 
/usr/local/hadoop 
[root@hadoop104 hadoop]# ls tmp/ 
dfs 
[root@hadoop104 hadoop]# ls tmp/dfs/ 
name 
[root@hadoop104 hadoop]#
## 12.启动c2中另一个Namenode
在hadoop104上执行命令：/usr/local/hadoop/sbin/hadoop-daemon.sh start namenode
命令输出：
[root@hadoop104 hadoop]# /usr/local/hadoop/sbin/hadoop-daemon.sh start namenode 
starting namenode, logging to /usr/local/hadoop/logs/hadoop-root-namenode-hadoop104.out 
[root@hadoop104 hadoop]#
验证：
[root@hadoop104 hadoop]# jps 
8822 NameNode 
8975 Jps 
[root@hadoop104 hadoop]#
也可以通过浏览器访问[http://hadoop104:50070](http://hadoop104:50070/)，可以看到如上图页面，此处省略截图。
## 13.启动所有的DataNode
在hadoop101上执行命令：/usr/local/hadoop/sbin/hadoop-daemons.sh start datanode
命令输出：
[root@hadoop101 hadoop]# /usr/local/hadoop/sbin/hadoop-daemons.sh start datanode 
hadoop101: starting datanode, logging to /usr/local/hadoop/logs/hadoop-root-datanode-hadoop101.out 
hadoop103: starting datanode, logging to /usr/local/hadoop/logs/hadoop-root-datanode-hadoop103.out 
hadoop102: starting datanode, logging to /usr/local/hadoop/logs/hadoop-root-datanode-hadoop102.out 
hadoop104: starting datanode, logging to /usr/local/hadoop/logs/hadoop-root-datanode-hadoop104.out 
[root@hadoop101 hadoop]#
【上述命令会在四个节点分别启动DataNode进程】
验证（以hadoop101为例）：
[root@hadoop101 hadoop]# jps 
23396 JournalNode 
24302 Jps 
24232 DataNode 
23558 NameNode 
22491 QuorumPeerMain 
[root@hadoop101 hadoop]#
【可以看到java进程DataNode】
## 14.启动Yarn
在hadoop101上执行命令：/usr/local/hadoop/sbin/start-yarn.sh
命令输出：
[root@hadoop101 hadoop]# /usr/local/hadoop/sbin/start-yarn.sh 
starting yarn daemons 
starting resourcemanager, logging to /usr/local/hadoop/logs/yarn-root-resourcemanager-hadoop101.out 
hadoop104: starting nodemanager, logging to /usr/local/hadoop/logs/yarn-root-nodemanager-hadoop104.out 
hadoop103: starting nodemanager, logging to /usr/local/hadoop/logs/yarn-root-nodemanager-hadoop103.out 
hadoop102: starting nodemanager, logging to /usr/local/hadoop/logs/yarn-root-nodemanager-hadoop102.out 
hadoop101: starting nodemanager, logging to /usr/local/hadoop/logs/yarn-root-nodemanager-hadoop101.out 
[root@hadoop101 hadoop]#
验证：
[root@hadoop101 hadoop]# jps 
23396 JournalNode 
25154 ResourceManager 
25247 NodeManager 
24232 DataNode 
23558 NameNode 
22491 QuorumPeerMain 
25281 Jps 
[root@hadoop101 hadoop]#
【产生java进程ResourceManager和NodeManager】
也可以通过浏览器访问，如下图
![image](http://images.cnitblog.com/blog/346286/201402/121441539477632.png)
## 15.启动ZooKeeperFailoverController
在hadoop101、hadoop102、hadoop103、hadoop104上分别执行命令：/usr/local/hadoop/sbin/hadoop-daemon.sh start zkfc
命令输出（以hadoop101为例）：
[root@hadoop101 hadoop]# /usr/local/hadoop/sbin/hadoop-daemon.sh start zkfc 
starting zkfc, logging to /usr/local/hadoop/logs/hadoop-root-zkfc-hadoop101.out 
[root@hadoop101 hadoop]#
验证（以hadoop101为例）：
[root@hadoop101 hadoop]# jps 
24599 DFSZKFailoverController 
23396 JournalNode 
24232 DataNode 
23558 NameNode 
22491 QuorumPeerMain 
24654 Jps 
[root@hadoop101 hadoop]#
【产生java进程DFSZKFailoverController】
## 16.验证HDFS是否好用
在任意一个节点上执行以下命令（这里以hadoop101为例），把数据上传到HDFS集群中
[root@hadoop101 hadoop]# pwd 
/usr/local/hadoop/etc/hadoop 
[root@hadoop101 hadoop]# ls 
capacity-scheduler.xml      hadoop-metrics.properties  httpfs-site.xml             ssl-server.xml.example 
configuration.xsl           hadoop-policy.xml          log4j.properties            startall.sh 
container-executor.cfg      hdfs2-site.xml             mapred-env.sh               yarn-env.sh 
core-site.xml               hdfs-site.xml              mapred-queues.xml.template  yarn-site.xml 
fairscheduler.xml           httpfs-env.sh              mapred-site.xml             zookeeper.out 
hadoop-env.sh               httpfs-log4j.properties    slaves 
hadoop-metrics2.properties  httpfs-signature.secret    ssl-client.xml.example 
[root@hadoop101 hadoop]# hadoop fs -put core-site.xml /
【上传到集群中，默认是上传到HDFS联盟的c1集群中】
验证：
[root@hadoop101 hadoop]# hadoop fs -ls / 
Found 1 items 
-rw-r--r--   2 root supergroup        446 2014-02-12 09:00 /core-site.xml 
[root@hadoop101 hadoop]# 
也可以通过浏览器查看，数据默认是放在第一个集群中的
![image](http://images.cnitblog.com/blog/346286/201402/121441555207377.png)
## 17.验证Yarn是否好用
在hadoop101上执行以下命令 hadoop jar /usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.2.0.jar   wordcount /core-site.xml /out
命令输出：
[root@hadoop101 hadoop]# hadoop jar /usr/local/hadoop/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.2.0.jar   wordcount /core-site.xml /out 
14/02/12 11:43:55 INFO client.RMProxy: Connecting to ResourceManager at hadoop101/192.168.80.101:8032 
14/02/12 11:43:59 INFO input.FileInputFormat: Total input paths to process : 1 
14/02/12 11:43:59 INFO mapreduce.JobSubmitter: number of splits:1 
14/02/12 11:43:59 INFO Configuration.deprecation: user.name is deprecated. Instead, use mapreduce.job.user.name 
14/02/12 11:43:59 INFO Configuration.deprecation: mapred.jar is deprecated. Instead, use mapreduce.job.jar 
14/02/12 11:43:59 INFO Configuration.deprecation: mapred.output.value.class is deprecated. Instead, use mapreduce.job.output.value.class 
14/02/12 11:43:59 INFO Configuration.deprecation: mapreduce.combine.class is deprecated. Instead, use mapreduce.job.combine.class 
14/02/12 11:43:59 INFO Configuration.deprecation: mapreduce.map.class is deprecated. Instead, use mapreduce.job.map.class 
14/02/12 11:43:59 INFO Configuration.deprecation: mapred.job.name is deprecated. Instead, use mapreduce.job.name 
14/02/12 11:43:59 INFO Configuration.deprecation: mapreduce.reduce.class is deprecated. Instead, use mapreduce.job.reduce.class 
14/02/12 11:43:59 INFO Configuration.deprecation: mapred.input.dir is deprecated. Instead, use mapreduce.input.fileinputformat.inputdir 
14/02/12 11:43:59 INFO Configuration.deprecation: mapred.output.dir is deprecated. Instead, use mapreduce.output.fileoutputformat.outputdir 
14/02/12 11:43:59 INFO Configuration.deprecation: mapred.map.tasks is deprecated. Instead, use mapreduce.job.maps 
14/02/12 11:43:59 INFO Configuration.deprecation: mapred.output.key.class is deprecated. Instead, use mapreduce.job.output.key.class 
14/02/12 11:43:59 INFO Configuration.deprecation: mapred.working.dir is deprecated. Instead, use mapreduce.job.working.dir 
14/02/12 11:44:01 INFO mapreduce.JobSubmitter: Submitting tokens for job: job_1392169506119_0002 
14/02/12 11:44:04 INFO impl.YarnClientImpl: Submitted application application_1392169506119_0002 to ResourceManager at hadoop101/192.168.80.101:8032 
14/02/12 11:44:05 INFO mapreduce.Job: The url to track the job: [http://hadoop101:8088/proxy/application_1392169506119_0002/](http://hadoop101:8088/proxy/application_1392169506119_0002/)
14/02/12 11:44:05 INFO mapreduce.Job: Running job: job_1392169506119_0002 
14/02/12 11:44:41 INFO mapreduce.Job: Job job_1392169506119_0002 running in uber mode : false 
14/02/12 11:44:41 INFO mapreduce.Job:  map 0% reduce 0% 
14/02/12 11:45:37 INFO mapreduce.Job:  map 100% reduce 0% 
14/02/12 11:46:54 INFO mapreduce.Job:  map 100% reduce 100% 
14/02/12 11:47:01 INFO mapreduce.Job: Job job_1392169506119_0002 completed successfully 
14/02/12 11:47:02 INFO mapreduce.Job: Counters: 43 
        File System Counters 
                FILE: Number of bytes read=472 
                FILE: Number of bytes written=164983 
                FILE: Number of read operations=0 
                FILE: Number of large read operations=0 
                FILE: Number of write operations=0 
                HDFS: Number of bytes read=540 
                HDFS: Number of bytes written=402 
                HDFS: Number of read operations=6 
                HDFS: Number of large read operations=0 
                HDFS: Number of write operations=2 
        Job Counters 
                Launched map tasks=1 
                Launched reduce tasks=1 
                Data-local map tasks=1 
                Total time spent by all maps in occupied slots (ms)=63094 
                Total time spent by all reduces in occupied slots (ms)=57228 
        Map-Reduce Framework 
                Map input records=17 
                Map output records=20 
                Map output bytes=496 
                Map output materialized bytes=472 
                Input split bytes=94 
                Combine input records=20 
                Combine output records=16 
                Reduce input groups=16 
                Reduce shuffle bytes=472 
                Reduce input records=16 
                Reduce output records=16 
                Spilled Records=32 
                Shuffled Maps =1 
                Failed Shuffles=0 
                Merged Map outputs=1 
                GC time elapsed (ms)=632 
                CPU time spent (ms)=3010 
                Physical memory (bytes) snapshot=255528960 
                Virtual memory (bytes) snapshot=1678471168 
                Total committed heap usage (bytes)=126660608 
        Shuffle Errors 
                BAD_ID=0 
                CONNECTION=0 
                IO_ERROR=0 
                WRONG_LENGTH=0 
                WRONG_MAP=0 
                WRONG_REDUCE=0 
        File Input Format Counters 
                Bytes Read=446 
        File Output Format Counters 
                Bytes Written=402 
[root@hadoop101 hadoop]# 
验证：
[root@hadoop101 hadoop]# hadoop fs -ls /out 
Found 2 items 
-rw-r--r--   2 root supergroup          0 2014-02-12 11:46 /out/_SUCCESS 
-rw-r--r--   2 root supergroup        402 2014-02-12 11:46 /out/part-r-00000 
[root@hadoop101 hadoop]# hadoop fs -text /out/part-r-00000 
< /configuration>        1 
< /property>     3 
< ?xml   1 
< ?xml-stylesheet        1 
< configuration> 1 
< name>fs.defaultFS</name>       1 
< name>ha.zookeeper.quorum</name>        1 
< name>hadoop.tmp.dir</name>     1 
< property>      3 
< value>/usr/local/hadoop/tmp</value>    1 
< value>hadoop101:2181,hadoop102:2181,hadoop103:2181</value>     1 
< value>hdfs://cluster1</value>  1 
encoding="UTF-8"?>      1 
href="configuration.xsl"?>      1 
type="text/xsl" 1 
version="1.0"   1 
[root@hadoop101 hadoop]#
## 18.验证HA的故障自动转移是否好用
观察cluster1的两个NameNode的状态，hadoop101的状态是standby，hadoop102的状态是active，如下图。
![image](http://images.cnitblog.com/blog/346286/201402/121441574473236.png)
![image](http://images.cnitblog.com/blog/346286/201402/121442052141841.png)
下面我们杀死hadoop102的NameNode进程，观察hadoop101的状态是否会自动切换成active。
在hadoop102执行以下命令
[root@hadoop102 hadoop]# jps 
13389 DFSZKFailoverController 
12355 JournalNode 
13056 DataNode 
15660 Jps 
14496 NodeManager 
12573 NameNode 
12081 QuorumPeerMain 
[root@hadoop102 hadoop]# kill -9 12573 
[root@hadoop102 hadoop]# jps 
13389 DFSZKFailoverController 
12355 JournalNode 
13056 DataNode 
14496 NodeManager 
15671 Jps 
12081 QuorumPeerMain 
[root@hadoop102 hadoop]#
再观察页面，发现如下图所示
![image](http://images.cnitblog.com/blog/346286/201402/121442071622400.png)
![image](http://images.cnitblog.com/blog/346286/201402/121442090296473.png)
证明HDFS的高可靠是可用的。
