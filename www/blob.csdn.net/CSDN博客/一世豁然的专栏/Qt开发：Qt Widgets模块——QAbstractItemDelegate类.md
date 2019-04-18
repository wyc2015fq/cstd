# Qt开发：Qt Widgets模块——QAbstractItemDelegate类 - 一世豁然的专栏 - CSDN博客





2018年05月16日 09:27:48[一世豁然](https://me.csdn.net/Explorer_day)阅读数：456








QAbstractItemDelegate类用于显示和编辑模型中的数据项。

头文件：#include <QAbstractItemDelegate> 

qmake：QT += widgets

继承：QObject

被继承：QItemDelegate和QStyledItemDelegate

QAbstractItemDelegate为模型/视图体系结构中的委托提供接口和通用功能。 代表在视图中显示单个项目，并处理模型数据的编辑。


QAbstractItemDelegate类是模型/视图类之一，是Qt的模型/视图框架的一部分。


要以自定义的方式呈现项目，您必须实现paint（）和sizeHint（）。 QItemDelegate类为这些函数提供默认实现; 如果您不需要自定义渲染，请改为将该类继承。


我们给出一个绘制项目进度条的例子; 在我们的案例中是一个包管理程序。


![](https://img-blog.csdn.net/2018051608495999)


我们创建了从QStyledItemDelegate继承的WidgetDelegate类。 我们在paint（）函数中执行绘图：


```cpp
void WidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
  {
      if (index.column() == 1) {
          int progress = index.data().toInt();

          QStyleOptionProgressBar progressBarOption;
          progressBarOption.rect = option.rect;
          progressBarOption.minimum = 0;
          progressBarOption.maximum = 100;
          progressBarOption.progress = progress;
          progressBarOption.text = QString::number(progress) + "%";
          progressBarOption.textVisible = true;

          QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                             &progressBarOption, painter);
      } else
          QStyledItemDelegate::paint(painter, option, index);
```

请注意，我们使用QStyleOptionProgressBar并初始化其成员。 然后我们可以使用当前的QStyle来绘制它。

为了提供自定义编辑，可以使用两种方法。 第一种方法是创建一个编辑器小部件，并将其直接显示在该项目的顶部。 为此，您必须重新实现createEditor（）以提供编辑器小部件，setEditorData（）以使用模型中的数据填充编辑器，以及setModelData（），以便代表可以使用编辑器中的数据更新模型。


第二种方法是通过重新实现editorEvent（）直接处理用户事件。








一、成员类型

1、enum QAbstractItemDelegate::EndEditHint

此枚举描述委托人可以为模型和视图组件提供的不同提示，以使模型中的编辑数据成为用户的舒适体验。




|内容|值|描述|
|----|----|----|
|QAbstractItemDelegate::NoHint|0|没有推荐的操作要执行。|
这些提示让代表影响了视图的行为：


|内容|值|描述|
|----|----|----|
|QAbstractItemDelegate::EditNextItem|1|视图应该使用委托在视图中的下一个项目上打开编辑器。|
|QAbstractItemDelegate::EditPreviousItem|2|视图应该使用委托在视图中的前一个项目上打开编辑器。|
请注意，自定义视图可能不同地解释下一个和上一个的概念。
当使用缓存数据的模型时，以下提示最为有用，例如那些为了提高性能或节省网络带宽而在本地操纵数据的提示。



|内容|值|描述|
|----|----|----|
|QAbstractItemDelegate::SubmitModelCache|3|如果模型缓存数据，它应该将缓存的数据写入底层数据存储。|
|QAbstractItemDelegate::RevertModelCache|4|如果模型缓存数据，则应放弃缓存的数据并将其替换为底层数据存储中的数据。|
尽管模型和视图应以适当的方式响应这些提示，但如果不相关，自定义组件可能会忽略任何或所有这些提示。






二、成员函数

1、QAbstractItemDelegate::QAbstractItemDelegate(QObject *parent = Q_NULLPTR)

用给定的父项创建一个新的抽象项目委托。





2、[virtual] QAbstractItemDelegate::~QAbstractItemDelegate()

销毁抽象项目委托。





3、[signal] void QAbstractItemDelegate::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint = NoHint)

当用户使用指定编辑器完成编辑项目时发出此信号。


该提示为委托人提供了一种方式，可以在编辑完成后影响模型和视图的行为方式。 它向这些组件指示接下来应该执行什么操作以为用户提供舒适的编辑体验。 例如，如果指定了EditNextItem，视图应使用委托在模型中的下一个项目上打开编辑器。





4、[signal] void QAbstractItemDelegate::commitData(QWidget *editor)

编辑器小部件编辑完数据后，必须发出此信号，并且要将其写回模型中。





5、[virtual] QWidget *QAbstractItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const

返回用于编辑具有给定索引的数据项的编辑器。 请注意，该索引包含有关正在使用的模型的信息。 编辑器的父窗口小部件由父项指定，而项目选项按选项指定。


基本实现返回0.如果你想自定义编辑，你需要重新实现这个功能。


返回的编辑器小部件应该有Qt :: StrongFocus; 否则，小部件接收到的QMouseEvents将传播到视图。 除非编辑器绘制自己的背景（例如，使用setAutoFillBackground（）），否则视图的背景将发光。





6、[virtual] void QAbstractItemDelegate::destroyEditor(QWidget *editor, const QModelIndex &index) const

当编辑器不再需要编辑具有给定索引的数据项并且应该被销毁时调用。 默认行为是在编辑器上调用deleteLater。 例如可能 通过重新实现此功能来避免此删除。


此函数在Qt5.0中引入。




7、[virtual] bool QAbstractItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)

