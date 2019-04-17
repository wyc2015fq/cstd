# Qt开发：Qt Widgets模块——QBoxLayout类 - 一世豁然的专栏 - CSDN博客





2018年05月18日 21:23:33[一世豁然](https://me.csdn.net/Explorer_day)阅读数：327








QBoxLayout类水平或垂直排列子窗口小部件。



头文件：#include <QBoxLayout> 

qmake：QT += widgets

继承：QLayout

被继承：QHBoxLayout和QVBoxLayout

QBoxLayout从它的父布局或从parentWidget（）获取它获得的空间，将它分成一行框，并使每个被管理的小部件填满一个框。


![](https://img-blog.csdn.net/20180518202217435)


如果QBoxLayout的方向是Qt :: Horizontal，那么这些方块会放在一排，并且尺寸合适。 每个小部件（或其他盒子）将至少获得其最小尺寸和最大尺寸。 任何多余的空间是根据拉伸因子共享的（更多关于下面的内容）。


![](https://img-blog.csdn.net/20180518202249810)


如果QBoxLayout的方向是Qt :: Vertical，那么这些方框将放置在一列中，并且大小合适。


创建QBoxLayout最简单的方法是使用其中一个便利类，例如 QHBoxLayout（用于Qt :: Horizontal框）或QVBoxLayout（用于Qt :: Vertical框）。 您也可以直接使用QBoxLayout构造函数，将其方向指定为LeftToRight，RightToLeft，TopToBottom或BottomToTop。


如果QBoxLayout不是顶层布局（即它不管理所有窗口部件的区域和子窗口），则必须将其添加到其父窗口，然后才能对其执行任何操作。 添加布局的正常方法是调用parentLayout-> addLayout（）。


完成此操作后，您可以使用以下四种功能之一将框添加到QBoxLayout：


1）、addWidget（）向QBoxLayout添加一个小部件并设置小部件的拉伸因子。 （拉伸因子沿着一排方框。）

2）、addSpacing（）创建一个空框; 这是您用来创建漂亮而宽敞的对话框的功能之一。 请参阅下面的设置边距的方法。

3）、addStretch（）创建一个空的可拉伸框。

4）、addLayout（）将包含另一个QLayout的框添加到该行并设置该布局的拉伸因子。

使用insertWidget（），insertSpacing（），insertStretch（）或insertLayout（）在布局中的指定位置插入一个框。


QBoxLayout还包含两个边距宽度：


1）、setContentsMargins（）设置窗口小部件每边的外边框宽度。 这是每个QBoxLayout四边的保留空间的宽度。

2）、setSpacing（）设置相邻框之间的宽度。 （您可以使用addSpacing（）在特定位置获取更多空间。）

保证金默认值由样式提供。 大多数Qt样式指定的默认边界为9，子窗口小部件和11窗口。 间距默认与顶层布局的边距宽度相同，或者与父布局相同。


要从布局中移除小部件，请调用removeWidget（）。 在小部件上调用QWidget :: hide（）也会有效地从布局中移除小部件，直到调用QWidget :: show（）。


你几乎总是希望使用QVBoxLayout和QHBoxLayout而不是QBoxLayout，因为它们方便的构造函数。








一、成员类型

1、enum QBoxLayout::Direction

这种类型用于确定箱子布局的方向。



|类型|值|描述|
|----|----|----|
|QBoxLayout::LeftToRight|0|从左到右水平。|
|QBoxLayout::RightToLeft|1|从右到左水平。|
|QBoxLayout::TopToBottom|2|垂直从上到下。|
|QBoxLayout::BottomToTop|3|从下到上垂直。|




二、成员函数

1、QBoxLayout::QBoxLayout(Direction dir, QWidget *parent = Q_NULLPTR)


用方向目录和父窗口小部件父项构造一个新的QBoxLayout。





2、QBoxLayout::~QBoxLayout()

销毁这个盒子布局。

布局的小部件不会被销毁。





3、[virtual] void QBoxLayout::addItem(QLayoutItem *item)

从QLayout :: addItem（）重新实现。





4、void QBoxLayout::addLayout(QLayout *layout, int stretch = 0)

将布局添加到方框的末端，并使用连续拉伸系数拉伸。





5、void QBoxLayout::addSpacerItem(QSpacerItem *spacerItem)

将spacerItem添加到此框布局的末尾。

这个函数是在Qt 4.4中引入的。





6、void QBoxLayout::addSpacing(int size)

在此框布局的末尾添加一个大小不可拉伸的空间（一个QSpacerItem）。 QBoxLayout提供了默认的边距和间距。 该功能增加了额外的空间。





7、void QBoxLayout::addStretch(int stretch = 0)

添加具有零最小尺寸和拉伸因子拉伸的可拉伸空间（一个QSpacerItem），直到此框布局结束。





8、void QBoxLayout::addStrut(int size)

限制框的垂直尺寸（例如，如果框为LeftToRight，则为高度）为最小尺寸。 其他限制可能会增加限制。





9、void QBoxLayout::addWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment())



将小部件添加到此框布局的末尾，并使用拉伸因子和对齐线对齐。

拉伸因子只适用于QBoxLayout的方向，并且相对于此QBoxLayout中的其他框和小部件。 具有较高拉伸因子的小部件和盒子增长更多。

如果伸展因子为0，并且QBoxLayout中没有其他东西的伸展因子大于零，则根据涉及的每个小部件的QWidget：sizePolicy（）分配空间。

对齐由对齐指定。 默认对齐方式为0，这意味着小部件将填充整个单元格。




10、[virtual] int QBoxLayout::count() const

从QLayout :: count（）中重新实现。





11、Direction QBoxLayout::direction() const

返回框的方向。 addWidget（）和addSpacing（）在这个方向上工作; 伸展在这个方向延伸。





12、[virtual] Qt::Orientations QBoxLayout::expandingDirections() const

从QLayoutItem :: expandingDirections（）重新实现。





13、[virtual] bool QBoxLayout::hasHeightForWidth() const

从QLayoutItem :: hasHeightForWidth（）重新实现。





14、[virtual] int QBoxLayout::heightForWidth(int w) const

从QLayoutItem :: heightForWidth（）重新实现。





15、void QBoxLayout::insertItem(int index, QLayoutItem *item)

在位置索引处将项目插入此框布局。 如果索引是负数，则在最后添加项目。





16、void QBoxLayout::insertLayout(int index, QLayout *layout, int stretch = 0)

在位置索引处插入布局，拉伸因子拉伸。 如果索引是负数，则在最后添加布局。

布局成为箱子布局的一个子。





17、void QBoxLayout::insertSpacerItem(int index, QSpacerItem *spacerItem)

在位置索引处插入spacerItem，零最小尺寸和拉伸因子。 如果索引是负数，则在最后添加空格。

这个函数是在Qt 4.4中引入的。





18、void QBoxLayout::insertSpacing(int index, int size)

在位置索引处插入一个不可伸展的空间（一个QSpacerItem），大小为size。 如果索引是负数，则在最后添加空格。

箱子布局有默认的边距和间距。 该功能增加了额外的空间。





19、void QBoxLayout::insertStretch(int index, int stretch = 0)

在位置索引处插入可拉伸的空间（一个QSpacerItem），具有零最小尺寸和拉伸因子拉伸。 如果索引是负数，则在最后添加空格。





20、void QBoxLayout::insertWidget(int index, QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment())



在位置索引处插入小部件，并进行拉伸因子拉伸和对齐。 如果索引为负数，则在最后添加小部件。

拉伸因子只适用于QBoxLayout的方向，并且相对于此QBoxLayout中的其他框和小部件。 具有较高拉伸因子的小部件和盒子增长更多。

如果伸展因子为0，并且QBoxLayout中没有其他东西的伸展因子大于零，则根据涉及的每个小部件的QWidget：sizePolicy（）分配空间。

对齐由对齐指定。 默认对齐方式为0，这意味着小部件将填充整个单元格。




21、[virtual] void QBoxLayout::invalidate()

从QLayoutItem :: invalidate（）重新实现。

重置缓存的信息。





22、[virtual] QLayoutItem *QBoxLayout::itemAt(int index) const

从QLayout :: itemAt（）重新实现。





23、[virtual] QSize QBoxLayout::maximumSize() const

从QLayoutItem :: maximumSize（）重新实现。





24、[virtual] int QBoxLayout::minimumHeightForWidth(int w) const

从QLayoutItem :: minimumHeightForWidth（）重新实现。





25、[virtual] QSize QBoxLayout::minimumSize() const

从QLayoutItem :: minimumSize（）重新实现。





26、void QBoxLayout::setDirection(Direction direction)

将此布局的方向设置为方向。





27、[virtual] void QBoxLayout::setGeometry(const QRect &r)

从QLayoutItem :: setGeometry（）重新实现。





28、void QBoxLayout::setSpacing(int spacing)

重新实现QLayout :: setSpacing（）。 将间距属性设置为间距。





29、void QBoxLayout::setStretch(int index, int stretch)

在位置索引处设置拉伸因子。 伸展。

这个函数是在Qt 4.5中引入的。





30、bool QBoxLayout::setStretchFactor(QWidget *widget, int stretch)

如果在此布局中找到小部件（不包括子布局），则将小部件的拉伸因子设置为拉伸并返回true; 否则返回false。





31、bool QBoxLayout::setStretchFactor(QLayout *layout, int stretch)

这是一个重载函数。

如果在此布局中找到布局（不包括子布局），则将布局布局的拉伸因子设置为拉伸并返回true; 否则返回false。





32、[virtual] QSize QBoxLayout::sizeHint() const

从QLayoutItem :: sizeHint（）重新实现。





33、int QBoxLayout::spacing() const

重新实现QLayout :: spacing（）。 如果spacing属性有效，则返回该值。 否则，计算并返回间隔属性的值。 由于窗口小部件中的布局间距与样式有关，如果父窗口是窗口小部件，它将查询布局的（水平或垂直）间距样式。 否则，父级是一个布局，并且它会查询父级布局的间距（）。





34、int QBoxLayout::stretch(int index) const

返回位置索引处的拉伸因子。

这个函数是在Qt 4.5中引入的。





35、[virtual] QLayoutItem *QBoxLayout::takeAt(int index)

从QLayout :: takeAt（）重新实现。




