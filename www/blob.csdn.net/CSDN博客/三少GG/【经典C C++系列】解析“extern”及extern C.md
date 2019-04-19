# 【经典C/C++系列】解析“extern”及extern "C" - 三少GG - CSDN博客
2011年10月25日 14:18:47[三少GG](https://me.csdn.net/scut1135)阅读数：804标签：[c																[编译器																[语言																[module																[汇编																[编程](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
### [解析“extern”](http://blog.csdn.net/keensword/article/details/401114)
分类： [经验总结(个人原创)](http://blog.csdn.net/keensword/article/category/127611) 2005-06-23 13:17 30800人阅读[评论](http://blog.csdn.net/keensword/article/details/401114#comments)(85)收藏[举报](http://blog.csdn.net/keensword/article/details/401114#report)
**解析“extern”**
**1、声明外部变量**
现代编译器一般采用按文件编译的方式，因此在编译时，各个文件中定义的全局变量是
互相透明的，也就是说，在编译时，全局变量的可见域限制在文件内部。下面举一个简单的例子。创建一个工程，里面含有A.cpp和B.cpp两个简单的C++源文件：
//A.cpp
int i;
void main()
{
}


//B.cpp
int i;
这两个文件极为简单，在A.cpp中我们定义了一个全局变量i，在B中我们也定义了一个全局变量i。
我们对A和B分别编译，都可以正常通过编译，但是进行链接的时候，却出现了错误，错误提示如下：
Linking...
B.obj : error LNK2005: "int i" (?i@@3HA) already defined in A.obj
Debug/A.exe : fatal error LNK1169: one or more multiply defined symbols found
Error executing link.exe.
A.exe - 2 error(s), 0 warning(s)

这就是说，在编译阶段，各个文件中定义的全局变量相互是透明的，编译A时觉察不到B中也定义了i，同样，编译B时觉察不到A中也定义了i。
但是到了链接阶段，要将各个文件的内容“合为一体”，因此，如果某些文件中定义的全局变量名相同的话，在这个时候就会出现错误，也就是上面提示的重复定义的错误。
因此，各个文件中定义的全局变量名不可相同。
在链接阶段，各个文件的内容（实际是编译产生的obj文件）是被合并到一起的，因而，定义于某文件内的全局变量，在链接完成后，它的可见范围被扩大到了整个程序。
这样一来，按道理说，一个文件中定义的全局变量，可以在整个程序的任何地方被使用，举例说，如果A文件中定义了某全局变量，那么B文件中应可以该变量。修改我们的程序，加以验证：
//A.cpp
void main()
{
    i = 100; //试图使用B中定义的全局变量
}


//B.cpp
int i;

编译结果如下：
Compiling...
A.cpp
C:/Documents and Settings/wangjian/桌面/try extern/A.cpp(5) : error C2065: 'i' : undeclared identifier
Error executing cl.exe.
A.obj - 1 error(s), 0 warning(s)


编译错误。
其实出现这个错误是意料之中的，因为：文件中定义的全局变量的可见性扩展到整个程序是在链接完成之后，而在编译阶段，他们的可见性仍局限于各自的文件。
编译器的目光不够长远，编译器没有能够意识到，某个变量符号虽然不是本文件定义的，但是它可能是在其它的文件中定义的。
虽然编译器不够远见，但是我们可以给它提示，帮助它来解决上面出现的问题。这就是extern的作用了。
       extern的原理很简单，就是告诉编译器：“你现在编译的文件中，有一个标识符虽然没有在本文件中定义，但是它是在别的文件中定义的全局变量，你要放行！”
我们为上面的错误程序加上extern关键字：
//A.cpp
**extern int i;**
void main()
{
    i = 100; //试图使用B中定义的全局变量
}


//B.cpp
int i;

顺利通过编译，链接。
**2、****在C++文件中调用C方式编译的函数**
C方式编译和C++方式编译
相对于C，C++中新增了诸如重载等新特性，对于他们的编译，必然有一些重要的区别。
我们将下面的小程序分别按C和C++方式编译，来探讨两种编译方式的区别。
int i;
int func(int t)
{
         return 0;
}
void main()
{
}


以C方式编译的结果：
COMM     _i  :  DWORD
PUBLIC    _func
PUBLIC    _main

以C++方式编译的结果：
PUBLIC    ?i@@3HA                                               ; i
PUBLIC    ?func@@YAHH@Z                                         ; func
PUBLIC    _main

可见，C方式编译下，变量名和函数名之前被统一加上了一个下划线，而C++编译后的结果却复杂的多，i变成了?i@@3HA，func变成了?func@@YAHH@Z。C++中的这种看似复杂的命名规则是为C++中的函数重载，参数检查等特性服务的。
多文件程序中的函数调用
一般情况下，工程中的文件都是CPP文件（以及头文件）。如下面的程序仅包含两个文件：A.CPP和B.CPP：
//A.CPP
void func();
void main()
{
         func();
}

//B.CPP
void func()
{
}

程序的结构是这样的：在文件B.CPP中定义了一个函数void func()，main函数位于文件A.CPP，在main函数中调用了B中定义的函数func()。
要在A中调用B中定义的函数，必须要加上该函数的声明。如本例中的void func();就是对函数func()的声明。
如果没有声明的话，编译A.CPP时就会出错。因为编译器的目光只局限于被编译文件，必须通过加入函数声明来告诉编译器：“某个函数是定义在其它的文件中的，你要放行！”，这一点跟用extern来声明外部全局变量是一个道理。
需要注意的是，一般的程序都是通过包含头文件来完成函数的声明。拿本例来说，一般是创建一个头文件B.H，在头文件中加入声明语句void func();并且在A.CPP中加入包含语句：#include
 “B.H”。
在C++程序中，头文件的功能从函数声明被扩展为类的定义。
不同编译方式下的函数调用
如果在工程中，不仅有CPP文件，还有以C方式编译的C文件，函数调用就会有一些微妙之处。我们将B.CPP改作B.C：
//A.CPP
void func();
void main()
{
         func();
}

//B.C
void func()
{
}

对A.CPP和B.C分别编译，都没有问题，但是链接时出现错误。
Linking...
A.obj : error LNK2001: unresolved external symbol "void __cdecl func(void)" (?func@@YAXXZ)
Debug/A.exe : fatal error LNK1120: 1 unresolved externals
Error executing link.exe.
A.exe - 2 error(s), 0 warning(s)

原因就在于不同的编译方式产生的冲突。
对于文件A，是按照C++的方式进行编译的，其中的func()调用被编译成了
call    ?func1@@YAXXZ     

如果B文件也是按照C++方式编译的，那么B中的func函数名也会被编译器改成?func1@@YAXXZ，这样的话，就没有任何问题。
但是现在对B文件，是按照C方式编译的，B中的func函数名被改成了_func，这样一来，A中的call
 ?func1@@YAXXZ这个函数调用就没有了着落，因为在链接器看来，B文件中没有名为?func1@@YAXXZ的函数。
事实是，我们编程者知道，B文件中有A中调用的func函数的定义，只不过它是按照C方式编译的，故它的名字被改成了_func。因而，我们需要通过某种方式告诉编译器：“B中定义的函数func()经编译后命名成了_func，而不是?func1@@YAXXZ，你必须通过call
 _func来调用它，而不是call ?func1@@YAXXZ。”简单的说，就是告诉编译器，调用的func()函数是以C方式编译的，fun();语句必须被编译成call
 _func;而不是call ?func1@@YAXXZ。
我们可以通过extern关键字，来帮助编译器解决上面提到的问题。
对于本例，只需将A.CPP改成如下即可：
//A.CPP
**extern "C"**
**{**
**         void func();**
**}**
void main()
{
         func();
}


察看汇编代码，发现此时的func();语句被编译成了call _func。

**3、补充**
同2一样，仍然是C，C++混合编程的情形，考虑下面的程序：
//A.CPP
extern int i;
void main()
{
         i = 100;
}


//B.C
int i;
程序很简单：在文件B.C中定义了一个全局变量i，在A.CPP中使用了这个全局变量。
编译没有问题，链接时却出现错误：
Linking...
A.obj : error LNK2001: unresolved external symbol "int i" (?i@@3HA)
Debug/A.exe : fatal error LNK1120: 1 unresolved externals
Error executing link.exe.
A.exe - 2 error(s), 0 warning(s)

这是因为，在C方式编译下，i被重命名为_i，而在C++方式下，i会被重命名为?i@@3HA。
因而，我们只用extern int i;来声明还不够，必须告诉编译器，全局变量i是以C方式编译的，
它会被重命名为_i，而不是?i@@3HA。
我们修改A.CPP，如下：
//A.CPP
**extern "C"**
**{**
**         int i;**
**}**
void main()
{
         i = 100;
}



程序正常通过编译和链接。
我们察看一下汇编代码，发现语句i = 100;被编译成了mov  DWORD PTR _i, 100。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[C++中extern “C”含义深层探索](http://blog.tianya.cn/blogger/post_show.asp?BlogID=956193&PostID=10814458)
　　1.引言
　　C++语言的创建初衷是“a better C”，但是这并不意味着C++中类似C 语言的全局变量
　　和函数所采用的编译和链接方式与C 语言完全相同。作为一种欲与C 兼容的语言，C++保留了
　　一部分过程式语言的特点，因而它可以定义不属于任何类的全局变量和函数。但是，C++毕竟
　　是一种面向对象的程序设计语言，为了支持函数的重载，C++对全局函数的处理方式与C 有明
　　显的不同。
　　2.从标准头文件说起
　　某企业曾经给出如下的一道面试题：
　　面试题
　　为什么标准头文件都有类似以下的结构？
　　#ifndef _TEST_H
　　#define _TEST_H
　　#ifdef __cplusplus
　　extern "C" {
　　#endif
　　/*...*/
　　#ifdef __cplusplus
　　}
　　#endif
　　#endif /* _TEST_H */
　　分析
　　显然，头文件中的编译宏“#ifndef _TEST_H、#define _TEST_H、#endif” 的作用是防止
　　该头文件被重复引用。
　　那么
　　#ifdef __cplusplus
　　extern "C" {
　　#endif
　　#ifdef __cplusplus
　　}
　　#endif
　　的作用又是什么呢？
　　3.深层揭密extern "C"
　　extern "C" 包含双重含义，从字面上即可得到：首先，被它修饰的目标是“extern”的；
　　其次，被它修饰的目标是“C”的。让我们来详细解读这两重含义。
　　被extern "C"限定的函数或变量是extern 类型的；
　　extern 是C/C++语言中表明函数和全局变量作用范围（可见性）的关键字，该关键字告
　　诉编译器，其声明的函数和变量可以在本模块或其它模块中使用。记住，下列语句：
　　extern int a;
　　仅仅是一个变量的声明，其并不是在定义变量a，并未为a 分配内存空间(特别注意：实
　　际上现在一般的编译器都会对上述语句作声明处理，但链接器在链接过程中如果没有发现该
　　变量的定义，一般会在第一次遇到该变量声明的地方，自动定义)。变量a 在所有模块中作为
　　一种全局变量只能被定义一次，否则会出现连接错误。
　　通常，在模块的头文件中对本模块提供给其它模块引用的函数和全局变量以关键字
　　extern 声明。例如，如果模块B 欲引用该模块A 中定义的全局变量和函数时只需包含模块A
　　的头文件即可。这样，模块B 中调用模块A 中的函数时，在编译阶段，模块B 虽然找不到该
　　函数，但是并不会报错；它会在连接阶段中从模块A 编译生成的目标代码中找到此函数。
　　与extern 对应的关键字是static，被它修饰的全局变量和函数只能在本模块中使用。因
　　此，一个函数或变量只可能被本模块使用时，其不可能被extern “C”修饰。
　　被extern "C"修饰的变量和函数是按照C 语言方式编译和连接的；
　　未加extern “C”声明时的编译方式
　　首先看看C++中对类似C 的函数是怎样编译的。
　　作为一种面向对象的语言，C++支持函数重载，而过程式语言C 则不支持。函数被C++编
　　译后在符号库中的名字与C 语言的不同。例如，假设某个函数的原型为：
　　void foo( int x, int y );
　　该函数被C 编译器编译后在符号库中的名字为_foo，而C++编译器则会产生像
　　_foo_int_int 之类的名字（不同的编译器可能生成的名字不同，但是都采用了相同的机制，
　　生成的新名字称为“mangled name”）。
　　_foo_int_int 这样的名字包含了函数名、函数参数数量及类型信息，C++就是靠这种机制
　　来实现函数重载的。例如，在C++中，函数void foo( int x, int y )与void foo( int x, float
　　y )编译生成的符号是不相同的，后者为_foo_int_float。
　　同样地，C++中的变量除支持局部变量外，还支持类成员变量和全局变量。用户所编写程序的
　　类成员变量可能与全局变量同名，我们以"."来区分。而本质上，编译器在进行编译时，与函
　　数的处理相似，也为类中的变量取了一个独一无二的名字，这个名字与用户程序中同名的全
　　局变量名字不同。
　　未加extern "C"声明时的连接方式
　　假设在C++中，模块A 的头文件如下：
　　// 模块A 头文件 moduleA.h
　　#ifndef MODULE_A_H
　　#define MODULE_A_H
　　int foo( int x, int y );
　　#endif
　　在模块B 中引用该函数：
　　// 模块B 实现文件 moduleB.cpp
　　＃i nclude "moduleA.h"
　　foo(2,3);
　　实际上，在连接阶段，连接器会从模块A 生成的目标文件moduleA.obj 中寻找
　　_foo_int_int 这样的符号！
　　加extern "C"声明后的编译和连接方式
　　加extern "C"声明后，模块A 的头文件变为：
　　// 模块A 头文件 moduleA.h
　　#ifndef MODULE_A_H
　　#define MODULE_A_H
　　extern "C" int foo( int x, int y );
　　#endif
　　在模块B 的实现文件中仍然调用foo( 2,3 )，其结果是：
　　（1）模块A 编译生成foo 的目标代码时，没有对其名字进行特殊处理，采用了C 语言的
　　方式；
　　（2）连接器在为模块B 的目标代码寻找foo(2,3)调用时，寻找的是未经修改的符号名_foo。
　　如果在模块A 中函数声明了foo 为extern "C"类型，而模块B 中包含的是extern int
　　foo( int x, int y ) ，则模块B 找不到模块A 中的函数；反之亦然。
　　所以，可以用一句话概括extern “C”这个声明的真实目的（任何语言中的任何语法特
　　性的诞生都不是随意而为的，来源于真实世界的需求驱动。我们在思考问题时，不能只停留
　　在这个语言是怎么做的，还要问一问它为什么要这么做，动机是什么，这样我们可以更深入
　　地理解许多问题）：
　　实现C++与C 及其它语言的混合编程。
　　明白了C++中extern "C"的设立动机，我们下面来具体分析extern "C"通常的使用技巧。
　　4.extern "C"的惯用法
　　（1）在C++中引用C 语言中的函数和变量，在包含C 语言头文件（假设为cExample.h）
　　时，需进行下列处理：
　　extern "C"
　　{
　　＃i nclude "cExample.h"
　　}
　　而在C 语言的头文件中，对其外部函数只能指定为extern 类型，C 语言中不支持extern
　　"C"声明，在.c 文件中包含了extern "C"时会出现编译语法错误。
　　笔者编写的C++引用C 函数例子工程中包含的三个文件的源代码如下：
　　/* c 语言头文件：cExample.h */
　　#ifndef C_EXAMPLE_H
　　#define C_EXAMPLE_H
　　extern int add(int x,int y);
　　#endif
　　/* c 语言实现文件：cExample.c */
　　＃i nclude "cExample.h"
　　int add( int x, int y )
　　{
　　return x + y;
　　}
　　// c++实现文件，调用add：cppFile.cpp
　　extern "C"
　　{
　　＃i nclude "cExample.h"
　　}
　　int main(int argc, char* argv[])
　　{
　　add(2,3);
　　return 0;
　　}
　　如果C++调用一个C 语言编写的.DLL 时，当包括.DLL 的头文件或声明接口函数时，应加
　　extern "C" { }。
　　（2）在C 中引用C++语言中的函数和变量时，C++的头文件需添加extern "C"，但是在C
　　语言中不能直接引用声明了extern "C"的该头文件，应该仅将C 文件中将C++中定义的extern
　　"C"函数声明为extern 类型。
　　笔者编写的C 引用C++函数例子工程中包含的三个文件的源代码如下：
　　//C++头文件 cppExample.h
　　#ifndef CPP_EXAMPLE_H
　　#define CPP_EXAMPLE_H
　　extern "C" int add( int x, int y );
　　#endif
　　//C++实现文件 cppExample.cpp
　　＃i nclude "cppExample.h"
　　int add( int x, int y )
　　{
　　return x + y;
　　}
　　/* C 实现文件 cFile.c
　　/* 这样会编译出错：＃i nclude "cExample.h" */
　　extern int add( int x, int y );
　　int main( int argc, char* argv[] )
　　{
　　add( 2, 3 );
　　return 0;
　　}
　　如果深入理解了第3 节中所阐述的extern "C"在编译和连接阶段发挥的作用，就能真正
　　理解本节所阐述的从C++引用C 函数和C 引用C++函数的惯用法。对第4 节给出的示例代码，
　　需要特别留意各个细节。

