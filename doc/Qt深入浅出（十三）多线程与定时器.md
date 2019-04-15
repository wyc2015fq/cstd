# Qt深入浅出（十三）多线程与定时器

2018年02月24日 00:36:24 [吓人的猿](https://me.csdn.net/qq769651718) 阅读数：1858



# 多线程

​	我们之前的程序都是单线程运行，接下来我们开始引入多线程。就相当于以前的一个人在工作，现在多个人一起工作。

​	Qt中非常有必要使用多线程，这是因为，Qt应用是事件驱动型的，一旦某个事件处理函数处理时间过久，就会造成其它的事件得不到及时处理。

​	Qt中使用QThread来管理线程，一个QThread对象，就是一个线程。QThread对象也有消息循序exec()函数，用来处理自己这个线程的事件。

​	实现多线程有两种方式。







## 1 第一种创建线程方式

1. 首先要继承QThread
2. 重写虚函数QThread::run

```cpp
[virtual protected] void QThread::run()
 /*
 * 基类QThread的run函数只是简单启动exec()消息循环
 */
```



例如：

```cpp
#include <QApplication>
#include <QThread>
#include <QDebug>
class MyThread : public QThread
{
public:
    void run()
    {
        qDebug() << "QThread begin" << endl;
        qDebug() << "child thread" << QThread::currentThreadId() << endl;
        QThread::sleep(5);
        qDebug() << "QThread end"   << endl;
        exec();
    }
};
​
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
​
    MyThread thread;
    thread.start();
    qDebug() << "main thread" << QThread::currentThreadId() << endl;
    QThread::sleep(5);
    qDebug() << "main thread" << QThread::currentThreadId() << endl;
    thread.quit();
    qDebug() << "main thread thread.quit()" << endl;
    tread.wait();
    qDebug() << "main thread thread.wait()" << endl;
    return app.exec();
}
```

​	使用QThread的quit可以退出线程的消息循环，有时候不是马上退出，需要等到cpu的控制权交还给线程的exec()。

​        一般在子线程退出的时候需要主线程去回收资源，可以调用QThread的wait，等待子线程的退出，然后回收资源.

 

## 2 第二种创建线程方式

1. 继承 QObject

2. 实例化一个QThread对象

3. 实现槽函数.

4. QObject子类对象通过moveToThread将自己放到线程QThread对象中.

5. 调用QThread对象的start函数启动线程

6. 必须通过发射信号来让槽函数在线程中执行，发射的信号存放在线程exec()消息队列中。

   例如： 

   mywork.h

```cpp
#ifndef MYWORK_H
#define MYWORK_H
#include <QThread>
#include <QDebug>
class MyWork : public QObject
{
    Q_OBJECT
public slots:
    void workSlot()
    {
        qDebug() << "QThread begin" << endl;
        qDebug() << "child thread" << QThread::currentThreadId() << endl;
        QThread::sleep(5);
        qDebug() << "QThread end"   << endl;
    }
};
#endif // MYWORK_H
```

​	widget.cpp

```cpp
#include <QApplication>
#include <QThread>
#include <QDebug>
#include "mywork.h"
​
int main(int argc, char** argv)
{
    qDebug() << "main thread" << QThread::currentThreadId() << endl;
    QApplication app(argc, argv);
    QThread thread;
    MyWork work;
    work.moveToThread(&thread);
    QObject::connect(&thread, SIGNAL(started()), &work,  SLOT(workSlot()));
    thread.start();
  
    QThread::sleep(6);
    qDebug() << "thread is runing" << thread.isRunning() << endl;
    thread.quit(); //调用quit让线程退出消息循环,否则线程一直在exec循环中
    thread.wait(); //调用完quit后紧接着要调用wait来回收线程资源
    qDebug() << "thread is runing" << thread.isRunning() << endl;
​
    return app.exec();
}
```



需要特别注意的是：

1. 线槽函数已经执行完进入线程exec()中，可以通过发射信号重新让槽函数在线程中执行。也可以通过quit()退出线程exec()。
2. QObject派生类对象，将要调用moveToThread，不能指定一个主线程父对象托管内存。
3. QWidget的对象及派生类对象都只能在GUI主线程运行，不能使用moveToThread移到子线程中，即使没有指定父对象。





- 多线程对象内存释放

  既然QObject对象无法托管内存对象，那么到底是先释放线程对象，还是先释放这个QObject对象？

  先把QObject在线程循环中释放(使用QObject::deleteLater函数)，然后QThread::quit，然后QThread::wait。

例如：

​	mywork.h

```cpp
#ifndef MYWORK_H
#define MYWORK_H
​
#include <QThread>
#include <QDebug>
class MyWork : public QObject
{
    Q_OBJECT
public:
​
    ~MyWork() { qDebug() << __FUNCTION__   << endl; }
public slots:
    void workSlot()
    {
        while(1)
        {
            qDebug() << "Work begin" << endl;
            QThread::sleep(5);
            qDebug() << "work end"   << endl;
        }
    }
    void otherWorkSlot()
    {
        qDebug() << "otherWork begin" << endl;
        QThread::sleep(5);
        qDebug() << "otherWork end"   << endl;
    }
​
​
};
​
#endif // MYWORK_H
```

​	widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QWidget>
#include "mywork.h"
​
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
    QThread *_thread;
    MyWork * _myWork;
};
​
#endif // WIDGET_H
```

​	widget.cpp

```cpp
#include "widget.h"
#include <QPushButton>
#include "mywork.h"
#include <QThread>
#include <QHBoxLayout>
​
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    _myWork = new MyWork;
    _thread = new QThread(this);
    _myWork->moveToThread(_thread);
    _thread->start();
