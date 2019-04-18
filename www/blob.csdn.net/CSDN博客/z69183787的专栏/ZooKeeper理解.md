# ZooKeeper理解 - z69183787的专栏 - CSDN博客
2017年03月20日 16:18:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：794
## 分布式一致性问题 
http://www.cnblogs.com/xrq730/p/4944768.html
在我们日常的开发经验中，相信很多开发人员都碰到过这样的问题：假设客户端C1将系统中的一个值K由V1更新为V2，但客户端C2无法立即读取到K的最新值，需要一段时间之后才能读到。这个例子就是常见的数据库之间的复制的延时问题。
分布式系统对于数据的复制要求一般都来自一下两个原因。
·        为了增加系统的可用性，以防止单点故障引起的系统不可用。
·        提高系统的整体性能，通过负载均衡技术，能够让分布在不同地方的数据副本都能够为用户提供服务。
数据复制在可用性和性能方面给分布式系统带来的巨大好处是不言而喻的，然而数据复制所带来的一致性挑战，也是每一个系统研发人员不得不面对的。
所谓的分布式一致性问题，是指在分布式环境中引入数据复制机制后，不同数据节点间可能出现的，并无法依靠计算机应用程序自身解决的数据不一致情况。简单的讲，数据一致性就是指对一个副本数据进行更新时，必须确保也能更新其他的副本，否则不同副本之间的数据将不在一致。
那么怎么来解决这个问题呢？一种观点就是：既然是由于延时引起的问题，那我可以将写入的动作阻塞，知道数据复制完成后，才完成写入操作。没错，这似乎能解决问题，而且有一些系统的构架也确实直接使用了这个思路。但这个思路在解决一致性问题的同时，又带来了新的问题：写入的性能。如果你的应用场景有非常多的写入，那么使用这个思路之后，后续的写请求都将会阻塞在前一个请求的写操作上，导致系统整体性能急剧下降。
总的来说，我们无法找到一种能够满足分布式系统所有系统属性的分布式一致性解决方案。因此，如何保证数据的一致性，同时又不影响系统运行的性能，是每一个分布式系统都需要重点考虑和权衡的。于是，产生了下面的要说的一致性级别。
·        强一致性。这种一致性级别是最符合用户直觉的，它要求系统写入什么，读出来的就是说什么，用户体验好，但实现起来往往对系统的性能影响较大。
·        弱一致性。这种一致性级别约束了系统在写入成功后，不承若里可以读到写入的值，也不具体承诺多久之后数据能够达到一致，但会尽可能保证到某个时间级别后，数据能够达到一致状态。
·        最终一致性。最终一致性是弱一致性的一个特例，系统会保证在一定时间内，能够达到一个数据一致的状态。这里之所以将最终一致性单独提出来，是因为它是弱一致性中非常重要的一种一致性模型，也是业界待大型分布式系统的数据一致性上比较推崇的模型。
后续会深入介绍分布式一致性问题的工业解决方案--Zookeeper。
## Zookeeper介绍
       ZooKeeper是一个开放源代码的分布式协调服务，由知名互联网公司雅虎创建，是Google的Chubby的开源实现。ZooKeeper的设计目标是将那些复杂且容易出错的分布一致性服务封装起来，构成一个高效可靠的原语集，并以一系列简单易用的接口提供给用户使用。
##### Zookeeper是什么
       ZooKeeper是一个典型的分布式数据一致性的解决方案，分布式应用程序可以基于它实现诸如发布/订阅、负载均衡、命名服务、分布式协调/通知、集群管理、Master选举、分布式锁和分布式队列等功能。ZooKeeper可以保证如下分布式一致性特性。
