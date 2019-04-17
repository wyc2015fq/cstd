# Qt开发：Qt Widgets模块——QAbstractGraphicsShapeItem类 - 一世豁然的专栏 - CSDN博客





2018年05月15日 20:21:07[一世豁然](https://me.csdn.net/Explorer_day)阅读数：196








QAbstractGraphicsShapeItem类为所有路径项目提供了一个公共基础。

头文件：#include <QAbstractGraphicsShapeItem> 

qmake：QT += widgets

继承：QGraphicsItem

被继承：QGraphicsEllipseItem，QGraphicsPathItem，QGraphicsPolygonItem，QGraphicsRectItem和QGraphicsSimpleTextItem

这个类没有完全实现一个项目本身; 特别是它没有实现由QGraphicsItem继承的boundingRect（）和paint（）。


您可以对此项进行子类化，以为项目的笔和笔刷提供访问器的简单基础实现。








一、成员函数

1、QAbstractGraphicsShapeItem::QAbstractGraphicsShapeItem(QGraphicsItem *parent = Q_NULLPTR)

构造一个QAbstractGraphicsShapeItem。 父母被传递给QGraphicsItem的构造函数。





2、QAbstractGraphicsShapeItem::~QAbstractGraphicsShapeItem()

销毁QAbstractGraphicsShapeItem。





3、QBrush QAbstractGraphicsShapeItem::brush() const

如果没有设置画笔，则返回项目的画笔或空画笔。





4、[virtual] bool QAbstractGraphicsShapeItem::isObscuredBy(const QGraphicsItem *item) const

从QGraphicsItem :: isObscuredBy（）重新实现。





5、[virtual] QPainterPath QAbstractGraphicsShapeItem::opaqueArea() const

从QGraphicsItem :: opaqueArea（）重新实现。





6、QPen QAbstractGraphicsShapeItem::pen() const

返回物品的笔。 如果没有设置笔，此函数返回QPen（），一个宽度为1的默认黑色实线笔。





7、void QAbstractGraphicsShapeItem::setBrush(const QBrush &brush)

将项目的画笔设置为画笔。


该物品的画笔用于填充物品。


如果您使用带QGradient的笔刷，则渐变与项目的坐标系相关。





8、void QAbstractGraphicsShapeItem::setPen(const QPen &pen)

将此项目的笔设置为笔。


笔用于绘制项目的轮廓。




