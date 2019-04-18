# 【Exceptional C++（4）】编译防火墙及pimpl idiom - weixin_33985507的博客 - CSDN博客
2018年01月29日 16:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
> 
## 编译期的依赖关系
- 下面的头文件中哪些#include语句可以在不对程序产生副作用的前提下去掉
```cpp
// gotw007.h
#include "a.h" // class A
#include "b.h" // class B
#include "c.h" // class C
#include "d.h" // class D
    // 只有A和C有虚函数
#include <iostream>
#include <ostream>
#include <sstream>
#include <list>
#include <string>
class X : public A {
public:
    X(const C&);
    D Function1(int, char*);
    D Function1(int, C);
    B& Function2(B);
    void Function3(std::ostream&) const;
    std::ostream& print(std::ostream&) const;
private:
    std::string name_;
    std::list<C> clist_;
    D d_;
};
std::ostream& operator<<(std::ostream& os, const X& x)
{ return x.print(os); }
class Y : private B {
public:
    C Function4(A);
private:
    std::list<std::wostringstream*> alist_;
};
```
## 解答
- 可以直接去掉的
- iostream，尽管用到了流，但没有用到iostream里特定的东西
- ostream和sstream，换成iosfwd
- 不能直接去掉的
- a.h，因为A是X的基类
- b.h，因为B是Y的基类
- c.h，因为list<C>需要看到C的定义
- d.h，list，string，因为X需要知道D和string的大小，X和Y需要知道list的大小
- 再来考虑通过隐藏X和Y的实现细节可以去掉的头文件
- 通过让X和Y使用pimlp_的方法去掉d.h，list和string，即私有部分被一个指针代替，这时X和Y都不再需要知道list、D或者string的大小，同理也可以这样去掉c.h，因为这时在X::clist中的C对象只作为参数或返回值出现
- B是Y的private基类，且B没有虚函数，因此b.h也可以去掉，使用private继承说明想重写虚函数，与其让Y继承B不如让Y拥有类型为B的成员，要去掉b.h应该让Y的B类型成员存在于Y中隐藏的pimlp_部分
- 使用pimpl_把代码的调用者和实现者分离，声明一个类时要避免暴露私有成员，应该使用一个struct xxxImpl *pimpl_的不透明指针来存储私有成员
- 现在还不能动a.h，A被用作public基类，含有虚函数。然而X和Y两个类之间没有任何关系，因此至少可以把X和Y的定义分别放到两个头文件中（另外还要让现有的头文件包含x.h和y.h），这样至少可以让y.h不包含a.h，因为它只把A用作函数参数的类型，不需要A的定义
```cpp
//x.h
#include "a.h"
#include <iosfwd>
class C;
class D;
class X : public A {
public:
    X(const C&);
    D Function1(int, char*);
    D Function1(int, C);
    B& Function2(B);
    void Function3(std::wostringstream&);
    std::ostream& print(std::ostream&) const;
private:
    class XImpl* pimpl_;
};
inline std::ostream& operator<<(std::ostream&os, const X& x)
{ return x.print(os); }
// y.h，没有#include
class A;
class C;
class Y {
public:
    C Function4(A);
private:
    class TImpl* pimpl_;
};
// gotw007.h作为存根包含两个#include
// 通过x.h又附带了另外两个
#include "x.h"
#include "y.h"
// gotw007.cpp中的新结构
// impl对象应该在X和Y的构造函数中用new创建
// 并在X和Y的析构函数中用delete清除
// X和Y的成员函数要通过pimpl_指针访问数据
struct XImpl
{
    std::string name_;
    std::list<C> clist_;
    D d_;
};
struct YImpl
{
    std::list<std::wostringstream*> alist_;
    B b_;
};
// 现在，X和Y的使用者只要包含a.h和iosfwd
// 即使后来更新代码需要包含y.h并去掉gotw007.h
// 也不用加一行#include
```
> 
## 编译防火墙
- 如果类定义中的任何部分被改变，这个类的使用者必须重新编译代码，为了降低依赖性，一种常见技术是利用一个不透明指针隐藏一部分实现细节
```cpp
class X {
    ...
private:
    class XImpl* pimpl_;
};
```
- 可见类：客户代码所见并操控的类（即这里的X）
- pimpl：可见类隐藏在一个指针（pimpl_）下的类实现（XImpl）
- 这种方法的主要代价是性能
- 每个操作必须分配内存
- 每个隐藏成员需要一个额外的间接层来予以对其访问
- 应该放入XImpl的部分有四种常见的原则
- 全部私有数据：不错的开端，但可以做的更好
- 全部私有成员（包括函数）：这几乎是最常用的用法，但对此有两个警告
- 但虚函数即使是私有的也不能放到pmpl中
- 如果pimpl中的函数要使用其他函数，可能需要一个指向可见对象的back pointer，这又增加了一层间接性，这个指针通常称为self_
- 全部私有成员和保护成员：这样更进一步的做法是错误的，保护成员不能放入pimpl。保护成员是为了让派生类看到并使用而存在的，如果派生类看不到的话等于对其弃之不用
- 使XImpl完全成为原来的X，将X编写为一个完全由简单的前置函数组成的公共接口：这只在少数几个有限情况下有用，好处是可以避免back pointer，但这样会使得可见类对继承来说全无用处
- XImpl通常需要一个指向X对象的back pointer，因为在pimpl中的函数也经常要调用可见类中的函数，通常要调用的函数是公有成员或虚函数
> 
## Fast Pimpl技术
- malloc()和new()调用开销很大，下面代码最初在class Y中设计了一个类型X的成员
```cpp
// file y.h
#include "x.h"
class Y {
    ...
    X x_;
};
// file y.cpp
Y::Y() {}
```
- 这个class Y的声明需要X的声明可见（从x.h中），为避免这个条件改写为
```cpp
// file y.h
class X;
class Y {
    ...
    X* px_;
};
// file y.cpp
#include "x.h"
Y::Y() : px_(new X) {}
Y::~Y() { delete px_; px_ = 0; }
```
- 这很好地隐藏了X，但Y被广泛使用时动态内存分配开销降低了性能。隐藏X的动机是避免Y的用户必须知道X，为了消除这种依赖，通常使用pimp方法，但pimpl由于为X的对象分配内存也会导致性能下降。解决方法是Fast Pimpl，使用一个高效的Pimpl，也就是有自己的operator new的pimpl类
```cpp
// file y.h
class YImpl;
class Y {
    ...
    YImpl* pimpl_;
};
// file y.cpp
#include "x.h"
struct YImpl {
    ... // private stuff here
    void* operator new(size_t) { ... }
    void operator delete(void*) { ... }
};
Y::Y() : pimpl_(new YImpl) {}
Y::~Y() { delete pimpl_; pimpl_ = 0; }
```
- 讨论可用性，一个将它们放入一个固定尺寸内存分配器模板中的技巧如下
```cpp
template<size_t S>
class FixedAllocator {
public:
    void* Allocate(...);
    void Deallocate(void*);
private:
    // implemented using static?
};
```
- 因为私有细节很可能用static实现，因此问题是Deallocate是否被一个static对象的析构函数调用，也许更安全的方法是使用single模式，为每个被请求的尺寸使用一个独立链表，并用一个唯一对象管理这些链表
```cpp
class FixedAllocator {
public:
    static FixedAllocator* Instance();
    void* Allocate(size_t);
    void Deallocate(void*);
private:
    // singleton implementation
};
// 用一个辅助基类封装调用
struct FastPimpl {
    void* operator new(size_t s) {
        return FixedAllocator::Instance()->Allocate(s);
    }
    void operator delete(void* p) {
        FixedAllocator::Instance()->Deallocate(p);
    }
};
// 现在可以很容易地写出任意的Fast Pimpl
struct YImpl : FastPimpl {
    ... // private stuff here
};
```
- 但不要乱用Fast Pimpl，虽然得到了最佳内存分配速度，但维护这些独立链表将导致空间性能小将，因为这会有更多内存碎片，只有在使用了profiler剖析性能并证明需要性能优化后才使用此方法
> 
## 名称查找
- 下面代码调用的是哪个函数
```cpp
namespace A {
    struct X;
    struct Y;
    void f(int);
    void g(X);
}
namespace B {
    void f(int i) { 
        f(i); // 1
    }
    void g(A::X x) {
        g(x); // 2
    }
    void h(A::Y y) {
        h(y); // 3
    }
}
```
## 解答
- 1调用的是它自己，并且是无穷递归，因为B没有using namespace A或using A::f，如果写了，将在A::f(int)和B::f(int)间造成二义性
- 2调用则会造成二义性，Koenig Lookup（简化版）规定如果给函数提供一个class类型实参，在名称查找时编译器将认为包含实参类型的命名空间中的同名函数为可选函数
- 3调用的是它自己，也是无穷递归，因为没有其他的h(A::Y)函数
> 
## 接口原则
- Class定义：一个Class描述了一组数据及操作这些数据的函数
```cpp
class X { ... };
void f(const X&); // f是X的一部分吗？
// 如果上面的代码出现在一个头文件中
class X {
public:
    void f() const; // f是X的一部分吗？
}
```
- 接口原则：对于一个类X，所有的函数，包括非成员函数，只要同时满足提到X且与X同期提供，那么就是X的逻辑组成部分，因为它们形成了X的接口。根据定义，所有成员函数都是X的组成部分
- 每个成员函数都必须提到X：非static的成员函数有一个隐含参数，类型是X* const或const X* const。static的成员函数也是在X的作用范围内的
- 每个成员函数都与X同期提供（在X的定义体内）：f提到X，如果f与X同期存在（如存在于相同的头文件、命名空间），根据接口原则，f是X的逻辑组成部分，因为它是X接口的一部分
- 接口原则可以用于判断什么是一个class的组成部分
```cpp
class X { ... };
ostream& operator<<(ostream&, const X&);
```
- 这样就很容易看出来，如果operator<<与X同期提供，那么operator<<是X的逻辑组成部分，因为它是X的接口的一部分，class的定义中并没有要求函数是否为成员
- 接口原则还有更深远的意义，如果你能认识到它和Koenig Lookup干了相同的事
```cpp
namespace N {
    class X {};
    void f(T);
}
N::X parm;
int main() {
    f(parm); // 通过ADL调用N::f
}
```
- 很明显f(parm)不需要明确写为NS::f(parm)，但这对编译器来说并不明显，Koenig Lookup让编译器正确完成。Koenig Lookup规定如果传给函数一个class类型实参，为了查找这个函数名，编译器不仅要查找局部作用域，还要查找包含实参类型的命名空间
- 不用明确限定f我们就很容易限定函数名
```cpp
#include <iostream>
#include <string>
int main() {
    std::string hello = "hello, world";
    std::cout << hello; // call std::operator<<
}
```
- 如果没有Koenig Lookup编译器就无法找到operator<<，我们只知道它是string的一部分，如果必须强迫限定这个函数名会很麻烦，得写成std::operator<<(std::cout, hello)
- 回到接口原则，看这个Myers Example
```cpp
namespace NS {
    class T {};
}
void f(NS::T);
int main() {
    NS::T parm;
    f(parm); // call global f
}
```
- 有一天NS的作者需要增加一个函数
```cpp
namespace NS {
    class T {};
    void f(T); // new function
}
void f(NS::T);
int main() {
    NS::T parm;
    f(parm); // ambiguous: call NS::f or global f?
}
```
- 在命名空间增加一个函数的行为破坏了命名空间外的代码，即使用户没有用using把NS带到作用域中。不过Nathan Myers（C++标准委员会的长期会员）指出了在命名空间与Koenig Lookup之间的关联
```cpp
namespace A {
    class X {};
}
namespace B {
    void f(A::X);
    void g(A::X parm) {
        f(parm); // call B::f
    }
}
// 增加一个函数
namespace A {
    class X {};
    void f(X); // new function
}
namespace B {
    void f(A::X);
    void g(A::X parm) {
        f(parm); // ambiguous: call A::f or B::f
    }
}
```
- 命名空间的作用就是为了防止名字冲突，但在一个命名空间中增加函数却看似破坏了另一个完全隔离的命名空间的代码。事实上，这是应该发生的正确行为。如果X所在的命名空间中有f(X)，根据接口原则，f是X的接口的一部分，f是一个自由函数不是关键，想确认它仍然是X的逻辑组成部分只要换个名字看
```cpp
namespace A {
    class X {};
    ostream& operator<<(ostream&, const X&);
}
namespace B {
    ostream& operator<<(ostream&, const A::X&);
    void g(A::X parm) {
        cout << parm; // ambiguous: A::operator<< or B::operator<< ?
    }
}
```
- 如果用户代码提供了一个提及X的函数，而它与X所处的命名空间的某个函数重合。B必须明确表明想调用的函数，这正是接口原则应该提供的东西，因此接口原则与Koenig Lookup的行为相同并非意外，Koenig Lookup的行为正是建立在接口原则基础上的
- 虽然接口原则说明成员和非成员函数都是class的逻辑组成部分，但并没有说明两者是平等的，在名称搜索中，成员函数与class之间的关联关系比非成员函数更强
```cpp
namespace A {
    class X { };
    void f(X);
}
class B { // class, not namespace 
    void f(A::X);
    void g(A::X parm) {
        f(parm); // call B::f
    }
};
```
- 如果A和B都是class，f(A, B)是一个自由函数
- 如果A与f同期提供，那么f是A的组成部分，且A将依赖B
- 如果B与f同期提供，那么f是B的组成部分，且B将依赖A
- 如果A、B、f同期提供，那么f同时是A和B的组成部分，且A与B是循环依赖
- 如果A和B都是class，且A::g(B)是A的一个成员函数
- 因为A::g(B)的存在，A总是依赖B
- 如果A和B是同期提供的，那么显然A::g(B)和B也是同期提供的，于是A::g(B)同时满足提及B和与B同期提供，所以根据接口原则A::g(B)是B的组成部分，又因为A::g(B)使用了一个隐含的A*参数，所以B依赖A，因此A和B循环依赖
- 与class不同，namespace不需要一次声明完毕，同期提供取决于namespace当前的可见部分
```cpp
// file a.h
namespace N { class B; }
namespace N { class A; }
class N::A { public: void g(B); };
// file b.h
namespace N { class B { ... }; }
```
- A的用户包含了a.h，于是A和B是同期提供且循环依赖的，B的用户包含了b.h，于是A和B不是同期提供的
