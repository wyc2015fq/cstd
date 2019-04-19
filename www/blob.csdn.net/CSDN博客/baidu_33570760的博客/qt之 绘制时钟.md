# qt之 绘制时钟 - baidu_33570760的博客 - CSDN博客
2017年05月04日 11:19:55[carman_风](https://me.csdn.net/baidu_33570760)阅读数：306
 转载自：http://blog.csdn.net/liang19890820/article/details/52064169
QPainter 提供了 2D 绘图的常用操作，QTimer 提供了定时器功能，将两者相结合，可以做出很多的自定义特效绘制。
下面，来实现一个每天都要接触的东西 - 时钟。包含了常见的所有功能：时针、分针、秒针。。。
- [简述](http://blog.csdn.net/liang19890820/article/details/52064169#%E7%AE%80%E8%BF%B0)
- [实现方式](http://blog.csdn.net/liang19890820/article/details/52064169#%E5%AE%9E%E7%8E%B0%E6%96%B9%E5%BC%8F)
- [示例](http://blog.csdn.net/liang19890820/article/details/52064169#%E7%A4%BA%E4%BE%8B)- [效果](http://blog.csdn.net/liang19890820/article/details/52064169#%E6%95%88%E6%9E%9C)
- [源码](http://blog.csdn.net/liang19890820/article/details/52064169#%E6%BA%90%E7%A0%81)
# 实现方式
由于时钟是妙级更新的，所以我们需要定时刷新，时钟本身则使用之前讲过的 QPainter 来进行绘制。
- 使用 QTimer 定时刷新，设置超时时间为 1000 毫秒（1 秒）。
- 绘制时钟，通过 paintEvent() 实现，包括：时针、分针、秒针、及面板、表盘等。
- 绘制小时对应的文本，文本区域通过 textRectF() 计算。
# 示例
## 效果
![这里写图片描述](https://img-blog.csdn.net/20170120132712612?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlhbmcxOTg5MDgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 源码
首先，我们构造一个定时器 QTimer，连接其超时信号 timeout() 到槽函数 update()，当调用 update() 时，系统会自动通知当前界面进行重绘（paintEvent()）。
```
ClockWidget::ClockWidget(QWidget *parent)
    : QWidget(parent)
{
    ...
    // 构造定时器，设置超时为 1 秒
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
    ...
}
```
``
实现一个用于获取文本区域的接口：
```
QRectF ClockWidget::textRectF(double radius, int pointSize, double angle)
{
    QRectF rectF;
    rectF.setX(radius*qCos(angle*M_PI/180.0) - pointSize*2);
    rectF.setY(radius*qSin(angle*M_PI/180.0) - pointSize/2.0);
    rectF.setWidth(pointSize*4);
    rectF.setHeight(pointSize);
    return rectF;
```
``
**注意：**这里，textRectF() 主要用于获取小时对应的文本区域。
重写 paintEvent()，用于绘制时钟。
```
void ClockWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    // 时针、分针、秒针位置 - 多边形
    static const QPoint hourHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -30)
    };
    static const QPoint minuteHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -65)
    };
    static const QPoint secondHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -80)
    };
    // 时针、分针、秒针颜色
    QColor hourColor(200, 100, 0, 200);
    QColor minuteColor(0, 127, 127, 150);
    QColor secondColor(0, 160, 230, 150);
    int side = qMin(width(), height());
    QTime time = QTime::currentTime();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // 平移坐标系原点至中心点
    painter.translate(width() / 2, height() / 2);
    // 缩放
    painter.scale(side / 200.0, side / 200.0);
    // 绘制时针
    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);
    painter.save();
    // 每圈360° = 12h 即：旋转角度 = 小时数 * 30°
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();
    painter.setPen(hourColor);
    // 绘制小时线 （360度 / 12 = 30度）
    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }
    int radius = 100;
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);
    int pointSize = font.pointSize();
    // 绘制小时文本
    int nHour = 0;
    for (int i = 0; i < 12; ++i) {
        nHour = i + 3;
        if (nHour > 12)
            nHour -= 12;
        painter.drawText(textRectF(radius*0.8, pointSize, i * 30), Qt::AlignCenter, QString::number(nHour));
    }
    // 绘制分针
    painter.setPen(Qt::NoPen);
    painter.setBrush(minuteColor);
    painter.save();
    // 每圈360° = 60m 即：旋转角度 = 分钟数 * 6°
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();
    painter.setPen(minuteColor);
    // 绘制分钟线 （360度 / 60 = 6度）
    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }
    // 绘制秒针
    painter.setPen(Qt::NoPen);
    painter.setBrush(secondColor);
    painter.save();
    // 每圈360° = 60s 即：旋转角度 = 秒数 * 6°
    painter.rotate(6.0 * time.second());
    painter.drawConvexPolygon(secondHand, 3);
    painter.restore();
}
```
好了，注释写的很清楚了，就不多做讲解了，如果要做一个应用，可以自己实现一些设置指针、表盘样式外观的一些接口。
**注意：**网上很多文本都是在旋转之后绘制上去的，效果并不好（6 显示的是 9），所以请避免这一点！ 
