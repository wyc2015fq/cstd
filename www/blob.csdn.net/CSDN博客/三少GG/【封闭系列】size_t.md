# 【封闭系列】size_t - 三少GG - CSDN博客
2011年07月12日 16:11:56[三少GG](https://me.csdn.net/scut1135)阅读数：1635
1. size_t 
size_t 类型定义在cstddef头文件中，该文件是C标准库的头文件[stddef.h](http://baike.baidu.com/view/3555591.htm)的C++版。它是一个与机器相关的unsigned类型，其大小足以保证存储内存中对象的大小。
在C++中，设计 size_t 就是为了适应多个平台的 。size_t的引入增强了程序在不同平台上的可移植性。size_t是针对系统定制的一种数据类型，一般是整形，因为C/C++标准只定义一最低的位数， 而不是必需的固定位数。而且在内存里，对数的高位对齐存储还是低位对齐存储各系统都不一样。为了提高代码的可移植性，就有必要定议这样的数据类型。一般这种类型都会定义到它具体占几位内存等。当然，有些是编译器或系统已经给定义好的。经测试发现，在32位系统中size_t是4字节的，而在64位系统中，size_t是8字节的，这样利用该类型可以增强程序的可移植性。
2.  
[http://support.microsoft.com/kb/156810](http://support.microsoft.com/kb/156810)
Representation of positive infinity for float " <<
        numeric_limits<float>::infinity() << endl;
numeric_limits 类提供有关给定类型的信息。例如对于您可以确定给定的类型是否已签名或完全或方式，它可能表示无穷大。
标准库的 char、 bool、 有符号的字符、 无符号的字符，较短的无符号短，int、 无符号的整数、 长的无符号长，浮点，双，和长双实例化 numeric_limits。 此类可能还能实例化的用户定义的类型。
下面的代码示例阐释如何使用 **Visual c + +** 中的从 **numeric_limits 类**以下成员：
3. 用法：**static_cast < type-id > ( expression ) **
　　该运算符**把expression转换为type-id类型**，但没有运行时类型检查来保证转换的安全性。它主要有如下几种用法： 
　　①用于类层次结构中基类（父类）和[派生类](http://baike.baidu.com/view/535532.htm)（子类）之间指针或引用的转换。 
　　进行上行转换（把派生类的指针或引用转换成基类表示）是安全的； 
　　进行下行转换（把基类指针或引用转换成派生类表示）时，由于没有动态类型检查，所以是不安全的。 
　　②用于基本数据类型之间的转换，如把int转换成char，把int转换成enum。这种转换的安全性也要开发人员来保证。 
　　③把空指针转换成目标类型的空指针。 
　　④把任何类型的表达式转换成void类型。 
　　注意：static_cast不能转换掉expression的const、volitale、或者__unaligned属性。 
[C++primer](http://baike.baidu.com/view/1859829.htm)第五章里写了[编译器](http://baike.baidu.com/view/487018.htm)隐式执行任何类型转换都可由static_cast显示完成;
**C++中的static_cast执行非**[**多态**](http://baike.baidu.com/view/126521.htm)**的转换，用于代替C中通常的转换操作。因此，被做为隐式类型转换使用**。比如： 
　　int i; 
　　float f = 166.7f; 
　　i = static_cast<int>(f); 
　　此时结果，i的值为166。 
4. C++ Template 中的typename、class关键字
[http://blog.csdn.net/wzq981264/article/details/705002](http://blog.csdn.net/wzq981264/article/details/705002)
在c++Template中很多地方都用到了typename与class这两个关键字，而且好像可以替换，是不是这两个关键字完全一样呢?
相信学习C++的人对class这个关键字都非常明白，class用于定义类，在模板引入c++后，最初定义模板的方法为： template<class T>...... 
在
这里class关键字表明T是一个类型，后来为了避免class在这两个地方的使用可能给人带来混淆，所以引入了typename这个关键字，它的作用同
class一样表明后面的符号为一个类型，这样在定义模板的时候就可以使用下面的方式了： template<typename
T>......
在模板定义语法中关键字class与typename的作用完全一样。
typename难道仅仅在模板定义中起作用吗？其实不是这样，typename另外一个作用为：使用嵌套依赖类型(nested depended name)
+++++++++++++++++++++++++++++++++++++++++++++++++++
为什么我不能运行？？？？？？？？？？？？
#include<iostream>
using namespace std;
  template <typename T>
     int compare(const T &v1, const T &v2)
     {
         if (v1 < v2) return -1;
         if (v2 < v1) return 1;
         return 0;
     }    
int main ()
     {
         // T is int;
         // compiler instantiates int compare(const int&, const int&)
         cout << compare(1, 0) << endl;
         // T is string;
         // compiler instantiates int compare(const string&, const string&)
         string s1 = "hi", s2 = "world";
         cout << compare(s1, s2) << endl;
         return 0;
     }
因为string 类型没有默认的>.<的运算，所以出错。
你可以把这两句注释掉再跑，就没有问题。 如果你想比较字符串大小，需要先重载字符串的operator>,operator<
+++++++++++++++++++++++++++++++++++++++++++++++++++++
 5.  vector的**reserve**
   vector 的reserve增加了vector的capacity，但是它的size没有改变！而resize改变了vector的capacity同时也增加了它的size！
原因如下：
       reserve是容器预留空间，但在空间内不真正创建元素对象，所以在没有添加新的对象之前，不能引用容器内的元素。加入新的元素时，要调用push_back()/insert()函数。
       resize是改变容器的大小，且在创建对象，因此，调用这个函数之后，就可以引用容器内的对象了，因此当加入新的元素时，用operator[]操作符，或者用迭代器来引用元素对象。此时再调用push_back()函数，是加在这个新的空间后面的。
