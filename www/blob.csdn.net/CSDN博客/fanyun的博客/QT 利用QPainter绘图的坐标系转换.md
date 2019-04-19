# QT 利用QPainter绘图的坐标系转换 - fanyun的博客 - CSDN博客
2017年01月25日 09:15:17[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：8522标签：[Qt](https://so.csdn.net/so/search/s.do?q=Qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/fanyun_01/article/category/6429937)
       Qt绘制图形时，少不了坐标计算，那么如何更好更快地计算出坐标呢？现在来分析一下。
       如下图所示，原来坐标系是(0,0)X axis右Y axis下
现在想把它变成，坐标在窗口中间，X右，Y上，标准的数学中的坐标系。
Painter.setWindow(x,y,width().height());
用setWindow这个函数，表示什么意思。前两个参数左上角位置，后两个参数宽高。
![](https://img-blog.csdn.net/20161231220032203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个是原本的坐标系，要把他变成下面这个
![](https://img-blog.csdn.net/20161231220115707?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
怎么算呢，可以把变换好的坐标系在纸上画出来，左上角坐标传给前两个参数，后两个参数宽高可以算出来。
```cpp
Width= 50-(-50) = 100;
Height = -50-50 = -100;
```
那么就是painter.setWindow(-50,50,100,-100);
一般painter.setWindow(-width()/2,height()/2,width(),-height());就可以达到目的了
|World Corrdinates|===>|Window Coordinates|===>|Device Coordinates|
|----|----|----|----|----|
|(逻辑坐标)|世界变换|中间态坐标|窗口视口变换|(物理坐标)|
在默认情况下，3个坐标系是一致的。
世界变换
世界变换直接相关的函数：
|Qpainter::setWorldMatrixEnabled|启用，禁用世界变换|
|----|----|
|Qpainter:;setWorldTransform|设置世界变换|
|Qpainter：：worldTransform|获取当前|
|Qpainter：：resetTransform|重置Qtransform()|
4个常用的函数:Qpainter::scale  Qpainter::shear  Qpainter::rotate Qpainter::translate
注:它们通过直接调用的Qtransform的相应成员直接修改世界变换
```cpp
Void Qpainter::scale(qreal sx,qreal sy)
{
......
    d->state->worldMatrix.scale(sx,sy);
.......
}
```
世界变换的两个马甲:Qpainter::setTransform  Qpainter::transform
```cpp
Void Qpainter::setTransform(const Qtransfrom &transform,bool combine)
{
    setWorldTransform(transform,combine);
}
```
废弃的函数(从Qt4.3开始，Qtransform取代了Qmatrix的位置，下列函数已不建议使用)：
Qpainter::setWorldMatrix   Qpainter::worldMatrix
窗口视口变换
直接相关:
|Qpainter::setViewTransformEnabled|启用，禁用视口变换|
|----|----|
|Qpainter::viewTransformEnabled||
|Qpainter::setViewport|设置 视口(物理坐标)|
|Qpainter::setWindow|设置 窗口(与视口是同一矩形，中间太坐标)|
该变换是简单的线性变换。
复合变换
窗口视口变换和世界变换的复合:Qpainter::combinedTransform
```cpp
Qtransform Qpainter::combinedTransform() const
{
    Q_D(const QPainter);
    Return d->state->worldMatrix* d->viewTransform();
}
```
典型应用:对鼠标事件的响应中，讲坐标从物理坐标变换Qpainter需要的逻辑坐标
仿射变换、透射变换
Qt4.3(包括)之前的Qmatrix只支持仿射变换(Affine
 transformation)
平移(Translation)  缩放(Scale)  旋转(Rotation)  剪切(Shear)
Qtransform支持透射变换(perspective transformation).
|M11|M12|M13|
|----|----|----|
|M21|M22|M23|
|M31 dx|M32 dy|M33|
变换关系：
```cpp
X’=m11*x + m21*y +dx
Y’=m22*y + m12*x +dy
if(is not affine){
    w’=m13*x + m23*y + m33
    x’/=w’
    y’/=w’
}
```
相关(射影几何学，仿射几何学，欧式几何学)
        Qt的坐标系统常用的几个函数：  
        translate()函数，进行平移变换；scale()函数，进行比例变换；rotate()函数，进行旋转变换；shear()函数，进行扭曲变换。
translate()函数，进行平移变换；scale()函数，进行比例变换；rotate()函数，进行旋转变换；shear()函数，进行扭曲变换。最后介绍两个有用的函数save()和restore()，利用它们来保存和弹出坐标系的状态，从而实现快速利用几个变换来绘图。
一、坐标系简介：
        Qt中每一个窗口都有一个坐标系，默认的，窗口左上角为坐标原点，然后水平向右依次增大，水平向左依次减小，垂直向下依次增大，垂直向上依次减小。原点即为（0，0）点，然后以像素为单位增减。
例如：
```cpp
void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::red);
    painter.drawRect(0,0,100,100);
    painter.setBrush(Qt::yellow);
    painter.drawRect(-50,-50,100,100);
}
```
        我们先在原点（0，0）绘制了一个长宽都是100像素的红色矩形，又在（-50，-50）点绘制了一个同样大小的黄色矩形。可以看到，我们只能看到黄色矩形的一部分。效果如下图。
二、坐标系变换：
        坐标系变换是利用变换矩阵来进行的，我们可以利用QTransform类来设置变换矩阵，因为一般我们不需要进行更改，所以这里不在涉及。下面我们只是对坐标系的平移，缩放，旋转，扭曲等应用进行介绍。
1.利用translate()函数进行平移变换
```cpp
void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::yellow);
    painter.drawRect(0,0,50,50);
   painter.translate(100,100); //将点（100，100）设为原点
    painter.setBrush(Qt::red);
    painter.drawRect(0,0,50,50);
   painter.translate(-100,-100);
    painter.drawLine(0,0,20,20);
}
```
效果如下：
            这里将（100，100）点作为了原点，所以此时（100，100）就是（0，0）点，以前的（0，0）点就是
（-100，-100）点。要想使原来的（0，0）点重新成为原点，就是将（-100，-100）设为原点。
2.利用scale()函数进行比例变换，实现缩放效果
```cpp
void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::yellow);
    painter.drawRect(0,0,100,100);
    painter.scale(2,2); //放大两倍
    painter.setBrush(Qt::red);
    painter.drawRect(50,50,50,50);
}
```
      效果如下：
可以看到，painter.scale(2,2)，是将横纵坐标都扩大了两倍，现在的（50，50）点就相当于以前的（100，100）点。
3.利用shear()函数就行扭曲变换
```cpp
void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::yellow);
    painter.drawRect(0,0,50,50);
    painter.shear(0,1); //纵向扭曲变形
    painter.setBrush(Qt::red);
    painter.drawRect(50,0,50,50);
}
```
效果如下：
        这里，painter.shear(0,1)，是对纵向进行扭曲，0表示不扭曲，当将第一个0更改时就会对横行进行扭曲，关于扭曲变换到底是什么效果，你观察一下是很容易发现的。
4.利用rotate()函数进行比例变换，实现缩放效果
```cpp
void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawLine(0,0,100,0);
    painter.rotate(30); //以原点为中心，顺时针旋转30度
    painter.drawLine(0,0,100,0);
    painter.translate(100,100);
    painter.rotate(30);
    painter.drawLine(0,0,100,0);
}
```
效果如下：
        因为默认的rotate()函数是以原点为中心进行顺时针旋转的，所以我们要想使其以其他点为中心进行旋转，就要先进行原点的变换。这里的painter.translate(100,100)将（100，100）设置为新的原点，想让直线以其为中心进行旋转，可是你已经发现效果并非如此。是什么原因呢？我们添加一条语句，如下：
```cpp
void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawLine(0,0,100,0);
    painter.rotate(30); //以原点为中心，顺时针旋转30度
    painter.drawLine(0,0,100,0);
    painter.rotate(-30);
    painter.translate(100,100);
    painter.rotate(30);
    painter.drawLine(0,0,100,0);
}
```
效果如下：
        这时就是我们想要的效果了。我们加的一句代码为painter.rotate(-30)，这是因为前面已经将坐标旋转了30度，我们需要将其再旋转回去，才能是以前正常的坐标系统。不光这个函数如此，这里介绍的这几个函数均如此，所以很容易出错。下面我们将利用两个函数来很好的解决这个问题。
三、坐标系状态的保护
       我们可以先利用save()函数来保存坐标系现在的状态，然后进行变换操作，操作完之后，再用restore()函数将以前的坐标系状态恢复，其实就是一个入栈和出栈的操作。
例如：
```cpp
void Dialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.save(); //保存坐标系状态
    painter.translate(100,100);
    painter.drawLine(0,0,50,50);
    painter.restore(); //恢复以前的坐标系状态
    painter.drawLine(0,0,50,50);
}
```
效果如下：
             利用好这两个函数，可以实现快速的坐标系切换，绘制出不同的图形。
            
