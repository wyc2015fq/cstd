# Qt深入浅出（十四）图形视图框架 - qq769651718的专栏 - CSDN博客












2018年02月24日 00:37:52[吓人的猿](https://me.csdn.net/qq769651718)阅读数：951








# 图形视图框架

​	 使用QPushButton、QLabel、QCheckBox等构成GUI的控件或自定义图形时，开发应用程序会变得很简单。

但是如果想在GUI中使用数十个或者数百个图形对象,向用户完美展示控制场景，则会受到很多的限制。



    	图形视图框架用来管理2D图形项，支持绘制、缩放、事件响应等等。

​	 1、快速提供并管理大量对象

​	2、将事件传递到每一个对象

​	3、管理焦点处理或对象选择等状态



    	图形视图框架是按照MVC设计模式绘图， MVC设计模式包括三个元素：数据的模型(Model)，用户界面的视图(View) ，用户再界面上的操作控制Controller。


- 
QGraphicsView


  	QGraphicsView 是为了在场景上显示图元而提供的类。QGraphicsView包括可视控件区域和用于显示大场景滚动区域,可以接受用户输入事件。QGraphicsView间接继承至QWidget。




- 
QGraphicsScene


 	QGraphicsScene类可以保存图元，也可以处理用户输入事件。是图形对象QGraphicsItem的容器，为管理大量的items提供一个快速的接口。QGraphicsScene只继承自QObject，所以本身是不可见的，必须通过与之相连的视图类QGraphicsView来显示.


- 
QGraphicsItem


 	QGraphicsItem是为了在图形视图上实现图形对象而提供的类。支持鼠标、键盘、焦点事件，支持拖放，在它的基础上可以继承出各种图元类。支持碰撞检测collision detection.



## 1 QGraphicsScene的常用函数


- 
常用添加图元函数


```cpp
QGraphicsScene::setBackgroundBrush   //填充背景色
​
QGraphicsScene::setForegroundBrush   //填充前景色
​
QGraphicsScene::addSimpleText        //添加简单文本
​
QGraphicsScene::addLine             //添加直线
​
QGraphicsScene::addRect             //添加矩形
​
QGraphicsScene::addEllipse          //添加椭圆
​
QGraphicsScene::addWidget           //添加窗口
​
QGraphicsScene::addPixmap           //添加图片
```



例如:

```cpp
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QLabel>
​
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QGraphicsView view;
    QGraphicsScene scene;
    view.setScene(&scene);
    view.show();
    view.resize(400, 400);
​
    /*设置场景的背景色前景色*/
    scene.setBackgroundBrush(QBrush(Qt::red));
    scene.setForegroundBrush(QBrush(QColor(0, 255, 0, 50)));
​
    /*添加线*/
    scene.addLine(0, 0, 100, 100, QPen(Qt::black));
​
    /*添加矩形*/
    scene.addRect(0, 100, 100, 100, QPen(Qt::yellow), QBrush(Qt::blue));
​
    /*添加椭圆*/
    scene.addEllipse(100, 0, 100, 100, QPen(Qt::red), QBrush(Qt::green));
    /*添加简单文本,并且设置文本字体,并且描边*/
    scene.addSimpleText("hello", QFont("system", 40))
            ->setPen(QPen(QBrush(Qt::yellow), 3));
​
    /*添加图片,并且移动位置*/
    scene.addPixmap(QPixmap("E:\\qt_workspace\\pic\\wallet.png"))
            ->setPos(200, 200);
​
    /*添加一个窗口*/
    QLabel label("widget");
    scene.addWidget(&label);
  
    return app.exec();
}
```


- 
操作图元函数


```cpp
QGraphicsScene::itemAt          //查找场景某个中最表层的item
​
QGraphicsScene::setSelectionArea   //设置选定区域
​
QGraphicsScene::setSceneRect         //设置场景的区域大小
​
QGraphicsScene::itemsBoundingRect    //根据所有的item计算区域大小
​
QGraphicsScene:: selectedItems  //获取被选中的item,item必须为可选QGraphicsItem::ItemIsSelectable
```





## 2 QGraphicsItem的常用函数

```cpp
QGraphicsItem::rect                     //不带边框的图形区域 
QGraphicsItem::boundingRect             //带边框的图形区域
QGraphicsItem::collidesWithItem         //碰撞检测
QGraphicsItem::setScale                 //缩放
QGraphicsItem::setRotation              //旋转
QGraphicsItem::setZValue                //设置z坐标,图元的叠加先后顺序可以用它来设置
QGraphicsItem::setPos                   //设置位置坐标
```


- 
设置item的属性


```cpp
void QGraphicsItem::setFlags(GraphicsItemFlags flags);
/*
*参数GraphicsItemFlags flags 为枚举类型，可以以下值
*   QGraphicsItem::ItemIsMovable  是否可以移动
*   QGraphicsItem::ItemIsSelectable 是否可以被选中
*   QGraphicsItem::ItemIsFocusable 是否可以设置为焦点item
*/
```



## 3 图形视图的坐标系



### 3.1 QGraphscItem图元坐标系

​	图元对象都有自身的坐标系，坐标系以(0,0)为坐标原点，自左向右递增是x轴，自上而下递增是y轴，而且所有图元对象的移动转换作用点都是(0, 0)，坐标值可以是浮点型数值。





### 3.2  QGraphicsScene场景坐标系

 	场景坐标系是所有图元对象的绝对坐标. 有四个象限。坐标系以(0,0)为坐标原点，自左向右递增是x轴，自上而下递增是y轴，坐标值可以是浮点型数值。







### 3.3  QGraphicsView视图坐标系



 	 视图坐标是窗口坐标。视图中的坐标单位1个像素。





### 3.4 坐标映射


- 
父子图元坐标映射


```cpp
QGraphicsItem::mapToParent     //子图元坐标映射到父图元
QGraphicsItem::mapFromParent   //父图元坐标映射到子图元
```




- 
图元与场景坐标映射


```cpp
QGraphicsItem::mapToScene     //图元到场景
QGraphicsItem::mapFromScene   //场景到图元
```
- 
场景与视图坐标映射


```cpp
QGraphicsView::mapFromScene  //场景到视图
QGraphicsView::mapToScene    //视图到场景
```



### 3.5 调整坐标系

​	调整坐标系的的类是QTransform、QMatrix，这两个类都提供了缩放、旋转、变形坐标系的方法。

#### 3.5.1 调整QGraphicsVeiw坐标系

```cpp
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QLabel>
#include <QTransform>
​
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QGraphicsView view;
    QGraphicsScene scene;
    view.setScene(&scene);
    view.show();
    view.resize(400, 400);
    QTransform transform;
​
    //transform.translate(100, 0); //移动坐标原点
    transform.scale(1.5, 1);  //缩放
    //transform.rotate(10);    //旋转
   // transform.shear(0.1, 0.1); //变形
​
    view.setTransform(transform);
​
    /*添加矩形*/
    scene.addRect(0, 100, 100, 100, QPen(Qt::yellow), QBrush(Qt::blue));
    return app.exec();
}
​
```



​	

#### 3.5.2  调整QGraphicsItem坐标系



```cpp
#include <QApplication>
#include <QGraphicsView>
#include <QTransform>
#include <QGraphicsRectItem>
​
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QGraphicsView view;
    QGraphicsScene scene;
    view.setScene(&scene);
    view.show();
    view.resize(400, 400);
    QTransform transform;
​
    //transform.translate(100, 0); //移动坐标原点
    transform.scale(1.5, 1);  //缩放
    //transform.rotate(10);    //旋转
   // transform.shear(0.1, 0.1); //变形
    /*添加矩形*/
    QGraphicsRectItem *rect = scene.addRect(0, 100, 100, 100
                                  , QPen(Qt::yellow),  QBrush(Qt::blue));
​
    rect->setTransform(transform);
​
    return app.exec();
}
```



## 4  图形视图的事件处理

​	QGraphicsView是窗口类，那么它当然可以处理鼠标、键盘等事件。QGraphicsScene

不是窗口类，但是它也能处理鼠标、键盘等窗口类的事件。

​	QGraphicsView收到event,就会转换成QGraphicsScene的事件，QGraphicsScene对象再传递给对应的QGraphicsItem。



### 4.1 QGraphicsView的事件处理

​	QGraphicsView的事件处理机制与普通的QWidget相同。

```cpp
#include "widget.h"
#include <QDebug>
​
GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
​
}
​
void GraphicsView::mousePressEvent(QMouseEvent* ev)
{
    qDebug() << "GraphicsView" << __FUNCTION__ << endl;
    /*必须传递到基类的mousePressEvent,否则QGraphicsScene无法收到消息*/
    QGraphicsView::mousePressEvent(ev);
}
​
GraphicsView::~GraphicsView()
{
​
}
```



### 4.2  QGraphicsScene的事件处理

​	QGraphicsScene的事件处理机制与QWidget不同, 使用QGraphicsSceneEvent来传递事件。



例如，鼠标点击事件的虚函数为:

```cpp
[virtual protected] void QGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
```

scene.cpp

```cpp
#include "scene.h"
#include <QDebug>
​
MyScene::MyScene(QObject * parent) : QGraphicsScene(parent)
{
​
}
​
void MyScene::mousePressEvent(QGraphicsSceneMouseEvent* ev)
{
    qDebug() << "GraphicsScene" << __FUNCTION__ << endl;
    /*必须传递到基类的mousePressEvent中,否则QGraphicsItem无法收到消息*/
    QGraphicsScene::mousePressEvent(ev);
}
```





### 4.3 QGraphicsItem的事件处理

​	QGraphicsItem的事件处理函数传递事件也是使用的QGraphicsSceneEvent。
- 
常用的事件处理函数有:


```cpp
//鼠标按下事件，场景上面的鼠标事件的触发位置坐标需要使用mouseEvent->scenePos()来获取
[virtual protected] void QGraphicsScene::mousePressEvent(
        QGraphicsSceneMouseEvent *mouseEvent)
//鼠标释放事件
[virtual protected] void QGraphicsScene::mouseReleaseEvent(
        QGraphicsSceneMouseEvent *mouseEvent)
​
//鼠标移动事件
​
[virtual protected] void QGraphicsScene::mouseMoveEvent(
        QGraphicsSceneMouseEvent *mouseEvent)
​
//鼠标滚轮事件
[virtual protected] void QGraphicsScene::wheelEvent(
        QGraphicsSceneWheelEvent *wheelEvent)
​
​
/*注意:先使用void QGraphicsItem::setAcceptHoverEvents(bool enabled),才能接收到hoverevent*/
​
//鼠标进入事件
[virtual protected] void QGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
​
//鼠标离开事件
[virtual protected] void QGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
​
//鼠标不点击移动事件
[virtual protected] void QGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
  
/*键盘按下事件 setFlags(QGraphicsItem::ItemIsFocusable); 再调用setFocus才能接受键盘事件*/
​
[virtual protected] void QGraphicsItem::keyPressEvent(QKeyEvent *event)
​
//键盘释放事件
[virtual protected] void QGraphicsItem::keyReleaseEvent(QKeyEvent *event)
​
//绘画事件
[pure virtual] void QGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR)
```





## 5  图形视图的内存管理

​	QGraphicsView通过指定父窗口托管内存, QGraphicsScene通过指定父QObject对象托管内存。QGraphicsItem 通过指定父QGraphicsItem对象托管内存, 顶级QGraphicsItem对象通过QGraphicsScene对象托管。







## 6 图形视图碰撞检测

​	所谓的碰撞检测，就是判断某个点是否在图元对象中，或者两个图元是否有接触。

```cpp
//判断QGraphicsItem坐标系中某点， 是否包含在QGraphicsItem中，需要将点转换成QGraphicsItem坐标
QGraphicsItem::contains()
  
//item的矩形边框
QGraphicsItem::boundingRect()
  
//返回item的形状path，可以是任何形状。
QGraphicsItem::shape() 
  
//判断是否与某个item碰撞
QGraphicsItem::collidesWithItem
```



 	QGraphicsScene认为所有图元的boundingRect函数与shape函数都是不发生改变的，除非用户进行通知。

 如果你想改变一个图元的范围，必需先调用prepareGeometryChange以允许QGraphicsScene进行更新。





## 7 背景缓冲

​	如果场景的背景需要大量耗时的渲染，可以利用背景缓存，当下次需要渲染背景时，可以快速进行渲染。它的原理就是，把整个视口先绘制到一个QPixmap上。

​	但是这个只适合较小的视口，也就是说，如果视图窗口很大，而且有滚动条，那么就不再适合缓存背景。



​	我们可以使用setCacheMode(QGraphicsView::CacheBackground)来设置背景缓存，默认设置是没有缓存QGraphicsView::CacheNone。



```cpp
void QGraphicsView::setCacheMode(CacheMode mode);
```





## 8 打印QGraphicsScene

​	将整个场景打印到一个QPainterDevice的派生类对象中，例如QPixmap。

​	main.cpp

```cpp
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsRectItem>
​
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QGraphicsView view;
    QGraphicsScene scene;
    view.setScene(&scene);
    view.show();
    view.resize(400, 400);
​
    scene.addRect(0, 0, 100, 100, QPen(Qt::red), QBrush(Qt::yellow));
    QPixmap pixmap(scene.sceneRect().width(), scene.sceneRect().height());
    pixmap.fill(QColor(0, 0, 0, 0));
    QPainter painter;
    painter.begin(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    scene.render(&painter);
    painter.end();
    pixmap.save("d:\\123.png");
​
    return app.exec();
}
```



## 9 图形效果叠加

​	Qt中可以给图元添加特效，QGraphicsEffect为特效基本类。

​	
- 
有以下具体特效


```cpp
QGraphicsBlurEffect   //模糊效果
QGraphicsDropShadowEffect //阴影效果
QGraphicsColorizeEffect //使用色调的着色效果
QGraphicsOpacityEffect //透明效果
```



​	例如：main.cpp

```cpp
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsBlurEffect>
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QGraphicsView view;
    QGraphicsScene scene;
    view.setScene(&scene);
    view.show();
    view.resize(400, 400);
    QGraphicsBlurEffect effect;
    effect.setBlurRadius(10);
    QGraphicsRectItem* rect = scene.addRect(0, 0, 100, 100, QPen(Qt::red), QBrush(Qt::yellow));
    rect->setGraphicsEffect(&effect);
​
    return app.exec();
}
```






