# Qt自适应图片之scaled()函数详解 - fanyun的博客 - CSDN博客
2017年11月26日 21:53:17[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1752
       如何在改变窗口组件的情况下改变窗口背景图片的大小，我们通常会使用Qt自带的scaled()函数；QImage、QPixmap等绘图设备类都提供scaled()函数。
scaled()是一个重载函数，按照指定的宽和高，根据纵横比模式和转换模式从原有图像返回一个经过比例转换的图像，如果宽高为0，返回一个空图像
所以，获取控件的改变后的宽高，就能设定图像转换的宽高转换比例，用scaled()的返回重新进行绘图即可自适应窗口，以下是个例子:
```cpp
void Widget::paintEvent(QPaintEvent *)  
{  
  
    QImage img((unsigned char*)im.data,im.cols,  
                   im.rows,QImage::Format_RGB888);  
    QPainter painter(this);  
    if(0==flag)  
        painter.drawImage(0,0,nImg);  
    /* 
        一定要加标记位判断，控件在绘制之前的size为NULL， 
        所以scaled()返回值也为NULL，会提示nImg是空的 
    */  
    else if(1==flag)  
    {  
        nImg=img.scaled(width(),height());  
        painter.drawImage(0,0,nImg);  
    }  
}
```
备注：如果图片缩放比例较大，最好原始资源图片的分辨率高点，否则会在放大后，出现图片模糊。
