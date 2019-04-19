# HBase分布式安装 - BlueSky - CSDN博客
2016年03月02日 20:49:26[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：852
## 说明
对于HBase与Hadoop版本要选择相对应的（否则会出很多问题），具体对应版本可参考：https://hbase.apache.org/book.html#basic.prerequisites
        本次集群对应版本为：Hadoop-2.7.2 + HBase-1.2.0
192.168.203.148    Master
        192.168.203.149   Slave1
        192.168.203.150   Slave2 
版本要选择对应的否则话会出现很多问题，最后会说明下遇到的问题（爬坑过程）。主要是基于之前安装过hadoop、spark等，所以一些基本环境比如Java、ssh访问、防火墙等问题，这些基本就不涉及了。主要是相应配置文件的配置。
集群时间最好统一
![](https://img-blog.csdn.net/20160302210658556?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 安装：
### 第一步：依赖环境
主要有Java、Hadoop、zookeeper
HBase分布式下安装，则需首先安装hdfs也即使安装hadoop，对于zookeeper可以选择不安装（不安装的话就是用hbase自带的来管理hbase，安装的话就是利用zookeeper额外的集群来）。
Hadoop安装则可以参考以前的博客，spark集群安装
### 第二步：HBase安装
将下载的HBase文件解压，并放于/home/hadoop/soft/下：
```
[hadoop@Master soft]$ pwd
/home/hadoop/soft
[hadoop@Master soft]$ ls
hadoop-2.7.2  hbase-1.2.0
[hadoop@Master soft]$
```
### 第三步：HBase相关文件配置
以下文件位置都是在HBase目录下
**1、conf/hbase-env.sh  添加如下：**
```
export JAVA_HOME=/usr/local/java/jdk1.8.0_65
export HBASE_CLASSPATH=/home/hadoop/soft/hadoop-2.7.2/etc/hadoop/
export HBASE_MANAGES_ZK=True
```
   不管是什么模式，你都需要编辑 conf/hbase-env.sh来告知Hbase java的安装路径.在这个文件里你还可以设置Hbase的运行环境，诸如heapsize和其他 JVM有关的选项, 还有Log文件地址，等等. 设置 JAVA_HOME指向 java安装的路径.
    一个分布式运行的Hbase依赖一个zookeeper集群。所有的节点和客户端都必须能够访问zookeeper。默认的情况下Hbase会管理一个zookeep集群。这个集群会随着Hbase的启动而启动。当然，你也可以自己管理一个zookeeper集群，但需要配置Hbase。
    你需要修改conf/hbase-env.sh里面的HBASE_MANAGES_ZK 来切换。这个值默认是true的，作用是让Hbase启动的时候同时也启动zookeeper。让Hbase使用一个现有的不被Hbase托管的Zookeep集群，需要设置 conf/hbase-env.sh文件中的HBASE_MANAGES_ZK 属性为 false。
**2、conf/hbase-site.xml**
```
<configuration>
	<property>
		<name>hbase.rootdir</name>
		<value>hdfs://Master:9000/hbase</value>
	</property>
	<property>
		<name>hbase.master</name>
		<value>Master</value>
	</property>
	<property>
		<name>hbase.cluster.distributed</name>
		<value>true</value>
	</property>
	<property>
		<name>hbase.zookeeper.quorum</name>
		<value>Master,Slave1,Slave2</value>
	</property>
	<property>
		<name>hbase.zookeeper.property.clientPort</name>
		<value>2181</value>
	</property>
	<property>  
   		<name>hbase.zookeeper.property.dataDir</name>  
  		<value>/home/hadoop/soft/hbase-1.2.0/dataZookeeper</value>  
	</property>  
	<property>
		<name>zookeeper.session.timeout</name>
		<value>60000000</value>
	</property>
	<property>
		<name>dfs.support.append</name>
		<value>true</value>
	</property>
</configuration>
```
要想运行完全分布式模式，加一个属性 hbase.cluster.distributed 设置为 true 然后把 hbase.rootdir 设置为HDFS的NameNode的位置。 例如，你的namenode运行在node1，端口是49002 你期望的目录是 /hbase,使用如下的配置：hdfs://node1:49002/hbase
hbase.rootdir：这个目录是region server的共享目录，用来持久化Hbase。URL需要是'完全正确'的，还要包含文件系统的scheme。例如，要表示hdfs中的'/hbase'目录，namenode 运行在node1的49002端口。则需要设置为hdfs://node1:49002/hbase。默认情况下Hbase是写到/tmp的。不改这个配置，数据会在重启的时候丢失。默认: file:///tmp/hbase-${user.name}/hbase
hbase.cluster.distributed ：Hbase的运行模式。false是单机模式，true是分布式模式。若为false,Hbase和Zookeeper会运行在同一个JVM里面。
默认: false
在hbase-site.xml配置zookeeper：
当Hbase管理zookeeper的时候，你可以通过修改zoo.cfg来配置zookeeper，
一个更加简单的方法是在 conf/hbase-site.xml里面修改zookeeper的配置。Zookeeer的配置是作为property写在 hbase-site.xml里面的。
对于zookeepr的配置，你至少要在 hbase-site.xml中列出zookeepr的ensemble servers，具体的字段是 hbase.zookeeper.quorum. 该这个字段的默认值是 localhost，这个值对于分布式应用显然是不可以的. (远程连接无法使用)。
hbase.zookeeper.property.clientPort：ZooKeeper的zoo.conf中的配置。 客户端连接的端口。
hbase.zookeeper.quorum：Zookeeper集群的地址列表，用逗号分割。例如："host1.mydomain.com,host2.mydomain.com,host3.mydomain.com".默认是localhost,是给伪分布式用的。要修改才能在完全分布式的情况下使用。如果在hbase-env.sh设置了HBASE_MANAGES_ZK，这些ZooKeeper节点就会和Hbase一起启动。
默认: localhost
运行一个zookeeper也是可以的，但是在生产环境中，你最好部署3，5，7个节点。部署的越多，可靠性就越高，当然只能部署奇数个，偶数个是不可以的。你需要给每个zookeeper 1G左右的内存，如果可能的话，最好有独立的磁盘。 (独立磁盘可以确保zookeeper是高性能的。).如果你的集群负载很重，不要把Zookeeper和RegionServer运行在同一台机器上面。就像DataNodes 和 TaskTrackers一样
hbase.zookeeper.property.dataDir：ZooKeeper的zoo.conf中的配置。 快照的存储位置
把ZooKeeper保存数据的目录地址改掉。默认值是 /tmp ，这里在重启的时候会被操作系统删掉，可以把它修改到 /home/hadoop/zookeeper (这个路径hadoop用户拥有操作权限)
对于独立的Zookeeper，要指明Zookeeper的host和端口。可以在 hbase-site.xml中设置, 也可以在Hbase的CLASSPATH下面加一个zoo.cfg配置文件。 HBase 会优先加载 zoo.cfg 里面的配置，把hbase-site.xml里面的覆盖掉.
**3、conf/regionservers**
Slave1
Slave2
完全分布式模式的还需要修改conf/regionservers. 在这里列出了你希望运行的全部 HRegionServer，一行写一个host (就像Hadoop里面的 slaves 一样). 列在这里的server会随着集群的启动而启动，集群的停止而停止。
### 第四步：将HBase文件分发到其余节点
```
scp -r hbase-1.2.0 Slave1:/home/hadoop/soft/
scp -r hbase-1.2.0 Slave2:/home/hadoop/soft/
```
### 第五步：运行并验证
```
首先，要启动Hadoop（或者Hdfs）：
/home/hadoop/soft/hadoop-2.7.2/sbin/start-all.sh
其次，启动HBase：
bin/start-hbase.sh<strong>
</strong>
```
5.1当Hbase托管ZooKeeper的时候
当Hbase托管ZooKeeper的时候Zookeeper集群的启动是Hbase启动脚本的一部分。
首先确认你的HDFS是运行着的。你可以运行HADOOP_HOME中的 bin/start-hdfs.sh 来启动HDFS.你可以通过put命令来测试放一个文件，然后有get命令来读这个文件。通常情况下Hbase是不会运行mapreduce的。所以比不需要检查这些。
用如下命令启动Hbase:
bin/start-hbase.sh
这个脚本在HBASE_HOME目录里面。
你现在已经启动Hbase了。Hbase把log记在 logs 子目录里面. 当Hbase启动出问题的时候，可以看看Log.
Hbase也有一个界面，上面会列出重要的属性。默认是在Master的60010端口上H (HBase RegionServers 会默认绑定 60020端口，在端口60030上有一个展示信息的界面 ).如果Master运行在 node1，端口是默认的话，你可以用浏览器在
 http://node:60010看到主界面. .
一旦Hbase启动，可以看到如何建表，插入数据，scan你的表，还有disable这个表，最后把它删掉。
可以在Hbase Shell停止Hbase：
$./bin/stop-hbase.sh
stoppinghbase...............
停止操作需要一些时间，你的集群越大，停的时间可能会越长。如果你正在运行一个分布式的操作，要确认在Hbase彻底停止之前，Hadoop不能停。
5.2独立的zookeeper启动：
除了启动habse，
执行：bin/start-hbase.sh启动habse
你需要自己去运行zookeeper：
${HBASE_HOME}/bin/hbase-daemons.sh {start,stop} zookeeper
你可以用这条命令启动ZooKeeper而不启动Hbase. HBASE_MANAGES_ZK 的值是 false， 如果你想在Hbase重启的时候不重启ZooKeeper,你可以这样。
```
<span style="font-weight: normal;">输入jps验证：
Master节点
[hadoop@Master bin]$ jps
3761 HMaster
3703 HQuorumPeer
3065 ResourceManager
4012 Jps
2909 SecondaryNameNode
2718 NameNode
slave节点：
[hadoop@Slave1 soft]$ jps
2901 HRegionServer
2825 HQuorumPeer
2510 DataNode
3022 Jps</span>
```
也可以通过web窗口验证：http://Master:16010 来查看HBase应用窗口
## 遇到的问题：
HBase中HMaster启动后就消失（查了试了很多都没有解决）：
查找资料，所说解决方法主要为以下几点：1、格式化hdfs节点 hadoop namenode -format ，然后重启集群；2、修改hdfs集群上/hbase权限，hadoop fs -chmod 777 /hbase ；3、修改HBase文件lib下与hadoop相关的jar包文件；4、版本问题。。。
解决方法参照：
http://stackoverflow.com/questions/22663484/get-error-cant-get-master-address-from-zookeeper-znode-data-null-when-us
http://chengjianxiaoxue.iteye.com/blog/2242982
http://blog.csdn.net/hguisu/article/details/7244413
**最后还是尝试选择安装了对应版本，这次却没出现什么问题，看来还是选择相对应版本为好。**
**但是还是学到了一些东西：**
**集群时间统一：利用ntp  http://www.kevenking.cn/archives/2013/06/27/334.html**
**linux命令查找并拷贝：find /home/hadoop/hadoop-2.2.0/share/hadoop -name "hadoop*jar" | xargs -i cp {} /home/hadoop/hbase-0.96.0-hadoop2/lib/ **
