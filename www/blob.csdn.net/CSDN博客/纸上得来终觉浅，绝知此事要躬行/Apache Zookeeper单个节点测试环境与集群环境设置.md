# Apache Zookeeper单个节点测试环境与集群环境设置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月27日 17:53:19[boonya](https://me.csdn.net/boonya)阅读数：713








Apache Zookeeper依赖于Java虚拟机环境。我们首先需要准备一个Linux Ubuntu 15.04环境可以通过wget命令下载Zookeeper的安装包，所以不必事先下载Zookeeper的安装包。


## 环境说明

Linux：Ubuntu-server-15.04.ios

Java：jdk_1.7.x或以上。

注：Java支持为必须环境。

## Java环境设置

1.编辑/etc/profile，在文件末尾加入如下内容：



```
#set java environment
JAVA_HOME=/usr/java/jdk1.7.0_79
JRE_HOME=/usr/java/jdk1.7.0_79/jre
CLASS_PATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar:$JRE_HOME/lib
PATH=$PATH:$JAVA_HOME/bin:$JRE_HOME/bin
export JAVA_HOME JRE_HOME CLASS_PATH PATH
```


2.使修改生效，需要执行source命令：




`source /etc/profile`


3.测试是否安装成功：

![](https://img-blog.csdn.net/20170327173431878)


## 安装Zookeeper

1.进入Linux系统中，下载Zookeeper的安装文件：


`wget https://mirrors.tuna.tsinghua.edu.cn/apache/zookeeper/zookeeper-3.4.9/zookeeper-3.4.9.tar.gz`


2.解压Zookeeper的安装文件：


`tar -zxvf zookeeper-3.4.9.tar.gz`


3.修改config下的zoo_sample.cfg文件名为zoo.cfg

注：无需修改配置文件的内容。


4.进入bin目录，启动Zookeeper：



```
root@ubuntu:/usr/local/zookeeper-3.4.9/bin# ./zkServer.sh start
ZooKeeper JMX enabled by default
Using config: /usr/local/zookeeper-3.4.9/bin/../conf/zoo.cfg
Starting zookeeper ... STARTED
root@ubuntu:/usr/local/zookeeper-3.4.9/bin#
```


5.使用jps命令测试启动是否成功：





```
root@ubuntu:/usr/local/zookeeper-3.4.9/bin# jps -ml
1326 sun.tools.jps.Jps -ml
1286 org.apache.zookeeper.server.quorum.QuorumPeerMain /usr/local/zookeeper-3.4.9/bin/../conf/zoo.cfg
root@ubuntu:/usr/local/zookeeper-3.4.9/bin#
```
6.如想测试网络连通性使用如下命令：




`telnet 192.168.234.128 2181`
7.停止Zookeeper服务



```
root@ubuntu:/usr/local/zookeeper-3.4.9/bin# ./zkServer.sh stop
ZooKeeper JMX enabled by default
Using config: /usr/local/zookeeper-3.4.9/bin/../conf/zoo.cfg
Stopping zookeeper ... STOPPED
root@ubuntu:/usr/local/zookeeper-3.4.9/bin#
```



## 集群配置

在zoo.cfg文件中新增配置如下server属性：



```
server.1=IP1:2888:3888
server.2=IP2:2888:3888
server.3=IP3:2888:3888
```



**配置文件配置参数说明：**

![](https://img-blog.csdn.net/20170327181135818)


**集群模式说明：server.id=host:port1:port2**

其中id为Server ID，用来标识该机器在集群中的编号。同时，在所在服务器的数据目录（/tmp/zookeeper）下创建一个myid文件，该文件只有一行内容，并且是一个数字，就是对应每台服务器的Server ID数字。比如server.1=IP1:2888:3888的myid中的内容就是1。不同服务器的ID需要保持不同，并且和zoo.cfg文件中server.id中的id和myid文件的内容保持一致。id的取值范围为1~255。

**server.id中配置参数说明：**

host：主机IP地址。


port1是集群中其他机器与Leader之间通信的端口。

port2为当Leader宕机或其他故障时，集群进行重新选举Leader时使用的端口。

按照以上相同步骤，配置集群中的其他机器。每个集群的zoo.cfg文件都是相同的，可通过版本控制或其他工具保证每台zookeeper服务器的配置文件相同。集群中每台机器唯一不同的是server.id对应的myid文件中的数字不同。
注：主要是添加sever的配置和myid的配置，启动停止操作都是一样的。

## 配置Zookeeper的环境变量

在/etc/profile文件中添加如下内容：




```
ZOOKEEPER=/usr/local/zookeeper-3.4.9/
PATH=$PATH:$ZOOKEEPER/bin
```
使修改生效：




`source /etc/profile`


查看状态：




```
root@ubuntu:~# zkServer.sh status
ZooKeeper JMX enabled by default
Using config: /usr/local/zookeeper-3.4.9/bin/../conf/zoo.cfg
Mode: standalone
root@ubuntu:~#
```




## 参考资料

Zookeeper安装与部署配置(二)：[http://blog.csdn.net/wo541075754/article/details/56483533](http://blog.csdn.net/wo541075754/article/details/56483533)

Zookeeper集群部署与配置(三)：[http://blog.csdn.net/wo541075754/article/details/56830137](http://blog.csdn.net/wo541075754/article/details/56830137)


Zookeeper集群模式下的安装和配置：[http://blog.chinaunix.net/uid-20498361-id-3184380.html](http://blog.chinaunix.net/uid-20498361-id-3184380.html)










