# 用Qt设计一个属于自己的图标按钮 - xqhrs232的专栏 - CSDN博客
2018年01月08日 10:15:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1409
原文地址::[http://blog.csdn.net/u010101067/article/details/41578315](http://blog.csdn.net/u010101067/article/details/41578315)
 学c++以来，用qt做过一些界面，个人感觉软件界面还是比较漂亮的，但是如果界面想嵌入一些漂亮的图标做按钮，利用qt本身已有的控件实现就有点困难了，当然可以通过为QPushButton类添加背景图标来实现，但是自我感觉效果不是很好，所以写了这篇博客，想实现一下用纯图片来做按钮控件，通过改变图片的观感，比如图片的形状大小，来表示按钮的选中或者未选中，同时为这图片按钮添加鼠标事件，从而模拟一个按键出来，这样的好处主要是它能为界面增色，不同的图片，形状，大小以及按钮选中和未选中所展现给用户的不同观感将大大提高界面整体的美化程度。
设计思路主要通过重载Qt的几个事件实现，
void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
void leaveEvent(QEvent *event);
同时还需要为图标按钮添加一个信号函数，void isclicked()，由该函数发送鼠标按下的信号，供其他类使用。
代码简要分析：
/*IconButton类的构造函数*/
IconButton::IconButton(QString iconStr1,QString iconStr2,QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint)
{
    //加载图片作为程序的界面
    m_Pixmap.load(iconStr1);
    m_Pixmap1.load(iconStr2);
    if(m_Pixmap1.size().width()<m_Pixmap.size().width())
    {
        m_Pixmap.load(iconStr2);
        m_Pixmap1.load(iconStr1);
    }
    leave=true; //初始化图标按钮为未选中状态
    this->x=0;  //初始化图标按钮的位置
    this->y=0;
}
/*重载mousePressEvent函数*/
void IconButton::mousePressEvent(QMouseEvent *event)
{
    //按下左键则发送按下信号
    if(event->button() == Qt::LeftButton)
    {
        emit isclicked();
    }
}
/*重载paintEvent函数*/
void IconButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
//分两种情况来显示不同的外观，未选中时画m_Pixmap，选中时画m_Pixmap1
    if(leave)
    {
        resize( m_Pixmap.size() );
        painter.drawPixmap(0,0, m_Pixmap);
        move(x+(m_Pixmap1.size().width()-m_Pixmap.size().width())/2,y+(m_Pixmap1.size().width()-m_Pixmap.size().width())/2);
    }
   else
    {
        resize( m_Pixmap1.size() );
        painter.drawPixmap(0, 0, m_Pixmap1);
        move(x,y);
        this->setToolTip(iconTip);
    }
}
/*重载:leaveEvent函数*/
void IconButton::leaveEvent(QEvent *event)
{
    //鼠标离开窗口时是普通的指针
    setCursor(Qt::ArrowCursor);
    leave=true;
    update(); //调用paintevent函数进行更新
}
/*重载:enterEvent函数*/
void IconButton::enterEvent(QEvent *event)
{
    //鼠标留在窗口上时是一个手指，表示可以读取按键事件
    setCursor(Qt::PointingHandCursor);
    leave=false;
    update();//调用paintevent函数进行更新
}
//下面是供给用户使用的两个基本接口，分别是设置图标按钮的位置以及需要显示的提示信息，这里只简单的写了两个功能，还可以增加其他功能，使这个IconButton类更加完善
void IconButton::setPosition(int x,int y)
{
    this->x=x;
    this->y=y;
}
void IconButton::setIconTip(QString iconTip)
{
    this->iconTip=iconTip;
}
以下是实现该类的源码：
iconButton.h
[cpp][view
 plain](http://blog.csdn.net/u010101067/article/details/41578315#)[copy](http://blog.csdn.net/u010101067/article/details/41578315#)
- #ifndef ICONBUTTON_H
- #define ICONBUTTON_H
- 
- #include <QtGui/QWidget>
- class IconButton : public QWidget  
- {  
-     Q_OBJECT  
- public:  
-     IconButton(QString iconStr1,QString iconStr2,QWidget *parent = 0);  
- void setPosition(int x,int y);  
- void setIconTip(QString iconTip);  
- 
- protected:  
- void mousePressEvent(QMouseEvent *event);  
- void paintEvent(QPaintEvent *event);  
- void enterEvent(QEvent *event);  
- void leaveEvent(QEvent *event);  
- signals:  
- void isclicked();  
- private:  
-     QPixmap m_Pixmap,m_Pixmap1;  
- int x,y;  
-     QString iconTip;  
- bool leave;  
- };  
- 
- 
- #endif // ICONBUTTON_H
[cpp][view
 plain](http://blog.csdn.net/u010101067/article/details/41578315#)[copy](http://blog.csdn.net/u010101067/article/details/41578315#)
- 
[cpp][view
 plain](http://blog.csdn.net/u010101067/article/details/41578315#)[copy](http://blog.csdn.net/u010101067/article/details/41578315#)
- iconButton.cpp  
[cpp][view
 plain](http://blog.csdn.net/u010101067/article/details/41578315#)[copy](http://blog.csdn.net/u010101067/article/details/41578315#)
- 
[cpp][view
 plain](http://blog.csdn.net/u010101067/article/details/41578315#)[copy](http://blog.csdn.net/u010101067/article/details/41578315#)
- <pre class="cpp" name="code">#include "iconButton.h"
- #include <QtGui>
- 
- IconButton::IconButton(QString iconStr1,QString iconStr2,QWidget *parent)  
-     : QWidget(parent, Qt::FramelessWindowHint)  
- {  
- 
- //加载一幅有部分区域是透明的图片作为程序的界面
-     m_Pixmap.load(iconStr1);  
-     m_Pixmap1.load(iconStr2);  
- if(m_Pixmap1.size().width()<m_Pixmap.size().width())  
-     {  
-         m_Pixmap.load(iconStr2);  
-         m_Pixmap1.load(iconStr1);  
-     }  
- //setStyleSheet("background-color:lightblue");
-     leave=true;  
- this->x=0;  
- this->y=0;  
- 
- }  
- 
- void IconButton::mousePressEvent(QMouseEvent *event)  
- {  
- //按住左键关闭程序
- if(event->button() == Qt::LeftButton)  
-     {  
-         emit isclicked();  
-     }  
- }  
- void IconButton::setPosition(int x,int y)  
- {  
- this->x=x;  
- this->y=y;  
- }  
- void IconButton::setIconTip(QString iconTip)  
- {  
- this->iconTip=iconTip;  
- }  
- 
- void IconButton::paintEvent(QPaintEvent *event)  
- {  
-     qDebug("paint");  
-     QPainter painter(this);  
- if(leave)  
-     {  
-         resize( m_Pixmap.size() );  
- //setMask( m_Pixmap.mask() );
-         painter.drawPixmap(0,0, m_Pixmap);  
-         move(x+(m_Pixmap1.size().width()-m_Pixmap.size().width())/2,y+(m_Pixmap1.size().width()-m_Pixmap.size().width())/2);  
-     }  
- else
-     {  
-         resize( m_Pixmap1.size() );  
- //setMask( m_Pixmap1.mask() );
-         painter.drawPixmap(0, 0, m_Pixmap1);  
-         move(x,y);  
- this->setToolTip(iconTip);  
-     }  
- }  
- 
- void IconButton::leaveEvent(QEvent *event)  
- {  
- //鼠标离开窗口时是普通的指针
-     setCursor(Qt::ArrowCursor);  
-     leave=true;  
-     update();  
- }  
- 
- void IconButton::enterEvent(QEvent *event)  
- {  
- //鼠标留在窗口上时是一个手指
-     setCursor(Qt::PointingHandCursor);  
-     leave=false;  
-     update();  
- }  
