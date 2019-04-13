
# HDFS读写数据的原理 - Magician的博客 - CSDN博客


2017年12月26日 22:04:08[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：1141所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)[1 概述](#1-概述)
[2 HDFS写数据流程](#2-hdfs写数据流程)
[3 HDFS读数据流程](#3-hdfs读数据流程)


# 目录
最近由于要准备面试，就把之前学过的东西好好整理下，权当是复习。
下面说下HDFS读写数据的原理。
## 1 概述
HDFS集群分为两大角色：NameNode、DataNode
NameNode负责管理整个文件系统的元数据
DataNode 负责管理用户的文件数据块
文件会按照固定的大小（blocksize）切成若干块后分布式存储在若干台datanode上
每一个文件块可以有多个副本，并存放在不同的datanode上
Datanode会定期向Namenode汇报自身所保存的文件block信息，而namenode则会负责保持文件的副本数量
HDFS的内部工作机制对客户端保持透明，客户端请求访问HDFS都是通过向namenode申请来进行
## 2 HDFS写数据流程
**2.1 概述**
客户端要向HDFS写数据，首先要跟namenode通信以确认可以写文件并获得接收文件block的datanode，然后，客户端按顺序将文件逐个block传递给相应datanode，并由接收到block的datanode负责向其他datanode复制block的副本
**2.2 详细步骤图**
![这里写图片描述](https://img-blog.csdn.net/20171226220016807?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**2.3 详细步骤解析**
1、根namenode通信请求上传文件，namenode检查目标文件是否已存在，父目录是否存在
2、namenode返回是否可以上传
3、client请求第一个 block该传输到哪些datanode服务器上
4、namenode返回3个datanode服务器ABC
5、client请求3台dn中的一台A上传数据（本质上是一个RPC调用，建立pipeline），A收到请求会继续调用B，然后B调用C，将真个pipeline建立完成，逐级返回客户端
6、client开始往A上传第一个block（先从磁盘读取数据放到一个本地内存缓存），以packet为单位，A收到一个packet就会传给B，B传给C；A每传一个packet会放入一个应答队列等待应答
7、当一个block传输完成之后，client再次请求namenode上传第二个block的服务器。
## 3 HDFS读数据流程
**3.1 概述**
客户端将要读取的文件路径发送给namenode，namenode获取文件的元信息（主要是block的存放位置信息）返回给客户端，客户端根据返回的信息找到相应datanode逐个获取文件的block并在客户端本地进行数据追加合并从而获得整个文件
**3.2 详细步骤图**
![这里写图片描述](https://img-blog.csdn.net/20171226220157390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171226220157390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**3.3 详细步骤解析**
1、跟namenode通信查询元数据，找到文件块所在的datanode服务器
2、挑选一台datanode（就近原则，然后随机）服务器，请求建立socket流
3、datanode开始发送数据（从磁盘里面读取数据放入流，以packet为单位来做校验）
4、客户端以packet为单位接收，先在本地缓存，然后写入目标文件

