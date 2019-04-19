# QT截取屏幕的实现 - xqhrs232的专栏 - CSDN博客
2016年10月09日 15:54:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：923
原文地址::[http://blog.csdn.net/pamxy/article/details/9793883](http://blog.csdn.net/pamxy/article/details/9793883)
相关文章
1、[qt实现截取屏幕](http://blog.csdn.net/yyingwei/article/details/3895832)----[http://blog.csdn.net/yyingwei/article/details/3895832](http://blog.csdn.net/yyingwei/article/details/3895832)
2、关于grabwidget的问题----[http://bbs.csdn.net/topics/390643240?page=1](http://bbs.csdn.net/topics/390643240?page=1)
3、[QT实现截屏功能](http://blog.chinaunix.net/uid-20680669-id-324256.html) ----[http://blog.chinaunix.net/uid-20680669-id-324256.html](http://blog.chinaunix.net/uid-20680669-id-324256.html)
# 关于grabwidget的
转自：[http://blog.csdn.net/dansin/article/details/259990](http://blog.csdn.net/dansin/article/details/259990)
[操作系统](http://lib.csdn.net/base/operatingsystem):ARM-[Linux](http://lib.csdn.net/base/linux)
图形用户界面:Qtopia
图形开发库:QT
Qtopia是Trolltech公司面向嵌入式Linux的综合应用平台。Qtopia包含完整的应用层、灵活的用户界面、窗口操作、应用程序启动程序以及开发框架。另外Qtopia基于QT/Embedded的C++类库，拥有统一的应用程序编程接口，支持多任务管理，实用性强。
Qt是Trolltech公司开发的一个多平台的C++图形用户界面应用程序框架，它提供给应用程序开发者建立艺术级的图形用户界面所需的所用功能。Qt是完全面向对象，很容易进行扩展，并且允许真正的组件编程。Qt同时也是流行的Linux桌面环境KDE 的基础。 
本文阐述了一个在Qtopia下截取屏幕的方法，实际上Qtopia和QT都支持帧缓冲技术，我们可以将帧缓冲的数据读出来即为屏幕数据。但是这种方法需要对设备驱动的了解，而且实现起来不那么方便。这里介绍利用QT库中自带的grabWidget与grabWindow函数截取屏幕的方法。
两个函数的原型如下：
QPixmap QPixmap::grabWindow ( WId window, int x=0, int y=0, int w=-1, int h=-1 )
WId表示被截取窗口的ID号；
x，y表示截取范围的起始作标；
w，h分别表示截取范围的宽度和高度，如果为-1，则表示全部截取。
QPixmap QPixmap::grabWidget (  QWidget  * widget, int x=0, int y=0, int w=-1, int h=-1 )
widget表示被截取窗口的画布部分，其余参数同grabWindow
下面这段代码实现了当前整个窗口的截取。
    QPixmap pixmap;   
    //QT库中的图形类，可以处理QT支持的所有图形类型，如JPG，BMP等
    QPixmap pix;
    pix=pixmap.grabWindow(this->winId(),0,0,-1,-1);     
    //this->winId()为获得当前窗口ID
    if(pix.isNull())
    {
        QMessageBox::information(this,"error","grab Screen failed",QMessageBox::Ok);
    }
    else
    {
        if(pix.save( "/tmp/grab.bmp", "BMP" )==false)
        {
            QMessageBox::information(this,"right","save error",QMessageBox::Ok);
        }
        else
            QMessageBox::information(this,"Grab","bitmap saved as /tmp/grab.bmp",QMessageBox::Ok);
    }
将grabWindow改为grabWidget即可实现画布数据的采集。
