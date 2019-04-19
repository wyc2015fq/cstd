# java的若干问题（3）——多线程 - 刘炫320的博客 - CSDN博客
2016年05月24日 10:12:20[刘炫320](https://me.csdn.net/qq_35082030)阅读数：298标签：[java																[多线程																[线程](https://so.csdn.net/so/search/s.do?q=线程&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java技术](https://blog.csdn.net/qq_35082030/article/category/6240462)
今天我们讲一讲关于java多线程的一些事情。
1、线程的基本
Thread类，核心，只有他才能进行线程的操作，runnable的接口虽然也能实现run方法，但是不能进行线程操作。thread类其实是runnable的子类，这是前面讲的代理者模式，把对线程操作的都放到了tread类里。
2、对线程的操作
Thread 线程类
Thread.start()运行线程
Thread.run()运行run方法一次
Thread.sleep() 线程睡眠
Thread.interrupt()线程中断
Thread.join() 线程强制执行
Thread.setDaemon()线程变为后台
Thread.isAlive()判断线程是否启动
3、对于线程的一点注意
线程是有优先级的，但并非高的优先级一定会获得CPU。
通常java会启动2个线程，一个主线程，一个垃圾回收线程
java的主线程是中等级别的。
4、线程的同步
关键字 synchronized ,用在public synchronize void set()这里，也可以用在代码块中。
5、wait()与notify()
与线程同步常用的是wait()等待和notify()唤醒，他们俩还得需要一个标志位来协作。
6、不常用的
suspend()、resume()、stop()这三个不常用，是因为会造成死锁现象。因此，终止一个线程常用的操作是设置标志位。
这些虽然不多，但是还是可以对于开发来讲有一些帮助。
