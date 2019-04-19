# Performanced C++ 经验规则（2）：你不知道的构造函数（中） - 文章 - 伯乐在线
原文出处： [张峻崇](http://www.cnblogs.com/ccdev/archive/2012/12/23/2830085.html)
上一篇你不知道的构造函数（上）主要讲述了，C++构造函数在进入构造函数体之前，你可能不知道的一些细节。这一篇将讲述，进入构造函数体后，又发生了什么。
4、虚表初始化
上一篇曾提到，如果一个类有虚函数，那么虚表的初始化工作，无论构造函数是你定义的还是由编译器产生的，这部分工作都将由编译器隐式“合成”到构造函数中，以表示其良苦用心。上一篇还提到，这部分工作，在“刚”进入构造函数的时候，就开始了，之后，编译器才会理会，你构造函数体的第一行代码。这一点，通过反汇编，我们已经看的非常清楚。
虚表初始化的主要内容是：将虚表指针置于对象的首4字节；用该类的虚函数实际地址替换虚表中该同特征标（同名、同参数）函数的地址，以便在调用的时候实现多态，如果有新的虚函数（派生类中新声明的），则依次添加至虚表的后面位置。
5、构造函数中有虚特性（即多态、即动态绑定、晚绑定）产生吗？
这个问题，看似简单，答案却比较复杂，正确答案是：对于构造函数，构造函数中没有虚特性产生（在C++中答案是**NO**，但在Java中，答案是YES，非常的奇葩）。
先从基类构造函数说起，为什么要提基类构造函数呢，因为，派生类总是要调用一个基类的构造函数（无论是显式调用还是由编译器隐式地调用默认构造函数，因为这里讨论的是有虚函数的情况，所以一定会有基类构造函数产生并调用），而此时，在基类构造函数中，派生类对象根本没有创建，也就是说，基类根本不知道派生类中产生了override，即多态，故没有虚特性产生。
这一段非常让人疑惑。让我们再看一小段代码，事实胜于雄辩。

C++
```
#include <iostream>
using namespace std;
class Base
{
public:
        Base() { foo(); }
        virtual void foo(void) { cout << "Base::foo(void)" << endl; }
        virtual void callFoo(void) { foo(); }
};
class Derived : public Base
{
public:
        Derived() { foo(); }
        void foo(void) { cout << "Derived::foo(void)" << endl; }
};
int main(int argc, char** argv)
{
        Base* pB = new Derived;
        pB->callFoo();
        if(pB)
                delete pB;
        return 0;
}
```
在Ubuntu 12.04 + gcc 4.6.3输出结果如下：

C++
```
Base::foo(void)
Derived::foo(void)
Derived::foo(void)
```
这个结果可以很好的解释上述问题，第一行，由于在Base构造函数中，看不到Derived的存在，所以**根本不会产生虚特性**；而第二行，虽然输出了Derived::foo(void)，但因为在派生类直接调用方法名，调用的就是本类的方法，（当然，也可认为在Derived构造函数中，执行foo()前，虚表已经OK，故产生多态，输出的是派生类的行为）。再看第三行，也产生多态，因为，此时，派生类对象已经构建完成，虚表同样也已经OK，所以产生多态是必然。
这个问题其实是C++比较诟病的陷阱问题之一，但我们只要记住结论：**不要在构造函数内调用其它的虚成员函数，否则，当这个类被继承后，在构造函数内调用的这些虚成员函数就没有了虚特性（丧失多态性）**。（非虚成员函数本来就没有多态性，不在此讨论范围）
解决此类问题的方法，是使用“工厂模式”，在后续篇幅中笔者会继续提到，这也是《Effective C++》中阐述的精神：尽可能以工厂方法替换公有构造函数。
另外，有兴趣的同学，可以将上述代码稍加修改成Java跑一跑，你会惊喜的发现，三个输出都是Derived::foo(void)，也就是说，JVM为你提供了一种未卜先知的超自然能力。
6、构造函数中调用构造函数、析构函数
上面已经提到，不要在构造函数内调用其它成员函数，那么调用一些“特殊”的函数，情况又如何呢？我知道，有同学想到了，在构造函数中调用本类的析构函数，情况如何？如下面的代码

C++
```
#include <iostream>
using namespace std;
class A
{
public:
        ~A() { cout << hex << (int)this <<"destructed!" << endl; }
        A() { cout << hex << (int)this << "constructed!" << endl;
                ~A();  }
};
int main(int argc, char** argv)
{
        A a;
        return 0;
}
```
虽然我对有这种想法的同学有强拖之去精神病院的冲动，但还是本着研究精神，把上述“疯子”代码跑一遍，还特地把析构函数的定义提到构造函数之前以防构造函数不认识它。结论是：**构造函数中调用析构函数，编译器拒绝接受~A()是析构函数，从而拒绝这一不讲理行为。**此时编译器认为，你是在重载~操作符，并给出没有找到operator ~()声明的错误提示。其实，无论是在构造函数A()里面调用~A()不行，在成员函数里，也是不行的（编译器仍认为你要调用operator ~()，而你并没有声明这个函数）。但是，有个小诡计，却可以编译通过，就是通过this->~A()来调用析构函数，这将导致对象a被析构多次，隐藏着巨大的安全隐患。
总之，在构造函数中调用析构函数，是十分不道德的行为，应严格禁止。
好了，接下来是，构造函数中，调用构造函数，情况又如何呢？
（1）首先，如果构造函数中递归调用本构造函数，产生无限递归调用，很快就栈溢出（栈上分配）或其它crash，应严格禁止；
（2）如果构造函数中，调用另一个构造函数，情况如何？

C++
```
#include <iostream>
using namespace std;
class ConAndCon
{
public:
    int _i;
    ConAndCon( int i ) : _i(i){}
    ConAndCon()
    {
        ConAndCon(0);
    }
};
int main(int argc, char** argv)
{
    ConAndCon cac;
    cout << cac._i << endl;
    return 0;
}
```
上面代码，输出为0吗？
答案是：不一定。输出结果是不确定的。根据C++类非静态成员是没有默认值的规则，可以推定，上述代码里，**在无参构造函数中调用另一个构造函数，并没有成功完成对成员的初始化工作**，也就是说，这个调用，是不正确的。
那么，由ConAndCon产生的对象哪里去了？如果用gdb跟踪调试或在上述类的构造、析构函数中打印出对象信息就会发现，在构造函数中调用另一个构造函数，会产生一个匿名的临时对象，然后这个对象又被销毁，而调用它的cac对象，仍未得到本意的初始化（设置_i为0）。这也是应严格禁止的。
通常解决此问题的三个方案是：
方案一，我们称为一根筋方案，即，我仍要继续在构造函数中调用另一个构造函数，还要让它正确工作，即“一根筋”，解决思路：不要产生新分配的对象，即在第一个构造函数产生了对象的内存分配之后，仍在此内存上调用另一个构造函数，通过布局new操作符（replacement new）可以做到：

C++
```
//标准库中replacement new操作符的定义：
//需要#include <new>
inline void *__cdecl operator new(size_t, void *_P)
{
    return (_P); 
}
//那么修改ConAndCon()为：
    ConAndCon()
    {
        new (this)ConAndCon(0);
    }
```
即在第一次分配好的内存上再次分配。
某次在Ubuntu 12.04 + gcc 4.6.3运行结果如下（修改后的代码）：

C++
```
#include <iostream>
#include <new>
using namespace std;
class ConAndCon
{
public:
    int _i;
    ConAndCon( int i ) : _i(i){cout << hex << (int)this <<"constructed!" << endl;}
    ConAndCon()
    {
        cout << hex << (int)this <<"constructed!" << endl;
        new (this)ConAndCon(0);
    }
        ~ConAndCon() { cout << hex << (int)this <<"destructed!" << endl; }
};
int main(int argc, char** argv)
{
    ConAndCon cac;
    cout << cac._i << endl;
    return 0;
}
//运行结果：
bfd1ae9cconstructed!
bfd1ae9cconstructed!
0
bfd1ae9cdestructed!
```
可以看到，成功在第一次分配的内存上调用了另一个构造函数，且无需手动为replacement new调用析构函数（此处不同于在申请的buffer上应用replacement new，需要手动调用对象析构函数后，再释放申请的buffer）
方案二，我们称为“AllocAndCall”方案，即构造函数只完成对象的内存分配和调用初始化方法的功能，即把在多个构造函数中都要初始化的部分“提取”出来，通常做为一个private和非虚方法（为什么不能是虚的参见上面第5点），然后在每个构造函数中调用此方法完成初始化。通常，这样的方法取名为init，initialize之类。

C++
```
class AllocAndCall
{
private:
    void initial(...) {...} //初始化集中这里
public:
    AllocAndCall() { initial(); ...}
    AllocAndCall(int x) { initail(); ...}
};
```
这个方案和后面要详述的“工厂模式”，在一些思想上类似。
这个方案最大的不足，是在于，initial()初始化方法不是构造函数而**不能使用初始化列表，对于非静态const成员的初始化将无能为力**。也就是说，如果该类包含非静态的const成员（静态的成员初始化参看上一篇中的第2点），则**对这些非静态const成员的初始化，必须要在每个构造函数的初始化列表完成**，无法“抽取“到初始化方法中。
方案三，我们称为“C++ 0x“方案，这是C++ 0x中的新特性，叫做“委托构造函数”，通过在构造函数的初始化列表（注意不是构造函数体内）中调用其它构造函数，来得到相应目的。感谢C++ 0x！

C++
```
class CPerson
{
public:
 CPerson() : CPerson(0, "") { NULL; }
 CPerson(int nAge) : CPerson(nAge, "") { NULL; }
 CPerson(int nAge, const string &strName)
 {
  stringstream ss;
  ss << strName << "is " << nAge << "years old.";
  m_strInfo = ss.str();
 }
private:
 string m_strInfo;
};
```
其实，对于这样的问题，笔者认为，最好的解决方式，没有在这几种方案中讨论，仍是——使用“工厂模式”，替换公有构造函数。
中篇到此结束，下一篇将会有更多精彩内容——in C++ Constructor！。谢谢大家！
