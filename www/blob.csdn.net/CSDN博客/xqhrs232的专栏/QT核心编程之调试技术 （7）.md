# QT核心编程之调试技术 （7） - xqhrs232的专栏 - CSDN博客
2016年10月15日 09:45:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：260
原文地址::[http://mobile.51cto.com/symbian-270614.htmhttp:/](http://mobile.51cto.com/symbian-270614.htmhttp:/)

Qt应用程序的调试可以通过DDD进行跟踪调试和打印各种调试或警告信息。DDD（Data Display Debugger）是使用gdb调试工具的图形工具，它安装在Linux操作系统中，使用方法可参考DDD的帮助文档。
AD：[51CTO 网+ 第十二期沙龙：大话数据之美_如何用数据驱动用户体验](http://mobile.51cto.com/mobile/mdsa12/)
QT核心编程之调试技术是本节要介绍的内容，QT核心编程我们要分几个部分来介绍，想参考更多内容，请看末尾的编辑推荐进行详细阅读，先来看本篇内容。
Qt应用程序的调试可以通过DDD进行跟踪调试和打印各种调试或警告信息。DDD（Data Display Debugger）是使用gdb调试工具的图形工具，它安装在Linux操作系统中，使用方法可参考DDD的帮助文档。下面说明如何打印各种调试或警告信息
1、命令行参数
当你运行Q应用程序时，你可以指定几个命令行参数来帮助你调试。这几个命令行参数说明如下：
-nograb 应用程序不再捕获鼠标或者键盘。当程序在Linux下运行在gdb调试器中时这个选项是默认的。
-dograb 忽略任何隐含的或明显得-nograb。即使-nograb出现在命令行的最后，-dograb也会超过-nograb生效的。
-sync 在X同步模式下运行应用程序。同步模式强迫X服务器立即执行每一个X客户端的请求，而不使用缓存优化。它使得程序更加容易测试并且通常会更慢。-sync模式只对X11版本的Qt有效。
2、打印警告和调试消息
Qt使用三个全局函数qDebug、qWarning和qFatal来打印警告和调试信息到标准错误输出stderr（它在缺省情况下为显示屏，也可指定为文件）。 这三个函数说明如下：
qDebug()用来打印调试信息，在调试版本中输出信息，在发布版本中，函数将不起作用。
qWarning()用来在程序发生错误时打印警告信息。
qFatal()用来打印致命错误消息并且退出。
这些函数的Qt实现在Unix/X11下把文本打印到标准错误输出（stderr），在Windows下会打印到调试器。你可以通过安装一个消息处理器，qInstallMsgHandler()来接收这些函数。
因为这3个函数的实现类似，这里只分析函数qDebug，qDebug函数的参数格式与函数printf类似，打印格式化字符串。qDebug函数列出如下（在src/tools/qglobal.cpp中）：
- static QtMsgHandler handler = 0;  //指向用户定义的打印输出函数的句柄  
- static const int QT_BUFFER_LENGTH = 8196;　//内部buffer长度  
-  void qDebug( const char *msg, ... )　//msg格式化的需要打印的字符串  
- {  
-     char buf[QT_BUFFER_LENGTH];  
-     va_list ap;  
-     va_start( ap, msg );//使用可变的参数链表  
- #if defined(QT_VSNPRINTF)   
-    QT_VSNPRINTF( buf, QT_BUFFER_LENGTH, msg, ap );  
- #else    vsprintf( buf, msg, ap );　//将需要打印的信息放入到buf中  
- #endif    va_end( ap );  
-     if ( handler ) {    //如果用户指定的输出函数存在，使用它来输出信息  
-  (*handler)( QtDebugMsg, buf );   
-    } else {  
- #if defined(Q_CC_MWERKS)  
-         mac_default_handler(buf); //mac系统下的缺省输出函数  
- #elif defined(Q_OS_TEMP)  
-  QString fstr( buf );  
-  OutputDebugString( (fstr + "\n").ucs2() );  
- #else fprintf( stderr, "%s\n", buf );  // 输出到stderr#endif   
-    }  
- } 
在src/tools/qglobal.h中定义了QtMsgHandler的函数类型，并将函数qInstallMsgHandler定义为从动态库中输出函数名。这两个定义列出如下：
typedef void (*QtMsgHandler)(QtMsgType, const char *);// Q_EXPORT表示动态库中输出这个函数名Q_EXPORT QtMsgHandler qInstallMsgHandler( QtMsgHandler );
函数qInstallMsgHandler被用户用来定义一个安装处理函数，并返回以前定义的消息处理函数的指针。在一个应用程序中只能定义一个消息处理函数。恢复以前的消息处理函数时，调用qInstallMsgHandler(0)。函数列出如下（在src/tools/qglobal.cpp中）：
- QtMsgHandler qInstallMsgHandler( QtMsgHandler h ){   
-    QtMsgHandler old = handler;  
- hhandler = h;   
-    return old;  
- } 
示例：应用qInstallMsgHandler
下面的例子说明如果在一个应用程序中安装自己的程序运行信息输出函数。这个例子先定义了信息输出函数myMessageOutput，然后，在程序的main函数中安装了信息输出函数。当这个应用函数运行时，就会使用函数myMessageOutput输出运行信息。代码如下：
- #include <qapplication.h>
- #include <stdio.h>
- #include <stdlib.h>
- void myMessageOutput( QtMsgType type, const char *msg )//定义信息输出函数{  
-     switch ( type ) {  
-   case QtDebugMsg: //输出调试信息  
-       fprintf( stderr, "Debug: %s\n", msg );  
-       break;  
-   case QtWarningMsg:　//输出警告信息  
-       fprintf( stderr, "Warning: %s\n", msg );  
-       break;  
-   case QtFatalMsg:　//输出致命信息   
-      fprintf( stderr, "Fatal: %s\n", msg );  
-       abort(); //中断运行，退出程序   
-    }  
- }   
- int main( int argc, char **argv ){   
-    qInstallMsgHandler( myMessageOutput );　//安装信息输出函数  
-     QApplication a( argc, argv );   
-    ...    
-   return a.exec();  
- } 
还有另外两个打印对象信息的调试函数QObject::dumpObjectTree()和QObject::dumpObjectInfo()。它们只在程序调试版本下，输出信息，在发布版本中，这两个函数不起作用。函数QObject::dumpObjectInfo()打印一个对象信号连接等方面的信息。函数QObject::dumpObjectTree()打印出子对象树。
3、调试宏
在程序运行中还常使用宏Q_ASSERT和Q_CHECK_PTR来输出信息，这两个宏说明如下：
（1）Q_ASSERT(b)中的b是一个布尔表达式，当b是FALSE的时候，打印出类似的警告信息："ASSERT:‘b’ in file file.cpp (234)"。
（2）Q_CHECK_PTR(p)中的p是一个指针。如果p是空的话，打印出类似的警告信息："In file file.cpp, line 234: Out of memory"。
宏Q_ASSERT实质上是调用函数qFatal或qWarning输出信息，列出如下（在src/tools/qglobal.h中）：
- #if !defined(Q_ASSERT)  
- #  if defined(QT_CHECK_STATE)  
- #  if defined(QT_FATAL_ASSERT)  
- #  define Q_ASSERT(x)   //打印x，文件名，在程序源代码中的行号  
- #   else  
- #  define Q_ASSERT(x)    
- #    endif  
- #  else  
- #    define Q_ASSERT(x)  
- #  endif#endif  
- 
宏Q_CHECK_PTR实质上调用函数qWarning输出信息，宏定义Q_CHECK_PTR列出如下（在src/tools/qglobal.h中）：
- #if defined(QT_CHECK_NULL)  
- #  define Q_CHECK_PTR(p) (qt_check_pointer#else#  define Q_CHECK_PTR(p)  
- #endif Q_EXPORT bool qt_check_pointer( bool c, const char *, int );  
函数qt_check_pointer实现信息输出操作，函数列出如下（在src/tools/qglobal.cpp中）：
- bool qt_check_pointer( bool c, const char *n, int l ){    if ( c ) qWarning( "In file %s, line %d: Out of memory", n, l );   
-    return TRUE;} 
示例2：运行宏Q_ASSERT和Q_ASSERT
宏Q_ASSERT和Q_ASSERT常用来检测程序错误，下面例子使用了这两个宏：
- char *alloc( int size ){   
-    Q_ASSERT( size > 0 ); //如果size > 0表达式不成立，打印警告信息  
-     char *p = new char[size];  
-     Q_CHECK_PTR( p );　//如果指针p为空，打印警告信息  
-     return p;  
- } 
Qt基于不同的调试标记打印不同类型的警告信息。Qt使用了下面的宏定义说明了不同的调试标记（在src/tools/qglobal.h中）：
QT_CHECK_STATE：检测一致的/期望的对象状态
QT_CHECK_RANGE：检测变量范围错误
QT_CHECK_NULL：检测危险的空指针
QT_CHECK_MATH：检测危险的数学，比如被0除
QT_NO_CHECK：关闭所有的QT_CHECK_...标记
QT_DEBUG：使调试代码生效
QT_NO_DEBUG：关闭QT_DEBUG标记
默认情况下，QT_DEBUG和所有的QT_CHECK标记都是打开的。如果要关闭QT_DEBUG，请定义QT_NO_DEBUG。如果要关闭QT_CHECK标记，请定义QT_NO_CHECK。
示例3： 打印不同类型的警告信息
下面的例子根据不同的宏定义打印不同类型的警告信息。代码如下：
- void f( char *p, int i ){  
- #if defined(QT_CHECK_NULL) //检测危险的空指针  
-       if ( p == 0 )          qWarning( "f: Null pointer not allowed" );  
- #endif #if defined(QT_CHECK_RANGE) //检测变量范围错误   
-      if ( i <0 )  
-           qWarning( "f: The index cannot be negative" );  
- #endif} 
小结：QT核心编程之调试技术的内容介绍完了，需要本文能对你有所帮助，需要更多资料的话，请参考编辑推荐。
【编辑推荐】
- [QT核心编程之Qt模板库（1）](http://mobile.51cto.com/symbian-270573.htm)
- [QT核心编程之集合类 （2）](http://mobile.51cto.com/symbian-270583.htm)
- [QT核心编程之Qt线程 （3）](http://mobile.51cto.com/symbian-270589.htm)
- [QT核心编程之鼠标拖放 （4）](http://mobile.51cto.com/symbian-270600.htm)
- [QT核心编程之键盘焦点 （5）](http://mobile.51cto.com/symbian-270605.htm)
- [QT核心编程之会话管理 （6）](http://mobile.51cto.com/symbian-270610.htm)
//======================================================================================
//备注::
还有另外两个打印对象信息的调试函数QObject::dumpObjectTree()和QObject::dumpObjectInfo()。它们只在程序调试版本下，输出信息，在发布版本中，这两个函数不起作用。函数QObject::dumpObjectInfo()打印一个对象信号连接等方面的信息。函数QObject::dumpObjectTree()打印出子对象树。
