# Qt布局管理详解 - fanyun的博客 - CSDN博客
2016年09月30日 09:00:41[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：8821
**Qt布局管理手册：**
http://doc.qt.io/qt-5/qtwidgets-index.html#styles
http://doc.qt.io/qt-5/qtwidgets-index.html#widgets
http://doc.qt.io/qt-5/qtwidgets-index.html#layouts
### 以下是Qt手册中的《布局管理》的译文：
         在一个Widget中，Qt布局管理系统提供了一个简单而有效的方式来自动组织子widget，以保证他们能够很好地利用可用空间。
#### 介绍：
        Qt包含一个布局管理类的集合，它们被用来描述widgets如何在应用程序的用户界面中呈现的。当可用空间发生变化时，这些布局将自动调整widgets的位置和大小，以确保它们布局的一致性和用户界面主体可用。
所有QWidget的子类都可以用布局来管理它们的子类。QWidget::setLayout()函数给widget提供一个布局。当布局通过这种方式设置到widget，它将负责以下任务：
1.子widget的定位
2.窗口的合理默认空间
3.窗口的合理最小空间
4.调整大小处理
5.当内容发生变化时自动调整
6.字体、大小或者内容变化
7.显示或 隐藏widget
8.移除子widget
#### Qt的布局类：
|[QGraphicsAnchorLayout]()|Layout where one can anchor widgets together in Graphics View|在制图视图中布局widget|
|----|----|----|
|[QGraphicsAnchor]()|Represents an anchor between two items in a QGraphicsAnchorLayout|表示一个QGraphicsAnchorLayout中两个图元之间的anchor|
|[QBoxLayout]()|Lines up child widgets horizontally or vertically|水平或垂直整理子widget|
|[QHBoxLayout]()|Lines up widgets horizontally|水平整理子控件|
|[QVBoxLayout]()|Lines up widgets vertically|垂直整理子控件|
|[QFormLayout]()|Manages forms of input widgets and their associated labels|label-inputwidget模式表单布局|
|[QGridLayout]()|Lays out widgets in a grid|网格布局|
|[QLayout]()|The base class of geometry managers|布局，几何管理的基类|
|[QLayoutItem]()|Abstract item that a QLayout manipulates|管理的抽象元素|
|[QSpacerItem]()|Blank space in a layout|空白区域布局|
|[QWidgetItem]()|Layout item that represents a widget|布局元素|
|[QSizePolicy]()|Layout attribute describing horizontal and vertical resizing policy|大小策略|
|[QStackedLayout]()|Stack of widgets where only one widget is visible at a time|栈模式布局，一次只显示一个|
|[QButtonGroup]()|Container to organize groups of button widgets|管理按钮的容器|
|[QGroupBox]()|Group box frame with a title|带标题的组箱框架|
|[QStackedWidget]()|Stack of widgets where only one widget is visible at a time|栈模式的widget，一次只显示一个|
#### 水平、垂直、网格和表格布局：
       Qt布局类之间的关系如图1所示：
![](https://img-blog.csdn.net/20161124132840510?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                                                    图1 Qt布局类之间的关系
        给widgets一个很好布局的最好方式是使用内置的布局管理器： [QHBoxLayout](), [QVBoxLayout](), [QGridLayout](),
 and [QFormLayout](). 这些类都从QLayout继承而来，它们都来源于QObject（而不是QWidget）。创建更加复杂的布局，可以让它们彼此嵌套完成。
**QHBoxLayout：水平布局**
**QVBoxLayout：垂直布局**
**QGridLayout： 表格布局**
QGridLayout::addWidget()语法
layout->addWidget(widget, row, column, rowSpan, columnSpan);
参数widget：为插入到这个布局的子控件；
参数（row，column）为控件占据的左上角单元格位置；
参数rowSpan是控件占据的行数，
参数colunmSpan是控件占据的列的个数。
（rowSpan和colunmSpan默认值为1）
**Stacked Layouts：分组布局**
     QStackedLayout类把子控件进行分组或者分页，一次只显示一组或者一页，隐藏其他组或者页上的控件。
使用这些Qt布局管理类的另一个原因是，在程序、系统改变字体，语言或者在不同的平台上运行时，布局管理器能够自动调整窗体里所有控件的大小和尺寸。
其他可进行布局管理的类：这些类的共同特点是提供了更加灵活的布局管理，在一定程度上用户能够控制窗体内控件的大小。
**QSplitter，QScrollArea，QMainWindow，QWorkspace**（对多文档的支持）
**2）         布局管理中结合控件的sizePolicy属性，进行调整**
   结合控件的SizePolicy属性，来控制布局管理中的控件的尺寸自适应方式。
控件的sizePolicy说明控件在布局管理中的缩放方式。Qt提供的控件都有一个合理的缺省sizePolicy，但是这个缺省值有时不能适合所有的布局，开发人员经常需要改变窗体上的某些控件的sizePolicy。一个QSizePolicy的所有变量对水平方向和垂直方向都适用。下面列举了一些最长用的值：
A.Fixed：控件不能放大或者缩小，控件的大小就是它的sizeHint。
B.Minimum：控件的sizeHint为控件的最小尺寸。控件不能小于这个sizeHint，但是可以
放大。
C.Maximum：控件的sizeHint为控件的最大尺寸，控件不能放大，但是可以缩小到它的最小
的允许尺寸。
D.Preferred：控件的sizeHint是它的sizeHint，但是可以放大或者缩小
E.Expandint：控件可以自行增大或者缩小
注：sizeHint（布局管理中的控件默认尺寸，如果控件不在布局管理中就为无效的值）
1.QHBoxLayout是水平布局，将从左往右（orright to left for right-to-left languages ）widget布局成水平行
![](https://img-blog.csdn.net/20160922163728898?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
[](http://images.cnitblog.com/blog/288640/201301/29140408-d73f936fe1b84ea78fb885bfb86b3691.jpg)
2.QVBoxLayout是垂直布局，从顶部到底部
![](https://img-blog.csdn.net/20160922163823802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
[](http://images.cnitblog.com/blog/288640/201301/29140409-9f3a0d01dc4f44f381384ce1207ebbee.jpg)
3.[QGridLayout]() 是二位的网格布局。它可以容纳多个单元格：
![](https://img-blog.csdn.net/20160922163858571?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
[](http://images.cnitblog.com/blog/288640/201301/29140410-068cc93415184cce9aa5661baeb59026.jpg)
4.QFormLayout是两列label-field式的表单布局
![](https://img-blog.csdn.net/20160922163949523?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)[](http://images.cnitblog.com/blog/288640/201301/29140411-71b17b919f11466487486d76ab7f8e28.jpg)
##### 代码举例：
下面代码创建QHBoxLayout来管理5个QPushButtons的几何图形：
[QWidget]() *window= new [QWidget]();
[QPushButton]() *button1= new [QPushButton]()("One");
[QPushButton]() *button2= new [QPushButton]()("Two");
[QPushButton]() *button3= new [QPushButton]()("Three");
[QPushButton]() *button4= new [QPushButton]()("Four");
[QPushButton]() *button5= new [QPushButton]()("Five");
[QHBoxLayout]() *layout= new [QHBoxLayout]();
layout->addWidget(button1);
layout->addWidget(button2);
layout->addWidget(button3);
layout->addWidget(button4);
layout->addWidget(button5);
window->setLayout(layout);
window->show();
QGridLayout示例如下：
[QWidget]() *window = new [QWidget]();[QPushButton]() *button1 = new [QPushButton]()("One");[QPushButton]() *button2 = new [QPushButton]()("Two");[QPushButton]() *button3 = new [QPushButton]()("Three");[QPushButton]() *button4 = new [QPushButton]()("Four");[QPushButton]() *button5 = new [QPushButton]()("Five");[QGridLayout]() *layout = new [QGridLayout]();    layout->addWidget(button1, 0, 0);    layout->addWidget(button2, 0, 1);    layout->addWidget(button3, 1, 0, 1, 2);    layout->addWidget(button4, 2, 0);    layout->addWidget(button5, 2, 1);    window->setLayout(layout);    window->show();
QFormLayout示例如下：
[QWidget]() *window = new [QWidget]();[QPushButton]() *button1 = new [QPushButton]()("One");[QLineEdit]() *lineEdit1 = new [QLineEdit]()();[QPushButton]() *button2 = new [QPushButton]()("Two");[QLineEdit]() *lineEdit2 = new [QLineEdit]()();[QPushButton]() *button3 = new [QPushButton]()("Three");[QLineEdit]() *lineEdit3 = new [QLineEdit]()();[QFormLayout]() *layout = new [QFormLayout]();    layout->addRow(button1, lineEdit1);    layout->addRow(button2, lineEdit2);    layout->addRow(button3, lineEdit3);    window->setLayout(layout);    window->show();
##### 布局技巧：
当使用布局的时候，在创建子widget时，没必要给它传递父类。布局会自动重新定义它们的父类（通过[QWidget::setParent]()()）以确保它们是装载布局的widget的子类。
**注意1：**布局中的控件是**装载**布局控件的子控件，不是布局的子控件。控件只能以其他控件作为父类，不可以以布局作为父类。在布局上，可以使用addLayout来嵌套布局；被嵌套的布局，将变成上层布局的子布局。
#### 向布局添加widgets：
添加布局到widgets时，布局过程执行如下：
1.所有widgets将根据它们的 [QWidget::sizePolicy]()() and [QWidget::sizeHint]()()首先分配一些空间。
2. 如果有widgets设置了大于0的拉伸系数，接下来它们将按照拉伸系数的比例来分配空间。
3. 如果有widgets设置的拉伸系数是0，它将在没有其他widgets需要空间时获取更多空间。其中，带Expanding大小策略的widget将首先获得空间。
4. 所有分配了小于最小空间（或者设置了最小的sizehint）的widget将按要求分配最小空间。（在拉伸系数成为决定因子时，widgets没必要再用最小值或者最小hint）。
5. 任何分配了大于最大空间的widget将按要求分配最大空间。（拉伸系数起着决定作用）
##### 拉伸系数：
        通常，widgets创建的时候没有设置拉伸系数。当widget整理到一个布局中时，它们将根据[QWidget::sizePolicy]()()或者最小大小hint（取决于谁更大）分配一定空间。拉伸系数被用于按比例改变widget的分配空间。
        如果3个widget用[QHBoxLayout]() 来布局，不带拉伸系数，它们将得到像下面的布局：
![](https://img-blog.csdn.net/20160922165245077?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
[](http://images.cnitblog.com/blog/288640/201301/29140412-64761503ac354922adca78e871a41988.jpg)
        如果带上拉伸系数，情况将变成这样：
![](https://img-blog.csdn.net/20160922165314952?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)[](http://images.cnitblog.com/blog/288640/201301/29140413-fddeae0bae01456a95f77630069db8ad.jpg)
#### 自定义widget的布局：
       当编写自定义widget类时，需要显示提供它的布局属性。如果widget有Qt自带的布局，它能够自己满足自己。如果没有任何子布局，或者使用手动布局，可以通过下面的机制来改变widget的行为：
1.实现[QWidget::sizeHint]()() 来返回首先大小
2.实现[QWidget::minimumSizeHint]()()来返回widget可以拥有的最小空间
3.调用[QWidget::setSizePolicy]()来描述widget所需的空间
       当size hint、minimum size或size policy改变时，调用[QWidget::updateGeometry]()()。这将促使布局重新进行计算。连续多次调用[QWidget::updateGeometry]()()只会发生一次布局重新计算。
即便实现了[QWidget::heightForWidth]()()，也有必要提供合理的sizeHint()。
进一步了解，参见：[Trading Height for Width](http://doc.qt.nokia.com/qq/qq04-height-for-width.html).
#### 布局问题：
        The use of rich text in a label widget can introduce some problemsto the layout of its parent widget. Problems occur due to the way rich text ishandled by Qt's layout managers when
 the label is word wrapped.
Incertain cases the parent layout is put into QLayout::FreeResize mode, meaningthat it will not adapt the layout of its contents to fit inside small sizedwindows, or even prevent the user from
 making the window too small to beusable. This can be overcome by subclassing the problematic widgets, andimplementing suitable [sizeHint()]() and[minimumSizeHint()]() functions.
Insome cases, it is relevant when a layout is added to a widget. When you set thewidget of a [QDockWidget]() ora [QScrollArea]() (with [QDockWidget::setWidget]()()
 and[QScrollArea::setWidget]()()), the layout mustalready have been set on the widget. If not, the widget will not be visible.
       在QLabel中使用富文本会给布局的父类widget带来一些问题。问题发生的原因是因为当label被文字环绕时，富文本被Qt的布局管理器控制。
       在某些情况下，父类布局被放入QLayout::FreeResize模式，这意味着它将不适应内容布局所设置的最小窗口，或者甚至阻止用户让窗口小到不可用的情况。这个可以通过将问题控件作为子类来解决，并实现合适的[sizeHint()]()和[minimumSizeHint()]()函数。
       在一些情况下，当布局被添加到widget时需要特别注意。当设置[QDockWidget]() ora [QScrollArea]() widget时（用[QDockWidget::setWidget]()()
 and[QScrollArea::setWidget]()())，布局必须已经被设置到widget上。否则，这些widget将不可见。
#### 手动布局：
          如果想自定义一个独特的布局，可以按 如上所述地自定义一个widget。实现[QWidget::resizeEvent]()()来计算所需的大小分配并在每个子类中调用[setGeometry()]() 。
需要布局需要重新计算大小时，widget将提供一个事件接口[QEvent::LayoutRequest]() 。实现[QWidget::event]()()来接收[QEvent::LayoutRequest]()事件。
#### 自定义布局管理：
          自定义布局的唯一方法是继承[QLayout]()来完成自己布局管理器。[Border
 Layout]() 和[Flow Layout]() 例子将说明如何来完成。
下面将举个例子来说明。CardLayout 类，受同名java布局管理的启发。它分层管理每个元素，每个元素的通过[QLayout::spacing]()()来设置位移量。
编写自定义布局类，必须定义以下内容：
由布局控制的存放元素的数据结构。每个元素都是一个[QLayoutItem]()。在这个例子中，我们将使用[QList]() 。
1. [addItem()](),描述如何添加元素到布局。
2.[setGeometry()](),描述如何完成布局
3.[sizeHint()](),布局的首选大小
4.[itemAt()](),描述如何递归布局
5.[takeAt()](),描述如何移除布局中的元素。
在大多数情况下，还需要实现[minimumSize()]()。
```cpp
头文件
card.h
#ifndef CARD_H
#define CARD_H
 
#include <QtGui>
#include <QList>
 
class CardLayout : public QLayout
{
public:
    CardLayout(QWidget *parent, int dist): QLayout(parent, 0, dist) {}
    CardLayout(QLayout *parent, int dist): QLayout(parent, dist) {}
    CardLayout(int dist): QLayout(dist) {}
    ~CardLayout();
 
    void addItem(QLayoutItem *item);
    QSize sizeHint() const;
    QSize minimumSize() const;
    int count() const;
    QLayoutItem *itemAt(int) const;
    QLayoutItem *takeAt(int);
    void setGeometry(const QRect &rect);
 
private:
    QList<QLayoutItem*> list;
};
#endif
实现文件
card.cpp
#include "card.h"
int CardLayout::count() const
{
        // QList::size() returns the number of QLayoutItems in the list
    return list.size();
}
 
int CardLayout::count() const
{
        // QList::size() returns the number of QLayoutItems in the list
    return list.size();
}
 
int CardLayout::count() const
{
        // QList::size() returns the number of QLayoutItems in the list
    return list.size();
}
 
CardLayout::~CardLayout()
{
     QLayoutItem *item;
     while ((item = takeAt(0)))
         delete item;
}
 
void CardLayout::setGeometry(const QRect &r)
{
    QLayout::setGeometry(r);
 
    if (list.size() == 0)
        return;
 
    int w = r.width() - (list.count() - 1) * spacing();
    int h = r.height() - (list.count() - 1) * spacing();
    int i = 0;
    while (i < list.size()) {
        QLayoutItem *o = list.at(i);
        QRect geom(r.x() + i * spacing(), r.y() + i * spacing(), w, h);
        o->setGeometry(geom);
        ++i;
    }
}
QSize CardLayout::sizeHint() const
{
    QSize s(0,0);
    int n = list.count();
    if (n > 0)
        s = QSize(100,70); //start with a nice default size
    int i = 0;
    while (i < n) {
        QLayoutItem *o = list.at(i);
        s = s.expandedTo(o->sizeHint());
        ++i;
    }
    return s + n*QSize(spacing(), spacing());
}
 
QSize CardLayout::minimumSize() const
{
    QSize s(0,0);
    int n = list.count();
    int i = 0;
    while (i < n) {
        QLayoutItem *o = list.at(i);
        s = s.expandedTo(o->minimumSize());
        ++i;
    }
    return s + n*QSize(spacing(), spacing());
}
```
进一步说明：
自定义布局没有控制宽和高。
忽略了 [QLayoutItem::isEmpty]()()，这意味着布局将把隐藏widget作为可见的。
对于复杂布局，通过缓存计算将大大提高速度。在那种情况下，实现[QLayoutItem::invalidate]()() 来标记数据是脏数据。
调用[QLayoutItem::sizeHint]()()等的代价比较大。在通过函数中，需要再次使用，最好将结果保存在本地变量中。
在同样函数的同一个元素中，不应该调用两次 [QLayoutItem::setGeometry]()()。 这个调用将耗费巨大，如果它用几个子widget，因为布局管理器每次都要做一个完整的布局。替代方法：先计算geometry，然后再设置（这种事情，不仅应该在布局时注意，在实现resizeEvent()时也需要按同样方法来做）。
#### 参考：
1. Qt手册《LayoutManagement》
### 窗体小部件和布局：
#### 窗体小部件：
窗体小部件（Widgets）是Qt中创建用户界面的主要元素。窗体小部件可以显示数据和状态信息，接受用户输入，和提供组织其他窗体小部件的容器。
没有嵌入到父级窗体小部件的部件被称为窗口（window）。
#### 布局：
布局是一个种高雅而灵活的方式来自动把子类窗体小部件组织到它们的容器中。每个窗体小部件通过**sizeHint和sizePolicy**属性向布局提供大小需求，布局根据可用空间进行分配。
#### 窗体小部件的样式：
样式（styles）绘制窗体小部件，并封装了GUI的外观和感觉。Qt的内置窗体小部件使用QStyle类完成几乎所有的绘制工作，以确保它们看来确实是一致的、本地窗体小部件。
QSS（[Qt Style Sheets](http://qt-project.org/doc/qt-4.8/stylesheet.html)）允许自定义窗体小部件的外观。
#### 窗体小部件的类：
[QtWidget Gallery](http://qt-project.org/doc/qt-4.8/gallery.html)
##### 基础部件：
|[QCheckBox](http://qt-project.org/doc/qt-4.8/qcheckbox.html)|Checkbox with a text label|
|----|----|
|[QComboBox](http://qt-project.org/doc/qt-4.8/qcombobox.html)|Combined button and popup list|
|[QCommandLinkButton](http://qt-project.org/doc/qt-4.8/qcommandlinkbutton.html)|Vista style command link button|
|[QDateEdit](http://qt-project.org/doc/qt-4.8/qdateedit.html)|Widget for editing dates based on the QDateTimeEdit widget|
|[QDateTimeEdit](http://qt-project.org/doc/qt-4.8/qdatetimeedit.html)|Widget for editing dates and times|
|[QDial](http://qt-project.org/doc/qt-4.8/qdial.html)|Rounded range control (like a speedometer or potentiometer)|
|[QDoubleSpinBox](http://qt-project.org/doc/qt-4.8/qdoublespinbox.html)|Spin box widget that takes doubles|
|[QFocusFrame](http://qt-project.org/doc/qt-4.8/qfocusframe.html)|Focus frame which can be outside of a widget's normal paintable area|
|[QFontComboBox](http://qt-project.org/doc/qt-4.8/qfontcombobox.html)|Combobox that lets the user select a font family|
|[QLCDNumber](http://qt-project.org/doc/qt-4.8/qlcdnumber.html)|Displays a number with LCD-like digits|
|[QLabel](http://qt-project.org/doc/qt-4.8/qlabel.html)|Text or image display|
|[QLineEdit](http://qt-project.org/doc/qt-4.8/qlineedit.html)|One-line text editor|
|[QMenu](http://qt-project.org/doc/qt-4.8/qmenu.html)|Menu widget for use in menu bars, context menus, and other popup menus|
|[QProgressBar](http://qt-project.org/doc/qt-4.8/qprogressbar.html)|Horizontal or vertical progress bar|
|[QPushButton](http://qt-project.org/doc/qt-4.8/qpushbutton.html)|Command button|
|[QRadioButton](http://qt-project.org/doc/qt-4.8/qradiobutton.html)|Radio button with a text label|
|[QScrollArea](http://qt-project.org/doc/qt-4.8/qscrollarea.html)|Scrolling view onto another widget|
|[QScrollBar](http://qt-project.org/doc/qt-4.8/qscrollbar.html)|Vertical or horizontal scroll bar|
|[QSizeGrip](http://qt-project.org/doc/qt-4.8/qsizegrip.html)|Resize handle for resizing top-level windows|
|[QSlider](http://qt-project.org/doc/qt-4.8/qslider.html)|Vertical or horizontal slider|
|[QSpinBox](http://qt-project.org/doc/qt-4.8/qspinbox.html)|Spin box widget|
|[QTabBar](http://qt-project.org/doc/qt-4.8/qtabbar.html)|Tab bar, e.g. for use in tabbed dialogs|
|[QTabWidget](http://qt-project.org/doc/qt-4.8/qtabwidget.html)|Stack of tabbed widgets|
|[QTimeEdit](http://qt-project.org/doc/qt-4.8/qtimeedit.html)|Widget for editing times based on the QDateTimeEdit widget|
|[QToolBox](http://qt-project.org/doc/qt-4.8/qtoolbox.html)|Column of tabbed widget items|
|[QToolButton](http://qt-project.org/doc/qt-4.8/qtoolbutton.html)|Quick-access button to commands or options, usually used inside a QToolBar|
|[QWidget](http://qt-project.org/doc/qt-4.8/qwidget.html)|The base class of all user interface objects|
##### 高级部件：
|[QCalendarWidget](http://qt-project.org/doc/qt-4.8/qcalendarwidget.html)|Monthly based calendar widget allowing the user to select a date|
|----|----|
|[QColumnView](http://qt-project.org/doc/qt-4.8/qcolumnview.html)|Model/view implementation of a column view|
|[QDataWidgetMapper](http://qt-project.org/doc/qt-4.8/qdatawidgetmapper.html)|Mapping between a section of a data model to widgets|
|[QDesktopWidget](http://qt-project.org/doc/qt-4.8/qdesktopwidget.html)|Access to screen information on multi-head systems|
|[QListView](http://qt-project.org/doc/qt-4.8/qlistview.html)|List or icon view onto a model|
|[QMacCocoaViewContainer](http://qt-project.org/doc/qt-4.8/qmaccocoaviewcontainer.html)|Widget for Mac OS X that can be used to wrap arbitrary Cocoa views (i.e., NSView subclasses) and insert them into Qt hierarchies|
|[QMacNativeWidget](http://qt-project.org/doc/qt-4.8/qmacnativewidget.html)|Widget for Mac OS X that provides a way to put Qt widgets into Carbon or Cocoa hierarchies depending on how Qt was configured|
|[QTableView](http://qt-project.org/doc/qt-4.8/qtableview.html)|Default model/view implementation of a table view|
|[QTreeView](http://qt-project.org/doc/qt-4.8/qtreeview.html)|Default model/view implementation of a tree view|
|[QUndoView](http://qt-project.org/doc/qt-4.8/qundoview.html)|Displays the contents of a QUndoStack|
|[QWSEmbedWidget](http://qt-project.org/doc/qt-4.8/qwsembedwidget.html)|Enables embedded top-level widgets in Qt for Embedded Linux|
|[QWebView](http://qt-project.org/doc/qt-4.8/qwebview.html)|Widget that is used to view and edit web documents|
|[QX11EmbedContainer](http://qt-project.org/doc/qt-4.8/qx11embedcontainer.html)|XEmbed container widget|
|[QX11EmbedWidget](http://qt-project.org/doc/qt-4.8/qx11embedwidget.html)|XEmbed client widget|
|[Phonon::VideoWidget](http://qt-project.org/doc/qt-4.8/phonon-videowidget.html)|Widget that is used to display video|
##### 组织者部件：
|[QButtonGroup](http://qt-project.org/doc/qt-4.8/qbuttongroup.html)|Container to organize groups of button widgets|
|----|----|
|[QGroupBox](http://qt-project.org/doc/qt-4.8/qgroupbox.html)|Group box frame with a title|
|[QSplitter](http://qt-project.org/doc/qt-4.8/qsplitter.html)|Implements a splitter widget|
|[QSplitterHandle](http://qt-project.org/doc/qt-4.8/qsplitterhandle.html)|Handle functionality of the splitter|
|[QStackedWidget](http://qt-project.org/doc/qt-4.8/qstackedwidget.html)|Stack of widgets where only one widget is visible at a time|
|[QTabWidget](http://qt-project.org/doc/qt-4.8/qtabwidget.html)|Stack of tabbed widgets|
##### 抽象部件类：
|[QAbstractButton](http://qt-project.org/doc/qt-4.8/qabstractbutton.html)|The abstract base class of button widgets, providing functionality common to buttons|
|----|----|
|[QAbstractScrollArea](http://qt-project.org/doc/qt-4.8/qabstractscrollarea.html)|Scrolling area with on-demand scroll bars|
|[QAbstractSlider](http://qt-project.org/doc/qt-4.8/qabstractslider.html)|Integer value within a range|
|[QAbstractSpinBox](http://qt-project.org/doc/qt-4.8/qabstractspinbox.html)|Spinbox and a line edit to display values|
|[QDialog](http://qt-project.org/doc/qt-4.8/qdialog.html)|The base class of dialog windows|
|[QFrame](http://qt-project.org/doc/qt-4.8/qframe.html)|The base class of widgets that can have a frame|
