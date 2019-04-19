# C++中的volatile - 电影旅行敲代码 - CSDN博客
2015年11月09日 11:30:55[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：471标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[c++基础](https://blog.csdn.net/dashuniuniu/article/category/5682427)
# 引子
在阅读代码的过程中，相信大家都遇到过”volatile”这个关键字，有的人知道和编译器优化相关，甚至有的人根本不知道该关键字的功用
## volatile
> 
A type qualitier that you can use to declare that an object can be modified in the program by the hardware. —[[volatile]](https://msdn.microsoft.com/en-us/library/12a04hfd.aspx)
考虑下面的代码：
```
int some_int = 100;
while (some_int == 100)
{
    // your code
}
```
当这个程序编译的时候，编译器如果发现这个程序永远都不会改变”some_int”的值，有可能会优化这个代码。编译器会尝试优化while循环，将”while(some_int == 100)” 简单的替换成”while(true)”，这样程序执行起来会稍微快一点。如果编译器没有对该段代码进行优化，每次循环时都会从内存读取该变量的值（如果没有将其放入寄存器的情况下），并与100进行比较。
但是有时候这种优化并不是我们所想的，如果有人想从程序外部编译器所没有意识到的地方对some_int进行修改，那么就会出错。所以为了确保正确的行为，你必须确保编译器不会因为想要优化”while(loop)”而优化掉”some_int”变量。volatile关键字就是用于这些场合的。比如：
`volatile int some_int = 100;`
换句话说，volatile关键字告诉编译器不要扮演上帝的角色而对代码进行任意的优化。
> 
C++ Standard ($7.1.5.1/8):**volatile** is a hint to the implementation to **avoid aggressive optimization involving the object** because the value of the object might be changed by means undetectable by an implementation. —[[volatile]](http://stackoverflow.com/questions/4437527/why-do-we-use-volatile-keyword-in-c)
volatile关键就是一个类型限定符，用来声明有可能被操作系统，硬件，或者在多线程环境中修改的对象。
volatile对象就是被volatile关键字修饰的对象或者它的子对象是一个volatile对象。每次对volatile对象进行访问（读写或者成员函数调用等），对象都会产生可见的副作用（即使在优化的情况下）。在单线程的情况下，volatile对象的访问不会被重排序或者优化掉。
