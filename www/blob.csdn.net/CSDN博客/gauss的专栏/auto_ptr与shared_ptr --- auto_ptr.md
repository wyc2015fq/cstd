# auto_ptr与shared_ptr --- auto_ptr - gauss的专栏 - CSDN博客
2013年01月04日 00:04:36[gauss](https://me.csdn.net/mathlmx)阅读数：245
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)
Roger ([roger2yi@gmail.com](mailto:roger2yi@gmail.com))
这篇文章试图说明如何使用auto_ptr和shared_ptr，从而使得动态分配对象的使用和管理更安全，方便。除了一般的使用说明外，更主要是说明它们之间的异同 —— 满足需求的不同和开销上的差异。
文章的多数知识都来源于：
<!--[if !supportLists]-->1． <!--[endif]-->Exceptional C++（Herb）Item 37 auto_ptr
<!--[if !supportLists]-->2． <!--[endif]-->Exceptional C++ Style（Herb）和C++ Coding Standard（Herb，Andrei）其中一些关于使用shared_ptr的论述
<!--[if !supportLists]-->3． <!--[endif]-->GC++和VC++ 8.0 auto_ptr的源代码
<!--[if !supportLists]-->4． <!--[endif]-->Boost库shared_ptr的源码和文档
auto_ptr和shared_ptr都是智能指针的一种实现，所谓智能指针，多数情况下都是指这样的一些对象：
<!--[if !supportLists]-->1． <!--[endif]-->内部有一个动态分配对象的指针，拥有该对象的使用权和所有权（独占或共享）。
<!--[if !supportLists]-->2． <!--[endif]-->重载*和->操作，行为上跟所拥有的对象的指针一致。
<!--[if !supportLists]-->3． <!--[endif]-->当自身的生命期结束的时候，会做一些跟拥有对象相关的清理动作。
***auto_ptr***
auto_ptr是现在标准库里面一个轻量级的智能指针的实现，存在于头文件 memory中，之所以说它是轻量级，是因为它只有一个成员变量（拥有对象的指针），相关的调用开销也非常小。
下面的代码来自于VC++ 8.0里面的源码：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/rogeryi/89150/o_auto_ptr_code.JPG)
里面有个auto_ptr_ref的数据结构，我们可以把它忽略，这个只是内部使用的代理结构，用于一些隐式的const变化，我们客户端代码通常不会直接使用到它。
我们可以看到除了构造函数，拷贝构造函数，赋值函数，析构函数和两个重载操作符（*，->）外，还有get，release和reset三个函数，它们的作用分别是：
<!--[if !supportLists]-->1． <!--[endif]-->get，获得内部对象的指针
<!--[if !supportLists]-->2． <!--[endif]-->release，放弃内部对象的所有权，将内部指针置为空
<!--[if !supportLists]-->3． <!--[endif]-->reset，销毁内部对象并接受新的对象的所有权（如果使用缺省参数的话，也就是没有任何对象的所有权）
下面的例程来自Exceptional C++，Item 37：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// Example 2: Using an auto_ptr
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void g()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     T* pt1 =new T;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    // right now, we own the allocated object
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    // pass ownership to an auto_ptr
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     auto_ptr<T> pt2( pt1 );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    // use the auto_ptr the same way
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    // we'd use a simple pointer
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    *pt2 =12;       // same as "*pt1 = 12;"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     pt2->SomeFunc(); // same as "pt1->SomeFunc();"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// use get() to see the pointer value
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     assert( pt1 == pt2.get() );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// use release() to take back ownership
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     T* pt3 = pt2.release();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// delete the object ourselves, since now
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// no auto_ptr owns it any more
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     delete pt3;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}// pt2 doesn't own any pointer, and so won't
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// try to delete it... OK, no double delete
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// Example 3: Using reset()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void h()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     auto_ptr<T> pt( new T(1) );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     pt.reset( new T(2) );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// deletes the first T that was
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// allocated with "new T(1)"
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}// finally, pt goes out of scope and
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// the second T is also deleted
从上面的例子来看，auto_ptr的使用很简单，通过构造函数拥有一个动态分配对象的所有权，然后就可以被当作对象指针来使用，当auto_ptr对象被销毁的时候，它也会自动销毁自己拥有所有权的对象（嗯，标准的RAAI做法），release可以用来手动放弃所有权，reset可用于手动销毁内部对象。
但实际上，auto_ptr是一个相当容易被误用并且在实际中常常被误用的类。原因是由于它的对象所有权占用的特性和它非平凡的拷贝行为。
auto_ptr的对象所有权是独占性的！
这决定了不可能有两个auto_ptr对象同时拥有同一动态对象的所有权，从而也导致了auto_ptr的拷贝行为是非对等的，其中伴随着对象所有权的转移。
我们仔细观察auto_ptr的源码就会发现拷贝构造和赋值操作符所接受的参数类型都是非const的引用类型（auto_ptr<_Ty>&
），而不是我们一般应该使用的const引用类型，查看源码我们会发现：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)         auto_ptr(auto_ptr<_Ty>& _Right) _THROW0()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)                 : _Myptr(_Right.release())
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){        // construct by assuming pointer from _Right auto_ptr
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)                 }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)         template<class _Other>
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)                 auto_ptr<_Ty>&operator=(auto_ptr<_Other>& _Right) _THROW0()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){        // assign compatible _Right (assume pointer)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                 reset(_Right.release());
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return (*this);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)                 }
拷贝过程中被拷贝的对象（_Right）都会被调用release来放弃所包括的动态对象的所有权，动态对象的所有权被转移了，新的auto_ptr独占了动态对象的所有权。也就是说被拷贝对象在拷贝过程中会被修改，拷贝物与被拷贝物之间是非等价的。这意味着如下的代码是错误的（例程来自 Exceptional C++ Item 37）：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// Example 6: Never try to do work through
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//            a non-owning auto_ptr
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void f()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     auto_ptr<T> pt1( new T );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     auto_ptr<T> pt2;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     pt2 = pt1; // now pt2 owns the pointer, and
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// pt1 does not
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     pt1->DoSomething();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// error: following a null pointer
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
同时也不要将auto_ptr放进标准库的容器中，否则在标准库容器无准备的拷贝行为中（标准库容器需要的拷贝行为是等价的），会导致难以发觉的错误。（请参考Exceptional C++ Item 37获得更多信息）
auto_ptr特殊的拷贝行为使得使用它来远距离传递动态对象变成了一件十分危险的行为，在传递的过程中，一不小心就会留下一些实际为空但程序本身却缺少这样认知的auto_ptr对象。
简单的说来，auto_ptr适合用来管理生命周期比较短或者不会被远距离传递的动态对象，使用auto_ptr来管理动态分配对象，最好是局限于某个函数内部或者是某个类的内部。也就是说，动态对象的产生，使用和销毁的全过程是处于一个小的受控的范围，而不会在其中加入一些适应未来时态的扩展。
Roger ([roger2yi@gmail.com](mailto:roger2yi@gmail.com))
[auto_ptr与shared_ptr --- auto_ptr (1)](http://blog.csdn.net/rogeryi/archive/2006/12/14/1442700.aspx)
***shared_ptr***
shared_ptr是Boost库所提供的一个智能指针的实现，正如其名字所蕴意的一样：
An important goal of shared_ptr is to provide a standard shared-ownership pointer.
shared_ptr的一个重要目的就是为了提供一个标准的共享所有权的智能指针。
—— Boost库文档
没错，shared_ptr就是为了解决auto_ptr在对象所有权上的局限性（auto_ptr是独占的），在使用引用计数的机制上提供了可以共享所有权的智能指针，当然这不会没有任何额外的代价……
首先一个shared_ptr对象除了包括一个所拥有对象的指针（px）外，还必须包括一个引用计数代理对象（shared_count）的指针（pn）。而这个引用计数代理对象包括一个真正的多态的引用计数对象（sp_counted_base）的指针（_pi）,真正的引用计数对象在使用VC编译器的情况下包括一个虚表，一个虚表指针，和两个计数器。
下图中result是一个shared_ptr对象，我们可以清楚看到它展开后所包含的数据：
<!--[if !vml]-->![](https://p-blog.csdn.net/images/p_blog_csdn_net/rogeryi/89150/o_shared_ptr_data.JPG)<!--[endif]-->
假设我们有多个（5个以上）shared_ptr共享一个动态对象，那么每个shared_ptr的开销比起只使用原生指针的开销大概在3，4倍左右（这还是理想状况，忽略了动态分配带来的俑余开销）。如果只有一个shared_ptr独占动态对象，空间上开销更是高度十数倍！而auto_ptr的开销只是使用原生指针的两倍。
时间上的开销主要在初始化和拷贝操作上，*和->操作符重载的开销跟auto_ptr是一样的。
当然开销并不是我们不使用shared_ptr的理由，永远不要进行不成熟的优化，直到性能分析器告诉你这一点，这是Hurb提出的明智的建议。以上的说明只是为了让你了解强大的功能背后总是伴随着更多的开销，shared_ptr应该被使用，但是也不要过于滥用，特别是在一些auto_ptr更擅长的地方。
下面是shared_ptr的类型定义：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif) template<class T>class shared_ptr {
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)public:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      typedef T element_type;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      shared_ptr(); // never throws
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      template<class Y>explicit shared_ptr(Y * p);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      template<class Y, class D> shared_ptr(Y * p, D d);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)~shared_ptr(); // never throws
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      shared_ptr(shared_ptr const& r); // never throws
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      template<class Y> shared_ptr(shared_ptr<Y>const& r); // never throws
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      template<class Y>explicit shared_ptr(weak_ptr<Y>const& r);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      template<class Y>explicit shared_ptr(std::auto_ptr<Y>& r);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      shared_ptr &operator=(shared_ptr const& r); // never throws 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      template<class Y> shared_ptr &operator=(shared_ptr<Y>const& r); // never throws
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      template<class Y> shared_ptr &operator=(std::auto_ptr<Y>& r);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)void reset(); // never throws
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      template<class Y>void reset(Y * p);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      template<class Y, class D>void reset(Y * p, D d);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      T &operator*() const; // never throws
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      T *operator->() const; // never throws
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      T *get() const; // never throws
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)bool unique() const; // never throws
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)long use_count() const; // never throws
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)operator unspecified-bool-type() const; // never throws
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)void swap(shared_ptr & b); // never throws
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif) };
大多数成员函数都跟auto_ptr类似，但是没有了release（请看注释），reset用来放弃所拥有对象的所有权或拥有对象的变更，会引起原有对象的引用计数的减少。
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Note：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Boost文档里面的QA说明了为什么不提供release函数
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Q. Why doesn't shared_ptr provide a release() function?
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)A. shared_ptr cannot give away ownership unless it's unique() because the other copy will still destroy
 the object.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Consider:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)shared_ptr<int> a(newint);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)shared_ptr<int> b(a); // a.use_count() == b.use_count() == 2
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int* p = a.release();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// Who owns p now? b will still call delete on it in its destructor.
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Furthermore, the pointer returned by release() would be difficult to deallocate reliably,
as the source shared_ptr could have been created with a custom deleter.
use_count返回引用计数的个数，unique拥于确认是否是独占所有权（use_count为1），swap用于交换两个shared_ptr对象（即交换所拥有的对象），有一个bool类型转换操作符使得shared_ptr可用于需要的bool类型的语境下，比如我们通常用if(pointer)来判断某个指针是否为空。
Boost库里面有很多shared_ptr的使用例程，文档里面也列举了许许多多shared_ptr的用途，其中最有用也最常用的莫过于传递动态分配对象，有了引用计数机制，我们现在可以安全地将动态分配的对象包裹在shared_ptr里面跨越模块，跨越线程的边界传递。shared_ptr为我们自动管理对象的生命周期，嗯，C++也可以体会到Java里面使用引用的美妙之处了。
另外，还记得Effective C++里面（或者其它的C++书籍），Scott Meyer告诉你的：在一个由多个模块组成的系统里面，一个模块不用试图自己去释放另外一个模块分配的资源，而应该遵循谁分配谁释放的原则。正确的原则但是有时难免有时让人忽略（过于繁琐），将资源包装在shared_ptr里面传递，而shared_ptr保证了在资源不再被拥有的时候，产生资源的模块的delete语句会被调用。
shared_ptr是可以拷贝和赋值的，拷贝行为也是等价的，并且可以被比较，这意味这它可被放入标准库的一般容器（vector，list）和关联容器中（map）。
shared_ptr可以用来容纳多态对象，比如所下面的例子：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)class Base
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)class Derived : public Base
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)shared_ptr<Base> sp_base(new Derived);
甚至shared_ptr也具备多态的行为：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Derived* pd =new Derived;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)shared_ptr<Derived> sp_derived(pd);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)shared_ptr<Base> sp_base2(sp_derived);
上面的语句是合法的，shared_ptr会完成所需的类型转换，当shared_ptr的模版参数Base的确是Derived的基类的时候。
最后是一个小小的提醒，无论是使用auto_ptr还是shared_ptr，都永远不要写这样的代码：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)A* pa =new A;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)xxx_ptr<A> ptr_a_1(pa);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)xxx_ptr<A> ptr_a_2(pa);
很明显，在ptr_a_1和ptr_a_2生命周期结束的时候都会去删除pa，pa被删除了两次，这肯定会引起你程序的崩溃，当然，这个误用的例子比较明显，但是在某种情况下，可能会一不小心就写出如下的代码（嗯，我承认我的确做过这样的事情）：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void DoSomething(xxx_ptr<A>)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//do something
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)class A
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         doSomething()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                 xxx_ptr<A> ptr_a(this);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)                 DoSomething(ptr_a);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)         }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int main()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif){
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         A a;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         a.doSomething();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//continue do something with a, but it was already destory
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
在函数a.doSomething()里面发生了什么事情，它为了调用DoSomething所以不得不把自己包装成一个xxx_ptr，但是忘记在函数结束的时候，xxx ptr_a被销毁的同时也销毁了自己，程序或者立刻崩溃或者在下面的某个时间点上崩溃！
所以你在使用智能指针做为函数参数的时候请小心这样的误用，有时候使用智能指针作为函数参数不一定是一个好注意。比如请遵循下面的建议，请不要在属于类型A的接口的一部分的非成员函数或者跟A有紧密联系的辅助函数里面使用xxx_ptr<A>作为函数的参数类型
            
