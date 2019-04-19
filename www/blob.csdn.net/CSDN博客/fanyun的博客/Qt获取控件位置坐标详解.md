# Qt获取控件位置坐标详解 - fanyun的博客 - CSDN博客
2017年10月27日 22:15:13[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3937
1.     QPoint QMouseEvent::pos()
     这个只是返回相对这个widget(重载了QMouseEvent的widget)的位置。
      const Returns the position of the mouse cursor, relative to the widgetthat received the event. If you move the widget as a result of the mouse event,use the global position returned by globalPos() to avoid a shaking motion.
2.     QPoint QMouseEvent::globalPos()
     窗口坐标，这个是返回鼠标的全局坐标
    const Returns the global position of the mouse cursor at the time of theevent. This is important on asynchronous window systems like X11. Whenever youmove your widgets around in response to mouse events, globalPos() may differ alot from the current pointer
 position QCursor::pos(), and fromQWidget::mapToGlobal(pos()).
3.     QPoint QCursor::pos() [static]
     返回相对显示器的全局坐标
     Returns the position of the cursor (hot spot) of the primary screen inglobal screen coordinates. You can call QWidget::mapFromGlobal() to translateit to widget coordinates. Note: The position is queried from the windowingsystem. If mouse events are
 generated via other means (e.g., viaQWindowSystemInterface in a unit test), those fake mouse moves will not bereflected in the returned value. Note: On platforms where there is no windowingsystem or cursors are not available, the returned position is based
 on the mousemove events generated via QWindowSystemInterface.
4.     QPoint QWidget::mapToGlobal(const QPoint & pos)  const
      将窗口坐标转换成显示器坐标
      Translates the widget coordinate pos to global screen coordinates. Forexample, mapToGlobal(QPoint(0,0)) would give the global coordinates of thetop-left pixel of the widget. See also mapFromGlobal(), mapTo(), andmapToParent().
5.     QPoint QWidget::mapFromGlobal(const QPoint & pos) const
      将显示器坐标转换成窗口坐标
      Translates the global screen coordinate pos to widget coordinates.
6.     QPoint QWidget::mapToParent(const QPoint & pos) const
      将窗口坐标获得的pos转换成父类widget的坐标
Translates the widget coordinate pos to acoordinate in the parent widget.
7.     QPoint QWidget::mapFromParent(const QPoint & pos) const
       将父类窗口坐标转换成当前窗口坐标
Translates the parent widget coordinate posto widget coordinates. Same as mapFromGlobal() if the widget has no parent.
8.     QPoint QWidget::mapTo(const QWidget * parent, const QPoint &pos) const
将当前窗口坐标转换成指定parent坐标。
Translates the widget coordinate pos to thecoordinate system of parent. The parent must not be 0 and must be a parent ofthe calling widget. See also mapFrom(), mapToParent(), mapToGlobal(), andunderMouse().
9.     QWidget::pos() : QPoint
这个属性获得的是当前目前控件在父窗口中的位置，
This property holds the position of thewidget within its parent widget.
If the widget is a window, the position isthat of the widget on the desktop, including its frame.
When changing the position, the widget, ifvisible, receives a move event (moveEvent()) immediately. If the widget is notcurrently visible, it is guaranteed to receive an event before it is shown.
By default, this property contains aposition that refers to the origin.
Warning: Calling move() or setGeometry()inside moveEvent() can lead to infinite recursion.
See the Window Geometry documentation foran overview of geometry issues with windows.
10.    const QPointF &QMouseEvent::screenPos() const
Returns the position of the mouse cursor asa QPointF, relative to the screen that received the event.
和QPoint QMouseEvent::globalPos() 值相同，但是类型更高精度的QPointF
This function was introduced in Qt 5.0.
11.   QCursor::pos() == QMouseEvent::globalPos() 都是全局坐标
12.   将鼠标的坐标转换成全局坐标。
QMouseEvent::globalPos() == ui.posBtn->mapToGlobal(ui.posBtn->pos());
13.   将全局坐标（鼠标当前坐标，QCursor::pos()）直接转换成当前窗口相对坐标
ui.posBtn->mapFromGlobal(QCursor::pos());
