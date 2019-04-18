# Qt开发：Qt Widgets模块——QAccessibleWidget类 - 一世豁然的专栏 - CSDN博客





2018年05月17日 20:00:11[一世豁然](https://me.csdn.net/Explorer_day)阅读数：305








QAccessibleWidget类实现QWidgets的QAccessibleInterface。



头文件：#include <QAccessibleWidget> 

qmake：QT += widgets

继承：QWidget

被继承：QAccessibleObject和QAccessibleActionInterface

该类是QWidget应用程序的辅助功能的一部分

。这个类可以方便地用作QAccessibleInterfaces自定义实现的基类，它提供了关于窗口小部件对象的信息。

该类提供了检索parentObject（）（小部件的父小部件）和关联的小部件（）的函数。 可以使用addControllingSignal（）添加控制信号，并为接口实现的各个方面提供setter，例如setValue（），setDescription（），setAccelerator（）和setHelp（）。







一、成员函数

1、QAccessibleWidget::QAccessibleWidget(QWidget *w, QAccessible::Role role = QAccessible::Client, const QString &name = QString())


为小部件w创建一个QAccessibleWidget对象。 角色和名称是设置对象的角色和名称属性的可选参数。





2、[protected] QAccessibleWidget::~QAccessibleWidget()

销毁这个对象。





3、[virtual] QStringList QAccessibleWidget::actionNames() const

从QAccessibleActionInterface :: actionNames（）重新实现。





4、[protected] void QAccessibleWidget::addControllingSignal(const QString &signal)

将信号寄存为控制信号。

对象是连接到控制信号的任何其他对象的控制器。





5、[virtual] QColor QAccessibleWidget::backgroundColor() const

从QAccessibleInterface :: backgroundColor（）重新实现。





6、[virtual] QAccessibleInterface *QAccessibleWidget::child(int index) const

从QAccessibleInterface :: child（）中重新实现。





7、[virtual] int QAccessibleWidget::childCount() const

从QAccessibleInterface :: childCount（）重新实现。





8、[virtual] void QAccessibleWidget::doAction(const QString &actionName)

从QAccessibleActionInterface :: doAction（）重新实现。





9、[virtual] QAccessibleInterface *QAccessibleWidget::focusChild() const

从QAccessibleInterface :: focusChild（）重新实现。





10、[virtual] QColor QAccessibleWidget::foregroundColor() const

从QAccessibleInterface :: foregroundColor（）重新实现。





11、[virtual] int QAccessibleWidget::indexOfChild(const QAccessibleInterface *child) const

从QAccessibleInterface :: indexOfChild（）重新实现。





12、[virtual] void *QAccessibleWidget::interface_cast(QAccessible::InterfaceType t)

从QAccessibleInterface :: interface_cast（）重新实现。





13、[virtual] bool QAccessibleWidget::isValid() const

从QAccessibleInterface :: isValid（）重新实现。





14、[virtual] QStringList QAccessibleWidget::keyBindingsForAction(const QString &actionName) const

从QAccessibleActionInterface :: keyBindingsForAction（）重新实现。





15、[virtual] QAccessibleInterface *QAccessibleWidget::parent() const

从QAccessibleInterface :: parent（）重新实现。





16、[protected] QObject *QAccessibleWidget::parentObject() const

返回关联窗口小部件的父对象，它可以是父窗口小部件，也可以是用于顶层窗口小部件的qApp。





17、[virtual] QRect QAccessibleWidget::rect() const

从QAccessibleInterface :: rect（）重新实现。





18、[virtual] QVector<QPair<QAccessibleInterface *, QAccessible::Relation> > QAccessibleWidget::relations(QAccessible::Relation match = QAccessible::AllRelations) const

从QAccessibleInterface :: relations（）重新实现。





19、[virtual] QAccessible::Role QAccessibleWidget::role() const

从QAccessibleInterface :: role（）重新实现。





20、[virtual] QAccessible::State QAccessibleWidget::state() const

从QAccessibleInterface :: state（）中重新实现。





21、[virtual] QString QAccessibleWidget::text(QAccessible::Text t) const

从QAccessibleInterface :: text（）重新实现。





22、[protected] QWidget *QAccessibleWidget::widget() const

返回关联的小部件。





23、[virtual] QWindow *QAccessibleWidget::window() const

从QAccessibleInterface :: window（）重新实现。




