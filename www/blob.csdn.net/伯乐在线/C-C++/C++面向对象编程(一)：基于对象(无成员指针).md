# C++面向对象编程(一)：基于对象(无成员指针) - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [SamLau](http://www.jobbole.com/members/samlau13) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
在面向对象编程中，[类(Class)](https://en.wikipedia.org/wiki/Class_(computer_programming))和[对象(Object)](https://en.wikipedia.org/wiki/Object_(computer_science))是两个非常重要和基本的概念，类(Class)包含成员数据和实现行为的函数，当然还提供构造函数来创建对象。如果是一些需要手动释放内存的语言，例如C++，还提供析构函数来帮助释放内存空间；如果是一些有垃圾回收机制的语言，比如Java，就不需要提供析构函数来释放内存，内存释放交给系统来管理。而对象(Object)是类的实例，每次创建一个对象都有不同的标识符来表示不同的对象，虽然对象中的数据有些是相同的，但它们是否相同根据标识符来判断的。
### 关于成员数据与函数
在C++中，Class有两个经典的分类：
- Class without pointer member (complex复数类)
- Class with pointer member (string字符串类)
一个是类的成员数据不含指针，一个是类的成员数据含指针。complex类来讲述成员数据不含指针。
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1ev29gf5arqj20qr0agtac.jpg)
complex类有两个数据成员：实部和虚部，它们的数据类型都是double，而不是指针；它还定义对复数的基本操作：加、减、乘、除、共轭和正弦等。
string类来讲述成员数据含指针。
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1ev29gfuplpj20qr0agjt1.jpg)
string类有一个数据成员：字符指针s，它指向一串字符；它还定义对字符串的操作：拷贝，输出，附加，插入等。
### Object-Based(基于对象)** vs. Object-Oriented(面向对象)**
类的设计主要分两类，基于对象和面向对象：
- Object-Based：面对的是单个class的设计
- Object-Oriented：面向的是多个classes的设计，class与class之间是有关系的：继承、组合或委托
大家先了解一下这两个概念，后面会有详细介绍。
### C++代码基本形式
C/C++程序都有一个函数入口：main函数。当执行main函数时，大多数都会用到标准库(iostream)和自定义的类(complex)，所以用文件包含指令#include <iostream>来包含I/O标准库，#include “complex.h”来包含自定义类complex。它们之间的语法有一点不同，一个是用尖括号<>来专门包含系统文件和标准库，另一个是用双引号””来包含自定义的类和文件。
使用预处理中的文件包含，能够将一个大文件分离到各种不同职责类的头文件和实现文件。这样不仅减少文件体积而无需加载无用的代码，提供编译速度；还能够提高代码的复用性。
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1ev29gge1f5j20y506twg7.jpg)
扩展文件名(extension file name)不一定是.h或cpp，有可能是.hpp或其他扩展文件名。
### **Header(头文件)防卫式声明**
头文件经常#include其他头文件，甚至一个头文件可能被多次包含进同一个源文件。为了避免重复包含，使用大写的预处理器变量以及其他语句来处理。预处理器变量有两种状态：未定义和已定义；定义预处理器变量和检测其状态所用的预处理指示不同。
#define指示表示定义一个预处理变量，而ifndef指示检测预处理器变量是否未定义；如果未定义，那么跟在其后的所有指示都被处理，如果已经被定义，那么跟在其后的所有指示会跳过不处理。 部分示例代码如下：
complex.h头文件

C++
```
#ifndef __MYCOMPLEX__
#define __MYCOMPLEX__
//Class Declaration
......
#endif
```
complex-test.c测试文件

C++
```
#include <iostream>
#include "complex.h"
using namespace std;
ostream&
operator << (ostream& os, const complex& x)
{
    return os << '(' << real (x) << ',' << imag (x) << ')';
}
int main()
{
    complex c1(2, 1);
    complex c2(4, 0);
    cout << c1 << endl;
    cout << c2 << endl;
    cout << c1+c2 << endl;
    cout << c1-c2 << endl;
    cout << c1*c2 << endl;
    cout << c1 / 2 << endl;
    cout << conj(c1) << endl;
    cout << (c1 += c2) << endl;
    cout << (c1 == c2) << endl;
    cout << (c1 != c2) << endl;
    cout << +c2 << endl;
    cout << -c2 << endl;
    cout << (c2 - 2) << endl;
    cout << (5 + c2) << endl;
    return 0;
}
```
**Class的声明**
首先给出complex类声明的代码，然后逐步来解析各个部分，示例代码如下：

