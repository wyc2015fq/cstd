# 【温故知新C/C++】vector||max ||virtual(C++) - 三少GG - CSDN博客
2011年04月24日 16:21:00[三少GG](https://me.csdn.net/scut1135)阅读数：1210标签：[vector																[c++																[iostream																[class																[iterator																[编译器](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=iostream&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=vector&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
1.
vector< vector<int> > arr;
vector<int> v;
//v里面可以放你想要的数据。
arr.push(v);
2.
vector<int> arr[100]。
//这就开了一个大小为100的vector<Int>的数据。
//同样是二维的效果。 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++有些数据结构（如vector）可以动态增长！！！
C++中有些函数可以声明为内联(inline)函数，编译器遇到内联函数时就会直接扩展相应代码，而不是进行实际的函数调用。想size这样的小库函数几乎都定义为内联函数，所以每次循环过程中调用它的运行时代价是比较小的。
vector的下标操作
类似于string类型的下标操作，通常使用下标操作来获取元素。
vector元素的位置从0开始，下标操作接受一个值，并返回对应位置的元素。（警告：必须是已存在的元素才能用下标操作符进行索引）
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
官方MSDN说明： [http://msdn.microsoft.com/en-us/library/b6ezyw32.aspx](http://msdn.microsoft.com/en-us/library/b6ezyw32.aspx)
Illustrates how to use the [vector::empty](http://msdn.microsoft.com/en-us/library/86968s02.aspx), [vector::erase](http://msdn.microsoft.com/en-us/library/ceh559x2.aspx), and [vector::push_back](http://msdn.microsoft.com/en-us/library/7fthz5xd.aspx) STL functions in Visual C++.
// Empty.cpp
// compile with: /EHsc
// Illustrates the vector::empty and vector::erase functions.
// Also demonstrates the vector::push_back function.
//
// Functions:
//
//    **vector::empty - Returns true if vector has no elements.**//
//    vector::erase - Deletes elements from a vector (single & range).
//
//    vector::begin - Returns an iterator to start traversal of the
//                    vector.
//
//    vector::end - Returns an iterator for the last element of the
//                  vector.
//
//    vector::push_back - Appends (inserts) an element to the end of a
//                        vector, allocating memory for it if necessary.
//
//    vector::iterator - Traverses the vector.
//
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://blog.csdn.net/masterlee/archive/2004/11/09/174129.aspx](http://blog.csdn.net/masterlee/archive/2004/11/09/174129.aspx)STL vector 容器介绍A Presentation of the STL Vector Container (By Nitron)翻译 masterlee介绍std::vector，并且讨论它在STL中的算法和条件函数remove_if()。Download Console Demo - 6.19 Kb Download MFC Demo - 14.6 Kb 介绍这篇文章的目的是为了介绍std::vector，如何恰当地使用它们的成员函数等操作。本文中还讨论了条件函数和函数指针在迭代算法中使用，如在remove_if()和for_each()中的使用。通过阅读这篇文章读者应该能够有效地使用vector容器，而且应该不会再去使用C类型的动态数组了。Vector总览vector是C++标准模板库中的部分内容，它是一个多功能的，能够操作多种数据结构和算法的模板类和函数库。vector之所以被认为是一个容器，是因为它能够像容器一样存放各种类型的对象，简单地说，vector是一个能够存放任意类型的动态数组，能够增加和压缩数据。为了可以使用vector，必须在你的头文件中包含下面的代码：#include <vector>
vector属于std命名域的，因此需要通过命名限定，如下完成你的代码：using std::vector;vector<int> vInts;
或者连在一起，使用全名：std::vector<int> vInts;
建议使用全局的命名域方式：using namespace std;
在后面的操作中全局的命名域方式会造成一些问题。vector容器提供了很多接口，在下面的表中列出vector的成员函数和操作。Vector成员函数函数
 表述
c.assign(beg,end)c.assign(n,elem)
 将[beg; end)区间中的数据赋值给c。将n个elem的拷贝赋值给c。
c.at(idx)
 传回索引idx所指的数据，如果idx越界，抛出out_of_range。
c.back()
 传回最后一个数据，不检查这个数据是否存在。
c.begin()
 传回迭代器重的可一个数据。
c.capacity()
 返回容器中数据个数。
c.clear()
 移除容器中所有数据。
c.empty()
 判断容器是否为空。
c.end()
 指向迭代器中的最后一个数据地址。
c.erase(pos)c.erase(beg,end)
 删除pos位置的数据，传回下一个数据的位置。删除[beg,end)区间的数据，传回下一个数据的位置。
c.front()
 传回地一个数据。
get_allocator
 使用构造函数返回一个拷贝。
c.insert(pos,elem) c.insert(pos,n,elem)c.insert(pos,beg,end)
 在pos位置插入一个elem拷贝，传回新数据位置。在pos位置插入n个elem数据。无返回值。在pos位置插入在[beg,end)区间的数据。无返回值。
c.max_size()
 返回容器中最大数据的数量。
c.pop_back()
 删除最后一个数据。
c.push_back(elem)
 在尾部加入一个数据。
c.rbegin()
 传回一个逆向队列的第一个数据。
c.rend()
 传回一个逆向队列的最后一个数据的下一个位置。
c.resize(num)
 重新指定队列的长度。
c.reserve()
 保留适当的容量。
c.size()
 返回容器中实际数据的个数。
c1.swap(c2) swap(c1,c2)
 将c1和c2元素互换。同上操作。
vector<Elem> cvector <Elem> c1(c2)vector <Elem> c(n)vector <Elem> c(n, elem)vector <Elem> c(beg,end)c.~ vector <Elem>()
 创建一个空的vector。复制一个vector。创建一个vector，含有n个数据，数据均已缺省构造产生。创建一个含有n个elem拷贝的vector。创建一个以[beg;end)区间的vector。销毁所有数据，释放内存。
Vector操作函数
 描述
operator[] 
 返回容器中指定位置的一个引用。
创建一个vectorvector容器提供了多种创建方法，下面介绍几种常用的。创建一个Widget类型的空的vector对象：vector<Widget> vWidgets;//     ------//      |//      |- Since vector is a container, its member functions//         operate on iterators and the container itself so //         it can hold objects of any type.
创建一个包含500个Widget类型数据的vector：vector<Widget> vWidgets(500);
创建一个包含500个Widget类型数据的vector，并且都初始化为0：vector<Widget> vWidgets(500, Widget(0));
创建一个Widget的拷贝：vector<Widget> vWidgetsFromAnother(vWidgets);
向vector添加一个数据vector添加数据的缺省方法是push_back()。push_back()函数表示将数据添加到vector的尾部，并按需要来分配内存。例如：向vector<Widget>中添加10个数据，需要如下编写代码：for(int i= 0;i<10; i++)    vWidgets.push_back(Widget(i));
获取vector中制定位置的数据很多时候我们不必要知道vector里面有多少数据，vector里面的数据是动态分配的，使用push_back()的一系列分配空间常常决定于文件或一些数据源。如果你想知道vector存放了多少数据，你可以使用empty()。获取vector的大小，可以使用size()。例如，如果你想获取一个vector v的大小，但不知道它是否为空，或者已经包含了数据，如果为空想设置为-1，你可以使用下面的代码实现：int nSize = v.empty() ? -1 : static_cast<int>(v.size());
访问vector中的数据使用两种方法来访问vector。1、   vector::at()2、   vector::operator[]operator[]主要是为了与C语言进行兼容。它可以像C语言数组一样操作。但at()是我们的首选，因为at()进行了边界检查，如果访问超过了vector的范围，将抛出一个例外。由于operator[]容易造成一些错误，所有我们很少用它，下面进行验证一下：分析下面的代码：vector<int> v;v.reserve(10);for(int i=0; i<7; i++)    v.push_back(i);try{ int iVal1 = v[7];  // not bounds checked - will not throw int iVal2 = v.at(7); // bounds checked - will throw if out of range}catch(const exception& e){ cout << e.what();}
我们使用reserve()分配了10个int型的空间，但并不没有初始化。如下图所示：你可以在这个代码中尝试不同条件，观察它的结果，但是无论何时使用at()，都是正确的。删除vector中的数据vector能够非常容易地添加数据，也能很方便地取出数据，同样vector提供了erase()，pop_back()，clear()来删除数据，当你删除数据的时候，你应该知道要删除尾部的数据，或者是删除所有数据，还是个别的数据。在考虑删除等操作之前让我们静下来考虑一下在STL中的一些应用。 Remove_if()算法现在我们考虑操作里面的数据。如果要使用remove_if()，我们需要在头文件中包含如下代码：：#include <algorithm>
         Remove_if()有三个参数：1、   iterator _First：指向第一个数据的迭代指针。2、   iterator _Last：指向最后一个数据的迭代指针。3、   predicate _Pred：一个可以对迭代操作的条件函数。条件函数条件函数是一个按照用户定义的条件返回是或否的结果，是最基本的函数指针，或者是一个函数对象。这个函数对象需要支持所有的函数调用操作，重载operator()()操作。remove_if()是通过unary_function继承下来的，允许传递数据作为条件。例如，假如你想从一个vector<CString>中删除匹配的数据，如果字串中包含了一个值，从这个值开始，从这个值结束。首先你应该建立一个数据结构来包含这些数据，类似代码如下：#include <functional>enum findmodes { FM_INVALID = 0, FM_IS, FM_STARTSWITH, FM_ENDSWITH, FM_CONTAINS};typedef struct tagFindStr{ UINT iMode; CString szMatchStr;} FindStr;typedef FindStr* LPFINDSTR;
然后处理条件判断：class FindMatchingString     : public std::unary_function<CString, bool> {public:   FindMatchingString(const LPFINDSTR lpFS) : m_lpFS(lpFS) {}   bool operator()(CString& szStringToCompare) const   {      bool retVal = false;     switch(m_lpFS->iMode)      {     case FM_IS:        {          retVal = (szStringToCompare == m_lpFDD->szMatchStr);          break;       }     case FM_STARTSWITH:        {          retVal = (szStringToCompare.Left(m_lpFDD->szMatchStr.GetLength())               == m_lpFDD->szWindowTitle);         break;       }      case FM_ENDSWITH:        {         retVal = (szStringToCompare.Right(m_lpFDD->szMatchStr.GetLength())                == m_lpFDD->szMatchStr);         break;        }      case FM_CONTAINS:        {         retVal = (szStringToCompare.Find(m_lpFDD->szMatchStr) != -1);         break;        }     }     return retVal; }private:     LPFINDSTR m_lpFS;};
通过这个操作你可以从vector中有效地删除数据：// remove all strings containing the value of // szRemove from vector<CString> vs.FindStr fs;fs.iMode = FM_CONTAINS;fs.szMatchStr = szRemove;vs.erase(std::remove_if(vs.begin(), vs.end(), FindMatchingString(&fs)), vs.end());
Remove_if()能做什么？你可能会疑惑，对于上面那个例子在调用remove_if()的时候还要使用erase()呢？这是因为大家并不熟悉STL中的算法。Remove(),remove_if()等所有的移出操作都是建立在一个迭代范围上的，那么不能操作容器中的数据。所以在使用remove_if()，实际上操作的时容器里数据的上面的。思考上面的例子：1、   szRemove = “o”.2、   vs见下面图表中的显示。
观察这个结果，我们可以看到remove_if()实际上是根据条件对迭代地址进行了修改，在数据的后面存在一些残余的数据，那些需要删除的数据。剩下的数据的位置可能不是原来的数据，但他们是不知道的。调用erase()来删除那些残余的数据。注意上面例子中通过erase()删除remove_if()的结果和vs.enc()范围的数据。压缩一个臃肿的vector很多时候大量的删除数据，或者通过使用reserve()，结果vector的空间远远大于实际需要的。所有需要压缩vector到它实际的大小。resize()能够增加vector的大小。Clear()仅仅能够改变缓存的大小，所有的这些对于vector释放内存等九非常重要了。如何来解决这些问题呢，让我们来操作一下。我们可以通过一个vector创建另一个vector。让我们看看这将发生什么。假定我们已经有一个vector v，它的内存大小为1000，当我们调用size()的时候，它的大小仅为7。我们浪费了大量的内存。让我们在它的基础上创建一个vector。std::vector<CString> vNew(v);cout << vNew.capacity();
vNew.capacity()返回的是7。这说明新创建的只是根据实际大小来分配的空间。现在我们不想释放v，因为我们要在其它地方用到它，我们可以使用swap()将v和vNew互相交换一下？    vNew.swap(v);    cout << vNew.capacity();    cout << v.capacity();
有趣的是：vNew.capacity()是1000，而v.capacity()是7。现在是达到我的目的了，但是并不是很好的解决方法，我们可以像下面这么写：    std::vector<CString>(v).swap(v);
你可以看到我们做了什么？我们创建了一个临时变量代替那个命名的，然后使用swap(),这样我们就去掉了不必要的空间，得到实际大小的v。结论我希望这个文档可以给那些使用STL vector容器的开发者很有价值的参考。我也希望通过阅读这篇文章你可以放心地使用vector来代替C语言中的数据了。参考Plauger, P.J. Standard C++ Library Reference. February, 2003. MSDN. Schildt, Herbert. C++ from the Ground Up, Second Edition. Berkeley: 1998. Sutter, Herb. More Exceptional C++. Indianapolis: 2002.    郑重声明：
                 允许复制、修改、传递或其它行为
                 但不准用于任何商业用途.
                  写于  2004/11/9  masterlee深入研究 STL Deque 容器本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/masterlee/archive/2004/11/09/174129.aspx](http://blog.csdn.net/masterlee/archive/2004/11/09/174129.aspx)++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++[http://blog.csdn.net/kiwigiving/archive/2009/08/22/4472578.aspx](http://blog.csdn.net/kiwigiving/archive/2009/08/22/4472578.aspx) 原来std命名空间中使用了max变量啊今天在CSDN看了一个贴子，为什么定**义的全局max会被认为是ambiguous symbol，即模棱两可的变量呢？突然发现，哦……原来在std命名空间中也用了max变量了啊**！程序如下：#include "iostream"
using namespace std;const int max = 10;
 int main()
{
 int a[max]={0};
 for (int i = 0;i <max; i++)
        a[i]=i;
 for (int i=0; i<max;i++)
       cout << a[i];
       cout << endl;
}这里的max被定义成了全局变量，所以这个max是全局可见的。而且这里把std命名空间全体都引入了程序，但是在std命名空间中也用到了max这个变量，所以定义的全局max和std中的max就产生了冲突,编译产生了错误：ambiguous symbol，即模棱两可的符号。对于这种问题，在不想改变定义的max标志符的情况下，修改方法有两种：1、不要把std整个命名空间都引入程序。这个程序中只用到了std命名空间中的cout和endl，所以我们可以把开头的using namespace std;改成：using std::cout; using std::endl; 程序如下：
#include "iostream"
**using std::cout;using std::endl;**int main()
{
 int a[max]={0};
 for (int i = 0;i <max; i++)
        a[i]=i;
 for (int i=0; i<max;i++)
       cout << a[i];
       cout << endl;
} 2、我们可以把我们定义的全局max，定义在我们自己的命名空间中，这样就不会与std的max冲突了。
namespace m
{
 const int max = 10;
}
 这样就把我们定义的max就属于我们定义的命名空间m了。使用这个max时，要记得m::max这样的形式。程序如下：
#include "iostream"
using namespace std;
namespace m
{
const int max = 10;
}
 int main()
{
 int a[m::max]={0};
 for (int i = 0;i <m::max; i++)
        a[i]=i;
 for (int i=0; i<m::max;i++)
       cout << a[i];
       cout << endl;
}本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/kiwigiving/archive/2009/08/22/4472578.aspx](http://blog.csdn.net/kiwigiving/archive/2009/08/22/4472578.aspx)
**++++++++++++++++++++++++++Virtual++++++++++++++++++++++++++++++++++++++++++++**
**1.先去msdn查阅相关用法：**
[**http://msdn.microsoft.com/zh-cn/library/0y01k918**](http://msdn.microsoft.com/zh-cn/library/0y01k918)
2.**今天看一本书上说：作为一条经验，要把所有的方法都用virtual声明（包括析构函数，不包括构造函数），来避免因遗漏关键字virtual而产生的相关的问题。**这样全部用virtual声明有什么影响？比如对性能什么的。。。。。
在需要使用类似Init的时候如果你去virtual反而容易出乱子。
不过如果析构不用virtual，乱子更大。
坏处就是1L说的，增加了开销，因为使用virtual意味着一个virtual function table的形成，这张表本身就是一些函数指针，消耗还好，但是每次访问virtual函数都会多一次的指针跳转，因为需要在vft中进行查找，再跳转。
（一般编译器优化后也就多一次跳转，查找都省掉了。）慢也就慢那一下，如果这个消耗你觉得吃不消，那还是用纯C好了。
如果只知道virtual加在函数前，那对virtual只了解了一半，virtual还有一个重要用法是virtual public，就是虚拟继承。虚拟继承在C++ Primer中有详细的描述，下面稍作修改的阐释一下：
在缺省情况下C++中的继承是“按值组合”的一种特殊情况。当我们写
class Bear : public ZooAnimal { ... };
每个Bear 类对象都含有其ZooAnimal 基类子对象的所有非静态数据成员以及在Bear中声明的非静态数据成员类似地当派生类自己也作为一个基类对象时如：
class PolarBear : public Bear { ... };
则PolarBear 类对象含有在PolarBear 中声明的所有非静态数据成员以及其Bear 子对象的所有非静态数据成员和ZooAnimal 子对象的所有非静态数据成员。在单继承下这种由继承支持的特殊形式的按值组合提供了最有效的最紧凑的对象表示。在多继承下当一个基类在派生层次中出现多次时就会有问题最主要的实际例子是iostream 类层次结构。ostream 和istream 类都从抽象ios 基类派生而来，而iostream 类又是从ostream 和istream 派生
class iostream :public istream, public ostream { ... };
缺省情况下，每个iostream 类对象含有两个ios 子对象：在istream 子对象中的实例以及在ostream 子对象中的实例。这为什么不好？从效率上而言，存储ios 子对象的两个复本，浪费了存储区，因为iostream 只需要一个实例。而且，ios 构造函数被调用了两次每个子对象一次。更严重的问题是由于两个实例引起的二义性。例如，任何未限定修饰地访问ios 的成员都将导致编译时刻错误：到底访问哪个实例？如果ostream 和istream 对其ios 子对象的初始化稍稍不同，会怎样呢？怎样通过iostream 类保证这一对ios 值的一致性？在缺省的按值组合机制下，真的没有好办法可以保证这一点。
C++语言的解决方案是，提供另一种可替代按“引用组合”的继承机制虚拟继承（virtual inheritance ）在虚拟继承下只有一个共享的基类子对象被继承而无论该基类在派生层次
中出现多少次共享的基类子对象被称为虚拟基类。
       通过用关键字virtual 修政一个基类的声明可以将它指定为被虚拟派生。例如，下列声明使得ZooAnimal 成为Bear 和Raccoon 的虚拟基类：
// 关键字 public 和 virtual
// 的顺序不重要
class Bear : public virtual ZooAnimal { ... };
class Raccoon : virtual public ZooAnimal { ... };
虚拟派生不是基类本身的一个显式特性，而是它与派生类的关系如前面所说明的，虚拟继承提供了“按引用组合”。也就是说，对于子对象及其非静态成员的访问是间接进行的。这使得在多继承情况下，把多个虚拟基类子对象组合成派生类中的一个共享实例，从而提供了必要的灵活性。同时，即使一个基类是虚拟的，我们仍然可以通过该基类类型的指针或引用，来操纵派生类的对象。
c++是一门面向对象的语言，但是它和c#，java不同，它没有反射机制。没有反射机制使得c++在语言的一些设计方面与其他语言有点不一样，主要体现在智能化方面，许多东西得程序员明确指定，例如本文要讲的virtual关键字。virtual是在运行时才体现的，而c++在运行时无法使用反射来确定当前类的父类是否有此方法，此方法是否被重载等信息，所以必须在写代码时用virtual来明确指明，然后通过编译器做一些特殊处理（也就是使用虚表）。
我们见到virtual最多的地方是在c++里面的多态实现。
view plaincopy to clipboardprint?
// dynamic_poly.h   
#include <iostream>   
// 公共抽象基类Vehicle   
class Vehicle   
{   
public:   
    virtual void run() const = 0;   
};   
// 派生于Vehicle的具体类Car   
class Car: public Vehicle   
{   
public:   
    virtual void run() const  
    {   
        std::cout << "run a car/n";   
    }   
};   
// 派生于Vehicle的具体类Airplane   
class Airplane: public Vehicle   
{   
public:   
    virtual void run() const  
    {   
        std::cout << "run a airplane/n";   
    }   
};   
// dynamic_poly.h
#include <iostream>
// 公共抽象基类Vehicle
class Vehicle
{
public:
    virtual void run() const = 0;
};
// 派生于Vehicle的具体类Car
class Car: public Vehicle
{
public:
    virtual void run() const
    {
        std::cout << "run a car/n";
    }
};
// 派生于Vehicle的具体类Airplane
class Airplane: public Vehicle
{
public:
    virtual void run() const
    {
        std::cout << "run a airplane/n";
    }
};  
view plaincopy to clipboardprint?
// dynamic_poly_1.cpp   
#include <iostream>   
#include <vector>   
#include "dynamic_poly.h"   
// 通过指针run任何vehicle   
void run_vehicle(const Vehicle* vehicle)   
{   
    vehicle->run();            // 根据vehicle的具体类型调用对应的run()   
}   
int main()   
{   
    Car car;   
    Airplane airplane;   
    run_vehicle(&car);         // 调用Car::run()   
    run_vehicle(&airplane);    // 调用Airplane::run()   
}  
// dynamic_poly_1.cpp
#include <iostream>
#include <vector>
#include "dynamic_poly.h"
// 通过指针run任何vehicle
void run_vehicle(const Vehicle* vehicle)
{
    vehicle->run();            // 根据vehicle的具体类型调用对应的run()
}
int main()
{
    Car car;
    Airplane airplane;
    run_vehicle(&car);         // 调用Car::run()
    run_vehicle(&airplane);    // 调用Airplane::run()
} 
上面的例子来自于[http://www.vckbase.com/document/viewdoc/?id=948](http://www.vckbase.com/document/viewdoc/?id=948)，这篇文章里面还提到了多态的一些别的例子。关于多态的文章还有孟岩写的一篇[http://blog.csdn.net/wuliming_sc/archive/2009/01/31/3855906.aspx](http://blog.csdn.net/wuliming_sc/archive/2009/01/31/3855906.aspx)，可以看看。
在很多多态的例子中，我们都可以看到将基类的方法声明为纯虚函数（virtual void run() const = 0;），这样可以要求子类必须实现这个方法，同时可以体现面向接口编程。
对于使用了virtual之后编译器会做些什么，我觉得用代码更容易说明问题。下面的例子来自[http://www.cppblog.com/zhangyq/archive/2009/06/13/87597.html](http://www.cppblog.com/zhangyq/archive/2009/06/13/87597.html)。
1 编译器会为这个类的虚函数添加一个虚表，类似下面的： 
// Pseudo-code (not C++, not C) for a static table defined within file Base.cpp 
// Pretend FunctionPtr is a generic pointer to a generic member function 
// (Remember: this is pseudo-code, not C++ code) 
FunctionPtr Base::__vtable[5] = { 
   &Base::virt0, &Base::virt1, &Base::virt2, &Base::virt3, &Base::virt4 
};
2 然后增加一个指向虚表的指针为每一个类对象，这个指针是隐藏的 
// Your original C++ source code 
class Base { 
public: 
   ... 
   FunctionPtr* __vptr;  ← supplied by the compiler, hidden from the programmer 
   ... 
}; 
3 编译器在构造中初始化这个指针 
Base::Base(...arbitrary params...) 
   : __vptr(&Base::__vtable[0])  ← supplied by the compiler, hidden from the programmer 
   ... 
{ 
   ... 
} 
在派生类中，它也会增加一个隐藏的虚表，但是它可以overrides基类的虚函数如： 
// Pseudo-code (not C++, not C) for a static table defined within file Der.cpp 
// Pretend FunctionPtr is a generic pointer to a generic member function 
// (Remember: this is pseudo-code, not C++ code) 
FunctionPtr Der::__vtable[5] = { 
   &Der::virt0, &Der::virt1, &Der::virt2, &Base::virt3, &Base::virt4 
};    
从上面的代码我们可以非常容易的划出class的结构图，当然可以用/d1reportSingleClassLayout来显示class的布局图（[http://blog.csdn.net/chief1985/archive/2009/10/23/4720191.aspx](http://blog.csdn.net/chief1985/archive/2009/10/23/4720191.aspx)）。
使用virtual的地方还有虚拟继承和虚拟析构函数。
虚拟继承的例子如下，来自[http://blog.csdn.net/wuliming_sc/archive/2009/01/31/3855607.aspx](http://blog.csdn.net/wuliming_sc/archive/2009/01/31/3855607.aspx)：
view plaincopy to clipboardprint?
class Point2d{   
public:   
//...   
protected:   
float _x;   
float _y;   
};   
class Vertex : public virtual Point2d{   
public:   
//...   
protected:   
Vertex *next;   
};   
class Point3d : public virtual Point2d{   
public:   
//...   
protected:   
float _z;   
};   
class Vertex3d: public Vertex, public Point3d{   
public:   
//...   
protected:   
float mumble;   
};  
class Point2d{
public:
//...
protected:
float _x;
float _y;
};
class Vertex : public virtual Point2d{
public:
//...
protected:
Vertex *next;
};
class Point3d : public virtual Point2d{
public:
//...
protected:
float _z;
};
class Vertex3d: public Vertex, public Point3d{
public:
//...
protected:
float mumble;
}; 
使用虚拟继承一般都是出现了菱形继承（c++允许多重继承），这种情况下若不使用虚拟继承，便会导致以下情况：
1.公共基类子对象的重复创建问题。
2.成员函数的名字冲突问题
3. 数据成员的名字冲突问题
出现虚拟析构函数的地方也是在多态的时候，如果不使用虚拟析构函数，将会导致子类的析构函数没被调用，例子可以参考[http://blog.csdn.net/starlee/archive/2006/03/09/619827.aspx](http://blog.csdn.net/starlee/archive/2006/03/09/619827.aspx)。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/chief1985/archive/2009/11/01/4755560.aspx](http://blog.csdn.net/chief1985/archive/2009/11/01/4755560.aspx)#