开始编辑项目时，将使用触发编辑的事件，模型，项目的索引以及用于渲染项目的选项来调用此函数。


即使鼠标事件没有开始编辑该项目，它也会发送到editorEvent（）。 例如，如果您希望在项目上按下鼠标右键时打开上下文菜单，这可能很有用。


基本实现返回false（表示它没有处理事件）。





8、[virtual] bool QAbstractItemDelegate::helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)

无论何时发生帮助事件，都会使用事件视图选项和与发生事件的项目对应的索引调用此函数。

如果委托可以处理事件，则返回true; 否则返回false。 返回值为true表示使用索引获得的数据具有所需的角色。

对于成功处理的QEvent :: ToolTip和QEvent :: WhatsThis事件，可能会根据用户的系统配置显示相关的弹出窗口。


此函数在Qt4.3中引入。





9、[pure virtual] void QAbstractItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const

如果您想提供自定义渲染，则必须重新实现此纯粹的抽象函数。 使用画家和风格选项来呈现项目索引指定的项目。


如果你重新实现这个，你还必须重新实现sizeHint（）。





10、[virtual] void QAbstractItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const

将给定编辑器的内容设置为给定索引处项目的数据。 请注意，该索引包含有关正在使用的模型的信息。


基本实现什么都不做。 如果你想自定义编辑，你需要重新实现这个功能。





11、[virtual] void QAbstractItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const

将模型中给定索引处项目的数据设置为给定编辑器的内容。


基本实现什么都不做。 如果你想自定义编辑，你需要重新实现这个功能。





12、[pure virtual] QSize QAbstractItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const

如果您想提供自定义渲染，则必须重新实现此纯粹的抽象函数。 选项由选项指定，模型项按指定。


如果你重新实现这个，你还必须重新实现paint（）。





13、[signal] void QAbstractItemDelegate::sizeHintChanged(const QModelIndex &index)

当索引的sizeHint（）发生变化时，必须发出此信号。


视图自动连接到此信号并根据需要重新布置物品。


这个函数是在Qt 4.4中引入的。





14、[virtual] void QAbstractItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const

根据选项中指定的矩形更新具有给定索引的项目的编辑器的几何图形。 如果该项目具有内部布局，编辑器将相应地布局。 请注意，该索引包含有关正在使用的模型的信息。


基本实现什么都不做。 如果你想自定义编辑，你必须重新实现这个功能。




