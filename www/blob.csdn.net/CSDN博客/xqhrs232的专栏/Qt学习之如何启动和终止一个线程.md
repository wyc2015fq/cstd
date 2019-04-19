# Qt学习之如何启动和终止一个线程 - xqhrs232的专栏 - CSDN博客
2018年05月24日 11:30:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1030
原文地址::[http://qimo601.iteye.com/blog/1287279](http://qimo601.iteye.com/blog/1287279)
相关文章
1、QT线程池实验研究与分析（QThread与和QThreadPool + QRunnable使用上的区别）----[https://blog.csdn.net/qinpanke/article/details/51705283](https://blog.csdn.net/qinpanke/article/details/51705283)
2、QRunnable中，如何接收tcp连接信息----[https://jingyan.baidu.com/article/dca1fa6f140f54f1a440520b.html](https://jingyan.baidu.com/article/dca1fa6f140f54f1a440520b.html)
今天来看看Qt如何启动一个线程吧，代码就以精通Qt4编程的为例
先来给出每个文件的相关代码然后再加以分析
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- //*************dialog.h**************//
- 
- 
- #ifndef DIALOG_H
- #define DIALOG_H
- #include <QDialog>
- #define MAXSIZE 5  //最大的线程数
- class QDialogButtonBox;  
- class QProgressBar;  
- class QPushButton;  
- class WorkThread;  
- class ThreadDlg : public QDialog  
- {  
-     Q_OBJECT  
- public:  
-     ThreadDlg(QWidget *parent = 0);  
- public slots:  
- void start();     
- void stop();  
- private:  
-     QPushButton *startButton;  
-     QPushButton *quitButton;  
-     QPushButton *stopButton;  
-     QDialogButtonBox *buttonBox;  
-         WorkThread* threadVector[MAXSIZE];  
- };  
- #endif
- //***********end end end************//
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- //***********mainwindow.h************//
- 
- 
- #ifndef WORKTHREAD_H
- #define WORKTHREAD_H
- #include <QThread>
- class WorkThread : public QThread  
- {  
- protected:  
- void run();   //重新实现run()函数
- };  
- 
- #endif
- //***********end end end************//
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- //***********dialog.cpp************//
- 
- 
- #include <QtGui>
- #include "workThread.h"
- #include "dialog.h"
- 
- ThreadDlg::ThreadDlg(QWidget *parent)       
-     : QDialog(parent)  
- {  
-     startButton = new QPushButton(tr("开始"));  
-     quitButton = new QPushButton(tr("退出"));  
-         stopButton = new QPushButton(tr("停止"));  
-         stopButton->setEnabled(false);  
-     buttonBox = new QDialogButtonBox;  
-     buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);  
-         buttonBox->addButton(stopButton, QDialogButtonBox::ActionRole);  
-     buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);  
-     connect(startButton, SIGNAL(clicked()), this, SLOT(start()));  
-     connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));  
-     connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));  
-     QVBoxLayout *mainLayout = new QVBoxLayout;  
-     mainLayout->addWidget(buttonBox);  
-     setLayout(mainLayout);  
-     setWindowTitle(tr("启动线程"));  
- }  
- void ThreadDlg::start()  
- {  
- for(int i=0;i<MAXSIZE;i++)  
-     {  
-         threadVector[i] = new WorkThread();   //创建线程
-     }  
- 
- for(int i=0;i<MAXSIZE;i++)  
-     {  
-         threadVector[i]->start(QThread::LowestPriority);   //启动线程同时设置它的优先级，当然也可以不带，使用默认的优先级
-     }  
-     stopButton->setEnabled(true);  
-     startButton->setEnabled(false);  
- }  
- void ThreadDlg::stop()  
- {  
- for(int i=0;i<MAXSIZE;i++)  
-     {  
-         threadVector[i]->terminate();    //终止线程
-         threadVector[i]->wait();    //阻塞等待
-     }  
-     startButton->setEnabled(true);  
-     stopButton->setEnabled(false);  
- }  
- //***********end end end************//
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- //***********mainwindow.cpp************//
- 
- 
- 
- #include "workThread.h"
- #include "dialog.h"
- #include <QTextEdit>
- #include <QDebug>
- #include <stdio.h>
- void WorkThread::run()  
-  {  
- while(true)  
- for (int n = 0; n < 10;++n) {  
-                     printf("%d\n",n);  //打印输出
-      }  
-  }  
- 
- //***********end end end************//
- 
- //***********main.cpp************//
- 
- 
- #include <QApplication>
- #include <QtCore>
- #include "dialog.h"
- int main(int argc, char *argv[])  
- {  
-     QApplication app(argc, argv);  
-     QTextCodec::setCodecForTr( QTextCodec::codecForName("gb2312"));   
-     ThreadDlg dialog;  
-     dialog.show();  
- return dialog.exec();  
- }  
- //***********end end end************//
以上代码简单，没有必要做过多的讲解，但是其中的“threadVector[i]->terminate(); ”有必要讲解下，terminate()函数的调用便不会立刻终止线程，因为线程的何时终止取决于系统的调度策略，所在在之后又调用了wait()函数是线程阻塞等待直到退出或者超时。
最后加以一点就是在.pro文件中加入一行代码才能成功运行：
CONFIG+=thread
