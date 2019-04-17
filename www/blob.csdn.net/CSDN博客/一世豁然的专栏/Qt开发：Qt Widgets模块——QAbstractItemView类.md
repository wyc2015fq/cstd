# Qt开发：Qt Widgets模块——QAbstractItemView类 - 一世豁然的专栏 - CSDN博客





2018年05月17日 08:00:50[一世豁然](https://me.csdn.net/Explorer_day)阅读数：959








QAbstractItemView类提供了项目视图类的基本功能。



头文件：#include <QAbstractItemView> 

qmake：QT += widgets

继承：QAbstractScrollArea

被继承：QColumnView, QHeaderView, QListView, QTableView, 和QTreeView

QAbstractItemView类是每个使用QAbstractItemModel的标准视图的基类。 QAbstractItemView是一个抽象类，本身不能被实例化。 它提供了一个通过信号和插槽机制与模型进行互操作的标准接口，使得子类能够随着模型的变化而保持最新。 该类为键盘和鼠标导航，视口滚动，项目编辑和选择提供标准支持。 键盘导航实现了这一功能：


|键|功能|
|----|----|
|方向键|更改当前项目并选择它。|
|Ctrl +箭头键|更改当前项目，但不选择它。|
|Shift+箭头键|更改当前项目并选择它。 先前选择的项目不被取消选择。|
|Ctrl+空格键|切换当前项目的选择。|
|Tab/Backtab|将当前项目更改为下一个/上一个项目。|
|Home/End|选择模型中的第一个/最后一个项目。|
|Page Up/page Down|向上/向下滚动显示视图中可见行数的行。|
|Ctrl+A|选择模型中的所有项目。|
请注意，上表假设选择模式允许操作。 例如，如果选择模式为QAbstractItemView :: NoSelection，则不能选择项目。
QAbstractItemView类是模型/视图类之一，是Qt的模型/视图框架的一部分。


继承QAbstractItemView的视图类只需要实现自己的特定视图功能，例如绘制项目，返回项目的几何图形，查找项目等。


QAbstractItemView提供了常用插槽，如edit（）和setCurrentIndex（）。 还提供了许多受保护的插槽，包括dataChanged（），rowsInserted（），rowsAboutToBeRemoved（），selectionChanged（）和currentChanged（）。


根项目由rootIndex（）返回，当前项目由currentIndex（）返回。 要确保项目可见，请使用scrollTo（）。


QAbstractItemView的一些函数关心滚动，例如setHorizontalScrollMode（）和setVerticalScrollMode（）。 例如，要设置滚动条的范围，可以重新实现视图的resizeEvent（）函数：


```cpp
void MyView::resizeEvent(QResizeEvent *event) {
      horizontalScrollBar()->setRange(0, realWidth - width());
      ...
  }
```

请注意，在显示小部件之前，范围不会更新。

其他几个功能都与选择控制有关; 例如setSelectionMode（）和setSelectionBehavior（）。 这个类提供了一个默认的选择模型来处理（selectionModel（）），但是这可以通过使用setSelectionModel（）和QItemSelectionModel的一个实例来替换。


为了完全控制项目的显示和编辑，你可以用setItemDelegate（）指定一个委托。


QAbstractItemView提供了很多受保护的功能。 有些关注于编辑，例如edit（）和commitData（），而另一些则是键盘和鼠标事件处理程序。


注意：如果您继承QAbstractItemView并打算更新视口的内容，则应使用viewport-> update（）而不是update（），因为所有绘画操作都是在视口上进行的。








一、成员类型

1、enum QAbstractItemView::CursorAction


此枚举描述了在项目之间导航的不同方式，



|内容|值|描述|
|----|----|----|
|QAbstractItemView::MoveUp|0|移至当前项目上方的项目。|
|QAbstractItemView::MoveDown|1|移至当前项目下方的项目。|
|QAbstractItemView::MoveLeft|2|移至当前项目的左侧项目。|
|QAbstractItemView::MoveRight|3|移至当前项目的项目右侧。|
|QAbstractItemView::MoveHome|4|移至左上角的项目。|
|QAbstractItemView::MoveEnd|5|移到右下角的项目。|
|QAbstractItemView::MovePageUp|6|在当前项目上方移动一页。|
|QAbstractItemView::MovePageDown|7|在当前项目下移动一页。|
|QAbstractItemView::MoveNext|8|移至当前项目之后的项目。|
|QAbstractItemView::MovePrevious|9|移至当前项目之前的项目。|


2、enum QAbstractItemView::DragDropMode

描述视图可以采取的各种拖放事件。 默认情况下，视图不支持拖放（NoDragDrop）。



|内容|值|描述|
|----|----|----|
|QAbstractItemView::NoDragDrop|0|不支持拖放。|
|QAbstractItemView::DragOnly|1|该视图支持拖拽自己的物品|
|QAbstractItemView::DropOnly|2|该视图接受丢弃|
|QAbstractItemView::DragDrop|3|该视图支持拖放|
|QAbstractItemView::InternalMove|4|该视图只接受来自其自身的移动（不是复制）操作。|
请注意，使用的模型需要为拖放操作提供支持
这个枚举是在Qt 4.2中引入或修改的。




3、enum QAbstractItemView::DropIndicatorPosition

此枚举指示放置指示符相对于当前鼠标位置处的索引的位置：



|内容|值|描述|
|----|----|----|
|QAbstractItemView::OnItem|0|该项目将被放置在索引上。|
|QAbstractItemView::AboveItem|1|该项目将被放置在索引上方。|
|QAbstractItemView::BelowItem|2|该项目将被放置在索引下方。|
|QAbstractItemView::OnViewport|3|该项目将被丢弃到视口的一个区域中，而没有任何项目。 每个视图处理放置在视口上的项目的方式取决于所使用的基础模型的行为。|




4、enum QAbstractItemView::EditTrigger
flags QAbstractItemView::EditTriggers

此枚举描述将启动项目编辑的操作。



|内容|值|描述|
|----|----|----|
|QAbstractItemView::NoEditTriggers|0|无法编辑。|
|QAbstractItemView::CurrentChanged|1|只要当前项目发生变化，即开始编辑|
|QAbstractItemView::DoubleClicked|2|当一个项目被双击时开始编辑。|
|QAbstractItemView::SelectedClicked|4|点击已选项目后开始编辑。|
|QAbstractItemView::EditKeyPressed|8|平台编辑键被按下时，编辑开始。|
|QAbstractItemView::AnyKeyPressed|16|当任何键被按下时，编辑开始。|
|QAbstractItemView::AllEditTriggers|31|所有上述操作都将开始编辑。|
EditTriggers类型是QFlags <EditTrigger>的typedef。 它存储EditTrigger值的或组合。



5、enum QAbstractItemView::ScrollHint


|内容|值|描述|
|----|----|----|
|QAbstractItemView::EnsureVisible|0|滚动以确保该项目可见。|
|QAbstractItemView::PositionAtTop|1|滚动以将项目放置在视口顶部。|
|QAbstractItemView::PositionAtBottom|2|滚动以将项目放置在视口的底部。|
|QAbstractItemView::PositionAtCenter|3|滚动以将项目定位在视口的中心。|

6、enum QAbstractItemView::ScrollMode

介绍滚动条的行为方式。 将滚动模式设置为ScrollPerPixel时，除非使用setSingleStep（）明确设置，否则单步大小将自动调整。 通过将单步大小设置为-1可以恢复自动调整。



|内容|值|描述|
|----|----|----|
|QAbstractItemView::ScrollPerItem|0|该视图将一次滚动一个项目的内容。|
|QAbstractItemView::ScrollPerPixel|1|该视图将一次滚动一个像素的内容。|
这个枚举是在Qt 4.2中引入或修改的。



7、enum QAbstractItemView::SelectionBehavior


|内容|值|描述|
|----|----|----|
|QAbstractItemView::SelectItems|0|选择单个项目。|
|QAbstractItemView::SelectRows|1|只选择行。|
|QAbstractItemView::SelectColumns|2|只选择列。|

8、enum QAbstractItemView::SelectionMode

此枚举指示视图如何响应用户选择：



|内容|值|描述|
|----|----|----|
|QAbstractItemView::SingleSelection|1|当用户选择一个项目时，任何已经选择的项目都会被取消选择，并且用户不能通过点击选择该项目。|
|QAbstractItemView::ContiguousSelection|4|当用户以通常的方式选择一个项目时，选择被清除并且选择新的项目。 但是，如果用户在点击某个项目时按住Shift键，则根据所点击的项目的状态，当前项目和所点击的项目之间的所有项目被选择或取消选择。|
|QAbstractItemView::ExtendedSelection|3|当用户以通常的方式选择一个项目时，选择被清除并且选择新的项目。 但是，如果用户在点击某个项目时按下Ctrl键，则点击的项目会被切换，而所有其他项目将保持不变。 如果用户在按住Shift键的同时点击某个项目，则当前项目与被点击项目之间的所有项目都将被选中或取消选定，具体取决于所点击项目的状态。 通过拖动鼠标可以选择多个项目。|
|QAbstractItemView::MultiSelection|2|当用户以通常的方式选择一个项目时，该项目的选择状态被切换并且其他项目被单独保留。 通过将鼠标拖到它们上方可以切换多个项目。|
|QAbstractItemView::NoSelection|0|项目不能被选择。|
最常用的模式是SingleSelection和ExtendedSelection。



9、enum QAbstractItemView::State

描述视图可以处于的不同状态。这通常只在重新实现自己的视图时才有意义。



|内容|值|描述|
|----|----|----|
|QAbstractItemView::NoState|0|这是默认状态。|
|QAbstractItemView::DraggingState|1|用户正在拖动项目。|
|QAbstractItemView::DragSelectingState|2|用户正在选择项目。|
|QAbstractItemView::EditingState|3|用户正在编辑小部件编辑器中的项目。|
|QAbstractItemView::ExpandingState|4|用户正在打开一个项目分支。|
|QAbstractItemView::CollapsingState|5|用户正在关闭一个项目分支。|
|QAbstractItemView::AnimatingState|6|项目视图正在执行动画。|




二、属性

1、alternatingRowColors : bool

该属性保持是否使用交替颜色绘制背景。


如果此属性为true，则使用QPalette :: Base和QPalette :: AlternateBase绘制项目背景; 否则背景将使用QPalette :: Base颜色绘制。


默认情况下，此属性为false。


访问函数；

```cpp
bool alternatingRowColors() const
void setAlternatingRowColors(bool enable)
```




2、autoScroll : bool

此属性保持是否启用拖动移动事件中的自动滚动功能。


如果此属性设置为true（默认值），则如果用户在视口边缘的16个像素内拖动，QAbstractItemView将自动滚动视图的内容。 如果当前项目更改，则视图将自动滚动以确保当前项目完全可见。


该属性仅适用于视口接受丢弃。 通过将此属性设置为false来关闭自动滚动。


访问函数；

```cpp
bool hasAutoScroll() const
void setAutoScroll(bool enable)
```




3、autoScrollMargin : int

此属性保存触发自动滚动时区域的大小。


此属性控制触发自动滚动的视口边缘区域的大小。 默认值是16像素。


这个属性是在Qt 4.6中引入的。


访问函数；

```cpp
Qt::DropAction defaultDropAction() const
void setDefaultDropAction(Qt::DropAction dropAction)
```




4、dragDropMode : DragDropMode

该属性包含视图将执行的拖放事件。


该属性在Qt 4.2中引入。


访问函数；

```cpp
DragDropMode dragDropMode() const
void setDragDropMode(DragDropMode behavior)
```




5、dragDropOverwriteMode : bool

该属性保存视图的拖放行为。


如果其值为true，则选定的数据将在丢弃时覆盖现有的项目数据，而移动数据将清除该项目。 如果其值为假，则在删除数据时，所选数据将作为新项目插入。 当数据移动时，该项目也会被移除。


默认值为false，如在QListView和QTreeView子类中。 另一方面，在QTableView子类中，该属性已设置为true。


注意：这不是为了防止重写项目。 模型的flags（）实现应该通过不返回Qt :: ItemIsDropEnabled来实现。


该属性在Qt 4.2中引入。


访问函数；

```cpp
bool dragDropOverwriteMode() const
void setDragDropOverwriteMode(bool overwrite)
```




6、dragEnabled : bool

该属性保持视图是否支持拖拽自己的项目。


访问函数；

```cpp
bool dragEnabled() const
void setDragEnabled(bool enable)
```



7、editTriggers : EditTriggers

该属性保存哪些操作将启动项目编辑。


该属性是由EditTrigger定义的一组标志，使用OR运算符进行组合。 如果在此属性中设置了所执行的操作，视图将只启动项目的编辑。

访问函数；

```cpp
EditTriggers editTriggers() const
void setEditTriggers(EditTriggers triggers)
```




8、horizontalScrollMode : ScrollMode

该属性保存视图如何在水平方向上滚动其内容。


该属性控制视图如何水平滚动其内容。 滚动可以按照每个像素或每个项目完成。 它的默认值来自通过QStyle :: SH_ItemView_ScrollMode样式提示的样式。


该属性在Qt 4.2中引入。


访问函数；

```cpp
ScrollMode horizontalScrollMode() const
void setHorizontalScrollMode(ScrollMode mode)
void resetHorizontalScrollMode()
```




9、iconSize : QSize

该属性保存项目图标的大小。


当视图可见时设置此属性将导致项目再次布局。


访问函数；

```cpp
QSize iconSize() const
void setIconSize(const QSize &size)
```

通知信号：


```cpp
void iconSizeChanged(const QSize &size)
```



10、selectionBehavior : SelectionBehavior

该属性保存视图使用哪种选择行为。


该属性保持是否按单项，行或列进行选择。


访问函数；

```cpp
QAbstractItemView::SelectionBehavior selectionBehavior() const
void setSelectionBehavior(QAbstractItemView::SelectionBehavior behavior)
```



11、selectionMode : SelectionMode

该属性保存视图在哪种选择模式下操作。


该属性控制用户是否可以选择一个或多个项目，并在多项选择中确定该选择是否必须是连续范围的项目。


访问函数；

```cpp
QAbstractItemView::SelectionMode selectionMode() const
void setSelectionMode(QAbstractItemView::SelectionMode mode)
```



12、showDropIndicator : bool

此属性可以确定在拖放项目并放下时是否显示放置指示符。


访问函数；

```cpp
bool showDropIndicator() const
void setDropIndicatorShown(bool enable
```



13、tabKeyNavigation : bool

该属性保持是否启用带选项卡和后退选项卡的项目导航。


访问函数；

```cpp
bool tabKeyNavigation() const
void setTabKeyNavigation(bool enable)
```



14、textElideMode : Qt::TextElideMode

该属性在消除文本中保留“...”的位置。


所有项目视图的默认值是Qt :: ElideRight。


访问函数；

```cpp
Qt::TextElideMode textElideMode() const
void setTextElideMode(Qt::TextElideMode mode)
```



15、verticalScrollMode : ScrollMode

该属性保存视图如何在垂直方向上滚动其内容。


该属性控制视图垂直滚动其内容的方式。 滚动可以按照每个像素或每个项目完成。 它的默认值来自通过QStyle :: SH_ItemView_ScrollMode样式提示的样式。


该属性在Qt 4.2中引入。


访问函数；

```cpp
ScrollMode 
verticalScrollMode() const
void setVerticalScrollMode(ScrollMode mode)
void resetVerticalScrollMode()
```






三、成员函数

1、QAbstractItemView::QAbstractItemView(QWidget *parent = Q_NULLPTR)

用给定的父项构造一个抽象项目视图。





2、QAbstractItemView::~QAbstractItemView()

销毁视图。





3、[signal] void QAbstractItemView::activated(const QModelIndex &index)

索引指定的项目由用户激活时发出此信号。 如何激活项目取决于平台; 例如通过单击或双击该项目，或者在项目最新时按下Return或Enter键。





4、[slot] void QAbstractItemView::clearSelection()

取消选择所有选定的项目。 目前的指数不会改变。





5、[signal] void QAbstractItemView::clicked(const QModelIndex &index)

当鼠标左键单击时发出此信号。 鼠标点击的项目由索引指定。 只有索引有效时才会发出信号。





6、[virtual protected slot] void QAbstractItemView::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)

