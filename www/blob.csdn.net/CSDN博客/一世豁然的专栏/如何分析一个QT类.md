# 如何分析一个QT类 - 一世豁然的专栏 - CSDN博客





2015年10月06日 11:19:13[一世豁然](https://me.csdn.net/Explorer_day)阅读数：477标签：[qt																[类](https://so.csdn.net/so/search/s.do?q=类&t=blog)](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/Explorer_day/article/category/5857397)








本博客转载于：[http://www.cnblogs.com/gnuhpc/archive/2012/12/10/2811957.html](http://www.cnblogs.com/gnuhpc/archive/2012/12/10/2811957.html)







作者：[gnuhpc](http://www.cnblogs.com/gnuhpc/)

出处：[http://www.cnblogs.com/gnuhpc/](http://www.cnblogs.com/gnuhpc/)


我们以QLineEdit这个类为例来看看如何学习分析一个QT类。

1.Public Types：


这是一个在这个类中可以供我们使用的公共枚举类型。这也就是说，我们可以如下使用这个公共类型：
// some code
..
QLineEdit::EchoMode mode;
mode = myLineEdit->echoMode();
 
...
 
mode = QLineEdit::NoEcho;
myLineEdit->setEchoMode( mode );
// or directly myLineEdit->setEchoMode( QLineEdit::NoEcho );

Public types经常使用在Properties中。

2.Properties


Properties可以被get和set，上边的例子就有所体现。

3.Public Functions


这个部分提供了大量这个类实例的方法，textEdit->append( "Path to file: " + lineEdit->text() );

4.Public Slots


这个部分描述了我们能连接信号的一些动作，这些动作构成了与其连接的signal的结果，也就是说，一个signal发出回导致一个一个slot的动作发生。我们也可以自定义信号槽，比如：

myqtapp.h


public slots: 


void mySlot(const QString&);

myqtapp.cpp
void myQtApp::mySlot(const QString& passedStr)
{
    QMessageBox::information(this, "Some window label here", "String: " + passedStr);
}然后我们以这样的方式连接这个槽：connect( lineEdit, SIGNAL( textEdited (const QString&) ), this, SLOT( mySlot(const QString&) ) );
5.Signals


这个部分描述了这个类能发出信号的部分，这些信号与特定的slot连接，导致了slot对应动作的发生。也就是说，信号相当于原因，而slots相当于结果。比如
 connect( pushButton_clear, SIGNAL( clicked() ), this, SLOT( clear() ) );

在这里我们把pushButton_clear的clicked信号连接到了this的clear() 槽上，当这个按键按下时，this就会执行这个clear的动作。我们也可以自定义信号：

myqtapp.h
public:
    myQtApp(QWidget *parent = 0);
 
signals:
    void customSignal( const QString& );
..
myqtapp.cpp - 要使用emit关键字


void myQtApp::clear() 

{ 
emit customSignal( textEdit->toPlainText() );//信号在此发出，带一个字符串 
    textEdit->clear(); 

}


我们可以如下连接我们自定义的信号：connect( this, SIGNAL( customSignal (const QString&) ), this, SLOT( mySlot(const QString&) ) );

6.Protected functions


这个就是C++传统意义上的Protected方法。

7.Static Public Members


这个也是指全局都能使用的类方法，不需要创建实例就可以直接调用。

8.const QString &


这个定义在QT的文档中经常见到。其实这个也可以不加const &，主要是为了performance——在方法中不需要再复制一份参数，提高了程序性能。const则是说这个传进来的参数不可修改。

9.在堆上或者栈上分配内存


例如，我们使用QProcess这个类，我们写了如下的函数：
void myQtApp::function() 
{
    QProcess proc; // we create this proc variable on stack
    proc.start("regedit.exe"); // start program regedit.exe
}

我们以为会调用这个exe，但实际上并没有，终端调试信息显示：


QProcess: Destroyed while process is still running.


这是因为变量在程序栈上，当这个变量越出了这个范围，栈上的这个部分的内存就会被释放。在我们这个例子中，proc在这个程序执行结束前就被销毁了。解决的方法就是在堆上建立这个变量：当程序员使用new进行动态内存分配的时候，内存是分配在堆（虚拟内存，包括板上RAM和硬盘剩余空间所分配的硬盘）上的。
void myQtApp::function() 
{
    QProcess *proc; // pointer definition
    proc = new QProcess( this ); // memory allocation from heap, created with parent
    proc->start("regedit.exe"); // start program
}

注意使用this的的原因是父进程销毁后子进程亦销毁。


另外一种方法是将proc放入类myQtApp中成为一个类成员。


作者：[gnuhpc](http://www.cnblogs.com/gnuhpc/)

出处：[http://www.cnblogs.com/gnuhpc/](http://www.cnblogs.com/gnuhpc/)





