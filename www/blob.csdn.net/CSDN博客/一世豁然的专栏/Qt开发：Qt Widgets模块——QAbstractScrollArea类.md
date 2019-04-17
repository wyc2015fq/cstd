# Qt开发：Qt Widgets模块——QAbstractScrollArea类 - 一世豁然的专栏 - CSDN博客





2018年05月17日 11:22:30[一世豁然](https://me.csdn.net/Explorer_day)阅读数：965








QAbstractScrollArea小部件提供了一个带按需滚动条的滚动区域。



头文件：#include <QAbstractScrollArea> 

qmake：QT += widgets

继承：QFrame

被继承：QAbstractItemView，QGraphicsView，QMdiArea，QPlainTextEdit，QScrollArea和QTextEdit

QAbstractScrollArea是一个滚动区域的低级抽象。 该区域提供称为视口的中央小部件，其中该区域的内容将被滚动（即，内容的可见部分在视口中呈现）。


在视口旁边是一个垂直滚动条，下面是一个水平滚动条。 当所有区域内容适合视口时，根据滚动条的Qt :: ScrollBarPolicy，每个滚动条可以是可见的或隐藏的。 当隐藏滚动条时，视口会展开以覆盖所有可用空间。 当滚动条再次可见时，视口会缩小以便为滚动条腾出空间。


可以在视口周围预留一个边缘区域，请参阅setViewportMargins（）。 该功能主要用于在滚动区域上方或旁边放置QHeaderView小部件。 QAbstractScrollArea的子类应实现边距。


当继承QAbstractScrollArea时，您需要执行以下操作：


1)、通过设置它们的范围，值，页面步长和跟踪它们的移动来控制滚动条。


2）、根据滚动条的值在视口中绘制区域的内容。


3）、处理视口在viewportEvent（）中收到的事件 - 特别是调整事件大小。


4）、使用viewport-> update（）更新视口的内容而不是update（），因为所有绘画操作都在视口上进行。


使用Qt :: ScrollBarAsNeeded（默认值）的滚动条策略时，QAbstractScrollArea在滚动条提供非零滚动范围时显示滚动条，否则将其隐藏。

只要视口收到调整大小事件或内容大小发生更改，就应该更新滚动条和视口。 当滚动条值发生更改时，也需要更新视口。 滚动条的初始值通常在区域收到新内容时设置。


我们给出一个简单的例子，我们已经实现了一个可以滚动任何QWidget的滚动区域。 我们使小部件成为视口的一个子元素; 这样，我们不必计算要绘制哪个部件，而只需使用QWidget :: move（）即可移动该部件。 当区域内容或视口大小发生变化时，我们执行以下操作：


```cpp
QSize areaSize = viewport()->size();
      QSize  widgetSize = widget->size();

      verticalScrollBar()->setPageStep(areaSize.height());
      horizontalScrollBar()->setPageStep(areaSize.width());
      verticalScrollBar()->setRange(0, widgetSize.height() - areaSize.height());
      horizontalScrollBar()->setRange(0, widgetSize.width() - areaSize.width());
      updateWidgetPosition();
```

当滚动条更改值时，我们需要更新小部件位置，即查找要在视口中绘制的小部件的部分：

```cpp
int hvalue = horizontalScrollBar()->value();
 int vvalue = verticalScrollBar()->value();
 QPoint topLeft = viewport()->rect().topLeft();

 widget->move(topLeft.x() - hvalue, topLeft.y() - vvalue);
```

为了跟踪滚动条移动，重新实现虚函数scrollContentsBy（）。 为了微调滚动行为，连接到滚动条的QAbstractSlider :: actionTriggered（）信号并根据需要调整QAbstractSlider :: sliderPosition。

为方便起见，QAbstractScrollArea使所有视口事件在虚拟viewportEvent（）处理程序中可用。 在有意义的情况下，QWidget的专用处理程序会重新映射到视口事件。 重新映射的专用处理程序是：paintEvent（），mousePressEvent（），mouseReleaseEvent（），mouseDoubleClickEvent（），mouseMoveEvent（），wheelEvent（），dragEnterEvent（），dragMoveEvent（），dragLeaveEvent（），dropEvent（），contextMenuEvent（）， 和resizeEvent（）。


QScrollArea继承QAbstractScrollArea，为任何QWidget提供平滑的滚动（即，小部件是逐像素滚动的）。 如果您需要更专门的行为，则只需要派生子类QAbstractScrollArea。 例如，如果该区域的全部内容不适合在QWidget上绘制或者您不想平滑滚动，则为true。








一、成员类型

