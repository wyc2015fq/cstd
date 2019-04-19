# dubbo + zookeeper了解初步 - 左直拳的马桶_日用桶 - CSDN博客
2018年11月21日 18:33:35[左直拳](https://me.csdn.net/leftfist)阅读数：155
个人分类：[系统架构																[学习笔记](https://blog.csdn.net/leftfist/article/category/1790155)](https://blog.csdn.net/leftfist/article/category/2567173)
dubbo是一个分布式服务框架，而zookeeper则是其中的注册中心。dubbo的注册中心可以使用多种框架来充当，zookeeper只是其中之一。
**一、dubbo**
dubbo的作用一言以蔽之，就是服务治理。即服务太多了，该使用哪些，启动顺序如何，有些宕掉了怎么办。。。，十分复杂，不好处理。dubbo框架就是用来管理这些服务的。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181121172946139.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
dubbo提供了几个核心部件：
```
Provider: 暴露服务的服务提供方。
Consumer: 调用远程服务的服务消费方。
Registry: 服务注册与发现的注册中心。
Monitor: 统计服务的调用次调和调用时间的监控中心。
Container: 服务运行容器。
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181121173227248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181121173248195.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
调用关系
```
0：服务容器负责启动，加载，运行服务提供者   
1：服务提供者在启动时，向注册中心自己提供的服务  
2：服务消费者在启动时，向注册中心订阅自己所需的服务  
3：注册中心返回服务提供者地址给消费者，如果有变更，注册中心将基于长连接推送给服务消费者  
4：服务消费者从提供者地址列表中，基于软负载均衡算法，选一台提供者进行调用，如果调用失败，再选一台  
5：服务消费者和提供者，在内存中累计调用次数和调用时间，定时每分钟发送一次数据到监控中心
```
大致是，服务要先注册；调用方（消费者）调用服务前先从注册中心拿到一份名单，看有哪些服务可以用（消息订阅的方式）；如果中途服务加入或不可用，注册中心还会通知消费者。消费者调用服务的时候，会考虑负载均衡，用算法算一下应该用哪一个。最后还有一个监控中心，消费者和提供者都要定期通知它。
**二、zookeeper**
dubbo的注册中心可以使用多种框架来充当，zookeeper只是其中之一。
```
Multicast注册中心
Zookeeper注册中心
Redis注册中心
Simple注册中心
```
既然如此，为啥dubbo + zookeeper称为标配？除了zookeeper确实不错之外，还因为dubbo是阿里巴巴开源的框架，纯国货，为了剥离内部系统，所以dubbo将注册中心抽离，打造成支持相关的开源框架。而zookeeper当时已经是工业级的成熟稳定的产品。后来在国内，在业界的努力实践下，Dubbo + ZooKeeper 的典型的服务化方案成就了 ZooKeeper 作为注册中心的声名。
zookeeper流程如下：
（1）服务提供者启动时候向注册中心写下自己的URL地址。
（2）服务消费者启动时，向注册中心订阅服务清单
（3）Monitor启动时候，向注册中心订阅所有的服务提供者和消费者URL地址
同时具有以下的特性：
（1）当提供者出现断电等异常停机时，注册中心能自动删除提供者信息。
（2）当注册中心重启时，能自动恢复注册数据，以及订阅请求。
（3）当会话过期时，能自动恢复注册数据，以及订阅请求。
注册中心的发展历程：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181121183225711.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181121183238208.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181121183250469.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
三、总结
dubbo负责的工作处于业务层与数据库持久层之间，算是一个中间件。
