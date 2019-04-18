# Qt开发：Qt Widgets模块——QDial类 - 一世豁然的专栏 - CSDN博客





2018年07月02日 12:20:51[一世豁然](https://me.csdn.net/Explorer_day)阅读数：709








QDial类提供了一个四舍五入的范围控制（如速度计或电位计）。



头文件：#include <QDial> 

qmake：QT += widgets

继承：QAbstractSlider

当用户需要在程序可定义的范围内控制一个值时使用QDial，并且范围可以环绕（例如，角度从0度到359度测量），或者对话框布局需要一个方形控件。

由于QDial从QAbstractSlider继承，因此拨号的行为与滑块类似。 当wrapping（）为false（默认设置）时，滑块和刻度盘之间没有真正的区别。 它们共享相同的信号，插槽和成员功能。 您使用哪一个取决于您的用户期望和应用程序类型。


在滑块移动时，拨号盘最初会连续发出valueChanged（）信号; 您可以通过禁用跟踪属性来减少发射信号的频率。 即使禁用跟踪，也会连续发出sliderMoved（）信号。


当按下并释放鼠标按钮时，拨号也会发出sliderPressed（）和sliderReleased（）信号。 请注意，由于键盘和滚轮也可用于更改数值，因此表盘的数值可能会发生变化，而不会发出这些信号。


与滑块不同，QDial试图绘制一个“好”数量的凹槽，而不是每行一个。 如果可能，绘制的凹槽数量是每行一步，但如果没有足够的像素来绘制每一个像素，QDial将跳过凹槽尝试绘制一个统一集合（例如，通过绘制每秒或三次凹槽）。


与滑块一样，表盘使QAbstractSlider函数setValue（）可用作插槽。


表盘的键盘界面非常简单：左/上和右/下箭头键通过定义的pageStep定义的singleStep，Page Up和Page Down调整拨号值，Home和End键将值设置为定义的最小值 和最大值。


如果使用鼠标滚轮调整刻度盘，则增量值由wheelSrollLine的较小值（由singleStep和pageStep倍增）确定。


![](https://img-blog.csdn.net/20180702120803851?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


以各种小部件样式显示的拨号（从左到右）：Fusion，Windows Vista，Macintosh。








一、属性文档

1、notchSize : const int



该属性保持当前的缺口大小。

缺口大小在范围控制单元中，而不是像素，并且如果可能的话，它是singleStep（）的倍数，导致在notchTarget（）附近的屏幕上的切口尺寸。

默认情况下，此属性的值为1。

获取函数；

int notchSize() const





2、notchTarget : qreal



该属性拥有陷波之间的目标像素数。

陷波目标是QDial尝试在每个陷波之间放置的像素数量。

实际尺寸可能与目标尺寸不同。

默认陷波目标是3.7像素。

获取函数；

qreal notchTarget() const


void setNotchTarget(double target)




3、notchesVisible : bool



该属性保持是否显示切口。

如果属性为true，则在刻度盘周围绘制一系列凹槽以指示可用值的范围; 否则没有显示缺口。

默认情况下，此属性被禁用。

获取函数；

bool notchesVisible() const


void setNotchesVisible(bool visible)




4、wrapping : bool



该属性保持是否启用包装。

如果为true，则打开包装; 否则在表盘底部插入一些空格以分隔有效值范围的末端。

如果启用，箭头的方向可以是任意角度。 如果禁用，箭头将被限制在拨号盘的上半部分; 如果它旋转到刻度盘底部的空间中，它将被钳位到有效值范围的最近端。

默认情况下，该属性为false。

获取函数；

bool wrapping() const


void setWrapping(bool on)







二、成员函数文档

1、QDial::QDial(QWidget *parent = Q_NULLPTR)

构造一个表盘。

父参数被发送到QAbstractSlider构造函数。





2、QDial::~QDial()

销毁表盘。





3、[virtual protected] bool QDial::event(QEvent *e)

从QObject :: event（）重新实现。





4、[protected] void QDial::initStyleOption(QStyleOptionSlider *option) const

使用此QDial中的值初始化选项。 当他们需要QStyleOptionSlider时，此方法对子类很有用，但不希望自己填写所有信息。





5、[virtual] QSize QDial::minimumSizeHint() const

从QWidget :: minimumSizeHint（）重新实现。





6、[virtual protected] void QDial::mouseMoveEvent(QMouseEvent *e)

从QWidget :: mouseMoveEvent（）重新实现。





7、[virtual protected] void QDial::mousePressEvent(QMouseEvent *e)

从QWidget :: mousePressEvent（）重新实现。





8、[virtual protected] void QDial::mouseReleaseEvent(QMouseEvent *e)

Reimplemented from QWidget::mouseReleaseEvent().





9、[virtual protected] void QDial::paintEvent(QPaintEvent *pe)

从QWidget :: paintEvent（）重新实现。





10、[virtual protected] void QDial::resizeEvent(QResizeEvent *e)

从QWidget :: resizeEvent（）重新实现。





11、[virtual] QSize QDial::sizeHint() const

从QWidget :: sizeHint（）重新实现。





12、[virtual protected] void QDial::sliderChange(SliderChange change)

从QAbstractSlider :: sliderChange（）重新实现。




