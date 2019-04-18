# Qt开发：Qt Widgets模块——QCheckBox类 - 一世豁然的专栏 - CSDN博客





2018年05月21日 11:09:37[一世豁然](https://me.csdn.net/Explorer_day)阅读数：770标签：[Qt](https://so.csdn.net/so/search/s.do?q=Qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/Explorer_day/article/category/5857397)









QCheckBox小部件提供了一个带有文本标签的复选框。



头文件：#include <QCheckBox> 

qmake：QT += widgets

继承：QAbstractButton

QCheckBox是一个选项按钮，可以打开（选中）或关闭（取消选中）。 复选框通常用于表示应用程序中的功能，该功能可以启用或禁用而不会影响其他功能。 可以实施不同类型的行为。 例如，QButtonGroup可以用来逻辑分组检查按钮，允许独占复选框。 但是，QButtonGroup不提供任何可视化表示。

下面的图片进一步说明了独占和非独占复选框之间的差异。


![](https://img-blog.csdn.net/20180521105713134)


只要复选框被选中或清除，它就会发出signalChanged（）信号。 如果要在每次复选框更改状态时触发某个操作，请连接到此信号。 您可以使用isChecked（）来查询是否选中复选框。


除了通常的选中状态和未选中状态外，QCheckBox还可选择提供第三种状态来指示“不变”。 当你需要给用户既不检查也不取消选中复选框的选项时，这很有用。 如果你需要这个第三个状态，使用setTristate（）来启用它，并使用checkState（）来查询当前的切换状态。


就像QPushButton一样，复选框显示文本，并可以选择一个小图标。 该图标使用setIcon（）进行设置。 文本可以在构造函数或setText（）中设置。 快捷键可以通过在首字符前加一个＆符号来指定。 例如：


```cpp
QCheckBox *checkbox = new QCheckBox("C&ase sensitive", this);
```

在这个例子中，快捷键是Alt + A。 有关详细信息，请参阅QShortcut文档。 要显示实际与号，请使用'&&'。

重要的继承函数： text(), setText(), text(), pixmap(), setPixmap(), accel(), setAccel(), isToggleButton(), setDown(), isDown(), isOn(), checkState(), autoRepeat(), isExclusiveToggle(), group(), setAutoRepeat(), toggle(), pressed(), released(), clicked(), toggled(), checkState(), and stateChanged()。

![](https://img-blog.csdn.net/20180521110029658)








一、属性

1、tristate : bool

该属性保持复选框是否为三态复选框。

默认值为false，即复选框只有两种状态。


访问函数；

```cpp
bool isTristate() const
void setTristate(bool y = true)
```







二、成员函数

1、QCheckBox::QCheckBox(QWidget *parent = Q_NULLPTR)

用给定的父项构造一个复选框，但没有文本。

父项传递给QAbstractButton构造函数。





2、QCheckBox::QCheckBox(const QString &text, QWidget *parent = Q_NULLPTR)

用给定的父项和文本构造一个复选框。

父项传递给QAbstractButton构造函数。





3、QCheckBox::~QCheckBox()

析构函数。




4、Qt::CheckState QCheckBox::checkState() const

返回复选框的检查状态。 如果你不需要三态支持，你也可以使用QAbstractButton :: isChecked（），它返回一个布尔值。





5、[virtual protected] void QCheckBox::checkStateSet()

从QAbstractButton :: checkStateSet（）重新实现。





6、[virtual protected] bool QCheckBox::event(QEvent *e)

从QObject :: event（）重新实现。





7、[virtual protected] bool QCheckBox::hitButton(const QPoint &pos) const

从QAbstractButton :: hitButton（）重新实现。





8、[protected] void QCheckBox::initStyleOption(QStyleOptionButton *option) const

用此QCheckBox的值初始化选项。 此方法对于需要QStyleOptionButton的子类非常有用，但不希望自己填写所有信息。





9、[virtual] QSize QCheckBox::minimumSizeHint() const

从QWidget :: minimumSizeHint（）重新实现。





10、[virtual protected] void QCheckBox::mouseMoveEvent(QMouseEvent *e)

从QWidget :: mouseMoveEvent（）重新实现。





11、[virtual protected] void QCheckBox::nextCheckState()

从QAbstractButton :: nextCheckState（）重新实现。





12、[virtual protected] void QCheckBox::paintEvent(QPaintEvent *)

从QWidget :: paintEvent（）重新实现。





13、void QCheckBox::setCheckState(Qt::CheckState state)

将复选框的检查状态设置为状态。 如果你不需要三态支持，你也可以使用QAbstractButton :: setChecked（），它采用布尔值。





14、[virtual] QSize QCheckBox::sizeHint() const

从QWidget :: sizeHint（）重新实现。





15、[signal] void QCheckBox::stateChanged(int state)

只要复选框的状态发生变化，即每当用户选中或取消选中时，都会发出此信号。




