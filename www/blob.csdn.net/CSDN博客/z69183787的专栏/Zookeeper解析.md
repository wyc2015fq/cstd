# Zookeeper解析 - z69183787的专栏 - CSDN博客
2016年09月08日 11:02:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：859
转自公司内部分享：
一、Zookeeper简介
1、定义
zookeeper是一个分布式数据一致性解决方案。
2、历史
zookeeper 起源于雅虎研究院的一个研究小组。
“zookeeper”名字的由来。
3、Zookeeper 特点
顺序一致性：按照客户端发送请求的顺序更新数据。
原子性：更新要么成功，要么失败，不会出现部分更新。
单一性 ：无论客户端连接哪个server，都会看到同一个视图。
可靠性：一旦数据更新成功，将一直保持，直到新的更新。
及时性：客户端会在一个确定的时间内得到最新的数据。
二、Zookeeper的使用场景
1、数据发布/订阅
又叫配置中心，即发布者将数据发布到zookeeper集群，供订阅者进行数据订阅，进而达到动态获取数据的目的，实现配置信息的集中式管理和数
据动态更新。
2、命名服务
分布式应用中，通常需要有一套完整的命名规则，既能够产生唯一的名称又便于人识别和记住。被命名的实体通常可以使集群中的机器、提供服
务的地址或远程对象等。比如较为常见的分布式服务框架rpc中的服务地址列表，通过使用命名服务，客户端应用能够根据制定名字来获取服务地址
。命名服务提供注册、注销和查看命名等接口。
3、分布式协调/通知
分布式协调/通知服务是分布式系统中不可缺少的一个环节，是将不同分布式组件有机结合起来的关键所在。
对于分布式应用集群而言，引入这样一个协调者，便于将分布式协调职责从应用集群中分离出来，从而大大减少系统间的耦合性，而且能够显著提
高系统的可扩展性。
4、集群管理
所谓集群管理，包括集群监控与集群控制。集群监控侧重对集群运行时状态进行收集；集群控制对集群进行操作和控制。
例如如下需求：
a、希望知道当前集群中究竟有多少机器在工作。
b、对集群中每台机器的运行时状态进行数据收集。
b、对集群中每台机器的运行时状态进行数据收集。
c、对集群中的机器进行上下线操作。
例如分布式日志收集集群。
5、选举master
对于一个集群而言，很多情况需要动态的选举出一个master机器，负责一些特殊的任务。例如数据库集群中选举master机器进行写操作。
6、分布式锁
分布式锁是控制分布式系统之间同步访问共享资源的一种方式。
分为排它锁和共享锁。
三、Zookeeper的实现原理
1、系统模型
1）集群节点角色
no role Responsibilities
1 Leader a、事物请求的唯一调度和处理者，保证集群事务处理的顺序；
b、集群内部各服务器的调度者
2 Follower a、处理客户端非事物请求，转发事物请求给Leader服务器；
b、参与事物请求Proposal的投票；
c、参与Leader选举投票
3 Observer a、处理客户端非事物请求，转发事物请求给Leader服务器；
2）数据模型
Zookeeper表现为一个分层的文件系统目录树结构（不同于文件系统的是，节点可以有自己的数据，而文件系统中的目录节点只有子节点），每个
数据节点都被称作一个ZNode。
no 数据节点类型
1 持久节点创建后一直存在，直到有删除操作主动清楚该节点。
2 持久顺序节点在ZK中，每个父节点会为他的第一级子节点维护一份时序，会记录每个子节点创建的先后顺序。基于这个特性，在创建子节点的时候，可3 临时节点和持久节点不同的是，临时节点的生命周期和客户端会话绑定。也就是说，如果客户端会话失效，那么这个节点就会自动被清除掉。注意4 临时顺序节点在临时节点上加上顺序特性
3）ACL
Zookeeper的ACL，可以从三个维度来理解：一是scheme; 二是user; 三是permission，通常表示为scheme:id:permissions,
下面从这三个方面分别来介绍：
（1）scheme: 缺省支持下面几种scheme:
no name description
1 world 它下面只有一个id, 叫anyone, world:anyone代表任何人，zookeeper中对所有人有权限的结点就是属于world:anyone的
2 auth 它不需要id, 只要是通过authentication的user都有权限（zookeeper支持通过kerberos来进行authencation,
也支持username/password形式的authentication)
3 digest 它对应的id为username:BASE64(SHA1(password))，它需要先通过username:password形式的authentication
4 ip 它对应的id为客户机的IP地址，设置的时候可以设置一个ip段，比如ip:192.168.1.0/16, 表示匹配前16个bit的IP段
5 super 在这种scheme情况下，对应的id拥有超级权限，可以做任何事情(cdrwa)
（2）id: id与scheme是紧密相关的，具体的情况在上面介绍scheme的过程都已介绍，这里不再赘述。
（3）permission: zookeeper目前支持下面一些权限：
no name description
1 CREATE 创建权限，可以在在当前node下创建child node
2 DELETE 删除权限，可以删除当前的node
3 READ 读权限，可以获取当前node的数据，可以list当前node所有的child nodes
4 WRITE 写权限，可以向当前node写数据
5 ADMIN 管理权限，可以设置当前node的permission
4）版本
发挥着乐观锁的作用
no name description
1 version 当前数据节点数据内容的版本号
2 cversion 当前数据节点子节点的版本号
3 aversion 当前数据节点ACL变更版本号
2、ZAB协议
ZAB协议，全称Zookeeper Atomic
Broadcast，由Paxos算法发展而来。ZAB协议作为Zookeeper集群数据一致性的核心算法，为Zookeeper专门设计的一种支持崩溃恢复的原子广
播协议。
Zookeeper使用单一的主进程(即Leader)来接受并处理客户端的所有事物请求；并采用ZAB的原子广播协议，将服务器数据的状态变更以事物Prop
osal的形式广播到所有副本进程(即Follower)；之后Leader等待所有Follower反馈，如果有半数Follower进行正确反馈，那么Leader会再次向所有
Follower分发Commit消息，要求将前一Proposal提交，同时Leader自身业务完成对事物的提交。
ZAB协议包括崩溃恢复和消息广播两种模式。当Zookeeper集群处在启动、或是Leader出现网络中断、崩溃退出与重启等异常时，Zookeeper会
进入崩溃恢复模式。之后Zookeeper集群会进行新的Leader选举，当新的Leader选举出来，且集群中过半机器与Leader完成状态同步后，Zookee
per集群会进入消息广播模式。
当处在消息广播模式时。在广播事务Proposal之前 ，Leader会为这个事物分配一个全局单调递增且唯一ID(即ZXID)。由于ZAB协议必须能够保证
一个全局的变更序列被顺序引用，因此必须将每一个事物Proposal按照其ZXID的先后顺序来进行排序处理。
当处在崩溃恢复模式时。ZAB协议需要保证那些已经在Leader上提交的事物最终被所有服务器提交。同时确保丢弃那些只在Leader被提出的事物。
针对这个要求，Leader选举算法需要保证新选举出来的Leader拥有集群中所有机器最大ZXID的事物Proposal。ZXID(64bit)=epoch(32bit)+32bit
单调递增计数。
3、选举leader
选举Leader步骤：
1）自增选举轮次；
2）初始化内部投票；
3）发送内部投票；
4）接收外部投票；
5）判断选举伦次：
    a、如外部投票大于内部投票，则更新内部投票，然后3）；
    b、如果外部投票小于内部投票，则忽略；
    c、如外部投票等于内部投票，则6）。
