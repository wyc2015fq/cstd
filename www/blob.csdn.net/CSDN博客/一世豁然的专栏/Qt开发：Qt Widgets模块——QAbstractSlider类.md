# Qt开发：Qt Widgets模块——QAbstractSlider类 - 一世豁然的专栏 - CSDN博客





2018年05月17日 17:26:51[一世豁然](https://me.csdn.net/Explorer_day)阅读数：272








QAbstractSlider类提供一个范围内的整数值。



头文件：#include <QAbstractSlider> 

qmake：QT += widgets

继承：QWidget

被继承：QDial，QScrollBar和QSlider

该类被设计为一个常见的超类，如QScrollBar，QSlider和QDial。


以下是这个类的主要属性：


1、value：QAbstractSlider维护的有界整数。

2、最小值：可能的最低值。

3、最大值：尽可能高的值。

4、singleStep：抽象滑块提供的两个自然步中较小的一个，通常对应于用户按箭头键。

5、pageStep：抽象滑块提供的两个自然步骤中较大的一个，通常对应于用户按PageUp或PageDown。

6、跟踪：是否启用滑块跟踪。

7、sliderPosition：滑块的当前位置。 如果启用跟踪（默认），则这与值相同。

统一（1）可被视为第三步大小。 setValue（）允许您将当前值设置为允许的范围内的任何整数，而不是对整数值为n的minimum（）+ n * singleStep（）。 一些小部件可能允许用户设置任何值; 其他人可能只提供multipleStep（）或pageStep（）的倍数。

QAbstractSlider发出一组全面的信号：



|信号|什么时候发送|
|----|----|
|valueChanged()|值已经改变。 跟踪确定是否在用户交互期间发出此信号。|
|sliderPressed()|用户开始拖动滑块。|
|sliderMoved()|用户拖动滑块。|
|[sliderReleased](qabstractslider.html#sliderReleased)()|用户释放滑块。|
|actionTriggered()|一个滑块动作被触发。|
|[rangeChanged](qabstractslider.html#rangeChanged)()|范围发生了变化。|
QAbstractSlider提供了一个虚拟sliderChange（）函数，非常适合更新滑块的屏幕表示。 通过调用triggerAction（），子类触发滑块动作。 两个辅助函数QStyle :: sliderPositionFromValue（）和QStyle :: sliderValueFromPosition（）帮助子类和样式将屏幕坐标映射到逻辑范围值。






一、成员类型

1、enum QAbstractSlider::SliderAction


|内容|值|
|----|----|
|QAbstractSlider::SliderNoAction|0|
|QAbstractSlider::SliderSingleStepAdd|1|
|QAbstractSlider::SliderSingleStepSub|2|
|QAbstractSlider::SliderPageStepAdd|3|
|QAbstractSlider::SliderPageStepSub|4|
|QAbstractSlider::SliderToMinimum|5|
|QAbstractSlider::SliderToMaximum|6|
|QAbstractSlider::SliderMove|7|

2、enum QAbstractSlider::SliderChange


|内容|值|
|----|----|
|QAbstractSlider::SliderRangeChange|0|
|QAbstractSlider::SliderOrientationChange|1|
|QAbstractSlider::SliderStepsChange|2|
|QAbstractSlider::SliderValueChange|3|




二、属性

1、invertedAppearance : bool



该属性保持滑块是否显示其值颠倒。

如果此属性为false（默认值），则最小值和最大值将显示在其继承的小部件的经典位置中。 如果该值为真，则最小值和最大值出现在相反的位置。

注意：此属性对滑块和拨号最有意义。 对于滚动条，滚动条子控件的视觉效果取决于样式是否理解倒转的外观; 大多数样式忽略滚动条的此属性。

访问函数；

```cpp
bool invertedAppearance() const
void setInvertedAppearance(bool)
```




2、invertedControls : bool

此属性可以确定滑块是否反转轮子和键盘事件。

如果此属性为false，则向上滚动鼠标滚轮并使用向上翻页等按键可将滑块的值增加到最大值。 否则，向上翻页会将值向最小值移动。


访问函数；

```cpp
bool invertedControls() const
void setInvertedControls(bool)
```



3、maximum : int

该属性保持滑块的最大值。

设置此属性时，必要时调整最小值以确保范围保持有效。 滑块的当前值也被调整到新的范围内。


访问函数；

```cpp
int maximum() const
void setMaximum(int)
```



4、minimum : int

该属性包含滑块的最小值。

设置此属性时，如果需要调整最大值以确保范围保持有效。 滑块的当前值也被调整到新的范围内。


访问函数；

```cpp
int minimum() const
void setMinimum(int)
```



5、orientation : Qt::Orientation

该属性保存滑块的方向。

方向必须是Qt :: Vertical（默认）或Qt :: Horizontal。


访问函数；

```cpp
Qt::Orientation orientation() const
void setOrientation(Qt::Orientation)
```



6、pageStep : int

该属性包含页面步骤。

抽象滑块提供的两个自然步骤中较大的一个，通常对应于用户按PageUp或PageDown。


访问函数；

```cpp
int pageStep() const
void setPageStep(int)
```




7、singleStep：int



这个属性只有一个步骤。

抽象滑块提供的两个自然步骤中的较小者，并且通常对应于用户按下箭头键。

如果该属性在自动重复键事件期间被修改，则行为未定义。

访问函数；

```cpp
int singleStep() const
void setSingleStep(int)
```




8、sliderDown : bool



该属性保持滑块是否被按下。

该属性由子类设置，以便让抽象滑块知道跟踪是否有效。

更改滑块向下属性将发出sliderPressed（）和sliderReleased（）信号。

访问函数；

```cpp
bool isSliderDown() const
void setSliderDown(bool)
```




9、sliderPosition : int

该属性保存当前的滑块位置。

如果启用跟踪（默认），则这与值相同。


访问函数；

```cpp
int sliderPosition() const
void setSliderPosition(int)
```

通知信号：

```cpp
void sliderMoved(int value)
```



10、tracking : bool

该属性保持滑块跟踪是否被启用。

如果启用了跟踪（默认），滑块将在拖动滑块时发出valueChanged（）信号。 如果禁用了跟踪，则只有当用户释放滑块时，滑块才会发出valueChanged（）信号。


访问函数；

```cpp
bool hasTracking() const
void setTracking(bool enable)
```




11、value : int



该属性保存滑块的当前值。

滑块强制该值在合法范围内：最小值<=值<=最大值。

更改该值也会更改sliderPosition。

访问函数；

```cpp
int value() const
void setValue(int)
```

通知信号；

```cpp
void valueChanged(int value)
```







三、成员函数

1、QAbstractSlider::QAbstractSlider(QWidget *parent = Q_NULLPTR)



构造一个抽象滑块。

父参数被发送到QWidget构造函数。

最小值默认为0，最大值为99，其中singleStep大小为1，pageStep大小为10，初始值为0。




2、QAbstractSlider::~QAbstractSlider()

销毁滑块。





3、[signal] void QAbstractSlider::actionTriggered(int action)

该信号在触发滑块动作动作时发出。 操作为SliderSingleStepAdd，SliderSingleStepSub，SliderPageStepAdd，SliderPageStepSub，SliderToMinimum，SliderToMaximum和SliderMove。

当信号发出时，sliderPosition已根据动作进行了调整，但该值尚未传播（意味着valueChanged（）信号尚未发出），并且可视化显示尚未更新。 在连接到此信号的插槽中，您可以根据动作和滑块的值，通过自己调用setSliderPosition（）来安全地调整任何动作。





4、[virtual protected] void QAbstractSlider::changeEvent(QEvent *ev)

从QWidget :: changeEvent（）重新实现。





5、[virtual protected] bool QAbstractSlider::event(QEvent *e)

从QObject :: event（）重新实现。





6、[virtual protected] void QAbstractSlider::keyPressEvent(QKeyEvent *ev)

从QWidget :: keyPressEvent（）重新实现。





7、[signal] void QAbstractSlider::rangeChanged(int min, int max)

该信号在滑块范围发生变化时发出，其中min是新的最小值，max是新的最大值。





8、[protected] SliderAction QAbstractSlider::repeatAction() const

返回当前的重复动作。





9、[slot] void QAbstractSlider::setRange(int min, int max)

将滑块的最小值设置为最小值，将其最大值设置为最大值。

如果max小于min，则min成为唯一合法值。





10、[protected] void QAbstractSlider::setRepeatAction(SliderAction action, int thresholdTime = 500, int repeatTime = 50)

设置actionTime动作，在thresholdTime的初始延迟后，以repeatTime的间隔重复触发。





11、[virtual protected] void QAbstractSlider::sliderChange(SliderChange change)

重新实现此虚拟函数以跟踪滑块更改，如SliderRangeChange，SliderOrientationChange，SliderStepsChange或SliderValueChange。 默认实现只更新显示并忽略更改参数。





12、[signal] void QAbstractSlider::sliderMoved(int value)



当sliderDown为真并且滑块移动时，会发出此信号。 这通常发生在用户拖动滑块时。 该值是新的滑块位置。

即使关闭跟踪，也会发出此信号。

注意：属性sliderPosition的通知程序信号。




13、[signal] void QAbstractSlider::sliderPressed()

当用户用鼠标按下滑块时发出此信号，或者在调用setSliderDown（true）时以编程方式发出此信号。





14、[signal] void QAbstractSlider::sliderReleased()

当用户用鼠标释放滑块时发出此信号，或者在调用setSliderDown（false）时以编程方式发出此信号。





15、[virtual protected] void QAbstractSlider::timerEvent(QTimerEvent *e)

从QObject :: timerEvent（）重新实现。





16、void QAbstractSlider::triggerAction(SliderAction action)

触发滑块动作。 可能的操作是SliderSingleStepAdd，SliderSingleStepSub，SliderPageStepAdd，SliderPageStepSub，SliderToMinimum，SliderToMaximum和SliderMove。





17、[signal] void QAbstractSlider::valueChanged(int value)

当滑块值改变时，这个信号被发出，新的滑块值作为参数。

注意：属性值的通知程序信号。





18、[virtual protected] void QAbstractSlider::wheelEvent(QWheelEvent *e)

从QWidget :: wheelEvent（）重新实现。




