# STL中string的源码解读 - gauss的专栏 - CSDN博客
2013年01月08日 13:15:43[gauss](https://me.csdn.net/mathlmx)阅读数：656
STL中string的源码解读
Ryan peng
[cutezero@163.com](mailto:cutezero@163.com)
Sunday, June 03, 2007
这是个人最近比较闲暇之余，对SGI STL中string分析，如果有任何理解错误，请和我联系，谢谢！
为什么要分析string呢？我们知道大多数的编译器实现的string都各不相同（即便是同一个编译厂商在不同的版本string的实现也不一样，例如MSVC6.0和VS2005中string的实现就不一样，VC6.0中string的实现是用copy_on_write[COW,写时拷贝]，VS2005则是直接进行深拷贝）。
以前各个编译器的厂商对String的实现都是使用COW，但目前string的实现趋势是直接使用深拷贝，不再使用COW，其主要原因是目前多线程的使用越来越多，COW技术在多线程会带来额外的性能恶化（原因在于COW在成员函数内部加/解锁），但是不可否认的是，掌握这些技术仍然很有吸引力，也许会在其他地方使用到。
以SGI STL版本3.4.2为主，阅读string的源代码。本文主要分以下几个部分：
Ø         原子操作的作用和实现；
Ø         STL中的concepts；
Ø         String概述；
Ø         实现计数的结构体Rep_Base和Rep；
«        Rep_Base的定义;
«        Rep的定义；
«        Rep中的几个主要函数;
Ø         Basic_string的构造函数和析构函数；
Ø         赋值构造函数operator=;
Ø         replace函数；
Ø         insert和erase函数；
Ø         swap函数；
Ø         Operator[]函数；
Ø         Reverse和resize函数；
Ø         Swap函数；
Ø         Append和operator+函数；
Ø         其他函数；
Ø         调试版本的string。
最初为了描述的方便，把一些函数/变量进行了修改或者删减，后来发现太麻烦了，就偷懒了，可能导致前后文中的变量和函数名不一致。最开始的时候都是是使用字符串来表示，后来发现可能引起大家的误解，因此后来的描述区分的比较清楚分别使用string对象/string，或者c_style的字符串，但是前面写的可能需要根据上下文自己判断了，不好意思了。
1.原子操作的作用和实现
在string中真正存储的字符串使用COW，也就是说当两个字符串完全相同时可能（不是一定，取决于编译器实现string赋值和copy构造的方法）指向的是同一块内存，当其中一个string对象被修改时，才为这个string对象建立真正的copy（即分配内存，并初始化对象），很明显在单线程情况下，该方法很有效，因为分配内存很耗时间，而且也可能节约内存。COW用的非常广泛，如Linux下的fork()函数在创建新进程的时候也是使用COW，让子进程和父进程共享同一进程控制块。
在string中为了实现COW，必须记录有多少个对象指向真实的字符串（实际的存储体在下面会看到对应的数据结构），才能正确的操作string对象。为了正确的实现提供原子操作（即要么操作成功，要么什么也不做），如修改这个记录的时候需要原子操作，调用一些成员函数的时候需要原子操作。
一般来说原子操作都是操作系统提供的，当然也可以直接通过汇编代码控制CPU来完成。操作系统一般都会提供mutex，atom，lock等操作，但是不同的操作系统提供的接口不同，也就需要对这些接口进行封装。这些内容一般在GCC的头文件atomicity.h找到。
在此只简单的演示两种方法，看看是怎么实现的，如下：一个例子（Redhat中的实现）完成引用计数值修改的原子操作，
__exchange_and_add(volatile _Atomic_word* __mem, int __val)
{
        __glibcxx_mutex_lock(__gnu_internal::atomic_mutex);
        Int __result;
        __result = *__mem;
        *__mem += __val;
        __glibcxx_mutex_unlock(__gnu_internal::atomic_mutex);
        return __result;
}
其中__glibcxx_mutex_lock和__glibcxx_mutex_unlock都是封装好的操作，保证原子操作（如调用操作系统的函数pthread_mutex_lock, pthread_mutex_unlock, pthread_mutex_trylock）。
如果没有定义上面的函数（准确的说是通过宏调用操作系统的函数），也可以直接通过汇编代码完成，如下所示：
__exchange_and_add (volatile _Atomic_word *__mem, int __val)
{
register int __result;
__asm__ __volatile__ ("lock; xaddl %0,%2"
                                : "=r" (__result) 
: "0" (__val), "m" (*__mem) 
: "memory");
return __result;
}
关于C/C++语言中嵌套ASM汇编有很多资料可以参考，这里简单分析一下上面的代码：
lock; 
//lock；汇编指令前缀，表示后面的指令在CPU上操作是串行完成；当CPU中的控制器检测到这个前缀时候，就会锁定内存总线，一直到该条指令执行完毕，在此期间其它的CPU不能访问这条指令所访问的内存单元。
xaddl %0,%2"
//完成加法，其中0表示result，2表示*__mem
: "=r" (__result)
//表示输出
: "0" (__val), "m" (*__mem)
//表示输入，__val和__result使用同一个寄存器，*__mem表示在内存中；
    : "memory")
    //表示限定约束，memory中的内容被修改
