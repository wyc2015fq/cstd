# Qt 帧动画 （QWidget） - xqhrs232的专栏 - CSDN博客
2018年01月08日 10:13:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：87
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/CSND_Ayo/article/details/70175385](http://blog.csdn.net/CSND_Ayo/article/details/70175385)
# 引言
动画很让人烦恼，在Qt中，已经给出了很多简单的动画，例如 QPropertyAnimation 类实现的动画，但是还不够智能，不是我想要的，相信你也有同感，今天我们就来实现`自定义动画类`来方便我们日后的开发。
版权所有：瓯裔，转载请注明出处：[http://blog.csdn.net/csnd_ayo](http://blog.csdn.net/csnd_ayo)
# 简介
操作系统：window7 x64 
编程IDE：Qt Creator 4.2.1 
Qt版本： 5.0.3 · 5.3.0 · 5.8.0 
最后更新：2017年4月14日
- [引言](http://blog.csdn.net/CSND_Ayo/article/details/70175385#%E5%BC%95%E8%A8%80)
- [简介](http://blog.csdn.net/CSND_Ayo/article/details/70175385#%E7%AE%80%E4%BB%8B)
- [示例](http://blog.csdn.net/CSND_Ayo/article/details/70175385#%E7%A4%BA%E4%BE%8B)- [效果](http://blog.csdn.net/CSND_Ayo/article/details/70175385#%E6%95%88%E6%9E%9C)
- [下载](http://blog.csdn.net/CSND_Ayo/article/details/70175385#%E4%B8%8B%E8%BD%BD)
- [原理](http://blog.csdn.net/CSND_Ayo/article/details/70175385#%E5%8E%9F%E7%90%86)
- [实现](http://blog.csdn.net/CSND_Ayo/article/details/70175385#%E5%AE%9E%E7%8E%B0)- [逻辑函数](http://blog.csdn.net/CSND_Ayo/article/details/70175385#%E9%80%BB%E8%BE%91%E5%87%BD%E6%95%B0)
# 示例
## 效果
- 
效果展示 
为了减小文件，我对gif动画做了一些删减，所以看起来好像有跳帧的现象，其实并没有。
![Qt动画实现效果展示图](https://img-blog.csdn.net/20170414190045690?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ1NORF9BeW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
所需资源
![瓯裔](https://img-blog.csdn.net/20170414190204802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ1NORF9BeW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![瓯裔](https://img-blog.csdn.net/20170414194910583?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ1NORF9BeW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
封装后引用
封装后我只需要五行代码即可显示自定义的动画了。
```
#include "widget.h"
#include "ui_widget.h"
#include "customdynamicwidget.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    dynamicAnima_ = new CustomDynamicWidget(this);
    dynamicAnima_->setAnimation(QPixmap(":/q/level_num.png"),10,100);
    dynamicAnima_->setGeometry(41,41,41,41);
    dynamicAnima_->show();
    // 顺时针播放每帧
    dynamicAnima_->startClockwise();
}
Widget::~Widget()
{
    delete ui;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
## 下载
拥有了这个类，你就拥有了所有动画！
代码：[下载](http://download.csdn.net/download/csnd_ayo/9849505)
图片：[下载](https://img-blog.csdn.net/20170414190204802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ1NORF9BeW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 原理
- 实现关联
```cpp
#include <QTimer>
  #include <QPainter>
  #include <QWidget>
```
- 1
- 2
- 3
- 
逻辑流程
- 
等比例切割传入的图片元素
- 创建定时器，并关联更新槽
- 等比例对图片进行切割
- 
利用了`定时器(QTimer)` 定时更新界面
- 定时器被触发
- 辨别当前帧
- 若是尾帧辨别是否启用了动画无限循环
- 修改帧
- 更新界面
- 
重写paintEvent函数，利用`画家类(QPainter)` 完成自绘。
- 获取当前帧的对应图片
- 更新当前窗口
# 实现
## 逻辑函数
- 1、等比例切割传入的图片元素
```
/*
* 设置动画
* setAnimation
* _pix：图片
* _count：图片帧数
* _msec：切换速度 （毫秒 1秒 = 1000毫秒）
*/
void CustomDynamicWidget::setAnimation(const QPixmap &_pix, const short _count, const int _msec) {
    count_ = _count;
    currentIndex_ = 0;
    if (!pixList_.empty()) {
        pixList_.clear();
    }
    else {
        /*  顺时针动画关联  */
        clockTimer_ = new QTimer(this);
        clockTimer_->setInterval(_msec);
        connect(clockTimer_, SIGNAL(timeout()), this, SLOT(updateClockwise()));
        /*  逆时针动画关联  */
        counterclockTimer_ = new QTimer(this);
        counterclockTimer_->setInterval(_msec);
        connect(counterclockTimer_, SIGNAL(timeout()), this, SLOT(updateCounterclockwise()));
    }
    /*  链式动画图标分离  */
    for(short i=0; i != _count; ++i) {
        pixList_.append(_pix.copy(i * (_pix.width() / _count), 0,
                        _pix.width() / _count, _pix.height()));
    }
    currentPix_ = pixList_.at(0);
    this->setGeometry(0,0,currentPix_.width(),currentPix_.height());
    update();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 2、利用了`定时器(QTimer)` 定时更新界面
```
void CustomDynamicWidget::updateClockwise(void) {
    do {
        if (currentIndex_ < count_ && currentIndex_ >= 0) {
            /*  更新帧  */
            currentPix_ = pixList_.at(currentIndex_);
            update();
            /*  判断帧数  */
            if (currentIndex_ >= (count_ - 1)) {
                if(isLoop_) {
                    currentIndex_ = 0;
                    return;
                }
                break;
            }
            /*  跳帧  */
            ++currentIndex_;
            return;
        }
    #ifndef QT_NO_DEBUG
        else {
            qDebug() << __FUNCTION__ << "waring: 错误的下标" << currentIndex_;
        }
    #endif
    } while(false);
    clockTimer_->stop();
    currentIndex_ = 0;
    emit clockwiseFinished();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 3、重写paintEvent函数，利用`画家类(QPainter)` 完成自绘。
```
void CustomDynamicWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(rect(), currentPix_);
}
```
- 1
- 2
- 3
- 4
- 头文件
```
#ifndef CUSTOMDYNAMICWIDGET_H
#define CUSTOMDYNAMICWIDGET_H
/*
* 自定义动画类
* 作者：瓯裔
* 邮箱：727057301@qq.com
* CSDN：http://blog.csdn.net/csnd_ayo
* 创建时间：2017年1月9日 19:34:13
* QT版本：5.0.2 ~ 5.8.0
*/
#include <QWidget>
/*
* 说明：
*   当前类是针对一张链式图片
*   链式图片默认为8帧，默认0.1秒播放一帧
*   可以根据自己需要进行设置与更改
*   进行自动切割，循环播放每一帧
*
* 示例：
*   类内声明 CustomDynamicWidget* dynamicAnima_
*   ui->setupUi(this);
*   dynamicAnima_ = new CustomDynamicWidget(this);
*   dynamicAnima_->setAnimation(QPixmap(":/res/loading.png"),8,100);
*   dynamicAnima_->setGeometry(100,100,300,300);
*   dynamicAnima_->show();
*   // 顺时针播放每帧
*   dynamicAnima_->startClockwise();
*/
class QTimer;
class CustomDynamicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomDynamicWidget(QWidget *parent = 0);
    /*
    * 设置动画图标
    * 函数名：setAnimation
    * 参数 _pix：图标实例
    * 参数 _count：图标实例动画帧数
    * 参数 _msec：动画切帧速度 (毫秒级)
    */
    void setAnimation(const QPixmap& _pix, const short _count = 8, const int _msec = 100);
    /*  开始动画(顺时针)  */
    void startClockwise(void);
    /*  开始动画(逆时针)  */
    void startCounterclockwise(void);
    /*  停止动画  */
    void stop(void);
    /*  设置动画无限循环  */
    void setLoop(const bool _isLoop = false) { isLoop_ = _isLoop; }
signals:
    /*  顺时针动画结束  */
    void clockwiseFinished(void);
    /*  逆时针动画结束  */
    void counterclockwiseFinished(void);
private slots:
    /*  顺时针动画槽  */
    void updateClockwise(void);
    /*  逆时针动画槽  */
    void updateCounterclockwise(void);
protected:
    void paintEvent(QPaintEvent *);
private:
    /*  动画(是否无限循环)  */
    bool isLoop_;
    /*  图标列表数量  */
    short count_;
    /*  当前展示的图标下标  */
    short currentIndex_;
    /*  控制顺时针槽定时器  */
    QTimer *clockTimer_;
    /*  控制逆时针槽定时器  */
    QTimer *counterclockTimer_;
    /*  当前展示的图标  */
    QPixmap currentPix_;
    /*  图标列表  */
    QList<QPixmap> pixList_;
};
#endif // CUSTOMDYNAMICWIDGET_H
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
版权声明：分享得以延续，交流方知彼此。作者：_OE_ 博客地址：http://blog.csdn.net/csnd_ayo
- 
本文已收录于以下专栏：
- [《Qter
 必知必会》](http://blog.csdn.net/column/details/15254.html)

