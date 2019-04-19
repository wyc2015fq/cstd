# Qt multi screen Support - xqhrs232的专栏 - CSDN博客
2017年01月12日 11:43:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：492
原文地址::[https://www.e-consystems.com/blog/linux-android/?p=562](https://www.e-consystems.com/blog/linux-android/?p=562)
Published on January 12, 2015
Introduction:
This article discusses how to use a [Qt
 application](https://www.e-consystems.com/embedded-applications-development/qt-application-development.asp) in a multi-head system and to display windows across the monitors.
What is QdesktopWidget?
The QDesktopWidget class provides access to screen information on multi-head systems. QDesktopWidget class provides information about the user’s desktop, such as its total size, number of screens, the geometry of each screen, and whether they are configured
 as separate desktops or a single virtual desktop.
Widgets provided by Qt use this class to place tooltips, menus and dialog boxes on the correct screen for their parent or application widgets. Applications can use this class to obtain information that can be used to save window positions, or to place child
 widgets and dialogs on one particular screen.
How to start a window in the secondary screen?
As a first step, it is required to get the root window using QApplication::desktop(). With the instance returned by QApplication::desktop(), we can get the number of screens available using  screenCount() and the geometry of each screen using screenGeometry().
Once the above steps are performed, we can move the window to the secondary screen by getting the geometry of the secondary screen and move using the move() function.
How to save the positions of the window ?
QRect rect = widget->screenGeometry(1);
We can use screenGeometry()for getting the geometry of the screen whose positions are to be saved and restored.
The screen geometry can be saved and restored from a simple ini file using QSettings.
Sample Application:
#include <…>
int main(int argc, char *argv[])
{
QApplication a(argc, argv);
MainWindow w1;
MainWindow w2;
QDesktopWidget *widget = QApplication::desktop();
qDebug() << “Number of screens:” << widget->screenCount();
for (int i = 0; i < widget->screenCount(); i++) {
qDebug() << “Geometry:” << widget->screenGeometry(i);
}
// Get geometry of Secondary screen
QRect rect = widget->screenGeometry(1);
qDebug() << “Move to:” << rect.width() << “:” << rect.y();
w1.show();
//Move w2 to Secondary Screen
w2.move(rect.width(), rect.y());
w2.show();
return a.exec();
}
# References
1. [http://doc.qt.io/qt-5/qdesktopwidget.html](http://doc.qt.io/qt-5/qdesktopwidget.html)
For any technical support or queries feel free to write to us at [sales@e-consystems.com](mailto:sales@e-consystems.com)
[Tweet](https://twitter.com/share)

