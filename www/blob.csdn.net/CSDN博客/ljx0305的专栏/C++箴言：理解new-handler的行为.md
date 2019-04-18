# C++箴言：理解new-handler的行为 - ljx0305的专栏 - CSDN博客
2008年07月27日 22:45:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：584
 当 operator new 不能满足一个内存分配请求时，它抛出一个 exception（异常）。很久以前，他返回一个 null pointer（空指针），而一些比较老的编译器还在这样做。你依然能达到以前的目的（在一定程度上），但是我要到本文的最后再讨论它。
　　在 operator new 因回应一个无法满足的内存请求而抛出一个 exception 之前，它先调用一个可以由客户指定的被称为 new-handler 的 error-handling function（错误处理函数）。（这并不完全确切，operator new 真正做的事情比这个稍微复杂一些，详细细节将在下一篇文章中讨论。）为了指定 out-of-memory-handling function，客户调用 set_new_handler ——一个在 中声明的标准库函数：
namespace std {
　typedef void (*new_handler)();
　new_handler set_new_handler(new_handler p) throw();
}
　　就像你能够看到的，new_handler 是一个指针的 typedef，这个指针指向不取得和返回任何东西的函数，而 set_new_handler 是一个取得和返回一个 new_handler 的函数。（set_new_handler 的声明的结尾处的 "throw()" 是一个 exception specification（异常规范）。它基本上是说这个函数不会抛出任何异常，尽管真相更有趣一些。关于细节，参见《C++箴言：争取异常安全的代码》。）
　　set_new_handler 的形参是一个指向函数的指针，这个函数是 operator new 无法分配被请求的内存时应该调用的。set_new_handler 的返回值是一个指向函数的指针，这个函数是 set_new_handler 被调用前有效的目标。
　　你可以像这样使用 set_new_handler：
// function to call if operator new can't allocate enough memory
void outOfMem()
{
　std::cerr << "Unable to satisfy request for memory/n";
　std::abort();
}
int main()
{
　std::set_new_handler(outOfMem);
　int *pBigDataArray = new int[100000000L];
　...
}
　　如果 operator new 不能为 100,000,000 个整数分配空间，outOfMem 将被调用，而程序将在发出一个错误信息后中止。（顺便说一句，考虑如果在写这个错误信息到 cerr... 的过程中内存必须被动态分配会发生什么。）
当 operator new 不能满足一个内存请求时，它反复调用 new-handler function 直到它能找到足够的内存。但是从这种高层次的描述已足够推导出一个设计得好的 new-handler function 必须做到以下事情之一：
　　·Make more memory available（使得更多的内存可用）。这可能使得 operator new 中下一次内存分配的尝试成功。实现这一策略的一个方法是在程序启动时分配一大块内存，然后在 new-handler 第一次被调用时释放它供程序使用。
　　·Install a different new-handler（安装一个不同的 new-handler）。如果当前的 new-handler 不能做到使更多的内存可用，或许它知道有一个不同的 new-handler 可以做到。如果是这样，当前的 new-handler 能在它自己的位置上安装另一个 new-handler（通过调用 set_new_handler）。operator new 下一次调用 new-handler function 时，它会得到最近安装的那一个。（这个主线上的一个变化是让一个 new-handler 改变它自己的行为，这样，下一次它被调用时，可以做一些不同的事情。做到这一点的一个方法是让 new-handler 改变能影响 new-handler 行为的 static（静态），namespace-specific（名字空间专用）或 global（全局）的数据。）
　　·Deinstall the new-handler（卸载 new-handler），也就是，将空指针传给 set_new_handler。没有 new-handler 被安装，当内存分配没有成功时，operator new 抛出一个异常。
　　·Throw an exception（抛出一个异常），类型为 bad_alloc 或继承自 bad_alloc 的其它类型。这样的异常不会被 operator new 捕获，所以它们将被传播到发出内存请求的地方。
　　·Not return（不再返回），典型情况下，调用 abort 或 exit。
　　这些选择使你在实现 new-handler functions 时拥有极大的弹性。
　　有时你可能希望根据被分配 object 的不同，用不同的方法处理内存分配的失败：
class X {
public:
　static void outOfMemory();
　...
};
class Y {
public:
　static void outOfMemory();
　...
};
X* p1 = new X; // if allocation is unsuccessful,
// call X::outOfMemory
Y* p2 = new Y; // if allocation is unsuccessful,
// call Y::outOfMemory
C++ 没有对 class-specific new-handlers 的支持，但是它也不需要。你可以自己实现这一行为。你只要让每一个 class 提供 set_new_handler 和 operator new 的它自己的版本即可。class 的 set_new_handler 允许客户为这个 class 指定 new-handler（正像standard set_new_handler 允许客户指定global new-handler）。class 的 operator new 确保当为 class objects 分配内存时，class-specific new-handler 代替 global new-handler 被使用。
　　假设你要为 Widget class 处理内存分配失败。你就必须清楚当 operator new 不能为一个 Widget object 分配足够的内存时所调用的函数，所以你需要声明一个 new_handler 类型的 static member（静态成员）指向这个 class 的 new-handler function。Widget 看起来就像这样：
class Widget {
public:
　static std::new_handler set_new_handler(std::new_handler p) throw();
　static void * operator new(std::size_t size) throw(std::bad_alloc);
private:
　static std::new_handler currentHandler;
};
　　static class members（静态类成员）必须在 class 定义外被定义（除非它们是 const 而且是 integral），所以：
std::new_handler Widget::currentHandler = 0; // init to null in the class
// impl. file
　　Widget 中的 set_new_handler 函数会保存传递给它的任何指针，而且会返回前次调用时被保存的任何指针，这也正是 set_new_handler 的标准版本所做的事情：
std::new_handler Widget::set_new_handler(std::new_handler p) throw()
{
　std::new_handler oldHandler = currentHandler;
　currentHandler = p;
　return oldHandler;
}
　　最终，Widget 的 operator new 将做下面这些事情：
以 Widget 的 error-handling function 为参数调用 standard set_new_handler。这样将 Widget 的new-handler 安装为 global new-handler。
调用 global operator new 进行真正的内存分配。如果分配失败，global operator new 调用 Widget 的 new-handler，因为那个函数刚才被安装为 global new-handler。如果 global operator new 最后还是无法分配内存，它会抛出一个 bad_alloc exception。在此情况下，Widget 的 operator new 必须恢复原来的 global new-handler，然后传播那个 exception。为了确保原来的 new-handler 总能被恢复，Widget 将 global new-handler 作为一种资源对待，并遵循《C++箴言：使用对象管理资源》中的建议，使用 resource-managing objects（资源管理对象）来预防 resource leaks（资源泄漏）。
　　如果 global operator new 能够为一个 Widget object 分配足够的内存，Widget 的 operator new 返回一个指向被分配内存的指针。object 的用于管理 global new-handler 的 destructor（析构函数）自动将 global new-handler 恢复到调用 Widget 的 operator new 之前的状态。
　　以下就是你如何在 C++ 中表达这所有的事情。我们以 resource-handling class 开始，组成部分中除了基本的 RAII 操作（在构造过程中获得资源并在析构过程中释放）（《C++箴言：使用对象管理资源》），没有更多的东西：
class NewHandlerHolder {
public:
　explicit NewHandlerHolder(std::new_handler nh) // acquire current
　:handler(nh) {} // new-handler
　~NewHandlerHolder() // release it
　{ std::set_new_handler(handler); }
private:
　std::new_handler handler; // remember it
　NewHandlerHolder(const NewHandlerHolder&); // prevent copying
　NewHandlerHolder& // (see 《C++箴言：谨慎考虑资源管理类的拷贝行为》)
　operator=(const NewHandlerHolder&);
};
　　这使得 Widget 的 operator new 的实现非常简单：
void * Widget::operator new(std::size_t size) throw(std::bad_alloc)
{
　NewHandlerHolder // install Widget's
　h(std::set_new_handler(currentHandler)); // new-handler
　return ::operator new(size); // allocate memory
　// or throw
} // restore global
// new-handler
Widget 的客户像这样使用它的 new-handling capabilities（处理 new 的能力）：
void outOfMem(); // decl. of func. to call if mem. alloc.
// for Widget objects fails
Widget::set_new_handler(outOfMem); // set outOfMem as Widget's
// new-handling function
Widget *pw1 = new Widget; // if memory allocation
// fails, call outOfMem
std::string *ps = new std::string; // if memory allocation fails,
// call the global new-handling
// function (if there is one)
Widget::set_new_handler(0); // set the Widget-specific
// new-handling function to
// nothing (i.e., null)
Widget *pw2 = new Widget; // if mem. alloc. fails, throw an
// exception immediately. (There is
// no new- handling function for
// class Widget.)
　　无论 class 是什么，实现这个方案的代码都是一样的，所以在其它地方重用它就是一个合理的目标。使它成为可能的一个简单方法是创建一个 "mixin-style" base class（“混合风格”基类），也就是说，一个设计为允许 derived classes（派生类）继承一个单一特定能力（在当前情况下，就是设定一个 class-specific new-handler 的能力）的 base class（基类）。然后把这个 base class（基类）转化为一个 template（模板），以便于你得到针对每一个 inheriting class（继承来的类）的 class data 的不同拷贝。
　　这个设计的 base class（基类）部分让 derived classes（派生类）继承它们全都需要的 set_new_handler 和 operator new functions，而这个设计 template（模板）部分确保每一个 inheriting class（继承来的类）得到一个不同的 currentHandler data member（数据成员）。这听起来可能有点复杂，但是代码看上去可靠而且熟悉。实际上，仅有的真正不同是它现在可以用在任何需要它的 class 之上：
template // "mixin-style" base class for
class NewHandlerSupport{
　// class-specific set_new_handler
public: // support
　static std::new_handler set_new_handler(std::new_handler p) throw();
　static void * operator new(std::size_t size) throw(std::bad_alloc);
　... // other versions of op. new
private:
　static std::new_handler currentHandler;
};
template
std::new_handler
NewHandlerSupport::set_new_handler(std::new_handler p) throw()
{
　std::new_handler oldHandler = currentHandler;
　currentHandler = p;
　return oldHandler;
}
template
void* NewHandlerSupport::operator new(std::size_t size)
throw(std::bad_alloc)
{
　NewHandlerHolder h(std::set_new_handler(currentHandler));
　return ::operator new(size);
}
// this initializes each currentHandler to null
template
std::new_handler NewHandlerSupport::currentHandler = 0;
有了这个 class template（类模板），为 Widget 增加 set_new_handler 支持就很容易了：Widget 只需要从 NewHandlerSupport 继承即可。（可能看起来很奇特，但是下面我将解释更多的细节。）
class Widget: public NewHandlerSupport {
　... // as before, but without declarations for
}; // set_new_handler or operator new
　　这些就是 Widget 为了提供一个 class-specific set_new_handler 所需要做的全部。
　　但是也许你依然在为 Widget 从 NewHandlerSupport 继承而烦恼。如果是这样，当你注意到 NewHandlerSupport template 从来没有用到它的 type parameter T 时，你可能会更加烦恼。它不需要那样做。我们需要的全部就是为每一个从 NewHandlerSupport 继承的 class 提供一份不同的 NewHandlerSupport ——特别是它的 static data member（静态数据成员）currentHandler ——的拷贝。template parameter T 只是为了将一个 inheriting class 同另一个区分开来。template 机制自己自动地为每一个被实例化的 NewHandlerSupport 中的 T 生成一个 currentHandler 的拷贝。
　　对于 Widget 从一个把 Widget 当作一个 type parameter（类型参数）的 templatized base class（模板化基类）继承，如果这个概念把你弄得有点糊涂，不必难受。它最开始对每一个人都有这种影响。然而，它发展成如此有用的一项技术，它有一个名字，虽然它正常看上去所反映的事实并不是他们第一次看到它的样子。它被称作 curiously recurring template pattern（奇特的递归模板模式） (CRTP)。真的。
　　在这一点上，我发表了一篇文章建议一个更好的名字叫做 "Do It For Me"，因为当 Widget 从 NewHandlerSupport 继承时，它其实是在说：“我是 Widget，而我要从针对 Widget 的 NewHandlerSupport class 继承。”没有人使用我提议的名字（甚至是我自己），但是把 CRTP 考虑成说 "do it for me" 的一种方式也许会帮助你理解 templatized inheritance（模板化继承）在做些什么。
　　像 NewHandlerSupport 这样的 templates 使得为任何有需要的 class 添加一个 class-specific new-handler 变得易如反掌。然而，mixin-style inheritance（混合风格继承）总是会导致 multiple inheritance（多继承）的话题，而在我们沿着这条路走下去之前，你需要阅读《C++箴言：谨慎使用多继承》。
直到 1993 年，C++ 还要求 operator new 不能分配被请求的内存时要返回 null。operator new 现在则被指定抛出一个 bad_alloc exception，但是很多 C++ 程序是在编译器开始支持这个修订标准之前写成的。C++ 标准化委员会不想遗弃这些 test-for-null（检验是否为 null）的代码基础，所以他们提供了 operator new 的另一种可选形式，用以提供传统的 failure-yields-null（失败导致 null）的行为。这些形式被称为 "nothrow" 形式，这在一定程度上是因为它们在使用 new 的地方使用了 nothrow objects（定义在头文件  中）：
class Widget { ... };
Widget *pw1 = new Widget; // throws bad_alloc if
// allocation fails
if (pw1 == 0) ... // this test must fail
Widget *pw2 =new (std::nothrow) Widget; // returns 0 if allocation for
// the Widget fails
if (pw2 == 0) ... // this test may succeed
　　对于异常，nothrow new 提供了比最初看上去更少的强制保证。在表达式 "new (std::nothrow) Widget" 中，发生了两件事。首先，operator new 的 nothrow 版本被调用来为一个 Widget object 分配足够的内存。如果这个分配失败，众所周知，operator new 返回 null pointer。然而，如果它成功了，Widget constructor 被调用，而在此刻，所有打的赌都失效了。Widget constructor 能做任何它想做的事。它可能自己 new 出来一些内存，而如果它这样做了，它并没有被强迫使用 nothrow new。那么，虽然在 "new (std::nothrow) Widget" 中调用的 operator new 不会抛出，Widget constructor 却可以。如果它这样做了，exception 像往常一样被传播。结论？使用 nothrow new 只能保证 operator new 不会抛出，不能保证一个像 "new (std::nothrow) Widget" 这样的表达式绝不会导致一个 exception。在所有的可能性中，你最好绝不需要 nothrow new。
　　无论你是使用 "normal"（也就是说，exception-throwing）new，还是它的稍微有些矮小的堂兄弟，理解 new-handler 的行为是很重要的，因为它可以用于两种形式。
　　Things to Remember
　　·set_new_handler 允许你指定一个当内存分配请求不能被满足时可以被调用的函数。
　　·nothrow new 作用有限，因为它仅适用于内存分配，随后的 constructor 调用可能依然会抛出 exceptions。 
引用[http://www.oklinux.cn/html/developer/java/20070630/33552.html](http://www.oklinux.cn/html/developer/java/20070630/33552.html)