值得注意的是不同类型的CPU对应的汇编也不同，上面的汇编是对应i386结构的。
为了能更好的说明这一部分，再举一个简单的例子，这个是针对m68000的实现，利用了C++语言的性质，如下：
template<int __inst>
struct _Atomicity_lock
{
static volatile unsigned char _S_atomicity_lock;
};
template<int __inst>
volatile unsigned char _Atomicity_lock<__inst>::_S_atomicity_lock = 0;
__exchange_and_add(volatile _Atomic_word* __mem, int __val)
{
    int __result;
// Use bset with immediate addressing for 68000/68010 (not SMP-safe)
__asm__ __volatile__("1: bset.b #7,%0/n/tjbne 1b"
                       : "+m"(_Atomicity_lock<0>::_S_atomicity_lock)
                       : /* none */
                       : "cc");
__result = *__mem;
__mem = __result + __val;
_Atomicity_lock<0>::_S_atomicity_lock = 0;
return __result;
}
基本思路也是锁总线，只不过实现方式不同罢了。
在__exchange_and_add函数前面还要修饰符__attribute__((unused))，这是GCC的扩展，表示该函数或变量可能不使用，这个属性可以避免编译器产生警告信息。
2. STL中的concepts
在众多的STL的实现中SGI STL的现实相对来说非常好的，在string中也使用到了concepts的概念，首先介绍一下concepts。
概念（Concepts）简单的说是用于模板参数的类型系统，对模板的参数进行约束。模板因为其独特的性质，只有在实例点的时候才会真正的生成代码，那么也就说按照以前我们所写的代码，即便是模板参数有错误，在编译时候我们也不能得到错误，这当然和我们的期望相违背，为了解决这个问题有一些方法如boost库中使用的约束类，而约束类也存在一些缺点，引入concepts能够解决很多问题。
下面就简单的看一下SGI STL中的concepts。
例如我们在assign函数中看到这样的代码__glibcxx_requires_string(__s);这些其实是一些宏，和我们原来的方法一样，它的对应展开就是assert(__s)，还不能说这是concepts。但是下面的例子就是concepts了。
template<typename _Tp>
inline void swap(_Tp& __a, _Tp& __b)
{
// concept requirements
__glibcxx_function_requires(_SGIAssignableConcept<_Tp>)
const _Tp __tmp = __a;
__a = __b;
__b = __tmp;
}
这是STL中swap算法，交换两个变量，我们知道两个变量能够交换的条件就是他们具有可赋值性，也就是说我们期望在编译的时候判断模板参数是否具有可赋值性，为此上面的范型算法就加入了concepts，对模板参数进行判断。Concepts将会通过下面的宏展开：
#define __glibcxx_function_requires(...) /
__gnu_cxx::__function_requires< __gnu_cxx::__VA_ARGS__ >();
template <class _Concept>
inline void __function_requires()
{
void (_Concept::*__x)() _IsUnused = &_Concept::__constraints;
}
其中参数为：
template <class _Tp>
struct _SGIAssignableConcept
{
void __constraints()
{
_Tp __b _IsUnused(__a);
__a = __a;// require assignment operator
__const_constraints(__a);
}
void __const_constraints(const _Tp& __b)
{
_Tp __c _IsUnused(__b);
__a = __b;// const required for argument to assignment
}
_Tp __a;
};
可以看出实际上最后执行的是_SGIAssignableConcept：：__constraints()。
最后要注意一点，STL中的关于concepts不一定打开，如果你要使用concepts应该自己打开编译开关。
3. string概述
很多资料都告诉了这样的事实string其实就是使用typedef对下面模版类的别名定义。即basic_string<char, char_traits<char>, allocator<char> > class；typedef basic_string string；其中char_traits<char>也是模版类，主要定义了几种类型和基本的操作。Allocator<char>也是模版类，主要是进行内存管理。当然上面的char也有可能是w_chart，当且仅当我们定义使用宽字符（是通过宏变量来控制）。
basic_string中的模板参数Char_traits<class type>定义的类型主要有char_type（表明类型），int_type（就是int类型，定义该类型的目的是type可能和int发生类型转换），pos_type（表明位置信息），off_type（表明结束信息）和state_type（表明目前的状态，其实就是int），基本的操作主要有assign，copy，find，move，eq（等于），lt（小于）。
对于basic_string中的模板参数allocator<>的分析在《STL源码剖析》中已经分析的较为详细，就是进行内存管理。
在模版类basic_string中定义了typedef __gnu_cxx::__normal_iterator<pointer, basic_string> iterator;首先请问你是觉得这样的定义如果不是在模版中正常吗？你会不会觉得basic_string的定义还没有完成，怎么可以看成是一个完整的类型作为参数传递呢？噢，这个应该不是问题，为什么？因为我们只是定义一个类型，并没有定义任何变量，当然不用内存分配，编译器当然会饶过他继续编译不会报错。其实内部仍然是使用Pointer直接作为它的迭代器，只不过对Pointer进行了封装，形成类（重载了++，--，*，->，[]，&，+等操作符）。你会不会觉得这很麻烦，确实是，没有提供比原始指针更强大的功能，但也要想想为什么这么设计，原因就是一个简单的Pointer不能提供一些类型，如value_type等等（即traits），没有办法必须封装。
4. 实现计数的结构体Rep_Base和Rep
在模板类basic_string中嵌套定义了这两个结构体，Rep_base在该结构体中主要进行引用计数的定义和Rep继承于Rep_base主要是进行引用计数的相关操作和内存的分配策略，因此这两个结构体是非常关键的。
结构体_Rep_base的定义如下：
Struct _Rep_base
{
    Size_type  _M_length;
    Size_type  _M_capaticty;
    Int        _M_refCount;
};
代码中有这样的解释：
Ø         字符串真正存储的是原字符串加上一个NULL，故真正的长度是_M_length+1;
Ø         _M_Capacity一定不小于_M_length，而且内存的分配的增长总是以当前_M_capacity+1为单位；
Ø         _M_refCount的取值可以分为三种：
«        -1：可能内存泄露，有一个变量指向字符串，字符串可以被更改，不允许引用copy，也就是当出现这种情况时，这个string对象不会再和其他string对象共享了；
«        0： 有一个变量指向字符串，字符串可以被更改；
«        n>=1:有n+1个变量指向字符串，对字符串操作时应该加锁，字符串不可以被更改；
Ø         当_M_length，_M_capactiy和_M_refCount均为零，表示空串。
_Rep的定义
_Rep继承于_Rep_base，同时_Rep中还定义了三个静态数据成员，这些数据成员都有独特的意思。size_type    _S_max_size和_CharT  _S_terminal分表表示字符的最大长度和字符串的结束标志（即是’/0’也就是0）。_S_max_size这个值表示可以最大分配的内存，这个值表示使用1G内存分配字符串，_S_max_size = (((npos - sizeof(_Rep_base))/sizeof(_CharT)) - 1) / 4;，其中npos是定义在模版类basic_string中，初始值为-1(也即0xFFFFFFFF)。
定义一个数组size_type _S_empty_rep_storage[];（这并不是一个0长度的数组，0长度的数组是在编译时并不分配空间，仅仅作为占位符），在对应的定义文件（basic_string.tcc）中有明确的定义，如下_S_empty_rep_storage[ (sizeof(_Rep_base) + sizeof(_CharT) + sizeof(size_type) - 1) / sizeof(size_type)];
最后要注意一下静态对象初始化的时机，静态对象一般是在.ini段中完成初始化，即在main函数之前代码段中完成，一般使用缺省的构造函数完成，象上面的数组中的元素会被初始化为0（该数组初始化的结果可表示空串有1个引用）。
_Rep中的几个主要函数
1. _S_Create分配字符串占用的内存空间
_S_create(size_type __capacity, size_type __old_capacity,
                const _Alloc& __alloc)
{
const size_type __pagesize = 4096; // must be 2^i * __subpagesize
const size_type __subpagesize = 128;
const size_type __malloc_header_size = 4 * sizeof (void*);
// The biggest string which fits in a memory page
const size_type __page_capacity = ((__pagesize - __malloc_header_size - sizeof(_Rep) - sizeof(_CharT)) / sizeof(_CharT));
//capacity使用指数增长的方法
if (__capacity > __old_capacity && __capacity < 2 * __old_capacity && __capacity > __page_capacity)
__capacity = 2 * __old_capacity;
size_type __size = (__capacity + 1) * sizeof(_CharT) + sizeof(_Rep); //加1的原因是在字符串最后添加一个’/0’
//根据_M_capacity调整size（真正需要new/malloc的内存大小）
const size_type __adj_size = __size + __malloc_header_size;
if (__adj_size > __pagesize)
{
const size_type __extra = __pagesize - __adj_size % __pagesize;
__capacity += __extra / sizeof(_CharT);
// Never allocate a string bigger than _S_max_size.
if (__capacity > _S_max_size)
__capacity = _S_max_size;
__size = (__capacity + 1) * sizeof(_CharT) + sizeof(_Rep);
}
else if (__size > __subpagesize)
{
const size_type __extra = __subpagesize - __adj_size % __subpagesize;
__capacity += __extra / sizeof(_CharT);
__size = (__capacity + 1) * sizeof(_CharT) + sizeof(_Rep);
}
//注意这里是分配大小为size的内存，不是上面的adjsize，因为mallocheadersize是在new/malloc系统增加的
void* __place = _Raw_bytes_alloc(__alloc).allocate(__size);
_Rep *__p = new (__place) _Rep;
__p->_M_capacity = __capacity;
__p->_M_set_sharable();  // One reference.，设置共享标志
 __p->_M_length = 0;
return __p;
}
上面的函数中几个常量的含义是理解函数的关键，弄清楚这几个常量这个函数的实现也就明白，这几个变量的作用如下：
1.    __pageSize的大小是指在分配内存的时候使用的，很类似于实际中virtual memory（但是和实际中virtual memory的大小无关），__pageSize是每次内存分配的最小单位；
2.    __subPageSize的大小是每次分配的字符串必须以__subPageSize对齐，显然这可以加快分配速度，不必要每次都对齐，但是显然可能浪费了空间；
3.    __mallocHeaderSize的意思是这样的，我们在malloc内存的时候，每次调用new/malloc都会比真正的所需要的内存大上几个字节（一般来说是4个字节），这几个字节是存储的是分配内存的真正的长度。【源码中的注释说这个可以为N ×sizeof(void*)（其中N ＝0，2，4），并且写到，据说N大比小好，所以取4，实际（vc/dev）中new/malloc附加的空间都是4个字节而已，即1×sizeof(void*)】；
4.    字符串的存储示意图如下： 
_Rep             string
__P
2. _M_refdata返回字符串的内存位置
_CharT* _M_refdata() throw()
{
return reinterpret_cast<_CharT*>(this + 1);
}
这个函数非常简单，只要注意一点，那就是this ＋ 1真正的位置。This指的是从头开始的内存地址，this + 1就是上图中的__P所指的位置（1就是sizeof(_Rep)）。
3. _M_colne创建新的字符串空间和信息
_M_clone(const _Alloc& __alloc, size_type __res)
{
// Requested capacity of the clone.
const size_type __requested_cap = this->_M_length + __res;
//分配空间
_Rep* __r = _Rep::_S_create(__requested_cap, this->_M_capacity,
__alloc);
    //copy对应的字符串
if (this->_M_length)
traits_type::copy(__r->_M_refdata(),_M_refdata(),this->_M_length);
    //设置字符串的长度和结束标志
__r->_M_length = this->_M_length;
__r->_M_refdata()[this->_M_length] = _Rep::_S_terminal;
return __r->_M_refdata();
}
4. _M_refdata仅仅增加计数信息
_CharT*    _M_refcopy() throw()
{
    if(__builtin_expect(this != &_S_empty_rep(), false))
__gnu_cxx::__atomic_add(&this->_M_refcount, 1);
    return _M_refdata();
}
__builtin_expect(x,expected_value)是GCC提供的实现的一个内部函数，其值就是x，但x的值等于expected_value的可能较大，这可以让gcc产生较好的跳转代码。这只是一种优化写法。
If判断完成的就是，this不是空串，则为真，执行原子操作，为计数值加1.
5. _M_grab是clone和refdata的入口判断
_CharT*    _M_grab(const _Alloc& __alloc1, const _Alloc& __alloc2)
{
    return (!_M_is_leaked() && __alloc1 == __alloc2)
               ? _M_refcopy() : _M_clone(__alloc1);
}
在这个函数中将判断是进行引用计数加1还是重新建立一个新的字符串。必须说明的该函数只有才basic_string的copy ctor和assignment（赋值指的是相同类型的赋值，当有str ＝ “123”,这将是调用构造函数，即便是有很多的这样的语句也不会调用引用计数的）中才可能被调用，也就是说在有在新的字符串按copy或者赋值创建的时候才考虑使用引用计数。
进行refcopy或者clone的关键标识是：首先没有内存泄漏标志（关于这个标志主要是禁止string再次被共享，后面会有具体的描述），然后就是两个string对象的分配相同。
6. _M_destroy释放空间
_M_destroy(const _Alloc& __a) throw ()
{
//如果不是空串，将释放空间
if (this == &_S_empty_rep())    return;
//调整释放空间的大小，这才是真正需要释放的大小
const size_type __size = sizeof(_Rep_base) + (this->_M_capacity + 1) * sizeof(_CharT);
//_Raw_bytes_alloc是allocator类型
_Raw_bytes_alloc(__a).deallocate(reinterpret_cast<char*>(this), __size);
}
7. _M_dispose减少引用计数值并决定释放空间
void   _M_dispose(const _Alloc& __a)
{
    if (__builtin_expect(this != &_S_empty_rep(), false))
       if (__gnu_cxx::__exchange_and_add(&this->_M_refcount, -1) <= 0)
           _M_destroy(__a);
}
当引用计数值小于等于0的时候，已经表示没有字符串指向这块内存，需要释放。注意这个地方的等于0也释放内存的，和我们最初所说的0表示一个引用有矛盾的。但是注意这里是完全正确的，__exchange_and_add()函数返回的是没有修改前的值，因此返回值为0其实真实的refcount已经为-1了。
Rep中其他简单的函数如设置length，capacity，refcount等都比较简单。  
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/abortexit/archive/2007/06/04/1638254.aspx](http://blog.csdn.net/abortexit/archive/2007/06/04/1638254.aspx)
5. Basic_string的构造函数和析构函数
在看Basic_string构造函数之前先看一下string中对应的成员变量。在basic_string<>中定义了一个辅助的存储结构Alloc_hider（继承于Allocator），该结构仅仅简单的封装了真实的数据即char* mPointer；在basic_string<>中直接用Alloc_hider定义一个数据成员mDataPlus（即对应char* mPointer）。其实mPointer就是上面图中__P所指的位置。
SGI STL重载了众多的构造函数，分别如下：
构造函数用到的一个重要函数_S_construct：
_S_construct是一组重载的函数，主要是区别具有不同类型的的迭代器，提高效率。
template<class _InIterator>
static _CharT*    _S_construct(_InIterator __beg, _InIterator __end, const _Alloc& __a)
{
    typedef typename _Is_integer<_InIterator>::_Integral _Integral;
    return _S_construct_aux(__beg, __end, __a, _Integral());
}
_S_construct_aux是一个辅助函数，当_Integral()为假，即迭代器不是整数性质，会根据迭代器的类型标志分别调用下面最后一个参数为input_iterator_tag或者forward_iterator_tag的_S_construct函数，当_Integral()为真，调用只有三个参数的_S_construct函数。
1. 迭代器属于输入迭代器
// For Input Iterators, used in istreambuf_iterators, etc.
template<class _InIterator>
static _CharT*    _S_construct(_InIterator __beg, _InIterator __end, const _Alloc& __a, input_iterator_tag)
{
if (__beg == __end && __a == _Alloc())
return _S_empty_rep()._M_refdata();
// Avoid reallocation for common case.
_CharT __buf[128];
size_type __len = 0;
while (__beg != __end && __len < sizeof(__buf) / sizeof(_CharT))
{
__buf[__len++] = *__beg;
++__beg;
}
//预先建立存储128个字符的空间
_Rep* __r = _Rep::_S_create(__len, size_type(0), __a);
traits_type::copy(__r->_M_refdata(), __buf, __len);
try
{
while (__beg != __end)
{
//If为真，表示预定义的长度不够，需要增加分配空间
if (__len == __r->_M_capacity)
{
_Rep* __another = _Rep::_S_create(__len + 1, __len, __a);
traits_type::copy(__another->_M_refdata(),__r->_M_refdata(), __len);
__r->_M_destroy(__a);
__r = __another;
}
__r->_M_refdata()[__len++] = *__beg;
++__beg;
}
}
catch(...)
{
__r->_M_destroy(__a);
__throw_exception_again;
}
__r->_M_length = __len;
__r->_M_refdata()[__len] = _Rep::_S_terminal;       // grrr.
return __r->_M_refdata();
}
2. 迭代器属于输入输出迭代器
// For forward_iterators up to random_access_iterators, used for
// string::iterator, _CharT*, etc.
template<class _FwdIterator>
static _CharT*    _S_construct(_FwdIterator __beg, _FwdIterator __end, const _Alloc& __a, forward_iterator_tag)
{
if (__beg == __end && __a == _Alloc())
return _S_empty_rep()._M_refdata();
// NB: Not required, but considered best practice.
if (__builtin_expect(__is_null_pointer(__beg), 0))
__throw_logic_error(__N("basic_string::_S_construct NULL not valid"));
const size_type __dnew = static_cast<size_type>(std::distance(__beg,__end));
// Check for out_of_range and length_error exceptions.
//直接根据已知数据长度分配字符串的空间
_Rep* __r = _Rep::_S_create(__dnew, size_type(0), __a);
try
{
_S_copy_chars(__r->_M_refdata(), __beg, __end);
}
catch(...)
{
__r->_M_destroy(__a);
__throw_exception_again;
}
__r->_M_length = __dnew;
__r->_M_refdata()[__dnew] = _Rep::_S_terminal;  // grrr.
return __r->_M_refdata();
}
3. 迭代器属于随机迭代器
static _CharT*    _S_construct(size_type __n, _CharT __c, const _Alloc& __a)
{
if (__n == 0 && __a == _Alloc())
return _S_empty_rep()._M_refdata();
// Check for out_of_range and length_error exceptions.
//直接根据输入的长度分配空间
_Rep* __r = _Rep::_S_create(__n, size_type(0), __a);
if (__n)
traits_type::assign(__r->_M_refdata(), __n, __c);
__r->_M_length = __n;
__r->_M_refdata()[__n] = _Rep::_S_terminal;  // grrr
return __r->_M_refdata();
}
显然SGI STL提供的这三种方式效率是不同的，方法1中STL是通过猜测每次分配128个字节，有可能浪费也有可能不足，不足的时候只能再次分配，方法2是根据迭代器的性质计算出距离然后分配空间，方法3则直接根据距离数据分配空间。
缺省构造函数：
template<typename _CharT, typename _Traits, typename _Alloc>
inline basic_string<_CharT, _Traits, _Alloc>::
basic_string()
: _M_dataplus(_S_empty_rep()._M_refdata(), _Alloc()) { }
其中_S_empty_rep()返回静态数组的地址，_M_refdata()返回的下一个地址（this + 1）,并传递对应的Allocator分配内存。
由此可以看出，所有用缺省构造函数定义的string对象都是使用空串。
空串有分配器的构造函数：
template<typename _CharT, typename _Traits, typename _Alloc>
basic_string<_CharT, _Traits, _Alloc>::
basic_string(const _Alloc& __a)
    : _M_dataplus(_S_construct(size_type(), _CharT(), __a), __a)
{ }
临时对象size_type()实际上是0,仍然返回的是空串。
Copy构造函数：
basic_string<_CharT, _Traits, _Alloc>::
basic_string(const basic_string& __str)
: _M_dataplus(__str._M_rep()->_M_grab(_Alloc(__str.get_allocator()), __str.get_allocator()), __str.get_allocator())
{ }
这个非常简单，就是实质就是增加了引用计数。
从一个string构造子串：
template<typename _CharT, typename _Traits, typename _Alloc>
basic_string<_CharT, _Traits, _Alloc>::
basic_string(const basic_string& __str, size_type __pos, size_type __n)
: _M_dataplus(_S_construct(__str._M_data()+ __str._M_check(__pos, "basic_string::basic_string"),__str._M_data() + __str._M_limit(__pos, __n)+ __pos, _Alloc()), _Alloc())
{ }
_M_check(__pos,"basic_string::basic_string")检查是否越界，如果越界抛出字符串异常，否则返回__pos；_M_data()返回原有字符串；_M_limit(__pos, __n)完成长度检测，即__pos + n的距离不应该超过原字符串的长度，值得注意的是没有对__pos的合法性做检查。
因此该函数直接根据原有字符串新建一个串。注意这个串虽然和原串有字符相同，但他们并没有共享存储的。（当然也有实现了共享子串的方法）。
STL中还有一个使用指定分配器的构造函数，除了最后关于分配器的参数其余都相同。此外还提供了从char*到string的构造函数，根据两个迭代器构造string等构造函数，都比较简单，直接调用上面的_S_construct函数完成。
析构函数：
最后简单的看一下析构函数，因为非常简单，所以放在这里没有单独作为一个大一点的标题。
~basic_string()
{
_M_rep()->_M_dispose(this->get_allocator());
}
在这里就是释放空间。
6.           赋值构造函数operator=
operator=中用到的一个重要函数assign：
assign在STL定义成为6个重载函数。其中有两个非常关键，其余介在此基础上实现。
1. 从string对象赋值
basic_string&
assign(const basic_string& __str)
{
if (_M_rep() != __str._M_rep())
{
//如果两个字符串不相同，则根据引用计数规则发生计数
const allocator_type __a = this->get_allocator();
_CharT* __tmp = __str._M_rep()->_M_grab(__a, __str.get_allocator());
//释放原来计数对象
_M_rep()->_M_dispose(__a);
//把自己的字符指针指向共享的字符串
_M_data(__tmp);
}
return *this;
}
2. 从c-style的字符串对象赋值
basic_string&
assign(const _CharT* __s, size_type __n)
{
    //这是一个宏，判断字符串不为NULL，长度n不为0;
__glibcxx_requires_string_len(__s, __n);
if (__n > this->max_size())
__throw_length_error(__N("basic_string::assign"));
    //这个if判断非常有意思，显然当我们发现这个string对象共享的是否，在赋值的时候必须小心，因为有多个对象指向同一个c_style的字符串；
    //对于第二个和第三个判断我还没有弄清楚到底防止哪些情况；假单的说就是要着这两个字符串不重叠，当然了字符串重叠可能需要单独处理
    //单纯丛代码上分析显然是为了警戒c_style和string对象所指的c_style字符串相连
    //我的唯一的猜测是这两个判断语句为了保护string对象中的结构体Rep_base，防止因为赋值修改了该结构体（也许有人进行了类型转换，然后赋值，那么可能造成程序当掉）
    //在下面会分析replace_safe的代码
if (_M_rep()->_M_is_shared() || less<const _CharT*>()(__s, _M_data()) || less<const _CharT*>()(_M_data() + this->size(), __s))
return _M_replace_safe(size_type(0), this->size(), __s, __n);
else
{
//判断c_style的字符串是否重叠，进行内存copy或者move
const size_type __pos = __s - _M_data();
if (__pos >= __n)
traits_type::copy(_M_data(), __s, __n);
else if (__pos)
traits_type::move(_M_data(), __s, __n);
_M_rep()->_M_set_sharable();
_M_rep()->_M_length = __n;
_M_data()[__n] = _Rep::_S_terminal;  // grr.
return *this;
}
}
还有4个重载的函数，有两个非常简单，有两个使用了replace函数，下面会讨论到。
有了assign()函数，operator=就非常容易实现了，直接根据参数调用相应的assign函数。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/abortexit/archive/2007/06/04/1638256.aspx](http://blog.csdn.net/abortexit/archive/2007/06/04/1638256.aspx)
7.           replace函数
replace函数是basic_string中一个最重要的函数，很多操作都是直接/间接通过replace完成，包括insert，erase，assignment等等。Repalce函数在basic_string中有多个重载的形式。下面开始分析repalce函数。由于repalce函数调用了其他的函数，还是现从被调用的函数开始出发。
在下面的描述中，原字符串主要是指被替换的字符串（即要被修改的字符串）。
1. _M_mutate函数
_M_mutate函数主要是用于判断从__pos开始，用长度为__len2的串替换长度为__len1的串，是否进行内存的分配。
Void _M_mutate(size_type __pos, size_type __len1, size_type __len2)
{
const size_type __old_size = this->size();
//__new_size指的是替换以后字符串的长度
const size_type __new_size = __old_size + __len2 - __len1;
//__how_much表示原字符串末端保留下来字符串的长度
const size_type __how_much = __old_size - __pos - __len1;
//if判断主要是必须重新分配内存
if(_M_rep() == &_S_empty_rep() || _M_rep()->_M_is_shared() || __new_size > capacity())
    {
       const allocator_type __a = get_allocator();
       _Rep* __r = _Rep::_S_create(__new_size, capacity(), __a);
       //如果pos不为0（pos应该是大于0的），把原字符串开头到pos之间的子串copy到新串
       if(__pos)
           traits_type::copy(__r->_M_refdata(), _M_data(), __pos);
       //如果how_much不为0，把原字符串末端留下的子串copy到新串的末端
       if(__how_much)
           traits_type::copy(__r->_M_refdata() + __pos + __len2, _M_data() + __pos + __len1, __how_much);
       //减去原字符串的引用计数，并交换原串和新串
       _M_rep()->_M_dispose(__a);
       _M_data(__r->_M_refdata());
    }
else if (__how_much && __len1 != __len2)
    {
    //else主要在不重新分配内存的情况下，并且需要移动原字符串末端的字符
       traits_type::move(_M_data() + __pos + __len2, _M_data() + __pos + __len1, __how_much);
    }
_M_rep()->_M_set_sharable();
_M_rep()->_M_length = __new_size;
//很关键，要设置最后的结束标志
_M_data()[__new_size] = _Rep::_S_terminal; // grrr. (per 21.3.4)
}
那么_M_mutate函数执行结束以后，我们可以得到的结论是在字符串中从__pos开始留下了长度为__len2的空白区间，等待填充。
2. _M_replace_safe函数
这个函数主要填充字符串中从__pos开始留下了长度为__len2的空白区间。
basic_string&
_M_replace_safe(size_type __pos1, size_type __n1, const _CharT* __s, size_type __n2)
{
    _M_mutate(__pos1, __n1, __n2);
    if (__n2 == 1)
    _M_data()[__pos1] = *__s;
    else if (__n2)
    traits_type::copy(_M_data() + __pos1, __s, __n2);
    return *this;
}
3. _M_replace函数
有了上面的_M_replace_safe函数，则_M_replace非常容易完成。
basic_string<_CharT, _Traits, _Alloc>&
replace(size_type __pos, size_type __n1, const _CharT* __s, size_type __n2)
{
//判断字符串__s和它的长度__n2都是有效
__glibcxx_requires_string_len(__s, __n2);
//判断__pos在原字符串是一个合法的位置
_M_check(__pos, "basic_string::replace");
//_M_limit(__pos, __n)完成长度检测，即__pos + n的距离不应该超过原字符串的长度
__n1 = _M_limit(__pos, __n1);
//下面的if判断主要防止字符串太长了，超过了可表示的最大值
if (this->max_size() - (this->size() - __n1) < __n2)
__throw_length_error(__N("basic_string::replace"));
bool __left;
//判断是否和其他字符对象共享，并且这两个字符串不重叠
if (_M_rep()->_M_is_shared() || less<const _CharT*>()(__s, _M_data())|| less<const _CharT*>()(_M_data() + this->size(), __s))
return _M_replace_safe(__pos, __n1, __s, __n2);
else if ((__left = __s + __n2 <= _M_data() + __pos) || _M_data() + __pos + __n1 <= __s)
    {
       //这个if判断主要是判断这两个字符串时候有重叠，如果没有重叠执行下面的
       const size_type __off = __s - _M_data();
       _M_mutate(__pos, __n1, __n2);
       if (__left)
           traits_type::copy(_M_data() + __pos, _M_data() + __off, __n2);
       else
           traits_type::copy(_M_data() + __pos, _M_data() + __off + __n2 - __n1, __n2);
       return *this;
    }
else 
    {
           //两个字符串有重叠的情况，先生成一个临时对象
       const basic_string __tmp(__s, __n2);
           return _M_replace_safe(__pos, __n1, __tmp._M_data(), __n2);
       }
}
4. _M_replace_aux函数
_M_replace_aux函数和_M_replace_safe函数非常相似。这个函数主要完成的是拷贝__n2个字符__C，所以有一点点区别（别的函数都是处理字符串的）。
basic_string&
_M_replace_aux(size_type __pos1, size_type __n1, size_type __n2, _CharT __c)
{
    if (this->max_size() - (this->size() - __n1) < __n2)
        __throw_length_error(__N("basic_string::_M_replace_aux"));
    _M_mutate(__pos1, __n1, __n2);
    if (__n2 == 1)
        _M_data()[__pos1] = __c;
    else if (__n2)
        traits_type::assign(_M_data() + __pos1, __n2, __c);
    return *this;
}
5. replace函数小结
在basic_string中的其他重载的replace函数，有12个函数都是使用上面的replace函数，有两个使用的上面的_M_replace_aux函数。
8.           insert和erase函数
insert和erase函数都是借助于replace函数实现的，也是比较简单。
Insert函数：
Insert函数共有8个重载的形式，根据返回值可以分为3类，其中最为主要的是返回值为basic_string&。
1. 返回值为basic_string&的insert函数
这个insert完成的给定__pos插入长度为__n的字符串__s。
basic_string&
insert(size_type __pos, const _CharT* __s, size_type __n)
{
__glibcxx_requires_string_len(__s, __n);
_M_check(__pos, "basic_string::insert");
if (this->max_size() - this->size() < __n)
    __throw_length_error(__N("basic_string::insert"));
//照样判断是否需要重新分配内存
if(_M_rep()->_M_is_shared() || less<const _CharT*>()(__s, _M_data())|| less<const _CharT*>()(_M_data() + this->size(), __s))
return _M_replace_safe(__pos, size_type(0), __s, __n);
else
{
//两个串有重叠,在源代码中有一段注释，说明了为什么引入和临时变量__off
//如果是你第一次写这样的代码，不知道你是否能考虑到？？
//由于_M_mutate函数可能会重新分配内存，也就说字符串实际的位置可能发生变化，而在这段代码中__s和_M_data()实际上有重叠，那么当_M_data()实际所指的c_style字符串发生变化，__s也就会失效，所以引入临时变量，保存他们之间的相对距离，然后在_M_mutate函数执行后重新找到字符串__s。
const size_type __off = __s - _M_data();
_M_mutate(__pos, 0, __n);
__s = _M_data() + __off;
_CharT* __p = _M_data() + __pos;
//被插入的子串末端在__p之前，直接拷贝
if (__s  + __n <= __p)
traits_type::copy(__p, __s, __n);
//被插入的子串始端在__p之后，直接拷贝
else if (__s >= __p)
traits_type::copy(__p, __s + __n, __n);
else
{
//被插入的子串和插入子串位置重叠，需要小心，防止覆盖原来字符
//不过这里的算法也算是奇怪，居然是从__S开始计算__n个字符，但是中间吆除去__P开头__n个字符。如下图所示：
__s
__p
n
n
__nleft
__nleft
n - __nleft
    const size_type __nleft = __p - __s;
traits_type::copy(__p, __s, __nleft);
traits_type::copy(__p + __nleft, __p + __n, __n - __nleft);
}
return *this;
}
}
返回值为basic_string&的insert函数共有5个，其中4个都是借用调用上面的实现。还有一个是调用_M_replace_aux函数完成的是插入__n2个字符__C。
2. 返回值为void的insert函数
void
insert(iterator __p, size_type __n, _CharT __c)
{
    this->replace(__p, __p, __n, __c);
}
调用的repalce函数。调用的是replace(iterator __i1, iterator __i2, const basic_string& __str)这样的函数，最后还是转化为调用上面描述的replace函数。这样的函数有两个。
3. 返回值为iterator的insert函数
iterator
insert(iterator __p, _CharT __c)
{
    _GLIBCXX_DEBUG_PEDASSERT(__p >= _M_ibegin() && __p <= _M_iend());
    const size_type __pos = __p - _M_ibegin();
    _M_replace_aux(__pos, size_type(0), size_type(1), __c);
//很是抱歉，我没有看明白这样设计的目的。
//我的猜测是这样的，由于这个函数返回的是iterator，防止在insert以后和其他string对象共享，当其他string对象重新分配内存之后，这个返回值iterator就是一个无效值。
//因此就设置这样的标志，表示该string对象不能被共享的。
    _M_rep()->_M_set_leaked();
    return this->_M_ibegin() + __pos;
}
这样的函数只有一个。插入一个字符，返回插入的位置。
Erase函数：
1. 返回值为basic_string&的erase函数
basic_string&
erase(size_type __pos = 0, size_type __n = npos)
{
return _M_replace_safe(_M_check(__pos, "basic_string::erase"), _M_limit(__pos, __n), NULL, size_type(0));
}
2. 返回值为iterator的erase函数
iterator
erase(iterator __position)
{
    _GLIBCXX_DEBUG_PEDASSERT(__position >= _M_ibegin()&& __position < _M_iend());
    const size_type __pos = __position - _M_ibegin();
    _M_replace_safe(__pos, size_type(1), NULL, size_type(0));
    _M_rep()->_M_set_leaked();
    return _M_ibegin() + __pos;
}
iterator
erase(iterator __first, iterator __last)
{
    _GLIBCXX_DEBUG_PEDASSERT(__first >= _M_ibegin() && __first <= __last && __last <= _M_iend());
const size_type __pos = __first - _M_ibegin();
    _M_replace_safe(__pos, __last - __first, NULL, size_type(0));
    _M_rep()->_M_set_leaked();
    return _M_ibegin() + __pos;
}
前面已经介绍过replace_safe函数，所以erase函数无须再介绍了。值得注意的仍然是在两个返回值为iterator的erase函数中在执行replace_safe函数后也有设置string对象为资源泄露标志，我在此处的推测仍然是和前面的推测保持一致。
9.           Operator[]函数
Const函数：
const_reference
operator[] (size_type __pos) const
{
    _GLIBCXX_DEBUG_ASSERT(__pos <= size());
    return _M_data()[__pos];
}
非常简单，直接返回数据，并且使用const_conference接受字符对象，这是一个const point不能修改字符。
Non-Const函数：
reference
operator[](size_type __pos)
{
    _GLIBCXX_DEBUG_ASSERT(__pos < size());
///首先是否需要重新分配内存，然后设置内存泄露标志，也就是有_M_rep()->_M_set_leaked();的语句
    _M_leak();
    return _M_data()[__pos];
}
对_M_rep()->_M_set_leaked()推测仍然是和前面的推测保持一致。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/abortexit/archive/2007/06/04/1638257.aspx](http://blog.csdn.net/abortexit/archive/2007/06/04/1638257.aspx)
