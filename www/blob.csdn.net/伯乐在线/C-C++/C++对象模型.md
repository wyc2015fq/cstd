# C++对象模型 - 文章 - 伯乐在线
原文出处： [吴秦的博客]()
### **何为C++对象模型？**
C++对象模型可以概括为以下2部分：
1. 语言中直接支持面向对象程序设计的部分
2. 对于各种支持的底层实现机制
语言中直接支持面向对象程序设计的部分，如构造函数、析构函数、虚函数、继承（单继承、多继承、虚继承）、多态等等，这也是组里其他同学之前分享过的内容。第一部分这里我简单过一下，重点在底层实现机制。
在c语言中，“数据”和“处理数据的操作（函数）”是分开来声明的，也就是说，语言本身并没有支持“数据和函数”之间的关联性。在c++中，通过抽象数据类型（abstract data type，ADT），在类中定义数据和函数，来实现数据和函数直接的绑定。
概括来说，在C++类中有两种成员数据：static、nonstatic；三种成员函数：static、nonstatic、virtual。
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1es19a5l0fej20bd05uwfd.jpg)
如下面的Base类定义：

C++
```
#pragma once
#include<iostream>
using namespace std;
class Base
{
public:
    Base(int);
    virtual ~Base(void);
    int getIBase() const;
    static int instanceCount();
    virtual void print() const;
protected:
    int iBase;
    static int count;
};
```
Base类在机器中我们如何构建出各种成员数据和成员函数的呢？
### **基本C++对象模型**
在介绍C++使用的对象模型之前，介绍2种对象模型：简单对象模型（a simple object model）、表格驱动对象模型（a table-driven object model）。
**简单对象模型（a simple object model）**
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1es19a5wk0ij20at08jjsw.jpg)
所有的成员占用相同的空间（跟成员类型无关），对象只是维护了一个包含成员指针的一个表。表中放的是成员的地址，无论上成员变量还是函数，都是这样处理。对象并没有直接保存成员而是保存了成员的指针。
**表格对象模型（a table-driven object model）**
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1es19a69gucj20em0b30uw.jpg)
这个模型在简单对象的基础上又添加了一个间接层。将成员分成函数和数据，并且用两个表格保存，然后对象只保存了两个指向表格的指针。这个模型可以保证所有的对象具有相同的大小，比如简单对象模型还与成员的个数相关。其中数据成员表中包含实际数据；函数成员表中包含的实际函数的地址（与数据成员相比，多一次寻址）。
**C++对象模型**
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1es19a6lc56j20ev095jt9.jpg)
这个模型从结合上面2中模型的特点，并对内存存取和空间进行了优化。在此模型中，non static 数据成员被放置到对象内部，static数据成员， static and nonstatic 函数成员均被放到对象之外。对于虚函数的支持则分两步完成：
1. 每一个class产生一堆指向虚函数的指针，放在表格之中。这个表格称之为虚函数表（virtual table，vtbl）。
2. 每一个对象被添加了一个指针，指向相关的虚函数表vtbl。通常这个指针被称为vptr。vptr的设定（setting）和重置（resetting）都由每一个class的构造函数，析构函数和拷贝赋值运算符自动完成。
另外，虚函数表地址的前面设置了一个指向type_info的指针，RTTI（Run Time Type Identification）运行时类型识别是有编译器在编译器生成的特殊类型信息，包括对象继承关系，对象本身的描述，RTTI是为多态而生成的信息，所以只有具有虚函数的对象在会生成。
这个模型的优点在于它的空间和存取时间的效率；缺点如下：如果应用程序本身未改变，但当所使用的类的non static数据成员添加删除或修改时，需要重新编译。
**模型验证测试**
为了验证上述C++对象模型，我们编写如下测试代码。

