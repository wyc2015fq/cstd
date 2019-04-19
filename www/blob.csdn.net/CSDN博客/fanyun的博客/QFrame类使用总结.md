# QFrame类使用总结 - fanyun的博客 - CSDN博客
2016年11月22日 22:25:45[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：14120标签：[Qt](https://so.csdn.net/so/search/s.do?q=Qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/fanyun_01/article/category/6429937)
**QFrame与QWidget的区别：**
QFrame是基本控件的基类，QWidget是QFrame基类，关系如下：
QPushButton,QLabel… -> QFrame ->QWidget
      我们经常会从QFrame或者QWidget继承然后自定义一个复杂的widget，在设置样式表的时候它们就有一个大的区别。 从QWidget继承实现了一个自定义的widget，在设置样式表的时候发现没有生效，发现原来跟QFrame有关。
dialog.h
```cpp
#ifndef DIALOG_H
#define DIALOG_H
 
#include <QDialog>
#include <QFrame>
namespace Ui {
class Dialog;
}
 
class MyWidget : public QWidget {
   Q_OBJECT
public:
    MyWidget(QWidget*parent = 0) : QWidget(parent) {
       this->setStyleSheet("QWidget{background:#ff0000;}QWidget:hover{background:#00ff00;}");
    }
};
 
class MyFrame : public QFrame {
    Q_OBJECT
public:
    MyFrame(QWidget*parent = 0) : QFrame(parent) {
       this->setStyleSheet("QWidget{background:#ff0000;}QWidget:hover{background:#00ff00;}");
    }
};
 
class Dialog : public QDialog
{
   Q_OBJECT
 
public:
    explicitDialog(QWidget *parent = 0);
   ~Dialog();
 
private:
   Ui::Dialog *ui;
};
 
#endif // DIALOG_H
```
dialog.cpp
```cpp
#include "dialog.h"
#include "ui_dialog.h"
 
Dialog::Dialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::Dialog)
{
   ui->setupUi(this);
 
    MyWidget*widget = new MyWidget(this);
   MyFrame *frame = new MyFrame(this);
   widget->setFixedSize(50, 50);
   frame->setFixedSize(50, 50);
   widget->move(0, 0);
   frame->move(0, 50);
}
 
Dialog::~Dialog()
{
   delete ui;
}
```
如下图，发现从QWidget继承过来的MyWidget并没有显示出样式来 
![](https://img-blog.csdn.net/20161122100732838?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
具体原因还不是很清楚，不过下面一段话可能对我们有些帮助： 
*QWidget Supports only the background, background-clip and background-originproperties. 
If you subclass from QWidget, you need to provide a paintEvent for your customQWidget as below:*
```cpp
void CustomWidget::paintEvent(QPaintEvent*)
{
   QStyleOption opt;
   opt.init(this);
   QPainter p(this);
   style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
```
*The above code is a no-operation if thereis no stylesheet set. 
Warning: Make sure you define the Q_OBJECT macro for your custom widget.*
** QFrame窗口操作函数：**
```cpp
QFrame *frame = new QFrame;
    frame->resize(400,566);
    QPixmappixmap("image/frame.png");
    QPalette   palette;
    palette.setBrush(frame->backgroundRole(),QBrush(pixmap));
    frame->setPalette(palette);
    frame->setMask(pixmap.mask());  //可以将图片中透明部分显示为透明的
    frame->setAutoFillBackground(true);
    frame->setWindowTitle("HelloWord");
    frame->setWindowFlags(Qt::FramelessWindowHint);//去掉最小化，关闭按钮
    QLineEdit *lineEdit = new QLineEdit;
    QPushButton*pushButton   = new QPushButton;
    QVBoxLayout*vLayout =new QVBoxLayout;
```
//控制放在布局里
vLayout->addWidget(lineEdit);
vLayout->addWidget(pushButton);
 //布局放在窗口
frame->setLayout(vLayout);
//窗口前置
this->setWindowFlags(Qt::WindowStaysOnTopHint);
 隐藏窗口最大化、最小化、关闭按钮
setWindowFlags(Qt::FramelessWindowHint);
w.setAttribute( Qt::WA_QuitOnClose, true );//关闭时进程也退出
更多窗口样试：
Qt::WindowContextHelpButtonHint 像对话框一样，有个问号和关闭按钮
Qt::CustomizeWindowHint 标题栏也没有按钮也没有在那里出现就站在那里不到，也不能移动和拖到，任务栏右击什么也没有，任务栏窗口名也没有，做流氓软件很好，但是可惜可以从任务管理器里关闭灰色
Qt::WindowTitleHint 也是窗口只有一个关闭按钮
Qt::WindowSystemMenuHint 他妈还是一样只有一个关闭按钮
Qt::WindowCloseButtonHint 他妈还是一样只有一个关闭按钮
Qt::WindowMaximizeButtonHint 一看就知道最小化按钮怎么了。。。原来不可用。。。。
Qt::WindowMinimizeButtonHint 还原按钮不可用。。
Qt::SubWindow 窗口没有按钮但是有标题栏 任务里什么也看不到
Qt::Desktop 没有显示在桌面也没在任务。但是任务管里器里还是有的。。。
Qt::SplashScreen 标题栏也没有按钮也没有在那里出现就站在那里不到，也不能移动和拖到，任务栏右击什么也没有，任务栏窗口名也没有，但是可惜可以从任务管理器里关闭白色
Qt::ToolTip 标题栏也没有 按钮也没有在那里出现就站在那里不到，也不能移动和拖到，任务栏右击什么也没有，任务栏窗口名也没有，但是可惜可以从任务管理器里关闭白色有个好外，顶层窗口一直都是在最上面..
Qt::Tool 有一个小小的关闭按钮，但是好像不能真正的关闭。
窗口全屏：
setWindowState(windowState() |Qt::WindowFullScreen);
