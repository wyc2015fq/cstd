# QObject::killTimer: timers cannot be stopped from another thread - xqhrs232的专栏 - CSDN博客
2018年06月07日 22:11:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1061
原文地址::[https://blog.csdn.net/qq_30043465/article/details/54606287](https://blog.csdn.net/qq_30043465/article/details/54606287)
QObject::killTimer: timers cannot be stopped from another thread
出现这个错误的原因在于在次线程中执行主线程对象的一些操作引起的 
可以这样修改 
如果次线程需要更新主线程对象状态，需要发送消息，主线程对象接收后处理而不能在此线程中直接操作
QT定时器非常好，只要注意跨线程问题
使用定时器的注意事项： 
1. QT定时器线程相关的使用注意事项: 
1> 不能夸线程启动定时器和停止定时器 
1> 不能夸线程启动一个定时器关联的对象,但在另一个线程释放(析构)此和定时器关联的对象(相当于1>的情况不能在其他线程停止定时器).
qWarning(“QObject::killTimers:timers cannot be stopped from another thread”);
也就是定时器相关的逻辑和对象只能用在一个线程中. 
因为QT的定时器有内部的数据结构来关联定时器相关逻辑.
否则:启动定时器的线程不能删除定时器,而定时器关联的对象已经不存在,那么下次定时器事件来时会崩溃.
详细原理可见 
[QThread 与 QObject的关系](http://blog.csdn.net/hai200501019/article/details/9748173?utm_source=tuicool&utm_medium=referral)
