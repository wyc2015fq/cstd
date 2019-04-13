
# zookeeper多种方式安装 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月20日 16:56:01[Jlins](https://me.csdn.net/dyllove98)阅读数：1483


本文系统：Centos6.5 x64
一、单机模式安装
二、伪集群模式安装
三、集群模式安装
1、单机模式安装zookeeper
1.1、创建帐号zk
|1
|2
|[root@master ~]|\# useradd -d /opt/zk zk
|[root@master ~]|\# echo "zk" |passwd --stdin zk
|

1.2、下载、解压、
|1
|2
|3
|4
|5
|6
|[root@master ~]|\# su - zk
|[zk@master ~]$|pwd
|/opt/zk
|[zk@master ~]$  wget
|[zk@master ~]$|tar|zxvf zookeeper-3.5.0-alpha.|tar|.gz
|[zk@master ~]$|mv|zookeeper-3.5.0-alpha zk
|

1.3、配置当前用户环境变量
|1
|2
|3
|export|ZOOKEEPER_HOME=|/opt/zk/zk
|PATH=$ZOOKEEPER_HOME|/bin|:$PATH
|export|PATH
|

1.4、配置配置文件zoo.cfg
|1
|2
|3
|4
|5
|6
|7
|[zk@master conf]$|cat|zoo.cfg
|initLimit=10
|syncLimit=5
|clientPort=2181
|tickTime=2000
|dataDir=|/opt/zk/zk/data
|dataLogDir=|/opt/zk/zk/logs
|

1.5、启动zk
|1
|2
|3
|[zk@master bin]$|pwd
|/opt/zk/zk/bin
|[zk@master bin]$ .|/zkServer|.sh start
|

1.6、客户端链接测试下：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|[zk@master bin]$|pwd
|/opt/zk/zk/bin
|[zk@master bin]$ .|/zkCli|.sh -server localhost:2181
|Connecting to localhost:2181
|。。。
|[zk: localhost:2181(CONNECTED) 0]|ls|/
|[zookeeper]
|[zk: localhost:2181(CONNECTED) 1] create|/Test|hellozk
|Created|/Test
|[zk: localhost:2181(CONNECTED) 2] get|/Test
|hellozk
|[zk: localhost:2181(CONNECTED) 3]|set|/Test|hellozookeeper
|[zk: localhost:2181(CONNECTED) 4] get|/Test
|hellozookeeper
|[zk: localhost:2181(CONNECTED) 5] delete|/Test
|[zk: localhost:2181(CONNECTED) 6] get|/Test
|Node does not exist:|/Test
|[zk: localhost:2181(CONNECTED) 7] quit
|2014-11-19 03:53:50,180 [myid:] - INFO  [main:ZooKeeper@968] - Session: 0x149c475d7db0000 closed
|2014-11-19 03:53:50,182 [myid:] - INFO  [main-EventThread:ClientCnxn$EventThread@529] - EventThread shut down
|

