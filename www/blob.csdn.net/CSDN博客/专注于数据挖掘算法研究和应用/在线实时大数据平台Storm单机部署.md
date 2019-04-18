# 在线实时大数据平台Storm单机部署 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月11日 15:51:29[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2576
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









centos单机下部署storm，主要用于开发测试用。部署的IP地址用zoo1代替，要注意/etc/hosts下对应ip和主机名。



**1、Zookeeper部署**

Apache官网http://zookeeper.apache.org/下载zookeeper-3.4.8.tar.gz版本。

1）解压在/var/zookeeper-3.4.8目录下

#tar -xzvf/tmp/zookeeper-3.4.8.tar.gz

#cp -R /tmp/zookeeper-3.4.8/var

#rm -rf/tmp/zookeeper-3.4.8

2）建立zookeeper的data目录和集群myid文件

#mkdir -p/var/zookeeper-3.4.8/zkdata

#cd/var/zookeeper-3.4.8/zkdata

#echo 1>myid

3）配置conf/zoo.cfg文件

拷贝zookeeper-3.4.8的conf/zoo_sample.cfg并重命名为zoo.cfg

#cpconf/zoo_sample.cfgconf/zoo.cfg

#viconf/zoo.cfg

修改dataDir：

dataDir=/var/zookeeper-3.4.8/zkdata

增加集群服务器

server.1=zoo1:2888:3888

说明：

server.A=B:C:D是集群zk使用的。如果是单个zk，可不配置。

A - 是一个数字，表示这是第几号服务器。与/var/zookeeper-3.4.8/zkdata下的myid文件内容一致

B - 是该服务器的IP地址

C - 表示该服务器与集群中的Leader服务器交换信息的端口

D - 表示如果万一集群中的Leader服务器挂了，需要各服务器重新选举时所用的通讯端口

4）zookeeper目录加入环境变量

#vi/etc/profile

加入：

#zookeeper

exportZOOKEEPER=/var/zookeeper-3.4.8

PATH=$PATH:$ZOOKEEPER/bin

#source /etc/profile

5）启动和关闭Zookeeper

bin目录下，配置环境变量，任何目录下直接执行。

#zkServer.shstart

#zkServer.shstop

6）集群配置

其他机器重复以上步骤，注意myid对应。

**2、Storm部署**

Apache官网http://storm.apache.org/index.html下载apache-storm-1.0.1.tar.gz版本。

1）解压在/var/apache-storm-1.0.1

#tar -xzvf/tmp/apache-storm-1.0.1.tar.gz

#cp -R /tmp/apache-storm-1.0.1/var

#rm -rf/tmp/apache-storm-1.0.1

2）配置storm的bin目录环境变量

#vi/etc/profile

加入：

#storm

exportSTORM=/var/apache-storm-1.0.1

PATH=$PATH:$STORM/bin

#source /etc/profile

3）创建一个Storm的本地数据目录

#mkdir -p/var/apache-storm-1.0.1/workdir

4）nimbus配置conf/storm.yaml文件

storm.yaml默认配置参见

https://github.com/nathanmarz/storm/blob/master/conf/defaults.yaml

注意：配置项前面空格要去掉。

增加：

storm.local.dir:"/var/apache-storm-1.0.1/workdir "

storm.zookeeper.servers:

- "zoo1"

5）supervisor配置conf/storm.yaml文件

和nimbus配置一样，不过要增加：

nimbus.host:"zoo1"

supervisor.slots.ports:

     - 6700

     - 6701

     - 6702

其中nimbus.host是nimbus的IP或hostname；supervisor.slots.ports 是配置slot的ip地址。配了几个地址，就有几个slot，即几个worker。如果尝试提交的topology所声明的worker数超过当前可用的slot，该topology提交会失败。

6）启动storm

在nimbus机器的Storm的bin目录，已配置环境变量直接执行。

#stormnimbus &

在supervisor机器的Storm的bin目录，已配置环境变量直接执行。

#stormsupervisor &

可在/var/apache-storm-1.0.1/logs下看日志，日志下分别有nimbus和supervisor。

#jps 

17700QuorumPeerMain  //zookeeper进程

30038supervisor  

29948 nimbus

**3、Storm命令参数集合**

Commands:

activate

classpath

deactivate

dev-zookeeper

drpc

help

jar

kill

list

localconfvalue

logviewer

monitor

nimbus

rebalance

remoteconfvalue

repl

shell

supervisor

ui

version




