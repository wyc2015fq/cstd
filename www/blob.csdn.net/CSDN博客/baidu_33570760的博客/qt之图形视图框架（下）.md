# qt之图形视图框架（下） - baidu_33570760的博客 - CSDN博客
2017年05月12日 15:35:30[carman_风](https://me.csdn.net/baidu_33570760)阅读数：1854
转载自：https://wizardforcel.gitbooks.io/qt-beginning/content/24.html
导语
环境：Windows Xp + Qt 4.8.4+QtCreator 2.6.2
## 目录
- 三、场景（QGraphicsScene）
- （一）场景层
- （二）索引算法
- （三）边界矩形
- （四）图形项查找
- （五）事件处理和传播
- （六）打印
- 四、视图（QGraphicsView）
- （一）缩放与旋转
- （二）场景边框与场景对齐方式
- （三）拖动模式
- （四）事件传递
- （五）背景缓存
- （六）OpenGL渲染
- （七）图形项查找与图形项组
- （八）打印
## 正文
三、场景（`QGraphicsScene`）
`QGraphicsScene`提供了图形视图框架的场景，它有以下功能：
- 提供了一个管理大量图形项的快速接口
- 向每个图形项传播事件
- 管理图形项的状态，比如选择和焦点处理
- 提供无转换的渲染功能，主要用于打印
我们新建空的Qt项目，项目名称为`graphicsView03`，完成后添加`main.cpp`文件，更改其内容如下：
```cpp
#include <QtGui>
int main(int argc,char* argv[ ])
{
   QApplication app(argc,argv);
   QGraphicsScene scene;
   scene.addText("Hello, world!");
   QGraphicsView view(&scene);
   view.show();
   return app.exec();
}
```
运行程序，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-1.jpg)
这里使用`addText()`函数添加了一个文本图形项。执行这条语句就相当于执行了下面两条语句：
```cpp
QGraphicsTextItem*item = new QGraphicsTextItem("Hello, world!");
scene.addItem(item);
```
如果要删除一个图形项我们可以调用`removeItem()`函数，如：`scene.removeItem(item);`
（一）场景层
一个场景分为三个层：图形项层（`ItemLayer`）、前景层（`ForegroundLayer`）和背景层（`BackgroundLayer`）。场景的绘制总是从背景层开始，然后是图形项层，最后是前景层。看下面的例子：
我们在上面的程序中添加代码：
```cpp
scene.setForegroundBrush(QColor(255,255,255,100));
scene.setBackgroundBrush(Qt::green);
```
运行程序，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-2.jpg)
对于前景层，我们一般不进行设置，或者像上面这样设置为半透明的白色。对于背景层，这里设置为了绿色，当然，我们也可以将一张图片设置为背景。
```cpp
scene.setBackgroundBrush(QPixmap("../graphicsView03/yafeilinux.jpg"));
```
运行程序，我们可以看到，图片默认是平铺的。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-3.jpg)
如果想进一步控制前景和背景层，我们可以重新实现`drawForeground()`函数和`drawBackground()`函数。
（二）索引算法
索引算法，是指在场景中进行图形项查找的算法。`QGraphicsScene`中提供了两种选择，它们在一个枚举变量`QGraphicsScene::ItemIndexMethod`中，分别是：
- `QGraphicsSecne::BspTreeIndex` ：应用Binary Space Partition tree，适合于大量的静态图形项。这个是默认值。
- `QGraphicsScene::NoIndex`：不用索引，搜索场景中所有的图形项，适合于经常进行图形项的添加、移动和删除等操作的情况。
我们可以使用`setItemIndexMethod()`函数进行索引算法的更改。
（三）边界矩形
图形项可以放到场景的任何位置，场景的大小默认是没有限制的。而场景的边界矩形仅用于场景内部进行索引的维护。因为如果没有边界矩形，场景就要搜索所有的图形项，然后确定出其边界，这是十分费时的。所以如果要操作一个较大的场景，我们应该给出它的边界矩形。设置边界矩形，可以使用`setSceneRect()`函数。
（四）图形项查找
场景最大的优势之一就是可以快速的锁定图形项的位置，即使有上百万个图形项，`items()`函数也能在数毫秒的时间内锁定一个图形项的位置。`items()`函数有几个重载函数来方便的进行图形项的查找。但是有时在场景的一个点可能重叠着几个图形项，这时我们可以使用`itemAt()`函数返回最上面的一个图形项。对于这些函数的使用，我们到后面讲视图时再举例讲解。
（五）事件处理和传播
场景可以传播来自视图的事件，将事件传播给该点最顶层的图形项。但是就像我们在讲图形项时所说的那样，如果一个图形项要接收键盘事件，那么它必须获得焦点。而且，如果我们在场景中重写了事件处理函数，那么在该函数的最后，必须调用场景默认的事件处理函数，只有这样，图形项才能接收到该事件。这一点我们也到后面讲视图时再细讲。
（六）打印
该部分内容也放到后面和视图一起讲。
四、视图（QGraphicsView）
`QGraphicsView` 提供了视图窗口部件，它使场景的内容可视化。你可以给一个场景关联多个视图，从而给一个数据集提供多个视口。视图部件是一个滚动区域，就是说，它可以提供一个滚动条来显示大型的场景。如果要使用OpenGL，你可以使用`QGraphicsView::setViewport()`函数来添加`QGLWidget` 。
（一）缩放与旋转
我们新建空的Qt项目，项目名称为`graphicsView04`，然后添加`main.cpp`文件，再新添一个C++ 类，类名为`MyView`，基类为`QGraphicsView`，类型信息选择“继承自`QWidget`”。
然后在`myview.h`中添加头文件：`#include <QtGui>`
然后声明事件槽函数：
```cpp
protected:
   void wheelEvent(QWheelEvent *event);
voidmousePressEvent(QMouseEvent *event);
```
我们到`myview.cpp`文件中进行函数的定义：
```cpp
MyView::MyView(QWidget *parent) :
   QGraphicsView(parent)
{
   resize(400,400);
    setBackgroundBrush(QPixmap("../graphicsView04/01.jpg"));//其实就是设置场景的背景
   QGraphicsScene *scene = new QGraphicsScene(this);
   scene->setSceneRect(0,0,200,200);
   QGraphicsRectItem *item = new QGraphicsRectItem(0,0,20,20);
   item->setBrush(Qt::red);
   scene->addItem(item);
   setScene(scene);
}
void MyView::wheelEvent(QWheelEvent*event)  //滚轮事件
{
   if(event->delta() > 0)  //如果鼠标滚轮远离使用者，则delta()返回正值
       scale(0.5,0.5);  //视图缩放
   else scale(2,2);
}
void MyView::mousePressEvent(QMouseEvent*event)
{
   rotate(90);  //视图旋转顺时针90度
}
```
这里我们定义了鼠标的滚轮事件和按下事件，在滚轮事件中，利用`delta()`函数返回值的正负来判断滚轮的移动方向，然后我们让视图进行缩放。
最后到`main.cpp`文件中，更改其内容如下：
```cpp
#include "myview.h"
int main(int argc,char *argv[])
{
   QApplication app(argc,argv);
   MyView *view = new MyView;
   view->show();
   return app.exec();
}
```
我们运行程序，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-4.jpg)
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-5.jpg)
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-6.jpg)
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-7.jpg)
上面四幅图分别是：正常，旋转90度后，缩小后，放大后的效果。可以看到实现视图的变换是十分简单的。
（二）场景边框与场景对齐方式
我们在上面讲场景时就提到了场景边框(`SceneRect`)，这里再说说它在视图中的作用。我们前面说过，视图是可以提供滚动条的，但是，这只是在视图窗口小于场景时才自动出现的。如果我们不定义场景边框，那么当场景中的图形项移动到视图可视窗口以外的地方时，视图就会自动出现滚动条，但是即使是图形项再次回到可视区域，滚动条也不会消失。为了解决这个问题，我们可以为场景设置边框，这样，当图形项移动到场景边框以外时，视图是不会提供额外的滚动区域的。
而当整个场景都可视时，也就是说视图没有滚动条时，我们可以通过`setAlignment()`函数来设置场景在视图中的对齐方式，如左对齐`Qt::AlignLeft` ，向上对齐`Qt::AlignTop`，中心对齐`Qt::AlignCenter`。更多的对齐方式，可以查看帮助中`Qt::Alignment` 关键字。默认的对齐方式是`Qt::AlignCenter`
 。而且几种对齐方式可以通过“按位或”操作一起使用。我们在上面的程序中的`myitem.cpp`文件中的构造函数最后添加一行代码：
