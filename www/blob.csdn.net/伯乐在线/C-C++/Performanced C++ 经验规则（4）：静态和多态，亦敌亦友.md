# Performanced C++ 经验规则（4）：静态和多态，亦敌亦友 - 文章 - 伯乐在线
原文出处： [张峻崇](http://www.cnblogs.com/ccdev/archive/2012/12/25/2832268.html)
这一篇，我们讨论C++中静态和多态的关系。我们都知道，C++并不是一门“动态”语言，虽然它提供了同样强大于其它动态语言的多态性，但很多时候，我们之所以选择C++，看重中正是其“静态”所带来的High Performance。所谓静态，通常是指，在程序运行的过程，是“静止”不变，固定的（特别是内存地址），当然“多态”就是与之对立的概念。这一篇我们并不讨论静态（成员）变量或静态（成员）函数有什么作用，而是讨论“静态”的行为，对比“多态”。我们这里所说的静态，是指：compiler time，即编译时绑定、早绑定、静态联编；而“多态”就是真正的runtime绑定、晚绑定、动态联编。
很奇妙，这一组对立的概念，却可以在C++中和平共存，时而协同工作。
老规矩，还是一小段代码提出问题，当一个虚成员函数（多态性）在其子类中被声明为静态成员函数时（或相反过来），会发生什么？
1、当虚函数遭遇静态函数

C++
```
#include <iostream>
using namespace std;
class Base
{
public:
    virtual void foo(void){ cout << "Base::foo()" << endl; }
};
class Derived : public Base
{
public:
    void foo(void){ cout << "Derived::foo()" << endl; }
} ;
class DerivedAgain : public Derived
{
public:
    static void foo(void){ cout << "DerivedAgain::foo()"<< endl; }
} ;
int main(int argc, char** argv)
{
    DerivedAgain da;
    Base* pB = &da;
    da.foo();
    pB->foo(); 
    return 0;
}
```
上述代码运行结果是什么？等等，你确定上述代码能通过编译？在笔者Ubuntu 12.04 + gcc 4.6.3的机器上，上述代码**编译不能通过**。显示如下信息：

C++
```
stawithvir.cpp:19:17: error: ‘static void DerivedAgain::foo()’ cannot be declared
stawithvir.cpp:13:10: error:   since ‘virtual void Derived::foo()’ declared in base class
```
很明显，**编译不能通过的原因，是在DerivedAgain类中将虚函数声明为static**，编译器拒绝此“静态”与“多态”的和平共处。此时理由很简单，static成员函数，是类级共享的，不属于任何对象，也不会传入this指针，不能访问非静态成员；然而，虚函数的要求与此正相反，需要绑定对象（this指针），进而获得虚表，然后进行调用。如此矛盾的行为，编译器情何以堪，因为选择报错来表达其不满。我们可以暂时记住结论：**不能将虚函数声明为静态的**。
接下来你可能会问，编译都不能通过的东西，对错不是明摆着的吗？为什么还要拿来讨论，这是因为，在某些编译器上（可以在VC6，VC2008等尝试），该代码能编译通过，并输出结果，不可思议？不过这些编译器同时也给出了一个警告（参与MSDN warning c4526），指出静态函数不能用做虚函数进行调用。虽然通过了编译，但思想与上述Gcc是一致的。

C++
```
//输出结果
DerivedAgain::foo()
Derived::foo()
```
da.foo()输出DerivedAgain::foo()没有疑问（通过对象调用方法，无论是否虚方法，本来就不会产生动态绑定，即无虚特性）；而pB->foo()输出Derived::foo()则需要解释一下，因为pB是指针调用虚方法，产生“多态”，动态绑定时发现pB指向的对象类型为DerivedAgain，于是去查找DerivedAgain对象虚表中foo()的地址，但此时发现DerivedAgain的虚表中foo()的地址其实是Derived::foo()，**因为DerivedAgain中的foo已经被声明为static，不会更新此函数在虚表中的地址**（实际上，由于DerivedAgain没有声明任何新的虚函数，它对象的虚表同Derived对象是完全一样的，如果有兴趣，可以通过汇编查看），所以输出的是Derived::foo()，也从一个侧面证明了：在继承链中，使用最”新”的虚函数版本。
至此，这个问题已经解释清楚，再次记住结论：**静态成员函数，不能同时也是虚函数**。
2、重载（overload）并非真正的多态，其本质是静态行为
笔者曾不止一次的看到，许多书籍、资料，在谈到C++多态性的时候，经常把“重载”（overload）归入多态行为中。这种说法看似也没什么不正确，实际上我认为十分不妥。虽然重载，通过区分特征标的不同（注意，同函数名而参数不同、或同函数名但是否是const成员函数，都是重载依据），而使相同函数名的方法调用产生了不同的行为，确实体现了“多态”的思想，但**重载的本质是静态绑定，是编译期就能确定调用哪个方法，而非动态绑定，所以不是真正的多态**。所以，头脑要清醒，即如果两个（或多个）方法之间的关系是“重载”（overload），那么就不会有真正的多态行为产生。
3、何时产生真正的多态？
讨论重载之后，就要谈到，何时产生真正的多态行为，即动态绑定呢？笔者归纳三个必要条件如下：
（1）方法是虚的；
（2）有覆盖（override）产生；
（3）通过指针或引用调用相应的虚方法，而非通过对象调用；通过对象调用方法，无论方法是否是虚方法，均是静态联编行为。
条件（1）（2）很明显，如果方法是虚的也没有覆盖，何来“多”的“态”？而条件（3）容易被新手忽视，因为**通过对象调用，对象的类型已经确知，所以静态绑定，不会再产生多态。而通过指针或引用调用相应虚方法，由于在编译期不能确定指针或引用指向的具体类型，所以只能动态联编，从而产生多态**。
4、不正确的代码将阻止多态行为
好了，接下来我们看一小段代码，来自《C++ Primer Plus》：

C++
```
class Base
{
public:
    virtual void foo(void) {...}
    ...
};
class Derived : public Base
{
public:
    void foo(void) {...}
    ...
};
//版本1
void show1(const Base& b)
{
    b.foo();
}
//版本2
void show2(Base b)
{
   b.foo();
}
int main(int argc, char** argv)
{
    Derived d;
    show1(d);
    show2(d);
    return 0;
}
```
上述代码有什么问题？我们看到，两个版本的show函数唯一不同之处，就是版本1按引用传递对象，版本2按值传递对象。在main函数中，新建了一个Derived对象并传给版本1函数，由于版本1中的参数b是引用类型，OK，没有问题，b.foo()将按照b实际指向的对象调用，即可以正确调用Derived::foo()；而版本2参数b是对象类型（b是Base(const Base&)拷贝构造创建的一个Base对象，自动向上的强制类型转换使得基类拷贝构造函数可以引用一个子类对象），根据上述第3点，则b.foo()将按对象类型（Base）调用到Base::foo()，不产生多态行为。即，**由于按值传递，在此处阻止了动态绑定，阻止了多态行为**。
说到这里的话，又是老生常谈的问题，即**除非必须要这样做，否则不要按值方式传递参数，而应选择指针或引用**，关于这个问题，本系列后面还会再谈。