C++
```
void test_base_model()
{
    Base b1(1000);
    cout << "对象b1的起始内存地址：" << &b1 << endl;
    cout << "type_info信息：" << ((int*)*(int*)(&b1) - 1) << endl;
    RTTICompleteObjectLocator str=
        *((RTTICompleteObjectLocator*)*((int*)*(int*)(&b1) - 1));
    //abstract class name from RTTI
    string classname(str.pTypeDescriptor->name);
    classname = classname.substr(4,classname.find("@@")-4);
    cout << classname <<endl;
    cout << "虚函数表地址：\t\t\t" << (int*)(&b1) << endl;
    cout << "虚函数表 — 第1个函数地址：\t" << (int*)*(int*)(&b1) << "\t即析构函数地址：" << (int*)*((int*)*(int*)(&b1)) << endl;
    cout << "虚函数表 — 第2个函数地址：\t" << ((int*)*(int*)(&b1) + 1) << "\t";
    typedef void(*Fun)(void);
    Fun pFun = (Fun)*(((int*)*(int*)(&b1)) + 1);
    pFun();
    b1.print();
    cout << endl;
    cout << "推测数据成员iBase地址：\t\t" << ((int*)(&b1) +1) << "\t通过地址取值iBase的值：" << *((int*)(&b1) +1) << endl;
    cout << "Base::getIBase(): " << b1.getIBase() << endl;
    b1.instanceCount();
    cout << "静态函数instanceCount地址： " << b1.instanceCount << endl;
}
```
根据C++对象模型，实例化对象b1的起始内存地址，即虚函数表地址。
-  虚函数表的中第1个函数地址是虚析构函数地址；
-  虚函数表的中第2个函数地址是虚函数print()的地址，通过函数指针可以调用，进行验证；
-  推测数据成员iBase的地址，为虚函数表的地址 + 1，((int*)(&b1) +1)；
-  静态数据成员和静态函数所在内存地址，与对象数据成员和函数成员位段不一样；
下面是测试代码输出：（从下面2个图验证了，上面的观点。）
注意：本测试代码及后面的测试代码中写的函数地址，是对应虚函数表项的地址，不是实际的函数地址。
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1es19a72my4j20fe0903zg.jpg)
图：测试代码输出结果
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1es19a7gdf8j20fe02sdg3.jpg)
图：vs断点观察（注意看虚函数表中第一个函数的地址，名称与测试代码输出一致）
上面介绍并验证了基本的C++对象模型，引入继承之后，C++对象模型又是怎样的？
### **C++对象模型中加入单继承**
不管是单继承、多继承，还是虚继承，如果基于“简单对象模型”，每一个基类都可以被派生类中的一个slot指出，该slot内包含基类对象的地址。这个机制的主要缺点是，因为间接性而导致空间和存取时间上的额外负担；优点则是派生类对象的大小不会因其基类的改变而受影响。
如果基于“表格驱动模型”，派生类中有一个slot指向基类表，表格中的每一个slot含一个相关的基类地址（这个很像虚函数表，内含每一个虚函数的地址）。这样每个派生类对象汗一个bptr，它会被初始化，指向其基类表。这种策略的主要缺点是由于间接性而导致的空间和存取时间上的额外负担；优点则是在每一个派生类对象中对继承都有一致的表现方式，每一个派生类对象都应该在某个固定位置上放置一个基类表指针，与基类的大小或数量无关。第二个优点是，不需要改变派生类对象本身，就可以放大，缩小、或更改基类表。
不管上述哪一种机制，“间接性”的级数都将因为集成的深度而增加。C++实际模型是，对于一般继承是扩充已有存在的虚函数表；对于虚继承添加一个虚函数表指针。
**无重写的单继承**
无重写，即派生类中没有于基类同名的虚函数。

C++
```
#pragma once
#include "base.h"
class Derived :
    public Base
{
public:
    Derived(int);
    virtual ~Derived(void);
    virtual void derived_print(void);
protected:
    int iDerived;
};
```
Base、Derived的类图如下所示：
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1es19a82xzsj204j0d7js6.jpg)
Base的模型跟上面的一样，不受继承的影响。Derived不是虚继承，所以是扩充已存在的虚函数表，所以结构如下图所示：
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1es19a8lynnj20fd073tan.jpg)
为了验证上述C++对象模型，我们编写如下测试代码。