1、enum QAbstractScrollArea::SizeAdjustPolicy

此枚举指定当视口大小更改时QAbstractScrollArea的大小提示应如何调整。



|内容|值|描述|
|----|----|----|
|QAbstractScrollArea::AdjustIgnored|0|滚动区域将像以前一样运行 - 并且不做任何调整。|
|QAbstractScrollArea::AdjustToContents|2|滚动区域将始终调整到视口|
|QAbstractScrollArea::AdjustToContentsOnFirstShow|1|第一次显示时，滚动区域将调整到其视口。|
这个枚举是在Qt 5.2中引入或修改的。






二、属性

1、horizontalScrollBarPolicy : Qt::ScrollBarPolicy

该属性包含水平滚动条的策略。

默认策略是Qt :: ScrollBarAsNeeded。


访问函数；

```cpp
Qt::ScrollBarPolicy horizontalScrollBarPolicy() const
void setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy)
```




2、sizeAdjustPolicy : SizeAdjustPolicy



该属性包含描述当视口大小发生变化时滚动区域的大小如何变化的策略。

默认策略是QAbstractScrollArea :: AdjustIgnored。 改变这个属性可能实际上调整了Scrollarea的大小。

这个属性是在Qt 5.2中引入的。

访问函数；

```cpp
SizeAdjustPolicy sizeAdjustPolicy() const
void setSizeAdjustPolicy(SizeAdjustPolicy policy)
```



3、verticalScrollBarPolicy : Qt::ScrollBarPolicy

该属性包含垂直滚动条的策略。

默认策略是Qt :: ScrollBarAsNeeded。


访问函数；

```cpp
Qt::ScrollBarPolicy verticalScrollBarPolicy() const
void setVerticalScrollBarPolicy(Qt::ScrollBarPolicy)
```






三、成员函数

1、QAbstractScrollArea::QAbstractScrollArea(QWidget *parent = Q_NULLPTR)

构造一个视口。

父参数被发送到QWidget构造函数。





2、QAbstractScrollArea::~QAbstractScrollArea()

销毁一个视口。




3、void QAbstractScrollArea::addScrollBarWidget(QWidget *widget, Qt::Alignment alignment)



在通过对齐指定的位置添加小部件作为滚动条小部件。

滚动条小部件显示在水平或垂直滚动​​条旁边，并可放置在其任一侧。如果您希望滚动条小部件始终可见，请将相应滚动条的scrollBarPolicy设置为AlwaysOn。

对齐方式必须是映射到水平滚动条的Qt :: Alignleft和Qt :: AlignRight，或者映射到垂直滚动条的Qt :: AlignTop和Qt :: AlignBottom之一。

滚动条小部件可以通过重新加载小部件或删除它来删除。也可以用QWidget :: hide（）隐藏一个小部件

滚动条窗口小部件将调整大小以适合当前样式的滚动条几何图形。以下介绍水平滚动条上的滚动条小部件的情况：

小部件的高度将被设置为与滚动条的高度相匹配。要控制窗口部件的宽度，请使用QWidget :: setMinimumWidth和QWidget :: setMaximumWidth，或实现QWidget :: sizeHint（）并设置水平尺寸策略。如果你想要一个方形的小部件，调用QStyle :: pixelMetric（QStyle :: PM_ScrollBarExtent）并将宽度设置为这个值。

这个函数是在Qt 4.2中引入的。




4、[virtual protected] void QAbstractScrollArea::contextMenuEvent(QContextMenuEvent *e)

从QWidget :: contextMenuEvent（）重新实现。

该事件处理程序可以在子类中重新实现，以接收viewport（）小部件的上下文菜单事件。 该事件通过e。





5、QWidget *QAbstractScrollArea::cornerWidget() const



返回两个滚动条之间角落中的小部件。

默认情况下，不存在角落控件。
这个函数是在Qt 4.2中引入的。




6、[virtual protected] void QAbstractScrollArea::dragEnterEvent(QDragEnterEvent *event)

从QWidget :: dragEnterEvent（）重新实现。

这个事件处理程序可以在子类中重新实现，以接收拖动输入事件（传入事件），用于viewport（）小部件。





7、[virtual protected] void QAbstractScrollArea::dragLeaveEvent(QDragLeaveEvent *event)

从QWidget :: dragLeaveEvent（）重新实现。

这个事件处理程序可以在子类中重新实现，以接收拖拽事件（传入事件），用于viewport（）小部件。





8、[virtual protected] void QAbstractScrollArea::dragMoveEvent(QDragMoveEvent *event)

从QWidget :: dragMoveEvent（）重新实现。

