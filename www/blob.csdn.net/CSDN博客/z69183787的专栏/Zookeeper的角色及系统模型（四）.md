# Zookeeper的角色及系统模型（四） - z69183787的专栏 - CSDN博客
2018年02月24日 18:21:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：114
个人分类：[分布式-Zookeeper](https://blog.csdn.net/z69183787/article/category/5568407)
[http://blog.csdn.net/wo541075754/article/details/60316543](http://blog.csdn.net/wo541075754/article/details/60316543)
## 角色
zookeeper主要有以下角色：
|角色|说明|
|----|----|
|Leader(领导者)|为客户端提供读和写的服务，负责投票的发起和决议，更新系统状态。|
|Follower（跟随者）|为客户端提供读服务，如果是写服务则转发给Leader。在选举过程中参与投票。|
|Observe（观察者）|为客户端提供读服务器，如果是写服务则转发给Leader。不参与选举过程中的投票，也不参与“过半写成功”策略。在不影响写性能的情况下提升集群的读性能。此角色于zookeeper3.3系列新增的角色。|
|client（客户端）|连接zookeeper服务器的使用着，请求的发起者。独立于zookeeper服务器集群之外的角色。|
集群模型
上面介绍了zookeeper集群中的各个角色，下面通过一张图系统架构图了解一下各个角色所处的位置。 
![这里写图片描述](https://img-blog.csdn.net/20170304084030868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd281NDEwNzU3NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