C++
```
void test_single_inherit_norewrite()
{
    Derived d(9999);
    cout << "对象d的起始内存地址：" << &d << endl;
    cout << "type_info信息：" << ((int*)*(int*)(&d) - 1) << endl;
    RTTICompleteObjectLocator str=
        *((RTTICompleteObjectLocator*)*((int*)*(int*)(&d) - 1));
    //abstract class name from RTTI
    string classname(str.pTypeDescriptor->name);
    classname = classname.substr(4,classname.find("@@")-4);
    cout << classname <<endl;
    cout << "虚函数表地址：\t\t\t" << (int*)(&d) << endl;
    cout << "虚函数表 — 第1个函数地址：\t" << (int*)*(int*)(&d) << "\t即析构函数地址" << endl;
    cout << "虚函数表 — 第2个函数地址：\t" << ((int*)*(int*)(&d) + 1) << "\t";
    typedef void(*Fun)(void);
    Fun pFun = (Fun)*(((int*)*(int*)(&d)) + 1);
    pFun();
    d.print();
    cout << endl;
    cout << "虚函数表 — 第3个函数地址：\t" << ((int*)*(int*)(&d) + 2) << "\t";
    pFun = (Fun)*(((int*)*(int*)(&d)) + 2);
    pFun();
    d.derived_print();
    cout << endl;
    cout << "推测数据成员iBase地址：\t\t" << ((int*)(&d) +1) << "\t通过地址取得的值：" << *((int*)(&d) +1) << endl;
    cout << "推测数据成员iDerived地址：\t" << ((int*)(&d) +2) << "\t通过地址取得的值：" << *((int*)(&d) +2) << endl;
}
```
输出结果如下图所示：
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1es19a8xp4tj20fe09ndgu.jpg)
**有重写的单继承**
派生类中重写了基类的print()函数。

C++
```
#pragma once
#include "base.h"
class Derived_Overrite :
    public Base
{
public:
    Derived_Overrite(int);
    virtual ~Derived_Overrite(void);
    virtual void print(void) const;
protected:
    int iDerived;
};
```
Base、Derived_Overwrite的类图如下所示：
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1es19a9agxaj20aj070t9l.jpg)
重写print()函数在虚函数表中表现如下：
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1es19a9n7ibj20fe07n0v0.jpg)
为了验证上述C++对象模型，我们编写如下测试代码。

C++
```
void test_single_inherit_rewrite()
{
    Derived_Overrite d(111111);
    cout << "对象d的起始内存地址：\t\t" << &d << endl;
    cout << "虚函数表地址：\t\t\t" << (int*)(&d) << endl;
    cout << "虚函数表 — 第1个函数地址：\t" << (int*)*(int*)(&d) << "\t即析构函数地址" << endl;
    cout << "虚函数表 — 第2个函数地址：\t" << ((int*)*(int*)(&d) + 1) << "\t";
    typedef void(*Fun)(void);
    Fun pFun = (Fun)*(((int*)*(int*)(&d)) + 1);
    pFun();
    d.print();
    cout << endl;
    cout << "虚函数表 — 第3个函数地址：\t" << *((int*)*(int*)(&d) + 2) << "【结束】\t";
    cout << endl;
    cout << "推测数据成员iBase地址：\t\t" << ((int*)(&d) +1) << "\t通过地址取得的值：" << *((int*)(&d) +1) << endl;
    cout << "推测数据成员iDerived地址：\t" << ((int*)(&d) +2) << "\t通过地址取得的值：" << *((int*)(&d) +2) << endl;
}
```
输出结果如下图所示：
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1es19aa3tbbj20fd07jjs4.jpg)
特别注意下，前面的模型虚函数表中最后一项没有打印出来，本实例中共2个虚函数，打印虚函数表第3项为0。其实虚函数表以0x0000000结束，类似字符串以’’结束。
### **C++对象模型中加入多继承**
从单继承可以知道，派生类中只是扩充了基类的虚函数表。如果是多继承的话，又是如何扩充的？
1) 每个基类都有自己的虚表。
2) 子类的成员函数被放到了第一个基类的表中。
3) 内存布局中，其父类布局依次按声明顺序排列。
4) 每个基类的虚表中的print()函数都被overwrite成了子类的print ()。这样做就是为了解决不同的基类类型的指针指向同一个子类实例，而能够调用到实际的函数。
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1es19aagw48j20970ep403.jpg)
上面3个类，Derived_Mutlip_Inherit继承自Base、Base_1两个类，Derived_Mutlip_Inherit的结构如下所示：
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1es19aawed4j20fd0a4whc.jpg)
为了验证上述C++对象模型，我们编写如下测试代码。

