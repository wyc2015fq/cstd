# Qwt--散点图/函数图 - wishchinYang的专栏 - CSDN博客
2014年07月11日 09:26:39[wishchin](https://me.csdn.net/wishchin)阅读数：4203
**1.Qwt库**
        QwtPlot是用来绘制二维图像的widget。在它的画板上可以无限制的显示绘画组件。绘画组件可以是曲线（QwtPlotCurve）、标记（QwtPlotMarker）、网格（QwtPlotGrid）、或者其它从QwtPlotItem继承的组件。
**2.简单介绍**：[原文链接，原作者辛苦........](http://blog.csdn.net/evense/article/details/10087501)
# QwtPlot拥有4个axes（轴线）：
|yLeft|Y axis left of the canvas.|
|----|----|
|yRight|Y axis right of the canvas.|
|xBottom|X axis below the canvas.|
|xTop|X axis above the canvas.|
# 常用函数接口：
|setAxisTitle|设置轴标题|
|----|----|
|enableAxis|主要是显示xTop，yRight坐标轴|
|setAxisMaxMajor|设置某个某个坐标轴扩大比例尺的最大间隔数目|
|setAxisMaxMinor|设置某个某个坐标轴缩小比例尺的最大间隔数目|
|setAxisScale|禁用自动缩放比例尺，为某个坐标轴指定一个修改的比例尺|
|insertLegend|添加图例（标注）|
# 常用组件：
|QwtPlotCurve|曲线  可以用曲线来完成散点图等等......|
|----|----|
|QwtPlotMarker|标记|
|QwtPlotGrid|网格|
|QwtPlotHistogram|直方图|
|other|从QwtPlotItem继承的组件|
|QwtPlotItem|plot能显示的类，如果想要实现自己绘画图形，要继承此类实现rtti和draw接口|
|----|----|
|QwtPlotPanner|平移器    （用鼠标左键平移）|
|QwtPlotMagnifier|放大器    （用鼠标滚轮缩放）|
|QwtPlotCanvas|画布|
|QwtScaleMap|比例图---可以提供一个逻辑区域到实际区域的坐标转换|
|QwtScaleWidget|比例窗口|
|QwtScaleDiv|比例布局|
|QwtLegent|标注|
|QwtPlotLayout|布局管理器|
|QwtScaleDraw|自画坐标轴|
**常见接口：**
|setPen|设置画笔|
|----|----|
|setData|设置曲线的数据|
|setStyle|设置曲线形式，点、直线、虚线等等|
|setCurveAttribute|设置曲线属性，一般设置Fitted|
|attch|把曲线附加到QwlPlot上|
下面看一个小例子，结果如下：
![](https://img-my.csdn.net/uploads/201301/15/1358253870_9131.png)
源代码： 
```cpp
#include <QtGui/QApplication>  
#include <Qt/qmath.h>  
#include <QVector>  
#include <qwt_plot.h>  
#include <qwt_plot_curve.h>  
#include <qwt_plot_magnifier.h>  
#include <qwt_plot_panner.h>  
#include <qwt_legend.h>  
  
int main(int argc, char *argv[])  
{  
    QApplication a(argc, argv);  
  
    QwtPlot plot(QwtText("CppQwtExample1"));  
    plot.resize(640,400);  
   
    plot.setAxisTitle(QwtPlot::xBottom, "x->");  //设置坐标轴的名称 
    plot.setAxisTitle(QwtPlot::yLeft, "y->");  
    plot.setAxisScale(QwtPlot::xBottom, 0.0, 2.0 * M_PI);  //设置坐标轴的范围 
    plot.setAxisScale(QwtPlot::yLeft, -1.0, 1.0); 
    plot.insertLegend(new QwtLegend(), QwtPlot::RightLegend); //设置右边标注   
    (void) new QwtPlotMagnifier( plot.canvas() );  //使用滚轮放大/缩小 
    (void) new QwtPlotPanner( plot.canvas() );   //使用鼠标左键平移 
  
    //计算曲线数据  
    QVector<double> xs;  
    QVector<double> ys;  
    for (double x = 0; x < 2.0 * M_PI; x+=(M_PI / 10.0))  {  
        xs.append(x);  
        ys.append(qSin(x));  
    } 
    //构造曲线数据  
    QwtPointArrayData * const data = new QwtPointArrayData(xs, ys);  
    QwtPlotCurve curve("Sine");  
    curve.setData(data);//设置数据  
    curve.setStyle(QwtPlotCurve::Lines);//直线形式  
    curve.setCurveAttribute(QwtPlotCurve::Fitted, true);//是曲线更光滑  
    curve.setPen(QPen(Qt::blue));//设置画笔  
  
    curve.attach(&plot);//把曲线附加到plot上  
    plot.show();  
    return a.exec();  
}
```
**3.使用QWT绘制科学图表、绘图**
解释QwtSimple：simple是qwt自带的例子中最简单的一个， 一共只有一百来行的代码， 实现了数学中的正弦函数(sin())和余弦函数(cos())曲线。
**4.我使用到的两种情况**
**4.1 对函数进行画图----曲线图（这个功能很别致的！）**
```cpp
包含文件：
#include <qlayout.h>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_series_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_text.h>
#include <qwt_math.h>
#include <math.h>
把连续函数定义为全局函数，输入/返回值为double：
double funcMy( doublef)
{
    return 0.15*f;
}
定义序列数据;
class CArraySeriesDat:publicQwtSyntheticPointData
{
public:
    CArraySeriesDat(double(*y)(double)):
        QwtSyntheticPointData(100),d_y(y）
         {   }
    virtual double y(double x) const    
    {   return d_y(x);    }
private:
    double(*d_y)(double);
};
自定义画图类：
class Plot:publicQwtPlot
{
public:
    Plot( QWidget *parent = NULL);
    ~Plot( );
    void drawArray();//画图函数
protected:
    virtual void resizeEvent( QResizeEvent * );
private:
    void populateCos();
    void updateGradient();
};
画图类初始化：
Plot::Plot(QWidget*parent):
    QwtPlot( parent )
{
   // panning with the left mouse button
    (void) new QwtPlotPanner( canvas() );
    // zoom in/out with the wheel
    (void) new QwtPlotMagnifier( canvas() );
    setAutoFillBackground( true );
    setPalette( QPalette( QColor( 165, 193, 228 ) ) );
    updateGradient();
    setTitle("A Simple QwtPlot Demonstration");
    insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    // axes
    setAxisTitle(xBottom, "x -->" );
    setAxisScale(xBottom, 0.0, 10.0);
    setAxisTitle(yLeft, "y -->");
    setAxisScale(yLeft, -1.0, 1.0);
    // canvas
    canvas()->setLineWidth( 1 );
    canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas()->setBorderRadius( 15 );
    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    canvas()->setPalette( canvasPalette );
}
私有成员函数实现：
void Plot::updateGradient()
{
    QPalette pal = palette();
    const QColor buttonColor = pal.color( QPalette::Button );
#ifdef Q_WS_X11
    // Qt 4.7.1: QGradient::StretchToDeviceMode is buggy on X11
    QLinearGradient gradient( rect().topLeft(), rect().bottomLeft() );
    gradient.setColorAt( 0.0, Qt::white );
    gradient.setColorAt( 0.7, buttonColor );
    gradient.setColorAt( 1.0, buttonColor );
#else
    QLinearGradient gradient( 0, 0, 0, 1 );
    gradient.setCoordinateMode( QGradient::StretchToDeviceMode );
    gradient.setColorAt( 0.0, Qt::white );
    gradient.setColorAt( 0.7, buttonColor );
    gradient.setColorAt( 1.0, buttonColor );
#endif
    pal.setBrush( QPalette::Window, gradient );
    setPalette( pal );
}
void Plot::resizeEvent( QResizeEvent *event )
{
    QwtPlot::resizeEvent( event );
#ifdef Q_WS_X11
    updateGradient();
#endif
}
画图函数实现：
void Plot::drawArray()
{
    // Insert new curves
    QwtPlotCurve *cSin = new QwtPlotCurve("y = sin(x)");
    cSin->setRenderHint(QwtPlotItem::RenderAntialiased);
    cSin->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    cSin->setPen(QPen(Qt::red));
    cSin->attach(this);
    QwtPlotCurve *cCos = new QwtPlotCurve("y = cos(x)");
    cCos->setRenderHint(QwtPlotItem::RenderAntialiased);
    cCos->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    cCos->setPen(QPen(Qt::blue));
    cCos->attach(this);
    CArraySeriesDat*  ArraySeriesDat = new CArraySeriesDat(funcMy); //以函数指针的形式获取曲线
    cCos->setData(ArraySeriesDat);
    return;
}
函数测试：
int main(intargc,char**argv)
{
    QApplication a(argc, argv);
    Plot *plot = new Plot();
    // We put a dummy widget around to have
    // so that Qt paints a widget background
    // when resizing
    QWidget window;
    QHBoxLayout *layout = new QHBoxLayout( &window );
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->addWidget( plot );  
    plot->drawArray();//画图...........
    window.resize(600,400);
    window.show();
    return a.exec();
}
```
**4.2 散点图（基本功能）**
**头文件：**
```cpp
class CCruvePlot:publicQwtPlot
{
public:
    CCruvePlot();
    ~CCruvePlot(void);
public:
    void drawPlotCruve();
private:
    QwtPlotCurve *  curve;
    QVector<double> xData;
    QVector<double> yData;
};
//实现文件：
#include "cruvePlot.h"
const int LineNum=7;
const int PointNum=7;
CCruvePlot::CCruvePlot(){}
CCruvePlot::~CCruvePlot(void){}
void CCruvePlot::drawPlotCruve()
{
    //QMessageBox::information(this,"Running！","Running matlab Function.....");
    setTitle("A Simple QwtPlot Demonstration");//设置标题
    insertLegend(new QwtLegend(), QwtPlot::RightLegend);//设置标线的栏
    setAxisTitle(xBottom, "x -->");
    setAxisScale(xBottom, 0.0, 10.0);
    setAxisTitle(yLeft, "y -->");
    setAxisScale(yLeft, 0, 10.0);
    QwtPlotCurve *curve = new QwtPlotCurve("lineFirst");//实例化一条曲线
    curve->attach(this);
    double *x=new double[PointNum];
    double *y=new double[PointNum];
    for(int i=0;i<PointNum;i++) {
      x[i]=i;
      y[i]=i+3;
     }
     curve->setSamples (x,y,PointNum);//传画曲线的数据
     curve->setPen(QPen(Qt::red));
     QwtPlotCurve *curve2 = new QwtPlotCurve("lineSecond");//实例化另一条线
     curve2->attach(this);
     double *x2=new double[PointNum];
     double *y2=new double[PointNum];
     for(int i=0;i<PointNum;i++){
      x2[i]=i*3;
      y2[i]=i+3;
     }
     curve2->setSamples (x2,y2,PointNum);
     curve2->setPen(QPen(Qt::blue));
    return;
}
```
4.3 对于QwtSymbol的使用，详细参考此博客：[http://blog.csdn.net/qustdjx/article/details/7940896](http://blog.csdn.net/qustdjx/article/details/7940896)
[为什么不封装成类似Matlab的用法呢？](http://blog.csdn.net/qustdjx/article/details/7940896)
