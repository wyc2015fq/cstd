# QT视图标尺功能实现(待补充) - fanyun的博客 - CSDN博客
2017年01月25日 09:26:32[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2637
实现了QT的视图的标尺功能
![](http://images.cnblogs.com/cnblogs_com/olswcsr/a.png)
![](http://images.cnblogs.com/cnblogs_com/olswcsr/b.png)
首先一个继承视图QFrame的类，重载paintEvent，其中用于绘制坐标，坐标的计算是根据绘制间隔线的数据得到的
在里面一层继承视图QGraphicView，重载drawBackground绘制基线，并记录相关数据，比如（线间距离，线段数，缩放比例等）
部分实现代码：
```
int     cn = scene->coordinateNumber();
    int     cofs = scene->coordinateOffset();
    int     sceneHeight = height();
    QRectF  rect;
    //ver
    for( int i = 0 ; i < cn ; i += 5 )
    {
        painter->setFont( maxFont );
        QPointF pt( 0 ,  i * cofs );
        pt = scene->viewportTransform().map( pt );//将坐标转换成scene的viewport坐标系中
        //小于0或大于当前窗口大小都不需要显示
        if( pt.y() < 0 )
            continue;
        if( pt.y() > sceneHeight )
            break;
        rect.setTop( pt.y() + 16.0f );
        rect.setLeft( 0 );
        rect.setWidth( 12.0f );
        rect.setHeight( 12.0f );
        painter->drawText( rect  , Qt::AlignCenter , tr("%1").arg(i) );
    }
```
