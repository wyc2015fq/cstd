# Qt入门教程系列之五    事件与事件处理 - DumpDoctorWang的博客 - CSDN博客





2018年11月18日 20:37:08[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1779











### 目录
- [1、事件系统](#1_1)
- [2、事件传递](#2_3)
- [3、事件类型](#3_6)
- [4、事件处理](#4_9)
- [5、事件过滤器](#5_135)
- [6、参考链接](#6_176)




# 1、事件系统

在Qt中，事件是从抽象类QEvent派生的类的实例，它们表示在应用程序中发生的事情或者应用程序需要了解的外部活动的结果。 事件可以由QObject子类的任何实例接收和处理，但它们与Widget相关。比如说，点击鼠标时，会触发QMouseEvent；敲击键盘时，会触发QKeyEvent；窗口大小发生变化时，会触发QResizeEvent。

# 2、事件传递

当事件发生时，Qt通过构造适当的QEvent子类的实例来创建一个事件对象来表示该事件，并通过调用它的event()函数将它传递给QObject（或其子类之一）的特定实例。

一些事件来自窗口系统，如QMouseEvent和QKeyEvent; 一些来自其他来源，如QTimerEvent; 一些来自应用程序本身。

# 3、事件类型

大多数事件类型都有特殊的类，特别是QResizeEvent，QPaintEvent，QMouseEvent，QKeyEvent和QCloseEvent。 每个类都对QEvent进行子类化，并添加特定于事件的函数。 例如，QResizeEvent添加size()和oldSize()以使Widget能够发现其大小的变化。

某些类支持多种实际事件类型。 QMouseEvent支持按下鼠标按钮，双击，移动和其他相关操作。每个事件都有一个在QEvent::Type中定义的类型，通过查询事件的QEvent::Type，就可以知道该事件是从哪个子类实例化的。

# 4、事件处理

直接来个例子，在一个窗口中接收鼠标事件。新建一个项目叫做EventTest，选项都保持默认就好了。然后在mainwindow.h中加入接收鼠标点击、鼠标移动、鼠标释放和鼠标双击的函数，这样mainwindow.h关联的那个“窗口”就可以接收到鼠标事件：

```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

protected:
    // 鼠标点击事件
    void mousePressEvent(QMouseEvent *event);
    // 鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    // 鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *event);
    // 鼠标双击事件
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
```

有注释那三行是我添加的，分别用来接收鼠标按下、鼠标移动和鼠标释放的事件，是不是非常简单呢？类似的，如果要接收键盘按下事件，那么就可以再加一行void keyPressEvent(QKeyEvent *event)，这样就可以接收键盘事件了。还有其他很多事件，也是同理，至于处理相应事件的是什么函数叫啥，很好百度。

为啥在mainwindow.h里面定义上述的三个函数就可以接收鼠标事件了呢？实际上，上面三个函数重写了基类的函数，在基类中，处理鼠标事件就是这几个函数。如果要在自己的窗口类处理相应事件，就需要自己重写对应的处理事件的函数。

接下来，就是在mainwindow.cpp中实现处理鼠标事件的函数。

（在这里悄悄说一下Qt的快捷操作，当一个函数定义好了之后，把光标移动到函数名字中间，按一下Alt+Enter，就可以弹出一个菜单，可以很方便的在mainwindow.cpp添加实现。另外，按住Ctrl件，点击函数名字，可以在定义和实现之间跳转。）
```cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        qDebug()<<"鼠标左键点击";
        break;
    case Qt::RightButton:
        qDebug()<<"鼠标右键点击";
        break;
    case Qt::MidButton:
        qDebug()<<"鼠标中键点击";
        break;
    default:
        break;
    }
    // 记得调用父类的对应函数，把事件传递给父类
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<"Mouse pos:"<<event->pos();
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        qDebug()<<"鼠标左键释放";
        break;
    case Qt::RightButton:
        qDebug()<<"鼠标右键释放";
        break;
    case Qt::MidButton:
        qDebug()<<"鼠标中键释放";
        break;
    default:
        break;
    }
    // 记得调用父类的对应函数，把事件传递给父类
    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        qDebug()<<"鼠标左键双击";
        break;
    case Qt::RightButton:
        qDebug()<<"鼠标右键双击";
        break;
    case Qt::MidButton:
        qDebug()<<"鼠标中键双击";
        break;
    default:
        break;
    }
    // 记得调用父类的对应函数，把事件传递给父类
    QMainWindow::mouseDoubleClickEvent(event);
}
```

按Ctr+R，就会编译运行项目，在打开的窗口中点击任意位置，就能在Qt的“应用程序输出”中看到输出。可以注意到，在出发双击事件时，会先触发一对鼠标点击和鼠标释放，然后是鼠标双击事件，然后又是一对鼠标点击和释放事件，这里要小心处理。

# 5、事件过滤器

顾名思义，对事件过滤器最简单的理解就是用来过滤事件。比如说，在上面的例子中，我希望拦截鼠标单击事件，不拦截鼠标双击事件，就可以使用事件过滤器来完成。首先在mainwindow.h中加入如下代码：

```cpp
protected:
    /**
     * @brief eventFilter 定义一个事件过滤函数（事件过滤器）
     * @param watched 被监控的对象，接收event的对象
     * @param event 发送到watched对象的所有事件
     * @return 返回true，则事件event不再传递，会被拦截下来，否则，继续发送到watched对象。
     */
    bool eventFilter(QObject *watched, QEvent *event);
```

然后在mainwindow.cpp中实现eventFilter：

```cpp
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    // 如果是鼠标双击事件
    if(event->type() == QEvent::MouseButtonDblClick){
        return false;// 返回false，事件event会被继续传递
    }else{
        return true;// 返回true，事件event会被拦截
    }
}
```

然后在mainwindow.cpp的构造函数中“安装”事件过滤器：

```cpp
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->installEventFilter(this);
}
```

此时贬义运行程序，鼠标单击窗口，“应用程序输出”中没有输出。鼠标双击时，会有一个“鼠标*键双击”和“鼠标*键点击”。讲道理，为什么使用了事件过滤器以外，还有一个鼠标单击事件呢？因为鼠标双击事件，好像也被当成了鼠标点击事件。但是，除鼠标双击事件之外，鼠标释放和鼠标单击事件全部都被拦截了。

这里说明一下QObject::installEventFilter(QObject *filterObj)，哪个实例调用installEventFilter，filterObj中的事件过滤器（eventFilter）就会被安装到该实例上。在上例中，就是mainwindow的事件过滤器eventFilter安装到了mainwindow的实例本身。

同样，还有一个函数叫做void QObject::removeEventFilter(QObject *obj)，当一个实例调用该函数时，会移除事件过滤器obj。当obj没有被安装时，该请求会被忽略。
# 6、参考链接
- [The Event System](http://doc.qt.io/qt-5/eventsandfilters.html)。
- [QEvent Class](http://doc.qt.io/qt-5/qevent.html)。





