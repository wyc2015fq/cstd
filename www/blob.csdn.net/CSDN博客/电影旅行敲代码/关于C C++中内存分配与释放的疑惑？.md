# 关于C/C++中内存分配与释放的疑惑？ - 电影旅行敲代码 - CSDN博客
2015年07月26日 11:07:26[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：677标签：[c++基础](https://so.csdn.net/so/search/s.do?q=c++基础&t=blog)
个人分类：[c++基础](https://blog.csdn.net/dashuniuniu/article/category/5682427)
# 关于C/C++中内存分配与释放的疑惑？
## **注：下面的测试有问题：暂时得到的结果是，只要内存地址是对的，无论对象是什么类型，经过多少次转换。delete都会根据堆上的”指导”信息，指导真正的释放过程。指导信息一般是指一些，guard byte和new出来的字节数**
在编写代码过程中，如果需要自己控制变量的生存周期或者栈空间不足以存放我们想要的数据时，我们就只能在堆上分配空间来管理我们的数据。在C/C++中，使用malloc/free和new/delete来创建和释放内存。当然手动管理内存永远是精巧危险但又有效率的做法，会存在内存泄露或者悬空指针的危险。
指针在底层实现时，只是一个4字节的内存地址，附加在指针上的大小其实在于编译器对指针的解释，如果编译器将其解释成int型指针，那么该指针就能在首地址的基础上操控4字节大小的内存，超过4字节编译器就会报错。对于内置类型来说，类型转换其实也是编译器改变了对该指针的解释。
关于C/C++的内存管理我存在以下疑惑：
- **C语言中的类型转换会不会导致内存泄漏？**
- **C++语言中的定位new表达式会不会导致内存泄漏？**
- **C++语言中的new和delete应用在子类和基类场景中会不会导致内存泄漏**
- **猜测实证**
## C语言中的类型转换会不会导致内存泄漏？
- 从malloc的使用方法上猜测应该不会导致内存泄漏，因为malloc返回的指针类型是void*类型，并且free接受的参数也是void*类型。 
示例代码如下：
```cpp
#include <stdlib.h>
int main()
{
    while (true)
    {
        int* ip = (int*)malloc(sizeof(int)* 1024);
        free(ip);
    }
    return 0;
}
```
使用visual studio2013编译该代码，查看任务管理器，内存并没有变化。猜想这是运行库和操作系统共同作用的结果。free函数接受的只是一个4字节的内存地址，当前程序运行的时候，操作系统会创建一个进程并将代码段首地址存入eip寄存器，每个进程自己都有维护一个虚拟地址空间，当然包含一个堆地址空间，这个地址空间是由一系列不连续的内存地址空间构成的，如下图所示： 
![进程虚拟地址空间描述](https://img-blog.csdn.net/20150725171240936)
虚拟地址空间链表结构体存储了相应地址空间的信息，比如说地址空间的首地址及大小，猜测堆分配时有可能使用了这种机制，这段已分配的地址空间中含有收尾及对大小的数据信息。通过首地址即可获得该次内存分配的大小，当然堆分配肯定不止这么简单，有可能借鉴了伙伴算法的思想，来保证不会产生过多的内存碎片。 
## C++语言中的定位new表达式会不会导致内存泄漏？
如果C++的delete关键字是基于C语言中的free机制实现的话，那么按照上述的猜测，delete时，只要首地址正确肯定不会出现内存泄漏，示例代码如下：
```
#include <iostream>
#include <string>
using namespace std;
class A
{
    private:
        int m_num;
        string m_str;
    public:
        A(int num, string str = "hello world") : m_num(num), m_str(str) {}
};
class B
{
    private:
        int m_num;
    public:
        B(int num) : m_num(num){}
};
int main()
{
    while (true)
    {
        A *a = new A(10);
        a->~A();
        B *b = new(a) B(10);
        delete b;
    }
    return 0;
}
```
我们在空闲内存上，重新new了一个新的较小的对象出来，并delete该指针，按照编程语言类型系统的观念，delete释放内存应该会导致部分内存无法释放。如下图所示： 
![定位new](https://img-blog.csdn.net/20150726092009500)
按照指针的含义，指针a所指的对象析构之前，它所能操纵的内存范围只有绿色区域（是通过编译器的类型系统来解释的）。在指针a调用**类型A的析构函数**后，又重新在该内存区域上构造了类型B的对象，并用指针b指向，指针b所能操纵的内存范围只有蓝色区域。**“delete b”**其实包含两方面的含义，一方面是调用类B的析构函数，该析构函数负责的内存区域同样是蓝色区域，另一方面释放指针b指向的内存区域，蓝色区域和下方的白色区域。
delete有两步操作，调用对象的析构函数并释放内存。但是事实证明，**该程序运行时内存并没有变化**，说明该内存已正确释放。
## C++语言中的new和delete应用在子类和基类场景中会不会导致内存泄漏?
众所周知在类层次中，为了保证对象指针能够调用正确的析构函数，最好将析构函数定义为virtual函数。但是如果析构函数不是virtual函数，并且用父类的指针指向子类的对象，结果会怎样？示例代码如下图所示：
```
#include <iostream>
#include <string>
using namespace std;
class A{
public:
    int mem;
    A(int para) : mem(para) {}
};
class B : public A{
private:
    int m_num1;
    int m_num2;
    int m_num3;
    int m_num4;
    string m_str;
public:
    B(int num, int num1, int num2, int num3, int num4, string str)
: A(num), m_num1(num1), m_num2(num2), m_num3(num3), m_num4(num4), m_str(str) {}
};
int main(){
    int i = 0;
    while (true){
        A* b = new B(1, 1, 2, 3, 4, "nihao");
        delete b;
    }
    return 0;
}
```
运行此程序，通过任务管理器发现改程序所占的**内存暴涨**，说明有内存泄漏。当B类里只有内置类型时，是没有内存泄漏的。但是按照上述的解释与猜想，指针b指向类型A，但是操作系统和运行库应该能够保证该指针指向的内存区域能够正确释放。内存如下图所示： 
![父类子类内存区域](https://img-blog.csdn.net/20150726094045845)
子类对象内存区域分为两个部分，一部分是基类部分，一部分是子类扩展的部分，这是子类对象能够赋值给父类对象的内部机制所在。我们首先开辟一段内存，并在内存上构造子类的对象，并用基类类型的指针指向它，即用基类类型来解释这个指针所指向的内存区域，但是**delete指针的时候只是调用基类的析构函数来析构蓝色区域，绿色区域没有调用析构函数来析构这部分**。
难道是因为这个原因导致了绿色区域内存没有被系统所释放？联系上面定位new示例中，对原先所占有的内存区域调用析构函数，虽然用较小指针类型指向了该内存区域，但是最终该内存区域都正确释放了，猜测**自定义类型所申请的动态内存，能否正确释放和所调用的析构函数有关？**
## 猜测实证
经过上述三个示例，我们已经得到如下结论：
- C语言中显示采用类型转换来改变指针类型，并不能引发内存泄漏
- new一个不含有virtual函数的子类对象，将其地址赋值给父类指针有可能会引发内存泄漏
猜测，对于自定义类型，部分内存区域没有调用析构函数，该内存区域不会被释放。如下示例代码所示：
```
##include <iostream>
#include <string>
using namespace std;
class A{
private:
    int m_num;
public:
    A(int num) : m_num(num) {}
};
class B : public A{
private:
    int m_num1;
    int m_num2;
    int m_num3;
    int m_num4;
    string m_str;
public:
    B(int num, int num1, int num2, int num3, int num4, string str)
: A(num), m_num1(num1), m_num2(num2), m_num3(num3), m_num4(num4), m_str(str) {}
};
int main(){
    int i = 0;
    while (true){
        B* b = new B(1, 1, 2, 3, 4, "nihao");
        // 对于子类对象b，没有调用子类析构函数
        A* a = b;
        // 调用基类的析构函数，子类对应的部分内存没有调用析构函数
        delete a;
    }
    return 0;
}
```
指向子类的指针b赋值给父类类型的指针a时，指针b并没有调用子类的析构函数，也就是一部分的内存区域并没有调用析构函数，该示例代码**会造成内存泄漏**。
前面导致内存泄漏的代码都是父类与子类场景下，在其他场景下会怎么样呢？C++里的强制类型转换都只能作用在类层次之间的指针，类之间的指针转换则是不允许。自定义类型之间的指针转换又该如何呢？如下示例代码所示：
```
#include <iostream>
#include <string>
using namespace std;
class A {
private:
    int m_num;
public:
    A(int num) : m_num(num) {}
};
class B {
private:
    int m_num;
    string m_str;
public:
    B(int num, string str): m_num(num), m_str(str)
    {}
};
int main() {
    int i = 0;
    while (true) {
        // A和B是毫无关联的对象
        B* b = new B(1, "nihao");
        void* v = b;
        // 没有调用自定义类型B的析构函数
        A* a = (A*)(v);
        // 调用自定义类型A的析构函数并释放内存
        delete a;
    }
    return 0;
}
```
上述代码经过测试也**会导致内存泄漏**。上述代码中，源类型指针没有调用析构函数，如果调用析构函数会怎样呢？如下示例代码所示：
```
int main() {
    int i = 0;
    while (true) {
        B* b = new B(1, "nihao");
        void* v = b;
        // 调用源类型的析构函数
        b->~B();
        A* a = (A*)(v);
        delete a;
    }
    return 0;
}
```
B调用了析构函数后，该程序并**没有造成内存泄漏**。好像推翻了前面所讲，显示类型转换不会导致内存泄漏的问题。但是是不是由于我们前面的显示类型转换都是想基本类型转换，如果把代码改为如下形式，会不会导致内存泄漏呢？
```
int main() {
    int i = 0;
    while (true) {
        B* b = new B(1, "nihao");
        void* v = b;
        b->~B();
        // 将指针转化为int类型
        int* i = (int*)(v);
        delete i;
    }
    return 0;
}
```
经过测试该代码不会导致内存泄漏。说明内置类型指针之间的显示类型转换不会诱发内存泄漏，但是自定义类型指针之间的显示类型转换有可能导致内存泄漏，**没有显示调用析构函数的那部分内存不会被回收**。
经过上述测试和分析，发现即使用户成对地使用了new和delete还是有可能导致内存泄漏，不同大小的自定义类型指针之间进行类型转换时，没有**显示析构的那部分内存**是不会被回收的。
