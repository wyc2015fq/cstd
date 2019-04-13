
# Hadoop基础 - Wesley's Blog - CSDN博客


2018年05月09日 20:06:13[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：46标签：[Hadoop																](https://so.csdn.net/so/search/s.do?q=Hadoop&t=blog)[大数据																](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Hadoop&t=blog)个人分类：[大数据																](https://blog.csdn.net/yinanmo5569/article/category/7652577)



## Hadoop是什么
开源的大数据框架
分布式计算的解决方案
Hadoop = HDFS(分布式文件系统) + MapReduce(分布式计算）
## HDFS
HDFS由一个NameNode，多个DataNode构成
普通的成百上千的机器
按TB甚至PB为单位的大量的数据
简单便捷的文件获取
### HDFS数据块
数据块是抽象块而非整个文件作为存储单元，默认大小为64MB，一般设置为128MB，备份X3
简化了存储系统的设计，适用于数据的备份，提升了容错能力与可用性
### NameNode
管理文件系统的命名空间，存放文件元数据
维护着文件系统的所有文件和目录，文件与数据块的映射
记录每个文件中各个块所在的数据节点的信息
### HDFS写流程
客户端向NameNode发起写数据请求
分块写入DataNode节点，DataNode自动完成副本备份
DataNode向NameNode汇报存储完成，NameNode通知客户端
![这里写图片描述](https://img-blog.csdn.net/20180509200503375?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### HDFS读流程
客户端向NameNode发起读数据请求
NameNode找出距离最近的DataNode节点信息
客户端从DataNode分块下载文件
![这里写图片描述](https://img-blog.csdn.net/20180509200514331?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### DataNode
存储并检索数据块
向NameNode更新所存储块的列表
### HDFS优缺点
优点：
适合大文件存储，支持TB,PB级的数据存储，并有副本策略
可以构建在廉价的机器上，并有一定的容错和恢复机制
支持流式数据访问，一次写入，多次读取最高效
缺点：
不适合大量小文件存储
不适合并发写入，不支持文件随机修改
不支持随机读等低延时的访问方式
## MapReduce和Yarn
### Yarn（Haoop2之后的资源管理器）
ResourceManager：分配和调度资源；启动并监控ApplicationMaster; 监控NodeManager
ApplicatonMaster：为MR类型的程序申请资源，并分配给内部任务；负责数据的切分；监控任务的执行及容错
NodeManager：管理单个节点的资源；处理来自ResourceManager的命令；处理来自ApplicationMaster的命令
### MapReduce
输入一个大文件，通过Split之后，将其分为多个分片
每个文件分片由单独的机器去处理，这就是Map方法
将各个机器计算的结果进行汇总并得到最终的结果，这就是Reduce方法
### 延伸思考
如何通过Hadoop存储小文件？
当有节点故障的时候，集群是如何继续提供服务的，如何读，如何写？
哪些是影响MapReduce性能的因素？
## Hadoop生态
![这里写图片描述](https://img-blog.csdn.net/20180509200530462?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### HBase简介
高可靠，高性能，面向列，可伸缩，实时读写的分布式数据库
利用HDFS作为其文件存储系统，支持MR程序读取数据
存储非结构化和半结构化数据
![这里写图片描述](https://img-blog.csdn.net/20180509200545787?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180509200545787?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- RowKey:数据唯一标识，按字典排序
- Column Family:列族，多个列的集合．最多不要超过３个
- TimeStamp时间戳:支持多个版本数据同时存在
### Spark简介
基于内存计算的大数据并行计算框架
抽象出分布式内存存储数据结构　弹性分布式数据集RDD
基于事件驱动，通过线程池复用线程提高性能
Spark是MapReduce的替代方案，兼容HDFS,HIVE等数据源

[
](https://img-blog.csdn.net/20180509200545787?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
