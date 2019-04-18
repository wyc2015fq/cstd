# zookeeper集群环境搭建(centos) - z69183787的专栏 - CSDN博客
2018年02月25日 11:14:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：155
个人分类：[分布式-Zookeeper](https://blog.csdn.net/z69183787/article/category/5568407)
## [http://blog.csdn.net/m290345792/article/details/52576890](http://blog.csdn.net/m290345792/article/details/52576890)
## zookeeper 介绍
### zookeeper是什么
Zookeeper,一种分布式应用的协作服务,是Google的Chubby一个开源的实现,是Hadoop的分布式协调服务,它包含一个简单的原语集,应用于分布式应用的协作服务,使得分布式应用可以基于这些接口实现诸如同步、配置维护和分集群或者命名的服务。
zookeeper是一个由多个service组成的集群,一个leader,多个follower,每个server保存一份数据部分,全局数据一致,分布式读写,更新请求转发由leader实施.
更新请求顺序进行,来自同一个client的更新请求按其发送顺序依次执行,数据更新原子性,一次数据更新要么成功,要么失败,全局唯一数据试图,client无论连接到哪个server,数据试图是一致的.
### 为什么要用zookeeper
大部分分布式应用需要一个主控、协调器或控制器来管理物理分布的子进程（如资源、任务分配等）,目前,大部分应用需要开发私有的协调程序,缺乏一个通用的机制.协调程序的反复编写浪费,且难以形成通用、伸缩性好的协调器,ZooKeeper：提供通用的分布式锁服务,用以协调分布式应用
### zookeeper工作原理
zookeeper的核心是原子广播,这个机制保证了各个server之间的同步,实现这个机制的协议叫做Zab协议.Zab协议有两种模式,他们分别是恢复模式和广播模式.
　 1.当服务启动或者在领导者崩溃后,Zab就进入了恢复模式,当领导着被选举出来,且大多数server都完成了和leader的状态同步后,恢复模式就结束了.状态同步保证了leader和server具有相同的系统状态.
　 2.一旦leader已经和多数的follower进行了状态同步后,他就可以开始广播消息了,即进入广播状态.这时候当一个server加入zookeeper服务中,它会在恢复模式下启动,发下leader,并和leader进行状态同步,待到同步结束,它也参与广播消息.
说明:
广播模式需要保证proposal被按顺序处理,因此zk采用了递增的事务id号(zxid)来保证.所有的提议(proposal)都在被提出的时候加上了zxid.实现中zxid是一个64为的数字,它高32位是epoch用来标识leader关系是否改变,每次一个leader被选出来,它都会有一个新的epoch.低32位是个递增计数.
当leader崩溃或者leader失去大多数的follower,这时候zk进入恢复模式,恢复模式需要重新选举出一个新的leader,让所有的server都恢复到一个正确的状态.
Broadcast模式极其类似于分布式事务中的2pc（two-phrase commit 两阶段提交）：即leader提起一个决议,由followers进行投票,leader对投票结果进行计算决定是否通过该决议,如果通过执行该决议（事务）,否则什么也不做.
#### Leader选举
每个Server启动以后都询问其它的Server它要投票给谁,对于其他server的询问,server每次根据自己的状态都回复自己推荐的leader的id和上一次处理事务的zxid（系统启动时每个server都会推荐自己）,收到所有Server回复以后,就计算出zxid最大的哪个Server,并将这个Server相关信息设置成下一次要投票的Server.计算这过程中获得票数最多的的sever为获胜者,如果获胜者的票数超过半数,则改server被选为leader.否则,继续这个过程,直到leader被选举出来.leader就会开始等待server连接,Follower连接leader,将最大的zxid发送给leader,Leader根据follower的zxid确定同步点,完成同步后通知follower
 已经成为uptodate状态,Follower收到uptodate消息后,又可以重新接受client的请求进行服务了.
#### zookeeper的数据模型
层次化的目录结构,命名符合常规文件系统规范
每个节点在zookeeper中叫做znode,并且其有一个唯一的路径标识
节点Znode可以包含数据和子节点,但是EPHEMERAL类型的节点不能有子节点
Znode中的数据可以有多个版本,比如某一个路径下存有多个数据版本,那么查询这个路径下的数据就需要带上版本
客户端应用可以在节点上设置监视器,节点不支持部分读写,而是一次性完整读写
Zoopkeeper 提供了一套很好的分布式集群管理的机制,就是它这种基于层次型的目录树的数据结构,并对树中的节点进行有效管理,从而可以设计出多种多样的分布式的数据管理模型
Zookeeper的节点
Znode有两种类型,短暂的（ephemeral）和持久的（persistent）
Znode的类型在创建时确定并且之后不能再修改
短暂znode的客户端会话结束时,zookeeper会将该短暂znode删除,短暂znode不可以有子节点
持久znode不依赖于客户端会话,只有当客户端明确要删除该持久znode时才会被删除
Znode有四种形式的目录节点,PERSISTENT、PERSISTENT_SEQUENTIAL、EPHEMERAL、EPHEMERAL_SEQUENTIAL.
znode 可以被监控,包括这个目录节点中存储的数据的修改,子节点目录的变化等,一旦变化可以通知设置监控的客户端,这个功能是zookeeper对于应用最重要的特性,
通过这个特性可以实现的功能包括配置的集中管理,集群管理,分布式锁等等.
#### Zookeeper的角色
领导者（leader）,负责进行投票的发起和决议,更新系统状态
学习者（learner）,包括跟随者（follower）和观察者（observer）.
follower用于接受客户端请求并想客户端返回结果,在选主过程中参与投票
Observer可以接受客户端连接,将写请求转发给leader,但observer不参加投票过程,只同步leader的状态,observer的目的是为了扩展系统,提高读取速度
客户端（client）,请求发起方
Watcher
Watcher 在 ZooKeeper 是一个核心功能,Watcher 可以监控目录节点的数据变化以及子目录的变化,一旦这些状态发生变化,服务器就会通知所有设置在这个目录节点上的 Watcher,从而每个客户端都很快知道它所关注的目录节点的状态发生变化,而做出相应的反应
可以设置观察的操作：exists,getChildren,getData
可以触发观察的操作：create,delete,setData
znode以某种方式发生变化时,“观察”（watch）机制可以让客户端得到通知.
可以针对ZooKeeper服务的“操作”来设置观察,该服务的其他 操作可以触发观察.
比如,客户端可以对某个客户端调用exists操作,同时在它上面设置一个观察,如果此时这个znode不存在,则exists返回 false,如果一段时间之后,这个znode被其他客户端创建,则这个观察会被触发,之前的那个客户端就会得到通知.
## zookeeper 环境搭建
Zookeeper伪分布式环境搭建流程（因为我只有一台虚拟机，如果有多台主机的话，就可以搭建真实的集群，注意集群环境要求奇数个node,最少数量为3，偶数没有意义，因为集群选举机制为“半数以上”投票）:
#### 1.安装
在 /usr/local/ 下新建 zk-cluster:
`mkdir zk-cluster `- 1
下载安装包：
`wget http://mirrors.hust.edu.cn/apache/zookeeper/zookeeper-3.4.6/zookeeper-3.4.6.tar.gz`- 1
解压安装包到 zk-cluster
`tar -zxvf zookeeper-3.4.6.tar.gz`- 1
重命名 zookeeper-3.4.6
`mv zookeeper-3.4.6 zookeeper-node1`- 1
#### 2.创建数据存放路径
```
cd zookeeper-node1
mkdir data
```
- 1
- 2
#### 3.创建并配置zoo.cfg文件
```
cd ../conf
cp zoo_sample.cfg zoo.cfg
```
- 1
- 2
配置zoo.cfg文件(zookeeper-node1的配置):
```
tickTime=2000
initLimit=10
syncLimit=5
clientPort=2181
dataDir=../data
server.1=localhost:2887:3887
server.2=localhost:2888:3888
server.3=localhost:2889:3889
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
参数说明: 
tickTime=2000: 
tickTime这个时间是作为Zookeeper服务器之间或客户端与服务器之间维持心跳的时间间隔,也就是每个tickTime时间就会发送一个心跳； 
initLimit=10: 
initLimit这个配置项是用来配置Zookeeper接受客户端（这里所说的客户端不是用户连接Zookeeper服务器的客户端,而是Zookeeper服务器集群中连接到Leader的Follower 服务器）初始化连接时最长能忍受多少个心跳时间间隔数。 
当已经超过10个心跳的时间（也就是tickTime）长度后 Zookeeper 服务器还没有收到客户端的返回信息,那么表明这个客户端连接失败。总的时间长度就是 10*2000=20 秒； 
syncLimit=5: 
syncLimit这个配置项标识Leader与Follower之间发送消息,请求和应答时间长度,最长不能超过多少个tickTime的时间长度,总的时间长度就是5*2000=10秒； 
dataDir=/export/search/zookeeper-cluster/zookeeper-3.4.6-node1/data 
dataDir顾名思义就是Zookeeper保存数据的目录,默认情况下Zookeeper将写数据的日志文件也保存在这个目录里； 
clientPort=2181 
clientPort这个端口就是客户端连接Zookeeper服务器的端口,Zookeeper会监听这个端口接受客户端的访问请求； 
server.1=localhost:2887:3887 
server.2=localhost:2888:3888 
server.3=localhost:2889:3889 
server.A=B：C：D： 
A是一个数字,表示这个是第几号服务器,B是这个服务器的ip地址 
C第一个端口用来集群成员的信息交换,表示的是这个服务器与集群中的Leader服务器交换信息的端口 
D是在leader挂掉时专门用来进行选举leader所用
仿照上边的过程，分别配置zookeeper-node2 和 zookeeper-node3
```
[root@localhost zookeeper-cluster]# cp -r zookeeper-node1 zookeeper-node2
[root@localhost zookeeper-cluster]# cp -r zookeeper-node1 zookeeper-node3
```
- 1
- 2
修改 zoo.cfg文件： 
zookeeper-node2 里边，clientPort=2182 
zookeeper-node3 里边，clientPort=2183
#### 4.创建ServerID标识
除了修改zoo.cfg配置文件,集群模式下还要配置一个文件myid,这个文件在data目录下,这个文件里面就有一个数据就是A的值,在上面配置文件中zoo.cfg中配置的dataDir路径中创建myid文件
```
Cd data
Vim myid
zookeeper-node1 Myid文件里内容为 1
zookeeper-node2 Myid文件里内容为 2
zookeeper-node3 Myid文件里内容为 3
```
- 1
- 2
- 3
- 4
- 5
启动zookeeper
```
[root@localhost zookeeper-cluster]# ./zookeeper-node1/bin/zkServer.sh start
[root@localhost zookeeper-cluster]# ./zookeeper-node2/bin/zkServer.sh start
[root@localhost zookeeper-cluster]# ./zookeeper-node3/bin/zkServer.sh start
```
- 1
- 2
- 3
输出内容为：
```
JMX enabled by default
Using config: /export/search/zookeeper-cluster/zookeeper-3.4.6-node1/bin/../conf/zoo.cfg
Starting zookeeper ... STARTED
```
- 1
- 2
- 3
检测集群是否启动
```
[root@localhost zookeeper-cluster]# echo stat|nc localhost 2181
[root@localhost zookeeper-cluster]# echo stat|nc localhost 2182
[root@localhost zookeeper-cluster]# echo stat|nc localhost 2183
```
- 1
- 2
- 3
伪集群部署注意事项:
在一台机器上部署了3个server；需要注意的是clientPort这个端口如果在1台机器上部署多个server,那么每台机器都要不同的clientPort。比如 server.1是2181,server.2是2182,server.3是2183 
最后几行唯一需要注意的地方就是: 
server.X 这个数字就是对应 data/myid中的数字。你在3个server的myid文件中分别写入了1,2,3,那么每个server中的zoo.cfg都配 server.1,server.2,server.3就OK了
转自：[http://www.linuxidc.com/Linux/2015-02/114230.htm](http://www.linuxidc.com/Linux/2015-02/114230.htm)
