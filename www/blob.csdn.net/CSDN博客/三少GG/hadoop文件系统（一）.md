# hadoop文件系统（一） - 三少GG - CSDN博客
2014年01月17日 09:08:10[三少GG](https://me.csdn.net/scut1135)阅读数：835
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)

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

