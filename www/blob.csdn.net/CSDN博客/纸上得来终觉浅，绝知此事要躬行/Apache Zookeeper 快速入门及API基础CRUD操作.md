# Apache Zookeeper 快速入门及API基础CRUD操作 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年04月07日 17:37:57[boonya](https://me.csdn.net/boonya)阅读数：1052








本文内容涵盖Zookeeper概述、Zookeeper的基础（架构、分层命明空间、会话、观察者）、Zookeeper安装、Zookeeper工作流、Zookeeper API 基础连接节点的CRUD操作。



# Zookeeper概述



ZooKeeper是一种分布式协调服务，用于管理大型主机。在分布式环境中协调和管理服务是一个复杂的过程。ZooKeeper通过其简单的架构和API解决了这个问题。ZooKeeper允许开发人员专注于核心应用程序逻辑，而不必担心应用程序的分布式特性。

ZooKeeper框架最初是在“Yahoo!"上构建的，用于以简单而稳健的方式访问他们的应用程序。后来，Apache ZooKeeper成为Hadoop，HBase和其他分布式框架使用的有组织服务的标准。例如，Apache HBase使用ZooKeeper跟踪分布式数据的状态。

在进一步移动之前，重要的是我们知道关于分布式应用程序的一两件事情。因此，让我们开始讨论，快速概述分布式应用程序。

## 分布式应用程序

分布式应用可以在给定时间(同时)通过在它们自己之间协调以快速和有效的方式完成特定任务而在网络中的多个系统上运行。通常，通过使用所有涉及的系统的计算能力，分布式应用可以在几分钟内完成非分布式应用(在单个系统中运行)需要数小时完成的复杂和耗时的任务。

通过将分布式应用程序配置为在更多系统上运行，可以进一步减少完成任务的时间。其中运行分布式应用的一组系统称为集群，并且在集群中运行的每个机器被称为节点。

分布式应用程序有两部分， Server 和 Client 应用程序。服务器应用程序实际上是分布式的，并具有通用接口，以便客户端可以连接到集群中的任何服务器，并获得相同的结果。客户端应用程序是与分布式应用程序交互的工具。

