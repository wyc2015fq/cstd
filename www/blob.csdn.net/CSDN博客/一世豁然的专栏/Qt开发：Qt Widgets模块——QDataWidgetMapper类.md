# Qt开发：Qt Widgets模块——QDataWidgetMapper类 - 一世豁然的专栏 - CSDN博客





2018年05月25日 16:38:46[一世豁然](https://me.csdn.net/Explorer_day)阅读数：521








QDataWidgetMapper类提供了数据模型的一部分与小部件之间的映射。


头文件：#include <QDataWidgetMapper> 

qmake：QT += widgets

继承：QObject

通过将QDataWidgetMapper映射到项目模型的各个部分，QDataWidgetMapper可用于创建数据感知小部件。 如果方向是水平的（默认），则区域是模型的一列，否则是一行。


每当当前索引发生变化时，每个小部件都会通过映射时指定的属性使用模型中的数据进行更新。 如果用户编辑小部件的内容，则使用相同的属性读取更改并写回模型。 默认情况下，每个小部件的用户属性用于在模型和小部件之间传输数据。 从Qt 4.3开始，一个额外的addMapping（）函数可以使用命名属性来代替默认的用户属性。


可以设置一个项目委托来支持自定义小部件。 默认情况下，使用QItemDelegate将模型与小部件进行同步。


让我们假设我们有一个名为model的项目模型，其中包含以下内容：

![](https://img-blog.csdn.net/20180525161119942)


以下代码将将模型的列映射到称为mySpinBox，myLineEdit和myCountryChooser的小部件：


```cpp
QDataWidgetMapper *mapper = new QDataWidgetMapper;
  mapper->setModel(model);
  mapper->addMapping(mySpinBox, 0);
  mapper->addMapping(myLineEdit, 1);
  mapper->addMapping(myCountryChooser, 2);
  mapper->toFirst();
```

呼叫toFirst（）后，mySpinBox显示值1，myLineEdit显示Qt Norway，myCountryChooser显示奥斯陆。 导航函数toFirst（），toNext（），toPrevious（），toLast（）和setCurrentIndex（）可用于在模型中导航并使用模型中的内容更新小部件。

setRootIndex（）函数可以将模型中的特定项目指定为根索引 - 此项目的子项将映射到用户界面中的相关小部件。


QDataWidgetMapper支持两个提交策略，AutoSubmit和ManualSubmit。 AutoSubmit会在当前窗口小部件失去焦点后立即更新模型，除非调用submit（），否则ManualSubmit不会更新模型。 当显示允许用户取消所有修改的对话框时，ManualSubmit非常有用。 此外，显示模型的其他视图在用户完成所有修改和提交之后才会更新。


请注意，QDataWidgetMapper会跟踪外部修改。 如果模型的内容在应用程序的另一个模块中更新，则窗口小部件也会更新。








一、成员类型

1、enum QDataWidgetMapper::SubmitPolicy

此枚举描述QDataWidgetMapper支持的可能的提交策略。



|内容|值|描述|
|----|----|----|
|QDataWidgetMapper::AutoSubmit|0|每当小部件失去焦点时，小部件的当前值将设置为项目模型。|
|QDataWidgetMapper::ManualSubmit|1|在调用submit（）之前，模型不会更新。|




二、属性

1、currentIndex : int

该属性包含当前行或列。

如果方向是水平的（缺省值），则使用来自索引处的行的数据填充小部件，否则使用来自索引处的列的数据填充小部件。


访问函数；

```cpp
int 
currentIndex() const
virtual void 
setCurrentIndex(int index)
```

通知信号；

```cpp
void 
currentIndexChanged(int index)
```



2、orientation : Qt::Orientation



该属性拥有模型的方向。

如果方向为Qt :: Horizontal（默认值），则小部件将映射到数据模型的列。 该小部件将填充来自其映射列的模型数据以及currentIndex（）指向的行。
为表格数据使用Qt :: Horizontal，如下所示：

![](https://img-blog.csdn.net/20180525162051608)


如果方向设置为Qt :: Vertical，则小部件将映射到一行。 调用setCurrentIndex（）将更改当前列。 该小部件将填充来自其映射行和currentIndex（）指向的列的模型数据。

使用Qt :: Vertical作为如下所示的表格数据：


![](https://img-blog.csdn.net/20180525162131392)


更改方向会清除所有现有的映射。


访问函数；

```cpp
Qt::Orientation orientation() const
void setOrientation(Qt::Orientation aOrientation)
```



3、submitPolicy : SubmitPolicy

该属性保存当前的提交策略。

更改当前提交策略会将所有小部件还原为模型中的当前数据。


访问函数；

```cpp
SubmitPolicy submitPolicy() const
void setSubmitPolicy(SubmitPolicy policy)
```







三、成员函数

1、QDataWidgetMapper::QDataWidgetMapper(QObject *parent = Q_NULLPTR)

用父对象parent构造一个新的QDataWidgetMapper。 默认情况下，方向是水平的，提交策略是AutoSubmit。





2、QDataWidgetMapper::~QDataWidgetMapper()

销毁对象。





3、void QDataWidgetMapper::addMapping(QWidget *widget, int section)

在小部件和模型的一部分之间添加映射。 该部分是模型中的一列，如果方向为水平（默认），否则为一行。

对于以下示例，我们假设一个模型myModel有两列：第一列包含组中的人员名称，第二列包含他们的年龄。 第一列被映射到QLineEdit nameLineEdit，第二列被映射到QSpinBox ageSpinBox：


```cpp
QDataWidgetMapper *mapper = new QDataWidgetMapper();
  mapper->setModel(myModel);
  mapper->addMapping(nameLineEdit, 0);
  mapper->addMapping(ageSpinBox, 1);
```
注意：

如果小部件已经映射到一个部分，旧的映射将被新的部分替换。

只允许部分和小部件之间的一对一映射。 无法将单个部分映射到多个小部件，也无法将单个小部件映射到多个部分。




4、void QDataWidgetMapper::addMapping(QWidget *widget, int section, const QByteArray &propertyName)

本质上与addMapping（）相同，但增加了指定要使用指定propertyName的属性的可能性。

这个函数是在Qt 4.3中引入的。





5、void QDataWidgetMapper::clearMapping()

清除所有映射。





6、[signal] void QDataWidgetMapper::currentIndexChanged(int index)

该信号在当前索引发生变化并且所有小部件都填充了新数据后发出。 指数是新的当前指数。

注意：属性currentIndex的通知程序信号。





7、QAbstractItemDelegate *QDataWidgetMapper::itemDelegate() const

返回当前的项目委托。





8、QByteArray QDataWidgetMapper::mappedPropertyName(QWidget *widget) const

返回将数据映射到给定小部件时使用的属性的名称。

这个函数是在Qt 4.3中引入的。





9、int QDataWidgetMapper::mappedSection(QWidget *widget) const

返回小部件映射到的部分，如果小部件未映射，则返回-1。





10、QWidget *QDataWidgetMapper::mappedWidgetAt(int section) const

返回在部分映射的小部件，如果没有小部件映射到该部分，则返回0。





11、QAbstractItemModel *QDataWidgetMapper::model() const

返回当前模型。





12、void QDataWidgetMapper::removeMapping(QWidget *widget)

删除给定小部件的映射。





13、[slot] void QDataWidgetMapper::revert()

使用模型的当前数据重新填充所有小部件。 所有未提交的更改都将丢失。





14、QModelIndex QDataWidgetMapper::rootIndex() const

返回当前的根索引。





15、[slot] void QDataWidgetMapper::setCurrentModelIndex(const QModelIndex &index)



如果方向为水平（默认），则将当前索引设置为索引的行，否则设置为索引的列。

在内部调用setCurrentIndex（）。 这个便捷槽可以连接到另一个视图选择模型的信号currentRowChanged（）或currentColumnChanged（）。

以下示例说明如何在每次选择名为myTableView的QTableView更改时使用新数据更新所有窗口小部件：

```cpp
QDataWidgetMapper *mapper = new QDataWidgetMapper();
  connect(myTableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
          mapper, SLOT(setCurrentModelIndex(QModelIndex)));
```




16、void QDataWidgetMapper::setItemDelegate(QAbstractItemDelegate *delegate)



将项目委托设置为委派。 使用QAbstractItemDelegate :: setEditorData（）和QAbstractItemDelegate :: setModelData（），委托将用于将数据从模型写入到窗口部件中，并从窗口部件写入模型。

委托人还决定何时应用数据以及何时更改编辑器，使用QAbstractItemDelegate :: commitData（）和QAbstractItemDelegate :: closeEditor（）。

警告：您不应该在小部件映射器或视图之间共享委托的同一个实例。 这样做会导致不正确或不直观的编辑行为，因为连接到给定委托的每个视图都可能会收到closeEditor（）信号，并尝试访问，修改或关闭已关闭的编辑器。




17、void QDataWidgetMapper::setModel(QAbstractItemModel *model)

将当前模型设置为模型。 如果设置了另一个模型，则清除该旧模型的所有映射。





18、void QDataWidgetMapper::setRootIndex(const QModelIndex &index)

将根项目设置为索引。 这可以用来显示树的分支。 传递无效的模型索引以显示最顶层的分支。





19、[slot] bool QDataWidgetMapper::submit()



提交从映射小部件到模型的所有更改。

对于每个映射节，项目委托从窗口小部件中读取当前值并将其设置在模型中。 最后，模型的submit（）方法被调用。

如果所有值都已提交，则返回true，否则返回false。

注意：对于数据库模型，可以使用QSqlQueryModel :: lastError（）来检索最后一个错误。




20、[slot] void QDataWidgetMapper::toFirst()

如果方向为水平（默认），则使用模型第一行的数据填充小部件，否则使用第一列的数据填充小部件。

这相当于调用setCurrentIndex（0）。





21、[slot] void QDataWidgetMapper::toLast()

如果方向为水平（默认），则使用模型最后一行的数据填充窗口小部件，否则使用最后一列的数据填充窗口小部件。

在内部调用setCurrentIndex（）。





22、[slot] void QDataWidgetMapper::toNext()

如果方向为水平（默认），则使用模型下一行的数据填充小部件，否则使用下一列的数据填充小部件。

在内部调用setCurrentIndex（）。 如果模型中没有下一行，则不做任何事情。





23、[slot] void QDataWidgetMapper::toPrevious()

如果方向为水平（默认），则使用模型的上一行中的数据填充窗口小部件，否则使用前一列的数据填充窗口小部件。

在内部调用setCurrentIndex（）。 如果模型中没有前一行，则不执行任何操作。




