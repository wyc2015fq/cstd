# Qt开发：Qt Widgets模块——QColumnView类 - 一世豁然的专栏 - CSDN博客





2018年05月22日 08:47:27[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1750








QColumnView类提供了一个列视图的模型/视图实现。



头文件：#include <QColumnView>

qmake：QT += widgets

继承：QAbstractItemView



QColumnView在许多QListView中显示一个模型，树中的每个层次结构都有一个模型。 这有时被称为级联列表。

QColumnView类是模型/视图类之一，是Qt的模型/视图框架的一部分。

QColumnView实现由QAbstractItemView类定义的接口，以允许其显示由QAbstractItemModel类派生的模型提供的数据。

![](https://img-blog.csdn.net/20180522083008838)








一、属性

1、resizeGripsVisible : bool

该属性可以指定列表视图是否获取调整大小。

默认情况下，可见性设置为true。


访问函数；

```cpp
bool resizeGripsVisible() const
void setResizeGripsVisible(bool visible)
```







二、成员函数

1、QColumnView::QColumnView(QWidget *parent = Q_NULLPTR)

使用父级构造一个列视图来表示模型的数据。 使用setModel（）来设置模型。





2、QColumnView::~QColumnView()

销毁列视图。





3、QList<int> QColumnView::columnWidths() const

返回此视图中所有列的宽度列表。





4、[virtual protected] QAbstractItemView *QColumnView::createColumn(const QModelIndex &index)

当您选择一个项目重载此函数并返回一个小部件时，要为最终列使用自定义小部件。 索引是将被分配给视图的根索引。

返回新视图。 QColumnView将自动获取小部件的所有权。





5、[virtual protected] void QColumnView::currentChanged(const QModelIndex ¤t, const QModelIndex &previous)

从QAbstractItemView :: currentChanged（）重新实现。





6、[virtual protected] int QColumnView::horizontalOffset() const

从QAbstractItemView :: horizontalOffset（）重新实现。





7、[virtual] QModelIndex QColumnView::indexAt(const QPoint &point) const

从QAbstractItemView :: indexAt（）重新实现。





8、[protected] void QColumnView::initializeColumn(QAbstractItemView *column) const

复制列视图的行为和选项，并将其应用于列，如iconSize（），textElideMode（）和alternatingRowColors（）。 这在重新实现createColumn（）时很有用。

这个函数是在Qt 4.4中引入的。





9、[virtual protected] bool QColumnView::isIndexHidden(const QModelIndex &index) const

从QAbstractItemView :: isIndexHidden（）重新实现。





10、[virtual protected] QModelIndex QColumnView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)

从QAbstractItemView :: moveCursor（）重新实现。

向左移动应该转到父索引。如果没有子类，移动向右应该转到子索引或向下。





11、QWidget *QColumnView::previewWidget() const

返回预览小部件，如果没有则返回0。





12、[virtual protected] void QColumnView::resizeEvent(QResizeEvent *event)

从QWidget :: resizeEvent（）重新实现。





13、[virtual protected] void QColumnView::rowsInserted(const QModelIndex &parent, int start, int end)

从QAbstractItemView :: rowsInserted（）重新实现。





14、[virtual protected] void QColumnView::scrollContentsBy(int dx, int dy)

从QAbstractScrollArea :: scrollContentsBy（）重新实现。





15、[virtual] void QColumnView::scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible)

从QAbstractItemView :: scrollTo（）重新实现。





16、[virtual] void QColumnView::selectAll()

从QAbstractItemView :: selectAll（）重新实现。





17、void QColumnView::setColumnWidths(const QList<int> &list)

将列宽设置为列表中给出的值。 列表中的额外值将在创建列时保留和使用。

如果列表包含的值太少，则只有其余列的宽度不会被修改。





18、[virtual] void QColumnView::setModel(QAbstractItemModel *model)

从QAbstractItemView :: setModel（）重新实现。





19、void QColumnView::setPreviewWidget(QWidget *widget)

设置预览小部件。

该小部件成为列视图的子项，并且在删除列区域或设置新小部件时将被销毁。





20、[virtual] void QColumnView::setRootIndex(const QModelIndex &index)

从QAbstractItemView :: setRootIndex（）重新实现。





21、[virtual protected] void QColumnView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)

从QAbstractItemView :: setSelection（）重新实现。





22、[virtual] void QColumnView::setSelectionModel(QItemSelectionModel *newSelectionModel)

从QAbstractItemView :: setSelectionModel（）重新实现。





23、[virtual] QSize QColumnView::sizeHint() const

从QWidget :: sizeHint（）重新实现。





24、[signal] void QColumnView::updatePreviewWidget(const QModelIndex &index)

当应该更新预览小部件以提供有关索引的丰富信息时，会发出此信号





25、[virtual protected] int QColumnView::verticalOffset() const

从QAbstractItemView :: verticalOffset（）重新实现。





26、[virtual] QRect QColumnView::visualRect(const QModelIndex &index) const

从QAbstractItemView :: visualRect（）重新实现。





27、[virtual protected] QRegion QColumnView::visualRegionForSelection(const QItemSelection &selection) const

从QAbstractItemView :: visualRegionForSelection（）重新实现。




