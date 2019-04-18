# 定长内存池之BOOST::pool - 深之JohnChen的专栏 - CSDN博客

2011年01月19日 22:58:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：9966


内存池可有效降低动态申请内存的次数，减少与内核态的交互，提升系统性能，减少内存碎片，增加内存空间使用率，避免内存泄漏的可能性，这么多的优点，没有理由不在系统中使用该技术。

内存池分类：

1、不定长内存池。典型的实现有apr_pool、obstack。优点是不需要为不同的数据类型创建不同的内存池，缺点是造成分配出的内存不能回收到池中。这是由于这种方案以session为粒度，以业务处理的层次性为设计基础。

2、定长内存池。典型的实现有LOKI、BOOST。特点是为不同类型的数据结构分别创建内存池，需要内存的时候从相应的内存池中申请内存，优点是可以在使用完毕立即把内存归还池中，可以更为细粒度的控制内存块。
与变长的相比，这种类型的内存池更加通用，另一方面对于大量不同的数据类型环境中，会浪费不少内存。但一般系统主要的数据结构都不会很多，并且都是重复申请释放使用，这种情况下，定长内存池的这点小缺点可以忽略了。

Boost库的pool提供了一个内存池分配器，用于管理在一个独立的、大的分配空间里的动态内存分配。Boost库的pool主要适用于快速分配同样大小的内存块，尤其是反复分配和释放同样大小的内存块的情况。使用pool内存池主要有以下两个优点：

1. 能够有效地管理许多小型对象的分配和释放工作，避免了自己去管理内存而产生的内存碎片和效率低下问题。

2.告别程序内存泄漏的烦恼，pool库会在内部对内存自动进行管理，避免了程序员一不小心而造成的内存泄漏问题。

pool库主要提供了四种内存池接口，分别是pool、object_pool、singleton_pool和pool_allocator/fast_pool_allocator。

1）pool

基本的定长内存池

#include <boost/pool/pool.hpp>

typedef struct student_st

{

char name[10];

int age;

}CStudent;

int main()

{

boost::pool<> student_pool(sizeof(CStudent));

CStudent * const obj=(CStudent *)student_pool.malloc();

student_pool.free(obj);

return 0;

}

pool的模版参数只有一个分配子类型，boost提供了两种default_user_allocator_new_delete/default_user_allocator_malloc_free，指明申请释放内存的时候使用new/delete，还是malloc/free，默认是default_user_allocator_new_delete。构造函数有2个参数：nrequested_size,nnext_size。nrequested_size是block的大小（因为void*保存序号，因此boost内置了block的最小值，nrequested_size过小则取内置值），nnext_size是simple_segregated_storage中内存不足的时候，申请的block数量，默认是32。最全面的实例化pool类似这样：boost::pool<boost::default_user_allocator_malloc_free> student_pool(sizeof(CStudent),255);

pool提供的函数主要有：

malloc/free基于add_block/malloc/free实现，高效

ordered_malloc/ordered_free基于add_ordered_block/malloc/ordered_free实现，在pool中无任何意义，切勿使用。

release_memory/purge_memory 前者释放池中未使用内存，后者释放池中所有内存。另池析构也会释放内存

2）object_pool

对象内存池，这是最失败的一个内存池设计。

#include <boost/pool/object_pool.hpp>

class A{

public:

A():data_(0){}

private:

int data_;

};

int main()

{

boost::object_pool<A> obj_pool;

A *const pA=obj_pool.construct();

obj_pool.destroy(pA);

return 0;

}

object_pool继承至pool，有两个模版参数，第一个就是对象类型，第二个是分配子类型，默认同pool是default_user_allocator_new_delete。构造函数参数只有nnext_size，意义以及默认值同pool。最全面的实例化object_pool类似这样：boost::pool<A,boost::default_user_allocator_malloc_free> obj_pool(255);

object_pool提供的函数主要有（继承至父类的略）： malloc/free 复写pool的malloc/free，add_ordered_block/malloc/ordered_free实现

construct/destroy 基于本类的malloc/free实现，额外调用默认构造函数和默认析构函数。

~object_pool单独拿出这个说下，若析构的时候有对象未被destroy，可以检测到，释放内存前对其执行destroy 

