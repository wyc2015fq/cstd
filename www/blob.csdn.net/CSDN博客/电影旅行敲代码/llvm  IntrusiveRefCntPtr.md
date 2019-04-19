# llvm:: IntrusiveRefCntPtr - 电影旅行敲代码 - CSDN博客
2018年04月14日 17:48:51[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：110
# [boost::intrusive_ptr](http://www.boost.org/doc/libs/1_61_0/libs/smart_ptr/intrusive_ptr.html)
在介绍`llvm::IntrusiveRefCntPtr`之前，先介绍`intrusive_ptr`的概念，`intrusive_ptr`也是智能指针的一种，同样避免内存泄漏的安全问题，并且`intrusive_ptr`也是基于RAII实现的。但`intrusive_ptr`与`shared_ptr`也有一些不同，最本质不同就是计数器存放的位置。对于`std::shared_ptr`，计数器并不是对象的数据成员，而是`std::shared_ptr`的数据成员，如下图所示，当然有时候Object和count所占用的内存是一起分配的，例如当使用`std::make_shared`的时候。 
![shared_ptr](https://img-blog.csdn.net/20180204200208008?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
而`intrusive_ptr`中的计数器是作为对象的数据成员存在的，并且`intrusive_ptr`只是提供了增减计数器的接口。`intrusive_ptr`中计数器与对象的示意图如下： 
![这里写图片描述](https://img-blog.csdn.net/20180204203833725?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
为了能够使count作为对象的数据成员存在，一种可行的实现方案是使所有欲使用`instrusive_ptr`的对象继承自一个专门”提供”count的类型，这个类型在boost中就是[`basic_intrusive_ref_counter`](http://www.boost.org/doc/libs/1_55_0/libs/smart_ptr/intrusive_ref_counter.html)，如下：
```cpp
namespace boost {
    struct thread_unsafe_counter;
    struct thread_safe_counter;
    template<class DrivedT, class CounterPolicyT = thread_safe_counter>
    class intrusive_ref_counter
    {
    public:
        intrusive_ref_counter() = noexecpt;
        intrusive_ref_counter(intrusive_ref_counter *r) = noexcept;
        intrusive_ref_counter & operator=(intrusive_ref_counter const & r) noexcept;
        unsigned int use_count() const noexcept;
    protected:
        ~intrusive_ref_counter() = default;
    };
}
```
提供count的任务从智能指针转移到`boost::intrusive_ref_counter`，那么`boost::intrusive_ptr`主要就是进行做好计数的工作，关于[`boost::intrusive_ptr`](http://www.boost.org/doc/libs/1_61_0/libs/smart_ptr/intrusive_ptr.html)的描述如下：
> 
The `intrusive_ptr` class template stores a pointer to an object with an embedded reference count. Every new `intrusive_ptr` instance increments the reference count by using an unqualified call to the function `intrusive_ptr_add_ref`, passing it the pointer as an argument. Similarly, when an `intrusive_ptr` is destroyed, it calls `intrusive_ptr_release`; this function is responsible for destroying the object when its reference count drops to zero. The user is expected to provide suitable definitions of these two functions.
从上面的描述中我们得到如下几点信息：
- `intrusive_ptr`本质上存储的是一个指向对象的指针，其中这个对象“内嵌“了计数器
- 每当`intrusive_ptr`由对象A创建时，就将对象A的计数加一
- 每当`intrusive_ptr`销毁时，其创建时对象的计数就减一，当计数减到0时，将该对象销毁
- 其中计数加一和减一的函数，`intrusive_ptr_add_ref()`和`intrusive_ptr_release()`的实现由用户提供
`instrusive_ptr`的声明大致如下：
```cpp
namespace boost {
    template<class T> element_type;
    intrusive_ptr(); // never throws
    intrusive_ptr(T *p, bool add_ref = true);
    intrusive_ptr(intrusive_ptr const & r)l
    template<class Y> intrusive_ptr(intrusive_ptr<T> const &r);
    ~intrusive_ptr();
    T & operator*() const;
    T *operator->() const;
    T *get() const;
    T *detach();
};
```
其中对于上述的构造函数和析构函数，细节如下：
> 
`intrusive_ptr(T * p, bool add_ref = true);`
***Effects***: `if (p != 0 && add_ref) intrusive_ptr_add_ref(p);`
***Postconditions***: `get() == p`
`intrusive_ptr(intrusive_ptr const & r);`
`template<class Y> intrusive_ptr(intrusive_ptr<Y> const & r);`
***Effects***: `if (r.get() != 0) intrusive_ptr_add_ref(r.get());`
***Postconditions***: `get() == r.get()`
`~intrusive_ptr();`
***Effects***: `if(get() != 0) intrusive_ptr_release(get())`
另外对于`intrusive_ptr_add_ref()`和`intrusive_ptr_release()`，如果编译器支持ADL（Argument Dependent Lookup），就会在它们的参数所定义的namespace中查找二者的定义，否早它们必须定义在boost namespace中。
# `intrusive_ptr` vs `std::shared_ptr`
在[intrusive_ptr class template](http://www.boost.org/doc/libs/1_61_0/libs/smart_ptr/intrusive_ptr.html)中有如下两段描述，第一段描述使用`intrusive_ptr`的主要原因，第二段说明在`intrusive_ptr`和`std::shared_ptr`相同的情况下，尽量使用`std::shared_ptr`。
> 
The main reasons to use `intrusive_ptr` are:
- Some existing frameworks or Oses provide objects with embedded reference counts;
- The memory footprint of `intrusive_ptr` is the same as the corresponding raw pointer;
- `intrusive_ptr<T>` can be constructed from an arbitrary raw pointer of type `T*`.
As a general rule, if it isn’t obvious whether `intrusive_ptr` better fits your needs than `shared_ptr`, try a `shared_ptr-`based design first.
对于第一段中第二个原因需要说明一下，为什么`intrusive_ptr`占用的内存比`shared_ptr`少，原因在于`std::shared_ptr`理念上对象和计数的存放是分开的，所以大部分情况下每个`std::shared_ptr`有两个指针的内存开销，一个指针指向control block（存储引用计数等信息），一个指针指向对象。之所以说大部分情况，是因为`std::make_shared<>`会统一分配一整块内存存储计数和对象，此时就只需存储一个指针。
> 
**Memory footprint** refers to the amount of main memory that a program uses or references while running. [[Memory footprint]](https://en.wikipedia.org/wiki/Memory_footprint)
## `std::shared_ptr`中的`std::make_shared<>`
前面提到了`std::make_shared<>`会为计数和对象分配一整块内存，虽然标准中没有这样规定，但是由于`std::make_shared`提供了这样的一个将计数和对象放在一起的机会，所以基本上所有的实现都会这样做。
> 
std::shared_ptr<T>(new T(args…)) performs **at least two allocations (one for the object T and one for the control block of the shared pointer)**, while std::make_shared<T> typically **performs only one allocation (the standard recommends, but does not require this, all known implementations do this)**. [[make_shared]](http://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared)
**The storage is typically larger than sizeof(T) in order to use one allocation for both the control block of the shared pointer and the T object.** The std::shared_ptr constructor called by this function enables shared_from_this with a pointer to the newly constructed object of type T. [[make_shared]](http://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared)
`std::make_shared`所带来的好处和坏处都是由这一个特点引发的，通常意义上使用`std::make_shared`是好的，不仅包裹了内存的分配，不会将raw pointer暴露出来，避免了访问已释放内存的问题，同时也减少了内存的分配。stackoverflow上有一个相关的问题，见[Difference in make_shared and normal shared_ptr in C++](https://stackoverflow.com/questions/20895648/difference-in-make-shared-and-normal-shared-ptr-in-c)，比较详尽。
了解了`std::make_shared`的好处以后，`intrusive_ptr`的第三个优点也就显而易见了。使用一个raw pointer来构造`std::shared_ptr`通常不能保证是安全的，反而更容易出错，因为raw pointer有可能在其他地方被使用，而`std::shared_ptr`不能为这些使用增加计数或减少计数。而`intrusive_ptr`就不会存在这个问题，因为raw pointer本身就存储有计数信息。
知乎上有一个`intrusive_ptr`的问题，[为什么 intrusive_ptr 没有进入标准库？](https://www.zhihu.com/question/25271180/answer/30379106)，该问题下设提到了`std::enable_shared_from_this`，由于我在工作中没有遇到很多使用`std::enable_shared_from_this`的代码场景，理解不够深刻，所以先挖个坑。
# `llvm::IntrusiveRefCntPtr`
待填
