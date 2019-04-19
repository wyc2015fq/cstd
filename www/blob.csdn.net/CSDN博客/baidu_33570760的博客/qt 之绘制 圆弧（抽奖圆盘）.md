# qt 之绘制 圆弧（抽奖圆盘） - baidu_33570760的博客 - CSDN博客
2017年05月04日 11:22:49[carman_风](https://me.csdn.net/baidu_33570760)阅读数：1767
 转载自：http://blog.csdn.net/liang19890820/article/details/51394007
# 简述
综合前面对二维绘图的介绍，想必我们对一些基本绘图有了深入的了解，下面我们来实现一些漂亮的图形绘制。
- [简述](http://blog.csdn.net/liang19890820/article/details/51394007#%E7%AE%80%E8%BF%B0)
- [圆形](http://blog.csdn.net/liang19890820/article/details/51394007#%E5%9C%86%E5%BD%A2)- [效果](http://blog.csdn.net/liang19890820/article/details/51394007#%E6%95%88%E6%9E%9C)
- [源码](http://blog.csdn.net/liang19890820/article/details/51394007#%E6%BA%90%E7%A0%81)
- [弧形](http://blog.csdn.net/liang19890820/article/details/51394007#%E5%BC%A7%E5%BD%A2)- [效果](http://blog.csdn.net/liang19890820/article/details/51394007#%E6%95%88%E6%9E%9C-1)
- [源码](http://blog.csdn.net/liang19890820/article/details/51394007#%E6%BA%90%E7%A0%81-1)
- [文本](http://blog.csdn.net/liang19890820/article/details/51394007#%E6%96%87%E6%9C%AC)- [效果](http://blog.csdn.net/liang19890820/article/details/51394007#%E6%95%88%E6%9E%9C-2)
- [源码](http://blog.csdn.net/liang19890820/article/details/51394007#%E6%BA%90%E7%A0%81-2)
- [旋转](http://blog.csdn.net/liang19890820/article/details/51394007#%E6%97%8B%E8%BD%AC)- [效果](http://blog.csdn.net/liang19890820/article/details/51394007#%E6%95%88%E6%9E%9C-3)
- [源码](http://blog.csdn.net/liang19890820/article/details/51394007#%E6%BA%90%E7%A0%81-3)
# 圆形
经常地，我们会在网上看到一些列的抽奖活动，里面就有圆盘抽奖，是不是有点手痒了O(∩_∩)O~
## 效果
![这里写图片描述](https://img-blog.csdn.net/20160513110720833)
## 源码
```java
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int radius = 150;
    int arcHeight = 30;
    // >> 1（右移1位）相当于width() / 2
    painter.translate(width() >> 1, height() >> 1);
    /**
     * 参数二：半径
     * 参数三：开始的角度
     * 参数四：指扫取的角度-顺时针（360度 / 8 = 45度）
     * 参数五：圆环的高度
     * 参数六：填充色
    **/
    gradientArc(&painter, radius, 0,  45, arcHeight, qRgb(200, 200, 0));
    gradientArc(&painter, radius, 45, 45, arcHeight, qRgb(200, 0, 200));
    gradientArc(&painter, radius, 90, 45, arcHeight, qRgb(0, 200, 200));
    gradientArc(&painter, radius, 135, 45, arcHeight, qRgb(200, 0, 0));
    gradientArc(&painter, radius, 225, 45, arcHeight, qRgb(0, 200, 0));
    gradientArc(&painter, radius, 180, 45, arcHeight, qRgb(0, 0, 200));
    gradientArc(&painter, radius, 270, 45, arcHeight, qRgb(0, 0, 0));
    gradientArc(&painter, radius, 315, 45, arcHeight, qRgb(150, 150, 150));
}
void MainWindow::gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color)
{
    // 渐变色
    QRadialGradient gradient(0, 0, radius);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1.0, color);
    painter->setBrush(gradient);
    // << 1（左移1位）相当于radius*2 即：150*2=300
    //QRectF(-150, -150, 300, 300)
    QRectF rect(-radius, -radius, radius << 1, radius << 1);
    QPainterPath path;
    path.arcTo(rect, startAngle, angleLength);
    painter->setPen(Qt::NoPen);
    painter->drawPath(path);
}
```
```java
```
# 弧形
我们可以在之前的基础上加一些处理，从而实现一个圆弧。
## 效果
![这里写图片描述](https://img-blog.csdn.net/20160513111127244)
## 源码
```
void MainWindow::gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color)
{
    // 渐变色
    QRadialGradient gradient(0, 0, radius);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1.0, color);
    painter->setBrush(gradient);
    // << 1（左移1位）相当于radius*2 即：150*2=300
    //QRectF(-150, -150, 300, 300)
    QRectF rect(-radius, -radius, radius << 1, radius << 1);
    QPainterPath path;
    path.arcTo(rect, startAngle, angleLength);
    // QRectF(-120, -120, 240, 240)
    QPainterPath subPath;
    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));
    // path为扇形 subPath为椭圆
    path -= subPath;
    painter->setPen(Qt::NoPen);
    painter->drawPath(path);
}
```
``
这些只不过是我们实现的一个小效果，如果说你有什么特殊的需要，可以在此基础上进行扩展，比如：添加文本、动画旋转等。
# 文本
可以通过QPainterPath的addText()来添加文本。
## 效果
![这里写图片描述](https://img-blog.csdn.net/20160513112000322)
## 源码
```
void MainWindow::gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color)
{
    // 渐变色
    QRadialGradient gradient(0, 0, radius);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1.0, color);
    painter->setBrush(gradient);
    // << 1（左移1位）相当于radius*2 即：150*2=300
    //QRectF(-150, -150, 300, 300)
    QRectF rect(-radius, -radius, radius << 1, radius << 1);
    QPainterPath path;
    path.arcTo(rect, startAngle, angleLength);
    // QRectF(-120, -120, 240, 240)
    QPainterPath subPath;
    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));
    // path为扇形 subPath为椭圆
    path -= subPath;
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(14);
    painter->setPen(Qt::NoPen);
    path.addText(path.pointAtPercent(0.5), font, QStringLiteral("一去丶二三里"));
    painter->drawPath(path);
}
```
``
# 旋转
我们对前面的圆盘进行强化，添加一个旋转效果。当然，常见的抽奖圆盘旋转的是指针，而我们下面实现的是对圆盘的旋转，如果你要实现一个抽奖转盘，那么可以再扩展。
## 效果
![这里写图片描述](https://img-blog.csdn.net/20160513113155717)
## 源码
```
// 利用定时器，定时变换角度，进行旋转。
QTimer *pTimer = new QTimer(this);
pTimer->setInterval(100);
connect(pTimer, SIGNAL(timeout()), this, SLOT(updatePaint()));
pTimer->start();
// 改变角度，进行旋转
void MainWindow::updatePaint()
{
    m_nRotationAngle++;
    if (m_nRotationAngle > 360)
        m_nRotationAngle = 0;
    update();
}
```
``
然后，只需要在绘图事件中添加简单的一行代码即可：
```
void MainWindow::paintEvent(QPaintEvent *)
{
    ...
    // 旋转
    painter.rotate(m_nRotationAngle);
    ...
}
```
好了，基本的介绍就到这里，是不是很有意思呢，O(∩_∩)O哈哈~