·        顺序一致性。从同一个客户端发起的事务请求，最终将会严格地按照其发起顺序被应用到Zookeeper中去。
·        原子性。所有事务请求的处理结果在整个集群中所有机器上的应用情况是一致的，也就是说，要么整个集群所有机器都成功应用了某一个事务，要么都没应用，一定不会出现集群中部分机器应用了事务，而另外一部分没有应用的情况。
·        单一视图。无论客户端连接的是哪个Zookeeper服务器，其看到的服务器端数据模型都是一致的。
·        可靠性。一旦服务器端成功应用了一个事务，并完成对客户端的响应，那么该事务所引起的服务器端状态的改变将会一直保留下去，除非有另一个事务又对其进行了变更。
·        实时性。通常人们看到实时性的第一发应是，一旦一个事务被创建成功，那么客户端能够立即从服务器端上读取到这个事务变更后的最新数据状态。这里需要注意的是，Zookeeper仅仅保证在一定时间段内，客户端最终一定能够从服务器端读取到最新的数据状态。
##### Zookeeper的设计目标
       Zookeeper致力于提供一个高性能、高可用且具有严格的顺序访问控制能力(主要是写操作的严格顺序性)的分布式协调服务。高性能使得Zookeeper能够应用于那些对系统吞吐有明确要求的大型分布式系统中，高可用使得分布式的单点问题得到了很好地解决，而严格的顺序访问控制使得客户端能够基于Zookeeper实现复杂的同步原语。下面我们来具体看一下Zookeeper的四个设计目标。
###### 目标一：简单的数据模型
       Zookeeper使得分布式程序能够通过一个人共享的、树型结构的名字空间来进行相互协调。这里所说的树型结构的命名空间，是指Zookeeper服务器内存中的一个数据模型，其由一系列被称为ZNode的数据节点组成，总的来说，其数据模型类似于一个文件系统，而ZNode之间的层级关系，就像文件系统的目录结构一样。不过和传统的磁盘文件系统不同的是，Zookeeper讲全量数据存储在内存中，以此来实现提高服务器吞吐、减少延迟的目的。
