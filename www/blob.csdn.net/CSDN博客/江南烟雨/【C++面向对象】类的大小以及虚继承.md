# 【C++面向对象】类的大小以及虚继承 - 江南烟雨 - CSDN博客
2012年05月18日 21:33:42[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：2384标签：[c++																[编译器																[alignment																[class																[语言																[优化](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=alignment&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C/C++](https://blog.csdn.net/xiajun07061225/article/category/723127)
先看下面一段代码：
```cpp
#include <iostream>
using namespace std;
class X{};
class Y:public virtual X{};
class Z:public virtual X{};
class A:public Y,public Z{};
int main()
{
	cout << "sizeof(X) : " << sizeof(X) << endl;
	cout << "sizeof(Y) : " << sizeof(Y) << endl;
	cout << "sizeof(Z) : " << sizeof(Z) << endl;
	cout << "sizeof(A) : " << sizeof(A) << endl;
}
```
运行于VS2008上的结果是：
![](https://img-my.csdn.net/uploads/201205/18/1337346887_6590.jpg)
为什么会出现这样的结果？下面来进行分析。
先看看上述程序中类的继承关系：
![](https://img-my.csdn.net/uploads/201205/18/1337347035_9954.jpg)
我们先看类X。X类明明是空的，为什么sizeof(X) = 1呢？
原因是因为事实上X并不是空的，它有一个隐晦的1字节，那是编译器安插进去的一个char。这会是的class X的两个objects得以在内存中配置独一无二的地址。
class Y和Z的结果同样令人不解。
**事实上Y和Z的大小受到三个因素的影响：**
（1）语言本身所造成的额外负担。当语言支持虚基类（virtual base classes）时，就会造成一些额外负担。在子类中，这个额外负担反映在某种形式的指针身上，或者指向虚基类子对象，或者指向一个相关的表格；表格中存放的不是虚基类子对象的地址，就是其偏移量。
（2）编译器对于特殊情况所做的优化处理。虚基类子对象的1字节大小也出现在Y和Z中。传统上放在子类对象的尾端。
现在的编译器一般会对空虚基类提供特殊支持（比如VS2008）。在这个策略下，一个空虚基类被视为子类对象最开头的一部分，也就是说它并没有花费任何的额外空间。这就节省了前述的1字节。（因为既然有了成员，就不需要原本为了空类而安插的1个char）。
（3）Alignment的限制。就是字节对齐，具体可参考：[字节对齐详解](http://www.ibm.com/developerworks/library/pa-dalign/)。在这里没有增加额外的空间。
因此，Y和Z的大小都是4字节。
下面我们讨论A的大小。
这里需要注意的是：**一个虚基类子对象只会在继承类中存在一份实体，不管它在继承体系中出现了多少次**！！！
**A类大小由以下几点决定：**
（1）被共享的X类实体，大小为1字节。
（2）Y的大小，需要减去因为虚基类X而配置的大小。4字节。
（3）A自己的大小：0字节。
（4）A字节对齐作用产生的额外空间。
结果是12字节。
注意：C++标准并不强制规定如“基类子对象的排列次序”或不同存取层级的数据成员的排列次序这种琐碎细节。也不规定虚函数和虚基类的实现细节。
