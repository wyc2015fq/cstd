# QT窗口操作方法总结 - fanyun的博客 - CSDN博客
2017年11月30日 20:55:56[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：216
1、Qt主窗口没有最小化，最大化按钮且最大化显示 
```cpp
int main(int argc, char *argv[]) 
{ 
QApplication a(argc, argv); 
TestQtForWinCE w; 
w.setWindowFlags(w.windowFlags()& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint); 
w.showMaximized(); 
return a.exec(); 
}
```
这里的“&~”是取反以后再按位与的意思，下面的“|”是按位或的意思
//同时禁止最大化最小化按钮
Qt::WindowMinMaxButtonsHint
//也禁止关闭
w.setWindowFlags(w.windowFlags() &~ (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
2、Qt全屏显示函数        
(1).window.showFullScreen()//此方法只对顶级窗口有效，对子窗口无效
(2).yourwidget->setWindowFlags(Qt::window | Qt::FramelessWindowHint); 
   （第一个Qt::window表示此widget是窗口类型，第二个参数使用无框架就是没有标题，状态栏和边框）
Qt最大化显示函数：         window.showMaximized()
Qt最小化显示函数 ：       window.showMinimized()
Qt固定尺寸显示函数：         window.resize(x,y)
3、子窗口全屏显示：
        QT中窗口部件QWidget成员函数showFullScreen();是用于将窗口部件全屏显示，但是他只对窗口模式的部件有用。子窗口的特征是 Qt::SubWindow，不是独立的窗口。因此对其调用showFullScreen（）无效。通过对对子窗口调 用：setWindowFlags（Qt::Dialog）；或setWindowFlags（Qt::Window）；将其设为窗口模式后，即可调用 showFullScreen();进行全屏显示了。
       如果要还原为普通窗口，调用setWindowFlags(Qt::subwindow)将子窗口设置为非顶级窗口，再调用showNormal()还原子窗口显示。
       相对于子窗口的全屏显示方法，不得不提的是子窗口退出全屏的方法。MFC中，子窗口退出全屏的方法简单直接，直接调用SetWindowPos（）函数将子窗口显示到预定的位置即可。QT中，直接使用move和resize对窗口定位是没有效果的。需要先将子窗口设位非窗口模式。之后再将窗口移到理想位置。 即先调setWindowFlags（Qt::Dialog）；或者setWindowFlags（Qt::SubWindow）；在调用move和resize函数。有人提到退出全屏时，调用showNormal（），再调setWindowFlags（Qt::SubWindow）。
       这有两个问题：1）showNormal（）也是只对顶级窗口有效。2）可以考虑先调用showNormal（）将窗口设为原来大小，在将窗口设为子窗口模式。但这样窗口回到父窗口之中也不会显示原来的大小。
       获取屏幕宽度和高度：
```cpp
QApplication::desktop()->width()  ;
QApplication::desktop()->height() ;
```
即得到屏幕分辨率,如1024*768,1920*968
4、Qt窗口居中显示
(1).构造函数时调用
```cpp
#include <QDesktopWidget>
QDesktopWidget* desktop = QApplication::desktop(); 
move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
```
(2).在调用show()函数后调用move()函数,将窗口移动到屏幕中央。
```cpp
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.move ((QApplication::desktop()->width() - w.width())/2,(QApplication::desktop()->height() - w.height())/2);
    return a.exec();
}
```
5.QMainwindow中将多个dockwidget叠加，并将叠加中的一个dockwidget置顶
```cpp
QMainWindow::tabifiedDockWidgets(dock1,dock2); // 叠加两个dockwidgets  
DockWidget::raise(); // 将当前dockwidget置顶
```
