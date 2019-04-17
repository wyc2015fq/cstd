# Hadoop平台日志结构 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年05月27日 11:26:32[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2460
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









## 1.Hadoop集群部署

Hadoop集群：

Master:10.18.18.1

Slave1：10.18.18.100

Slave2：10.18.18.101

可互相通信、可连外网，操作系统均为：

#cat/etc/issue

Ubuntu14.04.4 LTS \n \l

# uname -a

Linux XXX3.13.0-66-generic #108-Ubuntu SMP Wed Oct 7 15:20:27 UTC 2015 x86_64x86_64x86_64 GNU/Linux

### 1.1Master部署

1）创建用户hadoop

#sudo addgroup hadoop

#sudoadduser--ingroup hadoop hadoop  //password:hadoop

#vi/etc/sudoers  //在root下面增加一行，如下：

hadoop  ALL=(ALL:ALL) ALL

$ls -l /home   //看到有hadoop文件夹。

$su hadoop//切换到hadoop用户

2）jdk安装

#cd/home/hadoop

#ls–ljdk1.8.0_20 //jdk解压并通过cp–r命令复制到/home/hadoop文件夹下

drwxrwxr-x 2hadoophadoop     4096 May 12 11:39 bin

drwxrwxr-x 3hadoophadoop     4096 May 12 11:39include

drwxrwxr-x 5hadoophadoop     4096 May 12 11:39 jre

配置jdk路径

#vi .bashrc

exportJAVA_HOME=/home/hodoop/jdk1.8.0_20

exportJRE_HOME=$JAVA_HOME/jre

exportPATH=$PATH:$JAVA_HOME/bin:$JRE_HOME/bin

exportCLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar:$JRE_HOME/lib

#source.bashrc

#java –version

java version"1.8.0_20"

Java(TM) SERuntime Environment (build 1.8.0_20-b26)

JavaHotSpot(TM) 64-Bit Server VM (build 25.20-b23, mixed mode)

3）SSH免密码

#sudoapt-get install ssh  //安装ssh

#ls–a //看到.ssh文件夹，没有mkdir .ssh创建

#ssh-keygen-t rsa  //提示输入，直接回车

#ls -a .ssh  //.ssh文件下生成密钥对：私钥id_rsa和公钥id_rsa.pub

.  .. id_rsa  id_rsa.pub

#cat.ssh/id_rsa.pub >> .ssh/authorized_keys //追加到授权key中

4）设置主机名及与IP地址映射关系

#sudo vi/etc/hostname  //主机名修改为Master

或者通过#hostname Master

重启生效

#sudo vi/etc/hosts  //映射IP地址

10.18.18.100    Slave1

10.18.18.101    Slave2

10.18.18.1      Master

5）Hadoop安装

下载hadoop-2.7.2.tar.gz解压到/home/hadoop目录下。

#tar -zxvf hadoop-2.7.2.tar.gz

#ls -lhadoop-2.7.2

drwxr-xr-x 2hadoophadoop  4096 Jan 26 08:20 bin

drwxr-xr-x 3hadoophadoop  4096 Jan 26 08:20 etc

drwxr-xr-x 2hadoophadoop  4096 Jan 26 08:20 include

drwxr-xr-x 3hadoophadoop  4096 Jan 26 08:20 lib

drwxr-xr-x 2hadoophadoop  4096 Jan 26 08:20 libexec

-rw-r--r-- 1hadoophadoop 15429 Jan 26 08:20 LICENSE.txt

-rw-r--r-- 1hadoophadoop   101 Jan 26 08:20NOTICE.txt

-rw-r--r-- 1hadoophadoop  1366 Jan 26 08:20README.txt

drwxr-xr-x 2hadoophadoop  4096 Jan 26 08:20 sbin

drwxr-xr-x 4hadoophadoop  4096 Jan 26 08:20 share

如果文件夹所有者非hadoop用户，要授权，命令如下：

#sudochown-R hadoop:hadoop hadoop-2.7.2

#cdhadoop-2.7.2 //进入hadoop目录下增加

#mkdirhdfs

#mkdirhdfs/name

#mkdirhdfs/data

