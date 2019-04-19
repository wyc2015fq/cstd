# Hadoop文件系统的接口（二） - 三少GG - CSDN博客
2014年01月19日 00:04:41[三少GG](https://me.csdn.net/scut1135)阅读数：5323
**HDFS全称是Hadoop Distributed System。HDFS是为以流的方式存取大文件而设计的。适用于几百MB，GB以及TB，并写一次读多次的场合。而对于低延时数据访问、大量小文件、同时写和任意的文件修改，则并不是十分适合。**
hadoop提供了许多文件系统的接口，用户可使用URI方案选取合适的文件系统来实现交互。
（1）接口
       hadoop是使用Java编写的。而Hadoop中不同文件系统之间的交互是由java API进行调节的。事实上，前面使用的文件系统的shell就是一个java应用，它使用java文件系统来提供文件系统操作。即使其他文件系统比如FTP、S3都有自己的访问工具，这些接口在HDFS中还是广泛使用，主要用来进行hadoop文件系统之间的协作。
（2）Thrift
      上面提到可以通过java API 与Hadoop的文件系统进行交互，而对于其它非java应用访问hadoop文件系统则比较麻烦。Thriftfs分类单元中的Thrift API 可通过将Hadoop文件系统展示为一个Apache Thrift服务来填补这个不足，让任何有Thrift绑定的语言都能轻松地与Hadoop文件系统进行交互。Thrift是由Facebook公司开发的一种可伸缩的跨语言服务的发展软件框架。Thrift解决了各系统间大数据量的传输通信，以及系统之间语言环境不同而需要跨平台的问题。在多种不同的语言之间通信时，Thrift可以作为二进制的高性能的通信中间件，它支持数据序列化和多种类型的RPC服务。
（3）C语言库
        hadoop提供了映射java文件系统接口的C语言库----libhdfs。libhdfs可以编写为一个访问HDFS的C语言库，实际上，它可以访问任意的Hadoop文件系统，也可以使用JNI(Java Native Interface)来调用java文件系统的客户端。
（4）FUSE
        FUSE允许文件系统整合为一个Unix文件系统并在用户空间中执行。通过使用Hadoop Fuse-DFS的contrib模块支持任意的Hadoop文件系统作为一个标准文件系统进行挂载，便可以使用UNIX的工具和文件系统进行交互，还可以通过任意一种编程语言使用POSIX库来访问文件系统。
（5）WebDAV
         WebDAV是一系列支持编辑和更新文件的HTTP扩展。在大部分的操作系统中，WebDAV共享都可以作为文件系统进行挂载，因此，通过WebDEV向外提供HDFS或其它Hadoop文件系统，可以将HDFS作为一个标准的文件系统进行访问。
（6）其他HDFS接口
           HTTP-HDFS定义了一个只读接口，用来在HTTP上检索目录列表和数据。NameNode的嵌入式Ｗeb服务器运行在50070端口上，以XML格式提供服务，文件数据DataNood通过它们的Web服务器50075端口向NameNode提供。这个协议并不局限于某个HDFS版本，所以用户可以自己编写使用HTTP从运行不同版本的Hadoop的HDFS中读取数据。ＨftpFileSystem就是其中一种实现，它是一个通过HTTP和HDFS交流的hadoop文件系统，是HTTPS的变体。
           FTP：Hadoop接口中还有一个HDFS的FTP接口，它允许使用FTP协议和HDFS交互，即使用FTP客户端和HDFS进行交互。
# [hadoop学习笔记：hadoop文件系统浅析](http://www.cnblogs.com/sharpxiajun/archive/2013/06/15/3137765.html)
1.什么是分布式文件系统？
管理网络中跨多台计算机存储的文件系统称为分布式文件系统。
2.为什么需要分布式文件系统了？
原因很简单，当数据集的大小超过一台独立物理计算机的存储能力时候，就有必要对它进行分区（partition）并存储到若干台单独计算机上。
3.分布式系统比传统的文件的系统更加复杂
因为分布式文件系统架构在网络之上，因此分布式系统引入了网络编程的复杂性，所以分布式文件系统比普通文件系统更加复杂。
4.Hadoop的文件系统
很多童鞋会把hdfs等价于hadoop的文件系统，其实hadoop是一个综合文件系统抽象，而hdfs是hadoop旗舰级文件系统，hadoop除了hdfs还能集成其他文件系统。Hadoop的这个特点充分体现了hadoop的优良的可扩展性。
在hadoop里，hadoop定义了一个抽象的文件系统的概念，具体就是hadoop里面定义了一个java的抽象类：org.apache.hadoop.fs.FileSystm，这个抽象类用来定义hadoop中的一个文件系统接口，只要某个文件系统实现了这个接口，那么它就可以作为hadoop支持的文件系统。下面是目前实现了hadoop抽象文件类的文件系统，如下表所示：
|文件系统|URI方案|Java实现（org.apache.hadoop）|定义|
|----|----|----|----|
|Local|file|fs.LocalFileSystem|支持有客户端校验和本地文件系统。带有校验和的本地系统文件在fs.RawLocalFileSystem中实现。|
|HDFS|hdfs|hdfs.DistributionFileSystem|Hadoop的分布式文件系统。|
|HFTP|hftp|hdfs.HftpFileSystem|支持通过HTTP方式以只读的方式访问HDFS，distcp经常用在不同的HDFS集群间复制数据。|
|HSFTP|hsftp|hdfs.HsftpFileSystem|支持通过HTTPS方式以只读的方式访问HDFS。|
|HAR|har|fs.HarFileSystem|构建在Hadoop文件系统之上，对文件进行归档。Hadoop归档文件主要用来减少NameNode的内存使用。|
|KFS|kfs|fs.kfs.KosmosFileSystem|Cloudstore（其前身是Kosmos文件系统）文件系统是类似于HDFS和Google的GFS文件系统，使用C++编写。|
|FTP|ftp|fs.ftp.FtpFileSystem|由FTP服务器支持的文件系统。|
|S3（本地）|s3n|fs.s3native.NativeS3FileSystem|基于Amazon S3的文件系统。|
|S3（基于块）|s3|fs.s3.NativeS3FileSystem|基于Amazon S3的文件系统，以块格式存储解决了S3的5GB文件大小的限制。|
最后我要强调一点：在hadoop里有一个文件系统概念，例如上面的FileSystem抽象类，它是位于hadoop的Common项目里，主要是定义一组分布式文件系统和通用的I/O组件和接口，hadoop的文件系统准确的应该称作hadoop I/O。而HDFS是实现该文件接口的hadoop自带的分布式文件项目,hdfs是对hadoop I/O接口的实现。
下面我给大家展示一张表，这样大家对hadoop的FileSystem里的相关API操作就比较清晰了，表如下所示：
|Hadoop的FileSystem|Java操作|Linux操作|描述|
|----|----|----|----|
|URL.openSteamFileSystem.openFileSystem.createFileSystem.append|URL.openStream|open|打开一个文件|
|FSDataInputStream.read|InputSteam.read|read|读取文件中的数据|
|FSDataOutputStream.write|OutputSteam.write|write|向文件写入数据|
|FSDataInputStream.closeFSDataOutputStream.close|InputSteam.closeOutputSteam.close|close|关闭一个文件|
|FSDataInputStream.seek|RandomAccessFile.seek|lseek|改变文件读写位置|
|FileSystem.getFileStatusFileSystem.get*|File.get*|stat|获取文件/目录的属性|
|FileSystem.set*|File.set*|Chmod等|改变文件的属性|
|FileSystem.createNewFile|File.createNewFile|create|创建一个文件|
|FileSystem.delete|File.delete|remove|从文件系统中删除一个文件|
|FileSystem.rename|File.renameTo|rename|更改文件/目录名|
|FileSystem.mkdirs|File.mkdir|mkdir|在给定目录下创建一个子目录|
|FileSystem.delete|File.delete|rmdir|从一个目录中删除一个空的子目录|
|FileSystem.listStatus|File.list|readdir|读取一个目录下的项目|
|FileSystem.getWorkingDirectory||getcwd/getwd|返回当前工作目录|
|FileSystem.setWorkingDirectory||chdir|更改当前工作目录|
有了这张表，大家对FileSystem的理解应该会清晰多了吧。
大家从对照表里会发现，hadoop的FileSystem里有两个类：FSDataInputStream和FSDataOutputStream类，它们相当于java I/O里的InputStream和Outputsteam，而事实上这两个类是继承java.io.DataInputStream和java.io.DataOutputStream。
至于关于hadoop I/O本文今天不做介绍，以后也许会专门写篇文章讲讲我自己的理解，不过为了给大家一个清晰的印象，我在博客园里找到了两篇文章，有兴趣的童鞋可以好好看看看，连接如下：
[http://www.cnblogs.com/xuqiang/archive/2011/06/03/2042526.html](http://www.cnblogs.com/xuqiang/archive/2011/06/03/2042526.html)
[http://www.cnblogs.com/xia520pi/archive/2012/05/28/2520813.html](http://www.cnblogs.com/xia520pi/archive/2012/05/28/2520813.html)
5.数据的完整性
数据完整性也就是检测数据是否损坏的技术。Hadoop用户肯定都希望系统在存储和处理数据时候，数据不会有任何的丢失或损坏，尽管磁盘或网络上的每个I/O操作都不太可能将错误引入到自己正在读写的数据里，但是如果系统需要处理的数据量大到hadoop能够处理的极限，数据被损坏的概率就很高了。Hadoop引入了数据完整性校验的功能，下面我将其原理描述如下：
检测数据是否损坏的措施是，在数据第一次引入系统时候计算校验和(checksum)，并在数据通过一个不可靠的通道时候进行传输时再次计算校验和，这样就能发现数据是否损坏了，如果两次计算的校验和不匹配，你就认为数据已经损坏了，但是该技术不能修复数据，它只能检测出错误。常用的错误检测码是CRC-32（循环冗余校验），任何大小的数据输入均计算得到一个32位的整数校验和。
6.压缩与输入分片
文件压缩有两大好处：一是可以减少存储文件所需要的磁盘空间，二是可以加速数据在网络和磁盘上的传输。对于处理海量数据的hadoop而言，这两个好处就变得相当重要了，所以理解hadoop的压缩是很有必要的，下表列出了hadoop支持的压缩格式，如下表：
|压缩格式|工具|算法|文件扩展名|多文件|可分割性|
|----|----|----|----|----|----|
|DEFLATE|无|DEFLATE|.deflate|不|不|
|gzip|gzip|DEFLATE|.gz|不|不|
|ZIP|zip|DEFLATE|.zip|是|是，在文件范围内|
|bzip2|bzip2|bzip2|.bz2|不|是|
|LZO|lzop|LZO|.lzo|不|是|

在hadoop对于压缩有两个指标很重要一个是压缩率还有就是压缩速度，下表列出一些压缩格式在此方面表现的性能，如下所示：
|压缩算法|原始文件大小|压缩后的文件大小|压缩速度|解压缩速度|
|----|----|----|----|----|
|gzip|8.3GB|1.8GB|17.5MB/s|58MB/s|
|bzip2|8.3GB|1.1GB|2.4MB/s|9.5MB/s|
|LZO-bset|8.3GB|2GB|4MB/s|60.6MB/s|
|LZO|8.3GB|2.9GB|49.3MB/S|74.6MB/s|
在hadoop支持压缩里，是否支持切分（splitting）文件的特性也是相当重要的，下面我将讲述切分的问题，也就是我标题写的输入分片的问题：
　　压缩格式是否可以切分的特性是针对mapreduce处理数据而言的，比如我们有一个压缩为1GB的文件，如果hdfs块大小设置为（hdfs块我的文章里没有讲解，不理解的童鞋可以先查查百度，以后我在写hdfs时候会重点讲这个的）64mb，那么这个文件将存储在16个块里，如果把这个文件作为mapreduce作业的输入数据，mapreduce会根据这16个数据块，产生16个map操作，每个块都是其中一个map操作的输入，那么mapreduce执行效率会非常的高，但是这个前提就是该压缩格式要支持切分。假如压缩格式不支持切分的话，那么mapreduce也是可以做出正确处理，这时候它会将16个数据块放到一个map任务里面，这时候map任务数少了，作业粒度也变大了，那么执行效率就会大大下降。
由于本人知识还是有限，关于压缩和切入分片的问题我就讲述到这里，下面提供一篇相关的文章，有兴趣的童鞋可以看看，链接如下：
[http://www.cnblogs.com/ggjucheng/archive/2012/04/22/2465580.html](http://www.cnblogs.com/ggjucheng/archive/2012/04/22/2465580.html)
7.hadoop序列化
我们先看两个定义：
序列化：是指将结构化对象转化为字节流，以便在网络上传输或写到磁盘上进行永久存储。
反序列化：是指将字节流转向结构化对象的逆过程。
序列化在分布式数据处理量大领域经常出现：进程通信和永久存储。
Hadoop中，各个节点的通信是通过远程调用（RPC）实现的，RPC将数据序列化成二进制后发送给远程节点，远程节点收到数据后将二进制字节流反序列化为原始数据。序列化在RPC应用中有着自己的特点，RPC序列化的特点是：
- 
紧凑：紧凑的格式能让我们能充分利用网络带宽，而带宽是数据中心最稀缺的资源；
- 
快速：进程通信形成了分布式系统的骨架，所以需要尽量减少序列化和反序列化的性能开销，这是基本的
- 
可扩展：协议为了满足新的需求变化，所以控制客户端和服务器过程中，需要直接引进相应的协议，这些事新协议，原序列化方式能支持心得协议报文
- 
互操作：能支持不同语言写的客户端和服务端进行交互
在hadoop里面有自己定义的序列化格式：writable，它是hadoop的核心之一。
Writable是一个接口，要实现hadoop的序列化就得实现该接口。因为时间原因，序列化我也不展开了，我下面也推荐一篇文章，里面讲述了hadoop的序列化，虽然讲的简单点，而且不全面，但是看完后对hadoop序列化的具体实现会有个初步的了解，链接如下：
[http://blog.csdn.net/a15039096218/article/details/7591072](http://blog.csdn.net/a15039096218/article/details/7591072)
转：[http://www.cnblogs.com/xia520pi/archive/2012/05/28/2520813.html](http://www.cnblogs.com/xia520pi/archive/2012/05/28/2520813.html)
## 1、HDFS简介
　　HDFS（Hadoop Distributed File System）是Hadoop项目的核心子项目，是分布式计算中数据存储管理的基础，是基于流数据模式访问和处理超大文件的需求而开发的，可以运行于廉价的商用服务器上。它所具有的高容错、高可靠性、高可扩展性、高获得性、高吞吐率等特征为海量数据提供了不怕故障的存储，为超大数据集（Large
 Data Set）的应用处理带来了很多便利。
　　Hadoop整合了众多文件系统，在其中有一个综合性的文件系统抽象，它提供了文件系统实现的各类接口，HDFS只是这个抽象文件系统的一个实例。提供了一个高层的文件系统抽象类org.apache.hadoop.fs.FileSystem，这个抽象类展示了一个分布式文件系统，并有几个具体实现，如下表1-1所示。
表1-1 Hadoop的文件系统
|文件系统|URI方案|Java实现（org.apache.hadoop）|定义|
|----|----|----|----|
|Local|file|fs.LocalFileSystem|支持有客户端校验和本地文件系统。带有校验和的本地系统文件在fs.RawLocalFileSystem中实现。|
|HDFS|hdfs|hdfs.DistributionFileSystem|Hadoop的分布式文件系统。|
|HFTP|hftp|hdfs.HftpFileSystem|支持通过HTTP方式以只读的方式访问HDFS，distcp经常用在不同的HDFS集群间复制数据。|
|HSFTP|hsftp|hdfs.HsftpFileSystem|支持通过HTTPS方式以只读的方式访问HDFS。|
|HAR|har|fs.HarFileSystem|构建在Hadoop文件系统之上，对文件进行归档。Hadoop归档文件主要用来减少NameNode的内存使用。|
|KFS|kfs|fs.kfs.KosmosFileSystem|Cloudstore（其前身是Kosmos文件系统）文件系统是类似于HDFS和Google的GFS文件系统，使用C++编写。|
|FTP|ftp|fs.ftp.FtpFileSystem|由FTP服务器支持的文件系统。|
|S3（本地）|s3n|fs.s3native.NativeS3FileSystem|基于Amazon S3的文件系统。|
|S3（基于块）|s3|fs.s3.NativeS3FileSystem|基于Amazon S3的文件系统，以块格式存储解决了S3的5GB文件大小的限制。|
　　Hadoop提供了许多文件系统的接口，用户可以使用URI方案选取合适的文件系统来实现交互。
## 2、HDFS基础概念
### 2.1 数据块（block）
- 
HDFS(Hadoop Distributed File System)默认的最基本的存储单位是64M的数据块。
- 
和普通文件系统相同的是，HDFS中的文件是被分成64M一块的数据块存储的。
- 
不同于普通文件系统的是，HDFS中，如果一个文件小于一个数据块的大小，并不占用整个数据块存储空间。
### 2.2 NameNode和DataNode
　　HDFS体系结构中有两类节点，一类是NameNode，又叫"元数据节点"；另一类是DataNode，又叫"数据节点"。这两类节点分别承担Master和Worker具体任务的执行节点。
　　1）元数据节点用来管理文件系统的命名空间
- 
其将所有的文件和文件夹的元数据保存在一个文件系统树中。
- 
这些信息也会在硬盘上保存成以下文件：命名空间镜像(namespace image)及修改日志(edit log)
- 
其还保存了一个文件包括哪些数据块，分布在哪些数据节点上。然而这些信息并不存储在硬盘上，而是在系统启动的时候从数据节点收集而成的。
　　2）数据节点是文件系统中真正存储数据的地方。
- 
客户端(client)或者元数据信息(namenode)可以向数据节点请求写入或者读出数据块。
- 
其周期性的向元数据节点回报其存储的数据块信息。
　　3）从元数据节点（secondary namenode）
- 
从元数据节点并不是元数据节点出现问题时候的备用节点，它和元数据节点负责不同的事情。
- 
其主要功能就是周期性将元数据节点的命名空间镜像文件和修改日志合并，以防日志文件过大。这点在下面会相信叙述。
- 
合并过后的命名空间镜像文件也在从元数据节点保存了一份，以防元数据节点失败的时候，可以恢复。
### 2.3 元数据节点目录结构
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927303969.png)
　　VERSION文件是java properties文件，保存了HDFS的版本号。
- 
layoutVersion是一个负整数，保存了HDFS的持续化在硬盘上的数据结构的格式版本号。
- 
namespaceID是文件系统的唯一标识符，是在文件系统初次格式化时生成的。
- 
cTime此处为0
- 
storageType表示此文件夹中保存的是元数据节点的数据结构。
> 
namespaceID=1232737062
cTime=0
storageType=NAME_NODE
layoutVersion=-18
### 2.4 数据节点的目录结构
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927316444.png)
- 
数据节点的VERSION文件格式如下：
> 
namespaceID=1232737062
storageID=DS-1640411682-127.0.1.1-50010-1254997319480
cTime=0
storageType=DATA_NODE
layoutVersion=-18
- 
blk_<id>保存的是HDFS的数据块，其中保存了具体的二进制数据。
- 
blk_<id>.meta保存的是数据块的属性信息：版本信息，类型信息，和checksum
- 
当一个目录中的数据块到达一定数量的时候，则创建子文件夹来保存数据块及数据块属性信息。
### 2.5 文件系统命名空间映像文件及修改日志
- 
当文件系统客户端(client)进行写操作时，首先把它记录在修改日志中(edit log)
- 
元数据节点在内存中保存了文件系统的元数据信息。在记录了修改日志后，元数据节点则修改内存中的数据结构。
- 
每次的写操作成功之前，修改日志都会同步(sync)到文件系统。
- 
fsimage文件，也即命名空间映像文件，是内存中的元数据在硬盘上的checkpoint，它是一种序列化的格式，并不能够在硬盘上直接修改。
- 
同数据的机制相似，当元数据节点失败时，则最新checkpoint的元数据信息从fsimage加载到内存中，然后逐一重新执行修改日志中的操作。
- 
从元数据节点就是用来帮助元数据节点将内存中的元数据信息checkpoint到硬盘上的
- 
checkpoint的过程如下：
- 
从元数据节点通知元数据节点生成新的日志文件，以后的日志都写到新的日志文件中。
- 
从元数据节点用http get从元数据节点获得fsimage文件及旧的日志文件。
- 
从元数据节点将fsimage文件加载到内存中，并执行日志文件中的操作，然后生成新的fsimage文件。
- 
从元数据节点奖新的fsimage文件用http post传回元数据节点
- 
元数据节点可以将旧的fsimage文件及旧的日志文件，换为新的fsimage文件和新的日志文件(第一步生成的)，然后更新fstime文件，写入此次checkpoint的时间。
- 
这样元数据节点中的fsimage文件保存了最新的checkpoint的元数据信息，日志文件也重新开始，不会变的很大了。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927313379.jpg)
## 3、HDFS体系结构
　　HDFS是一个主/从（Mater/Slave）体系结构，从最终用户的角度来看，它就像传统的文件系统一样，可以通过目录路径对文件执行CRUD（Create、Read、Update和Delete）操作。但由于分布式存储的性质，HDFS集群拥有一个NameNode和一些DataNode。NameNode管理文件系统的元数据，DataNode存储实际的数据。客户端通过同NameNode和DataNodes的交互访问文件系统。客户端联系NameNode以获取文件的元数据，而真正的文件I/O操作是直接和DataNode进行交互的。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927319443.jpg)
图3.1 HDFS总体结构示意图
　　1）NameNode、DataNode和Client
- 
NameNode可以看作是分布式文件系统中的管理者，主要负责管理文件系统的命名空间、集群配置信息和存储块的复制等。NameNode会将文件系统的Meta-data存储在内存中，这些信息主要包括了文件信息、每一个文件对应的文件块的信息和每一个文件块在DataNode的信息等。
- 
DataNode是文件存储的基本单元，它将Block存储在本地文件系统中，保存了Block的Meta-data，同时周期性地将所有存在的Block信息发送给NameNode。
- 
Client就是需要获取分布式文件系统文件的应用程序。
　　2）文件写入
- 
Client向NameNode发起文件写入的请求。
- 
NameNode根据文件大小和文件块配置情况，返回给Client它所管理部分DataNode的信息。
- 
Client将文件划分为多个Block，根据DataNode的地址信息，按顺序写入到每一个DataNode块中。
　　3）文件读取
- 
Client向NameNode发起文件读取的请求。
- 
NameNode返回文件存储的DataNode的信息。
- 
Client读取文件信息。
　　HDFS典型的部署是在一个专门的机器上运行NameNode，集群中的其他机器各运行一个DataNode；也可以在运行NameNode的机器上同时运行DataNode，或者一台机器上运行多个DataNode。一个集群只有一个NameNode的设计大大简化了系统架构。
## 4、HDFS的优缺点
### 4.1 HDFS的优点
　　1）处理超大文件
　　这里的超大文件通常是指百MB、设置数百TB大小的文件。目前在实际应用中，HDFS已经能用来存储管理PB级的数据了。
　　2）流式的访问数据
　　HDFS的设计建立在更多地响应"一次写入、多次读写"任务的基础上。这意味着一个数据集一旦由数据源生成，就会被复制分发到不同的存储节点中，然后响应各种各样的数据分析任务请求。在多数情况下，分析任务都会涉及数据集中的大部分数据，也就是说，对HDFS来说，请求读取整个数据集要比读取一条记录更加高效。
　　3）运行于廉价的商用机器集群上
　　Hadoop设计对硬件需求比较低，只须运行在低廉的商用硬件集群上，而无需昂贵的高可用性机器上。廉价的商用机也就意味着大型集群中出现节点故障情况的概率非常高。这就要求设计HDFS时要充分考虑数据的可靠性，安全性及高可用性。
### 4.2 HDFS的缺点
　　1）不适合低延迟数据访问
　　如果要处理一些用户要求时间比较短的低延迟应用请求，则HDFS不适合。HDFS是为了处理大型数据集分析任务的，主要是为达到高的数据吞吐量而设计的，这就可能要求以高延迟作为代价。
　　改进策略：对于那些有低延时要求的应用程序，HBase是一个更好的选择。通过上层数据管理项目来尽可能地弥补这个不足。在性能上有了很大的提升，它的口号就是goes
 real time。使用缓存或多master设计可以降低client的数据请求压力，以减少延时。还有就是对HDFS系统内部的修改，这就得权衡大吞吐量与低延时了，HDFS不是万能的银弹。
　　2）无法高效存储大量小文件
　　因为Namenode把文件系统的元数据放置在内存中，所以文件系统所能容纳的文件数目是由Namenode的内存大小来决定。一般来说，每一个文件、文件夹和Block需要占据150字节左右的空间，所以，如果你有100万个文件，每一个占据一个Block，你就至少需要300MB内存。当前来说，数百万的文件还是可行的，当扩展到数十亿时，对于当前的硬件水平来说就没法实现了。还有一个问题就是，因为Map
 task的数量是由splits来决定的，所以用MR处理大量的小文件时，就会产生过多的Maptask，线程管理开销将会增加作业时间。举个例子，处理10000M的文件，若每个split为1M，那就会有10000个Maptasks，会有很大的线程开销；若每个split为100M，则只有100个Maptasks，每个Maptask将会有更多的事情做，而线程的管理开销也将减小很多。
　　改进策略：要想让HDFS能处理好小文件，有不少方法。
- 
利用SequenceFile、MapFile、Har等方式归档小文件，这个方法的原理就是把小文件归档起来管理，HBase就是基于此的。对于这种方法，如果想找回原来的小文件内容，那就必须得知道与归档文件的映射关系。
- 
横向扩展，一个Hadoop集群能管理的小文件有限，那就把几个Hadoop集群拖在一个虚拟服务器后面，形成一个大的Hadoop集群。google也是这么干过的。
- 
多Master设计，这个作用显而易见了。正在研发中的GFS II也要改为分布式多Master设计，还支持Master的Failover，而且Block大小改为1M，有意要调优处理小文件啊。
- 
附带个Alibaba DFS的设计，也是多Master设计，它把Metadata的映射存储和管理分开了，由多个Metadata存储节点和一个查询Master节点组成。
　　3）不支持多用户写入及任意修改文件
　　在HDFS的一个文件中只有一个写入者，而且写操作只能在文件末尾完成，即只能执行追加操作。目前HDFS还不支持多个用户对同一文件的写操作，以及在文件任意位置进行修改。
## 5、HDFS常用操作
　　先说一下"hadoop fs 和hadoop
 dfs的区别"，看两本Hadoop书上各有用到，但效果一样，求证与网络发现下面一解释比较中肯。
　　粗略的讲，fs是个比较抽象的层面，在分布式环境中，fs就是dfs，但在本地环境中，fs是local file system，这个时候dfs就不能用。
### 5.1 文件操作
　　1）列出HDFS文件
　　此处为你展示如何通过"-ls"命令列出HDFS下的文件：
> 
hadoop fs -ls
　　执行结果如图5-1-1所示。在这里需要注意：在HDFS中未带参数的"-ls"命名没有返回任何值，它默认返回HDFS的"home"目录下的内容。在HDFS中，没有当前目录这样一个概念，也没有cd这个命令。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927326835.png)
图5-1-1 列出HDFS文件
　　2）列出HDFS目录下某个文档中的文件
　　此处为你展示如何通过"-ls 文件名"命令浏览HDFS下名为"input"的文档中文件：
> 
hadoop fs –ls input
　　执行结果如图5-1-2所示。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927334261.png)
图5-1-2 列出HDFS下名为input的文档下的文件
　　3）上传文件到HDFS
　　此处为你展示如何通过"-put 文件1 文件2"命令将"Master.Hadoop"机器下的"/home/hadoop"目录下的file文件上传到HDFS上并重命名为test：
> 
hadoop fs –put ~/file test
　　执行结果如图5-1-3所示。在执行"-put"时只有两种可能，即是执行成功和执行失败。在上传文件时，文件首先复制到DataNode上。只有所有的DataNode都成功接收完数据，文件上传才是成功的。其他情况（如文件上传终端等）对HDFS来说都是做了无用功。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927345524.png)
图5-1-3 成功上传file到HDFS
　　4）将HDFS中文件复制到本地系统中
　　此处为你展示如何通过"-get 文件1 文件2"命令将HDFS中的"output"文件复制到本地系统并命名为"getout"。
> 
hadoop fs –get output getout
　　执行结果如图5-1-4所示。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927362295.png)
图5-1-4 成功将HDFS中output文件复制到本地系统
　　备注：与"-put"命令一样，"-get"操作既可以操作文件，也可以操作目录。
　　5）删除HDFS下的文档
　　此处为你展示如何通过"-rmr 文件"命令删除HDFS下名为"newoutput"的文档：
> 
hadoop fs –rmr newoutput
　　执行结果如图5-1-5所示。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927373242.png)
图5-1-5 成功删除HDFS下的newoutput文档
　　6）查看HDFS下某个文件
　　此处为你展示如何通过"-cat 文件"命令查看HDFS下input文件中内容：
> 
hadoop fs -cat input/*
　　执行结果如图5-1-6所示。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927389273.png)
图5-1-6 HDFS下input文件的内容
　　"hadoop fs"的命令远不止这些，本小节介绍的命令已可以在HDFS上完成大多数常规操作。对于其他操作，可以通过"-help commandName"命令所列出的清单来进一步学习与探索。
### 5.2 管理与更新
　　1）报告HDFS的基本统计情况
　　此处为你展示通过"-report"命令如何查看HDFS的基本统计信息：
> 
hadoop dfsadmin -report
　　执行结果如图5-2-1所示。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927402106.png)
图5-2-1 HDFS基本统计信息
　　2）退出安全模式
　　NameNode在启动时会自动进入安全模式。安全模式是NameNode的一种状态，在这个阶段，文件系统不允许有任何修改。安全模式的目的是在系统启动时检查各个DataNode上数据块的有效性，同时根据策略对数据块进行必要的复制或删除，当数据块最小百分比数满足的最小副本数条件时，会自动退出安全模式。
　　系统显示"Name node is in safe mode"，说明系统正处于安全模式，这时只需要等待17秒即可，也可以通过下面的命令退出安全模式：
> 
hadoop dfsadmin –safemode enter
　　成功退出安全模式结果如图5-2-2所示。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927402912.png)
图5-2-2 成功退出安全模式
　　3）进入安全模式
　　在必要情况下，可以通过以下命令把HDFS置于安全模式：
> 
hadoop dfsadmin –safemode enter
　　执行结果如图5-2-3所示。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927407024.png)
图5-2-3 进入HDFS安全模式
　　4）添加节点
　　可扩展性是HDFS的一个重要特性，向HDFS集群中添加节点是很容易实现的。添加一个新的DataNode节点，首先在新加节点上安装好Hadoop，要和NameNode使用相同的配置（可以直接从NameNode复制），修改"/usr/hadoop/conf/master"文件，加入NameNode主机名。然后在NameNode节点上修改"/usr/hadoop/conf/slaves"文件，加入新节点主机名，再建立到新加点无密码的SSH连接，运行启动命令：
> 
start-all.sh
　　5）负载均衡
　　HDFS的数据在各个DataNode中的分布肯能很不均匀，尤其是在DataNode节点出现故障或新增DataNode节点时。新增数据块时NameNode对DataNode节点的选择策略也有可能导致数据块分布的不均匀。用户可以使用命令重新平衡DataNode上的数据块的分布：
> 
start-balancer.sh
　　执行命令前，DataNode节点上数据分布情况如图5-2-4所示。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927412846.png)
　　负载均衡完毕后，DataNode节点上数据的分布情况如图5-2-5所示。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/20120528092741305.png)
　　执行负载均衡命令如图5-2-6所示。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927435156.png)
## 6、HDFS API详解
　　Hadoop中关于文件操作类基本上全部是在"org.apache.hadoop.fs"包中，这些API能够支持的操作包含：打开文件，读写文件，删除文件等。
Hadoop类库中最终面向用户提供的接口类是FileSystem，该类是个抽象类，只能通过来类的get方法得到具体类。get方法存在几个重载版本，常用的是这个：
> 
static FileSystem get(Configuration conf);
　　该类封装了几乎所有的文件操作，例如mkdir，delete等。综上基本上可以得出操作文件的程序库框架：
> 
operator()
{
    得到Configuration对象
    得到FileSystem对象
    进行文件操作
}
### 6.1 上传本地文件
　　通过"FileSystem.copyFromLocalFile（Path src，Patch
 dst）"可将本地文件上传到HDFS的制定位置上，其中src和dst均为文件的完整路径。具体事例如下：
> 
package com.hebut.file;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
publicclass CopyFile
 {
publicstaticvoid main(String[]
 args) throws Exception
 {
        Configuration conf=new Configuration();
       
 FileSystem hdfs=FileSystem.get(conf);
//本地文件
        Path src =new Path("D:\\HebutWinOS");
//HDFS为止
        Path dst =new Path("/");
       
 hdfs.copyFromLocalFile(src, dst);
        System.out.println("Upload
 to"+conf.get("fs.default.name"));
       
 FileStatus files[]=hdfs.listStatus(dst);
for(FileStatus
 file:files){
            System.out.println(file.getPath());
       
 }
   
 }
}
　　运行结果可以通过控制台、项目浏览器和SecureCRT查看，如图6-1-1、图6-1-2、图6-1-3所示。
　　1）控制台结果
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/20120528092743107.png)
图6-1-1 运行结果（1）
　　2）项目浏览器
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927434534.png)
图6-1-2 运行结果（2）
　　3）SecureCRT结果
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927445515.png)
图6-1-3 运行结果（3）
### 6.2 创建HDFS文件
　　通过"FileSystem.create（Path f）"可在HDFS上创建文件，其中f为文件的完整路径。具体实现如下：
> 
package com.hebut.file;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
publicclass CreateFile
 {
publicstaticvoid main(String[]
 args) throws Exception
 {
        Configuration conf=new Configuration();
       
 FileSystem hdfs=FileSystem.get(conf);
byte[]
 buff="hello hadoop world!\n".getBytes();
        Path dfs=new Path("/test");
       
 FSDataOutputStream outputStream=hdfs.create(dfs);
        outputStream.write(buff,0,buff.length);
   
 }
}
　　运行结果如图6-2-1和图6-2-2所示。
　　1）项目浏览器
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927466429.png)
图6-2-1 运行结果（1）
　　2）SecureCRT结果
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927473822.png)
图6-2-2 运行结果（2）
### 6.3 创建HDFS目录
　　通过"FileSystem.mkdirs（Path f）"可在HDFS上创建文件夹，其中f为文件夹的完整路径。具体实现如下：
> 
package com.hebut.dir;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
publicclass CreateDir
 {
publicstaticvoid main(String[]
 args) throws Exception{
        Configuration conf=new Configuration();
       
 FileSystem hdfs=FileSystem.get(conf);
        Path dfs=new Path("/TestDir");
       
 hdfs.mkdirs(dfs);
    }
}
　　运行结果如图6-3-1和图6-3-2所示。
　　1）项目浏览器
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927476297.png)
图6-3-1 运行结果（1）
　　2）SecureCRT结果
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927486264.png)
图6-3-2 运行结果（2）
### 6.4 重命名HDFS文件
　　通过"FileSystem.rename（Path src，Path dst）"可为指定的HDFS文件重命名，其中src和dst均为文件的完整路径。具体实现如下：
> 
package com.hebut.file;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
publicclassRename{
publicstaticvoid main(String[]
 args) throws Exception
 {
        Configuration conf=new Configuration();
       
 FileSystem hdfs=FileSystem.get(conf);
       
 Path frpaht=new Path("/test");    //旧的文件名
       
 Path topath=new Path("/test1");    //新的文件名
boolean isRename=hdfs.rename(frpaht,
 topath);
       
 String result=isRename?"成功":"失败";
       
 System.out.println("文件重命名结果为："+result);
    }
}
　　运行结果如图6-4-1和图6-4-2所示。
　　1）项目浏览器
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927483200.png)
图6-4-1 运行结果（1）
2）SecureCRT结果
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927504953.png)
图6-4-2 运行结果（2）
### 6.5 删除HDFS上的文件
　　通过"FileSystem.delete（Path f，Boolean recursive）"可删除指定的HDFS文件，其中f为需要删除文件的完整路径，recuresive用来确定是否进行递归删除。具体实现如下：
> 
package com.hebut.file;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
publicclass DeleteFile
 {
publicstaticvoid main(String[]
 args) throws Exception
 {
        Configuration conf=new Configuration();
       
 FileSystem hdfs=FileSystem.get(conf);
        Path delef=new Path("/test1");
boolean isDeleted=hdfs.delete(delef,false);
//递归删除
//boolean
 isDeleted=hdfs.delete(delef,true);
        System.out.println("Delete?"+isDeleted);
   
 }
}
　　运行结果如图6-5-1和图6-5-2所示。
　　1）控制台结果
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927507395.png)
图6-5-1 运行结果（1）
2）项目浏览器
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927519871.png)
图6-5-2 运行结果（2）
### 6.6 删除HDFS上的目录
　　同删除文件代码一样，只是换成删除目录路径即可，如果目录下有文件，要进行递归删除。
### 6.7 查看某个HDFS文件是否存在
　　通过"FileSystem.exists（Path f）"可查看指定HDFS文件是否存在，其中f为文件的完整路径。具体实现如下：
> 
package com.hebut.file;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
publicclass CheckFile
 {
publicstaticvoid main(String[]
 args) throws Exception
 {
        Configuration conf=new Configuration();
       
 FileSystem hdfs=FileSystem.get(conf);
        Path findf=new Path("/test1");
boolean isExists=hdfs.exists(findf);
        System.out.println("Exist?"+isExists);
   
 }
}
　　运行结果如图6-7-1和图6-7-2所示。
　　1）控制台结果
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927519838.png)
图6-7-1 运行结果（1）
　　2）项目浏览器
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927514821.png)
图6-7-2 运行结果（2）
### 6.8 查看HDFS文件的最后修改时间
　　通过"FileSystem.getModificationTime()"可查看指定HDFS文件的修改时间。具体实现如下：
> 
package com.hebut.file;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
publicclass GetLTime
 {
publicstaticvoid main(String[]
 args) throws Exception
 {
        Configuration conf=new Configuration();
       
 FileSystem hdfs=FileSystem.get(conf);
        Path fpath =new Path("/user/hadoop/test/file1.txt");
       
 FileStatus fileStatus=hdfs.getFileStatus(fpath);
long modiTime=fileStatus.getModificationTime();
       
 System.out.println("file1.txt的修改时间是"+modiTime);
   
 }
}
　　运行结果如图6-8-1所示。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927525246.png)
图6-8-1 控制台结果
### 6.9 读取HDFS某个目录下的所有文件
　　通过"FileStatus.getPath（）"可查看指定HDFS中某个目录下所有文件。具体实现如下：
> 
package com.hebut.file;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
publicclass ListAllFile
 {
publicstaticvoid main(String[]
 args) throws Exception
 {
        Configuration conf=newConfiguration();
       
 FileSystem hdfs=FileSystem.get(conf);
        Path listf =new Path("/user/hadoop/test");
       
 FileStatus stats[]=hdfs.listStatus(listf);
for(int i
 = 0; i < stats.length; ++i)
　　　　
 {
　　　　　　 System.out.println(stats[i].getPath().toString());
　　　　
 }
       
 hdfs.close();
   
 }
}
　　运行结果如图6-9-1和图6-9-2所示。
　　1）控制台结果
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927543129.png)
图6-9-1 运行结果（1）
　　2）项目浏览器
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927541983.png)
图6-9-2 运行结果（2）
### 6.10 查找某个文件在HDFS集群的位置
　　通过"FileSystem.getFileBlockLocation（FileStatus
 file，long start，long len）"可查找指定文件在HDFS集群上的位置，其中file为文件的完整路径，start和len来标识查找文件的路径。具体实现如下：
> 
package com.hebut.file;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.BlockLocation;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
publicclass FileLoc
 {
publicstaticvoid main(String[]
 args) throws Exception
 {
        Configuration conf=new Configuration();
       
 FileSystem hdfs=FileSystem.get(conf);
        Path fpath=new Path("/user/hadoop/cygwin");
       
 FileStatus filestatus = hdfs.getFileStatus(fpath);
        BlockLocation[] blkLocations = hdfs.getFileBlockLocations(filestatus,
 0, filestatus.getLen());
int blockLen
 = blkLocations.length;
for(int i=0;i<blockLen;i++){
           
 String[] hosts = blkLocations[i].getHosts();
            System.out.println("block_"+i+"_location:"+hosts[0]);
       
 }
   
 }
}
　　运行结果如图6-10-1和6.10.2所示。
　　1）控制台结果
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927559408.png)
图6-10-1 运行结果（1）
　　2）项目浏览器
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927552963.png)
图6-10-2 运行结果（2）
### 6.11 获取HDFS集群上所有节点名称信息
　　通过"DatanodeInfo.getHostName（）"可获取HDFS集群上的所有节点名称。具体实现如下：
> 
package com.hebut.file;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.hdfs.DistributedFileSystem;
import org.apache.hadoop.hdfs.protocol.DatanodeInfo;
publicclass GetList
 {
publicstaticvoid main(String[]
 args) throws Exception
 {
        Configuration conf=new Configuration();
       
 FileSystem fs=FileSystem.get(conf);
       
 DistributedFileSystem hdfs = (DistributedFileSystem)fs;
       
 DatanodeInfo[] dataNodeStats = hdfs.getDataNodeStats();
for(int i=0;i<dataNodeStats.length;i++){
            System.out.println("DataNode_"+i+"_Name:"+dataNodeStats[i].getHostName());
       
 }
   
 }
}
　　运行结果如图6-11-1所示。
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927564326.png)
图6-11-1 控制台结果
## 7、HDFS的读写数据流
### 7.1 文件的读取剖析
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927578621.png)
　　文件读取的过程如下：
　　1）解释一
- 
客户端(client)用FileSystem的open()函数打开文件。
- 
DistributedFileSystem用RPC调用元数据节点，得到文件的数据块信息。
- 
对于每一个数据块，元数据节点返回保存数据块的数据节点的地址。
- 
DistributedFileSystem返回FSDataInputStream给客户端，用来读取数据。
- 
客户端调用stream的read()函数开始读取数据。
- 
DFSInputStream连接保存此文件第一个数据块的最近的数据节点。
- 
Data从数据节点读到客户端(client)。
- 
当此数据块读取完毕时，DFSInputStream关闭和此数据节点的连接，然后连接此文件下一个数据块的最近的数据节点。
- 
当客户端读取完毕数据的时候，调用FSDataInputStream的close函数。
- 
在读取数据的过程中，如果客户端在与数据节点通信出现错误，则尝试连接包含此数据块的下一个数据节点。
- 
失败的数据节点将被记录，以后不再连接。
　　2）解释二
- 
使用HDFS提供的客户端开发库，向远程的Namenode发起RPC请求；
- 
Namenode会视情况返回文件的部分或者全部block列表，对于每个block，Namenode都会返回有该block拷贝的datanode地址；
- 
客户端开发库会选取离客户端最接近的datanode来读取block；
- 
读取完当前block的数据后，关闭与当前的datanode连接，并为读取下一个block寻找最佳的datanode；
- 
当读完列表的block后，且文件读取还没有结束，客户端开发库会继续向Namenode获取下一批的block列表。
- 
读取完一个block都会进行checksum验证，如果读取datanode时出现错误，客户端会通知Namenode，然后再从下一个拥有该block拷贝的datanode继续读。
### 7.2 文件的写入剖析
![](http://images.cnblogs.com/cnblogs_com/xia520pi/201205/201205280927583538.png)
　　写入文件的过程比读取较为复杂：
　　1）解释一
- 
客户端调用create()来创建文件
- 
DistributedFileSystem用RPC调用元数据节点，在文件系统的命名空间中创建一个新的文件。
- 
元数据节点首先确定文件原来不存在，并且客户端有创建文件的权限，然后创建新文件。
- 
DistributedFileSystem返回DFSOutputStream，客户端用于写数据。
- 
客户端开始写入数据，DFSOutputStream将数据分成块，写入data queue。
- 
Data queue由Data Streamer读取，并通知元数据节点分配数据节点，用来存储数据块(每块默认复制3块)。分配的数据节点放在一个pipeline里。
- 
Data Streamer将数据块写入pipeline中的第一个数据节点。第一个数据节点将数据块发送给第二个数据节点。第二个数据节点将数据发送给第三个数据节点。
- 
DFSOutputStream为发出去的数据块保存了ack queue，等待pipeline中的数据节点告知数据已经写入成功。
- 
如果数据节点在写入的过程中失败：
- 
关闭pipeline，将ack queue中的数据块放入data queue的开始。
- 
当前的数据块在已经写入的数据节点中被元数据节点赋予新的标示，则错误节点重启后能够察觉其数据块是过时的，会被删除。
- 
失败的数据节点从pipeline中移除，另外的数据块则写入pipeline中的另外两个数据节点。
- 
元数据节点则被通知此数据块是复制块数不足，将来会再创建第三份备份。
- 
当客户端结束写入数据，则调用stream的close函数。此操作将所有的数据块写入pipeline中的数据节点，并等待ack queue返回成功。最后通知元数据节点写入完毕。
　　2）解释二
- 
使用HDFS提供的客户端开发库，向远程的Namenode发起RPC请求；
- 
Namenode会检查要创建的文件是否已经存在，创建者是否有权限进行操作，成功则会为文件创建一个记录，否则会让客户端抛出异常；
- 
当客户端开始写入文件的时候，开发库会将文件切分成多个packets，并在内部以"data queue"的形式管理这些packets，并向Namenode申请新的blocks，获取用来存储replicas的合适的datanodes列表，列表的大小根据在Namenode中对replication的设置而定。
- 
开始以pipeline（管道）的形式将packet写入所有的replicas中。开发库把packet以流的方式写入第一个datanode，该datanode把该packet存储之后，再将其传递给在此pipeline中的下一个datanode，直到最后一个datanode，这种写数据的方式呈流水线的形式。
- 
最后一个datanode成功存储之后会返回一个ack packet，在pipeline里传递至客户端，在客户端的开发库内部维护着"ack queue"，成功收到datanode返回的ack packet后会从"ack queue"移除相应的packet。
- 
如果传输过程中，有某个datanode出现了故障，那么当前的pipeline会被关闭，出现故障的datanode会从当前的pipeline中移除，剩余的block会继续剩下的datanode中继续以pipeline的形式传输，同时Namenode会分配一个新的datanode，保持replicas设定的数量。
libhdfs is a JNI based C api for Hadoop’s DFS.
It provides C apis to a subset of the HDFS APIs to manipulate DFS files and the filesystem.
libhdfs is part of the hadoop distribution and comes pre-compiled in ${HADOOP_HOME}/libhdfs/libhdfs.so .
API简介
    建立、关闭与HDFS连接：hdfsConnect()、hdfsConnectAsUser()、hdfsDisconnect()。hdfsConnect()实际上是直接调用hdfsConnectAsUser。
    打开、关闭HDFS文件：hdfsOpenFile()、hdfsCloseFile()。当用hdfsOpenFile()创建文件时，可以指定replication和blocksize参数。写打开一个文件时，隐含O_TRUNC标志，文件会被截断，写入是从文件头开始的。
    读HDFS文件：hdfsRead()、hdfsPread()。两个函数都有可能返回少于用户要求的字节数，此时可以再次调用这两个函数读入剩下的部分（类似APUE中的readn实现）；只有在两个函数返回零时，我们才能断定到了文件末尾。
    写HDFS文件：hdfsWrite()。HDFS不支持随机写，只能是从文件头顺序写入。
    查询HDFS文件信息：hdfsGetPathInfo()
    查询和设置HDFS文件读写偏移量：hdfsSeek()、hdfsTell()
    查询数据块所在节点信息：hdfsGetHosts()。返回一个或多个数据块所在数据节点的信息，一个数据块可能存在多个数据节点上。
    libhdfs中的函数是通过jni调用JAVA虚拟机，在虚拟机中构造对应的HDFS的JAVA类，然后反射调用该类的功能函数。总会发生JVM和程序之间内存拷贝的动作，性能方面值得注意。
**  HDFS不支持多个客户端同时写入的操作，无文件或是记录锁的概念。**
**建议只有超大文件才应该考虑放在HDFS上，而且最好对文件的访问是写一次，读多次。小文件不应该考虑放在HDFS上，得不偿失！**
测试1：
#include “hdfs.h”
int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, “Usage: hdfs_read \n”);
    exit(-1);
    }
    hdfsFS fs = hdfsConnect(“default”, 0);
    if (!fs) {
        fprintf(stderr, “Oops! Failed to connect to hdfs!\n”);
        exit(-1);
    }
    const char* rfile = argv[1];
    tSize fileTotalSize = strtoul(argv[2], NULL, 10);
    tSize bufferSize = strtoul(argv[3], NULL, 10);
    hdfsFile readFile = hdfsOpenFile(fs, rfile, O_RDONLY, bufferSize, 0, 0);
    if (!readFile) {
        fprintf(stderr, “Failed to open %s for writing!\n”, rfile);
        exit(-2);
    }
    // data to be written to the file
    char* buffer = malloc(sizeof(char) * bufferSize);
    if(buffer == NULL) {
        return -2;
    }
    // read from the file
    tSize curSize = bufferSize;
    for (; curSize == bufferSize;) {
        curSize = hdfsRead(fs, readFile, (void*)buffer, curSize);
    }
    free(buffer);
    hdfsCloseFile(fs, readFile);
    hdfsDisconnect(fs);
    return 0;
}
这段代码是从hdfs或者本地读文件。
hdfsFS fs = hdfsConnect(“default”, 0); 这个是默认的连接方式，是从本地读数据
hdfsFS fs = hdfsConnect(“192.168.0.128″, 9000); 这个是连接指定的hdfs
编译命令如下：
gcc hdfs_read.c -I${HADOOP_HOME}/src/c++/libhdfs -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -L${HADOOP_HOME}/libhdfs -lhdfs -o hdfs_read
[hadoop@hadoop00 sunwg]$ ./hdfs_read /sunwg/1.txt 4 10
tom
mary
测试2：
#include “hdfs.h”
int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, “Usage: hdfs_write \n”);
        exit(-1);
    }
    hdfsFS fs = hdfsConnect(“default”, 0);
    if (!fs) {
        fprintf(stderr, “Oops! Failed to connect to hdfs!\n”);
        exit(-1);
    }
    const char* writeFileName = argv[1];
    tSize fileTotalSize = strtoul(argv[2], NULL, 10);
    tSize bufferSize = strtoul(argv[3], NULL, 10);
    hdfsFile writeFile = hdfsOpenFile(fs, writeFileName, O_WRONLY, bufferSize, 0, 0);
    if (!writeFile) {
        fprintf(stderr, “Failed to open %s for writing!\n”, writeFileName);
        exit(-2);
    }
    // data to be written to the file
    char* buffer = malloc(sizeof(char) * bufferSize);
    if(buffer == NULL) {
        return -2;
    }
    int i = 0;
    for (i=0; i < bufferSize; ++i) {
        buffer[i] = 'a' + (i%26);
    }
    // write to the file
    tSize nrRemaining;
    for (nrRemaining = fileTotalSize; nrRemaining > 0; nrRemaining -= bufferSize ) {
        int curSize = ( bufferSize < nrRemaining ) ? bufferSize : (int)nrRemaining;
        hdfsWrite(fs, writeFile, (void*)buffer, curSize);
    }
    free(buffer);
    hdfsCloseFile(fs, writeFile);
    hdfsDisconnect(fs);
    return 0;
}
这段代码是向hdfs或者本地写文件。
hdfsFS fs = hdfsConnect("default", 0); 这个是默认的连接方式，是向本地写数据
hdfsFS fs = hdfsConnect("192.168.0.128", 9000); 这个是连接指定的hdfs
gcc hdfs_write.c -I${HADOOP_HOME}/src/c++/libhdfs -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -L${HADOOP_HOME}/libhdfs -lhdfs -o hdfs_write
所以的函数的详细介绍可以看hdfs.h这个头文件。
注意：在执行过程可能会报一些java包的错误，不要忘记把下面两个目录加到classpath中。
/home/hadoop/hadoop/*.jar
/home/hadoop/hadoop/lib/*.jar
