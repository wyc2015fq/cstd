# Qt开发：Qt Widgets模块——QDesktopWidget类 - 一世豁然的专栏 - CSDN博客





2018年07月02日 11:59:43[一世豁然](https://me.csdn.net/Explorer_day)阅读数：721








QDesktopWidget类提供对多头系统的屏幕信息的访问。



头文件：#include <QDesktopWidget>

qmake：QT += widgets

继承：QWinght

具有多个图形卡和监视器的系统可以管理可用作多个桌面或大型虚拟桌面的物理屏幕空间。

此类提供有关用户桌面的信息，例如其总大小，屏幕数，每个屏幕的几何形状，以及它们是配置为单独的桌面还是单个虚拟桌面。


由Qt提供的小工具使用这个类来将工具提示，菜单和对话框放置在他们的父应用程序或应用程序小部件的正确屏幕上。 应用程序可以使用此类来获取可用于保存窗口位置的信息，或将子窗口小部件和对话框放置在一个特定屏幕上。





**获取桌面小部件**


QApplication :: desktop（）函数用于获取QDesktopWidget的实例。


小部件的screenGeometry（）函数提供有关可用屏幕几何的信息。 可用屏幕的数量由screenCount返回，并且在添加或删除屏幕时发出screenCountChanged（）信号。 screenNumber（）返回特定点或小部件所在的屏幕编号。





**屏幕几何**


要获取特定屏幕的尺寸，请调用screenGeometry（）函数。 在某些桌面环境中，并非所有屏幕都可供应用程序使用; 例如，应用程序停靠栏或菜单栏可能会占用一些空间。 使用availableGeometry（）函数来获取应用程序的可用区域。


QDesktopWidget还继承QWidget属性width（）和height（），它们指定桌面的大小。 但是，对于具有多个屏幕的桌面，桌面的大小是所有屏幕大小的并集，因此不应使用width（）和height（）来计算要放置在其中一个屏幕上的小部件的大小。


在配置为将可用屏幕用作单个大型虚拟桌面的系统上，virtualDesktop属性将设置为true。 在这种情况下，小部件的大小通常是所有屏幕的边界矩形的大小。





**使用主屏幕**


对于应用程序，主窗口小部件所在的屏幕是主屏幕。 这存储在primaryScreen属性中。 在应用程序上下文中打开的所有窗口都应限制在主屏幕的边界上; 例如，如果在不同的屏幕上弹出一个对话框，或者分成两个屏幕，则会很不方便。


![](https://img-blog.csdn.net/20180702114509458?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


在上图中，Application One的主屏幕是屏幕0，App Two的主屏幕是屏幕1。








一、属性文档

1、primaryScreen : const int

该属性保存配置为系统主屏幕的屏幕的索引。


获取函数；

int primaryScreen() const


发送信号；

void primaryScreenChanged()





2、screenCount : const int



该属性保存系统当前可用的屏幕数量。

这个属性是在Qt 4.6中引入的。

获取函数；

int screenCount() const


发送信号；

void screenCountChanged(int newCount)





3、virtualDesktop : const bool



如果系统管理虚拟桌面中的可用屏幕，则此属性成立。

对于虚拟桌面，screen（）将始终返回相同的小部件。 虚拟桌面的大小就是这个桌面小部件的大小。

获取函数；

bool isVirtualDesktop() const








二、成员函数文档

1、const QRect QDesktopWidget::availableGeometry(int screen = -1) const

使用索引屏幕返回屏幕的可用几何图形。 根据平台决定的可用内容（例如，排除OS X上的停靠栏和菜单栏，或Windows上的任务栏），可以使用screenGeometry（）。 如果屏幕为-1，则使用默认屏幕。





2、const QRect QDesktopWidget::availableGeometry(const QWidget *widget) const

这是一个重载函数。

返回包含小部件的屏幕的可用几何图形。





3、const QRect QDesktopWidget::availableGeometry(const QPoint &p) const

这是一个重载函数。

返回包含p的屏幕的可用几何图形。





4、[signal] void QDesktopWidget::primaryScreenChanged()



主屏幕变化时会发出此信号。

注意：这并不意味着QDesktopWidget :: primaryScreen索引必然会有所不同，但现在它将引用新的主屏幕。

该功能在Qt 5.6中引入。

注意：属性primaryScreen的通知程序信号。




5、[virtual protected] void QDesktopWidget::resizeEvent(QResizeEvent *event)

从QWidget :: resizeEvent（）重新实现。





6、[signal] void QDesktopWidget::resized(int screen)

屏幕大小发生变化时发出此信号。





7、QWidget *QDesktopWidget::screen(int screen = -1)

返回表示带索引屏幕的屏幕的小部件（值为-1表示默认屏幕）。

如果系统使用虚拟桌面，则返回的小部件将具有整个虚拟桌面的几何图形; 即限制每个屏幕。





8、[signal] void QDesktopWidget::screenCountChanged(int newCount)



当屏幕数量变为newCount时发出此信号。

该功能在Qt 4.6中引入。

注意：属性screenCount的通知程序信号。




9、const QRect QDesktopWidget::screenGeometry(int screen = -1) const

用索引屏幕返回屏幕的几何图形。 如果屏幕为-1，则使用默认屏幕。





10、const QRect QDesktopWidget::screenGeometry(const QWidget *widget) const

这是一个重载函数。


返回包含小部件的屏幕的几何图形。





11、const QRect QDesktopWidget::screenGeometry(const QPoint &p) const



这是一个过载功能。

返回包含p的屏幕几何。




12、int QDesktopWidget::screenNumber(const QWidget *widget = Q_NULLPTR) const

返回包含窗口小部件最大部分的屏幕索引，如果窗口小部件不在屏幕上，则返回-1。





13、int QDesktopWidget::screenNumber(const QPoint &point) const

这是一个重载函数。
返回包含该点的屏幕的索引，或距该点最短距离的屏幕。




