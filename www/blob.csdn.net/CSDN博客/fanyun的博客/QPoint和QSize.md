# QPoint和QSize - fanyun的博客 - CSDN博客
2017年01月25日 21:05:35[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：645
QPoint类定义了平面上的一个点：
一个点由一个x坐标和一个y坐标确定。
坐标类型是QCOORD（一个32位整数）。QCOORD的最小值是QCOORD_MIN（-2147483648），最大值是QCOORD_MAX（2147483647）。
坐标可以通过函数[x](http://writeblog.csdn.net/#x)()和[y](http://writeblog.csdn.net/#y)()来访问，它们可以由[setX](http://writeblog.csdn.net/#setX)()和[setY](http://writeblog.csdn.net/#setY)()来设置并且由[rx](http://writeblog.csdn.net/#rx)()由[ry](http://writeblog.csdn.net/#ry)()来参考。
假设一个点*p*，下面的语句都是等价的：
        p.setX( p.x() + 1 );
        p += [QPoint](http://writeblog.csdn.net/#QPoint)( 1, 0 );
        p.rx()++;
    
一个QPoint也可以用作是一个矢量。QPoint的加法和减法也像矢量一样定义了（每个分量分别的被相加或者相减）。你可以把一个QPoint与一个int或者一个double做乘法或者除法。函数[manhattanLength](http://writeblog.csdn.net/#manhattanLength)()像对矢量那样给出了QPoint一个比较廉价的长度接近值的说明。
Example:
        //QPoint oldPos在其它什么地方定义过了
        MyWidget::mouseMoveEvent( [QMouseEvent](http://writeblog.csdn.net/qmouseevent.html) *e )
        {
            QPoint vector = e->[pos](http://writeblog.csdn.net/qmouseevent.html#pos)() - oldPos;
            if ( vector.[manhattanLength](http://writeblog.csdn.net/#manhattanLength)() > 3 )
            ... //鼠标从oldPos移动超过3个象素
        }
    
QPoint可以进行比较得出相等或者不等，并且它们可以写到QStream或者从QStream中读出。
 也可以参考[QPointArray](http://writeblog.csdn.net/qpointarray.html)、[QSize](http://writeblog.csdn.net/qsize.html)、[QRect](http://writeblog.csdn.net/qrect.html)、[图形类](http://writeblog.csdn.net/graphics.html)和[图像处理类](http://writeblog.csdn.net/images.html)。
     QSize定义了一个二维对象的大小。
 一个大小由一个宽和一个高确定。
坐标类型是QCOORD（和int一样在qwindowdefs.h中定义的）。QCOORD的最小值是QCOORD_MIN（-2147483648），最大值是QCOORD_MAX（2147483647）。
大小可以在构造函数中设置并且可以用[setWidth](http://writeblog.csdn.net/#setWidth)()和[setHeight](http://writeblog.csdn.net/#setHeight)()来修改，或者使用[operator+=](http://writeblog.csdn.net/#operator+-eq)()、[operator-=](http://writeblog.csdn.net/#operator--eq)()、[operator*=](http://writeblog.csdn.net/#operator*-eq)()和[operator/=](http://writeblog.csdn.net/#operator/-eq)()等等。你可以使用[transpose](http://writeblog.csdn.net/#transpose)()来交换宽和高。你可以使用[expandedTo](http://writeblog.csdn.net/#expandedTo)()来得到宽和高为两个大小的宽和高的最大值的大小，并且可以使用[boundedTo](http://writeblog.csdn.net/#boundedTo)()来得到宽和高为两个大小的宽和高的最小值的大小。
 也可以参考[QPoint](http://writeblog.csdn.net/qpoint.html)、[QRect](http://writeblog.csdn.net/qrect.html)、[图形类](http://writeblog.csdn.net/graphics.html)和[图像处理类](http://writeblog.csdn.net/images.html)。
