# QT 获取系统屏幕分辨率 - xqhrs232的专栏 - CSDN博客
2017年10月11日 13:04:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：219
原文地址::[http://blog.csdn.net/imxiangzi/article/details/52530001](http://blog.csdn.net/imxiangzi/article/details/52530001)
相关文章
1、[在Qt中获取屏幕大小](http://blog.csdn.net/yangluoning/article/details/16116061)----[http://blog.csdn.net/yangluoning/article/details/16116061](http://blog.csdn.net/yangluoning/article/details/16116061)
2、【QT】在Qt中获取屏幕大小----[http://blog.csdn.net/iEearth/article/details/43669821](http://blog.csdn.net/iEearth/article/details/43669821)
1，在Windows下可以使用 GetSystemMetrics(SM_CXSCREEN);GetSystemMetrics(SM_CYSCREEN) 获取。 
2，在Linux下可以使用[XDisplayWidth](http://blog.chinaunix.net/link.php?url=http://www.cuteqt.com/blog/wp-admin/display/image-format-macros.html#DisplayWidth) ；[XDisplayHeight](http://blog.chinaunix.net/link.php?url=http://www.cuteqt.com/blog/wp-admin/display/image-format-macros.html#DisplayHeight) ()获取。
3，在QT中呢？很多人说是 QApplication::desktop()->width();QApplication::desktop()->height(); 这个方法对于单显示器模式当然没有问题。但是对于多显示器，特别是使用了扩展桌面的就会有问题了。今天上午仔细看了QDesktopWidget的帮助，需要使用QApplication::desktop()->screenGeometry();这个函数有好几个重载的版本，意思都一样。该函数返回一个QRect，这个QRect的宽和高就是所在Screen的分辨率。获取方法如下：
void GetScreenInfo()
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //获取可用桌面大小
    QRect deskRect = desktopWidget->availableGeometry();
    //获取设备屏幕大小
    QRect screenRect = desktopWidget->screenGeometry();
    g_nActScreenX = screenRect.width();
    g_nActScreenY = screenRect.height();
    //g_nActScreenX = deskRect.width();
    //g_nActScreenY = deskRect.height();
    //获取系统设置的屏幕个数（屏幕拷贝方式该值为1）
    g_nScreenCount = desktopWidget->screenCount();
}说到这里，顺便标记以下多屏幕设置成拷贝方式时，获取的屏幕的个数是一个，只有设置成扩展时才返回多个。
打印屏幕分辨率和个数信息：
void printscreeninfo()
{
    QDesktopWidget *dwsktopwidget = QApplication::desktop();
    QRect deskrect = dwsktopwidget->availableGeometry();
    QRect screenrect = dwsktopwidget->screenGeometry();
    QDesktopWidget *dwsktopwidget = QApplication::desktop();
    QRect deskrect = dwsktopwidget->availableGeometry();
    QRect screenrect = dwsktopwidget->screenGeometry();
    int scrcount = dwsktopwidget->screenCount();
    qCritical("screenrect.w==%s\n",qPrintable(QString::number(screenrect.width())));
    qCritical("screenrect.h==%s\n",qPrintable(QString::number(screenrect.height())));
    qCritical("deskrect.w==%s\n",qPrintable(QString::number(deskrect.width())));
    qCritical("deskrect.h==%s\n",qPrintable(QString::number(deskrect.height())));
    qCritical("scrcount==%s\n",qPrintable(QString::number(scrcount)));
}

FROM:  http://blog.csdn.net/zhb123168/article/details/6744982

