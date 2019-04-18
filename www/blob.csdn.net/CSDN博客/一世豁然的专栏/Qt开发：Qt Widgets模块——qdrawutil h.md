# Qt开发：Qt Widgets模块——qdrawutil.h - 一世豁然的专栏 - CSDN博客





2018年05月15日 09:14:40[一世豁然](https://me.csdn.net/Explorer_day)阅读数：172








1、void qDrawBorderPixmap(QPainter *painter, const QRect &target, const QMargins &margins, const QPixmap &pixmap)

这是一个重载函数。


qDrawBorderPixmap函数用于将一个像素图绘制到矩形的边缘。


使用给定的画板将给定的像素图绘制到给定的目标矩形中。 像素图将被分成九个部分并根据边距结构进行绘制。


此函数在Qt4.6中介绍。




2、void qDrawPlainRect(QPainter *painter, int x, int y, int width, int height, const QColor &lineColor, int lineWidth = 1, const QBrush *fill = Q_NULLPTR)

使用指定的画板lineColor和lineWidth绘制以（x，y）开始的具有给定宽度和高度的纯色矩形。 除非填充为0，否则矩形的内部填充填充刷。


警告：此函数不会查看QWidget :: style（）或QApplication :: style（）。 使用QStyle中的绘图函数来制作遵循当前GUI风格的小部件。


或者，您可以使用QFrame小部件并应用QFrame :: setFrameStyle（）函数来显示一个普通的矩形：


```cpp
QFrame frame:
  frame.setFrameStyle(QFrame::Box | QFrame::Plain);
```




3、void qDrawPlainRect(QPainter *painter, const QRect &rect, const QColor &lineColor, int lineWidth = 1, const QBrush *fill = Q_NULLPTR)

此为重载函数。

使用给定的画家lineColor和lineWidth绘制由rect指定的纯矩形。 除非填充为0，否则矩形的内部填充填充刷。

警告：此函数不会查看QWidget :: style（）或QApplication :: style（）。 使用QStyle中的绘图函数来制作遵循当前GUI风格的小部件。

或者，您可以使用QFrame小部件并应用QFrame :: setFrameStyle（）函数来显示一个普通的矩形：



```cpp
QFrame frame:
  frame.setFrameStyle(QFrame::Box | QFrame::Plain);
```




4、void qDrawShadeLine(QPainter *painter, int x1, int y1, int x2, int y2, const QPalette &palette, bool sunken = true, int lineWidth = 1, int midLineWidth = 0)

使用给定的画板绘制水平（y1 == y2）或垂直（x1 == x2）阴影线。 请注意，如果y1！= y2且x1！= x2（即，该线既不是水平也不是垂直），则不会绘制任何图形。


提供的调色板指定着色（浅色，深色和中间色）。 给定的lineWidth指定每一行的行宽; 它不是总线宽度。 给定的midLineWidth指定以QPalette :: mid（）颜色绘制的中间线的宽度。


如果凹陷是真的，则线条会出现凹陷，否则会凸起。


警告：此函数不会查看QWidget :: style（）或QApplication :: style（）。 使用QStyle中的绘图函数来制作遵循当前GUI风格的小部件。


或者，您可以使用QFrame小部件并应用QFrame :: setFrameStyle（）函数来显示阴影线：


```cpp
QFrame frame:
  frame.setFrameStyle(QFrame::HLine | QFrame::Sunken);
```




5、void qDrawShadeLine(QPainter *painter, const QPoint &p1, const QPoint &p2, const QPalette &palette, bool sunken = true, int lineWidth = 1, int midLineWidth = 0)

此为重载函数。

使用给定的画板在p1和p2之间绘制水平或垂直阴影线。 请注意，如果点之间的线不是水平的或垂直的，则不会绘制任何东西。


提供的调色板指定着色（浅色，深色和中间色）。 给定的lineWidth指定每一行的行宽; 它不是总线宽度。 给定的midLineWidth指定以QPalette :: mid（）颜色绘制的中间线的宽度。


如果凹陷是真的，则线条会出现凹陷，否则会凸起。


警告：此函数不会查看QWidget :: style（）或QApplication :: style（）。 使用QStyle中的绘图函数来制作遵循当前GUI风格的小部件。


或者，您可以使用QFrame小部件并应用QFrame :: setFrameStyle（）函数来显示阴影线：


```cpp
QFrame frame:
  frame.setFrameStyle(QFrame::HLine | QFrame::Sunken);
```




6、void qDrawShadePanel(QPainter *painter, int x, int y, int width, int height, const QPalette &palette, bool sunken = false, int lineWidth = 1, const QBrush *fill = Q_NULLPTR)

使用提供的画板和给定的lineWidth绘制以（x，y）开始的具有给定宽度和高度的阴影面板。


给定的调色板指定着色（浅色，深色和中间色）。 除非填充为0，否则面板的内部填充填充刷。


如果凹陷是真的，面板会出现凹陷，否则会凸起。


警告：此函数不会查看QWidget :: style（）或QApplication :: style（）。 使用QStyle中的绘图函数来制作遵循当前GUI风格的小部件。


或者，您可以使用QFrame小部件并应用QFrame :: setFrameStyle（）函数来显示阴影面板：


```cpp
QFrame frame:
  frame.setFrameStyle( QFrame::Panel | QFrame::Sunken);
```




7、void qDrawShadePanel(QPainter *painter, const QRect &rect, const QPalette &palette, bool sunken = false, int lineWidth = 1, const QBrush *fill = Q_NULLPTR)

此为重载函数。

使用给定的画板和给定的lineWidth，在由rect指定的矩形中绘制阴影面板。


给定的调色板指定着色（浅色，深色和中间色）。 除非填充为0，否则面板的内部填充填充刷。


如果凹陷是真的，面板会出现凹陷，否则会凸起。


警告：此函数不会查看QWidget :: style（）或QApplication :: style（）。 使用QStyle中的绘图函数来制作遵循当前GUI风格的小部件。


或者，您可以使用QFrame小部件并应用QFrame :: setFrameStyle（）函数来显示阴影面板：


```cpp
QFrame frame:
  frame.setFrameStyle( QFrame::Panel | QFrame::Sunken);
```




8、void qDrawShadeRect(QPainter *painter, int x, int y, int width, int height, const QPalette &palette, bool sunken = false, int lineWidth = 1, int midLineWidth = 0, const QBrush *fill = Q_NULLPTR)

使用提供的画板绘制以给定宽度和高度开始的（x，y）阴影矩形。


提供调色板指定了着色的颜色（浅色，深色和中间色，给定的lineWidth指定了每一行的线宽;它不是总线宽度，midLineWidth指定在QPalette中绘制的中间线的宽度： ：mid（）颜色。除非填充为0，否则矩形的内部填充填充刷。


如果凹陷是真的，面板会出现凹陷，否则会凸起。


警告：此函数不会查看QWidget :: style（）或QApplication :: style（）。 使用QStyle中的绘图函数来制作遵循当前GUI风格的小部件。


或者，您可以使用QFrame小部件并应用QFrame :: setFrameStyle（）函数来显示阴影矩形：


```cpp
QFrame frame:
  frame.setFrameStyle(QFrame::Box | QFrame::Raised);
```




9、void qDrawShadeRect(QPainter *painter, const QRect &rect, const QPalette &palette, bool sunken = false, int lineWidth = 1, int midLineWidth = 0, const QBrush *fill = Q_NULLPTR)

此为重载函数。

使用给定的画家绘制由rect指定的阴影矩形。


提供调色板指定了着色的颜色（浅色，深色和中间色，给定的lineWidth指定了每一行的线宽;它不是总线宽度，midLineWidth指定在QPalette中绘制的中间线的宽度： ：mid（）颜色。除非填充为0，否则矩形的内部填充填充刷。




如果凹陷是真的，面板会出现凹陷，否则会凸起。


警告：此函数不会查看QWidget :: style（）或QApplication :: style（）。 使用QStyle中的绘图函数来制作遵循当前GUI风格的小部件。

或者，您可以使用QFrame小部件并应用QFrame :: setFrameStyle（）函数来显示阴影矩形：

```cpp
QFrame frame:
  frame.setFrameStyle(QFrame::Box | QFrame::Raised)
```




10、void QDrawWinButton（QPainter * painter，int x，int y，int width，int height，const QPalette＆palette，bool sunken = false，const QBrush * fill = Q_NULLPTR）

绘制由给定点（x，y），宽度和高度指定的Windows风格的按钮，使用提供的画家，线宽为2像素。除非填充为0，否则按钮的内部填充填充画笔。


给定的调色板指定着色（浅色，深色和中间色）。


如果凹陷是真的，按钮会出现凹陷，否则会凸起。


警告：此函数不会查看QWidget :: style（）或QApplication :: style（） - >使用QStyle中的绘图函数来制作遵循当前GUI样式的小部件。





11、void qDrawWinButton(QPainter *painter, const QRect &rect, const QPalette &palette, bool sunken = false, const QBrush *fill = Q_NULLPTR)

此为重载函数。

在由rect指定的矩形中使用给定画家的线宽2像素绘制Windows样式按钮。 除非填充为0，否则按钮的内部填充有填充刷。


给定的调色板指定着色（浅色，深色和中间色）。


如果凹陷是真的，按钮会出现凹陷，否则会凸起。


警告：此函数不会查看QWidget :: style（）或QApplication :: style（） - >使用QStyle中的绘图函数来制作遵循当前GUI样式的小部件。





12、void qDrawWinPanel(QPainter *painter, int x, int y, int width, int height, const QPalette &palette, bool sunken = false, const QBrush *fill = Q_NULLPTR)

使用提供的画家绘制由给定点（x，y），宽度和高度指定的Windows样式面板，线宽为2像素。 除非填充为0，否则按钮的内部填充有填充刷。


给定的调色板指定了着色颜色。 如果凹陷是真的，面板会出现凹陷，否则会凸起。


警告：此函数不会查看QWidget :: style（）或QApplication :: style（）。 使用QStyle中的绘图函数来制作遵循当前GUI风格的小部件。


或者，您可以使用QFrame小部件并应用QFrame :: setFrameStyle（）函数来显示阴影面板：


```cpp
QFrame frame:
  frame.setFrameStyle(QFrame::WinPanel | QFrame::Raised);
```




