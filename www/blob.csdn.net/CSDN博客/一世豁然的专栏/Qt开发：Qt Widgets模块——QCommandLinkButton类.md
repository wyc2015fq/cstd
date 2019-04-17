# Qt开发：Qt Widgets模块——QCommandLinkButton类 - 一世豁然的专栏 - CSDN博客





2018年05月23日 09:09:46[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1100








QCommandLinkButton小部件提供了一个Vista风格的命令链接按钮。



头文件：#include <QColumnView>

qmake：QT += widgets

继承：QPushButton

该命令链接是Windows Vista引入的新控件。 它的用途类似于单选按钮，因为它用于在一组互斥选项中进行选择。 命令链接按钮不应该单独使用，而是作为向导和对话框中单选按钮的替代选项，并使“下一个”按钮变为冗余。 外观与平面按钮的外观大致相似，但除了正常的按钮文字外，它还允许使用描述性文字。 默认情况下，它还会带有一个箭头图标，表示按下控件将打开另一个窗口或页面。







一、属性

1、description : QString

该属性拥有一个描述性标签来补充按钮文本。

设置此属性将在按钮上设置一个描述性文本，补充文本标签。 这通常会以比主文本更小的字体显示。


访问函数；

```cpp
QString description() const
void setDescription(const QString &description)
```




2、flat : bool

此属性拥有此属性，用于确定按钮是显示为平板还是边框。

默认情况下，该属性设置为false。


访问函数；

```cpp
bool isFlat() const
void setFlat(bool)
```







二、成员函数

1、QCommandLinkButton::QCommandLinkButton(QWidget *parent = Q_NULLPTR)

构造一个没有文本和父项的命令链接。





2、QCommandLinkButton::QCommandLinkButton(const QString &text, QWidget *parent = Q_NULLPTR)

构造与父父代和文本文本的命令链接。





3、QCommandLinkButton::QCommandLinkButton(const QString &text, const QString &description, QWidget *parent = Q_NULLPTR)

构造一个包含文本，描述和父项的命令链接。





4、QCommandLinkButton::~QCommandLinkButton()

析构函数。





5、[virtual protected] bool QCommandLinkButton::event(QEvent *e)

从QObject :: event（）重新实现。





6、[virtual protected] int QCommandLinkButton::heightForWidth(int width) const

从QWidget :: heightForWidth（）重新实现。





7、[virtual protected] QSize QCommandLinkButton::minimumSizeHint() const

从QWidget :: minimumSizeHint（）重新实现。





8、[virtual protected] void QCommandLinkButton::paintEvent(QPaintEvent *)

从QWidget :: paintEvent（）重新实现。





9、[virtual protected] QSize QCommandLinkButton::sizeHint() const

从QWidget :: sizeHint（）重新实现。