C++
```
// forward declarations (前置声明)
class complex; 
complex&
  __doapl (complex* ths, const complex& r);
// class declarations (类声明)
class complex
{
public:
    complex (double r = 0, double i = 0): re (r), im (i) { }
    complex& operator += (const complex&);
    complex& operator -= (const complex&);
    double real () const { return re; }
    double imag () const { return im; }
private:
    double re, im;
    friend complex& __doapl (complex *, const complex&);
};
// no-member function definition (非成员函数定义)
inline complex&
__doapl (complex* ths, const complex& r)
{
    ths->re += r.re;
    ths->im += r.im;
    return *ths;
}
// class definition (类定义)
// operator overloading (成员函数-操作符重载)
inline complex&
complex::operator += (const complex& r)
{
    return __doapl (this, r);
}
// operator overloading(非成员函数-操作符重载)
inline double
imag (const complex& x)
{
    return x.imag ();
}
inline double
real (const complex& x)
{
    return x.real ();
}
inline complex
operator + (const complex& x, const complex& y)
{
    return complex (real (x) + real (y), imag (x) + imag (y));
}
inline complex
operator + (const complex& x, double y)
{
    return complex (real (x) + y, imag (x));
}
inline complex
operator + (double x, const complex& y)
{
    return complex (x + real (y), imag (y));
}
```
更加详细的示例代码下载地址：[C++面向对象高级编程](https://github.com/samlaudev/CppObjectOrientedProgramming)
### **Access Level(访问级别)**
上面有两个关键字public和private来标明数据成员和成员函数的访问级别，public表示类的外部能够访问类里面的数据或函数，而private表示类的外部不能访问类里面的数据和函数，只允许类内部来访问；通常使用private来修饰数据成员来封装数据，不让类外部的数据轻易访问。如果类外部的数据想访问，就定义一些public的accessors方法来暴露给外部接口来访问。
### **Constructor(构造函数)**
如果你使用类来创建对象并初始化数据成员，就需要定义构造函数。complex类的构造函数定义如下：

C++
```
// 使用初始化列表 (推荐使用)
complex (double r = 0, double i = 0)
    : re(r), im(i)
{}
```
或

C++
```
// 使用函数体 (不推荐使用)
complex (double r = 0, double i = 0)
{
    re = r;
    im = i;
}
```
在定义构造函数时，需要指定类名complex，数据成员(double r = 0, double i = 0)作为参数和函数体，但并不需要返回值，它还为参数设置默认值(r = 0, i =0)。但有一个问题值得注意：究竟在哪里初始化数据成员呢？大多数的C++程序员都会在构造函数函数体来初始化，但有经验的C++程序员都会使用初始化列表。
从概念上讲，构造函数分为两个阶段执行：(1)使用初始化列表来初始化阶段；(2)普通的计算阶段，也就是构造函数的函数体中所有的语句。虽然complex类这个例子，使用其中一种方式会让最终效果一样，但有些情况只能使用初始化列表。看下面这个例子：

C++
```
class ConstRef
{
    public:
        ConstRef(int ii);
    private:
        int i;
        const int ci;
        int &ri;
}
ConstRef::ConstRef(int ii)
{
    i = ii;    // ok
    ci = ii;  // error: 不能给一个const赋值
    ri = i;   // error: 不能绑定到其他对象，ri已经被初始化过
}
```
> 
注意：没有默认构造函数的类数据成员，以及const或引用类型的成员，不管哪种类型，都必须在构造函数初始化列表中进行初始化。
所以上面那个例子应该改为：

C++
```
ConstRef::ConstRef(int ii)
: i(ii), ci(ii), ri(ii) {}
```
> 
建议：使用构造函数初始化列表，而不是函数体来初始化数据成员。
### **重载(Overloaded)函数**
在设计构造函数创建对象时，可能需要不同参数来创建对象，这时需要重载函数。
重载函数：出现在相同作用域中两个函数，如果有相同的名字而形参表不同，则称为重载函数。
就我们这个complex类的构造函数而言，有两个构造函数：

C++
```
complex (double r = 0, double i = 0)
: re(r), im(i) {}
complex (double r) : re(r), im(0) {}
```
第一个是有两个参数的构造函数，第二个是没有任何参数的构造函数，虽然它们的函数名相同，但由于它们的参数不同，C++编译器能够分辨出两个不同的函数，从而调用对应的构造函数。当使用complex c1(2, 3)创建对象时，对应会调用第一个构造函数。而当使用complex c2(2)创建对象时，对应会调用第二个构造函数。
当然，重载函数的概念不仅仅是用在构造函数，而应用在所有类型的函数，包括内联函数和普通的函数。
### **Inline(内联)函数**
对于一些简单操作，我们有时将它定义为函数，例如：

C++
```
// find longer of two strings
const string& shorterString(const string& s1, const string& s2)
{
    return s1.size() < s2.size() ? s1 : s2;
}
```
这样做的话，有几点好处：
- 使用函数可以确保统一的行为，并可以测试。
- 阅读和理解函数shorterString的调用，要比读一条用等价的条件表达式取代函数调用更加容易理解。
- 如果需要做任何修改，修改函数要比逐条修改条件表达式更加容易。
- 函数可以重用，不必为其他应用重写代码。
但简短的shorterString函数有个潜在的缺点：就是调用函数比求解条件表达式要慢的多，因为调用函数一般都要做以下工作：
- 调用前要先保存寄存器，并在返回时恢复
- 复制实参
- 程序转向一个新位置执行。
**内联函数避免函数调用的开销**
如果使用内联函数，就可以避免函数调用的开销。编译器会将内联函数在程序中每个调用点“内联地”展开。假设我们将shorterString定义为内联函数，则调用：

C++
```
cout << shorterString(s1, s2) << endl;
```
在编译时就会展开为：


```
cout << s1.size() < s2.size() ? s1 : s2 << endl;
```
**内联函数放在头文件**
内联函数应该在头文件定义，这一点不同于其他函数，这样编译器才能在调用点内联展开函数代码。内联机制适用于只有几行且经常被调用的代码，如果代码行数或操作太多，即使你使用inline关键字来修饰函数，编译器也不会将它看作为内联函数。
### **Const(常量)成员函数**
每个成员函数都有一个额外的、隐形的形参this，在调用成员函数时，形参this初始化为调用函数的对象地址。为了理解成员函数的调用，请看complex类这个例子：

C++
```
complex c1 (2, 4); // create object
cout << c1.real() << endl; // access const function real
```
编译器就会重写real函数的调用：


```
complex::real(&c1);
```
在这个调用中，在real函数的参数表中，有个this指针指向c1对象。如果在成员函数声明的形参表后面加入const关键字，那么const改变隐含this形参的类型，即隐含的this形参是一个指向c1对象的const complex*类型指针。因此，real函数对成员变量re所做操作是只能访问，而不能修改。同理，imag成员函数也是。
### **参数传递: pass by value vs. pass by reference**
每次调用函数时，所传递的实参将会初始化对应的形参；参数传递有两种方式：一种是值传递，另一种就是引用传递。如果形参是使用值传递，那么复制实参的值；如果形参是引用传递，那么它只是实参的别名。看complex类这个例子中定义一个函数：

C++
```
complex& operator+= (const complex& );
```
将&符号放在complex类后面，则表示调用函数式是使用引用传递来传递数据。为什么使用引用传递而不使用值传递呢？
**值传递的局限性**
- 当需要在函数中修改实参的值时
- 当传递的实参是大型对象时，复制对象所付出的时间和存储空间代价比较大
- 当没有办法实现对象复制时
**参数传递选择**
- 优先考虑引用传递(const)，避免复制
- 当在函数中处理后的结果是使用局部变量来存储，而不是形参的引用参数，使用值传递来返回。
### **Friend(友元)**
在某些情况下，允许特定的非成员函数访问一个类的私有成员，同时仍然阻止一般的访问。例如，被重载的操作符，如输入或输出操作符，经常需要访问类的私有数据成员，这些操作不可能为类的成员；然而，尽管不是类的成员，它们仍是类的“接口组成部分”。
友元机制允许一个类将对其非公有成员的访问权授予指定的函数或类。友元的声明以关键字friend开始，它只能出现在类定义的内部。
以complex类为例，它有一个友元函数__doapl：

C++
```
friend complex& __doapl (complex*, const complex&);
```
由于它参数是complex类，在函数内部需要访问到complex类的私有数据re和im，虽然可以通过real()和imag()函数来访问，但是如果直接访问re和im两个数据成员，就能提高程序运行速度。
> 
重要提示： 相同class的各个objects互为friends(友元)
### **(Operator Overloading)操作符重载**
C语言的操作符只能应用在基本数据类型，例如：整形、浮点型等。但C++的基本组成单元是类，如果对类的对象也能进行加减乘除等操作符运算，那么操作起来比调用函数更加方便。因此C++提供操作符重载来支持类与类之间也能使用操作符来运算。
操作符重载是具有特殊名称的函数：关键字operator后接需要定义的操作符符号。像任意其他函数一样，操作符重载具有返回值和形参表。
如果想操作符重载，有两种选择：
- 成员函数的操作符重载
- 非成员函数的操作符重载
两者之间有什么不同呢？对于成员函数的操作符重载，每次调用成员函数时，都会有一个隐含this形参，限定为第一个操作数，而this指针的数据类型是固定的，就是该类类型。而非成员函数的操作符重载，形参表比成员函数灵活，第一个形参不再限死为this形参，而是可以是其他类型的形参。下面我们分别通过两个例子来看看为什么这样选择。
**成员函数的操作符重载**

C++
```
complex c1(2, 1);
complex c2 (5);
c2 += c1;
```
上面代码创建两个complex对象c1和c2，然后使用+=操作符来进行相加赋值操作。我们站在设计API角度来思考，如果重载操作符+=的话，需要提供两个参数(complex&, complex&)，但由于第一个参数类型是complex&跟成员函数this形参一样，所以优先考虑成员函数。
complex类重载+=操作符：

C++
```
complex& operator += (const complex&);
```
而代码实现放在类声明外面：

C++
```
inline complex&
complex::operator += (const complex& r)
{
return __doapl (this, r);
}
```
**非成员函数的操作符重载**

C++
```
complex c1(2, 1);
complex c2;
c2 = c1 + c2;
c2 = c1 + 5;
c2 = 7 + c1;
```
上面代码创建两个complex对象c1和c2，然后使用+操作符进行相加操作。
其中有一个c2 = 7 + c1代码片段，第一操作数是double，而不是complex。所以如果还是使用成员函数的话，编译器会报错，因为成员函数的第一个形参类型是complex而不是double。最后我们选择的是使用非成员函数来实现，而不是成员函数。
非成员函数重载+操作符：

C++
```
inline double
imag (const complex& x)
{
    return x.imag ();
}
inline double
real (const complex& x)
{
    return x.real ();
}
inline complex
operator + (const complex& x, const complex& y)
{
    return complex (real (x) + real (y), imag (x) + imag (y));
}
inline complex
operator + (const complex& x, double y)
{
    return complex (real (x) + y, imag (x));
}
inline complex
operator + (double x, const complex& y)
{
    return complex (x + real (y), imag (y));
}
```
**Temp Object(临时对象)**

C++
```
inline complex
operator + (const complex& x, const complex& y)
{
    return complex (real (x) + real (y), imag (x) + imag (y));
}
```
上面用非成员函数实现+操作符重载时，计算后结果没有使用引用形参来保存，而是使用一种特殊对象叫临时对象来保存，它是一个局部变量。语法是typename(data)，typename表示类型，data表示传入的数据。
### **总结**
当设计一个C++类的时候，需要思考一下问题：
- 首先要考虑它是基于对象(单个类)还是面向对象(多个类)的设计
- 类由数据成员和成员函数组成；一般来说，数据成员的访问权限应该设置为private，以防止类的外部随意访问修改数据。如果类的外部想访问数据，类可以定义数据成员的setter和getter。由于getter是不会改变数据成员的值，所以用const关键字修饰函数，防止getter函数修改数据
- 考虑完数据成员之后，然后考虑函数的设计。要创建对象，需要在类中定义构造函数。构造函数的参数一般是所有的私有数据成员，而要初始化数据成员，一般采用初始化列表，而不使用构造函数的函数体。
- 而对于一般的函数，在参数设计中，除了考虑变量名和数据类型之外，还要考虑参数传递、是否使用const修饰和有没有默认值等，参数传递优先考虑引用传递(避免复制开销)，而不是值传递，返回值也是一样。当在函数体内处理完结果之后，没使用引用形参来存储结果的话，可以使用临时对象存储并返回结果。有些函数实现只有几个操作的简短代码，将实现代码放在头文件，设置函数为inline。
- 当重载操作符时，可以使用两种方式来实现：成员函数和非成员函数。当第一个操作数是固定的类类型，优先使用成员函数，否则就使用非成员函数。
暂时总结这么多，后续还有其他C++面向对象编程的总结，会继续补充！！！
## 扩展阅读
[极客班[C++系统工程师教程]](http://geekband.com/major/cpp)
[C++ Primer](http://book.douban.com/subject/1767741/)
