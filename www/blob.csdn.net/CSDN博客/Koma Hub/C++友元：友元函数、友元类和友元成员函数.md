# C++友元：友元函数、友元类和友元成员函数 - Koma Hub - CSDN博客
2019年03月28日 21:50:06[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：17
个人分类：[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)
**Table of Contents**
[1、友元函数](#1%E3%80%81%E5%8F%8B%E5%85%83%E5%87%BD%E6%95%B0)
[2、友元类](#2%E3%80%81%E5%8F%8B%E5%85%83%E7%B1%BB)
[3、友元成员函数](#3%E3%80%81%E5%8F%8B%E5%85%83%E6%88%90%E5%91%98%E5%87%BD%E6%95%B0)
[友元小结：](#%E5%8F%8B%E5%85%83%E5%B0%8F%E7%BB%93%EF%BC%9A)
[参考资料：](#%E5%8F%82%E8%80%83%E8%B5%84%E6%96%99%EF%BC%9A)
        有些情况下，允许特定的非成员函数访问一个类的私有成员，同时仍阻止一般的访问，这是很方便做到的。例如被重载的操作符，如输入或输出操作符，经常需要访问类的私有数据成员。
        友元(frend)机制允许一个类将对其非公有成员的访问权授予指定的函数或者类，友元的声明以friend开始，它只能出现在类定义的内部，友元声明可以出现在类中的任何地方：友元不是授予友元关系的那个类的成员，所以它们不受其声明出现部分的访问控制影响。通常，将友元声明成组地放在类定义的开始或结尾是个好主意。
## 1、友元函数
     友元函数是指某些虽然不是类成员函数却能够访问类的所有成员的函数。类授予它的友元特别的访问权，这样该友元函数就能访问到类中的所有成员。
```cpp
#include <iostream>
using namespace std;
class A
{
public:
    friend void set_show(int x, A &a);      //该函数是友元函数的声明
private:
    int data;
};
void set_show(int x, A &a)  //友元函数定义，为了访问类A中的成员
{
    a.data = x;
    cout << a.data << endl;
}
int main(void)
{
    class A a;
    set_show(1, a);
    return 0;
}
```
## 2、友元类
        友元类的所有成员函数都是另一个类的友元函数，都可以访问另一个类中的隐藏信息（包括私有成员和保护成员）。当希望一个类可以存取另一个类的私有成员时，可以将该类声明为另一类的友元类。
关于友元类的注意事项：
(1) 友元关系不能被继承。
(2) 友元关系是单向的，不具有交换性。若类B是类A的友元，类A不一定是类B的友元，要看在类中是否有相应的声明。
(3) 友元关系不具有传递性。若类B是类A的友元，类C是B的友元，类C不一定是类A的友元，同样要看类中是否有相应的申明。
```cpp
#include <iostream>
using namespace std;
class A
{
public:
    friend class C;                         //这是友元类的声明
private:
    int data;
};
class C             //友元类定义，为了访问类A中的成员
{
public:
    void set_show(int x, A &a) { a.data = x; cout<<a.data<<endl;}
};
int main(void)
{
    class A a;
    class C c;
    c.set_show(1, a);
    return 0;
}
```
## 3、友元成员函数
        使类B中的成员函数成为类A的友元函数，这样类B的该成员函数就可以访问类A的所有成员了。
        当用到友元成员函数时，需注意友元声明和友元定义之间的相互依赖，在该例子中，类B必须先定义，否则类A就不能将一个B的函数指定为友元。然而，只有在定义了类A之后，才能定义类B的该成员函数。更一般的讲，必须先定义包含成员函数的类，才能将成员函数设为友元。另一方面，不必预先声明类和非成员函数来将它们设为友元。
```cpp
#include <iostream>
using namespace std;
class A;    //当用到友元成员函数时，需注意友元声明与友元定义之间的互相依赖。这是类A的声明
class B
{
public:
    void set_show(int x, A &a);             //该函数是类A的友元函数
};
class A
{
public:
    friend void B::set_show(int x, A &a);   //该函数是友元成员函数的声明
private:
    int data;
    void show() { cout << data << endl; }
};
void B::set_show(int x, A &a)       //只有在定义类A后才能定义该函数，毕竟，它被设为友元是为了访问类A的成员
{
    a.data = x;
    cout << a.data << endl;
}
int main(void)
{
    class A a;
    class B b;
    b.set_show(1, a);
    return 0;
}
```
## 友元小结：
        在需要允许某些特定的非成员函数访问一个类的私有成员（及受保护成员），而同时仍阻止一般的访问的情况下，友元是可用的。
### 优点：
        可以灵活地实现需要访问若干类的私有或受保护的成员才能完成的任务；
        便于与其他不支持类概念的语言(如[C语言](http://lib.csdn.net/base/c)、汇编等)进行混合编程；
        通过使用友元函数重载可以更自然地使用C++语言的IO流库。
### 缺点：
        一个类将对其非公有成员的访问权限授予其他函数或者类，会破坏该类的封装性，降低该类的可靠性和可维护性。
## 参考资料：
1、《C++ Prime》第4版 第12章类的友元小节
[http://blog.csdn.net/u012796139/article/details/46404401](http://blog.csdn.net/u012796139/article/details/46404401)
[https://www.cnblogs.com/zhuguanhao/p/6286145.html](https://www.cnblogs.com/zhuguanhao/p/6286145.html)
