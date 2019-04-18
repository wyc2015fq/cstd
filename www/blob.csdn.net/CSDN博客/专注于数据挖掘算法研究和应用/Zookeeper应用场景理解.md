# Zookeeper应用场景理解 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月07日 14:28:44[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2309








发现分布式工具，谁都不能缺了zookeeper，虽然此前笔者在http://blog.csdn.net/fjssharpsword/article/details/50884201博客中略有了解，但总感觉意犹未尽，这里重点理解下zookeeper的应用场景。




1、Zookeeper部署和安装，参考官网就可以。

      http://zookeeper.apache.org/doc/current/zookeeperStarted.html




2、Zookeeper的数据模型



Zookeeper 会维护一个具有层次关系的数据结构，它非常类似于一个标准的文件系统，如下图：

#####  Zookeeper 数据结构
![图 1 Zookeeper 数据结构](http://www.ibm.com/developerworks/cn/opensource/os-cn-zookeeper/image001.gif)
Zookeeper 这种数据结构有如下这些特点：

1）每个子目录项如 NameService 都被称作为 znode，这个 znode 是被它所在的路径唯一标识，如 Server1 这个 znode 的标识为 /NameService/Server1；

2）znode 可以有子节点目录，并且每个 znode 可以存储数据，注意 EPHEMERAL 类型的目录节点不能有子节点目录；

3）znode 是有版本的，每个 znode 中存储的数据可以有多个版本，也就是一个访问路径中可以存储多份数据；

4）znode 可以是临时节点，一旦创建这个 znode 的客户端与服务器失去联系，这个 znode 也将自动删除，Zookeeper 的客户端和服务器通信采用长连接方式，每个客户端和服务器通过心跳来保持连接，这个连接状态称为 session，如果 znode 是临时节点，这个 session 失效，znode 也就删除了；

5）znode 的目录名可以自动编号，如 App1 已经存在，再创建的话，将会自动命名为 App2；

6）znode 可以被监控，包括这个目录节点中存储的数据的修改，子节点目录的变化等，一旦变化可以通知设置监控的客户端，这个是 Zookeeper 的核心特性，Zookeeper 的很多功能都是基于这个特性实现的，后面在典型的应用场景中会有实例介绍；


正是基于这个数据结构的特性，Zookeeper作为一个分布式的服务框架，可用来解决分布式集群中应用系统的一致性问题，它能提供基于类似于文件系统的目录节点树方式的数据存储，当然 Zookeeper 并不是用来专门存储数据的，它的主要作用是用来维护和监控你存储的数据的状态变化。通过监控这些数据状态的变化，从而可以达到基于数据的集群管理。




3、Zookeeper机制


1）数据发布与订阅 

应用配置集中到节点上，应用启动时主动获取，并在节点上注册一个watcher，每次配置更新都会通知到应用。


2）名空间服务

分布式命名服务，创建一个节点后，节点的路径就是全局唯一的，可以作为全局名称使用。


3）分布式通知/协调

不同的系统都监听同一个节点，一旦有了更新，另一个系统能够收到通知。


4）分布式锁

Zookeeper能保证数据的强一致性，用户任何时候都可以相信集群中每个节点的数据都是相同的。一个用户创建一个节点作为锁，另一个用户检测该节点，如果存在，代表别的用户已经锁住，如果不存在，则可以创建一个节点，代表拥有一个锁。



5）集群管理

每个加入集群的机器都创建一个节点，写入自己的状态。监控父节点的用户会受到通知，进行相应的处理。离开时删除节点，监控父节点的用户同样会收到通知。




Zookeeper 从设计模式角度来看，是一个基于观察者模式设计的分布式服务管理框架，它负责存储和管理大家都关心的数据，然后接受观察者的注册，一旦这些数据的状态发生变化，Zookeeper 就将负责通知已经在 Zookeeper 上注册的那些观察者做出相应的反应，从而实现集群中类似 Master/Slave 管理模式。从一个直观感觉上说，这是一个分布式集群的管理工具，通过什么来管理呢？就是它的数据结构和机制。





4、Zookeeper的应用场景

1）统一命名服务（Name Service）

分布式应用中，通常需要有一套完整的命名规则，既能够产生唯一的名称又便于人识别和记住，通常情况下用树形的名称结构是一个理想的选择，树形的名称结构是一个有层次的目录结构，既对人友好又不会重复。说到这里你可能想到了 JNDI，没错 Zookeeper 的 Name Service 与 JNDI 能够完成的功能是差不多的，它们都是将有层次的目录结构关联到一定资源上，但是 Zookeeper 的 Name Service 更加是广泛意义上的关联，也许你并不需要将名称关联到特定资源上，你可能只需要一个不会重复名称，就像数据库中产生一个唯一的数字主键一样。

Name Service 已经是 Zookeeper 内置的功能，你只要调用 Zookeeper 的 API 就能实现。如调用 create 接口就可以很容易创建一个目录节点。

2）配置管理（Configuration Management）

配置的管理在分布式应用环境中很常见，例如同一个应用系统需要多台 PC Server 运行，但是它们运行的应用系统的某些配置项是相同的，如果要修改这些相同的配置项，那么就必须同时修改每台运行这个应用系统的 PC Server，这样非常麻烦而且容易出错。

像这样的配置信息完全可以交给 Zookeeper 来管理，将配置信息保存在 Zookeeper 的某个目录节点中，然后将所有需要修改的应用机器监控配置信息的状态，一旦配置信息发生变化，每台应用机器就会收到 Zookeeper 的通知，然后从 Zookeeper 获取新的配置信息应用到系统中。


##### 配置管理结构图
![图 2. 配置管理结构图](http://www.ibm.com/developerworks/cn/opensource/os-cn-zookeeper/image002.gif)



3）集群管理（Group Membership）

Zookeeper 能够很容易的实现集群管理的功能，如有多台 Server 组成一个服务集群，那么必须要一个“总管”知道当前集群中每台机器的服务状态，一旦有机器不能提供服务，集群中其它集群必须知道，从而做出调整重新分配服务策略。同样当增加集群的服务能力时，就会增加一台或多台 Server，同样也必须让“总管”知道。

Zookeeper 不仅能够帮你维护当前的集群中机器的服务状态，而且能够帮你选出一个“总管”，让这个总管来管理集群，这就是 Zookeeper 的另一个功能 Leader Election。

它们的实现方式都是在 Zookeeper 上创建一个 EPHEMERAL 类型的目录节点，然后每个 Server 在它们创建目录节点的父目录节点上调用 getChildren(String path, boolean watch) 方法并设置 watch 为 true，由于是 EPHEMERAL 目录节点，当创建它的 Server 死去，这个目录节点也随之被删除，所以 Children 将会变化，这时 getChildren上的 Watch 将会被调用，所以其它 Server 就知道已经有某台 Server
 死去了。新增 Server 也是同样的原理。

Zookeeper 如何实现 Leader Election，也就是选出一个 Master Server。和前面的一样每台 Server 创建一个 EPHEMERAL 目录节点，不同的是它还是一个 SEQUENTIAL 目录节点，所以它是个 EPHEMERAL_SEQUENTIAL 目录节点。之所以它是 EPHEMERAL_SEQUENTIAL 目录节点，是因为我们可以给每台 Server 编号，我们可以选择当前是最小编号的 Server 为 Master，假如这个最小编号的 Server 死去，由于是 EPHEMERAL
 节点，死去的 Server 对应的节点也被删除，所以当前的节点列表中又出现一个最小编号的节点，我们就选择这个节点为当前 Master。这样就实现了动态选择 Master，避免了传统意义上单 Master 容易出现单点故障的问题。



##### 集群管理结构图
![图 3. 集群管理结构图](http://www.ibm.com/developerworks/cn/opensource/os-cn-zookeeper/image003.gif)



4）共享锁（Locks）

共享锁在同一个进程中很容易实现，但是在跨进程或者在不同 Server 之间就不好实现了。Zookeeper 却很容易实现这个功能，实现方式也是需要获得锁的 Server 创建一个 EPHEMERAL_SEQUENTIAL 目录节点，然后调用 getChildren方法获取当前的目录节点列表中最小的目录节点是不是就是自己创建的目录节点，如果正是自己创建的，那么它就获得了这个锁，如果不是那么它就调用 exists(String path, boolean watch) 方法并监控 Zookeeper 上目录节点列表的变化，一直到自己创建的节点是列表中最小编号的目录节点，从而获得锁，释放锁很简单，只要删除前面它自己所创建的目录节点就行了。



##### Zookeeper 实现 Locks 的流程图
![图 4. Zookeeper 实现 Locks 的流程图](http://www.ibm.com/developerworks/cn/opensource/os-cn-zookeeper/image004.gif)



5）队列管理

Zookeeper 可以处理两种类型的队列：

当一个队列的成员都聚齐时，这个队列才可用，否则一直等待所有成员到达，这种是同步队列。

队列按照 FIFO 方式进行入队和出队操作，例如实现生产者和消费者模型。

同步队列用 Zookeeper 实现的实现思路如下：

创建一个父目录 /synchronizing，每个成员都监控标志（Set Watch）位目录 /synchronizing/start 是否存在，然后每个成员都加入这个队列，加入队列的方式就是创建 /synchronizing/member_i 的临时目录节点，然后每个成员获取 / synchronizing 目录的所有目录节点，也就是 member_i。判断 i 的值是否已经是成员的个数，如果小于成员个数等待 /synchronizing/start 的出现，如果已经相等就创建 /synchronizing/start。

用下面的流程图更容易理解：



##### 同步队列流程图
![图 5. 同步队列流程图](http://www.ibm.com/developerworks/cn/opensource/os-cn-zookeeper/image005.gif)



FIFO 队列用 Zookeeper 实现思路如下：

实现的思路也非常简单，就是在特定的目录下创建 SEQUENTIAL 类型的子目录 /queue_i，这样就能保证所有成员加入队列时都是有编号的，出队列时通过 getChildren( ) 方法可以返回当前所有的队列中的元素，然后消费其中最小的一个，这样就能保证 FIFO。





5、总结

 Zoopkeeper 提供了一套分布式集群管理的机制，基于层次型的目录树的数据结构，并对树中的节点进行有效管理，从而可以设计出多种多样的分布式的数据管理模型。这种数据结构及其机制是实现分布式集群管理的基础，自然在这个之上还有很多的应用场合和想象空间。在分布式生产环境中，资源的最大化和各项工作的步调一致是很重要，需要Zookeeper这种工具的参与。




