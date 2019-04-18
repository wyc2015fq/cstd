# shared_ptr用法 - gauss的专栏 - CSDN博客
2013年01月04日 00:03:37[gauss](https://me.csdn.net/mathlmx)阅读数：294
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)
#### 头文件: "boost/shared_ptr.hpp"
几乎所有稍微复杂点的程序都需要某种形式的引用计数智能指针。这些智能指针让我们不再需要为了控制被两个或多个对象共享的对象的生存期而编写复杂的逻辑。当引用计费降为零，没有对象再需要这个共享的对象，这个对象就自动被销毁了。引用计数智能指针可以分为插入式(intrusive)和非插入式(non-intrusive)两类。前者要求它所管理的类提供明确的函数或数据成员用于管理引用计数。这意味着在类的设计时就必须预见到它将与一个插入式的引用计数智能指针一起工作，或者重新设计它。非插入式的引用计数智能指针对它所管理的类没有任何要求。引用计数智能指针拥有与它所存指针有关的内存的所有权。没有智能指针的帮助，对象的共享会存在问题，必须有人负负责删除共享的内存。谁负责？什么时候删除？没有智能指针，你必须在管理的内存之外增加生存期的管理，这意味着在各个拥有者之间存在更强的依赖关系。换言之，没有了重用性并增加了复杂性。
被管理的类可能拥有一些特性使得它更应该与引用计数智能指针一起使用。例如，它的复制操作很昂贵，或者它所代表的有些东西必须被多个实例共享，这些特性都值得去共享所有权。还有一种情形是共享的资源没有一个明确的拥有者。使用引用计数智能指针可以在需要访问共享资源的对象之间共享资源的所有权。引用计数智能指针还让你可以把对象指针存入标准库的容器中而不会有泄漏的风险，特别是在面对异常或要从容器中删除元素的时候。如果你把指针放入容器，你就可以获得多态的好处，可以提高性能(如果复制的代价很高的话)，还可以通过把相同的对象放入多个辅助容器来进行特定的查找。
在你决定使用引用计数智能指针后，你应该选择插入式的还是非插入式的？非插入式智能指针几乎总是更好的选择，由于它们的通用性、不需要修改已有代码，以及灵活性。你可以对你不能或不想修改的类使用非插入式的引用计数智能指针。而把一个类修改为使用插入式引用计数智能指针的常见方法是从一个引用计数基类派生。这种修改可能比你想象的更昂贵。至少，它增加了相关性并降低了重用性。[[6]]()
 它还增加了对象的大小，这在一些特定环境中可能会限制其可用性。[[7]]()
> 
[6] 考虑一下对同一个类型使用两个以上引用计数智能指针的需要。如果两个都是插入式的，两个不同的基类可能会不兼容，而且也很浪费。如果其中一个是插入式的，那么使用非插入式的智能指针可以使基类的额外负担为零。
> 
[7] 另一方面，非插入式智能指针要求额外的存储用于智能指针本身。
shared_ptr 可以从一个裸指针、另一个shared_ptr、一个std::auto_ptr、或者一个boost::weak_ptr构造。还可以传递第二个参数给shared_ptr的构造函数，它被称为删除器(deleter)。删除器稍后会被调用，来处理共享资源的释放。这对于管理那些不是用new分配也不是用delete释放的资源时非常有用(稍后将看到创建客户化删除器的例子)。shared_ptr被创建后，它就可象普通指针一样使用了，除了一点，它不能被显式地删除。
以下是[shared_ptr]()的部分摘要；最重要的成员和相关普通函数被列出，随后是简单的讨论。
namespace boost {
  template<typename T> class shared_ptr {
  public:
    template <class Y> explicit shared_ptr(Y* p);
    template <class Y,class D> shared_ptr(Y* p,D d);
    ~shared_ptr();
    shared_ptr(const shared_ptr & r);
    template <class Y> explicit 
      shared_ptr(const weak_ptr<Y>& r);
    template <class Y> explicit shared_ptr(std::auto_ptr<Y>& r);
    shared_ptr& operator=(const shared_ptr& r);
    void reset(); 
  
    T& operator*() const;
    T* operator->() const;
    T* get() const;
    bool unique() const;
    long use_count() const;
    operator unspecified_bool_type() const;  //译注：原文是unspecified-bool-type()，有误
    void swap(shared_ptr<T>& b);
  };
  template <class T,class U>
    shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r);
}
#### 成员函数
template <class Y> explicit shared_ptr(Y* p);
这个构造函数获得给定指针p的所有权。参数
p 必须是指向 Y 的有效指针。构造后引用计数设为1。唯一从这个构造函数抛出的异常是std::bad_alloc (仅在一种很罕见的情况下发生，即不能获得引用计数器所需的自由空间)。
template <class Y,class D> shared_ptr(Y* p,D d);
这个构造函数带有两个参数。第一个是shared_ptr将要获得所有权的那个资源，第二个是shared_ptr被销毁时负责释放资源的一个对象，被保存的资源将以d(p)的形式传给那个对象。因此p的值是否有效取决于d。如果引用计数器不能分配成功，shared_ptr抛出一个类型为std::bad_alloc的异常。
shared_ptr(const shared_ptr& r);
r中保存的资源被新构造的shared_ptr所共享，引用计数加一。这个构造函数不会抛出异常。
template <class Y> explicit shared_ptr(const weak_ptr<Y>& r);
从一个[weak_ptr]() (本章稍后会介绍)构造[shared_ptr]()。这使得weak_ptr的使用具有线程安全性，因为指向weak_ptr参数的共享资源的引用计数将会自增(weak_ptr不影响共享资源的引用计数)。如果weak_ptr为空
 (r.use_count()==0), shared_ptr 抛出一个类型为bad_weak_ptr的异常。
