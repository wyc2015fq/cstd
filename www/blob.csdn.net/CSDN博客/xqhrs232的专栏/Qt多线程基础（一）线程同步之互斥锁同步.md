# Qt多线程基础（一）线程同步之互斥锁同步 - xqhrs232的专栏 - CSDN博客
2018年06月08日 22:32:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：452
原文地址::[https://blog.csdn.net/hechao3225/article/details/53033993](https://blog.csdn.net/hechao3225/article/details/53033993)
一、直接使用QMutex进行同步
创建线程方法：继承自QThread,重写void run()函数，调用成员start()启动线程，start()中可加入优先级参数。
互斥锁同步方法：void run()函数中使用QMutex来实现同步，当多个线程访问共享变量时，应使用lock/trylock和unlock将对共享变量的操作代码包裹，以保证同步访问共享变量。（C++中引起线程安全的共享资源只有两种：全局变量和静态变量）
示例代码中两个Thread均继承自QThread()，为了保证互斥锁对两个线程均可见，QMutex在一个线程CPP文件中定义，另一个线程文件做extern声明。
示例代码如下：
thread.h
[cpp][view plain](https://blog.csdn.net/hechao3225/article/details/53033993#)[copy](https://blog.csdn.net/hechao3225/article/details/53033993#)
- #ifndef MYTHREAD_H
- #define MYTHREAD_H
- #include <QtCore>
- #include <QMutex>
- 
- class MyThread:public QThread  
- {  
- public:  
-     MyThread(QString name);  
- void run();  
- private:  
-     QString mName;  
- 
- };  
- 
- #endif // MYTHREAD_H
thread.cpp
[cpp][view plain](https://blog.csdn.net/hechao3225/article/details/53033993#)[copy](https://blog.csdn.net/hechao3225/article/details/53033993#)
- #include "mythread.h"
- #include <QDebug>
- 
- int i=50;  
- QMutex mutex;  
- MyThread::MyThread(QString name):QThread(),mName(name)  
- {  
-     qDebug()<<"creating.."<<endl;  
- }  
- 
- void MyThread::run()  
- {  
-     qDebug()<<this->mName<<"running.."<<endl;  
-     mutex.lock();  
- /*
-     for(;i<100;i++)
-     {
-         qDebug()<<this->mName<<i<<endl;
- 
-     }
-     */
-     i++;  
-     i*=2;  
-     qDebug()<<this->mName<<i<<endl;  
-     mutex.unlock();  
- 
-     qDebug()<<this->mName<<"stop running.."<<endl;  
-     sleep(1);  
- 
- }  
thread2.h
[cpp][view plain](https://blog.csdn.net/hechao3225/article/details/53033993#)[copy](https://blog.csdn.net/hechao3225/article/details/53033993#)
- #ifndef MYTHREAD2_H
- #define MYTHREAD2_H
- #include <QThread>
- #include <QMutex>
- class MyThread2:public QThread  
- {  
- public:  
-     MyThread2(QString name);  
- void run();  
- private:  
-     QString mName;  
- 
- };  
- 
- #endif // MYTHREAD2_H
thread2.cpp
[cpp][view plain](https://blog.csdn.net/hechao3225/article/details/53033993#)[copy](https://blog.csdn.net/hechao3225/article/details/53033993#)
- #include "mythread2.h"
- #include <QDebug>
- 
- externint i;  
- extern QMutex mutex;  
- MyThread2::MyThread2(QString name):QThread(),mName(name)  
- {  
-  qDebug()<<"creating.."<<endl;  
- }  
- 
- void MyThread2::run()  
- {  
-     qDebug()<<this->mName<<"running.."<<endl;  
-     mutex.lock();  
- /*
-     for(;i>0;i--)
-     {
-         qDebug()<<this->mName<<i<<endl;
-     }
-     */
-     i--;  
-     i/=2;  
-     qDebug()<<this->mName<<i<<endl;  
-     mutex.unlock();  
-     qDebug()<<this->mName<<"stop runnning.."<<endl;  
-     sleep(1);  
- }  
main.cpp
[cpp][view plain](https://blog.csdn.net/hechao3225/article/details/53033993#)[copy](https://blog.csdn.net/hechao3225/article/details/53033993#)
- #include <QCoreApplication>
- #include "mythread.h"
- #include "mythread2.h"
- 
- int main(int argc, char *argv[])  
- {  
-     QCoreApplication a(argc, argv);  
-     MyThread thread1("thread1");  
- 
-     MyThread2 thread2("thread2");  
- 
-     thread1.start(QThread::HighestPriority);//高优先级的任务获得更多的CPU使用比，因此先计算完成
-     thread2.start();//相对低优先级的任务会后完成
- 
-     thread1.wait();  
-     qDebug()<<"thread1 is done!"<<endl;  
-     thread1.wait();  
-     qDebug()<<"thread2 is done!"<<endl;  
- 
- //    thread1.exit();
- return a.exec();  
- }  
实验结果：
![](https://img-blog.csdn.net/20170223125854770?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVjaGFvMzIyNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
二、使用互斥锁类QMutexLocker（浅谈RAII）
问题：使用QMutex的上锁、解锁操作直接同步会有一个致命缺陷：当代码提前退出时（如函数中多处return或C++抛出异常），可能并未执行unlock()，若其他线程采用lock()阻塞式上锁会一直被阻塞等待释放，导致资源泄露。
解决：根据RAII的思想，我们应该尽量使用对象管理资源，构造时获取互斥锁，析构时释放锁。（参见Effective C++条款13）
具体来讲，QMutexLocker作为一个便利类，可以解决以下两种函数有多个出口的情况：
（1）第一种情况是函数内部多次return，如果直接使用QMutex上锁，必须保证每个return之前都及时释放锁资源（每个return前都要加上unlock()），否则当前线程的run()退出时另一个线程的run()无法获取锁，造成死锁。如下例所示：
[cpp][view plain](https://blog.csdn.net/hechao3225/article/details/53033993#)[copy](https://blog.csdn.net/hechao3225/article/details/53033993#)
- int complexFunction(int flag)  
- {  
-     QMutexLocker locker(&mutex);  
- 
- int retVal = 0;  
- 
- switch (flag) {  
- case 0:  
- case 1:  
- return moreComplexFunction(flag);  
- case 2:  
-         {  
- int status = anotherFunction();  
- if (status < 0)  
- return -2;  
-             retVal = status + flag;  
-         }  
- break;  
- default:  
- if (flag > 10)  
- return -1;  
- break;  
-     }  
- 
- return retVal;  
- }  
可以看到如果使用QMutex进行上锁，在线程的run()函数中调用该函数，一旦该函数在中途return，又没有及时调用unlock()就会导致互斥锁永远没有机会释放。除非在每一个return前加上QMutex的unlock()。
如果我们按上述代码所示，使用QMutexLocker管理QMutex，由于函数中的QMutexLocker是一个局部对象，因此return的时候一定会调用析构并在析构内部完成互斥锁的释放。
（2）另一种情况是C++抛出异常的情况：C++标准里明确规定抛出异常时仍能保证局部对象的析构调用，这也是RAII技术的保证。也就是说由于QMutexLocker是局部对象，所以一旦遇到函数退出时，局部对象被释放都会调用析构，析构内部会释放锁。（参见Effective C++条款29）
至于为何C++抛出异常时仍能保证释放局部对象（栈上变量），这是C++标准规定，请参看：
https://segmentfault.com/q/1010000002498987
因此，Qt提供了互斥锁类QMutexLocker，当QMutexLocker作为局部对象时，函数中途return或抛出异常时均会调用析构释放对象，而该类的析构函数内部调用了参数绑定的QMutex对应的unlock()函数，这也是RAII技术的基础保证。空口无凭，如图为证，这是QMutexLocker内部的析构函数实现：
![](https://img-blog.csdn.net/20170223125903065?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVjaGFvMzIyNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到，析构里面调用unlock()函数，而unlock()函数内部调用mutex()->unlock()，mutex()是一个常量函数，返回QMutexLocker绑定的QMutex，因此mutex()->unlock()调用了QMutex的unlock()函数，完成了对互斥锁的解锁。如图：
![](https://img-blog.csdn.net/20170223130942898?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVjaGFvMzIyNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到上方的QMutex类中将QMutexLocker声明成了QMutex的友元！因此QMutexLocker可以在析构中调用QMutex的unlock()完成锁资源的释放。这就使得当run()函数有多个出口退出时（多处return或抛出异常），析构被调用并及时完成互斥锁的释放，从而避免锁资源的泄露问题。
另外一个用到RAII思想的技术比如C++STL的智能指针，也是为了避免堆上空间未及时释放的情况。如果使用普通指针申请堆空间，函数中途抛出异常（比如另一个指针申请空间失败，抛出bad_alloc异常），那该指针申请的空间将无法释放，有人说使用捕获异常在catch中释放所有资源，比如此处泄露的内存，但这并不是个好办法，于是根据RAII思想，智能指针产生了，当智能指针的引用计数减为0时会释放这块内存（delete）。
看到这里，终于放心了吗？这是为什么Qt也推荐使用QMutexLocker的原因：RAII技术可以让我们写出异常安全的代码。
