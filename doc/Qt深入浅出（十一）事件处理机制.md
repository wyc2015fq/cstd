# Qt深入浅出（十一）事件处理机制

2018年02月24日 00:34:32 [吓人的猿](https://me.csdn.net/qq769651718) 阅读数：259



# 事件处理机制

​	什么是事件，比如当我们打开一个窗口, 如果我们不动鼠标或者键盘, 那这个窗口就永远静静的躺着, 只有我们使用鼠标点击,或者键盘按下键的时候, 窗口才会有对应的反应。那么这个过程中就是用户向窗口发送了事件。

​	在 Qt 的界面应用程序都是事件驱动的，程序的每个动作也都是由某个事件所触发。在Qt 中事件抽象成QEvent类。	Qt中的事件有其对应的事件处理函数，事件先由QApplication::exec()消息循环接收，然后传递给对应窗口。

## 1 窗口事件处理函数event



​	Qt中所有的事件都是继承至QEvent, QEvent继承至QObject。每个窗口会收到各种事件，每个窗口也都有一个总的事件处理函数，用来处理事件或者进一步分派事件。

​	窗口的这个事件处理函数为QWidget::event()。

```cpp
[virtual protected] bool QWidget::event(QEvent *event);
```

​	需要注意它是一个保护权限的虚函数，那么我们一般通过派生QWidget对象，重写这个虚函数，来自定义事件处理。

Widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QDialog>
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    bool event(QEvent *event);
    ~Widget();
​
};
#endif // WIDGET_H
```

Widget.cpp

```cpp
#include "widget.h"
#include <QEvent>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
}
​
bool Widget::event(QEvent *ev)
{
    /*只对鼠标点击事件进行处理，其它的调用基类的event函数*/
    if(ev->type() == QEvent::MouseButtonPress || 
            ev->type() == QEvent::MouseButtonDblClick)
    {
        qDebug() << __FUNCTION__ << endl;
        //event->ignore();  //代表我无视这个消息
        event->accept();    //代表我接收了这个消息
        //return false;     //这个消息我没有处理.
        return true;        //这个消息在我已经处理过了
        
    }
    return QWidget::event(ev);
}
​
Widget::~Widget()
{
​
}
```



​	需要注意的是：只有`event->accept()`以及`return true`时，才不往父窗口传递，否则都会传递。



## 2 鼠标事件

### 2.1 鼠标点击事件

​	Qt中的鼠标事件为QMouseEvent,  鼠标按下事件对应的事件处理函数为：

```cpp
[virtual protected] void QWidget::mousePressEvent(QMouseEvent *event);
```



​	这个是QWidget里面实现的鼠标按下事件处理函数, 它也是保护类型的虚函数,那么,我们一般是去覆写这个虚函数来实现我们想要的处理函数.



- 示例代码:

  Widget.h 

  ```cpp
  #ifndef WIDGET_H
  #define WIDGET_H
  ​
  #include <QWidget>
  #include <QDialog>
  class Widget : public QWidget
  {
      Q_OBJECT
  ​
  public:
      Widget(QWidget *parent = 0);
      void mousePressEvent(QMouseEvent*); //重写虚函数
      ~Widget();
  ​
  };
  ​
  #endif // WIDGET_H
  ```

  Widget.cpp

  ```cpp
  #include "widget.h"
  #include <QMouseEvent>
  #include <QDebug>
  Widget::Widget(QWidget *parent)
      : QWidget(parent)
  {
  }
  ​
  void Widget::mousePressEvent(QMouseEvent* ev)
  {
      qDebug() << __FUNCTION__ << endl;
      if(ev->button() == Qt::LeftButton)
          {
          qDebug() << "left button press" << endl;
      }
      else if(ev->button() == Qt::RightButton)
          {
          qDebug() << "right button press" << endl;
      }
      else if(ev->button() == Qt::MidButton)
          {
          qDebug() << "mid button press" << endl;
      }
      qDebug() << ev->pos() << endl;
  }
  ```

  普通鼠标有左右中三个键，可通过QMouseEvent::button来判断是哪个键：

  ```cpp
  Qt::MouseButton QMouseEvent::button() const;
  /*
  *返回值为枚举类型：Qt::LeftButton 左键、Qt::RightButton 右键、Qt::MidButton 中键、Qt::NoButton没有键按下。
  */
  ```

  鼠标的点击位置可以通过QMouseEvent::pos来获取：

  ```cpp
  QPoint QMouseEvent::pos() const
  ```

  

### 2.2 鼠标释放事件

​	Qt中的鼠标释放事件也是一个QMouseEvent对象,  鼠标按下事件对应的事件处理函数为：

```cpp
[virtual protected] void QWidget::mouseReleaseEvent(QMouseEvent *event);
```

​	使用方式，与鼠标点击事件类似，不再复述。





### 2.3 鼠标移动事件

​	Qt中的鼠标移动事件为也是一个QMouseEvent对象,  鼠标移动事件对应的事件处理函数为：

```cpp
[virtual protected] void QWidget::mouseMoveEvent(QMouseEvent *event);
```

- 默认情况下, 鼠标移动事件触发条件必须是鼠标至少一个按键按下状态, 再去移动鼠标，但是鼠标QMouseEvent::Button()还是Qt::NoButton。

```cpp
void Widget::mouseMoveEvent(QMouseEvent*  ev)
{
    qDebug() << ev->pos() << endl;
    qDebug() << ev->button() << endl; //不能用这个判断到底是哪个键按下
}
```



- 如果想要不按下鼠标按键，就触发鼠标移动事件可以调用QWidget::setMouseTracking

```cpp
void QWidget::setMouseTracking(bool enable);
```

​	你可以参数在Widget中构造函数中调用该函数，enable参数为true代表开启鼠标跟踪，那么鼠标不需要按下任何键都能触发鼠标移动事件。







### 2.4 鼠标滚轮事件

​	鼠标滚轮事件为QWheelEvent, 继承自QObject，对应的事件处理函数为:

```cpp
[virtual protected] void QWidget::wheelEvent(QWheelEvent *event);
```

​	例如:

```cpp
void Widget::wheelEvent(QWheelEvent* ev)
{
    qDebug() << ev->angleDelta() << endl;
}
```









### 2.5 其它鼠标事件

- 鼠标双击事件对应的事件处理函数

```cpp
[virtual protected] void QWidget::mouseDoubleClickEvent(QMouseEvent *event)
```

- 鼠标进入窗口对应的事件处理函数

```cpp
[virtual protected] void QWidget::enterEvent(QEvent *event)
```

- 鼠标离开窗口对应的事件处理函数

```cpp
[virtual protected] void QWidget::leaveEvent(QEvent *event)
```







## 3 键盘事件

​	在Qt中键盘事件是QKeyEvent，继承自QObject。



### 3.1 键盘按下事件

​	按下的事件对应的处理函数为:

```cpp
[virtual protected] void QWidget::keyPressEvent(QKeyEvent *event)
```

​	例如:

​	widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void keyPressEvent(QKeyEvent* ev);
};
​
#endif // WIDGET_H
​
```