关闭给定的编辑器，然后释放它。 该提示用于指定视图应如何响应编辑操作的结束。 例如，提示可能表示应该打开视图中的下一个项目进行编辑。





7、void QAbstractItemView::closePersistentEditor(const QModelIndex &index)

关闭给定索引处项目的持久性编辑器。





8、[virtual protected slot] void QAbstractItemView::commitData(QWidget *editor)

将编辑器中的数据提交给模型。





9、[virtual protected slot] void QAbstractItemView::currentChanged(const QModelIndex ¤t, const QModelIndex &previous)

当新项目成为当前项目时调用此插槽。 前一个当前项目由前一个索引指定，而新项目由当前索引指定。


如果您想了解项目的更改，请参阅dataChanged（）信号。





10、QModelIndex QAbstractItemView::currentIndex() const

返回当前项目的模型索引。





11、[virtual protected slot] void QAbstractItemView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ())

当在模型中更改具有给定角色的项目时，将调用此插槽。 更改的项目是从topLeft到bottomRight包含的项目。 如果只有一个项目改变topLeft == bottomRight。


已更改的角色可以是空容器（意味着所有内容都已更改），也可以是角色子集已更改的非空容器。





12、[protected] QPoint QAbstractItemView::dirtyRegionOffset() const

返回视图中脏区域的偏移量。


