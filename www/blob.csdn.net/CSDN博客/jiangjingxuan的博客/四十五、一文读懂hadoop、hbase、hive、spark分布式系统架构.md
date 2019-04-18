# 四十五、一文读懂hadoop、hbase、hive、spark分布式系统架构 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:59:50[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：1079












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_272_default_big.png)



机器学习、数据挖掘等各种大数据处理都离不开各种开源分布式系统，hadoop用于分布式存储和map-reduce计算，spark用于分布式机器学习，hive是分布式数据库，hbase是分布式kv系统，看似互不相关的他们却都是基于相同的hdfs存储和yarn资源管理，本文通过全套部署方法来让大家深入系统内部以充分理解分布式系统架构和他们之间的关系

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址。本文较长，精华在最后

## 本文结构

首先，我们来分别部署一套hadoop、hbase、hive、spark，在讲解部署方法过程中会特殊说明一些重要配置，以及一些架构图以帮我们理解，目的是为后面讲解系统架构和关系打基础。

之后，我们会通过运行一些程序来分析一下这些系统的功能

最后，我们会总结这些系统之间的关系



## 分布式hadoop部署

首先，在[http://hadoop.apache.org/releases.html](http://hadoop.apache.org/releases.html)找到最新稳定版tar包，我选择的是

[**http://apache.fayea.com/hadoop/common/hadoop-2.7.2/hadoop-2.7.2.tar.gz**](http://apache.fayea.com/hadoop/common/hadoop-2.7.2/hadoop-2.7.2.tar.gz)

**下载到/data/apache并解压**

**在真正部署之前，我们先了解一下hadoop的架构**

**hadoop分为几大部分：yarn负责资源和任务管理、hdfs负责分布式存储、map-reduce负责分布式计算**

先来了解一下yarn的架构：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/70d3ca2d9cd45cedcae117cf3b18589b6a37f1ac.png)

yarn的两个部分：资源管理、任务调度。

资源管理需要一个全局的ResourceManager(RM)和分布在每台机器上的NodeManager协同工作，RM负责资源的仲裁，NodeManager负责每个节点的资源监控、状态汇报和Container的管理

任务调度也需要ResourceManager负责任务的接受和调度，在任务调度中，在Container中启动的ApplicationMaster(AM)负责这个任务的管理，当任务需要资源时，会向RM申请，分配到的Container用来起任务，然后AM和这些Container做通信，AM和具体执行的任务都是在Container中执行的

yarn区别于第一代hadoop的部署(namenode、jobtracker、tasktracker)

然后再看一下hdfs的架构：hdfs部分由NameNode、SecondaryNameNode和DataNode组成。DataNode是真正的在每个存储节点上管理数据的模块，NameNode是对全局数据的名字信息做管理的模块，SecondaryNameNode是它的从节点，以防挂掉。

最后再说map-reduce：Map-reduce依赖于yarn和hdfs，另外还有一个JobHistoryServer用来看任务运行历史

hadoop虽然有多个模块分别部署，但是所需要的程序都在同一个tar包中，所以不同模块用到的配置文件都在一起，让我们来看几个最重要的配置文件：

各种默认配置：core-default.xml, hdfs-default.xml, yarn-default.xml, mapred-default.xml

各种web页面配置：core-site.xml, hdfs-site.xml, yarn-site.xml, mapred-site.xml

从这些配置文件也可以看出hadoop的几大部分是分开配置的。

除上面这些之外还有一些重要的配置：hadoop-env.sh、mapred-env.sh、yarn-env.sh，他们用来配置程序运行时的java虚拟机参数以及一些二进制、配置、日志等的目录配置

下面我们真正的来修改必须修改的配置文件。

修改etc/hadoop/core-site.xml，把配置改成：

```xml
<configuration>
    <property>
        <name>fs.defaultFS</name>
        <value>hdfs://127.0.0.1:8000</value>
    </property>
    <property>
        <name>io.file.buffer.size</name>
        <value>131072</value>
    </property>
</configuration>
```

这里面配置的是hdfs的文件系统地址：本机的9001端口

修改etc/hadoop/hdfs-site.xml，把配置改成：

```xml
<configuration>
    <property>
        <name>dfs.namenode.name.dir</name>
        <value>file:/data/apache/dfs/name</value>
    </property>
    <property>
        <name>dfs.datanode.data.dir</name>
        <value>file:/data/apache/dfs/data</value>
    </property>
    <property>
        <name>dfs.datanode.fsdataset.volume.choosing.policy</name>
        <value>org.apache.hadoop.hdfs.server.datanode.fsdataset.AvailableSpaceVolumeChoosingPolicy</value>
    </property>
    <property>
        <name>dfs.namenode.http-address</name>
        <value>127.0.0.1:50070</value>
    </property>
    <property>
        <name>dfs.namenode.secondary.http-address</name>
        <value>127.0.0.1:8001</value>
    </property>
</configuration>
```

这里面配置的是hdfs文件存储在本地的哪里以及secondary namenode的地址

修改etc/hadoop/yarn-site.xml，把配置改成：

```xml
<configuration>
    <property>
        <name>yarn.resourcemanager.hostname</name>
        <value>127.0.0.1</value>
    </property>
    <property>
        <name>yarn.resourcemanager.webapp.address</name>
        <value>127.0.0.1:8088</value>
    </property>
    <property>
        <name>yarn.nodemanager.aux-services</name>
        <value>mapreduce_shuffle</value>
    </property>
    <property>
        <name>yarn.nodemanager.aux-services.mapreduce.shuffle.class</name>
        <value>org.apache.hadoop.mapred.ShuffleHandler</value>
    </property>
    <property>
        <name>yarn.log-aggregation-enable</name>
        <value>true</value>
    </property>
    <property>
        <name>yarn.log-aggregation.retain-seconds</name>
        <value>864000</value>
    </property>
    <property>
        <name>yarn.log-aggregation.retain-check-interval-seconds</name>
        <value>86400</value>
    </property>
    <property>
        <name>yarn.nodemanager.remote-app-log-dir</name>
        <value>/YarnApp/Logs</value>
    </property>
    <property>
        <name>yarn.log.server.url</name>
        <value>http://127.0.0.1:19888/jobhistory/logs/</value>
    </property>
    <property>
        <name>yarn.nodemanager.local-dirs</name>
        <value>/data/apache/tmp/</value>
    </property>
    <property>
        <name>yarn.scheduler.maximum-allocation-mb</name>
        <value>5000</value>
    </property>
    <property>
        <name>yarn.scheduler.minimum-allocation-mb</name>
        <value>1024</value>
    </property>
    <property>
        <name>yarn.nodemanager.vmem-pmem-ratio</name>
        <value>4.1</value>
    </property>
    <property>
        <name>yarn.nodemanager.vmem-check-enabled</name>
        <value>false</value>
    </property>
</configuration>
```

这里面配置的是yarn的日志地址以及一些参数配置

通过cp etc/hadoop/mapred-site.xml.template etc/hadoop/mapred-site.xml创建etc/hadoop/mapred-site.xml，内容改为如下：

```xml
<configuration>
    <property>
        <name>mapreduce.framework.name</name>
        <value>yarn</value>
        <description>Execution framework set to Hadoop YARN.</description>
    </property>
    <property>
        <name>yarn.app.mapreduce.am.staging-dir</name>
        <value>/tmp/hadoop-yarn/staging</value>
    </property>
    <property>
        <name>mapreduce.jobhistory.address</name>
        <value>127.0.0.1:10020</value>
    </property>
    <property>
        <name>mapreduce.jobhistory.webapp.address</name>
        <value>127.0.0.1:19888</value>
    </property>
    <property>
        <name>mapreduce.jobhistory.done-dir</name>
        <value>${yarn.app.mapreduce.am.staging-dir}/history/done</value>
    </property>

    <property>
        <name>mapreduce.jobhistory.intermediate-done-dir</name>
        <value>${yarn.app.mapreduce.am.staging-dir}/history/done_intermediate</value>
    </property>
    <property>
        <name>mapreduce.jobhistory.joblist.cache.size</name>
        <value>1000</value>
    </property>

    <property>
        <name>mapreduce.tasktracker.map.tasks.maximum</name>
        <value>8</value>
    </property>
    <property>
        <name>mapreduce.tasktracker.reduce.tasks.maximum</name>
        <value>8</value>
    </property>
    <property>
        <name>mapreduce.jobtracker.maxtasks.perjob</name>
        <value>5</value>
        <description>The maximum number of tasks for a single job.
            A value of -1 indicates that there is no maximum.
        </description>
    </property>
</configuration>
```

这里面配置的是mapred的任务历史相关配置

如果你的hadoop部署在多台机器，那么需要修改etc/hadoop/slaves，把其他slave机器ip加到里面，如果只部署在这一台，那么就留一个localhost即可

下面我们启动hadoop，启动之前我们配置好必要的环境变量：

```bash
export JAVA_HOME="你的java安装地址"
```

先启动hdfs，在此之前要格式化分布式文件系统，执行：
`./bin/hdfs namenode -format myclustername`
如果格式化正常可以看到/data/apache/dfs下生成了name目录

然后启动namenode，执行：
`./sbin/hadoop-daemon.sh --script hdfs start namenode`
如果正常启动，可以看到启动了相应的进程，并且logs目录下生成了相应的日志

然后启动datanode，执行：
`./sbin/hadoop-daemon.sh --script hdfs start datanode`
如果考虑启动secondary namenode，可以用同样的方法启动

下面我们启动yarn，先启动resourcemanager，执行：
`./sbin/yarn-daemon.sh start resourcemanager`
如果正常启动，可以看到启动了相应的进程，并且logs目录下生成了相应的日志

然后启动nodemanager，执行：
`./sbin/yarn-daemon.sh start nodemanager`
如果正常启动，可以看到启动了相应的进程，并且logs目录下生成了相应的日志

然后启动MapReduce JobHistory Server，执行：
`./sbin/mr-jobhistory-daemon.sh start historyserver`
如果正常启动，可以看到启动了相应的进程，并且logs目录下生成了相应的日志

下面我们看下web界面

打开[http://127.0.0.1:8088/cluster](http://182.92.80.220:8088/cluster)看下yarn管理的集群资源情况(因为在yarn-site.xml中我们配置了yarn.resourcemanager.webapp.address是127.0.0.1:8088)

![](http://www.shareditor.com/uploads/media/my-context/0001/01/9875094d4c709184aea1e43f4b9b026d3455badd.png)

打开[http://127.0.0.1:19888/jobhistory](http://182.92.80.220:19888/jobhistory/logs/)看下map-reduce任务的执行历史情况(因为在mapred-site.xml中我们配置了mapreduce.jobhistory.webapp.address是127.0.0.1:19888)

![](http://www.shareditor.com/uploads/media/my-context/0001/01/a12c11ee0d1adcb46b3943a402aa6982acdea9f9.png)

打开[http://127.0.0.1:50070/dfshealth.html](http://182.92.80.220:19888/jobhistory/logs/)看下namenode的存储系统情况(因为在hdfs-site.xml中我们配置了dfs.namenode.http-address是127.0.0.1:50070)

![](http://www.shareditor.com/uploads/media/my-context/0001/01/23fe73c7a4c3409252429d4f0ae039a002a24a02.png)

到此为止我们对hadoop的部署完成。下面试验一下hadoop的功能

先验证一下hdfs分布式文件系统，执行以下命令看是否有输出：

```
[root@MYAY hadoop]# ./bin/hadoop fs -mkdir /input
[root@MYAY hadoop]# cat data
1
2
3
4
[root@MYAY hadoop]# ./bin/hadoop fs -put input /input
[root@MYAY hadoop]# ./bin/hadoop fs -ls /input
Found 1 items
-rw-r--r--   3 root supergroup          8 2016-08-07 15:04 /input/data
```

这时通过[http://127.0.0.1:50070/dfshealth.html](http://182.92.80.220:19888/jobhistory/logs/)可以看到存储系统的一些变化

下面我们以input为输入启动一个mapreduce任务
`[root@MYAY hadoop]# ./bin/hadoop jar ./share/hadoop/tools/lib/hadoop-streaming-2.7.2.jar -input /input -output /output -mapper cat -reducer wc`
之后看是否产生了/output的输出：

```
[root@MYAY hadoop]# ./bin/hadoop fs -ls /output
Found 2 items
-rw-r--r--   3 root supergroup          0 2016-08-07 15:11 /output/_SUCCESS
-rw-r--r--   3 root supergroup         25 2016-08-07 15:11 /output/part-00000
[root@MYAY hadoop]# ./bin/hadoop fs -cat /output/part-00000
      4       4      12
```

这时通过[http://127.0.0.1:19888/jobhistory](http://182.92.80.220:19888/jobhistory/logs/)可以看到mapreduce任务历史：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/2d452a41ea817d5d054cd355286ebb5a4f281b07.png)

也可以通过[http://127.0.0.1:8088/cluster](http://182.92.80.220:8088/cluster)看到任务历史

![](http://www.shareditor.com/uploads/media/my-context/0001/01/ec4e5f3cf314f7b20c929ab85327fad1c356b4c6.png)

为什么两处都有历史呢？他们的区别是什么呢？

我们看到cluster显示的其实是每一个application的历史信息，他是yarn(ResourceManager)的管理页面，也就是不管是mapreduce还是其他类似mapreduce这样的任务，都会在这里显示，mapreduce任务的Application Type是MAPREDUCE，其他任务的类型就是其他了，但是jobhistory是专门显示mapreduce任务的

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址



## hbase的部署

首先从[http://www.apache.org/dyn/closer.cgi/hbase/](http://www.apache.org/dyn/closer.cgi/hbase/)下载稳定版安装包，我下的是https://mirrors.tuna.tsinghua.edu.cn/apache/hbase/stable/hbase-1.2.2-bin.tar.gz

解压后修改conf/hbase-site.xml，改成：

```xml
<configuration>
    <property>
        <name>hbase.cluster.distributed</name>
        <value>true</value>
    </property>
    <property>
        <name>hbase.rootdir</name>
        <value>hdfs://127.0.0.1:8001/hbase</value>
    </property>
    <property>
        <name>hbase.zookeeper.quorum</name>
        <value>127.0.0.1</value>
    </property>
</configuration>
```

其中hbase.rootdir配置的是hdfs地址，ip:port要和hadoop/core-site.xml中的fs.defaultFS保持一致

其中hbase.zookeeper.quorum是zookeeper的地址，可以配多个，我们试验用就先配一个

启动hbase，执行：
`./bin/start-hbase.sh`
这时有可能会让你输入本地机器的密码

启动成功后可以看到几个进程起来，包括zookeeper的HQuorumPeer和hbase的HMaster、HRegionServer

下面我们试验一下hbase的使用，执行：

```
hbase(main):001:0> status
1 active master, 0 backup masters, 1 servers, 0 dead, 3.0000 average load
```

创建一张表

```
hbase(main):004:0> create 'table1','field1'
0 row(s) in 1.3430 seconds

=> Hbase::Table - table1
```

获取一张表

```
hbase(main):005:0> t1 = get_table('table1')
0 row(s) in 0.0010 seconds

=> Hbase::Table - table1
```

添加一行

```bash
hbase(main):008:0> t1.put 'row1', 'field1:qualifier1', 'value1'
0 row(s) in 0.4160 seconds
```

读取全部

```
hbase(main):009:0> t1.scan
ROW                                                                 COLUMN+CELL
 row1                                                               column=field1:qualifier1, timestamp=1470621285068, value=value1
1 row(s) in 0.1000 seconds
```

我们同时也看到hdfs中多出了hbase存储的目录：

```
[root@MYAY hbase]# ./hadoop/bin/hadoop fs -ls /hbase
Found 7 items
drwxr-xr-x   - root supergroup          0 2016-08-08 09:05 /hbase/.tmp
drwxr-xr-x   - root supergroup          0 2016-08-08 09:58 /hbase/MasterProcWALs
drwxr-xr-x   - root supergroup          0 2016-08-08 09:05 /hbase/WALs
drwxr-xr-x   - root supergroup          0 2016-08-08 09:05 /hbase/data
-rw-r--r--   3 root supergroup         42 2016-08-08 09:05 /hbase/hbase.id
-rw-r--r--   3 root supergroup          7 2016-08-08 09:05 /hbase/hbase.version
drwxr-xr-x   - root supergroup          0 2016-08-08 09:24 /hbase/oldWALs
```

这说明hbase是以hdfs为存储介质的，因此它具有分布式存储拥有的所有优点

hbase的架构如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/0035b53b007f16c8399e5b04aa6cd0bc352ddf10.png)

其中HMaster负责管理HRegionServer以实现负载均衡，负责管理和分配HRegion(数据分片)，还负责管理命名空间和table元数据，以及权限控制

HRegionServer负责管理本地的HRegion、管理数据以及和hdfs交互。

Zookeeper负责集群的协调(如HMaster主从的failover)以及集群状态信息的存储

客户端传输数据直接和HRegionServer通信



## hive的部署

从[http://mirrors.hust.edu.cn/apache/hive](http://mirrors.hust.edu.cn/apache/hive)下载安装包，我下的是http://mirrors.hust.edu.cn/apache/hive/stable-2/apache-hive-2.1.0-bin.tar.gz

解压后，我们先准备hdfs，执行：

```
[root@MYAY hadoop]# ./hadoop/bin/hadoop fs -mkdir /tmp
[root@MYAY hadoop]# ./hadoop/bin/hadoop fs -mkdir /user
[root@MYAY hadoop]# ./hadoop/bin/hadoop fs -mkdir /user/hive
[root@MYAY hadoop]# ./hadoop/bin/hadoop fs -mkdir /user/hive/warehourse
[root@MYAY hadoop]# ./hadoop/bin/hadoop fs -chmod g+w /tmp
[root@MYAY hadoop]# ./hadoop/bin/hadoop fs -chmod g+w /user/hive/warehourse
```

使用hive必须提前设置好HADOOP_HOME环境变量，这样它可以自动找到我们的hdfs作为存储，不妨我们把各种HOME和各种PATH都配置好，如：

```bash
HADOOP_HOME=/data/apache/hadoop
export HADOOP_HOME
HBASE_HOME=/data/apache/hbase
export HBASE_HOME
HIVE_HOME=/data/apache/hive
export HIVE_HOME
PATH=$PATH:$HOME/bin
PATH=$PATH:$HBASE_HOME/bin
PATH=$PATH:$HIVE_HOME/bin
PATH=$PATH:$HADOOP_HOME/bin
export PATH
```

拷贝创建hive-site.xml、hive-log4j2.properties、hive-exec-log4j2.properties，执行

```
[root@MYAY hive]# cp conf/hive-default.xml.template conf/hive-site.xml
[root@MYAY hive]# cp conf/hive-log4j2.properties.template conf/hive-log4j2.properties
[root@MYAY hive]# cp conf/hive-exec-log4j2.properties.template conf/hive-exec-log4j2.properties
```

修改hive-site.xml，把其中的${system:java.io.tmpdir}都修改成/data/apache/tmp，你也可以自己设置成自己的tmp目录，把${system:user.name}都换成用户名

```
:%s/${system:java.io.tmpdir}/\/data\/apache\/tmp/g
:%s/${system:user.name}/myself/g
```

初始化元数据数据库(默认保存在本地的derby数据库，也可以配置成mysql)，注意，不要先执行hive命令，否则这一步会出错，具体见[http://stackoverflow.com/questions/35655306/hive-installation-issues-hive-metastore-database-is-not-initialized](http://stackoverflow.com/questions/35655306/hive-installation-issues-hive-metastore-database-is-not-initialized)，下面执行：
`[root@MYAY hive]# schematool -dbType derby -initSchema`
成功之后我们可以以客户端形式直接启动hive，如：

```
[root@MYAY hive]# hive
hive> show databases;
OK
default
Time taken: 1.886 seconds, Fetched: 1 row(s)
hive>
```

试着创建个数据库是否可以：

```
hive> create database mydatabase;
OK
Time taken: 0.721 seconds
hive> show databases;
OK
default
mydatabase
Time taken: 0.051 seconds, Fetched: 2 row(s)
hive>
```

这样我们还是单机的hive，不能在其他机器登陆，所以我们要以server形式启动：
`nohup hiveserver2 &> hive.log &`
默认会监听10000端口，这时可以通过jdbc客户端连接这个服务访问hive

hive的具体使用在这里不赘述



## spark部署

首先在[http://spark.apache.org/downloads.html](http://spark.apache.org/downloads.html)下载指定hadoop版本的安装包，我下载的是http://d3kbcqa49mib13.cloudfront.net/spark-2.0.0-bin-hadoop2.7.tgz

spark有多种部署方式，首先支持单机直接跑，如执行样例程序：
`./bin/spark-submit examples/src/main/python/pi.py 10`
它可以直接运行得出结果

下面我们说下spark集群部署方法：

解压安装包后直接执行：
`[root@MYAY spark-2.0.0-bin-hadoop2.7]# sbin/start-master.sh`
这时可以打开[http://127.0.0.1:8080/](http://182.92.80.220:8080/)看到web界面如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/87ae57ed70f05f2f6bd5c38bcd6c6a947f8646b5.png)

根据上面的url：spark://MYAY:7077，我们再启动slave：
`[root@MYAY spark-2.0.0-bin-hadoop2.7]# ./sbin/start-slave.sh spark://MYAY:7077`
刷新web界面如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/724a76469f08483d7c813d62de0a449da1572b56.png)

出现了一个worker，我们可以根据需要启动多个worker

下面我们把上面执行过的任务部署到spark集群上执行：
`./bin/spark-submit --master spark://MYAY:7077 examples/src/main/python/pi.py 10`
web界面如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/44e9dc8e3e6d047e42d83e300ba44423fe7f9131.png)

spark程序也可以部署到yarn集群上执行，也就是我们部署hadoop时启动的yarn

我们需要提前配置好HADOOP_CONF_DIR，如下：

```bash
HADOOP_CONF_DIR=${HADOOP_HOME}/etc/hadoop/
export HADOOP_CONF_DIR
```

下面我们把任务部署到yarn集群上去：
`./bin/spark-submit --master yarn --deploy-mode cluster examples/src/main/python/pi.py 10`
看[http://127.0.0.1:8088/cluster](http://182.92.80.220:8088/cluster)效果如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/4dffe013b70ba411914666ed18d911c6bdc1c803.png)



## 总结一下

hdfs是所有hadoop生态的底层存储架构，它主要完成了分布式存储系统的逻辑，凡是需要存储的都基于其上构建

yarn是负责集群资源管理的部分，这个资源主要指计算资源，因此它支撑了各种计算模块

map-reduce组件主要完成了map-reduce任务的调度逻辑，它依赖于hdfs作为输入输出及中间过程的存储，因此在hdfs之上，它也依赖yarn为它分配资源，因此也在yarn之上

hbase基于hdfs存储，通过独立的服务管理起来，因此仅在hdfs之上

hive基于hdfs存储，通过独立的服务管理起来，因此仅在hdfs之上

spark基于hdfs存储，即可以依赖yarn做资源分配计算资源也可以通过独立的服务管理，因此在hdfs之上也在yarn之上，从结构上看它和mapreduce一层比较像

总之，每一个系统负责了自己擅长的一部分，同时相互依托，形成了整个hadoop生态。




