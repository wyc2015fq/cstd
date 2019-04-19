# Qt使用QWT绘制科学图表和图形 - fanyun的博客 - CSDN博客
2017年07月30日 21:37:46[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1418
 
              Qwt是一个基于LGPL版权协议的开源项目，
 其目标是提供一组2D的窗体库显示技术领域的数据，
 数据源以浮点数组或范围的方式提供， 输出方式可以是Curves（曲线），Slider（滚动条）， Dials（圆盘）， compasses（仪表盘）等等。
 该工具库基于Qt开发，
 所以也继承了Qt的跨平台特性，
 据原作者文档所说， 该项目在Qt-win/Qt-x11/Qt-embedded（qvfb环境）上都测试过，
 运行正常。 项目的主页在：[http://qwt.sourceforge.net/](http://qwt.sourceforge.net/)
在网上搜了一下， 发现关于qwt的中文资料实在很少，
 基本上只有关于编译和安装的文章。 实际上经过笔者实践， Qwt的编译和运行实在乏善可陈， qmake;make就可以搞定，
 没什么可memo的东西， 所以这篇文章将以扫盲为主， 介绍Qwt的feature。
当然按照正常的顺序， 我们还是从编译安装开始。 从svn服务器上下载最新代码：svn
 co[https://qwt.svn.sourceforge.net/svnroot/qwt/trunk/qwt](https://qwt.svn.sourceforge.net/svnroot/qwt/trunk/qwt)
进入qwt目录，
 运行你电脑上qt4对应的qmake，
 再运行make编译。如笔者环境中是
$ cd qwt
$ export PATH=/usr/local/Trolltech/Qt-4.5.1/bin/:$PATH
$ qmake
$ make
编译要花个几分钟的时间。 成功后在lib下会生成 libqwt.so*文件，
 并且examples也参与编译，
 生成的binary在examples/bin下，
 我们可以运行这些例子初步查看qwt的功能。
$exportLD_LIBRARY_PATH=$PWD/lib
$cd examples/bin
$ ./simple
simple是qwt自带的例子中最简单的一个，
 一共只有一百来行的代码， 实现了数学中的正弦函数(sin())和余弦函数(cos())曲线。
 如下图：
这个例子里用到的核心类有四个（以下内容是笔者的理解， 有可能有不对的地方， 请酌情阅读）：
QwtPlot
类似一个2D绘图的容器，
 里面可以放其他的QwtPlotItem派生类对象，
 比如本例子中使用的QwtPlotMarker等。（有点类似Qt里的graphicsview的感觉。）
 这个类负责控制绘图数据的产生和传递， 并且绘制一个坐标轴。
QwtPlotMarker
标尺类， 用于绘制刻度线。
QwtPlotCurve
曲线类， 用于绘制各种曲线。
QwtSyntheticPointData
比较奇怪， 这是个undocumented的类，
 估计不小心被作者遗漏了， 文档中没有提供该类的说明， 只能从源码中寻找答案了， 这一点很让人郁闷。 通过看code当中的注释和例子代码，
 笔者的理解是该类负责产生数据， 它的作用是提供固定数量的浮点数的点， 点的数量在构造类时传入。 获取数据时QwtPlotCurve类会调用该类的y()方法获取纵坐标。
程序的基本流程是：
1、初始化绘图容器， 设置坐标轴的参数
// Set axes
setAxisTitle(xBottom, “x –>”);
setAxisScale(xBottom,0.0,
 10.0);//横坐标从0到10， xBottom表示横坐标的方向从下往上
setAxisTitle(yLeft,
 “y –>”);
setAxisScale(yLeft,-1.0,
 1.0); //纵坐标－1到1，yLeft表示纵坐标的方向从左到右
2、添加正弦余弦曲线
// Insert new curves
QwtPlotCurve *cSin = new QwtPlotCurve(”y = sin(x)”);
#if QT_VERSION >= 0×040000
cSin->setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
cSin->setPen(QPen(Qt::red));//红色曲线
cSin->attach(this);
QwtPlotCurve *cCos = new QwtPlotCurve(”y = cos(x)”);
#if QT_VERSION >= 0×040000
cCos->setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
cCos->setPen(QPen(Qt::blue));//蓝色曲线
cCos->attach(this);
3、设置曲线的数据内容
// Create sin and cos data
cSin->setData(FunctionData(::sin));
cCos->setData(FunctionData(::cos));
4、添加横纵标尺线作为坐标的参照
// Insert markers
// …a horizontal line at y = 0…
QwtPlotMarker *mY = new QwtPlotMarker();
mY->setLabel(QString::fromLatin1(”y = 0″));
mY->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
mY->setLineStyle(QwtPlotMarker::HLine);//线的方向
mY->setYValue(0.0);//标尺线画在y＝0的位置
mY->attach(this);
//
 …a vertical line at x = 2 * pi
QwtPlotMarker *mX = new QwtPlotMarker();
mX->setLabel(QString::fromLatin1(”x = 2 pi”));
mX->setLabelAlignment(Qt::AlignLeft | Qt::AlignBottom);
mX->setLabelOrientation(Qt::Vertical);
mX->setLineStyle(QwtPlotMarker::VLine);
mX->setLinePen(QPen(Qt::black, 0, Qt::DashDotLine));
mX->setXValue(2.0
 * M_PI);//该标线画在x＝2PI的位置
mX->attach(this);
同样的功能如果用Qt来写的话需要写大量的绘图代码，
 比较麻烦， 但使用Qwt的封装库代码更OO且更容易理解和维护。
 在这个例子中使用Qwt的优势体现得可能还不太明显，
 相比而言复杂一些的图表更能体现Qwt的优越性。
