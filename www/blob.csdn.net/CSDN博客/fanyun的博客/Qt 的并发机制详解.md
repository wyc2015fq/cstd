# Qt 的并发机制详解 - fanyun的博客 - CSDN博客
2018年03月31日 21:50:53[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：320
   QProcess和QThread提供了两种并发方式。
   QProcess是一个能够非常方便的用于启动和控制其它进程的类。它派生自QObject，可充分利用信号和槽与其它对象交互。
   一个QProcess可以使用start（）函数来启动一个进程。新进程将会成为一个子进程并随着父进程的终止而终止。
   QThread为我们QT项目的异步和同步并发提供了强大工具。
