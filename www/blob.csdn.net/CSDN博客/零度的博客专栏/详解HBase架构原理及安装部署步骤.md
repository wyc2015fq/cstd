# 详解HBase架构原理及安装部署步骤 - 零度的博客专栏 - CSDN博客
2017年02月13日 10:21:26[零度anngle](https://me.csdn.net/zmx729618)阅读数：1976
## **一、什么是HBase**
　 HBase 是一个高可靠、高性能、面向列、可伸缩的分布式存储系统，利用Hbase技术可在廉价PC Server上搭建 大规模结构化存储集群。
　HBase 是Google Bigtable 的开源实现，与Google Bigtable 利用GFS作为其文件存储系统类似， HBase 利用Hadoop HDFS 作为其文件存储系统；Google 运行MapReduce 来处理Bigtable中的海量数据， HBase 同样利用Hadoop MapReduce来处理HBase中的海量数据；Google Bigtable 利用Chubby作为协同服务， HBase 利用Zookeeper作为对应。
## **二、HBase 设计模型**
　　 HBase 中的每一张表就是所谓的 BigTable。BigTable 会存储一系列的行记录，行记录有三个基本类型的定义：Row Key、Time Stamp、Column。
1、Row Key 是行在 BigTable 中的唯一标识。
2、Time Stamp 是每次数据操作对应关联的时间戳，可以看做 SVN 的版本。
3、Column 定义为< family>:< label>，通过这两部分可以指定唯一的数据的存储列，family 的定义和修改需要 对 HBase 进行类似于 DB 的 DDL 操作，而 label ，不需要定义直接可以使用，这也为**动态定制列**提供了一种手段 。family 另一个作用体现在物理存储优化读写操作上，同 family 的数据物理上保存的会比较临近，因此在业务设计的过程中可以利用这个特性。
### 1、逻辑存储模型
　　 HBase 以表的形式存储数据。表由行和列组成。列划分为若干个列族（row family），如下图所示。 
![这里写图片描述](https://img-blog.csdn.net/20160501212317302)
下面是对表中元素的详细解析： 
**Row Key**
与nosql数据库们一样,row key是用来检索记录的主键。访问hbase table中的行，只有三种方式：
1 通过单个row key访问
2 通过row key的range
3 全表扫描
Row key行键 (Row key)可以是任意字符串(最大长度是 64KB，实际应用中长度一般为 10-100bytes)，在hbase内部，row key保存为字节数组。
存储时，数据按照Row key的字典序(byte order)排序存储。设计key时，要充分排序存储这个特性，将经常一起读取的行存储放到一起。(位置相关性)
> 
注意： 
  １、字典序对int排序的结果是1,10,100,11,12,13,14,15,16,17,18,19,2,20,21,…,9,91,92,93,94,95,96,97,98,99。要保持整形的自然序，行键必须用0作左填充。
  ２、行的一次读写是原子操作 (不论一次读写多少列)。
**列族**
　　hbase表中的每个列，都归属与某个列族。列族是表的chema的一部分(而列不是)，必须在使用表之前定义。列名都以列族作为前缀。例如courses:history ， courses:math 都属于 courses 这个列族。
　　访问控制、磁盘和内存的使用统计都是在列族层面进行的。实际应用中，列族上的控制权限能 帮助我们管理不同类型的应用：我们允许一些应用可以添加新的基本数据、一些应用可以读取基本数据并创建继承的列族、一些应用则只允许浏览数据（甚至可能因 为隐私的原因不能浏览所有数据）。
**时间戳**
　　HBase中通过row和columns确定的为一个存贮单元称为cell。每个 cell都保存着同一份数据的多个版本。版本通过时间戳来索引。时间戳的类型是 64位整型。时间戳可以由hbase(在数据写入时自动 )赋值，此时时间戳是精确到毫秒的当前系统时间。时间戳也可以由客户显式赋值。如果应用程序要避免数据版本冲突，就必须自己生成具有唯一性的时间戳。每个 cell中，不同版本的数据按照时间倒序排序，即最新的数据排在最前面。
为了避免数据存在过多版本造成的的管理 (包括存贮和索引)负担，hbase提供了两种数据版本回收方式。一是保存数据的最后n个版本，二是保存最近一段时间内的版本（比如最近七天）。用户可以针对每个列族进行设置。
**Cell**
由{row key, column( = + ), version} 唯一确定的单元。cell中的数据是没有类型的，全部是字节码形式存贮。
### 2、物理存储模型
　 Table 在行的方向上分割为多个HRegion，每个HRegion分散在不同的RegionServer中。 
![这里写图片描述](https://img-blog.csdn.net/20160501214549631)
　　每个HRegion由多个Store构成，每个Store由一个memStore和0或多个StoreFile组成，每个Store保存一个Columns Family　
![这里写图片描述](https://img-blog.csdn.net/20160501214637828)
StoreFile以HFile格式存储在HDFS中。
## **三、HBase 存储架构**
　　从HBase的架构图上可以看出，HBase中的存储包括HMaster、HRegionServer、HRegion、Store、MemStore、StoreFile、HFile、HLog等， 以下是 HBase 存储架构图:
![这里写图片描述](https://img-blog.csdn.net/20160501215448050)
　 HBase中的每张表都通过行键按照一定的范围被分割成多个子表（HRegion），默认一个HRegion超过256M就要被分割成两个，这个过程由HRegionServer管理，而HRegion的分配由HMaster管理。
**HMaster的作用：**
       　　 1、 为Region server分配region。 
    　　   2、 负责Region server的负载均衡。
　　 3、发现失效的Region server并重新分配其上的region。
　　4、 HDFS上的垃圾文件回收。
　　  5、 处理schema更新请求。
**HRegionServer作用：**
　　   1、 维护master分配给他的region，处理对这些region的io请求。
　　   2、 负责切分正在运行过程中变的过大的region。
　　   可以看到，client访问hbase上的数据并不需要master参与（寻址访问zookeeper和region server，数据读写访问region server），master仅仅维护table和region的元数据信息（table的元数据信息保存在zookeeper上），负载很低。 HRegionServer存取一个子表时，会创建一个HRegion对象，然后对表的每个列族创建一个Store实例，每个Store都会有一个MemStore和0个或多个StoreFile与之对应，每个StoreFile都会对应一个HFile，
 HFile就是实际的存储文件。因此，一个HRegion有多少个列族就有多少个Store。 一个HRegionServer会有多个HRegion和一个HLog。
**HRegion**
　　table在行的方向上分隔为多个Region。Region是HBase中分布式存储和负载均衡的最小单元，即不同的region可以分别在不同的Region Server上，但同一个Region是不会拆分到多个server上。
　　Region按大小分隔，每个表一般是只有一个region。随着数据不断插入表，region不断增大，当region的某个列族达到一个阈值（默认256M）时就会分成两个新的region。
　　   1、< 表名,startRowkey,创建时间>
　　 2、由目录表(-ROOT-和.META.)记录该region的endRowkey
　　 HRegion定位：Region被分配给哪个Region Server是完全动态的，所以需要机制来定位Region具体在哪个region server。
　　 HBase使用三层结构来定位region：
　　 1、 通过zk里的文件/hbase/rs得到-ROOT-表的位置。-ROOT-表只有一个region。
　　2、通过-ROOT-表查找.META.表的第一个表中相应的region的位置。其实-ROOT-表是.META.表的第一个region；.META.表中的每一个region在-ROOT-表中都是一行记录。
　3、通过.META.表找到所要的用户表region的位置。用户表中的每个region在.META.表中都是一行记录。
　　 -ROOT-表永远不会被分隔为多个region，保证了最多需要三次跳转，就能定位到任意的region。client会将查询的位置信息保存缓存起来，缓存不会主动失效，因此如果client上的缓存全部失效，则需要进行6次网络来回，才能定位到正确的region，其中三次用来发现缓存失效，另外三次用来获取位置信息。
**Store**
　　每一个region由一个或多个store组成，至少是一个store，hbase会把一起访问的数据放在一个store里面，即为每个ColumnFamily建一个store，如果有几个ColumnFamily，也就有几个Store。一个Store由一个memStore和0或者多个StoreFile组成。 HBase以store的大小来判断是否需要切分region。　
**MemStore**
　　memStore 是放在内存里的。保存修改的数据即keyValues。当memStore的大小达到一个阀值（默认64MB）时，memStore会被flush到文件，即生成一个快照。目前hbase 会有一个线程来负责memStore的flush操作。
**StoreFile**
　　 memStore内存中的数据写到文件后就是StoreFile，StoreFile底层是以HFile的格式保存。 
**HFile**
　　 HBase中KeyValue数据的存储格式，是hadoop的二进制格式文件。 首先HFile文件是不定长的，长度固定的只有其中的两块：Trailer和FileInfo。Trailer中有指针指向其他数据块的起始点，FileInfo记录了文件的一些meta信息。 Data Block是hbase io的基本单元，为了提高效率，HRegionServer中有基于LRU的block cache机制。每个Data块的大小可以在创建一个Table的时候通过参数指定（默认块大小64KB），大号的Block有利于顺序Scan，小号的Block利于随机查询。每个Data块除了开头的Magic以外就是一个个KeyValue对拼接而成，Magic内容就是一些随机数字，目的是防止数据损坏，结构如下。
![这里写图片描述](https://img-blog.csdn.net/20160502093030457)
HFile结构图如下： 
![这里写图片描述](https://img-blog.csdn.net/20160502093535959)
　　Data Block段用来保存表中的数据，这部分可以被压缩。 Meta Block段（可选的）用来保存用户自定义的kv段，可以被压缩。 FileInfo段用来保存HFile的元信息，不能被压缩，用户也可以在这一部分添加自己的元信息。 Data Block Index段（可选的）用来保存Meta Blcok的索引。 Trailer这一段是定长的。保存了每一段的偏移量，读取一个HFile时，会首先读取Trailer，Trailer保存了每个段的起始位置(段的Magic Number用来做安全check)，然后，DataBlock
 Index会被读取到内存中，这样，当检索某个key时，不需要扫描整个HFile，而只需从内存中找到key所在的block，通过一次磁盘io将整个 block读取到内存中，再找到需要的key。DataBlock Index采用LRU机制淘汰。 HFile的Data Block，Meta Block通常采用压缩方式存储，压缩之后可以大大减少网络IO和磁盘IO，随之而来的开销当然是需要花费cpu进行压缩和解压缩。（备注： DataBlock Index的缺陷。    a) 占用过多内存　b) 启动加载时间缓慢）
**HLog**
　　HLog(WAL log)：WAL意为write ahead log，用来做灾难恢复使用，HLog记录数据的所有变更，一旦region server 宕机，就可以从log中进行恢复。
**LogFlusher**
　　定期的将缓存中信息写入到日志文件中 
**LogRoller**
　   　对日志文件进行管理维护　　
## **四、Hbase 的部署安装步骤**
安装 HBase 有两种方式，单机安装和分布式安装。 
### １、 单机安装
   　　 HBase 需要再 Hadoop 环境运行，因此安装HBase 的前提是必须安装 Hadoop 环境。Hadoop 环境的安装可以参考[http://blog.csdn.net/u010330043/article/details/51235373](http://blog.csdn.net/u010330043/article/details/51235373)。下载与Hadoop2.2.0 相匹配的hbase-0.98.11-hadoop2-bin.tar.gz
 软件包。 
HBase 的安装步骤如下所示。
**步骤一** 解压hbase-0.98.11-hadoop2-bin.tar.gz 到指定的目录下（这里是在/usr/java），并将权限分配给hadoop用户（运行hadoop的账户）。
这里下载的是hbase-0.98.11-hadoop2，Hadoop集群使用的是2.2.0，将其解压到/usr/java下并重命名为 hbase
```
[root@cs0 java]$ tar -zxvf hbase-0.98.11-hadoop2-bin.tar.gz
[root@cs0 java]$ mv hbase-0.98.11-hadoop2 hbase
[root@cs0 java]$ chown -R hadoop:hadoop hbase
```
**步骤二**　配置 HBase 的环境变量到 /etc/profile 文件中。
```
[root@cs0 java]$ vi /etc/profile
HBASE_HOME=/usr/java/hbase
PATH=$JAVA_HOME/bin:$HIVE_HOME/bin:$HADOOP_HOME/bin:$HBASE_HOME/bin:$PATH
```
使配置文件立即生效。
`[root@cs0 tmp]# source /etc/profile`
**步骤三** 修改 conf/hbase-env.sh。
```bash
1) 去掉 JAVA_HOME 前的 “#”，并将其修改成自己安装的 Java 路径。
       2) 去掉 HBASE_MANAGES_ZK 前的 “#”，并设置其值为 true（HBase 管理自己的 ZooKeeper，这样就不需要安装 ZooKeeper）。
```
**步骤四** 打开 conf/hbase-site.xml，添加如下内容。
　　hbase.rootdir 需要与之前安装的 Hadoop 目录下 conf/core-site.xml 文件中的 fs.default.name 属性值对应。
```xml
fs.default.name 设置为hdfs://ywendeng:9000/
hbase.rootdir 设置为hdfs://ywendeng:9000/hbase
hbase.ZooKeeper.quorum 设置为 ywendeng
hbase.tmp.dir 设置为之前创建的 tmp 目录：/usr/java/hbase/tmp
代码如下：
<configuration>
<property>
    <name>fs.default.name</name>
    <value>hdfs://cs:9000/</value>
</property>
<property>
    <name>hbase.rootdir</name>
    <value>hdfs://cs:9000/hbase</value>
</property>
<property>
    <name>hbase.ZooKeeper.quorum</name>
    <value>cs</value>
</property>
<property>
    <name>hbase.tmp.dir</name>
    <value>/home/hadoop/data/hbase_${user.name}</value>
</property>
</configuration>
```
**步骤五** 启动HBase（前提已经启动 Hadoop）。
`[hadoop@cs0 conf]$ start-hbase.sh`
[]()
**步骤六** 在Hbase启动成功之后，在浏览器中输入网址 192.168.80.128:60010（备注： 192.168.1.128 为安装HBase 虚拟机的IP地址）
### 2、分布式安装
**步骤一** 上传hbase安装包
**步骤二** 解压
**步骤三** 配置hbase集群，要修改3个文件（首先zk集群已经安装好了） 
    注意：要把hadoop的hdfs-site.xml和core-site.xml 放到hbase/conf下
```
1、vim hbase-env.sh
    export JAVA_HOME=/usr/java/jdk1.7.0_55
    //告诉hbase使用外部的zk
    export HBASE_MANAGES_ZK=false
```
```xml
2、vim hbase-site.xml
    <configuration>
        <!-- 指定hbase在HDFS上存储的路径 -->
        <property>
                <name>hbase.rootdir</name>
                <value>hdfs://ns1/hbase</value>
        </property>
        <!-- 指定hbase是分布式的 -->
        <property>
                <name>hbase.cluster.distributed</name>
                <value>true</value>
        </property>
        <!-- 指定zk的地址，多个用“,”分割 -->
        <property>
                <name>hbase.zookeeper.quorum</name>
                <value>cs3:2181,cs4:2181,cs5:2181</value>
        </property>
    </configuration>
```
```
3、 vim regionservers
    cs3
    cs4
    cs5
```
```
4、拷贝hbase到其他节点
    scp -r  app/hbase-0.96.2-hadoop2/ cs2:/home/hadoop/app/
    scp -r app/hbase-0.96.2-hadoop2/  cs3: /home/hadoop/app/
    scp -r app/hbase-0.96.2-hadoop2/  cs4: /home/hadoop/app/
    scp -r app/hbase-0.96.2-hadoop2/  cs5: /home/hadoop/app/
```
**步骤四** 启动所有的hbase
```
分别启动zk
./zkServer.sh start
启动hbase集群
start-dfs.sh
启动hbase，在主节点上运行：
start-hbase.sh
```
**步骤五** 通过浏览器访问hbase管理页面
`192.168.80.128:60010`
**步骤六** 为保证集群的可靠性，要启动多个HMaster
` hbase-daemon.sh start master`
