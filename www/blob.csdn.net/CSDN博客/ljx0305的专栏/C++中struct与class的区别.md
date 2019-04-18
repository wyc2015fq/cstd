# C++中struct与class的区别 - ljx0305的专栏 - CSDN博客
2008年06月01日 20:53:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：621标签：[struct																[class																[c++																[语言																[c																[oo](https://so.csdn.net/so/search/s.do?q=oo&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
[C++中struct与class的区别](http://blog.csdn.net/fisher_jiang/archive/2008/05/23/2471119.aspx)
从语法上，在C++中（只讨论C++中）。class和struct做类型定义时只有两点区别： 
（一）默认继承权限。如果不明确指定，来自class的继承按照private继承处理，来自struct的继承按照public继承处理； 
（二）成员的默认访问权限。class的成员默认是private权限，struct默认是public权限。 
除了这两点，class和struct基本就是一个东西。语法上没有任何其它区别。 
不能因为学过C就总觉得连C++中struct和class都区别很大，下面列举的说明可能比较无聊，因为struct和class本来就是基本一样的东西，无需多说。但这些说明可能有助于澄清一些常见的关于**struct和class的**错误认识： 
（1）都可以有成员函数；包括各类构造函数，析构函数，重载的运算符，友元类，友元结构，友元函数，虚函数，纯虚函数，静态函数； 
（2）都可以有一大堆public/private/protected修饰符在里边； 
（3）虽然这种风格不再被提倡，但语法上二者都可以使用大括号的方式初始化：A a = {1, 2, 3};不管A是个struct还是个class，前提是这个类/结构足够简单，比如所有的成员都是public的，所有的成员都是简单类型，没有显式声明的构造函数。 
（4）都可以进行复杂的继承甚至多重继承，一个struct可以继承自一个class，反之亦可；一个struct可以同时继承5个class和5个struct，虽然这样做不太好。 
（5）如果说class的设计需要注意OO的原则和风格，那么没任何理由说设计struct就不需要注意。 
（6）再次说明，以上所有说法都是指在C++语言中，至于在C里的情况，C里是根本没有“class”，而C的struct从根本上也只是个包装数据的语法机制。 
--------------------------------------------------------------- 
最后，作为语言的两个关键字，除去定义类型时有上述区别之外，另外还有一点点：“class”这个关键字还用于定义模板参数，就像“typename”。但关键字“struct”不用于定义模板参数。