![Zookeeper 概述](https://img.w3cschool.cn/attachments/image/20161229/1482983310991406.png)


### 分布式应用程序的优点
- 
可靠性 - 单个或几个系统的故障不会使整个系统出现故障。

- 
可扩展性 - 可以通过添加更多机器，在应用程序配置中进行微小更改而不增加停机时间，从而提高性能。

- 
透明 - 隐藏系统的复杂性，并将其显示为单个实体/应用程序。


### 分布式应用的挑战
- 
种族条件 - 两个或多个机器尝试执行特定任务，实际上只需在任何给定时间由单个机器完成。例如，共享资源只能在任何给定时间由单个机器修改。

- 
死锁 - 两个或多个操作等待互相无限期完成。

- 
不一致 - 数据部分失败。


## 什么是Apache ZooKeeper？

Apache ZooKeeper是由集群(节点组)使用的一种服务，用于在自身之间协调并使用鲁棒同步技术维护共享数据。ZooKeeper本身是一个分布式应用程序，提供用于编写分布式应用程序的服务。

ZooKeeper提供的常见服务如下 :
- 
命名服务 - 按名称标识集群中的节点。它类似于DNS，但是对于节点。

- 
配置管理 - 加入节点的系统的最新和最新的配置信息。

- 
集群管理 - 实时加入/退出集群中的节点和节点状态。

- 
选举算法 - 选择节点作为协调目的的leader。

- 
锁定和同步服务 - 在修改数据时锁定数据。此机制可帮助您在连接其他分布式应用程序(如Apache HBase)时进行自动故障恢复。

- 
高度可靠的数据注册表 - 即使一个或几个节点关闭时数据的可用性。


分布式应用程序提供了很多好处，但它们也提出了一些复杂和难以克服的挑战。ZooKeeper框架提供了一个完整的机制来克服所有的挑战。使用故障安全同步方法来处理比赛条件和死锁。另一个主要缺点是数据的不一致性，ZooKeeper使用原子性解析。

## ZooKeeper的好处

这里有使用ZooKeeper的好处 :
- 
简单分布式协调过程

- 
同步 - 服务器进程之间的相互排斥和协作。此过程有助于Apache HBase进行配置管理。

- 
订购的邮件

- 
序列化 - 根据特定规则对数据进行编码。确保应用程序始终运行。这种方法可以在MapReduce中使用来协调队列以执行运行的线程。

- 
可靠性

- 
原子性 - 数据传输成功或完全失败，但没有事务处理。



# Zookeeper基础

在深入了解ZooKeeper的工作之前，让我们来看看ZooKeeper的基本概念。 我们将在本章中讨论以下主题:
1、架构
2、分层命名空间
3、会话
4、观察者


## ZooKeeper的架构



看看下面的图表。 它描述了ZooKeeper的“客户端 - 服务器架构"。

![ZooKeeper的架构](https://img.w3cschool.cn/attachments/day_161229/201612291344222238.jpg)




作为ZooKeeper体系结构的一部分的每个组件在下表中进行了说明。
|部分|描述|
|----|----|
|Client|客户端，我们的分布式应用程序集群中的一个节点，从服务器访问信息。对于特定的时间间隔，每个客户端向服务器发送消息以使服务器知道客户端是活着的。类似地，当客户端连接时，服务器发送确认。如果连接的服务器没有响应，客户端会自动将消息重定向到另一个服务器。|
|Server|服务器，我们的ZooKeeper集合中的一个节点，为客户端提供所有的服务。向客户端发送确认，通知服务器处于活动状态。|
|Ensemble|ZooKeeper服务器组。形成整体所需的最小节点数为3。|
|Leader|服务器节点，如果任何连接的节点发生故障，则执行自动恢复。领导者在服务启动时被选举。|
|Follower|服务器节点跟随引导指令。|
分层命名空间
下图描述了用于内存表示的ZooKeeper文件系统的树结构。ZooKeeper节点称为 znode 。每个znode由一个名称标识，并用路径(/)序列分隔。
- 
在图中，首先有一个根 znode 以“/"分隔。在根目录下，您有两个逻辑命名空间 config 和 workers 。

- 
config 命名空间用于集中配置管理， workers 命名空间用于命名。

- 
在 config 命名空间下，每个znode最多可存储1MB的数据。这类似于UNIX文件系统，除了父znode也可以存储数据。这种结构的主要目的是存储同步数据并描述znode的元数据。此结构称为 ZooKeeper数据模型。


![分层命名空间](https://img.w3cschool.cn/attachments/day_161229/201612291345162031.jpg)


ZooKeeper数据模型中的每个znode都维护着一个 stat 结构。A stat仅提供znode的元数据。它由版本号，操作控制列表(ACL)，时间戳和数据长度组成
- 
版本号 - 每个znode都有版本号，这意味着每次与znode关联的数据发生更改时，其对应的版本号也会增加。当多个zookeeper客户端尝试在同一znode上执行操作时，版本号的使用很重要。

- 
操作控制列表(ACL) - ACL基本上是访问znode的认证机制。它管理所有znode读取和写入操作。

- 
时间戳 - 时间戳表示创建和修改znode所经过的时间。它通常表示为毫秒。ZooKeeper标识“事务ID"(zxid)对znode的每个更改。 Zxid 是唯一的，并且为每个事务维护时间，以便您可以轻松地确定从一个请求到另一个请求的时间。

- 
数据长度 - 存储在znode中的数据总量是数据长度。您最多可以存储1MB的数据。


### Znodes的类型

Znodes被分为持久性，连续性和短暂性。
- 
Persistence znode - 即使在创建该特定znode的客户端断开连接后，Persistence znode仍然存在。默认情况下，除非另有说明，否则所有znode都是持久的。

- 
临时znode - 临时znode有效，直到客户端活着。当客户端从ZooKeeper集合断开连接时，临时znode会自动删除。为此，只有临时znode不允许有孩子进一步。如果删除临时znode，则下一个合适的节点将填充其位置。临时znode在领导选举中发挥重要作用。

- 
顺序znode - 顺序znode可以是持久的或短暂的。当一个新的znode被创建为一个连续的znode，然后ZooKeeper设置znode的路径，通过附加一个10位的序列号到原始名称。例如，如果将具有路径 / myapp 的znode创建为顺序znode，则ZooKeeper将将路径更改为 / myapp0000000001 ，并将下一个序列号设置为0000000002.如果两个顺序znode是并发创建的，那么ZooKeeper从不对每个znode使用相同的数字。顺序znode在锁定和同步中起重要作用。


## 会话

会话对于ZooKeeper的操作非常重要。会话中的请求按FIFO顺序执行。一旦客户端连接到服务器，将建立会话并向客户端分配会话ID 。

客户端以特定的时间间隔发送心跳以保持会话有效。如果ZooKeeper集合没有从客户端接收到超过在服务启动时指定的时间段(会话超时)的心跳，则它决定客户端死亡。

会话超时通常用毫秒表示。当会话由于任何原因结束时，在该会话期间创建的临时znode也会被删除。

## 观察者

观察者（Watcher）是一种简单的机制，使客户端得到关于ZooKeeper集合中的更改的通知。客户端可以在读取特定znode时设置Watcher。Watcher会向注册的客户端发送任何znode(客户端注册表)更改的通知。

Znode更改是与znode的相关数据的修改或znode的孩子的更改。只触发一次watcher。如果客户端想要再次通知，则必须通过另一个读取操作来完成。当连接会话过期时，客户端将与服务器断开连接，相关联的Watcher也将被删除。

# Zookeeper安装


在安装ZooKeeper之前，请确保您的系统正在以下任何操作系统上运行:
- 
任何Linux操作系统 - 支持开发和部署。它适用于演示应用。

- 
Windows操作系统 - 仅支持开发。

- 
Mac OS - 仅支持开发。


ZooKeeper服务器是用Java创建的，它在JVM上运行。您需要使用JDK 6或更高版本。

现在，按照以下步骤在您的机器上安装ZooKeeper框架。

## 步骤1:验证Java安装

我们相信您已经在系统上安装了Java环境。只需使用以下命令验证它。
$ java -version

如果您在机器上安装了Java，那么您可以看到已安装的Java的版本。否则，请按照以下简单步骤安装最新版本的Java。

### 步骤1.1:下载JDK

通过访问以下链接下载最新版本的JDK，并下载最新版本。[Java](http://www.oracle.com/technetwork/java/javase/downloads/index.html)

最新版本(在编写本教程时)是JDK 8u 60，文件是“jdk-8u60-linuxx64.tar.gz"。请在您的机器上下载该文件。

### 步骤1.2:解压缩文件

通常，文件会下载到下载文件夹中。使用以下命令验证它并解压缩tar设置。
$ cd /go/to/download/path
$ tar -zxf jdk-8u60-linux-x64.gz

### 步骤1.3:移动到opt目录

要使Java对所有用户可用，请将提取的Java内容移动到“/ usr / local / java"文件夹。
$ su 
password: (type password of root user)
$ mkdir /opt/jdk
$ mv jdk-1.8.0_60 /opt/jdk/

### 步骤1.4:设置路径

要设置路径和JAVA_HOME变量，请将以下命令添加到〜/ .bashrc文件。
export JAVA_HOME = /usr/jdk/jdk-1.8.0_60
export PATH=$PATH:$JAVA_HOME/bin

现在，将所有更改应用到当前运行的系统。
$ source ~/.bashrc

### 步骤1.5:Java替代

使用以下命令更改Java替代项。
update-alternatives --install /usr/bin/java java /opt/jdk/jdk1.8.0_60/bin/java 100

### 步骤1.6

使用步骤1中说明的验证命令(java -version)验证Java安装。

## 步骤2:ZooKeeper框架安装

### 步骤2.1:下载ZooKeeper

要在您的计算机上安装ZooKeeper框架，请访问以下链接并下载最新版本的ZooKeeper。[http://zookeeper.apache.org/releases.html](http://zookeeper.apache.org/releases.html)

到目前为止，最新版本的ZooKeeper是3.4.6(ZooKeeper-3.4.6.tar.gz)。

### 步骤2.2:解压tar文件

使用以下命令提取tar文件 -
$ cd opt/
$ tar -zxf zookeeper-3.4.6.tar.gz
$ cd zookeeper-3.4.6
$ mkdir data

### 步骤2.3:创建配置文件

使用命令 vi conf / zoo.cfg 打开名为 conf / zoo.cfg 的配置文件，并将所有以下参数设置为起点。
$ vi conf/zoo.cfg

tickTime = 2000
dataDir = /path/to/zookeeper/data
clientPort = 2181
initLimit = 5
syncLimit = 2

一旦成功保存配置文件，再次返回终端。您现在可以启动zookeeper服务器。

### 步骤2.4:启动ZooKeeper服务器

执行以下命令 -
$ bin/zkServer.sh start

执行此命令后，您将收到一个响应如下 -
$ JMX enabled by default
$ Using config: /Users/../zookeeper-3.4.6/bin/../conf/zoo.cfg
$ Starting zookeeper ... STARTED

### 步骤2.5:启动CLI

键入以下命令 -
$ bin/zkCli.sh

输入上述命令后，您将连接到ZooKeeper服务器，您应该得到以下响应。
Connecting to localhost:2181
................
................
................
Welcome to ZooKeeper!
................
................
WATCHER::
WatchedEvent state:SyncConnected type: None path:null
[zk: localhost:2181(CONNECTED) 0]

### 停止ZooKeeper服务器

连接服务器并执行所有操作后，可以使用以下命令停止zookeeper服务器。
$ bin/zkServer.sh stop





# Zookeeper工作流



一旦ZooKeeper集合启动，它将等待客户端连接。客户端将连接到ZooKeeper集合中的一个节点。它可以是领导者或跟随者节点。一旦客户端被连接，该节点向该特定客户端分配会话ID并向该客户端发送确认。如果客户端没有得到确认，它只是尝试连接ZooKeeper集合中的另一个节点。一旦连接到节点，客户端将以定期间隔向节点发送心跳，以确保连接不会丢失。
- 
如果客户端想要读取特定的znode，会向具有znode路径的节点发送读取请求，并且节点通过从其自己的数据库获取它来返回所请求的znode。为此，在ZooKeeper集合中读取速度快。

- 
如果客户端想要将数据存储在ZooKeeper集合中，它会将znode路径和数据发送到服务器。连接的服务器将该请求转发给领导者，然后领导者将向所有的跟随者重新发出写入请求。如果只有大多数节点成功响应，则写请求将成功，并且成功的返回码将被发送到客户端。否则，写入请求将失败。严格的大多数节点被称为 Quorum 。


## ZooKeeper集合中的节点

让我们分析在ZooKeeper集合中拥有不同数量的节点的效果。
- 
如果我们有单个节点，则当该节点失败时，ZooKeeper集合失败。它有助于“单点故障"，并且不推荐在生产环境中使用。

- 
如果我们有两个节点和一个节点失败，我们没有多数，因为两个中的一个不是多数。

- 
如果我们有三个节点和一个节点失败，我们有大多数，所以，这是最低要求。ZooKeeper集合在实际生产环境中必须至少有三个节点。

- 
如果我们有四个节点和两个节点失败，它再次失败，它类似于有三个节点。额外节点不用于任何目的，因此，最好添加奇数的节点，例如3,5,7。


我们知道写入过程比ZooKeeper集合中的读取过程昂贵，因为所有节点都需要在其数据库中写入相同的数据。因此，与具有用于平衡环境的大量节点相比，具有更少数量的节点(3,5或7)是更好的。

下图描述了ZooKeeper WorkFlow，后面的表解释了它的不同组件。

![Zookeeper - 工作流](https://img.w3cschool.cn/attachments/image/20161229/1482990578752713.png)

|零件|描述|
|----|----|
|Write|写过程由领导节点处理。领导者将写请求转发到所有znode，并等待znode的答案。如果znode的一半回复，则写入过程完成。|
|Read|读取由特定连接的znode在内部执行，因此不需要与群集交互。|
|复制数据库|它用于在zookeeper中存储数据。每个znode都有自己的数据库，每个znode在一致性的帮助下每次都有相同的数据。|
|Leader|Leader是负责处理写入请求的Znode。|
|Follower|关注者从客户端接收写请求并将它们转发到领导znode。|
|请求处理器|只存在于领导节点。它管理来自从节点的写请求。|
|原子广播|负责将从领导节点到从节点的变化广播。|



# ZooKeeper API






ZooKeeper有一个用于Java和C的官方API绑定.Zookeeper社区为大多数语言(.NET，python等)提供非官方API。使用ZooKeeper API，应用程序可以连接，交互，操作数据，协调，最后断开与ZooKeeper集合。

ZooKeeper API具有丰富的功能，以一种简单和安全的方式获得ZooKeeper集合的所有功能。ZooKeeper API提供同步和异步方法。

ZooKeeper集合和ZooKeeper API在每个方面完全互补，它以一种伟大的方式对开发人员有利。让我们在本章讨论Java绑定。

## ZooKeeper API的基础知识

与ZooKeeper集合的应用程序交互称为 ZooKeeper客户端或简称客户端。

Znode是ZooKeeper集合的核心组件，ZooKeeper API提供了一组小集合来使用ZooKeeper集合来操纵znode的所有细节。

客户端应该遵循以下给出的步骤来与ZooKeeper集合进行清晰和干净的交互。
- 
连接到ZooKeeper集合。ZooKeeper集合为客户端分配会话ID。

- 
定期向服务器发送心跳。否则，ZooKeeper集合将过期会话ID，客户端需要重新连接。

- 
只要会话ID处于活动状态，就可以获取/设置znode。

- 
所有任务完成后，断开与ZooKeeper集成的连接。如果客户端长时间不活动，则ZooKeeper集合将自动断开客户端。


## Java绑定

让我们来了解本章中最重要的一组ZooKeeper API。ZooKeeper API的中心部分是 ZooKeeper类。它提供了在其构造函数中连接ZooKeeper集合的选项，并具有以下方法 -
- 
connect - 连接到ZooKeeper集合

- 
create- 创建znode

- 
exists- 检查znode是否存在及其信息

- 
getData - 从特定znode获取数据

- 
setData - 在特定znode中设置数据

- 
getChildren - 获取特定znode中的所有子节点

- 
delete - 获取特定的znode及其所有子项

- 
close - 关闭连接


## 连接到ZooKeeper

ZooKeeper类通过其构造函数提供连接功能。构造函数的签名如下 :



```java
ZooKeeper(String connectionString, int sessionTimeout, Watcher watcher)
```


- 
connectionString - ZooKeeper集成主机。

- 
sessionTimeout - 会话超时(以毫秒为单位)。

- 
watcher - 实现“观察者"界面的对象。ZooKeeper集合通过观察者对象返回连接状态。


让我们创建一个新的帮助类 ZooKeeperConnection ，并添加一个方法 connect 。connect 方法创建一个ZooKeeper对象，连接到ZooKeeper集合，然后返回对象。

这里 CountDownLatch 用于停止(等待)主进程，直到客户端与ZooKeeper集合连接。

这里 CountDownLatch 用于停止(等待)主进程，直到客户端与ZooKeeper集合连接。...

这里是与ZooKeeper集合连接的完整代码。

### 编码:ZooKeeperConnection.java


```java
// import java classes
import java.io.IOException;
import java.util.concurrent.CountDownLatch;

// import zookeeper classes
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.Watcher.Event.KeeperState;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.AsyncCallback.StatCallback;
import org.apache.zookeeper.KeeperException.Code;
import org.apache.zookeeper.data.Stat;

public class ZooKeeperConnection {

   // declare zookeeper instance to access ZooKeeper ensemble
   private ZooKeeper zoo;
   final CountDownLatch connectedSignal = new CountDownLatch(1);

   // Method to connect zookeeper ensemble.
   public ZooKeeper connect(String host) throws IOException,InterruptedException {
	
      zoo = new ZooKeeper(host,5000,new Watcher() {
		
         public void process(WatchedEvent we) {

            if (we.getState() == KeeperState.SyncConnected) {
               connectedSignal.countDown();
            }
         }
      });
		
      connectedSignal.await();
      return zoo;
   }

   // Method to disconnect from zookeeper server
   public void close() throws InterruptedException {
      zoo.close();
   }
}
```




保存上面的代码，它将在下一节中用于连接ZooKeeper集群。

## 创建Znode

ZooKeeper类提供创建方法在ZooKeeper集合中创建一个新的znode。create 方法的签名如下 -



```java
create(String path, byte[] data, List<ACL> acl, CreateMode createMode)
```


- 
路径 - Znode路径。例如，/ myapp1，/ myapp2，/ myapp1 / mydata1，myapp2 / mydata1 / myanothersubdata

- 
data - 要存储在指定znode路径中的数据

- 
acl - 要创建的节点的访问控制列表。ZooKeeper API提供了一个静态接口 ZooDefs.Ids 来获取一些基本的acl列表。例如，ZooDefs.Ids.OPEN_ACL_UNSAFE返回打开znode的acl列表。

- 
createMode - 节点的类型，短暂，顺序或两者。这是一个枚举。


让我们创建一个新的Java应用程序来检查ZooKeeper API的 create 功能。创建文件 ZKCreate.java 。在main方法中，创建一个类型为 ZooKeeperConnection 的对象，并调用 connect 方法连接到ZooKeeper集合。

connect方法将返回ZooKeeper对象 zk 。现在，请使用自定义路径和数据调用 zk 对象的 create 方法。

创建znode的完整程序代码如下 -

### 编码:ZKCreate.java


```java
import java.io.IOException;

import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.Watcher.Event.KeeperState;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.ZooDefs;

public class ZKCreate {
   // create static instance for zookeeper class.
   private static ZooKeeper zk;

   // create static instance for ZooKeeperConnection class.
   private static ZooKeeperConnection conn;

   // Method to create znode in zookeeper ensemble
   public static void create(String path, byte[] data) throws 
      KeeperException,InterruptedException {
      zk.create(path, data, ZooDefs.Ids.OPEN_ACL_UNSAFE,
      CreateMode.PERSISTENT);
   }

   public static void main(String[] args) {

      // znode path
      String path = "/MyFirstZnode"; // Assign path to znode

      // data in byte array
      byte[] data = "My first zookeeper app".getBytes(); // Declare data
		
      try {
         conn = new ZooKeeperConnection();
         zk = conn.connect("localhost");
         create(path, data); // Create the data to the specified path
         conn.close();
      } catch (Exception e) {
         System.out.println(e.getMessage()); //Catch error message
      }
   }
}
```
一旦编译和执行应用程序，将在ZooKeeper集合中创建具有指定数据的znode。您可以使用ZooKeeper CLI zkCli.sh 进行检查。



```
cd /path/to/zookeeper
bin/zkCli.sh
>>> get /MyFirstZnode
```



## 存在 - 检查Znode的存在

ZooKeeper类提供了 exists方法来检查znode的存在。如果指定的znode存在，则返回znode的元数据。存在方法的签名如下 -



```java
exists(String path, boolean watcher)
```


- 
path- Znode路径

- 
watcher - 布尔值，用于指定是否观看指定的znode


让我们创建一个新的Java应用程序来检查ZooKeeper API的“exists"功能。创建文件“ZKExists.java"。在main方法中，使用“ZooKeeperConnection"对象创建ZooKeeper对象“zk"然后，使用自定义“path"调用“zk"对象的“exists"完整的列表如下 -

### 编码:ZKExists.java


```java
import java.io.IOException;

import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.Watcher.Event.KeeperState;
import org.apache.zookeeper.data.Stat;

public class ZKExists {
   private static ZooKeeper zk;
   private static ZooKeeperConnection conn;

   // Method to check existence of znode and its status, if znode is available.
   public static Stat znode_exists(String path) throws
      KeeperException,InterruptedException {
      return zk.exists(path, true);
   }

   public static void main(String[] args) throws InterruptedException,KeeperException {
      String path = "/MyFirstZnode"; // Assign znode to the specified path
			
      try {
         conn = new ZooKeeperConnection();
         zk = conn.connect("localhost");
         Stat stat = znode_exists(path); // Stat checks the path of the znode
				
         if(stat != null) {
            System.out.println("Node exists and the node version is " +
            stat.getVersion());
         } else {
            System.out.println("Node does not exists");
         }
				
      } catch(Exception e) {
         System.out.println(e.getMessage()); // Catches error messages
      }
   }
}
```
一旦编译和执行应用程序，您将获得以下输出。



```java
Node exists and the node version is 1.
```



## getData方法

ZooKeeper类提供 getData 方法来获取指定znode中附加的数据及其状态。getData 方法的签名如下 -



```java
getData(String path, Watcher watcher, Stat stat)
```


- 
path - Znode路径。

- 
watcher - 类型观察者的回调函数。当指定的znode的数据改变时，ZooKeeper集合将通过Watcher回调通知。这是一次性通知。

- 
stat - 返回znode的元数据。


让我们创建一个新的Java应用程序来了解ZooKeeper API的 getData 功能。创建文件 ZKGetData.java 。在main方法中，使用他 ZooKeeperConnection 对象创建一个ZooKeeper对象 zk 。然后，使用自定义路径调用zk对象的 getData 方法。

下面是从指定节点获取数据的完整程序代码 -

### 编码:ZKGetData.java


```java
import java.io.IOException;
import java.util.concurrent.CountDownLatch;

import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.Watcher.Event.KeeperState;
import org.apache.zookeeper.data.Stat;

public class ZKGetData {

   private static ZooKeeper zk;
   private static ZooKeeperConnection conn;
   public static Stat znode_exists(String path) throws 
      KeeperException,InterruptedException {
      return zk.exists(path,true);
   }

   public static void main(String[] args) throws InterruptedException, KeeperException {
      String path = "/MyFirstZnode";
      final CountDownLatch connectedSignal = new CountDownLatch(1);
		
      try {
         conn = new ZooKeeperConnection();
         zk = conn.connect("localhost");
         Stat stat = znode_exists(path);
			
         if(stat != null) {
            byte[] b = zk.getData(path, new Watcher() {
				
               public void process(WatchedEvent we) {
					
                  if (we.getType() == Event.EventType.None) {
                     switch(we.getState()) {
                        case Expired:
                        connectedSignal.countDown();
                        break;
                     }
							
                  } else {
                     String path = "/MyFirstZnode";
							
                     try {
                        byte[] bn = zk.getData(path,
                        false, null);
                        String data = new String(bn,
                        "UTF-8");
                        System.out.println(data);
                        connectedSignal.countDown();
							
                     } catch(Exception ex) {
                        System.out.println(ex.getMessage());
                     }
                  }
               }
            }, null);
				
            String data = new String(b, "UTF-8");
            System.out.println(data);
            connectedSignal.await();
				
         } else {
            System.out.println("Node does not exists");
         }
      } catch(Exception e) {
        System.out.println(e.getMessage());
      }
   }
}
```

一旦编译和执行应用程序，您将获得以下输出

`My first zookeeper app`应用程序将等待ZooKeeper集合的进一步通知。使用ZooKeeper CLI zkCli.sh 更改指定znode的数据。


```
cd /path/to/zookeeper
bin/zkCli.sh
>>> set /MyFirstZnode Hello
```


现在，应用程序将打印以下输出并退出。


`Hello`


## setData方法

ZooKeeper类提供 setData 方法来修改指定znode中附加的数据。setData 方法的签名如下 :



```java
setData(String path, byte[] data, int version)
```


- 
path- Znode路径

- 
data - 要存储在指定znode路径中的数据。

- 
version- znode的当前版本。每当数据更改时，ZooKeeper会更新znode的版本号。


现在让我们创建一个新的Java应用程序来了解ZooKeeper API的 setData 功能。创建文件 ZKSetData.java 。在main方法中，使用 ZooKeeperConnection 对象创建一个ZooKeeper对象 zk 。然后，使用指定的路径，新数据和节点版本调用 zk 对象的 setData 方法。

这里是完整的程序代码来修改附加在指定znode中的数据。

### 代码:ZKSetData.java


```java
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.Watcher.Event.KeeperState;

import java.io.IOException;

public class ZKSetData {
   private static ZooKeeper zk;
   private static ZooKeeperConnection conn;

   // Method to update the data in a znode. Similar to getData but without watcher.
   public static void update(String path, byte[] data) throws
      KeeperException,InterruptedException {
      zk.setData(path, data, zk.exists(path,true).getVersion());
   }

   public static void main(String[] args) throws InterruptedException,KeeperException {
      String path= "/MyFirstZnode";
      byte[] data = "Success".getBytes(); //Assign data which is to be updated.
		
      try {
         conn = new ZooKeeperConnection();
         zk = conn.connect("localhost");
         update(path, data); // Update znode data to the specified path
      } catch(Exception e) {
         System.out.println(e.getMessage());
      }
   }
}
```
一旦应用程序被编译和执行，指定的znode的数据将被改变，并且可以使用ZooKeeper CLI， zkCli.sh 进行检查。



```
cd /path/to/zookeeper
bin/zkCli.sh
>>> get /MyFirstZnode
```



## getChildren方法

ZooKeeper类提供 getChildren 方法来获取特定znode的所有子节点。getChildren 方法的签名如下 -



```java
getChildren(String path, Watcher watcher)
```


- 
path - Znode路径。

- 
watcher - 类型为“Watcher"的回调函数。当指定的znode被删除或znode下的子节点被创建/删除时，ZooKeeper集合将通知。这是一次性通知。


### 编码:ZKGetChildren.java


```java
import java.io.IOException;
import java.util.*;

import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.Watcher.Event.KeeperState;
import org.apache.zookeeper.data.Stat;

public class ZKGetChildren {
   private static ZooKeeper zk;
   private static ZooKeeperConnection conn;

   // Method to check existence of znode and its status, if znode is available.
   public static Stat znode_exists(String path) throws 
      KeeperException,InterruptedException {
      return zk.exists(path,true);
   }

   public static void main(String[] args) throws InterruptedException,KeeperException {
      String path = "/MyFirstZnode"; // Assign path to the znode
		
      try {
         conn = new ZooKeeperConnection();
         zk = conn.connect("localhost");
         Stat stat = znode_exists(path); // Stat checks the path

         if(stat!= null) {

            //“getChildren" method- get all the children of znode.It has two
            args, path and watch
            List <String> children = zk.getChildren(path, false);
            for(int i = 0; i < children.size(); i++)
            System.out.println(children.get(i)); //Print children's
         } else {
            System.out.println("Node does not exists");
         }

      } catch(Exception e) {
         System.out.println(e.getMessage());
      }

   }

}
```

在运行程序之前，让我们使用ZooKeeper CLI zkCli.sh 为 / MyFirstZnode 创建两个子节点。


```
cd /path/to/zookeeper
bin/zkCli.sh
>>> create /MyFirstZnode/myfirstsubnode Hi
>>> create /MyFirstZnode/mysecondsubmode Hi
```


现在，编译和运行程序将输出上面创建的znode。



```
myfirstsubnode
mysecondsubnode
```



## 删除Znode

ZooKeeper类提供了 delete 方法来删除指定的znode。删除方法的签名如下 -



```java
delete(String path, int version)
```


- 
path - Znode路径。

- 
version - znode的当前版本。


让我们创建一个新的Java应用程序来了解ZooKeeper API的 delete 功能。创建文件 ZKDelete.java 。在main方法中，使用 ZooKeeperConnection 对象创建一个ZooKeeper对象 zk 。然后，使用指定的路径和版本号调用 zk 对象的 delete

删除znode的完整程序代码如下 

### 编码:ZKDelete.java


```java
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.KeeperException;

public class ZKDelete {
   private static ZooKeeper zk;
   private static ZooKeeperConnection conn;

   // Method to check existence of znode and its status, if znode is available.
   public static void delete(String path) throws KeeperException,InterruptedException {
      zk.delete(path,zk.exists(path,true).getVersion());
   }

   public static void main(String[] args) throws InterruptedException,KeeperException {
      String path = "/MyFirstZnode"; //Assign path to the znode
		
      try {
         conn = new ZooKeeperConnection();
         zk = conn.connect("localhost");
         delete(path); //delete the node with the specified path
      } catch(Exception e) {
         System.out.println(e.getMessage()); // catches error messages
      }
   }
}
```




参考地址：[http://www.w3cschool.cn/zookeeper/](http://www.w3cschool.cn/zookeeper/)




