# qt 之 自适应分辨率 - baidu_33570760的博客 - CSDN博客
2017年05月04日 11:11:46[carman_风](https://me.csdn.net/baidu_33570760)阅读数：1403
转载自：http://blog.csdn.net/liang19890820/article/details/51833870
# 简述
在自定义无边框、标题栏的界面中，需要自己实现最小化、最大化、关闭、窗体背景等功能。最小化、最大化、关闭等按钮设计及功能比较简单，这里就不多做介绍。今天主要介绍一下绘制背景的问题，主要实现自适应屏幕分辨率。
- [简述](http://blog.csdn.net/liang19890820/article/details/51833870#%E7%AE%80%E8%BF%B0)
- [实现](http://blog.csdn.net/liang19890820/article/details/51833870#%E5%AE%9E%E7%8E%B0)- [自适应方案](http://blog.csdn.net/liang19890820/article/details/51833870#%E8%87%AA%E9%80%82%E5%BA%94%E6%96%B9%E6%A1%88)
- [效果](http://blog.csdn.net/liang19890820/article/details/51833870#%E6%95%88%E6%9E%9C)
- [源码](http://blog.csdn.net/liang19890820/article/details/51833870#%E6%BA%90%E7%A0%81)
# 实现
先看一下UI设计的图（大小：1298*786）：
![这里写图片描述](https://img-blog.csdn.net/20160706095132915)
## 自适应方案
如何自适应屏幕分辨率呢？下面是常用的一些方案：
- 为每个分辨率单独做一张图
- 将图片切分为九宫格形
- 在原图基础上进行实现
下面，我们分别对它们一一进行分析：
- 常用的分辨率很多，800*600、1024*768、1280*800、1680*1050等等。。。那么得需要多少张图呢？而且资源一旦过多，会加大程序开销。
- 需要将图片切分，而且如果切分不合适，还得来回重复切图，加大UI工作量。。。
- 在原图的基础上实现，在代码逻辑上处理！
综上所述：很明显，方案3是最好的，那么如何实现呢？请继续往下看！
由于界面存在缩放，所以如果窗体有圆角、或者存在阴影效果，缩放过程中会变形，所以需要进行特殊化处理！
- 左上角、左下角、右上角、右下角进行切分分别绘制（下面所说的切分都使用代码实现）
- 左、上、右、下部位进行切分，计算出窗体的大小后，在原方向进行拉伸
- 中央部分切分出一部分进行平铺
- 让UI将圆角及阴影部分标注出来，这里需要标注宽和高，以便于实现切图。
## 效果
![这里写图片描述](https://img-blog.csdn.net/20160706095156103)
## 源码
现在，来开始我们的代码之旅吧！
```
void paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPixmap background(":/background");
    int nLeftWidth = 144;
    int nBottomHeight = 24;
    int nTopHeight = 67;
    // 分别计算左、上、右、下的区域
    QRect left(0, 100, nLeftWidth, 100);
    QRect right(background.width() - nLeftWidth, 100, nLeftWidth, 100);
    QRect leftTop(0, 0, nLeftWidth, nTopHeight);
    QRect rightTop(background.width() - nLeftWidth, 0, nLeftWidth, nTopHeight);
    QRect top(150, 0, 150, nTopHeight);
    QRect leftBottom(0, background.height() - nBottomHeight, nLeftWidth, nBottomHeight);
    QRect rightBottom(background.width() - nLeftWidth, background.height() - nBottomHeight, nLeftWidth, nBottomHeight);
    QRect bottom(150, background.height() - nBottomHeight, 100, nBottomHeight);
    QRect center(300, 300, 100, 100);
    QRect leftRect(0, nTopHeight, nLeftWidth, this->height() - nTopHeight - nBottomHeight);
    QRect rightRect(this->width() - nLeftWidth, nTopHeight, nLeftWidth, this->height() - nTopHeight - nBottomHeight);
    QRect leftTopRect(0, 0, nLeftWidth, nTopHeight);
    QRect rightTopRect(this->width() - nLeftWidth, 0, nLeftWidth, nTopHeight);
    QRect topRect(nLeftWidth, 0, this->width() - nLeftWidth*2, nTopHeight);
    QRect leftBottomRect(0, this->height() - nBottomHeight, nLeftWidth, nBottomHeight);
    QRect righttBottomRect(this->width() - nLeftWidth, this->height() - nBottomHeight, nLeftWidth, nBottomHeight);
    QRect bottomRect(nLeftWidth, this->height() - nBottomHeight, this->width() - nLeftWidth*2, nBottomHeight);
    QRect centerRect(nLeftWidth, nTopHeight, this->width() - nLeftWidth*2, this->height() - nTopHeight - nBottomHeight);
    // 绘制图片
    painter.drawPixmap(topRect, background, top);
    painter.drawPixmap(leftRect, background, left);
    painter.drawPixmap(rightRect, background, right);
    painter.drawPixmap(rightTopRect, background, rightTop);
    painter.drawPixmap(leftTopRect, background, leftTop);
    painter.drawPixmap(leftBottomRect, background, leftBottom);
    painter.drawPixmap(righttBottomRect, background, rightBottom);
    painter.drawPixmap(bottomRect, background, bottom);
    painter.drawPixmap(centerRect, background, center);
}
```