zookeeper的配置文件说明：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|clientPort|\# 客户端连接server的端口，即对外服务端口，一般设置为2181。
|dataDir|\# 存储快照文件snapshot的目录。默认情况下，事务日志也会存储在这里。建议同时配置参数dataLogDir, 事务日志的写性能直接影响zk性能。
|tickTime|\# ZK中的一个时间单元。ZK中所有时间都是以这个时间单元为基础，进行整数倍配置的。例如，session的最小超时时间是2*tickTime。
|dataLogDir|\# 事务日志输出目录。尽量给事务日志的输出配置单独的磁盘或是挂载点，这将极大的提升ZK性能。
|globalOutstandingLimit|\# 最大请求堆积数。默认是1000。ZK运行的时候， 尽管server已经没有空闲来处理更多的客户端请求了，但是还是允许客户端将请求提交到服务器上来，提高吞吐性能。当然，为了防止Server内存溢出，这个请求堆积数还是需要限制下的。 Java system property:zookeeper.globalOutstandingLimit.
|preAllocSize|\# 预先开辟磁盘空间，用于后续写入事务日志。默认是64M，每个事务日志大小就是64M。如果ZK的快照频率较大的话，建议适当减小这个参数。
|snapCount|\# 每进行snapCount次事务日志输出后，触发一次快照(snapshot), 此时，ZK会生成一个snapshot.*文件，同时创建一个新的事务日志文件log.*。默认是100000.（真正的代码实现中，会进行一定的随机数处理，以避免所有服务器在同一时间进行快照而影响性能）。
|traceFile|\# 用于记录所有请求的log，一般调试过程中可以使用，但是生产环境不建议使用，会严重影响性能
|maxClientCnxns|\# 单个客户端与单台服务器之间的连接数的限制，是ip级别的，默认是60，如果设置为0，那么表明不作任何限制。请注意这个限制的使用范围，仅仅是单台客户端机器与单台ZK服务器之间的连接数限制，不是针对指定客户端IP，也不是ZK集群的连接数限制，也不是单台ZK对所有客户端的连接数限制。
|clientPortAddress|\# 对于多网卡的机器，可以为每个IP指定不同的监听端口。默认情况是所有IP都监听 clientPort 指定的端口。
|minSessionTimeoutmaxSessionTimeout|\# Session超时时间限制，如果客户端设置的超时时间不在这个范围，那么会被强制设置为最大或最小时间。默认的Session超时时间是在2 * tickTime ~ 20 * tickTime 这个范围 。
|fsync.warningthresholdms|\# 事务日志输出时，如果调用fsync方法超过指定的超时时间，那么会在日志中输出警告信息。默认是1000ms。
|autopurge.purgeInterval|\# 3.4.0及之后版本，ZK提供了自动清理事务日志和快照文件的功能，这个参数指定了清理频率，单位是小时，需要配置一个1或更大的整数，默认是0，表不开启自动清理功能
|autopurge.snapRetainCount|\# 这个参数和上面的参数搭配使用，这个参数指定了需要保留的文件数目。默认是保留3个。
|electionAlg|\#在之前的版本中， 这个参数配置是允许我们选择leader选举算法，但是由于在以后的版本中，只会留下一种“TCP-based version of fast leader election”算法，所以这个参数目前看来没有用了。
|initLimit|\# Follower在启动过程中，会从Leader同步所有最新数据，然后确定自己能够对外服务的起始状态。Leader允许F在 initLimit 时间内完成这个工作。通常情况下，我们不用太在意这个参数的设置。如果ZK集群的数据量确实很大了，F在启动的时候，从Leader上同步数据的时间也会相应变长，因此在这种情况下，有必要适当调大这个参数了。
|syncLimit|\# 在运行过程中，Leader负责与ZK集群中所有机器进行通信，例如通过一些心跳检测机制，来检测机器的存活状态。如果L发出心跳包在syncLimit之后，还没有从F那收到响应，那么就认为这个F已经不在线了。注意：不要把这个参数设置得过大，否则可能会掩盖一些问题。
|leaderServes|\# 默认情况下，Leader是会接受客户端连接，并提供正常的读写服务。但是，如果你想让Leader专注于集群中机器的协调，那么可以将这个参数设置为no，这样一来，会大大提高写操作的性能。
|server.X=A:B:C|\# 其中X是一个数字, 表示这是第几号server. A是该server所在的IP地址. B配置该server和集群中的leader交换消息所使用的端口. C配置选举leader时所使用的端口. 这里的x是一个数字，与myid文件中的id是一致的。右边可以配置两个端口，第一个端口用于F和L之间的数据同步和其它通信，第二个端口用于Leader选举过程中投票通信。
|group.x=nnnnn[:nnnnn]weight.x=nnnnn|\# 对机器分组和权重设置，
|cnxTimeout|\# Leader选举过程中，打开一次连接的超时时间，默认是5s
|zookeeper.DigestAuthenticationProvider.superDigest|\# ZK权限设置相关
|skipACL|\# 对所有客户端请求都不作ACL检查。如果之前节点上设置有权限限制，一旦服务器上打开这个开头，那么也将失效
|forceSync|\# 这个参数确定了是否需要在事务日志提交的时候调用 FileChannel .force来保证数据完全同步到磁盘
|jute.maxbuffer|\# 每个节点最大数据量，是默认是1M。这个限制必须在server和client端都进行设置才会生效。
|


2、伪集群模式安装：
伪集群, 是指在单台机器中启动多个zookeeper进程, 并组成一个集群. 以启动3个zookeeper进程为例
2.1、拷贝zk目录，分别为zk2、zk3
|1
|2
|[zk@master ~]$|cp|-r zk zk2
|[zk@master ~]$|cp|-r zk zk3
|

2.2、分别修改配置文件
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|[zk@master conf]$ vim zoo.cfg
|initLimit=10
|syncLimit=5
|clientPort=2181
|tickTime=2000
|dataDir=|/opt/zk/zk/data
|dataLogDir=|/opt/zk/zk/logs
|server.0=127.0.0.1:5555:6666
|server.1=127.0.0.1:5556:6667
|server.2=127.0.0.1:5557:6668
|

分别修改其他俩个zk目录配置文件dataDir, dataLogDir, clientPort参数即可.
分别在zk、zk2、zk3、的dataDir中新建myid文件, 写入一个数字, 该数字表示这是第几号server. 该数字必须和zoo.cfg文件中的server.X中的X一一对应.
/opt/zk/zk/data/myid文件中写入0, /opt/zk/zk2/data/myid文件中写入1, /opt/zk/zk3/data/myid文件中写入2.
|1
|2
|3
|[zk@master ~]$|echo|0 >|/opt/zk/zk/data/myid
|[zk@master ~]$|echo|1 >|/opt/zk/zk2/data/myid
|[zk@master ~]$|echo|2 >|/opt/zk/zk3/data/myid
|

2.3、分别启动
略
3、集群模式安装

集群模式的配置和伪集群基本一致.
由于集群模式下, 各server部署在不同的机器上, 因此各server的conf/zoo.cfg文件可以完全一样.
示例：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|tickTime=2000
|initLimit=5
|syncLimit=2
|dataDir=|/opt/zk/zk/data
|dataLogDir=|/opt/zk/zk/logs
|clientPort=2180
|server.0=192.168.10.128:5555:6666
|server.1=192.168.10.129:5555:6666
|server.2=192.168.10.130:5555:6666
|

示例文件部署了3台zookeeper server, 分别部署在192.168.10.128~130上. 需要注意的是, 各server的dataDir目录下的myid文件中的数字必须不同.
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