C++
```
void test_multip_inherit()
{
    Derived_Mutlip_Inherit dmi(3333);
    cout << "对象dmi的起始内存地址：\t\t" << &dmi << endl;
    cout << "虚函数表_vptr_Base地址：\t" << (int*)(&dmi) << endl;
    cout << "_vptr_Base — 第1个函数地址：\t" << (int*)*(int*)(&dmi) << "\t即析构函数地址" << endl;
    cout << "_vptr_Base — 第2个函数地址：\t" << ((int*)*(int*)(&dmi) + 1) << "\t";
    typedef void(*Fun)(void);
    Fun pFun = (Fun)*(((int*)*(int*)(&dmi)) + 1);
    pFun();
    cout << endl;
    cout << "_vptr_Base — 第3个函数地址：\t" << ((int*)*(int*)(&dmi) + 2) << "\t";
    pFun = (Fun)*(((int*)*(int*)(&dmi)) + 2);
    pFun();
    cout << endl;
    cout << "_vptr_Base — 第4个函数地址：\t" << *((int*)*(int*)(&dmi) + 3) << "【结束】\t";
    cout << endl;
    cout << "推测数据成员iBase地址：\t\t" << ((int*)(&dmi) +1) << "\t通过地址取得的值：" << *((int*)(&dmi) +1) << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
    cout << "虚函数表_vptr_Base1地址：\t" << ((int*)(&dmi) +2) << endl;
    cout << "_vptr_Base1 — 第1个函数地址：\t" << (int*)*((int*)(&dmi) +2) << "\t即析构函数地址" << endl;
    cout << "_vptr_Base1 — 第2个函数地址：\t" << ((int*)*((int*)(&dmi) +2) + 1) << "\t";
    typedef void(*Fun)(void);
    pFun = (Fun)*((int*)*((int*)(&dmi) +2) + 1);
    pFun();
    cout << endl;
    cout << "_vptr_Base1 — 第3个函数地址：\t" << *((int*)*(int*)((int*)(&dmi) +2) + 2) << "【结束】\t";
    cout << endl;  
    cout << "推测数据成员iBase1地址：\t" << ((int*)(&dmi) +3) << "\t通过地址取得的值：" << *((int*)(&dmi) +3) << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
    cout << "推测数据成员iDerived地址：\t" << ((int*)(&dmi) +4) << "\t通过地址取得的值：" << *((int*)(&dmi) +4) << endl;
}
```
输出结果如下图所示：
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1es19abe8o1j20fe0c5myt.jpg)
### **C++对象模型中加入虚继承**
虚继承是为了解决重复继承中多个间接父类的问题的，所以不能使用上面简单的扩充并为每个虚基类提供一个虚函数指针（这样会导致重复继承的基类会有多个虚函数表）形式。
虚继承的派生类的内存结构，和普通继承完全不同。虚继承的子类，有单独的虚函数表，另外也单独保存一份父类的虚函数表，两部分之间用一个四个字节的0x00000000来作为分界。派生类的内存中，首先是自己的虚函数表，然后是派生类的数据成员，然后是0x0，之后就是基类的虚函数表，之后是基类的数据成员。
如果派生类没有自己的虚函数，那么派生类就不会有虚函数表，但是派生类数据和基类数据之间，还是需要0x0来间隔。
因此，在虚继承中，派生类和基类的数据，是完全间隔的，先存放派生类自己的虚函数表和数据，中间以0x分界，最后保存基类的虚函数和数据。如果派生类重载了父类的虚函数，那么则将派生类内存中基类虚函数表的相应函数替换。
**简单虚继承（无重复继承情况）**
简单虚继承的2个类Base、Derived_Virtual_Inherit1的关系如下所示：
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1es19abttawj20bv070jsi.jpg)
Derived_Virtual_Inherit1的对象模型如下图：
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1es19ac3gujj20fe0am41x.jpg)
为了验证上述C++对象模型，我们编写如下测试代码。

