# zookeeper windows 入门安装和测试 - z69183787的专栏 - CSDN博客
2016年09月26日 20:59:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：403
个人分类：[分布式-Zookeeper](https://blog.csdn.net/z69183787/article/category/5568407)
**一、序言**
**      以下是我对zookeeper 的一些理解：**      zookeeper 作为一个服务注册信息存储的管理工具，好吧，这样说得很抽象，我们举个“栗子”。
      栗子1号：
      假设我是一家KTV的老板，我同时拥有5家KTV，我肯定得时刻监视我KTV 的情况吧，是不是有人打架，或者发生火灾什么的，这时候我会给设置一个视频监控，然后每一家都连接到我的视频监控里面，那么我就可以在家里看到所有KTV 的情况了，如果某一家出现问题，我就能及时发现，并且做出反应。
这个视频监控就相当于zookeeper,每一家的连接，就相当于KTV 的信息。
**二、安装过程**
     2.1  http://mirrors.hust.edu.cn/apache/zookeeper/  下载，我的版本是 3.4.6(stable) 稳定
     2.2  解压到 F:\zookeeper-3.4.6 
     3.3  到目录conf 下创建 zoo.cfg 文件，默认就是加载这个文件，文件内容 我直接copy 的sample里面的
Java代码  ![收藏代码](http://greemranqq.iteye.com/images/icon_star.png)
- #zoo.cfg 的内容  
- #   心跳检查的时间 2秒  
- tickTime=2000
- # 初始化时 连接到服务器端的间隔次数，总时间10*2=20秒  
- initLimit=10
- # ZK Leader 和follower 之间通讯的次数，总时间5*2=10秒   
- syncLimit=5
- # 存储内存中数据库快照的位置，如果不设置参数，更新事务日志将被存储到默认位置。  
- dataDir=F:\\zk\\tmp\\zookeeper  
- # 错误日志的存放位置  
- dataLogDir=F:\\zk\\logs\\zookeeper  
- 
- # ZK 服务器端的监听端口  
- clientPort=2181
   上面的说明介绍：http://zookeeper.apache.org/doc/current/zookeeperStarted.html
   然后 cd 到bin 目录下 执行zkServer.cmd 就启动成功了。
   注意：dataDir  和  dataLogDir 目录不会自动创建，得手动创建才能启动。
   可以用netstat -ano|findstr "2181" 看看是否OK。
   也可以用JPS 查看启动的JAVA 进程的情况，会出现这样
Java代码  ![收藏代码](http://greemranqq.iteye.com/images/icon_star.png)
- C:\windows\system32>jps  
- 8068
- 10040 QuorumPeerMain  // 这东西是zk的东西，源码有介绍
- 10556 Jps  
   也可以用自带客户端命令 ：  zkCli.cmd -server 127.0.0.1:2181
   关于JPS的东西，可以自己去JAVA_HOME\bin 目录下去看，里面很多命令。
四、JAVA 操作zookeeper :
       上面安装挺简单的，我们来实际操作下：
       4.1  导入依赖：
Java代码  ![收藏代码](http://greemranqq.iteye.com/images/icon_star.png)
- <dependency>  
-            <groupId>org.apache.zookeeper</groupId>  
-            <artifactId>zookeeper</artifactId>  
-            <version>3.4.6</version>  
-        </dependency>  
    4.2 JAVA 实现：这里我们简单实现上面的栗子
Java代码  ![收藏代码](http://greemranqq.iteye.com/images/icon_star.png)
- // 根节点
- publicstaticfinal String ROOT = "/root-ktv";  
- 
- publicstaticvoid main(String[] args) throws Exception {  
- // 创建一个与服务器的连接
-     ZooKeeper zk = new ZooKeeper("localhost:2181", 30000, new Watcher() {  
- // 监控所有被触发的事件
- publicvoid process(WatchedEvent event) {  
-             System.out.println("状态:" + event.getState()+":"+event.getType()+":"+event.getWrapper()+":"+event.getPath());  
-         }  
-     });  
- // 创建一个总的目录ktv，并不控制权限，这里需要用持久化节点，不然下面的节点创建容易出错
-     zk.create(ROOT, "root-ktv".getBytes(), ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);  
- 
- // 然后杭州开一个KTV ,       PERSISTENT_SEQUENTIAL 类型会自动加上 0000000000 自增的后缀
-     zk.create(ROOT+"/杭州KTV", "杭州KTV".getBytes(), ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT_SEQUENTIAL);  
- 
- // 也可以在北京开一个,       EPHEMERAL session 过期了就会自动删除
-     zk.create(ROOT+"/北京KTV", "北京KTV".getBytes(), ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL);  
- 
- // 同理，我可以在北京开多个，EPHEMERAL_SEQUENTIAL  session 过期自动删除，也会加数字的后缀
-     zk.create(ROOT+"/北京KTV-分店", "北京KTV-分店".getBytes(), ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL_SEQUENTIAL);  
- 
- // 我们也可以 来看看 一共监视了多少家的ktv
-     List<String> ktvs = zk.getChildren(ROOT, true);  
-     System.out.println(Arrays.toString(ktvs.toArray()));  
- for(String node : ktvs){  
- // 删除节点
-         zk.delete(ROOT+"/"+node,-1);  
-     }  
- // 根目录得最后删除的
-     zk.delete(ROOT, -1);  
-     zk.close();  
- }  
    zookeeper 的结构图和其他的一些功能，可参考：                                                                                         http://www.blogjava.net/shenh062326/archive/2011/10/29/zookeeper_yuling.html
 三、zookeeper 伪集群
      上面栗子我们看到，如果我们的监控室也停电了，那不是就监测不到KTV情况了？一般情况下，zk 也是作为分布式部署了，也就是有多台监控，由于监控多了，肯定要有一定为准(比如直播会有一些延迟)，就要涉及到选举的算法，这里暂时不介绍，先搭建一个伪集群，因为机器不够，只能再一台机器上模拟搭建,整个过程无非是将上面的一些配置copy 几份，然后配置不同的 地址和端口就行。
       3.1  我们将F:\zookeeper-3.4.6\conf\下的zoo.cfg 改成zoo1.cfg,内容改为：
Java代码  ![收藏代码](http://greemranqq.iteye.com/images/icon_star.png)
- # 存储内存中数据库快照的位置，如果不设置参数，更新事务日志将被存储到默认位置。  
- # 每一个文件路径和下面的对应,zk1 zk2 zk3  
- dataDir=F:\\zk\\tmp\\zk1  
- # 错误日志的存放位置  
- dataLogDir=F:\\zk\\logs\\zk1  
- 
- # ZK 服务器端的监听端口  
- # 对应分别：218121822183
- clientPort=2181
- # 伪集群   
- #2887 是server 之间通讯的，3887 是应用程序通讯的  
- # 同时加入其他两个服务的地址和端口信息  
- server.1=127.0.0.1:2887:3887
- server.2=127.0.0.1:2888:3888
- server.3=127.0.0.1:2889:3889
- # 最后在钱文件目录下创建3份，zoo1.cfg,zoo2.cfg,zoo3,cfg 记得改参数  
   3.2 同时我们将F:\zookeeper-3.4.6\bin\下的 zkServer.cmd 改为zkServer1.cmd,内容加上：
Java代码  ![收藏代码](http://greemranqq.iteye.com/images/icon_star.png)
- set ZOOMAIN=org.apache.zookeeper.server.quorum.QuorumPeerMain  
- # 读取配置的路径，每个启动服务对应一份  
- set ZOOCFG=..\conf\zoo1.cfg  
- # 同理创建3个zkServer1.cmd，zkServer2.cmd,zkServer3.cmd  记得改zoo 123.cfg  
   3.3 还得在dataDir 指定目录，也就是F:\\zk\\tmp\\zk1 下创建myid 的文件，内容对应1 2 3 即可。
  这个的数字是唯一的，在1-255 之间，用来表示自身的id(其实我不明白 为啥zk 要这么设计- -！)
   3.4 启动3个zkServer1.cmd 就OK了，如果要多服务器配置，只需要要将 3份分开放到不同服务器就OK
   依次启动的时刻有错误信息，因为你启动server1 的时候 2 和 3 没找到，但是后面都启动了 就没问题了。
小结：
      1.zookeeper  现在大家都用得比较多，这里也是仅仅介绍下入门知识，linux 上安装区别不打，还是得看具体应用。
      2.关于选举、一致性 和一些其他的东西，慢慢再写！
      3.有错误，请指出哦~。~ 感激。
安装配置很简单，官网也有介绍，这里就只对后面的文章有提到的点说明下。
配置-zoo.cfg
这是zookeeper的主要配置文件，因为Zookeeper是一个集群服务，集群的每个节点都需要这个配置文件。为了避免出差错，zoo.cfg这个配置文件里没有跟特定节点相关的配置，所以每个节点上的这个zoo.cfg都是一模一样的配置。这样就非常便于管理了，比如我们可以把这个文件提交到版本控制里管理起来。其实这给我们设计集群系统的时候也是个提示：集群系统一般有很多配置，应该尽量将通用的配置和特定每个服务的配置(比如服务标识)分离，这样通用的配置在不同服务之间copy就ok了。ok，下面来介绍一些配置点：
clientPort=2181
client port，顾名思义，就是客户端连接zookeeper服务的端口。这是一个TCP port。
dataDir=/data
dataLogDir=/datalog
dataLogDir如果没提供的话使用的则是dataDir。zookeeper的持久化都存储在这两个目录里。dataLogDir里是放到的顺序日志(WAL)。而dataDir里放的是内存数据结构的snapshot，便于快速恢复。为了达到性能最大化，一般建议把dataDir和dataLogDir分到不同的磁盘上，这样就可以充分利用磁盘顺序写的特性。
下面是集群中服务的列表
server.1=127.0.0.1:20881:30881
server.2=127.0.0.1:20882:30882
server.3=127.0.0.1:20883:30883
在上面的例子中，我把三个zookeeper服务放到同一台机器上。上面的配置中有两个TCP port。后面一个是用于Zookeeper选举用的，而前一个是Leader和Follower或Observer交换数据使用的。我们还注意到server.后面的数字。这个就是myid(关于myid是什么下一节会介绍)。
上面这几个是一些基本配置。
还有像 tickTime，这是个时间单位定量。比如tickTime=1000，这就表示在zookeeper里1 tick表示1000 ms，所有其他用到时间的地方都会用多少tick来表示。
比如 syncLimit = 2 就表示fowller与leader的心跳时间是2 tick。
maxClientCnxns -- 对于一个客户端的连接数限制，默认是60，这在大部分时候是足够了。但是在我们实际使用中发现，在测试环境经常超过这个数，经过调查发现有的团队将几十个应用全部部署到一台机器上，以方便测试，于是这个数字就超过了。
minSessionTimeout, maxSessionTimeout -- 一般，客户端连接zookeeper的时候，都会设置一个session timeout，如果超过这个时间client没有与zookeeper server有联系，则这个session会被设置为过期(如果这个session上有临时节点，则会被全部删除，这就是实现集群感知的基础，后面的文章会介绍这一点)。但是这个时间不是客户端可以无限制设置的，服务器可以设置这两个参数来限制客户端设置的范围。
autopurge.snapRetainCount，autopurge.purgeInterval -- 客户端在与zookeeper交互过程中会产生非常多的日志，而且zookeeper也会将内存中的数据作为snapshot保存下来，这些数据是不会被自动删除的，这样磁盘中这样的数据就会越来越多。不过可以通过这两个参数来设置，让zookeeper自动删除数据。autopurge.purgeInterval就是设置多少小时清理一次。而autopurge.snapRetainCount是设置保留多少个snapshot，之前的则删除。
不过如果你的集群是一个非常繁忙的集群，然后又碰上这个删除操作，可能会影响zookeeper集群的性能，所以一般会让这个过程在访问低谷的时候进行，但是遗憾的是zookeeper并没有设置在哪个时间点运行的设置，所以有的时候我们会禁用这个自动删除的功能，而在服务器上配置一个cron，然后在凌晨来干这件事。
以上就是zoo.cfg里的一些配置了。下面就来介绍myid。
配置-myid
在dataDir里会放置一个myid文件，里面就一个数字，用来唯一标识这个服务。这个id是很重要的，一定要保证整个集群中唯一。zookeeper会根据这个id来取出server.x上的配置。比如当前id为1，则对应着zoo.cfg里的server.1的配置。
2. 而且在后面我们介绍leader选举的时候，这个id的大小也是有意义的。
OK，上面就是配置的讲解了，现在我们可以启动zookeeper集群了。进入到bin目录，执行 ./zkServer.sh start即可。
参考资料：
官网的：
http://zookeeper.apache.org/doc/trunk/zookeeperStarted.html
API 文档：
http://zookeeper.apache.org/doc/r3.4.6/api/
别人介绍的一些流程：
http://cailin.iteye.com/blog/2014486
常用的场景：
http://nileader.blog.51cto.com/1381108/1040007