​	widget.cpp

```cpp
#include "widget.h"
#include <QKeyEvent>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
      
}
void Widget::keyPressEvent(QKeyEvent* ev)
{
    switch(ev->key())
    {
        case Qt::Key_A:
            qDebug() << "key a pressed" << endl;
            break;
        case Qt::Key_B:
            qDebug() << "key b pressed" << endl;
            break;
        default:
            break;
    }
}
```



​	通过QKeyEvent::key()函数获取到具体按下的某个键值：

```cpp
int QKeyEvent::key() const;
/*
* 返回值的具体键值可以查看Qt帮助文档
*/
```





### 3.2 键盘释放事件

​	鼠标按下事件对应的事件处理函数为：

```cpp
[virtual protected] void QWidget::keyReleaseEvent(QKeyEvent *event);
```

​	使用方法与键盘按下事件类似，不再复述。





### 3.3 设置焦点窗口

​	我们知道键盘事件是发送给当前激活状态的窗口，当只有一个窗口的时候，键盘事件定然是传递给这个窗口的。但是如果这个窗口上面有多个子窗口的时候，我们需要指定一个焦点窗口用来接收键盘事件。

​	焦点窗口可以通过tab键、鼠标按键点击等方式切换，也可以通过代码的方式来切换：

```cpp
void QWidget::setFocus(Qt::FocusReason reason);
/*
*Qt::FocusReason reason 告诉Qt焦点切换的原因，一般填个Qt::OtherFocusReason即可
*/
```



#### 3.4 判断按键大小写

​	通过QKeyEvent的text()函数：

```cpp
QString QKeyEvent::text() const
/*
*返回一个QString类型，这个是键盘产生的字符串
*/
```

​	那么我们可以这样判断大小写：

