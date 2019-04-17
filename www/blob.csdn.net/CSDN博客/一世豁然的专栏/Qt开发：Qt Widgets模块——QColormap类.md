# Qt开发：Qt Widgets模块——QColormap类 - 一世豁然的专栏 - CSDN博客





2018年05月21日 19:38:02[一世豁然](https://me.csdn.net/Explorer_day)阅读数：548








QColormap类将设备独立的QColors映射到与设备相关的像素值。



头文件：#include <QColormap> 

qmake：QT += widgets







一、成员类型

1、enum QColormap::Mode

此枚举描述了QColormap如何将设备无关的RGB值映射到与设备相关的像素值。

|内容|值|描述|
|----|----|----|
|QColormap::Direct|0|像素值直接来自RGB值，也称为“真彩色”。|
|QColormap::Indexed|1|像素值代表可用颜色向量中的索引，即QColormap使用最接近RGB值的颜色索引。|
|QColormap::Gray|2|与索引类似，像素值表示可用灰色色调的矢量。 QColormap使用最接近RGB值计算灰度的灰色调的索引。|




二、成员函数

1、QColormap::QColormap(const QColormap &colormap)

构造另一个色彩映射的副本。





2、QColormap::~QColormap()

销毁色彩地图。





3、const QColor QColormap::colorAt(uint pixel) const

返回像素的QColor。





4、const QVector<QColor> QColormap::colormap() const

返回代表索引和灰度模式的设备色彩图的颜色矢量。 此函数为直接模式返回空向量。





5、int QColormap::depth() const

返回设备的深度。





6、[static] QColormap QColormap::instance(int screen = -1)

返回指定屏幕的色彩图。 如果屏幕为-1，则此函数返回默认屏幕的色彩图。





7、Mode QColormap::mode() const

返回此颜色映射的模式。





8、uint QColormap::pixel(const QColor &color) const

返回颜色的设备相关像素值。





9、int QColormap::size() const

返回索引和灰度模式的颜色映射的大小; 直接模式返回-1。





10、QColormap &QColormap::operator=(const QColormap &colormap)

将给定的颜色表分配给该颜色贴图并返回对该颜色贴图的引用。

这个函数是在Qt 4.2中引入的。




