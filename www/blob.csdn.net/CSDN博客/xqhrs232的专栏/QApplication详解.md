# QApplication详解 - xqhrs232的专栏 - CSDN博客
2017年11月08日 14:54:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：243
原文地址::[http://blog.csdn.net/hudfang/article/details/52979434](http://blog.csdn.net/hudfang/article/details/52979434)
相关文章
1、[QApplication类参考](http://blog.chinaunix.net/uid-25749806-id-302132.html)----[http://blog.chinaunix.net/uid-25749806-id-302132.html](http://blog.chinaunix.net/uid-25749806-id-302132.html)
2、新手必学 Qt 类简介之 Qt QApplication 类(4)----[http://mobile.51cto.com/symbian-269525_3.htm](http://mobile.51cto.com/symbian-269525_3.htm)
一、详细描述
QApplication 类管理图形用户界面应用程序的控制流和主要设置。 可以说 QApplication是Qt的整个后台管理的命脉
它包含主事件循环，在其中来自窗口系统和其它资源的所有事件被处理和调度。它也处理应用程序的初始化和结束，并且提供对话管理。它也处理绝大多数系统范围和应用程序范围的设置。
对于任何一个使用Qt的图形用户界面应用程序，都正好存在一个QApplication对象，而不论这个应用程序在同一时间内是不是有0、1、2或更多个窗口。
QApplication 对象是可以通过全局变量qApp访问。它的负责的主要范围有：
它使用用户的桌面设置，例如palette()、font()和doubleClickInterval()来初始化应用程序。如果用户改变全局桌面，例如通过一些控制面板，它会对这些属性保持跟踪。
它执行事件处理，也就是说它从低下的窗口系统接收事件并且把它们分派给相关的窗口部件。通过使用sendEvent()和postEvent()，你可以发送你自己的事件到窗口部件。
它分析命令行参数并且根据它们设置内部状态。关于这点的详细情况请参考下面的构造函数文档。
它定义了由QStyle对象封装的应用程序的观感。在运行状态下，可以通过setStyle()来改变。
它指定了应用程序如何分配颜色。详细情况请参考setColorSpec()。
它定义了默认文本编码（请参考setDefaultCodec()）并且提供了通过translate()用户可见的本地化字符串。
它提供了一些像desktop()和clipboard()这样的魔术般的对象。
它知道应用程序的窗口。你可以使用widgetAt()来询问在一个确定点上存在哪个窗口部件，得到一个topLevelWidgets()（顶级窗口部件）的列表和通过closeAllWindows()来关闭所有窗口，等等。
它管理应用程序的鼠标光标处理，请参考setOverrideCursor()和setGlobalMouseTracking()。
在X窗口系统上，它提供刷新和同步通讯流的函数，请参考flushX()和syncX()。
它提供复杂的对话管理支持。这使得当用户注销时，它可以让应用程序很好地结束，如果无法终止，撤消关闭进程并且甚至为未来的对话保留整个应用程序的状态。详细情况请参考isSessionRestored()、sessionId()、commitData()和saveState()。
应用程序排演实例包含了一个QApplication通常用法的典型完整的main()。
因为QApplication对象做了如此多的初始化，它必须在所有与用户界面相关的其它类被创建之前被创建。
因为它也处理命令行参数，在应用程序中对argv解释和修改之前创建它通常是一个好主意。（注意，也对于X11，setMainWidget()可以根据-geometry选项来改变主窗口部件。为了保持这个功能，你必须在setMainWidget()和它的任何重载之前设置你的默认。）
二、QApplication两类构造的讲解
QApplication::QApplication ( int & argc, char ** argv )
初始化窗口系统并且使用在argv中的argc个命令行参数构造一个应用程序对象。
全局指针qApp指向这个应用程序对象。应该只有一个应用程序对象被创建。
这个应用程序对象必须在任何绘制设备（包括窗口部件、像素映射、位图等等）之前被构造。
注意argc和argv也可以被改变。Qt会移除它能够识别的命令行参数。原来的argc和argv稍后可以通过qApp->argc()和qApp->argv()来访问。argv()的文档中包含如何处理命令行参数的详细描述。
Qt调试选项（如果Qt被使用被定义的QT_NO_DEBUG标记进行编译，这些选项就是不可用的）：
-nograb，告诉Qt永远不要捕获鼠标或者键盘。
-dograb （只有在X11下），在调试器下运行可能导致和-nograb冲突，使用-dograb来覆盖。
-sync （只有在X11下），为了调试切换为同步模式。
更详细的解释请参考调试技术。
所有的Qt程序自动支持下面这些命令行选项：
-style= style，设置应用程序图形用户界面风格。可能的值有motif、windows和platinum。如果你使用其它风格编译Qt或者有作为插件编译了其它风格，那么它们都可以被用作-style命令行选项。
-style style，这个和上面的一样。
-session= session，从以前的对话中重新载入应用程序。
-session session，这个和上面的一样。
X11版本的Qt也支持传统的X11命令行选项：
-display display，设置X显示（默认为$DISPLAY）。
-geometry geometry，设置主窗口部件的客户端位置和大小。
-fn或者-font font，定义应用程序的字体。这个字体必须使用X逻辑字体描述来北指定。
-bg或者-background color，设置默认背景色和应用程序调色板（亮的和暗的阴影将被计算出来）。
-fg或者-foreground color，设置默认前景色。
-btn或者-button color，设置默认按钮颜色。
-name name，设置应用程序名称。
-title title，设置应用程序标题。
-visual TrueColor，强制应用程序在8位显示上使用真彩图像。
-ncols count，如果应用程序使用QApplication::ManyColor颜色规格，在8位显示中限制颜色立方体重分配的颜色。如果count为216，那么一个6x6x6的颜色立方体将被使用（也就是说6级红色，6级绿色和6级蓝色），对于其它值，与2x3x1立方体成比例的立方体将被使用。
-cmap，导致应用程序在8位显示上安装一个私有的颜色映射。
QApplication::QApplication ( int & argc, char ** argv, bool GUIenabled )
构造一个使用argv中argc个命令行参数的应用程序对象。如果GUIenabled为真，图形用户界面应用程序被构造，否则非图形用户界面（命令行）应用程序被创建。
对于没有图形用户界面的程序，设置GUIenabled为假可以在没有窗口系统的情况下运行。
在X11下，如果GUIenabled为真，窗口系统被初始化。如果GUIenabled为假，应用程序不连接X服务器。在Windows和Macintosh，现在窗口系统总是被初始化，而不管GUIenabled的值。在Qt未来的版本这也许会改变。
对于线程配置（也就是说当Qt已经被作为线程库建立），应用程序全局互斥量在构造函数中将被锁定并且当使用exec()进入事件循环中时解锁。如果你不调用exec()，你必须明显地对这个互斥量解锁，否则你将会在应用程序退出时得到警告。
下面这个例子显示如何创建在可能情况下使用图形界面的应用程序。
1. int main( int argc, char **argv )
2. {
3. #ifdef Q_WS_X11
4. bool useGUI = getenv( "DISPLAY" ) != 0;
5. #else
6. bool useGUI = TRUE;
7. #endif
8. QApplication app(argc, argv, useGUI);
9.
10. if ( useGUI ) {
11. //开始图形用户界面版本
12. ...
13. } else {
14. //开始非图形用户界面版本
15. ...
16. }
17. return app.exec();
18. }
三、窗口管理
在窗口管理方面，对于独立的1个进程，qApplication 提供窗口管理的成员函数有（多进程的窗口管理，这里暂不介绍）：
1. QDesktopWidget * QApplication::desktop () [静态]
返回桌面窗口部件（也调用根窗口）。
桌面窗口部件对于获得屏幕大小很有用。在桌面上绘制也是可能的。我们建议不要假设可以在桌面上绘制，因为它不能在所有的操作系统上工作。
1. QDesktopWidget *d = QApplication::desktop();
2. int w = d->width(); // 返回桌面宽度
3. int h = d->height(); // 返回桌面高度
4. QWidgetList * QApplication::allWidgets () [静态]
返回应用程序中所有窗口部件的列表。
这个列表必须使用new来被创建并且必须被调用者删除。
如果没有窗口部件，这个列表为空（QPtrList::isEmpty()）。
注意一些窗口部件也许被隐藏。
更新所有窗口部件的实例：
1. QWidgetList *list = QApplication::allWidgets();
2. QWidgetListIt it( *list ); // 遍历窗口部件
3. QWidget * w;
4. while ( (w=it.current()) != 0 ) { // 对于每一个窗口部件……
5. ++it;
6. w->update();
7. }
8. delete list; // 删除这个列表，不是这个窗口部件
1. QWidgetList * QApplication::topLevelWidgets () [静态]
返回应用程序中顶级窗口部件的列表。
这个列表是使用new创建的并且必须由调用者删除。
如果没有顶级窗口部件，这个列表是空的（QPtrList::isEmpty()）。
注意一些顶级窗口部件也许被隐藏了，例如如果当前没有工具提示被显示的工具提示。
实例：
1. // 显示所有隐藏的顶级窗口部件。
2.
3. QWidgetList *list = QApplication::topLevelWidgets();
4. QWidgetListIt it( *list ); // 遍历窗口部件
5. QWidget * w;
6. while ( (w=it.current()) != 0 ) { // 对每一个顶级窗口部件
7. ++it;
8. if ( !w->isVisible() )
9. w->show();
10. }
11. delete list; // 删除这个列表，而不是窗口部件
1. QWidget * QApplication::topLevelAt ( int x, int y ) [static]
返回最上层x,y点的widget, 可以用来定位最上层窗口
返回当前层x,y点的widget, 可以用来定位当前窗口，和topLevelAt不同的是 widgetAt是以global
1. QWidget * QApplication::widgetAt ( int x, int y ) [static]
screen position 来判断的，而topLevelAt是以显示区域为坐标区域的。
1. void QApplication::closeAllWindows () [static slot]
关闭所有顶级窗口。
1. exitAct = new QAction(tr("E&xit"), this);
2. exitAct->setShortcuts(QKeySequence::Quit);
3. exitAct->setStatusTip(tr("Exit the application"));
4. connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
四、关于事件循环
qApplication 的事件循环，就是MainLoop的事件循环，当然它的原理和QEventLoop的原理是一样的。
void QCoreApplication::processEvents ( QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents ) [static]
Processes all pending events for the calling thread according to the specified flags until there are no more events to process.
You can call this function occasionally when your program is busy performing a long operation (e.g. copying a file).
In event you are running a local loop which calls this function continuously, without an event loop, the DeferredDelete events will not be processed. This can affect the behaviour of widgets, e.g. QToolTip, that rely on DeferredDelete events to function properly.
 An alternative would be to call sendPostedEvents() from within that local loop.
Calling this function processes events only for the calling thread.
大体意思就是预先调用处于队列状态的事件，直到全部处理完毕。这个函数大多用于系统繁忙时，导致一些事件被延后了（如绘制事件等），用此函数可保证全部事件处理完毕后在继续执行。常见于show(),update()之后调用。
Note: 这个函数线程安全
五、事件的发送
关于事件的发送有两种方式：
1. bool QApplication::sendEvent ( QObject * receiver, QEvent * event ) [静态]
使用notify()函数直接发送事件event给接受者receiver。返回由事件处理器返回的值。
这个事件在它被发送时，不能被删除。通常的访问方式是在栈中创建这个事件，例如：
1. QMouseEvent me( QEvent::MouseButtonPress, pos, 0, 0 );
2. QApplication::sendEvent( mainWindow, &me );
如果你在堆中创建这个事件，你就必须删除它。
1. void QApplication::postEvent ( QObject * receiver, QEvent * event ) [静态]
把事件event添加为对象receiver的接收对象，把它添加到事件队列中并且立即返回。
这个时间必须在堆中被分配，因为递送事件队列将会得到事件的所有权并且一旦它被递送就删除它。
当控制返回主事件循环，存储在队列中的所有事件将被使用notify()函数发送。
基本上Qt大部分的地方都是用到sendEvent, 而少部分地方会用到postEvent， 研究线程安全的童鞋可以多看看这方面的资料
小结：QApplication 包含主事件循环，在其中来自窗口系统和其它资源的所有事件被处理和调度。它也处理应用程序的初始化和结束，并且提供对话管理。它也处理绝大多数系统范围和应用程序范围的设置。对于任何一个使用Qt的图形用户界面应用程序，都正好存在一个QApplication 对象，而不论这个应用程序在同一时间内是不是有0、1、2或更多个窗口。
