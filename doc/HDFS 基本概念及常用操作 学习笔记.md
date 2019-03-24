# HDFS 基本概念及常用操作 学习笔记

2018年12月09日 21:08:16

小鹅鹅

阅读数：57

更多

 								所属专栏： 																[Spark / Hadoop生态系列](https://blog.csdn.net/column/details/31206.html) 																 							

 									

​                   					 					版权声明：本文为博主原创文章，转载请附http://blog.csdn.net/asd136912。					https://blog.csdn.net/asd136912/article/details/84932177				



### 目录

- - - [基本概念](https://blog.csdn.net/asd136912/article/details/84932177#_3)
    - [写文件操作](https://blog.csdn.net/asd136912/article/details/84932177#_30)
    - [读文件操作](https://blog.csdn.net/asd136912/article/details/84932177#_63)
    - [可靠性](https://blog.csdn.net/asd136912/article/details/84932177#_71)
    - [常用命令](https://blog.csdn.net/asd136912/article/details/84932177#_85)
    - [Ref](https://blog.csdn.net/asd136912/article/details/84932177#Ref_115)



### 基本概念

Hadoop 附带了一个名为 HDFS(Hadoop Distributed File System,  Hadoop分布式文件系统)的分布式文件系统，基于 Hadoop 的应用程序使用 HDFS 。HDFS  是专为存储超大数据文件，运行在集群的商品硬件上。它是容错的，可伸缩的，并且非常易于扩展。

HDFS集群主要由 NameNode [管理文件](https://www.baidu.com/s?wd=%E7%AE%A1%E7%90%86%E6%96%87%E4%BB%B6&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)系统 Metadata 和 DataNodes 存储的实际数据。

![img](https://upload-images.jianshu.io/upload_images/2021461-96b47ae4dd32059b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

- **NameNode**: 可以被认为是系统的主站。它维护所有系统中存在的文件和目录的文件系统树和元数据。其中两个文件：“命名空间映像”和“编辑日志”是用来存储元数据信息。
  1. 存储文件的metadata，运行时所有数据都保存到内存，整个HDFS可存储的文件数受限于NameNode的内存大小
  2. 一个Block在NameNode中对应一条记录（一般一个block占用150字节），如果是大量的小文件，会消耗大量内存。同时maptask的数量是由splits来决定的，所以用MapReduce处理大量的小文件时，就会产生过多的map  task，线程管理开销将会增加作业时间。处理大量小文件的速度远远小于处理同等大小的大文件的速度。因此Hadoop建议存储大文件
  3. 数据会定时保存到本地磁盘，但不保存block的位置信息，而是由DataNode注册时上报和运行时维护（NameNode中与DataNode相关的信息并不保存到NameNode的文件系统中，而是NameNode每次重启后，动态重建）
  4. NameNode失效则整个HDFS都失效了，所以要保证NameNode的可用性
- **DataNode**：作为从机，每台机器位于一个集群中，并提供实际的存储. 它负责为客户读写请求服务。
  1. 保存具体的block数据
  2. 负责数据的读写操作和复制操作
  3. DataNode启动时会向NameNode报告当前存储的数据块信息，后续也会定时报告修改信息
  4. DataNode之间会进行通信，复制数据块，保证数据的冗余性
- Secondary NameNode：定时与NameNode进行同步（定期合并文件系统镜像和编辑日志，然后把合并后的传给NameNode，替换其镜像，并清空编辑日志，类似于CheckPoint机制），但NameNode失效后仍需要手工将其设置成主机

### 写文件操作

![img](https://upload-images.jianshu.io/upload_images/2021461-3f45d2510b6545f9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

1. 客户端将文件写入本地磁盘的临时文件中

2. 当临时文件大小达到一个block大小时，HDFS client通知NameNode，申请写入文件

3. NameNode在HDFS的文件系统中创建一个文件，并把该block id和要写入的DataNode的列表返回给客户端

4. 客户端收到这些信息后，将临时文件写入DataNodes

   ```
    4.1 客户端将文件内容写入第一个DataNode（一般以4kb为单位进行传输）
    4.2 第一个DataNode接收后，将数据写入本地磁盘，同时也传输给第二个DataNode
    4.3 依此类推到最后一个DataNode，数据在DataNode之间是通过pipeline的方式进行复制的
    4.4 后面的DataNode接收完数据后，都会发送一个确认给前一个DataNode，最终第一个DataNode返回确认给客户端
    4.5 当客户端接收到整个block的确认后，会向NameNode发送一个最终的确认信息
    4.6 如果写入某个DataNode失败，数据会继续写入其他的DataNode。然后NameNode会找另外一个好的DataNode继续复制，以保证冗余性
    4.7 每个block都会有一个校验码，并存放到独立的文件中，以便读的时候来验证其完整性
   
   12345678
   ```

5. 文件写完后（客户端关闭），NameNode提交文件（这时文件才可见，如果提交前，NameNode垮掉，那文件也就丢失了。fsync：只保证数据的信息写到NameNode上，但并不保证数据已经被写到DataNode中）

**Rack aware（机架感知）**

通过配置文件指定机架名和DNS的对应关系

假设复制参数是3，在写入文件时，会在本地的机架保存一份数据，然后在另外一个机架内保存两份数据（同机架内的传输速度快，从而提高性能）

整个HDFS的集群，最好是负载平衡的，这样才能尽量利用集群的优势

### 读文件操作

![img](https://upload-images.jianshu.io/upload_images/2021461-e29df4d74e2abb69.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

1. 客户端向NameNode发送读取请求
2. NameNode返回文件的所有block和这些block所在的DataNodes（包括复制节点）
3. 客户端直接从DataNode中读取数据，如果该DataNode读取失败（DataNode失效或校验码不对），则从复制节点中读取（如果读取的数据就在本机，则直接读取，否则通过网络读取）

### 可靠性

1. DataNode可以失效

   DataNode会定时发送心跳到NameNode。如果在一段时间内NameNode没有收到DataNode的心跳消息，则认为其失效。此时NameNode就会将该节点的数据（从该节点的复制节点中获取）复制到另外的DataNode中

2. 数据可以毁坏

   无论是写入时还是硬盘本身的问题，只要数据有问题（读取时通过校验码来检测），都可以通过其他的复制节点读取，同时还会再复制一份到健康的节点中

3. NameNode不可靠

### 常用命令

1. 从本地文件系统复制文件到 HDFS

```
@ubuntu:~$ $HADOOP_HOME/bin/hdfs dfs -copyFromLocal temp.txt /
1
```

此命令将文件从本地文件系统拷贝 temp.txt 文件到 HDFS。

1. 我们可以通过以下命令列出一个目录下存在的文件 -ls

```
@ubuntu:~$ $HADOOP_HOME/bin/hdfs dfs -ls /
1
```

1. 以下命令将文件从 HDFS 拷贝到本地文件系统

```
@ubuntu:~$ $HADOOP_HOME/bin/hdfs dfs -copyToLocal /temp.txt
1
```

1. 以下命令用来创建新的目录

```
@ubuntu:~$ $HADOOP_HOME/bin/hdfs dfs -mkdir /mydirectory
1
```

1. 检查文件的完整性: fsck

### Ref

1. <https://pennywong.gitbooks.io/hadoop-notebook/content/hdfs/introduction.html>
2. <https://www.yiibai.com/hadoop/hdfs_beginners_guide.html>
3. GFS论文
4. Hadoop权威指南