#mkdirhdfs/tmp

#ls -l hdfs

drwxrwxr-x 2hadoophadoop 4096 May 12 16:54 data

drwxrwxr-x 2hadoophadoop 4096 May 12 16:54 name

drwxrwxr-x 2hadoophadoop 4096 May 12 16:54 tmp

6）Hadoop配置

Set1：配置JDK路径

#vi etc/hadoop/hadoop-env.sh

export JAVA_HOME=/home/hadoop/jdk1.8.0_20

#vi etc/hadoop/yarn-env.sh

export JAVA_HOME=/home/hadoop/jdk1.8.0_20

Set2：配置core-site

#vietc/hadoop/core-site.xml

<property>

<name>fs.default.name</name>

<value>hdfs://Master:9000/</value>

</property>

<property>

<name>hadoop.tmp.dir</name>

<value>/home/hadoop/hadoop-2.7.2/hdfs/tmp/hadoop-${user.name}</value>

</property>

Set3：配置hdfs-site

#vietc/hadoop/hdfs-site.xml

<property>

<name>dfs.namenode.name.dir</name>

<value>/home/hadoop/hadoop-2.7.2/hdfs/name</value>

</property>

<property>

<name>dfs.datanode.data.dir</name>

<value>/home/hadoop/hadoop-2.7.2/hdfs/data</value>

</property>

<property>

<name>dfs.replication</name>

<value>1</value>

</property>

<property>

<name>dfs.namenode.secondary.http-address</name>

<value>Master:9001</value>

</property>

Set4：配置yarn-site

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

<value>Master:8032</value>

</property>

<property>

<name>yarn.resourcemanager.scheduler.address</name>

<value>Master:8030</value>

</property>

<property>

<name>yarn.resourcemanager.resource-tracker.address</name>

<value>Master:8031</value>

</property>

<property>

<name>yarn.resourcemanager.admin.address</name>

<value>Master:8033</value>

</property>

<property>

<name>yarn.resourcemanager.webapp.address</name>

<value>Master:8088</value>

</property>

<property>

<name>yarn.nodemanager.resource.memory-mb</name>

<value>4096</value>

</property>

Set5：配置mapred-site

#vietc/hadoop/mapred-site.xml

<property>

<name>mapreduce.framework.name</name>

<value>yarn</value>

</property>

<property>

<name>mapreduce.jobhistory.address</name>

<value>Master:10020</value>

</property>

<property>

<name>mapreduce.jobhistory.webapp.address</name>

<value>Master:19888</value>

</property>

Set6：配置Slaves

#vietc/hadoop/slaves

加入：Slave1和Slave2作为节点。

### 1.2Slave部署

1）、2）两个步骤和Master步骤一样，其中jdk文件包可通过scp远程复制。如将master的jdk包复制到Slave1下：#scp-r /home/hadoop/jkd1.8.0_20 hadoop@10.18.18.100:/home/hadoop/

3）步骤安装ssh后，复制master节点生成的文件，如：

$ssh-copy-idhadoop@Slave1

复制在Master服务器测试是否可以免密码登录：

$sshhadoop@Slave1

4）步骤和Master一样，主机名分别设置为Slave1、Slave2，并和Master一样建立映射IP地址。

5）和6）步骤直接将Master上配置好的Hadoop包复制到Slave1和Slave2的/home/hadoop目录下。

#scp -r/home/hadoop/hadoop-2.7.2 hadoop@10.18.18.100:/home/hadoop/

### 1.3集群启动

Master服务器进入/home/hadoop目录

1）配置HADOOP_HOME

#vi .bashrc

export HADOOP_HOME=/home/hadoop/hadoop-2.7.2

exportPATH=$PATH:$JAVA_HOME/bin:$JRE_HOME/bin:$HADOOP_HOME/bin

#source.bashrc

2）初始化：

#hdfsnamenode-format

3）启动hdfs集群

进入hadoop-2.7.2目录

#sbin/start-dfs.sh

通过ssh远程连接Master，启动集群出现22端口无法连接。

Master: ssh: connect to host master port 22:Connection refused

#vi /etc/ssh/sshd_config

port配置成22000，增加port端口22

