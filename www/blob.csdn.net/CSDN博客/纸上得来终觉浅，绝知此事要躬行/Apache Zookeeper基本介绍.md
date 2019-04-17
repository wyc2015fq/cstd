# Apache Zookeeper基本介绍 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月27日 16:29:29[boonya](https://me.csdn.net/boonya)阅读数：791标签：[apache																[zookeeper](https://so.csdn.net/so/search/s.do?q=zookeeper&t=blog)](https://so.csdn.net/so/search/s.do?q=apache&t=blog)
个人分类：[Zookeeper](https://blog.csdn.net/boonya/article/category/6821765)








ZooKeeper是分布式应用程序的高性能协调服务。它是Apache Hadoop项目的一个子项目，主要用来解决分布式应用场景中存在的一些问题，如：统一命名服务、状态同步服务、集群管理、分布式应用配置管理等，它支持Standalone模式和分布式模式，在分布式模式下，能够为分布式应用提供高性能和可靠地协调服务，而且使用ZooKeeper可以大大简化分布式协调服务的实现，为开发分布式应用极大地降低了成本。



Apache Zookeeper官网地址：[http://zookeeper.apache.org/](http://zookeeper.apache.org/)

Apache Zookeeper Github地址：[https://github.com/apache/zookeeper](https://github.com/apache/zookeeper)



# 欢迎使用Apache ZooKeeper

Apache ZooKeeper是开发和维护开源服务器的一个努力，可以实现高度可靠的分布式协调。




# 什么是ZooKeeper？

ZooKeeper是用于维护配置信息，命名，提供分布式同步和提供组服务的集中式服务。所有这些类型的服务以分布式应用程序以某种形式或另一种形式使用。每次执行它们时，都会有很多工作用于修复不可避免的错误和竞争条件。由于实施这些服务的困难，应用程序最初通常会吝啬，这使得它们在变化存在时变脆，难以管理。即使正确完成，这些服务的不同实现也会导致应用程序部署时的管理复杂性。

在[ZooKeeper Wiki](https://cwiki.apache.org/confluence/display/ZOOKEEPER/Index)上了解更多关于ZooKeeper的信息。




# 入门

首先将ZooKeeper安装在单个机器或非常小的集群上。
- 通过阅读文档[了解](http://zookeeper.apache.org/doc/trunk)ZooKeeper。
- 从发行页面[下载](http://zookeeper.apache.org/releases.html)ZooKeeper。

# 深入

Apache ZooKeeper是Apache Software Foundation下的一个开源志愿者项目。我们鼓励您了解该项目并贡献您的专业知识。这里有一些起始链接：
- 请参阅我们的[如何贡献ZooKeeper](https://cwiki.apache.org/confluence/display/ZOOKEEPER/HowToContribute)页面。
- 给我们[反馈](https://issues.apache.org/jira/browse/ZOOKEEPER)：我们能做些什么更好？
- 加入[邮件列表](http://zookeeper.apache.org/lists.html)：认识社区。




# 场景

![](https://img-blog.csdn.net/20170327163454797)




# 特点



Zookeeper可以保证如下的分布式特性：
- 顺序一致性
- 原子性
- 单一视图
- 可靠性
- 实时性

# 设计目标

 ZooKeeper是基于如下4个目标来进行权衡和设计的，我们从设计及其特性的角度来详细说明：


- **简单**




 分布式应用中的各个进程可以通过ZooKeeper的命名空间（Namespace）来进行协调，这个命名空间是共享的、具有层次结构的，更重要的是它的结构足够简单，像我们平时接触到的文件系统的目录结构一样容易理解，如图所示：

![](https://img-blog.csdn.net/20170327171112426)

 在ZooKeeper中每个命名空间（Namespace）被称为ZNode，你可以这样理解，每个ZNode包含一个路径和与之相关的元数据，以及继承自该节点的孩子列表。与传统文件系统不同的是，ZooKeeper中的数据保存在内存中，实现了分布式同步服务的高吞吐和低延迟。在上图示例的ZooKeeper的数据模型中，有如下要点：



- 每个节点（ZNode）中存储的是同步相关的数据（这是ZooKeeper设计的初衷，数据量很小，大概B到KB量级），例如状态信息、配置内容、位置信息等。
- 一个ZNode维护了一个状态结构，该结构包括：版本号、ACL变更、时间戳。每次ZNode数据发生变化，版本号都会递增，这样客户端的读请求可以基于版本号来检索状态相关数据。
- 每个ZNode都有一个ACL，用来限制是否可以访问该ZNode。
- 在一个命名空间中，对ZNode上存储的数据执行读和写请求操作都是原子的。
- 客户端可以在一个ZNode上设置一个监视器（Watch），如果该ZNode数据发生变更，ZooKeeper会通知客户端，从而触发监视器中实现的逻辑的执行。
- 每个客户端与ZooKeeper连接，便建立了一次会话（Session），会话过程中，可能发生CONNECTING、CONNECTED和CLOSED三种状态。
- ZooKeeper支持临时节点（Ephemeral Nodes）的概念，它是与ZooKeeper中的会话（Session）相关的，如果连接断开，则该节点被删除。
- **冗余**
ZooKeeper被设计为复制集群架构，每个节点的数据都可以在集群中复制传播，使集群中的每个节点数据同步一致，从而达到服务的可靠性和可用性。前面说到，ZooKeeper将数据放在内存中来提高性能，为了避免发生单点故障（SPOF），支持数据的复制来达到冗余存储，这是必不可少的。

- **有序**
ZooKeeper使用时间戳来记录导致状态变更的事务性操作，也就是说，一组事务通过时间戳来保证有序性。基于这一特性。ZooKeeper可以实现更加高级的抽象操作，如同步等。

- **快速**
ZooKeeper包括读写两种操作，基于ZooKeeper的分布式应用，如果是读多写少的应用场景（读写比例大约是10:1），那么读性能更能够体现出高效。








注：如后续有时间更新会根据官方文档作深入研究。


# 参考文章

Apache zookeeper:[http://zookeeper.apache.org/](http://zookeeper.apache.org/)




zookeeper架构设计及其要点：[http://shiyanjun.cn/archives/474.html](http://shiyanjun.cn/archives/474.html)





Zookeeper简介(一)：[http://blog.csdn.net/wo541075754/article/details/56335059](http://blog.csdn.net/wo541075754/article/details/56335059)











