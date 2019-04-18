# #ifndef/#define/#endif使用和交叉引用问题详解 - L_Andy的专栏 - CSDN博客

2015年10月26日 11:54:49[卡哥](https://me.csdn.net/L_Andy)阅读数：1352


想必很多人都看过“头文件中的 #ifndef/#define/#endif 防止该头文件被重复引用”。但是是否能理解“被重复引用”是什么意思？是不能在不同的两个文件中使用include来包含这个头文件吗？如果头文件被重复引用了，会产生什么后果？是不是所有的头文件中都要加入#ifndef/#define/#endif 这些代码？

    其实“被重复引用”是指一个头文件在同一个cpp文件中被include了多次，这种错误常常是由于include嵌套造成的。比如：存在a.h文件#include "c.h"而此时b.cpp文件导入了#include "a.h" 和#include "c.h"此时就会造成c.h重复引用。

头文件被重复引用引起的后果：

有些头文件重复引用只是增加了编译工作的工作量，不会引起太大的问题，仅仅是编译效率低一些，但是对于大工程而言编译效率低下那将是一件多么痛苦的事情。
有些头文件重复包含，会引起错误，比如在头文件中定义了全局变量(虽然这种方式不被推荐，但确实是C规范允许的)这种会引起重复定义。

    是不是所有的头文件中都要加入#ifndef/#define/#endif 这些代码？

    答案：不是一定要加，但是不管怎样，用ifnde xxx #define xxx#endif或者其他方式避免头文件重复包含，只有好处没有坏处。个人觉得培养一个好的编程习惯是学习编程的一个重要分支。

    下面给一个#ifndef/#define/#endif的格式：

#ifndef A_H意思是"if not define a.h"  如果不存在a.h

    接着的语句应该#define A_H  就引入a.h

    最后一句应该写#endif   否则不需要引入

--------------------------------------------------------------------------------------------------
#ifndef GRAPHICS_H // 防止graphics.h被重复引用 
#define GRAPHICS_H 

#include <math.h> // 引用标准库的头文件 
… 
#include “header.h” // 引用非标准库的头文件 
… 
void Function1(…); // 全局函数声明 
… 
class Box // 类结构声明 
{ 
… 
}; 
#endif
--------------------------------------------------------------------------------------------------


~~注：类的交叉引用本身就是一种不好的设计，BUT……如果我们非要这么用……~~

1、错误的方法

main.cpp

[cpp] [view
 plain](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[copy](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[print](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[?](http://blog.csdn.net/jmy5945hh/article/details/22911615#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/274312/fork)

- #include <iostream>
- 
- #include "class_a.h"
- #include "class_b.h"
- 
- int main()  
- {  
-     A aa;  
-     aa.InvokeB();  
- 
-     B bb;  
-     bb.InvokeA();  
- }  


class_a.h

[cpp] [view
 plain](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[copy](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[print](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[?](http://blog.csdn.net/jmy5945hh/article/details/22911615#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/274312/fork)

- #ifndef CLASS_A_H
- #define CLASS_A_H
- 
- #include "class_b.h"
- 
- class A  
- {  
- public:  
- void InvokeB()  
-     {  
-         B *b;  
-         b->Print();  
-     }  
- 
- void Print()  
-     {  
-         std::cout << "This is class A\n";  
-     }  
- };  
- 
- #endif

class_b.h

[cpp] [view
 plain](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[copy](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[print](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[?](http://blog.csdn.net/jmy5945hh/article/details/22911615#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/274312/fork)

- #ifndef CLASS_B_H
- #define CLASS_B_H
- 
- #include "class_a.h"
- 
- class B  
- {  
- public:  
- void InvokeA()  
-     {  
-         A *a;  
-         a->Print();  
-     }  
- 
- void Print()  
-     {  
-         std::cout << "This is class B\n";  
-     }  
- };  
- 
- #endif

此处我们用#ifndef...#endif保护机制，导致编译器（VS2012）认为在class_b.h中，A仍然是未声明的标识符。

如果去掉这个保护机制，又会引起“重定义”或者“包含文件太多，深度=1024”等问题。

必须考虑其他的实现方法。

2、一种正确的实现

main.cpp

[cpp] [view
 plain](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[copy](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[print](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[?](http://blog.csdn.net/jmy5945hh/article/details/22911615#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/274312/fork)

- #include <iostream>
- 
- #include "class_a.h"
- #include "class_b.h"
- 
- int main()  
- {  
-     A aa;  
-     aa.b->Print();  
- 
-     B bb;  
-     bb.a->Print();  
- }  


class_a.h

[cpp] [view
 plain](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[copy](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[print](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[?](http://blog.csdn.net/jmy5945hh/article/details/22911615#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/274312/fork)

- #ifndef CLASS_A_H
- #define CLASS_A_H
- 
- class B;  
- class A  
- {  
- public:  
- void Print()  
-     {  
-         std::cout << "This is class A\n";  
-     }  
- 
-     B *b;  
- };  
- 
- #endif


class_b.h

[cpp] [view
 plain](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[copy](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[print](http://blog.csdn.net/jmy5945hh/article/details/22911615#)[?](http://blog.csdn.net/jmy5945hh/article/details/22911615#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/274312/fork)

- #ifndef CLASS_B_H
- #define CLASS_B_H
- 
- class A;  
- class B  
- {  
- public:  
- void Print()  
-     {  
-         std::cout << "This is class B\n";  
-     }  
- 
-     A *a;  
- };  
- 
- #endif

输出：
![](https://img-blog.csdn.net/20140404095640171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvam15NTk0NWho/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在这种方法中使用了指针。

好处在于，程序可以在不知道类的具体实现是什么的情况下创建相关的指针。

前提条件是必须先声明类，并且指针在public域。此时不再有头文件交叉包含的困扰~

两个类在另一个类中都是组合的关系,也就是表示has a的关系,就是说A has a B,B has a A....这样在逻辑上是说不过去的,至少在我看过的代码中从来没有出现过这样的使用方法.
另外,在类中使用到另一个类的时候,如果用到的是指针,那么只需要做一个前置声明就可以了,因为指针的大小是固定的,不需要知道具体类体;但是如果使用的是类的对象,那么就需要include相应的类的头文件了.




