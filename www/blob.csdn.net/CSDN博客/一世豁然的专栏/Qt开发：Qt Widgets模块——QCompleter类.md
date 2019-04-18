# Qt开发：Qt Widgets模块——QCompleter类 - 一世豁然的专栏 - CSDN博客





2018年05月24日 21:37:35[一世豁然](https://me.csdn.net/Explorer_day)阅读数：448








QCompleter类根据项目模型提供完成项。



头文件：#include <QCommonStyle> 

qmake：QT += widgets

继承：QObject

您可以使用QCompleter在任何Qt小部件中提供自动完成功能，例如QLineEdit和QComboBox。 当用户开始输入单词时，QCompleter根据单词列表提出了完成单词的可能方法。 单词列表是作为QAbstractItemModel提供的。 （对于简单的应用程序，单词列表是静态的，您可以将QStringList传递给QCompleter的构造函数。）





1、基础用法

QCompleter通常用于QLineEdit或QComboBox。 例如，以下是如何从QLineEdit中的简单单词列表中提供自动完成：


```cpp
QStringList wordList;
  wordList << "alpha" << "omega" << "omicron" << "zeta";

  QLineEdit *lineEdit = new QLineEdit(this);

  QCompleter *completer = new QCompleter(wordList, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  lineEdit->setCompleter(completer);
```

QFileSystemModel可用于提供文件名的自动完成。 例如：

```cpp
QCompleter *completer = new QCompleter(this);
  completer->setModel(new QDirModel(completer));
  lineEdit->setCompleter(completer);
```

要设置QCompleter应该运行的模型，请调用setModel（）。 默认情况下，QCompleter会尝试将完成前缀（即用户开始键入的单词）与模型中列0中存储的Qt :: EditRole数据进行匹配。 这可以使用setCompletionRole（），setCompletionColumn（）和setCaseSensitivity（）来更改。

如果模型在用于完成的列和角色上排序，则可以使用QCompleter :: CaseSensitivelySortedModel或QCompleter :: CaseInsensitivelySortedModel作为参数来调用setModelSorting（）。 在大型模型中，这可以显着提高性能，因为QCompleter可以使用二分搜索而不是线性搜索。 二进制搜索仅适用于filterMode为Qt :: MatchStartsWith的情况。


该模型可以是列表模型，表格模型或树模型。 有关树模型的完成稍微涉及更多，并在下面的“处理树模型”部分中进行介绍。

completionMode（）确定用于向用户提供完成的模式。





2、迭代完成

要检索单个候选字符串，请使用需要完成的文本调用setCompletionPrefix（）并调用currentCompletion（）。 您可以遍历完成列表，如下所示：


```cpp
for (int i = 0; completer->setCurrentRow(i); i++)
      qDebug() << completer->currentCompletion() << " is match number " << i;
```

completionCount（）返回当前前缀的完成总数。 如果可能，应尽可能避免completionCount（），因为它需要扫描整个模型。




3、完成模型

completionModel（）返回一个列表模型，该列表模型包含当前完成前缀的所有可能完成项，并按它们在模型中的显示顺序排列。 此模型可用于在自定义视图中显示当前完成。 调用setCompletionPrefix（）会自动刷新完成模型。





4、处理树模型

假设任何项目（或子项目或子子项目）都可以通过指定项目的路径明确表示为字符串，QCompleter可以在树模型中查找完成。 然后完成一次执行一个级别。


我们以用户输入文件系统路径为例。 该模型是一个（分层）QFileSystemModel。 完成发生在路径中的每个元素。 例如，如果当前文本是C：\ Wind，QCompleter可能会建议Windows完成当前路径元素。 同样，如果当前文本是C：\ Windows \ Sy，QCompleter可能会提示系统。


为了完成这种工作，QCompleter需要能够将路径分割成每个级别匹配的字符串列表。 对于C：\ Windows \ Sy，它需要分为“C：”，“Windows”和“Sy”。 splitPath（）的默认实现，如果模型是QFileSystemModel，则使用QDir :: separator（）分割completionPrefix。


为了提供完整性，QCompleter需要知道索引的路径。 这由pathFromIndex（）提供。 pathFromIndex（）的默认实现返回列表模型的编辑角色的数据，如果模式是QFileSystemModel，则返回绝对文件路径。








一、成员类型

1、enum QCompleter::CompletionMode

此枚举指定如何完成提供给用户。



|内容|值|描述|
|----|----|----|
|QCompleter::PopupCompletion|0|当前完成显示在一个弹出窗口中。|
|QCompleter::InlineCompletion|2|完成内联显示（作为选定的文本）。|
|QCompleter::UnfilteredPopupCompletion|1|所有可能的完成都显示在弹出窗口中，最有可能的建议显示为当前。|

2、enum QCompleter::ModelSorting

此枚举指定模型中项目的排序方式。



|内容|值|描述|
|----|----|----|
|QCompleter::UnsortedModel|0|该模型是未排序的。|
|QCompleter::CaseSensitivelySortedModel|1|该模型是大小写敏感的。|
|QCompleter::CaseInsensitivelySortedModel|2|模型不区分大小写。|




二、属性

1、caseSensitivity : Qt::CaseSensitivity

该属性保持匹配的大小写敏感性。

默认值是Qt :: CaseSensitive。


访问函数；

```cpp
Qt::CaseSensitivity 
caseSensitivity() const
void 
setCaseSensitivity(Qt::CaseSensitivity caseSensitivity)
```



2、completionColumn : int



该属性拥有完成搜索模型中的列。

如果popup（）是QListView，它会自动设置为显示此列。

默认情况下，匹配列为0。

访问函数；

```cpp
int 
completionColumn() const
void 
setCompletionColumn(int column)
```



3、completionMode : CompletionMode

该属性保存了如何完成提供给用户。

默认值是QCompleter :: PopupCompletion。


访问函数；

```cpp
CompletionMode 
completionMode() const
void 
setCompletionMode(CompletionMode mode)
```



4、completionPrefix : QString

该属性包含用于提供完成的完成前缀。

completionModel（）被更新以反映可能的前缀匹配列表。


访问函数；

```cpp
QString 
completionPrefix() const
void 
setCompletionPrefix(const QString &prefix)
```



5、completionRole : int

此属性包含要用于查询项目内容以匹配的项目角色。

默认角色是Qt :: EditRole。


访问函数；

```cpp
int 
completionRole() const
void 
setCompletionRole(int role)
```



6、filterMode : Qt::MatchFlags



该属性保存如何执行过滤。

如果filterMode设置为Qt :: MatchStartsWith，则只会显示以类型化字符开头的条目。 Qt :: MatchContains将显示包含类型字符的条目，并且Qt :: MatchEnds以以类型字符结尾的条目显示。

目前，只有这三种模式得以实施。 将filterMode设置为任何其他Qt :: MatchFlag将发出警告，并且不会执行任何操作。

默认模式是Qt :: MatchStartsWith。

这个属性是在Qt 5.2中引入的。

访问函数；

```cpp
Qt::MatchFlags 
filterMode() const
void 
setFilterMode(Qt::MatchFlags filterMode)
```



7、maxVisibleItems : int



该属性在完成者的屏幕上保持允许的最大尺寸，以项目计量。

默认情况下，此属性的值为7。

这个属性是在Qt 4.6中引入的。

访问函数；

```cpp
int 
maxVisibleItems() const
void 
setMaxVisibleItems(int maxItems)
```



8、modelSorting : ModelSorting



该属性保持模型排序的方式。

默认情况下，不提供有关提供完成的模型中项目的顺序的假设。

如果completionColumn（）和completionRole（）的模型数据按升序排序，则可以将此属性设置为CaseSensitivelySortedModel或CaseInsensitivelySortedModel。 在大型模型中，这会导致显着的性能改进，因为Completer对象可以使用二分搜索算法而不是线性搜索算法。

通过检查模型的内容动态确定模型的排序顺序（即升序或降序）。

注意：当完成者的caseSensitivity与模型在分类时使用的大小写敏感性不同时，上述性能改进不会发生。

访问函数；

```cpp
ModelSorting 
modelSorting() const
void 
setModelSorting(ModelSorting sorting
```



9、wrapAround : bool



此属性在导航项目时包含完成项。

默认值是true。

这个属性是在Qt 4.3中引入的。

访问函数；

```cpp
bool 
wrapAround() const
void 
setWrapAround(bool wrap)
```






三、成员函数

1、QCompleter::QCompleter(QObject *parent = Q_NULLPTR)

用给定的父对象构造一个完成者对象。





2、QCompleter::QCompleter(QAbstractItemModel *model, QObject *parent = Q_NULLPTR)

用给定的父对象构造一个完成对象，该对象提供来自指定模型的完成对象。





3、QCompleter::QCompleter(const QStringList &list, QObject *parent = Q_NULLPTR)

用给定的父对象构造一个QCompleter对象，该对象使用指定的列表作为可能的完成来源。





4、QCompleter::~QCompleter()

销毁完成者对象。





5、[signal] void QCompleter::activated(const QString &text)

当用户激活弹出窗口（）中的项目时（通过点击或按下返回键），发送此信号。 该项目的文本给出。

注意：该类中的信号被激活。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(completer, static_cast<void(QCompleter::*)(const QString &)>(&QCompleter::activated),
      [=](const QString &text){ /* ... */ });
```




6、[signal] void QCompleter::activated(const QModelIndex &index)

当用户激活popup（）中的项目时发送此信号。 （通过点击或按回车）。 给出了completionModel（）中的项目索引。

注意：该类中的信号被激活。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(completer, static_cast<void(QCompleter::*)(const QModelIndex &)>(&QCompleter::activated),
      [=](const QModelIndex &index){ /* ... */ });
```




7、[slot] void QCompleter::complete(const QRect &rect = QRect())

对于QCompleter :: PopupCompletion和QCompletion :: UnfilteredPopupCompletion模式，调用此函数将显示弹出窗口，显示当前完成情况。 默认情况下，如果未指定rect，则弹出窗口显示在窗口小部件（）的底部。 如果指定了rect，则弹出窗口显示在矩形的左边缘上。

对于QCompleter :: InlineCompletion模式，突出显示的（）信号在当前完成时触发。





8、int QCompleter::completionCount() const

返回当前前缀的完成数目。 对于包含大量物品的未排序模型，这可能很昂贵。 使用setCurrentRow（）和currentCompletion（）遍历所有完成。





9、QAbstractItemModel *QCompleter::completionModel() const

返回完成模型。 完成模型是一个只读列表模型，它包含当前完成前缀的所有可能匹配项。 完成模型会自动更新以反映当前完成情况。

注意：这个函数的返回值被定义为一个QAbstractItemModel纯粹为了一般性。 这种实际返回的模型是QAbstractProxyModel子类的一个实例。





10、QString QCompleter::currentCompletion() const

返回当前的完成字符串。 这包括completionPrefix。 与setCurrentRow（）一起使用时，可以用它遍历所有匹配。





11、QModelIndex QCompleter::currentIndex() const

返回completionModel（）中当前完成的模型索引。





12、int QCompleter::currentRow() const

返回当前行。





13、[virtual protected] bool QCompleter::event(QEvent *ev)

从QObject :: event（）重新实现。





14、[virtual protected] bool QCompleter::eventFilter(QObject *o, QEvent *e)

从QObject :: eventFilter（）重新实现。





15、[signal] void QCompleter::highlighted(const QString &text)

当用户突出显示弹出窗口（）中的项目时发送此信号。 如果使用completionMode（）设置为QCompleter :: InlineCompletion来调用complete（），则它也会被发送。 该项目的文本给出。

注意：此类中突出显示的信号超载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(completer, static_cast<void(QCompleter::*)(const QString &)>(&QCompleter::highlighted),
      [=](const QString &text){ /* ... */ });
```




16、[signal] void QCompleter::highlighted(const QModelIndex &index)

当用户突出显示弹出窗口（）中的项目时发送此信号。 如果使用completionMode（）设置为QCompleter :: InlineCompletion来调用complete（），则它也会被发送。 给出了completionModel（）中的项目索引。

注意：此类中突出显示的信号超载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(completer, static_cast<void(QCompleter::*)(const QModelIndex &)>(&QCompleter::highlighted),
      [=](const QModelIndex &index){ /* ... */ });
```




11、QAbstractItemModel *QCompleter::model() const

返回提供完成字符串的模型。





12、[virtual] QString QCompleter::pathFromIndex(const QModelIndex &index) const

返回给定索引的路径。 完成者对象使用它从基础模型中获取完成文本。

默认实现返回列表模型的项目的编辑角色。 如果模型是QFileSystemModel，它将返回绝对文件路径。





13、QAbstractItemView *QCompleter::popup() const

返回用于显示完成的弹出窗口。





14、bool QCompleter::setCurrentRow(int row)

将当前行设置为指定的行。 如果成功则返回true; 否则返回false。

这个函数可以和currentCompletion（）一起用来遍历所有可能的完成。





15、void QCompleter::setModel(QAbstractItemModel *model)

设置为模型提供完成的模型。 该模型可以是列表模型或树模型。 如果一个模型已经被预先设置好了，并且它有QCompleter作为它的父项，它将被删除。

为方便起见，如果model是QFileSystemModel，QCompleter会将其caseSensitivity切换为Windows上的Qt :: CaseInsensitive和其他平台上的Qt :: CaseSensitive。





16、void QCompleter::setPopup(QAbstractItemView *popup)



将用于显示完成的弹出窗口设置为弹出窗口。 QCompleter拥有该视图的所有权。

当completionMode（）设置为QCompleter :: PopupCompletion或QCompleter :: UnfilteredPopupCompletion时，将自动创建QListView。 默认弹出窗口显示completionColumn（）。

确保在修改视图设置之前调用此函数。 这是必需的，因为视图的属性可能需要在视图上设置模型（例如，隐藏视图中的列需要在视图上设置模型）。




17、void QCompleter::setWidget(QWidget *widget)

设置完成提供给窗口小部件的窗口小部件。 当使用QLineEdit :: setCompleter（）或QComboBox使用QComboBox :: setCompleter（）在QLineEdit上设置QCompleter时，会自动调用此函数。 在为自定义小部件提供完成时，小部件需要显式设置。





18、[virtual] QStringList QCompleter::splitPath(const QString &path) const



将给定的路径分割成用于匹配模型（）中的每个级别的字符串。

当sourceModel（）是QFileSystemModel时，splitPath（）的默认实现基于QDir :: separator（）分割文件系统路径。

与列表模型一起使用时，返回列表中的第一项用于匹配。




19、QWidget *QCompleter::widget() const

返回完成者对象提供完成的小部件。




