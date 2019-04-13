
# Secondary Namenode的Check point机制以及Namenode、Datanode工作机制说明 - Magician的博客 - CSDN博客


2017年12月28日 23:00:07[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：344所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)[前言：](#前言)
[1、NameNode的工作机制](#1namenode的工作机制)
[2、DataNode的工作机制](#2datanode的工作机制)
[3、Secondary Namenode的Check point机制](#3secondary-namenode的check-point机制)


# 目录
## 前言：
在说明checkpoint机制之前，先要了解下namenode、datanode的一些功能和职责。
## 1、NameNode的工作机制
问题场景：
1、集群启动后，可以查看文件，但是上传文件时报错，打开web页面可看到namenode正处于safemode状态，怎么处理？(safemode是由于当前的namenode和大部分的datanode未建立完全的链接<即namenode不清楚有些数据的存储位置造成的>)
2、Namenode服务器的磁盘故障导致namenode宕机，如何挽救集群及数据？(多种方法：1、早起设置集群时配置HA机制。2、在配置文件hdfs.site中将dfs.namenode.name.dir属性配置多个目录，就相当于对namenode数据做了多份备份，这样做的好处是当其中一个目录损坏了，也不会影响到Hadoop的元数据，特别是当其中一个目录是NFS（网络文件系统Network File System，NFS）之上，即使你这台机器损坏了，元数据也得到保存。)
3、Namenode是否可以有多个（是，后面问题根据实际情况来具体分析）？namenode内存要配置多大？namenode跟集群数据存储能力有关系吗？
4、文件的blocksize究竟调大好还是调小好？
……
诸如此类问题的回答，都需要基于对namenode自身的工作原理的深刻理解。（目前的话个人件建议只需要了解以下的主要内容就行了，后续如果遇到问题再深入了解相关内容）
**1. namenode的职责：**
负责客户端请求的响应；
元数据的管理（查询，修改）；
**2. 对元数据管理：**
namenode对数据的管理采用了三种存储形式：
A、内存元数据(NameSystem)：内存中有一份完整的元数据(内存meta data)。
B、磁盘元数据镜像文件：磁盘有一个“准完整”的元数据镜像（fsimage）文件(在namenode的工作目录中)。
C、数据操作日志文件（可通过日志运算出元数据）：用于衔接内存metadata和持久化元数据镜像fsimage之间的操作日志（edits文件）。
注：当客户端对hdfs中的文件进行新增或者修改操作，操作记录首先被记入edits日志文件中，当客户端操作成功后，相应的元数据会更新到内存meta.data中。
## 2、DataNode的工作机制
问题场景：
1、集群容量不够，怎么扩容？
2、如果有一些datanode宕机，该怎么办？
3、datanode明明已启动，但是集群中的可用datanode列表中就是没有，怎么办？
以上这类问题的解答，有赖于对datanode工作机制的深刻理解（目前的话个人件建议只需要了解以下的主要内容就行了，后续如果遇到问题再深入了解相关内容）
**1、Datanode工作职责：**
存储管理用户的文件块数据
定期向namenode汇报自身所持有的block信息（通过心跳信息上报）
（这点很重要，因为，当集群中发生某些block副本失效时，集群如何恢复block初始副本数量的问题）
## 3、Secondary Namenode的Check point机制
了解了以上知识，接下来再来理解元数据的checkpoint机制是怎么一回事的时候就很容易了。
**什么叫checkpoint?**
每隔一段时间，会由secondary namenode将namenode上积累的所有edits和一个最新的fsimage下载到本地，并加载到内存进行merge（这个过程称为checkpoint）
**checkpoint机制如下图所示：**
![这里写图片描述](https://img-blog.csdn.net/20171228224339535?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
客户端发送更新元数据的请求。
namenode在内存中更新元数据。
edits记录更新的操作日志
namenode滚动当前正在写的edits。
将生成的fsimage和edtis下载到secender namenode.
将下载到secender namenode的fsimage和edtis放入内存进行合并。
合并更新后的元数据dump成新的image文件
然后secender namenode请求namenode是否需要checkpoint
如果需要就将edtis正在写的那条日志进行滚动（即重复4步骤的操作）并将secender namenode生成的新的fsimage上传到namenode并重命名为fsimage。
**checkpoint的附带作用**
namenode和secondary namenode的工作目录存储结构完全相同，所以，当namenode故障退出需要重新恢复时，可以从secondary namenode的工作目录中将fsimage拷贝到namenode的工作目录，以恢复namenode的元数据。
最后重点陈述下**Secondary Namenode与Namenode的HA机制的区别**，有许多人认为Secondary Namenode其实就是namenode一个HA机制，其实不是这样的。Secondary Namenode相当于Namenode的一个热备份（比如电脑的一键还原），只是定期将合并的image文件通过checkpoint机制传给Namenode，并不具备HA机制中当namenode挂了之后直接接管namenode的能力。更深层来说，HA机制中备份的namenode是对namenode内存、edtis、image的实时的备份，而Secondary Namenode只是对image定时的备份。所以两者不是同一个东西。

