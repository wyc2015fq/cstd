# Qt开发：Qt Widgets模块——QActionGroup类 - 一世豁然的专栏 - CSDN博客





2018年05月18日 08:08:21[一世豁然](https://me.csdn.net/Explorer_day)阅读数：493








QActionGroup类将动作组合在一起。



头文件：#include <QActionGroup>

qmake：QT += widgets

继承：QObject

在某些情况下，将QAction对象组合在一起很有用。 例如，如果您有“左对齐”操作，“右对齐”操作，“对齐”操作和“中心”操作，则任何时候这些操作中只有一个应该处于活动状态。 实现这一点的一个简单方法是将行动组合在一个行动组中。


下面是一个例子（来自Menus例子）：


```cpp
alignmentGroup = new QActionGroup(this);
  alignmentGroup->addAction(leftAlignAct);
  alignmentGroup->addAction(rightAlignAct);
  alignmentGroup->addAction(justifyAct);
  alignmentGroup->addAction(centerAct);
  leftAlignAct->setChecked(true);
```

在这里我们创建一个新的行动组。 由于默认情况下操作组是独占的，因此在任何时候都只检查组中的一个操作。

![](https://img-blog.csdn.net/20180518075722705)


QActionGroup在选择其中一个动作时发出触发（）信号。 操作组中的每个操作都像往常一样发出其触发的（）信号。

如上所述，一个行动组默认是排他性的; 它确保在任何时候只有一个可检查操作处于活动状态。 如果您想将可检查的操作分组而不排除它们，您可以通过调用setExclusive（false）来打开排他性。


可以使用addAction（）将操作添加到操作组中，但在创建操作时指定组通常更方便; 这可以确保操作是由父级自动创建的。 通过向组添加分隔符操作，可以在视觉上将操作彼此分开; 创建一个动作并使用QAction的setSeparator（）函数将其视为分隔符。 动作组通过QWidget :: addActions（）函数添加到窗口小部件。








一、属性

1、enabled : bool

该属性可以确定操作组是否已启用。

除非明确禁用，否则组中的每个操作都将被启用或禁用。


访问函数；

```cpp
bool isEnabled() const
void setEnabled(bool)
```




2、exclusive : bool

此属性可以确定操作组是否进行独占检查。

如果排他性为真，则行动组中只有一个可检查的行为可以随时处于活动状态。 如果用户在组中选择另一个可检查操作，则他们选择的操作变为活动状态，并且处于活动状态的操作变为非活动状态。


访问函数；

```cpp
bool isExclusive() const
void setExclusive(bool)
```




3、visible : bool

此属性可以确定操作组是否可见。

除非明确隐藏，否则动作组中的每个动作都将与该组的可见状态相匹配。


访问函数；

```cpp
bool isVisible() const
void setVisible(bool)
```







二、成员函数

1、QActionGroup::QActionGroup(QObject *parent)

构造父对象的操作组。

该操作组默认是独占的。 调用setExclusive（false）使操作组非独占。





2、QActionGroup::~QActionGroup()

摧毁行动集。





3、QList<QAction *> QActionGroup::actions() const

返回这些组操作的列表。 这可能是空的。





4、QAction *QActionGroup::addAction(QAction *action)

将操作添加到该组中，并将其返回。

通常情况下，通过以该组作为其父项创建一个操作来添加操作，所以通常不会使用此功能。





5、QAction *QActionGroup::addAction(const QString &text)

用文本创建并返回一个动作。 新创建的操作是此操作组的一个子项。

通常情况下，通过将组创建为父组，将操作添加到组中，因此通常不使用此功能。





6、QAction *QActionGroup::addAction(const QIcon &icon, const QString &text)

用文本和图标创建并返回一个动作。 新创建的操作是此操作组的一个子项。

通常情况下，通过以该组作为其父项创建一个操作来添加操作，所以通常不会使用此功能。





7、QAction *QActionGroup::checkedAction() const

返回组中当前检查的操作，如果没有选中，则返回0。





8、[signal] void QActionGroup::hovered(QAction *action)

当用户突出显示动作组中的给定动作时，发出此信号; 例如，当用户将光标悬停在菜单选项，工具栏按钮上或按动作的快捷键组合时。





9、void QActionGroup::removeAction(QAction *action)

从该组中删除操作。 因此，该行为将没有父母。





10、[slot] void QActionGroup::setDisabled(bool b)

这对启用属性来说是一个方便的功能，对于信号 - 插槽连接非常有用。 如果b为真，则操作组被禁用; 否则启用。





11、[signal] void QActionGroup::triggered(QAction *action)

当用户激活动作组中的给定动作时，发出此信号; 例如，当用户点击菜单选项，工具栏按钮或按动作的快捷键组合。

连接到此信号以执行命令操作。




