# Hadoop发展历程及重要核心模块解读 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年01月04日 00:08:36[boonya](https://me.csdn.net/boonya)阅读数：1488








本文内容及截图来自网络课堂。

**目录**

[Hadoop 四大模块演变](#Hadoop%20%E5%9B%9B%E5%A4%A7%E6%A8%A1%E5%9D%97%E6%BC%94%E5%8F%98)

[Hadoop HDFS](#Hadoop%20HDFS)

[Hadoop MapReduce](#Hadoop%20MapReduce)

[Hadoop YARN](#Hadoop%20YARN)

## Hadoop 四大模块演变

说明：

![](https://img-blog.csdnimg.cn/20190103235043456.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

版本特点：

![](https://img-blog.csdnimg.cn/20190103235104523.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/201901032351188.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

## Hadoop HDFS

HDFS:Hadoop Distributed File System——分布式文件系统

文件系统： 文件+存储block块。

**a.单机文件系统：**

windows:FAT16,FAT32,NTFS

linux:ext2/3/4,VFS

**b. 分布式文件系统**

多个服务器存储 文件系统

**c.三大组件**
- Namenode:

I.元数据：文件名、目录名、属性（生成时间、权限、副本）

II.文件名与block列表映射关系

III.block与Datanode列表映射关系
-  DataNode:

block块数据、校验和编码
- SecondaryNamenode

分担Namenode压力，合并编辑日志edits和镜像文件fsimage(因为合并操作需要占用很 大的系统资源，影响客户端使用。)，合并后将最终的镜像文件 fsimage返回给Namenode处理。

![](https://img-blog.csdnimg.cn/20190103235543176.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190103235556453.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)



## Hadoop MapReduce

——分布式计算



![](https://img-blog.csdnimg.cn/20190103235828313.png)



![](https://img-blog.csdnimg.cn/20190103235849956.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)



处理流程：

![](https://img-blog.csdnimg.cn/20190103235907109.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190103235923223.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)





## Hadoop YARN

——资源调度和任务管理

四大组件：
- ResourceManager (RM)

![](https://img-blog.csdnimg.cn/20190104000520897.png)
- NodeManager (NM)

![](https://img-blog.csdnimg.cn/20190104000534808.png)
- ApplicationManager(AM)

![](https://img-blog.csdnimg.cn/20190104000546436.png)
- Container

![](https://img-blog.csdnimg.cn/20190104000557374.png)

执行流程：

![](https://img-blog.csdnimg.cn/20190104000613600.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
- Client连接RM提交作业，RM给Client一个Job Id(包括:AplicationsManager和Resource Scheduler)
- RM中的ApplicationsManager 连接一个NM，让NM创建一个 AM客户端作业请求
- AM连接RM中ApplicationsManager 申请NodeManger
- AM去ResourceScheduler给Client的作业申请资源(CPU、内存、磁盘、网络等)
- AM连接NM，发送Client Job 作业程序和申请资源(CPU、内存、磁盘、网络等)
- NM启动Container进程运行Job的不同任务
- Container进程运行状态实时反馈给AM
- AM反馈任务状态信息给RM中的ApplicationsManager
- Client端可以连接RM或AM查询JBO的执行情况

注意：NM启动后回去RM上进行注册，会不断发送心跳，说明处于存活状态。











