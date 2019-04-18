# HDFS HA介绍及配置理解 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年02月28日 16:27:04[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：8281
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









## 1、HDFS HA介绍

相比于Hadoop1.0，Hadoop 2.0中的HDFS增加了两个重大特性，HA和Federaion。HA即为High Availability，用于解决NameNode单点故障问题，该特性通过热备的方式为主NameNode提供一个备用者，一旦主NameNode出现故障，可以迅速切换至备NameNode，从而实现不间断对外提供服务。Federation即为“联邦”，该特性允许一个HDFS集群中存在多个NameNode同时对外提供服务，这些NameNode分管一部分目录（水平切分），彼此之间相互隔离，但共享底层的DataNode存储资源。

在一个典型的HDFS HA场景中，通常由两个NameNode组成，一个处于active状态，另一个处于standby状态。Active NameNode对外提供服务，比如处理来自客户端的RPC请求，而Standby NameNode则不对外提供服务，仅同步active namenode的状态，以便能够在它失败时快速进行切换。

为了能够实时同步Active和Standby两个NameNode的元数据信息（实际上editlog），需提供一个共享存储系统，可以是NFS、QJM（Quorum Journal Manager）或者Bookeeper，Active Namenode将数据写入共享存储系统，而Standby监听该系统，一旦发现有新数据写入，则读取这些数据，并加载到自己内存中，以保证自己内存状态与Active NameNode保持基本一致，如此这般，在紧急情况下standby便可快速切为active
 namenode。

Hadoop0.23.2版本之前, NameNode是HDFS集群的单点故障点，每一个集群只有一个NameNode,如果这个机器或进程不可用，整个集群就无法使用，直到重启NameNode或者新启动一个NameNode节点。影响HDFS集群不可用主要包括以下两种情况：

1）第一种情况是如机器宕机这样的意外情况，将导致集群不可用，只有在重启NameNode之后才可使用。

2）第二种情况是计划内的软件或硬件升级(NameNode节点)，将导致集群在短时间范围内不可用。



