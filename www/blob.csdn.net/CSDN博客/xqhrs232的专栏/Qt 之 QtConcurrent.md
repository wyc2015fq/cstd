# Qt 之 QtConcurrent - xqhrs232的专栏 - CSDN博客
2018年08月20日 16:52:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1476
原文地址::[https://www.cnblogs.com/xinxue/p/6840315.html](https://www.cnblogs.com/xinxue/p/6840315.html)
文以 Qt 中的 QtConcurrent::run() 函数为例，介绍如何将函数运行在单独的某一个线程中。
# **1  QtConcurrent::run()**
  QtConcurrent 是一个命名空间，它提供了高层次的函数接口 (APIs)，使得编写的程序，可根据当前计算机中实际的 CPU 核数，自动调整运行的线程数目。
  下面是 Qt 中的自带例程 runfunction，对应于安装目录为  D:\Qt\Qt5.8.0\Examples\Qt-5.8\qtconcurrent\runfucntion
## **1.1  .pro 工程文件**
  使用 QtConcurrent 模块，需要在 .pro 中添加： QT += concurrent
QT += concurrent widgets
CONFIG += console
CONFIG -= app_bundle
SOURCES += main.cpp
## **1.2  main.cpp**
 1 #include <QApplication>
 2 #include <QDebug>
 3 #include <QThread>
 4 
 5 #include <QString>
 6 #include <QtConcurrent>
 7 
 8 void func(QString name)
 9 {
10     qDebug() << "Hello" << name << "from" << QThread::currentThread();
11 }
12 
13 int main(int argc, char **argv)
14 {
15     QApplication app(argc, argv);
16 
17     QFuture<void> fut1 = QtConcurrent::run(func, QString("Thread 1"));
18     QFuture<void> fut2 = QtConcurrent::run(func, QString("Thread 2"));
19 
20     fut1.waitForFinished();
21     fut2.waitForFinished();
22 }
  可以看到，使用 QtConcurrent::run() 函数，分别将  func() 运行在两个不同的线程之中，输出的结果如下：
Hello "Thread 2" from QThread(0x3597f0, name = "Thread (pooled)")
Hello "Thread 1" from QThread(0x337720, name = "Thread (pooled)")
  下面是对使用 QtConcurrent::run() 的详细阐释，阅读完 2 和 3，再来看上面的 runfunction 例程，就非常容易理解了。
# **2  普通函数**
## **2.1  将函数运行在某一个线程中**
extern void func();
QFuture<void> future = QtConcurrent::run(func);
  如果要为其指定线程池，可以将线程池的指针作为第一个参数传递进去
extern void func();
QThreadPool pool;
QFuture<void> future = QtConcurrent::run(&pool, func);
## **2.2  向该函数传递参数**
  需要传递的参数，需要跟在函数名之后，依次加入
extern void FuncWithArguments(int arg1, const QString &string);
 int integer = ...;
 QString string = ...;
 QFuture<void> future = QtConcurrent::run(FuncWithArguments,integer,string);
## **2.3  获取该函数的计算结果**
extern QString Func(const QByteArray &input);
QByteArray byte_array = ...;
QFuture<QString> future = QtConcurrent::run(func, byte_array);
...
QString result = future.result();
# **3  成员函数**
  要将类中的成员函数运行在某一个线程中，可将指向该类实例的 引用或指针 作为 QtConcurrent::run 的第一个参数传递进去，
  常量成员函数一般传递 常量引用 (const reference)，而非常量成员函数一般传递 指针 (pointer)
## **3.1  常量成员函数**
   在一个单独的线程中，调用 QByteArray 的常量成员函数 split()，传递给 run() 函数的参数是 bytearray
  QByteArray bytearray = "hello world";
  QFuture<QList<QByteArray> > future = QtConcurrent::run(bytearray, &QByteArray::split, ',');
  ...
  QList<QByteArray> result = future.result();
## **3.2  非常量成员函数**
   在一个单独的线程中，调用 QImage 的非常量成员函数 invertPixels()，传递给 run() 函数的参数是 &image
  QImage image = ...;
  QFuture<void> future = QtConcurrent::run(&image, &QImage::invertPixels, QImage::InvertRgba);
  ...
  future.waitForFinished();
  // At this point, the pixels in 'image' have been inverted
## **参考资料：**
  Qt 助手 | Qt 5.8 | Qt Concurrent | Concurrent Run
