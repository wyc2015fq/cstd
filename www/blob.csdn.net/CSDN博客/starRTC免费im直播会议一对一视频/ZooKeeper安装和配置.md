# ZooKeeper安装和配置 - starRTC免费im直播会议一对一视频 - CSDN博客
2019年04月03日 15:59:40[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：60
ZooKeeper是一个开源的分布式协调服务，由雅虎创建，是Google Chubby的开源实现。ZooKeeper的设计目标是将那些复杂且容易出错的分布式一致性服务封装起来，构成一个高效可靠的原语集，并以一系列简单易用的接口提供给用户使用。
ZooKeeper是一个典型的分布式数据一致性的解决方案。分布式应用程序可以基于它实现诸如数据发布/订阅、负载均衡、命名服务、分布式协调/通知、集群管理、Master选举、分布式锁和分布式队列等功能。
Zookeeper安装方式有三种，单机模式和集群模式以及伪集群模式。
单机模式：Zookeeper只运行在一台服务器上，适合测试环境；
■ 伪集群模式：就是在一台物理机上运行多个Zookeeper 实例；
■ 集群模式：Zookeeper运行于一个集群上，适合生产环境，这个计算机集群被称为一个“集合体”（ensemble）
下载 ZooKeeper，解压，将 conf 目录下的 zoo_sample.cfg 复制一份重命名为 zoo.cfg
cp zoo_sample.cfg zoo.cfg
然后直接启动 ZooKeeper 就行了
bin/zkServer.sh start
查看
bin/zkServer.sh status
停止 ZooKeeper
bin/zkServer.sh stop
[https://mirrors.tuna.tsinghua.edu.cn/apache/zookeeper/stable/zookeeper-3.4.14.tar.gz](https://mirrors.tuna.tsinghua.edu.cn/apache/zookeeper/stable/zookeeper-3.4.14.tar.gz)
[https://juejin.im/post/5abc984c51882555635e66ef](https://juejin.im/post/5abc984c51882555635e66ef)