6）投票PK：
    a、先比较ZXID，大者胜出；
    b、如两者ZXID一样，则比较SID，大者胜出。
7）PK结果，如外部投票胜出，则更新内部投票，然后3）
如内部投票胜出，则8）。
8）统计投票：
    a、如过半投票支持内部投票，则9），
    b、否则4）。
9）更新服务器状态：
如内部投票投的是自己，就将自己的状态修改为LEADING；
否则，改为FOLLOWING。
4、Watcher机制
1）步骤：
a、客户端注册Watcher；
b、服务端处理Watcher；
c、客户端回调Watcher
2）事件类型
KeeperStatus EventType 触发条件说明
SynConnected None 客户端与服务器端成功建立会话客户端与服务器处于连接状态
NodeCreated Watcher监听的对应数据节点被创建
NodeDeleted Watcher监听的对应数据节点被删除
NodeDataChanged Watcher监听的对应数据节点的数据内容发生变更
NodeChildrenChanged Watcher监听的对应数据节点的子节点列表发生变更
Disconnected None 客户端与zookeeper服务器断开连接客户端与服务器处于断开状态
Expired None 会话超时客户端会话失效，通常同时也会收到SessionExpiredException
AuthFailed None 通常有两种情况：
a、使用错误的scheme进行权限检查；
b、SASL权限检查失败
通常会收到AuthFailedException异常
3）特点
a、一次性；
b、客户端串行执行
c、轻量
四、总结
ZooKeeper是一个高可用的分布式数据管理与系统协调框架。基于对Paxos算法的改进实现，使该框架保证了分布式环境中数据的强一致性，
也正是基于这样的特性，使得zookeeper能够应用于很多场景。
PPT：http://pan.baidu.com/s/1c63SOQ
