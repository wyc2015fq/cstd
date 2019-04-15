# Qt深入浅出（七）QMainWindow

2018年02月24日 00:27:57 [吓人的猿](https://me.csdn.net/qq769651718) 阅读数：1521



# 窗口类QMainWindow

​	普通的桌面应用程序有个共同的特性，有菜单栏、工具栏、状态栏、中央窗口等部件。菜单栏其实可以看成是一个窗口，菜单栏中的每一个菜单也可以看成一个窗口，每个部件基本都可以认为是一个窗口。那么这些典型的桌面应用可以认为是一些QWidget的组合，通过QWidget派生的方式也确实能够完成这样的窗口。

​	但是如果每次都自己去设计，重复这些工作，想想都有些麻烦，于是Qt帮我们实现这样的窗口，叫做QMainWindow，QMainWindow已经布置好菜单栏、工具栏、状态栏等窗口，我们只需要懂得如何去应用就行了。

- QMainWindow的布局



![img](https://img-blog.csdn.net/20180224002659221)

*注：MenuBar是菜单栏，toolbars是工具栏，Dock Widgets是停口窗口，Central Widget是中央窗口，Status Bar是状态栏*





## 1 创建一个QMainWindow工程

1. 在 Qt Creator 中,点击菜单”文件”

2. 依次选择 “新建文件或项目->application->Qt widget application” 再点击 Choose…按钮

3. 然后填写项目名称点击下一步, 到配置工具链窗口,再点击下一步.

4. 到如图所示的窗口, 选择基类为 QMainWindow, 不勾选”创建界面”（其实也可以勾选）, 如图

   ![img](https://img-blog.csdn.net/20180224002722802)

5. 再点击下一步, 到版本控制窗口,然后再点击完成，那么我们的 QMainWindow 的一个工程就创建好了。





## 2 菜单栏应用

### 2.1 添加菜单栏

​	在Qt中菜单栏类是QMenuBar，它也是继承至QWidget，菜单栏是一个长条状窗口，可以是横向也可以是纵向的，菜单栏上可以布置菜单。

- 通过QMainWindow::MenuBar() 来获取一个QMenuBar对象的指针, 如果该对象不存在, 就新建一个对象。这个函数只能创建一个菜单栏，函数原型如下：

```cpp
QMenuBar *QMainWindow::menuBar()
```

​	例如：

```cpp
#include "mainwindow.h"
#include <QMenubar>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QMenuBar* menuBar = this->menuBar();
}
```





- 也可以通过new一个QMenuBar对象，然后通过QMainWindow::setMenuBar来设置主窗口的菜单栏。

```cpp
void QMainWindow::setMenuBar(QMenuBar *menuBar)
```

​	例如：

```cpp
#include "mainwindow.h"
#include <QMenubar>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QMenuBar* menuBar = new QMenuBar;
    this->setMenuBar(menuBar);
}
```







### 2.2 添加菜单

​	创建完菜单栏之后，就可以在菜单栏上面布置菜单了，菜单类叫作QMenu，QMenu也是继承至QWidget。

- QMenuBar对象中添加QMenu对象可以使用函数：

```cpp
QAction *QMenuBar::addMenu(QMenu *menu)   //添加一个已经创建好的QMenu对象
QMenu *QMenuBar::addMenu(const QString &title) //给定菜单标题，自动创建QMenu对象
```



例如：

```cpp
#include "mainwindow.h"
#include <QMenubar>
#include <QMenu>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenuBar* menuBar = this->menuBar();
    QMenu *fileMenu = new QMenu("文件(&F)", menuBar);
    menuBar->addMenu(fileMenu);
      
    QMenu *editMenu = menuBar->addMenu("编辑(&E)");
    
}
```







### 2.3 添加动作

​	菜单中的诸如打开、保存、关闭等，都是一个个动作，在Qt里面对应的类是QAction，它不是窗口类，它是继承至QObject。可以使用QMenu::addAction来添加动作，QMenu::addAction有多个重载的函数，都是返回一个新建的QAction对象。

```cpp
QAction* addAction(const QString &text)
QAction* addAction(const QIcon &icon, const QString &text)
QAction* addAction(const QString &text, const QObject *receiver, const char *member, const QKeySequence &shortcut = 0)
QAction* addAction(const QIcon &icon, const QString &text, const QObject *receiver, const char *member, const QKeySequence &shortcut = 0)
```







- 添加一个最简单的动作

```cpp
#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QKeySequence>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(600, 480);
    QMenuBar * menuBar = this->menuBar();
    QMenu *  fileMenu = new QMenu("文件(&F)", menuBar);
    menuBar->addMenu(fileMenu);
    fileMenu->addAction("close");
}
```

- 添加一个带图标的动作

```cpp
fileMenu->addAction(QIcon("E:\\close.png"), "close");
```

​	QIcon是图标类，没有继承任何基类，它的作用就是将图片加载到Qt应用中来，可以当做图标来使用。



- 添加一个与当前窗口close()槽函数链接的QAction

```cpp
fileMenu->addAction("close", this, SLOT(close()));
```



- 添加一个与当前窗口close()槽函数链接的QAction,并且设置一个快捷键

```cpp
fileMenu->addAction("close", this, SLOT(close()), QKeySequence("Ctrl+Q"));
```

​	QKeySequence是快捷键类，没有继承任何基类，就是用来设置快捷方式。

   

 

- 添加一个带图标与当前窗口close()槽函数链接的QAction,并且设置一个快捷键

```cpp
fileMenu->addAction(QIcon("E:\close.png"), "close",
                   this, SLOT(close()), QKeySequence("Ctrl+Q"));
```



- 设置QAction为可点选状态

```cpp
void setCheckable(bool)
```



### 2.4 添加分隔符

​	分隔符可以起到修饰的作用，使用QMenu::addSeparator()添加分割符号。

```cpp
QAction *QMenu::addSeparator()
```





### 2.5 添加子菜单

​	在菜单中添加一个子菜单,调用QMenu::addMenu()。

```cpp
QAction *QMenu::addMenu(QMenu *menu)     //添加一个已经创建好的QMenu对象
QMenu *QMenu::addMenu(const QString &title) //给定菜单标题，自动创建QMenu对象
```







## 3 工具栏应用

### 3.1 添加工具栏

​	工具栏类是QToolbar，QToolbar对象中可以布置一些QAction动作，QToolbar继承至QWidget，主窗口可以使用QMainWindow::addToolBar来添加工具栏。

```cpp
QToolBar *QMainWindow::addToolBar(const QString &title) //创建一个窗口标题为title的工具栏
void QMainWindow::addToolBar(QToolBar *toolbar) //添加一个已经存在的toolbar对象
void QMainWindow::addToolBar(Qt::ToolBarArea area, QToolBar *toolbar)//添加一个已经存在的toolbar对象到指定区域，Qt::ToolBarArea是工具栏位置的枚举类型。
```

​	例如：

```cpp
#include "mainwindow.h"
#include <QToolBar>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->addToolBar("tool bar 1");
    QToolBar* toolBar = new QToolBar("tool bar 2", this);
    this->addToolBar(Qt::LeftToolBarArea, toolBar); //添加一个工具栏到左边区域。
}
​
```







### 3.2 添加动作

​	工具栏上面也能添加动作QAction对象，使用QToolBar::addAction。

```cpp
QAction *QToolBar::addAction(const QString &text)
QAction *QToolBar::addAction(const QIcon &icon, const QString &text)
QAction *QToolBar::addAction(const QString &text, const QObject *receiver, const char *member)
QAction *QToolBar::addAction(const QIcon &icon, const QString &text, const QObject *receiver, const char *member)
```





```cpp
#include "mainwindow.h"
#include <QToolBar>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(600, 480);
    QToolBar* toolBar = this->addToolBar("tool bar");
    /*在QToolBar中添加一个close动作*/
    toolBar->addAction("close");
​
    //添加一个带图标QAction
    toolBar->addAction(QIcon("E:\\close.png"), "close");
​
    //添加一个与当前窗口close()槽函数链接的QAction
    toolBar->addAction("close", this, SLOT(close()));
​
    //添加一个带图标与当前窗口close()槽函数链接的QAction
    toolBar->addAction(QIcon("E:\\close.png"), "close",
                   this, SLOT(close()));
}
```

​	QAction对象可以复用，那么可以把菜单上面的动作对象拿下来使用。一般的桌面型应用菜单跟工具栏的动作往往是共用的。





## 4 状态栏应用

​	Qt中的状态栏类是QStatusBar，也是继承至QWidget，状态栏用来显示程序的运行状态，比如“就绪、编辑中”。

### 4.1 创建状态栏



- QMainWindow中创建状态栏可以使用QMainWindow::statusBar

```cpp
QStatusBar *QMainWindow::statusBar() const
```

​	如果QStatusBar对象不存在，就新建一个。如果存在就返回QStatusBar对象。用这个函数也只能创建一个

QStatusBar对象。



- 也可以自己创建，然后使用QMainWindow::setStatusBar设置主窗口的状态栏

```cpp
void QMainWindow::setStatusBar(QStatusBar *statusbar)
```





### 4.2 设置状态栏信息

- QStatusBar::showMessage函数可以用来设置状态栏的显示信息

```cpp
[slot] void QStatusBar::showMessage(const QString &message, int timeout = 0) 
  // message是具体要显示的信息，timeout为显示多长时间，单位为毫秒。
```

例如：

```cpp
#include "mainwindow.h"
#include <QStatusBar>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QStatusBar * statusBar = this->statusBar();
    statusBar->showMessage("就绪", 5000);
}
```



## 5 设置中央窗口

​	QMainWindow的中央窗口可以使用QMainWindow::CentralWidget来设置一个QWidget对象或者派生类对象作为主窗口的中央窗口。

```cpp
void QMainWindow::setCentralWidget(QWidget *widget)
```

​	例如：

```cpp
#include "mainwindow.h"
#include <QTextEdit>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QTextEdit * edit = new QTextEdit(this);
    this->setCentralWidget(edit);
}
```



​	将一个QTextEdit作为主窗口的中央窗口，QTextEdit是一个文本编辑窗口，继承至QAbstractScrollArea。



## 6 设置停靠窗口

​	Qt中停靠窗口类为QDockWidget，QDockWidget继承至QWidget，这种窗口可以停靠在主窗口的停靠区域。

- QDockWidget构造函数有两个

```cpp
QDockWidget::QDockWidget(const QString &title, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags()); //title窗口标题，parent父窗口，flags窗口属性
  
QDockWidget::QDockWidget(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::WindowFlags());
  //parent父窗口，flags窗口属性
```

- QDockWidget对象并不能当作普通窗口来使用，但是可以在个窗口上嵌套一个普通窗口

```cpp
void QDockWidget::setWidget(QWidget *widget) //用来设置一个普通窗口到停靠窗口中
```





- QMainWindow的停靠窗口可以使用QMainWindow::addDockWidget来设置一个QDockWidget对象或者派生类对象作为主窗口的中央窗口。

```cpp
void QMainWindow::addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget)
void QMainWindow::addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget, Qt::Orientation orientation)
```

​	例如：

```cpp
#include "mainwindow.h"
#include <QTextEdit>
#include <QDockWidget>
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QTextEdit * edit1 = new QTextEdit("mainWindow", this);
    this->setCentralWidget(edit1); //设置中央窗口
​
    QDockWidget* dock = new QDockWidget("dock", this); //新建一个标题为dock的停靠窗口
    QTextEdit * edit2 = new QTextEdit("dock", dock); 
    dock->setWidget(edit2); //将edit2嵌套到停靠窗口中
    this->addDockWidget(Qt::LeftDockWidgetArea, dock); //主窗口添加dock停靠窗口到左边停靠区域
}
```