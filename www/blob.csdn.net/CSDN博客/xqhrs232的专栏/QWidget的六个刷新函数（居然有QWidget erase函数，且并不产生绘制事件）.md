# QWidget的六个刷新函数（居然有QWidget::erase函数，且并不产生绘制事件） - xqhrs232的专栏 - CSDN博客
2017年07月25日 17:02:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：481
原文地址::[http://www.cnblogs.com/findumars/p/6009579.html](http://www.cnblogs.com/findumars/p/6009579.html)
相关文章
1、[Qt重绘之update,repaint详解](http://blog.csdn.net/zb872676223/article/details/38084225)----[http://blog.csdn.net/zb872676223/article/details/38084225](http://blog.csdn.net/zb872676223/article/details/38084225)
一．主要理解一下几个方法和属性：
1．QWidget * QScrollView::viewport () const
2．void QWidget::paintEvent ( QPaintEvent * ) [虚 保护]
3．void QWidget::repaint ( int x, int y, int w, int h, bool erase = TRUE ) [槽]
4．void QWidget::update () [槽]
5．void QWidget::erase ( int x, int y, int w, int h )
6．bool updatesEnabled
二．现分别详细介绍：
1．QWidget * QScrollView::viewport () const
返回滚动视图中的视口窗口部件，这个窗口部件包含内容窗口部件或者要画的区域。
2．void QWidget::paintEvent ( QPaintEvent * ) [虚 保护]
只要窗口部件需要被重绘就被调用。每个要显示输出的窗口部件必须实现它。这个事件处理器可以在子类中被重新实现来接收绘制事件。 它可以是repaint()或update()的结果。 很多窗口部件在当它们被请求时，它们很简单地重新绘制整个界面，但是一些窗口部件通过仅仅绘制被请求的区域QPaintEvent::region()进 行优化，例如，QListView和QCanvas就是这样做的。
Qt也可通过把多个绘制事件合并为一个来加快绘制速度。当update()被调用几次或者窗口系统发送几次绘制事件，Qt把它们合并为 一个比较大区域（请参考QRegion::unite()）的一个事件中。repaint()不允许这样优化，所以只要可能我们尽量使用update ()。
当绘制事件发生，更新区域通常被擦除。这里有一些例外，通过QPaintEvent::erased()可以得知这个窗口部件是否被擦除。
3．void QWidget::repaint ( int x, int y, int w, int h, bool erase = TRUE ) [槽]
通过立即调用paintEvent()来直接重新绘制窗口部件，如果erase为真，Qt在paintEvent()调用之前擦除区域 (x,y,w,h)。 如果w是负数，它被width()-x替换，并且如果h是负数，它被height()-y替换。 如果你需要立即重新绘制，建议使用repaint()，比如在动画期间。在绝大多数情况下，update()更好，因为它允许Qt来优化速度并且防止闪 烁。 警告：如果你在一个函数中调用repaint()，而它自己又被paintEvent()调用，你也许会看到无线循环。update()函数从来不会产生
 循环。
4．void QWidget::update () [槽]
更新窗口部件，当Qt回到主事件中时，它规划了所要处理的绘制事件。这样允许Qt进行优化从而得到比调用repaint()更快的速度和更 少的闪烁。 几次调用update()的结果通常仅仅是一次paintEvent()调用。 Qt通常在paintEvent()调用之前擦除这个窗口部件的区域，仅仅只有在WRepaintNoErase窗口部件标记被设置的时候才不会。
5．void QWidget::erase ( int x, int y, int w, int h )
在窗口部件中擦除指定区域(x, y, w, h)，并不产生绘制事件。
如果w为负数，它被width()-x替换。如果h为负数，它被height()-y替换。
子窗口部件不被影响。
6．bool updatesEnabled
这个属性保存的是更新是否生效。
如果更新失效，调用update()和repaint()是没有效果的。如果更新失效，来自窗口系统的绘制事件会被正常处理。 setUpdatesEnabled()通常被用于在一小段事件内使更新失效，例如为了避免在大的变化期间发生屏幕闪烁。
实例：
    setUpdatesEnabled( FALSE );
    bigVisualChanges();
    setUpdatesEnabled( TRUE );
    repaint();
通过setUpdatesEnabled()设置属性值并且通过isUpdatesEnabled()来获得属性值。

