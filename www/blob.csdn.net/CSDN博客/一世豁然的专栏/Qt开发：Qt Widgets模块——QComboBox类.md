# Qt开发：Qt Widgets模块——QComboBox类 - 一世豁然的专栏 - CSDN博客





2018年05月23日 08:49:30[一世豁然](https://me.csdn.net/Explorer_day)阅读数：3365








QComboBox小部件是一个组合的按钮和弹出列表。

头文件：#include <QColumnView>

qmake：QT += widgets

继承：QWidget

被继承：QFontComboBox

QComboBox提供了一种向用户呈现选项列表的方式，其占用最小量的屏幕空间。

组合框是一个显示当前项目的选择小部件，可以弹出可选项目列表。 组合框可以是可编辑的，允许用户修改列表中的每个项目。


组合框可以包含像素图以及字符串; insertItem（）和setItemText（）函数适当重载。 对于可编辑组合框，提供了函数clearEditText（），以清除显示的字符串而不更改组合框的内容。


如果组合框的当前项目发生更改，currentIndexChanged（）和activate（）会发出两个信号。 无论以编程方式或通过用户交互完成更改，currentIndexChanged（）总是被发射，而只有当更改由用户交互导致时才激活（）。 突出显示的（）信号在用户突出显示组合框弹出列表中的项目时发出。 所有三个信号都有两个版本，一个带有QString参数，另一个带有int参数。 如果用户选择或突出显示一个像素图，则只会发出整型信号。 每当可编辑组合框的文本发生改变时，editTextChanged（）信号就会发出。


当用户在可编辑的组合框中输入一个新的字符串时，该小部件可能会插入它，也可能不会插入它，并且它可以将它插入到多个位置。 默认策略是InsertAtBottom，但您可以使用setInsertPolicy（）更改它。


可以使用QValidator将输入约束为可编辑的组合框; 请参阅setValidator（）。 默认情况下，接受任何输入。


例如，可以使用插入函数insertItem（）和insertItems（）来填充组合框。 可以使用setItemText（）更改项目。 一个项目可以使用removeItem（）来移除，所有项目都可以使用clear（）来移除。 当前项目的文本由currentText（）返回，编号项目的文本使用text（）返回。 当前项目可以使用setCurrentIndex（）来设置。 count（）返回组合框中的项目数; 可以用setMaxCount（）设置项目的最大数量。 您可以允许使用setEditable（）进行编辑。 对于可编辑组合框，您可以使用setCompleter（）设置自动完成，并且用户是否可以添加重复项由setDuplicatesEnabled（）进行设置。


QComboBox为其弹出列表使用模型/视图框架并存储其项目。 默认情况下，QStandardItemModel存储项目，QListView子类显示弹出列表。 您可以直接访问模型和视图（使用model（）和view（）），但QComboBox还提供了设置和获取项目数据的函数（例如，setItemData（）和itemText（））。 您还可以设置新的模型和视图（使用setModel（）和setView（））。 对于组合框标签中的文本和图标，将使用具有Qt :: DisplayRole和Qt :: DecorationRole的模型中的数据。 请注意，您不能通过使用setSelectionMode（）来更改view（）的SelectionMode。


![](https://img-blog.csdn.net/20180522213956913)








一、成员类型

1、enum QComboBox::InsertPolicy

此枚举指定用户输入新字符串时QComboBox应执行的操作。



|内容|值|描述|
|----|----|----|
|QComboBox::NoInsert|0|该字符串不会被插入到组合框中。|
|QComboBox::InsertAtTop|1|该字符串将作为组合框中的第一项插入。|
|QComboBox::InsertAtCurrent|2|当前项目将被字符串替换。|
|QComboBox::InsertAtBottom|3|该字符串将插入组合框中最后一项之后。|
|QComboBox::InsertAfterCurrent|4|该字符串插入组合框中的当前项目之后。|
|QComboBox::InsertBeforeCurrent|5|该字符串被插入到组合框中的当前项目之前。|
|QComboBox::InsertAlphabetically|6|该字符串按组合框中的字母顺序插入。|

2、enum QComboBox::SizeAdjustPolicy

此枚举指定添加新内容或更改内容时QComboBox的大小提示应如何调整。



|内容|值|描述|
|----|----|----|
|QComboBox::AdjustToContents|0|组合框将始终根据内容进行调整|
|QComboBox::AdjustToContentsOnFirstShow|1|第一次显示时，组合框将调整为其内容。|
|QComboBox::AdjustToMinimumContentsLength|2|改为使用AdjustToContents或AdjustToContentsOnFirstShow。|
|QComboBox::AdjustToMinimumContentsLengthWithIcon|3|组合框将调整为最小内容长度加上图标的空间。 出于性能原因，在大型模型上使用此策略。|




二、属性

1、count : const int

该属性保存组合框中的项目数量。

默认情况下，对于空组合框，此属性的值为0。


访问函数；

```cpp
int count() const
```




2、currentData : const QVariant



该属性保存当前项目的数据。

默认情况下，对于没有设置当前项目的空组合框或组合框，此属性包含无效的QVariant。

这个属性是在Qt 5.2中引入的。

访问函数；

```cpp
QVariant currentData(int role = Qt::UserRole) const
```



3、currentIndex : int



该属性包含组合框中当前项目的索引。

插入或删除项目时，当前索引可能会更改。

默认情况下，对于未设置当前项目的空组合框或组合框，此属性的值为-1。

访问函数；

```cpp
int currentIndex() const
void setCurrentIndex(int index)
```

通知信号；

```cpp
void currentIndexChanged(int index)
void currentIndexChanged(const QString &text)
```




4、currentText : QString



该属性保存当前文本。

如果组合框是可编辑的，则当前文本是线条编辑显示的值。 否则，如果组合框为空或未设置当前项目，则为当前项目的值或空字符串。

如果组合框是可编辑的，则setter setCurrentText（）只需调用setEditText（）。 否则，如果列表中有匹配的文本，则将currentIndex设置为相应的索引。

访问函数；

```cpp
QString currentText() const
void setCurrentText(const QString &text)
```

通知信号；

```cpp
void currentTextChanged(const QString &text)
```




5、duplicatesEnabled : bool



此属性保持用户是否可以将重复项目输入到组合框中。

请注意，总是可以以编程方式将重复项插入组合框。

默认情况下，此属性为false（不允许重复）。

访问函数；

```cpp
bool duplicatesEnabled() const
void setDuplicatesEnabled(bool enable)
```




6、editable : bool

```cpp
bool hasFrame() const
void setFrame(bool)
```


该属性保持组合框是否可由用户编辑。

默认情况下，此属性为false。 编辑的效果取决于插入策略。


访问函数；

```cpp
bool isEditable() const
void setEditable(bool editable)
```




7、frame : bool

该属性可以确定组合框是否使用框架绘制自己。

如果启用（默认），组合框会在框架内自行绘制，否则组合框会自动绘制而没有任何框架。


访问函数；

```cpp
bool hasFrame() const
void setFrame(bool)
```




8、iconSize : QSize



该属性保存组合框中显示的图标的大小。

除非明确设置，否则返回当前样式的默认值。 这个尺寸是图标可以具有的最大尺寸; 较小尺寸的图标不会放大。

访问函数；

```cpp
QSize iconSize() const
void setIconSize(const QSize &size)
```


9、insertPolicy : InsertPolicy

此属性包含用于确定用户插入的项目应该出现在组合框中的策略。

默认值是InsertAtBottom，表示新项目将出现在项目列表的底部。


访问函数；

```cpp
InsertPolicy insertPolicy() const
void setInsertPolicy(InsertPolicy policy)
```




10、maxCount : int



该属性拥有组合框中允许的最大项目数。

注意：如果您将最大数量设置为小于组合框中当前数量的项目，则额外项目将被截断。 如果您在组合框上设置了外部模型，这也适用。

默认情况下，此属性的值来自可用的最高有符号整数（通常为2147483647）。

访问函数；

```cpp
int maxCount() const
void setMaxCount(int max)
```






11、maxVisibleItems : int



此属性在组合框的屏幕上保持允许的最大尺寸，以项目计量。

默认情况下，此属性的值为10。

注意：对于QStyle :: SH_ComboBox_Popup（例如Mac风格或Gtk +风格）返回true的样式中的不可编辑组合框，该属性将被忽略。

访问函数；

```cpp
int maxVisibleItems() const
void setMaxVisibleItems(int maxItems)
```




12、minimumContentsLength : int



该属性保留了适合组合框的最少字符数。

默认值是0。

如果此属性设置为正值，则minimumSizeHint（）和sizeHint（）会将其考虑在内。

访问函数；

```cpp
int minimumContentsLength() const
void setMinimumContentsLength(int characters)
```



13、modelColumn : int



该属性拥有可见模型中的列。

如果在填充组合框之前设置，则弹出视图不会受到影响，并会显示第一列（使用此属性的默认值）。

默认情况下，此属性的值为0。

访问函数；

```cpp
int modelColumn() const
void setModelColumn(int visibleColumn)
```



14、sizeAdjustPolicy : SizeAdjustPolicy

此属性包含描述内容更改时组合框大小如何更改的策略。

默认值是AdjustToContentsOnFirstShow。


访问函数；

```cpp
SizeAdjustPolicy sizeAdjustPolicy() const
void setSizeAdjustPolicy(SizeAdjustPolicy policy
```







三、成员函数

1、QComboBox::QComboBox(QWidget *parent = Q_NULLPTR)

使用默认模型QStandardItemModel构造与给定父级的组合框。





2、QComboBox::~QComboBox()

摧毁组合框。





3、[signal] void QComboBox::activated(int index)

该信号在用户选择组合框中的项目时发送。 该项目的索引已通过。 请注意，即使选择没有改变，也会发送此信号。 如果您需要知道选择实际更改的时间，请使用signal currentIndexChanged（）。

注意：该类中的信号被激活。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
      [=](int index){ /* ... */ });
```



4、[signal] void QComboBox::activated(const QString &text)

该信号在用户选择组合框中的项目时发送。 该项目的文本已通过。 请注意，即使选择没有改变，也会发送此信号。 如果您需要知道选择实际更改的时间，请使用signal currentIndexChanged（）。

注意：该类中的信号被激活。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(comboBox, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated),
      [=](const QString &text){ /* ... */ });
```




5、void QComboBox::addItem(const QString &text, const QVariant &userData = QVariant())

使用给定的文本向组合框添加一个项目，并包含指定的userData（存储在Qt :: UserRole中）。 该项目被追加到现有项目的列表中。





6、void QComboBox::addItem(const QIcon &icon, const QString &text, const QVariant &userData = QVariant())

使用给定的图标和文本向组合框添加一个项目，并且包含指定的userData（存储在Qt :: UserRole中）。 该项目被追加到现有项目的列表中。





7、void QComboBox::addItems(const QStringList &texts)

将给定文本中的每个字符串添加到组合框中。 每个项目依次附加到现有项目列表中。





8、[virtual protected] void QComboBox::changeEvent(QEvent *e)

从QWidget :: changeEvent（）重新实现。





9、[slot] void QComboBox::clear()

清除组合框，删除所有项目。

注意：如果您在组合框上设置了外部模型，调用此功能时该模型仍将被清除。





10、[slot] void QComboBox::clearEditText()

清除组合框中用于编辑的行编辑内容。





11、QCompleter *QComboBox::completer() const

返回用于自动完成组合框文本输入的完成器。

这个函数是在Qt 4.2中引入的。





12、[virtual protected] void QComboBox::contextMenuEvent(QContextMenuEvent *e)

从QWidget :: contextMenuEvent（）重新实现。





13、[signal] void QComboBox::currentIndexChanged(int index)

只要组合框中的currentIndex通过用户交互或以编程方式更改，就会发送此信号。 如果组合框变空或currentIndex被重置，则传递项目的索引或-1。

注意：此类中的信号currentIndexChanged被重载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
      [=](int index){ /* ... */ });
