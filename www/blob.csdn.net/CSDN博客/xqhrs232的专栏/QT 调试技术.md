# QT 调试技术 - xqhrs232的专栏 - CSDN博客
2016年10月14日 22:42:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：222
原文地址::[http://www.cnblogs.com/rogerer/archive/2013/01/16/2863130.html](http://www.cnblogs.com/rogerer/archive/2013/01/16/2863130.html)
相关文章
1、
## QT 调试技术
 ----[http://blog.sina.com.cn/s/blog_550405a10100roq2.html](http://blog.sina.com.cn/s/blog_550405a10100roq2.html)
下面提供一些有用的提示来帮助你调试基于QT开发的应用程序。
## 调试配置
[安装配置qt](http://doc.qt.digia.com/qt/installation.html)时，要确保包含调试选项。在一些平台上，在调试模式下编译qt将导致应用程序比预想的要大。
## 带/不带框架的调试
　　有关调试库和框架的东西创建在developer.apple.com。[Apple Technical
 Note TN2124](http://developer.apple.com/technotes/tn2004/tn2124.html#SECDEBUGLIB)
　　在编译Qt的时候，默认要编译框架。在结果中，可以找到发行版和调试版（如QtCore和QtCore_debug)。如果通过-no-framework参数来编译Qt，将为每个Qt库编译动态库（如libQtCore.4.dylib和libQtCore_debug.4.dylib）。
　　在连接时使用框架或者不使用，将发生什么事情。目前，没有找到令人信服的理由来确认此事。
使用框架
由于发行版和调试版的库在框架中，应用程序自然靠框架连接。然后在调试器中运行，通过设置DYLD_IMAGE_SUFFIX来确定是运行发现版还是调试版。默认运行发行版。如果设置了DYLD_IMAGE_SUFFIX=debug，就运行调试版。
### 不使用框架
　　当通过调试配置要求qmake生成Makefile时，qmake将靠_debug版本的库来连接，并为应用程序生成调试标示符。然后出将在GDB模式下运行，和其他平台运行GDB一样，程序员可以跟踪到Qt内部。
### 调试符号和大小
　　GCC生成的调试符号将占用相当大的空间。然而，通过Xcode 2.3正式版使用Dwarf符号将占用数量更小的控件。选择-dwarf-2选项配置脚本，在配置Qt时可以激活这个功能。
　　默认不支持，因为之前版本的Xcode没有和编译器标示一起运行来实现这个功能。Mac OS X 10.5将默认设置dwarf-2符号。
　　dwarf-2符号包含相关源码，因此最后的调试应用程序会比发行版编译大一些。
## Qt认识的命令行选项
　　在运行Qt应用程序时，需要添加几个帮助调试的命令行选项。它们将被QApplication识别：
-nograb：应用程序不能独占鼠标或者键盘。在Linux的gdb调试器中，这个选项默认生效。
-dograb：忽略任何隐式或显示配置-nograb。即使设置了-nograb，-dograb也强制生效。
　　　 -sync：在X同步模式运行应用程序。同步模式强制X服务器首先不使用缓存立即完成每个X客户端的请求。这将使用程序更容易调试和慢一点。这个选择仅对X11版本的Qt有效。
## 警告和调试消息
　　Qt自带4个向外写警告和调试文本的方法。可以把它们使用在如下目的：
- qDebug()：用于写自定调试信息的输出；
- qWarning()：用于报告程序中的警告和可恢复的错误；
- qCritical()：用于写关键错误信息和报告系统错误；
- qFatal()：用于退出前简要地描述致命错误消息。
　　如果包含<QtDebug>头文件，qDebug()可以当做输出流来使用。例如：
```
qDebug() << "Widget" << widget << "at position" << widget->pos();
```
　　在Unix/X11 and Mac OS X平台，Qt实现了将错误信息输出到stderr设备。在windows中，如果是一个控制台程序，这个信息将发送给控制台；否则就发送给调试器。你可以通过 使用[qInstallMsgHandler](http://doc.qt.digia.com/qt/qtglobal.html#qInstallMsgHandler)()安装一个消息管理者来接管那些函数。
　　如果设置了QT_FATAL_WARNINGS环境变量，打印完警告信息后，qWarning就退出。这让获取向后跟踪更方便了。
　　qDebug和qWarning都是调试工具。它们都可以通过QT_NO_DEBUG_OUTPUT和QT_NO_WARNING_OUTPUT取消调试。
　　当程序表现得十分奇怪时，QObject::dumpObjectTree()和QObject::dumpObjectInfo()调试方法将十分有用。可以使用对象名，也可以不用。
## 为qDebug()提供流操作符号
　　你可以通过qDebug()为你的类实现流操作符号。这个类实现的流是QDebug。在QDebug中你需要知道的方法是space()和nospace()。它们都返回一个debug流；它们之间的区别是十分在每条记录中插入一个空格。如下是一个描绘2D坐标的类的例子：
```
QDebug operator<<(QDebug dbg, const Coordinate &c)
 {
     dbg.nospace() << "(" << c.x() << ", " << c.y() << ")";
     return dbg.space();
 }
```
　　在Creating Custom Qt Types 文档中，用源对象系统集成自定义类型被掩盖地更深。
## 调试宏
　　头文件QtGlobal包含一些宏定义和预定义。
　　3个重要的宏定义：
　　Q_ASSERT(cond)：cond是一个布尔表达式，如果cond是false，写警告信息：ASSERT:'cond' in file xyz.cpp,line 234，并退出。
　　Q_ASSERT_X(cond, where, what)：cond是一个布尔表达式，where是位置，what是消息，如果cond是false，写警告信息：ASSERT
 failure in where: 'what', file xyz.cpp, line 234，并退出。
　　Q_CHECK_PTR(ptr)：ptr是一个指针。如果ptr是0，就写警告信息：In file xyz.cpp, line 234: Out of memory，并退出。
　　这些宏对诊断程序错误非常有用。例如：
```
char *alloc(int size)
 {
     Q_ASSERT(size > 0);
     char *ptr = new char[size];
     Q_CHECK_PTR(ptr);
     return ptr;
 }
```
　　如果编译过程中定义了QT_NO_DEBUG，Q_ASSERT(), Q_ASSERT_X(), and Q_CHECK_PTR()将什么也没有。基于这个原因，宏的参数不应该带有副加功能。下面是一个使用Q_CHECK_PTR不正确的例子：
```
char *alloc(int size)
 {
     char *ptr;
     Q_CHECK_PTR(ptr = new char[size]);  // WRONG
     return ptr;
 }
```
　　如果这段代码通过定义QT_NO_DEBUG来编译，Q_CHECK_PTR中的表达式将不被执行，alloc将返回一个未初始化的指针。
　　Qt库包含成百个错误检查。当程序错误发生时，它将被打印出来。因此，我们建议你在开发基于Qt的软件时，使用Qt的调试版本。
## 常见问题(bugs)
　　有个常见的错误需要在这里提到的是：如果在类定义中包含Q_OBJECT宏和运行源对象编译器（moc），但是忘记将moc生成的源代码连接到你要执行的对象代码中，你将得到混乱不堪的错误消息。任何与vtbl, _vtbl, __vtbl或者相关的错误都是由这个问题导致的。
参考：
1. [Debugging Techniques](http://doc.qt.digia.com/qt/debug.html)
2. [轻松实现Qt日志输出到文件](http://www.thisisqt.com/forum/viewthread.php?tid=90)
