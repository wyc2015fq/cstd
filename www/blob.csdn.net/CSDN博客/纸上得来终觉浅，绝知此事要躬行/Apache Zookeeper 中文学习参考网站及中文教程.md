# Apache Zookeeper 中文学习参考网站及中文教程 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年04月07日 18:19:23[boonya](https://me.csdn.net/boonya)阅读数：5061










Zookeeper是一个高性能的分布式系统的协调服务。它在一个简单的接口里暴露公共服务：像命名、配置管理、同步、和群组服务，所以你没有必要从头开始实现它们。你可以使用现成的Zookeeper去实现共识、群组管理、领导人选举和业务协议。并且你可以在它的基础之上建立自己特定的需求。

Zookeeper集群结构



![Image.png](http://www.tianshouzhi.com/ueditor/upload/image/20160412/1460473199471032965.png)

Server实例共同组成了Zookeeper服务。Client连接到其中一个Server实例。客户端维护一个TCP连接，通过其进行发送请求，获得相应，获得监控事件，进行心跳检测。如果心跳检测失败，Client将会连接到另一个Server实例。

     Zookeeper数据模型和等级命名空间

![Image.png](http://www.tianshouzhi.com/ueditor/upload/image/20160412/1460473237896035697.png)




与标准文件系统不同的是，Zookeeper命名空间中的每一个节点都可以维护数据，并且含有子节点。





# 综述

针对于客户端开发人员、管理员和贡献者的技术综述文档。

- [Zookeeper简介](http://zookeeper.majunwei.com/document/3.4.6/OverView.html) - Zookeeper的鸟瞰图，包括Zookeeper的设计思想和技术架构。
- [Zookeeper入门指南](http://zookeeper.majunwei.com/document/3.4.6/GettingStarted.html) - 针对于开发者Zookeeper安装、运行、和编程的辅导指南。
- [发行说明](http://zookeeper.majunwei.com/document/3.4.6/ReleaseNotes.html) - 新开发者和用户面对的特性、改善、和不兼容性。


# 开发人员

针对于开发人员使用Zookeeper客户端API的文档。
- [Zookeeper API](http://zookeeper.apache.org/doc/r3.4.6/api/index.html) - Zookeeper客户端API的技术参考说明书。
- [Zookeeper开发者编程指南](http://zookeeper.majunwei.com/document/3.4.6/DeveloperProgrammerGuide.html) - 客户端应用程序开发人员的指导说明。
- [Zookeeper JAVA实例](http://zookeeper.majunwei.com/document/3.4.6/DeveloperJavaExample.html) - 用java写的简单的Zookeeper客户端应用程序。
- [Zookeeper阻塞和队列教程](http://zookeeper.majunwei.com/document/3.4.6/DeveloperBarrierAndQueue.html) - 阻塞和队列的实例实现。
- [Zookeeper秘诀](http://zookeeper.majunwei.com/document/3.4.6/DeveloperRecipes.html) - 在分布式应用程序中对于常见问题的更高级的解决方案。

# 管理员和运维人员

针对于管理员和运维人员部署Zookeeper的文档。
- [Zookeeper管理员指南](http://zookeeper.majunwei.com/document/3.4.6/AdminGuide.html) - 针对于管理员和其他部署Zookeeper的用户指南。
- [Zookeeper配置指南](http://zookeeper.majunwei.com/document/3.4.6/AdminQuotaGuide.html) - 针对于系统管理员的Zookeeper中配置的用户指南。
- [JMX](http://zookeeper.majunwei.com/document/3.4.6/AdminJMX.html) - 在Zookeeper中如何启用JMX。
- [分层次的法定人数](http://zookeeper.majunwei.com/document/3.4.6/zookeeperHierarchicalQuorums.html)
- [观察者](http://zookeeper.majunwei.com/document/3.4.6/AdminObserversGuide.html) - 无投票权的成员更容易增加系统的扩展性。

# 贡献者

针对于开发者贡献Zookeeper开源项目的文档。

- [Zookeeper内部构件](http://zookeeper.majunwei.com/document/3.4.6/ContributorInternals.html) - Zookeeper在内部运作的各式各样的主题。

# 其他文档
- [Wiki](https://cwiki.apache.org/confluence/display/ZOOKEEPER)
- [FAQ](https://cwiki.apache.org/confluence/display/ZOOKEEPER/FAQ)


# BookKeeper文档

BookKeeper是一个实现了高性能的预写日志的高可用系统。它用Zookeeper管理元数据，这个是它成为Zookeeper扩展包的主要原因。

- [BookKeeper是什么?](http://zookeeper.majunwei.com/document/3.4.6/BookKeeperOverview.html)
- [现在我怎么试用它？](http://zookeeper.majunwei.com/document/3.4.6/BookKeeperGettingStarted.html)
- [很棒，但是我怎么和我的应用集成？](http://zookeeper.majunwei.com/document/3.4.6/BookKeeperProgrammerGuide.html)
- [入口可以用流字节吗？](http://zookeeper.majunwei.com/document/3.4.6/BookKeeperSetupGuide.html)






以上文字转载自[Zookeeper中文网](http://zookeeper.majunwei.com/) - [Zookeeper文档目录](http://zookeeper.majunwei.com/document/3.4.6/Welcome.html)
开始阅读正文：[http://zookeeper.majunwei.com/document/3.4.8/OverView.html](http://zookeeper.majunwei.com/document/3.4.8/OverView.html)



另外田守枝的技术学习教程（包含Zookeeper但不限于Zookeeper）：[http://www.tianshouzhi.com/](http://www.tianshouzhi.com/)


# Apache Zookeeper中文教程

请参考：[https://github.com/SunflowersOfJava/dubbo-zookeeper-spring](https://github.com/SunflowersOfJava/dubbo-zookeeper-spring)





