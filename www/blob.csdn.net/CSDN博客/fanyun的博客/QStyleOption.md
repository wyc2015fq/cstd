# QStyleOption - fanyun的博客 - CSDN博客
2017年12月28日 22:08:15[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：395
QStyleOption类存储由QStyle函数使用的参数。
QStyle选项及其子类包含QStyle函数绘制图形元素所需的所有信息。
由于性能原因，成员函数很少，并且成员变量的访问是直接的(即，使用。或->运算符)。这种低级感觉使结构易于使用，并强调这些只是样式函数使用的参数。
QStyle函数的调用方通常在堆栈上创建QStyleOption对象。这与Qt对QString、QPalette和QColor等类型的隐式共享的广泛使用相结合，可确保无需进行内存分配。
下面的代码片段演示如何使用特定的QStyleOption子类来绘制按钮:
```cpp
void MyPushButton::paintEvent(QPaintEvent *)
{
    QStyleOptionButton option;
    option.initFrom(this);
    option.state = isDown() ? QStyle::State_Sunken : QStyle::State_Raised;
    if (isDefault())
        option.features |= QStyleOptionButton::DefaultButton;
    option.text = text();
    option.icon = icon();
    QPainter painter(this);
    style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);
}
```
在我们的示例中，控件是QStyle : : CE _ button，根据QStyle : : drawControl ()文档，相应的类是QStyle选项按钮。
重新实现采用QStyleOption参数的QStyle函数时，通常需要将QStyleOption转换为子类。为了安全起见，可以使用qstyleoption _ cast ()来确保指针类型正确。
如果选项指向的对象类型不正确，则qstyleoption _ cast ()函数将返回0。
```cpp
void MyStyle::drawPrimitive(PrimitiveElement element,
                            const QStyleOption *option,
                            QPainter *painter,
                            const QWidget *widget)
{
    if (element == PE_FrameFocusRect) {
        const QStyleOptionFocusRect *focusRectOption =
                qstyleoption_cast<const QStyleOptionFocusRect *>(option);
        if (focusRectOption) {
            // ...
        }
    }
    // ...
}
```
http://doc.qt.io/qt-5/qstyleoption.html
