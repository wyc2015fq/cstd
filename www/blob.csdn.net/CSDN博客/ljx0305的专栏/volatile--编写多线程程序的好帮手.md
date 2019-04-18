# volatile--编写多线程程序的好帮手 - ljx0305的专栏 - CSDN博客
2008年07月30日 10:13:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：608
并不是我故意想弄糟你的心情，但是在这期专栏里，我们将讨论多线程编程这一话题。正如上一期Generic里所说的，编写异常安全（exception-safe）的程序是非常困难的，但是和编写多线程程序比起来，那简直就是儿戏。
多线程的程序是出了名的难编写、难验证、难调试、难维护，这通常是件苦差事。不正确的多线程程序可能可以运行很多年也不出一点错，直到满足某些临界的条件时，才出现意想不到的奇怪错误。
不用说，编写多线程程序的程序员需要使用可能得到的所有帮助。这期专栏将专注于讨论竞争条件（race conditions）——这通常是多线程程序中各种麻烦的根源——深入了解它并提供一些工具来防止竞争。令人惊异的是，我们将让编译器尽其所能来帮助你做这些事。
仅仅一个不起眼的关键字
尽管C和C++标准对于线程都明显的“保持沉默”，但它们以volatile关键字的形式，确实为多线程保留了一点特权。
就象大家更熟悉的const一样，volatile是一个类型修饰符（type modifier）。它是被设计用来修饰被不同线程访问和修改的变量。如果没有volatile，基本上会导致这样的结果：要么无法编写多线程程序，要么编译器失去大量优化的机会。下面我们来一个个说明。
考虑下面的代码： 
CODE
class Gadget
{
public:
   void Wait()
   {
       while (!flag_)
       {
           Sleep(1000); // sleeps for 1000 milliseconds
       }
   }
   void Wakeup()
   {
       flag_ = true;
   }
   ...
private:
   bool flag_;
};
上面代码中Gadget::Wait的目的是每过一秒钟去检查一下flag_成员变量，当flag_被另一个线程设为true时，该函数才会返回。至少这是程序作者的意图，然而，这个Wait函数是错误的。
假设编译器发现Sleep(1000)是调用一个外部的库函数，它不会改变成员变量flag_，那么编译器就可以断定它可以把flag_缓存在寄存器中，以后可以访问该寄存器来代替访问较慢的主板上的内存。这对于单线程代码来说是一个很好的优化，但是在现在这种情况下，它破坏了程序的正确性：当你调用了某个Gadget的Wait函数后，即使另一个线程调用了Wakeup，Wait还是会一直循环下去。这是因为flag_的改变没有反映到缓存它的寄存器中去。编译器的优化未免有点太……乐观了。
在大多数情况下，把变量缓存在寄存器中是一个非常有价值的优化方法，如果不用的话很可惜。C和C++给你提供了显式禁用这种缓存优化的机会。如果你声明变量是使用了volatile修饰符，编译器就不会把这个变量缓存在寄存器里——每次访问都将去存取变量在内存中的实际位置。这样你要对 Gadget的Wait/Wakeup做的修改就是给flag_加上正确的修饰：
CODE
class Gadget
{
public:
   ... as above ...
private:
   volatile bool flag_;
};
大多数关于volatile的原理和用法的解释就到此为止，并且建议你用volatile修饰在多个线程中使用的原生类型变量。然而，你可以用volatile做更多的事，因为它是神奇的C++类型系统的一部分。
把volatile用于自定义类型
volatile修饰不仅可以用于原生类型，也可以用于自定义类型。这时候，volatile修饰方式类似于const（你也可以对一个类型同时使用const和volatile）。
与const不同，volatile的作用对于原生类型和自定义类型是有区别的。就是说，原生类型有volatile修饰时，仍然支持它们的各种操作（加、乘、赋值等等），然而对于class来说，就不是这样。举例来说，你可以把一个非volatile的int的值赋给一个volatile 的int，但是你不能把一个非volatile的对象赋给一个volatile对象。
让我们举个例子来说明自定义类型的volatile是怎么工作的。
CODE
class Gadget
{
public:
   void Foo() volatile;
   void Bar();
   ...
private:
   String name_;
   int state_;
};
...
Gadget regularGadget;
volatile Gadget volatileGadget;
如果你认为volatile对于对象来说没有什么作用的话，那你可要大吃一惊了。
volatileGadget.Foo(); // ok, volatile fun called for
// volatile object
regularGadget.Foo(); // ok, volatile fun called for
// non-volatile object
volatileGadget.Bar(); // error! Non-volatile function called for
// volatile object!
从没有volatile修饰的类型到相应的volatile类型的转换是很平常的。但是，就象const一样，你不能反过来把volatile类型转换为非volatile类型。你必须用类型转换运算符：
Gadget& ref = const_cast<Gadget&>(volatileGadget);
ref.Bar(); // ok
一个有volatile修饰的类只允许访问其接口的一个子集，这个子集由类的实现者来控制。用户只有用const_cast才可以访问这个类型的全部接口。而且，象const一样，类的volatile属性会传递给它的成员（例如，volatileGadget.name_和 volatileGadget.state_也是volatile变量）。
volatile，临界区和竞争条件
多线程程序中最简单也是最常用的同步机制要算是mutex（互斥对象）了。一个mutex只提供两个基本操作：Acquire和Release。一旦某个线程调用了Acquire，其他线程再调用Acquire时就会被阻塞。当这个线程调用Release后，刚才阻塞在Acquire里的线程中，会有一个且仅有一个被唤醒。换句话说，对于一个给定的mutex，只有一个线程可以在Acquire和Release调用之间获取处理器时间。在 Acquire和Release调用之间执行的代码叫做临界区（critical section）。（Windows的用语可能会引起一点混乱，因为Windows把mutex本身叫做临界区，而Windows的mutex实际上指进程间的mutex。如果把它们分别叫作线程mutex和进程mutex可能会好些。）
Mutex是用来避免数据出现竞争条件。根据定义，所谓竞争条件就是这样一种情况：多个线程对数据产生的作用要依赖于线程的调度顺序的。当两个线程竞相访问同一数据时，就会发生竞争条件。因为一个线程可以在任意一个时刻打断其他线程，数据可能会被破坏或者被错误地解释。因此，对数据的修改操作，以及有些情况下的访问操作，必须用临界区保护起来。在面向对象的编程中，这通常意味着你在一个类的成员变量中保存一个mutex，然后在你访问这个类的状态时使用这个mutex。
多线程编程高手看了上面两个段落，可能已经在打哈欠了，但是它们的目的只是提供一个准备练习，我们现在要和volatile联系起来了。我们将把C++的类型和线程的语义作一个对比。
在一个临界区以外，任意线程会在任何时间打断别的线程；这是不受控制的，所以被多个线程访问的变量容易被改得面目全非。这和volatile的原意[1]是一致的——所以需要用volatile来防止编译器无意地缓存这样的变量。
在由一个mutex限定的临界区里，只有一个线程可以进入。因此，在临界区中执行的代码有和单线程程序有相同的语义。被控制的变量不会再被意外改变——你可以去掉volatile修饰。
简而言之，线程间共享的数据在临界区之外是volatile的，而在临界区之内则不是。
你通过对一个mutex加锁来进入一个临界区，然后你用const_cast去掉某个类型的volatile修饰，如果我们能成功地把这两个操作放到一起，那么我们就在C++类型系统和应用程序的线程语义建立起联系。这样我们可以让编译器来帮我们检测竞争条件。
LockingPtr
我们需要有一个工具来做mutex的获取和const_cast两个操作。让我们来设计一个LockingPtr类，你需要用一个volatile 的对象obj和一个mutex对象mtx来初始化它。在LockingPtr对象的生命期中，它会保证mtx处于被获取状态，而且也提供对去掉 volatile修饰的obj的访问。对obj的访问类似于smart pointer，是通过operator->和operator*来进行的。const_cast是在LockingPtr内部进行。这个转化在语义上是正确的，因为LockingPtr在其生存期中始终拥有mutex。
首先，我们来定义和LockingPtr一起工作的Mutex类的框架： 
CODE
public:
   void Acquire();
   void Release();
   ...    
};
为了使用LockingPtr，你需要用操作系统提供的数据结构和底层函数来实现Mutex。
LockingPtr是一个模板，用被控制变量的类型作为模板参数。例如，如果你希望控制一个Widget，你就要这样写LockingPtr <Widget>。
LockingPtr的定义很简单，它只是实现了一个单纯的smart pointer。它关注的焦点只是在于把const_cast和临界区操作放在一起。
CODE
template <typename T>
class LockingPtr {
public:
  // Constructors/destructors
  LockingPtr(volatile T& obj, Mutex& mtx)
      : pObj_(const_cast<T*>(&obj)),
       pMtx_(&mtx)
  {    mtx.Lock();    }
  ~LockingPtr()
  {    pMtx_->Unlock();    }
  // Pointer behavior
  T& operator*()
  {    return *pObj_;    }
  T* operator->()
  {   return pObj_;   }
private:
  T* pObj_;
  Mutex* pMtx_;
  LockingPtr(const LockingPtr&);
  LockingPtr& operator=(const LockingPtr&);
};
尽管很简单，LockingPtr对于编写正确的多线程代码非常有用。你应该把线程间共享的对象声明为volatile，但是永远不要对它们使用 const_cast——你应该始终是用LockingPtr的自动对象（automatic objects）。让我们举例来说明。
比如说你有两个线程需要共享一个vector<char>对象： 
CODE
class SyncBuf {
public:
   void Thread1();
   void Thread2();
private:
   typedef vector<char> BufT;
   volatile BufT buffer_;
   Mutex mtx_; // controls access to buffer_
};
在一个线程的函数里，你只需要简单地使用一个LockingPtr<BufT>对象来获取对buffer_成员变量的受控访问：
CODE
void SyncBuf::Thread1() {
   LockingPtr<BufT> lpBuf(buffer_, mtx_);
   BufT::iterator i = lpBuf->begin();
   for (; i != lpBuf->end(); ++i) {
       ... use *i ...
   }
}
这样的代码很容易编写，也很容易理解——每当你需要使用buffer_时，你必须创建一个LockingPtr<BufT>来指向它。当你这样做了以后，你就可以访问vector的全部接口。
这个方法的好处是，如果你犯了错误，编译器会指出它：
CODE
void SyncBuf::Thread2() {
   // Error! Cannot access 'begin' for a volatile object
   BufT::iterator i = buffer_.begin();
   // Error! Cannot access 'end' for a volatile object
   for (; i != lpBuf->end(); ++i) {
       ... use *i ...
   }
}
你不能访问buffer_的任何函数，除非你进行了const_cast或者用LockingPtr。这两者的区别是LockingPtr提供了一个有规则的方法来对一个volatile变量进行const_cast。
LockingPtr有非常好的表达力。如果你只需要调用一个函数，你可以创建一个无名的临时LockingPtr对象，然后直接使用它：
unsigned int SyncBuf::Size() {
   return LockingPtr<BufT>(buffer_, mtx_)->size();
}
回到原生类型
我们已经看到了volatile对于保护对象免于不受控的访问是多么出色，并且看到了LockingPtr是怎么提供了一个简单有效的办法来编写线程安全的代码。现在让我们回到原生类型，volatile对它们的作用方式是不同的。
让我们来考虑一个多个线程共享一个int变量的例子。
CODE
class Counter
{
public:
   ...
   void Increment() { ++ctr_; }
   void Decrement() { --ctr_; }
private:
   int ctr_;
};
如果Increment和Decrement是在不同的线程里被调用的，上面的代码片断里就有bug。首先，ctr_必须是volatile的。其次，即使是一个看上去是原子的操作，比如++ctr_，实际上也分为三个阶段。内存本身是没有运算功能的，当对一个变量进行增量操作时，处理器会：
* 把变量读入寄存器
* 对寄存器里的值加1
* 把结果写回内存 
这个三步操作称为RMW（Read-Modify-Write）。在一个RMW操作的Modify阶段，大多数处理器都会释放内存总线，以使其他处理器能够访问内存。
如果在这个时候另一个处理器对同一个变量也进行RMW操作，我们就遇到了一个竞争条件：第二次写入会覆盖掉第一次的值。
为了防止这样的事发生，你又要用到LockingPtr：
CODE
class Counter
{
public:
   ...
   void Increment() { ++*LockingPtr<int>(ctr_, mtx_); }
   void Decrement() { --*LockingPtr<int>(ctr_, mtx_); }
private:
   volatile int ctr_;
   Mutex mtx_;
};
现在这段代码正确了，但是和SyncBuf相比，这段代码的质量要差一些。为什么？因为对于Counter，编译器不会在你错误地直接访问 ctr_（没有对它加锁）时产生警告。虽然ctr_是volatile的，但是编译器还是可以编译++ctr_，尽管产生的代码绝对是不正确的。编译器不再是你的盟友了，你只有自己留意竞争条件。
那么你该怎么做呢？很简单，你可以用一个高层的结构来包装原生类型的数据，然后对那个结构使用volatile。这有点自相矛盾，直接用volatile修饰原生类型是一个不好的用法，尽管这是volatile最初期望的用法！
volatile成员函数
到现在为止，我们讨论了具有volatile数据成员的类；现在让我们来考虑设计这样的类，它会作为更大的对象的一部分并且在线程间共享。这里，volatile的成员函数可以帮很大的忙。
在设计类的时候，你只对那些线程安全的成员函数加volatile修饰。你必须假定外面的代码会在任何地方任何时间调用volatile成员函数。不要忘记：volatile相当于自由的多线程代码，并且没有临界区；非volatile相当于单线程的环境或者在临界区内。
比如说，你定义了一个Widget类，它用两个方法实现了同一个操作——一个线程安全的方法和一个快速的不受保护的方法。
CODE
class Widget
{
public:
   void Operation() volatile;
   void Operation();
   ...
private:
   Mutex mtx_;
};
注意这里的重载（overloading）用法。现在Widget的用户可以用一致的语法调用Operation，对于volatile对象可以得到线程安全性，对于普通对象可以得到速度。用户必须注意把共享的Widget对象定义为volatile。
在实现volatile成员函数时，第一个操作通常是用LockingPtr对this进行加锁，然后其余工作可以交给非volatile的同名函数做：
CODE
void Widget::Operation() volatile
{
   LockingPtr<Widget> lpThis(*this, mtx_);
   lpThis->Operation(); // invokes the non-volatile function
}
小结
在编写对线程程序的时候，使用volatile将对你十分有益。你必须坚持下面的规则：
* 把所有共享对象声明为volatile
* 不要对原生类型直接使用volatile
* 定义共享类时，用volatile成员函数来表示它的线程安全性。 
如果你这么做了，而且用了简单的通用组件LockingPtr，你就可以写出线程安全的代码，并且大大减少对竞争条件的担心，因为编译器会替你操心，并且勤勤恳恳地为你指出哪里错了。
在我参与的几个项目中，使用volatile和LockingPtr产生了很大效果。代码十分整洁，也容易理解。我记得遇到过一些死锁的情况，但是相对于竞争条件，我宁愿对付死锁的情况，因为它们调试起来容易多了。那些项目实际上根本没有碰到过有关竞争条件的问题。
致谢
非常感谢James Kanze和Sorin Jianu提供了很有洞察力的意见。
附：滥用volatile的本质？[2]
在上一期的专栏《Generic<Programming>: volatile — Multithreaded Programmer's Best Friend》发表以后，我收到很多反馈意见。就像是注定的一样，大部分称赞都是私人信件，而抱怨都发到USENET新闻组comp.lang.c++. moderated和 comp.programming.threads里去了。随后引起了很长很激烈的讨论，如果你对这个主题有兴趣，你可以去看看这个讨论，它的标题是 “volatile, was: memory visibility between threads.”。
我知道我从这个讨论中学到了很多东西。比如说，文章开头的Widget的例子不太切题。长话短说，在很多系统（比如POSIX兼容的系统）中，volatile修饰是不需要的，而在另一些系统中，即使加了volatile也没有用，程序还是不正确。
关于volatile correctness，最重要的一个问题是它依赖于类似POSIX的mutex，如果在多处理器系统上，光靠mutex就不够了——你必须用memory barriers。
另一个更哲理性的问题是：严格来说通过类型转换把变量的volatile属性去掉是不合法的，即使volatile属性是你自己为了 volatile correctness而加上去的。正如Anthony Williams指出的，可以想象一个系统可能把volatile数据放在一个不同于非volatile数据的存储区中，在这种情况下，进行地址变换会有不确定的行为。
另一个批评是volatile correctness虽然可以在一个较低层次上解决竞争条件，但是不能正确的检测出高层的、逻辑的竞争条件。例如，你有一个mt_vector模版类，用来模拟std::vector，成员函数经过正确的线程同步修正。考虑这段代码：
volatile mt_vector<int> vec;
…
if (!vec.empty()) {
vec.pop_back();
}
这段代码的目的是删除vector里的最后一个元素，如果它存在的话。在单线程环境里，他工作地很好。然而如果你把它用在多线程程序里，这段代码还是有可能抛出异常，尽管empty和pop_back都有正确的线程同步行为。虽然底层的数据（vec）的一致性有保证，但是高层操作的结果还是不确定的。
无论如何，经过辩论之后，我还是保持我的建议，在有类POSIX的mutex的系统上，volatile correctness还是检测竞争条件的一个有价值的工具。但是如果你在一个支持内存访问重新排序的多处理器系统上，你首先需要仔细阅读你的编译器的文档。你必须知己知彼。
最后，Kenneth Chiu提到了一篇非常有趣的文章http://theory.stanford.edu/~freunds/race.ps，猜猜题目是什么？ “Type-Based Race Detection for Java”。这篇文章讲了怎么对Java的类型系统作一点小小的补充，从而让编译器和程序员一起在编译时检测竞争条件。
上面写了很多
其实voliate功能就是
说明某个变量是可变的
不应该对它的值进行假设
如两个信号处理程序与主程序同时使用一个变量时
或是两个线程同时使用一个变量时
就应该对它进行说明
引用:http://bbs.loveunix.net/viewthread.php?tid=8404