如果使用scrollDirtyRegion（）并在QAbstractItemView的子类中实现paintEvent（），则应该将paint事件给出的区域与此函数返回的偏移量一起转换。





13、[signal] void QAbstractItemView::doubleClicked(const QModelIndex &index)

双击鼠标按钮时会发出此信号。 鼠标双击的项目由索引指定。 只有索引有效时才会发出信号。





14、[virtual protected] void QAbstractItemView::dragEnterEvent(QDragEnterEvent *event)

从QWidget :: dragEnterEvent（）重新实现。


当拖放操作进入窗口小部件时，该函数将与给定事件一起调用。 如果拖动结束了有效的放置位置（例如在接受放置的项目上），则该事件被接受; 否则会被忽略。





15、[virtual protected] void QAbstractItemView::dragLeaveEvent(QDragLeaveEvent *event)

从QWidget :: dragLeaveEvent（）重新实现。


当被拖动的项目离开视图时调用此函数。 该事件描述了拖放操作的状态。





16、[virtual protected] void QAbstractItemView::dragMoveEvent(QDragMoveEvent *event)

从QWidget :: dragMoveEvent（）重新实现。


在对窗口小部件进行拖放操作的过程中，该函数将与给定事件一起被连续调用。 例如，如果用户拖动选区以查看右边缘或底部边缘，则可以导致视图滚动。 在这种情况下，该活动将被接受; 否则会被忽略。





