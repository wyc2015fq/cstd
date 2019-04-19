# C++11中的POD和Trivial - 电影旅行敲代码 - CSDN博客
2015年11月25日 20:50:09[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：2398
# 引子
在介绍C++11的文章或者博客中，经常会出现POD类型和Trivial类型的影子。但是POD类型和Trivial类型到底是什么意思呢？
- POD类型
- POD类型的好处
## POD类型
**粗略上来讲，POD是C++为兼容C的内存布局而设计的，主要用于修饰用户自定义类型**。但POD却远比这个要复杂。POD(Plain Old Data)，从字面意思上来看，是平凡普通的老旧格式的数据，POD是一个类型属性，既不是关键字也不会像”volatile”用来修饰类型信息。
POD类型，说明该数据是普通的，不会有什么虚函数啊，虚继承啊，或者内嵌的数据类型很复杂的情况。也就是说，POD类型的C++变量，可以直接用C语言中的struct来解释。
如下代码所示：
```
class PODType
{
    public:
        int mem_int;
        char* mem_cp;
};
```
PODType类型的内存布局如下的，POD类型太过于普通以至于使用简简单单的C语言结构体即可表示：
![这里写图片描述](https://img-blog.csdn.net/20151125205656037)
如果类型含有虚函数将会如何呢？比如：
```
class NonPODType
{
    public:
        int mem_int;
        // 含有虚函数，内存布局中有虚表指针
        virtual int func() { return 0; }
};
```
NonPODType类型的内存布局如下所示，C语言内存布局中哪里来的虚表指针呢，C语言根本就无法解释这种情况： 
![这里写图片描述](https://img-blog.csdn.net/20151125215924583)
直观的来书，POD类型中的O代表了与C语言的兼容性，可以直接使用memcpy()直接复制而不会出现任何问题。POD这个想法的由来就是要支持两种最基本的属性： 
（1）支持静态初始化 
（2）编译C++中的POD类型所得到的内存布局，和C中编译struct的内存布局相同
具体地，C++11将POD类型划分为两个概念：
- trivial
- standard layout
在C++11中，更多的是用trivial和standard类代替POD类型。POD类型的递归定义如下：
> 
A POD struct is a non-union class that is both a trivial class and a standard-layout class, and has no non-static data members of type non-POD struct, non-POD union (or array of such types). 
Similarly, a POD union is a union that is both a trivial class and a standard layout class, and has no non-static data members of type non-POD struct, non-POD union (or array of such types). 
A POD class is a class that is either a POD struct or a POD union.
### Trivial平凡类型
平凡类型的标准定义如下：
> 
A trivially copyable class is a class that: 
  — has no non-trivial copy constructors (12.8), 
  — has no non-trivial move constructors (12.8), 
  — has no non-trivial copy assignment operators (13.5.3, 12.8), 
  — has no non-trivial move assignment operators (13.5.3, 12.8), and 
  — has a trivial destructor (12.4). 
  A trivial class is a class that has a trivial default constructor (12.1) and is trivially copyable. 
  [ Note: In particular, a trivially copyable or trivial class does not have virtual functions or virtual base classes.—end note ]
下面我们依次解释上面的定义， 
（1）拥有平凡的构造函数(trivial constructor)和析构函数(trivial destructor) 
。平凡的默认构造函数即构造函数“什么都不干”。**通常情况下，不定义类的构造函数，编译器就会为我们生成一个平凡的默认构造函数**。注意哦，是通常情况下，如果该类含有虚函数或者数据成员没有默认构造函数，编译器是不会为我们生成平凡的默认构造函数的。
注意，如果你定义了构造函数，即使构造函数体为空，则该类型也是不平凡的。仔细想一想，也对，既然POD类型与C语言结构体等价，C语言哪来的用户定义构造函数呢。当然可以使用default关键字强制使用编译器提供的默认的平凡构造函数。
（2）拥有平凡的拷贝构造函数（trivial copy constructor）和移动构造函数（trivial copy constructor）。平凡的拷贝构造函数基本上等同于memcpy进行类型的构造。同平凡构造函数一样，编译器在用户不提供的情况下，通常会提供平凡的拷贝构造函数。
平凡的移动构造函数执行的操作与平凡的拷贝构造函数相同，都是bytewise的拷贝，只是用于移动语义。移动构造函数嘛，只是提供了一个接口来捕获右值，右值在C++11中分为两种，**将忘值（xvalue, expiring value）**和**纯右值（prvalue, Pure Rvalue）**。将忘值包括返回T&&的函数返回值，std::move()返回值以及返回T&&的类型转换函数的返回值。纯右值基本等同于临时变量（对象）。
（3）拥有平凡的拷贝赋值运算符（trivial assignment operator）和移动赋值运算符（move operator）。
（4）不能包含虚函数以及虚基类。
C++11提供了内置函数来支持Trivial类型的判断。
```cpp
template <typename T> struct std::is_trivial
```
### Standard Layout标准布局
POD包含的另外一个概念就是标准布局。标准布局的类或结构体应该符合以下定义： 
（1）所有的非静态成员有相同的访问权限。成员变量同为public，private，protected。
首先静态成员不参与对象的内存布局，而是存放在全局数据区。
相同访问权限，也可以理解，C语言中struct数据成员默认都是public，数据成员的访问权限都相同，不存在相邻的数据成员在类型以外的任何不同。
（2）在类或者结构体继承时，满足一下两种情况之一：
- 派生类中有非静态成员，且只有一个仅包含静态成员的基类。
- 基类有非静态成员，而派生类中没有非静态成员
- 派生类有非静态成员，而基类中没有静态成员（**但是非静态成员的类型必须与基类不同**）
只要非静态成员同时出现在派生类中或者基类中，都不属于标准布局。而多重继承也会导致类型布局的一些变化，所以一旦非静态成员出现在多个基类中，派生类也不属于标准布局的。
（3）类中第一非静态成员的类型与其基类不同
```
class A {};
class B : public A
{ 
public:
    A a;
    int mem;
};
```
B类型就不是标准布局的。这个特殊要求要从C++中的一个标准说起，**同一类型的两个对象的地址必须不同**。
```cpp
class Non {};
int main()
{
    Non n1;
    Non n2;
    int num = 10;
    // Non类型的大小为1
    cout << sizeof(Non) << endl;
    return num;
}
```
上述代码中的n1和n2属于同一类型，并且该类型是空的。如果编译器不对该类型的内存做特殊处理，那么n1和n2的地址是相同，这当然是不合理的。所以，**编译器给空的class类型都安插了一个隐含的字节**。当然，有时候编译器会padding出3个字节以便对齐。main的栈帧如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20151126144611648)
C++标准允许，在基类没有成员时，派生类第一个成员与基类共享地址。但是当派生类中第一个数据成员类型为基类类型时，有趣的问题就来了。首先，**这时派生类的内存布局包括基类部分的内存布局，同时自己又添加了另外一个基类类型的变量，如果编译器优化实现第一个成员和基类部分共享地址，那么就违背了C++标准的另一个要求，同类型的不同对象地址必须不同。**
所以在标准布局的解释中，C++11标准强制要求派生类的第一个非静态成员的类型必须与基类不同。比如：
```
class A{};
class B : public A
{
public:
    A a;
    int mem;
};
int main()
{
    B b;
    return 0;
}
```
main的栈帧如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20151126150127269)
（4）没有虚函数和虚基类 
（5）所有非静态数据成员均符合标准布局类型，其基类也符合标准布局，这是一个递归定义。注意，静态数据成员是存放在全局数据区的，不参与内存布局。
## POD类型的好处
POD类型多用于类和结构体。POD带来的好处如下：
- 
字节赋值(bytewise copy)。可以使用memset和memcpy对POD类型进行初始化
- 
提供对C内存布局的兼容。C++程序可以与C函数进行交互操作，POD类型保证这种在C与C++之间的操作总是安全的
- 
保证了静态初始化的安全有效，用于提供程序性能。直接放入.bss段，在初始化中直接赋0
*尾注：本博客中的内容借鉴摘抄了[《深入理解C++11》](http://book.douban.com/subject/24738301/)中的部分内容*
