# (转载)hadoop2.2.0集群的HA高可靠的最简单配置 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年02月28日 16:35:46[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2286








转载网址：http://www.cnblogs.com/meiyuanbao/p/hadoop2.html



# 简介






hadoop中的NameNode好比是人的心脏，非常重要，绝对不可以停止工作。在hadoop1时代，只有一个NameNode。如果该NameNode数据丢失或者不能工作，那么整个集群就不能恢复了。这是hadoop1中的单点问题，也是hadoop1不可靠的表现，如图1所示。hadoop2就解决了这个问题。

![image](http://images.cnitblog.com/blog/346286/201401/250030324762.png)


图1




hadoop2.2.0中HDFS的高可靠指的是可以同时启动2个NameNode。其中一个处于工作状态，另一个处于随时待命状态。这样，当一个NameNode所在的服务器宕机时，可以在数据不丢失的情况下，手工或者自动切换到另一个NameNode提供服务。




这些NameNode之间通过共享数据，保证数据的状态一致。多个NameNode之间共享数据，可以通过Nnetwork File System或者Quorum Journal Node。前者是通过linux共享的文件系统，属于操作系统的配置；后者是hadoop自身的东西，属于软件的配置。



我们这里讲述使用Quorum Journal Node的配置方式，方式是手工切换。




集群启动时，可以同时启动2个NameNode。这些NameNode只有一个是active的，另一个属于standby状态。active状态意味着提供服务，standby状态意味着处于休眠状态，只进行数据同步，时刻准备着提供服务，如图2所示。

![image](http://images.cnitblog.com/blog/346286/201401/250030333350.png)


图2



# 架构




在一个典型的HA集群中，每个NameNode是一台独立的服务器。在任一时刻，只有一个NameNode处于active状态，另一个处于standby状态。其中，active状态的NameNode负责所有的客户端操作，standby状态的NameNode处于从属地位，维护着数据状态，随时准备切换。




两个NameNode为了数据同步，会通过一组称作JournalNodes的独立进程进行相互通信。当active状态的NameNode的命名空间有任何修改时，会告知大部分的JournalNodes进程。standby状态的NameNode有能力读取JNs中的变更信息，并且一直监控edit log的变化，把变化应用于自己的命名空间。standby可以确保在集群出错时，命名空间状态已经完全同步了，如图3所示。

![image](http://images.cnitblog.com/blog/346286/201401/250030342413.png)


图3




为了确保快速切换，standby状态的NameNode有必要知道集群中所有数据块的位置。为了做到这点，所有的datanodes必须配置两个NameNode的地址，发送数据块位置信息和心跳给他们两个。




对于HA集群而言，确保同一时刻只有一个NameNode处于active状态是至关重要的。否则，两个NameNode的数据状态就会产生分歧，可能丢失数据，或者产生错误的结果。为了保证这点，JNs必须确保同一时刻只有一个NameNode可以向自己写数据。



# 硬件资源




为了部署HA集群，应该准备以下事情：


* NameNode服务器：运行NameNode的服务器应该有相同的硬件配置。


* JournalNode服务器：运行的JournalNode进程非常轻量，可以部署在其他的服务器上。注意：必须允许至少3个节点。当然可以运行更多，但是必须是奇数个，如3、5、7、9个等等。当运行N个节点时，系统可以容忍至少(N-1)/2个节点失败而不影响正常运行。




在HA集群中，standby状态的NameNode可以完成checkpoint操作，因此没必要配置Secondary NameNode、CheckpointNode、BackupNode。如果真的配置了，还会报错。

# 配置




HA集群需要使用nameservice ID区分一个HDFS集群。另外，HA中还要使用一个词，叫做NameNode ID。同一个集群中的不同NameNode，使用不同的NameNode ID区分。为了支持所有NameNode使用相同的配置文件，因此在配置参数中，需要把“nameservice ID”作为NameNode ID的前缀。




HA配置内容是在文件hdfs-site.xml中的。下面介绍关键配置项。



dfs.nameservices 命名空间的逻辑名称。如果使用HDFS Federation，可以配置多个命名空间的名称，使用逗号分开即可。
<property>
  <name>dfs.nameservices</name>
  <value>mycluster</value>
</property>

dfs.ha.namenodes.[nameservice ID] 命名空间中所有NameNode的唯一标示名称。可以配置多个，使用逗号分隔。该名称是可以让DataNode知道每个集群的所有NameNode。当前，每个集群最多只能配置两个NameNode。
<property>
  <name>dfs.ha.namenodes.mycluster</name>
  <value>nn1,nn2</value>
</property>

dfs.namenode.rpc-address.[nameservice ID].[name node ID] 每个namenode监听的RPC地址。如下所示
<property>
  <name>dfs.namenode.rpc-address.mycluster.nn1</name>
  <value>machine1.example.com:8020</value>
</property>
<property>
  <name>dfs.namenode.rpc-address.mycluster.nn2</name>
  <value>machine2.example.com:8020</value>
</property>

dfs.namenode.http-address.[nameservice ID].[name node ID] 每个namenode监听的http地址。如下所示
<property>
  <name>dfs.namenode.http-address.mycluster.nn1</name>
  <value>machine1.example.com:50070</value>
</property>
<property>
  <name>dfs.namenode.http-address.mycluster.nn2</name>
  <value>machine2.example.com:50070</value>
</property>

如果启用了安全策略，也应该对每个namenode配置htts-address信息，与此类似。

dfs.namenode.shared.edits.dir 这是NameNode读写JNs组的uri。通过这个uri，NameNodes可以读写edit log内容。URI的格式"qjournal://host1:port1;host2:port2;host3:port3/journalId"。这里的host1、host2、host3指的是Journal Node的地址，这里必须是奇数个，至少3个；其中journalId是集群的唯一标识符，对于多个联邦命名空间，也使用同一个journalId。配置如下
<property>
  <name>dfs.namenode.shared.edits.dir</name>
  <value>qjournal://node1.example.com:8485;node2.example.com:8485;node3.example.com:8485/mycluster</value>
</property>

dfs.client.failover.proxy.provider.[nameservice ID] 这里配置HDFS客户端连接到Active NameNode的一个java类。

<property>
  <name>dfs.client.failover.proxy.provider.mycluster</name>
  <value>org.apache.hadoop.hdfs.server.namenode.ha.ConfiguredFailoverProxyProvider</value>
</property>

dfs.ha.fencing.methods配置active namenode出错时的处理类。当active namenode出错时，一般需要关闭该进程。处理方式可以是ssh也可以是shell。

如果使用ssh，配置如下

<property>
  <name>dfs.ha.fencing.methods</name>
  <value>sshfence</value>
</property>

<property>
  <name>dfs.ha.fencing.ssh.private-key-files</name>
  <value>/home/exampleuser/.ssh/id_rsa</value>
</property>

这种方法配置简单，推荐使用。
fs.defaultFS 客户端连接HDFS时，默认的路径前缀。如果前面配置了nameservice ID的值是mycluster，那么这里可以配置为授权信息的一部分。

可以在core-site.xml中配置如下

<property>
  <name>fs.defaultFS</name>
  <value>hdfs://mycluster</value>
</property>

dfs.journalnode.edits.dir 这是JournalNode进程保持逻辑状态的路径。这是在linux服务器文件的绝对路径。

配置如下

<property>
  <name>dfs.journalnode.edits.dir</name>
  <value>/path/to/journal/node/local/data</value>
</property>
# 部署




以上配置完成后，就可以启动JournalNode进程了。在各个JournalNode机器上执行命令“hadoop-daemon.sh  journalnode”。




如果是一个新的HDFS集群，还要首先执行格式化命令“hdfs  namenode  -format”，紧接着启动本NameNode进程。


如果存在一个已经格式化过的NameNode，并且已经启动了。那么应该把该NameNode的数据同步到另一个没有格式化的NameNode。在未格式化过的NameNode上执行命令“hdfs  namenode  -bootstrapStandby”。


如果是把一个非HA集群转成HA集群，应该运行命令“hdfs –initializeSharedEdits”，这会初始化JournalNode中的数据。




做了这些事情后，就可以启动两个NameNode了。启动成功后，通过web页面观察两个NameNode的状态，都是standby。




下面执行命令“hdfs  haadmin  -failover  --forcefence  serviceId   serviceId2”。就会把NameNode的状态进行安全的切换。其中后面一个会变为active状态。这时候再通过web页面观察就能看到正确结果了。



# 管理员命令




执行命令“hdfs  haadmin”，会显示子命令列表，如下
Usage: DFSHAAdmin [-ns <nameserviceId>]
    [-transitionToActive <serviceId>]
    [-transitionToStandby <serviceId>]
    [-failover [--forcefence] [--forceactive] <serviceId> <serviceId>]
    [-getServiceState <serviceId>]
    [-checkHealth <serviceId>]
    [-help <command>]

如果要查看具体用法，使用“hdfs  haadmin  -help  <command>”。其中，

transitionToActive和transitionToStandby是用于在不同状态之间切换的。这些命令没有任何防护错误，很少使用。

failover 初始化一个故障恢复。该命令会从一个失效的NameNode切换到另一个上面。

getServiceState 获取当前NameNode的状态。

checkHealth 检查NameNode的状态。正常就返回0，否则返回非0值。

以下是我的详细安装过程描述：


1.确定集群结构


我这里采用 5台虚拟机，内存是512MB，硬盘是20GB，以下是我对这5台机器的角色分配。
|ip地址|主机名|NameNode|JournalNode|DataNode|
|----|----|----|----|----|
|192.168.80.100|hadoop100|是|是|否|
|192.168.80.101|hadoop101|是|是|否|
|192.168.80.102|hadoop102|否|是|是|
|192.168.80.103|hadoop103|否|否|是|
|192.168.80.104|hadoop104|否|否|是|




2.设置linux、安装jdk




首先在VMWare中安装一台虚拟机，设置ssh免密码登录、设置静态ip为192.168.80.100、设置主机名为hadoop100、编辑/etc/hosts文件、安装jdk等，这些内容参考作者前面的文章。不再累述。


然后修改hadoop的配置文件，下面重点描述。




2.1 编辑文件$HADOOP_HOME/etc/hadoop/hadoop-env.sh，修改一行内容如下


export JAVA_HOME=/usr/local/jdk


把这里的JAVA_HOME前面的#去掉，把值改为自己安装的jdk路径；




2.2 编辑文件$HADOOP_HOME/etc/hadoop/core-site.xml，修改内容如下所示


<configuration>


<property>


<name>hadoop.tmp.dir</name>


<value>/usr/local/hadoop/tmp</value>


</property>


<property>


<name>fs.default.name</name>


<value>hdfs://hadoop100:9000</value>


</property>


</configuration>


以上配置中，name是hadoop.tmp.dir的值表示hadoop存放数据的目录，即包括NameNode的数据，也包括DataNode的数据。该路径任意指定，只要实际存在该文件夹即可。


name是fs.defaultFS的值表示hdfs路径的逻辑名称。因为我们会启动2个NameNode，每个NameNode的位置不一样，那么切换后，用户也要修改代码，很麻烦，因此使用一个逻辑路径，用户就可以不必担心NameNode切换带来的路径不一致问题了。




2.3 编辑文件$HADOOP_HOME/etc/hadoop/hdfs-site.xml，修改内容如下所示

<configuration>


<property>


<name>dfs.replication</name>


<value>2</value>


</property>


<property>


<name>dfs.nameservices</name>


<value>cluster1</value>


</property>


<property>


<name>dfs.ha.namenodes.cluster1</name>


<value>hadoop100,hadoop101</value>


</property>


<property>


<name>dfs.namenode.rpc-address.cluster1.hadoop100</name>


<value>hadoop100:9000</value>


</property>


<property>


<name>dfs.namenode.rpc-address.cluster1.hadoop101</name>


<value>hadoop101:9000</value>


</property>


<property>


<name>dfs.namenode.http-address.cluster1.hadoop100</name>


<value>hadoop100:50070</value>


</property>


<property>


<name>dfs.namenode.http-address.cluster1.hadoop101</name>


<value>hadoop101:50070</value>


</property>


<property>


<name>dfs.namenode.shared.edits.dir</name>


<value>qjournal://hadoop100:8485;hadoop101:8485;hadoop102:84


85/cluster1</value>


</property>


<property>


<name>dfs.client.failover.proxy.provider.cluster1</name>


<value>org.apache.hadoop.hdfs.server.namenode.ha.ConfiguredF


ailoverProxyProvider</value>


</property>


<property>


<name>dfs.ha.fencing.methods</name>


<value>sshfence</value>


</property>


<property>


<name>dfs.ha.fencing.ssh.private-key-files</name>


<value>/root/.ssh/id_rsa</value>


</property>


<property>


<name>dfs.journalnode.edits.dir</name>


<value>/usr/local/hadoop/tmp/journal</value>


</property>


</configuration>




以上配置信息在前面的描述中已经做了解释。




2.4 编辑文件$HADOOP_HOME/etc/hadoop/slaves，修改内容如下所示


hadoop102


hadoop103


hadoop104


表示以上三个节点作为DataNode节点。




3.使用虚拟机再复制出4个虚拟机。把ip和主机名修改一下，同时修改/etc/hosts文件。在这里要确保5个节点之间互相都可以使用ssh免密码登录。




4.执行命令启动集群


以下命令严格注意执行顺序，不能颠倒！




4.1 启动JournalNode集群


在hadoop100、hadoop101、hadoop102上，执行命令 hadoop-daemon.sh  start  journalnode




4.2 格式化一个NameNode


在hadoop100执行命令 hdfs  namenode  –format




4.3 启动一个NameNode


在hadoop100执行命令  hadoop-daemon.sh  start namenode




4.4 格式化另一个NameNode


在hadoop101执行命令 hdfs namenode  -bootstrapStandby




4.5 启动另一个NameNode


在hadoop101执行命令 hadoop-daemon.sh  start  namenode


这时候，使用浏览器访问 [http://hadoop100:50070](http://hadoop100:50070/) 和 [http://hadoop101:50070](http://hadoop101:50070/) 。如果能够看到两个页面，证明NameNode启动成功了。这时，两个NameNode的状态都是standby。




4.6 转换active


在hadoop100执行命令  hdfs  haadmin  -transitionToActive  hadoop100


再使用浏览器访问 [http://hadoop100:50070](http://hadoop100:50070/) 和 [http://hadoop101:50070](http://hadoop101:50070/)，会发现hadoop100节点变为active，hadoop101还是standby。




4.7 启动DataNodes


在hadoop100执行命令 hadoop-daemons.sh  start datanode 会启动3个DataNode节点。




这时候HA集群就启动了。




你如果想实验一下NameNode切换，执行命令  hdfs  haadmin –failover –forceactive hadoop100 hadoop101


这时候观察hadoop100和hadoop101的状态，就会发现，已经改变了。




如果向上传数据，还需要修改core-site.xml中的fs.default.name的值，改为hdfs://hadoop101:9000 才行。




如果有的同学配置失败，可以使用我的一键运行脚本，hadoop目录是/usr/local/hadoop，一键脚本放在该目录即可。使用root用户登录运行。下载地址是[http://pan.baidu.com/s/1gdHsVmV](http://pan.baidu.com/s/1gdHsVmV)