4）启动yarn集群

Master启动resourcemanager

#sbin/yarn-daemon.shstart resourcemanager

Slave启动nodemanager

#sbin/yarn-daemon.shstart nodemanager

5）查看进程

#jps  //master

13616 SecondaryNameNode

13763 Jps

7028ResourceManager

6645NameNode

#jps  //salve

1632 Jps

1285DataNode

1519NodeManager

集群启动和停止也可分别执行sbin/start-all.sh和sbin/stop-all.sh命令。

### 1.4集群监控

集群监控命令：#hadoopdfsadmin-report

也可通过页面监控：http://master:50070

问题：发现datanode已启动，但网页[Live Nodes](http://223.82.251.54:50070/dfshealth.html#tab-datanode)=0和
 DFS=0 B,通过log查了好久，发现master对应的IP地址不是10.18.18.1，推测可能是/etc/hosts的映射出现问题，于是修改/etc/hosts文件，只保留如下四行:

10.18.18.100    Slave1

10.18.18.101    Slave2

10.18.18.1      Master

127.0.0.1       localhost

重新启动集群，可以看到2个节点的信息。

### 1.5作业执行

Master服务器下提交作业。

#hadoopfs-mkdir /data

#hadoopfs-putLICENSE.txt /data

#hadoop jar

/home/hadoop/hadoop-2.7.2/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.7.2.jarwordcount/data /output

#hadoopfs-ls /output

-rw-r--r--   1 hadoopsupergroup          0 2016-05-17 09:49 /output/_SUCCESS

-rw-r--r--   1 hadoopsupergroup       8006 2016-05-17 09:49/output/part-r-00000

#hadoopfs-cat /output/part-r-00000  //看结果

### 1.6Yarn框架

1）要重点理解下yarn框架，梳理master和slave的组件运作关系：

ResourceManager：RM，YARN的中控模块，负责统一规划资源的使用。

NodeManager:NM，YARN的资源结点模块，负责启动管理container。

ApplicationMaster:AM，YARN中每个应用都会启动一个AM，负责向RM申请资源，请求NM启动container，并告诉container执行的任务内容。

Container：资源容器，YARN中的AM在container上运行的， AM的container向RM申请。



当客户向RM提交作业时，由AM负责向RM提出资源申请，和向NM提出task执行。RM负责资源调度，AM 负责任务调度。RM负责整个集群的资源管理与调度；NM负责单个节点的资源管理与调度；NM定时的通过心跳的形式与RM进行通信，报告节点的健康状态与内存使用情况；AM通过与RM交互获取资源，然后通过与NM交互，启动计算任务。

2）在配置yarn-site.xml和mapred-site.xml时要注意不同集群内存资源配置不同的运行参数。

RM内存资源配置项（yarn-site.xml）：

yarn.scheduler.minimum-allocation-mb

yarn.scheduler.maximum-allocation-mb

RM加载的调度器类实例（yarn-site.xml）：

yarn.resourcemanager.scheduler.class

值可选：FairScheduler/CapacityScheduler

NM内存资源配置项（yarn-site.xml）：

yarn.nodemanager.resource.memory-mb

yarn.nodemanager.vmem-pmem-ratio

AM内存配置项（mapred-site.xml）：

mapreduce.map.memory.mb

mapreduce.reduce.memory.mb

JVM参数配置项（mapred-site.xml）：

mapreduce.map.java.opts

mapreduce.reduce.java.opts

## 2.Hive部署

### 2.1元数据库mysql

Master服务器Hadoop用户：

#sudoapt-get install mysql-server

#mysql -uroot –p

mysql>createdatabase hive;

mysql>grantall on hive.* to hive@'%'  identified by'hive';

mysql>flushprivileges;

下载mysql的JDBC驱动包并复制到Hive的lib目录下。

#cp

mysql-connector-java-5.1.39/mysql-connector-java-5.1.39-bin.jar

apache-hive-2.0.0-bin/lib

### 2.2Hive安装配置

1）#tar-zxvfapache-hive-2.0.0-bin.tar.gz  //解压到/home/hadoop

2）配置hive环境变量

#vi .bashrc

exportHIVE_HOME=/home/hadoop/apache-hive-2.0.0-bin

exportPATH=$PATH:$HIVE_HOME/bin

exportCLASSPATH=$CLASSPATH.:HIVE_HOME/lib

#source.bashrc

3）配置conf/hive-env.sh文件

复制hive-env.sh.template并命名为hive-env.sh

#mvconf/hive-env.sh.template conf/hive-env.sh

指定HADOOP_HOME及HIVE_CONF_DIR的路径如下：

#viconf/hive-env.sh

HADOOP_HOME=/home/hadoop/hadoop-2.7.2

exportHIVE_CONF_DIR=/home/hadoop/apache-hive-2.0.0-bin/conf

4）新建conf/hive-site.xml文件

<?xmlversion="1.0"?>

<?xml-stylesheettype="text/xsl"href="configuration.xsl"?>

<configuration>

<property>

<name>javax.jdo.option.ConnectionURL</name>

<value>jdbc:mysql://Master:5306/hive</value>

</property>

<property>

<name>javax.jdo.option.ConnectionDriverName</name>

<value>com.mysql.jdbc.Driver</value>

</property>

<property>

<name>javax.jdo.option.ConnectionUserName</name>

<value>hive</value>

</property>

<property>

<name>javax.jdo.option.ConnectionPassword</name>

<value>hive</value>

</property>

<property>

 <name>hive.metastore.local</name>

 <value>true</value>

 </property>

</configuration>

### 2.3Hive运行

1）初始化：第一次

#schematool-dbTypemysql-initSchema

#schematool-dbTypemysql-info//查看

2）启动Hadoop服务：$sbin/start-all.sh

3）#hive  //运行进入hive

4）查看hive元数据

#mysql -uhive -p

mysql>select*from VERSION;

## 3.日志结构

Hadoop平台的运维需要从日志从寻找蛛丝马迹，本文试着梳理下Hadoop平台的各类日志，包括生态圈内相关组件。

### 3.1Linux日志

Linux日志路径：/var/log，记录日志文件是syslogd服务。

syslog的记录一般包含：事件发生的日期和时间、事件发生的主机名、启动时间的服务名、信息内容。日志记录格式如下：

May 1016:34:09 server-128-15 sshd[638]: pam_unix(sshd:session): session opened foruser root by (uid=0)

syslog的配置文件在/etc/rsyslog.conf，规定不同服务的不同等级信息以及记录输出位置（设备或文件），如下：

# Log allthe mail messages in one place.

mail.*                                                  -/var/log/maillog

如果要汇集Linux日志文件到日志文件服务器，则启动日志文件服务器server的514端口，修改/etc/sysconfig/rsyslog文件的SYSLOGD_OPTIONS。对客户端来说，要指定将信息传送到主机上，假设日志文件服务器IP为：192.168.1.10，可在客户端/etc/rsyslog.conf上增加如下一行：

*.*   @192.168.1.10

重新启动服务器和客户端的syslogd服务，就可以在日志文件服务器上看到每行记录上增加了客户端主机名信息。

### 3.2Hadoop日志

默认日志路径：${HADOOP_HOME}/logs目录，日志的命名均包含用户名称、守护进程名称和本地主机名等信息。

1）Master服务器日志

master中存放以log与out为后缀的两类日志：

log日志文件通过log4j记录的，大部分应用程序的日志消息都写到该日志文件中，故障诊断的首要步骤即为检查该文件；

hadoop-hadoop-namenode-Master.log

hadoop-hadoop-secondarynamenode-Master.log

yarn-hadoop-resourcemanager-Master.log

out日志文件记录标准输出和标准错误日志，由于大多日志均使用log4j输出至log日志文件中，因此文件很小或者为空。系统仅保留最新的5个日志；

hadoop-hadoop-namenode-Master.out

hadoop-hadoop-secondarynamenode-Master.out

yarn-hadoop-resourcemanager-Master.Out

对于SecurityAuth-hadoop.audit日志文件暂不清楚其具体意义。

2）Slave服务器日志

和Master命名规则一致，也是输出log和out为后缀的两类日志：

hadoop-hadoop-datanode-Slave1.log

May 16 16:20hadoop-hadoop-datanode-Slave1.out

同样也有SecurityAuth-hadoop.audit文件。

不同master的是，Slave服务器中还有userlogs文件夹，里面存放着application相关的日志信息。

#ls -llogs/userlogs

drwx--x--- 3hadoophadoop 4096 May 16 15:53 application_1463385140428_0001

drwx--x--- 3hadoophadoop 4096 May 16 14:57 application_1463381663123_0002

drwx--x--- 3hadoophadoop 4096 May 16 14:58 application_1463381663123_0003

3）log日志结构

每行记录基本结构是：日期时间、类型、程序类、信息内容，类型分为：INFO、WARN、ERROR，如下：

2016-05-16 14:13:40,016 INFOorg.apache.hadoop.hdfs.server.namenode.FSNamesystem: fsLock is fair:true

2016-05-13 15:53:41,212 WARNorg.apache.hadoop.hdfs.server.common.Util: Path/home/hadoop/hadoop-2.7.2/hdfs/name should be specified as a URI inconfiguration files. Please update hdfs configuration.

2016-05-13 15:52:51,025 ERRORorg.apache.hadoop.hdfs.server.namenode.NameNode: RECEIVED SIGNAL 15: SIGTERM

4）out日志结构

RM和NM上的基本结构看是：日期时间、组件名、程序类、信息类型、信息内容，如下：

May 17, 2016 9:47:23 

AM 

com.sun.jersey.guice.spi.container.GuiceComponentProviderFactorygetComponentProvider

INFO: 

Bindingorg.apache.hadoop.yarn.server.resourcemanager.webapp.JAXBContextResolver toGuiceManagedComponentProvider with the scope "Singleton"

Namenode和Datanode上貌似是提供程序的资源使用情况，如下：

ulimit -a for user hadoop

core file size          (blocks, -c) 0

dataseg size           (kbytes, -d) unlimited

scheduling priority             (-e) 0

file size               (blocks, -f) unlimited

pending signals                 (-i) 256823

max locked memory       (kbytes, -l) 64

max memory size         (kbytes, -m) unlimited

open files                      (-n) 1024

pipe size            (512 bytes, -p) 8

POSIX message queues     (bytes, -q) 819200

real-time priority              (-r) 0

stack size              (kbytes, -s) 8192

cpu time               (seconds, -t) unlimited

max user processes              (-u) 256823

virtual memory          (kbytes, -v) unlimited

file locks                      (-x) unlimited

### 3.3Hive日志

Hive中的日志分为两种：系统日志，记录了hive的运行情况、错误状况；Job 日志，记录Hive 中job的执行的历史过程。

1）日志存储位置：

在hive/conf/hive-log4j.properties文件中记录Hive日志的默认存储情况，如下：

# list ofproperties

property.hive.log.level= INFO

property.hive.root.logger= DRFA

property.hive.log.dir= ${sys:java.io.tmpdir}/${sys:user.name}

property.hive.log.file= hive.log

到linux的根目录下/tmp/hadoop查看hive.log文件。

2）#more/tmp/hadoop/hive.log

日志记录结构：日期时间、类型、程序类、信息内容，看出类型有WARN、INFO、FATAL三类，如下：

2016-05-17T12:18:30,617 WARN  [main]: conf.HiveConf(HiveConf.java:initialize(3327)) - HiveConf of name hive.metastore.local doesnot exist

2016-05-17T12:19:09,666 INFO  [973807a9-510f-47a2-acec-4b24489b1e31973807a9-510f-47a2-acec-4b24489b1e31 main]: CliDriver (SessionState.java:printInfo(1007))- Time taken: 1.103 seconds

2016-05-17T11:44:21,969 FATAL [main]:conf.Configuration (Configuration.java:loadResource(2644)) - error parsing conffile:/home/hadoop/apache-hive-2.0.0-bin/conf/hive-site.xml



## 4.总结

Hadoop生态平台存在大量日志，在hadoop运维过程中存在很多盲点，需要采集linux系统syslog以及其上hadoop集群生态组件的日志，加以分析辅助运维。



