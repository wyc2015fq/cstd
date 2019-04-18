# Qt开发：Qt Widgets模块——QButtonGroup类 - 一世豁然的专栏 - CSDN博客





2018年05月19日 10:02:44[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1330标签：[Qt](https://so.csdn.net/so/search/s.do?q=Qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/Explorer_day/article/category/5857397)









QButtonGroup类提供了一个容器来组织按钮小部件组。



头文件：#include <QButtonGroup>

qmake：QT += widgets

继承：QObject

QButtonGroup提供了一个抽象容器，可以在其中放置按钮小部件。 它不提供此容器的可视化表示（请参阅容器小部件的QGroupBox），而是管理组中每个按钮的状态。


独占按钮组关闭除已被点击的按钮之外的所有可检查（切换）按钮。 默认情况下，按钮组是独占的。 按钮组中的按钮通常是可检查的QPushButtons，QCheckBoxes（通常用于非专属按钮组）或QRadioButtons。 如果您创建专属按钮组，则应确保组中的其中一个按钮最初被检查; 否则，该组将最初处于未检查按钮的状态。


可以使用addButton（）将按钮添加到组中，并使用removeButton（）将其删除。 如果该组是独占的，则当前选中的按钮与checkedButton（）一起可用。 如果点击一个按钮，会发出buttonClicked（）信号; 对于排他组中的可检查按钮，这意味着该按钮已被检查。 buttons（）返回组中按钮的列表。


另外，QButtonGroup可以在整数和按钮之间进行映射。 您可以使用setId（）将一个整数id分配给一个按钮，并使用id（）检索它。 当前选中的按钮的id对于checkedId（）是可用的，并且有一个重载信号buttonClicked（），它发出按钮的id。 Q-1被QButtonGroup保留为“没有这样的按钮”。 映射机制的目的是简化用户界面中枚举值的表示。








一、属性

1、exclusive : bool

该属性保持按钮组是否是独占的。


如果此属性为true，则在任何给定时间只能检查组中的一个按钮。 用户可以点击任何按钮来检查它，并且该按钮将替换现有的按钮作为组中的选中按钮。


在专属组中，用户不能通过点击来取消当前选中的按钮; 相反，组中的另一个按钮必须被点击以设置该组的新选中按钮。


默认情况下，该属性为true。


访问函数；

```cpp
bool exclusive() const
void setExclusive(bool)
```







二、成员函数

1、QButtonGroup::QButtonGroup(QObject *parent = Q_NULLPTR)

用给定的父项构造一个新的空按钮组。





2、QButtonGroup::~QButtonGroup()

销毁按钮组。





3、void QButtonGroup::addButton(QAbstractButton *button, int id = -1)

将给定的按钮添加到按钮组。 如果id为-1，则将为该按钮分配一个id。 自动分配的ID保证为负数，从-2开始。 如果您正在分配自己的ID，请使用正值来避免冲突。





4、QAbstractButton *QButtonGroup::button(int id) const

返回具有指定ID的按钮，如果不存在此按钮，则返回0。

这个函数是在Qt 4.1中引入的。





5、[signal] void QButtonGroup::buttonClicked(QAbstractButton *button)

点击给定按钮时会发出此信号。 当第一次按下然后释放按钮时，当它的快捷键被键入时，或者以编程方式调用QAbstractButton :: click（）或QAbstractButton :: animateClick（）时，会单击一个按钮。


注意：此类中的信号buttonClicked已被重载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(buttonGroup, static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked),
      [=](QAbstractButton *button){ /* ... */ });
```




6、[signal] void QButtonGroup::buttonClicked(int id)

点击具有给定ID的按钮时会发出此信号。

注意：此类中的信号buttonClicked已被重载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：

```cpp
connect(buttonGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
      [=](int id){ /* ... */ });
```




7、[signal] void QButtonGroup::buttonPressed(QAbstractButton *button)

按下给定按钮时发出此信号。

注意：此类中的信号buttonPressed被重载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(buttonGroup, static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonPressed),
      [=](QAbstractButton *button){ /* ... */ });
```

这个函数是在Qt 4.2中引入的。




8、[signal] void QButtonGroup::buttonPressed(int id)

当具有给定ID的按钮被按下时，该信号被发射。

注意：此类中的信号buttonPressed被重载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(buttonGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),
      [=](int id){ /* ... */ });
```

这个函数是在Qt 4.2中引入的。




9、[signal] void QButtonGroup::buttonReleased(QAbstractButton *button)

当给定的按钮被释放时，该信号被发射。

注意：此类中的信号buttonReleased被重载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(buttonGroup, static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonReleased),
      [=](QAbstractButton *button){ /* ... */ });
```

这个函数是在Qt 4.2中引入的。




10、[signal] void QButtonGroup::buttonReleased(int id)

当具有给定ID的按钮被释放时，该信号被发射。

注意：此类中的信号buttonReleased被重载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(buttonGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonReleased),
      [=](int id){ /* ... */ });
```

这个函数是在Qt 4.2中引入的。




11、[signal] void QButtonGroup::buttonToggled(QAbstractButton *button, bool checked)

当给定的按钮被切换时，该信号被发射。 如果按钮被选中，则检查为true;如果未选中该按钮，则检查为false。

注意：此类中的信号buttonToggled超载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(buttonGroup, static_cast<void(QButtonGroup::*)(QAbstractButton *, bool)>(&QButtonGroup::buttonToggled),
      [=](QAbstractButton *button, bool checked){ /* ... */ });
```

这个函数是在Qt 5.2中引入的。




12、[signal] void QButtonGroup::buttonToggled(int id, bool checked)

当具有给定ID的按钮被切换时，该信号被发射。 如果按钮被选中，则检查为true;如果未选中该按钮，则检查为false。

注意：此类中的信号buttonToggled超载。 要使用函数指针语法连接到此函数，您必须在静态转换中指定信号类型，如下例所示：


```cpp
connect(buttonGroup, static_cast<void(QButtonGroup::*)(int, bool)>(&QButtonGroup::buttonToggled),
      [=](int id, bool checked){ /* ... */ });
```

这个函数是在Qt 5.2中引入的。




13、QList<QAbstractButton *> QButtonGroup::buttons() const

返回按钮组的按钮列表。 这可能是空的。





14、QAbstractButton *QButtonGroup::checkedButton() const

返回按钮组的选中按钮，如果没有按钮被选中，则返回0。





15、int QButtonGroup::checkedId() const

返回checkedButton（）的id，如果没有选中按钮，则返回-1。

这个函数是在Qt 4.1中引入的。





16、int QButtonGroup::id(QAbstractButton *button) const

返回指定按钮的ID，如果不存在此按钮，则返回-1。

这个函数是在Qt 4.1中引入的。





17、void QButtonGroup::removeButton(QAbstractButton *button)

从按钮组中删除给定的按钮。





18、void QButtonGroup::setId(QAbstractButton *button, int id)

设置指定按钮的ID。 请注意，ID不能是-1。

这个函数是在Qt 4.1中引入的。




