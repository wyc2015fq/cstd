# Effective C++ 读书笔记(5) - nosmatch的专栏 - CSDN博客
2013年07月08日 22:51:18[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：401
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款五：了解C++默默编写并调用那些函数
> 
是否处在空的类？假设定义类为class Empty{};当C++编译器处理过后会变成如下的形式：
```cpp
class Empty{
	Empty(){}
	~Empty(){}
	Empty& operator=(const Empty& rhs){.....}
	Empty(const Empty& rhs){.....}
}
```
可以分别称为构造函数，析构函数，赋值构造函数，复制构造函数！注意后面两个的区别，从字面上将只有对象存在的
情况下才能被赋值，相反的在对象以其他相同对象构造的时候调用的是复制构造函数；例如
> 
```cpp
People lhs；
People rhs("whz")；
lhs = rhs；
people zhs(rhs)
```
第一个调用的是赋值构造函数，后面调用的是复制构造函数！系统生成的赋值构造函数和复制构造函数只是单纯的将对
像的成员变量进行赋值，没有任何的检查操作，例如对于上面提到的类People，当调用赋值或者复制操作的时候，只是单纯的将
右边对象的对应三个成员变量赋值给左边对象的三个成员变量！
> 
在有些情况下编译器会阻止产生赋值构造函数和复制构造函数，例如当函数的成员变量中存在引用变量或者const变量的
时候，因为此时编译器不知道怎样将右边的对象的相应成员赋值给左边的成员，此时要主要构造函数中对于const或者引用的成
员一定要采用初始化列表的形式，不然会出错！例如对于下面的类将不会产生对应的赋值或者复制构造函数：
> 
```cpp
class People{
private:
    const int age;
    Addredd& addr; 
    .......
public:
    ......
}
```
请记住:
- 编译器会默认为class创建构造函数，析构函数，copy构造函数以及copy assignment操作符！
