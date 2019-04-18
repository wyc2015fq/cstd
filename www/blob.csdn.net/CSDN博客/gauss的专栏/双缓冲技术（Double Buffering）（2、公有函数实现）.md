# 双缓冲技术（Double Buffering）（2、公有函数实现） - gauss的专栏 - CSDN博客
2013年01月06日 23:32:54[gauss](https://me.csdn.net/mathlmx)阅读数：247
#include <QtGui>
#include <cmath>
using namespace std;
#include "plotter.h"
以上代码为文件的开头，在这里把std的名空间加入到当前的全局命名空间。这样在使用<cmath>里的函数时，就不用前缀std::了，如可以直接使用函数floor()，而不用写成std::floor()。
Plotter::Plotter(QWidget *parent) : QWidget(parent)
{
    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
    rubberBandIsShown = false;
    zoomInButton = new QToolButton(this);
    zoomInButton->setIcon(QIcon(":/images/zoomin.png"));
    zoomInButton->adjustSize();
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));
    zoomOutButton = new QToolButton(this);
    zoomOutButton->setIcon(QIcon(":/images/zoomout.png"));
    zoomOutButton->adjustSize();
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));
    setPlotSettings(PlotSettings());
}
在构造函数中，调用setBackGroundRole(QPalette::Dark)，当对控件进行放大需要重新绘制时，提供给Qt一个缺省的颜色填充新的区域，为了能够使用这个机制，还调用了setAutoFillBackground(true)。
函数setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding)让控件在水平和垂直两个方向上都可以进行伸缩。如果控件需要占据屏幕上很大的控件，经常设置这个属性。缺省的设置是两个方向都是QSizePolicy::Preferred，意思是控件的实际尺寸和它的sizeHint一致，控件最小只能缩小到它的最小的sizeHint，并能够无限放大。
调用setFocusPolicy(Qt::StrongFocus)可以使控件通过鼠标点击或者Tab得到焦点。当Plotter控件得到焦点时，它可以接受键盘敲击事件。Plotter控件能够理解一些键盘事件，如+放大，-为缩小，可以向上下左右平移。
Figure 5.9. Scrolling the Plotter widget
![](https://p-blog.csdn.net/images/p_blog_csdn_net/iamdbl/287708/o_5-9.JPG)
在构造函数中，我们还创建了两个QToolButton，每一个按钮都有一个图标。点击这些图标可以放大或者缩小显示的图像。图标保存在资源文件中，为了任何程序都可以使用Plotter控件，需要在.pro添加资源条目：
RESOURCES     = plotter.qrc
资源文件是一个XML格式的文本文件，和在Spreadsheet中使用的很像：
<!DOCTYPE RCC><RCC version="1.0">
<qresource>
    <file>images/zoomin.png</file>
    <file>images/zoomout.png</file>
</qresource>
</RCC>
调用QToolButton::adjustSize()调整按钮的大小为它们的sizeHint。在这里按钮不在布局中，在控件大小改变的时候，又程序计算它们的位置。由于没有了布局管理，因为我们必须在按钮的构造函数中确定按钮的父控件。
调用setPlotSettings()函数用来完成控件的初始化。函数代码如下：
void Plotter::setPlotSettings(const PlotSettings &settings)
{
    zoomStack.clear();
    zoomStack.append(settings);
    curZoom = 0;
    zoomInButton->hide();
    zoomOutButton->hide();
    refreshPixmap();
}
函数setPlotSettings()确定显示控件时的PlotSettings。它在Plotter构造函数中调用，也可以被Plotter的用户调用。开始的时候，Plotter使用的是缺省的放缩值。用户进行放大一次，就有一个新的PlotSettings对象加入到堆栈中。这个堆栈中有两个变量：
       zoomStack是保存PlotSettings对象的一个数组；
       curZoom是当前使用的PlotSettings的一个索引值。
调用setPlotSettings()后，zoomStack中只有一项，zoomIn和zoomOut按钮隐藏。如果我们调用函数zoomIn()和zoomOut()，这两个函数中调用了按钮的show()函数，它们才能显示出来。（通常，调用父控件的show()函数就显示所有的子控件。但是如果我们显式调用了子控件的hide()，必须要显示调用其show()函数显示它，否则就会一直隐藏）
调用refreshPixmap()来更新显示。通常，我们调用update()就可以，这里有些不一样，因为我们要保持QPixmap一直最新的状态。更新了图片后，refreshPixmap()再调用update()把图片显示到控件上。
void Plotter::zoomOut()
{
    if (curZoom > 0) {
        --curZoom;
        zoomOutButton->setEnabled(curZoom > 0);
        zoomInButton->setEnabled(true);
        zoomInButton->show();
        refreshPixmap();
    }
}
如果图片放大了，调用zoomOut()缩小它。它缩小比例系数，如果还能进一步缩小，zoomOut按钮一直有效。显示zoomIn按钮使之按钮有效，调用refreshPixmap()刷新控件。
void Plotter::zoomIn()
{
    if (curZoom < zoomStack.count() - 1) {
        ++curZoom;
        zoomInButton->setEnabled(curZoom < zoomStack.count() - 1);
        zoomOutButton->setEnabled(true);
        zoomOutButton->show();
        refreshPixmap();
    }
}
如果用户放大后又缩小控件，下一个放缩系数的PlotSettings就进入zoomStack。我们就可以再放大控件。
函数zoomIn增加放缩系数，zoomIn按钮显示出来，只要能够放大，按钮会一直有效。同事显示zoomOut按钮使之有效状态。
void Plotter::setCurveData(int id, const QVector<QPointF> &data)
{
    curveMap[id] = data;
   refreshPixmap();
}
函数setCurveData()设置一个指定id的曲线数据。如果曲线中有一个同样的id，那么就用新的数据替代旧数据。如果没有指定的id，则增加一个新的曲线。曲线的数据类型为QMap<int, QVector<QPointF> >
void Plotter::clearCurve(int id)
{
    curveMap.remove(id);
    refreshPixmap();
}
函数clearCurve()删除一个指定id的曲线。
QSize Plotter::minimumSizeHint() const
{
    return QSize(6 * Margin, 4 * Margin);
}
函数minimumSizeHint()和sizeHint()很像，确定控件的理想的尺寸。minimumSizeHint()确定控件的最大尺寸。布局管理器排列控件时不会超过控件的最大尺寸。
由于Margin值为50，所以我们返回的值为300×200，包括四个边界的宽度和Plot本身。如果再小，尺寸太小Plot就不能正常显示了。
QSize Plotter::sizeHint() const
{
    return QSize(12 * Margin, 8 * Margin);
}
在sizeHint()中，我们返回控件的理想尺寸，用Margin常数作为倍数，长宽的比例为3：2，与minimumSizeHint()中比例一致。
以上是Plotter的公有函数和槽函数。