17、[virtual protected] void QAbstractItemView::dropEvent(QDropEvent *event)

从QWidget :: dropEvent（）重新实现。


当在控件上发生拖放事件时，使用给定事件调用此函数。 如果模型接受均匀位置，则放弃事件被接受; 否则会被忽略。





18、[protected] DropIndicatorPosition QAbstractItemView::dropIndicatorPosition() const

返回放置指示符相对于最近项目的位置。

这个函数是在Qt 4.1中引入的。





19、[slot] void QAbstractItemView::edit(const QModelIndex &index)

如果可编辑，则开始编辑与给定索引对应的项目。


请注意，此功能不会更改当前索引。 由于当前索引定义了要编辑的下一个和上一个项目，因此用户可能会发现键盘导航无法按预期工作。 为了提供一致的导航行为，请在此函数之前使用相同的模型索引调用setCurrentIndex（）。





20、[virtual protected] bool QAbstractItemView::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event)



在索引处开始编辑项目，必要时创建编辑器，如果视图的状态现在是EditingState，则返回true; 否则返回false。

导致编辑过程的操作由触发器描述，相关事件由事件指定。

通过将触发器指定为QAbstractItemView :: AllEditTriggers，可以强制编辑。




21、[virtual protected slot] void QAbstractItemView::editorDestroyed(QObject *editor)