template <typename Y> shared_ptr(std::auto_ptr<Y>& r);
这个构造函数从一个auto_ptr获取r中保存的指针的所有权，方法是保存指针的一份拷贝并对auto_ptr调用release。构造后的引用计数为1。而r当然就变为空的。如果引用计数器不能分配成功，则抛出
std::bad_alloc 。
~shared_ptr();
shared_ptr析构函数对引用计数减一。如果计数为零，则保存的指针被删除。删除指针的方法是调用operator delete 或者，如果给定了一个执行删除操作的客户化删除器对象，就把保存的指针作为唯一参数调用这个对象。析构函数不会抛出异常。
shared_ptr& operator=(const shared_ptr& r);
赋值操作共享r中的资源，并停止对原有资源的共享。赋值操作不会抛出异常。
void reset();
reset函数用于停止对保存指针的所有权的共享。共享资源的引用计数减一。
T& operator*() const;
这个操作符返回对已存指针所指向的对象的一个引用。如果指针为空，调用operator* 会导致未定义行为。这个操作符不会抛出异常。
T* operator->() const;
这个操作符返回保存的指针。这个操作符与operator*一起使得智能指针看起来象普通指针。这个操作符不会抛出异常。
T* get() const;
get函数是当保存的指针有可能为空时(这时
operator* 和 operator-> 都会导致未定义行为)获取它的最好办法。注意，你也可以使用隐式布尔类型转换来测试
shared_ptr 是否包含有效指针。这个函数不会抛出异常。
bool unique() const;
这个函数在shared_ptr是它所保存指针的唯一拥有者时返回
true ；否则返回 false。
unique 不会抛出异常。
long use_count() const;
use_count 函数返回指针的引用计数。它在调试的时候特别有用，因为它可以在程序执行的关键点获得引用计数的快照。小心地使用它，因为在某些可能的shared_ptr实现中，计算引用计数可能是昂贵的，甚至是不行的。这个函数不会抛出异常。
operator unspecified-bool-type() const;
这是个到unspecified-bool-type类型的隐式转换函数，它可以在Boolean上下文中测试一个智能指针。如果shared_ptr保存着一个有效的指针，返回值为True；否则为false。注意，转换函数返回的类型是不确定的。把返回类型当成bool用会导致一些荒谬的操作，所以典型的实现采用了safe
 bool idiom,[[8]]() 它很好地确保了只有可适用的Boolean测试可以使用。这个函数不会抛出异常。
> 
[8] 由Peter Dimov发明的。
void swap(shared_ptr<T>& b);
这可以很方便地交换两个shared_ptr。swap 函数交换保存的指针(以及它们的引用计数)。这个函数不会抛出异常。
#### 普通函数
template <typename T,typename U>
  shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r);
