# C++中的out-of-line虚函数 - 电影旅行敲代码 - CSDN博客
2015年12月03日 16:37:46[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：2559
## ——————问题很多，正在修改—————
# 引子
在现实编码过程中，曾经遇到过这样的问题**“warning:’Base’ has no out-of-line method definition; its vtable will be emitted in every translation unit”**。由于对这个warning感兴趣，于是搜集了相关资料来解释这个warning相关的含义。
- C++虚表内部架构
- Vague Linkage
- out-of-line virtual method
## C++虚表内部架构
在[C++实现机制RTTI中](http://blog.csdn.net/dashuniuniu/article/details/49621833)，我们大概谈到过C++虚表的组织结构。 但是我们对C++虚表的详细实现细节并没有具体谈及，例如在继承体系下虚表的组织以及在多重继承下虚表的组织方式。
（1）**没有继承情况下的类虚表结构**
```cpp
#include <iostream>
using namespace std;
class Base
{
public:
    virtual void Add() { cout << "Base Virtual Add()!"<< "\n";  }
    virtual void Sub() { cout << "Base Virtual Sub()!" << "\n"; }
    virtual void Div() { cout << "Base Virtual Div()!" << "\n"; }
};
int main()
{
    Base* b = new Base();
    b->Add();
    b->Sub();
    b->Div();
    return 0;
}
```
由于虚函数调用时的行为由指针或者引用所关联的对象所决定，当然我们已经知道虚表指针存放在对象头4个字节，对象b的值**“0x00e8ac38”**，调出内存监视器，查看该内存的情况，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20151203170619867)
![这里写图片描述](https://img-blog.csdn.net/20151203170723184)
对象b只存放了虚表的指针**“0x00a3cc74”**，后面的**“0xfdfdfdfd”**为Visual Studio在Debug模式下，堆内存上的守护字节。我们跳转到**“0x00a3cc74”**查看该内存到底存放了什么，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20151203171111813)
这三个字存放的数据就是Base类三个虚函数所存放的虚函数地址，我们验证下，我查看调用”b->Add()”时，跳转地址为**“0x00a31483”**，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20151203171254906)
和虚表所存放的第一个slot的数据进行比对，是相同的。画出虚表示意图如下所示：
![这里写图片描述](https://img-blog.csdn.net/20151203171751014)
注意，这里虚表结构和[C++实现机制RTTI中](http://blog.csdn.net/dashuniuniu/article/details/49621833)中的略有差异，那里type_info信息存放在虚表头，这里存放在虚表尾，由于虚表实现是编译器相关，只要理解用于RTTI的type_info和虚表相关即可。
（2）**存在继承时的虚表结构**
```cpp
#include <iostream>
using namespace std;
class Base
{
public:
    virtual void Add() { cout << "Base Virtual Add()!"<< "\n";  }
    virtual void Sub() { cout << "Base Virtual Sub()!" << "\n"; }
    virtual void Div() { cout << "Base Virtual Div()!" << "\n"; }
};
class Derive : public Base
{
public:
    // 定义Drived类的Sub函数，与父类Base的Sub不同
    virtual void Sub() { cout << "Derive Virtual Sub()!" << "\n"; }
};
int main()
{
    Base* b = new Base();
    b->Add();
    Base* d = new Derive();
    d->Add();
    return 0;
}
```
Base虚表信息如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20151203184422191)
Derived虚表信息如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20151203184458754)
从这两幅图中可以看到，两张图中虚表唯一的不同是，虚表第二项不一样。Derive自定义了Sub()函数，所以理应相应的虚表应该指向Derive新定义的函数位置，而Derive继承了（没有覆盖Add()和Div()函数）Add()和Div()函数，所以第一项和第三项和Base类虚表的第一项和第三项相同。
![这里写图片描述](https://img-blog.csdn.net/20151203185153572)
也就是说子类重写了相应的虚函数，那么虚表中相应位置的地址会指向新的函数，没有重写那么相应位置的地址和父类相同。
（3）**在多重继承下**
代码如下，Derive继承自Base1和Base2，也就是说Derive从两个父类继承了两个虚表，现在的问题是，两个虚表会不会合并到一起呢？如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20151203203946643)
这种形式的话，只有从第一个父类继承的虚表的下标和父类虚表的下标是相同的，后面的虚表都要移动一定的偏移量，这样做显然不太漂亮。所以现在Visual Studio不是通过这样的方式，而是将从父类继承的多个虚表分开，以每个父类为一个单位，如下图所示。
![这里写图片描述](https://img-blog.csdn.net/20151203204825232)
如果子类覆盖了相应父类的虚函数，则会在相应父类的内存区域头部虚表指针所对应的虚表上覆盖掉对应的虚函数地址。
```cpp
#include <iostream>
using namespace std;
class Base1
{
public:
    int m_base1;
    Base1(int para):m_base1(para){}
    virtual void Add() { cout << "Base1 Virtual Add()!"<< "\n";  }
    virtual void Sub() { cout << "Base1 Virtual Sub()!" << "\n"; }
    virtual void Div() { cout << "Base1 Virtual Div()!" << "\n"; }
};
class Base2
{
public:
    int m_base2;
    Base2(int para) : m_base2(para){}
    virtual void Mul() { cout << "Base2 Virtual Mul()!" << "\n"; }
    virtual void INC() { cout << "Base2 Virtual INC()!" << "\n"; }
    virtual void DEC() { cout << "Base2 Virtual DEC()!" << "\n"; }
};
class Derive : public Base1, public Base2
{
public:
    int m_derive;
    Derive(int b1, int b2, int d) : Base1(b1), Base2(b2), m_derive(d){}
    virtual void Sub() { cout << "Derive Virtual Sub()!" << "\n"; }
    virtual void INC() { cout << "Derive Virtual INC()!" << "\n"; }
};
int main()
{
    Derive* d = new Derive(1, 11, 22);
    // 此时指针指向的位置不是Derive的开头位置，而是Derive对象中子区域Base2的头部
    Base2* b2 = d;
    // 此时b2只能调用Base2的虚函数
    b2->INC();
    Base1* b1 = d;
    return 0;
}
```
如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20151203210556878)
类继承时的内存区域布局时非常重要的，特别是在多继承时很重要的，虚析构函数在虚表中的存放还不是很明确。后面会继续分析。
## Vague Linkage
在C++中，有些创建过程需要占用.o文件的空间，例如函数的定义需要占用.o文件的空间。但是函数能够比较明确地创建到指定的.o文件中，有些创建过程却并没有明确的指定创建到那个编译单元中。我们称这些创建过程需要”Vague Linkage”，及模糊链接。通常它们会在任何需要的地方创建，所以这样创建的信息有可能会有冗余。
- inline函数（Inline Functions）
- 虚表（VTables）
- 类型信息（type_info objects）
- 模板实例化（Template Instantiations）
（1）**inline函数**
inline函数通常会定义在头文件中，以便能够被不同的编译单元包含进来。但是inline只是一个建议，编译器不一定会真的执行inline操作，并且有时候真的会需要一份inline函数的拷贝，比如说获取inline函数的地址或者inline操作失败。在这种情况下，通常我们会将inline函数的定义散播到所有需要用到该函数的编译单元中。
另外，我们通常会将附带虚表的inline虚函数（**虚函数大部分情况下不会为inline函数**）散播到目标文件中，因为虚函数通常需要真正地定义出来。
（2）**虚表**
对于C++虚函数机制，大部分编译器都是使用查找表（lookup table）实现的，也就是虚表。虚表保存着指向虚函数的指针，另外每个含有虚函数的类对象都有一个指向虚表的指针（虚表在多重继承下，有可能有多个）。如果class声明了一个非inline，非纯虚的虚函数，那么这些虚函数中的第一个out-of-line方法就被选为**关键方法（key method）**，那么虚表只会散播到（即定义到）这个关键方法所定义的编译单元中。
其实关于关键方法，还有一个有趣的例子，有时候大家会遇到“未定义的外部符号”这样的链接错误，这样的错误是由于你使用了声明但是没有定义的外部符号导致的。虚表其实在一定程度上也可以称为全局变量，只是这个全局变量是隐式地被C++语言机制实现的。虚表只会生成在第一个out-of-line虚函数所在编译单元中，如果没有定义out-of-line虚函数，那么所有include该头文件的编译单元中生成虚表。
```
// Base.h
class Base{
public:
    // 第一函数print为关键方法，虚表只会散播到（定义在）print所定义在的编译单元中
    // 如果print也定义在Base.h，那么所有包含Base.h的所有.cpp都会有一份vtable的拷贝
    // 通过链接器来消除冗余数据
    virtual int print();
    virtual int add(int lhs, int rhs) { return lhs + rhs; }
};
// A.cpp
#include "Base.h"
// vtable会定义在A.cpp编译单元中
int Base::print() { cout << "print" << endl;}
// main.cpp
#include "Base.h"
int main() {return 0;}
```
（3）**type_info对象**
为了实现”dynamic_cast”，”type_id”, 异常处理，C++要求类型信息能够完整地写出来（即存储，以便运行时能够获取）。对于多态类（含有虚函数）来说，”type_info”结构体随着虚表一起出现，虚表中会有一个slot来存放type_info结构体的指针，这样才能在运行时，在执行dynamic_cast<>的时候获得对象具体的类型信息。
对于其他类型，我们只会在需要的时候实现其type_info结构体。比如，当你使用”typeid”来获取表达式的类型信息时，或者抛出对象时和捕获对象信息时。
（4）**模板实例化**
最常见的就是我们又可能在多个编译单元中，同时实例化同一个类型的模板。当然连链接器会做冗余处理，或者使用C++11的外部模板。
## ouf-of-line virtual method
前面我们已经知道虚函数满足vague linkage的条件，有可能需要链接器去消除冗余。
**如果一个类中所有的虚函数都是inline的，那么编译器就无法知道该挑选哪一个编译单元来存放虚表的唯一的一份拷贝，相对应地，每一个需要虚表（例如调用虚函数）的目标文件中都会有一份虚表拷贝。在很多平台上，链接器能够统一这些重复的拷贝，要么丢弃重复的定义或者将所有虚表引用指向同一份拷贝，所以只会产生一个warning**。
**相对应的std::type_info也会使用这种形式，即vague linkage**，从字面意思上看就是说type_info并不是紧紧地绑定在每个编译单元中，而是以一个弱链接的形式出现。所以接下来的任务就交给链接器了，确保在最后的可执行文件中只有一份type_info的结构体对象。
> 
**these std::type_info objects have what is called vague linkage** because they are not tightly bound to any one particular translation unit (object file). 
The compiler has to emit them in any translation unit that requires their presence, and then rely on the linking and loading process to make sure that only one of them is active in the final executable. 
With static linking all of these symbols are resolved at link time, but with dynamic linking, further resolution occurs at load time. –[[GCC Frequently Asked Questions]](https://gcc.gnu.org/faq.html)
上面的链接是GCC关于这方面信息的解释，下面是LLVM在其编码规范中给出的关于Out-of-line虚函数的解释。
> 
**If a class is defined in a header file and has a vtable (either it has virtual methods or it derives from classes with virtual methods), it must always have at least one out-of-line virtual method in the class. Without this, the compiler will copy the vtable and RTTI into every .o file that #includes the header, bloating .o file sizes and increasing link times**. –[[LLVM Coding Standards]](http://llvm.org/docs/CodingStandards.html#provide-a-virtual-method-anchor-for-classes-in-headers)
**“out-of-line”**虚函数是指类中第一个虚函数的实现的能够让编译器选择一个特定的编译单元，来实现这些虚函数或者实现类的具体细节（例如类型信息），并在这个编译单元中存放一份共享的虚表。**但是如果有多个”out-of-line”虚函数分别定义在不同的.cpp文件中，那么编译器就会将虚表以及类型信息，生成在类中声明最靠前的”out-of-line”虚函数所在的TranslationUnit中。**
我以前看**LLVM**源码的时候，看到过一条有趣的注释信息：
![这里写图片描述](https://img-blog.csdn.net/20160518110406358)
如下代码所示：
```cpp
//===--------------------test.h---------------------===//
class Base
{
    public:
    // virtual函数全部是默认inline
    virtual int print() { return 0;}
    virtual int Add() { return 1;}
};
//===-----------------------------------------------===//
//===-------------------test.cpp--------------------===//
#include "test.h"
// test.cpp需要用到虚表，所以虚表应该在test.cpp中生成一份儿
int main()
{   
    Base* b = new Base();
    b->Add();
    delete b;
    return 0;
}
//===-----------------------------------------------===//
//===--------------------foo.cpp--------------------===//
#include "test.h"
// foo.cpp 也用到了虚表所以在编译的时候，在foo.cpp中也应该产生一份儿
void func()
{
    Base* b = new Base();
    b->print();
    delete b;
}
//===-----------------------------------------------===//
```
我们编译一下，看一下编译结果是否如此：
```
$g++ -c test.cpp foo.cpp
$objdump -d foo.o
// 得到下面结果，说明在foo.o中生成了虚函数定义
Disassembly of section .text$_ZN4Base5printEv:
00000000 <__ZN4Base5printEv>:
   0:   55                      push   %ebp
   1:   89 e5                   mov    %esp,%ebp
   3:   83 ec 04                sub    $0x4,%esp
   6:   89 4d fc                mov    %ecx,-0x4(%ebp)
   9:   b8 00 00 00 00          mov    $0x0,%eax
   e:   c9                      leave  
   f:   c3                      ret    
Disassembly of section .text$_ZN4Base3AddEv:
00000000 <__ZN4Base3AddEv>:
   0:   55                      push   %ebp
   1:   89 e5                   mov    %esp,%ebp
   3:   83 ec 04                sub    $0x4,%esp
   6:   89 4d fc                mov    %ecx,-0x4(%ebp)
   9:   b8 01 00 00 00          mov    $0x1,%eax
   e:   c9                      leave  
   f:   c3                      ret  
$objdump -d test.o
// 得到下面的结果
Disassembly of section .text$_ZN4Base5printEv:
00000000 <__ZN4Base5printEv>:
   0:   55                      push   %ebp
   1:   89 e5                   mov    %esp,%ebp
   3:   83 ec 04                sub    $0x4,%esp
   6:   89 4d fc                mov    %ecx,-0x4(%ebp)
   9:   b8 00 00 00 00          mov    $0x0,%eax
   e:   c9                      leave  
   f:   c3                      ret    
Disassembly of section .text$_ZN4Base3AddEv:
00000000 <__ZN4Base3AddEv>:
   0:   55                      push   %ebp
   1:   89 e5                   mov    %esp,%ebp
   3:   83 ec 04                sub    $0x4,%esp
   6:   89 4d fc                mov    %ecx,-0x4(%ebp)
   9:   b8 01 00 00 00          mov    $0x1,%eax
   e:   c9                      leave  
   f:   c3                      ret
```
我们从上面的结果中看到，确实在test.o和foo.o中都产生了虚函数print()和add()的定义，如果我们使用”readelf -s test.o”查看更详细的信息的话，会发现虚表和type_info在test.o和foo.o也都存在一份拷贝。
```
Num:    Value  Size Type    Bind   Vis      Ndx Name
     0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 00000000     0 FILE    LOCAL  DEFAULT  ABS test.cpp
     2: 00000000     0 SECTION LOCAL  DEFAULT    7 
     3: 00000000     0 SECTION LOCAL  DEFAULT    9 
     4: 00000000     0 SECTION LOCAL  DEFAULT   10 
     5: 00000000     0 SECTION LOCAL  DEFAULT   11 
     6: 00000000     0 SECTION LOCAL  DEFAULT   12 
     7: 00000000     0 SECTION LOCAL  DEFAULT   13 
     8: 00000000     0 SECTION LOCAL  DEFAULT   15 
     9: 00000000     0 SECTION LOCAL  DEFAULT   17 
    10: 00000000     0 SECTION LOCAL  DEFAULT   18 
    11: 00000000     0 SECTION LOCAL  DEFAULT   21 
    12: 00000000     0 SECTION LOCAL  DEFAULT   22 
    13: 00000000     0 NOTYPE  LOCAL  DEFAULT    3 _ZN4BaseC5Ev
    14: 00000000     0 SECTION LOCAL  DEFAULT   20 
    15: 00000000     0 SECTION LOCAL  DEFAULT    1 
    16: 00000000     0 SECTION LOCAL  DEFAULT    2 
    17: 00000000     0 SECTION LOCAL  DEFAULT    3 
    18: 00000000     0 SECTION LOCAL  DEFAULT    4 
    19: 00000000     0 SECTION LOCAL  DEFAULT    5 
    20: 00000000     0 SECTION LOCAL  DEFAULT    6 
    21: 00000000    10 FUNC    WEAK   DEFAULT   11 _ZN4Base5printEv
    22: 00000000    10 FUNC    WEAK   DEFAULT   12 _ZN4Base3AddEv
    23: 00000000    14 FUNC    WEAK   DEFAULT   13 _ZN4BaseC2Ev
    24: 00000000    16 OBJECT  WEAK   DEFAULT   15 _ZTV4Base
    25: 00000000    14 FUNC    WEAK   DEFAULT   13 _ZN4BaseC1Ev
    26: 00000000    84 FUNC    GLOBAL DEFAULT    7 main
    27: 00000000     0 NOTYPE  GLOBAL DEFAULT  UND _Znwj
    28: 00000000     0 NOTYPE  GLOBAL DEFAULT  UND _ZdlPv
    29: 00000000     8 OBJECT  WEAK   DEFAULT   18 _ZTI4Base
    30: 00000000     6 OBJECT  WEAK   DEFAULT   17 _ZTS4Base
    31: 00000000     0 NOTYPE  GLOBAL DEFAULT  UND _ZTVN10__cxxabiv117__clas
```
我们可以看到在test.o中生成了类Base的虚表和type_info结构体，**_ZTV表示虚表**，**_ZTI表示type_info结构**， **_ZTS表示type name**，注意在gcc的设计中，type_info存放在虚表的第一个slot（Visual Studio是存放在虚表的最后一个slot中）。我们看一下foo.o的相关信息，如下：
```
Num:    Value  Size Type    Bind   Vis      Ndx Name
     0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 00000000     0 FILE    LOCAL  DEFAULT  ABS foo.cpp
     2: 00000000     0 SECTION LOCAL  DEFAULT    7 
     3: 00000000     0 SECTION LOCAL  DEFAULT    9 
     4: 00000000     0 SECTION LOCAL  DEFAULT   10 
     5: 00000000     0 SECTION LOCAL  DEFAULT   11 
     6: 00000000     0 SECTION LOCAL  DEFAULT   12 
     7: 00000000     0 SECTION LOCAL  DEFAULT   13 
     8: 00000000     0 SECTION LOCAL  DEFAULT   15 
     9: 00000000     0 SECTION LOCAL  DEFAULT   17 
    10: 00000000     0 SECTION LOCAL  DEFAULT   18 
    11: 00000000     0 SECTION LOCAL  DEFAULT   21 
    12: 00000000     0 SECTION LOCAL  DEFAULT   22 
    13: 00000000     0 NOTYPE  LOCAL  DEFAULT    3 _ZN4BaseC5Ev
    14: 00000000     0 SECTION LOCAL  DEFAULT   20 
    15: 00000000     0 SECTION LOCAL  DEFAULT    1 
    16: 00000000     0 SECTION LOCAL  DEFAULT    2 
    17: 00000000     0 SECTION LOCAL  DEFAULT    3 
    18: 00000000     0 SECTION LOCAL  DEFAULT    4 
    19: 00000000     0 SECTION LOCAL  DEFAULT    5 
    20: 00000000     0 SECTION LOCAL  DEFAULT    6 
    21: 00000000    10 FUNC    WEAK   DEFAULT   11 _ZN4Base5printEv
    22: 00000000    10 FUNC    WEAK   DEFAULT   12 _ZN4Base3AddEv
    23: 00000000    14 FUNC    WEAK   DEFAULT   13 _ZN4BaseC2Ev
    24: 00000000    16 OBJECT  WEAK   DEFAULT   15 _ZTV4Base
    25: 00000000    14 FUNC    WEAK   DEFAULT   13 _ZN4BaseC1Ev
    26: 00000000    70 FUNC    GLOBAL DEFAULT    7 _Z4funcv
    27: 00000000     0 NOTYPE  GLOBAL DEFAULT  UND _Znwj
    28: 00000000     0 NOTYPE  GLOBAL DEFAULT  UND _ZdlPv
    29: 00000000     8 OBJECT  WEAK   DEFAULT   18 _ZTI4Base
    30: 00000000     6 OBJECT  WEAK   DEFAULT   17 _ZTS4Base
    31: 00000000     0 NOTYPE  GLOBAL DEFAULT  UND _ZTVN10__cxxabiv117__clas
```
可以发现在foo.o中也生成了虚表和type_info信息，也就是说如果inline虚函数都没有设置成out-of-line的话，那么编译器会向每个需要用到虚表结构的目标文件中散播虚表，虚函数和type_info定义。**直到链接的时候，链接器进行冗余消除操作**。由于链接器需要消除冗余的type_info和vtable，所以就要求虚表和type_info的符号必须是弱符号（weak symbols），GCC好像永远会将RTTI信息设置为弱符号，即使虚函数中有关键方法（key method）。
> 
对于目标文件中的符号名，可以使用**c++filt**命令来得到符号名所表示的真正的name，例如： 
**$ c++filt *ZNK3MapI10StringName3RefI8GDScriptE10ComparatorIS0_E16DefaultAllocatorE3hasERKS0***
但是如果派生类没有覆盖掉任何父类的虚函数的话，完全可以完成虚函数调用时的静态决议，则不需要对象的头4个字节的虚表指针，其实也就不需要虚表了。
相关信息请见： 
[LLVM：multiple typeinfo name](https://llvm.org/bugs/show_bug.cgi?id=10178)
[GCC Frequently Asked Questions](GCC%20Frequently%20Asked%20Questions)
[LLVM:CodingStandards](http://llvm.org/docs/CodingStandards.html#provide-a-virtual-method-anchor-for-classes-in-headers)