当给定的编辑器被销毁时，这个函数被调用。





22、[signal] void QAbstractItemView::entered(const QModelIndex &index)

当鼠标光标进入索引指定的项目时，此信号发出。 需要启用鼠标跟踪功能才能使用此功能。





23、[virtual protected] bool QAbstractItemView::event(QEvent *event)

从QObject :: event（）重新实现。





24、[protected] void QAbstractItemView::executeDelayedItemsLayout()

执行预定的布局而无需等待事件处理开始。





25、[virtual protected] void QAbstractItemView::focusInEvent(QFocusEvent *event)

从QWidget :: focusInEvent（）重新实现。

当小部件获得焦点时，使用给定事件调用此函数。 默认情况下，该事件被忽略。





26、[virtual protected] bool QAbstractItemView::focusNextPrevChild(bool next)

从QWidget :: focusNextPrevChild（）重新实现。





27、[virtual protected] void QAbstractItemView::focusOutEvent(QFocusEvent *event)

从QWidget :: focusOutEvent（）重新实现。

当小部件失去焦点时，使用给定事件调用此函数。 默认情况下，该事件被忽略。





28、[pure virtual protected] int QAbstractItemView::horizontalOffset() const

返回视图的水平偏移量。

在基类中，这是一个纯虚函数。





29、[pure virtual] QModelIndex QAbstractItemView::indexAt(const QPoint &point) const



返回视口坐标点处的项目的模型索引。

在基类中，这是一个纯虚函数。




30、QWidget *QAbstractItemView::indexWidget(const QModelIndex &index) const

返回给定索引处的项目的小部件。

这个函数是在Qt 4.1中引入的。





31、[virtual protected] void QAbstractItemView::inputMethodEvent(QInputMethodEvent *event)

从QWidget :: inputMethodEvent（）重新实现。





32、[virtual] QVariant QAbstractItemView::inputMethodQuery(Qt::InputMethodQuery query) const

从QWidget :: inputMethodQuery（）重新实现。





33、[pure virtual protected] bool QAbstractItemView::isIndexHidden(const QModelIndex &index) const

如果给定索引引用的项目隐藏在视图中，则返回true，否则返回false。

隐藏是视图特定功能。 例如在TableView中，一列可以被标记为隐藏或TreeView中的一行。


在基类中，这是一个纯虚函数。





34、QAbstractItemDelegate *QAbstractItemView::itemDelegate() const

返回此视图和模型使用的项目委托。 这是用setItemDelegate（）设置的一个，或者默认的一个。





35、QAbstractItemDelegate *QAbstractItemView::itemDelegate(const QModelIndex &index) const

返回此视图和模型对给定索引使用的项目委托。





36、QAbstractItemDelegate *QAbstractItemView::itemDelegateForColumn(int column) const

返回此视图和模型对给定列使用的项目委托。 您可以调用itemDelegate（）以获取指向当前委托的指针。

这个函数是在Qt 4.2中引入的。





37、QAbstractItemDelegate *QAbstractItemView::itemDelegateForRow(int row) const

返回此视图和模型对给定行使用的项目委托，如果未分配委派，则返回0。 您可以调用itemDelegate（）以获取指向当前委托的指针。

这个函数是在Qt 4.2中引入的。





38、[virtual protected] void QAbstractItemView::keyPressEvent(QKeyEvent *event)

从QWidget :: keyPressEvent（）重新实现。

当一个关键事件被发送到这个小部件时，这个函数被调用给定的事件。 默认实现处理基本的光标移动，例如 Up，Down，Left，Right，Home，PageUp和PageDown; 如果当前索引有效且激活键被按下（例如根据平台的输入或返回），则激活（）信号被发射。 该功能是通过按键启动编辑的地方，例如， 如果F2被按下。





