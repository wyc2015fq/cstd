# Performanced C++ 经验规则（3）：你不知道的构造函数（下） - 文章 - 伯乐在线
原文出处： [张峻崇](http://www.cnblogs.com/ccdev/archive/2012/12/24/2830976.html)
前面两篇，我们已经讨论了C++构造函数中诸多细枝末节，但百密一疏，还有一些地方我们没有考虑到。这一篇将对这些问题进行完结。
7、构造函数中的异常
当你在构造函数中写代码的时候，你有没有想过，如果构造函数中出现异常（别告诉我，你不抛异常。“必要”时系统会替你抛的），那会出现怎样的情况？
对象还能构建完成吗？构造函数中已经执行的代码产生的负面效应（如动态分配内存）如何解决？对象退出其作用域时，其析构函数能被调用吗？
上述这些问题，正是构造函数中产生异常要面临的问题。让我们先看结论，再分析过程：**尽可能不要在构造函数中产生（抛出）异常，否则，一定会产生问题**。
我们先看一段代码：

C++
```
#include <iostream>
#include <exception>
#include <stdexcept>
using namespace  std;
class ConWithException
{
public:
    ConWithException() : _pBuf(NULL)
    {
        _pBuf = new int[100];
        throw std::runtime_error("Exception in Constructor!");
    }
    ~ConWithException()
    {
        cout << "Destructor!" << endl;
        if( _pBuf != NULL )
        {
            cout <<  "Delete buffer..." << endl;;
            delete[] _pBuf;
            _pBuf = NULL;
        }
    }
private:
    int* _pBuf;
};
int main(int argc, char** argv)
{
    ConWithException* cwe = NULL;
    try
    {
        cwe = new ConWithException;
    }
    catch( std::runtime_error& e )
    {
        cout<< e.what() << endl;
    }
    delete cwe;
    return 0;
}
```
这段代码运行结果是什么呢？
输出

C++
```
Exception in Constructor!
```
输出“Exception in Constructor!”说明，我们抛出的异常已经成功被捕获，但有没有发现什么问题呢？有一个很致命的问题，那就是，对象的析构函数没有被调用！也就是说，delete cwe这一句代码没有起任何作用，相当于对delete NULL指针。再往上推，我们知道cwe值还是初始化的NULL，说明对象没有成功的构建出来，因为**在构造函数中抛出了异常，终止了构造函数的正确执行，没有返回对象**。即使我们把cwe = new ConWithException换成在栈中分配（ConWithException cwe;），仍是相同的结果，但cwe退出其作用域时，其析构函数也不会被调用，因为cwe根本不是一个正确的对象！继续看，在这个构造函数中，为成员指针_pBuf动态申请了内存，并计划在析构函数中释放这一块内存。然而，由于构造函数抛出异常，没有返回对象，析构函数也没有被调用，_pBuf指向的内存就发生了泄露！每调用一次这个构造函数，就泄露一块内存，产生严重的问题。现在，你知道了，为什么不能在构造函数中抛出异常，即使没有_pBuf这样需要动态申请内存的指针成员存在。
然而很多时候，异常并不是由你主动抛出的，也就是说，将上述构造函数改造成这样：

C++
```
ConWithException() : _pBuf(NULL)
{
    _pBuf = new int[100];
}
```
这是我们十分熟悉的格式吧？没错，但是，这样的写法仍然可能产生异常，因为这取决于编译器的实现。当动态内存分配失败时，编译器可能返回一个NULL指针（这也是惯用方式），OK，那没有问题。但是，有些编译器也有可能引发bad_alloc异常，如果对异常进行捕获（通常也不会这样做），结果将同上述例子所示。而如果未对异常进行捕获，结果更加糟糕，这将产生Uncaught exception，通常将导致程序终止。并且，此类问题是运行阶段可能出现的问题，这将更难发现和处理。
说了半天，就是认为上述写法，还不够好，不OK，接下来讲述解决方案。
解决方案一：使用智能指针shared_ptr（c++0x后STL提供，c++0x以前可采用boost），注意，在此处不能使用auto_ptr（因为要申请100个int，而即使申请的是单个对象，也不建议使用auto_ptr，关于智能指针，本系列后面的规则会有讲述）；
解决方案二：就是前面多次提到的，采用**“工厂模式”替换公有构造函数，从而尽可能使构造函数“轻量级“**。

C++
```
class ConWithException //为和前面比对，类名没改，糟糕的类名
{
public:
    ConWithException* factory(some parameter...)
    {
        ConWithException* cwe = new ConWithException;
        if(cwe)
        {
            cwe->_pBuf = new int[100];
            //other initialization...
        }
        return cwe;
    }
    ~ConWithException()
    {
        if(cwe->_pBuf)
        {
            delete[] cwe->_pBuf;
            _pBuf = NULL;
        }
        //other destory process...
    }
private:
    ConWithException() : _pBuf(NULL) {} //如果有非静态const成员还需要在初始化列表中进行初始化，否则什么也不做
    int* _pBuf;
};
```
使用“工厂模式”的好处是显而易见的，上述构造函数中异常的问题可以得到完美解决？why？因为构造函数十分轻量级，可轻松的完成对象的构建，“重量级”的工作都交由“工厂”（factory）方法完成，这是一个公有的普通成员函数，如果在这个函数中产生任何异常，因为对象已经正确构建，可以完美的进行异常处理，也能保证对象的析构函数被正确地调用，杜绝memory leak。构造函数被声明为私有，以保证从工厂“安全”地产生对象，使用“工厂模式”，还可以禁止从栈上分配对象（其实Java、Objective-C都是这么做的），在必要的时候，这会很有帮助。
8、构造函数不能被继承：虽然子类对象中包含了基类对象，但并不能代表构造函数被继承，即，除了在子类构造函数的初始化列表里，你可以显式地调用基类的构造函数，在子类的其它地方调用父类的构造函数都是非法的。
9、当类中有需要动态分配内存的成员指针时，需要使用“深拷贝“重写拷贝构造函数和赋值操作符，杜绝编译器“用心良苦”的产生自动生成版本，以防资源申请、释放不正确。
10、除非必要，否则最好在构造函数前添加explicit关键字，杜绝隐式使构造函数用作自动类型转换。
终于写完了，这三篇有关构造函数的“经验”之谈，其实，这些问题，也是老生常谈了。经过这三篇的学习，为敲开C++的壁垒，我们又添加了一把强有力的斧头。