HDFS的HA就是为了解决上述问题，通过提供选择运行在同一集群中的一个热备用的“主/备”两个冗余NameNodes。这允许在机器宕机或[系统](http://www.2cto.com/os/)维护的时候，快速的转移到另一个NameNode.

一个典型的HA集群，两个单独的机器配置为NameNodes，在任何时候，一个NameNode处于活动状态，另一个处于待机状态，活动的NameNode负责处理集群中所有客户端的操作，待机时仅仅作为一个slave，保持足够的状态，如果有必要提供一个快速的故障转移.

为了保持备用节点与活动节点状态的同步，目前的实现需要两个节点同时访问一个共享存储设备（例如从NASNFS挂载）到一个目录。将有可能在未来的版本中放宽此限制。当活动节点对命名空间进行任何修改，它将把修改记录写到共享目录下的一个日志文件，备用节点会监听这个目录，当发现更改时，它会把修改内容同步到自己的命名空间。备用节点在故障转移时，它将保证已经读取了所有共享目录内的更改记录，保证在发生故障前的状态与活动节点保持完全一致。

为了提供快速的故障转移，必须保证备用节点有最新的集群中块的位置信息，为了达到这一点，Datanode节点需要配置两个nameNode的位置，同时发送块的位置信息和心跳信息到两个nameNode。

任何时候只有一个namenode处于活动状态，对于HA集群的操作是至关重要的，否则两个节点之间的状态就会产生冲突，数据丢失或其它不正确的结果，为了达到这个目的或者所谓的“裂脑场景”出现，管理员必须为共享存储配置至少一个（fencing）方法。在宕机期间，如果不能确定之间的活动节点已经放弃活动状态，fencing进程负责中断以前的活动节点编辑存储的共享访问。这可以防止任何进一步的修改命名空间，允许新的活动节点安全地进行故障转移。

注:目前，只有手动故障转移支持。这就意味着HA nameNode不能自动检测活动nameNode的失败，而是通过手动启动故障转移。自动故障检测和故障转移将在未来的版本中实现。

## 2、HA部署硬件资源

为了部署一个HA集群环境，您需要准备以下资源：

1）NameNode 
机器：运行活动节点和备用节点的机器和非HA环境的机器需要有同相的硬件配置。

2）共享存储：需要有一个主备namenode节点都是可读写的共享目录，通常情况，这是一个远程的文件管理器，它支持使用NFS挂载到每个namenode节点。目前只支持一个可编辑目录。因此，系统的是否可用将受限于共享目录是否可用，因此，为了消除所有单点故障，需要对共享目录再加冗余，具体来说，多个网络路径的存储需要实现存储系统自身的冗余。因为这个原因，建议共享存储服务器是一个高品质的专用NAS设备，而不是一个简单的[Linux](http://www.2cto.com/os/linux/)服务器。、

注：在HA集群环境里，备用的namenode还要执行检测命名空间的状态，因此，没有必要再运行Secondary NameNode,CheckpointNode,BackupNode。事实上，这样做将会报错，这也允许在从非集群环境到集群环境的重新配置时，重新利用之前的Secondary NameNode的硬件资源。

## 3、HA部署配置简介

类似联邦配置，HA配置向后兼容，允许在不改变当前单节点的情况下配置成集群环境，新的配置方案确保了在集群环境中的所有节点的配置文件都是相同的，没有必要因为节点的不同而配置不同的文件。

和联邦配置一样，HA集群环境重复使用名称服务ID来确定一个单一的HDFS实例，实际上可能包括多个HA namenodes.此外，一个新的namenode增加到HA，集群中的每一个nameNode都有一个不同的ID来标识它，为了支持所有namenode有同一个配置文件，所有的配置参数都以命名服务ID和nameNodeID为后缀。配置HA nameNodes,您需要增加一些配置选项到 hdfs-site.xml 配置文件。

配置选项的顺序不重要，但dfs.federation.nameservices和dfs.ha.namenodes.[nameserviceID]的值将决定下面配置的Key值。因此，在配置其它选项前，需要确定这两个选项的值。

1）dfs.federation.nameservices一个新的名称服务的逻辑名称。为名称服务选择一个逻辑名称，如“mycluster”,使用这个逻辑名称作为这个配置项的值。这个名称可以是随意的，它将用于配置和集群环境中HDFS绝对路径的认证[组件](http://www.2cto.com/kf/all/zujian/)。

注：如果您还使用HDFS联邦，这个配置项应该包括其它的名称服务列表，HA或者其它，用逗号进行分隔。

<property>

<name>dfs.federation.nameservices</name>

 <value>mycluster</value>

</property>

2）dfs.ha.namenodes.[nameservice ID]在名称服务中每一个nameNode的唯一标识符配置一个逗号分隔的NameNode的ID的列表，DataNode会用它来确定在集群中的所有namenode.如我们前面使用"mycluster"作为我们名称服务的ID,你如果使用nn1和nn2作为namenode的ID，你应该这样配置：

<property>

<name>dfs.ha.namenodes.mycluster</name>

<value>nn1,nn2</value>

</property>

注：当前一个名称服务最多只允许配置两个namenode。

3）dfs.namenode.rpc-address.[nameservice ID].[name node ID]每一个namenode监听的标准RPC地址对于前面配置的两个namenode的ID,设置namenode节点详细的地址和端口，注意，这个配置项将在两个单独的配置项里配置，如：

<property>

<name>dfs.namenode.rpc-address.mycluster.nn1</name>

<value>machine1.example.com:8020</value>

</property>

<property>

 <name>dfs.namenode.rpc-address.mycluster.nn2</name>

<value>machine2.example.com:8020</value>

</property>

注：如果你愿意，你可以配置相同的RPC地址。

4）dfs.namenode.http-address.[nameservice ID].[namenode ID]每一个namenode监听的标准HTTP地址和RPC地址一样，设置两个namenode监听的http地址，如：

<property>

<name>dfs.namenode.http-address.mycluster.nn1</name>

<value>machine1.example.com:50070</value>

</property>

<property>

<name>dfs.namenode.http-address.mycluster.nn2</name>

<value>machine2.example.com:50070</value>

</property>

注：如果你启用了hadoop的安全功能，你也可以同样设置成https地址。

5）dfs.namenode.shared.edits.dir共享存储目录的位置这是配置备份节点需要随时保持同步活动节点所作更改的远程共享目录，你只能配置一个目录，这个目录挂载到两个namenode上都必须是可读写的，且必须是绝对路径。如：

<property>

<name>dfs.namenode.shared.edits.dir</name>

<value>file:///mnt/filer1/dfs/ha-name-dir-shared</value>

</property>

6）dfs.client.failover.proxy.provider.[nameserviceID]HDFS客户端用来和活动的namenode节目联系的java类配置的java类是用来给HDFS客户端判断哪个namenode节点是活动的，当前是哪个namenode处理客户端的请求，当前hadoop唯一的实现类是ConfiguredFailoverProxyProvider，除非你自己定义了一个类，否则都将使用这个类。如：

<property>

<name>dfs.client.failover.proxy.provider.mycluster</name>

<value>org.apache.hadoop.hdfs.server.namenode.ha.ConfiguredFailoverProxyProvider</value>

</property>

7）dfs.ha.fencing.methods将用于停止活动NameNode节点的故障转移期间的脚本或[Java](http://www.2cto.com/kf/ware/Java/)类的列表任何时候只有一个namenode处于活动状态，对于HA集群的操作是至关重要的，因此，在故障转移期间，在启动备份节点前，我们首先要确保活动节点处于等待状态，或者进程被中止，为了达到这个目的，您至少要配置一个强行中止的方法，或者回车分隔的列表，这是为了一个一个的尝试中止，直到其中一个返回成功，表明活动节点已停止。hadoop提供了两个方法：shell和sshfence，要实现您自己的方法，请看org.apache.hadoop.ha.NodeFencer类。

sshfence 通过ssh连接活动namenode节点，杀掉进程。为了实现SSH登录杀掉进程，还需要配置免密码登录的SSH密匙信息，如下所示：

<property>

<name>dfs.ha.fencing.methods</name>

<value>sshfence</value>

</property>

<property>

<name>dfs.ha.fencing.ssh.private-key-files</name>

<value>/home/exampleuser/.ssh/id_rsa</value>

</property>

另外，也可以配置一个用户名和SSH端口。也可以为SSH设定一个超时，以毫秒为单位，它可以像这样配置：

<property>

<name>dfs.ha.fencing.methods</name>

<value>sshfence([[username][:port]])</value>

</property>

<property>

<name>dfs.ha.fencing.ssh.connect-timeout</name>

<value>34</value>

</property>

shell -执行任意的shell命令来终止活动namenode节点，配置如下:

<property>

<name>dfs.ha.fencing.methods</name>

<value>shell(/path/to/my/script.sharg1arg2 ...)</value>

</property>

shell脚本将会运行在包括所有hadoop配置参数变量的环境中，只需将配置key的.换成_即可。如dfs_namenode_rpc-address。除此之外，还包括以下的变量：

$target_host需要中止的服务器hostname

$target_port需要中止的服务器端口

$target_address上面两个参数的组合

$target_nameserviceid需要中止的名称服务ID

$target_namenodeid

需要中止的namenode的ID

这些变量也可以直接在shell脚本中使用，如：

<property>

<name>dfs.ha.fencing.methods</name>

<value>shell(/path/to/my/script.sh--nameservice=$target_nameserviceid$target_host:$target_port)</value>

</property>

如果脚本返回0表示中止成功，返回其它值表示失败，同时将尝试列表中的其它中止方法。

注：这个方法没有实现超时设置，所有超时设置都取决于脚本自身。

8）fs.defaultFS当FS客户端没有设置时的默认路径的前缀。另外，您可以配置为hadoop客户端配置HA集群的URI作为默认路径。如果你使用"mycluster"作为名称服务ID，那么在core-site.xml文件中将配置为：

<property>

<name>fs.defaultFS</name>

<value>hdfs://mycluster</value>

</property>



**4、HA部署详情**

所有配置完成后，最初必须同步两个HAnamenode磁盘上的元数据。如果您是全新安装一个HDFS集群，那么您需要在其中一个namenode运行格式化命令（hdfsnamenode -format），如果你已经格式化过namenode或者是从一个非HA环境转换到HA环境，那么你需要使用scp或类似的命令把namenode上的元数据目录复制到另一个namenode。通过配置 dfs.namenode.name.dir 和dfs.namenode.edits.dir两个选项到包含namenode元数据目录的位置。在这个时候，你必须保证之前配置的共享目录包括在您namenode元数据目录下的最近的编辑文件信息。然后，您可以和平时启动namenode一样，启动两个HA
 namenode。

您可以通过配置的HTTP web地址访问两个namenode,你会看到两个HAnamenode当前的状态（主/备），当namenode启动的时候，它的最被状态都是“备”状态。

**5、HA管理命令**

现在完成了配置和启动，你可以执行其它的命令来管理你的HA集群。具体来说，你应该熟悉所有hdfs haadmin下的所有命令。不加任何参数的情况下，将显示如下信息：

Usage:DFSHAAdmin [-ns<nameserviceId>]

[-transitionToActive<serviceId>]

 [-transitionToStandby<serviceId>]

 [-failover [--forcefence][--forceactive]<serviceId> <serviceId>]

 [-getServiceState<serviceId>]

 [-checkHealth<serviceId>]

 [-help <command>]

本指南将介绍这此子命令的高级别用法，每个命令的详细使用帮助，您可以运行如下命令来查看：

hdfs haadmin -help<command> transitionToActive和transitionToStandby就是把指定的namenode转换到主/备状态。

注：这个命令把指定的namenode转换到主/备状态，但这个命令不会试图去停止活动节点，所以，尽量少使用它，可以使用hdfs haadmin -failover来代替。

failover- 在两个namenode之间做故障转移。此命令将会把故障从第一个转移到第二个namenode，如果第一个namenode是“备”状态，这个命令会成功的将第二个namenode转换到“主”状态，如果第一个namenode是“主”状态，它将会尝试把它转换到“备”状态，它会使用在之前配置的 dfs.ha.fencing.methods 所有方法列表中从第一个开始，只到成功为止，它才会把第二个namenode转换成“主”状态。如果所有方法都不能把第一个namenode转换到“备”状态，那么第二个namenode也不能转换成“主”状态，它将返回一个错误信息。

getServiceState- 获取指定namenode的状态。连接到指定的namenode去确定它当前的状态，并打印出出它的状态（主或备）。根据namenode不同的状态，这个命令会使用cron作业或监控脚本来执行。

checkHealth- 检查指定namenode的健康状态。连接到指定的namenode去检查它的健康状态，namenode能够对自身进行一些诊断，包括检查内部服务是否按预期运行。如果namenode运行正常，它将返回0，否则返回非0的值。这个命令的唯一用途就是进行监测。

注: 当前这个命令还没有实现, 除非namenode完成宕机,否则它总是返回成功.



