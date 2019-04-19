# Hadoop集群搭建中的问题 - LC900730的博客 - CSDN博客
2017年11月19日 21:02:24[lc900730](https://me.csdn.net/LC900730)阅读数：154
## 节点启动正常，从节点失败
从连接主的时候出现问题，查看日志发现，slave在连接master的端口失败，retry了多次。原因：腾讯云hosts文件不支持配置本主机的外网ip，master只监听了127.0.0.1的9002端口，而slave连接的是master(外网ip)的9002端口，因此出现slave连接master失败
## 集群启动中配置顺序hadoop2.6.5
### core-site.xml
```xml
<property>
    <name>hadoop.tmp.dir</name>
    <value>file:/data/hadooptmp</value>
</property>
<property>
    <name>fs.defaultFS</name>
    //腾讯云主机上监听所有网卡9002端口
    <value>hdfs://0.0.0.0/9002</value>
</property>
```
### hdfs-site.xml
```xml
<property>
    <name>dfs.replication</name>
    <value>2</value>
</property>
<property>
    <name>dfs.namenode.name.dir</name>
    <value>file:/data/hadooptmp/dfs/name</value>
</property>
<property>
<name>dfs.datanode.data.dir</name>
<value>file:/data/hadooptmp/dfs/data</value>
</property> 
<property>
<name>dfs.secondary.http.address</name>
<value>192.168.1.152:50090</value>
</property>
```
### mapred-site.xml
```xml
<property>
    <name>mapreduce.framework.name</name>
    <value>yarn</value>
</property>
```
### mapred-site.xml
```xml
<property>
    <name>mapreduce.framework.name</name>
    <value>yarn</value>
</property>
```
### yarn-site.xml
```xml
<property>
    <name>yarn.resourcemanager.hostname</name>
    <value>master</value>
</property>
<property>
    <name>yarn.nodemanager.aux-services</name>
    <value>mapreduce_shuffle</value>
</property>
```
在master主机的hosts文件中，配置另外2台主机，可以通过slave脚本自动启动。
主机名不能带有下划线，如VM_56就是非法的，会提示invaid host:port
启动： 
首先格式化 
hadoop namenode -format 
hadoop-daemon.sh start namenode 
hadoop-daemon.sh start datanode
