# QPushButton设置背景图片变换（素材四连图） - yixianfeng41的专栏 - CSDN博客
2016年08月09日 22:57:27[第2梦](https://me.csdn.net/yixianfeng41)阅读数：14835
在QT中，QPushButton是我们经常用的控件，但是系统默认控件的样式不好看，因此我们一般都会给QPushButton设置背景图片，并且一般想达到的效果是，当鼠标放上去时是一张图片，当鼠标点击时是一张图片，当鼠标离开时，又是另外一张图片，怎么实现这个效果了。
**1、setStyleSheet**
```cpp
setStyleSheet("QPushButton{background-image: url(:/images/1.bmp);}" 
              "QPushButton:hover{background-image: url(:/2.bmp);}" 
              "QPushButton:pressed{background-image: url(:/3.bmp);}");
```
**2、自定义QPushButton**
但是大家有没有遇到这样的情况，素材是几张连在一起，比如四张图片连在一起的（大多数软件图标素材），如
![](https://img-blog.csdn.net/20160809222919481)
这种情况怎么办了，或许你会说可以用ps将上面的图片切成四个单独图片，然后用setStyleSheet,这样是可以的，但是这个方法太不方便了，也有违制作这个素材的人的初衷，那我们怎么才能利用上面素材，实现按钮背景图片的切换了？
方法是：我们可以用代码（根据位置，长宽）将上述素材分别保存为四张图片于链表中，
```cpp
QPixmap pixmap(FileName);
    QList<QPixmap> pixmatpList;
    for (int cnt = 0; cnt < xnum; ++cnt)
    {
        for (int y = 0; y < ynum; ++ y)
        {
            pixmatpList.push_back( pixmap.copy(cnt * (pixmap.width() / xnum),
                                               y * (pixmap.height() / ynum),
                                               pixmap.width() / xnum ,
                                               pixmap.height() / ynum));
        }
    }
```
然后重新实现控件的paintEvent ( QPaintEvent * event)，enterEvent(QEvent *event)，leaveEvent(QEvent *event)，mousePressEvent(QMouseEvent *event)，mouseReleaseEvent(QMouseEvent *event)事件。
在实际应用中，我们可以自己定义一个Button，继承自QPushButton,然后以后使用这个自定义Button的实例。
完整代码如下：
///button.h
```cpp
#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H
#include <QPushButton>
#include <QString>
#include <QWidget>
class Button : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(int curIndex READ getcurIndex WRITE setcurIndex)
public:   
    Button( QString FileName, int xnum  = 1,  QWidget *parent = 0,
            int ynum = 1, QString bkGrnd = NULL);
    Button(QVector<QString> &list, QWidget *parent = 0, QString bkGrnd = NULL);
    QList<QPixmap> *getPixmapList(void){return &pixmatpList;}
    void setcurIndex(int index){curIndex = index; update();}
    int getcurIndex(void){return curIndex;}
private:
    void setPixmapList(QVector<QString> &list);
    QPixmap *background;
protected:
    QList<QPixmap> pixmatpList;
    int curIndex;
protected:
    virtual void paintEvent ( QPaintEvent * event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent ( QEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event ) ;
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual  QSize sizeHint () const;
};
#endif // CLOSEBUTTON_H
```
///button.cpp
```cpp
#include "button.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QLabel>
#include <QDebug>
Button::Button( QString FileName, int xnum , QWidget *parent,
                int ynum, QString bkGrnd):QPushButton(parent)
{
    QPixmap pixmap(FileName);
    for (int cnt = 0; cnt < xnum; ++cnt)
    {
        for (int y = 0; y < ynum; ++ y)
        {
            pixmatpList.push_back( pixmap.copy(cnt * (pixmap.width() / xnum),
                                               y * (pixmap.height() / ynum),
                                               pixmap.width() / xnum ,
                                               pixmap.height() / ynum));
        }
    }
    if (bkGrnd != NULL)
        background  = new QPixmap(bkGrnd);
    else
        background = NULL;
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed	);
    curIndex = 0;
}
Button::Button(QVector<QString> &list, QWidget *parent,
               QString bkGrnd):QPushButton(parent)
{
    setPixmapList(list);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    if (bkGrnd != NULL)
        background  = new QPixmap(bkGrnd);
    else
        background = NULL;
    curIndex = 0;
}
void Button::setPixmapList(QVector<QString> &list)
{
    if (list.size() <= 0) return;
    pixmatpList.clear();
    for (QVector<QString>::iterator pos = list.begin();
             pos != list.end(); ++pos)
    {
        pixmatpList.push_back(QPixmap(*pos));
    }
}
void Button::paintEvent ( QPaintEvent * event)
{
     QPainter painter(this);
     painter.drawPixmap(event->rect(), pixmatpList[curIndex]);
}
void Button::enterEvent(QEvent *event)
{
    if (pixmatpList.size() > 1)
        curIndex = 1;
    else
        curIndex = 0;
    update();
    QPushButton::enterEvent(event);
}
void Button::leaveEvent(QEvent *event)
{
    curIndex = 0;
    update();
    QPushButton::leaveEvent(event);
}
void Button::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (pixmatpList.size() > 2)
        {
            curIndex = 2;
            update();
        }
    }
    QPushButton::mousePressEvent(event);
}
void Button::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (pixmatpList.size() > 1)
        {
            curIndex = 1;
            update();
        }
    }
    QPushButton::mouseReleaseEvent(event);
}
QSize Button::sizeHint() const
{
    return background != NULL? QSize(background->width(), background->height()):
                        QSize(pixmatpList[0].width(), pixmatpList[0].height());
}
```
**调用：**
```cpp
Button exitbtn=new Button(":/VanView/images/sys_button_close.png", 4, this);
```
**效果：**
![](https://img-blog.csdn.net/20160809225040510)
![](https://img-blog.csdn.net/20160809225036260)

