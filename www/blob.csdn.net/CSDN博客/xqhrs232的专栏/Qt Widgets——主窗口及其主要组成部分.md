# Qt Widgets——主窗口及其主要组成部分 - xqhrs232的专栏 - CSDN博客
2016年10月10日 15:02:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：573
原文地址::[http://www.cnblogs.com/newstart/p/4476141.html](http://www.cnblogs.com/newstart/p/4476141.html)

### Main Window and Related Classes
|[QAction](http://qt-project.org/doc/qt-5/qaction.html)|动作类，用于当做一个菜单项或工具项插入菜单或工具栏|
|----|----|
|[QActionGroup](http://qt-project.org/doc/qt-5/qactiongroup.html)|动作组，用于管理多个动作，设置它们之间的互斥性等|
|[QWidgetAction](http://qt-project.org/doc/qt-5/qwidgetaction.html)|部件动作，继承自[QAction](http://qt-project.org/doc/qt-5/qaction.html)，用于将小部件插入菜单或工具栏|
|[QDockWidget](http://qt-project.org/doc/qt-5/qdockwidget.html)|悬浮部件，程序运行后可鼠标拖动的部件|
|[QMainWindow](http://qt-project.org/doc/qt-5/qmainwindow.html)|主窗口，是其它部件的容器|
|[QMdiArea](http://qt-project.org/doc/qt-5/qmdiarea.html)|子区域，加到[QMainWindow](http://qt-project.org/doc/qt-5/qmainwindow.html)中，用于放置多个[QMdiSubWindow](http://qt-project.org/doc/qt-5/qmdisubwindow.html)子窗口|
|[QMdiSubWindow](http://qt-project.org/doc/qt-5/qmdisubwindow.html)|子窗口|
|[QMenu](http://qt-project.org/doc/qt-5/qmenu.html)|菜单|
|[QMenuBar](http://qt-project.org/doc/qt-5/qmenubar.html)|菜单栏，存放多个菜单|
|[QSizeGrip](http://qt-project.org/doc/qt-5/qsizegrip.html)|管理顶层窗口尺寸，一般位于软件的右下角，状态栏最右边，鼠标可拖动它改变窗口大小|
|[QStatusBar](http://qt-project.org/doc/qt-5/qstatusbar.html)|状态栏|
|[QToolBar](http://qt-project.org/doc/qt-5/qtoolbar.html)|工具栏|
以QMainWindow为中心，构成了传统界面的各部分，现将它们拖在一起记录。
以rar压缩软件界面为例，各部分组成如下：
![](https://img-blog.csdn.net/20140126203050203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWxvdmVxdDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
最上一行，叫菜单栏，由“文件”、“命令”等菜单组成，“选项”菜单已点击，弹出很多菜单项，Qt中用动作类QAction来表示菜单项。即QAction构成了菜单，菜单构成了菜单条，QAction可加入文字，图片等构成漂亮的菜单项。[QDockWidget](http://qt-project.org/doc/qt-5/qdockwidget.html) 悬浮部件较特殊，它在程序运行时，拖动可改变其位置。
——————————————————————————————————————————————————————————————————
建立步骤：
为了构建传统界面，需要在建立工程某步时选择继承类，如下图。
![](https://img-blog.csdn.net/20140126203835984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWxvdmVxdDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
建成后，ui文件如下，“在这里输入”即是要求我们创建菜单，菜单栏下面一层即是默认的工具栏，没有添加动作，所以很细小（最左方有一个小点）
![](https://img-blog.csdn.net/20140126204009828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWxvdmVxdDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
双击“在这里输入后”，可以写入文字，如下图，输入了创建了常用的“文件”。在下方五个按钮处的第一个新建一个动作，可以指定动作的名称，快捷键等，建完后，将它拖动到菜单或工具栏。
![](https://img-blog.csdn.net/20140126204513468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWxvdmVxdDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
以下是效果图：
![](https://img-blog.csdn.net/20140126205053718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWxvdmVxdDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这时，只要将这个“动作”的“触发”信号连接到某个槽函数后，点击该菜单项或工具栏上的“新建”，就会执行槽函数。
以上就是建立一个传统界面的粗糙步骤
——————————————————————————————————————————————————————————————
# QMainWindow
一个 主窗口各部分分布如图：
对比（图中中间白色大框为一个QTableWidget表格小部件）：
![](https://img-blog.csdn.net/20140129205645437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWxvdmVxdDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
公有函数分类如下：
```
QMainWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0)  
    ~QMainWindow()  
//设置菜单部分  
void    setMenuBar(QMenuBar * menuBar)//带ui文件默认是有一个菜单条的，如果设置了这个之后，则默认的被覆盖，一个MainWindow只能有一个主菜单处于最上方  
void    setMenuWidget(QWidget * menuBar)//覆盖默认菜单，用一个自定义的部件代替菜单，甚至可以用一个按钮……，比较灵活，看不惯默认菜单的，可试试这个。  
  
//工具栏部分  
void    addToolBar(Qt::ToolBarArea area, QToolBar * toolbar)//添加一个工具条，用枚举值Qt::ToolBarArea(见1)设定停靠区域，由图可见，停靠区分为上下左右四区域。  
void    addToolBar(QToolBar * toolbar)//添加一个工具条（默认紧跟上一个工具条）  
QToolBar *  addToolBar(const QString & title)  
void    addToolBarBreak(Qt::ToolBarArea area = Qt::TopToolBarArea)//添加一个Break（破裂; 间断; 或译为区域），这时，新添加的工具条将不再紧跟前一个工具条，而是另起一行。  
void    insertToolBar(QToolBar * before, QToolBar * toolbar)//在before工具条后加一个  
void    insertToolBarBreak(QToolBar * before)//添加分隔线  
void    removeToolBar(QToolBar * toolbar)  
void    removeToolBarBreak(QToolBar * before)  
void    setToolButtonStyle(Qt::ToolButtonStyle toolButtonStyle)//设置工具条上显示模式，默认只有文件，可设置成图片或文字形式（见2）  
void setIconSize(const QSize & iconSize)//设置工具条上图标大小，在上一个函数设置图标模式后有效  
  
//可停靠窗口部件  
void addDockWidget(Qt::DockWidgetArea area, QDockWidget * dockwidget)//area枚举值见3  
void addDockWidget(Qt::DockWidgetArea area, QDockWidget * dockwidget, Qt::Orientation orientation)  
void removeDockWidget(QDockWidget * dockwidget)  
void setCorner(Qt::Corner corner, Qt::DockWidgetArea area)//让四个角落（corner）中一个区域"归属于"某个区域（area），具体看最下方例子  
void setDockOptions(DockOptions options)//设置停靠选项，比如允许动画或是否允许拖放重合成一个tab选项卡类型的部件，枚举值见4  
void setTabPosition(Qt::DockWidgetAreas areas, QTabWidget::TabPosition tabPosition)//设置形成的tab位置  
void setTabShape(QTabWidget::TabShape tabShape)  
void setDocumentMode(bool enabled)  
void splitDockWidget(QDockWidget * first, QDockWidget * second, Qt::Orientation orientation)//部件拖动至处于同一区域时，默认重合形成tab,利用这个，可将它们分开  
  
//设置中间部件  
void setCentralWidget(QWidget * widget)  
QWidget * centralWidget() const  
  
//添加状态栏  
void setStatusBar(QStatusBar * statusbar)  
  
//保存窗口形状信息  
bool restoreState(const QByteArray & state, int version = 0)  
<p>QByteArray saveState(int version = 0) const  
bool restoreDockWidget(QDockWidget * dockwidget)  
  
</p>//其它  
Qt::DockWidgetArea  corner(Qt::Corner corner) const  
virtual QMenu * createPopupMenu()  
DockOptions dockOptions() const  
Qt::DockWidgetArea  dockWidgetArea(QDockWidget * dockwidget) const  
bool    documentMode() const  
QSize   iconSize() const  
bool    isAnimated() const  
bool    isDockNestingEnabled() const  
QMenuBar *  menuBar() const  
QWidget *   menuWidget() const  
QStatusBar *    statusBar() const  
QTabWidget::TabPosition tabPosition(Qt::DockWidgetArea area) const  
QTabWidget::TabShape    tabShape() const  
QList<QDockWidget *>  tabifiedDockWidgets(QDockWidget * dockwidget) const  
void    tabifyDockWidget(QDockWidget * first, QDockWidget * second)  
QWidget *   takeCentralWidget()  
Qt::ToolBarArea toolBarArea(QToolBar * toolbar) const  
bool    toolBarBreak(QToolBar * toolbar) const  
Qt::ToolButtonStyle toolButtonStyle() const
```
1，enum Qt::ToolBarArea
|Constant|Value|
|----|----|
|Qt::LeftToolBarArea|0x1|
|Qt::RightToolBarArea|0x2|
|Qt::TopToolBarArea|0x4|
|Qt::BottomToolBarArea|0x8|
|Qt::AllToolBarAreas|ToolBarArea_Mask|
|Qt::NoToolBarArea|0|
2，enum Qt::ToolButtonStyle
|Constant|Value|Description|
|----|----|----|
|Qt::ToolButtonIconOnly|0|只显示图标|
|Qt::ToolButtonTextOnly|1|Only display the text.|
|Qt::ToolButtonTextBesideIcon|2|The text appears beside the icon.|
|Qt::ToolButtonTextUnderIcon|3|文本显示在图标下方|
|Qt::ToolButtonFollowStyle|4|Follow the [style](http://qt-project.org/doc/qt-5/qstyle.html#StyleHint-enum).|
3，enum Qt::DockWidgetArea
flags Qt::DockWidgetAreas
|Constant|Value|
|----|----|
|Qt::LeftDockWidgetArea|0x1|
|Qt::RightDockWidgetArea|0x2|
|Qt::TopDockWidgetArea|0x4|
|Qt::BottomDockWidgetArea|0x8|
|Qt::AllDockWidgetAreas|DockWidgetArea_Mask|
|Qt::NoDockWidgetArea|0|
4，enum QMainWindow::DockOption
flags QMainWindow::DockOptions
|Constant|Value|Description|
|----|----|----|
|QMainWindow::AnimatedDocks|0x01|与[animated](http://qt-project.org/doc/qt-5/qmainwindow.html#animated-prop)相同|
|QMainWindow::AllowNestedDocks|0x02|与[dockNestingEnabled](http://qt-project.org/doc/qt-5/qmainwindow.html#dockNestingEnabled-prop)相同|
|QMainWindow::AllowTabbedDocks|0x04|允许形成下方有tapBar的重合部件|
|QMainWindow::ForceTabbedDocks|0x08|Each dock area contains a single stack of tabbed dock widgets. In other words, dock widgets cannot be placed next to each other in a dock area. If this option is set, AllowNestedDocks has no effect.|
|QMainWindow::VerticalTabs|0x10|设置tabBar在竖直左方位置（默认在下方）|
以rar压缩软件为例，用代码写出界面主要部分：
![](https://img-blog.csdn.net/20140131115658328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWxvdmVxdDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140131115708609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWxvdmVxdDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
以下代码只是用于理解，是很不合理的。主要测试各部分小部件的位置，外观等，以及mainwidow的形状配置保存
```
```
#include
 "mainwindow.h"
```
```
#include
 "ui_mainwindow.h"
```
```
#include
 <QAction>
```
```
#include
 <QMenu>
```
```
#include
 <QIcon>
```
```
#include
 <QSize>
```
```
#include
 <QLabel>
```
```
#include
 <QDockWidget>
```
```
#include
 <QTableWidget>
```
```
#include
 <QSettings>
```
```
MainWindow::MainWindow(QWidget
 *parent) :
```
`    ``QMainWindow(parent),`
`    ``ui(``new``Ui::MainWindow)`
`{`
`    ``ui->setupUi(``this``);`
`    ``//菜单栏部分`
`    `
```
QMenu
 *menu1=ui->menuBar->addMenu(
```
`"文件(&F)"``);`
`    `
```
QAction
 *me1_act1=menu1->addAction(
```
`"打开压缩文件(O)"``);`
`    ``me1_act1->setShortcut(QKeySequence(tr(``"Ctrl+O"``)));`
`    ``//connect(me1_act1,SIGNAL(triggered()),……`
`    `
```
QAction
 *me1_act2=menu1->addAction(
```
`"保存压缩文件副本为(V)"``);`
`    ``me1_act2->setIcon(QIcon(``"://img/sava.ico"``));`
`    ``menu1->addSeparator();`
`    `
```
QAction
 *me1_act3=menu1->addAction(
```
`"退出(X)"``);`
`    ``connect(me1_act3,SIGNAL(triggered()),``this``,SLOT(close()));`
`    ``ui->menuBar->addMenu(``"命令(&C)"``);`
`    ``//"文件"菜单部分`
`    `
```
QAction
 *tb1_act1=ui->mainToolBar->addAction(QIcon(
```
`"://img/tool1.ico"``),``"添加"``);`
`    ``ui->mainToolBar->addSeparator();`
`    `
```
QAction
 *tb1_act2=ui->mainToolBar->addAction(QIcon(
```
`"://img/tool2.ico"``),``"解压到"``);`
`    ``setToolButtonStyle(Qt::ToolButtonTextUnderIcon);`
`    ``setIconSize(QSize(50,50));`
`    ``addToolBarBreak();`
`    ``toolBar=``new``QToolBar;`
`    ``toolBar->setObjectName(``"tb2"``);``//设置对象名,用于保存配置`
`    ``toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);`
`    ``toolBar->setIconSize(QSize(20,20));`
`    ``addToolBar(toolBar);`
`    `
```
QAction
 *tb2_act1=toolBar->addAction(QIcon(
```
`"://img/sharp.ico"``),``""``);`
`    ``//添加中间主部件`
`    `
```
QTableWidget
 *tb1=
```
`new``QTableWidget;`
`    ``setCentralWidget(tb1);`
`    ``//添加悬浮部件`
`    `
```
QDockWidget
 *dock1=
```
`new``QDockWidget(``"上侧部件"``);`
`    ``addDockWidget(Qt::TopDockWidgetArea,dock1);`
`    `
```
QDockWidget
 *dock2_1=
```
`new``QDockWidget(``"左侧部件1"``);`
`    ``addDockWidget(Qt::LeftDockWidgetArea,dock2_1);`
`    `
```
QDockWidget
 *dock2_2=
```
`new``QDockWidget(``"左侧部件2"``);`
`    ``addDockWidget(Qt::LeftDockWidgetArea,dock2_2);`
`    ``splitDockWidget(dock2_1,dock2_2,Qt::Horizontal);`
`    `
```
QDockWidget
 *dock3=
```
`new``QDockWidget(``"右侧部件"``);`
`    ``dock3->setFeatures(QDockWidget::DockWidgetVerticalTitleBar|QDockWidget::DockWidgetMovable);`
`    ``addDockWidget(Qt::RightDockWidgetArea,dock3);`
`    `
```
QDockWidget
 *dock4=
```
`new``QDockWidget(``"下侧部件"``);`
`    ``addDockWidget(Qt::BottomDockWidgetArea,dock4);`
`    ``//setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);//让左下角区域归于左边停靠区域`
`    ``dock1->setObjectName(``"dock1"``);`
`    ``dock2_1->setObjectName(``"dock2_1"``);`
`    ``dock2_2->setObjectName(``"dock2_2"``);`
`    ``dock3->setObjectName(``"dock3"``);`
`    ``dock4->setObjectName(``"dock4"``);`
`    ``//状态栏部分`
`    `
```
QLabel
 *lbl1=
```
`new``QLabel(``"无状态"``);`
`    ``ui->statusBar->addWidget(lbl1);`
`    ``//读取窗口及部件形状与状态`
`    ``readSettings();`
`}`
`void`
```
MainWindow::closeEvent(QCloseEvent
 *
```
`event``)`
`{`
`    `
```
QSettings
 settings(
```
`"大中华有限公司"``, ``"测试软件名"``);`
`    ``settings.setValue(``"geometry"`
```
,
 saveGeometry());
```
`    ``settings.setValue(``"windowState"`
```
,
 saveState());
```
`    ``settings.sync();`
`}`
`void``MainWindow::readSettings()`
`{`
`    `
```
QSettings
 settings(
```
`"大中华有限公司"``, ``"测试软件名"``);`
`    ``restoreGeometry(settings.value(``"geometry"``).toByteArray());`
`    ``restoreState(settings.value(``"windowState"``).toByteArray());`
`}`
`MainWindow::~MainWindow()`
`{`
`    `
```
delete
 ui;
```
`}`
```
作者：[江南烟雨居](http://www.cnblogs.com/newstart/)
出处：[http://www.cnblogs.com/newstart//](http://www.cnblogs.com/newstart/)
本文版权归作者和博客园共有，欢迎转载，但未经作者同意必须保留此段声明，且在文章页面明显位置给出原文连接，否则保留追究法律责任的权利。
分类: [Qt 编程](http://www.cnblogs.com/newstart/category/441259.html)
