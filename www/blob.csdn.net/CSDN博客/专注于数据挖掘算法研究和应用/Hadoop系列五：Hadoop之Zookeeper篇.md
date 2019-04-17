# Hadoop系列五：Hadoop之Zookeeper篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年03月14日 10:16:25[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2359
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









ZooKeeper安装和运行

Zookeeper是一个高性能、分布式的、开源分布式应用协调服务，具有命名服务、配置管理、同步和组服务等基本功能。Zookeeper服务自身组成一个集群(2n+1个服务允许n个失效)。Zookeeper服务有两个角色，一个是leader，负责写服务和数据同步，剩下的是follower，提供读服务，leader失效后会在follower中重新选举新的leader。

ZooKeeper有单机（standalone）模式、集群（Replicated）模式、伪集群模式。

1）官网[http://zookeeper.apache.org/](http://zookeeper.apache.org/)下载并解压，在/home/hp下（Ubuntu操作系统，主机名hp）。

2）单机模式配置

zookeeper目录下的conf子目录，创建zoo.cfg，复制zoo_sample.cfg，配置：

tickTime=2000  //毫秒值

dataDir=/home/hp/zookeeper/data   //数据目录.

dataLogDir=/home/hp/zookeeper/log  //log目录，不设置的话与dataDir同目录

clientPort=2181;  //监听client连接的端口号

服务端启动：$bin/zkServer.sh start

      关闭；$bin/zkServer.sh stop

客户端连接：$bin/zkCli.sh –server localhost:2181

3）伪集群模式：

伪集群, 是指在单台机器中启动多个zookeeper进程, 并组成一个集群.以启动3个zookeeper进程为例。

Ø  Zookeeper下载后解压到三个目录，分别为zookeeper0、zookeeper1、zookeeper2。

Ø  配置zookeeper目录下的conf子目录zoo.cfg文件

tickTime=2000  

initLimit=5  

syncLimit=2  

dataDir=/home/hp/zookeeper0/data

clientPort=2181  //其他两台分别为2182、2183

server.0=127.0.0.1:8880:7770  

server.1=127.0.0.1:8881:7771  

server.2=127.0.0.1:8882:7772

zookeeper集群中的包含多台server, 其中一台为leader, 集群中其余的server为follower. server.X=A:B:C 其中X是一个数字, 表示这是第几号server.A是该server所在的IP地址. B配置该server和集群中的leader交换消息所使用的端口. C配置选举leader时所使用的端口.由于配置的是伪集群模式, 所以各个server的B, C参数必须不同.

Ø  参照zookeeper0/conf/zoo.cfg,

配置zookeeper1/conf/zoo.cfg, 和zookeeper2/conf/zoo.cfg文件. 只需更改dataDir、clientPort参数即可.

Ø  在dataDir中新建myid文件, 写入一个数字, 该数字表示这是第几号server. 数字和zoo.cfg文件中的server.X中的X一一对应。

zookeeper0/data/myid文件中写入0,

zookeeper1/data/myid文件中写入1,

zookeeper2/data/myid文件中写入2.

Ø  分别启动三个server并选择其中一个启动客户端。

4）集群模式

集群模式的配置和伪集群基本一致。集群模式下, 各server部署在不同的机器上, 其conf/zoo.cfg文件可一样，注意data目录下myid的数字要和server编号一致。

5）zk客户端基本命令

ls(查看当前节点数据)

ls2(查看当前节点数据并能看到更新次数等数据) 

create(创建一个节点)  例子：$create /zk “test”

get(得到一个节点，包含数据和更新次数等数据) 例子：$get /zk

set(修改节点)

delete(删除一个节点)

Zookeeper的具体应用集合Hbase体现。
            


