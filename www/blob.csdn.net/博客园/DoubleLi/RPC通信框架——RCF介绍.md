# RPC通信框架——RCF介绍 - DoubleLi - 博客园






现有的软件中用了大量的COM接口，导致无法跨平台，当然由于与Windows结合的太紧密，还有很多无法跨平台的地方。那么为了实现跨平台，支持Linux系统，以及后续的分布式，首要任务是去除COM接口。

在对大量框架进行调研后，决定使用RCF替换COM接口。

# RPC通信框架
- [CORBA](http://corba.org/)
- [ICE](http://www.zeroc.com/)
- [Thrift](http://thrift.apache.org/)
- [zeromq](http://zeromq.org/)
- [dbus](http://www.freedesktop.org/wiki/Software/dbus/)
- [RCF](http://www.deltavsoft.com/)
- [YAMI4](http://www.inspirel.com/yami4/)
- [TAO](http://www.cs.wustl.edu/~schmidt/TAO.html)

# 为什么选择RCF

经过各项对比，认为：
- RCF的使用方式与现有的COM接口方式非常类似，在开发上可以更快速、更容易的替换COM，并且可以少犯错误；
- 从稳定性上来说，RCF从2008年发布的第一个正式版本1.0，到现在的2.01，经历了7个比较重大的版本变化，发展的用户包括”爱立信、惠普“等这样的大公司，在网络上查找RCF相关的介绍，普遍都说不错，是功能比较强大的分布式通信框架，由此可知RCF目前比较稳定，比较成熟；
- RCF基于标准C++，支持Windows、Linux、Unix等操作系统，可以很好的完成跨平台功能；
- RCF支持TCP、UDP、命名管道等通信方式；
- RCF的Server端支持单线程与线程池模式，正好对应COM的两种线程模式；
- 官方文档号称使用了零拷贝、零堆内存分配、Server端缓存等技术，用于提高性能；
- 比较重要的是，虽然官方只有英文文档，但是这份文档非常通俗易懂，即使英文水平比较差，也能在一个晚上或者一个上午的时间内对RCF整体的使用、包含的功能、支持的协议等有比较详细的了解，这对于后续的快速了解和快速开发并替换COM，有着至关重要的作用；

# 简单的性能测试

在我的笔记本 Windows7 专业版 SP1 x64 、Intel(R) Core(TM) i5-2450M CPU @ 2.5GHz、 12G内存 的机器上，编写了Demo，对RCF进行了测试。

一个Server程序，一个Client程序，通过TCP进行通信，Client程序中调用Server服务的接口，这个接口实现是一个空方法，用来测试纯接口调用的极限次数，Release x86版测试结果如下：

调用两万次，耗时1647ms左右，平均每秒可以调用12143次，平均每次调用耗时82微妙。

# 参考资料

[求推荐：进程调度库、进程间通信库，All 开源，谢谢！-C/C++-ChinaUnix.net](http://bbs.chinaunix.net/thread-4140638-1-1.html)

[RCF - Interprocess Communication for C++ – CodeProject](http://www.codeproject.com/Articles/12521/RCF-Interprocess-Communication-for-C)

[分布式通信的几种方式（EJB、RMI、RPC、JMS、web service杂谈）](http://www.tuicool.com/articles/bINz2qA)

[C/C++ 分布式应用/网格开源软件](http://www.oschina.net/project/tag/191/distributed-and-grid?lang=21&os=36&sort=view)

[现在主流开源分布式系统架构都有哪些？](http://www.zhihu.com/question/19832447)

[thrift 入门介绍](http://blog.csdn.net/archfree/article/details/6388951)

[Apache Thrift - 可伸缩的跨语言服务开发框架](http://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/index.html)

[cross platform IPC](http://stackoverflow.com/questions/60649/cross-platform-ipc)

# 总结

经过各项对比后，虽然RCF并不是最强大的框架，可能也不是性能最高的框架，但是在替换COM接口这种需求的情况下，RCF应该是最适合的框架，所以最终选择RCF框架来完成此任务。









