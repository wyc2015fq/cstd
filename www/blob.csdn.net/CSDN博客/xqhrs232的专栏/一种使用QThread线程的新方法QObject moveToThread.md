# 一种使用QThread线程的新方法QObject::moveToThread - xqhrs232的专栏 - CSDN博客
2018年06月06日 18:09:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：109
原文地址::[https://blog.csdn.net/sydnash/article/details/7425947](https://blog.csdn.net/sydnash/article/details/7425947)
昨晚在一个邮件列表里面看见一个关于在线程种使用signal/slot的讨论，由于回复太多，这里就不贴出原文了。
        主要是关于怎样从一个线程发送信号到另外一个线程的问题。其实这个也不是什么复杂的问题，在qt的asstant里面已经描诉的比较清楚了。当我们链接信号的时候使用qt::queuedConnection就能使slot在它自己的线程里面运行。
        另我惊讶的是在其中一个的回复种他给出了一些资料，其中一个名为you‘ar doing it wrong。帖子是英文的，由于英文水平有限，加上他所说的使用QT thread的方式和我们平时直接派生QThread实现run函数的方式不一样，所以让我看的非常含糊，甚至到了不清不楚的地步。看了后面的大量的回复和讨论，勉强明白了它的意思。
    具体请看这里[](http://qt-project.org/doc/qt-4.8/thread-basics.html)
[http://labs.qt.nokia.com/2010/06/17/youre-doing-it-wrong/](http://labs.qt.nokia.com/2010/06/17/youre-doing-it-wrong/)
       在那里他提出了一种新的使用QThread的方式，其实也不算是信了，据说qt 4.4就已经有了。那就是QObject::moveToThread。根据QT的asstant的描述，moveToThread的作用是把一个QOject移动到一个线程里面去，那么它到底是什么意思呢。我的理解就是当我们调用QObject的moveToThread方法之后，我们这个派生自QObject的类的代码就会在新的线程里面执行。而那篇文章所说的就是大多数对这个函数产生了误解，人们总是在派生的QThread的类的构造函数里面调用moveToThread（this）以希望把该类的所有函数都在该线程里面执行。这样是错误的。
         今天为了验证这个方法到底有什么用，写了一些代码来做测试。
1、
[cpp][view plain](https://blog.csdn.net/sydnash/article/details/7425947#)[copy](https://blog.csdn.net/sydnash/article/details/7425947#)
- #include <QObject>
- #include <QDebug>
- #include <QThread>
- 
- class MyObject : public QObject {  
-     Q_OBJECT  
- public:  
-         MyObject() {};  
-         ~MyObject() {}  
- 
- public slots:  
- void first() {  
-             qDebug() << QThread::currentThreadId();  
-         }  
- void second() {  
-             qDebug() << QThread::currentThreadId();  
-         }  
- void three() {  
-             qDebug() << QThread::currentThreadId();  
-         }  
- };  
2、mainwindow.cxx
[cpp][view plain](https://blog.csdn.net/sydnash/article/details/7425947#)[copy](https://blog.csdn.net/sydnash/article/details/7425947#)
- #include "mainwindow.h"
- #include <QPushButton>
- #include <QVBoxLayout>
- #include "myobject.h"
- 
- MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {  
-     my = new MyObject;  
-     firstButton = new QPushButton(tr("first"), 0);  
-     connect(firstButton, SIGNAL(clicked()), my, SLOT(first()), Qt::QueuedConnection);  
-     secondButton = new QPushButton(tr("second"), 0);  
-     connect(secondButton, SIGNAL(clicked()), my, SLOT(second()), Qt::QueuedConnection);  
-     threeButton = new QPushButton(tr("three"), 0);  
-     connect(threeButton, SIGNAL(clicked()), my, SLOT(three()), Qt::QueuedConnection);  
-     selfButton = new QPushButton(tr("self"), 0);  
-     connect(selfButton, SIGNAL(clicked()), this, SLOT(onSelfPushed()));  
-     exitButton = new QPushButton(tr("exit"), 0);  
-     connect(exitButton, SIGNAL(clicked()), this, SLOT(onExitPushed()));  
- 
-     QVBoxLayout *layout = new QVBoxLayout;  
-     layout->addWidget(firstButton);  
-     layout->addWidget(secondButton);  
-     layout->addWidget(threeButton);  
-     layout->addWidget(selfButton);  
-     layout->addWidget(exitButton);  
- 
-     QWidget *p = new QWidget;  
-     p->setLayout(layout);  
- 
-     QThread *thread = new QThread;  
-     my->moveToThread(thread);  
- 
- thread->start();  
-     connect(thread, SIGNAL(started()), my, SLOT(first()));  
- 
-     setCentralWidget(p);  
- }  
- 
- MainWindow::~MainWindow() {  
- }  
- 
- void MainWindow::onFirstPushed() {  
-     my->first();  
- }  
- 
- void MainWindow::onSecondPushed() {  
-     my->second();  
- }  
- 
- void MainWindow::onThreePushed() {  
-     my->three();  
- }  
- 
- void MainWindow::onSelfPushed() {  
-     qDebug() << QThread::currentThreadId();  
- }  
- 
- void MainWindow::onExitPushed() {  
-     close();  
- }  
    通过测试，在mainwidow.cxx使用上面的代码的时候，由于my调用了movetothread函数，那么它所有的槽函数都是执行在新开辟的线程里面。
   如果去掉moveToThread函数，那么所有的函数都将执行在gui线程里面。
   同时为了测试connect的第五个参数，在connect的时候可以将Qt::QueuedConnection修改为Qt::DirectConnection，这样所有的槽函数也将在主线程里面执行。
     最后要注意的是，如果上面connect的时候连接的是this的onXXXXXX槽函数再来调用的my的槽函数的话，那么这些槽函数也将执行在onXXXXX槽函数所在的线程，这里是主线程。
     通过上面的测试，我们在使用线程的时候，就可以将一个类派生自QObject，然后实现所有的signal/slot，然后通过调用movetothread函数来使他们执行在新的线程里面，而不是每次都要重新派生QThread，并且派生QThread函数的另外一个不好的地方是只有run函数内部的代码才会执行在新线程里面，相比起来，派生QObject并使用movetothread函数更具有灵活性。
    最后，把讨论中列出的所有的网址列出来哈。
[http://qt-project.org/doc/qt-4.8/thread-basics.html](http://qt-project.org/doc/qt-4.8/thread-basics.html)
[http://labs.qt.nokia.com/2010/06/17/youre-doing-it-wrong/](http://labs.qt.nokia.com/2010/06/17/youre-doing-it-wrong/)
[http://qt-project.org/wiki/Threads_Events_QObjects](http://qt-project.org/wiki/Threads_Events_QObjects)
[http://qt-project.org/wiki/QThreads_general_usage](http://qt-project.org/wiki/QThreads_general_usage)
[http://labs.qt.nokia.com/2006/12/04/threading-without-the-headache/](http://labs.qt.nokia.com/2006/12/04/threading-without-the-headache/)

