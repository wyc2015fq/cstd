# Qt开发：Qt Widgets模块——QDateEdit类 - 一世豁然的专栏 - CSDN博客





2018年05月28日 18:28:17[一世豁然](https://me.csdn.net/Explorer_day)阅读数：444








QDateEdit类提供了一个基于QDateTimeEdit类编辑日期的小部件。



头文件：#include <QDateEdit> 

qmake：QT += widgets

继承：QDateTimeEdit

QDateEdit提供的许多属性和函数都在QDateTimeEdit中实现。 这些是这个类的相关属性：


1、日期保存小部件显示的日期。

2、minimumDate定义了用户可以设置的最小（最早）日期。

3、maximumDate定义了用户可以设置的最大（最新）日期。

4、displayFormat包含一个字符串，用于格式化窗口小部件中显示的日期。

![](https://img-blog.csdn.net/20180528182519538)


Windows Vista小部件样式中显示的日期编辑小部件。


![](https://img-blog.csdn.net/20180528182549168)


Macintosh小部件样式中显示的日期编辑小部件。

![](https://img-blog.csdn.net/20180528182610204)


Fusion小部件样式中显示的日期编辑小部件。








一、成员函数

1、QDateEdit::QDateEdit(QWidget *parent = Q_NULLPTR)

用父级构造一个空的日期编辑器。





2、QDateEdit::QDateEdit(const QDate &date, QWidget *parent = Q_NULLPTR)

用父级构造一个空的日期编辑器。 日期设置为日期。





3、QDateEdit::~QDateEdit()

析构函数。



