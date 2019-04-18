# Hadoop 部署之环境准备(一) - weixin_33985507的博客 - CSDN博客
2018年11月15日 14:31:55[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：16
# 一、软硬件规划
|**ID**|**主机类型**|**主机名**|**IP**|**应用软件**|**操作系统**|**硬件配置**|
|----|----|----|----|----|----|----|
|1|物理机|namenode01|192.168.1.200|jdk1.8.0_171、hadoop-2.7.3、hive-2.3.3、hbase-1.2.6、zookeeper-3.4.10、spark-2.3.1|CentOS7.4|6核CPU、8G内存、250存储|
|2|物理机|datanode01|192.168.1.201|jdk1.8.0_171、hadoop-2.7.3、hive-2.3.3、hbase-1.2.6、zookeeper-3.4.10、spark-2.3.1|CentOS7.4|6核CPU、8G内存、250存储|
|3|物理机|datanode02|192.168.1.202|jdk1.8.0_171、hadoop-2.7.3、hive-2.3.3、hbase-1.2.6、zookeeper-3.4.10、spark-2.3.1|CentOS7.4|6核CPU、8G内存、250存储|
|4|物理机|datanode03|192.168.1.203|jdk1.8.0_171、hadoop-2.7.3、hive-2.3.3、hbase-1.2.6、zookeeper-3.4.10、spark-2.3.1|CentOS7.4|6核CPU、8G内存、250存储|
我这里规划的角色不是太好，如果是四台机器，建议大家按照我们下面的分配
- **ZooKeeper**
ZooKeeper Server：namenode01
- **HDFS**
NameNode：namenode01
SecondaryNameNode：namenode01
DataNode：datanode[01-03]
- **YARN**
ResourceManager：namenode01
NodeManager：datanode[01-03]
- **HBase**
Master：namenode01
RegionServer：datanode[01-03]
- **Hive**
HiveServer2：namenode01
Hive Metastore Server：namenode01
- **Spark**
Master：namenode01
Worker：datanode[01-03]
# 二、主机名解析
修改四台服务器的主机名。
```
192.168.1.200 namenode01 hbase01 spark01 hive01 cluster1
192.168.1.201 datanode01 hbase02 spark02 zk01 mysql01
192.168.1.202 datanode02 hbase03 spark03 zk02
192.168.1.203 datanode03 hbase04 spark04 zk03
```
![Hadoop 部署之环境准备(一)](http://i2.51cto.com/images/blog/201812/07/9030ae4d5ad1579eb15057db20e428f6.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
# 三、配置 SSH 互信
在namenode01节点上面进行操作。
```
ssh-keygen -t rsa
ssh-copy-id namenode01
ssh-copy-id datanode01
ssh-copy-id datanode02
ssh-copy-id datanode03
```
# 四、创建用户
创建用户（4台机器相同操作）
`useradd hadoop && echo '123456' |passwd --stdin hadoop`
使用命令`visudo`设置用户权限（4台机器相同操作），添加如下内容：
`hadoop    ALL=(ALL)       ALL`
# 五、JDK 的安装
下载JDK1.8，可以选择下载rpm包，直接安装即可使用。
[https://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html](https://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)
