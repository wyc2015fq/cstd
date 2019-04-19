# qt之图形视图框架（上） - baidu_33570760的博客 - CSDN博客
2017年05月12日 15:33:50[carman_风](https://me.csdn.net/baidu_33570760)阅读数：2498
## 转载自：https://wizardforcel.gitbooks.io/qt-beginning/content/24.html
## 导语
在前面讲的基本绘图中，我们可以自己绘制各种图形，并且控制它们。但是，如果需要同时绘制很多个相同或不同的图形，并且要控制它们的移动，检测它们的碰撞和叠加；或者我们想让自己绘制的图形可以拖动位置，进行缩放和旋转等操作。实现这些功能，要是还使用以前的方法，那么会十分困难。解决这些问题，可以使用Qt提供的图形视图框架。
图形视图可以对大量定制的2D图形项进行管理和相互作用。视图部件可以让所有图形项可视化，它还提供了缩放和旋转功能。我们在帮助中搜索`Graphics View`关键字，内容如下图：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-1.jpg)
这里一开始对这个框架进行了简单介绍，整个图形视图结构主要包含三部分：场景（`Scene`）、视图（`View`）和图形项（`Item`），它们分别对应
`QGraphicsScene` 、`QGraphicsView` 、QGraphicsItem三个类。其实图形视图框架是一组类的集合，在帮助中可以看到所有与它相关的类。下面我们就开始结合程序对整个框架进行介绍。
环境：Windows Xp + Qt 4.8.4+QtCreator 2.6.2
## 目录
- 一、基本应用
- 二、图形项（QGraphicsItem）
- （一）自定义图形项
- （二）光标和提示
- （三）拖放
- （四）键盘与鼠标事件
- （五）碰撞检测
- （六）移动
- （七）动画
- （八）右键菜单
## 正文
一、基本应用
我们新建空的Qt项目（在其他项目中），项目名称为`graphicsView01`。然后在这个项目中添加新的C++ 源文件，命名为`main.cpp`。
我们将`main.cpp`的内容更改如下。
```cpp
#include <QtGui>
int main(int argc,char* argv[ ])
{
   QApplication app(argc,argv);
   QGraphicsScene *scene = new QGraphicsScene;  //场景
   QGraphicsRectItem *item = new QGraphicsRectItem(100,100,50,50);  //矩形项
   scene->addItem(item);  //项添加到场景
   QGraphicsView *view = new QGraphicsView; //视图
   view->setScene(scene);  //视图关联场景
   view->show();  //显示视图
   return app.exec();
}
```
这里我们建立了一个最简单的基于这个图形视图框架的程序。分别新建了一个场景，一个图形项和一个视图，并将图形项添加到场景中，将视图与场景关联，最后显示视图就可以了。基于这个框架的所有程序都是这样实现的。运行效果如下。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-2.jpg)
就像我们看到的，场景是管理图形项的，所有的图形项必须添加到一个场景中，但是场景本身无法可视化，我们要想看到场景上的内容，必须使用视图。下面我们分别对图形项、场景和视图进行介绍。
二、图形项（`QGraphicsItem`）
`QGraphicsItem`类是所有图形项的基类。图形视图框架对一些典型的形状提供了一些标准的图形项。比如上面我们使用的矩形（`QGraphicsRectItem`）、椭圆（`QGraphicsEllipseItem`）、文本（`QGraphicsTextItem`）等多个图形项。但只有继承`QGraphicsItem` 类实现我们自定义的图形项时，才能显示出这个类的强大。`QGraphicsItem`支持以下功能：
- 鼠标的按下、移动、释放和双击事件，也支持鼠标悬停、滚轮和右键菜单事件。
- 键盘输入焦点和键盘事件
- 拖放
- 利用QGraphicsItemGroup进行分组
- 碰撞检测
（一）自定义图形项
1．在前面的项目中添加新的C++类，类名设为 `MyItem`，基类设为`QGraphicsItem`。
2．然后，我们在`myitem.h`文件中添加头文件`#include <QtGui>`。（说明：`QtGui`模块里面包含了所有图形界面类，所以为了简便，这里只包含了该头文件，正式开发程序时不推荐这么做！）
3．再添加两个函数的声明：
```cpp
QRectFboundingRect() const;
voidpaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*widget);
```
4．下面到`myitem.cpp`中对两个函数进行定义：
```cpp
QRectFMyItem::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(0 - penWidth / 2, 0 -penWidth / 2,
               20 + penWidth, 20 + penWidth);
}
voidMyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
Q_UNUSED(option);  //标明该参数没有使用
    Q_UNUSED(widget);
    painter->setBrush(Qt::red);
    painter->drawRect(0,0,20,20);
}
```
5．下面到`main.cpp`中添加`#include "myitem.h"`
然后将以前那个矩形项的定义语句改为：
```cpp
MyItem *item =new MyItem;
```
运行程序，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-3.jpg)
可以看到，我们要继承`QGraphicsItem`类实现自定义的图形项，必须先实现两个纯虚函数`boundingRect()`和`paint()`，前者用于定义`Item`的绘制范围，后者用于绘制图形项。其实`boundingRect()`还有很多用途，后面会涉及到。
（二）光标和提示
1．在`myitem.cpp` 中的构造函数中添加两行代码，如下：
```cpp
MyItem::MyItem()
{
   setToolTip("Click and drag me!");  //提示
   setCursor(Qt::OpenHandCursor);   //改变光标形状
}
```
然后运行程序，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-4.jpg)
当光标放到小方块上时，光标变为了手型，并且弹出了提示。更多的光标形状可以查看`Qt::CursorShape`，我们也可以使用图片自定义光标形状。
（三）拖放
下面写这样一个程序，有几个不同颜色的圆形和一个大矩形，我们可以拖动圆形到矩形上，从而改变矩形的颜色为该圆形的颜色。
1．将上面的程序进行改进，用来实现圆形图形项。
在`myitem.h`中添加一个私有变量和几个键盘事件处理函数的声明：
```cpp
protected:
   void mousePressEvent(QGraphicsSceneMouseEvent *event);
   void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
   void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
   QColor color;
```
2．然后到`myitem.cpp`中，在构造函数中初始化颜色变量：
```cpp
color = QColor(qrand() % 256, qrand() %256, qrand() % 256); //初始化随机颜色
```
在`paint()`函数中将绘制矩形的代码更改如下：
```cpp
painter->setBrush(color);
painter->drawEllipse(0, 0, 20, 20);
```
3．下面我们定义几个键盘事件处理函数：
```cpp
voidMyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   if(event->button() != Qt::LeftButton)
    {
       event->ignore();  //如果不是鼠标左键按下，则忽略该事件
       return;
    }
   setCursor(Qt::ClosedHandCursor); //如果是鼠标左键按下，改变光标形状
}
voidMyItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
   if(QLineF(event->screenPos(),event->buttonDownScreenPos(Qt::LeftButton))
       .length() < QApplication::startDragDistance())
{
//如果鼠标按下的点到现在的点的距离小于程序默认的拖动距离，表明没有拖动，则返回
       return;
    }
   QDrag *drag = new QDrag(event->widget()); //为event所在窗口部件新建拖动对象
   QMimeData *mime = new QMimeData; //新建QMimeData对象，它用来存储拖动的数据
   drag->setMimeData(mime); //关联
   mime->setColorData(color);  //放入颜色数据
   QPixmap pix(21,21);  //新建QPixmap对象，它用来重新绘制圆形，在拖动时显示
   pix.fill(Qt::white);
   QPainter painter(&pix);
   paint(&painter,0,0);
   drag->setPixmap(pix);
   drag->setHotSpot(QPoint(10, 15)); //我们让指针指向圆形的(10,15)点
   drag->exec();  //开始拖动
   setCursor(Qt::OpenHandCursor);   //改变光标形状
}
voidMyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   setCursor(Qt::OpenHandCursor);   //改变光标形状
}
```
此时运行程序，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-5.jpg)
4．下面我们新添一个类，它用来提供矩形图形项，并且可以接收拖动的数据。在`myitem.h`中，我们加入该类的声明：
```cpp
class RectItem : public QGraphicsItem
{
public:
   RectItem();
   QRectF boundingRect() const;
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event); //拖动进入事件
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event); //拖动离开事件
    void dropEvent(QGraphicsSceneDragDropEvent *event);  //放入事件
private:
    QColor color;
    bool dragOver;  //标志是否有拖动进入
};
```
5．然后进入`myitem.cpp`进行相关函数的定义：
```cpp
RectItem::RectItem()
{
   setAcceptDrops(true);  //设置接收拖放
   color = QColor(Qt::lightGray);
}
QRectF RectItem::boundingRect() const
{
    return QRectF(0, 0, 50, 50);
}
void RectItem::paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(dragOver? color.light(130) : color);  //如果其上有拖动，颜色变亮
   painter->drawRect(0,0,50,50);
}
voidRectItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
   if(event->mimeData()->hasColor()) //如果拖动的数据中有颜色数据，便接收
    {
       event->setAccepted(true);
       dragOver = true;
       update();
    }
   else event->setAccepted(false);
}
voidRectItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
   Q_UNUSED(event);
   dragOver = false;
   update();
}
void RectItem::dropEvent(QGraphicsSceneDragDropEvent*event)
{
   dragOver = false;
if(event->mimeData()->hasColor())
//我们通过类型转换来获得颜色
       color =qVariantValue<QColor>(event->mimeData()->colorData());
   update();
}
```
6．下面进入`main.cpp`文件，更改`main()`函数中的内容如下：
```cpp
int main(int argc,char* argv[ ])
{
   QApplication app(argc,argv);
   qsrand(QTime(0,0,0).secsTo(QTime::currentTime())); //设置随机数初值
   QGraphicsScene *scene = new QGraphicsScene;
   for(int i=0; i<5; i++) //在不同位置新建5个圆形
    {
       MyItem *item = new MyItem;
       item->setPos(i*50+20,100);
       scene->addItem(item);
    }
   RectItem *rect = new RectItem; //新建矩形
   rect->setPos(100,200);
   scene->addItem(rect);
   QGraphicsView *view = new QGraphicsView;
   view->setScene(scene);
   view->resize(400,300); //设置视图大小
   view->show();
   return app.exec();
}
```
这是运行程序，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-6.jpg)
这时我们已经实现了想要的效果。可以看到，要想实现拖放，必须源图形项和目标图形项都进行相关设置。在源图形项的鼠标事件中新建并执行拖动，而在目标图形项中必须指定`setAcceptDrops(true);` 这个函数，这样才能接收拖放，然后需要实现拖放的几个事件处理函数。
（四）键盘与鼠标事件
1．新建项目`graphicsView02`，然后按照（一）中自定义图形项进行操作（可以直接把那里的代码拷贝过来）。下面我们先来看键盘事件。
2．在`myitem.h`文件中声明键盘按下事件处理函数：
```cpp
protected:
voidkeyPressEvent(QKeyEvent *event);
```
然后在`myitem.cpp`中进行定义：
```cpp
void MyItem::keyPressEvent(QKeyEvent*event)
{
   moveBy(0, 10);  //相对现在的位置移动
}
```
这时运行程序，发现无论怎样方块都不会移动。其实要想使图形项接收键盘事件，就必须使其可获得焦点。我们在构造函数里添加一行代码：
```cpp
setFlag(QGraphicsItem::ItemIsFocusable);  //图形项可获得焦点
```
（我们在新建图形项时指定也是可以的，如`item->setFlag(QGraphicsItem::ItemIsFocusable);`）
这时运行程序，然后用鼠标点击一下方块，再按下任意按键，方块就会向下移动。效果如下图所示。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-7.jpg)
3．再看鼠标事件。我们先在`myitem.h`文件中声明鼠标按下事件处理函数：
```cpp
voidmousePressEvent(QGraphicsSceneMouseEvent *event);
```
然后再`myitem.cpp`文件中对其进行定义：
```cpp
voidMyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   moveBy(10,0);
}
```
此时运行程序，点击小方块，它便会向右移动。如果我们想让鼠标可以拖动小方块，那么我们可以重新实现`mouseMoveEvent()`函数，还有一种更简单的方法是，我们在构造函数中指明该图形项是可移动的：
```cpp
setFlag(QGraphicsItem::ItemIsMovable);
```
（当然我们也可以在新建图形项时指定它）
运行程序，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-8.jpg)
（五）碰撞检测
下面先看一个例子，再进行讲解。
我们将上面程序中`myitem.cpp`文件中的`paint()`函数中的设置画刷的代码更改如下：
```cpp
//如果与其他图形项碰撞则显示红色，否则显示绿色
painter->setBrush(!collidingItems().isEmpty()?Qt::red : Qt::green);
```
然后再`main.cpp`文件中在场景中添加一个直线图形项：
```cpp
QGraphicsLineItem *line = newQGraphicsLineItem(0,50,300,50);
scene->addItem(line);
```
这时运行程序，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-9.jpg)
刚开始，方块是绿色的，当我们拖动它与直线相交时，它就变成了红色。
在`QGraphicsItem`类中有三个碰撞检测函数，分别是`collidesWithItem(`)、`collidesWithPath()`和`collidingItems()`，我们使用的是第三个。第一个是该图形项是否与指定的图形项碰撞，第二个是该图形项是否与指定的路径碰撞，第三个是返回所有与该图形项碰撞的图形项的列表。在帮助中我们可以查看它们的函数原型和介绍，这里想说明的是，这三个函数都有一个共同的参数`Qt::ItemSelectionMode`，它指明了怎样去检测碰撞。我们在帮助中进行查看，可以发现它是一个枚举变量，一共有四个值，分别是：
- `Qt::ContainsItemShape`：只有图形项的`shape`被完全包含时；
- `Qt::IntersectsItemShape`：当图形项的`shape`被完全包含时，或者图形项与其边界相交；
- `Qt::ContainsItemBoundingRect`： 只有图形项的`bounding rectangle`被完全包含时；
- `Qt::IntersectsItemBoundingRect`：图形项的`boundingrectangle`被完全包含时，或者图形项与其边界相交。
如果我们不设置该参数，那么他默认使用`Qt::IntersectsItemShape`。这里所说的`shape`是指什么呢？在`QGraphicsItem`类中我们可以找到`shape()`函数，它返回的是一个`QPainterPath`对象，也就是说它能确定我们图形项的形状。但是默认的，它只是返回`boundingRect()`函数返回的矩形的形状。下面我们具体验证一下。
在`main.cpp`函数中添加两行代码：
```cpp
qDebug()<< item->shape();   //输出item的shape信息
qDebug()<< item->boundingRect();  //输出item的boundingRect信息
```
这时运行程序，在下面的程序输出窗口会输出如下信息：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-10.jpg)
我们发现，现在`shape`和`boundingRect`的大小是一样的。这时我们在到`myitem.cpp`中更改函数`boundingRect()`函数中的内容，将大小由20，改为50：
```cpp
return QRectF(0 - penWidth / 2, 0 -penWidth / 2,
               50 + penWidth, 50 + penWidth);
```
这时再次运行程序，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-11.jpg)
小方块一出来便成为了红色，下面的输出信息也显示了，现在`shape`的大小也变成了50。怎样才能使小方块按照它本身的形状，而不是其`boundingRect`的大小来进行碰撞检测呢？我们需要重新实现`shape()`函数。
在`myitem.h`中，我们在`public`里进行函数声明：`QPainterPath shape() const;`
然后到`myitem.cpp`中进行其定义：
```cpp
QPainterPath MyItem::shape() const
{
   QPainterPath path;
   path.addRect(0,0,20,20);  //图形项的真实大小
   return path;
}
```
这时我们再运行程序，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-12.jpg)
可以看到，现在`shape`和`boundingRect`的大小已经不同了。所以对于不是矩形的形状，我们都可以利用`shape()`函数来返回它的真实形状。
（六）移动
对于图形项的移动，我们有很多办法实现，也可以在很多层面上对其进行控制，比如说在`View`上控制或者在`Scene`上控制。但是对于大量的不同类型的图形项，怎样能一起控制呢？在图形视图框架中提供了`advance()`槽函数，这个函数在`QGraphicsScene`和`QGraphicsItem`中都有，在图形项类中它的原型是
```
advance(int
 phase)
```
。它的实现流程是，我们利用`QGraphicsScene`类的对象调用`QGraphicsScene`的`advance()`函数，这时就会执行两次该场景中所有图形项的`advance(int phase)`函数，第一次`phase`为0，告诉所有图形项即将要移动；第二次`phase`的值为1，这时执行移动。下面我们看一个例子。
我们在`myitem.h`中的`protected`中声明函数：`void advance(int phase);`
然后在`myitem.cpp`中对其进行定义：
```cpp
void MyItem::advance(int phase)
{
   if(!phase) return;  //如果phase为0，则返回
   moveBy(0,10);
}
```
在到`main.cpp`中添加以下代码：
```cpp
QTimer timer;
QObject::connect(&timer, SIGNAL(timeout()),scene, SLOT(advance()));
timer.start(1000);
```
这时运行程序，小方块就会每秒下移一下。
（七）动画
其实实现图形项的动画效果，也可以在不同的层面进行。如果我们只想控制一两个图形项的动画，一般在场景或视图中实现。但是要是想让一个图形项类的多个对象都进行同样的动画，那么我们就可以在图形项类中进行实现。我们先看一个例子。
在`myitem.cpp`文件中的构造函数中添加代码：
```cpp
MyItem::MyItem()
{
   setFlag(QGraphicsItem::ItemIsFocusable); //图形项可获得焦点
   setFlag(QGraphicsItem::ItemIsMovable); //图形项可移动
   QGraphicsItemAnimation *anim = new QGraphicsItemAnimation; //新建动画类对象
   anim->setItem(this);  //将该图形项加入动画类对象中
   QTimeLine *timeLine = new QTimeLine(1000);  //新建长为1秒的时间线
   timeLine->setLoopCount(0);  //动画循环次数为0，表示无限循环
   anim->setTimeLine(timeLine); //将时间线加入动画类对象中
   anim->setRotationAt(0.5,180); //在动画时间的一半时图形项旋转180度
   anim->setRotationAt(1,360);  //在动画执行完时图形项旋转360度
   timeLine->start();  //开始动画
}
```
这时执行程序，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-13.jpg)
小方块会在一秒内旋转一圈。我们这里使用了`QGraphicsItemAnimation`动画类和`QTimeLine`时间线类，关于这些内容我们会在后面的动画框架中细讲，所以在这里就不再介绍。
（八）右键菜单
图形项支持右键菜单，不过`QGraphicsItem`类并不是从`QObject`类继承而来的，所以它默认不能使用信号和槽机制，为了能使用信号和槽，我们需要将我们的`MyItem`类进行多重继承。
在`myitem.h`中，将`MyItem`类改为
```cpp
class MyItem : public QObject, publicQGraphicsItem
{
    Q_OBJECT    //进行宏定义
    … …
}
```
这样我们就可以使用信号和槽机制了，这时我们在下面添加一个槽：
```cpp
public slots:
   void moveTo(){setPos(0,0);}
```
因为其实现的功能很简单，我们在声明它的同时进行了定义，就是让图形项移动到`(0,0)`点。然后我们在`protected`中声明右键菜单事件处理函数：
```cpp
voidcontextMenuEvent(QGraphicsSceneContextMenuEvent *event);
```
最后我们在`myitem.cpp`文件中对该事件处理函数进行定义：
```cpp
voidMyItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   QMenu menu;
   QAction *action = menu.addAction("moveTo(0,0)");
   connect(action,SIGNAL(triggered()),this,SLOT(moveTo()));
   menu.exec(event->screenPos()); //在按下鼠标左键的地方弹出菜单
}
```
这里我们只是设置了一个菜单，当按下该菜单是，图形项移动到`(0,0)`点。
我们运行程序，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/19-14.jpg)
## 结语
这一节先介绍了图形项的相关内容，而场景、视图等内容放到下一节来讲。
涉及到的源码:
- [graphicsView01.zip](http://bbs.qter.org/forum.php?mod=attachment&aid=MjYyfDg1ZjU5M2FhfDE0NzE2OTIxODd8MTA5NTl8MTI1)
- [graphicsView02.zip](http://bbs.qter.org/forum.php?mod=attachment&aid=MjYzfDU0NmU0Mjc0fDE0NzE2OTIxODd8MTA5NTl8MTI1)
