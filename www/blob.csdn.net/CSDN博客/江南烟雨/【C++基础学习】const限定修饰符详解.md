# 【C++基础学习】const限定修饰符详解 - 江南烟雨 - CSDN博客
2012年03月30日 20:05:03[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：1386
C++中const的一个很重要的作用就是把一个对象转换成一个常量。在程序中任何改变这个值的企图会被标记为编译错误。它被称为是“只读的”。
**（1）const对象在定义的时候必须被初始化。未初始化的常量会引起编译错误。**
原因是因为常量在定义之后就不能被修改，所以必须被初始化。
**（2）不能将一个非const对象的指针指向一个const对象。**
比如下面的代码：
const int constVal = 100;
int *intPtr = &constVal;//OK or ERROR？
是否会出错呢？
我们可以编写下面这行代码：
*intPtr += 1;
这样就修改了const型对象constVal的值。注意：一般编译器不能跟踪指针在程序中任意一点指向的对象。所以将一个非const对象的指针指向一个const对象会引起编译时刻错误。
**（3）const对象的地址只能赋给指向const对象的指针。**
**（4）指向const对象的指针可以被赋予一个非const对象的地址。**
例如下面的代码：
int iVal = 20;
const int *iPtr = &iVal;
但是我们不能通过iPtr解引用来修改iVal的值，虽然iVal不是常量。否则会引起编译错误。
在实际应用中，指向const对象的指针经常被用作函数的参数，作用是可以保证被传递给函数的实际对象子啊函数中不会被修改。
**（5）const的使用**
定义常量：
int const a = 1;和cons int a = 1；等价；
**（6）当与指针放在一起使用的时候，就稍微有点复杂。**
const int *intPtr;
我们可以**从右往左看**，它是指向常量int型的非const型指针。指针所指向的对象不可改变，但是指针本身是可以改变的。比如我们可以使它指向另外一个int型对象：
int a = 1;
intPtr = &a;
而：
int a = 1;
int *const intPtr = &a;
是常量指针，指向非const类型的int型变量。指针本身不可改变，即不可赋给它其他的地址值。但是可以修改intPtr所指向的对象的值：
*intPtr = 2;
视图给const指针赋值会引起编译错误：
int b = 3;
intPtr = &b;
const int a = 4;
const int *const cicp = &a;
这是指向const对象的const指针。指针所指向的值和指针本身都不可改变。
**（7）const与引用在一起使用：**
1、const引用可以用不同该类型的对象初始化（只要能从一个类型转化到另一种类型即可）。也可以是不可寻址的值如文字常量。
例如：
double dVal = 1.18;
const int &ir = 12;
const int &ir2 = dVal;
const double &dr = dVal = 1.0;
注意：同样的初始化对于非const引用是不合法的。
引用在内部存放的是一个对象的地址，它是该对象的别名。
对于不可寻址的值比如文字常量以及不同类型的变量，编译器为了实现引用，必须生成一个临时变量，引用实际上指向该临时变量。
例如：
double dVal = 12;
const int &ri = dVal;
编译器实际上将其转化为：
int temp = dVal;
const int &ri = &temp;
这样，如果我们给ri赋给一个新值，则不会改变dVal的值，改变的是temp。用户不会看到这一改变。
下面我们要用一个const对象的地址来初始化一个引用。
非const引用的定义是非法的，将导致编译时刻错误：
const int iVal = 12;
int *&pi = &iVal;
我们可能想进行如下修改：
const int *&pi = &iVal;
但是仍然不正确。因为引用pi是一个指向定义为const的int型对象的指针。我们的引用不是指向一个常量，而是指向一个非常量指针。正确定义如下：
int *const &pi = &iVal;
**（8）const与typedef一起使用**
一个有趣的问题：
typedef char *cstring;
extern const cstring cstr;
请问cstr的类型是什么？
很多人都会回答是const char*，即指向const字符的指针。但是这是错误的。**错误地把typedef当做宏扩展了。在这里typedef并不是简单地宏扩展。**
cstring在这里是个类型就像const int a中int是个类型一样。这里说明cstr是const类型的。而cstr是个指针，所以它是const型的指针，指向的是char型。
