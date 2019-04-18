# 几个方便编程的C++特性 - wishchinYang的专栏 - CSDN博客
2014年03月28日 14:07:21[wishchin](https://me.csdn.net/wishchin)阅读数：640
个人分类：[C++编程																[BOOST/FlANN/Eigen/C+0X](https://blog.csdn.net/wishchin/article/category/1644381)](https://blog.csdn.net/wishchin/article/category/1508333)
前言：
        C++11的自动化特性给编程提供了不少方便，同时也给调试增加了很多负担，至于取舍看程序员的风格和侧重而定。
auto：自动类型推断
　　在C++11之前，auto关键字用来指定[存储](http://storage.chinabyte.com/)期。在新标准中，它的功能变为类型推断。auto现在成了一个类型的占位符，通知编译器去根据初始化 代码推断所声明变量的真实类型。各种作用域内声明变量[都可以](http://www.chinabyte.com/keyword/%E9%83%BD%E5%8F%AF%E4%BB%A5/)用到它。例如，名空间中，程序块中，或是for循环的初始化语句中。
　　auto i = 42; //[i is](http://www.chinabyte.com/keyword/IIS/) an int auto l = 42LL; // l is an long long auto p = new foo(); // p is a foo*
　　使用auto通常意味着更短的代码(除非你[所用](http://www.chinabyte.com/keyword/%E6%89%80%E7%94%A8/)类型是int，它会比auto少一个字母)。试想一下当你遍历STL容器时需要声明的那些迭代器(iterator)。现在不需要去声明那些typedef就可以得到简洁的代码了。
　　std::map> map; for(auto it = begin(map); it != end(map); ++it) { }
　　需要注意的是，auto不能用来声明函数的返回值。但如果函数有一个尾随的返回类型时，auto是可以出现在函数声明中返回值位置。这种情况下，auto 并不是告诉编译器去推断返回类型，而是指引编译器去函数的末端寻找返回值类型。在下面这个例子中，函数的返回值类型就是operator+操作符作用在 T1、T2类型变量上的返回值类型。
　　template auto compose(T1 t1, T2 t2) -> decltype(t1 + t2) { return t1+t2; } auto v = compose(2, 3.14); // v's type is double
       注：最主要的是 在小范围迭代时 不用刻意去注意其类型，节省了编程时间....
nullptr 空指针？
　　以前都是用0来表示空指针的，但由于0可以被隐式类型转换为整形，这就会存在一些问题。关键字nullptr是std::nullptr_t类型的 值，用来指代空指针。nullptr和任何指针类型以及类成员指针类型的空值之间可以发生隐式类型转换，同样也可以隐式转换为bool型(取值为 false)。但是不存在到整形的隐式类型转换。
　　void foo(int* p) {} void bar(std::shared_ptr p) {} int* p1 = NULL; int* p2 = nullptr; if(p1 == p2) { } foo(nullptr); bar(nullptr); bool f = nullptr; int i = nullptr; // error: A native nullptr can only be converted to bool or, using reinterpret_cast, to
 an integral type
　　为了向前兼容，0仍然是个合法的空指针值。
基于范围的for循环：
         C++使用了800行STL代码就是为了让一个遍历语句更像python这种动态语言.
　　为了在遍历容器时支持”foreach”用法，C++11扩展了for语句的语法。用这个新的写法，可以遍历C类型的数组、初始化列表以及任何重载了非成员的begin()和end()函数的类型。（这是向动态语言看齐啊...）
　　如果你只是想对集合或数组的每个元素做一些操作，而不关心下标、迭代器位置或者元素个数，那么这种foreach的for循环将会非常有用。
```cpp
std::map> map;
std::vector v;
v.push_back(1);v.push_back(2); v.push_back(3);
map["one"] = v;
for(const auto& kvp : map) 
{
   std::cout << kvp.first << std::endl;
   for(auto v : kvp.second) 
{
      std::cout << v << std::endl;
    } 
 }
int arr[] = {1,2,3,4,5};
for(int& e : arr) 
{
    e = e*e;
} //C++ 11 CPP 11 features
```
Smart Pointers 智能指针，有计数的指针.
　　已经有成千上万的文章讨论这个问题了，所以我只想说：现在能使用的，带引用计数，并且能自动释放内存的智能指针包括以下几种：
　　unique_ptr: 如果内存资源的所有权不需要共享，就应当使用这个(它没有拷贝构造函数)，但是它可以转让给另一个unique_ptr(存在move构造函数)。
　　shared_ptr: 如果内存资源需要共享，那么使用这个(所以叫这个名字)。
　　weak_ptr: 持有被shared_ptr所管理对象的引用，但是不会改变引用计数值。它被用来打破依赖循环(想象在一个tree结构中，父节点通过一个共享所有权的引 用(chared_ptr)引用子节点，同时子节点又必须持有父节点的引用。如果这第二个引用也共享所有权，就会导致一个循环，最终两个节点内存都无法释 放)。
　　另一方面，auto_ptr已经被废弃，不会再使用了。
　　什么时候使用unique_ptr，什么时候使用shared_ptr取决于对所有权的需求，我建议阅读以下的讨 论：http://stackoverflow.com/questions/15648844/using-smart-pointers-for- class-members
　　以下第一个例子使用了unique_ptr。如果你想把对象所有权转移给另一个unique_ptr，需要使用std::move(我会在最后几段讨论这个函数)。在所有权转移后，交出所有权的智能指针将为空，get()函数将返回nullptr。
　　void foo(int* p) { std::cout << *p << std::endl; } std::unique_ptr p1(new int(42)); std::unique_ptr p2 = std::move(p1); // transfer ownership if(p1) foo(p1.get()); (*p2)++; if(p2) foo(p2.get());
　　第二个例子展示了shared_ptr。用法相似，但语义不同，此时所有权是共享的。
　　void foo(int* p) { } void bar(std::shared_ptr p) { ++(*p); } std::shared_ptr p1(new int(42)); std::shared_ptr p2 = p1; bar(p1); foo(p2.get());
　　第一个声明和以下这行是等价的：
　　auto p3 = std::make_shared(42);
　　make_shared是一个非成员函数，使用它的好处是可以一次性分配共享对象和智能指针自身的内存。而显示地使用 shared_ptr构造函数来构造则至少需要两次内存分配。除了会产生额外的开销，还可能会导致内存泄漏。在下面这个例子中，如果seed()抛出一个 错误就会产生内存泄漏。
　　void foo(std::shared_ptr p, int init) { *p = init; } foo(std::shared_ptr(new int(42)), seed());
　　如果使用make_shared就不会有这个问题了。第三个例子展示了weak_ptr。注意，你必须调用lock()来获得被引用对象的shared_ptr，通过它才能访问这个对象。
　　auto p = std::make_shared(42); std::weak_ptr wp = p; { auto sp = wp.lock(); std::cout << *sp << std::endl; } p.reset(); if(wp.expired()) std::cout << "expired" << std::endl;
　　如果你试图锁定(lock)一个过期(指被弱引用对象已经被释放)的weak_ptr，那你将获得一个空的shared_ptr.
Lambdas (省去了 写大量的函数对象)
       思想：函数就近使用原则.
　　匿名函数(也叫lambda)已经加入到C++中，并很快异军突起。这个从函数式编程中借来的强大特性，使很多其他特性以及类库得以实现。你可以在 任何使用函数对象或者函子(functor)或std::function的地方使用lambda。你可以从这里 (http://msdn.[microsoft](http://com.chinabyte.com/microsoft/).com/en-us/library/dd293603.aspx)找到语法说明。
```cpp
std::vector v; 
v.push_back(1); 
v.push_back(2); 
v.push_back(3); 
std::for_each(std::begin(v), std::end(v), [](int n) 
{
    std::cout << n << std::endl;}); 
    auto is_odd = [](int n) {return n%2==1;
}; 
auto pos = std::find_if(std::begin(v), std::end(v), is_odd); if(pos != std::end(v)) std::cout << *pos << std::endl;
```
　　更复杂的是递归lambda。考虑一个实现Fibonacci函数的lambda。如果你试图用auto来声明，就会得到一个编译错误。
Move semantics (Move语义)
 移动语义避免了移动原始数据，只是修改了记录...
　　这是C++11中所涵盖的另一个重要话题。就这个话题可以写出一系列文章，仅用一个段落来说明显然是不够的。因此在这里我不会过多的深入细节，如果你还不是很熟悉这个话题，我鼓励你去阅读更多地资料。
　　C++11加入了右值引用(rvalue reference)的概念(用&&标识)，用来区分对左值和右值的引用。左值就是一个有名字的对象，而右值则是一个无名对象(临时对 象)。move语义允许修改右值(以前右值被看作是不可修改的，等同于const T&类型)。
　　C++的class或者struct以前都有一些隐含的成员函数：默认构造函数(仅当没有显示定义任何其他构造函数时才存在)，拷贝构造函数，析构 函数还有拷贝赋值操作符。拷贝构造函数和拷贝赋值操作符提供bit-wise的拷贝(浅拷贝)，也就是逐个bit拷贝对象。也就是说，如果你有一个类包含 指向其他对象的指针，拷贝时只会拷贝指针的值而不会管指向的对象。在某些情况下这种做法是没问题的，但在很多情况下，实际上你需要的是深拷贝，也就是说你 希望拷贝指针所指向的对象。而不是拷贝指针的值。这种情况下，你需要显示地提供拷贝构造函数与拷贝赋值操作符来进行深拷贝。
　　如果你用来初始化或拷贝的源对象是个右值(临时对象)会怎么样呢？你仍然需要拷贝它的值，但随后很快右值就会被释放。这意味着产生了额外的操作开销，包括原本并不需要的空间分配以及内存拷贝。
　　现在说说move constructor和move assignment operator。这两个函数接收T&&类型的参数，也就是一个右值。在这种情况下，它们可以修改右值对象，例如“偷走”它们内部指针所 指向的对象。举个例子，一个容器的实现(例如vector或者queue)可能包含一个指向元素数组的指针。当用一个临时对象初始化一个对象时，我们不需 要分配另一个数组，从临时对象中把值复制过来，然后在临时对象析构时释放它的内存。我们只需要将指向数组内存的指针值复制过来，由此节约了一次内存分配， 一次元数组的复制以及后来的内存释放。
