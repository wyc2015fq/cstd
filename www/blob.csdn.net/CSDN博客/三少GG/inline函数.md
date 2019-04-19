# inline函数 - 三少GG - CSDN博客
2012年02月29日 15:48:48[三少GG](https://me.csdn.net/scut1135)阅读数：720
一、inline [关键字](http://baike.baidu.com/view/390935.htm)用来定义一个类的[内联函数](http://baike.baidu.com/view/534064.htm)，引入它的主要原因是用它替代C中表达式形式的宏定义。
　　表达式形式的宏定义一例：
　　#define ExpressionName(Var1,Var2) ((Var1)+(Var2))*((Var1)-(Var2))为什么要取代这种形式呢，且听我道来：
　　1． 首先谈一下在C中使用这种形式宏定义的原因，C语言是一个效率很高的语言，这种宏定义在形式及使用上像一个函数，但它使用预处理器实现，没有了参数压栈，代码生成 等一系列的操作,因此，效率很高，这是它在C中被使用的一个主要原因。
　　2． 这种宏定义在形式上类似于一个函数，但在使用它时，仅仅只是做预处理器符号表中的简单替换，因此它不能进行参数有效性的检测，也就不能享受C++[编译器](http://baike.baidu.com/view/487018.htm)严格类型检查的好处，另外它的返回值也不能被强制转换为可转换的合适的类型，这样，它的使用就存在着一系列的隐患和局限性。
　　3． 在C++中引入了类及类的访问控制，这样，如果一个操作或者说一个表达式涉及到类的保护成员或私有成员，你就不可能使用这种宏定义来实现(因为无法将this指针放在合适的位置)。
　　4． inline 推出的目的，也正是为了取代这种表达式形式的宏定义，它消除了它的缺点，同时又很好地继承了它的优点。
### 为什么inline能很好地取代预定义呢？
　　对应于上面的1-3点，阐述如下：
　　1． inline 定义的类的内联函数，函数的代码被放入符号表中，在使用时直接进行替换，（像宏一样展开），没有了调用的开销，效率也很高。
　　2． 很明显，类的内联函数也是一个真正的函数，编译器在调用一个内联函数时，会首先检查它的参数的类型，保证调用正确。然后进行一系列的相关检查，就像对待任何一个真正的函数一样。这样就消除了它的隐患和局限性。
　　3． inline 可以作为某个[类的成员函数](http://baike.baidu.com/view/2345889.htm)，当然就可以在其中使用所在类的保护成员及私有成员。
　　在何时使用inline函数：
　　首先，你可以使用inline函数完全取代表达式形式的宏定义。
　　另外要注意，内联函数一般只会用在函数内容非常简单的时候，这是因为，内联函数的代码会在任何调用它的地方展开，如果函数太复杂，代码膨胀带来的恶果很可能会大于效率的提高带来的益处。内联函数最重要的使用地方是用于类的存取函数。
### 如何使用类的inline函数：
　　简单提一下inline 的使用吧：
　　1.在类中定义这种函数：
　　class ClassName{
　　.....
　　....
　　INT GetWidth(){return m_lPicWidth;}; // 如果在类中直接定义，不需要用inline修饰,编译器自动化为内联函数
　　.... //此说法在《C++ Primer》中提及
　　....
　　}
　　2.在类外定义前加inline关键字:
　　class Account {
　　public:
　　Account(double initial_balance) { balance = initial_balance; } //与1相同
　　double GetBalance(); //在类中声明
　　double Deposit( double Amount );
　　double Withdraw( double Amount );
　　private:
　　double balance;
　　};
　　inline double Account::GetBalance() { return balance; } //在类外定义时添加inline关键字
　　inline double Account::Deposit( double Amount ) { return ( balance += Amount ); }
　　inline double Account::Withdraw( double Amount ) { return ( balance -= Amount ); }
　此外含有一些规则需注意:
　　1、inline说明对对编译器来说只是一种建议，编译器可以选择忽略这个建议。比如，你将一个长达1000多行的函数指定为inline，编译器就会忽略这个inline，将这个函数还原成普通函数。
　　2、在调用内联函数时，**要保证内联函数的定义让编译器"看"到，也就是说内联函数的定义要在头文件中，这与通常的函数定义不一样**。
但如果你习惯将函数定义放在CPP文件中，或者想让头文件更简洁一点，可这样做:    (Grant 不建议这么用，参照MIL。直接在.h文件中定义即可)
　　//SomeInline.h中
　　#ifndef SOMEINLINE_H
　　#define SOMEINLINE_H
　　inline Type Example(void);
　　//........其他函数的声明
　　#include“SomeInlie.cpp” //源文件后缀名随编译器而定
　　#endif
　　//SomeInline.cpp中
　　#include"SomeInline.h"
　　Type Example(void)
　　{
　　//..........
　　}
　　//...............其他函数的定义
　　以上方法是通用、有效的，可放心使用，不必担心在头文件包含CPP文件会导致编译错误。
*******************************************************************************************
（一）inline函数（摘自C++ Primer的第三版） 
在函数声明或定义中函数返回类型前加上关键字inline即把min（）指定为内联。 
inline int min(int first, int secend) {/****/}; 
inline函数对编译器而言必须是可见的，以便它能够在调用点内展开该函数。与非inline函数不同的是，inline函数必须在调用该函数的每个文本文件中定义。当然，对于同一程序 的不同文件，如果inline函数出现的话，其定义必须相同。对于由两个文件compute.C和draw.C构成的程序来说，程序员不能定义这样的min()函数，它在compute.C中指一件事情， 而在draw.C中指另外一件事情。如果两个定义不相同，程序将会有未定义的行为：
**为保证不会发生这样的事情，建议把inline函数的定义放到头文件中。在每个调用该inline函数的 文件中包含该头文件。**这种方法保证对每个inline函数只有一个定义，且程序员无需复制代码，并且不可能在程序的生命期中引起无意的不匹配的事情。
（二）内联函数的编程风格(摘自高质量C++/C 编程指南) 
关键字inline 必须与函数定义体放在一起才能使函数成为内联，仅将inline 放在函数声明前面不起任何作用。 
如下风格的函数Foo 不能成为内联函数： 
inline void Foo(int x, int y); // inline 仅与函数声明放在一起 
void Foo(int x, int y) 
{ 
} 
而如下风格的函数Foo 则成为内联函数： 
void Foo(int x, int y); 
inline void Foo(int x, int y) // inline 与函数定义体放在一起 
{ 
} 
所以说，inline 是一种“用于实现的关键字”，而不是一种“用于声明的关键字”。 
一般地，用户可以阅读函数的声明，但是看不到函数的定义。尽管在大多数教科书中内 
联函数的声明、定义体前面都加了inline 关键字，但我认为inline 不应该出现在函数 
的声明中。这个细节虽然不会影响函数的功能，但是体现了高质量C++/C 程序设计风格 
的一个基本原则：声明与定义不可混为一谈，用户没有必要、也不应该知道函数是否需 
要内联。
            
