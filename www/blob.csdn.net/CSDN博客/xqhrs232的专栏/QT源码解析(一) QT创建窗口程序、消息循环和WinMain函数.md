# QT源码解析(一) QT创建窗口程序、消息循环和WinMain函数 - xqhrs232的专栏 - CSDN博客
2016年10月15日 09:36:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：262
原文地址::[http://blog.csdn.net/tingsking18/article/details/4737925](http://blog.csdn.net/tingsking18/article/details/4737925)
相关文章
1、
# [QT源码解析(七）Qt创建窗体的过程](http://blog.csdn.net/tingsking18/article/details/5528666)----[http://blog.csdn.net/tingsking18/article/details/5528666](http://blog.csdn.net/tingsking18/article/details/5528666)
## 版权声明
请尊重原创作品。转载请保持文章完整性，并以超链接形式注明原始作者“[tingsking18](mailto:tingsking@gmail.com)”和[主站点](http://blog.csdn.net/tingsking18/)地址，方便其他朋友提问和指正。
QT源码解析(一) QT创建窗口程序、消息循环和WinMain函数
[QT源码解析(二）深入剖析QT元对象系统和信号槽机制](http://blog.csdn.net/tingsking18/archive/2009/11/12/4800828.aspx)
[QT源码解析(三）深入剖析QT元对象系统和信号槽机制（续）](http://blog.csdn.net/tingsking18/archive/2009/12/12/4991563.aspx)
[QT源码解析(四）剖析Qt的事件机制原理](http://blog.csdn.net/tingsking18/archive/2010/01/04/5127507.aspx)
[QT源码解析(五）QLibrary跨平台调用动态库的实现](http://blog.csdn.net/tingsking18/archive/2010/01/04/5128307.aspx)
[QT源码解析(六）Qt信号槽机制与事件机制的联系](http://blog.csdn.net/tingsking18/archive/2010/01/08/5157811.aspx)
[QT源码解析(七）Qt创建窗体的过程](http://blog.csdn.net/tingsking18/archive/2010/04/26/5528666.aspx)
[QT源码解析(八）Qt是如何处理windows消息的](http://blog.csdn.net/tingsking18/archive/2010/04/26/5528675.aspx)
[QT源码解析(九）解析QDateTime](http://blog.csdn.net/tingsking18/archive/2010/04/26/5528683.aspx)
使用QT也有一段时间了，有的时候需要跟踪代码到QT的源码中去查找问题。在这里我将记录一下我跟踪QT源码学习到的一些知识。
我的开发环境是VC6.0+QT4.3.3。QT已经不为VC6.0提供addin了，所以有的时候我也会使用EclipseCDT来编写代码，因为有了QT for Eclipse的plugin写代码会方便一些。
我们在学习QT的时候，接触的第一个程序就是下面的helloworld程序：
**[cpp]**[view
 plain](http://blog.csdn.net/tingsking18/article/details/4737925#)[copy](http://blog.csdn.net/tingsking18/article/details/4737925#)
- #include <QApplication>
- #include <QPushButton>
- 
- int main(int argc, char *argv[])  
- {  
-     QApplication app(argc, argv);     
-     QPushButton hello("Hello world!");  
-     hello.resize(100, 30);    
-     hello.show();  
- return app.exec();  
- }  
这个程序的作用很多手册和文档都已经讲了，讲的也都很细致，非常不错。
但是喜欢钻研，深入的童鞋也许开始注意了int main(int argc, char *argv[])，这个main函数是标准的main函数，而windows应用程序的入口是winmain函数，而main函数是命令行程序的入口。win下窗口程序都有RegisterClass，和消息循环,QT是如何RegisterClass和创建消息循环的？
下面我们将来一起学习一下QT的源码来解释一下这个main函数和整个窗口程序的创建过程：
设置好路径后，我们先F10一下，看看这个程序到底是从哪里开始运行的。
程序跳到了/winmain/qtmain_win.cpp文件的WinMain函数中，再看这个文件上面的宏定义：#define main qMain
继续看：在WinMain函数中调用了我们自己定义的main函数：int result = main(argc, argv.data());
哇塞，原来如此啊。原来我们写的main函数是假的。哈哈。
再来看一下QT是如何创建窗体和消息循环的
首先我们来到QApplication的构造函数：
QApplication::QApplication(int &argc, char **argv, int _internal)
    : QCoreApplication(*new QApplicationPrivate(argc, argv, GuiClient))
{ Q_D(QApplication); d->construct(); QApplicationPrivate::app_compile_version = _internal;}
很明显，首先调用的是QApplicationPrivate的构造函数。大家注意第三个参数：QApplication::Type type
这事Type类型的定义：enum Type { Tty, GuiClient, GuiServer };
下面是代码注释中对Type类型的解释：
    /enum QApplication::Type
    /value Tty a console application
    /value GuiClient a GUI client application
    /value GuiServer a GUI server application (for Qt for Embedded [Linux](http://lib.csdn.net/base/linux))
当程序运行到hello.show()的时候调用了QWidgetPrivate::create_sys函数。
在这里我们看到调用了类似RegisterClass的函数：QString windowClassName = qt_reg_winclass(q);
这里的q是指向QWidget的指针(我们先忽略掉这里)。
以及包括后面的CreateWindow，ShowWindow等等我们熟悉的WindowsAPI函数
const QString qt_reg_winclass(QWidget *w) 函数的原型是在qapplication_win.cpp中定义的。我们转到qt_reg_winclass函数的实现中。我们就看到了windows的API函数RegisterClass和窗口消息处理函数：wc.lpfnWndProc        = (WNDPROC)QtWndProc;
我们看一下QtWndProc的实现，原来窗口消息都是在这里进行处理的啊！
至于最后一句app.exec(); 调用了QCoreApplication的Exec函数，在这个函数中我们看到了下面创建消息循环的代码
    QEventLoop eventLoop;
    self->d_func()->in_exec = true;
    int returnCode = eventLoop.exec();
在QCoreApplication.cpp中的注释是这样解释的：
    The application will enter
    the event loop when exec() is called. exit() will not return
    until the event loop exits, e.g., when quit() is called.
 到这里，main和WinMain函数到底是怎么回事，以及QT是怎么创建窗口和消息循环的，我们已经非常清楚了。