###### 目标二：可以构建集群
一个Zookeeper集群通常由一组机器组成，一般3~5台机器就可以组成一个可用的Zookeeper集群了，如下图所示。
![](https://img-blog.csdn.net/20170320172225857?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
组成Zookeeper集群的每台机器都会在内存中维护当前的服务器状态，并且每台机器之间保持着通信。值得一提的是，主要集群中存在超过一半的机器能够正常工作，那么整个集群就能正常对外提供服务。
Zookeeper的客户端会选择集群中任意一台机器共同创建一个TCP连接，而一旦客户端和某台Zookeeper服务器之间连接断开后，客户端会自动连接到集群中的其它机器。
###### 目标三：顺序访问
对于来自客户端的每一个更新请求，Zookeeper都会分配一个全局唯一的递增编号，这个编号反映了所有事务操作的先后顺序，应用程序可以使用Zookeeper的这个特性来实现更高层次的同步原语。
###### 目标四：高性能
由于Zookeeper将全量数据存储在内存中，并直接服务于客户端的所有非事务请求，因此它尤其适用于以读操作为主的场景。
## 2.Zookeeper的基本概念
##### 集群角色
通常在分布式系统中，构成一个集群的每一台机器都有自己的角色，最典型的集群模式就是Master/Slave模式(主备模式)。在这种模式中，我们把能够处理所有写操作的机器成为Master机器，把所有通过异步复制方式获取最新数据，并且提供读服务的机器成为Slave机器。
而在ZooKeeper中，并没有使用传统的Master/Slave模式，而是引用了Leader、Follower和Observer三种角色。Zookeeper集群中的所有机器通过一个Leader选举过程来选定一台称为“Leader”的机器，Leader服务器为客户端提供读和写功能。除了Leader外，其它机器包括Follower和Observer，可以提供读服务，它们的区别在于，Observer机器不参与Leader选举过程，也不参与写操作的“过半写成功”策略，因此Observer可以在不影响写性能的情况下提升集群的读性能。
##### 会话（Session）
      Session是客户端会话，在讲解会话之前，我们首先来了解下客户端连接。在ZooKeeper中，一个客户端连接是指客户端和服务器之间的一个TCP长连接。ZooKeeper对外的服务端口默认是2181，客户端启动的事后，首先会于服务器建立一个TCP连接，从第一次连接建立开始，客户端会话的生命周期也开始了，通过这个连接，客户端能够通过心跳检测与服务端保持有效的会话，也能够向ZooKeeper服务器发送请求并接受响应，同时还能够通过该链接接受来自服务器的Watch事件通知。Session的sessionTimeout值用来设置一个客户端会话的超时时间。当由于服务器压力太大、网络故障或者客户端主动断开连接等各种原因导致客户端连接断开时，只要在sessionTimeout规定的时间内能够重新连接上集群中的任意一台服务器，那么值钱创建的会话仍然有效。
##### 数据节点(ZNode)
在谈到分布式的时候，我们通常说的“节点”是指组成集群的每一台机器。然而，在ZooKeeper中，“节点”分为两类，第一类同样是指构成集群的机器，我们称之为机器节点；第二类是指数据模型中的单元，我们称之为数据节点-ZNode。ZooKeeper讲所有数据存储在内存中，数据模型是一棵树，由斜杠（/）进行分割的路径，就是一个ZNode。每个ZNode上都会保存自己的数据内容，还会保存一系列属性信息。
在ZooKeeper中，ZNode可以分为持久节点和临时节点。所谓持久节点是指一旦这个ZNode被创建，除非主动进行ZNode的移除操作，否则这个ZNode降一直保存在ZooKeeper上。而临时节点就不一样了，它的生命周期和客户端会话绑定，一旦客户端会话失效，那么这个客户端会话创建的所有临时节点都会被移除。
##### 版本
      ZooKeeper的每一个ZNode上都会存储数据对应于每个ZNode，ZooKeeper都会为其维护一个叫做Stat的数据结构，Stat中记录了这个ZNode的三个数据版本，分别是version(当前ZNode的版本)、cversion(当前ZNode子节点的版本)和aversion(当前ZNode的ACL版本).
##### Watcher
      Watcher(事件监听器)，ZooKeeper中一个很重要的特性。ZooKeeper允许用户在指定节点上注册一些Watcher，并且在一些特定事件触发的时候，ZooKeeper服务器端会将事件通知到感兴趣的客户端上去，该机制是ZooKeeper实现分布式协调服务的重要特性。
##### ACL
       ZooKeeper采用ACL(Access Control Lists)策略来进行权限控制，类似于UNIX文件系统的权限控制，ZooKeeper定义如下五种权限。
·        CREATE：创建子节点的权限。
·        READ：获取节点数据和子节点列表。
·        WRITE：更新节点数据的权限。
·        DELETE：删除子节点的权限。
·        ADMIN：设置节点ACL的权限。
需要注意的是，CREATE和DELETE这两种权限都是针对子节点的权限控制。
## 3.为什么选择ZooKeeper
随着分布式架构的出现，越来越多的分布式应用都会面临数据一致性问题。很遗憾的是，在解决分布式数据一致性上，除了ZooKeeper之外，目前还没有一个成熟稳定切被大规模应用的解决方案。ZooKeeper无论从性能、易用性还是稳定性上来说，都已经达到了一个工业级产品的标准。
其次，ZooKeeper是开发源代码的，所有人都在关注它的发展，都有权利来贡献自己的力量，你可以和全世界成千上万的ZooKeeper开发者们一起交流经验，共同解决问题。
另外，ZooKeeper是免费的，你无须为它支付任何费用。这点对于一个小型公司，尤其是出创团队来说，无疑是非常重要的。
最后，ZooKeeper已经得到了非常广泛的应用。诸如Hadoop、Hbase、Storm和solr等越来来越多的大型分布式项目都已经将ZooKeeper作为核心组件没用于分布式协调。
参考：《从Paxos到ZooKeeper分布式一致性原理与实践》
Zookeeper系统模型
## 1.数据模型
       ZooKeeper的视图结构和标准的Unix文件系统非常类似，但没有引入传统文件系统中目录与文件等的概念，而是使用了其特有的“数据节点”的概念，称之为ZNode。ZNode是Zookeeper中数据的最小单元，每个ZNode上都可以保存数据，同时还可以挂在子节点，因此构成了一个层次化的命名空间，称之为树。ZooKeeper
下图是Zookeeper数据节点示意图。
![](https://img-blog.csdn.net/20170320172347342?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
ZNode节点路径标识方式和Unix文件系统路径非常相似，都是由一系列斜杠（/）进行分割的路径表示，开发人员可以向这个节点写入数据，也可以在节点下创建子节点。
另外，说一下ZooKeeper的事务。在ZooKeeper中，事务是指能够改变ZooKeeper服务器状态的操作，称之为事务操作或更新操作，一般包括数据节点的创建与删除、数据节点内容更新和客户端会话创建与失效等操作。对于每一个事务请求，ZooKeeper都会为其分配一个全局唯一的事务ID，用ZXID来表示，通常是一个64位的数字。每一个ZXID对应一次更新操作，从这些ZXID中可以间接地识别出ZooKeeper处理这些更新操作请求的全局顺序。
## 2.节点特性
在ZooKeeper中，每个数据节点都是有生命周期的，其生命周期的长短取决于数据节点的节点类型。在ZooKeeper中，节点类型可以分为持久节点、临时节点和顺序节点三大类，具体在创建节点时，通过组合，可以生成这四种组合类型的节点。
·        持久节点：ZooKeeper中最常见的一种节点类型。指该节点创建后，就会一直存在于Zk服务器上，直到有删除操作来主动清除这个节点。
·        持久顺序节点：和持久节点的基础属性一致，额外的特性表现在顺序上。在ZooKeeper中，每个父节点都会为它的第一级子节点维护一份顺序，用于记录每个子节点创建的先后顺序。基于这个顺序，在创建子节点的时候，可以设置这个标记。在创建节点过程中，ZooKeeper会自动为给定节点名加上一个数字后缀，作为一个新的、完整的节点名。
·        临时节点：和持久节点不同，临时节点的生命周期和客户端的会话绑定在一起，也就是说，如果客户端会话失效，那么这个节点就会自动清理掉。注意，这里提到的是客户端会话失效，而非TCP链接断开。关于ZooKeeper客户端会话和连接参考[2.会话](http://wiki.sankuai.com/pages/viewpage.action?pageId=679646862)。另外，ZooKeeper规定了不能基于临时节点来创建子节点，即临时节点只能作为叶子节点。
·        临时顺序节点：在临时节点的基础上，增加了顺序属性。
每个数据节点除了存储数据内容之外，还存储了数据节点本身的一些状态信息，使用get命令可以获取一个数据节点的内容和状态信息，如下图
![](https://img-blog.csdn.net/20170320172423543?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
从图可知，第一行是当前数据节点的内容，从第二行开始就是节点的状态信息了，这其实就是数据节点Stat对象的格式化输出。具体字段与说明见下图。
![](https://img-blog.csdn.net/20170320172446731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 3.版本-保证分布式数据原子性操作
     ZooKeeper中为数据节点引入了版本的概念，每个数据节点都具有三种类型的版本信息，对数据节点的任务更新操作都会引起版本号的变化，如下图所示。
![](https://img-blog.csdn.net/20170320172515030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
 ZooKeeper的版本表示的是对数据节点的数据内容、子节点列表或者是节点ACL的信息的修改次数。我们以version这种版本类型为例说明。在一个数据节点/zk-book被创建完毕后，节点的version值是0，表示的含义是“当前节点自从创建后，没有被更改过”。如果对该节点的数据内容进行更新操作，version的值就会变成1。如果内容从
 A变成B，再变成A，版本号会加1两次。
在ZooKeeper中，version属性是用来实现乐观锁机制中的“写入校验”的。具体看看ZooKeeper服务器的PreRequestProcess处理类处理数据更新setDataRequest请求的处理代码
![](https://img-blog.csdn.net/20170320172558671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 4. Watcher-数据变更的通知
     ZooKeeper 提供了分布式数据的发布/订阅功能。一个典型的发布/订阅模型系统定义了一种一对多的订阅关系，能够让多个订阅者同时监听某一个主体对象，当这个主体对象自身状态发生变化时，会通知所有订阅者，使它们能够做出相应的处理。在ZooKeeper中，引入了Watcher机制来实现这种分布式的通知功能。ZooKeeper允许客户端向服务器注册一个Watcher监听，当服务器的一些指定事件触发了这个Watcher，那么就会向指定客户端发送一个事件通知来实现分布式的通知功能。真个Watcher注册与通知过程如下图。
![](https://img-blog.csdn.net/20170320172619624?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
从上图可以看出，ZooKeeper的Watcher机制主要包括客户端线程、客户端WatchManager和Zk服务器三部分。在具体工作流程上，客户端在向ZooKeeper服务器注册Watcher的同时，会将Watcher存储在客户端的WatchManager中。当Zookeeper服务器触发Watcher事件后，会向客户端发送通知，客户端线程从WatchManager中取出对应的Watcher对象来执行回调逻辑。
在ZooKeeper中，接口类Watcher用于表示一个标准的事件处理器，其定义了事件通知的相关逻辑，包含KeeperState和EventType两个枚举类，分别代表了通知状态和事件类型，同时定义了回调方法：process（WatchedEvent
 event）。
```java
/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
package org.apache.zookeeper;
 
/**
 * This interface specifies the public interface an event handler class must
 * implement. A ZooKeeper client will get various events from the ZooKeepr
 * server it connects to. An application using such a client handles these
 * events by registering a callback object with the client. The callback object
 * is expected to be an instance of a class that implements Watcher interface.
 *
 */
public interface Watcher {
 
    /**
     * This interface defines the possible states an Event may represent
     */
    public interface Event {
        /**
         * Enumeration of states the ZooKeeper may be at the event
         */
        public enum KeeperState {
            /** Unused, this state is never generated by the server */
            @Deprecated
            Unknown (-1),
 
            /** The client is in the disconnected state - it is not connected
             * to any server in the ensemble. */
            Disconnected (0),
 
            /** Unused, this state is never generated by the server */
            @Deprecated
            NoSyncConnected (1),
 
            /** The client is in the connected state - it is connected
             * to a server in the ensemble (one of the servers specified
             * in the host connection parameter during ZooKeeper client
             * creation). */
            SyncConnected (3),
 
            /**
             * Auth failed state
             */
            AuthFailed (4),
 
            /**
             * The client is connected to a read-only server, that is the
             * server which is not currently connected to the majority.
             * The only operations allowed after receiving this state is
             * read operations.
             * This state is generated for read-only clients only since
             * read/write clients aren't allowed to connect to r/o servers.
             */
            ConnectedReadOnly (5),
 
            /**
              * SaslAuthenticated: used to notify clients that they are SASL-authenticated,
              * so that they can perform Zookeeper actions with their SASL-authorized permissions.
              */
            SaslAuthenticated(6),
 
            /** The serving cluster has expired this session. The ZooKeeper
             * client connection (the session) is no longer valid. You must
             * create a new client connection (instantiate a new ZooKeeper
             * instance) if you with to access the ensemble. */
            Expired (-112);
 
            private final int intValue;     // Integer representation of value
                                            // for sending over wire
 
            KeeperState(int intValue) {
                this.intValue = intValue;
            }
 
            public int getIntValue() {
                return intValue;
            }
 
            public static KeeperState fromInt(int intValue) {
                switch(intValue) {
                    case   -1: return KeeperState.Unknown;
                    case    0: return KeeperState.Disconnected;
                    case    1: return KeeperState.NoSyncConnected;
                    case    3: return KeeperState.SyncConnected;
                    case    4: return KeeperState.AuthFailed;
                    case    5: return KeeperState.ConnectedReadOnly;
                    case    6: return KeeperState.SaslAuthenticated;
                    case -112: return KeeperState.Expired;
 
                    default:
                        throw new RuntimeException("Invalid integer value for conversion to KeeperState");
                }
            }
        }
 
        /**
         * Enumeration of types of events that may occur on the ZooKeeper
         */
        public enum EventType {
            None (-1),
            NodeCreated (1),
            NodeDeleted (2),
            NodeDataChanged (3),
            NodeChildrenChanged (4);
 
            private final int intValue;     // Integer representation of value
                                            // for sending over wire
 
            EventType(int intValue) {
                this.intValue = intValue;
            }
 
            public int getIntValue() {
                return intValue;
            }
 
            public static EventType fromInt(int intValue) {
                switch(intValue) {
                    case -1: return EventType.None;
                    case  1: return EventType.NodeCreated;
                    case  2: return EventType.NodeDeleted;
                    case  3: return EventType.NodeDataChanged;
                    case  4: return EventType.NodeChildrenChanged;
 
                    default:
                        throw new RuntimeException("Invalid integer value for conversion to EventType");
                }
            }          
        }
    }
 
    abstract public void process(WatchedEvent event);
}
```
Watcher 通知状态与事件类型一览
![](https://img-blog.csdn.net/20170320172712969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
##### 工作机制【ZooKeeper挺核心的设计部分，抛砖引玉了】
ZooKeeper的Watcher工作机制，总的来说可以概括为三个过程：客户端注册Watcher、服务器端处理Watcher和客户端回调Watcher。具体工作机制过程参考《从Paxos到ZooKeeper分布式一致性原理与实践》7.1.4小节。新人看完之后肯定会很有收获的，这里就不再细讲了。
总结工作机制：
一次性：无论是服务端还是客户端，一旦一个Watcher被触发，ZooKeeper都会将其从相应的存储中移除。因此，开发人员在Watcher的使用上要记住一点的是需要反复注册。这样设计有效减轻了服务端的压力。试想，如果注册一个Watcher之后一直有效，那么，针对那些更新频繁的节点，服务端会不断向客户端发送事件通知，这无论对于网络还是对于服务端性能的影响都很大。
客户端串行执行：客户端回调的过程是一个串行同步的过程，这为我们保证了顺序，同时，需要开发人员注意的一点是，千万不要因为一个Watcher的处理逻辑影响了整个客户端的回调。
轻量：WatchedEvent是ZooKeeper整个Watcher通知机制的最小通知单元，这个数据包含三个部分：通知状态、事件类型和节点路径。也就是说，Watcher通知很简单，只会告诉客户端发生了事件，而不会说明事件的具体内容。
参考《从Paxos到ZooKeeper分布式一致性原理与实践》
 7.1 小节
Zookeeper Leader 选择
## Leader选举概述
       Leader选举是ZooKeeper中最重要的技术之一，也是保证分布式数据一致性的关键所在。
#### 1、服务器启动时期的Leader选举
我们讲解Leader选举的时候，隐式条件便是ZooKeeper的集群规模至少是2台机器，这里我们以3台机器组成的服务器集群为例。在服务器集群初始化阶段，当有一台服务器（Server1，myid为1）启动的时候，它是无法完成Leader选举的，也是无法进行Leader选举的。当第二台机器（Server1，myid为1）也启动后，此时这两台机器已经能够相互通信，每台机器都试图找到一个Leader，于是便进入了Leader选举流程。
##### a.每个Server会发出一个投票。
由于是初始情况，因此对于Server1和Server2来说，都会将自己作为Leader服务器来进行投票，每次投票包含的基本元素包括：所推举的服务器的myid和ZXID，我们以（myid，ZXID）的形式来表示。初始化阶段，无论Server1还是Server2，都会投给自己，即Server1的投票为（1，0），Server2的投票为（2，0），然后各自将这个投票发给集群中其它所有机器。
##### b.接收来自各个服务器的投票。
每个服务器都会接受来自其它服务器的投票。集群中的每一台服务器在接收到投票后，首先会判断该投票的有效性，包括检查是否是本轮投票，是否来自LOOKING状态的服务器。
##### c.处理投票。
在接收到来自其它服务器的投票后，针对每一个投票，服务器都需要将别人的投票和自己的投票进行PK。PK的规则如下。
·        优先检查ZXID。ZXID比较大的服务器优先作为Leader。
·        如果ZXID相同的话，那么就比较myid。myid比较大的服务器作为Leader服务器。
对于Server1来说，它自己的投票是（1，0），而接收的投票是（2，0），经过PK发现大于自己的投票，于是更新自己的投票为（2，0），然后重新将投票发出去。而对于Server2来说，不需要更新自己的投票信息，只是再一次向集群中所有机器发出上一次的投票信息即可。
##### d.统计投票。
每次投票之后，服务器都会统计所有投票，判断是否已经有过半(>= n/2 +1)的机器接收到相同的投票信息。对于Server1和Server2服务器来说，都统计出集群中已经有两台服务器接收（2，0）这个投票信息。当Server1和Server2都收到相同的投票（2，0）的时候，即认为已经选出了Leader。
##### e.改变服务器状态。
一旦确定了Leader，每个服务器就会更新自己的状态：如果是Follower，那么久变更为FOLLOWING，如果是Leader，那么就变更LEADING为。
#### 2、服务器运行期间的Leader选举
在ZooKeeper集群正常运行过程中，一旦选出一个Leader，那么所有服务器的角色一般都不会再发生变化，Leader服务器将一直作为集群的Leader，即使集群中有非Leader服务器挂了或者是有新的机器加入集群也不会影响原来的Leader。但是一旦Leader所在的机器挂了，那么这个集群将暂时无法对外提供服务，而是进去新一轮的Leader选举。服务器运行期间的Leader选举和启动时期的Leader选举过程基本一致。
##### a.变更状态。
当Leader挂了之后，余下的非Observer服务器都会将自己的服务器状态变更为LOOKING，然后开始进入Leader选举流程。
##### b.每个Server会发出一个投票。
##### c.接收来自各个服务器的投票。
##### d.处理投票。
##### e.统计投票。
##### f.改变服务器状态。
## Leader选举的算法分析
从3.4.0版本开始，ZooKeeper只保留了TCP版本的FatLeaderElection选举算法，本小节主要讲这个算法，首先了解几个概念。
·        SID：服务器ID。SID是一个数字，用来唯一标识一台ZooKeeper集群中的机器，每台机器不能重复，和myid一致。
·        ZXID：事务ID。ZXID是一个事务ID，用来标识一次服务器状态的变更。在某一时刻，集群中的每台机器的ZXID值不一定完全一致，这和ZooKeeper服务器对于客户端“更新请求”的处理逻辑有关。
·        Vote：投票。Leader选举通过投票来实现。当集群中的机器发现自己无法检测到Leader机器的时候，就会开始尝试进行投票。
·        QuoRum：过半机器数。quorum= (n/2 + 1),n为机器总数。
#### 1、进入Leader选举
当ZooKeeper集群中的一台服务器出现以下两种情况之一时，机会开始进入Leader选举：
·        服务器初始化启动。
·        服务器运行期间无法和Leader保持连接。
而当一台机器进入Leader选举流程时，当前集群也可能会处于以下两种状态：
·        集群中本来就已经存在一个Leader。
·        集群中确实不存在Leader。
对于第一种已经存在Leader的情况，机器试图去选举Leader的时候，会被告知当前服务器的Leader信息，对于该机器来说，仅仅需要和Leader机器建立连接，并进行状态同步即可。
下面重点来看集群中不存在Leader的情况，如何进行Leader选举。
#### 2、开始第一次投票
当集群中不存在Leader时，集群中的所有机器都处于一种试图选举出一个Leader的状态，这种状态为LOOKING。当一台服务器处于LOOKING状态的时候，那么它就会向集群中所有其它机器发送消息，我们称这个消息为“投票”。投票包含SID和ZXID，分别表示被推举服务器的唯一标识和事务ID。用（SID，ZXID）表示一个投票信息。
我们假设ZooKeeper由5台服务器组成，SID分别为1，2，3，4和5，ZXID分别为9，9，9，8和8，并且此时SID为2的服务器是Leader。某一时刻，1和2服务器出现故障，因此开始进行Leader选举。
在第一次投票的时候，由于还无法检测到集群中其它服务器的状态信息，因此，每台服务器都将自己作为被推举的对象来进行投票。于是SID为3，4和5的机器，投票情况分别为（3，9），（4，8）和（5，8）。
#### 3、变更投票
集群中的每台机器发出投票后，也会接收到来自集群中其它机器的投票。每台机器都会根据一定的规则，来处理收到的其它机器的投票，并以此来决定是否需要变更自己的投票。这个投票也成为了整个Leader选举算法的核心所在。先定义以下术语。
·        vote_sid：接收到的投票中所推举Leader服务器的SID。
·        vote_sid:  接收到的投票中所推举Leader服务器的ZXID。
·        self_sid:  当前服务器自己的SID。
·        self_zxid:当前服务器自己的ZXID。
每次对于收到的投票的处理，都是对（vote_sid，vote_sid）和（self_sid，self_zxid）对比的过程。
规则1：如果vote_zxid大于self_zxid，就认可当前收到的投票，并再次将该投票发送出去。
规则2：如果vote_zxid小于self_zxid，那么就坚持坚持自己的投票，不做任何变更。
规则3：如果vote_zxid等于self_zxid，那么就比较两者的SID。如果vote_sid大于self_sid，就认可当前收到的投票，并再次将该投票发送出去。
规则4：如果vote_zxid等于self_zxid，那么就比较两者的SID。如果vote_sid小于self_sid，那么就坚持坚持自己的投票，不做任何变更。
对于上面这个规则，我们分析上面提到例子的选举过程，如果所示。
![](https://img-blog.csdn.net/20170320172750281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
#### 4、确定leader
经过第二次投票之后，集群中的每台机器都会再次收到其他机器的投票，然后开始统计投票。如果每台机器收到超过半数的相同的投票，那么这个投票对应的SID机器即为Leader。上面例子中，ZooKeeper集群的总机器数为5。quorum=（5/2
 + 1）= 3。也就是说，只要收到3个或者3个以上（含当前服务器自身在内）一致的投票即可，Server3、Server4和Server5都投票（3，9），因此确定了Server3为Leader。
#### 5、小结
通常哪台服务器上的数据越新，那么越有可能成为Leader，原因很简单，数据越新，那么它的XZID越大，也就越能保证数据的恢复。当然，如果集群中有几个服务器具有相同的最大的XZID，那么SID最大的那台成为Leader。
## Leader选举的实现细节
通过上一小节介绍Leader选举的算法设计，从算法复杂度看，FastLeaderElection算法的设计并不复杂，但在真正实现过程中，对于一个需要应用在生产环境中的产品来说，还是有很多实际问题需要解决，本小节我们主要介绍ZooKeeper中对FastLeaderElection的实现。
#### 1、服务器状态
为了能够清楚地对ZooKeeper集群中每台机器的状态进行标识，在org.apache.zookeeper.server.quorum.ServerState类中列举了4种服务器状态，分别是：LOOKING,FOLLOWING,
 LEADING, OBSERVING。
       LOOKING:       寻找Leader状态。当前服务器处于该状态时，它会认为当前集群中没有Leader，因此需要进入Leader选举流程。
       FOLLOWING:  跟随者状态，表明当前服务器角色是Follower。
       LEADING:       领导者状态，表明当前服务器角色是Leader。
       OBSERVING:  观察者状态，表明当前服务器角色是Observer。
#### 2、 投票数据结构
        投票包含SID和ZXID，分别表示被推举服务器的唯一标识和事务ID。ZooKeeper中对Vote数据结构的定义，如下。
final private long id;
final private long zxid;
final private long electionEpoch;
final private long peerEpoch;
final private ServerState state;
![](https://img-blog.csdn.net/20170320172814814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
