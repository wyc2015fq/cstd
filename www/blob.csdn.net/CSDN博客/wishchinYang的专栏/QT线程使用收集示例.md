# QT线程使用收集示例 - wishchinYang的专栏 - CSDN博客
2013年10月20日 14:13:12[wishchin](https://me.csdn.net/wishchin)阅读数：2024
关于多线程问题：
Qt和Boost做跨平台的线程封装，OpenMP主要做并行计算，让不精通多线程的人也能高效地利用CPU的计算能力。
个人倾向于用boost.thread, boost.mpi.
**   一个openMP的指南网页**：[http://www.cnblogs.com/me115/archive/2011/01/25/1944567.html](http://www.cnblogs.com/me115/archive/2011/01/25/1944567.html)
      内容非常详细。
**关于[OpenMP创建线程中的锁及原子操作性能比较](http://blog.csdn.net/drzhouweiming/article/details/1689853)：**
      原文链接：  [http://blog.csdn.net/drzhouweiming/article/details/1689853](http://blog.csdn.net/drzhouweiming/article/details/1689853)
** 一片有关图像特征提取的openMp编程**：[http://www.cnblogs.com/yangyangcv/archive/2012/03/23/2413335.html](http://www.cnblogs.com/yangyangcv/archive/2012/03/23/2413335.html)
（1）：实现QT简单多线程
  在qt程序中，我们经常会遇到计算密集型操作或者存在大量I/O操作的时候，GUI就会发生冻结现象，并且会无法响应.仔细分析一下为什么会出现这种情况：因为GUI是一个主线程，而我们如果把计算密集型或者I/O操作放到主线程中去执行，（换句话说我们采用单一线程方案），那我们只能等待事件执行完之后GUI才能进行响应。
      1. 有种比较简单的办法是调用QApplication::processEvents()，但是属于治标不治本的方法。这个函数的作用主要就是处理qt在计算事件中处理某个点悬挂的事件。
      qt文档中有句话影响比较深刻：In event you are running a local loop which calls this function continuously, without an event loop, the [DeferredDelete](http://hi.baidu.com/chb_seaok/creat/blog/qevent.html#Type-enum) events
 will not be processed.
      2. 从单一线程的方式改为多线程的方式。
      qt应用程序中，多线程的操作实现是非常简单的：只需要子类化QThread，实现他的run()函数即可。但是多线程如果在单个CPU中运行相对单线程可能会慢点，多个CPU的话其优势会展现出来。下面我就实现一个多线程的简单案例：
```cpp
//Thread.h
#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include<QDebug>
class Thread : public QThread
{
 Q_OBJECT
public:
 Thread(int _type);
 ~Thread();
 void run();   
private:
 int type;         //线程类型1或2
 
};
#endif // THREAD_H
//Thread.cpp
#include "Thread.h"
Thread::Thread(int _type)
:type(_type)
{
}
Thread::~Thread()
{
}
void Thread::run()
{
 int count=5;
 while(count>0)
 {
  qDebug()<<"Thread"<<type;           //循环5次打印类型
  count--;
 }
}
//main.cpp     main函数中调用
#include <QtCore/QCoreApplication>
#include "Thread.h"
int main(int argc, char *argv[])
{
 QCoreApplication a(argc, argv);
 Thread *pThread1=new Thread(1);    //线程1
 pThread1->start();
 Thread *pThread2=new Thread(2);    //线程2
 pThread2->start();
 return a.exec();
}
```
两次运行结果：
（2）:实现同步QT ftp
   原文链接：http://mobile.51cto.com/symbian-268682.htm
在Qt中实现QThread线程同步QFtp ，对于QFtp，它是一个用来实现FTP协议的类，详情查阅资料。接触Qt没有多长时间，但简单几个小例子已经让我感受到Qt在C++运用方面的强大。写了一个小程序，需要在一个单独的线程中使用QFtp来获取FTP服务器上面的文件。FTP是FileZilla。
有两个问题我比较关心：
（1）QThread到底如何使用
（2）QFtp是Async（异步）操作，也就是说例如connectToHost这样的函数都是立刻返回，当操作完成后QFtp会发出signal。然而既然我的Ftp操作是在一个单独的线程，我想写一个函数downloadFtpFile() 来完成从connect到login到下载文件等一系列的操作，然后再返回。相当于我需要Sync（同步）的操作，所以需要等待（block）每个Ftp命令的结果。
在该文章最后有一个推荐的使用QThread的方法。我在这里想补充一点：obj.moveToThread(&thread); 这句话将obj从主线程移动到了thread对象所在的线程。但如果obj的对象里面有其他的变量，那么这些变量是在主线程中生成的。所以如果这些变量中有类变量，不能将obj的this指针作为parent传给他们。
对于第二个问题，我使用了QSemaphore类来完成我的block和同步操作：在slot函数里面接收QFtp命令执行结果的signal，释放信号，同时downloadFtpFile()函数里在调用完每一个QFtp异步命令后等待信号。在有点令人失望的是QSemaphore在通过tryAcquire()等待信号的时候是不处理事件event的。但是我需要在等待的时候程序也能触发slot，告诉我当前命令的执行情况。所以我使用了一个小循环，里面调用qApp->processEvents();来让我的slot函数被触发。下面是代码例子（只是样例，并不完全符合C++语法）：
首先是我的下载Ftp文件的函数：
```cpp
downloadFtpFile () //该函数在单独线程里执行     
    {     
         int m_idFtpOp; // 该变量用来存放每一个QFtp命令ID     
         int nVal;     
         QFtp*pFtp=newQFtp (this); // 生成QFtp工具对象     
         connect (pFtp,SIGNAL(listInfo(QUrlInfo)),this,SLOT(slotFtpListInfo(QUrlInfo))); // 我们需要listinfo，因为我们需要下载ftp所有当前目录文件     
         connect (pFtp,SIGNAL(commandFinished(int,bool)),this,SLOT(slotFtpCmdFinished(int,bool)));   
    // 每个QFtp命令完成之后，会发出commandFinished信号，我们在槽函数中处理该信号  
         m_idFtpOp = pFtp->connectToHost (<FTP地址>, 21); // 连接到远程FTP Server     
         bRet=false;     
         nVal=100;     
         while (bRet == false) // 使用nVal变量来做一个10000ms（10s）的超时     
         {     
               nVal--;     
               if (nVal == 0)     
                    break;     
               qApp->processEvents();   // 这里每100ms处理一次event，使slot函数能够被调用     
               bRet=m_SemOp.tryAcquire (1,100); // 等待信号100ms     
         }     
        if (!bRet || m_bFtpOpError)   // 如果超时，或者slot函数中将m_bFtpOpError置成true，则关闭Ftp，返回错误     
          {     
               pFtp->abort();     
               pFtp->deleteLater();     
               return ERRCODE_FCC_FTP_CONN_TIMEOUT;     
         }     
    }
```
下面是槽函数
- slotFtpCmdFinished (int id, bool error)     
- {     
-          if (m_idFtpOp == id)  // 如果返回的id是当前正在操作的命令     
-          {     
-                   if (error)     
- m_bFtpOpError=true;     
-                   else    
- m_bFtpOpError=false;     
-                   m_SemOp.release();  // 释放信号（使downloadFtpFile函数中m_SemOp.tryAcquire()返回true）     
-          }     
- }   
以上的代码只演示了对QFtp第一个命令connectToHost的等待过程。下面的login，list，get等操作都使用这个方法。
注意：在此例中，QFtp是在当前线程生成的，所以信号listInfo(QUrlInfo)的connect方式是direct连接。如果QFtp是在另一个线程生成（比如说是在函数downloadFtpFile所在类的构造函数中），那么第一：不能将this指针作为parent传给QFtp对象，第二：需要使用qRegisterMetaType<QUrlInfo>("QUrlInfo");来注册QUrlInfo类，因为信号发射与接收在不通的线程中，信号使用queued的方式。如果不注册QURlInfo类，会在运行时动态报告错误。
总结：本文介绍的是在Qt中如何实现QThread线程同步QFtp ，看过本文之后，如果对于QThread不了解的话，那么请参考[Qt中QThread使用方法](http://www.51cto.com/php/viewart.php?artID=268690)这篇文章。使用本文介绍的方法，可以在独立的线程中用同步的方式使用QFtp。在某些场合，尤其是采用应答机制的系统中，这样的实现可以很大程度上简化程序流程。
（3）：浅谈QT多线程
原文链接:[http://mobile.51cto.com/symbian-268343.htm](http://mobile.51cto.com/symbian-268343.htm)
Qt 作为一种基于 C++ 的跨平台 GUI 系统，能够提供给用户构造图形用户界面的强大功能。为了满足用户构造复杂图形界面系统的需求，Qt 提供了丰富的多线程编程支持。从 2.2 版本开始，Qt 主要从下面三个方面对多线程编程提供支持：一、构造了一些基本的与平台无关的线程类；二、提交用户自定义事件的
 Thread-safe 方式；三、多种线程间同步机制，如信号量，全局锁。这些都给用户提供了极大的方便。不过，在某些情况下，使用定时器机制能够比利用 Qt 本身的多线程机制更方便地实现所需要的功能，同时也避免了不安全的现象发生。本文不仅对Qt 中的多线程支持机制进行了讨论，还着重探讨了利用定时器机制模拟多线程编程的方法。
1、系统对多线程编程的支持
不同的平台对Qt 的多线程支持方式是不同的。当用户在 Windows 操作系统上安装 Qt 系统时，线程支持是编译器的一个选项，在 Qt 的 mkfiles 子目录中包括了不同种类编译器的编译文件，其中带有 -mt 后缀的文件才是支持多线程的。
而在 Unix 操作系统中，线程的支持是通过在运行 configure 脚本文件时添加 -thread 选项加入的。安装过程将创建一个独立的库，即 libqt-mt，因此要支持多线程编程时，必须与该库链接（链接选项为-lqt-mt），而不是与通常的 Qt 库（-lqt）链接。
另外，无论是何种平台，在增加线程支持时都需要定义宏 QT_THREAD_SUPPORT（即增加编译选项-DQT_THREAD_SUPPORT）。在 Windows 操作系统中，这一点通常是在 qconfig.h 文件中增加一个选项来实现的。而在 Unix 系统中通常添加在有关的 Makefile 文件中。
2、Qt中的线程类
在 Qt 系统中与线程相关的最重要的类当然是 QThread 类，该类提供了创建一个新线程以及控制线程运行的各种方法。线程是通过 QThread::run() 重载函数开始执行的，这一点很象 Java 语言中的线程类。在 Qt 系统中，始终运行着一个GUI 主事件线程，这个主线程从窗口系统中获取事件，并将它们分发到各个组件去处理。在 QThread 类中还有一种从非主事件线程中将事件提交给一个对象的方法，也就是 QThread::postEvent()方法，该方法提供了 Qt 中的一种 Thread-safe
 的事件提交过程。提交的事件被放进一个队列中，然后 GUI 主事件线程被唤醒并将此事件发给相应的对象，这个过程与一般的窗口系统事件处理过程是一样的。值得注意的是，当事件处理过程被调用时，是在主事件线程中被 调用的，而不是在调用QThread::postEvent 方法的线程中被调用。比如用户可以从一个线程中迫使另一个线程重画指定区域：
- QWidget *mywidget;  
- QThread::postEvent(mywidget, new QPaintEvent(QRect(0,0,100,100)));   
然而，只有一个线程类是不够的，为编写出支持多线程的程序，还需要实现两个不同的线程对共有数据的互斥访问，因此 Qt 还提供了 QMutex 类，一个线程在访问临界数据时，需要加锁，此时其他线程是无法对该临界数据同时加锁的，直到前一个线程释放该临界数据。通过这种方式才能实现对临界数据的 原子操作。
除此之外，还需要一些机制使得处于等待状态的线程在特定情况下被唤醒。QWaitCondition 类就提供了这种功能。当发生特定事件时，QWaitCondition 将唤醒等待该事件的所有线程或者唤醒任意一个被选中的线程。
3、用户自定义事件在多线程编程中的应用
在 Qt 系统中，定义了很多种类的事件，如定时器事件、鼠标移动事件、键盘事件、窗口控件事件等。通常，事件都来自底层的窗口系统，Qt 的主事件循环函数从系统的事件队列中获取这些事件，并将它们转换为 QEvent，然后传给相应的 QObjects 对象。
除此之外，为了满足用户的需求，Qt 系统还提供了一个 QCustomEvent 类，用于用户自定义事件，这些自定义事件可以利用 QThread::postEvent() 或者QApplication::postEvent() 被发给各种控件或其他 QObject 实例，而 QWidget 类的子类可以通过 QWidget::customEvent() 事件处理函数方便地接收到这些自定义的事件。需要注意的是：QCustomEvent 对象在创建时都带有一个类型标识 id 以定义事件类型，为了避免与 Qt 系统定义的事件类型冲突，该
 id 值应该大于枚举类型 QEvent::Type 中给出的 "User" 值。
在下面的例子中，显示了多线程编程中如何利用用户自定义事件类。
UserEvent类是用户自定义的事件类，其事件标识为346798，显然不会与系统定义的事件类型冲突。
- class UserEvent : public QCustomEvent   //用户自定义的事件类{  
- public: UserEvent(QString s) : QCustomEvent(346798), sz(s) { ; }   
- QString str() const {  
-  return sz;  
-  }  
- private: QString sz;     
-  };                  
UserThread类是由QThread类继承而来的子类，在该类中除了定义有关的变量和线程控制函数外，最主要的是定义线程的启动 函数UserThread::run()，在该函数中创建了一个用户自定义事件UserEvent，并利用QThread类的postEvent函数提交 该事件给相应的接收对象。      
- class UserThread : public QThread      //用户定义的线程类{  
- public: UserThread(QObject *r, QMutex *m, QWaitCondition *c);  
- QObject *receiver;  
- }  
- void UserThread::run()     //线程类启动函数，在该函数中创建了一个用户自定义事件{  
- UserEvent *re = new UserEvent(resultstring);     
- QThread::postEvent(receiver, re);   
- }  
UserWidget类是用户定义的用于接收自定义事件的QWidget类的子类，该类利用slotGo()函数创建了一个新的线程 recv（UserThread类），当收到相应的自定义事件（即id为346798）时，利用customEvent函数对事件进行处理。
- void UserWidget::slotGo()    //用户定义控件的成员函数{   
- mutex.lock();     
- if (! recv)    
- recv = new UserThread(this, &mutex, &condition);   
-   recv->start(); mutex.unlock();  
- }  
- void UserWidget::customEvent(QCustomEvent *e) //用户自定义事件处理函数{   
- if (e->type()==346798)  {   
-    UserEvent *re = (UserEvent *) e;          
- newstring = re->str();      
- }  
- }  
在这个例子中，UserWidget对象中创建了新的线程UserThread，用户可以利用这个线程实现一些周期性的处理（如接收底 层发来的消息等），一旦满足特定条件就提交一个用户自定义的事件，当UserWidget对象收到该事件时，可以按需求做出相应的处理，而一般情况 下，UserWidget对象可以正常地执行某些例行处理，而完全不受底层消息的影响。
4、利用定时器机制实现多线程编程
为了避免Qt系统中多线程编程带来的问题，还可以使用系统中提供的定时器机制来实现类似的功能。定时器机制将并发的事件串行 化，简化了对并发事件的处理，从而避免了thread-safe方面问题的出现。
在下面的例子中，同时有若干个对象需要接收底层发来的消息（可以通过Socket、FIFO等进程间通信机制），而消息是随机收到的， 需要有一个GUI主线程专门负责接收消息。当收到消息时主线程初始化相应对象使之开始处理，同时返回，这样主线程就可以始终更新界面显示并接收外界发来的 消息，达到同时对多个对象的控制；另一方面，各个对象在处理完消息后需要通知GUI主线程。对于这个问题，可以利用第3节中的用户自定义事件的方法，在主 线程中安装一个事件过滤器，来捕捉从各个对象中发来的自定义事件，然后发出信号调用主线程中的一个槽函数。
另外，也可以利用Qt中的定时器机制实现类似的功能，而又不必担心Thread-safe问题。下面就是有关的代码部分：
在用户定义的Server类中创建和启动了定时器，并利用connect函数将定时器超时与读取设备文件数据相关联：
- Server:: Server(QWidget *parent) : QWidget(parent){  
- readTimer = new QTimer(this);   //创建并启动定时器    
-  connect(readTimer, SIGNAL(timeout()), this, SLOT(slotReadFile()));   //每当定时器超时时调用函数slotReadFile读取文件     
- readTimer->start(100);}     
slotReadFile函数负责在定时器超时时，从文件中读取数据，然后重新启动定时器：
- int Server::slotReadFile()    // 消息读取和处理函数{    
- readTimer->stop();     //暂时停止定时器计时    
- ret = read(file, buf );   //读取文件if(ret == NULL){      
- readTimer->start(100);     //当没有新消息时，重新启动定时器      
- return(-1);  
- }  else         
-        根据buf中的内容将消息分发给各个相应的对象处理……；  
- readTimer->start(100);    //重新启动定时器}        
在该程序中，利用了类似轮循的方式定时对用户指定的设备文件进行读取，根据读到的数据内容将信息发送到各个相应的对象。用户可以在自己 的GUI主线程中创建一个Server类，帮助实现底层的消息接收过程，而本身仍然可以处理诸如界面显示的问题。当各个对象完成处理后，通过重新启动定时 器继续进行周期性读取底层设备文件的过程。当然，这种方法适合于各对象对事件的处理时间较短，而底层设备发来消息的频率又相对较慢的情况。在这种情况下， 上述方法完全可以满足用户的需求，而又避免了处理一些与线程并发有关的复杂问题。
当然，利用定时器机制实现多线程编程在某些方面具有一定的局限性，有关到底如何实现多线程编程，如何编写出效率更高的代码，还有待于开 发者进一步研究和探讨。

**（4）：示例解析QT线程同步**
**Qt 线程同步**实例介绍是本文介绍的内容，在**Qt**中使用**线程**，没有Mfc中那么繁琐，它提供了QThread**线程**类，提供了创建一个新的方法。**线程**通过重载QThread::run()函数来完成其操作的，这一点与Java中的线程类相似。
实现一个简单的继承自QThread的用户**线程**类，代码如下。
1.           class Thread : public QThread   2.           {  3.           public:  4.               Thread();  5.               void stop();  6.           protected:  7.               virtual void run();  8.           private:  9.               bool m_stop;  10.       };  11.       Thread::Thread()  12.       {  13.           m_stop = false;  14.       }  15.       void Thread::stop()  16.       {  17.           m_stop = true;  18.       }  19.       void Thread::run()  20.       {  21.           while (!m_stop)  22.           {  23.               sleep(1);  24.               qDebug("vic.MINg!");  25.           }  26.           qDebug("end!");  27.       } 
在以上的示例中可以看出，线程的编写并不难！
启动线程的时候可以，调用函数QThread::start()，开始Thread线程对象。
停止线程的时候可以，调用函数QThread::terminate()，但是terminate()函数并不会立刻终止线程，该线程何时终止取决于操作系统的调度策略。需要注意的是，terminate()函数过于毒辣，它可能在线程执行的任意一步终止执行，从而产生不可预知的后果(如修改某个重要数据时)，另外，它也没有给线程任何清理现场的机会(如释放内存和锁等)。
因此，停止线程可以，如上代码所示，手写函数stop()，使其线程柔和的退出。
线程停止后，应调用QThread::wait()函数，它使的线程阻塞等待直到退出或超时。
貌似在Unix或Linux下编译多线程应用程序还必须在.pro文件中加入如下一行,它告诉qmake使用Qt库中的线程版本。Windows上，Qt库默认就是线程的。
**CONFIG += thread**
介绍完了线程的创建，接下来走入正题了，多线程应用程序的一个最普通的需求就是**同步**几个线程。Qt提供了以下几个类来完成这一点：QMutex、QMutexLocker、QSemphore、QWaitCondition。
当然可能还包含QReadWriteLocker、QReadLocker、QWriteLocker，但**线程同步**是应用很少，这里只做简单的讲解！
**QMutex、QMutexLocker**
QMutex类提供了一个保护一段临界区代码的方法，他每次只允许一个**线程**访问这段临界区代码。QMutex::lock()函数用来锁住互斥量，如果互斥量处于解锁状态，当前线程就会立即抓住并锁定它；否则当前**线程**就会被阻塞，直到持有这个互斥量的**线程**对其解锁。**线程**调用lock()函数后就会持有这个互斥量直到调用unlock()操作为止。QMutex还提供了一个tryLock()函数，如果互斥量已被锁定，就立即返回。
现在使用QMutex保护上面的**线程**类的m_stop布尔变量，虽然没啥用，但这里的目的只是为了演示下QMutex的用法~~
1.           //thread.h头文件，添加互斥量对象 2.           private:  3.               ...  4.               QMutex mutex;  5.           };  6.           void Thread::run()  7.           {  8.               forever {  9.                   mutex.lock();  10.               if (m_stop) {  11.                   m_stop = false;  12.                   mutex.unlock();  13.                   break;  14.               }  15.               mutex.unlock();  16.               qDebug("vic.MINg!");  17.           }  18.           qDebug("end!");  19.       }  20.       void Thread::stop()  21.       {  22.           mutex.lock();  23.           m_stop = true;  24.           mutex.unlock();  25.       } 
在这里QMutex能够完全完成互斥操作，但是有些情况下QMutex类是无法某些特定的互斥操作的，下面举个例子：
这里我们把void stop()函数，重新定义下，让他以布尔形式返回，实际也没有啥用...只为示例的演示效果~~
1.           bool Thread::stop()  2.           {  3.               m_stop = true;  4.               return m_stop;  5.           } 
现在问题出来了，如果要在stop()函数中使用mutex进行互斥操作，但unlock()操作写在那里？unlock()操作却不得不再return之后，从而导致unlock()操作永远也无法执行...
**Qt**提供了QMutexLocker类何以简化互斥量的处理，它在构造函数中接受一个QMutex对象作为参数并将其锁定，在析构函数中解锁这个互斥量。
这样可以像下面这样重新编写stop()函数：
1.           bool Thread::stop()  2.           {  3.               QMutexLocker locker(&mutex);  4.               m_stop = true;  5.               return m_stop;  6.           } 
**QReadWriteLocker、QReadLocker、QWriteLocker**
下面是一段对QReadWriteLocker类的对象进行，读写锁的操作，比较简单，这里也不多做讲解了，自己看吧 :)
1.           MyData data;  2.           QReadWriteLock lock;  3.           void ReaderThread::run()  4.           {  5.               ...  6.               lock.lockForRead();  7.               access_data_without_modifying_it(&data);  8.               lock.unlock();  9.               ...  10.       }  11.       void WriterThread::run()  12.       {  13.           ...  14.           lock.lockForWrite();  15.           modify_data(&data);  16.           lock.unlock();  17.           ...  18.       } 
**QSemphore**
**Qt**中的信号量是由QSemaphore类提供的，信号量可以理解为互斥量功能的扩展，互斥量只能锁定一次而信号量可以获取多次，它可以用来保护一定数量的同种资源。
acquire(n)函数用于获取n个资源，当没有足够的资源时调用者将被阻塞直到有足够的可用资源。release(n)函数用于释放n个资源。
QSemaphore类还提供了一个tryAcquire(n)函数，在没有足够的资源是该函数会立即返回。
一个典型的信号量应用程序是在两个**线程**间传递一定数量的数据(DataSize)，而这两个**线程**使用一定大小(BufferSize)的共享循环缓存。
1.           const int DataSize = 100000;  2.           const int BufferSize = 4096;  3.           char buffer[BufferSize]; 
生产者**线程**向缓存中写入数据，直到它到达终点，然后在起点重新开始，覆盖已经存在的数据。消费者**线程**读取前者产生的数据。
生产者、消费者实例中对**同步**的需求有两处，如果生产者过快的产生数据，将会覆盖消费者还没有读取的数据，如果消费者过快的读取数据，将越过生产者并且读取到一些垃圾数据。
解决这个问题的一个有效的方法是使用两个信号量：
1.           QSemaphore freeSpace(BufferSize);  2.           QSemaphore usedSpace(0); 
freeSpace信号量控制生产者可以填充数据的缓存部分。usedSpace信号量控制消费者可以读取的区域。这两个信号量是互补的。其中freeSpace信号量被初始化为BufferSize(4096)，表示程序一开始有BufferSize个缓冲区单元可被填充，而信号量usedSpace被初始化为0，表示程序一开始缓冲区中没有数据可供读取。
对于这个实例，每个字节就看作一个资源，实际应用中常会在更大的单位上进行操作，从而减小使用信号量带来的开销。
1.           void Producer::run()  2.           {  3.               for (int i = 0; i **<DataSize**; ++i) {  4.                   freeSpace.acquire();  5.                   buffer[i % BufferSize] = "MING"[uint(rand()) % 4];  6.                   usedSpace.release();  7.               }  8.           } 
在生产者中，我们从获取一个“自由的”字节开始。如果缓存被消费者还没有读取的数据填满，acquire()的调用就会阻塞，直到消费者已经开始消耗这些数据为止。一旦我们已经获取了这个字节，我们就用一些随机数据("M"、"I"、"N"或"G")填充它并且把这个字节释放为“使用的”,所以它可以被消费者**线程**使用。
1.           void Consumer::run()  2.           {  3.               for (int i = 0; i **<DataSize**; ++i) {  4.                   usedSpace.acquire();  5.                   cerr **<<buffer**[i % BufferSize];  6.                   freeSpace.release();  7.               }  8.               cerr **<<endl**;  9.           } 
在消费者中，我们从获取一个“使用的”字节开始。如果缓存中没有包含任何可读的数据，acquire()调用将会阻塞，直到生产者已经产生一些数据。一旦我们已经获取了这个字节，我们就打印它并且把这个字节释放为“自由的”，使它可以被生产者使用来再次填充数据。
1.           int main()  2.           {  3.               Producer producer;  4.               Consumer consumer;  5.               producer.start();  6.               consumer.start();  7.               producer.wait();  8.               consumer.wait();  9.               return 0;  10.       } 
main()函数的功能比较简单，负责启动生产者和消费者**线程**，然后等待其各自执行完毕后自动退出。
**QWaitCondition**
对生产者和消费者问题的另一个解决方法是使用QWaitCondition,它允许**线程在**一定条件下唤醒其他**线程**。其中wakeOne()函数在条件满足时随机唤醒一个等待**线程**，而wakeAll()函数则在条件满足时唤醒所有等待**线程**。
下面重写生产者和消费者实例，以QMutex为等待条件，QWaitCondition允许一个**线程**在一定条件下唤醒其他**线程**。
1.           const int DataSize = 100000;  2.           const int BufferSize = 4096;  3.           char buffer[BufferSize];  4.           QWaitCondition bufferIsNotFull;  5.           QWaitCondition bufferIsNotEmpty;  6.           QMutex mutex;  7.           int usedSpace = 0; 
在缓存之外，我们声明了两个QWaitCondition、一个QMutex和一个存储了在缓存中有多少个“使用的”字节的变量。
1.           void Producer::run()  2.           {  3.               for (int i = 0; i **<DataSize**; ++i) {  4.                   mutex.lock();  5.                   if (usedSpace == BufferSize)  6.                       bufferIsNotFull.wait(&mutex);  7.                   buffer[i % BufferSize] = "MING"[uint(rand()) % 4];  8.                   ++usedSpace;  9.                   bufferIsNotEmpty.wakeAll();  10.               mutex.unlock();  11.           }  12.       } 
在生产者中，我们从检查缓存是否充满开始。如果是充满的，我们等待“缓存不是充满的”条件。当这个条件满足时，我们向缓存写入一个字节，增加usedSpace，并且在唤醒任何等待这个“缓存不是空白的”条件变为真的**线程**。
for循环中的所有语句需要使用互斥量加以保护，以保护其操作的原子性。
1.           bool wait ( QMutex * mutex, unsigned long time = ULONG_MAX ); 
这个函数做下说明，该函数将互斥量解锁并在此等待，它有两个参数，第一个参数为一个锁定的互斥量，第二个参数为等待时间。如果作为第一个参数的互斥量在调用是不是锁定的或出现递归锁定的情况，wait()函数将立即返回。
调用wait()操作的**线程**使得作为参数的互斥量在调用前变为锁定状态，然后自身被阻塞变成为等待状态直到满足以下条件：
其他**线程**调用了wakeOne()或者wakeAll()函数，这种情况下将返回"true"值。
第二个参数time超时(以毫秒记时)，该参数默认情况是ULONG_MAX，表示永不超时，这种情况下将返回"false"值。
wait()函数返回前会将互斥量参数重新设置为锁定状态，从而保证从锁定状态到等待状态的原则性转换。
1.           void Consumer::run()  2.           {  3.               forever {  4.                   mutex.lock();  5.                   if (usedSpace == 0)  6.                       bufferIsNotEmpty.wait(&mutex);  7.                   cerr **<<buffer**[i % BufferSize];  8.                   --usedSpace;  9.                   bufferIsNotFull.wakeAll();  10.               mutex.unlock();  11.           }  12.           cerr **<<endl**;  13.       } 
消费者做的和生产者正好相反，他等待“缓存不是空白的”条件并唤醒任何等待“缓存不是充满的”的条件的**线程**。
main()函数与上面的基本相同，这个不再多说。
在QThread类的静态函数currentThread()，可以返回当前**线程**的**线程**ID。在X11环境下，这个ID是一个unsigned long类型的值。
小结：关于 **Qt 线程同步**实例介绍的内容介绍完了，希望本文对你有所帮助。

