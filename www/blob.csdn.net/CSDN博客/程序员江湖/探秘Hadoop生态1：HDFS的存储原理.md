# 探秘Hadoop生态1：HDFS的存储原理 - 程序员江湖 - CSDN博客





2017年03月08日 09:40:41[黄小斜](https://me.csdn.net/a724888)阅读数：967
所属专栏：[后端技术杂谈](https://blog.csdn.net/column/details/25481.html)










根据Maneesh Varshney的漫画改编，以简洁易懂的漫画形式讲解HDFS存储机制与运行原理，非常适合Hadoop/HDFS初学者理解。

# 一、角色出演

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-14.jpg)

如上图所示，HDFS存储相关角色与功能如下：

Client：客户端，系统使用者，调用HDFS API操作文件；与NN交互获取文件元数据；与DN交互进行数据读写。

Namenode：元数据节点，是系统唯一的管理者。负责元数据的管理；与client交互进行提供元数据查询；分配数据存储节点等。

Datanode：数据存储节点，负责数据块的存储与冗余备份；执行数据块的读写操作等。

# 二、写入数据



## 1、发送写数据请求

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-15.jpg)

HDFS中的存储单元是block。文件通常被分成64或128M一块的数据块进行存储。与普通文件系统不同的是，在HDFS中，如果一个文件大小小于一个数据块的大小，它是不需要占用整个数据块的存储空间的。

## 2、文件切分

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-16.jpg)

## 3、DN分配

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-17.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-18.jpg)

## 4、数据写入

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-19.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-20.jpg)

## 5、完成写入

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-21.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-22.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-23.jpg)

## 6、角色定位

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-24.jpg)

# 三、HDFS读文件



## 1、用户需求

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-25.jpg)

HDFS采用的是“一次写入多次读取”的文件访问模型。一个文件经过创建、写入和关闭之后就不需要改变。这一假设简化了数据一致性问题，并且使高吞吐量的数据访问成为可能。

## 2、先联系元数据节点

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-26.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-27.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-28.jpg)

## 3、下载数据

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-29.jpg)

前文提到在写数据过程中，数据存储已经按照客户端与DataNode节点之间的距离进行了排序，距客户端越近的DataNode节点被放在最前面，客户端会优先从本地读取该数据块。

## 4、思考

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-30.jpg)

# 四、HDFS容错机制——第一部分：故障类型及监测方法



## 1、三类故障

（1）第一类：节点失败

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-31.jpg)

（2）第二类：网络故障

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-32.jpg)

（3）第三类：数据损坏（脏数据）

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-33.jpg)

## 2、故障监测机制

（1）节点失败监测机制

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-34.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-35.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-36.jpg)

（2）通信故障监测机制

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-37.jpg)

（3）数据错误监测机制

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-38.jpg)

## ![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-39.jpg)![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-40.jpg)3、回顾：心跳信息与数据块报告

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-41.jpg)

HDFS存储理念是以最少的钱买最烂的机器并实现最安全、难度高的分布式文件系统（高容错性低成本），从上可以看出，HDFS认为机器故障是种常态，所以在设计时充分考虑到单个机器故障，单个磁盘故障，单个文件丢失等情况。

# 五、容错第二部分：读写容错



## 1、写容错

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-42.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-43.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-44.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-45.jpg)

## 2、读容错

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-46.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-47.jpg)

# 六、容错第三部分：数据节点（DN）失效

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-48.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-49.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-50.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-51.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-52.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-53.jpg)

# 七、备份规则

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-54.jpg)

## 1、机架与数据节点

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-55.jpg)

## 2、副本放置策略

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-56.jpg)

数据块的第一个副本优先放在写入数据块的客户端所在的节点上，但是如果这个客户端上的数据节点空间不足或者是当前负载过重，则应该从该数据节点所在的机架中选择一个合适的数据节点作为本地节点。

如果客户端上没有一个数据节点的话，则从整个集群中随机选择一个合适的数据节点作为此时这个数据块的本地节点。

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-57.jpg)

HDFS的存放策略是将一个副本存放在本地机架节点上，另外两个副本放在不同机架的不同节点上。

这样集群可在完全失去某一机架的情况下还能存活。同时，这种策略减少了机架间的数据传输，提高了写操作的效率，因为数据块只存放在两个不同的机架上，减少了读取数据时需要的网络传输总带宽。这样在一定程度上兼顾了数据安全和网络传输的开销。

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-58.jpg)

![hdfs](http://7xipth.com1.z0.glb.clouddn.com/20160422-59.jpg)