```cpp
void Widget::keyPressEvent(QKeyEvent* e)
{
      if( e->text() >= "A" && e->text() <= "Z" )
      {
          qDebug() << "uppercase" << endl;
      }
      else if( e->text() >= "a" && e->text() <= "z" )
      {
          qDebug() << "lowcase" << endl;
      }
}
```









## 4 事件过滤器

​	上面我们看到，事件都只能在自己类中处理，如果其它对象想知道某个窗口的事件，可以通过给窗口安装事件过滤器，对应的函数为：

```cpp
void QObject::installEventFilter(QObject *filterObj);
/*
*这个函数谁调用，谁被监控
*QObject *filterObj 监控者
*/
```



​	事件过滤器的处理函数为:

```cpp
[virtual] bool QObject::eventFilter(QObject *watched, QEvent *event);
/*
* QObject *watched，被监控对象
* QEvent *event 具体哪些事件
* 返回值 true 代表过滤事件,  false 代表不过滤
*/
```

​	例如：

Widget.h

```cpp
Widget.h
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QWidget>
​
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    bool eventFilter(QObject *watched, QEvent *event);
​
};
​
#endif // WIDGET_H
```

Widget.c

```cpp
#include "widget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QEvent>
#include <mybutton.h>
​
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    this->resize(400, 300);
    QHBoxLayout* hBox = new QHBoxLayout;
    MyButton* pb = new MyButton("pb");
    hBox->addWidget(pb);
    this->setLayout(hBox);
    pb->installEventFilter(this); //给pb对象安装事件过滤器
}
​
bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress
            || event->type() ==  QEvent::MouseButtonDblClick)
    {
        qDebug() << "eventFilter" << endl;
        return true; //过滤掉点击事件
    }
    return QWidget::eventFilter(watched, event);
}
​
Widget::~Widget()
{
  
}
```

mybutton.h

```cpp
#ifndef MYBUTTON_H
#define MYBUTTON_H
#include <QPushButton>
#include <QKeyEvent>
class MyButton : public QPushButton
{
public:
    MyButton(const QString & text, QWidget * parent = 0);
    void mousePressEvent(QMouseEvent* ev);
};
​
#endif // MYBUTTON_H
```



mybutton.cpp

```cpp
#include "mybutton.h"
#include <QDebug>
​
MyButton::MyButton(const QString & text, QWidget * parent)
    : QPushButton(text, parent)
{
​
}
void MyButton::mousePressEvent(QMouseEvent* ev)
{
    static int i = 0;
    qDebug() << __FUNCTION__ << i++ << endl;
}
```



## 5 事件传递过程

​	QApplication的exec从桌面系统中获取事件，然后分发给对应的窗口事件处理函数bool event(QEvent)，然后该函数再分发给具体的某个事件处理函数，如鼠标事件、键盘事件等。





## 6 事件处理函数是如何被调用的

​	  我们重定义事件处理函数后,并没有显示的调用,那为什么,事件能够进入到我们的事件处理函数中呢?

   

   	这是其实是因为,我们的事件处理函数都是虚函数，我们override了基类的事件处理函数, 而Qt在底层实现了使用基类的指针指向我们的派生类对象, 并且在事件触发时，使用基类指针来调用我们override的事件处理函数。

​	简单来说，就是应用了c++多态。







## 7 绘图事件

​	在桌面系统中所有的窗口都是绘制出来的, 那么在窗口形态发生变化时, 就会产生一个绘画事件, 在Qt中叫作QPaintEvent, 其对应的事件处理函数是：

```cpp
[virtual protected] void QWidget::paintEvent(QPaintEvent *event);
```





### 7.1 QPainter画家

​	我们知道画图一般需要一个画家来画，画家想要画图首先要给它一张画纸，然后画家需要一把画笔来画轮廓, 一把笔刷来填充颜色。

​	那么在Qt中我们的画家叫做QPainter, 画纸可以是任何QPaintDevice的派生类对象,

​	QWidget本身也继承至QPaintDevice，所以也能用来当作画纸。继承至QPaintDevice的类还有QPixmap、QPicture、QImage。

​    	 QPainter有QPen画笔, QBrush笔刷。

​    	注意:当画纸为QWidget的派生类对象时, QPainter只能在QWidget的painteEvent事件处理函数中使用.

 

   



### 7.2 在窗口上绘图

​	设置画笔笔刷, 并且绘制出线,文本,矩形等等：

