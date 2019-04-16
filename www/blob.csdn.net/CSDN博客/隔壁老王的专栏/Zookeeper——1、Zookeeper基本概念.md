# Zookeeper——1、Zookeeper基本概念 - 隔壁老王的专栏 - CSDN博客





2016年11月30日 22:03:01[隔壁老王来也](https://me.csdn.net/haoyuyang)阅读数：1434








**1.**什么是Zookeeper？



Zookeeper是一个高效的分布式协调服务，它暴露了一些公用服务，比如命名/配置管理/同步控制/群组服务等。我们可以使用Zookeeper来实现比如达成共识/集群管理/leader选举等。

Zookeeper是一个高可用的分布式管理与协调框架，基于ZAB算法（原子消息广播协议）的实现。该框架能够很好的保证分布式环境中数据的一致性。也只是基于这样的特性，使得Zookeeper成为了解决分布式一致性问题的利器。

Zookeeper的特性：

①顺序一致性：从一个客户端发起的事务请求，最终将会严格的按照其发起的顺序被应用到zookeeper中去。

②原子性：所有事务请求的处理结果在整个集群中所有机器上的应用情况是一致的，也就是说，要么整个集群所有的机器都成功应用了某一事务，要么没有应用，一定不会出现部分机器应用了该事务，而另一部分没有应用的情况。

③单一视图：无论客户端连接的是哪一个zookeeper服务器，其看到的服务器端数据模型都是一致的。

④可靠性：一旦服务器成功地应用了一个事务，并完成对客户端的响应，那么该事务所引起的服务器端状态将会被一直保留，除非有另一个事务对其更改。

⑤实时性：通常所说的实时性就是指一旦事务被成功应用，那么客户端就能立刻从服务器上获取变更后的新数据，zookeeper仅仅能保证在一段时间内，客户端最终能从服务器端读取最新的数据状态。

**2.Zookeeper设计目标**

①简单的数据结构。zookeeper就是以简单的树形结构来进行相互协调的（也叫树形名字空间）。

![](https://img-blog.csdn.net/20161130224525621)![](https://img-blog.csdn.net/20161201223024417)


其中圆形节点可以含有子节点，多边形节点不能含有子节点。一个节点对应一个应用，节点存储的数据就是应用需要的配置信息。

②可以构建集群。一般zookeeper集群通常由一组机器构成，一般3~5台机器就可以组成一个zookeeper集群了。只要集群中超过半数以上的机器能够正常工作，那么整个集群就能够正常对外提供服务。

③顺序访问。对于来自每一个客户端的每一个请求，zookeeper都会分配一个全局唯一的递增编号，这个编号反应了所有事务操作的先后顺序，应用程序可以使用zookeeper的这个特性来实现更高层次的同步。

④高性能。由于zookeeper将全局数据存储在内存中，并直接服务于所有的非事务请求，因此尤其是在以读操作为主的场景下性能非常突出。在JMater压力测试下（100%读请求场景下），其结果大约在12~13W的QPS。

**3.Zookeeper的数据模型**

①每个子目录项如NameService都被称为znode，这个znode是被它所在的路径唯一标识，如Server1这个znode的标识为/NameService/Server1。

②znode可以有子节点目录，并且每个znode可以存储数据，注意EPHEMERAL类型的目录节点不能有子节点目录。

③znode是有版本的，每个znode中存储的数据可以有多个版本，也就是一个访问路径中可以存储多份数据。

④znode可以是临时节点，一旦创建这个znode的客户端与服务器失去联系，这个znode也将自动删除。Zookeeper的客户端和服务器通信采用长连接方式，每个客户端和服务器通过心跳来保持连接，这个连接状态称为session。如果znode是临时节点，这个session失效，znode也就自动删除了。

⑤znode的目录名可以自动编号，如app1已存在，再创建的话，也将自动重命名为app2。

⑥znode可以被监控，包括这个目录节点中存储数据的修改，子节点目录的变化等。一旦变化，可以通知设置监控的客户端，这个是zookeeper的核心特性，zookeeper的很多功能都是基于这个特性实现的。

**4.Zookeeper的组成**

ZK Server根据其身份特性分为三种：leader、follower、observer，其中follower和observer又统称为learner（学习者）。

leader：负责客户端的writer类型请求。

follower：负责客户端的reader类请求，参与leader的选举等。

observe：特殊的”follower“，其可以接受客户端reader请求，但不参与选举，只负责与leader同步数据。

**5.Zookeeper的应用场景（该部分出自[分布式服务框架 Zookeeper -- 管理分布式环境中的数据](http://netcome.iteye.com/blog/1474255)）**

Zookeeper从设计模式角度来看，是一个基于观察者模式设计的分布式服务管理框架，它负责存储和管理大家都关心的数据，然后接受观察者的注册，一旦这些数据的状态发生变化，Zookeeper就将负责通知已经在Zookeeper上注册的那些观察者做出相应的反应，从而实现集群中类似Master/Slave的管理模式。

下面将详细介绍这些典型的应用场景，也就是Zookeeper到底能帮我们解决哪些问题？

**1）统一命名服务（Name Service）**

分布式应用中，通常需要有一套完整的命名规则，既能够产生唯一的名称又便于人们识别和记忆，通常情况下用树形的名称结构是一个理想的选择，树形的名称结构是一个有层次的目录结构，既对人友好又不会重复。说到这里你可能会想到JNDI，没错Zookeeper的Name Service与JNDI更够完成的功能是差不多的，它们都是将有层次的目录结构关联到一定的资源上，但是Zookeeper的Name Service更加是广泛意义上的关联，也许你并不需要将名称关联到特定资源上，你可能只是需要一个不会重复的名称，就像数据库中产生的一个唯一数字主键一样。

Name Service已经是Zookeeper内置的功能，只需要调用Zookeeper的API就能实现。如调用Create接口就可以很容易创建一个目录节点。

**2）配置管理（Configuration Management）**

配置管理在分布式应用环境中很常见，例如同一个系统需要多台PC Server运行，但是它们运行的应用系统的某些配置项是相同的，如果要修改这些相同的配置项，那么就必须同时修改每台运行了这个应用系统的PC Server，这样非常麻烦而且容易出错。

像这样的配置信息完全可以交给Zookeeper来管理，将配置信息保存在Zookeeper的某个目录节点中，然后将所有需要修改的应用机器监控配置信息的状态，一旦配置信息发生变化，每台应用机器就会收到Zookeeper的通知，然后从Zookeeper获取新的配置信息应用到系统中。

配置管理结构图：

![](https://img-blog.csdn.net/20161204214519855)


**3）集群管理**

Zookeeper能够很容易的实现集群管理功能，如有多台Server组成一个服务集群，那么必须要有一个“总管”知道当前集群中每台机器的服务状态，一旦有机器不能提供服务，集群中其他集群必须知道，从而做出调整重新分配服务策略。同样当增加集群的服务能力时，就会增加一台或多台Server，同样也必须让“总管”知道。

Zookeeper不仅能够维护当前集群中机器的服务状态，而且能够选出一个“总管”，让这个总管来管理集群，这就是Zookeeper的另一个功能Leader Election。

它们的实现方式都是在Zookeeper上创建一个EPHEMERAL类型的目录节点，然后每个Server在它们创建目录节点的父目录节点上调用getChildren(String path, boolean watch)方法并设置watch为true，由于是EPHEMERAL类型的节点，当创建它的Server死去，这个目录节点也随之被删除，所以Children将会变化，这时getChildren上的Watch将会被调用，所以其它Server就知道已经有某台Server死去了。新增Server也是同样的原理。

Zookeeper 如何实现 Leader Election，也就是选出一个 Master Server。和前面的一样每台 Server 创建一个 EPHEMERAL 目录节点，不同的是它还是一个 SEQUENTIAL 目录节点，所以它是个 EPHEMERAL_SEQUENTIAL 目录节点。之所以它是 EPHEMERAL_SEQUENTIAL 目录节点，是因为我们可以给每台 Server 编号，我们可以选择当前是最小编号的 Server 为 Master，假如这个最小编号的 Server 死去，由于是
 EPHEMERAL 节点，死去的 Server 对应的节点也被删除，所以当前的节点列表中又出现一个最小编号的节点，我们就选择这个节点为当前 Master。这样就实现了动态选择 Master，避免了传统意义上单 Master 容易出现单点故障的问题。


集群管理结构图：

![](https://img-blog.csdn.net/20161204215634424)


Leader Election关键代码：

```java
void findLeader() throws InterruptedException { 
        byte[] leader = null; 
        try { 
            leader = zk.getData(root + "/leader", true, null); 
        } catch (Exception e) { 
            logger.error(e); 
        } 
        if (leader != null) { 
            following(); 
        } else { 
            String newLeader = null; 
            try { 
                byte[] localhost = InetAddress.getLocalHost().getAddress(); 
                newLeader = zk.create(root + "/leader", localhost, 
                ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL); 
            } catch (Exception e) { 
                logger.error(e); 
            } 
            if (newLeader != null) { 
                leading(); 
            } else { 
                mutex.wait(); 
            } 
        } 
    }
```
**4）共享锁**
共享锁在同一个进程中很容易实现，但是在跨进程或者在不同 Server 之间就不好实现了。Zookeeper 却很容易实现这个功能，实现方式也是需要获得锁的 Server 创建一个 EPHEMERAL_SEQUENTIAL 目录节点，然后调用 getChildren方法获取当前的目录节点列表中最小的目录节点是不是就是自己创建的目录节点，如果正是自己创建的，那么它就获得了这个锁，如果不是那么它就调用exists(String path, boolean watch) 方法并监控 Zookeeper 上目录节点列表的变化，一直到自己创建的节点是列表中最小编号的目录节点，从而获得锁，释放锁很简单，只要删除前面它自己所创建的目录节点就行了。

Zookeeper实现Locks的流程图：

![](https://img-blog.csdn.net/20161204220142936)


同步锁的关键思路：

**加锁：**

ZooKeeper 将按照如下方式实现加锁的操作：
**①ZooKeeper 调用 create()方法来创建一个路径格式为“ _locknode_/lock- ”的节点，此节点类型为sequence （连续）和 ephemeral （临时）。也就是说，创建的节点为临时节点，并且所有的节点连续编号，即“ lock-i ”的格式。②在创建的锁节点上调用 getChildren()方法，来获取锁目录下的最小编号节点，并且不设置 watch 。③步骤 2 中获取的节点恰好是步骤 1 中客户端创建的节点，那么此客户端获得此种类型的锁，然后退出操作。④客户端在锁目录上调用exists()方法，并且设置 watch 来监视锁目录下比自己小一个的连续临时节点的状态。⑤如果监视节点状态发生变化，则跳转到第 2 步，继续进行后续的操作，直到退出锁竞争。解锁： ZooKeeper 解锁操作非常简单，客户端只需要将加锁操作步骤 1 中创建的临时节点删除即可。**


同步锁关键代码：

```java
void getLock() throws KeeperException, InterruptedException{ 
        List<String> list = zk.getChildren(root, false); 
        String[] nodes = list.toArray(new String[list.size()]); 
        Arrays.sort(nodes); 
        if(myZnode.equals(root+"/"+nodes[0])){ 
            doAction(); 
        } 
        else{ 
            waitForLock(nodes[0]); 
        } 
    } 
    void waitForLock(String lower) throws InterruptedException, KeeperException {
        Stat stat = zk.exists(root + "/" + lower,true); 
        if(stat != null){ 
            mutex.wait(); 
        } 
        else{ 
            getLock(); 
        } 
    }
```
**5）队列管理**
Zookeeper 可以处理两种类型的队列：

①当一个队列的成员都聚齐时，这个队列才可用，否则一直等待所有成员到达，这种是同步队列。

②队列按照 FIFO 方式进行入队和出队操作，例如实现生产者和消费者模型。

同步队列用 Zookeeper 实现的实现思路如下：

创建一个父目录 /synchronizing，每个成员都监控标志（Set Watch）位目录 /synchronizing/start 是否存在，然后每个成员都加入这个队列，加入队列的方式就是创建 /synchronizing/member_i 的临时目录节点，然后每个成员获取 / synchronizing 目录的所有目录节点，也就是 member_i。判断 i 的值是否已经是成员的个数，如果小于成员个数等待 /synchronizing/start 的出现，如果已经相等就创建 /synchronizing/start。

用下面的流程图更容易理解：


![](https://img-blog.csdn.net/20161204220711945)


同步队列关键代码：

```java
void addQueue() throws KeeperException, InterruptedException{ 
        zk.exists(root + "/start",true); 
        zk.create(root + "/" + name, new byte[0], Ids.OPEN_ACL_UNSAFE, 
        CreateMode.EPHEMERAL_SEQUENTIAL); 
        synchronized (mutex) { 
            List<String> list = zk.getChildren(root, false); 
            if (list.size() < size) { 
                mutex.wait(); 
            } else { 
                zk.create(root + "/start", new byte[0], Ids.OPEN_ACL_UNSAFE,
                 CreateMode.PERSISTENT); 
            } 
        } 
 }
```
当队列没满时进入wait()，然后会一直等待Watch通知，Watch的代码如下：
```java
public void process(WatchedEvent event) { 
        if(event.getPath().equals(root + "/start") &&
         event.getType() == Event.EventType.NodeCreated){ 
            System.out.println("得到通知"); 
            super.process(event); 
            doAction(); 
        } 
    }
```


FIFO 队列用 Zookeeper 实现思路如下：

实现的思路也非常简单，就是在特定的目录下创建 SEQUENTIAL 类型的子目录 /queue_i，这样就能保证所有成员加入队列时都是有编号的，出队列时通过 getChildren( ) 方法可以返回当前所有的队列中的元素，然后消费其中最小的一个，这样就能保证 FIFO。

生产者代码：

```java
boolean produce(int i) throws KeeperException, InterruptedException{ 
        ByteBuffer b = ByteBuffer.allocate(4); 
        byte[] value; 
        b.putInt(i); 
        value = b.array(); 
        zk.create(root + "/element", value, ZooDefs.Ids.OPEN_ACL_UNSAFE, 
                    CreateMode.PERSISTENT_SEQUENTIAL); 
        return true; 
    }
```
消费者代码：
```java
int consume() throws KeeperException, InterruptedException{ 
        int retvalue = -1; 
        Stat stat = null; 
        while (true) { 
            synchronized (mutex) { 
                List<String> list = zk.getChildren(root, true); 
                if (list.size() == 0) { 
                    mutex.wait(); 
                } else { 
                    Integer min = new Integer(list.get(0).substring(7)); 
                    for(String s : list){ 
                        Integer tempValue = new Integer(s.substring(7)); 
                        if(tempValue < min) min = tempValue; 
                    } 
                    byte[] b = zk.getData(root + "/element" + min,false, stat); 
                    zk.delete(root + "/element" + min, 0); 
                    ByteBuffer buffer = ByteBuffer.wrap(b); 
                    retvalue = buffer.getInt(); 
                    return retvalue; 
                } 
            } 
        } 
 }
```

**6.Zookeeper的安装**

①先去[zookeeper官网](http://apache.fayea.com/zookeeper/)下载zookeeper的安装包，并将zookeeper通过工具上传到服务器。

②使用mv [zookeeper-3.5.2-alpha](http://apache.fayea.com/zookeeper/zookeeper-3.5.2-alpha/).tar.gz  /usr/local/命令将zookeeper移动到/usr/local/目录下，并使用tar -zxvf [zookeeper-3.5.2-alpha](http://apache.fayea.com/zookeeper/zookeeper-3.5.2-alpha/).tar.gz解压，最后使用mv [zookeeper-3.5.2-alpha](http://apache.fayea.com/zookeeper/zookeeper-3.5.2-alpha/) zookeeper重命名zookeeper的文件夹，方便后面环境变量的配置。

③使用vi /etc/profile命令配置zookeeper的环境变量，在profile文件最后加入下面两行：

export ZOOKEEPER_HOME=/usr/local/zookeeper

export PATH=$ZOOKEEPER_HOME/bin:$PATH

为了使环境变量生效，使用source /etc/profile命令刷新环境变量。

④使用cd /usr/local/zookeeper/conf目录下修改zookeeper的配置文件。因为zookeeper默认使用的配置文件是zoo.cfg，所以使用mv zoo_sample.cfg zoo.cfg命令重命名zookeeper的配置文件。

⑤使用vi zoo.cfg命令修改zoo.cfg文件，修改如下一处：

dataDir=/usr/local/zookeeper/data

并在文件最后添加下面三行（因为我在学习zookeeper时使用了三台虚拟机）：

server.0=192.168.1.102:2888:3888

server.1=192.168.1.103:2888:3888

server.2=192.168.1.104:2888:3888

退出并保存zoo.cfg文件。

**zookeeper配置文件详解：**

**tickTime：基本事件单元，以毫秒为单位。这个时间是作为zookeeper服务器与服务器之间或客户端与服务器之间维持心跳的事件间隔。也就是每隔tickTime发送一个心跳。**

**dataDir：存储内存中数据快照的位置，就是zookeeper保存数据的目录。默认情况下，zookeeper将写数据的日志文件也保存在这个位置。**

**clientPort：客户端连接zookeeper服务器的端口，zookeeper会监听该端口，接受客户端的访问请求。**

**initLimit：用来配置zookeeper接受客户端初始化连接时最长能忍受多少个心跳时间间隔，当超过10个心跳时间（也就是tickTime）长度后，zookeeper服务器还没有收到客户端的返回信息，则认为该客户端连接失败。总的时间长度就是10 * 2000=20秒（默认情况下）。**

**syncLimit：leader与follower之间发送消息，请求和应答时间长度，最长不能超过多少个tickTime的时间长度，总的时间长度就是5 * 2000=10秒（默认情况下）。**

**server.A=B:C:D：A表示这个是第几号服务器，B表示该服务器的ip地址，C表示该服务器与集群中的leader服务器蒋欢信息的端口，D表示如果集群中的leader服务器挂了，则通过该端口进行选举，选出一个新的leader。**

⑥服务器标识配置：

在/usr/local/zookeeper目录下使用mkdir data目录，并进入data目录，接着使用vi myid创建myid文件，该文件中的内容为0（另外两台虚拟机为1，2）。

⑦启动zookeeper

进入/usr/local/zookeeper/bin目录，使用zkServer.sh start启动zookeeper（如果想看日志或者想看一下在启动过程中是否报错，可以使用zkServer.sh start-foreground命令启动zookeeper）。

使用zkServer.sh tatus来查看zookeeper的状态，我这里因为有三个节点，所以其一个为leader，另外两个为follower。

**PS：在安装zookeeper的过程中，我遇到了下面的两个问题。**

**①java.net.ConnectException: 拒绝连接，如果出现这个问题，删除hosts文件中的第一行，也就是127.0.0.1这行。**

**②java.net.NoRouteToHostException: 没有到主机的路由，如果出现这个问题，则需要关闭防火墙。**

**7.操作zookeeper（shell）**

在ZOOKEEPER_HOME/bin目录下输入zkCli.sh可以进入zookeeper客户端。

查找：ls /

创建并赋值：create /hyy "mydata"

获取：get /hyy

设置值：set /hyy "hello"

可以看到192.168.1.102、192.168.1.103、192.168.1.104的zookeeper集群中的数据是一致的。

deleteall /path删除递归节点（当前要删除的节点下还有子节点则要使用rmr命令）

delete /path/child删除指定的节点

创建节点有两种类型：短暂（ephemeral）、持久（persistent）