C++
```
void test_single_vitrual_inherit()
{
    Derived_Virtual_Inherit1 dvi1(88888);
    cout << "对象dvi1的起始内存地址：\t\t" << &dvi1 << endl;
    cout << "虚函数表_vptr_Derived..地址：\t\t" << (int*)(&dvi1) << endl;
    cout << "_vptr_Derived — 第1个函数地址：\t" << (int*)*(int*)(&dvi1) << endl;
    typedef void(*Fun)(void);
    Fun pFun = (Fun)*((int*)*(int*)(&dvi1));
    pFun();
    cout << endl;
    cout << "_vptr_Derived — 第2个函数地址：\t" << *((int*)*(int*)(&dvi1) + 1) << "【结束】\t";
    cout << endl;
    cout << "=======================：\t" << ((int*)(&dvi1) +1) << "\t通过地址取得的值：" << (int*)*((int*)(&dvi1) +1) << "\t" <<*(int*)*((int*)(&dvi1) +1) << endl;
    cout << "推测数据成员iDerived地址：\t" << ((int*)(&dvi1) +2) << "\t通过地址取得的值：" << *((int*)(&dvi1) +2) << endl;
    cout << "=======================：\t" << ((int*)(&dvi1) +3) << "\t通过地址取得的值：" << *((int*)(&dvi1) +3) << endl;
    cout << "虚函数表_vptr_Base地址：\t" << ((int*)(&dvi1) +4) << endl;
    cout << "_vptr_Base — 第1个函数地址：\t" << (int*)*((int*)(&dvi1) +4) << "\t即析构函数地址" << endl;
    cout << "_vptr_Base — 第2个函数地址：\t" << ((int*)*((int*)(&dvi1) +4) +1) << "\t";
    pFun = (Fun)*((int*)*((int*)(&dvi1) +4) +1);
    pFun();
    cout << endl;
    cout << "_vptr_Base — 第3个函数地址：\t" << ((int*)*((int*)(&dvi1) +4) +2) << "【结束】\t" << *((int*)*((int*)(&dvi1) +4) +2);
    cout << endl;
    cout << "推测数据成员iBase地址：\t\t" << ((int*)(&dvi1) +5) << "\t通过地址取得的值：" << *((int*)(&dvi1) +5) << endl;
}
```
输出结果如下图所示：
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1es19achidtj20fe0baabm.jpg)
**菱形继承（含重复继承、多继承情况）**
菱形继承关系如下图：
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1es19acz3wgj20fd0euaaz.jpg)
Derived_Virtual的对象模型如下图：
![](http://ww4.sinaimg.cn/mw690/5e4d414cgw1es19adchgpj20fd0ftdld.jpg)
为了验证上述C++对象模型，我们编写如下测试代码。

C++
```
void test_multip_vitrual_inherit()
{
    Derived_Virtual dvi1(88888);
    cout << "对象dvi1的起始内存地址：\t\t" << &dvi1 << endl;
    cout << "虚函数表_vptr_inherit1地址：\t\t" << (int*)(&dvi1) << endl;
    cout << "_vptr_inherit1 — 第1个函数地址：\t" << (int*)*(int*)(&dvi1) << endl;
    typedef void(*Fun)(void);
    Fun pFun = (Fun)*((int*)*(int*)(&dvi1));
    pFun();
    cout << endl;
    cout << "_vptr_inherit1 — 第2个函数地址：\t" << ((int*)*(int*)(&dvi1) + 1) << endl;
    pFun = (Fun)*((int*)*(int*)(&dvi1) + 1);
    pFun();
    cout << endl;
    cout << "_vptr_inherit1 — 第3个函数地址：\t" << ((int*)*(int*)(&dvi1) + 2) << "\t通过地址取得的值：" << *((int*)*(int*)(&dvi1) + 2) << "【结束】\t";
    cout << endl;
    cout << "======指向=============：\t" << ((int*)(&dvi1) +1) << "\t通过地址取得的值：" << (int*)*((int*)(&dvi1) +1)<< "\t" <<*(int*)*((int*)(&dvi1) +1) << endl;
    cout << "推测数据成员iInherit1地址：\t" << ((int*)(&dvi1) +2) << "\t通过地址取得的值：" << *((int*)(&dvi1) +2) << endl;
    //
    cout << "虚函数表_vptr_inherit2地址：\t" << ((int*)(&dvi1) +3) << endl;
    cout << "_vptr_inherit2 — 第1个函数地址：\t" << (int*)*((int*)(&dvi1) +3) << endl;
    pFun = (Fun)*((int*)*((int*)(&dvi1) +3));
    pFun();
    cout << endl;
    cout << "_vptr_inherit2 — 第2个函数地址：\t" << (int*)*((int*)(&dvi1) +3) + 1 <<"\t通过地址取得的值：" << *((int*)*((int*)(&dvi1) +3) + 1) << "【结束】\t" << endl;
    cout << endl;
    cout << "======指向=============：\t" << ((int*)(&dvi1) +4) << "\t通过地址取得的值：" << (int*)*((int*)(&dvi1) +4) << "\t" <<*(int*)*((int*)(&dvi1) +4)<< endl;
    cout << "推测数据成员iInherit2地址：\t" << ((int*)(&dvi1) +5) << "\t通过地址取得的值：" << *((int*)(&dvi1) +5) << endl;
    cout << "推测数据成员iDerived地址：\t" << ((int*)(&dvi1) +6) << "\t通过地址取得的值：" << *((int*)(&dvi1) +6) << endl;
    cout << "=======================：\t" << ((int*)(&dvi1) +7) << "\t通过地址取得的值：" << *((int*)(&dvi1) +7) << endl;
    //
    cout << "虚函数表_vptr_Base地址：\t" << ((int*)(&dvi1) +8) << endl;
    cout << "_vptr_Base — 第1个函数地址：\t" << (int*)*((int*)(&dvi1) +8) << "\t即析构函数地址" << endl;
    cout << "_vptr_Base — 第2个函数地址：\t" << ((int*)*((int*)(&dvi1) +8) +1) << "\t";
    pFun = (Fun)*((int*)*((int*)(&dvi1) +8) +1);
    pFun();
    cout << endl;
    cout << "_vptr_Base — 第3个函数地址：\t" << ((int*)*((int*)(&dvi1) +8) +2) << "【结束】\t" << *((int*)*((int*)(&dvi1) +8) +2);
    cout << endl;
    cout << "推测数据成员iBase地址：\t\t" << ((int*)(&dvi1) +9) << "\t通过地址取得的值：" << *((int*)(&dvi1) +9) << endl;
}
```
输出结果如下图所示：
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1es19adv8qgj20fe0ibtbe.jpg)
至此，C++对象模型介绍的差不多了，清楚了C++对象模型之后，很多疑问就能迎刃而解了。下面结合模型介绍一些典型问题。
### **如何访问成员？**
前面介绍了C++对象模型，下面介绍C++对象模型的对访问成员的影响。其实清楚了C++对象模型，就清楚了成员访问机制。下面分别针对数据成员和函数成员是如何访问到的，给出一个大致介绍。
**对象大小问题**
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1es19ae7i9pj209v0apdgx.jpg)
其中：3个类中的函数都是虚函数
- Derived继承Base
- Derived_Virtual虚继承Base