为什么boost::object_pool要设计成这样？能调用构造函数和析构函数显然不是boost::object_pool类设计的出发点，因为构造函数只能执行默认构造函数（首次发表错误：可以调用任意的构造函数，参见代码文件：boost/pool/detail/pool_construct.inc和boost/pool/detail/pool_construct_simple.inc，感谢eXile指正），近似于无，它的重点是内存释放时候的清理工作，这个工作默认的析构函数就足够了。apr_pool内存池中就可以注册内存清理函数，在释放内存的时刻执行关闭文件描述符、关闭socket等操作。boost::object_pool也想实现同样的功能，因此设计了destroy这个函数，而同时为了防止用户遗漏掉这个调用，而又在内存池析构的时候进行了检测回收。为了这个目的而又不至于析构object_pool的时间复杂度是O(n平方），boost::object_pool付出了沉重的代价，在每次的destoy都执行排序功能，时间复杂度O(n),最后析构的时间复杂度是O(n)，同样为了这个目的，从simple_segregated_storage增加了add_ordered_block/ordered_free，pool增加了ordered_malloc/ordered_free等累赘多余的功能。

基于上面讨论的原因，boost::object_pool被设计成了现在的样子，成了一个鸡肋类。类的设计者似乎忘记了内存池使用的初衷，忘记了内存池中内存申请释放的频率很高，远远大于内存池对象的析构。如果你依然想使用类似于此的内存清理功能，可以在boost::object_pool上修改，不复写malloc/free即可，重写object_pool的析构，简单释放内存就好，因此析构object_pool前不要忘记调用destroy，这也是使用placement new默认遵守的规则，或者保持以前的析构函数，牺牲析构时的性能。placement new的作用是为已经申请好的内存调用构造函数，使用流程为（1）申请内存buf（2）调用placement new：new(buf)construtor()（3）调用析构destructor()（4）释放内存buf。#include<new>可以使用placement new。

3）singleton_pool

pool的加锁版本。

#include <boost/pool/singleton_pool.hpp>

typedef struct student_st

{

char name[10];

int age;

}CStudent;

typedef struct singleton_pool_tag{}singleton_pool_tag;

int main()

{

typedef boost::singleton_pool<singleton_pool_tag,sizeof(CStudent)>global;

CStudent * const df=(CStudent *)global::malloc();

global::free(df);

return 0;

}

singleton_pool为单例类，是对pool的加锁封装，适用于多线程环境，其中所有函数都是静态类型。它的模版参数有5个，tag：标记而已，无意义；RequestedSize：block的长度；UserAllocator：分配子，默认还是default_user_allocator_new_delete；Mutex：锁机制，默认值最终依赖于系统环境，linux下是pthread_mutex，它是对pthread_mutex_t的封装；NextSize：内存不足的时候，申请的block数量，默认是32。最全面的使用singleton_pool类似这样：typedef boost::singleton_pool<singleton_pool_tag,sizeof(CStudent),default_user_allocator_new_delete,details::pool::default_mutex,200>global;

它暴露的函数和pool相同。

4）pool_allocator/fast_pool_allocator

stl::allocator的替换方案。两者都是基于singleton_pool实现，实现了stl::allocator要求的接口规范。两者的使用相同，区别在于pool_allocator的内部实现调用了ordered_malloc和ordered_free，可以满足对大量的连续内存块的分配请求。fast_pool_allocator 的内部实现调用了malloc和free，比较适合于一次请求单个大内存块的情况，但也适用于通用分配，不过具有一些性能上的缺点。因此推荐使用后者。

#include <boost/pool/pool_alloc.hpp>

#include <vector>

typedef struct student_st

{

char name[10];

int age;

}CStudent;

int main()

{

std::vector<CStudent *,boost::fast_pool_allocator<CStudent *> > v(8);

CStudent *pObj=new CStudent();

v[1]=pObj;

boost::singleton_pool<boost::fast_pool_allocator_tag,sizeof(CStudent *)>::purge_memory(); 

return 0;

}

fast_pool_allocator的模版参数有四个：类型，分配子，锁类型，内存不足时的申请的block数量，后三者都有默认值，不再说了。它使用的singleton_pool的tag是boost::fast_pool_allocator_tag。

总结：boost::pool小巧高效，多多使用，多线程环境下使用boost::singleton_pool，不要使用两者的ordered_malloc/ordered_free函数。boost::object_pool不建议使用，可以改造后使用。pool_allocator/fast_pool_allocator推荐使用后者。

参考资料：

boost官方网站: [http://www.boost.org/](http://www.boost.org/)

