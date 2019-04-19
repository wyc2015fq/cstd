# C++中的new/delete和new[]/delete[] - 电影旅行敲代码 - CSDN博客
2015年11月07日 17:27:20[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：499
# 引子
- C++的new/delete
- C++的new[]/delete[]
## C++的new/delete
当我们使用一条new表达式时：
```
// new表达式
string *sp = new string("a value");
string *arr = new string[10];
```
实际执行了三步操作，
**第一步**：new表达式调用一个名为operator new(或者operator new[])的标准库函数。该函数分配一块足够大的、原始的、未命名的内存空间以便存储特定类型的对象（或者对象的数组）。 
**第二步**：编译器运行相应的构造函数以构造这些对象，并为其传入初始值。 
**第三步**：对象分配了空间并构造完成，返回一个指向该对象的指针。
相比于C语言来说，将两个操作合并在一起隐藏了内部实现并提供了安全性。但是也带来了不便，比如过在new []数组时，只能使用默认构造函数来构造对象。用户不可能使用自定义的构造函数来强行插入到内部new []中。
当我们使用一条delete表达式删除一个动态分配的对象时：
```java
delete sp; // 销毁*sp，然后释放sp指向的内存空间
delete [] arr; // 销毁数组中的元素，然后释放对应的内存空间
```
实际执行了两步操作， 
**第一步**：对sp所指的对象或者arr所指的数组中的元素执行对应的析构函数。 
**第二步**：编译器调用名为operator delete（或者operator delete[]）的标准库函数释放内存空间。
但是有部分C语言的支持者吐槽了delete[]的部分缺点，通过delete []来销毁一个对象数组并释放内存是低效的操作。由于运行时库的**内存分配器**需要知道包含这个数组的内存块的大小，所以在内部需要以某种方式存储这块内存的大小。delete []运算符需要知道有多少对象需要销毁，所以需要存储数组的大小。最终，用户代码通常需要迭代整个数组，所以不可避免又需要存储数组的大小。
## C++的new[]/delete[]
C++从C中继承了一个问题，就是指针可以指向个别的对象，而这个对象实际上却是某个数组的初始元素。
当然使用”new[]”在堆上创建对象数组，如果按照普通的”new/delete”模式中delete释放，是能够正确释放的，**因为每段在堆上分配的内存前后都有保护字节”守护”，并且前面还有当前分配的内存块有多大的信息，只要delete能够向前寻找到当前内存块的大小，就能够正确释放**。但是当前内存块的数据是正确释放掉了，但是对象的析构函数却没有正确调用，只有数组头部的对象是正确调用析构函数的。**如果对象有持有动态内存的数据成员，那么动态内存就会泄露。这是我们很容易犯的问题，所以在delete的时候，必须知道需要释放多少对象，然后依次调用其析构函数**。
**但是这就带了个问题，delete在删除时是怎么区分是普通对象还是数组呢？**
对于这个问题，C++在初始版本的时候使用delete []运算符，这样就减少了delete运算符同时处理释放单个对象和整个数组对象的复杂性。虽然中间曾一度出现过”delete[size] ptr”这种形式，但是最后还是将维护数组元素个数的负担转给了语言的实现。
在stackoverflow上也曾有人询问过C++中的delete []是如何知道要删除多少元素的？这里我们就来探讨一下delete []是如何知道数组元素个数的。
new[]将数组分配在一块连续的内存空间上，其中对象一个一个的连续对齐存放，在整个数组对象之前存放了一个双字（DWORD）来表示这里有多少个对象。这样delete []时才会在删除的时候知道有多少个元素需要删除。new []运算符返回的是指向数组头部对象的指针，这个双字的计数值Count就隐式地存放在这个对象前面。 
![new数组时的内存示意图](https://img-blog.csdn.net/20151108155624369)
下面我们将介绍一下Microsoft C++ Compiler是怎么处理自定义对象数组动态内存分配的。需要注意的是当你执行”delete p”时，你就是在告诉编译器，”p指向了单个对象”。编译器就会调用析构函数一次，亦即对你将要销毁的对象调用析构函数。当你执行”delete[] p”，你相当于告诉编译器”p指向了一系列的对象，但是我没有告诉你有多少”。在这种情况下，编译器就需要生成额外的代码来追踪需要销毁的对象个数。这个额外的信息将会在执行”new[]”时保存在一个”隐蔽的地方”。
示意代码：
```
class MyClass {
public:
    MyClass(); // 构造函数
    ~MyClass(); // 析构函数
};
MyClass *allocate_stuff(int howmany)
{
    // new数组时，数组大小不必为常量值或者常量表达式
    // 在栈上声明数组时则有此要求
    return new MyClass[howmany];
}
```
函数”allocate_stuff”所对应的汇编代码如下所示：
```
push    esi
    mov     esi, [esp+8] ; howmany
   ;  eax = howmany * sizeof(MyClass) + sizeof(size_t)
    lea     eax, [esi*4+4]
    push    eax
    call    operator new
    test    eax, eax
    pop     ecx
    je      fail
    push    edi
    push    OFFSET MyClass::MyClass
    push    esi
    lea     edi, [eax+4] ; edi = eax + sizeof(size_t)
    push    4 ; sizeof(MyClass)
    push    edi
    mov     [eax], esi ; howmany
    call    `vector constructor iterator'
    mov     eax, edi
    pop     edi
    jmp     done
fail:
    xor     eax, eax
done:
    pop     esi
    retd    4
```
如果翻译成伪C++代码，则如下所示：
```cpp
MyClass* allocate_stuff(int howmany)
{
    void* p = operator new(howmany * sizeof(MyClass) +
    sizeof(size_t));
    if (p) {
        size_t* a = reinterpret_cast<size_t*>(p);
            *a++ = howmany;
        vector constructor iterator(a, 
            sizeof(MyClass), &MyClass::MyClass);
        return reinterpret_cast<MyClass*>(a);
    }
    return NULL;
}
```
换句话说，MyClass对象数组的内存模型如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20151108162013019)
new[]运算符返回的指针并不是指向所分配内存的开始地址，而是指向了对象数组的首个对象。对象个数隐藏在了数组前面。
删除该数组时，所执行的操作时相反的：
```java
void free_stuff(MyClass* p)
{
    delete[] p;
}
```
生成的汇编指令：
```
mov     ecx, [esp+4] ; p
    test    ecx, ecx
    je      skip
    push    3
    call    MyClass::`vector deleting destructor`
skip
    ret     4
```
翻译成伪C++代码：
```
void free_stuff(MyClass* p)
{
  if (p) p->vector deleting destructor(3);
}
```
destructor的C++伪代码：
```cpp
void MyClass::vector deleting destructor(int flags)
{
  if (flags & 2) { // if vector destruct
    size_t* a = reinterpret_cast<size_t*>(this) - 1;
    size_t howmany = *a;
    vector destructor iterator(p, sizeof(MyClass),
      howmany, MyClass::~MyClass);
    if (flags & 1) { // if delete too
      operator delete(a);
    }
  } else { // else scalar destruct
    this->~MyClass(); // destruct one
    if (flags & 1) { // if delete too
      operator delete(this);
    }
  }
}
```
数组的销毁函数接收一些标识。如果标识为2，则执行数组销毁；否则销毁单个对象。如果设置为1，则表明内存已经释放。
为了验证上述说法，使用如下代码进行测试：
```cpp
#include <iostream>
using namespace std;
class A
{
public:
    int m_int;
    char m_char;
    A() : m_int(100), m_char(100){}
};
int main()
{
    A* start = new A[10];
    return 0;
}
```
在代码中我们使用”new A[10]”动态分配了10个A对象，个数为10(0x00000010)，占内存”10 * 8 = 80(0x00000050)”个字节。调试该代码，内存如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20151108164719354)
其中红色椭圆所表示的内存是分配动态的首地址，也就是start的值。注意蓝色方框所表示的内存就是一个A对象在内存中的表示，占有8个字节（其中有3个字节的填充，前面已经说过[0xcdcdcdcd](http://blog.csdn.net/dashuniuniu/article/details/49686893)表示未分配的堆内存）。蓝色线条所表示的堆内存分配时的守卫字节。由此我们清楚了在new[]时，在真正的数组内存前面还隐含存储着分配的内存大小（数组所占有的内存，不包括元素个数所占有的内存），数组元素的个数，并且在开始和结尾分别有守卫字节。
***注：operator new在源码中的实现就是一层malloc()函数的包裹，参见[libstdc++-v3/libsupc++/new_op.cc](https://code.woboq.org/gcc/libstdc++-v3/libsupc++/new_op.cc.html) 以及 [libstdc++-v3/libsupc++/del_op.cc](https://code.woboq.org/gcc/libstdc++-v3/libsupc++/del_op.cc.html)。但是new/delete和malloc/free还是有诸多不同的，参见[What is the difference between new/delete and malloc/free?](http://stackoverflow.com/questions/240212/what-is-the-difference-between-new-delete-and-malloc-free)***
