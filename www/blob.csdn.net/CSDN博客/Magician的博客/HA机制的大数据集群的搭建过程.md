
# HA机制的大数据集群的搭建过程 - Magician的博客 - CSDN博客


2018年09月21日 17:29:41[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：124所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)




### 文章目录
[集群规划](#_1)
[1、 准备Linux环境](#1_Linux_15)
[1.1、修改Linux主机名](#11Linux_19)
[1.3、修改主机名和IP的映射关系](#13IP_39)
[1.4、关闭防火墙](#14_46)
[1.5、ssh免登陆（主节点和从节点之间）](#15ssh_57)
[2、 安装JDK](#2_JDK_81)
[2.1、上传](#21_85)
[2.2、解压jdk](#22jdk_87)
[2.3、将java添加到环境变量中](#23java_94)
[2.4、刷新配置](#24_102)
[2.5、验证](#25_106)
[3、安装zookeeper](#3zookeeper_112)
[3.1、解压](#31_117)
[3.2、修改环境变量](#32_121)
[3.3、刷新配置](#33_128)
[3.4、修改配置文件](#34_132)
[3.5、将配置好的zookeeper拷贝到其他节点](#35zookeeper_148)
[4、安装配置hadoop集群](#4hadoop_163)
[4.1、解压](#41_168)
[4.2、修改环境变量](#42_172)
[4.3、修改hadoop配置文件](#43hadoop_182)
[4.4、将配置好的hadoop拷贝到其他节点](#44hadoop_340)
[4.5、初始化集群的步骤](#45_345)
[4.6、验证集群的配置](#46_389)

# 集群规划
![在这里插入图片描述](https://img-blog.csdn.net/20180921171103686?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
说明：
1、在hadoop2.0中通常由两个NameNode组成，一个处于active状态，另一个处于standby状态。Active NameNode对外提供服务，而Standby NameNode则不对外提供服务，仅同步active namenode的状态，以便能够在它失败时快速进行切换。主备NameNode之间通过一组JournalNode同步元数据信息，一条数据只要成功写入多数JournalNode即认为写入成功。通常配置奇数个JournalNode。这里还配置了一个zookeeper集群，用于ZKFC（DFSZKFailoverController）故障转移，当Active NameNode挂掉了，会自动切换Standby NameNode为standby状态。
2、hadoop-2.2.0中依然存在一个问题，就是ResourceManager只有一个，存在单点故障，hadoop-2.6.4解决了这个问题，有两个ResourceManager，一个是Active，一个是Standby，状态由zookeeper进行协调。
**集群环境：**
**Hadoop2.6.4+Hive2.1.1+Hbase2.0.0+Zookeeper3.4.5+JDK1.8+Centos6.9+Sqoop1.4.6**
**这里只说下hadoop的搭建过程，其他组件的安装过程可参考我的其他文章：**
**Hive2.1.1：****[https://blog.csdn.net/qq_16633405/article/details/78278979](https://blog.csdn.net/qq_16633405/article/details/78278979)**
**Hbase2.0.0：****[https://blog.csdn.net/qq_16633405/article/details/81908283](https://blog.csdn.net/qq_16633405/article/details/81908283)**
**Sqoop1.4.6：****[https://blog.csdn.net/qq_16633405/article/details/82224289](https://blog.csdn.net/qq_16633405/article/details/82224289)**
**1、 准备Linux环境**
**所有节点都需配置。**
**1.1、修改Linux主机名**
**vi /etc/sysconfig/network
1.	NETWORKING=yes
2.	HOSTNAME=ha1 \#\#\#自定义主机名****1.2、修改IP**
**vim /etc/sysconfig/network-scripts/ifcfg-eth0
3.	DEVICE=eth0
4.	TYPE=Ethernet
5.	ONBOOT=yes
6.	NM_CONTROLLED=yes
7.	BOOTPROTO=static
8.	IPADDR=192.168.112.200
9.	GATEWAY=192.168.112.2 \#\#\#网关地址和子网IP不要重复
10.	NETMASK=255.255.255.0
11.	DNS=192.168.112.2****1.3、修改主机名和IP的映射关系**
**vim /etc/hosts
1.	192.168.112.21 ha2
2.	192.168.112.22 ha3
3.	192.168.112.20 ha1****1.4、关闭防火墙**
**\#查看防火墙状态
service iptables status
\#关闭防火墙
service iptables stop
\#查看防火墙开机启动状态
chkconfig iptables --list
\#关闭防火墙开机启动
chkconfig iptables off****1.5、ssh免登陆（主节点和从节点之间）**
**在ha1上生产一对钥匙
ssh-keygen -t rsa
\#将公钥拷贝到其他节点，包括自己
ssh-coyp-id ha1
ssh-coyp-id ha2
……
\#配置ha2到ha3、ha4、ha5的免密码登陆
\#在ha2上生产一对钥匙
ssh-keygen -t rsa
\#将公钥拷贝到其他节点
ssh-coyp-id ha2				
ssh-coyp-id ha3
……
\#注意：两个namenode之间要配置ssh免密码登陆
\#在ha2上执行
ssh-coyp-id ha1				
				
验证：
ssh ha1
exit****2、 安装JDK**
**所有节点都需配置**
**2.1、上传**
**2.2、解压jdk**
**创建文件夹
mkdir /home/hadoop/app
\#解压
tar -zxvf jdk-7u55-linux-i586.tar.gz -C /home/hadoop/app****2.3、将java添加到环境变量中**
**vim /etc/profile
\#在文件最后添加
4.	export JAVA_HOME=/home/hadoop/app/jdk-7u_65-i585
5.	export PATH=$PATH:$JAVA_HOME/bin****2.4、刷新配置**
**source /etc/profile****2.5、验证**
**Jsp****3、安装zookeeper**
**版本：3.4.5**
**安装配置zooekeeper集群（在ha1、ha2、ha3上）**
**3.1、解压**
**tar -zxvf zookeeper-3.4.5.tar.gz -C /home/hadoop/app/****3.2、修改环境变量**
**vi /etc/profile
\#添加内容：
6.	export ZOOKEEPER_HOME=/root/hadoop/zookeeper
7.	export PATH=$PATH:$ZOOKEEPER_HOME/bin****3.3、刷新配置**
**source /etc/profile****3.4、修改配置文件**
**cd /home/hadoop/app/zookeeper-3.4.5/conf/
cp zoo_sample.cfg zoo.cfg
vim zoo.cfg
修改：dataDir=/root/apps/zookeeper-3.4.5/tmp
在最后添加：
8.	server.1=ha1:2888:3888
9.	server.2=ha2:2888:3888
10.	server.3=ha3:2888:3888
保存退出
然后创建一个tmp文件夹：
mkdir /home/hadoop/app/zookeeper-3.4.5/tmp
echo 1 > /home/hadoop/app/zookeeper-3.4.5/tmp/myid****3.5、将配置好的zookeeper拷贝到其他节点**
**首先分别在ha2、ha3根目录下创建一个hadoop目录：mkdir /Hadoop**
**scp -r /home/hadoop/app/zookeeper-3.4.5/ ha2:/home/hadoop/app/
scp -r /home/hadoop/app/zookeeper-3.4.5/ ha3:/home/hadoop/app/****注意****：修改ha2、ha3对应/hadoop/zookeeper-3.4.5/tmp/myid内容**
**Ha2：**
**echo 2 > /home/hadoop/app/zookeeper-3.4.5/tmp/myid**
**ha3：**
**echo 3 > /home/hadoop/app/zookeeper-3.4.5/tmp/myid**
**4、安装配置hadoop集群**
**版本：2.6.4**
**在ha1上操作。**
**4.1、解压**
**tar -zxvf hadoop-2.6.4.tar.gz -C /home/hadoop/app/****4.2、修改环境变量**
**hadoop2.0所有的配置文件都在$HADOOP_HOME/etc/hadoop目录下。**
**\#将hadoop添加到环境变量中
vim /etc/profile
11.	export HADOOP_HOME=/hadoop/hadoop-2.6.4
12.	export PATH=$PATH:$JAVA_HOME/bin:$HADOOP_HOME/bin:$HADOOP_HOME/sbin****4.3、修改hadoop配置文件**
**cd /home/hadoop/app/hadoop-2.6.4/etc/hadoop**
**[4.3.1修改hadoo-env.sh](http://4.3.xn--1hadoo-env-zu6pe85r.sh)**
**13.	export JAVA_HOME=/home/hadoop/app/jdk1.7.0_55****4.3.2修改core-site.xml**
**14.	<configuration>  
15.	<!-- 指定hdfs的nameservice为ns1 -->  
16.	<property>  
17.	<name>fs.defaultFS</name>  
18.	<value>hdfs://bi/</value>  
19.	</property>  
20.	<!-- 指定hadoop临时目录 -->  
21.	<property>  
22.	<name>hadoop.tmp.dir</name>  
23.	<value>/home/hadoop/app/hdpdata/</value>  
24.	</property>  
25.	  
26.	<!-- 指定zookeeper地址 -->  
27.	<property>  
28.	<name>ha.zookeeper.quorum</name>  
29.	<value>mini5:2181,mini6:2181,mini7:2181</value>  
30.	</property>  
31.	</configuration>****4.3.3修改hdfs-site.xml**
**32.	<configuration>
33.	<!--指定hdfs的nameservice为bi，需要和core-site.xml中的保持一致 -->
34.	<property>
35.	<name>dfs.nameservices</name>
36.	<value>bi</value>
37.	</property>
38.	<!-- bi下面有两个NameNode，分别是nn1，nn2 -->
39.	<property>
40.	<name>dfs.ha.namenodes.bi</name>
41.	<value>nn1,nn2</value>
42.	</property>
43.	<!-- nn1的RPC通信地址 -->
44.	<property>
45.	<name>dfs.namenode.rpc-address.bi.nn1</name>
46.	<value>ha1:9000</value>
47.	</property>
48.	<!-- nn1的http通信地址 -->
49.	<property>
50.	<name>dfs.namenode.http-address.bi.nn1</name>
51.	<value>ha1:50070</value>
52.	</property>
53.	<!-- nn2的RPC通信地址 -->
54.	<property>
55.	<name>dfs.namenode.rpc-address.bi.nn2</name>
56.	<value>ha2:9000</value>
57.	</property>
58.	<!-- nn2的http通信地址 -->
59.	<property>
60.	<name>dfs.namenode.http-address.bi.nn2</name>
61.	<value>ha2:50070</value>
62.	</property>
63.	<!-- 指定NameNode的edits元数据在JournalNode上的存放位置 -->
64.	<property>
65.	<name>dfs.namenode.shared.edits.dir</name>
66.	<value>qjournal://ha3:8485/bi</value>
67.	</property>
68.	<!-- 指定JournalNode在本地磁盘存放数据的位置 -->
69.	<property>
70.	<name>dfs.journalnode.edits.dir</name>
71.	<value>/home/hadoop/journaldata</value>
72.	</property>
73.	<!-- 开启NameNode失败自动切换 -->
74.	<property>
75.	<name>dfs.ha.automatic-failover.enabled</name>
76.	<value>true</value>
77.	</property>
78.	<!-- 配置失败自动切换实现方式 -->
79.	<property>
80.	<name>dfs.client.failover.proxy.provider.bi</name>
81.	<value>org.apache.hadoop.hdfs.server.namenode.ha.ConfiguredFailoverProxyProvider</value>
82.	</property>
83.	<!-- 配置隔离机制方法，多个机制用换行分割，即每个机制暂用一行-->
84.	<property>
85.	<name>dfs.ha.fencing.methods</name>
86.	<value>
87.	sshfence
88.	shell(/bin/true)
89.	</value>
90.	</property>
91.	<!-- 使用sshfence隔离机制时需要ssh免登陆 -->
92.	<property>
93.	<name>dfs.ha.fencing.ssh.private-key-files</name>
94.	<value>/root/.ssh/id_rsa</value>
95.	</property>
96.	<!-- 配置sshfence隔离机制超时时间 -->
97.	<property>
98.	<name>dfs.ha.fencing.ssh.connect-timeout</name>
99.	<value>30000</value>
100.	</property>
101.	</configuration>****4.3.4修改mapred-site.xml**
**102.	<configuration>
103.	<!-- 指定mr框架为yarn方式 -->
104.	<property>
105.	<name>mapreduce.framework.name</name>
106.	<value>yarn</value>
107.	</property>
108.	</configuration>****4.3.5修改yarn-site.xml**
**109.	<configuration>
110.	<!-- 开启RM高可用 -->
111.	<property>
112.	<name>yarn.resourcemanager.ha.enabled</name>
113.	<value>true</value>
114.	</property>
115.	<!-- 指定RM的cluster id -->
116.	<property>
117.	<name>yarn.resourcemanager.cluster-id</name>
118.	<value>yrc</value>
119.	</property>
120.	<!-- 指定RM的名字 -->
121.	<property>
122.	<name>yarn.resourcemanager.ha.rm-ids</name>
123.	<value>rm1,rm2</value>
124.	</property>
125.	<!-- 分别指定RM的地址 -->
126.	<property>
127.	<name>yarn.resourcemanager.hostname.rm1</name>
128.	<value>ha1</value>
129.	</property>
130.	<property>
131.	<name>yarn.resourcemanager.hostname.rm2</name>
132.	<value>ha2</value>
133.	</property>
134.	<!-- 指定zk集群地址，用来存储RM的状态，配置三个节点（三份数据）就OK了 -->
135.	<property>
136.	<name>yarn.resourcemanager.zk-address</name>
137.	<value>ha3:2181</value>
138.	</property>
139.	<property>
140.	<name>yarn.nodemanager.aux-services</name>
141.	<value>mapreduce_shuffle</value>
142.	</property>
143.	</configuration>****4.3.6修改slaves**
**slaves是指定子节点的位置，因为要在ha1上启动HDFS、yarn，所以ha1上的slaves文件指定的是datanode、nodemanager的位置。添加需要的节点：**
**144.	ha3****4.4、将配置好的hadoop拷贝到其他节点**
**scp -r /hadoop/hadoop-2.6.4/ hadoop@ha2:/hadoop/
scp -r /hadoop/hadoop-2.6.4/ hadoop@ha3:/hadoop/****4.5、初始化集群的步骤**
**注意：严格按照下面的步骤!!!**
**4.5.1启动zookeeper集群**
**分别在ha1、ha2、ha3上启动zk**
**cd /hadoop/zookeeper-3.4.5/bin/
	./zkServer.sh start
	\#查看状态：一个leader，两个follower
	./zkServer.sh status****4.5.2启动journalnode**
**在ha3上启动journalnode**
**cd /hadoop/hadoop-2.6.4
	sbin/hadoop-daemon.sh start journalnode
	\#运行jps命令检验，ha3上多了JournalNode进程****4.5.3格式化HDFS**
**在ha1上执行命令:**
**hdfs namenode -format
\#格式化后会在根据core-site.xml中的hadoop.tmp.dir配置生成个文件，这里我配置的是/hadoop/hadoop-2.6.4/tmp，然后将/hadoop/hadoop-2.6.4/tmp拷贝到ha2的/hadoop/hadoop-2.6.4/下。
scp -r tmp/ hadoop02:/home/hadoop/app/hadoop-2.6.4/
\#\#也可以这样，建议hdfs namenode -bootstrapStandby****4.5.4格式化ZKFC**
**在ha1上执行一次即可**
**hdfs zkfc -formatZK****4.5.5启动HDFS**
**在ha1上执行**
**sbin/start-dfs.sh****4.5.6启动YARN**
**分别在ha1、ha2上启动ResourceManager**
**sbin/start-yarn.sh****4.6、验证集群的配置**
**hadoop-2.6.4配置完毕，验证集群的配置。**
**可以统计浏览器访问:**
**[http://ha1:50070](http://ha1:50070)**
**NameNode ‘hadoop01:9000’ (active)**
**[http://ha2:50070](http://ha2:50070)**
**NameNode ‘hadoop02:9000’ (standby)**
**4.6.1验证HDFS HA**
**首先向hdfs上传一个文件**
**hadoop fs -put /etc/profile /profile****\#查看上传到hdfs的文件**
**hadoop fs -ls /****然后再kill掉active的NameNode**
**kill -9 <pid of NN>****通过浏览器访问：****[http://ha2:50070](http://ha2:50070)**
**NameNode ‘hadoop02:9000’ (active)**
**这个时候ha2上的NameNode变成了active**
**在执行命令：**
**hadoop fs -ls /
-rw-r--r--   3 root supergroup       1926 2014-02-06 15:36 /profile****刚才上传的文件依然存在!**
**手动启动那个挂掉的NameNode**
**sbin/hadoop-daemon.sh start namenode****通过浏览器访问：****[http://ha1:50070](http://ha1:50070)**
**NameNode ‘ha1:9000’ (standby)**
**4.6.2验证YARN：**
**运行一下hadoop提供的demo中的WordCount程序：**
**hadoop jar share/hadoop/mapreduce/hadoop-mapreduce-examples-2.4.1.jar wordcount /profile /out
无报错，OK，大功告成！****4.6.3测试集群工作状态的一些指令**
**bin/hdfs dfsadmin -report	 查看hdfs的各节点状态信息
bin/hdfs haadmin -getServiceState nn1		 获取一个namenode节点的HA状态
sbin/hadoop-daemon.sh start namenode  单独启动一个namenode进程
./hadoop-daemon.sh start zkfc   单独启动一个zkfc进程**

