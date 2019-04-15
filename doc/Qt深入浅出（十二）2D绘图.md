# Qt深入浅出（十二）2D绘图

2018年02月24日 00:35:27 [吓人的猿](https://me.csdn.net/qq769651718) 阅读数：406



# 2D绘图

​	之前我们使用 QPainter 在 QWidget 上绘图, 那么其实 QPainter 还能在任何 QPaintDevice上绘制, 继承至QPaintDevice 还有QPixMap、QImage、QPicture,那么它们的区别是.	1、使用 QPixMap 进行绘图，根据硬件显示进行优化.	2、使用 QImage 进行绘图，独立与硬件，保存最原始的图片数据.	3、QPicture,可以记录和重现 QPainter 命令

​	将所有需要绘制的图形，先绘制在 QPixmap QImage QPicture 中， 绘制完成后，再到paintEvent 中一次性绘制出来，可以大大提高 painterEvent 事件处理器的处理效率，可以有效避免绘制过程出现卡顿。

## 1 在QPixmap上绘图

​	先把所有的图形元素都画在 QPixmap 对象上面，然后再通过调用 update()， 去调度 paintEvent绘制出 QPixmap 对象的内容。QPixmap 对象可以使用save()，来保存成一个图片文件。

​	例如：

​	Widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QWidget>
#include <QPixmap>
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent* ev);
public slots:
    void drawPixmap();
private:
    QPixmap* _pixmap;
};
​
#endif // WIDGET_H
```

​	widget.cpp

```cpp
#include "widget.h"
#include <QDebug>
#include <QPainter>
#include <QDialog>
#include <QHBoxLayout>
#include <QPushButton>
​
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(400, 400);
    _pixmap = new QPixmap(this->size());
    _pixmap->fill(QColor(0, 0, 0, 0)); //填充透明,默认是黑色
​
/*dialog for controlling*/
    QDialog* dialog = new QDialog(this);
    QPushButton* button = new QPushButton("drawPixmap", dialog);
    QHBoxLayout* hBox = new QHBoxLayout(dialog);
    connect(button, SIGNAL(clicked()), this, SLOT(drawPixmap()));
    hBox->addWidget(button);
    dialog->setLayout(hBox);
    dialog->show();
}
​
void Widget::drawPixmap()
{
    QPainter painter;
    painter.begin(_pixmap);
    painter.drawLine(0, 0, 100, 100);
    painter.drawRect(100, 100, 100, 100);
    painter.end();
    qDebug() << __FUNCTION__ << endl;
    this->update(); //画完之后调用update();
}
void Widget::paintEvent(QPaintEvent* ev)
{
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0, 0, *_pixmap);
    painter.end();
​
}
Widget::~Widget()
{
    delete _pixmap; 
}
​
```



​	需要注意的是QPixmap不继承自QObject无法使用内存托管机制，需要显示释放内存。

## 2 在QImage上绘图

​	先把所有的图形元素都画在QImage对象上面，然后再通过调用update()，去调度paintEvent绘制出QImage对象的内容.。QImage对象可以使用save()，来保存成一个图片。

​	需要注意的是QImage的初始化方式与QPixmap的不同。

​	widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QWidget>
#include <QImage>
​
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent* ev);
public slots:
    void drawImage();
private:
    QImage* _image;
};
​
#endif // WIDGET_H
```

​	widget.cpp

```cpp
#include "widget.h"
#include <QDebug>
#include <QPainter>
#include <QDialog>
#include <QHBoxLayout>
#include <QPushButton>
​
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(400, 400);
    /*初始化的方式不同*/
    _image = new QImage(this->size(), QImage::Format_ARGB32_Premultiplied);
    _image->fill(QColor(0, 0, 0, 0)); //填充透明,默认是黑色
​
​
/* dialog controlling*/
    QDialog* dialog = new QDialog(this);
    QPushButton* button = new QPushButton("drawImage", dialog);
    QHBoxLayout* hBox = new QHBoxLayout(dialog);
    connect(button, SIGNAL(clicked()), this, SLOT(drawImage()));
    hBox->addWidget(button);
    dialog->setLayout(hBox);
    dialog->show();
}
​
void Widget::drawImage()
{
    QPainter painter;
    painter.begin(_image);
    painter.drawLine(0, 0, 100, 100);
    painter.drawRect(100, 100, 100, 100);
    painter.end();
    qDebug() << __FUNCTION__ << endl;
    this->update(); //画完之后调用update();
}
​
void Widget::paintEvent(QPaintEvent* ev)
{
    QPainter painter;
    painter.begin(this);
    painter.drawImage(0, 0, *_image);
    painter.end();
​
}
​
Widget::~Widget()
{
    delete _image;
}
```



## 3 在QPicture上绘图



​	先把所有的图形元素都画在QPicture对象上面,然后再通过调用update()，去调度paintEvent绘制出QPicture对象的内容。QPicture对象可以使用save()，来保存成一个文件。与QImage QPixmap的不同之处在于，QPicture强调的画图的动作的记录, 然后保存的文件不是一张图片。

```cpp
#include "widget.h"
#include <QDebug>
#include <QPainter>
#include <QDialog>
#include <QHBoxLayout>
#include <QPushButton>
​
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(400, 400);
    _picture = new QPicture;
​
​
/*dialog for controlling*/
    QDialog* dialog = new QDialog(this);
    QPushButton* button = new QPushButton("drawPicture", dialog);
    QHBoxLayout* hBox = new QHBoxLayout(dialog);
    connect(button, SIGNAL(clicked()), this, SLOT(drawPicture()));
    hBox->addWidget(button);
    dialog->setLayout(hBox);
    dialog->show();
}
​
void Widget::drawPicture()
{
    qDebug() << __FUNCTION__ << endl;
    QPainter painter;
    painter.begin(_picture);
    painter.drawLine(0, 0, 100, 100);
    painter.drawRect(100, 100, 100, 100);
    painter.end();
    this->update(); //画完之后调用update();
}
​
void Widget::paintEvent(QPaintEvent* ev)
{
    QPainter painter;
    painter.begin(this);
    painter.drawPicture(0, 0, *_picture);
    painter.end();
​
}
​
Widget::~Widget()
{
    delete _picture; 
}
```





## 4 绘制不规则窗体

​	绘制不规则窗体的步骤如下：

1. 顶级窗口设置为无边框 setWindowFlags(Qt::FramelessWindowHint);
2. 顶级窗口设置为透明窗体 setAttribute(Qt::WA_TranslucentBackground);
3. 重新实现paintEvent事件处理机制。绘制出一张图片出来。