要对保存在shared_ptr里的指针执行static_cast，我们可以取出指针然后强制转换它，但我们不能把它存到另一个shared_ptr里；新的
shared_ptr 会认为它是第一个管理这些资源的。解决的方法是用
static_pointer_cast. 使用这个函数可以确保被指物的引用计数保持正确。static_pointer_cast 不会抛出异常。
#### 用法
使用shared_ptr解决的主要问题是知道删除一个被多个客户共享的资源的正确时机。下面是一个简单易懂的例子，有两个类
A 和 B, 它们共享一个int实例。使用
boost::shared_ptr, 你需要必须包含 "boost/shared_ptr.hpp".
#include "boost/shared_ptr.hpp"
#include <cassert>
class A {
  boost::shared_ptr<int> no_;
public:
  A(boost::shared_ptr<int> no) : no_(no) {}
  void value(int i) {
    *no_=i;
  }
};
class B {
  boost::shared_ptr<int> no_;
public:
  B(boost::shared_ptr<int> no) : no_(no) {}
  int value() const {
    return *no_;
  }
};
int main() {
    boost::shared_ptr<int> temp(new int(14));
    A a(temp);
    B b(temp);
    a.value(28);
    assert(b.value()==28);
}
类 A 和 B都保存了一个
shared_ptr<int>. 在创建 A 和
B的实例时，shared_ptr temp 被传送到它们的构造函数。这意味着共有三个
shared_ptr：a,
b, 和 temp，它们都引向同一个int实例。如果我们用指针来实现对一个的共享，A 和
B 必须能够在某个时间指出这个int要被删除。在这个例子中，直到main的结束，引用计数为3，当所有
shared_ptr离开了作用域，计数将达到0，而最后一个智能指针将负责删除共享的
int.
#### 回顾Pimpl用法
前一节展示了使用scoped_ptr的pimpl 用法，如果使用这种用法的类是不允许复制的，那么scoped_ptr在保存pimpl的动态分配实例时它工作得很好。但是这并不适合于所有想从pimpl用法中获益的类型(注意，你还可以用
scoped_ptr，但必须手工实现复制构造函数和赋值操作符)。对于那些可以处理共享的实现细节的类，应该用
shared_ptr。当pimpl的所有权被传递给一个 shared_ptr, 复制和赋值操作都是免费的。你可以回忆起，当使用
scoped_ptr 去处理pimpl类的生存期时，对封装类的复制是不允许的，因为
scoped_ptr是不可复制的。这意味着要使这些类支持复制和赋值，你必须手工定义复制构造函数和赋值操作符。当使用
shared_ptr 去处理pimpl类的生存期时，就不再需要用户自己定义复制构造函数了。注意，这时pimpl实例是被该类的多个对象所共享，因此如果规则是每个pimpl实例只能被类的一个实例使用，你还是要手工编写复制构造函数。解决的方法和我们在scoped_ptr那看到的很相似，只是把scoped_ptr换成了shared_ptr。
#### shared_ptr 与标准库容器
把对象直接存入容器中有时会有些麻烦。以值的方式保存对象意味着使用者将获得容器中的元素的拷贝，对于那些复制是一种昂贵的操作的类型来说可能会有性能的问题。此外，有些容器，特别是
std::vector, 当你加入元素时可能会复制所有元素，这更加重了性能的问题。最后，传值的语义意味着没有多态的行为。如果你需要在容器中存放多态的对象而且你不想切割它们，你必须用指针。如果你用裸指针，维护元素的完整性会非常复杂。从容器中删除元素时，你必须知道容器的使用者是否还在引用那些要删除的元素，不用担心多个使用者使用同一个元素。这些问题都可以用shared_ptr来解决。
下面是如何把共享指针存入标准库容器的例子。
#include "boost/shared_ptr.hpp"
#include <vector>
#include <iostream>
class A {
public:
  virtual void sing()=0;
protected:
  virtual ~A() {};
};
class B : public A {
public:
  virtual void sing() {
    std::cout << "Do re mi fa so la";
  }
};
boost::shared_ptr<A> createA() {
  boost::shared_ptr<A> p(new B());
  return p;
}
int main() {
  typedef std::vector<boost::shared_ptr<A> > container_type;
  typedef container_type::iterator iterator;
  container_type container;
  for (int i=0;i<10;++i) {
    container.push_back(createA());
  }
  std::cout << "The choir is gathered: /n";
  iterator end=container.end();
  for (iterator it=container.begin();it!=end;++it) {
    (*it)->sing();
  }
}
这里有两个类, A 和 B, 各有一个虚拟成员函数 sing.
B 从 A公有继承而来，并且如你所见，工厂函数
createA 返回一个动态分配的B的实例，包装在shared_ptr<A>里。在
main里, 一个包含shared_ptr<A>的
std::vector 被放入10个元素，最后对每个元素调用sing。如果我们用裸指针作为元素，那些对象需要被手工删除。而在这个例子里，删除是自动的，因为在vector的生存期中，每个shared_ptr的引用计数都保持为1；当
vector 被销毁，所有引用计数器都将变为零，所有对象都被删除。有趣的是，即使
A 的析构函数没有声明为 virtual,
shared_ptr 也会正确调用 B的析构函数！
上面的例子示范了一个强有力的技术，它涉及A里面的protected析构函数。因为函数
createA 返回的是 shared_ptr<A>, 因此不可能对shared_ptr::get返回的指针调用
delete 。这意味着如果为了向某个需要裸指针的函数传送裸指针而从shared_ptr中取出裸指针的话，它不会由于意外地被删除而导致灾难。那么，又是如何允许
shared_ptr 删除它的对象的呢？ 这是因为指针指向的真正类型是
B; 而B的析构函数不是protected的。这是非常有用的方法，用于给shared_ptr中的对象增加额外的安全性。
#### shared_ptr 与其它资源
有时你会发现你要把shared_ptr用于某个特别的类型，它需要其它清除操作而不是简单的
delete. shared_ptr可以通过客户化删除器来支持这种需要。那些处理象
FILE*这样的操作系统句柄的资源通常要使用象fclose这样的操作来释放。要在shared_ptr里使用
FILE* ，我们要定义一个类来负责释放相应的资源。
class FileCloser {
public:
   void operator()(FILE* file) {
    std::cout << "The FileCloser has been called with a FILE*, "
      "which will now be closed./n";
    if (file!=0) 
      fclose(file);
  }
};
这是一个函数对象，我们用它来确保在资源要释放时调用 fclose 。下面是使用FileCloser类的示例程序。
int main() {
  std::cout << 
    "shared_ptr example with a custom deallocator./n"; 
  {
    FILE* f=fopen("test.txt","r");
    if (f==0) {
      std::cout << "Unable to open file/n";
      throw "Unable to open file";
    }
    boost::shared_ptr<FILE> 
      my_shared_file(f, FileCloser());
    // 定位文件指针
    fseek(my_shared_file.get(),42,SEEK_SET);
  }
  std::cout << "By now, the FILE has been closed!/n";
}
注意，在访问资源时，我们需要对shared_ptr使用
&* 用法, get, 或
get_pointer。(请注意最好使用 &*. 另两个选择不太清晰) 这个例子还可以更简单，如果我们在释放资源时只需要调用一个单参数函数的话，就根本不需要创建一个客户化删除器类型。上面的例子可以重写如下：
{
  FILE* f=fopen("test.txt","r");
  if (f==0) {
    std::cout << "Unable to open file/n";
    throw file_exception();
  }
  
  boost::shared_ptr<FILE> my_shared_file(f,&fclose);
  // 定位文件指针
  fseek(&*my_shared_file,42,SEEK_SET); 
}
std::cout << "By now, the FILE* has been closed!/n";
定制删除器在处理需要特殊释放程序的资源时非常有用。由于删除器不是 shared_ptr 类型的一部分，所以使用者不需要知道关于智能指针所拥有的资源的任何信息(当然除了如何使用它！)。例如，你可以使用对象池，定制删除器只需简单地把对象返还到池中。或者，一个 singleton 对象应该使用一个什么都不做的删除器。
#### 使用定制删除器的安全性
我们已经看到对基类使用 protected 析构函数有助于增加使用shared_ptr的类的安全性。另一个达到同样安全级别的方法是，声明析构函数为 protected (或 private) 并使用一个定制删除器来负责销毁对象。这个定制删除器必须是它要删除的类的友元，这样它才可以工作。封装这个删除器的好方法是把它实现为私有的嵌套类，如下例所示：
#include "boost/shared_ptr.hpp"
#include <iostream>
class A {
  class deleter {
    public:
      void operator()(A* p) {
        delete p;
      }
  };
  friend class deleter;
public:
  virtual void sing() {
    std::cout << "Lalalalalalalalalalala";
  }
  static boost::shared_ptr<A> createA() {
    boost::shared_ptr<A> p(new A(),A::deleter());
    return p;
  }
protected:
  virtual ~A() {};
};
int main() {
  boost::shared_ptr<A> p=A::createA();
}
注意，我们在这里不能使用普通函数来作为 shared_ptr<A> 的工厂函数，因为嵌套的删除器是A私有的。使用这个方法，用户不可能在栈上创建
A的对象，也不可能对A的指针调用
delete 。
#### 从this创建shared_ptr
有时候，需要从this获得
shared_ptr ，即是说，你希望你的类被shared_ptr所管理，你需要把"自身"转换为shared_ptr的方法。看起来不可能？好的，解决方案来自于我们即将讨论的另一个智能指针[boost::weak_ptr]().
weak_ptr 是 shared_ptr的一个观察者；它只是安静地坐着并看着它们，但不会影响引用计数。通过存储一个指向this的
weak_ptr 作为类的成员，就可以在需要的时候获得一个指向this的
shared_ptr。为了你可以不必编写代码来保存一个指向this的
weak_ptr，接着又从weak_ptr获shared_ptr得，Boost.Smart_ptr 为这个任务提供了一个助手类，称为
enable_shared_from_this. 只要简单地让你的类公有地派生自
enable_shared_from_this，然后在需要访问管理this的shared_ptr时，使用函数
shared_from_this 就行了。下面的例子示范了如何使用
enable_shared_from_this ：
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"
class A;
void do_stuff(boost::shared_ptr<A> p) {
  ...
}
class A : public boost::enable_shared_from_this<A> {
public:
  void call_do_stuff() {
    do_stuff(shared_from_this());
  }
};
int main() {
  boost::shared_ptr<A> p(new A());
  p->call_do_stuff();
}
这个例子还示范了你要用shared_ptr管理this的情形。类
A 有一个成员函数 call_do_stuff 需要调用一个普通函数
do_stuff, 这个普通函数需要一个类型为 boost:: shared_ptr<A>的参数。现在，在
A::call_do_stuff里, this 不过是一个
A指针, 但由于 A 派生自
enable_shared_from_this, 调用 shared_from_this 将返回我们所要的 shared_ptr 。在enable_shared_from_this的成员函数
shared_from_this里，内部存储的 weak_ptr 被转换为
shared_ptr, 从而增加了相应的引用计数，以确保相应的对象不会被删除。
#### 总结
引用计数智能指针是非常重要的工具。Boost的 shared_ptr 提供了坚固而灵活的解决方案，它已被广泛用于多种环境下。需要在使用者之间共享对象是常见的，而且通常没有办法通知使用者何时删除对象是安全的。shared_ptr 让使用者无需知道也在使用共享对象的其它对象，并让它们无需担心在没有对象引用时的资源释放。这对于Boost的智能指针类而言是最重要的。你会看到Boost.Smart_ptr中还有其它的智能指针，但这一个肯定是你最想要的。通过使用定制删除器，几乎所有资源类型都可以存入
shared_ptr。这使得shared_ptr 成为处理资源管理的通用类，而不仅仅是处理动态分配对象。与裸指针相比，shared_ptr会有一点点额外的空间代价。我还没有发现由于这些代价太大而需要另外寻找一个解决方案的情形。不要去创建你自己的引用计数智能指针类。没有比使用
shared_ptr智能指针更好的了。
在以下情况时使用 shared_ptr ：
- 
当有多个使用者使用同一个对象，而没有一个明显的拥有者时
- 
当要把指针存入标准库容器时
- 
当要传送对象到库或从库获取对象，而没有明确的所有权时
- 
当管理一些需要特殊清除方式的资源时[[9]]()
> 
[9] 通过定制删除器的帮助。