C++
```
void test_size()
{
    Base b;
    Derived d;
    Derived_Virtual dv;
    cout << "sizeof(b):\t" << sizeof(b) << endl;
    cout << "sizeof(d):\t" << sizeof(d) << endl;
    cout << "sizeof(dv):\t" << sizeof(dv) << endl;
}
```
输出如下：
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1es1ab508r9j204t01omx1.jpg)
因为Base中包含虚函数表指针，所有size为4；Derived继承Base，只是扩充基类的虚函数表，不会新增虚函数表指针，所以size也是4；Derived_Virtual虚继承Base，根据前面的模型知道，派生类有自己的虚函数表及指针，并且有分隔符（0x00000000），然后才是虚基类的虚函数表等信息，故大小为4+4+4=12。

C++
```
#pragma once
class Empty
{
public:
    Empty(void);
    ~Empty(void);
};
```
Empty p，sizeof(p)的大小是多少？事实上并不是空的，它有一个隐晦的1byte，那是被编译器安插进去的一个char。这将使得这个class的两个对象得以在内中有独一无二的地址。
**数据成员如何访问（直接取址）**
跟实际对象模型相关联，根据对象起始地址+偏移量取得。
**静态绑定与动态绑定**
程序调用函数时，将使用那个可执行代码块呢？编译器负责回答这个问题。将源代码中的函数调用解析为执行特定的函数代码块被称为函数名绑定（binding，又称联编）。在C语言中，这非常简单，因为每个函数名都对应一个不同的额函数。在C++中，由于函数重载的缘故，这项任务更复杂。编译器必须查看函数参数以及函数名才能确定使用哪个函数。然而编译器可以再编译过程中完成这种绑定，这称为静态绑定（static binding），又称为早期绑定（early binding）。
然而虚函数是这项工作变得更加困难。使用哪一个函数不是能在编译阶段时确定的，因为编译器不知道用户将选择哪种类型。所以，编译器必须能够在程序运行时选择正确的虚函数的代码，这被称为动态绑定（dynamic binding），又称为晚期绑定（late binding）。
使用虚函数是有代价的，在内存和执行速度方面是有一定成本的，包括：
- 每个对象都将增大，增大量为存储虚函数表指针的大小；
- 对于每个类，编译器都创建一个虚函数地址表；
- 对于每个函数调用，都需要执行一项额外的操作，即到虚函数表中查找地址。
虽然非虚函数比虚函数效率稍高，单不具备动态联编能力。
**函数成员如何访问（间接取址）**
跟实际对象模型相关联，普通函数（nonstatic、static）根据编译、链接的结果直接获取函数地址；如果是虚函数根据对象模型，取出对于虚函数地址，然后在虚函数表中查找函数地址。
### **多态如何实现?**
**多态的实现**
多态（Polymorphisn）在C++中是通过虚函数实现的。通过前面的模型【参见“有重写的单继承”】知道，如果类中有虚函数，编译器就会自动生成一个虚函数表，对象中包含一个指向虚函数表的指针。能够实现多态的关键在于：虚函数是允许被派生类重写的，在虚函数表中，派生类函数对覆盖（override）基类函数。除此之外，还必须通过指针或引用调用方法才行，将派生类对象赋给基类对象。
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1es19aejk7rj209v057dgd.jpg)
上面2个类，基类Base、派生类Derived中都包含下面2个方法：
- void print() const;
- virtual void print_virtual() const;
这个2个方法的区别就在于一个是普通成员函数，一个是虚函数。编写测试代码如下：