​	widet.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QWidget>
​
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent* event);
​
};
​
#endif // WIDGET_H
```

​	widget.cpp

```cpp
#include "widget.h"
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
​
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    this->resize(400, 300);
​
    QFont font = this->font(); //这个可以用来设置字体的样式
    font.setPixelSize(10);
    this->setFont(font);
}
​
void Widget::paintEvent(QPaintEvent* event)
{
    qDebug() << "paintEvent" << endl;
    QPainter painter;
    painter.begin(this);
    painter.setPen(Qt::red);
//  painter.setPen(QPen(QBrush(Qt::red), 20, Qt::DashLine)); //画笔的高级设置
    painter.setBrush(Qt::blue); //设置
//  painter.setBrush(QBrush(Qt::blue, Qt::Dense3Pattern); //笔刷的高级设置
    painter.drawText(100, 100, "huangwl"); //画字
    painter.drawLine(0, 0, 100, 100); //画线
    painter.drawRect(100, 100, 200, 200); //画矩形
    painter.drawEllipse(100, 0, 100, 200); //画椭圆
    painter.end();
}
​
Widget::~Widget()
{
​
}
```





### 7.3  转变窗口坐标系

- 更改绘图坐标系的坐标原点

```cpp
void QPainter::translate(constQPointF &offset)
```



 

- 更改绘图坐标系的角度

```cpp
void QPainter::rotate(qreal angle)
```



 

- 缩放坐标系单位

```cpp
void QPainter::scale(qreal sx, qreal sy)
```

​	Widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QWidget>
​
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent* event);
​
};
​
#endif // WIDGET_H
```

​	Widget.cpp

```cpp
#include "widget.h"
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
​
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    this->resize(600, 480);
}
​
void Widget::paintEvent(QPaintEvent* event)
{
    qDebug() << "paintEvent" << endl;
    QPainter painter;
    painter.begin(this);
    painter.setPen(Qt::red);
    painter.setBrush(Qt::blue);
    painter.translate(100, 0); //坐标原点转换到(100,0)位置
    painter.rotate(45);  //顺时针旋转45度
    painter.scale(0.5, 0.5); //坐标系的单位变为原来的0.5
    painter.drawRect(0, 0, 100, 100); //画矩形
    painter.end();
}
​
Widget::~Widget()
{
​
}
```



### 7.4 保存与还原QPainter设置

- 保存之前QPainter所有的设置

```cpp
void QPainter::save() 
```





- 还原之前QPainter的设置.

```cpp
void QPainter::restore()
```





### 7.5 绘制事件触发的时机

1. 窗口失去焦点, 窗口缩放.
2. 窗口被覆盖, 或者窗口被显示出来
3. 在代码中可以用函数repaint()和update()

- repaint() 立即重绘，并且可以指定重绘区域

  ```cpp
  [slot] void QWidget::repaint()
  ```

  

- update()  不是马上重绘，加入到消息队列中，由消息循环来调度，有时多个update()会合并成一个重绘事件。

  ```cpp
  [slot] void QWidget::update()
  ```





​	例如：

​	widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QWidget>
​
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent* event);
public slots:
    void doRepaint();
    void doUpdate();
private:
    QColor _color;
};
​
#endif // WIDGET_H
```

​	Widget.cpp

```cpp
#include "widget.h"



#include <QDebug>



#include <QPaintEvent>



#include <QPainter>



#include <QDialog>



#include <QPushButton>



#include <QHBoxLayout>







Widget::Widget(QWidget *parent) : QWidget(parent), _color(Qt::red)



{



    QDialog * dialog = new QDialog(this);



    QPushButton * pb0 = new QPushButton("update", this);



    QPushButton * pb1 = new QPushButton("repaint", this);



    QHBoxLayout * hBox = new QHBoxLayout(this);



    hBox->addWidget(pb0);



    hBox->addWidget(pb1);



    dialog->setLayout(hBox);







    this->resize(600, 480);



    dialog->resize(400, 300);



    dialog->show();







    connect(pb0, SIGNAL(clicked()), this, SLOT(doUpdate()));



    connect(pb1, SIGNAL(clicked()), this, SLOT(doRepaint()));



}



void Widget::doUpdate()



{



    _color.setRgb(0, 0, 255);



    this->update();



}







void Widget::doRepaint()



{



    _color.setRgb(0, 255, 0);



    this->repaint(100, 100, 200, 200);



}







void Widget::paintEvent(QPaintEvent* event)



{



    QPainter painter;



    painter.begin(this);



    painter.setBrush(_color);



    painter.drawRect(0, 0, this->width(), this->height()); //画矩形



    painter.end();



}







Widget::~Widget()



{







}
```