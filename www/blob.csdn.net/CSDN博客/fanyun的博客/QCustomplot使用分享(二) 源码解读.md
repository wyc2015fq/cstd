# QCustomplot使用分享(二) 源码解读 - fanyun的博客 - CSDN博客
2017年11月26日 21:56:17[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：192
### 一、头文件概述
     从这篇文章开始，我们将正式的进入到QCustomPlot的实践学习中来，首先我们先来学习下QCustomPlot的类图，如果下载了QCustomPlot源码的同学可以自己去QCustomPlot的目录下documentation/qcustomplot下寻找一个名字叫做index.html的文件，将其在浏览器中打开，也是可以找到这个库的类图。如图1所示，是组成一个QCustomPlot类图的可能组成形式。
- 
一个图表(QCustomPlot)：包含一个或者多个图层、一个或多个item(用于展示一些固定的元素，例如文本、线段等)、一个或者多个可以绘制的元素、一个布局
- 
一个图层(QCPLayer)：包含基本的元素(QCPLayerable)
- 
一个QCPAbstractItem：包含一个或者多个位置信息
- 
一个坐标轴矩形(QCPAxisRect):包含多个坐标轴、有一个图例类(多个图例项)
图1 图表组成
      在一个QCustomPlot类图中最重要、用的最多的是QCPLayerable元素，几乎除了QCPLayer以外的元素都是继承于该类。
- 
QCPAbstractPlottable：绘图元素，包含：折线图(QCPGraph)、曲线图(QCPCurve)、柱状图(QCPBars)、QCPStatiBox(盒子图)、QCPColorMap(色谱图)、QCPFinancial(金融图)
- 
QCPAbstractItem：标示项，包含：直线(QCPItemStraightLine)、线段(QCPItemLine)、曲线(QCPItemCurve)、矩形(QCPItemRect)、椭圆(QCPItemEllipse)、文本(QCPItemText)、小圆球(QCPItemTracer)、图片(QCPItemPixmap)
- 
布局项(QCPLayoutElement)：布局项(QCPAbstractLegendItem)、坐标轴矩形(QCPAxisRect)
- 
网格线(QCPGrid)：每一个坐标轴对应一个网格线
- 
坐标轴(QCPAxis)：一个坐标轴矩形包含四个坐标轴，上下左右四个坐标轴。
图2 可以绘制元素类图
### 二、一个简单的示例
    如下代码是一个简单的蜡烛图代码，源码我是从官方网站上扒下来的，只是为了让大家有一个初步的了解，本来是英文的注释我换成了中文，然后添加了我自己个人的一些理解，运行结果如图3所示
```
1 customPlot->legend->setVisible(true);
 2  
 3 // 生成2种随机的蜡烛图数据，第一个是蜡烛图数据，第二个是美国线数据
 4 int n = 500;
 5 QVector<double> time(n), value1(n), value2(n);
 6 QDateTime start = QDateTime(QDate(2014, 6, 11));
 7 start.setTimeSpec(Qt::UTC);
 8 double startTime = start.toTime_t();
 9 double binSize = 3600*24; // 1天的数据
10 time[0] = startTime;
11 value1[0] = 60;
12 value2[0] = 20;
13 qsrand(9);//生成随机数时给指定的种子，那么生成的随机数都是相同的，因此每次运行后得到的结果都是不变的
14 for (int i=1; i<n; ++i)
15 {
16   time[i] = startTime + 3600*i;
17   value1[i] = value1[i-1] + (qrand()/(double)RAND_MAX-0.5)*10;
18   value2[i] = value2[i-1] + (qrand()/(double)RAND_MAX-0.5)*3;
19 }
20  
21 // 初始化一个蜡烛图指针:
22 QCPFinancial *candlesticks = new QCPFinancial(customPlot->xAxis, customPlot->yAxis);
23 candlesticks->setName("Candlestick");
24 candlesticks->setChartStyle(QCPFinancial::csCandlestick);//设置图表类型为蜡烛图
25 candlesticks->data()->set(QCPFinancial::timeSeriesToOhlc(time, value1, binSize, startTime));//设置数据
26 candlesticks->setWidth(binSize*0.9);//设置每一个数据项的绘制宽度
27 candlesticks->setTwoColored(true);//设置是否显示两种颜色
28 candlesticks->setBrushPositive(QColor(245, 245, 245));//设置收>开画刷
29 candlesticks->setBrushNegative(QColor(40, 40, 40));//设置收<开画刷
30 candlesticks->setPenPositive(QPen(QColor(0, 0, 0)));//设置收>开画笔
31 candlesticks->setPenNegative(QPen(QColor(0, 0, 0)));//设置收>开画笔
32  
33 // 初始化一个美国线图指针:
34 QCPFinancial *ohlc = new QCPFinancial(customPlot->xAxis, customPlot->yAxis);
35 ohlc->setName("OHLC");
36 ohlc->setChartStyle(QCPFinancial::csOhlc);//设置图表类型为美国线
37 ohlc->data()->set(QCPFinancial::timeSeriesToOhlc(time, value2, binSize/3.0, startTime)); //为了区分于蜡烛图显示，
38 ohlc->setWidth(binSize*0.2);
39 ohlc->setTwoColored(true);
40  
41 // 创建一个坐标轴矩形
42 QCPAxisRect *volumeAxisRect = new QCPAxisRect(customPlot);
43 customPlot->plotLayout()->addElement(1, 0, volumeAxisRect);
44 volumeAxisRect->setMaximumSize(QSize(QWIDGETSIZE_MAX, 100));
45 volumeAxisRect->axis(QCPAxis::atBottom)->setLayer("axes");
46 volumeAxisRect->axis(QCPAxis::atBottom)->grid()->setLayer("grid");
47 // 设置自己构造的坐标轴矩形属性
48 customPlot->plotLayout()->setRowSpacing(0);
49 volumeAxisRect->setAutoMargins(QCP::msLeft|QCP::msRight|QCP::msBottom);
50 volumeAxisRect->setMargins(QMargins(0, 0, 0, 0));
51 // 生成两种颜色的柱状图
52 customPlot->setAutoAddPlottableToLegend(false);//是否自动生成图例
53 QCPBars *volumePos = new QCPBars(volumeAxisRect->axis(QCPAxis::atBottom), volumeAxisRect->axis(QCPAxis::atLeft));
54 QCPBars *volumeNeg = new QCPBars(volumeAxisRect->axis(QCPAxis::atBottom), volumeAxisRect->axis(QCPAxis::atLeft));
55 for (int i=0; i<n/5; ++i)
56 {
57   int v = qrand()%20000+qrand()%20000+qrand()%20000-10000*3;
58   (v < 0 ? volumeNeg : volumePos)->addData(startTime+3600*5.0*i, qAbs(v)); //构造随机数据
59 }
60 volumePos->setWidth(3600*4);
61 volumePos->setPen(Qt::NoPen);
62 volumePos->setBrush(QColor(100, 180, 110));
63 volumeNeg->setWidth(3600*4);
64 volumeNeg->setPen(Qt::NoPen);
65 volumeNeg->setBrush(QColor(180, 90, 90));
66  
67 // 设置自己构造的坐标轴矩形的x轴和QCustomPlot中的坐标轴矩形(默认的会生成一个)x轴同步，两个坐标轴永远显示的坐标范围是一样的
68 connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), volumeAxisRect->axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
69 connect(volumeAxisRect->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis, SLOT(setRange(QCPRange)));
70 // 构造一个新的坐标轴刻度计算类
71 QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
72 dateTimeTicker->setDateTimeSpec(Qt::UTC);
73 dateTimeTicker->setDateTimeFormat("dd. MMMM");
74 volumeAxisRect->axis(QCPAxis::atBottom)->setTicker(dateTimeTicker);//赋予自己构造的坐标轴矩形的x轴一个新的刻度计算类
75 volumeAxisRect->axis(QCPAxis::atBottom)->setTickLabelRotation(15);
76 customPlot->xAxis->setBasePen(Qt::NoPen);
77 customPlot->xAxis->setTickLabels(false);//不显示坐标轴文本
78 customPlot->xAxis->setTicks(false); //  不显示坐标轴  (这个接口实现的不友好，后续文章我会具体说到)
79 customPlot->xAxis->setTicker(dateTimeTicker);//赋予默认的坐标轴矩形的x轴一个新的刻度计算类
80 customPlot->rescaleAxes();
81 customPlot->xAxis->scaleRange(1.025, customPlot->xAxis->range().center());
82 customPlot->yAxis->scaleRange(1.1, customPlot->yAxis->range().center());
83  
84 // 设置两个坐标轴矩形左右对齐
85 QCPMarginGroup *group = new QCPMarginGroup(customPlot);
86 customPlot->axisRect()->setMarginGroup(QCP::msLeft|QCP::msRight, group);
87 volumeAxisRect->setMarginGroup(QCP::msLeft|QCP::msRight, group);
```
```cpp
![技术分享](http://www.qcustomplot.com/images/examples/financialchartsdemo.png)
```
图3 蜡烛图运行示意图
### 三、示例下载
    关于QCustomPlot的系列讲解，我可能会分为7篇文章来分别介绍，分别是QCustomplot使用分享(二) 源码解读、QCustomplot使用分享(三) 图   折线、参数曲线、蜡烛图、柱状图、面积图、QCustomplot使用分享(四) QCPAbstractItem、QCustomplot使用分享(五) 布局、QCustomplot使用分享(六) 坐标轴  网格线和QCustomplot使用分享(七) 层。等到图层讲完之后我会放出一个最终的demo
