# Qt 带有定位功能的图片控件 - DumpDoctorWang的博客 - CSDN博客





2017年08月03日 14:51:06[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1147标签：[Qt																[图片控件](https://so.csdn.net/so/search/s.do?q=图片控件&t=blog)](https://so.csdn.net/so/search/s.do?q=Qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/DumpDoctorWang/article/category/6989504)








# 原标题：Qt 自定义图片控件

20180328更新:

我编写了一个可以用来选点的程序, Windows和主要Linux发行版均可使用. 下载地址:https://gitee.com/Mannix1994/x-y

这个控件的核心部分是基于[liyuanbhu](http://my.csdn.net/liyuanbhu)大神写的[图片控件](http://blog.csdn.net/liyuanbhu/article/details/46687495)，我在他的基础之上，加入坐标追踪功能，在控件上左键点击时，在该位置画一个十字，同时将该位置记录下来，右键点击时，删除上一个坐标。在鼠标移动时，有一个十字跟着鼠标走。



头文件picturebox.h

```cpp
#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QEvent>
#include <QPoint>
#include <QLine>
#include <QVector>
#include <QStack>
#include <QStringList>
#include <QDebug>

class PictureBox : public QWidget
{
    Q_OBJECT
public:
    enum PB_MODE {FIXED_SIZE, FIX_SIZE_CENTRED, AUTO_ZOOM, AUTO_SIZE}; //图像显示模式
    explicit PictureBox(QWidget *parent = 0);
    void setMode(PB_MODE mode); //设置显示模式
    ~PictureBox();

private:
    QPixmap m_pixmap; //当前绘制的图像
    double m_scale; //缩放因子
    PB_MODE m_mode; //显示模式
    QBrush m_brush; //
    int x,y; //当前被鼠标点击的点的x,y坐标
    typedef enum {Border,TrackingCross,LocationCross} PaintMode; //绘图模式：边框、追踪十字、位置十字
    PaintMode paintMode; //绘图模式
    QStack<QPixmap> images; //存储历次绘制的图像
    QStringList points; //存储历次被点击的坐标
    QString getPointsString();//返回以","分割的店坐标

protected:
    void paintEvent(QPaintEvent * event); //绘制事件

signals:
    void clicked(QString points); //图片被点击的信号，发送的是被点击的点的坐标

public slots:
    bool setImage(QImage &image, double scale = 1.0); //设置要显示的图像
    void setBackground(QBrush brush); //设置背景

protected:
    void mousePressEvent(QMouseEvent *event); //鼠标点击事件
    void mouseMoveEvent(QMouseEvent *event); //鼠标移动事件
    void enterEvent(QEvent *event); //鼠标进入窗口事件
    void leaveEvent(QEvent *event); //鼠标离开窗口事件
};

#endif // PICTUREBOX_H
```



源文件picturebox.cpp

```cpp
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
    paintMode = Border;
}

/**
 * @brief PictureBox::setBackground 设置背景
 * @param brush
 */
void PictureBox::setBackground(QBrush brush)
{
    m_brush = brush;
    update();
}

/**
 * @brief PictureBox::setMode 设置显示模式
 * @param mode
 */
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

/**
 * @brief PictureBox::setImage 设置图片
 * @param image 图片
 * @param scale 缩放因子
 * @return
 */
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
    images.push(this->grab(this->rect()));
    m_pixmap = images.top();
    this->update();
    return true;
}

/**
 * @brief PictureBox::paintEvent 调用update()以后引发的paintEvent事件
 * @param event
 */
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

    //绘制指定线段
    int xMax = painter.window().width()-1;
    int yMax = painter.window().height()-1;
    QVector<QLine> lines;
    if(paintMode == Border){ //绘制边框
        QPoint p1(0,0),p2(xMax,0),p3(xMax,yMax),p4(0,yMax);
        QLine line1(p1,p2);
        QLine line2(p2,p3);
        QLine line3(p3,p4);
        QLine line4(p4,p1);
        lines.append(line1);
        lines.append(line2);
        lines.append(line3);
        lines.append(line4);
    }
    else if(paintMode == TrackingCross){ //绘制追踪十字
        QPoint p1(x,0),p2(x,yMax),p3(0,y),p4(xMax,y);
        QLine line1(p1,p2);
        QLine line2(p3,p4);
        lines.append(line1);
        lines.append(line2);
    }
    else if(paintMode == LocationCross){ //绘制定位十字
        int xMinP = (x-10)>0 ? (x-10):0;
        int xMaxP = (x+10)<xMax ? (x+10):xMax;
        int yMinP = (y-10)>0 ? (y-10):0;
        int yMaxP = (y+10)<yMax ? (y+10):yMax;
        QPoint p1(x,yMinP),p2(x,yMaxP),p3(xMinP,y),p4(xMaxP,y);
        QLine line1(p1,p2);
        QLine line2(p3,p4);
        lines.append(line1);
        lines.append(line2);
    }
    painter.drawLines(lines);
}

/**
 * @brief PictureBox::mousePressEvent 鼠标点击，左键在图片上添加一个定位十字，右键在图片上删除一个定位十字
 * @param event
 */
void PictureBox::mousePressEvent(QMouseEvent *event){
    this->setMouseTracking(false); //关闭鼠标追踪
    if(event->button() == Qt::LeftButton){ //鼠标左键被点击
        x = event->x();
        y = event->y();
        //存储当被点击的点
        points.append(QString("(%1,%2)").arg(x).arg(y));
        //绘制定位十字
        paintMode = LocationCross;
        this->update();
        //存储绘制后的图像
        images.push(this->grab(this->rect())); //抓取当前图像并存储
        //更新图像
        m_pixmap = images.top();
        this->update();
        paintMode = TrackingCross;
    }
    else if(event->button() == Qt::RightButton){ //右键，弹出顶层图像
        //退回上一张图像
        if(images.count()>1){
            images.pop();
            m_pixmap = images.top();
            this->update();
        }
        //删除最后一个点击的点
        if(points.count()>0){
            points.removeLast();
        }
    }
    this->setMouseTracking(true);
    emit clicked(this->getPointsString()); //发送被点击的信号
}

/**
 * @brief PictureBox::mouseMoveEvent 鼠标移动事件，绘制追踪十字
 * @param event
 */
void PictureBox::mouseMoveEvent(QMouseEvent *event){
    x = event->x();
    y = event->y();
    paintMode = TrackingCross;
    this->update();
}

/**
 * @brief PictureBox::enterEvent 进入此控件时设置鼠标追踪，通过mouseMoveEvent绘制追踪十字
 * @param event
 */
void PictureBox::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    this->setMouseTracking(true);
}

/**
 * @brief PictureBox::leaveEvent 离开此控件时关闭追踪，并将绘制模式paintMode设置边框(Border)，清除会绘制的追踪十字
 * @param event
 */
void PictureBox::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    this->setMouseTracking(false);
    paintMode = Border;
    this->update();
}

/**
 * @brief PictureBox::getPointsString 返回以","分割的坐标字符串
 * @return
 */
QString PictureBox::getPointsString(){
    if(points.isEmpty())
        return "";
    QString data = points.first();
    for(int i=1;i<points.count();i++){
        data += ","+points.at(i);
    }
    return data;
}

PictureBox::~PictureBox()
{

}
```



调用方法

```cpp
pictureBox = new PictureBox();
    this->connect(pictureBox,SIGNAL(clicked(QString)),this,SLOT(onPictureClicked(QString)));
    this->ui->vLayout->addWidget(pictureBox);
    QImage image("1.jpg");
    ui->verticalLayoutWidget->setFixedSize(image.size());
    pictureBox->setImage(image);
```



![](https://img-blog.csdn.net/20171128091818801?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRHVtcERvY3Rvcldhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




