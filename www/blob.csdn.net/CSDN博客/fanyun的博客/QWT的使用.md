# QWT的使用 - fanyun的博客 - CSDN博客
2017年01月25日 09:25:05[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1556
QT下开源图表库比较少,用的比较多的就是QWT, QWT提供的API不够简洁,绘图过程也比较复杂. 这一点不如Java的JFreeChart方便.
一. 编译安装
     QWT最新版本是6.1, QWT5与QWT6一点也不兼容,windows下编译非常简单:
     cd $(QWT_ROOT)
     qmake 
     nmake 
     QWT5.1的版本支持**VC6**编译,QWT6只支持**VS**编译,不能用vc6编译.  
二. 编译运行
     vs编译示例代码,报错: **moc_plot.obj : error LNK2001: 无法解析的外部符号 "public: static struct QMetaObject const QwtPlot::staticMetaObject" (?staticMetaObject@QwtPlot@@2UQMetaObject@@B)**
      解决方案: 在*.pro 加上QWT_DLL宏定义即可
三. 自定义XY轴
     示例代码分析发现,大部分示例xy轴都是默认double数据显示,可以通过子类化QwtScaleDraw来自定义xy轴显示数据,代码如下:
Cpp代码  
- #include <QApplication>
- #include <QTime>
- #include <qwt/qwt_plot.h>
- #include <qwt/qwt_plot_curve.h>
- #include <qwt/qwt_legend.h>
- #include <qwt/qwt_plot_grid.h>
- #include <qwt/qwt_symbol.h>
- #include <qwt/qwt_plot_zoneitem.h>
- #include "plot.h"
- #include <qwt/qwt_picker_machine.h>
- #include <qwt/qwt_scale_draw.h>
- 
- 
- // 自定义y轴的显示时标
- class TimeScaleDraw: public QwtScaleDraw  
- {  
- public:  
-     TimeScaleDraw()  
-     {  
-     }  
- 
- virtual QwtText label( double v ) const
-     {  
-         QDateTime dt;  
-         dt.setTime_t((int)v);  
- return dt.toString("yyyy-MM-dd\n hh:mm:ss");  
-     }  
- private:  
-     QTime baseTime;  
- };  
- 
- 
- int GetTime(const QString &pTime)  
- {  
-     QDateTime dt=QDateTime::fromString(pTime,"yyyy-MM-dd hh:mm:ss");  
- return dt.toTime_t();  
- }  
- 
- int main(int argc,char **argv)  
- {  
-     QApplication app(argc,argv);  
- 
-     QwtPlot plot;  
-     plot.setTitle("Plot Demo");  
-     plot.setCanvasBackground(Qt::white);  
-     plot.setAxisScale(QwtPlot::yLeft,0.0,10.0);  
-     plot.insertLegend(new QwtLegend(),QwtPlot::RightLegend);  
- 
- // 设置自定义的x坐标值(以时标为x轴)
-     plot.setAxisScaleDraw( QwtPlot::xBottom,new TimeScaleDraw());  
-     plot.setAxisScale(QwtPlot::xBottom,GetTime("2013-09-29 00:00:00"), GetTime("2013-09-30 00:00:00"),14400); // 设置起始点
- 
- // 网格
-     QwtPlotGrid *grid=new QwtPlotGrid();  
- //grid->setMajorPen( Qt::white, 0, Qt::SolidLine );
- //grid->setMinorPen( Qt::gray, 0 , Qt::SolidLine );
-     grid->attach(&plot);  
- 
- // 数据标示带
-     QwtPlotZoneItem* zone = new QwtPlotZoneItem();  
-     zone->setPen( Qt::darkGray );  
-     zone->setBrush( QColor( "#834358" ) );  
-     zone->setOrientation( Qt::Horizontal );  
-     zone->setInterval( 3.8, 5.7 );  
-     zone->attach(&plot);  
- 
- // 曲线
-     QwtPlotCurve *curve=new QwtPlotCurve;  
-     curve->setTitle("Some Points");  
-     curve->setPen(Qt::blue,0.5);  
-     curve->setRenderHint(QwtPlotItem::RenderAntialiased,true);  
- 
- // 黄色小圆圈标记曲线上的数据
-     QwtSymbol *symbol=new QwtSymbol(QwtSymbol::Ellipse,QBrush(Qt::yellow),QPen(Qt::red,2),QSize(8,8));  
-     curve->setSymbol(symbol);  
- 
- // 数据源
-     QPolygonF points;  
- /*
-     points << QPointF( 0.0, 4.4 ) <<QPointF(0.6,3.9)<< QPointF( 1.0, 3.0 )
-         << QPointF( 2.0, 4.5 ) << QPointF( 3.0, 6.8 )
-         << QPointF( 4.0, 7.9 ) << QPointF( 5.0, 7.1 );
-     */
- 
-     points<<QPointF(GetTime("2013-09-29 00:00:00"),4.5)  
-         <<QPointF(GetTime("2013-09-29 04:00:00"),3.9)  
-         <<QPointF(GetTime("2013-09-29 08:00:00"),3.0)  
-         <<QPointF(GetTime("2013-09-29 12:00:00"),7.9);  
- 
-     curve->setSamples( points );  
- 
- // 添加曲线
-     curve->attach(&plot);  
- 
- // 曲线跟踪线(拷贝自$(QWT_ROOT)/playground/curvetracker)
-     CurveTracker* tracker = new CurveTracker( plot.canvas() );  
-     tracker->setStateMachine( new QwtPickerTrackerMachine() );  
-     tracker->setRubberBandPen( QPen( "MediumOrchid" ) );  
- 
-     plot.resize(600,400);  
-     plot.show();  
- 
- return app.exec();  
- }  
    运行截图:
![](http://dl2.iteye.com/upload/attachment/0089/9493/8bd1f44a-3550-31ef-83f7-c9f19ac9c465.png)
