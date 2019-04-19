# QT输出调试信息 - xqhrs232的专栏 - CSDN博客
2017年10月11日 11:15:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：299
原文地址::[http://qsjming.blog.51cto.com/1159640/1078019](http://qsjming.blog.51cto.com/1159640/1078019)
相关文章
1、[Window下Qt中用qDebug()输出调试信息到Console控制台的设置方法](Window%E4%B8%8BQt%E4%B8%AD%E7%94%A8qDebug()%E8%BE%93%E5%87%BA%E8%B0%83%E8%AF%95%E4%BF%A1%E6%81%AF%E5%88%B0Console%E6%8E%A7%E5%88%B6%E5%8F%B0%E7%9A%84%E8%AE%BE%E7%BD%AE%E6%96%B9%E6%B3%95)----[http://blog.csdn.net/yangluoning/article/details/8449572](http://blog.csdn.net/yangluoning/article/details/8449572)
2、Qt输出调试信息 
 ----[http://blog.163.com/wslngcjsdxdr@126/blog/static/16219623020146173186351/](http://blog.163.com/wslngcjsdxdr@126/blog/static/16219623020146173186351/)
在QT中输出调试信息有四个函数，分别是：qDebug、qWarning、qCritical以及qFatal，从字面信息上就可以看出，他们属于不同的等级，由于这四个函数的使用都相似，所以这里只选择qDebug来介绍一下。
一个示例代码如下：
- #include "mainwindow.h"
- #include <QApplication>
- #include <QDebug>
- #include <QString>
- 
- int main(int argc, char *argv[]) 
- { 
-     QApplication a(argc, argv); 
-     MainWindow w; 
-     w.show(); 
- 
-     qDebug("Output debug infomation %d", 1); 
-     QString str("2"); 
-     qDebug("Output debug infomation %s", str.toStdString().data()); 
- 
- return a.exec(); 
- } 
运行结果如下，在应用程序的输出窗口中：
![](http://img1.51cto.com/attachment/201212/103508988.png)
本文出自 “[lilingshui](http://qsjming.blog.51cto.com/)” 博客，请务必保留此出处[http://qsjming.blog.51cto.com/1159640/1078019](http://qsjming.blog.51cto.com/1159640/1078019)