```

这个函数是在Qt 4.1中引入的。




14、[signal] void QComboBox::currentIndexChanged(const QString &text)

只要组合框中的currentIndex通过用户交互或以编程方式更改，就会发送此信号。 该项目的文本已通过。

注意：此类中的信号currentIndexChanged被重载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(comboBox, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
      [=](const QString &text){ /* ... */ });
```

这个函数是在Qt 4.1中引入的。




15、[signal] void QComboBox::currentTextChanged(const QString &text)

每当currentText改变时都会发送此信号。 新值作为文本传递。

这个函数是在Qt 5.0中引入的。





16、[signal] void QComboBox::editTextChanged(const QString &text)

当组合框的行编辑小部件中的文本发生改变时，将发出此信号。 新文本由文本指定。





17、[virtual] bool QComboBox::event(QEvent *event)

从QObject :: event（）重新实现。





18、int QComboBox::findData(const QVariant &data, int role = Qt::UserRole, Qt::MatchFlags flags = static_cast<Qt::MatchFlags> ( Qt::MatchExactly | Qt::MatchCaseSensitive )) const

返回包含给定角色给定数据的项目的索引; 否则返回-1。

标志指定如何搜索组合框中的项目。





19、int QComboBox::findText(const QString &text, Qt::MatchFlags flags = static_cast<Qt::MatchFlags> ( Qt::MatchExactly | Qt::MatchCaseSensitive )) const

