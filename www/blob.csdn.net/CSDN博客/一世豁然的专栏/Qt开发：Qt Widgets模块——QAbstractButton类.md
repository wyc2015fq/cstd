# Qt开发：Qt Widgets模块——QAbstractButton类 - 一世豁然的专栏 - CSDN博客





2018年05月15日 13:55:09[一世豁然](https://me.csdn.net/Explorer_day)阅读数：347








QAbstractButton类是按钮小部件的抽象基类，提供按钮通用的功能。

头文件：#include <QAbstractButton> 

qmake：QT += widgets


继承：QWidget


被继承：QCheckBox, QPushButton, QRadioButton, and QToolButton

这个类实现了一个抽象按钮。 此类的子类处理用户操作，并指定按钮的绘制方式。


QAbstractButton支持按钮和可选（切换）按钮。 QRadioButton和QCheckBox类中实现了可检查按钮。 按钮在QPushButton和QToolButton类中实现; 如果需要，这些还提供切换行为。


任何按钮都可以显示包含文本和图标的标签。 setText（）设置文本; setIcon（）设置图标。 如果一个按钮被禁用，它的标签被改变，以给按钮一个“disabled”的外观。


如果按钮是一个包含符号（'＆'）字符串的文本按钮，QAbstractButton会自动创建一个快捷键。 例如：


```cpp
QPushButton *button = new QPushButton(tr("Ro&ck && Roll"), this);
```

将Alt + C快捷键分配给该按钮，即，当用户按下Alt + C时，该按钮将调用animateClick（）。 有关详细信息，请参阅QShortcut文档。 要显示实际与号，请使用'&&'。

您还可以使用setShortcut（）函数设置自定义快捷键。 这对于没有任何文本的按钮来说很有用，因此不能有任何自动快捷方式。


```cpp
button->setIcon(QIcon(":/images/print.png"));
  button->setShortcut(tr("Alt+F7"));
```

Qt提供的所有按钮（QPushButton，QToolButton，QCheckBox和QRadioButton）都可以显示文本和图标。

可以通过QPushButton :: setDefault（）和QPushButton :: setAutoDefault（）将按钮设置为对话框中的默认按钮。


QAbstractButton提供了用于按钮的大部分状态：


·1、isDown（）指示按钮是否被按下。

2、isChecked（）指示是否检查按钮。 只有可检查的按钮可以被选中和取消选中（见下文）。

3、isEnabled（）表示按钮是否可以被用户按下。

注意：与其他小部件相反，从QAbstractButton派生的按钮在禁用时会接受鼠标和上下文菜单事件。


4、setAutoRepeat（）设置按钮是否会在用户按下时自动重复。 autoRepeatDelay和autoRepeatInterval定义了如何完成自动重复。

5、setCheckable（）设置按钮是否是切换按钮。

isDown（）和isChecked（）之间的区别如下。 当用户点击切换按钮来检查它时，首先按下按钮然后释放到检查状态。 当用户再次单击它（取消选中它）时，该按钮首先移动到按下状态，然后进入未选中状态（isChecked（）和isDown（）都为false）。


QAbstractButton提供了四个信号：


1、当鼠标光标在按钮内时按下鼠标左键，会发出pressed（）。

2、当释放鼠标左键时发送released（）。

3、当按钮被第一次按下然后释放时，当键入快捷键时，或者当调用click（）或animateClick（）时，发出clicked（）。

4、切换按钮的状态发生变化时，会发出toggled（）。

要继承QAbstractButton，必须至少重新实现paintEvent（）以绘制按钮的轮廓及其文本或像素图。 通常建议重新实现sizeHint（）以及有时hitButton（）（以确定按钮是否在按钮内）。 对于具有两种以上状态的按钮（如三态按钮），您还必须重新实现checkStateSet（）和nextCheckState（）。





一、属性

1、autoExclusive : bool

此属性保持是否启用自动排他性。


如果启用了自动排除功能，属于同一父窗口小部件的可选按钮的行为就好像它们是同一个排他按钮组的一部分一样。 在排他按钮组中，任何时候只能检查一个按钮; 自动检查另一个按钮将取消选中之前选中的按钮。


该属性对属于按钮组的按钮没有影响。


除了单选按钮，autoExclusive在默认情况下处于关闭状态。


访问函数：


```cpp
bool 
autoExclusive() const
void 
setAutoExclusive(bool)
```




2、autoRepeat : bool

该属性保持是否启用autoRepeat。


如果启用了autoRepeat，那么当按钮关闭时，pressed（），released（）和clicked（）信号将以固定间隔发出。 autoRepeat默认是关闭的。 初始延迟和重复间隔由autoRepeatDelay和autoRepeatInterval以毫秒定义。


注意：如果一个按钮被快捷键按下，则自动重复功能会被系统启用并定时，而不会被这个类别启用。 pressed（），released（）和clicked（）信号将像正常情况下发送。


访问函数；

```cpp
bool autoRepeat() const
void setAutoRepeat(bool)
```




3、autoRepeatDelay : int

该属性具有自动重复的初始延迟。


如果启用autoRepeat，则autoRepeatDelay会在自动重复启动之前以毫秒为单位定义初始延迟。


此属性在Qt4.2介绍。

访问函数；

```cpp
int autoRepeatDelay() const
void setAutoRepeatDelay(int)
```




4、autoRepeatInterval : int

该属性保存自动重复的时间间隔。


如果启用了autoRepeat，则autoRepeatInterval以毫秒为单位定义自动重复间隔的长度。


此属性在Qt4.2介绍。


访问函数；

```cpp
int autoRepeatInterval() const
void setAutoRepeatInterval(int)
```




5、checkable : bool

该属性保持按钮是否可检查。


默认情况下，该按钮不可检查。


访问函数；

```cpp
bool isCheckable() const
void setCheckable(bool)
```




6、checked : bool

该属性保持是否检查按钮。


只能检查可检查的按钮。 默认情况下，该按钮未被选中。


访问函数；

```cpp
bool isChecked() const
void setChecked(bool)
```

通知信号：


```cpp
void toggled(bool checked)
```




7、down : bool

该属性保持按钮是否被按下。


如果该属性为true，则该按钮被按下。 如果将此属性设置为true，则不会发出pressed（）和clicked（）信号。 默认值是false。


访问函数；

```cpp
bool isDown() const
void setDown(bool)
```




8、icon : QIcon

该属性保存按钮上显示的图标。


图标的默认大小由GUI样式定义，但可以通过设置iconSize属性进行调整。


访问函数；

```cpp
QIcon icon() const
void setIcon(const QIcon &icon)
```




9、iconSize : QSize

该属性保存用于此按钮的图标大小。


默认大小由GUI样式定义。 这是图标的最大尺寸。 较小的图标不会被放大。


访问函数；

```cpp
QSize iconSize() const
void setIconSize(const QSize &size)
```




10、shortcut : QKeySequence

该属性包含与该按钮关联的助记符。


访问函数；

```cpp
QKeySequence shortcut() const
void setShortcut(const QKeySequence &key)
```




11、text : QString

该属性保存按钮上显示的文本。


如果按钮没有文本，则text（）函数将返回一个空字符串。


如果文本包含＆字符（'＆'），则会自动为其创建快捷方式。 '＆'后面的字符将用作快捷键。 如果文本没有定义快捷方式，则以前的任何快捷方式都将被覆盖或清除。 有关详细信息，请参阅QShortcut文档。 要显示实际与号，请使用'&&'。


没有默认文字。


访问函数；

```cpp
QString text() const
void setText(const QString &text)
```







二、成员函数

1、QAbstractButton::QAbstractButton(QWidget *parent = Q_NULLPTR)

用父级构造一个抽象按钮。





2、QAbstractButton::~QAbstractButton()

销毁按钮。





3、[slot] void QAbstractButton::animateClick(int msec = 100)

执行动画点击：立即按下按钮，并在毫秒后释放（默认值为100毫秒）。


释放按钮之前再次调用此功能会重置释放计时器。


所有与点击相关的信号都会根据情况发出。


如果该按钮被禁用，该功能不起任何作用。





4、[virtual protected] void QAbstractButton::changeEvent(QEvent *e)

从QWidget :: changeEvent（）重新实现。





5、[virtual protected] void QAbstractButton::checkStateSet()

这个虚拟处理程序在使用setChecked（）时被调用，除非它是在nextCheckState（）中调用的。 它允许子类重置其中间按钮状态。





6、[slot] void QAbstractButton::click()

执行点击。


所有与点击相关的常见信号都会根据情况发出。 如果该按钮是可检查的，则该按钮的状态被切换。


如果该按钮被禁用，该功能不起任何作用。





7、[signal] void QAbstractButton::clicked(bool checked = false)

当按钮被激活时（即，当鼠标光标在按钮内时按下然后释放），当键入快捷键时，或者当调用click（）或animateClick（）时，该信号被发射。 值得注意的是，如果调用setDown（），setChecked（）或toggle（），则不会发出此信号。


如果该按钮是可检查的，则选中该按钮时检查为true;如果该按钮未选中，则检查为false。





8、[virtual protected] bool QAbstractButton::event(QEvent *e)

从QObject :: event（）重新实现。





9、[virtual protected] void QAbstractButton::focusInEvent(QFocusEvent *e)

从QWidget :: focusInEvent（）重新实现。





10、[virtual protected] void QAbstractButton::focusOutEvent(QFocusEvent *e)

从QWidget :: focusOutEvent（）重新实现。





11、QButtonGroup *QAbstractButton::group() const

返回此按钮所属的组。


如果该按钮不是任何QButtonGroup的成员，则该函数返回0。





12、[virtual protected] bool QAbstractButton::hitButton(const QPoint &pos) const

如果pos位于可点击的按钮矩形内，则返回true;


默认情况下，可点击区域是整个小部件。 子类可以重新实现此功能，以支持不同形状和大小的可点击区域。





13、[virtual protected] void QAbstractButton::keyPressEvent(QKeyEvent *e)

从QWidget :: keyPressEvent（）重新实现。





14、[virtual protected] void QAbstractButton::keyReleaseEvent(QKeyEvent *e)

从QWidget :: keyReleaseEvent（）重新实现。





15、[virtual protected] void QAbstractButton::mouseMoveEvent(QMouseEvent *e)

从QWidget :: mouseMoveEvent（）重新实现。





16、[virtual protected] void QAbstractButton::mousePressEvent(QMouseEvent *e)

从QWidget :: mousePressEvent（）重新实现。





17、[virtual protected] void QAbstractButton::mouseReleaseEvent(QMouseEvent *e)

从QWidget :: mouseReleaseEvent（）重新实现。





18、[virtual protected] void QAbstractButton::nextCheckState()

这个虚拟处理程序在单击按钮时被调用。 如果按钮ischellable（），则默认实现调用setChecked（！isChecked（））。 它允许子类实现中间按钮状态。





19、[pure virtual protected] void QAbstractButton::paintEvent(QPaintEvent *e)

从QWidget :: paintEvent（）重新实现。





20、[signal] void QAbstractButton::pressed()

按下此按钮时会发出此信号。





21、[signal] void QAbstractButton::released()

释放按钮时发出此信号。





22、[virtual protected] void QAbstractButton::timerEvent(QTimerEvent *e)

从QObject :: timerEvent（）重新实现。





23、[slot] void QAbstractButton::toggle()

切换可检查按钮的状态。





24、[signal] void QAbstractButton::toggled(bool checked)

只要可检查按钮改变其状态，就会发出该信号。 如果按钮被选中，则检查为true;如果未选中该按钮，则检查为false。


这可能是用户操作，click（）插槽激活或setChecked（）被调用的结果。


此信号发出之前，排他按钮组中按钮的状态会更新。 这意味着插槽可以作用于状态发生变化的组中按钮所发出的“关闭”信号或“开启”信号。


例如，可以使用以下模式来实现一个插槽，该插槽对新检查的按钮发出的信号作出反应，但会忽略未检查按钮的信号：


```cpp
void MyWidget::reactToToggle(bool checked)
  {
     if (checked) {
        // Examine the new button states.
        ...
     }
  }
```

可以使用QButtonGroup类创建按钮组，并使用QButtonGroup :: buttonClicked（）信号更新按钮状态。

注意：检查属性的通知程序信号。




