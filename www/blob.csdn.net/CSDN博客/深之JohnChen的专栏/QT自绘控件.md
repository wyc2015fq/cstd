# QT自绘控件 - 深之JohnChen的专栏 - CSDN博客

2019年04月14日 19:40:21[byxdaz](https://me.csdn.net/byxdaz)阅读数：62
个人分类：[QT](https://blog.csdn.net/byxdaz/article/category/8858602)



一、QT自绘控件有两种方式

1、从QWidget或QFrame继承实现了一个自定义的widget，重写void paintEvent(QPaintEvent * event)函数，然后在这个函数里利用QPainter类进行绘制。

2、qml中canvas

二、什么情况下调用paintEvent

当发生一下情况时会产生绘制事件并调用paintEvent()函数:

1、在窗口部件第一次显示时，系统会自动产生一个绘图事件，从而强制绘制这个窗口部件。

2、当重新调整窗口部件的大小时，系统也会产生一个绘制事件。

3、当窗口部件被其他窗口部件遮挡，然后又再次显示出来的时候，就会对那些隐藏的区域产生一个绘制事件。

同时可以调用QWidget::update()或者QWidget::repaint()来强制产生一个绘制事件。二者的区别是:

repaint()函数会强制产生一个即时的重绘事件,而update()函数只是在Qt下一次处理事件时才调用一次绘制事件。

如果多次调用update(),Qt会把连续多次的绘制事件压缩成一个单一的绘制事件，这样可避免闪烁现象。

三、QFrame与QWidget的区别

QFrame是基本控件的基类，QWidget是QFrame基类，关系如下：

QPushButton,QLabel… -> QFrame -> QWidget

QFrame可以设置样式表，另外QFrame窗口显示边框，而QWidget默认不显示边框。

```
#include <QWidget>
#include <QFrame>
namespace Ui {
class Dialog;
}

class MyWidget : public QWidget {
    Q_OBJECT
public:
    MyWidget(QWidget *parent = 0) : QWidget(parent) {
        //this->setStyleSheet("QWidget{background:#ff0000;} QWidget:hover{background:#00ff00;}");//样式不起作用
    }
};

class MyFrame : public QFrame {
    Q_OBJECT
public:
    MyFrame(QWidget *parent = 0) : QFrame(parent) {
        this->setStyleSheet("QWidget{background:#ff0000;} QWidget:hover{background:#00ff00;}");
    }
};
```

四、实例

paintEvent方法

Qt 中没有专门显示图片的控件，通常我们会使用QLabel来显示图片。但是QLabel 显示图片的能力还是有点弱。比如不支持图像的缩放一类的功能，使用起来不是很方便。因此我就自己写了个简单的类。

转载https://www.cnblogs.com/findumars/p/6546096.html

我这个类支持三种图像显示模式，我分别称之为：FIXED_SIZE, CENTRED，AUTO_ZOOM, AUTO_SIZE。

FIXED_SIZE 模式下，显示的图像大小等于图像尺寸乘以缩放因子，如果控件的尺寸小于这个大小则多出的部分被裁切掉。

FIX_SIZE_CENTRED模式与FIXED_SIZE 模式类似，但是，显示的图像居于窗口正中。

AUTO_ZOOM 模式下，显示的图像大小自动适应控件窗口大小。

AUTO_SIZE 模式下，这个控件的尺寸等于图像尺寸乘以缩放因子。

//代码

```
//PictureBox.h
#ifndef PICTUREBOX_H
#define PICTUREBOX_H
 
#include <QWidget>
#include <QImage>
#include <QPixmap>
 
 
class PictureBox : public QWidget
{    
    Q_OBJECT
public:
    enum PB_MODE {FIXED_SIZE, FIX_SIZE_CENTRED, AUTO_ZOOM, AUTO_SIZE};
    explicit PictureBox(QWidget *parent = 0);
    void setMode(PB_MODE mode);
    ~PictureBox();
private:
    QPixmap m_pixmap;
    double m_scale;
    PB_MODE m_mode;
    QBrush m_brush;
protected:
    void paintEvent(QPaintEvent * event);
signals:
 
public slots:
    bool setImage(QImage &image, double scale = 1.0);
    void setBackground(QBrush brush);
};
 
#endif // PICTUREBOX_H
```

```
//PictureBox.cpp
#include "picturebox.h"
#include <QPainter>
#include <QDebug>
static const int IMAGE_WIDTH = 160;
static const int IMAGE_HEIGHT = 120;
static const QSize IMAGE_SIZE = QSize(IMAGE_WIDTH, IMAGE_HEIGHT);
 
PictureBox::PictureBox(QWidget *parent) : QWidget(parent)
{
    m_pixmap = QPixmap(IMAGE_SIZE);
    m_pixmap.fill();
    m_scale = 1.0;
    m_mode = FIXED_SIZE;
    m_brush = QBrush(Qt::white);
}
 
void PictureBox::setBackground(QBrush brush)
{
    m_brush = brush;
    update();
}
 
void PictureBox::setMode(PB_MODE mode)
{
    m_mode = mode;
    if(m_mode == AUTO_SIZE)
    {
        setFixedSize(m_pixmap.size() * m_scale);
    }
    else
    {
        setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        setMinimumSize(0, 0);
    }
    update();
}
 
bool PictureBox::setImage(QImage &image, double scale)
{
    if(image.isNull())
    {
        return false;
    }
    m_pixmap = QPixmap::fromImage(image);
    m_scale = qBound(0.01, scale, 100.0);
    if(m_mode == AUTO_SIZE)
    {
        setFixedSize(m_pixmap.size() * m_scale);
    }
    update();
    return true;
}
 
void PictureBox::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setBackground(m_brush);
    painter.eraseRect(rect());
 
    double window_width, window_height;
    double image_width, image_height;
    double r1, r2, r;
    int offset_x, offset_y;
    switch (m_mode)
    {
    case FIXED_SIZE:
    case AUTO_SIZE:
        painter.scale(m_scale, m_scale);
        painter.drawPixmap(0, 0, m_pixmap);
        break;
    case FIX_SIZE_CENTRED:
        window_width = width();
        window_height = height();
        image_width = m_pixmap.width();
        image_height = m_pixmap.height();
        offset_x = (window_width - m_scale * image_width) / 2;
        offset_y = (window_height - m_scale * image_height) / 2;
        painter.translate(offset_x, offset_y);
        painter.scale(m_scale, m_scale);
        painter.drawPixmap(0, 0, m_pixmap);
        break;
    case AUTO_ZOOM:
        window_width = width();
        window_height = height();
        image_width = m_pixmap.width();
        image_height = m_pixmap.height();
        r1 = window_width / image_width;
        r2 = window_height / image_height;
        r = qMin(r1, r2);
        offset_x = (window_width - r * image_width) / 2;
        offset_y = (window_height - r * image_height) / 2;
        painter.translate(offset_x, offset_y);
        painter.scale(r, r);
        painter.drawPixmap(0, 0, m_pixmap);
        break;
    }
}
 
PictureBox::~PictureBox()
{
 
}
```

qml方法

```
import QtQuick 2.7
import QtQuick.Window 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Styles 1.4
import "../CustomComponent"

Rectangle{
    id: rhomboidButton
    implicitWidth: mw
    implicitHeight: mh
    border.width: 0
    color: Qt.rgba(0.2, 0.2, 0.2, 0);

    property int mw : 150           //边框宽度
    property int mh : 40             //边框高度
    property int padding: 4
    property int gap : (mh-2*padding)/2;

    property string outlineColor: "#FF8100"
    property string shadowColor:  "#FF9800"
    property string textColor:      "#64FF98"
    
    property string title: "展示线路"
    signal buttonClicked()
    
    states: [
            State {
                name: "default"
                PropertyChanges { target: titleText; opacity: 0.5 }
                PropertyChanges { target: outline; opacity: 0.5 }
                PropertyChanges { target: aniCircle1; opacity: 0.5 }
                PropertyChanges { target: aniCircle2; opacity: 0.5 }
            },
            State {
                name: "hovered"
                PropertyChanges { target: titleText; opacity: 1 }
                PropertyChanges { target: outline; opacity: 1 }
                PropertyChanges { target: aniCircle1; opacity: 1 }
                PropertyChanges { target: aniCircle2; opacity: 1 }
            }
        ]

    state: "default"

    MouseArea{
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        hoverEnabled: true
        onClicked: {
            buttonClicked();
        }

        onPressed: {
            titleText.topPadding = padding/2;
        }

        onReleased: {
            titleText.topPadding = 0;
        }

        onEntered: {
            rhomboidButton.state = "hovered";
        }

        onExited: {
           rhomboidButton.state = "default";
        }
    }

    Text{
        id: titleText
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: title
        font.family: "微软雅黑"
        font.pointSize: 12
        color: textColor
        z: 10
    }

    Canvas {
        id: outline
        anchors.fill: parent
        contextType: "2d"
        z: 5

        onPaint: {
            var ctx = outline.getContext("2d");
            ctx.strokeStyle = outlineColor;
            ctx.fillStyle = Qt.rgba(0.2, 0.2, 0.2, 0.5);
            ctx.lineWidth = 2;
            ctx.shadowBlur = 2;
            ctx.shadowColor = shadowColor;
            ctx.beginPath();
            ctx.moveTo(padding, padding);
            ctx.lineTo(mw - padding - gap, padding);
            ctx.lineTo(mw - padding, mh - padding);
            ctx.lineTo(padding + gap, mh - padding);
            ctx.lineTo(padding, padding);
            ctx.stroke();
            ctx.fill();
        }
    }

    Canvas {
        id: aniCircle1
        x:0
        y:0
        width: rhomboidButton.width
        height: rhomboidButton.height
        contextType: "2d"
        z: 10

        onPaint: {
            var ctx = aniCircle1.getContext("2d");
            ctx.strokeStyle = "#EEF4F2";
            ctx.fillStyle = "#EEF4F2";
            ctx.lineWidth = 4;
            ctx.shadowBlur = 2;
            ctx.shadowColor = shadowColor;

            ctx.beginPath();
            ctx.arc(padding, padding, padding/2, 0, Math.PI*2, false);
            ctx.stroke();
            ctx.fill();
        }
    }

    PathAnimation{
        id: pathAnim1
        target: aniCircle1
        running: true
        duration: 4500
        anchorPoint: Qt.point(padding,padding)
        path: Path{
            startX: padding
            startY: padding
            PathLine{x: mw - padding - gap;  y: padding ;}
            PathLine{x: mw - padding;        y: mh - padding }
            PathLine{x: padding + gap;       y: mh - padding }
            PathLine{x: padding;             y: padding }
        }
        easing.type: Easing.InOutSine
        loops: Animation.Infinite
    }

    Canvas {
        id: aniCircle2
        x:0
        y:0
        width: rhomboidButton.width
        height: rhomboidButton.height
        contextType: "2d"
        z: 10

        onPaint: {
            var ctx = aniCircle2.getContext("2d");
            ctx.strokeStyle = "#EEF4F2";
            ctx.fillStyle = "#EEF4F2";
            ctx.lineWidth = 4;
            ctx.shadowBlur = 2;
            ctx.shadowColor = shadowColor;

            ctx.beginPath();
            ctx.arc(mw - padding, mh - padding, padding/2, 0, Math.PI*2, false);
            ctx.stroke();
            ctx.fill();
        }
    }

    PathAnimation{
        id: pathAnim2
        target: aniCircle2
        running: true
        duration: 4500
        anchorPoint: Qt.point(mw - padding,mh - padding)
        path: Path{
            startX: mw - padding
            startY: mh - padding
            PathLine{x: padding + gap;       y: mh - padding }
            PathLine{x: padding;             y: padding }
            PathLine{x: mw - padding - gap;  y: padding ;}
            PathLine{x: mw - padding;        y: mh - padding }
        }
        easing.type: Easing.InOutSine
        loops: Animation.Infinite
    }
}
```

参考文章

https://blog.csdn.net/liyuanbhu/article/details/46687495#

https://blog.csdn.net/foruok/article/details/41152297/

