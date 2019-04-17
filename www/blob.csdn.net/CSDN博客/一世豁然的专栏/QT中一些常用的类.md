# QT中一些常用的类 - 一世豁然的专栏 - CSDN博客





2015年10月06日 11:20:31[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2076








本博客转载于：[http://blog.chinaunix.net/uid-23670869-id-2391612.html](http://blog.chinaunix.net/uid-23670869-id-2391612.html)






这篇文章主要是介绍Qt 当中经常使用的类，采取的是使用字母索引的方式，下面的类是被经常使用的。



QDataStream 为QIODevice提供了一串的二进制的数据
QDataWidgetMapper  提供了从数据模型到部件的映射
QData 日期的功能
QDateEdit 提供了日期的部件用来编辑日期，是基于QDateTimeEdit 部件
QDateTime  提供日期与时间的功能
QDateTimeEdit 提供了编辑日期与实践的部件
QDBusAbstractAdaptor 提供了基于D-BUS适配器的基础累
QDBusAbstractInterface 提供了所有QtDBus 绑定以及能够访问的D-BUS 界面
QDebug 提供了输出调试信息流，从后台输出
QDecoration 提供了Window 与嵌入式linux 的装饰支持
QDecorationDefault 提供了默认窗口的装饰
QDesktopSerivces  提供了访问常用桌面服务的方法
QDesktopWidget 提供了访问多头的屏幕信息的功能
QDial 提供了滑盘控制的支持
QDialog 提供了窗口系统的基类
QDialogButtonBox 提供了使用于当前部件类型的按钮对话框
QDir  提供了访问目录的结构与内容的支持
QDirectPainter 提供了直接用户绘画的硬件支持，使用在嵌入式linux 当中
QDirModel 提供了用于访问本地文件系统的模型支持
QDockWidget 提供了在一个QMainWindow 中的窗口或者作为桌面的顶级窗口出现


Effect  用来传输音频流
EffectParameter  用来描述一个effect 的参数
EffectWidget 提供了一个用来控制effect 参数的部件
QErrorMessage  通过一个对话框来显示错误信息
QEvent  提供了所有事件累的积累，事件对象包含了时间的参数
QEventLoop 提供了进入和离开事件循环的功能
Exception 提供了在线程之间传输的异常


QFile  提供了用于读写文件的界面
QFileDialog 提供了一个对话框使得用户可以选择文件或者目录
QFileIconProvider 为QDirModel 类提供的文件图标
QFileInfo 提供基于平台依赖的文件信息
QFileOpenEvent 当打开一个文件或者打开一个url 地址的时候抛出一个事件
QFileSystemModel 提供了本地文件系统的一个数据模型
QFileSystemWatcher 提供了坚实本地文件与目录的监视功能
QFocusEvent 当一个部件获得焦点则发出事件
QFocusFrame 提供了当前具有绘画的区域的焦点
QFont 为绘画文本提供制定的字体
QFontComboBox 提供了选择字体族的下拉列表框
QFontDatabase 提供了可以在Window 操作系统下能够使用的字体
QFontDialog 用来选择一个指定的字体
QFontEngineInfo 用来描述一个指定的字体引擎插件
QFontEnginePlugin  累提供了字体仓库插件的基础累，用于嵌入式linux

QFontInfo 包含了一个字体的常规信息
QFontDialog 用来弹出对话框来获得指定的字体
QFtp    提供了ftp通讯的客户端程序的实现功能


QGesture  包含了用户输入的信息描述
QGestureEvent 当一个用户输入来临触发此事件
QGLColormap  提供了用于QGLWidget  的自定义颜色图
QGLContext  包含了OpenGL 的渲染内容
QGLFormat 提供了OpenGL 渲染内容的格式
QGLFramebufferObject 包含了OpenGL 的帧缓冲对象
QGLFramebufferObjectFormat 指定了当前OpenGL 帧缓冲的对象格式
QGLPixelBuffer 包含了OpenGL pbuffer

QGLWidget 提供了用来渲染 OpenGL的图形部件
QGraphicsScale  提供了缩放的翻译
QGraphicsScene  提供了2D 图形项目的管理界面
QGraphicsView    他提供了QGraphicsScene 内容的显示部件
QGraphicsWebView   将web 内容添加到了GraphicsView 中
QGridLayout   提供了网状布局
QGroupBox  提供了拥有相同类型的组框，并带有标题


QHash  模板类，提供了哈希表字典
QHashIterator  提供了java 类型的迭代器，主要是给QHash 与QMultiHash 来使用
QHBoxLayout    部件横向布局
QHideEvent 当前部件隐藏时发出的事件
QHostAddress 提供ip 地址
QHostInfo  提供了静态的函数来对主机进行查询
QHoverEvent   提供描述鼠标的事件，当鼠标徘徊在某一处的时候


QIcon  提供了可以缩放的图标，可用在不同模式下不同状态中
QIconEngine 绘画QIcon 的抽象基础类
QImage 

QImageReader 提供了从一个文件或者其他设备读取指定文件格式的界面
QImageWriter 提供了写入一个文件或者其他设备所指定的文件格式的界面
QInputContext  作为一种输入法，他依赖于具体的数据与状态
QInputDialog  提供了方便的对话框让用户来输入单个数值
QInputEvent 当用户输入的时候发出信息
QInputMethodEvent  输入发生时抛出事件
QIODevice 他是Qt 中所有输入输出界面的基类

QKeyEvent 一个键盘输入事件发生

QLabel 在一个部件显示文字或者图片
QLayout 提供了几何位置管理的基础类
QLCDNumber 提供了一个部件用于类似LCD 的现实数字
QLibrary  在程序运行时加载共享库文件
QLibaryInfo   提供Qt 库的信息
QLine  两个坐标点提供一条具有芳香的直线
QLineEdit   提供了一行文本输入的编辑器
QList 模板类，提供链表的基本操作功能
QListIterator 提供了java 类型的链表迭代器
QListView   提供列表或者提供图标的试图，要求显示在一个模型上
QListWidget 提供了基于一个项目的列表部件
QListWidgetItem  创建项目用于QListWidget视图的显示
QLocalServer  本地套接字的基础服务器端
QLocalSocket  提供了一个本地的套接字



随后会发布一些其他字母索引的类的说明。




