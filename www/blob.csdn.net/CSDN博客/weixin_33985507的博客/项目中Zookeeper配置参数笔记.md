# 项目中Zookeeper配置参数笔记 - weixin_33985507的博客 - CSDN博客
2016年10月05日 20:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8

ZooKeeper是以Fast Paxos算法为基础的，[Paxos 算法](http://baike.baidu.com/view/8438269.htm)存在[活锁](http://baike.baidu.com/view/281236.htm)的问题，即当有多个proposer交错提交时，有可能互相排斥导致没有一个proposer能提交成功，而Fast Paxos作了一些优化，通过选举产生一个leader (领导者)，只有leader才能提交proposer，具体算法可见Fast Paxos。因此，要想弄懂ZooKeeper首先得对Fast Paxos有所了解。
ZooKeeper的基本运转流程：
1、选举Leader。
2、同步数据。
3、选举Leader过程中算法有很多，但要达到的选举标准是一致的。
4、Leader要具有最高的zxid。
5、集群中大多数的机器得到响应并follow选出的Leader
**zoo.cfg**
#客户端连接server的端口，即对外服务端口，一般设置为2181
**clientPort=2183**
#在运行过程中，Leader负责与ZK集群中所有机器进行通信，例如通过一些心跳检测机制，来检测机器的存活状态。如果L发出心跳包在syncLimit之后，还没有从F那里收到响应，那么就认为这个F已经不在线了
**syncLimit=5**
#ZK提供了自动清理事务日志和快照文件的功能，这个参数指定了清理频率，单位是小时，需要配置一个1或更大的整数，默认是0，表示不开启自动清理功能。
**autopurge.purgeInterval=1**
#单个客户端与单台服务器之间的连接数的限制，是ip级别的，默认是60，如果设置为0，那么表明不作任何限制。请注意这个限制的使用范围，仅仅是单台客户端机器与单台ZK服务器之间的连接数限制，不是针对指定客户端IP，也不是ZK集群的连接数限制，也不是单台ZK对所有客户端的连接数限制
**maxClientCnxns=0**
#存储快照文件snapshot的目录。默认情况下，事务日志也会存储在这里。建议同时配置参数dataLogDir, 事务日志的写性能直接影响zk性能。
**dataDir=/usr/local/...**
#Follower在启动过程中，会从Leader同步所有最新数据，然后确定自己能够对外服务的起始状态。Leader允许F在 **initLimit** 时间内完成这个工作。通常情况下，我们不用太在意这个参数的设置。如果ZK集群的数据量确实很大了，F在启动的时候，从Leader上同步数据的时间也会相应变长，因此在这种情况下，有必要适当调大这个参数了。
**initLimit=10**
#这个参数指定了需要保留的文件数目。默认是保留3个
**autopurge.snapRetainCount=30**
#这里的x是一个数字，与myid文件中的id是一致的。右边可以配置两个端口，第一个端口用于F和L之间的数据同步和其它通信，第二个端口用于Leader选举过程中投票通信。
**server.x=master:2898:3898**
#ZK中的一个时间单元。ZK中所有时间都是以这个时间单元为基础，进行整数倍配置的。例如，session的最小超时时间是2*tickTime。
**tickTime=3000**
**需要注意的是：**dataDir目录下数据文件 myid 文件中的数值在集群zookeeper的配置中是唯一的