C++
```
void test_polmorphisn()
{
    Base b;
    Derived d;
    b = d;
    b.print();
    b.print_virtual();
    Base *p;
    p = &d;
    p->print();
    p->print_virtual();
}
```
根据模型推测只有p->print_virtual()才实现了动态，其他3调用都是调用基类的方法。原因如下：
-  b.print();b.print_virtual();不能实现多态是因为通过基类对象调用，而非指针或引用所以不能实现多态。
-  p->print();不能实现多态是因为，print函数没有声明为虚函数（virtual），派生类中也定义了print函数只是隐藏了基类的print函数。
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1es19aeuiaaj205u02gmx5.jpg)
**为什么析构函数设为虚函数是必要的**
析构函数应当都是虚函数，除非明确该类不做基类（不被其他类继承）。基类的析构函数声明为虚函数，这样做是为了确保释放派生对象时，按照正确的顺序调用析构函数。
从前面介绍的C++对象模型可以知道，如果析构函数不定义为虚函数，那么派生类就不会重写基类的析构函数，在有多态行为的时候，派生类的析构函数不会被调用到（有内存泄漏的风险！）。
例如，通过new一个派生类对象，赋给基类指针，然后delete基类指针。

C++
```
void test_vitual_destructor()
{
    Base *p = new Derived();
    delete p;
}
```
如果基类的析构函数不是析构函数：
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1es19af46q6j208p02e3yi.jpg)
注意，缺少了派生类的析构函数调用。把析构函数声明为虚函数，调用就正常了：
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1es19afeukgj205q02lmx5.jpg)
### **相关资料**
[1] 深度探索C++对象模型，侯捷
[2] 测试代码下载：[https://github.com/saylorzhu/CppObjectDataModelTestCode](https://github.com/saylorzhu/CppObjectDataModelTestCode)
