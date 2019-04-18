# QT的信号与槽机制介绍（2） - gauss的专栏 - CSDN博客
2011年12月11日 09:36:45[gauss](https://me.csdn.net/mathlmx)阅读数：328标签：[qt																[signal																[scroll																[编译器																[object																[compiler](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=scroll&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
四、信号与槽的关联
通过调用QObject对象的connect函数来将某个对象的信号与另外一个对象的槽函数相关联，这样当发射者发射信号时，接收者的槽函数将被调用。该函数的定义如下：
bool QObject::connect ( const QObject * sender, const char * signal,
const QObject * receiver, const char * member ) [static]
这个函数的作用就是将发射者sender对象中的信号signal与接收者receiver中的member槽函数联系起来。当指定信号signal时必须使用QT的宏SIGNAL()，当指定槽函数时必须使用宏SLOT()。如果发射者与接收者属于同一个对象的话，那么在connect调用中接收者参数可以省略。
例如，下面定义了两个对象：标签对象label和滚动条对象scroll，并将valueChanged()信号与标签对象的setNum()相关联，另外信号还携带了一个整形参数，这样标签总是显示滚动条所处位置的值。
QLabel *label = new QLabel;
QScrollBar *scroll = new QScrollBar;
QObject::connect( scroll, SIGNAL(valueChanged(int)), label, SLOT(setNum(int)) );
一个信号甚至能够与另一个信号相关联，看下面的例子：
class MyWidget : public QWidget
{
public:
MyWidget();
...
signals:
void aSignal();
...
private:
...
QPushButton *aButton;
};
MyWidget::MyWidget()
{
aButton = new QPushButton( this );
connect( aButton, SIGNAL(clicked()), SIGNAL(aSignal()) );
}
在上面的构造函数中，MyWidget创建了一个私有的按钮aButton，按钮的单击事件产生的信号clicked()与另外一个信号aSignal()进行了关联。这样一来，当信号clicked()被发射时，信号aSignal()也接着被发射。当然，你也可以直接将单击事件与某个私有的槽函数相关联，然后在槽中发射aSignal()信号，这样的话似乎有点多余。
当信号与槽没有必要继续保持关联时，我们可以使用disconnect函数来断开连接。其定义如下：
bool QObject::disconnect ( const QObject * sender, const char * signal, const Object * receiver, const char * member ) [static]
这个函数断开发射者中的信号与接收者中的槽函数之间的关联。
有三种情况必须使用disconnect()函数：
断开与某个对象相关联的任何对象。这似乎有点不可理解，事实上，当我们在某个对象中定义了一个或者多个信号，这些信号与另外若干个对象中的槽相关联，如果我们要切断这些关联的话，就可以利用这个方法，非常之简洁。
disconnect( myObject, 0, 0, 0 )
或者
myObject->disconnect()
断开与某个特定信号的任何关联。
disconnect( myObject, SIGNAL(mySignal()), 0, 0 )
或者
myObject->disconnect( SIGNAL(mySignal()) )
断开两个对象之间的关联。
disconnect( myObject, 0, myReceiver, 0 )
或者
myObject->disconnect( myReceiver )
在disconnect函数中0可以用作一个通配符，分别表示任何信号、任何接收对象、接收对象中的任何槽函数。但是发射者sender不能为0，其它三个参数的值可以等于0。
五、元对象工具
元对象编译器moc（meta object compiler）对C++文件中的类声明进行分析并产生用于初始化元对象的C++代码，元对象包含全部信号和槽的名字以及指向这些函数的指针。
moc读C++源文件，如果发现有Q_OBJECT宏声明的类，它就会生成另外一个C++源文件，这个新生成的文件中包含有该类的元对象代码。例如，假设我们有一个头文件mysignal.h，在这个文件中包含有信号或槽的声明，那么在编译之前 moc 工具就会根据该文件自动生成一个名为mysignal.moc.h的C++源文件并将其提交给编译器；类似地，对应于mysignal.cpp文件moc工具将自动生成一个名为mysignal.moc.cpp文件提交给编译器。
元对象代码是signal/slot机制所必须的。用moc产生的C++源文件必须与类实现一起进行编译和连接，或者用#include语句将其包含到类的源文件中。moc并不扩展#include或者#define宏定义,它只是简单的跳过所遇到的任何预处理指令。