39、[virtual] void QAbstractItemView::keyboardSearch(const QString &search)

移至并选择最符合字符串搜索的项目。 如果没有发现任何物品。

在默认实现中，如果搜索为空，或者自上次搜索以来的时间间隔超过QApplication :: keyboardInputInterval（），则重置搜索。





40、QAbstractItemModel *QAbstractItemView::model() const

返回此视图呈现的模型。





41、[virtual protected] void QAbstractItemView::mouseDoubleClickEvent(QMouseEvent *event)

从QWidget :: mouseDoubleClickEvent（）重新实现。

当在控件内部双击鼠标按钮时，使用给定事件调用此函数。 如果双击位于有效项目上，它会发出doubleClicked（）信号并调用该项目上的edit（）。





42、[virtual protected] void QAbstractItemView::mouseMoveEvent(QMouseEvent *event)

从QWidget :: mouseMoveEvent（）重新实现。

当鼠标移动事件发送到小部件时，使用给定事件调用此函数。 如果正在进行选择并且新项目在选择范围内移动， 如果拖动正在进行，则继续。





43、[virtual protected] void QAbstractItemView::mousePressEvent(QMouseEvent *event)

从QWidget :: mousePressEvent（）重新实现。

当光标在小部件内部时按下鼠标按钮时，将使用给定事件调用此函数。 如果一个有效的物品被按下，它将被置入当前物品中。 该功能发出按下的（）信号。





44、[virtual protected] void QAbstractItemView::mouseReleaseEvent(QMouseEvent *event)

从QWidget :: mouseReleaseEvent（）重新实现。

在鼠标按下事件后，鼠标按钮被释放后，该函数将被调用。 如果用户在您的小部件内按下鼠标，然后在释放鼠标按钮之前将鼠标拖动到另一个位置，您的小部件将收到释放事件。 如果项目被按下，该功能将发出clicked（）信号。





45、[pure virtual protected] QModelIndex QAbstractItemView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)

基于由修饰符指定的给定cursorAction和键盘修饰符，返回指向视图中下一个对象的QModelIndex对象。

在基类中，这是一个纯虚函数。





46、void QAbstractItemView::openPersistentEditor(const QModelIndex &index)

在给定索引处的项目上打开持久编辑器。 如果没有编辑器，代表将创建一个新的编辑器。





47、[signal] void QAbstractItemView::pressed(const QModelIndex &index)

按下鼠标按钮时会发出此信号。 鼠标按下的项目由索引指定。 只有索引有效时才会发出信号。

使用QApplication :: mouseButtons（）函数来获取鼠标按钮的状态。





48、[virtual slot] void QAbstractItemView::reset()

重置视图的内部状态。

警告：此功能将重置打开的编辑器，滚动条位置，选择等。现有更改不会被提交。 如果您想在重置视图时保存更改，则可以重新实现此功能，提交更改，然后调用超类的实现。





49、[virtual protected] void QAbstractItemView::resizeEvent(QResizeEvent *event)

从QWidget :: resizeEvent（）重新实现。

当调整大小事件发送到窗口小部件时，该函数将与给定事件一起调用。





50、QModelIndex QAbstractItemView::rootIndex() const

返回模型根项目的模型索引。 根项目是视图的顶级项目的父项目。 根可能无效。





51、[virtual protected slot] void QAbstractItemView::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)

当行即将被删除时，此槽将被调用。 删除的行是从开始到结尾（含）的给定父项下的行。





52、[virtual protected slot] void QAbstractItemView::rowsInserted(const QModelIndex &parent, int start, int end)

该插槽在插入行时调用。 新行是从开始到结束包含在给定父项下的行。 基类实现在模型上调用fetchMore（）来检查更多数据。





53、[protected] void QAbstractItemView::scheduleDelayedItemsLayout()

计划视图中项目的布局，以便在事件处理开始时执行。

即使在处理事件之前多次调用scheduleDelayedItemsLayout（），该视图也只会执行一次布局。





54、[protected] void QAbstractItemView::scrollDirtyRegion(int dx, int dy)

通过在相反方向移动脏区来准备视图以滚动（dx，dy）像素。 如果您在视图子类中实现滚动视口，则只需调用此函数。

如果在QAbstractItemView的子类中实现scrollContentsBy（），请在调用视口上的QWidget :: scroll（）之前调用此函数。 或者，只需调用update（）。





55、[pure virtual] void QAbstractItemView::scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible)



如有必要滚动视图以确保索引处的项目可见。 该视图将尝试根据给定的提示来定位该项目。

在基类中，这是一个纯虚函数。




56、[slot] void QAbstractItemView::scrollToBottom()

将视图滚动到底部。

这个函数是在Qt 4.1中引入的。





57、[slot] void QAbstractItemView::scrollToTop()

将视图滚动到顶部。

这个函数是在Qt 4.1中引入的。





