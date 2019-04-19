# hadoop杂 - LC900730的博客 - CSDN博客
2017年11月06日 19:12:51[lc900730](https://me.csdn.net/LC900730)阅读数：144标签：[hadoop](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)
个人分类：[hadoop](https://blog.csdn.net/LC900730/article/category/7272965)
MapReduce输入的键和值并不是数据固有的属性，它们是由分析数据的人来选择的。
hdfs是提供数据存储的，mapreduce是方便数据计算的 
a.hdfs又对应namenode和datanode，namenode负责保存元数据的基本信息； 
    datanode之间存放数据本身 
b.mapreduce对应jobtracker负责分发任务，tasktracker负责执行具体任务 
c所以对应到master/slave架构，namenode和jobtracker就应该对应到master 
datanode和tasktracker就应该对应到slave
### 在apache中下载hadoop解压报错
可能是墙的原因，每次下载的文件大小不一致，也不完全。 
换镜像
wget [http://mirrors.hust.edu.cn/apache/common/hadoop-2.7.4/zookeeper-2.7.4.tar.gz](http://mirrors.hust.edu.cn/apache/common/hadoop-2.7.4/zookeeper-2.7.4.tar.gz)
根据经验，每个文件，索引目录以及块占据大约150个字节。因此hadoop存储上百万文件是可行的，但是上亿就不行。
### HDFS块
文件系统通过处理大小为一个磁盘块大小整数倍的数据块来运作这个磁盘。
一个因损坏或者机器故障而丢失的块会从其他地方复制到正常运行的机器上，以保证副本的数量回到正常水平。
