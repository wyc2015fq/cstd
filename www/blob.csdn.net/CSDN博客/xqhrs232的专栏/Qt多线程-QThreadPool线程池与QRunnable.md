# Qt多线程-QThreadPool线程池与QRunnable - xqhrs232的专栏 - CSDN博客
2018年05月24日 10:03:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：106
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://www.cnblogs.com/techiel/p/8018379.html](https://www.cnblogs.com/techiel/p/8018379.html)
本文标题：Qt多线程-QThreadPool线程池与QRunnable     本文地址：[http://techieliang.com/2017/12/605/](http://techieliang.com/2017/12/605/)
文章目录- [1. 介绍](http://techieliang.com/2017/12/605/#title-0)
- [2. QThreadPool](http://techieliang.com/2017/12/605/#title-1)
- [2.1. 基本操作函数](http://techieliang.com/2017/12/605/#title-2)
- [2.2. start tryStart tryTake](http://techieliang.com/2017/12/605/#title-3)
- [2.3. 全局线程池](http://techieliang.com/2017/12/605/#title-4)
- [2.4. 局部线程池](http://techieliang.com/2017/12/605/#title-5)
- [3. QRunnable](http://techieliang.com/2017/12/605/#title-6)
- [4. 范例](http://techieliang.com/2017/12/605/#title-7)
- [4.1. 简单使用范例](http://techieliang.com/2017/12/605/#title-8)
- [4.2. 全局线程池和局部线程池对比](http://techieliang.com/2017/12/605/#title-9)
## 1. 介绍
线程的创建及销毁需要与系统交互，会产生很大的开销。若需要频繁的创建线程建议使用线程池，有线程池维护一定数量的线程，当需要进行多线程运算时将运算函数传递给线程池即可。线程池会根据可用线程进行任务安排。
## 2. QThreadPool
相关帮助文档：[QThreadPool](http://doc.qt.io/qt-5/qthreadpool.html)
此类为Qt提供的线程池函数，使用此类只需要配置线程池的最大线程数量、线程长时间不使用的过期时间等参数，不需要进行QThread相关的操作。
此类有两种使用方式：全局线程池和局部线程池。下面首先介绍两种类型后续介绍类提供的方法
### 2.1. 基本操作函数
- intactiveThreadCount()const //当前的活动线程数量
- voidclear()//清除所有当前排队但未开始运行的任务
- intexpiryTimeout()const//线程长时间未使用将会自动退出节约资源，此函数返回等待时间
- intmaxThreadCount()const//线程池可维护的最大线程数量
- voidreleaseThread()//释放被保留的线程
- voidreserveThread()//保留线程，此线程将不会占用最大线程数量，从而可能会引起当前活动线程数量大于最大线程数量的情况
- voidsetExpiryTimeout(int expiryTimeout)//设置线程回收的等待时间
- voidsetMaxThreadCount(int maxThreadCount)//设置最大线程数量
- voidsetStackSize(uint stackSize)//此属性包含线程池工作线程的堆栈大小。
- uint stackSize()const//堆大小
- voidstart(QRunnable *runnable, int priority = 0)//加入一个运算到队列，注意start不一定立刻启动，只是插入到队列，排到了才会开始运行。需要传入QRunnable ，后续介绍
- booltryStart(QRunnable *runnable)//尝试启动一个
- booltryTake(QRunnable *runnable)//删除队列中的一个QRunnable，若当前QRunnable 未启动则返回成功，正在运行则返回失败
- boolwaitForDone(int?<i>msecs</i>?=?-1)//等待所有线程运行结束并退出，参数为等待时间-1表示一直等待到最后一个线程退出
[QThread::idealThreadCount](http://doc.qt.io/qt-5/qthread.html#idealThreadCount)函数，会根据当前设备的硬件情况给出一个线程数量，而maxThreadCount的默认值就是此值。
#### setStackSize
只有在线程池创建新线程时才使用该属性的值。更改它对已经创建或运行的线程没有影响。默认值是0，这使得qthread使用操作系统默认的堆栈大小。
> 
The value of the property is only used when the thread pool creates new threads. Changing it has no effect for already created or running threads.
The default value is 0, which makes [QThread](http://doc.qt.io/qt-5/qthread.html) use the operating system default stack size.
#### maxThreadCount? reserveThread? activeThreadCount
由于reserveThread 后的线程不计入线程数量，因此可能出现activeThreadCount>maxThreadCount? 情况
> 
Note: It is possible for this function to return a value that is greater than [maxThreadCount](http://doc.qt.io/qt-5/qthreadpool.html#maxThreadCount-prop)(). See [reserveThread](http://doc.qt.io/qt-5/qthreadpool.html#reserveThread)() for more details.
### 2.2. start tryStart tryTake
对于start，传入的是QRunnable对象指针，传入后线程池会调用QRunnable的[autoDelete](http://doc.qt.io/qt-5/qrunnable.html#autoDelete)() 函数，若返回true，则当此运算完成后自动释放内容，不需要后续主动判断是否运算完成并释放空间。
对于tryTake，若返回成功，不会自动释放内容，而是需要调用方主动释放，无论autodelete返回值是什么。返回false自然也不会自动delete
> 
Attempts to remove the specified runnable from the queue if it is not yet started. If the runnable had not been started, returns `true`, and ownership of runnable is transferred to the caller (even when `runnable->autoDelete() == true`). Otherwise returns `false`.
Note: If `runnable->autoDelete() == true`, this function may remove the wrong runnable. This is known as the [ABA problem](https://en.wikipedia.org/wiki/ABA_problem): the original runnable may already have executed and has since been deleted. The memory is re-used for another runnable, which then gets removed instead of the intended one. For this reason, we recommend calling this function only for runnables that are not auto-deleting.
对于tryStart，若返回成功，等同于start，若false，则不会自动delete
注意，对于autoDelete必须在调用state/trytake之前进行修改，不要再调用以后修改，否则结果不可预测
> 
Note that changing the auto-deletion on runnable after calling this function results in undefined behavior.
QRunnable的autoDelete默认返回true，若需要更改需要调用setAutoDelete进行更改
### 2.3. 全局线程池
QThreadPool提供了一个静态函数，[globalInstance](http://doc.qt.io/qt-5/qthreadpool.html#globalInstance)()，使用此方法可获取一个当前进程的全局线程池，可在多个类中共同使用一个线程池。
### 2.4. 局部线程池
和常规类的使用相同，可以通过? QThreadPool pool；的方式建立一个局部线程池，并由当前类维护，可保证此线程池仅供当前类应用
## 3. QRunnable
线程池每一个需要运行的任务均需要作为QRunnable的子类，并重写其run函数，帮助文档：[http://doc.qt.io/qt-5/qrunnable.html](http://doc.qt.io/qt-5/qrunnable.html)
QRunnable只有run、autodelete、setautodelete这三个关键函数。
run内重写需要运算的内容。
autodelete用来标识是否在运行结束后自动由线程池释放空间，具体说明见上述“QThreadPool-基本操作函数-start tryStart tryTake”
## 4. 范例
### 4.1. 简单使用范例
- #include <QCoreApplication>
- #include <QThreadPool>
- #include <QThread>
- #include <QRunnable>
- #include <QDebug>
- class MyRun : public QRunnable {
- public:
- voidrun(){
- int i=3;
- while(i){
- i--;
- qDebug()<<"thread start:"<<QThread::currentThreadId();
- QThread::msleep(500);
- }
- }
- };
- intmain(int argc, char *argv[]){
- QCoreApplication a(argc, argv);
- qDebug()<<"Main:"<<QThread::currentThreadId();
- QThreadPool m;
- MyRun *run=new MyRun;
- if(!run->autoDelete()){
- qDebug()<<"QRunnable's autoDelete default value is not true";
- run->setAutoDelete(true);
- }
- qDebug()<<m.maxThreadCount()<<m.expiryTimeout();
- qDebug()<<m.activeThreadCount();
- m.start(run);
- qDebug()<<m.activeThreadCount();
- m.waitForDone();
- qDebug()<<m.activeThreadCount();
- return0;
- }
结果：
- Main: 0xffc
- 430000
- 0
- 1
- thread start: 0x7e4
- thread start: 0x7e4
- thread start: 0x7e4
- 0
### 4.2. 全局线程池和局部线程池对比
- #include <QCoreApplication>
- #include <QThreadPool>
- #include <QThread>
- #include <QRunnable>
- #include <QDebug>
- class MyRun : public QRunnable {
- public:
- voidrun(){
- int i=3;
- while(i){
- i--;
- qDebug()<<"thread start:"<<QThread::currentThreadId();
- QThread::msleep(500);
- }
- }
- };
- intmain(int argc, char *argv[]){
- QCoreApplication a(argc, argv);
- qDebug()<<"Main:"<<QThread::currentThreadId();
- QThreadPool pool;
- QThreadPool *global_pool = QThreadPool::globalInstance();
- MyRun *run=new MyRun;
- if(!run->autoDelete()){
- qDebug()<<"QRunnable's autoDelete default value is not true";
- run->setAutoDelete(true);
- }
- pool.setMaxThreadCount(2);//修改了局部线程数量
- qDebug()<<"pool:"<<pool.maxThreadCount()<<pool.expiryTimeout()<<"\r\nglobal"<<global_pool->maxThreadCount();
- qDebug()<<pool.activeThreadCount()<<global_pool->activeThreadCount();
- pool.start(run);
- global_pool->start(new MyRun);
- qDebug()<<pool.activeThreadCount()<<global_pool->activeThreadCount();
- pool.waitForDone();
- global_pool->waitForDone();
- qDebug()<<pool.activeThreadCount()<<global_pool->activeThreadCount();
- return0;
- }
结果
- Main: 0x30c4
- pool: 230000
- global 4
- 00
- 11
- thread start: 0x22d0
- thread start: 0xfe0
- thread start: 0x22d0
- thread start: 0xfe0
- thread start: 0x22d0
- thread start: 0xfe0
- 00
当建立局部线程池，修改其参数后仅供局部使用，不会影响全局线程池的。