```cpp
setAlignment(Qt::AlignLeft | Qt::AlignTop);
```
运行效果如下图所示。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-8.jpg)
（三）拖动模式
在QGraphicView中提供了三种拖动模式，分别是：
- `QGraphicsView::NoDrag` ：忽略鼠标事件，不可以拖动。
- `QGraphicsView::ScrollHandDrag` ：光标变为手型，可以拖动场景进行移动。
- `QGraphicsView::RubberBandDrag` ：使用橡皮筋效果，进行区域选择，可以选中一个区域内的所有图形项。
我们可以利用`setDragMode()`函数进行相应设置。
下面我们更改上面的程序。在`myview.cpp`中的构造函数中的最后添加一行代码：
```cpp
setDragMode(QGraphicsView::ScrollHandDrag);//手型拖动
```
并将场景外框放大一点：
```cpp
scene->setSceneRect(0,0,800,800);
```
这时运行程序，虽然出现了小手，但是并不能拖动场景。为什么呢？我们在`mousePressEvent()`函数中添加一行代码：
```cpp
QGraphicsView::mousePressEvent(event);
```
这时再运行程序，发现已经成功了。效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-9.jpg)
我们在事件函数的最后添加了一行：`QGraphicsView::mousePressEvent(event);`这样程序才能执行默认的事件。这也是我们下面要说的事件传播的内容的一部分。
（四）事件传递
在上面我们看到必须在事件函数的最后将`event`参数传递出去，才能执行默认的事件操作。其实不止上面那一种情况，在图形视图框架中，鼠标键盘等事件是从视图进入的，视图将它们传递给场景，场景再将事件传递给该点的图形项，如果该点有多个图形项，那么就传给最上面的图形项。所以要想使这个事件能一直传播下去，我们就需要在重新实现事件处理函数时，在其最后将`event`参数传给默认的事件处理函数。比如我们重写了场景的键盘按下事件处理函数，那么我们就在该函数的最后写上`QGraphicsScene::keyPressEvent(event);`一行代码。
（五）背景缓存
如果场景的背景需要大量耗时的渲染，可以利用`CacheBackground`来缓存背景，当下次需要渲染背景时，可以快速进行渲染。它的原理就是，把整个视口先绘制到一个`pixmap`上。但是这个只适合较小的视口，也就是说，如果视图窗口很大，而且有滚动条，那么就不再适合缓存背景。我们可以使用`setCacheMode(QGraphicsView::CacheBackground);`来设置背景缓存。默认设置是没有缓存`QGraphicsView::CacheNone`。
（六）OpenGL渲染
`QGraphicsView`默认使用一个`QWidget`作为视口部件，如果我们要使用OpenGL进行渲染，可以使用`setViewport()`函数来添加一个`QGLWidget`对象。看下面的例子。
我们先在项目文件`graphicsView04.pro`中加入
```
QT += opengl
```
说明要使用OpenGL模块，然后在`myview.cpp`文件中添加头文件：
```cpp
#include <QtOpenGL>
```
最后在构造函数中加入代码：
```cpp
QGLWidget *widget =new QGLWidget(this);
setViewport(widget);
```
这样便使用OpenGL进行渲染了。关于OpenGL，我们在后面的3D绘图部分再讲。
（七）图形项查找与图形项组
在前面讲场景时，我们就涉及了图形项查找的内容，当时没有细讲，现在我们把它和图形项组放到一起来讲解。先看一个例子，然后再介绍。
在`myview.cpp`中的构造函数里将以前那个`item`改名为`item1`，然后再加入一个`item2`和一个图形项组对象`group`。更改后构造函数的部分代码如下：
```cpp
QGraphicsRectItem *item1 = newQGraphicsRectItem(0,0,20,20);
item1->setBrush(Qt::red);
item1->setPos(10,0);
scene->addItem(item1);
QGraphicsRectItem *item2 = newQGraphicsRectItem(0,0,20,20);
item2->setBrush(Qt::green);
item2->setPos(30,0);
scene->addItem(item2);
QGraphicsItemGroup *group = newQGraphicsItemGroup;  //新建图形项组
group->addToGroup(item1);
group->addToGroup(item2);
scene->addItem(group);
setScene(scene);
qDebug() << "itemAt(10,0) :" <<itemAt(10,0); //输出(10,0)点的图形项
qDebug() << "itemAt(30,0) :" <<itemAt(30,0);
qDebug() <<"#################################"; //分割线
```
然后我们到`myview.h`文件中`protected`下声明键盘按下事件槽函数：
```cpp
void keyPressEvent(QKeyEvent *event);
```
再到`myview.cpp`中定义它，如下：
```cpp
void MyView::keyPressEvent(QKeyEvent*event)
{
   qDebug() << items();  //输出场景中所有的图形项
   items().at(0)->setPos(100,0);
   items().at(1)->setPos(0,100);
   QGraphicsView::keyPressEvent(event); //执行默认的事件处理
}
```
这时运行程序，当按下键盘上任意键后，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-10.jpg)
下面是输出框输出的信息：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-11.jpg)
可以看到，`itemAt()`函数可以输出场景上任意点的图形项。而`items()`函数可以输出场景上所有的图形项。这里应该说明，`items()`函数返回的图形项列表是按栈的降序排序的，也就是说，`items().at(0)`返回的是最后加入场景的图形项。从上面可以看出，最后加入的图形项是`item2`，其实，因为我们使用了`group`，而`item1`和`item2`都在`group`里，所以我们只需将`group`加入场景中就可以了，前面把`item1`和`item2`也加入场景是多余的。我们可以将`scene->addItem(item1);`和`scene->addItem(item2);`两行代码删掉。那么这时加入场景的顺序就是，先加入`group`，因为`item1`先加入`group`，所以下面将`item1`加入场景，最后加入场景的是`item2`，这就是为什么`items.at(0)`会是`item2`的原因。
下面再说图形项组，其实图形项组也是一个图形项，它有图形项所拥有的所有特性。其作用就是，将加入它的所有图形项作为一个整体，对这个图形项组进行操作，就相当于对齐中所有图形项进行操作。图形项组是加入它的所有图形项的父图形项，在上面的输出的`parent`信息中我们可以看到这一点。下面我们将程序中的代码更改如下：
```cpp
void MyView::keyPressEvent(QKeyEvent*event)
{
   items().at(2)->setPos(100,100);
   QGraphicsView::keyPressEvent(event); //执行默认的事件处理
}
```
运行程序，按下键盘上任意键，效果如下：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-12.jpg)
可以看到，两个图形项是同时移动的。我们要从图形项组中移除一个图形项，可以使用`removeFromGroup()`函数，它可以将给定的`item`从`group`中删除，要注意这时`item`依然存在，它会回到`group`的父图形项中，如果`group`没有父图形项，那么`item`就会回到场景中。我们可以使用场景的`removeItme()`函数来删除`group`，这样也会将`group`中所有的图形项从场景中删除。还有一种办法是利用场景的`destroyItemGroup()`函数，它会删除`group`并销毁它，但是`group`中的所有图形项会回到`group`的父图形项中，如果它没有父图形项，那么所有图形项就会回到场景中。
（八）打印
图形视图框架提供了两个打印函数`render()`，一个是在`QGraphicsScene`中，一个是在`QGraphicsView`中，并且它们的函数原型是一模一样的。不过它们实现的效果稍有不同。看一面的例子。
我们更改鼠标按下事件槽函数的内容如下：
```cpp
void MyView::mousePressEvent(QMouseEvent*event)
{
    rotate(90); //视图旋转顺时针90度
   QPixmap pixmap(400,400);  //必须指定大小
   QPainter painter(&pixmap);
   render(&painter,QRectF(0,0,400,400),QRect(0,0,400,400));  //打印视图指定区域内容
    pixmap.save("../graphicsView04/save.png");
   QGraphicsView::mousePressEvent(event);
}
```
这里我们使用了视图的`render()`函数，其中的`QRectF`参数是指设备的区域，这里是指`pixmap`。而`QRect`参数是指视图上要打印的区域。我们利用`QPixmap`类的`save()`函数，将`pixmap`图片保存到我们项目源码目录中，文件名为`save.png`。下面是运行程序后，点击鼠标，生成的图片的效果：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-13.jpg)
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-14.jpg)
我们每点击一次鼠标，就会旋转视图，那么生成的图片就是当前视口的截图。下面我们使用场景的打印函数，将上面的打印一行的代码改为：
```cpp
scene()->render(&painter,QRectF(0,0,400,400),QRect(0,0,400,400));//打印场景内容
```
查看图片效果：
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/20-15.jpg)
这时无论视图怎样变换，生成的图片总是一样的。而且它并没有打印场景背景的图片。就像我们看到的，视图的打印函数是依据视图的坐标系进行打印的，我们看到的就是打印出来后的效果，它可以看做是程序窗口的截屏。而场景的打印函数，是依据场景的坐标系的，无论视图怎么转换，只要场景坐标系没有变换，它打印出来的图片都是一样的。
## 结语
图形视图框架是一个非常强大而且庞杂的系统，我们教程中也只是很笼统的介绍了一些最基本最常用的内容。如果大家想系统学习该部分知识，想学习如何使用该框架轻松搭建一个游戏，可以参考《Qt Creator快速入门》的第11章，以及《Qt 及Qt Quick开发实战精解》的第二章。
涉及到的源码:
- [graphicsView03.zip](http://bbs.qter.org/forum.php?mod=attachment&aid=Mjc5fDU5NzljM2M1fDE0NzE2OTI5MDZ8MTA5NTl8MTI2)
- [graphicsView04.zip](http://bbs.qter.org/forum.php?mod=attachment&aid=MjgwfDA2NGJlMWIxfDE0NzE2OTI5MDZ8MTA5NTl8MTI2)