此事件处理函数可以在子类中重新实现，以接收拖动移动事件（传入事件），用于viewport（）小部件。





9、[virtual protected] void QAbstractScrollArea::dropEvent(QDropEvent *event)

从QWidget :: dropEvent（）重新实现。

这个事件处理程序可以在子类中重新实现，以接收放置事件（传入事件），用于viewport（）小部件。





10、[virtual protected] bool QAbstractScrollArea::event(QEvent *event)

从QObject :: event（）重新实现。

这是QAbstractScrollArea小部件的主要事件处理程序（不是滚动区域视口（））。 指定的事件是一般事件对象，可能需要根据其类型将其转换为适当的类。





11、QScrollBar *QAbstractScrollArea::horizontalScrollBar() const

返回水平滚动条。





12、[virtual protected] void QAbstractScrollArea::keyPressEvent(QKeyEvent *e)

从QWidget :: keyPressEvent（）重新实现。

当按键发生时，使用键事件e调用此功能。 它处理PageUp，PageDown，Up，Down，Left和Right，并忽略所有其他按键。





13、QSize QAbstractScrollArea::maximumViewportSize() const

返回视口的大小，就好像滚动条没有有效的滚动范围一样。





14、[virtual] QSize QAbstractScrollArea::minimumSizeHint() const

从QWidget :: minimumSizeHint（）重新实现。





15、[virtual protected] void QAbstractScrollArea::mouseDoubleClickEvent(QMouseEvent *e)

从QWidget :: mouseDoubleClickEvent（）重新实现。

该事件处理程序可以在子类中重新实现，以接收用于viewport（）小部件的鼠标双击事件。 该事件通过e。





16、[virtual protected] void QAbstractScrollArea::mouseMoveEvent(QMouseEvent *e)

从QWidget :: mouseMoveEvent（）重新实现。

该事件处理程序可以在子类中重新实现，以接收viewport（）小部件的鼠标移动事件。 该事件通过e。





17、[virtual protected] void QAbstractScrollArea::mousePressEvent(QMouseEvent *e)

从QWidget :: mousePressEvent（）重新实现。

该事件处理程序可以在子类中重新实现，以接收用于viewport（）小部件的鼠标按下事件。 该事件通过e。





18、[virtual protected] void QAbstractScrollArea::mouseReleaseEvent(QMouseEvent *e)

从QWidget :: mouseReleaseEvent（）重新实现。

该事件处理程序可以在子类中重新实现，以接收viewport（）小部件的鼠标释放事件。 该事件通过e。





19、[virtual protected] void QAbstractScrollArea::paintEvent(QPaintEvent *event)



从QWidget :: paintEvent（）重新实现。

该事件处理程序可以在子类中重新实现，以接收绘制事件（传入事件），用于viewport（）小部件。

注意：如果您打开画家，请确保在视口（）上打开画家。




20、[virtual protected] void QAbstractScrollArea::resizeEvent(QResizeEvent *event)



从QWidget :: resizeEvent（）重新实现。

这个事件处理程序可以在子类中重新实现，以接收调整大小事件（传入事件），用于viewport（）小部件。

当调用resizeEvent（）时，视口已经有了新的几何体：它的新大小可以通过QResizeEvent :: size（）函数访问，旧大小通过QResizeEvent :: oldSize（）访问。




21、QWidgetList QAbstractScrollArea::scrollBarWidgets(Qt::Alignment alignment)

返回当前设置的滚动条小部件的列表。 对齐可以是四个位置标志的任意组合。

这个函数是在Qt 4.2中引入的。





22、[virtual protected] void QAbstractScrollArea::scrollContentsBy(int dx, int dy)



当通过dx，dy移动滚动条时，会调用此虚拟处理程序，因此应该相应地滚动视口的内容。

默认实现只是在整个视口（）上调用update（），子类可以重新实现此处理程序以进行优化，或者像QScrollArea一样移动内容小部件。 为了方便，参数dx和dy在那里，以便该类知道应该滚动多少（例如，在进行像素移位时有用）。 您也可以忽略这些值并直接滚动到滚动条指示的位置。

调用此函数以编程方式滚动是一个错误，请改用滚动条（例如，直接调用QScrollBar :: setValue（））。




23、void QAbstractScrollArea :: setCornerWidget（QWidget * widget）



将两个滚动条之间角落中的小部件设置为小部件。

您可能还需要将至少一个滚动条模式设置为AlwaysOn。

通过0显示角落里没有小工具。

任何以前的角落小部件都隐藏起

您可以在不同的时间使用相同的小部件调用setCornerWidget（）。

