# Python海量数据处理之_Hadoop家族 - weixin_33985507的博客 - CSDN博客
2017年12月27日 17:31:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
 本篇是hadoop部分的最后一篇，主要介绍Hadoop家族的常用工具。以及解答学习过程中的一些疑问。
## hadoop家族
![5357893-db0d7ffa523be138.png](https://upload-images.jianshu.io/upload_images/5357893-db0d7ffa523be138.png)
 Pig是上层封装了的数据流处理工具。
 Mahout是基于集群的数据挖掘工具。
 Zookeeper是集群管理工具，比如配置一些备用服务器，当重要的服务宕机时，及时补救。
 其中黄色部分是hadoop安装包中自带的，HDFS是文件系统支持，MapReduce是计算支持。
 绿色部分都是向上层提供类似数据库的数据访问，但方式不同。Hive是基于MapReduce的封装，它向上层提供了类似SQL语言的HQL，向下通过MapReduce方式访问数据。HBase在对HDFS层的封装，它本质上是一种key/value系统，主要负责数据存储，解决的是HDFS随机存储方面的问题。
## 有了mysql为什么还要Hadoop?
 我们之前介绍搭建比特币矿池的时候，使用zookeeper+kafka+mysql建立集群，其中用mysql建构数据服务器，集群中其它机器通过ip和端口访问其数据，配置方便，速度也不错。既然集群中可以使用mysql存储数据，那为什么还非要用hadoop系列存储工具呢？
 mysql存放的是数据库，而hadoop系列存放的是数据仓库，一方面HDFS针对的是更大规模的数据，另一方面mysql的数据类型只限于数据库，而HDFS比较灵活，可以存储各种类型的数据。
## 具体安装
 集群中的计算机软硬件可能有所差异，为了保证一致性，最好使用下载软件包的方式安装，而不用apt-get系列工具。一般的安装过程主要包括：下载，解包，设置环境变量，修改配置文件，启动等几个部分。
