# static_cast在CRTP中的应用 - 电影旅行敲代码 - CSDN博客
2015年11月03日 21:28:03[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：553标签：[多态																[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)](https://so.csdn.net/so/search/s.do?q=多态&t=blog)
个人分类：[c++基础](https://blog.csdn.net/dashuniuniu/article/category/5682427)
CRTP是C++中实现静态多态(static polymorphism)的方式，这种实现方式能够提高运行时效率及减少内存消耗，在嵌入式领域相当有优势。这里估计会有人反驳，难道针对每个派生类实例化独有的基类难道不占有内存吗？
编译时，实例化的独有基类确实会增加很多类型信息，但是这仅仅是类型信息，只存在于编译期。程序占内存的大小取决于数据段和代码段的大小，代码段只是一条条的汇编指令，里面根本没有类型信息，什么实例化出独有的基类都已经消失了，唯一会存在的是static_cast <T*>(this)->implementation()函数调用所对应的汇编指令，更不会有虚函数所带来的虚表以及为了实现RTTI儿存放的type_info结构体。但是这种方式没有虚函数来的直接明了。
在现实中有很多应用，例如在微软ATL以及LLVM项目中。
CRTP是通过派生类继承类模板实例化出的基类(拗口)，其中模板参数就是自身，基类中使用static_cast<T*>(this)强制类型转换，然后进行调用派生类中自定义的成员函数，从而实现一份代码多种行为。  
示例代码如下：
```cpp
template <class T>
struct Base {
    void interface(){
        // ...
        static_cast<T*>(this)->implementation();
    }
};
struct Derived : Base<Derived>{
    void implementation();
};
```
注意第5行中使用了static_cast，曾记得我大学老师在给我们讲授CRTP的原理时，一再强调这种转换方式是安全的，没有问题的。但是他举得例子比较单一，如下代码所示：
```
int main(){
    /// 第一种方式
    Derived D;
    // 此时调用当然是安全的
    // Derived继承了基类的interface()函数，函数中的this指针指向的就是D
    D->interface();
    /// 第二种方式
    Base<Derived> B_D;
    B_D.interface();
    return 0;
}
```
第一种方式当然没有问题，指向Derived类型的this指针转换成指向Derived类型的指针。但是第二种方式就有问题了，指向Base的this指针转换成Derived类型的指针就是有问题的了。
如果当Base类和Derived中没有数据成员时，这种转换不会造成实质性的危害，但是当类中有数据成员时就不同了，当代码如下所示时，就会出现问题。
```cpp
#include <iostream>
#include <typeinfo>
using namespace std;
template <class T>
class Base {
public:
    int m_b;
    void method() {
        static_cast<T*>(this)->implementation();
        // 将指向Base<T>的this指针强制转换为T*类型
        // 然后修改本不属于this内存范围内的数据（其实修改的是栈中和自己无关的其他数据）
        // 和缓冲区溢出很相似
        static_cast<T*>(this)->m_d_1 = 1;
        // 打印出三者的内存地址，其实是相等的
        cout << this << endl;
        cout << &(static_cast<T*>(this)->m_b) << endl;
        cout << &m_b << endl;
        cout << &(static_cast<T*>(this)->m_d_1) << endl;
        //cout << typeid(T).name() << endl;
        //cout << typeid(this).name() << endl;
    }
    Base() { m_b = 10; }
};
/*
class Base {
public:
    void method() {
    // 父类强转成子类
    static_cast<Derived*>(this)->implementation();
}
};
*/
class Derived : public Base<Derived>{
public:
    int m_d_1;
    Derived() : Base<Derived>() { m_d_1 = 10; }
    void implementation() {
        std::cout << "Derived" << endl;
    }
};
class Derived2 : public Base<Derived2> {
public:
    int m_d_1;
    Derived2() : Base<Derived2>() { m_d_1 = 30; }
    void implementation() {
        std::cout << "Derived2" << endl;
    }
};
int main() {
    Base<Derived> pBase;
    Base<Derived2> pBase2;
    pBase.method();
    cout << pBase.m_b << endl;
    pBase2.method();
    cout << pBase.m_b << endl;
    return 0;
}
```
上面pBase2.method()将Base类型的this指针强制转换成Derived类型指针，然后修改m_d_1，其实修改的是pBase对象中的m_b数据成员，测试结果如下：
```
Derived
0x24fe9c
0x24fe9c
0x24fe9c
0x24fea0
10
Derived2
0x24fe98
0x24fe98
0x24fe98
0x24fe9c
1
```
栈帧修改过程如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20151104095319115)
这是使用g++进行测试的，使用Visual Studio进行测试的时候，没有出现这种问题，由于Visual Studio对在栈上分配的对象中间有两个字的缓冲。但即使这样，当派生类中数据成员很多时同样很有可能导致数据被复写，需要程序员自己去控制。
如下是一种CRTP的常见使用方式，其中Base <T>就相当于运行时多态中的基类，只是CRTP中Base<T>执行的eval()方法是在编译期间就已经得到的，而在运行时多态中到底执行哪个eval()方法是在运行时通过virtual table实现的。
```cpp
#include <iostream>
using namespace std;
template <typename Derived>
struct Base
{
    void eval()
    {
        cout << "eval in Base;" << endl;
        static_cast<Derived*>(this)->eval();
    }
};
struct Child : Base<Child>
{
    void eval()
    {
        cout << "eval in Child;" << endl;
    }
};
// 下面这个函数体现出了CRTP的静态分发的效果
template<typename T>
void test(Base<T>& t)
{
    t.eval();
}
int main()
{
    Child c;
    test(c);
}
```
