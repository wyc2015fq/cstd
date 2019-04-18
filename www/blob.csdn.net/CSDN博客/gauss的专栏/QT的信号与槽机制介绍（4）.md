# QT的信号与槽机制介绍（4） - gauss的专栏 - CSDN博客
2011年12月11日 09:37:48[gauss](https://me.csdn.net/mathlmx)阅读数：287标签：[qt																[class																[pair																[object																[signal																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=pair&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
七、应注意的问题
信号与槽机制是比较灵活的，但有些局限性我们必须了解，这样在实际的使用过程中做到有的放矢，避免产生一些错误。下面就介绍一下这方面的情况。
1．信号与槽的效率是非常高的，但是同真正的回调函数比较起来，由于增加了灵活性，因此在速度上还是有所损失，当然这种损失相对来说是比较小的，通过在一台i586-133的机器上测试是10微秒（运行Linux），可见这种机制所提供的简洁性、灵活性还是值得的。但如果我们要追求高效率的话，比如在实时系统中就要尽可能的少用这种机制。
2．信号与槽机制与普通函数的调用一样，如果使用不当的话，在程序执行时也有可能产生死循环。因此，在定义槽函数时一定要注意避免间接形成无限循环，即在槽中再次发射所接收到的同样信号。例如,在前面给出的例子中如果在mySlot()槽函数中加上语句emit mySignal()即可形成死循环。
3．如果一个信号与多个槽相联系的话，那么，当这个信号被发射时，与之相关的槽被激活的顺序将是随机的。
4. 宏定义不能用在signal和slot的参数中。
既然moc工具不扩展#define，因此，在signals和slots中携带参数的宏就不能正确地工作，如果不带参数是可以的。例如，下面的例子中将带有参数的宏SIGNEDNESS(a)作为信号的参数是不合语法的：
#ifdef ultrix
#define SIGNEDNESS(a) unsigned a
#else
#define SIGNEDNESS(a) a
#endif
class Whatever : public QObject
{
[...]
signals:
void someSignal( SIGNEDNESS(a) );
[...]
};
5. 构造函数不能用在signals或者slots声明区域内。
的确，将一个构造函数放在signals或者slots区内有点不可理解，无论如何，不能将它们放在private slots、protected slots或者public slots区内。下面的用法是不合语法要求的：
class SomeClass : public QObject
{
Q_OBJECT
public slots:
SomeClass( QObject *parent, const char *name )
: QObject( parent, name ) {} // 在槽声明区内声明构造函数不合语法
[...]
};
6. 函数指针不能作为信号或槽的参数。
例如，下面的例子中将void (*applyFunction)(QList*, void*)作为参数是不合语法的：
class someClass : public QObject
{
Q_OBJECT
[...]
public slots:
void apply(void (*applyFunction)(QList*, void*), char*); // 不合
语法
};
你可以采用下面的方法绕过这个限制：
typedef void (*ApplyFunctionType)(QList*, void*);
class someClass : public QObject
{
Q_OBJECT
[...]
public slots:
void apply( ApplyFunctionType, char *);
};
7.信号与槽不能有缺省参数。
既然signal->slot绑定是发生在运行时刻，那么，从概念上讲使用缺省参数是困难的。下面的用法是不合理的：
class SomeClass : public QObject
{
Q_OBJECT
public slots:
void someSlot(int x=100); // 将x的缺省值定义成100，在槽函数声明中使用是错误的
};
8.信号与槽也不能携带模板类参数。
如果将信号、槽声明为模板类参数的话，即使moc工具不报告错误，也不可能得到预期的结果。例如，下面的例子中当信号发射时，槽函数不会被正确调用：
[...]
public slots:
void MyWidget::setLocation (pair location);
[...]
public signals:
void MyObject::moved (pair location);
但是，你可以使用typedef语句来绕过这个限制。如下所示：
typedef pair IntPair;
[...]
public slots:
void MyWidget::setLocation (IntPair location);
[...]
public signals:
void MyObject::moved (IntPair location);
这样使用的话，你就可以得到正确的结果。
9.嵌套的类不能位于信号或槽区域内，也不能有信号或者槽。
例如，下面的例子中，在class B中声明槽b()是不合语法的，在信号区内声明槽b()也是不合语法的。
class A
{
Q_OBJECT
public:
class B
{
public slots: // 在嵌套类中声明槽不合语法
void b();
[....]
};
signals:
class B
{
// 在信号区内声明嵌套类不合语法
void b();
[....]
}:
};
10.友元声明不能位于信号或者槽声明区内。相反，它们应该在普通C++的private、protected或者public区内进行声明。下面的例子是不合语法规范的：
class someClass : public QObject
{
Q_OBJECT
[...]
signals: //信号定义区
friend class ClassTemplate; // 此处定义不合语法
};
