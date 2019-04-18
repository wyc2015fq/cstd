# Alluxio在多级分布式缓存系统中的应用 - weixin_33985507的博客 - CSDN博客
2019年01月28日 12:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
## 1. 前言
随着移动互联网的发展，越来越多的业务数据和日志数据需要用户处理。从而，用数据去驱动和迭代业务发展。数据处理主要包括：计算和查询。计算主要为离线计算、实时流计算、图计算、迭代计算等；查询主要包括Ahdoc、OLAP、OLTP、KV、索引等。然而，在数据业务中，我们时常听到数据需求方和数据开发方对性能慢的不满，所以，如何高效响应海量且迫切的数据需求，是大数据平台需要面对的一个关键问题，本文将介绍如何基于Alluxio建设分布式多级缓存系统对数据进行计算加速和查询加速。
## 2. 离线数据计算查询加速问题
对于加速组件，结合生态兼容性和系统成熟性的基础上，我们选择Alluxio，我们知道，Alluxio是通过UFS思想访问底层持久化的分布式数据。通常，我们的数据主要存放在公司私有HDFS和MySQL上。那么，如何通过UFS思想访问到私有HDFS数据进行加速是我们面对的主要问题。私有HDFS由于历史原因，其基于的HDFS版本较低，加上公司对HDFS进行了部分改造，使得开源的计算和查询组件访问公司内部的离线数据较为困难。因此，如何打通Alluxio访问私有HDFS成为了系统的关键，后面的章节中，我们会做相关介绍。
## 3. 基于Alluxio的解决方案
整体上，我们当前的多级缓存系统由私有HDFS(PB级别) + MEM(400G) + SSD(64T)共3层组成。 示意如下：
![\"image\"](https://static.geekbang.org/infoq/5c4e6afbd246c.png)
系统会根据类似LRU思想对热点数据进行缓存。我们主要存放热点部门核心数据。由于资源申请原因，我们的Alluxio在南方，UFS在北方，***所以后面的测试数据均为跨地域性能数据***，提供基于Spark、Hive、 Presto、YARN、API等方式访问数据。
对于多级分布式缓存系统，我们实现的整体示意图如下：
![\"image\"](https://static.geekbang.org/infoq/5c4e6afeb1ed7.png)
从上述方案中，我们可以看出，如果通过path路径方式访问私有HDFS数据，直接使用alluxio://协议路径即可；但是，如果要开源的Hive或者开源组件(基于Hive桥接)以数据表方式访问公司内部的私有HDFS数据，需要进行“表schema映射”，思路如下图:
![\"image\"](https://static.geekbang.org/infoq/5c4e6afc8500a.png)
基于上述方案，可以大致抽象出使用Alluxio的几个一般性步骤，如下：
![\"image\"](https://static.geekbang.org/infoq/5c4e6afb9a189.png)
下面，我们会一一对每个步骤进行相关说明及案例数据分析。
### 3.1 Alluxio Meta数据同步(load和sync)
由于UFS随时可能会有变动，导致Alluxio Meta和UFS的Meta不一致，那么，如何在Alluxio的master中同步UFS文件的Meta十分重要，由于Alluxio Meta的load和sync性能成本较高（对于load和sync区别，本文不赘述），官方不推荐过度频繁load和sync Alluxio的Meta数据。
#### 3.1.1 同步方案
在我们的集群中，我们均采用了默认的配置，而对于Aluxio Meta和UFS Meta的状态主要有以下3种：
![\"image\"](https://static.geekbang.org/infoq/5c4e6afdc0885.png)
|Meta同步方式|说明|
|----|----|
|fs ls path|查看文件状态|
|fs checkConsistency -r path|检测Alluxio Meta和UFS Meta的一致性，并进行repair修复|
如果通过上面2种方法，Meta无论如何无法同步（大家应该会遇到这类情况），可以考虑fs rm -R --alluxioOnly path从Alluxio中删除数据(仅删除Alluxio中的数据，不删除对应UFS上的数据)，然后再fs ls path便可以让Alluxio中的Meta和UFS的真实Meta同步一致。
#### 3.1.2 Meta同步性能
我们使用Alluxio自带的fs ls命令进行Meta同步，相关Alluxio设置如下：
`alluxio.user.network.netty.timeout=600minalluxio.user.network.netty.writer.close.timeout=600minalluxio.user.network.socket.timeout=600min`
上面的设置主要是为了解决网络超时，Meta同步是非常耗时的操作。
数据同步性能数据如下（请忽略分区大小的合理性）：
![\"image\"](https://static.geekbang.org/infoq/5c4e6afc3a4ae.png)
### 3.2 Alluxio加载数据
#### 3.2.1 数据加载方案
有了缓存系统，那么，我们需要提前常用数据预加载到缓存系统中，如此，当用户使用时候，直接从缓存中读取，加快读取速度。数据预加载的方式有如下图的3种方式：
![\"image\"](https://static.geekbang.org/infoq/5c4e6afce01a8.png)
|加载方式|数据类型|相关说明|
|----|----|----|
|Alluxio load命令|path类型数据|alluxio文件系统的基础shell命令|
|Spark JAR|path类型数据|自己开发，基于分布式加载，利用yarn模式的jar，传入path这个参数，来count文件的条数，以达到加载文件的目的。好处在于，可以通过配置spark-submit的相关参数来调整加载的速度|
|SQL命令|Hive table类型数据|对于Hive表的数据，可以直接利用SELECT COUNT(*) FROM T WHERE condition方式加载。|
3.2.2 数据加载性能
我们主要利用Spark RDD count方式来预加载数据，Spark任务参数如下：
`spark.executor.cores 4spark.executor.instances 200spark.executor.memory 20gspark.speculation truespark.speculation.interval 1000msspark.speculation.multiplier 5spark.speculation.quantile 0.95 `
可以注意到上面开启了比较严格的探测执行，因为在实际应用中，我们经常发现极少个别part的load非常慢。所以，在严格条件下开起探测执行，既保证了计算执行效率，又不太会影响到集群性能。
![\"image\"](https://static.geekbang.org/infoq/5c4e6aff98357.png)
### 3.3 Alluxio数据的读取
#### 3.3.1 数据本地化
本地化数据，相信大家都比较清楚，使用本地数据可以有效地提到数据读取效率，具体可以将每个worker节点的alluxio.worker.hostname设置为对应的$HOSTNAME，熟悉Spark的同学知道，这个设置类似Spark中的SPARK_LOCAL_HOSTNAME设置。
#### 3.3.2 读取性能数据1(私有HDFS-\u0026gt;Alluxio-\u0026gt;Hive/Presto)
![\"image\"](https://static.geekbang.org/infoq/5c4e6afd25531.png)
上图中，介词through和on的区别是，前者为首次从UFS上加载数据（即直接从私有HDFS上读取数据），后者为直接从Alluxio缓存中读取数据。
#### 3.3.3 读取性能数据2(私有HDFS-\u0026gt;Alluxio-\u0026gt;Hive-\u0026gt;Cube)
![\"image\"](https://static.geekbang.org/infoq/5c4e6bd663f98.png)
#### 3.3.4 读取性能数据3(私有HDFS-\u0026gt;Alluxio-\u0026gt;Spark)
![\"image\"](https://static.geekbang.org/infoq/5c4e6bd624265.png)
### 3.4 写入数据性能
#### 3.4.1 通过alluxio://写入
根据Alluxio官方介绍，写数据方式主要有3种：
> 
Default write type when creating Alluxio files. Valid options are `MUST_CACHE` (write will only go to Alluxio and must be stored in Alluxio), `CACHE_THROUGH` (try to cache, write to UnderFS synchronously), `THROUGH` (no cache, write to UnderFS synchronously).
本文仅测试CACHE_THROUGH, 数据为150G，8000个part，由于性能不佳，推测主要由于Meta同步导致，所以后放弃此方法，由于任务结算结果数据通常不大， 改用MUST_CACHE或者直写HDFS。
#### 3.4.2 通过hdfs://直接写入
当结果数据无需缓存到Alluxio时，我们同样可以选择直接将数据写入到HDFS，此方案也无需进行Alluxio和UFS之前的Meta同步。
## 4 Alluxio集群建设
### 4.1 打通私有HDFS
#### 4.1.1 增加模块
由于公司内部的私有HDFS系统和开源最新版本的HADOOP生态难以直接融合，有些接口不支持，于是想到办法就是增加1个UFS存储支持。借鉴公司Spark对于私有HDFS访问思路，基于公司基础部门访问私有HDFS的动态库及JNI的思想，实现对私有HDFS的打通。
#### 4.1.2 遇到的libstdc++.so.6及libc.so.6版本过低问题
关于libstdc++.so.6和libc.so.6的关系，本文不赘述。由于我们增加的UFS是利用java通过jni调用so库，而使用的so文件依赖的libstdc++.so.6版本较高，提示错误如下：
`java.lang.UnsatisfiedLinkError: /tmp/libxxx5420953064508929748.so: /usr/lib64/libstdc++.so.6: version `GLIBCXX_3.4.15' not found`
或者
`glibc/libc.so.6: version `GLIBC_2.14' not found (required by /usr/lib64/libstdc++.so.6)`
可利用如：strings /usr/lib64/libstdc++.so.6 | grep \u0026quot;GLIB\u0026quot;查看libstdc++.so.6和libc.so.6的符号库版本。
其本质原因主要是，因为我们环境使用的java目录内部的so库依赖的libstdc++.so.6的路径有问题，可通过命令`scanelf -F \u0026quot;%F %r\u0026quot; -BR $JAVA_HOME`中的so对于libstdc++.so.6的搜索路径，同样也可以利用命令`patchelf --set-interpreter`和`patchelf --force-rpath --set-rpath`去修改优先搜索路径。
#### 4.1.3 无法连通的问题
在测试连通公司私有HDFS集群的过程中，我们遇到了一个表现个比较奇怪的问题，就是启动master后，Alluxio前端web页面菜单\u0026quot;Browse\u0026quot;点击无响应，debug代码发现，某个reentrantLock的读计数没有正确降为0（Master启动过程中出现，所以debug时候需要设置suspend=y，同时可从AlluxioMaster的main函数开始入手），导致一直处于死锁状态，这个问题排查了很久，近乎把Alluxio启动代码逻辑都看完了，代码上没有发现问题，后来无意发现是因为各节点（master和workers）上没有安装Agent，因为前文提到JNI使用的so库需要通过类似代理的方式先访问Agent, 利用本地的Agent去访问私有HDFS。
#### 4.1.4 recoverLease函数
由于私有HDFS和和最新HDFS的差异，不存在recoverLease， 这个函数主要在XXXUnderFileSystem.java中，主要用户当Alluxio的journal文件非正常关闭下进行恢复。目前，由于时间比较紧，这个功能暂时被跳过。
### 4.2 集群部署
相关机器配置和组件版本如下：
`CPU 56core、MEM 192G、SSD 1.6T、Disk XTCentOS 6.3Alluxio 1.8hadoop-2.6.0-cdh5.14.0spark-2.3.2-bin-hadoop2.7`
#### 4.2.1 用户
Alluxio集群使用alluxio用户创建，便于迁移及维护。如果是root账户部署，可能会减少不少权限问题，但是和root账号耦合度太高。所以，不推荐直接用root账号安装Alluxio。
#### 4.2.2 部署常见问题
##### 4.2.2.1 日志显示
Alluxio启动(bin/alluxio-start.sh)、关闭(bin/alluxio-top.sh)等脚本中的日志显示不够友好，都是用echo方式显示，它的启动关闭日志没有日期，对于新手来说，安装部署问题很多，而日志又没有时间，造成无法区分日志对应哪次操作，造成效率低下。
##### 4.2.2.2 集群启动
目前的./bin/alluxio-start.sh [workers|master|all] [NoMount|SudoMount]，它会默认不给任何提示，先杀死进程后再启动进程，而且不是类似hadoop，提示进程运行中，需要先人肉杀死再启动，我们进行了改造。不然，对于缓存在内存中的数据丢失。
##### 4.2.2.3 RAM内存申请
为了保证速度，我们可能设置MEM作为一级缓存如下
`alluxio.worker.tieredstore.level0.alias=MEMalluxio.worker.tieredstore.level0.dirs.path=/mnt/ramdiskalluxio.worker.tieredstore.level0.dirs.quota=${alluxio.worker.memory.size}alluxio.worker.tieredstore.level0.reserved.ratio=0.1`
那么在执行./bin/start-alluxio.sh workers SudoMount时会提示出错，因为申请内存申请没有权限，解决方案，可以在/etc/sudoers文件中增加一行： alluxio ALL=(ALL)  ALL
## 5 未来计划
### 5.1 更多应用场景
本文中的多级缓存系统（BigCache）除了可以加速访问热数据外，还变相起到了桥接作用。
***让原本无法直接访问公司存储系统数据的新生代开源计算查询组件，可以通过访问alluxio协议变相访问私有HDFS数据，从0到1开启了访问公司私有HDFS数据的可能!***
另外，多级缓存系统不但在离线数据加工方面能有效加速，同时它亦可在对象存储系统中得到较好的应用潜力。
### 5.2 Hive表映射自动探测更新
如上面第3章节提到，现在我们如果要让一些开源组件（基于Hive访问数据）访问到公司内部Hive(私有HDFS)的数据，我们需要人工建设表的schema进行映射，然后利用脚本进行表的partition加载，这一类通用工作，后续计划web化、自动化。
### 5.3 HA的支持
这个问题在整个新增UFS模块开发中，耽误了非常多的时间，主要原因是因为我们底层是通过JNI利用公司基础部门的.so动态库访问公司底层的私有HDFS，在我们配置了HA后，重启过程中，会出现core dump，参考了公司相关wiki，定位成本比较高，所以后续需要对core的问题进行进一步排查，支持系统的HA。
### 5.4 容器化部署
近年来，公司物理机器的配置越来越好，很多机器上都配置有SSD硬盘，而很多线上服务可能只用到了HD部分，对SSD的需求不大，所以如果能够通过混部的方式，将各台机器的SSD利用起来，形成一个巨大的多级缓存池，用来加速离线数据计算和查询，带来时效提高收益的同时，也大大提高了机器的利用率，降低成本。
### 5.5 易用性与性能
如上文一些章节提到，目前易用性还需要进一步提高，比如对于In Alluxio Data的搜索，某些数据缓存过期时间的设置，集群的维护等等都较为不便，同时，对于Meta同步、数据加载、数据读取等性能提高也有较大需求。
## 6 参考资料
- [https://www.alluxio.org/docs/1.8/en/reference/Properties-List.html](%5C)
- [https://www.alluxio.org/docs/1.8/en/compute/Hive.html](%5C)
- [https://www.alluxio.org/docs/1.8/en/deploy/Running-Alluxio-On-Yarn.html](%5C)
- [https://www.alluxio.org/docs/1.8/en/compute/Presto.html](%5C)
- [https://www.alluxio.org/docs/1.8/en/compute/Spark.html](%5C)
- [https://www.alluxio.org/docs/1.7/en/DevelopingUFSExtensions.html](%5C)
- [https://www.alluxio.org/docs/1.8/en/operation/Troubleshooting.html](%5C)
- [https://www.alluxio.org/overview/performance-tuning](%5C)
- [https://hortonworks.com/blog/resource-localization-in-yarn-deep-dive/](%5C)
## 作者介绍
**王冬**，软件研发工程师，2012年北京理工大学计算机本硕毕业，从事大数据领域7年多，先后负责大数据组件研发、大数据平台架构、大数据研发等工作，对大数据整体生态较为熟悉了解。
