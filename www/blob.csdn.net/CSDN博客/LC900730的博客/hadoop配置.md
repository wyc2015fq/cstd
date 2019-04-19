# hadoop配置 - LC900730的博客 - CSDN博客
2017年11月12日 21:52:31[lc900730](https://me.csdn.net/LC900730)阅读数：121
## 配置项1
```xml
<property>
    <name>fs.defaultFS</name>
    <value>hdfs://mini1:9000</value>
</property>
```
hdfs://mini1:9000  nameNode+Port 
URI:  jdbc:mysql://localhost:3306 
schema 
hadoop要使用哪一种文件系统
## 配置项2
```xml
<property>
    <name>hadoop.tmp.dir</name>
    <value>/home/hadoop/hdpdata</value>
</property>
```
在hdfs-site.xml中配置 
//默认是3份
```xml
<property>
    <name>dfs.replication</name>
    <value>2</value>
</property>
```
在mapred-site.xml中配置 
//方法一个资源调度平台上跑，yarn负责分配内存等 
//不配置的话，默认是local，即本地模拟运行，不会在集群中运行
```xml
<property>
    <name>mapreduce.framework.name</name>
    <value>yarn</value>
</property>
```
在yarn-site.xml中配置 
//配置yarn的老大在哪里
```xml
<property>
    <name>yarn.resourcemanager.hostname</name>
    <value>mini1</value>
</property>
<property>
    <name>yarn.nodemanager.aux-services</name>
    <value>mapreduce_shuffle</value>
</property>
```
启动的时候，加一台就认一台。
./hadoop-daemon.sh start namenode 
在mini2或者3／4 启动dataNode
./hadoop-daemon.sh start datanode 
dataNode与nameNode怎么握手？ 
dataNode通过配置知道nameNode，协同工作。 
如果启动失败，则查看日志。
etc中Hadoop中slaves中,给自动化启动脚本用的。 
这个自动化脚本是start-dfs.sh和stop-dfs.sh?
start-dfs.sh中
本质上是这样ssh mini2 hadoop-daemon.sh  start  datanode
### 免密码登录
ssh-keygen
ssh-copy-id mini1 
ssh-copy-id mini2 
ssh-copy-id mini3 
ssh-copy-id mini4
