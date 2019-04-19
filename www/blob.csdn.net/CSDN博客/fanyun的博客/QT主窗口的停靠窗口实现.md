# QT主窗口的停靠窗口实现 - fanyun的博客 - CSDN博客
2016年10月30日 20:53:54[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1194
关于停靠窗口，现在我们所见的许多软件都存在停靠窗口，如Visual Studio软件的类视图、属性视图和资源视图都是停靠窗口的实际案例；停靠窗口作为主窗口的一部分，可以停靠、浮动、显示隐藏等；
该博文主要讲解在Qt下的停靠窗口的实现；主窗口为单文档窗口，继承于QMainWindow类，该类包含了标准的菜单栏，工具栏，内容窗口，状态栏等；停靠窗口类为QDockWidget；主要是使用上述的两个类；界面效果如下图所示：
下面一步一步讲解实现方式：[喝小酒的网摘]http://blog.hehehehehe.cn/a/8772.htm
一、菜单的实现
菜单主要使用QMenu和QAction两类，如图中所示，主要有File和View菜单；代码如下所示：
m_pFMenu=menuBar()->addMenu("&File");
m_pExit = new QAction(tr("&Exit"),this);
m_pExit->setShortcuts(QKeySequence::Quit);
connect(m_pExit, SIGNAL(triggered()), this,SLOT(close()));
m_pFMenu->addAction(m_pExit);
View菜单主要控制三个停靠窗口的显示和隐藏：
m_pVMenu=menuBar()->addMenu("&View");
m_pVMenu->addAction(m_pDockWidget1->toggleViewAction());
m_pVMenu->addAction(m_pDockWidget2->toggleViewAction());
m_pVMenu->addAction(m_pDockWidget3->toggleViewAction());
二、内容窗口的实现
内容窗口作为主窗口的主要展示窗口，其随着主窗口的伸缩而伸缩，该程序中使用QTextEdit类作为显示文本信息
//创建QTextEdit对象
m_pTextEdit = new QTextEdit(this);
setCentralWidget(m_pTextEdit);
三、停靠窗口的实现
该程序使用三个停靠窗口，都放置在主窗口的左边；其中左边最顶端的停靠窗口实现如下代码所示：
//创建QDockWidget对象
m_pDockWidget1 = new QDockWidget(this);
m_pDockWidget1->setWindowTitle("dockdate");
//设置可停靠区域
m_pDockWidget1->setAllowedAreas(Qt::AllDockWidgetAreas);
//设置m_pDockWidget1内的部件
m_pCale = new QCalendarWidget(m_pDockWidget1);
m_pCale->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
m_pCale->setGridVisible(true);
m_pDockWidget1->setWidget(m_pCale);
//主窗口中添加dock
addDockWidget(Qt::LeftDockWidgetArea,m_pDockWidget1);
源码下载地址：http://files.cnblogs.com/appsucc/DockWindow.rar
