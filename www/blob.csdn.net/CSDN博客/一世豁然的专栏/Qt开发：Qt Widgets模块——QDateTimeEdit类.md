# Qt开发：Qt Widgets模块——QDateTimeEdit类 - 一世豁然的专栏 - CSDN博客





2018年05月31日 09:21:36[一世豁然](https://me.csdn.net/Explorer_day)阅读数：955








QDateTimeEdit类提供了一个用于编辑日期和时间的小部件。



头文件：#include <QDateTimeEdit>

qmake：QT += widgets

继承：QAbstractSpinBox

被继承：QDateEdit和QTimeEdit

QDateTimeEdit允许用户通过使用键盘或箭头键来增加和减少日期和时间值来编辑日期。 箭头键可用于在QDateTimeEdit框中从一个部分移动到另一个部分。 日期和时间按照设定的格式出现; 请参阅setDisplayFormat（）。


```cpp
QDateTimeEdit *dateEdit = new QDateTimeEdit(QDate::currentDate());
  dateEdit->setMinimumDate(QDate::currentDate().addDays(-365));
  dateEdit->setMaximumDate(QDate::currentDate().addDays(365));
  dateEdit->setDisplayFormat("yyyy.MM.dd");
```

在这里，我们创建了一个新的QDateTimeEdit对象，使用今天的日期进行初始化，并将有效日期范围限制在今天正负365天。 我们已将订单设置为月，日，年。

QDateTimeEdit的最小值是1752年9月14日。您可以通过调用setMinimumDate（）来更改此值，并考虑到QDate的最小值是1月2日4713BC。


其他有用的函数是setMaximumDate（），setMinimumTime（）和setMaximumTime（）





**使用弹出式日历小部件**


QDateTimeEdit可以配置为允许使用QCalendarWidget来选择日期。 这是通过设置calendarPopup属性来启用的。 另外，您可以通过调用setCalendarWidget（）函数来提供自定义日历小部件，以用作日历弹出窗口。 可以使用calendarWidget（）检索现有的日历小部件。


![](https://img-blog.csdn.net/20180531082917283)


Windows Vista小部件样式中显示的日期时间编辑小部件。


![](https://img-blog.csdn.net/20180531082946901)


Macintosh小部件样式中显示的日期时间编辑小部件。


![](https://img-blog.csdn.net/20180531083019877)


Fusion小部件样式中显示的日期时间编辑小部件。








一、成员类型

1、enum QDateTimeEdit::Section


flags QDateTimeEdit::Sections


|内容|值|
|----|----|
|QDateTimeEdit::NoSection|0x0000|
|QDateTimeEdit::AmPmSection|0x0001|
|QDateTimeEdit::MSecSection|0x0002|
|QDateTimeEdit::SecondSection|0x0004|
|QDateTimeEdit::MinuteSection|0x0008|
|QDateTimeEdit::HourSection|0x0010|
|QDateTimeEdit::DaySection|0x0100|
|QDateTimeEdit::MonthSection|0x200|
|QDateTimeEdit::YearSection|0x400|
Sections类型是QFlags <Section>的typedef。 它存储部分值的或组合。






二、属性

1、calendarPopup : bool



该属性保存当前日历弹出显示模式。

点击箭头按钮后将显示日历弹出窗口。 只有存在有效的日期显示格式时，此属性才有效。

该属性在Qt 4.2中引入。

访问函数；

```cpp
bool calendarPopup() const
void setCalendarPopup(bool enable)
```




2、currentSection : Section

该属性包含spinbox setCurrentSection（）的当前部分。


访问函数；

```cpp
Section currentSection() const
void setCurrentSection(Section section)
```




3、currentSectionIndex : int



该属性保存了spinbox的当前部分索引。

如果格式为'yyyy / MM / dd'，则displayText为'2001/05/21'，并且cursorPosition为5，currentSectionIndex返回1.如果cursorPosition为3，currentSectionIndex为0，依此类推。

setCurrentSection（）

这个属性是在Qt 4.3中引入的。

访问函数；

```cpp
int currentSectionIndex() const
void setCurrentSectionIndex(int index)
```




4、date : QDate

该属性包含在小部件中设置的QDate。

默认情况下，此属性包含引用2000年1月1日的日期。


访问函数；

```cpp
QDate date() const
void setDate(const QDate &date)
```

通知信号；

```cpp
void dateChanged(const QDate &date)
```




5、dateTime : QDateTime



该属性包含在QDateTimeEdit中设置的QDateTime。

设置此属性时，QDateTimeEdit的timespec保持不变，并且忽略新QDateTime的timespec。

默认情况下，此属性包含引用2000年1月1日和00:00:00和0毫秒的日期。

访问函数；

```cpp
QDateTime dateTime() const
void setDateTime(const QDateTime &dateTime)
```

通知信号；

```cpp
void dateTimeChanged(const QDateTime &datetime)
```




6、displayFormat : QString



该属性保存用于显示日期时间编辑的时间/日期的格式。

这种格式在QDateTime :: toString（）和QDateTime :: fromString（）中描述

示例格式字符串（假设日期是1969年7月2日）：


|格式|结果|
|----|----|
|dd.MM.yyyy|02.07.1968|
|MM.d.yy|Jul 2 69|
|MMMM.d.y|July 2 69|
请注意，如果您指定两位数的年份，它将被解释为日期时间编辑初始化的世纪。 默认的世纪是21（2000-2099）
。如果您指定的格式无效，则不会设置格式。

访问函数；

```cpp
QString displayFormat() const
void setDisplayFormat(const QString &format)
```




7、displayedSections : const Sections

该属性包含日期时间编辑的当前显示字段。

为此格式返回一组显示的部分。 setDisplayFormat（），displayFormat（）


访问函数；

```cpp
Sections displayedSections() const
```




8、maximumDate : QDate



该属性保存日期时间编辑的最大日期。

设置此属性时，必要时调整minimumDate以确保范围保持有效。 如果日期不是有效的QDate对象，则该函数不执行任何操作。

默认情况下，该属性包含一个日期，指的是7999年12月31日。

访问函数；

```cpp
QDate maximumDate() const
void setMaximumDate(const QDate &max)
void clearMaximumDate()
```




9、maximumDateTime : QDateTime



该属性保存日期时间编辑的最大日期时间。

设置此属性时，必要时调整minimumDateTime（）以确保范围保持有效。 如果日期时间不是有效的QDateTime对象，则此函数不执行任何操作。

可以使用clearMaximumDateTime（）恢复默认的maximumDateTime。

默认情况下，该属性包含一个日期，指的是7999年12月31日以及23:59:59和999毫秒的时间。

这个属性是在Qt 4.4中引入的。

访问函数；

```cpp
QDateTime maximumDateTime() const
void setMaximumDateTime(const QDateTime &dt)
void clearMaximumDateTime()
```




10、maximumTime : QTime



此属性保存日期时间编辑的最长时间。

设置此属性时，必要时调整minimumTime以确保范围保持有效。 如果时间不是有效的QTime对象，则此函数不执行任何操作。

默认情况下，此属性包含23:59:59和999毫秒的时间。

访问函数；

```cpp
QTime maximumTime() const
void setMaximumTime(const QTime &max)
void clearMaximumTime()
```




11、minimumDate : QDate



该属性保存日期时间编辑的最小日期。

设置此属性时，如有必要，调整maximumDate以确保范围保持有效。 如果日期不是有效的QDate对象，则该函数不执行任何操作。

默认情况下，此属性包含引用1752年9月14日的日期。最小日期必须至少为第100年的第一天，否则setMinimumDate（）不起作用。

访问函数；

```cpp
QDate minimumDate() const
void setMinimumDate(const QDate &min)
void clearMinimumDate()
```



12、minimumDateTime : QDateTime



该属性保存日期时间编辑的最小日期时间。

设置此属性时，必要时调整maximumDateTime（）以确保范围保持有效。 如果日期时间不是有效的QDateTime对象，则此函数不执行任何操作。

可以使用clearMinimumDateTime（）恢复默认的minimumDateTime

默认情况下，此属性包含引用1752年9月14日和00:00:00和0毫秒的日期。

这个属性是在Qt 4.4中引入的。

访问函数；

```cpp
QDateTime minimumDateTime() const
void setMinimumDateTime(const QDateTime &dt)
void clearMinimumDateTime()
```



13、minimumTime : QTime



此属性保留日期时间编辑的最短时间。

设置此属性时，必要时调整maximumTime，以确保范围保持有效。 如果时间不是有效的QTime对象，则此函数不执行任何操作。

默认情况下，此属性包含00:00:00和0毫秒的时间。

访问函数；

```cpp
QTime minimumTime() const
void setMinimumTime(const QTime &min)
void clearMinimumTime()
```




14、sectionCount : const int

该属性保存显示的部分的数量。 如果格式为'yyyy / yy / yyyy'，则sectionCount返回3。

这个属性是在Qt 4.3中引入的。


访问函数；

```cpp
int sectionCount() const
```




15、time : QTime

该属性保存在小部件中设置的QTime。

默认情况下，此属性包含00:00:00和0毫秒的时间。


访问函数；

```cpp
QTime time() const
void setTime(const QTime &time)
```

通知信号；

```cpp
void timeChanged(const QTime &time)
```




16、timeSpec : Qt::TimeSpec

该属性保存日期时间编辑使用的当前时间。

这个属性是在Qt 4.4中引入的。


访问函数；

```cpp
Qt::TimeSpec timeSpec() const
void setTimeSpec(Qt::TimeSpec spec)
```







三、成员函数

1、QDateTimeEdit::QDateTimeEdit(QWidget *parent = Q_NULLPTR)

用父级构造一个空的日期时间编辑器。





2、QDateTimeEdit::QDateTimeEdit(const QDateTime &datetime, QWidget *parent = Q_NULLPTR)

用父级构造一个空的日期时间编辑器。 该值设置为datetime。





3、QDateTimeEdit::QDateTimeEdit(const QDate &date, QWidget *parent = Q_NULLPTR)

用父级构造一个空的日期时间编辑器。 该值设置为最新。





4、QDateTimeEdit::QDateTimeEdit(const QTime &time, QWidget *parent = Q_NULLPTR)

用父级构造一个空的日期时间编辑器。 该值设置为时间。





5、QDateTimeEdit::~QDateTimeEdit()

析构函数。




6、QCalendarWidget *QDateTimeEdit::calendarWidget() const



如果calendarPopup设置为true并且（sections（）和DateSections_Mask）！= 0，则返回编辑器的日历小部件。

如果没有设置，此函数创建并返回日历小部件。

这个函数是在Qt 4.4中引入的。




7、[virtual] void QDateTimeEdit::clear()

从QAbstractSpinBox :: clear（）重新实现。





8、QDate QDateTimeEdit::date() const

返回日期时间编辑的日期。

注意：属性日期的Getter函数。





9、[signal] void QDateTimeEdit::dateChanged(const QDate &date)

每当更改日期时都会发出此信号。 新日期以日期传递。

注意：财产日期的通知信号。





10、[signal] void QDateTimeEdit::dateTimeChanged(const QDateTime &datetime)

无论何时更改日期或时间，都会发出此信号。 新日期和时间以日期时间传递。

注意：属性dateTime的通知程序信号。





11、[virtual protected] QDateTime QDateTimeEdit::dateTimeFromText(const QString &text) const

为给定文本返回适当的日期时间。

只要需要将用户输入的文本解释为值，该日期时间编辑就会使用此虚拟功能。





12、[virtual] bool QDateTimeEdit::event(QEvent *event)

从QObject :: event（）重新实现。





13、[virtual protected] void QDateTimeEdit::fixup(QString &input) const

从QAbstractSpinBox :: fixup（）重新实现。





14、[virtual protected] void QDateTimeEdit::focusInEvent(QFocusEvent *event)

从QWidget :: focusInEvent（）重新实现。





15、[virtual protected] bool QDateTimeEdit::focusNextPrevChild(bool next)

从QWidget :: focusNextPrevChild（）重新实现。





16、[protected] void QDateTimeEdit::initStyleOption(QStyleOptionSpinBox *option) const

使用此QDataTimeEdit中的值初始化选项。 当他们需要一个QStyleOptionSpinBox的时候，这个方法对子类很有用，但不想自己填写所有的信息。





17、[virtual protected] void QDateTimeEdit::keyPressEvent(QKeyEvent *event)

从QWidget :: keyPressEvent（）重新实现。





18、[virtual protected] void QDateTimeEdit::mousePressEvent(QMouseEvent *event)

从QWidget :: mousePressEvent（）重新实现。





19、[virtual protected] void QDateTimeEdit::paintEvent(QPaintEvent *event)

从QWidget :: paintEvent（）重新实现。





20、Section QDateTimeEdit::sectionAt(int index) const



返回索引处的部分。

如果格式为'yyyy / MM / dd'，sectionAt（0）返回YearSection，sectionAt（1）返回MonthSection，sectionAt（2）返回YearSection，

这个函数是在Qt 4.3中引入的。




21、QString QDateTimeEdit::sectionText(Section section) const

返回给定部分的文本。





22、void QDateTimeEdit::setCalendarWidget(QCalendarWidget *calendarWidget)



将给定的calendarWidget设置为用于日历弹出窗口的小部件。 编辑器不会自动获取日历小部件的所有权。

注意：在设置日历小部件之前，calendarPopup必须设置为true。

这个函数是在Qt 4.4中引入的。




23、void QDateTimeEdit::setDateRange(const QDate &min, const QDate &max)

通过一次函数调用设置最小和最大日期的便捷功能。


```cpp
setDateRange(min, max);
```

类似于：

```cpp
setMinimumDate(min);
  setMaximumDate(max);
```

如果min或max无效，该函数不执行任何操作。




24、void QDateTimeEdit::setDateTimeRange(const QDateTime &min, const QDateTime &max)

通过一次函数调用设置最小和最大日期时间的便捷功能。


```cpp
setDateTimeRange(min, max);
```

类似于：

```cpp
setMinimumDateTime(min);
  setMaximumDateTime(max);
```

如果min或max无效，该函数不执行任何操作。

这个函数是在Qt 4.4中引入的。




25、void QDateTimeEdit::setSelectedSection(Section section)

选择部分。 如果部分在当前显示的部分中不存在，则此功能不执行任何操作。 如果部分是NoSection，则此功能将取消选择编辑器中的所有文本。 否则，该功能会将光标和当前部分移动到所选部分。

这个函数是在Qt 4.2中引入的。





26、void QDateTimeEdit::setTimeRange(const QTime &min, const QTime &max)

便捷功能可通过一次函数调用来设置最小和最大时间。


```cpp
setTimeRange(min, max);
```

类似于：

```cpp
setMinimumTime(min);
  setMaximumTime(max);
```

如果min或max无效，该函数不执行任何操作。




27、[virtual] QSize QDateTimeEdit::sizeHint() const

从QWidget :: sizeHint（）重新实现。





28、[virtual] void QDateTimeEdit::stepBy(int steps)

从QAbstractSpinBox :: stepBy（）重新实现。





29、[virtual protected] StepEnabled QDateTimeEdit::stepEnabled() const

从QAbstractSpinBox :: stepEnabled（）重新实现。





30、[virtual protected] QString QDateTimeEdit::textFromDateTime(const QDateTime &dateTime) const

只要需要显示日期时间，日期时间编辑就会使用此虚拟功能。

如果你重新实现了这个，你可能还需要重新实现validate（）。




31、QTime QDateTimeEdit::time() const

返回日期时间编辑的时间。

注意：属性时间的Getter函数。





32、[signal] void QDateTimeEdit::timeChanged(const QTime &time)

只要时间改变就会发出这个信号。 新时间过去了。





33、[virtual protected] QValidator::State QDateTimeEdit::validate(QString &text, int &pos) const

从QAbstractSpinBox :: validate（）重新实现。





34、[virtual protected] void QDateTimeEdit::wheelEvent(QWheelEvent *event)

从QWidget :: wheelEvent（）重新实现。