返回包含给定文本的项目的索引; 否则返回-1。

标志指定如何搜索组合框中的项目。





20、[virtual protected] void QComboBox::focusInEvent(QFocusEvent *e)

从QWidget :: focusInEvent（）重新实现。





21、[virtual protected] void QComboBox::focusOutEvent(QFocusEvent *e)

从QWidget :: focusOutEvent（）重新实现。





22、[virtual protected] void QComboBox::hideEvent(QHideEvent *e)

从QWidget :: hideEvent（）重新实现。





23、[virtual] void QComboBox::hidePopup()

隐藏组合框中的项目列表（如果当前可见）并重置内部状态，以便如果自定义弹出窗口显示在重新实现的showPopup（）中，则还需要重新实现hidePopup（）函数以隐藏您的 自定义弹出窗口并调用基类实现以在隐藏自定义弹出窗口小部件时重置内部状态。





24、[signal] void QComboBox::highlighted(int index)

该信号在用户突出显示组合框弹出列表中的项目时发送。 该项目的索引已通过。

注意：此类中突出显示的信号超载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::highlighted),
      [=](int index){ /* ... */ });
```




25、[signal] void QComboBox::highlighted(const QString &text)

该信号在用户突出显示组合框弹出列表中的项目时发送。 该项目的文本已通过。

注意：此类中突出显示的信号超载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(comboBox, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::highlighted),
      [=](const QString &text){ /* ... */ });
```




