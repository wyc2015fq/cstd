# QT的信号与槽机制介绍（3） - gauss的专栏 - CSDN博客
2011年12月11日 09:33:51[gauss](https://me.csdn.net/mathlmx)阅读数：236标签：[qt																[signal																[编译器																[object																[扩展																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
六、程序样例
这里给出了一个简单的样例程序，程序中定义了三个信号、三个槽函数，然后将信号与槽进行了关联，每个槽函数只是简单的弹出一个对话框窗口。读者可以用kdevelop生成一个简单的QT应用程序，然后将下面的代码添加到相应的程序中去。
信号和槽函数的声明一般位于头文件中，同时在类声明的开始位置必须加上Q_OBJECT语句，这条语句是不可缺少的，它将告诉编译器在编译之前必须先应用moc工具进行扩展。关键字signals指出随后开始信号的声明，这里signals用的是复数形式而非单数，siganls没有public、private、protected等属性，这点不同于slots。另外，signals、slots关键字是QT自己定义的，不是C++中的关键字。
信号的声明类似于函数的声明而非变量的声明，左边要有类型，右边要有括号，如果要向槽中传递参数的话，在括号中指定每个形式参数的类型，当然，形式参数的个数可以多于一个。
关键字slots指出随后开始槽的声明，这里slots用的也是复数形式。
槽的声明与普通函数的声明一样，可以携带零或多个形式参数。既然信号的声明类似于普通C++函数的声明，那么，信号也可采用C++中虚函数的形式进行声明，即同名但参数不同。例如，第一次定义的void mySignal()没有带参数，而第二次定义的却带有参数，从这里我们可以看到QT的信号机制是非常灵活的。
信号与槽之间的联系必须事先用connect函数进行指定。如果要断开二者之间的联系，可以使用函数disconnect。
//tsignal.h
...
class TsignalApp:public QMainWindow
{
Q_OBJECT
...
//信号声明区
signals:
//声明信号mySignal()
void mySignal();
//声明信号mySignal(int)
void mySignal(int x);
//声明信号mySignalParam(int,int)
void mySignalParam(int x,int y);
//槽声明区
public slots:
//声明槽函数mySlot()
void mySlot();
//声明槽函数mySlot(int)
void mySlot(int x);
//声明槽函数mySignalParam (int，int)
void mySignalParam(int x,int y);
}
...
//tsignal.cpp
...
TsignalApp::TsignalApp()
{
...
//将信号mySignal()与槽mySlot()相关联
connect(this,SIGNAL(mySignal()),SLOT(mySlot()));
//将信号mySignal(int)与槽mySlot(int)相关联
connect(this,SIGNAL(mySignal(int)),SLOT(mySlot(int)));
//将信号mySignalParam(int,int)与槽mySlotParam(int,int)相关联
connect(this,SIGNAL(mySignalParam(int,int)),SLOT(mySlotParam(int,int)));
}
// 定义槽函数mySlot()
void TsignalApp::mySlot()
{
QMessageBox::about(this,"Tsignal", "This is a signal/slot sample without
parameter.");
}
// 定义槽函数mySlot(int)
void TsignalApp::mySlot(int x)
{
QMessageBox::about(this,"Tsignal", "This is a signal/slot sample with
one
parameter.");
}
// 定义槽函数mySlotParam(int,int)
void TsignalApp::mySlotParam(int x,int y)
{
char s[256];
sprintf(s,"x:%d y:%d",x,y);
QMessageBox::about(this,"Tsignal", s);
}
void TsignalApp::slotFileNew()
{
//发射信号mySignal()
emit mySignal();
//发射信号mySignal(int)
emit mySignal(5);
//发射信号mySignalParam(5，100)
emit mySignalParam(5,100);
}
