# Zookeeper开源客户端ZKClient和Curator简介 - z69183787的专栏 - CSDN博客
2018年02月25日 11:18:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：189
[http://blog.csdn.net/wo541075754/article/details/68067872](http://blog.csdn.net/wo541075754/article/details/68067872)
Zookeeper客户端提供了基本的操作，比如，创建会话、创建节点、读取节点、更新数据、删除节点和检查节点是否存在等。但对于开发人员来说，Zookeeper提供的基本操纵还是有一些不足之处。本篇博客就聊聊这些不足之处和两款开源框架ZKClient和Curator。
## Zookeeper API不足之处
（1）Zookeeper的Watcher是一次性的，每次触发之后都需要重新进行注册； 
（2）Session超时之后没有实现重连机制； 
（3）异常处理繁琐，Zookeeper提供了很多异常，对于开发人员来说可能根本不知道该如何处理这些异常信息； 
（4）只提供了简单的byte[]数组的接口，没有提供针对对象级别的序列化； 
（5）创建节点时如果节点存在抛出异常，需要自行检查节点是否存在； 
（6）删除节点无法实现级联删除；
## ZkClient简介
ZkClient是一个开源客户端，在Zookeeper原生API接口的基础上进行了包装，更便于开发人员使用。内部实现了Session超时重连，Watcher反复注册等功能。像dubbo等框架对其也进行了集成使用。
虽然ZkClient对原生API进行了封装，但也有它自身的不足之处：
- 几乎没有参考文档；
- 异常处理简化（抛出RuntimeException）；
- 重试机制比较难用；
- 没有提供各种使用场景的实现；
## Curator简介
Curator是Netflix公司开源的一套Zookeeper客户端框架，和ZkClient一样，解决了非常底层的细节开发工作，包括连接重连、反复注册Watcher和NodeExistsException异常等。目前已经成为Apache的顶级项目。另外还提供了一套易用性和可读性更强的Fluent风格的客户端API框架。
除此之外，Curator中还提供了Zookeeper各种应用场景（Recipe，如共享锁服务、Master选举机制和分布式计算器等）的抽象封装。
## 小结
上面介绍了Zookeeper原生API的不足和两款开源客户端的简介。后面的章节将会针对两款开源客户端进行详细的介绍。
