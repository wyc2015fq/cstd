# 人脸识别实战之用QT做点事（九）：QT小知识点（2） - sinat_31425585的博客 - CSDN博客
2018年08月23日 11:35:51[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：97标签：[qt 																[信号槽																[多线程](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=信号槽&t=blog)](https://so.csdn.net/so/search/s.do?q=qt &t=blog)
个人分类：[QT+opencv](https://blog.csdn.net/sinat_31425585/article/category/7742815)
# **1、信号槽机制**
      如果进行某种操作，例如点击某个按钮（例如：clicked）时，button部件就会发出一个信号，使用connect将这个信号与动作连接起来，这个动作就是槽函数。这种信号与槽函数，对信号进行监听，如果监听到信号，就执行对应监听动作，实际上就是信号与动作的绑定。例如：
```cpp
connect(sender, SIGNAL(signal), receiver, SLOT(slot));
```
   这句代码意思就是：如果sender部件发出signal信号 ，就执行slot动作，这里SIGNAL即信号，SLOT即对应槽函数，这样就建立了信号与槽的对应关系，SINGAL()和SLOT()宏用于把参数转换成字符串。
   信号与槽之间的连接关系可以存在以下几种情况：
   一个信号链接多个槽：
```cpp
connect(slider, SIGNAL(valueChanged(int)),
spinBox, SLOT(setValue(int)));
connect(slider, SIGNAL(valueChanged(int)),
this, SLOT(updateStatusBarIndicator(int)));
```
  多个信号链接一个槽：
```cpp
connect(lcd, SIGNAL(overflow()),
this, SLOT(handleMathError()));
connect(calculator, SIGNAL(divisionByZero()),
this, SLOT(handleMathError()));
```
一个信号链接另外一个信号：
```cpp
connect(lineEdit, SIGNAL(textChanged(const QString &)),
this, SIGNAL(updateRecord(const QString &)));
```
还可以取消信号与槽之间链接：
```cpp
disconnect(lcd, SIGNAL(overflow()),
this, SLOT(handleMathError()));
```
# 2、多线程
## 2.1 线程和进程之间区别
1）进程类似于独立的程序，他们被操作系统直接执行；
2）线程是进程的子集，或者说，一个进程可以包含多个线程；
3）通常，一个进程与其他进程之间是相互独立的，然而进程间共享内存和资源；
      为了实现最大的性能和响应，每个进程创建和执行不同线程可能取决于它的设计方式，另一方面，每个线程将根据进程的需求来执行特定任务。
## 2.2 Qt中的多线程
     QT框架中提供了许多不同技术来处理多线程应用。QThread类被用于处理各种类型多线程功能，他也是QT框架中最强大和最灵活的线程处理方式。此外，QT框架也提供了大量的命名空间、类别和函数来协助各种各样的多线程需求。
1） QThread：这个类别是QT框架中所有threads的基类。他可以子类化来新建一个线程，但是需要你来重写run方法，或者，你可以创建他的一个新实例，然后使用moveToThread函数将任意的QObject类放入一个新线程中。
2）QThreadPool：这个可以用来管理多线程，通过允许现有线程被用于新的任务来降低新线程创建过程带来消耗。每一个Qt应用都包含了一个全局的QThreadPool实例，可以用QThreadPOOL::globalInstance()静态函数来进行访问。这个类别被用于连接QRunnable类别实例来控制、管理和回收一个QT应用中的runnable objects。
3）QRunnable：这个提供另外一种创建线程方式，他是所有qt中runnable objects的基类。不像QThread，QRunnable不是一个QObject 子类。他被用作一段需要执行代码块的接口。你需要新建一个子类，然后重写run函数才能使用QRunnable。
4）QMutex，QMutexLocker，QSempaphore，QReadLocker，QWriteLocker：这些类被用于处理内部线程的同步任务。根据场景不同，这些类可以被用于避免一些问题，例如：线程间计算相互覆盖，多个线程同时去读写一个只能一个时间段处理一个线程的设备等。
5）QtConcurrent：这个命名空间被用于使用更高级API创建多线程应用。他可以使得写多线程函数变得简单，不需要去处理互斥、信号灯和内部线程同步等问题。
6）QFuture，QFutureWatcher，QFutureIterator和QFutureSynchronizer：这些类被用于连接QtConcurrent命名空间，来处理多线程和同步操作结果。
       一般来讲，QT中多线程有两种方式：一种基于QThread，这是一种低层方法，但是他提供了对线程更多的灵活性和可控性，但是需要更多代码和注意细节；第二种方法是基于QtConcurrent 命名空间，这是一种高层方法来创建和运行多个任务。
## 2.3 QT多线程例程
### 2.3.1 moveToThread()使用
首先，构造需要在线程中运行部分：
新建videoprocess类，然后添加信号和槽函数：
videoprocess.h
```cpp
signals:
    void inDisplay(QPixmap pixmap);
    void outDisplay(QPixmap pixmap);
public slots:
    void startVideo();
    void stopVideo();
```
videoprocess.cpp
```cpp
void videoprocessor::startVideo()
{
    using namespace cv;
    VideoCapture camera(0);
    Mat inFrame, outFrame;
    stopped = false;
    while(camera.isOpened() && !stopped)
    {
        camera >> inFrame;
        if(inFrame.empty())
            continue;
        bitwise_not(inFrame, outFrame);
        emit inDisplay(
                    QPixmap::fromImage(
                        QImage(
                            inFrame.data,
                            inFrame.cols,
                            inFrame.rows,
                            inFrame.step,
                            QImage::Format_RGB888)
                        .rgbSwapped()));
        emit outDisplay(
                    QPixmap::fromImage(
                        QImage(
                            outFrame.data,
                            outFrame.cols,
                            outFrame.rows,
                            outFrame.step,
                            QImage::Format_RGB888)
                        .rgbSwapped()));
    }
}
void videoprocessor::stopVideo()
{
    qDebug() << Q_FUNC_INFO;
    stopped = true;
}
```
这个代码关键部分就是，如果我调用startVideo槽函数，就会emit两个信号：inDisplay和outDisplay
然后，修改mainwindow部分
```cpp
processor = new videoprocessor();
    processor->moveToThread(new QThread(this));
    connect(processor->thread(),
                SIGNAL(started()),
                processor,
                SLOT(startVideo()));
    connect(processor->thread(),
                SIGNAL(finished()),
                processor,
                SLOT(deleteLater()));
    connect(processor,
                SIGNAL(inDisplay(QPixmap)),
                ui->inVideo,
                SLOT(setPixmap(QPixmap)));
    connect(processor,
                SIGNAL(outDisplay(QPixmap)),
                ui->outVideo,
                SLOT(setPixmap(QPixmap)));
    processor->thread()->start();
```
这个代码逻辑就是：首先，使用moveToThread函数，将processor放到新建的线程中去；然后将信号与信号槽连接起来：四个connect分别表示：如果线程启动，就启用process的startVideo槽函数；如果线程结束，就启用process的deleteLater槽函数（就是关闭线程）；如果processor发出inDisplay信号，就在inVideo控件上显示QPixmap；如果processor发出outDisplay信号，就在outVideo控件上显示QPixmap；最后，启动线程。
效果如下：
![](https://img-blog.csdn.net/20180825141117749?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 2.3.2  直接重写run()函数
新建threadRun类，并继承QThread类：
  threadRun.h
```cpp
class threadRun : public QThread
{
    Q_OBJECT
public:
    explicit threadRun(QObject *parent = nullptr);
    bool stopped;
signals:
    void inDisplay(QPixmap pixmap);
    void outDisplay(QPixmap pixmap);
public slots:
private:
    void run() override;
};
```
 threadrun.cpp
```cpp
void threadRun::run()
{
    using namespace cv;
    VideoCapture camera(0);
    Mat inFrame, outFrame;
    stopped = false;
    while(camera.isOpened() && !stopped)
    {
        camera >> inFrame;
        if(inFrame.empty())
            continue;
        bitwise_not(inFrame, outFrame);
        emit inDisplay(
                    QPixmap::fromImage(
                        QImage(
                            inFrame.data,
                            inFrame.cols,
                            inFrame.rows,
                            inFrame.step,
                            QImage::Format_RGB888)
                        .rgbSwapped()));
        emit outDisplay(
                    QPixmap::fromImage(
                        QImage(
                            outFrame.data,
                            outFrame.cols,
                            outFrame.rows,
                            outFrame.step,
                            QImage::Format_RGB888)
                        .rgbSwapped()));
    }
}
```
然后就是修改构造函数中内容：
```cpp
processor = new threadRun();
    connect(processor,
                SIGNAL(inDisplay(QPixmap)),
                ui->inVideo,
                SLOT(setPixmap(QPixmap)));
    connect(processor,
                SIGNAL(outDisplay(QPixmap)),
                ui->outVideo,
                SLOT(setPixmap(QPixmap)));
    processor->start();
```
效果如下：
![](https://img-blog.csdn.net/20180825143934863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
表演结束，enjoy！！！！！
参考资料：
 http://devbean.blog.51cto.com/448512/194031