26、[protected] void QComboBox::initStyleOption(QStyleOptionComboBox *option) const

使用此QComboBox中的值初始化选项。 这个方法在需要QStyleOptionComboBox的子类中很有用，但不想自己填写所有的信息。





27、[virtual protected] void QComboBox::inputMethodEvent(QInputMethodEvent *e)

从QWidget :: inputMethodEvent（）重新实现。





28、[virtual] QVariant QComboBox::inputMethodQuery(Qt::InputMethodQuery query) const

从QWidget :: inputMethodQuery（）重新实现。





29、void QComboBox::insertItem(int index, const QString &text, const QVariant &userData = QVariant())

将文本和userData（存储在Qt :: UserRole中）插入给定索引处的组合框中。

如果索引等于或高于项目总数，则将新项目附加到现有项目的列表中。 如果索引为零或为负数，则新项目将预先添加到现有项目的列表中。





30、void QComboBox::insertItem(int index, const QIcon &icon, const QString &text, const QVariant &userData = QVariant())

将图标，文本和userData（存储在Qt :: UserRole中）插入给定索引处的组合框中。

如果索引等于或高于项目总数，则将新项目附加到现有项目的列表中。 如果索引为零或为负数，则新项目将预先添加到现有项目的列表中。





31、void QComboBox::insertItems(int index, const QStringList &list)

将列表中的字符串作为单独项插入组合框，从指定的索引处开始。

如果索引等于或高于项目总数，则将新项目附加到现有项目列表中。 如果索引为零或为负数，则新项目将预先添加到现有项目的列表中。





32、void QComboBox::insertSeparator(int index)



在给定索引处的组合框中插入一个分隔项。

如果索引等于或高于项目总数，则将新项目附加到现有项目的列表中。 如果索引为零或为负数，则新项目将预先添加到现有项目的列表中。

这个函数是在Qt 4.4中引入的。




33、QVariant QComboBox::itemData(int index, int role = Qt::UserRole) const

返回组合框中给定索引中给定角色的数据，如果此角色没有数据，则返回QVariant :: Invalid。





34、QAbstractItemDelegate *QComboBox::itemDelegate() const

返回弹出列表视图使用的项目委托。





35、QIcon QComboBox::itemIcon(int index) const

返回组合框中给定索引的图标。





36、QString QComboBox::itemText(int index) const

返回组合框中给定索引的文本。





37、[virtual protected] void QComboBox::keyPressEvent(QKeyEvent *e)

从QWidget :: keyPressEvent（）重新实现。





38、[virtual protected] void QComboBox::keyReleaseEvent(QKeyEvent *e)

从QWidget :: keyReleaseEvent（）重新实现。





39、QLineEdit *QComboBox::lineEdit() const

返回用于编辑组合框中项目的行编辑，如果没有行编辑，则返回0。

