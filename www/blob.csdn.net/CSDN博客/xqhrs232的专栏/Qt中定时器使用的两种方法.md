# Qt中定时器使用的两种方法 - xqhrs232的专栏 - CSDN博客
2017年09月26日 17:58:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：570
原文地址::[http://blog.csdn.net/andy_93/article/details/52776283](http://blog.csdn.net/andy_93/article/details/52776283)
  Qt中定时器的使用有两种方法，一种是使用QObject类提供的定时器，还有一种就是使用QTimer类。
         其精确度一般依赖于操作系统和硬件，但一般支持20ms。下面将分别介绍两种方法来使用定时器。
方法一：QObject中的定时器的使用，需要用到三个函数
1、   int QObject::startTimer ( int interval ) ;
        这个是开启一个定时器的函数，他的参数interval是毫秒级别。当开启成功后会返回这个定时器的ID, 并且每隔interval 时间后会进入timerEvent 函数。直到定时器被杀死。
2、 void QObject::timerEvent ( QTimerEvent * event ); 
当定时器超时后，会进入该事件timerEvent函数,需要重写timerEvent函数，在函数中通过判断event->timerId()来确定定时器,然后执行某个定时器的超时函数。
3、 void QObject::killTimer ( int id );
 通过从startTimer返回的ID传入killTimer 函数中杀死定时器，结束定时器进入超时处理。
以下是QObject中的定时器具体使用简单例子：
[cpp][view
 plain](http://blog.csdn.net/andy_93/article/details/52776283#)[copy](http://blog.csdn.net/andy_93/article/details/52776283#)
- #define _MYTIMER_H
- 
- #include <QObject>
- 
- class MyTimer : public QObject  
- {  
-     Q_OBJECT  
- 
- public:  
-     MyTimer(QObject* parent = NULL);  
-     ~MyTimer();  
- void  handleTimeout();  //超时处理函数
- virtualvoid timerEvent( QTimerEvent *event);  
- private:  
- int m_nTimerID;  
- };  
- 
- #endif //_MYTIMER_H
[cpp][view
 plain](http://blog.csdn.net/andy_93/article/details/52776283#)[copy](http://blog.csdn.net/andy_93/article/details/52776283#)
- #include "mytimer.h"
- 
- #include<QDebug> 
- #include <QTimerEvent>
- 
- #define TIMER_TIMEOUT   (5*1000)
- 
- MyTimer::MyTimer(QObject *parent)  
-     :QObject(parent)  
- {  
-     m_nTimerID = this->startTimer(TIMER_TIMEOUT);  
- }  
- 
- MyTimer::~MyTimer()  
- {  
- 
- }  
- 
- void MyTimer::timerEvent(QTimerEvent *event)  
- {  
- if(event->timerId() == m_nTimerID){  
-         handleTimeout();  
-     }  
- }  
- 
- void MyTimer::handleTimeout()  
- {  
-     qDebug()<<"Enter timeout processing function\n";  
-     killTimer(m_nTimerID);  
- }  
方法二：使用QTimer定时器类
1、  首先创建一个定时器类的对象
QTimer *timer = new QTimer(this);
2、  timer 超时后会发出timeout()信号，所以在创建好定时器对象后给其建立信号与槽
connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
3、  在需要开启定时器的地方调用void QTimer::start ( int msec );
这个start函数参数也是毫秒级别；
timer->start(msec );
4、 在自己的超时槽函数里面做超时处理。
以下是QTimer定时器类具体使用简单例子：
[cpp][view
 plain](http://blog.csdn.net/andy_93/article/details/52776283#)[copy](http://blog.csdn.net/andy_93/article/details/52776283#)
- #ifndef _MYTIMER_H
- #define _MYTIMER_H
- 
- #include <QObject>
- class QTimer;  
- class MyTimer : public QObject  
- {  
-     Q_OBJECT  
- 
- public:  
-     MyTimer(QObject* parent = NULL);  
-     ~MyTimer();  
- public slots:  
- void handleTimeout();  //超时处理函数
- private:  
-     QTimer *m_pTimer;  
- };  
- 
- #endif //_MYTIMER_H
[cpp][view
 plain](http://blog.csdn.net/andy_93/article/details/52776283#)[copy](http://blog.csdn.net/andy_93/article/details/52776283#)
- #include "mytimer.h"
- 
- #include<QDebug> 
- #include <QTimer>
- 
- #define TIMER_TIMEOUT   (5*1000)
- 
- MyTimer::MyTimer(QObject *parent)  
-     :QObject(parent)  
- {  
-     m_pTimer = new QTimer(this);  
-     connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));  
-     m_pTimer->start(TIMER_TIMEOUT);  
- }  
- 
- MyTimer::~MyTimer()  
- {  
- 
- }  
- 
- void MyTimer::handleTimeout()  
- {  
-     qDebug()<<"Enter timeout processing function\n";  
- if(m_pTimer->isActive()){  
-         m_pTimer->stop();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)

