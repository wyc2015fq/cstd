# 唠唠Zookeeper的观察者 - Spark高级玩法 - CSDN博客
2018年07月25日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：204
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/cZkI8M0nbCkXzTeDWepHbhwXDIFJk7sPWCvPUM49IF54sSbFp9Ce4YR69fXp4JLSACibyMfpBiaFSmhZpIXPXuLw/640?wx_fmt=gif)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXibveudOded7LtLKyJ5rEbBQUnecLKSOAwRzeZzxzic5M6dLlQ4nFQXTCqHC4ibCxr8z8FQhJkJPPAA/640?wx_fmt=png)
阅读本文之前，推荐阅读：
[基于zookeeper leader选举方式一](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483890&idx=1&sn=95938c9d25e2273129798acefa08f0c1&chksm=9f38e2daa84f6bcc7212262354735617c1a5f14eb9738e7a658930a261e068b1f97cd2dff27c&scene=21#wechat_redirect)
[Kafka源码系列之源码分析zookeeper在kafka的作用](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483691&idx=1&sn=f8ded96f56e913089fd325563cbb249d&chksm=9f38e203a84f6b15d3c6322ba48441a1084097bc818794533c841768954e5d5fbce89ba974c4&scene=21#wechat_redirect)
**观察者简介**
回顾一下Zookeeper的运行时的角色。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXibveudOded7LtLKyJ5rEbBgCyvhh3ibUg434WgtLCoSrKwechga3Zawh0UOQO40Bx8PC4HjaK0l1Q/640?wx_fmt=png)
观察者的设计是希望能动态扩展zookeeper集群又不会降低写性能。
虽然通过让客户端直接连接到集群的投票成员，ZooKeeper也表现得非常好，但是这种架构使得很难扩展到有大量的客户端情况。问题是，随着我们添加更多投票成员，写入性能也会随着下降。这是因为写操作需要（通常）需要集群中至少一半的节点投票达成一致，因此随着更多投票者的加入，投票的成本会显著增加。
这里引入一种新zookeeper节点类型，叫做观察者，观察者的引入帮助解决了上面的问题同时大大增加了zookeeper的动态扩展能力。观察者不参与投票，只听取投票结果。除了这个简单的区别，Observers的功能与Followers完全相同 - 客户端可以连接到它们并向它们发送读写请求。Observer会像follower一样将消息转发给leader，但是Observer只会听取投票结果，不参与投票。由于这点，我们可以增加任意数量的Observer，同时不会影响我们集群的性能。
Observer还有其它优点。因为他们不投票，所以他们不是ZooKeeper集群的重要组成部分。因此，它们可以失败，或者与集群断开连接，而不会损害ZooKeeper服务的可用性。对用户的好处是Observer相比Follower来说更能通过不太可靠的网络链接进行连接。实际上，Observers可用于与另一个数据中心的ZooKeeper服务器通信。Observers的客户端可以快速读取，因为所有读取都在本地提供，并且写入消耗最小的网络流量，因为在没有投票协议的情况下所需的消息数量较少。
如何使用观察者
在zookeeper集群中使用观察者是非常简单的，仅仅需要修改配置文件里的两个配置即可。
在所有将会配置为zookeeper观察者的节点，添加下面一行：
peerType=observer
这行配置告诉zookeeper这台服务器将会成为一个Observers。
其次，在所有的服务器节点，在server定义处需要在末尾增加:observer。例如：
server.1:localhost:2181:3181:observer
这会告诉其它服务server.1是一个observer，不会参与投票。
运行下面的命令即可链接到集群：
bin/zkCli.sh -server localhost:2181
使用案例
关于observer下面举两个使用案例。实际上，无论您希望扩展ZooKeeper集群的客户端数量，还是希望将集群的关键部分与处理客户端请求的负载隔离开来，Observers都是一个很好的架构选择。
1，作为一个数据中心桥梁(datacenterbridge)：为两个数据中心构建同一套zookeeper集群是会费很大劲的，因为数据中心之间网络延迟的高度变化可能会导致故障检测的误报和集群分区。但是如果集群完全部署到一个数据中心，另一个数据中心用Observers，则分区问题不会出现了。Client依然可以看到和发布提案。
2，作为消息总线的链接：一些公司表示有兴趣将ZK用作持久可靠消息总线的组件。观察者将为这项工作提供一个自然的集成点：插件机制可用于将观察者看到的提案流附加到发布 - 订阅系统，同样不加载核心集群。
