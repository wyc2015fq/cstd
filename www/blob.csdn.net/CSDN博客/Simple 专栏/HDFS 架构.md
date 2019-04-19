# HDFS  架构 - Simple 专栏 - CSDN博客
2015年01月27日 23:32:52[Simple_Zz](https://me.csdn.net/love284969214)阅读数：4066标签：[大数据																[hadoop																[HDFS](https://so.csdn.net/so/search/s.do?q=HDFS&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)
个人分类：[hadoop生态圈](https://blog.csdn.net/love284969214/article/category/7975457)
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
# HDFS架构设计
- HDFS 采用Master/Slave的架构来存储数据，这种架构主要由四个部分组成。
- 分别为HDFS Client、NameNode、DataNode和Secondary NameNode。
![HDFS架构图](https://img-blog.csdn.net/20180928104434899?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)HDFS架构图
### Client：客户端
- 文件切分。文件上传 HDFS 的时候，Client 将文件切分成 一个一个的Block，然后进行存储。
- 与 NameNode 交互，获取文件的位置信息。
- 与 DataNode 交互，读取或者写入数据。
- Client 提供一些命令来管理 HDFS，比如启动或者关闭HDFS。
- Client 可以通过一些命令来访问 HDFS。
### NameNode：master，它是一个主管、管理者。
- 管理 HDFS 的名称空间。
- 管理数据块（Block）映射信息
- 配置副本策略
- 处理客户端读写请求。
### DataNode：Slave，NameNode 下达命令，DataNode 执行实际的操作。
- 存储实际的数据块。
- 执行数据块的读/写操作。
### SecondaryNameNode：并非 NameNode 的热备。当NameNode 挂掉的时候，它并不能马上替换 NameNode 并提供服务。
- 辅助 NameNode，分担其工作量。
- 定期合并 fsimage和fsedits，并推送给NameNode。
- 在紧急情况下，可辅助恢复 NameNode。