除非您在设置其他角落控件（或0）之后单独重新设置控件，否则在此处设置的所有控件都将被滚动区域删除。

任何新设置的小部件都应该没有当前父项。

默认情况下，不存在角落控件。

这个函数是在Qt 4.2中引入的。




24、void QAbstractScrollArea::setHorizontalScrollBar(QScrollBar *scrollBar)



用滚动条替换现有的水平滚动条，并在新滚动条上设置所有前滚动条的滑动条属性。 之前的滚动条被删除。

QAbstractScrollArea默认已经提供了水平和垂直滚动条。 您可以调用此函数以用您自己的自定义滚动条替换默认的水平滚动条。

这个函数是在Qt 4.2中引入的。




25、void QAbstractScrollArea::setVerticalScrollBar(QScrollBar *scrollBar)



用滚动条替换现有的垂直滚动条，并在新滚动条上设置所有前滚动条的滑动条属性。 之前的滚动条被删除。

QAbstractScrollArea默认已经提供了垂直和水平滚动条。 您可以调用此函数以用您自己的自定义滚动条替换默认的垂直滚动条。

这个函数是在Qt 4.2中引入的。




26、void QAbstractScrollArea::setViewport(QWidget *widget)



将视口设置为给定的小部件。 QAbstractScrollArea将获得给定小部件的所有权。

如果Widget为0，QAbstractScrollArea将为视口分配一个新的QWidget实例。

这个函数是在Qt 4.2中引入的。




27、[protected] void QAbstractScrollArea::setViewportMargins(int left, int top, int right, int bottom)



将滚动区域周围的边距设置为左侧，顶部，右侧和底部。 这对于具有“锁定”行和列的电子表格等应用程序非常有用。 边缘空间是空白的; 将小部件放在未使用的区域。

请注意，此函数通常由QTreeView和QTableView调用，所以边距必须由QAbstractScrollArea子类实现。 另外，如果要在项目视图中使用子类，则不应调用此函数。

默认情况下，所有边距都为零。




28、[protected] void QAbstractScrollArea::setViewportMargins(const QMargins &margins)



设置滚动区域周围的边距。 这对于具有“锁定”行和列的电子表格等应用程序非常有用。 边缘空间是空白的; 将小部件放在未使用的区域。

默认情况下，所有边距都为零。

这个函数是在Qt 4.6中引入的。




29、[virtual] void QAbstractScrollArea::setupViewport(QWidget *viewport)

在setViewport（视口）被调用之后，该槽由QAbstractScrollArea调用。 在QAbstractScrollArea的子类中重新实现此函数以在使用之前初始化新视口。





30、[virtual] QSize QAbstractScrollArea::sizeHint() const

从QWidget :: sizeHint（）重新实现。

返回滚动区域的sizeHint属性。 如果需要，大小由使用viewportSizeHint（）加上一些额外的滚动条空间决定。





31、QScrollBar *QAbstractScrollArea::verticalScrollBar() const

返回垂直滚动条。





32、QWidget *QAbstractScrollArea::viewport() const

返回视口小部件。

使用QScrollArea :: widget（）函数来检索视口小部件的内容。





33、[virtual protected] bool QAbstractScrollArea::viewportEvent(QEvent *event)



滚动区域的主要事件处理程序（viewport（）小部件）。 它处理指定的事件，并且可以由子类调用以提供合理的默认行为。

返回true以向事件系统表明事件已处理，不需要进一步处理; 否则返回false以指示该事件应该进一步传播。
您可以在子类中重新实现此函数，但我们建议使用其中一个专用事件处理程序。

viewEvent（），mouseEvent（），mouseReleaseEvent（），mouseDoubleClickEvent（），mouseMoveEvent（），wheelEvent（），dragEnterEvent（），dragMoveEvent（），dragLeaveEvent（），dropEvent（），contextMenuEvent（） ，和resizeEvent（）。




34、[protected] QMargins QAbstractScrollArea::viewportMargins() const

返回滚动区域周围的边距。 默认情况下，所有边距都为零。

这个函数是在Qt 5.5中引入的。





35、[virtual protected] QSize QAbstractScrollArea::viewportSizeHint() const

返回视口的建议大小。 默认实现返回viewport（） - > sizeHint（）。 请注意，大小只是视口的大小，没有任何滚动条可见

这个函数是在Qt 5.2中引入的。





36、[virtual protected] void QAbstractScrollArea::wheelEvent(QWheelEvent *e)

从QWidget :: wheelEvent（）重新实现。

该事件处理程序可以在子类中重新实现，以接收viewport（）小部件的轮子事件。 该事件通过e。