只有可编辑的组合框才有行编辑。





40、[virtual] QSize QComboBox::minimumSizeHint() const

从QWidget :: minimumSizeHint（）重新实现。





41、QAbstractItemModel *QComboBox::model() const

返回组合框使用的模型。





42、[virtual protected] void QComboBox::mousePressEvent(QMouseEvent *e)

从QWidget :: mousePressEvent（）重新实现。





43、[virtual protected] void QComboBox::mouseReleaseEvent(QMouseEvent *e)

从QWidget :: mouseReleaseEvent（）重新实现。





44、[virtual protected] void QComboBox::paintEvent(QPaintEvent *e)

从QWidget :: paintEvent（）重新实现。





45、void QComboBox::removeItem(int index)

索引被删除，这将更新当前索引。

如果索引超出范围，则此函数不执行任何操作。





46、[virtual protected] void QComboBox::resizeEvent(QResizeEvent *e)

从QWidget :: resizeEvent（）重新实现。





47、QModelIndex QComboBox::rootModelIndex() const

返回组合框中项目的根模型项目索引。





48、void QComboBox::setCompleter(QCompleter *completer)



将完成者设置为使用，而不是当前完成者。 如果完成者为0，则自动完成功能被禁用。

默认情况下，对于可编辑组合框，会自动创建执行不区分大小写内联完成的QCompleter。

这个函数是在Qt 4.2中引入的。




49、[slot] void QComboBox::setEditText(const QString &text)

设置组合框文本编辑中的文本。





50、void QComboBox::setItemData(int index, const QVariant &value, int role = Qt::UserRole)

将组合框中给定索引上项目的数据角色设置为指定值。





51、void QComboBox::setItemDelegate(QAbstractItemDelegate *delegate)

为弹出列表视图设置项目代理。 组合框取得了委托的所有权。

警告：您不应该在组合框，小部件映射器或视图之间共享委托的同一个实例。 这样做会导致不正确或不直观的编辑行为，因为连接到给定委托的每个视图都可能会收到closeEditor（）信号，并尝试访问，修改或关闭已关闭的编辑器。





52、void QComboBox::setItemIcon(int index, const QIcon &icon)

设置组合框中给定索引上项目的图标。





53、void QComboBox::setItemText(int index, const QString &text)

设置组合框中给定索引上项目的文本。





54、void QComboBox::setLineEdit(QLineEdit *edit)

将行编辑设置为使用而不是当前行编辑小部件。

组合框取得了线条编辑的所有权。





55、void QComboBox::setModel(QAbstractItemModel *model)

将模型设置为模型。 模型不能为0.如果要清除模型的内容，请调用clear（）。





56、void QComboBox::setRootModelIndex(const QModelIndex &index)

为组合框中的项目设置根模型项目索引。





57、void QComboBox::setValidator(const QValidator *validator)

将验证程序设置为使用而不是当前验证程序。





58、void QComboBox::setView(QAbstractItemView *itemView)

将组合框弹出窗口中使用的视图设置为给定的itemView。 组合框取得了该视图的所有权。

注意：如果要使用便利视图（如QListWidget，QTableWidget或QTreeWidget），请确保在调用此函数之前使用便利小部件模型在组合框上调用setModel（）。





59、[virtual protected] void QComboBox::showEvent(QShowEvent *e)

从QWidget :: showEvent（）重新实现。





60、[virtual] void QComboBox::showPopup()

显示组合框中的项目列表。 如果列表为空，那么将显示no项目。

如果您重新实现此功能以显示自定义弹出窗口，请确保调用hidePopup（）来重置内部状态。





61、[virtual] QSize QComboBox::sizeHint() const

显示组合框中的项目列表。 如果列表为空，那么将显示no项目。

如果您重新实现此功能以显示自定义弹出窗口，请确保调用hidePopup（）来重置内部状态。





62、[virtual] QSize QComboBox::sizeHint() const

从QWidget :: sizeHint（）重新实现。

此实现缓存大小提示以避免内容动态更改时调整大小。 要使缓存的值无效，请更改sizeAdjustPolicy。





63、const QValidator *QComboBox::validator() const

返回用于限制组合框文本输入的验证程序。





64、QAbstractItemView *QComboBox::view() const

返回用于组合框弹出窗口的列表视图。





65、[virtual protected] void QComboBox::wheelEvent(QWheelEvent *e)

从QWidget :: wheelEvent（）重新实现。




