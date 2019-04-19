# Qt对象树和拥有权 - xqhrs232的专栏 - CSDN博客
2016年10月15日 10:03:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：342
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://www.cnblogs.com/King-Gentleman/p/4830978.html](http://www.cnblogs.com/King-Gentleman/p/4830978.html)
相关文章
1、[Qt智能指针和QObject对象树系统（父子系统）结合使用出现的问题](http://jgsj.iteye.com/blog/1968445)----[http://jgsj.iteye.com/blog/1968445](http://jgsj.iteye.com/blog/1968445)
2、[qt学习笔记（六）之简析对象树](http://blog.csdn.net/fzu_dianzi/article/details/6949081)----[http://blog.csdn.net/fzu_dianzi/article/details/6949081](http://blog.csdn.net/fzu_dianzi/article/details/6949081)

一、对象模型
标准C++对象模型可以在运行时非常有效的支持对象范式（object paradigm），但是它的静态特性在一些问题领域中不够灵活。图形用户界面编程不仅需要运行时的高效性，还需要高度的灵活性。为此，Qt在标准C++对象模型的基础上添加了一些特性，形成了自己的对象模型。这些特性有：
- 一个强大的无缝对象通信机制——信号和槽（signals and slots）；
- 可查询和可设计的对象属性系统（object properties）；
- 强大的事件和事件过滤器（events and event filters）；
- 通过上下文进行国际化的字符串翻译机制（string translation for internationalization）；
- 完善的定时器（timers）驱动，使得可以在一个事件驱动的GUI中处理多个任务；
- 分层结构的、可查询的对象树（object trees），它使用一种很自然的方式来组织对象拥有权（object ownership）；
- 守卫指针即QPointer，它在引用对象被销毁时自动将其设置为0；
- 动态的对象转换机制（dynamic cast）；
Qt的这些特性都是在遵循标准C++规范内实现的，使用这些特性都必须要继承自QObject类。其中对象通信机制和动态属性系统，还需要元对象系统（Meta-ObjectSystem）的支持。关于对象模型的介绍，大家可以在帮助中查看Object Model关键字。
二、元对象系统
Qt中的元对象系统（Meta-Object System）提供了对象间通信的信号和槽机制、运行时类型信息和动态属性系统。元对象系统是基于以下三个条件的：
- 该类必须继承自QObject类；
- 必须在类的私有声明区声明Q_OBJECT宏（在类定义时，如果没有指定public或者private，则默认为private）；
- 元对象编译器Meta-Object Compiler（moc），为QObject的子类实现元对象特性提供必要的代码。
其中moc工具读取一个C++源文件，如果它发现一个或者多个类的声明中包含有Q_OBJECT宏，便会另外创建一个C++源文件（就是在项目目录中的debug目录下看到的以moc开头的C++源文件），其中包含了为每一个类生成的元对象代码。这些产生的源文件或者被包含进类的源文件中，或者和类的实现同时进行编译和链接。
元对象系统主要是为了实现信号和槽机制才被引入的，不过除了信号和槽机制以外，元对象系统还提供了其他一些特性：
- QObject::metaObject()函数可以返回一个类的元对象，它是QMetaObject类的对象；
- QMetaObject::className()可以在运行时以字符串形式返回类名，而不需要C++编辑器原生的运行时类型信息（RTTI）的支持；
- QObject::inherits()函数返回一个对象是否是QObject继承树上一个类的实例的信息；
- QObject::tr()和QObject::trUtf8()进行字符串翻译来实现国际化；
- QObject::setProperty()和QObject::property()通过名字来动态设置或者获取对象属性；
- QMetaObject::newInstance()构造该类的一个新实例。
除了这些特性外，还可以使用qobject_cast()函数来对QObject类进行动态类型转换，这个函数的功能类似于标准C++中的dynamic_cast()函数，但它不再需要RTTI的支持。这个函数尝试将它的参数转换为尖括号中的类型的指针，如果是正确的类型则返回一个非零的指针，如果类型不兼容则返回0。例如：
QObject *obj = new MyWidget;
QWidget *widget = qobject_cast<QWidget *>(obj);
信号和槽机制是Qt的核心内容，而信号和槽机制必须依赖于元对象系统，所以它是Qt中很关键的内容。关于元对象系统的知识，可以在Qt中查看The
 Meta-Object System关键字。
三、对象树与拥有权
     Qt中使用对象树（object tree）来组织和管理所有的QObject类及其子类的对象。当创建一个QObject时，如果使用了其他的对象作为其父对象（parent），那么这个QObject就会被添加到父对象的children()列表中，这样当父对象被销毁时，这个QObject也会被销毁。实践表明，这个机制非常适合于管理GUI对象。例如，一个QShortcut（键盘快捷键）对象是相应窗口的一个子对象，所以当用户关闭了这个窗口时，这个快捷键也可以被销毁。
     QWidget作为能够在屏幕上显示的所有部件的基类，扩展了对象间的父子关系。一个子对象一般也就是一个子部件，因为它们要显示在父部件的区域之中。例如，当关闭一个消息对话框（message box）后要销毁它时，消息对话框中的按钮和标签也会被销毁，这也正是我们所希望的，因为按钮和标签是消息对话框的子部件。当然，我们也可以自己来销毁一个子对象。关于这一部分的内容，大家可以在帮助索引中查看Object
 Trees &Ownership关键字。
在前面的Qt编程中我们应该看到过很多使用new来创建一个部件，但是却没有使用delete来进行释放的问题。这里再来研究一下这个问题。
  新建Qt Gui应用，项目名称为“myOwnership”，基类选择QWidget，然后类名保持“Widget”不变。完成后向项目中添加新文件，模板选择C++
 Class，类名为“MyButton”，基类为“QPushButton”，类型信息选择“继承自QWidget”。添加完文件后在mybutton.h文件中添加析构函数的声明：
~MyButton();
然后到mybutton.cpp文件中添加头文件#include <QDebug>并定义析构函数：
MyButton::~MyButton()
{
qDebug()
 << "delete button";
}
这样当MyButton的对象被销毁时，就会输出相应的信息。这里定义析构函数，只是为了更清楚的看到部件的销毁过程，其实一般在构建新类时不需要实现析构函数。下面在widget.cpp文件中进行更改，添加头文件：
#include"mybutton.h"
#include<QDebug>
在构造函数中添加代码：
MyButton*button=newMyButton(this); // 创建按钮部件，指定widget为父部件
button->setText(tr("button"));
更改析构函数：
Widget::~Widget()
{
deleteui;
    qDebug() << "delete widget";
}
       Widget类的析构函数中默认的已经有了销毁ui的语句，这里又添加了输出语句。当Widget窗口被销毁时，将输出信息。下面运行程序，然后关闭窗口，在QtCreator的应用程序输出栏中的输出信息为：
delete widget
delete button
可以看到，当关闭窗口后，因为该窗口是顶层窗口，所以应用程序要销毁该窗口部件（如果不是顶层窗口，那么关闭时只是隐藏，不会被销毁），而当窗口部件销毁时会自动销毁其子部件。这也就是为什么在Qt中经常只看到new操作而看不到delete操作的原因。再来看一下main.cpp文件，其中Widget对象是建立在栈上的：
Widgetw;
w.show();
这样对于对象w，在关闭程序时会被自动销毁。而对于Widget中的部件，如果是在堆上创建（使用new操作符），那么只要指定Widget为其父窗口就可以了，也不需要进行delete操作。整个应用程序关闭时，会去销毁w对象，而此时又会自动销毁它的所有子部件，这些都是Qt的对象树所完成的。
所以，对于规范的Qt程序，我们要在main()函数中将主窗口部件创建在栈上，例如“Widget
 w;”，而不要在堆上进行创建（使用new操作符）。对于其他窗口部件，可以使用new操作符在堆上进行创建，不过一定要指定其父部件，这样就不需要再使用delete操作符来销毁该对象了。
还有一种重定义父部件（reparented）的情况，例如，将一个包含其他部件的布局管理器应用到窗口上，那么该布局管理器和其中的所有部件都会自动将它们的父部件转换为该窗口部件。在widget.cpp文件中添加头文件#include<QHBoxLayout>，然后在构造函数中继续添加代码：
MyButton*button2=newMyButton;
MyButton*button3=newMyButton;
QHBoxLayout*layout=newQHBoxLayout;
layout->addWidget(button2);
layout->addWidget(button3);
setLayout(layout);      // 在该窗口中使用布局管理器
这里创建了两个MyButton和一个水平布局管理器，但是并没有指定它们的父部件，现在各个部件的拥有权（ownership）不是很清楚。但是当使用布局管理器来管理这两个按钮，并且在窗口中使用这个布局管理器后，这两个按钮和水平布局管理器都将重定义父部件而成为窗口Widget的子部件。可以使用children()函数来获取一个部件的所有子部件的列表，例如在构造函数中再添加如下代码：
qDebug() << children();    // 输出所有子部件的列表
这时大家可以运行一下程序，查看应用程序输出栏中的信息，然后根据自己的想法更改一下程序，来进一步体会Qt中对象树的概念。
四、总结
Qt中的对象树很好地解决了父子部件的关系，对于Gui编程是十分方便的，在创建部件时我们只需要关注它的父部件，这样就不用再考虑其销毁问题了。下一节，我们将讲解Qt中的信号和槽的内容。
![](http://images2015.cnblogs.com/blog/719920/201509/719920-20150923000523772-809276386.png)
分类: [Qt学习笔记](http://www.cnblogs.com/King-Gentleman/category/735597.html)
