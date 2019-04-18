# Qt开发：Qt Widgets模块——QColorDialog类 - 一世豁然的专栏 - CSDN博客





2018年05月21日 18:44:46[一世豁然](https://me.csdn.net/Explorer_day)阅读数：328








QColorDialog类提供了一个用于指定颜色的对话框小部件。



头文件：#include <QCheckBox> 

qmake：QT += widgets

继承：QDialog

颜色对话框的功能是让用户选择颜色。 例如，您可以在绘图程序中使用它来允许用户设置笔刷颜色。


静态函数提供模态颜色对话框。


静态getColor（）函数显示对话框，并允许用户指定颜色。 此函数也可用于让用户选择透明度级别的颜色：将ShowAlphaChannel选项作为附加参数传递。


用户可以存储customCount（）不同的自定义颜色。 自定义颜色由所有颜色对话框共享，并在程序执行过程中记住。 使用setCustomColor（）来设置自定义颜色，并使用customColor（）来获取它们。


当按下“拾取屏幕颜色”按钮时，光标变成一个横向扫描并且屏幕上的颜色被扫描。 用户可以通过单击鼠标或Enter按钮来选取一个。 按Escape可恢复进入此模式前所选的最后一种颜色。


标准对话框示例显示了如何使用QColorDialog以及其他内置的Qt对话框。


![](https://img-blog.csdn.net/20180521181134412)








一、成员类型

1、enum QColorDialog::ColorDialogOption


flags QColorDialog::ColorDialogOptions

此枚举指定影响颜色对话框的外观和感觉的各种选项。


|内容|值|描述|
|----|----|----|
|QColorDialog::ShowAlphaChannel|0x00000001|允许用户选择颜色的alpha分量。|
|QColorDialog::NoButtons|0x00000002|Don't display OK and Cancel buttons. (Useful for "live dialogs".)|
|QColorDialog::DontUseNativeDialog|0x00000004|使用Qt的标准颜色对话框而不是操作系统本地颜色对话框。|
这个枚举是在Qt 4.5中引入或修改的。
ColorDialogOptions类型是QFlags <ColorDialogOption>的typedef。 它存储ColorDialogOption值的或组合。







二、属性

1、currentColor : QColor

该属性在对话框中保存当前选定的颜色。


访问函数；

```cpp
QColor currentColor() const
void setCurrentColor(const QColor &color)
```

通知信号；

```cpp
void currentColorChanged(const QColor &color)
```



2、options : ColorDialogOptions



该属性包含影响对话框外观和感觉的各种选项。

默认情况下，所有选项都被禁用。

显示对话框之前应该设置选项。 在对话框可见时设置它们并不保证对对话框立即生效（取决于选项和平台）。

访问函数；

```cpp
ColorDialogOptions options() const
void setOptions(ColorDialogOptions options)
```





三、成员函数；

1、QColorDialog::QColorDialog(QWidget *parent = Q_NULLPTR)

用给定的父项构造一个颜色对话框。

这个函数是在Qt 4.5中引入的。





2、QColorDialog::QColorDialog(const QColor &initial, QWidget *parent = Q_NULLPTR)

用给定的父亲和指定的初始颜色构造一个颜色对话框。

这个函数是在Qt 4.5中引入的。





3、QColorDialog::~QColorDialog()

销毁颜色对话框。





4、[virtual protected] void QColorDialog::changeEvent(QEvent *e)

从QWidget :: changeEvent（）重新实现。





5、[signal] void QColorDialog::colorSelected(const QColor &color)

该信号在用户点击确定后选择要使用的颜色。 所选颜色由颜色指定。





6、[signal] void QColorDialog::currentColorChanged(const QColor &color)

只要当前颜色在对话框中改变，就会发出此信号。 当前颜色由颜色指定。

注意：属性currentColor的通知程序信号。





7、[static] QColor QColorDialog::customColor(int index)

作为QColor值返回给定索引处的自定义颜色。

这个函数是在Qt 4.5中引入的。





8、[static] int QColorDialog::customCount()

返回QColorDialog支持的自定义颜色的数量。 所有颜色对话框共享相同的自定义颜色。





9、[virtual protected] void QColorDialog::done(int result)

从QDialog :: done（）重新实现。

关闭对话框并设置其结果代码。 如果此对话框使用exec（）显示，done（）会导致本地事件循环结束，而exec（）将返回结果。





10、[static] QColor QColorDialog::getColor(const QColor &initial = Qt::white, QWidget *parent = Q_NULLPTR, const QString &title = QString(), ColorDialogOptions options = ColorDialogOptions())



用给定的窗口标题弹出一个模式颜色对话框（如果没有指定，则选择“选择颜色”），让用户选择一种颜色并返回该颜色。 颜色初始设置为初始。 该对话框是父母的孩子。 如果用户取消对话框，它将返回一个无效（参见QColor :: isValid（））颜色。

options参数允许您自定义对话框。

这个函数是在Qt 4.5中引入的。




11、void QColorDialog::open(QObject *receiver, const char *member)



这是一个重载函数。

打开对话框并将其colorSelected（）信号连接到接收器和成员指定的插槽。

关闭对话框时，信号将从插槽中断开。

这个函数是在Qt 4.5中引入的。




12、QColor QColorDialog::selectedColor() const

通过单击确定或等效按钮返回用户选择的颜色。

注意：由于用户在最终选择要使用的颜色之前可以选择不同的颜色，因此该颜色与currentColor属性所具有的颜色并不总是相同。





13、[static] void QColorDialog::setCustomColor(int index, QColor color)

将索引处的自定义颜色设置为QColor颜色值。

注意：此功能不适用于Mac OS X平台上的本机颜色对话框。 如果您仍然需要此功能，请使用QColorDialog :: DontUseNativeDialog选项。





14、void QColorDialog::setOption(ColorDialogOption option, bool on = true)

如果on为true，则将给定选项设置为启用; 否则，清除给定的选项。





15、[static] void QColorDialog::setStandardColor(int index, QColor color)

将索引处的标准颜色设置为QColor颜色值。

注意：此功能不适用于Mac OS X平台上的本机颜色对话框。 如果您仍然需要此功能，请使用QColorDialog :: DontUseNativeDialog选项。





16、[virtual] void QColorDialog::setVisible(bool visible)

从QWidget :: setVisible（）重新实现。

更改对话框的可见性。 如果可见，则显示对话框; 否则，它是隐藏的。





17、[static] QColor QColorDialog::standardColor(int index)

作为QColor值返回给定索引处的标准颜色。

这个函数是在Qt 5.0中引入的。





18、bool QColorDialog::testOption(ColorDialogOption option) const

如果给定选项已启用，则返回true; 否则，返回false。

这个函数是在Qt 4.5中引入的。




