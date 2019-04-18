# boost::mutex::scoped_lock - wishchinYang的专栏 - CSDN博客
2016年09月14日 13:28:00[wishchin](https://me.csdn.net/wishchin)阅读数：9208
        在三维重建过程中，世界地图 Map &world作为唯一 访问/更新 对象，可以使用boost::mutex::scoped_lock 。
一：boost::mutex::scoped_lock
        使用boost进行线程管理简单使用boost::mutex 。
        mutex对象本身并不知道它代表什么，它仅仅是被多个消费者线程使用的资源访问的锁定解锁标志。在某个时刻，只有一个线程可以锁定这个mutex对象，这就阻止了同一时刻有多个线程并发访问共享资源。一个mutex就是一个简单的信号机制。
        给mutex加解锁有多种策略，最简单的是使用scoped_lock类，它使用一个mutex参数来构造，并一直锁定这个mutex直到对象被销毁。如果这个正在被构造的mutex已经被别的线程锁定的话，当前线程就会进入wait状态，直到这个锁被解开。
稍微详细的一点介绍参考：[Boost锁~临界区保护和临界资源共享](http://blog.csdn.net/wishchin/article/details/12884387)
        mutex锁不分读写，作为智能锁是需要一定的使用代价的。
一种具体实现参考：[mutex、condition、scope_lock的简单实现](http://www.oschina.net/code/snippet_54334_1856)
二、读写锁的使用：
       参考：[Boost线程库学习笔记](http://www.cnblogs.com/younes/archive/2010/06/06/1752745.html)
       文章给出了**详细的**读写锁
```cpp
boost::read_write_mutex
```
的使用方法，省去了自己再写解死锁方法的劳动。并且对一些经典锁死现象进行了代码级分析和解决方案提出，课件作者付出了大量的劳动。