58、[virtual slot] void QAbstractItemView::selectAll()

选择视图中的所有项目。 此功能将使用选择时在视图上设置的选择行为。





59、[virtual protected] QModelIndexList QAbstractItemView::selectedIndexes() const

这个便利函数返回视图中所有选定和非隐藏项目索引的列表。 该列表不包含重复项，并且未被排序。





60、[virtual protected slot] void QAbstractItemView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)

当选择被改变时，该槽被调用。 先前的选择（可能为空）由取消选择指定，并通过选择新选择。





61、[virtual protected] QItemSelectionModel::SelectionFlags QAbstractItemView::selectionCommand(const QModelIndex &index, const QEvent *event = Q_NULLPTR) const

返回在更新包含指定索引的选择时要使用的SelectionFlags。 该事件是用户输入事件，例如鼠标或键盘事件。

重新实现这个功能来定义你自己的选择行为。





62、QItemSelectionModel *QAbstractItemView::selectionModel() const

返回当前的选择模型。





63、[slot] void QAbstractItemView::setCurrentIndex(const QModelIndex &index)



将当前项目设置为索引处的项目。

除非当前的选择模式是NoSelection，否则该项目也被选中。 请注意，此功能还会更新用户执行的任何新选择的起始位置。

要将项目设置为当前项目而不选择它，请调用；

```cpp
selectionModel()->setCurrentIndex(index, QItemSelectionModel::NoUpdate);
```




64、[protected] void QAbstractItemView::setDirtyRegion(const QRegion ®ion)

将给定区域标记为脏，并安排更新。 如果你正在实现你自己的视图子类，你只需要调用这个函数。

这个函数是在Qt 4.1中引入的。





65、void QAbstractItemView::setIndexWidget(const QModelIndex &index, QWidget *widget)



在给定索引的项目上设置给定的小部件，并将小部件的所有权传递给视口。

如果索引无效（例如，如果您传递根索引），则此函数不会执行任何操作。
给定小部件的autoFillBackground属性必须设置为true，否则小部件的背景将是透明的，在给定索引处显示模型数据和项目。

如果索引部件A被替换为索引部件B，则索引部件A将被删除。 例如，在下面的代码片段中，QLineEdit对象将被删除。

```cpp
setIndexWidget(index, new QLineEdit);
  ...
  setIndexWidget(index, new QTextEdit);
```

此功能只能用于显示与某项数据对应的可见区域内的静态内容。 如果要显示自定义动态内容或实现自定义编辑器小部件，请改为QItemDelegate的子类。

这个函数是在Qt 4.1中引入的。




66、void QAbstractItemView::setItemDelegate(QAbstractItemDelegate *delegate)



将此视图的项目委托及其模型设置为委派。 如果您想完全控制项目的编辑和显示，这非常有用。

任何现有的委托将被删除，但不会被删除。 QAbstractItemView不承担委托的所有权。

警告：您不应该在视图之间共享委托的同一个实例。 这样做会导致不正确或不直观的编辑行为，因为连接到给定委托的每个视图都可能会收到closeEditor（）信号，并尝试访问，修改或关闭已关闭的编辑器。




67、void QAbstractItemView::setItemDelegateForColumn(int column, QAbstractItemDelegate *delegate)



为给定列设置此视图和模型使用的给定项委托。 列上的所有项目都将由委托进行绘制和管理，而不是使用默认委托（即itemDelegate（））。

列的任何现有列委托都将被删除，但不会被删除。 QAbstractItemView不承担委托的所有权。

注意：如果代理已分配给行和列，则行代理将优先并管理相交的单元索引。

警告：您不应该在视图之间共享委托的同一个实例。 这样做会导致不正确或不直观的编辑行为，因为连接到给定委托的每个视图都可能会收到closeEditor（）信号，并尝试访问，修改或关闭已关闭的编辑器。

这个函数是在Qt 4.2中引入的。




68、void QAbstractItemView::setItemDelegateForRow(int row, QAbstractItemDelegate *delegate)



为给定行设置此视图和模型使用的给定项目委托。 行上的所有项目都将由委托进行绘制和管理，而不是使用默认委托（即itemDelegate（））。

任何现有的行代理行将被删除，但不会被删除。 QAbstractItemView不承担委托的所有权。

注意：如果代理已分配给行和列，则行代表（即此代理）将优先并管理相交的单元索引。

警告：您不应该在视图之间共享委托的同一个实例。 这样做会导致不正确或不直观的编辑行为，因为连接到给定委托的每个视图都可能会收到closeEditor（）信号，并尝试访问，修改或关闭已关闭的编辑器。

这个函数是在Qt 4.2中引入的。




69、[virtual] void QAbstractItemView::setModel(QAbstractItemModel *model)

设置要呈现的视图的模型。

