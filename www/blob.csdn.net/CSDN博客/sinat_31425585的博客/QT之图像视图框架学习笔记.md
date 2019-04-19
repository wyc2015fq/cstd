# QT之图像视图框架学习笔记 - sinat_31425585的博客 - CSDN博客
2018年08月29日 11:47:11[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：154
     图形视图框架提供了一个基于图形项的模型视图编程方法，它主要由场景、视图和图形项这三个部分组成，这三个部分分别由QGraphicsScene、QGraphicsView和QGraphicsItem三个类表示。多个视图可以查看同一个场景，场景包含各种各样的几何形状的图形项。
    1、场景
    QGraohicsScene提供了图形视图框架的场景，场景拥有以下功能：
    （1）提供用于管理图形项的快速接口；
   （2）传播事件给每一个图形项；
    （3）管理图形项的状态，例如，选择和焦点处理；
    （4）提供无变换的渲染功能，主要用于打印。
    场景是图形项QGraphicsItem对象的容器。可以调用一下几个函数对图形项进行操作：
```
QGraphicsScene::addItem()函数将图形项添加到场景中；
    QGraphicsScene::item()函数和其他几个重载函数可以返回符合条件的所有图形项，这些图形项不是与指定的点、矩形、多边形或矢量路径相交，就是包含在它们之中；
    QGraphicsScene::itemAt()函数返回指定点的最上面图形项，所有图形项发现函数返回的图形项都是使用递减顺序；
   QGraphicsScene::RemoveItem(): 移除一个图形项；
   QGraphicsScene::setSelectionArea()：需要传入一个形状来选取场景中指定的图形项；
   QGraphicsScene::selectedItem()：选取当前选取的所有图形项列表；
   QGraphicsScene::setFocus()或QGraphicsScene::focusItem()：为一个图形项聚焦；
  
   QGraphicScene::render()：将场景中一部分渲染到一个绘图设备上。
```
 一个小例程：
```cpp
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QDebug>
int main(int argc, cha* argv[])
{
    QApplication app(argc, argv);
    // 新建场景
    QGraphicsScene scene;
    // 创建矩形图形项
    QGraphicsItem * item = new QGraphicsRectItem(0, 0, 100, 100);
    // 将图形项添加到场景中
    scene.addItem(item);
    // 输出(50, 50)处的图形
    qdebug() << scene.itemAt(50, 50);
    return app.exec();
}
```
    由于没有提供视图，所以不会出现任何图形界面。
    2、视图
    QGraphicsView提供了视图部件，可用来使场景中的内容可视化。可以连接多个视图到同一个场景来为相同的数据集提供多个视口。视图部件是一个可滚动的区域，提供了一个滚动条来浏览大的场景，可以用setDragMode()函数设置拖动场景过程中的光标形状，例如：
     QGraphicsView::ScrollHandDrag：手掌形状   QGraphicsView::RubberBandDrag：橡皮筋形状
     默认QGraphicsView还提供了一个QWidget作为视口部件，如果需要用OpenGL进行渲染，可以调用：
     QGraphicsView::setViewport()：设置QGLWidget作为视口。QGraphicsView会获取视口的拥有权(ownership)；
     QGraphicsView::transform()：使用视图来变换场景的坐标系统，以便实现缩放、旋转等高级功能。
    对上面例程进行改进，加入QGraphicsView有：
```cpp
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QGraphicsView>
#include <QTransform>
int main(int argc,char* argv[ ])
{
    QApplication app(argc,argv);
    //新建场景
    QGraphicsScene scene;
    //创建矩形图形项
    QGraphicsRectItem *item = new QGraphicsRectItem(0, 0, 100, 100);
    //将图形项添加到场景中
    QTransform transform;
    scene.addItem(item);
    //输出(50, 50)点处的图形项
    qDebug() << scene.itemAt(50, 50, transform);
    //为场景创建视图
    QGraphicsView view(&scene);
    //设置场景的前景色
    view.setForegroundBrush(QColor(255, 255, 255, 100));
    //设置场景的背景图片
    view.setBackgroundBrush(QPixmap("../myScene/background.png"));
    view.resize(400, 300);
    view.show();
    return app.exec();
}
```
        其实也可以使用场景scene设置背景和前景，这样就使得对所有关联该场景scene的视图都有效，而QGraphicsView对象设置的前景或背景，只对它本身对应视图有效，这种方式有一个好处就是，可以在多个视图中使用不同背景和前景来实现特定的效果。
     3、图形项
    QGraphicsItem是场景中图形项的基类。图形视图框架为经典的形状提供了标准的图形项，例如矩形（QGraphicsRectItem）、椭圆（QGraphicsEllipseItem）、文本（QGraphicsTextItem）。QGraphicsItem主要支持如下功能：
      （1）键盘按下、移动、释放、双击、悬停、滚轮和右键菜单事件；
      （2）键盘输入焦点、键盘事件；
      （3）拖放事件；
      （4）分组，使用QGraphicsGroup通过parent-child关系来实现；
      （5）碰撞检测。
      除此之外，图像项还可以存储自定义数据，使用setData()进行数据存储，然后使用data()获取其中的数据。
     例程如下：
```cpp
// myItem.h
#ifndef MYITEM_H
#define MYITEM_H
#include <QGraphicsItem>
class MyItem : public QGraphicsItem
{
public:
    MyItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
};
#endif // MYITEM_H
```
```cpp
// myItem.cpp
#include "myitem.h"
#include <QPainter>
MyItem::MyItem()
{
}
QRectF MyItem::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(0 - penWidth / 2, 0 - penWidth / 2,
                  20 + penWidth, 20 + penWidth);
}
void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    painter->setBrush(Qt::red);
    painter->drawRect(0, 0, 20, 20);
}
```
```cpp
// main.cpp
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QGraphicsView>
#include <QTransform>
#include "myitem.h"
int main(int argc,char* argv[ ])
{
    QApplication app(argc,argv);
    //新建场景
    QGraphicsScene scene;
    //创建矩形图形项
    //QGraphicsRectItem *item = new QGraphicsRectItem(0, 0, 100, 100);
    MyItem *item = new MyItem;
    //将图形项添加到场景中
    scene.addItem(item);
    //输出(50, 50)点处的图形项
    QTransform transform;
    qDebug() << scene.itemAt(50, 50, transform);
    //为场景创建视图
    QGraphicsView view(&scene);
    //设置场景的前景色
    view.setForegroundBrush(QColor(255, 255, 255, 100));
    //设置场景的背景图片
    view.setBackgroundBrush(QPixmap("../myScene/background.png"));
    view.resize(400, 300);
    view.show();
    return app.exec();
}
```
效果如下：
![](https://img-blog.csdn.net/2018083014540695?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考资料：
Qt Creator 快速入门第二版
