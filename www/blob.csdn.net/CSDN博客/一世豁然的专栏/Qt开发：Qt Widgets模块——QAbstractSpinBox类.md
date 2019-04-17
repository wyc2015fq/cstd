# Qt开发：Qt Widgets模块——QAbstractSpinBox类 - 一世豁然的专栏 - CSDN博客





2018年05月17日 19:00:30[一世豁然](https://me.csdn.net/Explorer_day)阅读数：569








QAbstractSpinBox类提供了一个spinbox和一个行编辑来显示值。



头文件：#include <QAbstractSpinBox> 

qmake：QT += widgets

继承：QWidget

被继承：QDateTimeEdit，QDoubleSpinBox和QSpinBox

该类被设计为一个常见的超类，如QSpinBox，QDoubleSpinBox和QDateTimeEdit。

以下是这个类的主要属性：


1、文本：显示在QAbstractSpinBox中的文本。

2、对齐：QAbstractSpinBox中文本的对齐。

3、包装：QAbstractSpinBox是否从最小值包装到最大值，反之亦然。

QAbstractSpinBox提供了一个虚拟的stepBy（）函数，只要用户触发一个步骤就会被调用。 该函数采用整数值来表示采取了多少步骤。 例如。 按Qt :: Key_Down将触发对stepBy（-1）的调用。


QAbstractSpinBox还提供了一个虚函数stepEnabled（）来确定是否允许在任何点上加/减。 该函数返回StepEnabled的一个bitset。








一、成员类型

1、enum QAbstractSpinBox::ButtonSymbols

此枚举类型描述了可以在旋转框中的按钮上显示的符号。


![](https://img-blog.csdn.net/20180517181202327)



|内容|值|描述|
|----|----|----|
|QAbstractSpinBox::UpDownArrows|0|在经典风格的小箭头。|
|QAbstractSpinBox::PlusMinus|1|+和 - 符号。|
|QAbstractSpinBox::NoButtons|2|不要显示按钮。|

2、enum QAbstractSpinBox::CorrectionMode

此枚举类型描述了如果编辑完成，旋转框将用于更正中间值的模式。



|内容|值|描述|
|----|----|----|
|QAbstractSpinBox::CorrectToPreviousValue|0|Spinnerbox将恢复到最后一个有效值。|
|QAbstractSpinBox::CorrectToNearestValue|1|spinbox将恢复到最接近的有效值。|

3、enum QAbstractSpinBox::StepEnabledFlag


flags QAbstractSpinBox::StepEnabled


|内容|值|
|----|----|
|QAbstractSpinBox::StepNone|0x00|
|QAbstractSpinBox::StepUpEnabled|0x01|
|QAbstractSpinBox::StepDownEnabled|0x02|
StepEnabled类型是QFlags <StepEnabledFlag>的typedef。 它存储StepEnabledFlag值的或组合。






二、属性

1、accelerated : bool



此属性用于确定旋转框是否按下步进按钮时会加快步骤的频率。

如果启用，旋钮会在您按下按钮的时间越长时增加/减少数值。

该属性在Qt 4.2中引入。

访问函数；

```cpp
bool isAccelerated() const
void setAccelerated(bool on)
```



2、acceptableInput : const bool

该属性保持输入是否满足当前验证。

该属性在Qt 4.2中引入。


访问函数；

```cpp
bool hasAcceptableInput() const
```



3、alignment : Qt::Alignment



此属性保持旋转框的对齐。

可能的值是Qt :: AlignLeft，Qt :: AlignRight和Qt :: AlignHCenter。

默认情况下，对齐是Qt :: AlignLeft

尝试将对齐设置为非法标志组合不会执行任何操作。

访问函数；

```cpp
Qt::Alignment alignment() const
void setAlignment(Qt::Alignment flag)
```



4、buttonSymbols : ButtonSymbols



该属性保存当前按钮符号模式。

可能的值可以是UpDownArrows或PlusMinus。 默认是UpDownArrows。

请注意，某些样式可能会相同地呈现PlusMinus和UpDownArrows。

访问函数；

```cpp
ButtonSymbols buttonSymbols() const
void setButtonSymbols(ButtonSymbols bs)
```




5、correctionMode : CorrectionMode



如果编辑完成，此属性保持更正中间值的模式。

默认模式是QAbstractSpinBox :: CorrectToPreviousValue。

该属性在Qt 4.2中引入。

访问函数；

```cpp
CorrectionMode correctionMode() const
void setCorrectionMode(CorrectionMode cm)
```




6、frame : bool

该属性保持旋转框是否使用框架自身绘制。

如果启用（默认设置），则旋转框会在框架内自行绘制，否则旋转框会自动绘制而没有任何框架。


访问函数；

```cpp
bool hasFrame() const
void setFrame(bool)
```




7、keyboardTracking : bool



此属性可以确定是否为spinbox启用了键盘跟踪。

如果启用了键盘跟踪功能（默认设置），则当从键盘输入新值时，旋钮会发出valueChanged（）信号。

例如。 当用户通过键入6,0和0输入值600时，旋转盒分别发出3个信号，其值分别为6,60和600。

如果禁用键盘跟踪，则旋转框在键入时不会发出valueChanged（）信号。 它稍后发出信号，当按下返回键时，当键盘焦点丢失时，或者当使用其它旋转盒功能时， 按箭头键。

这个属性是在Qt 4.3中引入的。

访问函数；

```cpp
bool keyboardTracking() const
void setKeyboardTracking(bool kt)
```




8、readOnly : bool



该属性保持旋转框是否只读。

在只读模式下，用户仍然可以将文本复制到剪贴板，或者拖放文本; 但不能编辑它。

QAbstractSpinBox中的QLineEdit不显示处于只读模式的游标。

访问函数；

```cpp
bool isReadOnly() const
void setReadOnly(bool r)
```




9、showGroupSeparator : bool

该属性保持是否启用千位分隔符。 默认情况下，该属性为false。

这个属性是在Qt 5.3中引入的。


访问函数；

```cpp
bool isGroupSeparatorShown() const
void setGroupSeparatorShown(bool shown)
```




10、specialValueText : QString



该属性包含特殊值文本。

如果设置，只要当前值等于minimum（），旋转框就会显示该文本而不是数字值。 典型的用法是表明这个选择有一个特殊的（默认）含义。

例如，如果您的旋转框允许用户选择显示图像的比例因子（或缩放比例），并且您的应用程序能够自动选择一个可以使图像完全适合显示窗口的图像，则可以设置 像这样旋转盒子：

```cpp
QSpinBox *zoomSpinBox = new QSpinBox;
      zoomSpinBox->setRange(0, 1000);
      zoomSpinBox->setSingleStep(10);
      zoomSpinBox->setSuffix("%");
      zoomSpinBox->setSpecialValueText(tr("Automatic"));
      zoomSpinBox->setValue(100);
```



然后，用户可以选择从1％到1000％的比例或选择“自动”以将其留给应用程序选择。 然后，您的代码必须将旋转框值0解释为来自用户的请求，以缩放图像以适合窗口内部。

所有值都以前缀和后缀（如果已设置）显示，除了特殊值（仅显示特殊值文本）。 这个特殊的文本在传递QString的QSpinBox :: valueChanged（）信号中传递。
要关闭特殊值文本显示，请使用空字符串调用此函数。 默认值是没有特殊值的文本，即数字值如通常显示。

如果没有设置特殊值文本，则specialValueText（）返回一个空字符串。

访问函数；

```cpp
QString specialValueText() const
void setSpecialValueText(const QString &txt)
```


11、text : const QString

该属性包含旋钮框的文本，包括任何前缀和后缀。

没有默认文字。


访问函数；

```cpp
QString text() const
```



12、wrapping : bool

该属性可以确定纺丝箱是否为圆形。

如果包装是真实的从最大（）值增加将带你到最小（）值，反之亦然。 如果您设置了最小值（）和最大值（），则包装只有意义。


```cpp
QSpinBox *spinBox = new QSpinBox(this);
  spinBox->setRange(0, 100);
  spinBox->setWrapping(true);
  spinBox->setValue(100);
  spinBox->stepBy(1);
  // value is 0
```

访问函数；

```cpp
bool wrapping() const
void setWrapping(bool w)
```






三、成员函数

1、QAbstractSpinBox::QAbstractSpinBox(QWidget *parent = Q_NULLPTR)


使用默认包装和对齐属性构造具有给定父项的抽象旋转框。





2、QAbstractSpinBox::~QAbstractSpinBox()

在QAbstractSpinBox被销毁时调用。





3、[virtual protected] void QAbstractSpinBox::changeEvent(QEvent *event)

从QWidget :: changeEvent（）重新实现。





4、[virtual slot] void QAbstractSpinBox::clear()

清除所有文本的行编号，但前缀和后缀。





5、[virtual protected] void QAbstractSpinBox::closeEvent(QCloseEvent *event)

从QWidget :: closeEvent（）重新实现。





6、[virtual protected] void QAbstractSpinBox::contextMenuEvent(QContextMenuEvent *event)

从QWidget :: contextMenuEvent（）重新实现。





7、[signal] void QAbstractSpinBox::editingFinished()

该信号发出编辑完成。 当spinbox失去焦点并且按下Enter键时会发生这种情况。





8、[virtual] bool QAbstractSpinBox::event(QEvent *event)

从QObject :: event（）重新实现。





9、[virtual] void QAbstractSpinBox::fixup(QString &input) const

如果在按下Return或者调用interpretText（）时，输入未验证为QValidator :: Acceptable，则此虚函数由QAbstractSpinBox调用。 它会尝试更改文本以使其有效。 在各个子类中重新实现。





10、[virtual protected] void QAbstractSpinBox::focusInEvent(QFocusEvent *event)

从QWidget :: focusInEvent（）重新实现。





11、[virtual protected] void QAbstractSpinBox::focusOutEvent(QFocusEvent *event)

从QWidget :: focusOutEvent（）重新实现。





12、[virtual protected] void QAbstractSpinBox::hideEvent(QHideEvent *event)

从QWidget :: hideEvent（）重新实现。





13、[protected] void QAbstractSpinBox::initStyleOption(QStyleOptionSpinBox *option) const

使用此QSpinBox的值初始化选项。 当他们需要一个QStyleOptionSpinBox的时候，这个方法对子类很有用，但不想自己填写所有的信息。





14、[virtual] QVariant QAbstractSpinBox::inputMethodQuery(Qt::InputMethodQuery query) const

从QWidget :: inputMethodQuery（）重新实现。





15、void QAbstractSpinBox::interpretText()

该功能解释旋钮框的文字。 如果自上次解释后数值发生变化，它将发出信号。





16、[virtual protected] void QAbstractSpinBox::keyPressEvent(QKeyEvent *event)



从QWidget :: keyPressEvent（）重新实现。

该功能处理键盘输入。

以下按键具体处理：


|Enter/Return|这将重新解释文本并发出信号，即使该值自上次发出信号后没有改变。|
|----|----|
|Up|这将调用stepBy（1）|
|Down|这将调用stepBy（-1）|
|Page Ip|这将调用stepBy（10）|
|Page Down|这将调用stepBy（-10）|

17、[virtual protected] void QAbstractSpinBox::keyReleaseEvent(QKeyEvent *event)

从QWidget :: keyReleaseEvent（）重新实现。





18、[protected] QLineEdit *QAbstractSpinBox::lineEdit() const

该函数返回一个指向旋转框线条编辑的指针。





19、[virtual] QSize QAbstractSpinBox::minimumSizeHint() const

从QWidget :: minimumSizeHint（）重新实现。





20、[virtual protected] void QAbstractSpinBox::mouseMoveEvent(QMouseEvent *event)

从QWidget :: mouseMoveEvent（）重新实现。





21、[virtual protected] void QAbstractSpinBox::mousePressEvent(QMouseEvent *event)

从QWidget :: mousePressEvent（）重新实现。





22、[virtual protected] void QAbstractSpinBox::mouseReleaseEvent(QMouseEvent *event)

从QWidget :: mouseReleaseEvent（）重新实现。





23、[virtual protected] void QAbstractSpinBox::paintEvent(QPaintEvent *event)

从QWidget :: paintEvent（）重新实现。





24、[virtual protected] void QAbstractSpinBox::resizeEvent(QResizeEvent *event)

从QWidget :: resizeEvent（）重新实现。





25、[slot] void QAbstractSpinBox::selectAll()

选中除了前缀和后缀以外的所有文本。





26、[protected] void QAbstractSpinBox::setLineEdit(QLineEdit *lineEdit)



将spinbox的行编辑设置为lineEdit而不是当前的行编辑小部件。 lineEdit不能为0。

QAbstractSpinBox获得新的lineEdit的所有权

如果lineEdit的QLineEdit :: validator（）返回0，那么spinbox的内部验证器将在行编辑中设置。




27、[virtual protected] void QAbstractSpinBox::showEvent(QShowEvent *event)

从QWidget :: showEvent（）重新实现。





28、[virtual] QSize QAbstractSpinBox::sizeHint() const

从QWidget :: sizeHint（）重新实现。





29、[virtual] void QAbstractSpinBox::stepBy(int steps)

当用户触发某个步骤时调用的虚函数。 steps参数表示采取了多少步骤，例如， 按下Qt :: Key_Down将触发对stepBy（-1）的调用，而按Qt :: Key_Prior将触发对stepBy（10）的调用。

如果您继承QAbstractSpinBox，则必须重新实现此功能。 请注意，即使结果值超出最小值和最大值的范围，也会调用此函数。 这是处理这些情况的功能。





30、[slot] void QAbstractSpinBox::stepDown()

降低一个步长调用此插槽类似于调用stepBy（-1）;





31、[virtual protected] StepEnabled QAbstractSpinBox::stepEnabled() const



虚拟功能决定了在任何给定时间升降是合法的。

除非（stepEnabled（）＆StepUpEnabled）！= 0，向上箭头将被禁用。

如果打开包装，则默认实现将返回（StepUpEnabled | StepDownEnabled）。 否则，如果value> minimum（），则它将返回StepDownEnabled，如果value <maximum（），则返回StepUpEnabled。

如果您继承QAbstractSpinBox，则需要重新实现此功能。




32、[slot] void QAbstractSpinBox::stepUp()

提高一个步长调用此插槽类似于调用stepBy（1）;





33、[virtual protected] void QAbstractSpinBox::timerEvent(QTimerEvent *event)

从QObject :: timerEvent（）重新实现。





34、[virtual] QValidator::State QAbstractSpinBox::validate(QString &input, int &pos) const

这个虚函数被QAbstractSpinBox调用以确定输入是否有效。 pos参数指示字符串中的位置。 在各个子类中重新实现。





35、[virtual protected] void QAbstractSpinBox::wheelEvent(QWheelEvent *event)

从QWidget :: wheelEvent（）重新实现。




