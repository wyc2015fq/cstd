# Qt开发：Qt Widgets模块——QCalendarWidget类 - 一世豁然的专栏 - CSDN博客





2018年05月21日 10:49:45[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1493








QCalendarWidget类提供了一个基于每月的日历小部件，允许用户选择一个日期。



头文件：#include <QCalendarWidget> 

qmake：QT += widgets

继承：QWidget

![](https://img-blog.csdn.net/20180519190234697)


该小部件使用当前的月份和年份进行初始化，但QCalendarWidget提供了几个公开插槽来更改显示的年份和月份。


默认情况下，选择今天的日期，用户可以使用鼠标和键盘选择日期。 使用selectedDate（）函数可以检索当前选择的日期。 通过设置minimumDate和maximumDate属性，可以将用户选择限制到给定的日期范围。 或者，可以使用setDateRange（）便捷槽一次性设置这两个属性。 将selectionMode属性设置为NoSelection以禁止用户选择。 请注意，还可以使用setSelectedDate（）槽以编程方式选择日期。


可以分别使用monthShown（）和yearShown（）函数检索当前显示的月份和年份。


新创建的日历小部件使用缩短的日期名称，周六和周日都标记为红色。 日历网格不可见。 显示星期编号，第一个列日是日历区域设置的一周的第一天。


通过将horizontalHeaderFormat属性设置为QCalendarWidget :: SingleLetterDayNames，可以将日期的符号更改为单个字母缩写（“M”代表“Monday”）。 将相同的属性设置为QCalendarWidget :: LongDayNames会使标题显示完整的日期名称。 可以通过将verticalHeaderFormat属性设置为QCalendarWidget :: NoVerticalHeader来删除周数。 通过使用setGridVisible（）函数将gridVisible属性设置为true，可以打开日历网格：


![](https://img-blog.csdn.net/20180521094016266)


最后，可以使用setFirstDayOfWeek（）函数更改第一列中的日期。


QCalendarWidget类还提供了三个信号，selectionChanged（），activated（）和currentPageChanged（），可以响应用户交互。


通过设置QTextCharFormat的某些特殊的工作日，特殊的日期或头文件的渲染，可以在很大程度上定制头文件，工作日或单日的渲染。


日历小部件仅使用QTextCharFormat中的一部分属性。 目前，前景，背景和字体属性用于确定小部件中单个单元格的渲染。








一、成员类型

1、enum QCalendarWidget::HorizontalHeaderFormat

这个枚举类型定义了水平头可显示的各种格式。



|内容|值|描述|
|----|----|----|
|QCalendarWidget::SingleLetterDayNames|1|标题显示日期名称的单个字母缩写（例如，星期一为M）。|
|QCalendarWidget::ShortDayNames|2|标题显示日期名称的缩写（例如星期一的星期一）。|
|QCalendarWidget::LongDayNames|3|标题显示完整的日期名称（例如星期一）。|
|QCalendarWidget::NoHorizontalHeader|0|标题被隐藏。|

2、enum QCalendarWidget::SelectionMode

这个枚举描述了提供给用户选择日历中日期的选择类型。



|内容|值|描述|
|----|----|----|
|QCalendarWidget::NoSelection|0|无法选择日期。|
|QCalendarWidget::SingleSelection|1|可以选择单个日期。|

3、enum QCalendarWidget::VerticalHeaderFormat

此枚举类型定义了垂直标题可以显示的各种格式。



|内容|值|描述|
|----|----|----|
|QCalendarWidget::ISOWeekNumbers|1|标题显示QDate :: weekNumber（）所描述的ISO星期编号。|
|QCalendarWidget::NoVerticalHeader|0|标题被隐藏。|




二、属性

1、dateEditAcceptDelay : int

该属性保留在其内容被接受之前显示非活动日期编辑的时间。


如果启用日历小部件的日期编辑，则此属性指定日期编辑在最近的用户输入后保持打开的时间量（以毫秒为单位）。 一旦这段时间过去，日期编辑中指定的日期将被接受，弹出窗口将被关闭。


默认情况下，延迟定义为1500毫秒（1.5秒）。

这个属性是在Qt 4.3中引入的。


访问函数；

```cpp
int dateEditAcceptDelay() const
void setDateEditAcceptDelay(int delay)
```




2、dateEditEnabled : bool

此属性保持是否启用日期编辑弹出窗口。


如果启用此属性，如果日历窗口小部件具有焦点，则按下非修饰键会导致日期编辑弹出，从而允许用户以当前语言环境指定的格式指定日期。


默认情况下，此属性已启用。


日期编辑的外观比QDateEdit简单，但允许用户使用左右光标键在字段之间导航，使用向上和向下光标键递增和递减各个字段，并直接使用数字键输入值。

这个属性是在Qt 4.3中引入的。


访问函数；

```cpp
bool isDateEditEnabled() const
void setDateEditEnabled(bool enable)
```




3、firstDayOfWeek : Qt::DayOfWeek

此属性拥有一个值，用于标识第一列中显示的日期。

默认情况下，第一列中显示的日期是日历区域设置的第一天。

访问函数；

```cpp
Qt::DayOfWeek firstDayOfWeek() const
void setFirstDayOfWeek(Qt::DayOfWeek dayOfWeek)
```




4、gridVisible : bool

该属性保持是否显示表格。


![](https://img-blog.csdn.net/20180521100056331)


访问函数；

```cpp
bool isGridVisible() const
void setGridVisible(bool show)
```




5、horizontalHeaderFormat : HorizontalHeaderFormat

该属性保存水平头的格式。

缺省值是QCalendarWidget :: ShortDayNames。


访问函数；

```cpp
HorizontalHeaderFormat horizontalHeaderFormat() const
void setHorizontalHeaderFormat(HorizontalHeaderFormat format)
```




6、maximumDate : QDate

该属性保存当前指定日期范围的最大日期。

用户将无法选择当前设置的最大日期之后的日期。


![](https://img-blog.csdn.net/20180521100446382)


默认情况下，最大日期是QDate类可以处理的最后一天。

设置最大日期时，如果选择范围无效，则调整minimumDate和selectedDate属性。 如果提供的日期不是有效的QDate对象，则setMaximumDate（）函数不会执行任何操作。


访问函数；

```cpp
QDate maximumDate() const
void setMaximumDate(const QDate &date)
```




7、minimumDate : QDate

该属性保存当前指定日期范围的最小日期。

用户将无法选择当前设定的最短日期之前的日期。


![](https://img-blog.csdn.net/20180521100647725)


默认情况下，最小日期是QDate类可以处理的最早日期。

设置最短日期时，如果选择范围无效，则调整maximumDate和selectedDate属性。 如果提供的日期不是有效的QDate对象，则setMinimumDate（）函数将不执行任何操作。


访问函数；

```cpp
QDate minimumDate() const
void setMinimumDate(const QDate &date)
```




8、navigationBarVisible : bool



该属性保持是否显示导航栏。

当此属性为true（默认）时，下一个月，上个月，月份选择，年份选择控件显示在最上面。

当属性设置为false时，这些控件是隐藏的。

这个属性是在Qt 4.3中引入的。

访问函数；

```cpp
bool isNavigationBarVisible() const
void setNavigationBarVisible(bool visible)
```




9、selectedDate : QDate

该属性保存当前选择的日期。

所选日期必须在由minimumDate和maximumDate属性指定的日期范围内。 默认情况下，所选日期是当前日期。


访问函数；

```cpp
QDate selectedDate() const
void setSelectedDate(const QDate &date)
```




10、selectionMode : SelectionMode



该属性拥有用户可以在日历中进行选择的类型。

当此属性设置为SingleSelection时，用户可以使用鼠标或键盘在最小和最大允许日期内选择一个日期。

当该属性设置为NoSelection时，用户将无法选择日期，但仍然可以通过编程方式进行选择。 请注意，将属性设置为NoSelection时选择的日期仍将是日历的选定日期。

默认值是SingleSelection。

访问函数；

```cpp
SelectionMode selectionMode() const
void setSelectionMode(SelectionMode mode)
```



11、verticalHeaderFormat : VerticalHeaderFormat

该属性保存垂直标题的格式。

缺省值是QCalendarWidget :: ISOWeekNumber。


访问函数；

```cpp
VerticalHeaderFormat verticalHeaderFormat() const
void setVerticalHeaderFormat(VerticalHeaderFormat format)
```






三、成员函数

1、QCalendarWidget::QCalendarWidget(QWidget *parent = Q_NULLPTR)

用给定的父项构造一个日历小部件。

该小部件使用当前的月份和年份进行初始化，当前选择的日期为今天。





2、QCalendarWidget::~QCalendarWidget()

销毁日历小部件。





3、[signal] void QCalendarWidget::activated(const QDate &date)

只要用户按下Return或Enter键或双击日历小部件中的日期，就会发出此信号。





4、[signal] void QCalendarWidget::clicked(const QDate &date)

单击鼠标按钮时会发出此信号。 鼠标点击的日期由日期指定。 信号仅在有效日期点击时发出，例如，日期不在minimumDate（）和maximumDate（）之外。 如果选择模式为NoSelection，则不会发射此信号。





5、[signal] void QCalendarWidget::currentPageChanged(int year, int month)

当前显示的月份发生变化时会发出此信号。 新年和月份作为参数传递。





6、QMap<QDate, QTextCharFormat> QCalendarWidget::dateTextFormat() const

从QDate返回一个QMap到QTextCharFormat，显示所有使用改变其渲染的特殊格式的日期。





7、QTextCharFormat QCalendarWidget::dateTextFormat(const QDate &date) const

返回日期的QTextCharFormat。 如果日期没有专门呈现，char格式可以为空。





8、[virtual protected] bool QCalendarWidget::event(QEvent *event)

从QObject :: event（）重新实现。





9、[virtual protected] bool QCalendarWidget::eventFilter(QObject *watched, QEvent *event)

从QObject :: eventFilter（）重新实现。





10、QTextCharFormat QCalendarWidget::headerTextFormat() const

返回呈现标题的文本字符格式。





11、[virtual protected] void QCalendarWidget::keyPressEvent(QKeyEvent *event)

从QWidget :: keyPressEvent（）重新实现。





12、[virtual] QSize QCalendarWidget::minimumSizeHint() const

从QWidget :: minimumSizeHint（）重新实现。





13、int QCalendarWidget::monthShown() const

返回当前显示的月份。 月份编号从1到12。





14、[virtual protected] void QCalendarWidget::mousePressEvent(QMouseEvent *event)

从QWidget :: mousePressEvent（）重新实现。





15、[virtual protected] void QCalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const

使用给定的画家和矩形绘制给定日期指定的单元格。





16、[virtual protected] void QCalendarWidget::resizeEvent(QResizeEvent *event)

从QWidget :: resizeEvent（）重新实现。





17、[signal] void QCalendarWidget::selectionChanged()

该信号在当前选择的日期改变时发出。

用户可以使用鼠标或键盘或使用setSelectedDate（）由程序员更改当前选择的日期。





18、[slot] void QCalendarWidget::setCurrentPage(int year, int month)

显示给定年份的给定月份而不更改所选日期。 使用setSelectedDate（）函数更改所选日期。

当前显示的月份和年份可分别使用monthShown（）和yearShown（）函数进行检索。





19、[slot] void QCalendarWidget::setDateRange(const QDate &min, const QDate &max)

通过设置minimumDate和maximumDate属性来定义日期范围。


日期范围限制用户选择，即用户只能选择指定日期范围内的日期。 注意

```cpp
QCalendarWidget *calendar;

  calendar->setDateRange(min, max);
```

是类似于

```cpp
QCalendarWidget *calendar;

  calendar->setMinimumDate(min);
  calendar->setMaximumDate(max);
```

如果min或max参数不是有效的QDate对象，则此函数不执行任何操作。




20、void QCalendarWidget::setDateTextFormat(const QDate &date, const QTextCharFormat &format)

设置用于将给定日期呈现为格式指定的格式。

如果日期为空，则清除所有日期格式。





21、void QCalendarWidget::setHeaderTextFormat(const QTextCharFormat &format)

设置用于呈现标题格式的文本字符格式。 如果您还设置星期几文本格式，则此格式的前景色和背景色将优先于标题的格式。 其他格式信息仍将由标题格式决定。





22、void QCalendarWidget::setWeekdayTextFormat(Qt::DayOfWeek dayOfWeek, const QTextCharFormat &format)

设置文本字符格式以便在dayOfWeek格式中对星期几进行渲染。 在前景色和背景色的情况下，格式优先于标题格式。 其他文本格式信息取自标题格式。





23、[slot] void QCalendarWidget::showNextMonth()

显示相对于当前显示的月份的下个月。 请注意，所选日期不会更改。





24、[slot] void QCalendarWidget::showNextYear()

显示当前显示的与当前显示年份相关的下一个月的月份。 请注意，所选日期不会更改。





25、[slot] void QCalendarWidget::showPreviousMonth()

显示相对于当前显示的月份的上个月。 请注意，所选日期不会更改。





26、[slot] void QCalendarWidget::showPreviousYear()

显示当前显示的与当前显示年份相关的前一年的月份。 请注意，所选日期不会更改。





27、[slot] void QCalendarWidget::showSelectedDate()

显示所选日期的月份。





28、[slot] void QCalendarWidget::showToday()

显示今天的月份。





29、[virtual] QSize QCalendarWidget::sizeHint() const

从QWidget :: sizeHint（）重新实现。





30、[protected] void QCalendarWidget::updateCell(const QDate &date)

更新给定日期指定的单元格，除非禁用更新或隐藏单元格。

这个函数是在Qt 4.4中引入的。





31、[protected] void QCalendarWidget::updateCells()

更新所有可见单元格，除非更新被禁用。

这个函数是在Qt 4.4中引入的。





32、QTextCharFormat QCalendarWidget::weekdayTextFormat(Qt::DayOfWeek dayOfWeek) const

返回文本字符格式，用于呈现dayOfWeek中的星期几。





33、int QCalendarWidget::yearShown() const

返回当前显示的月份的年份。 月份编号从1到12。




