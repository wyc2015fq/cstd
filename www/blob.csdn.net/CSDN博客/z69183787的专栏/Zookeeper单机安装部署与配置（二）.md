# Zookeeper单机安装部署与配置（二） - z69183787的专栏 - CSDN博客
2018年02月24日 18:09:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：117
个人分类：[分布式-Zookeeper](https://blog.csdn.net/z69183787/article/category/5568407)
[http://blog.csdn.net/wo541075754/article/details/56483533](http://blog.csdn.net/wo541075754/article/details/56483533)
## 环境准备
（1）操作系统 
笔者的操作环境基于CentOS系统来进行操作，Linux环境是在VMware Workstation中搭建的虚拟机系统。
（2）Java环境 
Zookeeper是在Java运行环境下运行，确保在JDK1.6及以上版本，笔者使用JDK1.7。
### JDK安装
由于笔者完全从一个新的虚拟机开始搭建环境，因此把在CentOS上搭建Jdk的代码也分享出来。
首先，在本机下载好对应的jdk，笔者下载的文件为jdk-7u79-linux-x64.tar.gz。然后上传至Linux系统。下面一步步开始进行jdk配置操作。 
在/usr目录下创建java目录，并进入：
```
[root@localhost ~]# mkdir /usr/java
[root@localhost ~]# cd /usr/java
```
- 1
- 2
解压：
`[root@localhost java]# tar -zxvf jdk-7u79-linux-x64.tar.gz`- 1
设置环境变量：
`[root@localhost java]# vi /etc/profile`- 1
在profile文件中添加如下配置：
```bash
#set java environment
JAVA_HOME=/usr/java/jdk1.7.0_79
JRE_HOME=/usr/java/jdk1.7.0_79/jre
CLASS_PATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar:$JRE_HOME/lib
PATH=$PATH:$JAVA_HOME/bin:$JRE_HOME/bin
export JAVA_HOME JRE_HOME CLASS_PATH PATH
```
- 1
- 2
- 3
- 4
- 5
- 6
让修改生效：
`[root@localhost java]# source /etc/profile`- 1
验证安装是否成功：
```
[root@localhost java]# java -version
java version "1.7.0_79"
Java(TM) SE Runtime Environment (build 1.7.0_79-b15)
Java HotSpot(TM) 64-Bit Server VM (build 24.79-b02, mixed mode)
```
- 1
- 2
- 3
- 4
经过以上步骤，JDK环境安装成功。
## Zookeeper下载
下载Zookeeper
`curl -O https://mirrors.tuna.tsinghua.edu.cn/apache/zookeeper/zookeeper-3.4.9/zookeeper-3.4.9.tar.gz`- 1
解压文件：
`[root@localhost zookeeper]# tar -zxvf zookeeper-3.4.9.tar.gz `- 1
## 单机模式
完成下载解压之后，首先我们根据单机模式进行安装配置。 
进入zookeeper的conf目录下，找到zoo_sample.cfg文件。首先将zoo_sample.cfg文件备份，并重命名为zoo.cfg。
以下为默认配置，无需修改：
```
tickTime=2000
initLimit=10
syncLimit=5
dataDir=/tmp/zookeeper
clientPort=2181
```
- 1
- 2
- 3
- 4
- 5
进入bin目录，执行启动命令：
```
[root@localhost bin]# sh zkServer.sh start
ZooKeeper JMX enabled by default
Using config: /usr/zookeeper/zookeeper-3.4.9/bin/../conf/zoo.cfg
Starting zookeeper ... STARTED
```
- 1
- 2
- 3
- 4
Zookeeper启动完成。
查看运行情况：
```
[root@localhost bin]# jps -ml
2583 sun.tools.jps.Jps -ml
2563 org.apache.zookeeper.server.quorum.QuorumPeerMain /usr/zookeeper/zookeeper-3.4.9/bin/../conf/zoo.cfg
```
- 1
- 2
- 3
可以看到QuorumPeerMain已经处于运行状态。
执行telnet命令，连接上服务器之后，执行stat命令进行服务启动的验证，如果出现类似下面的信息则说明服务已经正常启动，一般情况下不需要验证，启动即可正常运行：
[root@localhost bin]# telnet 127.0.0.1 2181 
Trying 127.0.0.1… 
Connected to 127.0.0.1. 
Escape character is ‘^]’. 
stat 
Zookeeper version: 3.4.9-1757313, built on 08/23/2016 06:50 GMT 
Clients: 
/127.0.0.1:49660[0](http://blog.csdn.net/wo541075754/article/details/queued=0,recved=1,sent=0)
Latency min/avg/max: 0/0/0 
Received: 1 
Sent: 0 
Connections: 1 
Outstanding: 0 
Zxid: 0x0 
Mode: standalone 
Node count: 4 
Connection closed by foreign host.
