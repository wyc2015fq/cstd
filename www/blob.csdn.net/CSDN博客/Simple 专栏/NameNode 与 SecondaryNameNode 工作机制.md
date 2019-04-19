# NameNode 与 SecondaryNameNode 工作机制 - Simple 专栏 - CSDN博客
2015年03月30日 16:38:42[Simple_Zz](https://me.csdn.net/love284969214)阅读数：401标签：[HDFS																[hadoop																[大数据](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=HDFS&t=blog)
个人分类：[hadoop生态圈](https://blog.csdn.net/love284969214/article/category/7975457)
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
## 分析思路：
- 首先，做个假设：如果元数据存储在 NameNode 节点的磁盘中，因为经常需要进行随机访问、响应客户请求，必然是效率过低。
- 因此，元数据需要存放在内存中。
- 但是，只存在内存中，一旦断电、宕机，元数据丢失，整个集群就无法工作了。
- 因此，必须在磁盘中有备份，在磁盘中备份就是 **fsimage**，存放在 NameNode 节点对应的磁盘中。
- 这样，又有新问题，当在内存中的元数据更新时，如果同时更新 **fsimage**，就会导致效率过低。
- 但是，如果不更新，就会发生一致性问题，一旦 NameNode 节点断电、宕机，就会产生数据丢失。
- 因此，引入 **edits **文件（只进行追加操作，效率很高）。
- 这样，每当元数据有 **添、删、改（只能追加，不能修改） **操作时，修改内存中的元数据并追加到 **edits **中。
- 这样，一旦 NameNode 节点断电、宕机，可以通过 **fsimage **和 **edits **的合并，来恢复元数据。
- 这样，又有新问题，如果长时间添加数据到 **edits **中，会导致该文件数据过大，效率降低。
- 这时，断电、宕机，恢复元数据需要的时间过长。
- 因此，需要定期进行 **fsimage **和 **edits **的合并。
- 这样，如果这个操作由 NameNode 节点完成，又会效率过低。
- 因此，引入一个新的节点 SecondaryNameNode，专门用于 **fsimage **和 **edits **的合并。
- 合并之后再把 **fsimage **传给 NameNode 。
- 以上就是 NameNode 与 SecondaryNameNode 的工作机制原理分析。
## NameNode  工作机制
![NameNode 工作机制](https://img-blog.csdn.net/20180930110014172?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)NameNode 工作机制
- 第一次启动namenode格式化后，创建 **fsimage **和 edits 文件。如果不是，直接加载编辑日志和镜像文件到内存。
- 客户端对元数据进行增、删、改（只能追加，不能修改）、的请求。
- NameNode 记录操作日志，更新滚动日志。
- NameNode 在内存中对数据进行增、删、改（只能追加，不能修改）。
## SecondaryNameNode 工作机制
![SecondaryNameNode 工作机制](https://img-blog.csdn.net/20181001160741144?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)SecondaryNameNode 工作机制
- SecondaryNameNode 询问 NameNode 是否需要 checkpoint 。直接带回 NameNode 是否检查结果。
- SecondaryNameNode 请求执行 checkpoint 。
- NameNode 滚动正在写的 edits 日志。
- 将滚动前的编辑日志和镜像文件拷贝到 SecondaryNameNode 。
- SecondaryNameNode 加载编辑日志和镜像文件到内存，并合并。
- 生成新的镜像文件 fsimage.chkpoint。
- 拷贝 fsimage.chkpoint 到 NameNode。
- NameNode 将 fsimage.chkpoint 重新命名成 fsimage。
### fsimage 与 edits
NameNode 被格式化之后，将在 /soft/hadoop/data/tmp/dfs/name/current 目录中产生如下文件。
```
fsimage_0000000000000000000
fsimage_0000000000000000000.md5
seen_txid
VERSION
```
- fsimage文件：HDFS文件系统元数据的一个永久性的检查点，其中包含HDFS文件系统的所有目录和文件 idnode 的序列化信息。 
- edits文件：存放HDFS文件系统的所有更新操作的路径，文件系统客户端执行的所有写操作首先会被记录到edits文件中。 
- seen_txid文件：保存的是一个数字，就是最后一个edits_的数字。
- 每次Namenode启动的时候都会将fsimage文件读入内存，并从00001开始到seen_txid中记录的数字依次执行每个edits里面的更新操作，保证内存中的元数据信息是最新的、同步的，可以看成Namenode启动的时候就将fsimage和edits文件进行了合并。
查看 fsimage 和 edits 命令
```bash
oiv  apply the offline fsimage viewer to an fsimage
oev  apply the offline edits viewer to an edits file
```
基本语法
```
hdfs oiv -p 文件类型 -i镜像文件 -o 转换后文件输出路径
hdfs oev -p 文件类型 -i编辑日志 -o 转换后文件输出路径
```
案例实操
```
hdfs oiv -p XML -i fsimage_0000000000000000025 -o /soft/hadoop/fsimage.xml
hdfs oev -p XML -i edits_0000000000000000012-0000000000000000013 -o /soft/hadoop/edits.xml
```
 VERSION 版本号具体解释
```
namespaceID=1933630176
clusterID=CID-1f2bf8d1-5ad2-4202-af1c-6713ab381175
cTime=0
storageType=NAME_NODE
blockpoolID=BP-97847618-192.168.10.102-1493726072779
layoutVersion=-63
```
- namespaceID在HDFS上，会有多个Namenode，所以不同Namenode的namespaceID是不同的，分别管理一组blockpoolID。
- clusterID集群id，全局唯一
- cTime属性标记了namenode存储系统的创建时间，对于刚刚格式化的存储系统，这个属性为0；但是在文件系统升级之后，该值会更新到新的时间戳。
- storageType属性说明该存储目录包含的是namenode的数据结构。
- blockpoolID：一个block pool id标识一个block pool，并且是跨集群的全局唯一。当一个新的Namespace被创建的时候(format过程的一部分)会创建并持久化一个唯一ID。在创建过程构建全局唯一的BlockPoolID比人为的配置更可靠一些。NN将BlockPoolID持久化到磁盘中，在后续的启动过程中，会再次load并使用。
- layoutVersion是一个负整数。通常只有HDFS增加新特性时才会更新这个版本号。
### chkpoint 检查时间参数设置
- 通常情况下，SecondaryNameNode每隔一小时执行一次。
- 一分钟检查一次操作次数，当操作次数达到1百万时，SecondaryNameNode执行一次。
```
hdfs-default.xml
<property>
    <name>dfs.namenode.checkpoint.period</name>
    <value>3600</value>
</property>
<property>
    <name>dfs.namenode.checkpoint.txns</name>
    <value>1000000</value>
    <description>操作动作次数</description>
</property>
<property>
    <name>dfs.namenode.checkpoint.check.period</name>
    <value>60</value>
    <description> 1分钟检查一次操作次数</description>
</property>
```
### 滚动编辑日志
正常情况HDFS文件系统有更新操作时，就会滚动编辑日志。也可以用命令强制滚动编辑日志。
1）滚动编辑日志（前提必须启动集群）
`[root@hadoop1 current]$ hdfs dfsadmin -rollEdits`
2）镜像文件什么时候产生
Namenode启动时加载镜像文件和编辑日志
