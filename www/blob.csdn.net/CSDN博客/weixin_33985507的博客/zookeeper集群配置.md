# zookeeper集群配置 - weixin_33985507的博客 - CSDN博客
2016年11月02日 14:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
### 1、前期准备
下载zookeeper的包到linux下:
(1)：wget  wget [http://mirror.bit.edu.cn/apache/zookeeper/zookeeper-3.4.6/zookeeper-3.4.6.tar.gz](https://link.jianshu.com?t=http://mirror.bit.edu.cn/apache/zookeeper/zookeeper-3.4.6/zookeeper-3.4.6.tar.gz) ;
(2)：解压到当前目录，tar -zxvf  zookeeper-3.4.6.tar.gz
(3)：在/usr/local/ 目录下新建目录 mkdir zookeeper-cluster目录
(4):将解压的zookeeper移动到刚刚新建的目录下mv zookeeper-3.4.6  /usr/local/zookeeper-cluster/zookeeper-3.4.6-node1下。
注：下面的zookeeper-3.4.6-node1，zookeeper-3.4.6-node2,zookeeper-3.4.6-node3，在文字中简写为node1,node2,node3。
### 2、zookeeper集群
先配置好其中一个zookeeper，然后在copy出另外几个。
## 2.1 配置集群
先进入目录 /usr/local/zookeeper-cluster/zookeeper-3.4.6-node1/conf/，将改目录下zoo_sample.cfg复制一份为zoo.cfg,开始配置zoo.cfg
同一台机器上的配置
```
tickTime=2000
initLimit=10
syncLimit=5
dataDir=/usr/local/zookeeper-cluster/zookeeper-3.4.6-node1/data ###不同点
clientPort=2181  ###不同点
server.1=localhost:2887:3887 
server.2=localhost:2888:3888
server.3=localhost:2889:3889
```
```
tickTime=2000
initLimit=10
syncLimit=5
dataDir=/usr/local/zookeeper-cluster/zookeeper-3.4.6-node2/data  ###不同点
clientPort=2182  ###不同点
server.1=localhost:2887:3887
server.2=localhost:2888:3888
server.3=localhost:2889:3889
```
说明：我做的这个是在一台机器上的三个集群，所以一般是分开三台机器的，所以只需要将localhost改为自己的三台机器ip即可。所以在节点2，节点三上，配置不同点就是
三台不同机器的一般如下,每台机器上的配置文件可以一模一样
```
tickTime=2000
initLimit=10
syncLimit=5
dataDir=/usr/local/zookeeper-cluster/zookeeper-3.4.6-node1/data
clientPort=2181
server.1=10.20.153.10:2887:3887 
server.2=10.20.153.11:2887:3888
server.3=10.20.153.12:2887:3889
```
上面看到配置文件中有个dataDir的路径，那么需要在/usr/local/zookeeper-cluster/zookeeper-3.4.6-node1/下新建data目录，其他一样每个zookeeper都是如此，在data下面我们需要新建一个myid，用来存放server.*=*中第一个*的数据，那么我们在node1中就存放的为1,
```
[root@localhost data]# ls
myid  zookeeper_server.pid
[root@localhost data]# cat myid 
1
[root@localhost data]#
```
同理，在node2中同样的data目录下新建myid，值为2，node3同样。dubbo官网里面的配置说明如下图
![1456372-58ee1e1c04e4d755.png](https://upload-images.jianshu.io/upload_images/1456372-58ee1e1c04e4d755.png)
Paste_Image.png
我们现在配置好了node1，同样的道理，复制出node2,node3，按照上面要求改下需要修改的地方，然后我们现在来启动测试
进入到/usr/local/zookeeper-cluster/zookeeper-3.4.6-node1/bin目录下
node1:
```
[root@localhost conf]#cd /usr/local/zookeeper-cluster/zookeeper-3.4.6-node1/bin
[root@localhost bin]# ./zkServer.sh start
[root@localhost bin]# ./zkServer.sh status
JMX enabled by default
Using config: /usr/local/zookeeper-cluster/zookeeper-3.4.6-node1/bin/../conf/zoo.cfg
Mode: standalone
```
node2:
```
[root@localhost conf]#cd /usr/local/zookeeper-cluster/zookeeper-3.4.6-node2/bin
[root@localhost bin]# ./zkServer.sh start
[root@localhost bin]# ./zkServer.sh status
JMX enabled by default
Using config: /usr/local/zookeeper-cluster/zookeeper-3.4.6-node2/bin/../conf/zoo.cfg
Mode: follower
```
node3:
```
[root@localhost conf]#cd /usr/local/zookeeper-cluster/zookeeper-3.4.6-node3/bin
[root@localhost bin]# ./zkServer.sh start
[root@localhost bin]# ./zkServer.sh status
JMX enabled by default
Using config: /usr/local/zookeeper-cluster/zookeeper-3.4.6-node3/bin/../conf/zoo.cfg
Mode: leader
```
我第一次测试的时候是有问题的，测试状态如下
```
[root@localhost bin]# ./zkServer.sh  status
JMX enabled by default
Using config: /usr/local/zookeeper-cluster/zookeeper-3.4.6-node2/bin/../conf/zoo.cfg
Error contacting service. It is probably not running.
```
### 2.2错误信息查看
日志的位置其实就在bin目录下,zookeeper.out文件：
```
[root@localhost bin]# ls
README.txt  zkCleanup.sh  zkCli.cmd  zkCli.sh  zkEnv.cmd  zkEnv.sh  zkServer.cmd  zkServer.sh  zookeeper.out
```
cat zookeeper.out 看错误信息即可,我的两次错误信息如下
```
[root@localhost bin]# cat zookeeper.out 
2016-11-02 12:43:01,873 [myid:] - INFO  [main:QuorumPeerConfig@103] - Reading configuration from: /usr/local/zookeeper-cluster/zookeeper-3.4.6-node2/bin/../conf/zoo.cfg
2016-11-02 12:43:01,878 [myid:] - ERROR [main:QuorumPeerMain@85] - Invalid config, exiting abnormally
org.apache.zookeeper.server.quorum.QuorumPeerConfig$ConfigException: Error processing /usr/local/zookeeper-cluster/zookeeper-3.4.6-node2/bin/../conf/zoo.cfg
        at org.apache.zookeeper.server.quorum.QuorumPeerConfig.parse(QuorumPeerConfig.java:123)
        at org.apache.zookeeper.server.quorum.QuorumPeerMain.initializeAndRun(QuorumPeerMain.java:101)
        at org.apache.zookeeper.server.quorum.QuorumPeerMain.main(QuorumPeerMain.java:78)
Caused by: java.lang.IllegalArgumentException: initLimit is not set
        at org.apache.zookeeper.server.quorum.QuorumPeerConfig.parseProperties(QuorumPeerConfig.java:299)
        at org.apache.zookeeper.server.quorum.QuorumPeerConfig.parse(QuorumPeerConfig.java:119)
        ... 2 more
Invalid config, exiting abnormally
```
上面是因为initLimit 这个复制少了一个i
```
[root@localhost bin]# cat zookeeper.out 
2016-11-02 12:44:41,017 [myid:] - INFO  [main:QuorumPeerConfig@103] - Reading configuration from: /usr/local/zookeeper-cluster/zookeeper-3.4.6-node2/bin/../conf/zoo.cfg
2016-11-02 12:44:41,021 [myid:] - INFO  [main:QuorumPeerConfig@340] - Defaulting to majority quorums
2016-11-02 12:44:41,024 [myid:2] - INFO  [main:DatadirCleanupManager@78] - autopurge.snapRetainCount set to 3
2016-11-02 12:44:41,025 [myid:2] - INFO  [main:DatadirCleanupManager@79] - autopurge.purgeInterval set to 0
2016-11-02 12:44:41,025 [myid:2] - INFO  [main:DatadirCleanupManager@101] - Purge task is not scheduled.
2016-11-02 12:44:41,035 [myid:2] - INFO  [main:QuorumPeerMain@127] - Starting quorum peer
2016-11-02 12:44:41,045 [myid:2] - INFO  [main:NIOServerCnxnFactory@94] - binding to port 0.0.0.0/0.0.0.0:2182
2016-11-02 12:44:41,061 [myid:2] - INFO  [main:QuorumPeer@959] - tickTime set to 2000
2016-11-02 12:44:41,061 [myid:2] - INFO  [main:QuorumPeer@979] - minSessionTimeout set to -1
2016-11-02 12:44:41,061 [myid:2] - INFO  [main:QuorumPeer@990] - maxSessionTimeout set to -1
2016-11-02 12:44:41,061 [myid:2] - INFO  [main:QuorumPeer@1005] - initLimit set to 10
2016-11-02 12:44:41,075 [myid:2] - INFO  [main:QuorumPeer@473] - currentEpoch not found! Creating with a reasonable default of 0. This should only happen when you are upgrading your installation
2016-11-02 12:44:41,076 [myid:2] - INFO  [main:QuorumPeer@488] - acceptedEpoch not found! Creating with a reasonable default of 0. This should only happen when you are upgrading your installation
2016-11-02 12:44:41,080 [myid:2] - INFO  [Thread-1:QuorumCnxManager$Listener@504] - My election bind port: /10.10.130.140:3888
2016-11-02 12:44:41,089 [myid:2] - INFO  [QuorumPeer[myid=2]/0:0:0:0:0:0:0:0:2182:QuorumPeer@714] - LOOKING
2016-11-02 12:44:41,090 [myid:2] - INFO  [QuorumPeer[myid=2]/0:0:0:0:0:0:0:0:2182:FastLeaderElection@815] - New election. My id =  2, proposed zxid=0x0
2016-11-02 12:44:41,095 [myid:2] - WARN  [WorkerSender[myid=2]:QuorumCnxManager@382] - Cannot open channel to 1 at election address /10.10.130.140:3887
java.net.ConnectException: 拒绝连接
```
就是一些什么拒绝链接的，我发现我没有启动node3，然后把node3启动起来就好了。
## 2.3 集群启动检测
上述使用./zkServer status 查看是否启动，其实还可以使用
```
[root@localhost bin]# echo stat |nc localhost 2182
Zookeeper version: 3.4.6-1569965, built on 02/20/2014 09:09 GMT
Clients:
 /127.0.0.1:43750[0](queued=0,recved=1,sent=0)
Latency min/avg/max: 0/0/0
Received: 4
Sent: 3
Connections: 1
Outstanding: 0
Zxid: 0x0
Mode: follower
Node count: 4
```
这样查看，如果没有启动的话什么信息都没有。但是我在使用的时候没有nc命令，需要自己安装，中间安装又遇到一些问题，按照下面的两个地址都解决了
[http://raksmart.idcspy.com/781](https://link.jianshu.com?t=http://raksmart.idcspy.com/781)
[http://www.tuicool.com/articles/m67Z3m](https://link.jianshu.com?t=http://www.tuicool.com/articles/m67Z3m)
### 3、参数说明
在配置的时候，我看了下文档，对于zookeeper的容错机制2n+1的概念还是很郁闷，找运维聊了下，为什么是2n+1，说的意思是，如果是三台，那么其中一台挂了，那么另外两台还可以继续支持，但是如果这两台其中的一台再次挂了，那么就无法使用了。其实我不理解为什么剩下两台的时候其中一台挂了，另外一台就不能单独支持了，运维打了比方，比如我现在这两个屏幕，其中一个关了，那么是不是就看不到了。感觉有理但是又没有道理的，我说你有主屏幕，还可以点开的（哈哈），这个此时没有主屏幕的概念。
## 3.1 server参数
server.1=localhost:2887:3887
这里的server.1中的1代表第几台服务；localhost代表服务器的ip地址；2887端口用来集群成员的信息交换,是服务器与集群中leader服务器交换信息的端口。而最后一个3887的话是在leader挂掉的时候用来进行选举leader所用。
