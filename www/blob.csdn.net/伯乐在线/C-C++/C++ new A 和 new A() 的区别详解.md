# C++ new A 和 new A() 的区别详解 - 文章 - 伯乐在线
原文出处： [江南烟雨，2013-04-13](http://blog.csdn.net/xiajun07061225/article/details/8796257)
我们在C++程序中经常看到两种new的使用方式：new A以及new A()。那么这两种究竟有什么区别呢？
调用new分配的内存有时候会被初始化，而有时候不会，这依赖于A的类型是否是POD（Plain old data）类型，或者它是否是包含POD成员、使用编译器生成默认构造函数的类。
附：POD类型
POD是Plain old data的缩写，它是一个struct或者类，且不包含构造函数、析构函数以及虚函数。
维基百科给出了更加详细的解释：
C++的POD类型或者是一个标量值，或者是一个POD类型的类。POD class没有用户定义的析构函数、拷贝构造函数和非静态的非POD类型的数据成员。而且，POD class必须是一个**aggregate**，没有用户定义的构造函数，没有私有的或者保护的非静态数据，没有基类或虚函数。它只是一些字段值的集合，没有使用任何封装以及多态特性。
附：aggregate的定义：
An aggregate is an array or a class (clause 9) with no user-declared constructors (12.1), no private or protected non-static data members (clause 11), no base classes (clause 10), and no virtual functions (10.3).
接着介绍一下C++中的三种初始化方式：
zero-initialization,default-initialization,value-initialization。
首先需要注意的是value-initialization是在C++2003标准中新引入的，在原来的1998标准中并不存在。
C++03标准中针对这三种方式的说明：
To zero-initialize an object of type T means:
— if T is a scalar type (3.9), the object is set to the value of 0 (zero) converted to T;
— if T is a non-union class type, each nonstatic data member and each base-class subobject is zero-initialized;
— if T is a union type, the object’s first named data member is zero-initialized;
— if T is an array type, each element is zero-initialized;
— if T is a reference type, no initialization is performed.
To default-initialize an object of type T means:
— if T is a non-POD class type (clause 9), the default constructor for T is called (and the initialization is ill-formed if T has no accessible default constructor);
— if T is an array type, each element is default-initialized;
— otherwise, the object is zero-initialized.
To value-initialize an object of type T means:
— if T is a class type (clause 9) with a user-declared constructor (12.1), then the default constructor for T is called (and the initialization is ill-formed if T has no accessible default constructor);
— if T is a non-union class type without a user-declared constructor, then every non-static data member and base-class component of T is value-initialized;
— if T is an array type, then each element is value-initialized;
— otherwise, the object is zero-initialized
A program that calls for default-initialization or value-initialization of an entity of reference type is ill-formed. If T is a cv-qualified type, the cv-unqualified version of T is used for these definitions of zero-initialization, default-initialization, and value-initialization.
注意：VS2008遵循的是98标准，而GCC3.4.5遵循的是03标准。
采用如下代码可以验证编译器遵循的到底是哪一种标准：


```
#include <stdio.h>  
#include <string.h>  
#include <new>  
  
struct A { int m; }; // POD  
struct B { ~B(); int m; }; // non-POD, compiler generated default ctor  
struct C { C() : m() {}; ~C(); int m; }; // non-POD, default-initialising m  
  
int main()  
{  
    char buf[sizeof(B)];  
    memset( buf, 0x5a, sizeof( buf));  
  
    // use placement new on the memset'ed buffer to make sure   
    //  if we see a zero result it's due to an explicit   
    //  value initialization  
    B* pB = new(buf) B();   //C++98 rules - pB->m is uninitialized  
                            //C++03 rules - pB->m is set to 0  
    printf( "m  is %d\n", pB->m);  
    return 0;  
}
```
在VS008中输出就不是0，说明遵循的是98标准。
下面先看一段C++示例代码：


```
#include <iostream>  
  
using namespace std;  
  
struct A { int m; }; // POD  
struct B { ~B(){}; int m; }; // non-POD, compiler generated default ctor  
struct C { C() : m() {}; ~C(){}; int m; }; // non-POD, default-initialising m  
  
int main()  
{  
    A *aObj1 = new A;  
    A *aObj2 = new A();  
    cout << aObj1->m << endl;  
    cout << aObj2->m << endl;  
  
    B *bObj1 = new B;  
    B *bObj2 = new B();  
    cout << bObj1->m << endl;  
    cout << bObj2->m << endl;  
  
    C *cObj1 = new C;  
    C *cObj2 = new C();  
    cout << cObj1->m << endl;  
    cout << cObj2->m << endl;  
  
    delete aObj1;  
    delete aObj2;  
    delete bObj1;  
    delete bObj2;  
    delete cObj1;  
    delete cObj2;  
  
    return 0;  
}
```
运行结果：
![](http://jbcdn2.b0.upaiyun.com/2016/12/56bbe5b4eb63ae5814da96fd3c505d17.jpg)
上述测试平台是VS2008.需要注意的是，VS08只支持C++98。
在这种情况下：
new A：不确定的值
new A()：zero-initialize
new B：默认构造（B::m未被初始化）
new B()：默认构造（B::m未被初始化）
new C：默认构造（C::m被zero-initialize）
new C()：默认构造(C::m被zero-initialize)
如果用兼容C++03的编译器，比如G++结果：
![](http://jbcdn2.b0.upaiyun.com/2016/12/0644b4253347398cc22b50a005c59f62.jpg)
new A：不确定的值
new A()：value-initialize A，由于是POD类型所以是zero initialization
new B：默认构造（B::m未被初始化）
new B()：value-initialize B，zero-initialize所有字段，因为使用的默认构造函数
new C：default-initialize C，调用默认构造函数
new C()：value-initialize C，调用默认构造函数
在所有C++版本中，只有当A是POD类型的时候，new A和new A()才会有区别。而且，C++98和C++03会有区别。
参考资料：
[What are POD types in C++?](http://stackoverflow.com/questions/146452/what-are-pod-types-in-c)
[What are Aggregates and PODs and how/why are they special?](http://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special/4178176#4178176)
[What do the following phrases mean in C++: zero-, default- and value-initialization?](http://stackoverflow.com/questions/1613341/what-do-the-following-phrases-mean-in-c-zero-default-and-value-initializat)
