# C++的风格与技巧 - gauss的专栏 - CSDN博客
2013年01月08日 13:13:36[gauss](https://me.csdn.net/mathlmx)阅读数：215
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)

C++的风格与技巧
翻译：左轻侯
（译注：本文的翻译相当艰苦。Bjarne Stroustrup不愧是创立C++语言的一代大师，不但思想博大精深，而且在遣词造句上，也非常精微深奥。有很多地方，译者反复斟酌，都不能取得理想的效果，只能尽力而为。
Html格式的文档见译者主页：[http://www.wushuang.net](http://www.wushuang.net)
如果你对这个翻译稿有任何意见和建议，请发信给译者：[onekey@163.com](mailto:onekey@163.com)。
原文的地址为：[http://www.research.att.com/~bs/bs_faq2.html](http://www.research.att.com/~bs/bs_faq2.html)）
（Bjarne Stroustrup博士，1950年出生于丹麦，先后毕业于丹麦阿鲁斯大学和英国剑挢大学，AT&T大规模程序设计研究部门负责人，AT&T 贝尔实验室和ACM成员。1979年，B. S开始开发一种语言，当时称为"C with Class"，后来演化为C++。1998年，ANSI/ISO C++标准建立，同年，B. S推出其经典著作The C++ Programming Language的第三版。）
这是一些人们经常向我问起的有关C++的风格与技巧的问题。如果你能提出更好的问题，或者对这些答案有所建议，请务必发Email给我([bs@research.att.com](mailto:bs@research.att.com))。请记住，我不能把全部的时间都花在更新我的主页上面。
更多的问题请参见我的general FAQ。
关于术语和概念，请参见我的C++术语表（C++ glossary.）。
请注意，这仅仅是一个常见问题与解答的列表。它不能代替一本优秀教科书中那些经过精心挑选的范例与解释。它也不能象一本参考手册或语言标准那样，提供详细和准确的说明。有关C++的设计的问题，请参见《C++语言的设计和演变》（The Design and Evolution of C++）。关于C++语言与标准库的使用，请参见《C++程序设计语言》（The C++ Programming Language）。
目录：
我如何写这个非常简单的程序？
为什么编译要花这么长的时间？
为什么一个空类的大小不为0？
我必须在类声明处赋予数据吗？
为什么成员函数默认不是virtual的？
为什么析构函数默认不是virtual的？
为什么不能有虚拟构造函数？
为什么重载在继承类中不工作？
我能够在构造函数中调用一个虚拟函数吗？
有没有“指定位置删除”(placement delete)？
我能防止别人继承我自己的类吗？
为什么不能为模板参数定义约束（constraints）？
既然已经有了优秀的qsort()函数，为什么还需要一个sort()？
什么是函数对象（function object）？
我应该如何对付内存泄漏？
我为什么在捕获一个异常之后就不能继续？
为什么C++中没有相当于realloc()的函数？
如何使用异常？
怎样从输入中读取一个字符串？
为什么C++不提供“finally”的构造？
什么是自动指针（auto_ptr），为什么没有自动数组（auto_array）？
可以混合使用C风格与C++风格的内存分派与重新分配吗？
我为什么必须使用一个造型来转换*void？
我如何定义一个类内部（in-class）的常量？
为什么delete不会将操作数置0？
我能够写“void main()”吗？
为什么我不能重载点符号，::，sizeof，等等？
怎样将一个整型值转换为一个字符串？
“int* p”正确还是“int *p”正确？
对于我的代码，哪一种布局风格（layout style）是最好的？
我应该将“const”放在类型之前还是之后？
使用宏有什么问题？
我如何写这个非常简单的程序？
特别是在一个学期的开始，我常常收到许多关于编写一个非常简单的程序的询问。这个问题有一个很具代表性的解决方法，那就是（在你的程序中）读入几个数字，对它们做一些处理，再把结果输出。下面是一个这样做的例子：
#include<iostream>
#include<vector>
#include<algorithm>
usingnamespace std;
intmain()
{
vector<double>v;
doubled;
while(cin>>d)v.push_back(d); // 读入元素
if(!cin.eof()) { // 检查输入是否出错
cerr<< "format error/n";
return1; // 返回一个错误
}
cout<< "read " << v.size() << " elements/n";
reverse(v.begin(),v.end());
cout<< "elements in reverse order:/n";
for(int i = 0; i<v.size(); ++i) cout << v[i] << '/n';
return0; // 成功返回
}
对这段程序的观察：
这是一段标准的ISO C++程序，使用了标准库(standard library)。标准库工具在命名空间std中声明，封装在没有.h后缀的头文件中。
如果你要在Windows下编译它，你需要将它编译成一个“控制台程序”（consoleapplication）。记得将源文件加上.cpp后缀，否则编译器可能会以为它是一段C代码而不是C++。
是的，main()函数返回一个int值。
读到一个标准的向量(vector)中，可以避免在随意确定大小的缓冲中溢出的错误。读到一个数组(array)中，而不产生“简单错误”(silly error)，这已经超出了一个新手的能力——如果你做到了，那你已经不是一个新手了。如果你对此表示怀疑，我建议你阅读我的文章“将标准C++作为一种新的语言来学习”("Learning Standard C++as a New Language")，你可以在本人著作列表(my publications list)中下载到它。
!cin.eof()是对流的格式的检查。事实上，它检查循环是否终结于发现一个end-of-file(如果不是这样，那么意味着输入没有按照给定的格式)。更多的说明，请参见你的C++教科书中的“流状态”(stream state)部分。
vector知道它自己的大小，因此我不需要计算元素的数量。
这段程序没有包含显式的内存管理。Vector维护一个内存中的栈，以存放它的元素。当一个vector需要更多的内存时，它会分配一些；当它不再生存时，它会释放内存。于是，使用者不需要再关心vector中元素的内存分配和释放问题。
程序在遇到输入一个“end-of-file”时结束。如果你在UNIX平台下运行它，“end-of-file”等于键盘上的Ctrl+D。如果你在Windows平台下，那么由于一个BUG它无法辨别“end-of-file”字符，你可能倾向于使用下面这个稍稍复杂些的版本，它使用一个词“end”来表示输入已经结束。
#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
usingnamespace std;
intmain()
{
vector<double>v;
doubled;
while(cin>>d)v.push_back(d); // 读入一个元素
if(!cin.eof()) { // 检查输入是否失败
cin.clear(); //清除错误状态
strings;
cin>> s; // 查找结束字符
if(s != "end") {
cerr<< "format error/n";
return1; // 返回错误
}
}
cout<< "read " << v.size() << " elements/n";
reverse(v.begin(),v.end());
cout<< "elements in reverse order:/n";
for(int i = 0; i<v.size(); ++i) cout << v[i] << '/n';
return0; // 成功返回
}
更多的关于使用标准库将事情简化的例子，请参见《C++程序设计语言》中的“漫游标准库”("Tour of the StandardLibrary")一章。
为什么编译要花这么长的时间？
你的编译器可能有问题。也许它太老了，也许你安装它的时候出了错，也许你用的计算机已经是个古董。在诸如此类的问题上，我无法帮助你。
但是，这也是很可能的：你要编译的程序设计得非常糟糕，以至于编译器不得不检查数以百计的头文件和数万行代码。理论上来说，这是可以避免的。如果这是你购买的库的设计问题，你对它无计可施（除了换一个更好的库），但你可以将你自己的代码组织得更好一些，以求得将修改代码后的重新编译工作降到最少。这样的设计会更好，更有可维护性，因为它们展示了更好的概念上的分离。
看看这个典型的面向对象的程序例子：
classShape {
public: //使用Shapes的用户的接口
virtualvoid draw() const;
virtualvoid rotate(int degrees);
//...
protected: //common data (for implementers of Shapes)
Pointcenter;
Colorcol;
//...
};
classCircle : public Shape {
public: 
voiddraw() const;
voidrotate(int) { }
//...
protected:
intradius;
//...
};
classTriangle : public Shape {
public: 
voiddraw() const;
voidrotate(int);
//...
protected:
Pointa, b, c;
//...
}; 
设计思想是，用户通过Shape的public接口来操纵它们，而派生类（例如Circle和Triangle）的实现部分则共享由protected成员表现的那部分实现（implementation）。
这不是一件容易的事情：确定哪些实现部分是对所有的派生类都有用的，并将之共享出来。因此，与public接口相比，protected成员往往要做多得多的改动。举例来说，虽然理论上“中心”(center)对所有的图形都是一个有效的概念，但当你要维护一个三角形的“中心”的时候，是一件非常麻烦的事情——对于三角形，当且仅当它确实被需要的时候，计算这个中心才是有意义的。
protected成员很可能要依赖于实现部分的细节，而Shape的用户（译注：user此处译为用户，指使用Shape类的代码，下同）却不见得必须依赖它们。举例来说，很多（大多数？）使用Shape的代码在逻辑上是与“颜色”无关的，但是由于Shape中“颜色”这个定义的存在，却可能需要一堆复杂的头文件，来结合操作系统的颜色概念。
当protected部分发生了改变时，使用Shape的代码必须重新编译——即使只有派生类的实现部分才能够访问protected成员。
于是，基类中的“实现相关的信息”(informationhelpful to implementers)对用户来说变成了象接口一样敏感的东西，它的存在导致了实现部分的不稳定，用户代码的无谓的重编译（当实现部分发生改变时），以及将头文件无节制地包含进用户代码中（因为“实现相关的信息”需要它们）。有时这被称为“脆弱的基类问题”(brittle baseclass problem)。
一个很明显的解决方案就是，忽略基类中那些象接口一样被使用的“实现相关的信息”。换句话说，使用接口，纯粹的接口。也就是说，用抽象基类的方式来表示接口：
classShape {
public: //使用Shapes的用户的接口
virtualvoid draw() const = 0;
virtualvoid rotate(int degrees) = 0;
virtualPoint center() const = 0;
//...
//没有数据
};
classCircle : public Shape {
public: 
voiddraw() const;
voidrotate(int) { }
Pointcenter() const { return center; }
//...
protected:
Pointcent;
Colorcol;
intradius;
//...
};
classTriangle : public Shape {
public: 
voiddraw() const;
voidrotate(int);
Pointcenter() const;
//...
protected:
Colorcol;
Pointa, b, c;
//...
}; 
现在，用户代码与派生类的实现部分的变化之间的关系被隔离了。我曾经见过这种技术使得编译的时间减少了几个数量级。
但是，如果确实存在着对所有派生类（或仅仅对某些派生类）都有用的公共信息时怎么办呢？可以简单把这些信息封装成类，然后从它派生出实现部分的类：
classShape {
public: //使用Shapes的用户的接口
virtualvoid draw() const = 0;
virtualvoid rotate(int degrees) = 0;
virtualPoint center() const = 0;
//...
//no data
};
structCommon {
Colorcol;
//...
};
classCircle : public Shape, protected Common {
public: 
voiddraw() const;
voidrotate(int) { }
Pointcenter() const { return center; }
//...
protected:
Pointcent;
intradius;
};
classTriangle : public Shape, protected Common {
public: 
voiddraw() const;
voidrotate(int);
Pointcenter() const;
//...
protected:
Pointa, b, c;
}; 
为什么一个空类的大小不为0？
要清楚，两个不同的对象的地址也是不同的。基于同样的理由，new总是返回指向不同对象的指针。
看看：
classEmpty { };
voidf()
{
Emptya, b;
if(&a == &b) cout << "impossible: report error to compilersupplier";
Empty*p1 = new Empty;
Empty*p2 = new Empty;
if(p1 == p2) cout << "impossible: report error to compilersupplier";
} 
有一条有趣的规则：一个空的基类并不一定有分隔字节。 
structX : Empty {
inta;
//...
};
voidf(X* p)
{
void*p1 = p;
void*p2 = &p->a;
if(p1 == p2) cout << "nice: good optimizer";
}
这种优化是允许的，可以被广泛使用。它允许程序员使用空类以表现一些简单的概念。现在有些编译器提供这种“空基类优化”(empty base classoptimization)。
我必须在类声明处赋予数据吗？
不必须。如果一个接口不需要数据时，无须在作为接口定义的类中赋予数据。代之以在派生类中给出它们。参见“为什么编译要花这么长的时间？”。
有时候，你必须在一个类中赋予数据。考虑一下复数类的情况：
template<classScalar> class complex {
public:
complex(): re(0), im(0) { }
complex(Scalarr) : re(r), im(0) { }
complex(Scalarr, Scalar i) : re(r), im(i) { }
//...
complex&operator+=(const complex& a)
{re+=a.re; im+=a.im; return *this; }
//...
private:
Scalarre, im;
};
设计这种类型的目的是将它当做一个内建（built-in）类型一样被使用。在声明处赋值是必须的，以保证如下可能：建立真正的本地对象（genuinely localobjects）(比如那些在栈中而不是在堆中分配的对象)，或者使某些简单操作被适当地inline化。对于那些支持内建的复合类型的语言来说，要获得它们提供的效率，真正的本地对象和inline化都是必要的。
为什么成员函数默认不是virtual的？
因为很多类并不是被设计作为基类的。例如复数类。
而且，一个包含虚拟函数的类的对象，要占用更多的空间以实现虚拟函数调用机制——往往是每个对象占用一个字(word)。这个额外的字是非常可观的，而且在涉及和其它语言的数据的兼容性时，可能导致麻烦(例如C或Fortran语言)。
要了解更多的设计原理，请参见《C++语言的设计和演变》（The Design and Evolution of C++）。
为什么析构函数默认不是virtual的？
因为很多类并不是被设计作为基类的。只有类在行为上是它的派生类的接口时(这些派生类往往在堆中分配，通过指针或引用来访问)，虚拟函数才有意义。
那么什么时候才应该将析构函数定义为虚拟呢？当类至少拥有一个虚拟函数时。拥有虚拟函数意味着一个类是派生类的接口，在这种情况下，一个派生类的对象可能通过一个基类指针来销毁。例如：
classBase {
//...
virtual~Base();
};
classDerived : public Base {
//...
~Derived();
};
voidf()
{
Base*p = new Derived;
deletep; // 虚拟析构函数保证~Derived函数被调用
}
如果基类的析构函数不是虚拟的，那么派生类的析构函数将不会被调用——这可能产生糟糕的结果，例如派生类的资源不会被释放。
为什么不能有虚拟构造函数？
虚拟调用是一种能够在给定信息不完全(given partialinformation)的情况下工作的机制。特别地，虚拟允许我们调用某个函数，对于这个函数，仅仅知道它的接口，而不知道具体的对象类型。但是要建立一个对象，你必须拥有完全的信息。特别地，你需要知道要建立的对象的具体类型。因此，对构造函数的调用不可能是虚拟的。
当要求建立一个对象时，一种间接的技术常常被当作“虚拟构造函数”来使用。有关例子，请参见《C++程序设计语言》第三版15.6.2.节。
下面这个例子展示一种机制：如何使用一个抽象类来建立一个适当类型的对象。
structF { // 对象建立函数的接口
virtualA* make_an_A() const = 0;
virtualB* make_a_B() const = 0;
};
voiduser(const F& fac)
{
A*p = fac.make_an_A(); // 将A作为合适的类型
B*q = fac.make_a_B(); // 将B作为合适的类型
//...
}
structFX : F {
A*make_an_A() const { return new AX(); } // AX是A的派生
B*make_a_B() const { return new BX(); } // AX是B的派生
};
structFY : F {
A*make_an_A() const { return new AY(); } // AY是A的派生
B*make_a_B() const { return new BY(); } // BY是B的派生
};
intmain()
{
user(FX()); //此用户建立AX与BX
user(FY()); //此用户建立AY与BY
//...
}
这是所谓的“工厂模式”(the factory pattern)的一个变形。关键在于，user函数与AX或AY这样的类的信息被完全分离开来了。
为什么重载在继承类中不工作？
这个问题（非常常见）往往出现于这样的例子中：
#include<iostream>
usingnamespace std;
classB {
public:
intf(int i) { cout << "f(int): "; return i+1; }
//...
};
classD : public B {
public:
doublef(double d) { cout << "f(double): "; return d+1.3; }
//...
};
intmain()
{
D*pd = new D;
cout<< pd->f(2) << '/n';
cout<< pd->f(2.3) << '/n';
}
它输出的结果是：
f(double):3.3
f(double):3.6
而不是象有些人猜想的那样：
f(int):3
f(double):3.6
换句话说，在B和D之间并没有发生重载的解析。编译器在D的区域内寻找，找到了一个函数double f(double)，并执行了它。它永远不会涉及（被封装的）B的区域。在C++中，没有跨越区域的重载——对于这条规则，继承类也不例外。更多的细节，参见《C++语言的设计和演变》和《C++程序设计语言》。
但是，如果我需要在基类和继承类之间建立一组重载的f()函数呢？很简单，使用using声明：
classD : public B {
public:
usingB::f; // make every f from B available
doublef(double d) { cout << "f(double): "; return d+1.3; }
//...
};
进行这个修改之后，输出结果将是：
f(int):3
f(double):3.6
这样，在B的f()和D的f()之间，重载确实实现了，并且选择了一个最合适的f()进行调用。
我能够在构造函数中调用一个虚拟函数吗？
可以，但是要小心。它可能不象你期望的那样工作。在构造函数中，虚拟调用机制不起作用，因为继承类的重载还没有发生。对象先从基类被创建，“基类先于继承类(base beforederived)”。
看看这个：
#include<string>
#include<iostream>
usingnamespace std;
classB {
public:
B(conststring& ss) { cout << "B constructor/n"; f(ss); }
virtualvoid f(const string&) { cout << "B::f/n";}
};
classD : public B {
public:
D(conststring & ss) :B(ss) { cout << "D constructor/n";}
voidf(const string& ss) { cout << "D::f/n"; s = ss; }
private:
strings;
};
intmain()
{
Dd("Hello");
}
程序编译以后会输出：
Bconstructor
B::f
Dconstructor
注意不是D::f。设想一下，如果出于不同的规则，B::B()可以调用D::f()的话，会产生什么样的后果：因为构造函数D::D()还没有运行，D::f()将会试图将一个还没有初始化的字符串s赋予它的参数。结果很可能是导致立即崩溃。
析构函数在“继承类先于基类”的机制下运行，因此虚拟机制的行为和构造函数一样：只有本地定义(local definitions)被使用——不会调用虚拟函数，以免触及对象中的（现在已经被销毁的）继承类的部分。
更多的细节，参见《C++语言的设计和演变》13.2.4.2和《C++程序设计语言》15.4.3。
有人暗示，这只是一条实现时的人为制造的规则。不是这样的。事实上，要实现这种不安全的方法倒是非常容易的：在构造函数中直接调用虚拟函数，就象调用其它函数一样。但是，这样就意味着，任何虚拟函数都无法编写了，因为它们需要依靠基类的固定的创建(invariantsestablished by base classes)。这将会导致一片混乱。
有没有“指定位置删除”(placement delete)？
没有，不过如果你需要的话，可以自己写一个。
看看这个指定位置创建(placement new)，它将对象放进了一系列Arena中；
class Arena {
public:
void* allocate(size_t);
void deallocate(void*);
// ...
};
void* operator new(size_t sz, Arena& a)
{
return a.allocate(sz);
}
Arena a1(some arguments);
Arena a2(some arguments);
这样实现了之后，我们就可以这么写：
X* p1 = new(a1) X;
Y* p2 = new(a1) Y;
Z* p3 = new(a2) Z;
// ...
但是，以后怎样正确地销毁这些对象呢？没有对应于这种“placement new”的内建的“placement delete”，原因是，没有一种通用的方法可以保证它被正确地使用。在C++的类型系统中，没有什么东西可以让我们确认，p1一定指向一个由Arena类型的a1分派的对象。p1可能指向任何东西分派的任何一块地方。
然而，有时候程序员是知道的，所以这是一种方法：
template<class T> void destroy(T* p, Arena& a)
{
if (p) {
p->~T(); // explicit destructor call
a.deallocate(p);
}
}
现在我们可以这么写：
destroy(p1,a1);
destroy(p2,a2);
destroy(p3,a3);
如果Arena维护了它保存着的对象的线索，你甚至可以自己写一个析构函数，以避免它发生错误。
这也是可能的：定义一对相互匹配的操作符new()和delete()，以维护《C++程序设计语言》15.6中的类继承体系。参见《C++语言的设计和演变》10.4和《C++程序设计语言》19.4.5。
我能防止别人继承我自己的类吗？
可以，但你为什么要那么做呢？这是两个常见的回答：
效率：避免我的函数被虚拟调用
安全：保证我的类不被用作一个基类（例如，保证我能够复制对象而不用担心出事）
根据我的经验，效率原因往往是不必要的担心。在C++中，虚拟函数调用是如此之快，以致于它们在一个包含虚拟函数的类中被实际使用时，相比普通的函数调用，根本不会产生值得考虑的运行期开支。注意，仅仅通过指针或引用时，才会使用虚拟调用机制。当直接通过对象名字调用一个函数时，虚拟函数调用的开支可以被很容易地优化掉。
如果确实有真正的需要，要将一个类封闭起来以防止虚拟调用，那么可能首先应该问问为什么它们是虚拟的。我看见过一些例子，那些性能表现不佳的函数被设置为虚拟，没有其他原因，仅仅是因为“我们习惯这么干”。
这个问题的另一个部分，由于逻辑上的原因如何防止类被继承，有一个解决方案。不幸的是，这个方案并不完美。它建立在这样一个事实的基础之上，那就是：大多数的继承类必须建立一个虚拟的基类。这是一个例子：
classUsable;
classUsable_lock {
friendclass Usable;
private:
Usable_lock(){}
Usable_lock(constUsable_lock&) {}
};
classUsable : public virtual Usable_lock {
//...
public:
Usable();
Usable(char*);
//...
};
Usablea;
classDD : public Usable { };
DDdd; // 错误: DD::DD() 不能访问
// Usable_lock::Usable_lock()是一个私有成员
(来自《C++语言的设计和演变》11.4.3)
为什么不能为模板参数定义约束（constraints）？
可以的，而且方法非常简单和通用。
看看这个：
template<class Container>
void draw_all(Container& c)
{
for_each(c.begin(),c.end(),mem_fun(&Shape::draw));
}
如果出现类型错误，可能是发生在相当复杂的for_each()调用时。例如，如果容器的元素类型是int，我们将得到一个和for_each()相关的含义模糊的错误(因为不能够对对一个int值调用Shape::draw的方法)。
为了提前捕捉这个错误，我这样写：
template<class Container>
void draw_all(Container& c)
{
Shape* p = c.front(); // accept only containers of Shape*s
for_each(c.begin(),c.end(),mem_fun(&Shape::draw));
}
对于现在的大多数编译器，中间变量p的初始化将会触发一个易于了解的错误。这个窍门在很多语言中都是通用的，而且在所有的标准创建中都必须这样做。在成品的代码中，我也许可以这样写：
template<classContainer>
void draw_all(Container& c)
{
typedef typename Container::value_type T;
Can_copy<T,Shape*>(); // accept containers of only Shape*s
for_each(c.begin(),c.end(),mem_fun(&Shape::draw));
}
这样就很清楚了，我在建立一个断言(assertion)。Can_copy模板可以这样定义：
template<classT1, class T2> struct Can_copy {
staticvoid constraints(T1 a, T2 b) { T2 c = a; b = a; }
Can_copy(){ void(*p)(T1,T2) = constraints; }
};
Can_copy(在运行时)检查T1是否可以被赋值给T2。Can_copy<T,Shape*>检查T是否是Shape*类型，或者是一个指向由Shape类公共继承而来的类的对象的指针，或者是被用户转换到Shape*类型的某个类型。注意这个定义被精简到了最小：
一行命名要检查的约束，和要检查的类型
一行列出指定的要检查的约束(constraints()函数)
一行提供触发检查的方法(通过构造函数)
注意这个定义有相当合理的性质：
你可以表达一个约束，而不用声明或复制变量，因此约束的编写者可以用不着去设想变量如何被初始化，对象是否能够被复制，被销毁，以及诸如此类的事情。(当然，约束要检查这些属性的情况时例外。)
使用现在的编译器，不需要为约束产生代码
定义和使用约束，不需要使用宏
当约束失败时，编译器会给出可接受的错误信息，包括“constraints”这个词（给用户一个线索），约束的名字，以及导致约束失败的详细错误（例如“无法用double*初始化Shape*”）。
那么，在C++语言中，有没有类似于Can_copy——或者更好——的东西呢？在《C++语言的设计和演变》中，对于在C++中实现这种通用约束的困难进行了分析。从那以来，出现了很多方法，来让约束类变得更加容易编写，同时仍然能触发良好的错误信息。例如，我信任我在Can_copy中使用的函数指针的方式，它源自Alex Stepanov和Jeremy Siek。我并不认为Can_copy()已经可以标准化了——它需要更多的使用。同样，在C++社区中，各种不同的约束方式被使用；到底是哪一种约束模板在广泛的使用中被证明是最有效的，还没有达成一致的意见。
但是，这种方式非常普遍，比语言提供的专门用于约束检查的机制更加普遍。无论如何，当我们编写一个模板时，我们拥有了C++提供的最丰富的表达力量。看看这个：
template<classT, class B> struct Derived_from {
staticvoid constraints(T* p) { B* pb = p; }
Derived_from(){ void(*p)(T*) = constraints; }
};
template<classT1, class T2> struct Can_copy {
staticvoid constraints(T1 a, T2 b) { T2 c = a; b = a; }
Can_copy(){ void(*p)(T1,T2) = constraints; }
};
template<classT1, class T2 = T1> struct Can_compare {
staticvoid constraints(T1 a, T2 b) { a==b; a!=b; a<b; }
Can_compare(){ void(*p)(T1,T2) = constraints; }
};
template<classT1, class T2, class T3 = T1> struct Can_multiply {
staticvoid constraints(T1 a, T2 b, T3 c) { c = a*b; }
Can_multiply(){ void(*p)(T1,T2,T3) = constraints; }
};
structB { };
structD : B { };
structDD : D { };
structX { };
intmain()
{
Derived_from<D,B>();
Derived_from<DD,B>();
Derived_from<X,B>();
Derived_from<int,B>();
Derived_from<X,int>();
Can_compare<int,float>();
Can_compare<X,B>();
Can_multiply<int,float>();
Can_multiply<int,float,double>();
Can_multiply<B,X>();
Can_copy<D*,B*>();
Can_copy<D,B*>();
Can_copy<int,B*>();
}
//典型的“元素必须继承自Mybase*”约束:
template<classT> class Container : Derived_from<T,Mybase> {
//...
};
事实上，Derived_from并不检查来源（derivation），而仅仅检查转换（conversion），不过这往往是一个更好的约束。为约束想一个好名字是很难的。
既然已经有了优秀的qsort()函数，为什么还需要一个sort()？
对于初学者来说，
qsort(array,asize,sizeof(elem),elem_compare);
看上去太古怪了，而且比这个更难理解：
sort(vec.begin(),vec.end());
对于专家来说，在元素与比较方式（comparisoncriteria）都相同的情况下，sort()比qsort()更快，这是很重要的。而且，qsort()是通用的，所以它可以用于不同容器类型、元素类型、比较方式的任意有意义的组合。举例来说：
structRecord {
stringname;
//...
};
structname_compare { // 使用"name"作为键比较Record
booloperator()(const Record& a, const Record& b) const
{return a.name<b.name; }
};
voidf(vector<Record>& vs)
{
sort(vs.begin(),vs.end(), name_compare());
//...
} 
而且，很多人欣赏sort()是因为它是类型安全的，使用它不需要进行造型（cast），没有人必须去为基本类型写一个compare()函数。
更多的细节，参见我的文章《将标准C++作为一种新的语言来学习》（Learning C++ as a Newlanguage），可以从我的文章列表中找到。
sort()胜过qsort()的主要原因是，比较操作在内联（inlines）上做得更好。
什么是函数对象（function object）？
顾名思义，就是在某种方式上表现得象一个函数的对象。典型地，它是指一个类的实例，这个类定义了应用操作符operator()。
函数对象是比函数更加通用的概念，因为函数对象可以定义跨越多次调用的可持久的部分（类似静态局部变量），同时又能够从对象的外面进行初始化和检查（和静态局部变量不同）。例如：
classSum {
intval;
public:
Sum(inti) :val(i) { }
operatorint() const { return val; } // 取得值
intoperator()(int i) { return val+=i; } // 应用
};
voidf(vector v)
{
Sums = 0; // initial value 0
s= for_each(v.begin(), v.end(), s); // 求所有元素的和
cout<< "the sum is " << s << "/n";
//或者甚至：
cout<< "the sum is " << for_each(v.begin(), v.end(), Sum(0))<< "/n";
}
注意一个拥有应用操作符的函数对象可以被完美地内联化（inline），因为它没有涉及到任何指针，后者可能导致拒绝优化。与之形成对比的是，现有的优化器几乎不能（或者完全不能？）将一个通过函数指针的调用内联化。
在标准库中，函数对象被广泛地使用以获得弹性。
我应该如何对付内存泄漏？
写出那些不会导致任何内存泄漏的代码。很明显，当你的代码中到处充满了new 操作、delete操作和指针运算的话，你将会在某个地方搞晕了头，导致内存泄漏，指针引用错误，以及诸如此类的问题。这和你如何小心地对待内存分配工作其实完全没有关系：代码的复杂性最终总是会超过你能够付出的时间和努力。于是随后产生了一些成功的技巧，它们依赖于将内存分配（allocations）与重新分配（deallocation）工作隐藏在易于管理的类型之后。标准容器（standardcontainers）是一个优秀的例子。它们不是通过你而是自己为元素管理内存，从而避免了产生糟糕的结果。想象一下，没有string和vector的帮助，写出这个：
#include<vector>
#include<string>
#include<iostream>
#include<algorithm>
usingnamespace std;
intmain() // small program messing around with strings
{
cout<< "enter some whitespace-separated words:/n";
vector<string>v;
strings;
while(cin>>s) v.push_back(s);
sort(v.begin(),v.end());
stringcat;
typedefvector<string>::const_iterator Iter;
for(Iter p = v.begin(); p!=v.end(); ++p) cat += *p+"+";
cout<< cat << '/n';
}
你有多少机会在第一次就得到正确的结果？你又怎么知道你没有导致内存泄漏呢？
注意，没有出现显式的内存管理，宏，造型，溢出检查，显式的长度限制，以及指针。通过使用函数对象和标准算法（standard algorithm），我可以避免使用指针——例如使用迭代子（iterator），不过对于一个这么小的程序来说有点小题大作了。
这些技巧并不完美，要系统化地使用它们也并不总是那么容易。但是，应用它们产生了惊人的差异，而且通过减少显式的内存分配与重新分配的次数，你甚至可以使余下的例子更加容易被跟踪。早在1981年，我就指出，通过将我必须显式地跟踪的对象的数量从几万个减少到几打，为了使程序正确运行而付出的努力从可怕的苦工，变成了应付一些可管理的对象，甚至更加简单了。
如果你的程序还没有包含将显式内存管理减少到最小限度的库，那么要让你程序完成和正确运行的话，最快的途径也许就是先建立一个这样的库。
模板和标准库实现了容器、资源句柄以及诸如此类的东西，更早的使用甚至在多年以前。异常的使用使之更加完善。
如果你实在不能将内存分配/重新分配的操作隐藏到你需要的对象中时，你可以使用资源句柄（resource handle），以将内存泄漏的可能性降至最低。这里有个例子：我需要通过一个函数，在空闲内存中建立一个对象并返回它。这时候可能忘记释放这个对象。毕竟，我们不能说，仅仅关注当这个指针要被释放的时候，谁将负责去做。使用资源句柄，这里用了标准库中的auto_ptr，使需要为之负责的地方变得明确了。
#include<memory>
#include<iostream>
usingnamespace std;
structS {
S(){ cout << "make an S/n"; }
~S(){ cout << "destroy an S/n"; }
S(constS&) { cout << "copy initialize an S/n"; }
S&operator=(const S&) { cout << "copy assign an S/n"; }
};
S*f()
{
returnnew S; // 谁该负责释放这个S？
};
auto_ptr<S>g()
{
returnauto_ptr<S>(new S); // 显式传递负责释放这个S
}
intmain()
{
cout<< "start main/n";
S*p = f();
cout<< "after f() before g()/n";
// S*q = g(); // 将被编译器捕捉
auto_ptr<S>q = g();
cout<< "exit main/n";
//*p产生了内存泄漏
//*q被自动释放
}
在更一般的意义上考虑资源，而不仅仅是内存。
如果在你的环境中不能系统地应用这些技巧（例如，你必须使用别的地方的代码，或者你的程序的另一部分简直是原始人类（译注：原文是Neanderthals，尼安德特人，旧石器时代广泛分布在欧洲的猿人）写的，如此等等），那么注意使用一个内存泄漏检测器作为开发过程的一部分，或者插入一个垃圾收集器（garbage collector）。
我为什么在捕获一个异常之后就不能继续？
换句话说，C++为什么不提供一种简单的方式，让程序能够回到异常抛出点之后，并继续执行？
主要的原因是，如果从异常处理之后继续，那么无法预知掷出点之后的代码如何对待异常处理，是否仅仅继续执行，就象什么也没有发生一样。异常处理者无法知道，在继续之前，有关的上下文环境（context）是否是“正确”的。要让这样的代码正确执行，抛出异常的编写者与捕获异常的编写者必须对彼此的代码与上下文环境都非常熟悉才行。这样会产生非常复杂的依赖性，因此无论在什么情况下，都会导致一系列严重的维护问题。
当我设计C++的异常处理机制时，我曾经认真地考虑过允许这种继续的可能性，而且在标准化的过程中，这个问题被非常详细地讨论过。请参见《C++语言的设计和演变》中的异常处理章节。
在一次新闻组的讨论中，我曾经以一种稍微不同的方式回答过这个问题。
为什么C++中没有相当于realloc()的函数？
如果你需要，你当然可以使用realloc()。但是，realloc()仅仅保证能工作于这样的数组之上：它们被malloc()（或者类似的函数）分配，包含一些没有用户定义的复制构造函数（copy constructors）的对象。而且，要记住，与通常的期望相反，realloc()有时也必须复制它的参数数组。
在C++中，处理内存重新分配的更好的方法是，使用标准库中的容器，例如vector，并让它自我增长。
如何使用异常？
参见《C++程序设计语言》第4章，第8.3节，以及附录E。这个附录针对的是如何在要求苛刻的程序中写出异常安全的代码的技巧，而不是针对初学者的。一个关键的技术是“资源获得即初始化”（resourceacquisiton is initialization），它使用一些有析构函数的类，来实现强制的资源管理。
怎样从输入中读取一个字符串？
你可以用这种方式读取一个单独的以空格结束的词：
#include<iostream>
#include<string>
usingnamespace std;
intmain()
{
cout<< "Please enter a word:/n";
strings;
cin>>s;
cout<< "You entered " << s << '/n';
}
注意，这里没有显式的内存管理，也没有可能导致溢出的固定大小的缓冲区。
如果你确实想得到一行而不是一个单独的词，可以这样做：
#include<iostream>
#include<string>
usingnamespace std;
intmain()
{
cout<< "Please enter a line:/n";
strings;
getline(cin,s);
cout<< "You entered " << s << '/n';
}
在《C++程序设计语言》（可在线获得）的第3章，可以找到一个对诸如字符串与流这样的标准库工具的简介。对于使用C与C++进行简单输入输出的详细比较，参见我的文章《将标准C++作为一种新的语言来学习》(Learning Standard C++ as aNew Language)，你可以在本人著作列表(my publications list)中下载到它。
为什么C++不提供“finally”的构造？
因为C++提供了另外一种方法，它几乎总是更好的：“资源获得即初始化”（resourceacquisiton is initialization）技术。基本的思路是，通过一个局部对象来表现资源，于是局部对象的析构函数将会释放资源。这样，程序员就不会忘记释放资源了。举例来说：
classFile_handle {
FILE*p;
public:
File_handle(constchar* n, const char* a)
{p = fopen(n,a); if (p==0) throw Open_error(errno); }
File_handle(FILE*pp)
{p = pp; if (p==0) throw Open_error(errno); }
~File_handle(){ fclose(p); }
operatorFILE*() { return p; }
//...
};
voidf(const char* fn)
{
File_handlef(fn,"rw"); //打开fn进行读写
//通过f使用文件
}
在一个系统中，需要为每一个资源都使用一个“资源句柄”类。无论如何，我们不需要为每一个资源获得都写出“finally”语句。在实时系统中，资源获得要远远多于资源的种类，因此和使用“finally”构造相比，“资源获得即初始化”技术会产生少得多的代码。
什么是自动指针（auto_ptr），为什么没有自动数组（auto_array）？
auto_ptr是一个非常简单的句柄类的例子，在<memory>中定义，通过“资源获得即初始化”技术支持异常安全。auto_ptr保存着一个指针，能够象指针一样被使用，并在生存期结束时释放指向的对象。举例：
#include<memory>
usingnamespace std;
structX {
intm;
//..
};
voidf()
{
auto_ptr<X>p(new X);
X*q = new X;
p->m++; //象一个指针一样使用p
q->m++;
//...
deleteq;
}
如果在...部分抛出了一个异常，p持有的对象将被auto_ptr的析构函数正确地释放，而q指向的X对象则产生了内存泄漏。更多的细节，参见《C++程序设计语言》14.4.2节。
auto_ptr是一个非常简单的类。特别地，它不是一个引用计数（reference counted）的指针。如果你将一个auto_ptr赋值给另一个，那么被赋值的auto_ptr将持有指针，而原来的auto_ptr将持有0。举例：
#include<memory>
#include<iostream>
usingnamespace std;
structX {
intm;
//..
};
intmain()
{
auto_ptr<X>p(new X);
auto_ptr<X>q(p);
cout<< "p " << p.get() << " q " <<q.get() << "/n";
}
将会打印出一个指向0的指针和一个指向非0的指针。例如：
p0x0 q 0x378d0
auto_ptr::get()返回那个辅助的指针。
这种“转移”语义不同于通常的“复制”语义，这是令人惊讶的。特别地，永远不要使用auto_ptr作为一个标准容器的成员。标准容器需要通常的“复制”语义。例如：
std::vector<auto_ptr<X>>v; // 错误
auto_ptr只持有指向一个单独元素的指针，而不是指向一个数组的指针：
voidf(int n)
{
auto_ptr<X>p(new X[n]); //错误
//...
}
这是错误的，因为析构函数会调用delete而不是delete[]来释放指针，这样就不会调用余下的n-1个X的析构函数。
那么我们需要一个auto_array来持有数组吗？不。没有auto_array。原因是根本没有这种需要。更好的解决方案是使用vector：
voidf(int n)
{
vector<X>v(n);
//...
}
当...部分发生异常时，v的析构函数会被正确地调用。
可以混合使用C风格与C++风格的内存分派与重新分配吗？
在这种意义上是可以的：你可以在同一个程序中使用malloc()和new。
在这种意义上是不行的：你不能使用malloc()来建立一个对象，又通过delete来释放它。你也不能用new建立一个新的对象，然后通过free()来释放它，或者通过realloc()在数组中再建立一个新的。
C++中的new和delete操作可以保证正确的构造和析构：构造函数和析构函数在需要它们的时候被调用。C风格的函数alloc(), calloc(), free(), 和realloc()却不能保证这一点。此外，用new和delete来获得和释放的原始内存，并不一定能保证与malloc()和free()兼容。如果这种混合的风格在你的系统中能够运用，只能说是你走运——暂时的。
如果你觉得需要使用realloc()——或者要做更多——考虑使用标准库中的vector。例如：
//从输入中将词读取到一个字符串vector中
vector<string>words;
strings;
while(cin>>s && s!=".") words.push_back(s);
vector会视需要自动增长。
更多的例子与讨论，参见我的文章《将标准C++作为一种新的语言来学习》(Learning Standard C++ as aNew Language)，你可以在本人著作列表(my publications list)中下载到它。
我为什么必须使用一个造型来转换*void？
在C语言中，你可以隐式地将*void转换为*T。这是不安全的。考虑一下：
#include<stdio.h>
intmain()
{
chari = 0;
charj = 0;
char*p = &i;
void*q = p;
int*pp = q; /* 不安全的，在C中可以，C++不行 */
printf("%d%d/n",i,j);
*pp= -1; /* 覆盖了从i开始的内存 */
printf("%d%d/n",i,j);
}
使用一个并不指向T类型的T*将是一场灾难。因此，在C++中，如果从一个void*得到一个T*，你必须进行显式转换。举例来说，要得到上列程序的这个令人别扭的效果，你可以这样写：
int*pp = (int*)q;
或者使用一个新的类型造型，以使这种没有检查的类型转换操作变得更加清晰：
int*pp = static_cast<int*>(q);
造型被最好地避免了。
在C语言中，这种不安全的转换最常见的应用之一，是将malloc()的结果赋予一个合适的指针。例如：
int*p = malloc(sizeof(int));
在C++中，使用类型安全的new操作符：
int*p = new int;
附带地，new操作符还提供了胜过malloc()的新特性：
new不会偶然分配错误的内存数量；
new会隐式地检查内存耗尽情况，而且
new提供了初始化。
举例： 
typedefstd::complex<double> cmplx;
/*C风格: */
cmplx*p = (cmplx*)malloc(sizeof(int)); /* 错误：类型不正确 */
/*忘记测试p==0 */
if(*p == 7) { /* ... */ } /*糟糕，忘记了初始化*p */
//C++风格:
cmplx*q = new cmplx(1,2); // 如果内存耗尽，将抛出一个bad_alloc异常
if(*q == 7) { /* ... */ }
我如何定义一个类内部（in-class）的常量？
如果你需要一个通过常量表达式来定义的常量，例如数组的范围，你有两种选择： 
classX {
staticconst int c1 = 7;
enum{ c2 = 19 };
charv1[c1];
charv2[c2];
//...
};
乍看起来，c1的声明要更加清晰，但是要注意的是，使用这种类内部的初始化语法的时候，常量必须是被一个常量表达式初始化的整型或枚举类型，而且必须是static和const形式。这是很严重的限制：
classY {
constint c3 = 7; // 错误：不是static
staticint c4 = 7; // 错误：不是const
staticconst float c5 = 7; // 错误：不是整型
};
我倾向使用枚举的方式，因为它更加方便，而且不会诱使我去使用不规范的类内初始化语法。
那么，为什么会存在这种不方便的限制呢？一般来说，类在一个头文件中被声明，而头文件被包含到许多互相调用的单元去。但是，为了避免复杂的编译器规则，C++要求每一个对象只有一个单独的定义。如果C++允许在类内部定义一个和对象一样占据内存的实体的话，这种规则就被破坏了。对于C++在这个设计上的权衡，请参见《C++语言的设计和演变》。
如果你不需要用常量表达式来初始化它，那么可以获得更大的弹性：
classZ {
staticchar* p; // 在定义中初始化
constint i; // 在构造函数中初始化
public:
Z(intii) :i(ii) { }
};
char*Z::p = "hello, there";
你可以获取一个static成员的地址，当且仅当它有一个类外部的定义的时候：
classAE {
//...
public:
staticconst int c6 = 7;
staticconst int c7 = 31;
};
constint AE::c7; // 定义
intf()
{
constint* p1 = &AE::c6; // 错误：c6没有左值
constint* p2 = &AE::c7; // ok
//...
}
为什么delete不会将操作数置0？
考虑一下： 
deletep;
//...
deletep;
如果在...部分没有涉及到p的话，那么第二个“delete p;”将是一个严重的错误，因为C++的实现（译注：原文为a C++ implementation，当指VC++这样的实现了C++标准的具体工具）不能有效地防止这一点（除非通过非正式的预防手段）。既然delete 0从定义上来说是无害的，那么一个简单的解决方案就是，不管在什么地方执行了“delete p;”，随后都执行“p=0;”。但是，C++并不能保证这一点。
一个原因是，delete的操作数并不需要一个左值（lvalue）。考虑一下：
deletep+1;
deletef(x);
在这里，被执行的delete并没有拥有一个可以被赋予0的指针。这些例子可能很少见，但它们的确指出了，为什么保证“任何指向被删除对象的指针都为0”是不可能的。绕过这条“规则”的一个简单的方法是，有两个指针指向同一个对象：
T*p = new T;
T*q = p;
deletep;
deleteq; // 糟糕！
C++显式地允许delete操作将操作数左值置0，而且我曾经希望C++的实现能够做到这一点，但这种思想看来并没有在C++的实现中变得流行。
如果你认为指针置0很重要，考虑使用一个销毁的函数：
template<classT> inline void destroy(T*& p) { delete p; p = 0; }
考虑一下，这也是为什么需要依靠标准库的容器、句柄等等，来将对new和delete的显式调用降到最低限度的另一个原因。
注意，通过引用来传递指针（以允许指针被置0）有一个额外的好处，能防止destroy()在右值上（rvalue）被调用：
int*f();
int*p;
//...
destroy(f()); //错误：应该使用一个非常量（non-const）的引用传递右值
destroy(p+1); //错误：应该使用一个非常量（non-const）的引用传递右值
我能够写“void main()”吗？
这种定义：
voidmain() { /* ... */ }
在C++中从未被允许，在C语言中也是一样。参见ISO C++标准3.6.1[2]或者ISO C标准5.1.2.2.1。规范的实现接受这种方式：
intmain() { /* ... */ }
和
intmain(int argc, char* argv[]) { /* ... */ }
一个规范的实现可能提供许多版本的main()，但它们都必须返回int类型。main()返回的int值，是程序返回一个值给调用它的系统的方式。在那些不具备这种方式的系统中，返回值被忽略了，但这并不使“void main()”在C++或C中成为合法的。即使你的编译器接受了“void main()”，也要避免使用它，否则你将冒着被C和C++程序员视为无知的风险。
在C++中，main()并不需要包含显式的return语句。在这种情况下，返回值是0，表示执行成功。例如：
#include<iostream>
intmain()
{
std::cout<< "This program returns the integer value 0/n";
}
注意，无论是ISO C++还是C99，都不允许在声明中漏掉类型。那就是说，与C89和ARM C++形成对照，当声明中缺少类型时，并不会保证是“int”。于是： 
#include<iostream>
main(){ /* ... */ }
是错误的，因为缺少main()的返回类型。
为什么我不能重载点符号，::，sizeof，等等？
大多数的运算符能够被程序员重载。例外的是：
.(点符号) :: ?: sizeof
并没有什么根本的原因要禁止重载?:。仅仅是因为，我没有发现有哪种特殊的情况需要重载一个三元运算符。注意一个重载了 表达式1？表达式2：表达式3 的函数，不能够保证表达式2：表达式3中只有一个会被执行。
Sizeof不能够被重载是因为内建的操作（built-in operations），诸如对一个指向数组的指针进行增量操作，必须依靠它。考虑一下： 
Xa[10];
X*p = &a[3];
X*q = &a[3];
p++; //p指向a[4]
//那么p的整型值必须比q的整型值大出一个sizeof(X)
所以，sizeof(X)不能由程序员来赋予一个不同的新意义，以免违反基本的语法。
在N::m中，无论N还是m都不是值的表达式；N和m是编译器知道的名字，::执行一个（编译期的）范围解析，而不是表达式求值。你可以想象一下，允许重载x::y的话，x可能是一个对象而不是一个名字空间（namespace）或者一个类，这样就会导致——与原来的表现相反——产生新的语法（允许 表达式1::表达式2）。很明显，这种复杂性不会带来任何好处。
理论上来说，.（点运算符）可以通过使用和->一样的技术来进行重载。但是，这样做会导致一个问题，那就是无法确定操作的是重载了.的对象呢，还是通过.引用的一个对象。例如：
classY {
public:
voidf();
//...
};
classX { // 假设你能重载.
Y*p;
Y&operator.() { return *p; }
voidf();
//...
};
voidg(X& x)
{
x.f(); //X::f还是Y::f还是错误？
}
这个问题能够用几种不同的方法解决。在标准化的时候，哪种方法最好还没有定论。更多的细节，请参见《C++语言的设计和演变》。
怎样将一个整型值转换为一个字符串？
最简单的方法是使用一个字符串流（stringstream）：
#include<iostream>
#include<string>
#include<sstream>
usingnamespace std;
stringitos(int i) // 将int转换成string
{
stringstreams;
s<< i;
returns.str();
}
intmain()
{
inti = 127;
stringss = itos(i);
constchar* p = ss.c_str();
cout<< ss << " " << p << "/n";
}
自然地，这种技术能够将任何使用<<输出的类型转换为字符串。对于字符串流的更多说明，参见《C++程序设计语言》21.5.3节。
“int* p”正确还是“int *p”正确？
二者都是正确的，因为二者在C和C++中都是有效的，而且意义完全一样。就语言的定义与相关的编译器来说，我们还可以说“int*p”或者“int * p”。
在“int* p”和“int *p”之间的选择与正确或错误无关，而只关乎风格与侧重点。C侧重表达式；对声明往往比可能带来的问题考虑得更多。另一方面，C++则非常重视类型。
一个“典型的C程序员”写成“int *p”，并且解释说“*p表示一个什么样的int”以强调语法，而且可能指出C（与C++）的语法来证明这种风格的正确性。是的，在语法上*被绑定到名字p上。
一个“典型的C++程序员”写成“int* p”，并且解释说“p是一个指向int的指针类型”以强调类型。是的，p是一个指向int的指针类型。我明确地倾向于这种侧重方向，而且认为对于学好更多的高级C++这是很重要的。
严重的混乱（仅仅）发生在当人们试图在一条声明中声明几个指针的时候：
int*p, p1; // 也许是错的：p1不是一个int*
把*放到名字这一边，看来也不能有效地减少这种错误：
int*p, p1; // 也许是错的？
为每一个名字写一条声明最大程度地解决了问题——特别是当我们初始化变量的时候。人们几乎不会这样写： 
int*p = &i;
intp1 = p; // 错误：int用一个int*初始化了
如果他们真的这么干了，编译器也会指出。
每当事情可以有两种方法完成，有人就会迷惑。每当事情仅仅是一个风格的问题，争论就会没完没了。为每一个指针写一条声明，而且永远都要初始化变量，这样，混乱之源就消失了。更多的关于C的声明语法的讨论，参见《C++语言的设计和演变》。
对于我的代码，哪一种布局风格（layout style）是最好的？
这种风格问题属于个人的爱好。人们往往对布局风格的问题持有强烈的意见，不过，也许一贯性比某种特定的风格更加重要。象大多数人一样，我花了很长的时间，来为我的偏好作出一个固定的结论。
我个人使用通常称为“K&R”的风格。当使用C语言没有的构造函数时，需要增加新的习惯，这样就变成了一种有时被称为“Stroustrup”的风格。例如： 
classC : public B {
public:
//...
};
voidf(int* p, int max)
{
if(p) {
//...
}
for(int i = 0; i<max; ++i) {
//...
}
}
比大多数布局风格更好，这种风格保留了垂直的空格，我喜欢尽可能地在合理的情况下对齐屏幕。对函数开头的大括弧的放置，有助于我第一眼就分别出类的定义和函数的定义。
缩进是非常重要的。
设计问题，诸如作为主要接口的抽象基类的使用，使用模板以表现有弹性的类型安全的抽象，以及正确地使用异常以表现错误，比布局风格的选择要重要得多。
我应该将“const”放在类型之前还是之后？
我把它放在前面，但那仅仅是个人爱好问题。“const T”和“T const”总是都被允许的，而且是等效的。例如：
constint a = 1; // ok
intconst b = 2; // also ok
我猜想第一种版本可能会让少数（更加固守语法规范）的程序员感到迷惑。
为什么？当我发明“const”（最初的名称叫做“readonly”，并且有一个对应的“writeonly”）的时候，我就允许它出现在类型之前或之后，因为这样做不会带来任何不明确。标准之前的C和C++规定了很少的（如果有的话）特定的顺序规范。
我不记得当时有过任何有关顺序问题的深入思考或讨论。那时，早期的一些使用者——特别是我——仅仅喜欢这种样子：
constint c = 10;
看起来比这种更好：
intconst c = 10;
也许我也受了这种影响：在我最早的一些使用“readonly”的例子中
readonlyint c = 10;
比这个更具有可读性：
intreadonly c = 10;
我创造的那些最早的使用“const”的（C或C++）代码，看来已经在全球范围内取代了“readonly”。
我记得这个语法的选择在几个人——例如Dennis Ritchie——当中讨论过，但我不记得当时我倾向于哪种语言了。
注意在固定指针（const pointer）中，“const”永远出现在“*”之后。例如：
int*const p1 = q; // 指向int变量的固定指针
intconst* p2 = q; //指向int常量的指针
constint* p3 = q; //指向int常量的指针
使用宏有什么问题？
宏不遵循C++中关于范围和类型的规则。这经常导致一些微妙的或不那么微妙的问题。因此，C++提供更适合其他的C++（译注：原文为the rest of C++，当指C++除了兼容C以外的部分）的替代品，例如内联函数、模板与名字空间。
考虑一下：
#include"someheader.h"
structS {
intalpha;
intbeta;
};
如果某人（不明智地）地写了一个叫“alpha”或“beta”的宏，那么它将不会被编译，或者被错误地编译，产生不可预知的结果。例如，“someheader.h”可能包含：
#definealpha 'a'
#definebeta b[2]
将宏（而且仅仅是宏）全部大写的习惯，会有所帮助，但是对于宏并没有语言层次上的保护机制。例如，虽然成员的名字包含在结构体的内部，但这无济于事：在编译器能够正确地辨别这一点之前，宏已经将程序作为一个字符流进行了处理。顺便说一句，这是C和C++程序开发环境和工具能够被简化的一个主要原因：人与编译器看到的是不同的东西。
不幸的是，你不能假设别的程序员总是能够避免这种你认为“相当白痴”的事情。例如，最近有人报告我，他们遇到了一个包含goto的宏。我也见过这种情况，而且听到过一些——在很脆弱的时候——看起来确实有理的意见。例如：
#defineprefix get_ready(); int ret__
#defineReturn(i) ret__=i; do_something(); goto exit
#definesuffix exit: cleanup(); return ret__
voidf()
{
prefix;
//...
Return(10);
//...
Return(x++);
//...
suffix;
}
作为一个维护的程序员，就会产生这种印象；将宏“隐藏”到一个头文件中——这并不罕见——使得这种“魔法”更难以被辨别。
一个常见的微妙问题是，一个函数风格的宏并不遵守函数参数传递的规则。例如： 
#definesquare(x) (x*x)
voidf(double d, int i)
{
square(d); //好
square(i++); //糟糕：这表示 (i++*i++)
square(d+1); //糟糕：这表示(d+1*d+1); 也就是 (d+d+1)
//...
}
“d+1”的问题，可以通过在“调用”时或宏定义时添加一对圆括号来解决：
#definesquare(x) ((x)*(x)) /*这样更好 */
但是， i++被执行了两次（可能并不是有意要这么做）的问题仍然存在。
是的，我确实知道有些特殊的宏并不会导致C/C++预处理宏这样的问题。但是，我无心去发展C++中的宏。作为替代，我推荐使用C++语言中合适的工具，例如内联函数，模板，构造函数（用来初始化），析构函数（用来清除），异常（用来退出上下文环境），等等。
