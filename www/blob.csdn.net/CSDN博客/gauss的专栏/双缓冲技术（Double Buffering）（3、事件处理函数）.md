# 双缓冲技术（Double Buffering）（3、事件处理函数） - gauss的专栏 - CSDN博客
2013年01月06日 23:34:12[gauss](https://me.csdn.net/mathlmx)阅读数：327
 以下是Plotter控件的事件处理函数部分
void Plotter::paintEvent(QPaintEvent * /* event */)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
    if (rubberBandIsShown) {
        painter.setPen(palette().light().color());
        painter.drawRect(rubberBandRect.normalized()
                                       .adjusted(0, 0, -1, -1));
    }
    if (hasFocus()) {
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = palette().dark().color();
        painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
    }
}
通常情况下，paintEvent()是我们处理控件的所有绘制的地方。这里Plotter控件的绘制是在refreshPixmap()中完成的，因此在paintEvent()函数中只是把图片显示在控件的（0，0）位置上。
如果能看到橡皮线，我们把它画到控件的上面。使用控件当前颜色组的“轻”的颜色橡皮线的颜色，和“黑”的背景形成对比。需要注意的是这个线是直接画在控件上的，对图片没有任何影响。使用QRect::normalized()确保橡皮线的矩形有着正数的宽和高，adjusted()减掉一个象素宽的矩形，显示它的轮廓。
如果Plotter有焦点，用控件样式的drawPrimitive()绘制一个焦点矩形，第一个参数为QStyle::PE_FrameFocusRect，第二个参数为一个QStyleOptionFocusRect对象。焦点矩形的绘制选项用initFrom()函数设置，继承自Plotter，但是背景颜色必须明确设置。
如果我们想使用当前的样式，我们可以直接调用QStyle的函数，比如：
style()->drawPrimitive(QStyle::PE_FrameFocusRect, &option, &painter, this);
或者我们使用QStylePainter，能绘制更加方便。
QWidget::Style()函数返回绘制控件使用的样式。在Qt中，一个控件的样式是QStyle的基类。Qt提供的样式有QWindowStyle，QWindowXpStyle，QMotifStyle，QCDEStyle，QMacStyle和QPlastiqueStyle。这些样式类都是重新实现了QStyle的虚函数来模拟特定平台的样式。QStylePainter：：drawPrimitive()函数调用QStyle的同名函数，绘制控件的一些基本原色，如面板，按钮，焦点矩形等。在一个应用程序中，所有控件的样式都是一样的，可用通过QApplication::style()得到，也可以用QWidget::setStyle()设置某一个控件的样式。
把QStyle作为基类，可以定义一个用户样式。可以让一个应用程序看起来与众不同。通常的建议是使用和目标平台一致的样式。只要你有想法，Qt提供了很多的灵活性。
Qt提供的控件都是用QStyle绘制自己，所以在所有Qt支持的平台上，它们看起来都和平台的风格一致。
用户空间可以使用QStyle绘制自己或者使用Qt提供的控件作为子控件。对于Plotter，我们使用两个方式的组合，焦点矩形用QStyle样式绘制，zoomIn和zoomOut按钮为Qt提供的控件。
void Plotter::resizeEvent(QResizeEvent * /* event */)
{
    int x = width() - (zoomInButton->width()+ zoomOutButton->width() + 10);
    zoomInButton->move(x, 5);
    zoomOutButton->move(x + zoomInButton->width() + 5, 5);
    refreshPixmap();
}
控件大小改变时，Qt都会产生一个“resize”事件。这里，我们重写了resizeEvent()把zoomIn和zoomOut按钮放在Plotter控件的右上角。
我们把zoomIn和zoomOut按钮并排摆放，中间有5个象素的空隙，距离控件的上边距和右边距也为5个象素宽。
如果我们希望按钮放在控件的左上角（坐标点为（0，0））上，直接可以在Plotter构造函数中它们移动到左上角。如果我们想跟踪控件的右上角，它的坐标取决与控件的大小。因此需要重写resizeEvent()设置按钮位置。
在Plotter构造函数中，我们没有确定按钮的位置。但是不要紧，在控件第一次显示之前，Qt就会产生一个resize事件。
如果不重写resizeEvent()手工排列子控件，还可以使用布局管理器，如QGridLayout。使用一个布局会有些复杂，也会消耗更多资源。另一方面，它能够把左右排列的布局安排的更好，对Arabic和Hebrew语言尤其适用。
最后，调用refreshPixmap()绘制新的尺寸下的图片。
void Plotter::mousePressEvent(QMouseEvent *event)
{
    QRect rect(Margin, Margin,
               width() - 2 * Margin, height() - 2 * Margin);
    if (event->button() == Qt::LeftButton) {
        if (rect.contains(event->pos())) {
            rubberBandIsShown = true;
            rubberBandRect.setTopLeft(event->pos());
            rubberBandRect.setBottomRight(event->pos());
            updateRubberBandRegion();
            setCursor(Qt::CrossCursor);
        }
    }
}
当用户点击了鼠标左键，在控件上显示出一个橡皮线，显示的条件是rubberBandIsShown为true。把变量rubberBandRect的左上角和右下角都为当前的鼠标点，然后发出一个绘制事件绘制橡皮线，同时把光标改为十字型。
变量rubberBandRect为QRect类型。一个QRect可以由四个量（x,y,width,height）定义。其中（x,y）为矩形左上角的坐标，width*height为矩形的面积。或者由左上角和右下角的坐标对定义。在这里使用了坐标对定义的方法，把矩形的左上角和右下角的坐标都设置为鼠标点击的位置。然后调用updateRubberBandRegion()把橡皮线内的区域绘制出来。
Qt有两种设置光标形状的方法：
QWidget::setCursor()，当鼠标移动到一个控件上时，使用这个函数设置光标的形状。如果子控件上没有设置光标形状，则使用父控件的光标。通常使用的光标是一个箭头式光标。
QApplication::setOverrideCursor()设置应用程序的光标形状，取代控件中设定的光标，调用restoreOverrideCursor()后光标回到原来的状态。
在第四章中，我们调用了QApplication::setOverrideCursor()把光标设置为Qt::WaitCursor，把应用程序光标设置为等待式光标。
void Plotter::mouseMoveEvent(QMouseEvent *event)
{
    if (rubberBandIsShown) {
        updateRubberBandRegion();
        rubberBandRect.setBottomRight(event->pos());
        updateRubberBandRegion();
    }
}
当用户点中鼠标左键移动鼠标时，调用updateRubberBandRegion()重新绘制橡皮线所在区域。然后根据鼠标移动的位置重新计算橡皮线区域的大小，最后在调用updateRubberBandRegion()绘制新的橡皮线区域。这样就可以删除原来的橡皮线，在新的位置绘制新的橡皮线。
如果用户向上或者向下移动鼠标，rubberBandRect的右下角可能会到达它的左上角的上面或者左面，QRect的width和height会出现负值，在paintEvent()函数中调用了QRect::normalized()函数，它可以重新计算矩形的左上角和右下角的坐标值，保证得到一个非负的宽和高。
void Plotter::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) && rubberBandIsShown) {
        rubberBandIsShown = false;
        updateRubberBandRegion();
        unsetCursor();
        QRect rect = rubberBandRect.normalized();
        if (rect.width() < 4 || rect.height() < 4)
            return;
        rect.translate(-Margin, -Margin);
        PlotSettings prevSettings = zoomStack[curZoom];
        PlotSettings settings;
        double dx = prevSettings.spanX() / (width() - 2 * Margin);
        double dy = prevSettings.spanY() / (height() - 2 * Margin);
        settings.minX = prevSettings.minX + dx * rect.left();
        settings.maxX = prevSettings.minX + dx * rect.right();
        settings.minY = prevSettings.maxY - dy * rect.bottom();
        settings.maxY = prevSettings.maxY - dy * rect.top();
        settings.adjust();
        zoomStack.resize(curZoom + 1);
        zoomStack.append(settings);
        zoomIn();
    }
}
用户释放鼠标左键时，我们删除橡皮线，恢复到正常的箭头式光标。如果橡皮线区域大于4*4，则把这个区域放大。如果小于这个值，则很可能是用户的一个误操作，也许只是想给控件一个焦点罢了，程序返回，什么都不做了。
进行放大的这部分代码有点复杂，因为我们需要同时处理控件坐标和plotter的坐标。大部分代码都是把rubberBandRect从控件坐标转到plotter坐标。完成转换以后，调用PlotSettings::adjust()进行四舍五入，找到一个合理的坐标刻度。图5-10和图5-11示意了这个坐标的转换：
Figure 5.10. Converting the rubber band from widget to plotter coordinates
![](https://p-blog.csdn.net/images/p_blog_csdn_net/iamdbl/287708/o_5-10.JPG)
Figure 5.11. Adjusting plotter coordinates and zooming in on the rubber band
![](https://p-blog.csdn.net/images/p_blog_csdn_net/iamdbl/287708/o_5-11.JPG)
坐标转换以后，我们进行放大。同时把放大系数等设置形成一个新的PlotSettings对象，然后把它放到zoomStack的最上面。
void Plotter::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Left:
        zoomStack[curZoom].scroll(-1, 0);
        refreshPixmap();
      break;
    case Qt::Key_Right:
        zoomStack[curZoom].scroll(+1, 0);
        refreshPixmap();
        break;
    case Qt::Key_Down:
        zoomStack[curZoom].scroll(0, -1);
        refreshPixmap();
        break;
    case Qt::Key_Up:
        zoomStack[curZoom].scroll(0, +1);
        refreshPixmap();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}
当当前的焦点在Plotter控件上时，用户敲击了键盘的某一个键值，keyPressEvent()就会调用。这里我们重写了这个函数，相应用户对6个键的相应：+，-，Up，Down，Left和Right。如果用户敲击的键不在这六个之中，则调用基类的函数进行处理。为了简便，我们这里忽略了Shift，Ctrl，和Alt键，这些键可以通过QKeyEvent::modifiers()得到。
void Plotter::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numTicks = numDegrees / 15;
    if (event->orientation() == Qt::Horizontal) {
        zoomStack[curZoom].scroll(numTicks, 0);
    } else {
        zoomStack[curZoom].scroll(0, numTicks);
    }
    refreshPixmap();
}
鼠标滚轮转动时，Qt产生一个滚轮事件（Wheel event）。很多鼠标只有一个垂直的滚轮，但是考虑到一些鼠标也有水平滚轮，Qt对这两种方式的滚轮都支持。滚轮事件只是发生在有焦点的控件上。函数delta()返回的是滚轮滚动了8°时移动的距离。一般鼠标都是以15°事件发生后，我们修改zoomStack最上面的设置，然后刷新图片。
滚轮鼠标一般用来处理滚动条。如果我们使用了QScrollArea提供一个可以滚动的区域，QScrollBar自动处理滚轮事件，我们不用自己重写wheelEvent()函数。
