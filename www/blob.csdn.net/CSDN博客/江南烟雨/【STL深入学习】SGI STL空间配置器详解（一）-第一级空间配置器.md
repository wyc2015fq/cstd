# 【STL深入学习】SGI STL空间配置器详解（一）-第一级空间配置器 - 江南烟雨 - CSDN博客
2012年12月02日 16:17:20[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：5510
**一、SGI STL配置器简介**
SGI STL的配置器与众不同，它与标准规范不同。如果要在程序中明确使用SGI配置器，那么应该这样写：
```cpp
vector<int,std::alloc> iv;
```
他的名字是alloc，而且不接受任何参数。标准配置器的名字是allocator，而且可以接受参数。
SGI STL的每一个容器都已经指定了缺省配置器：alloc。我们很少需要自己去指定空间配置器。比如vector容器的声明：
```cpp
template <class T, class Alloc = alloc>  // 预设使用alloc为配置器
class vector {
//...
}
```
**二、SGI标准的空间配置器**
其实SGI也定义了一个符合部分标准，名为allocator的配置器，但是它自己不使用，也不建议我们使用，主要原因是效率不佳。**它只是把C++的操作符::operator new和::operator delete做了一层简单的封装而已。**下面仅仅贴出代码：
```cpp
#ifndef DEFALLOC_H
#define DEFALLOC_H
#include <new.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream.h>
#include <algobase.h>
template <class T>
inline T* allocate(ptrdiff_t size, T*) {
    set_new_handler(0);
    T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
    if (tmp == 0) {
	cerr << "out of memory" << endl; 
	exit(1);
    }
    return tmp;
}
template <class T>
inline void deallocate(T* buffer) {
    ::operator delete(buffer);
}
template <class T>
class allocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    pointer allocate(size_type n) { 
	return ::allocate((difference_type)n, (pointer)0);
    }
    void deallocate(pointer p) { ::deallocate(p); }
    pointer address(reference x) { return (pointer)&x; }
    const_pointer const_address(const_reference x) { 
	return (const_pointer)&x; 
    }
    size_type init_page_size() { 
	return max(size_type(1), size_type(4096/sizeof(T))); 
    }
    size_type max_size() const { 
	return max(size_type(1), size_type(UINT_MAX/sizeof(T))); 
    }
};
class allocator<void> {
public:
    typedef void* pointer;
};
#endif
```
**三、SGI特殊的空间配置器alloc**
通常，C++中用new操作符来分配内存都包括两个阶段：
（1）调用::operator new配置内存
（2）调用构造函数来构造对象内容
同理，delete操作也包括两个阶段：
（1）调用析构函数将对象析构
（2）调用::operator delete释放内存
为了精密分工，**SGI allocator将两个阶段分开**：
内存配置操作由alloc:allocate负责，内存释放由alloc:deallocate负责；对象构造操作由::contructor()负责，对象析构由::destroy()负责。
配置器定义在头文件<memory>中，它里面又包括两个文件：
```cpp
#include <stl_alloc.h>		// 负责内存空间的配置和器释放
#include <stl_construct.h>		// 负责对象的构造和析构
```
下图显示了其结构：
![](https://img-my.csdn.net/uploads/201211/29/1354196694_9025.jpg)
图一 头文件 <memory>结构
**1、对象的建构和结构函数construct()和destroy()**
图二显示了这两个函数的结构和功能。他们被包含在头文件stl_construct.h中。
![](https://img-my.csdn.net/uploads/201211/29/1354196922_8237.jpg)
图二 函数construct()和destroy()示意图
函数construct()使用了定位new操作符，其源代码：
```cpp
template <class T1, class T2>
inline void construct(T1* p, const T2& value) {
  new (p) T1(value); 	// 定为new操作符placement new; 在指针p所指处构造对象
}
```
函数destroy则有两个版本。
第一个版本较简单，接受一个指针作为参数，直接调用对象的析构函数即可，其源代码：
```cpp
template <class T>
inline void destroy(T* pointer) {
    pointer->~T();	// 调用析构函数
}
```
第二个版本，其参数接受两个迭代器，将两个迭代器所指范围内的所有对象析构掉。而且，它采用了一种特别的技术：**依据元素的型别，判断其是否有trivial destructor（无用的析构函数）进行不同的处理**。这也是为了效率考虑。因为如果每个对象的析构函数都是trivial的，那么调用这些毫无作用的析构函数会对效率造成影响。
下面看其源代码：
```cpp
// 如果元素的数值型別（value type）有 non-trivial destructor…
template <class ForwardIterator>
inline void
__destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
  for ( ; first < last; ++first)
    destroy(&*first);//调用析构函数
}
// 如果元素的数值型別（value type）有 trivial destructor…
template <class ForwardIterator> 
inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type) {}//不调用析构函数
// 判断元素的数值型別（value type）是否有 trivial destructor，分别调用上面的函数进行不同的处理
template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
  typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
  __destroy_aux(first, last, trivial_destructor());
}
// 以下是 destroy() 第二版本，接受两个迭代器。它会设法找出元素的数值型別，
// 进而利用 __type_traits<> 求取最适当措施。
template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
  __destroy(first, last, value_type(first));
}
```
第二版本还针对迭代器为char*和wchar_t*定义了特化版本：
```cpp
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}
```
**2、空间的配置和释放,std::alloc**
对象构造前的空间分配和析构后的空间释放，定义在头文件<stl_alloc.h>中。其设计思想是：
- 向system heap要求空间。
- 考虑多线程状态。
- 考虑内存不足时的应变措施。
- 考虑过多“小额区块”可能造成的内存碎片问题。
考虑到小型区块可能造成的内存破碎问题，SGI设计了**双层级配置器**，第一级配置器直接使用malloc()和free()，第二级则视情况采用不同的策略。而且采用了复杂的内存池memory pool整理方式。整个设计究竟是只开放第一级配置器还是同事开放第二级配置器取决于宏__USE_MALLOC是否被定义。
```cpp
# ifdef __USE_MALLOC 
... 
typedef __malloc_alloc_template<0> malloc_alloc; //令 alloc为第一级配置器
typedef malloc_alloc alloc; 
# else 
... 
//令 alloc 为第二级配置器 
typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0> alloc; 
#endif /* ! __USE_MALLOC */
```
SGI并未定义它。
无论alloc被定义为第一级或者是第二级配置器，SGI还为它包装一个接口如下，使配置器的接口能够符合STL规格：
```cpp
template<class T, class Alloc>
class simple_alloc {
public:
    static T *allocate(size_t n)
                { return 0 == n? 0 : (T*) Alloc::allocate(n * sizeof (T)); }
    static T *allocate(void)
                { return (T*) Alloc::allocate(sizeof (T)); }
    static void deallocate(T *p, size_t n)
                { if (0 != n) Alloc::deallocate(p, n * sizeof (T)); }
    static void deallocate(T *p)
                { Alloc::deallocate(p, sizeof (T)); }
};
```
**SGI STL容器全部是使用这个simple_alloc接口。**
第一级和第二级配置器之间的关系如图三所示。
![](https://img-my.csdn.net/uploads/201212/02/1354435909_9029.jpg)
图三 第一级配置器和第二级配置器
第一级和第二级配置器的包装接口和运用方式如下：
![](https://img-my.csdn.net/uploads/201304/16/1366077394_2642.jpg)
**第一级配置器__malloc_alloc_template剖析**
第一级配置器直接使用malloc(),free(),realloc()等C函数执行实际的内存配置、释放、重配置操作，并实现出**类似C++ new handler机制**。它有独特的**out-of-memory内存处理机制**：在抛出std::bad_alloc异常之前，调用内存不足处理例程尝试释放空间，如果用户没有定义相应的内存不足处理例程，那么还是会抛出异常。详细实现见函数oom_malloc(),oom_realloc()。
内存不足处理例程保存在函数指针__malloc_alloc_oom_handler里面。
下面列出代码：
```cpp
#if 0 
#   include <new> 
#   define  __THROW_BAD_ALLOC throw bad_alloc 
#elif !defined(__THROW_BAD_ALLOC) 
#   include <iostream.h> 
#   define  __THROW_BAD_ALLOC cerr << "out of memory" << endl; exit(1) 
#endif 
 
// malloc-based allocator. 通常比稍后介绍的 default alloc 速度慢， 
//一般而言是 thread-safe，并且对于空间的运用比较高效（efficient）。 
//以下是第一级配置器。 
//注意，无「template 型别参数」。至于「非型别参数」inst，完全没派上用场。
template <int inst>   
class __malloc_alloc_template { 
 
private: 
//以下都是函式指标，所代表的函式将用来处理内存不足的情况。 
// oom : out of memory. 
static void *oom_malloc(size_t); 
static void *oom_realloc(void *, size_t); 
static void (* __malloc_alloc_oom_handler)(); 
 
public: 
 
static void * allocate(size_t n) 
{ 
    void  *result =malloc(n);//第一级配置器直接使用 malloc() 
    // 以下，无法满足需求时，改用 oom_malloc() 
    if (0 == result) result = oom_malloc(n); 
    return  result; 
} 
static void deallocate(void *p, size_t /* n */) 
{ 
free(p); //第一级配置器直接使用 free() 
} 
 
static void * reallocate(void *p, size_t /* old_sz */, size_t new_sz) 
{ 
    void  *  result  =realloc(p, new_sz);//第一级配置器直接使用 rea
    // 以下，无法满足需求时，改用 oom_realloc() 
    if (0 == result) result = oom_realloc(p, new_sz); 
    return  result; 
} 
 
//以下模拟 C++的 set_new_handler(). 换句话说，你可以透过它， 
//指定你自己的 out-of-memory handler 
static void (* set_malloc_handler(void (*f)()))() 
{ 
    void  (*  old)()  =  __malloc_alloc_oom_handler; 
__malloc_alloc_oom_handler = f; 
    return(old); 
} 
}; 
 
// malloc_alloc out-of-memory handling 
//初值为 0。有待客端设定。 
template <int inst> 
void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0; 
 
template <int inst> 
void * __malloc_alloc_template<inst>::oom_malloc(size_t n) 
{ 
    void  (* my_malloc_handler)(); 
    void  *result; 
 
    for (;;)  { 
 
//不断尝试释放、配置、再释放、再配置… 
my_malloc_handler = __malloc_alloc_oom_handler; 
        if  (0  ==  my_malloc_handler)  {  __THROW_BAD_ALLOC; } 
        (*my_malloc_handler)();//呼叫处理例程，企图释放内存。 
        result = malloc(n);  //再次尝试配置内存。 
        if  (result)  return(result); 
    } 
} 
 
template <int inst> 
void * __malloc_alloc_template<inst>::oom_realloc(void *p, size_t n) 
{ 
    void  (* my_malloc_handler)(); 
    void  *result; 
       for (;;)  {  //不断尝试释放、配置、再释放、再配置… 
my_malloc_handler = __malloc_alloc_oom_handler; 
        if  (0  ==  my_malloc_handler)  {  __THROW_BAD_ALLOC; } 
        (*my_malloc_handler)();//呼叫处理例程，企图释放内存。 
        result = realloc(p, n);//再次尝试配置内存。 
        if  (result)  return(result); 
    } 
} 
 
//注意，以下直接将参数 inst指定为 0。 
typedef __malloc_alloc_template<0> malloc_alloc;
```
需要注意的是：设计“内存不足处理例程”是客端的责任。关于设计“内存不足处理例程”的具体做法可参考《Effective
 C++（第二版）》条款7。