​
    QPushButton *pb0  = new QPushButton("work", this);
    QPushButton *pb1  = new QPushButton("pb", this);
    QHBoxLayout *hBox = new QHBoxLayout(this);
    hBox->addWidget(pb0);
    hBox->addWidget(pb1);
    this->setLayout(hBox);
​
    /*发射信号给在另外一个线程的对象的队列中*/
    connect(pb0, SIGNAL(clicked()), _myWork, SLOT(workSlot()));
    connect(pb1, SIGNAL(clicked()), _myWork, SLOT(otherWorkSlot()));
​
    /*推荐用法释放内存*/
    //connect(_thread, SIGNAL(finished()), _myWork, SLOT(deleteLater()));
​
}
​
Widget::~Widget()
{
    _myWork->deleteLater(); //一定要在QThread线程退出之前
    _thread->quit();
    _thread->wait();
}
```





## 3 线程的同步

​	多线程在访问同时一个资源，(例如：多个线程可操作的变量、函数等)，到底谁来使用这个资源是一个问题，就像一大群人去抢同一块蛋糕，可能其中一个人抢到，更有可能蛋糕被抢个稀烂。在多线程中，这个叫作竞争冒险。那么我们需要定一个规则来约束每一个人，比如：每个人排队来领蛋糕，这个在多线程中叫作同步方法。

​	需要注意的是，同步不是同时，而是有序进行。



### 3.1 互斥锁

​	Qt中的互斥锁是QMutex，不继承任何Qt基类，使用QMutex来锁共享资源，哪个线程抢到钥匙，哪个线程就有这个资源的使用权，其它线程等待这个线程使用完资源并归还钥匙，然后它们再去抢钥匙。

​	例如：

```cpp
QMutex mutex; //这对象一般定义在多个线程能访问的地方
mutex.lock(); //多个线程调用这个函数去获取锁，没有获取到的线程，将阻塞等待在这个函数上。
mutex.unlock(); //释放锁
```

- QMutex::lock函数会让线程等待获取锁，如果不想等待，可以使用一下函数替换:

```cpp
bool QMutex::tryLock(int timeout = 0)
/*
*参数 int timeout:等到timeout毫秒，不管有没获取到锁都返回，timeout为0时，直接返回。
*返回值 true代表获取到锁，false没有获取到
*/
```

- 有时候我们会忘记释放锁，Qt还为我们提供了管理锁的类QMutexLocker

```cpp
QMutex mutex;
void func()
{
    QMutexLocker locker(_mutex); //QMutexLocker最好实例化成栈对象，释放之前将QMutex解锁。  
}
```

以上例子，一旦func()执行完成locker自动释放，释放之前先解锁。







### 3.2 信号量

​	互斥锁保护的资源同一时刻只能有一个线程能够获取使用权，有些资源是可以限定多个线程同时访问，那么这个时候可以使用信号量。在Qt中信号量为QSemaphore。



```cpp
QSemaphore sem(2) //初始化信号量为2
sem.acquire();  //信号量部位0的时候，调用这个函数会让信号量-1，一旦信号量为零，阻塞等待
semaphore.release(); //使信号量+1
```



## 4 定时器

​	定时器可以隔一段时间发出信号，通过接收这个信号来处理一些定时任务，需要注意的是，定时器并没有开启一个新线程。Qt中的定时器是QTimer继承自QObject。

- 通过QTimer::start()来启动定时器

```cpp
void start(int msec)
/*
*定时msec毫秒后，发射timeout()信号
*/
```

​	通过链接信号timeout()来处理一些定时任务，例如：

```cpp
#include "dialog.h"
#include <QTimer>
#include <qdebug.h>
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(doSomeThing()));
    timer->start(3000);
    
}
void Dialog::doSomeThing()
{
    qDebug() <<  __FUNCTION__ << endl;
}
​
Dialog::~Dialog()
{
​
}
```