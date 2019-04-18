# C++ 中两个类互相引用的问题 - 文章 - 伯乐在线
原文出处： [weixliu，2014-09-05](http://www.cnblogs.com/weixliu/p/3957227.html)
最近在改一个C++程序的时候碰到一条警告信息，警告信息为：“
删除指向不完整“Q2DTorusNode”类型的指针；没有调用析构函数
1> c:userslxwdesktopdragonfly第二阶段实验最终的实验版本实验目录dragonfly_modifysrcQ2DTorus.h(6) : 参见“Q2DTorusNode”的声明
”
警告信息很是奇怪，其实出于强迫症的原因想要解决掉这个警告信息，而且从警告信息来看，程序也应该存在内存泄露的问题，因为警告直接明白告诉你了，没有调用析构函数，接下来就是我解决的过程。我会搭建一个简单的程序来模拟这个错误，因为程序是在有些多~
**警告的来源：**
一个头文件A.h包含class A的代码如下：


```
#ifndef AH
#define AH
class B;
class A {
    B *memb;
    A() {
         
    }
    ~A() {
        delete memb;
    }
};
#endif
```
一个头文件B.h包含class B的代码如下：

```
#ifndef BH
#define BH
class B {
     
};
#endif
```
此时编译就会产生类似上面的警告信息：warning C4150: 删除指向不完整“B”类型的指针；没有调用析构函数。
**原因分析：**
因为class A中B的声明依赖于class B的前置声明，而不是#include “B.H”，所以B的定义对A来说不可见，所以无法调用析构函数，导致内存泄露。
**程序的变化**
此时如果class A和class B相互保持对方类型的成员会如何呢？
A.h的代码：

```
#ifndef AH
#define AH
class B;
class A {
    B b;
};
#endif
```
B.h的代码：

```
#ifndef BH
#define BH
#include "A.h"
class B {
    A a;
};
#endif
```
这段代码存在问题，因为如果静态定义对象A，B，此时必定存在一个对象的定义对于另外一个对象的定义不可见，所以定义失败。如果均是利用#include对方，取决于编译器的顺序必定一个定义不可见。然而前置声明不能定义对象。
**解决方案：**
此种状况的解决利用前置声明定义的那个类中的保持另外一个类的引用定义为指针，定义指针时不需要对那个类的定义可见。
**另外的问题：**
**A.h**

```
#ifndef AH
#define AH
class B;
class A {
    B* b;
    void setB() {
        b->haha();
    }
    ~A() {
        delete b;
    }
};
#endif
```
**B.h**

```
#ifndef BH
#define BH
#include "A.h"
class B {
    A a;
    void haha() {
 
    }
};
#endif
```
但是利用前置声明导致定义指针成员的类会出现最开始说的warning警告，因为定义不可见的原因。
“warning C4150: 删除指向不完整“B”类型的指针；没有调用析构函数”
而且另外的一个问题是在该.h文件中不能使用该指针调用这个类的成员，原因也是定义不可见。
“error C2227: “->haha”的左边必须指向类/结构/联合/泛型类型”
**解决方案：**
此时需要将A.h的所有成员函数实现重新定义一个.cpp文件，然后该.cpp文件去#include 指针成员类的头文件声明，此时定义可见，即可定义析构函数，调用指针的类成员了。
A.h

```
#ifndef AH
#define AH
class B;
class A {
public:
    B* b;
    void setB();
    ~A();
};
#endif
```
B.h

```
#ifndef BH
#define BH
#include "A.h"
class B {
public:
    A a;
    void haha() {
 
    }
};
#endif
```
A.cpp

```
#include "A.h"
#include "B.h"
A::~A() {
    delete b;
}
void A::setB() {
    b->haha();
}
```
问题到此就解决完毕了~
