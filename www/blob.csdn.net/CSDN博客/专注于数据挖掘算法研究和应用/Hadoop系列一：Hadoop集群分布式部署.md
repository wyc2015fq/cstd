# Hadoop系列一：Hadoop集群分布式部署 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年03月09日 15:48:40[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2596
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)

所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、环境准备

VirtualBox虚拟机上分布部署三套Ubuntu15.10操作系统（Linux 64位），命名为Ubuntu_Hadoop（用户名hp）、Ubuntu_C（用户名c）、Ubuntu_PHP（用户名php），并建立通信。

Ubuntu虚拟机之间通信建立：

1）三套Ubuntu虚拟机在Virtual Box的网络设置为内部（Internal）模式；

2）Ubuntu_Hadoop虚拟机IPV4设置为：

   地址：192.168.0.100

   掩码：255.255.255.0

   网关：192.168.0.1

Ubuntu_C虚拟机IPV4设置为：

   地址：192.168.0.101

   掩码：255.255.255.0

   网关：192.168.0.1

Ubuntu_PHP虚拟机IPV4设置为：

   地址：192.168.0.102

   掩码：255.255.255.0

网关：192.168.0.1

Virtual Box网络配置参考：[https://www.douban.com/group/topic/15558388/](https://www.douban.com/group/topic/15558388/)

Ubuntu_Hadoop(主机名为hp_VirualBox)虚拟机为Master节点，Ubuntu_C(主机名为c_VirualBox)和Ubuntu_PHP(主机名为php_VirualBox)作为Slave节点。Master机器主要配置NameNode和JobTracker的角色，负责总管分布式数据和分解任务的执行；2个Salve机器配置DataNode和TaskTracker的角色，负责分布式数据存储以及任务的执行。

3）统一用户设置

Ubuntu_C和Ubuntu_PHP新建用户名hp并赋予root权限，和Ubuntu_Hadoop一致，用户SSH认证。

$sudo adduserhp

$sudo gedit/etc/sudoers   修改hpALL=(ALL) ALL

~~4）网络配置：~~

~~设置主机名：$sudogedit /etc/hostname~~

~~Ubuntu_Hadoop虚拟机主机名为master~~

~~Ubuntu_C虚拟机主机名为slave1~~

~~Ubuntu_PHP虚拟机主机名为slave2~~

~~设置三机器的hosts：$sudo gedit /etc/hosts~~

~~删除hosts文件内容，同样追加如下内容：~~

~~192.168.0.100master~~

~~192.168.0.101slave1~~

~~192.168.0.102slave2~~

~~设置三机器networks：$sudo gedit /etc /network~~

~~每台机器按照各自网络地址和主机名修改红色部分内容~~

2、JDK安装

官网下载并解压，Master和Slave节点都安装home/hp路径下。

路径配置见：[http://blog.csdn.net/fjssharpsword/article/details/50352231](http://blog.csdn.net/fjssharpsword/article/details/50352231)

3、SSH安装和配置

1）SSH安装

Hadoop控制脚本（并非守护进程）依赖SSH来执行针对整个集群的操作。例如，某个脚本能够终止并重启集群中的所有守护进程。

Master和Slave节点都安装：

$apt-getinstall ssh 安装SSH协议；

$apt-getinstall rsync （rsync是一个远程数据同步工具，可通过LAN/WAN快速同步多台主机间的文件）；

2）SSH无密码验证配置：

为支持无缝式工作，SSH允许用户无需键入密码即可登录集群内及其，创建一个公钥/私钥对，整个集群共享。SSH无密码原理是： Master（NameNode | JobTracker）作为客户端，生成一个密钥对，包括一个公钥和一个私钥，而后将公钥复制到所有的Slave上，Master客户端通过无密码公钥认证连接到服务器Salve（DataNode | Tasktracker）。

当Master通过 SSH连接Salve时，Salve就会生成一个随机数并用Master的公钥对随机数进行加密，并发送给Master，Maste收到加密数之后再用私钥解密，并将解密数回传给Slave，Slave确认解密数无误之后就允许Master连接。

Ø  Master节点生成无密码密钥对：

$ssh-keygen –t rsa –f/home/hp/.ssh/id_rsa

命令执行后/home/hp/.ssh/路径下生成密钥对：私钥id_rsa和公钥id_rsa.pub。

/home/hp/命令下执行

$ls–a | grep.ssh  查看生成.ssh文件夹

$cd .ssh 进入文件夹

$ls 可以看到生成的密文件。

Ø  复制公钥到两台Slave机器

如果hp用户的home目录是在NFS系统中，通过Master节点把公钥id_rsa.pub追加到授权key中，使整个集群内共享密钥。

$cat/home/hp/.ssh/id_rsa.pub>>/home/hp/.ssh/authorized_keys

验证是否成功：ssh localhost

如果home目录并没有通过NFS共享，则采用其他方法共享公钥，如ssh-copy-id、scp等命令，本环境没有挂载NFS文件系统功能，通过以下复制公钥到Slave机器。

 Master节点执行：

$ssh-copy-id[hp@192.168.0.102](mailto:hp@192.168.0.102) 复制到Slave机器Ubuntu_PHP

测试：$ssh hp@192.168.0.102

#ssh-copy-id[hp@192.168.0.101](mailto:hp@192.168.0.101) 复制到Slave机器Ubuntu_C

测试：$ssh hp@192.168.0.101

4、HDFS集群部署

Hadoop集群分成两大类角色：Master和Salve。一个HDFS集群是由一个NameNode和若干个DataNode组成的。其中NameNode作为主服务器，管理文件系统的命名空间和客户端对文件系统的访问操作；集群中的DataNode管理存储的数据。MapReduce框架是由一个单独运行在主节点上的JobTracker和运行在每个集群从节点的TaskTracker共同组成的。主节点负责调度构成一个作业的所有任务，这些任务分布在不同的从节点上。主节点监控它们的执行情况，并且重新执行之前的失败任务；从节点仅负责由主节点指派的任务。当一个Job被提交时，JobTracker接收到提交作业和配置信息之后，就会将配置信息等分发给从节点，同时调度任务并监控TaskTracker的执行。

1）Master服务器安装

官网下载hadoop-2.7.1.tar.gz包。

$cd /home/hp

$tar xzf hadoop-2.7.1.tar.gz

$sudo chown–Rhp:hphadoop   解压出的文件夹hadoop拥有者改为hp用户和组。

2）Master服务器配置

   core-site.xml和hdfs-site.xml是站在 HDFS角度上配置文件，其中core-site.xml配置的是HDFS的地址和端口号。yarn-site.xml和mapred-site.xml是站在MapReduce角度上配置文件，其中mapred-site.xml配置的是JobTracker的地址和端口。

1）创建数据存放的文件夹，tmp、hdfs、hdfs/data、hdfs/name

$mkdirtmp

$mkdirhdfs

$mkdirhdfs/data

$mkdirhdfs/name

2）配置/home/hp/hadoop/etc/hadoop/core-site.xm

$gedit etc/hadoop/core-site.xml

<configuration>

<property>

       <name>fs.defaultFS</name>

       <value>hdfs://192.168.0.100:9000</value>

   </property>

   <property>

       <name>hadoop.tmp.dir</name>

       <value>/home/hp/hadoop/tmp</value>

   </property>

   <property>

       <name>io.file.buffer.size</name>

       <value>131072</value>

</property>

</configuration>

3）配置/home/hp/hadoop/etc/hadoop/hdfs-site.xm;

$gedit etc/hadoop/hdfs-site.xml

<configuration>

<property>

       <name>dfs.namenode.name.dir</name>

       <value>/home/hp/hadoop/dfs/name</value>

</property>

<property>

       <name>dfs.datanode.data.dir</name>

       <value>/home/hp/hadoop/dfs/data</value>

   </property>

<property>

       <name>dfs.replication</name>

       <value>2</value>

</property>

<property>

      <name>dfs.namenode.secondary.http-address</name>

       <value>192.168.0.100:9001</value>

</property>

<property>

<name>dfs.webhdfs.enabled</name>

<value>true</value>

</property>

</configuration>

4）配置/home/hp/hadoop/etc/hadoop/mapred-site.xm;

$geditetc/hadoop/mapred-site.xml

<configuration>

<property>

       <name>mapreduce.framework.name</name>

       <value>yarn</value>

   </property>

   <property>

       <name>mapreduce.jobhistory.address</name>

       <value>192.168.0.100:10020</value>

   </property>

   <property>

      <name>mapreduce.jobhistory.webapp.address</name>

       <value>192.168.0.100:19888</value>

   </property>

</configuration>

5）配置/home/hp/hadoop/etc/hadoop/yarn-site.xm;

$geditetc/hadoop/yarn-site.xml

<configuration>

<property>

       <name>yarn.nodemanager.aux-services</name>

       <value>mapreduce_shuffle</value>

   </property>

   <property>

       <name>yarn.nodemanager.auxservices.mapreduce.shuffle.class</name>

      <value>org.apache.hadoop.mapred.ShuffleHandler</value>

   </property>

   <property>

       <name>yarn.resourcemanager.address</name>

       <value>192.168.0.100:8032</value>

   </property>

   <property>

      <name>yarn.resourcemanager.scheduler.address</name>

       <value>192.168.0.100:8030</value>

   </property>

   <property>

      <name>yarn.resourcemanager.resource-tracker.address</name>

       <value>192.168.0.100:8031</value>

   </property>

   <property>

      <name>yarn.resourcemanager.admin.address</name>

       <value>192.168.0.100:8033</value>

   </property>

   <property>

      <name>yarn.resourcemanager.webapp.address</name>

       <value>192.168.0.100:8088</value>

   </property>

   <property>

      <name>yarn.nodemanager.resource.memory-mb</name>

       <value>768</value>

   </property>

</configuration>

6）配置/home/hp/hadoop/etc/hadoop/下hadoop-env.sh和yarn-env.sh的JAVA_HOME路径

exportJAVA_HOME=/home/hp/jdk

7）配置/home/hp/hadoop/etc/hadoop/下slaves文件

slaves文件中加入192.168.0.101/192.168.0.102

8）Slave服务器Ubuntu_PHP和Ubuntu_C配置

将hadoop本地文件拷贝到两台Slave机器：

$ scp –r /home/hp/hadoophp@192.168.0.101:/home/hp

$ scp –r /home/hp/hadoophp@192.168.0.102:/home/hp

两台Slave机器如果JDK路径不一致，则要修改hadoop-env.sh和yarn-env.sh的Java路径，并授权JDK文件夹权限给hp用户。

$sudo chmod –R777 /home/c/jdk    Ubuntu_C机器的JDK路径

$sudo chmod –R777 /home/php/jdk    Ubuntu_php机器的JDK路径

9）Master服务器进入Hadoop目录下执行$ bin/hdfs namenode –format 初始化

启动在sbin目录下执行$sbin/start-all.sh 

可以使用jps查看信息

停止执行$sbin/stop-all.sh

5、总结上，主要是JDK、SSH、Hadoop配置三步，先在Master都部署好，整个操作系统直接复制Slave最佳。



