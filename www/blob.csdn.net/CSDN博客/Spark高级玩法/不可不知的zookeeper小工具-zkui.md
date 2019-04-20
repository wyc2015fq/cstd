# 不可不知的zookeeper小工具-zkui - Spark高级玩法 - CSDN博客
2018年10月10日 00:02:45[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：102
本文主要是首先带着大家回顾一下zookeeper在大数据中的作用，然后给大家介绍一款zk的监控管理工具。
**zookeeper在分布式集群的作用**
**1，数据发布与订阅（配置中心）**
发布与订阅模型，即所谓的配置中心，顾名思义就是讲发布者将数据发布到zk节点上，共订阅者动态获取数据，实现配置的集中式管理和动态更新。例如，全局的配置信息，服务服务框架的地址列表就非常适合使用。
**2，负载均衡**
即软件负载均衡。最典型的是消息中间件的生产、消费者负载均衡。
**3，命名服务(Naming Service)**
常见的是发布者将自己的地址列表写到zookeeper的节点上，然后订阅者可以从固定名称的节点获取地址列表，链接到发布者进行相关通讯。
**4，分布式通知/协调**
这个利用的是zookeeper的watcher注册和异步通知机制，能够很好的实现分布式环境中不同系统间的通知与协调，实现对数据变更的实时处理。
**5，集群管理与Master选举**
集群管理，比如在线率，节点上线下线通知这些。Master选举可以使用临时顺序节点来实现。
**6，分布式锁**
分布式锁，这个主要得益于zookeeper数据的强一致性，利用的是临时节点。锁服务分为两类，一个是独占锁，另一个是控制时序。
独占，是指所有的客户端都来获取这把锁，最终只能有一个获取到。用的是临时节点。
控制时序，所有来获取锁的客户端，都会被安排得到锁，只不过要有个顺序。实际上是某个节点下的临时顺序子节点来实现的。
**zkui详解**
**简介**
zkui是zookeeper的一个web管理界面，可以对zookeeper进行CRUD操作。
github地址：
https://github.com/DeemOpen/zkui
**环境要求**
编译环境官方要求是jdk7，但是浪尖这里jdk8正常运行。
浪尖使用的maven版本是mvn-3.3.3
**安装部署**
**1. 编译打包**
mvn clean install
在执行结束之后，会在target目录下生成一个jar包
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUgF5slhBLZVvBmnTfQPcP1eHg4BHd5wCOgeekLicnIkh74Sc0j1KFFpiaWopicb2lgpgt1AvYjQ0ibSA/640)
**2. 配置**
执行 vim config.cfg
添加如下配置
# zkui web页面访问端口
serverPort=9090
# zookeeper集群的IP地址和端口
zkServer=localhost:2181
# 设置登录zkui的用户名和密码，这里是默认值
userSet = {"users": [{ "username":"admin" , "password":"manager","role": "ADMIN" },{ "username":"appconfig" , "password":"appconfig","role": "USER" }]}
**3. 启动**
启动zkui也比较简单，可以直接执行
$ java -jar zkui-2.0-SNAPSHOT-jar-with-dependencies.jar
也可以使用nohup &是其变为后台进程
nohup java -jar target/zkui-2.0-SNAPSHOT-jar-with-dependencies.jar &
**4. 登陆ui**
访问http://localhost:9090即可看到以下界面：输入刚刚设置的用户名密码：admin/manager即可登录。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUgF5slhBLZVvBmnTfQPcP1T4lJf1yILzL4KB64fG00Zx0sFz4IALAHzNpxKvD0bUbnT3paPGhZZg/640)
**5. CRUD操作**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUgF5slhBLZVvBmnTfQPcP1CPLeb1el6xlOPziazPfRk7b5hX5UN9sbAtBPIsLhFEwAMSD2TnpChSQ/640)
推荐阅读：
[唠唠Zookeeper的观察者](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485111&idx=1&sn=baa52a32cecf3645abaf4dde8d1090a1&chksm=9f38e59fa84f6c89c9618d27ae3a1f933b7ba2287d2366a26bb2fd48f0bbc6c74d2f6704e1a4&scene=21#wechat_redirect)
[基于zookeeper leader选举方式一](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483890&idx=1&sn=95938c9d25e2273129798acefa08f0c1&chksm=9f38e2daa84f6bcc7212262354735617c1a5f14eb9738e7a658930a261e068b1f97cd2dff27c&scene=21#wechat_redirect)
[分布式服务框架 Zookeeper -- 管理分布式环境中的数据](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485564&idx=2&sn=661c804f50d246c717758301c972c853&chksm=9f38eb54a84f6242371f9c218d1b79ce4bfdea4d026e286bae504de985d457d4db62273d4f51&scene=21#wechat_redirect)
[Kafka源码系列之源码分析zookeeper在kafka的作用](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484900&idx=1&sn=95577024ba531e6743d06e8c550662a3&chksm=9f38e6cca84f6fda5a37dd6a4f8d858a108a5bbfb802eeb4ed24f1a152a5f60b600e0fca125d&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