该函数将创建并设置一个新的选择模型，替换之前由setSelectionModel（）设置的任何模型。 但是，旧的选择模型不会被删除，因为它可能会在多个视图之间共享。 如果不再需要，我们建议您删除旧的选择模型。 这是用下面的代码完成的：


```cpp
QItemSelectionModel *m = view->selectionModel();
  view->setModel(new model);
  delete m;
```

如果旧模型和旧选择模型都没有父项，或者父项是长寿命对象，则最好调用它们的deleteLater（）函数以显式删除它们。

该视图不会取得该模型的所有权，除非它是该模型的父对象，因为该模型可以在许多不同视图之间共享。




70、[virtual slot] void QAbstractItemView::setRootIndex(const QModelIndex &index)

将根项目设置为给定索引处的项目。





71、[pure virtual protected] void QAbstractItemView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags)

将选择标志应用于矩形中的或矩形触摸的项目。

在实现自己的itemview时，setSelection应该调用selectionModel（） - > select（selection，flags），其中的选择是一个空的QModelIndex或一个包含rect中包含的所有项目的QItemSelection。





72、[virtual] void QAbstractItemView::setSelectionModel(QItemSelectionModel *selectionModel)



将当前选择模型设置为给定的selectionModel。

请注意，如果在此函数之后调用setModel（），则给定的selectionModel将被视图创建的selectionModel替换。

注意：如果不再需要，删除旧的选择模型取决于应用程序; 即，如果它没有被其他视图使用。 这会在其父对象被删除时自动发生。 但是，如果它没有父对象，或者父对象是一个长寿命的对象，则最好调用它的deleteLater（）函数来显式删除它。




73、[protected] void QAbstractItemView::setState(State state)

将项目视图的状态设置为给定状态。





74、[virtual] int QAbstractItemView::sizeHintForColumn(int column) const

返回指定列的宽度大小提示，如果没有模型，则返回-1。

此功能用于具有水平标题的视图，以根据给定列的内容查找标题部分的大小提示。





75、QSize QAbstractItemView::sizeHintForIndex(const QModelIndex &index) const

返回具有指定索引的项目的大小提示或无效索引的无效大小。





76、[virtual] int QAbstractItemView::sizeHintForRow(int row) const



返回指定行的高度尺寸提示，如果没有模型，则返回-1。

返回的高度使用给定行项目的大小提示进行计算，即返回值是项目中的最大高度。 请注意，要控制行的高度，您必须重新实现QAbstractItemDelegate :: sizeHint（）函数。

此函数用于具有垂直标题的视图中，以根据给定行的内容查找标题部分的大小提示。




77、[virtual protected] void QAbstractItemView::startDrag(Qt::DropActions supportedActions)

通过使用给定的supportedActions调用drag-> exec（）来启动拖动。





78、[protected] State QAbstractItemView::state() const

返回项目视图的状态。





79、[virtual protected] void QAbstractItemView::timerEvent(QTimerEvent *event)

从QObject :: timerEvent（）重新实现。

当定时器事件发送到小部件时，使用给定事件调用此函数。





80、[slot] void QAbstractItemView::update(const QModelIndex &index)

更新给定索引占用的区域。

这个函数是在Qt 4.3中引入的。





81、[virtual protected slot] void QAbstractItemView::updateGeometries()

更新视图的子部件的几何图形。

这个函数是在Qt 4.4中引入的。





82、[pure virtual protected] int QAbstractItemView::verticalOffset() const

返回视图的垂直偏移量。

在基类中，这是一个纯虚函数。





83、[virtual protected] QStyleOptionViewItem QAbstractItemView::viewOptions() const

返回填充了视图调色板，字体，状态，路线等的QStyleOptionViewItem结构。





84、[signal] void QAbstractItemView::viewportEntered()

该信号在鼠标光标进入视口时发出。 需要启用鼠标跟踪功能才能使用此功能。





85、[virtual protected] bool QAbstractItemView::viewportEvent(QEvent *event)

从QAbstractScrollArea :: viewportEvent（）重新实现。

此功能用于处理工具提示，这是什么？ 模式，如果给定的事件是QEvent :: ToolTip或QEvent :: WhatsThis。 它将所有其他事件传递给其基类viewportEvent（）处理函数。





86、[virtual protected] QSize QAbstractItemView::viewportSizeHint() const

从QAbstractScrollArea :: viewportSizeHint（）重新实现。

这个函数是在Qt 5.2中引入的。





87、[pure virtual] QRect QAbstractItemView::visualRect(const QModelIndex &index) const



返回索引处项目占用的视口上的矩形。

如果您的项目显示在多个区域，那么visualRect应返回包含索引的主区域，而不是索引可能包含，触摸或导致绘制的完整区域。

在基类中，这是一个纯虚函数。




88、[pure virtual protected] QRegion QAbstractItemView::visualRegionForSelection(const QItemSelection &selection) const

从给定选择中的项目的视口返回区域。

在基类中，这是一个纯虚函数。




