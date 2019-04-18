# c++内存分配优先使用内存池，而不是new，delete - gauss的专栏 - CSDN博客
2011年12月26日 22:36:02[gauss](https://me.csdn.net/mathlmx)阅读数：355标签：[delete																[c++																[object																[测试																[java																[设计模式](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)
# 认识一下new和delete的开销：
new和delete首先会转调用到malloc和free，这个大家应该很熟识了。很多人认为malloc是一个很简单的操作，其实巨复杂，它会执行一个系统调用(当然不是每一次，windows上是按页算)，该系统调用会锁住内存硬件，然后通过链表的方式查找空闲内存，如果找到大小合适的，就把用户的进程地址映射到内存硬件地址中，然后释放锁，返回给进程。
如果在多线程环境下，进程内的分配也会上锁，跟上面类似，不过不是以页，而是以分配的内存为单位。
delete是一个反过程。
相对的，如果不是使用堆分配，而是直接在栈上分配，比如类型int，那么开销就是把sp这个寄存器加上sizeof(int)。
# 内存池模式：
内存池就是预先分配好，放到进程空间的内存块，用户申请与释放内存其实都是在进程内进行，SGI-STL的alloc遇到小对象时就是基于内存池的。只有当内存池空间不够时，才会再从系统找一块很大的内存。
内存池模式是如此之重要，以至于让我想不明白为什么四人帮那本《设计模式》没有把内存池列为基本模式，目前其它的教材，包括学院教材，实践教材都没有列出这个模式(讲线程池模式的教材倒非常多)。可能他们认为这不属于设计，而属于具体实现吧。但我觉得这样的后果是间接把很多c++
 fans带向低效的编码方式。
sun公司就挺喜欢搞一些算法，用c++实现与java实现一遍，结果显示c++的效率有时甚至比java低，很多c++高手看了之后都会觉得很难解，其实有玄机：java的new其实是基于内存池的，而c++的new是直接系统调用。
# c++内存池模式的发展：
       c++98标准之前，基本上大多数程序员没用使用内存池，c++98
标准之后，内存池的使用也只是停留在STL内部的使用上，并没有得到推广。
其实我认为，STL的内存分配模式是一场变革，它不但包含内存分配的革命，也包含了内存管理(这个话题先放一边)的革命，只是这场变革被很多人忽略了。也有一些人认为STL的内存分配方案有潜在问题，就是只管从系统分配，但却永远不会调用系统级的释放，如果使用不当，程序拿住的内存会越来越多。我自己工作过的项目没遇上过这样的问题，但之前营帐报表组的一个容灾项目倒是遇上了。不过STL的内存模式没有推广最大的原因还是因为alloc不是标准组件，以至于被人忽略了。
       STL之后，一些c++ fans们开始搞出了几套内部使用的内存池。为了项目需要，我自己也曾经做过一个。但这些都没有很正式的公开，而且也不完美。
大概在200x年(-_-!)，主导c++标准的一群牛人发起了一个叫boost的项目，才正式的把内存池带到实用与标准化阶段。
插入一点题外话：关于boost，很多人(包括我自己也曾经)产生误解，认为它是准标准库，是下一代标准库。其实boost是套基础建设，用来证明哪些方案是可行，哪些是不可行的，它里面的一些组件有可能会出局，也有可能不是以库的方式存在，而是以语言核心的方式存在，下一代标准库名字叫TR1，再一下代叫TR2(我对使用TR这个名字很费解，为什么不统一叫STL呢)。
# new,delete调用与内存池调用的效率对比：
讲了这么多费话，要到关键时候了，用事例来证明为什么要优先使用内存池。下面这段代码是我很久以前的一段测试案例，细节上可能有点懂难，但流程还是清晰的：
#include<time.h>
#include<boost/pool/object_pool.hpp>
structCCC
{
CCC() {}
chardata[10];
};
structSSS
{
SSS() {}
shortdata[10];
};
structDDD
{
DDD() {}
doubledata[10];
};
// 把new,delete封装为一个与boost::object_pool一样的接口，以便于测试
template <typenameelement_type, typenameuser_allocator = boost::default_user_allocator_malloc_free>
classnew_delete_alloc
{
public:
element_type* construct() { 
returnnewelement_type; }
voiddestroy(element_type*
constchunk) { 
deletechunk; }
};
template
< 
template<typename, 
typename> 
classallocator
> 
doubletest_allocator()
{
// 使用了一些不规则的分配与释放，增加内存管理的负担
// 但总体流程还是很规则的，基本上不产生内存碎片，要不然反差效果会更大。
allocator<CCC> 
c_allc;
allocator<SSS> 
s_allc;
allocator<DDD> 
d_allc;
doublere = 0;
// 随便作一些运算，仿止编译器优化掉内存分配的代码
for (unsignedinti = 0; 
i < 10000; ++i)
    {
for (unsignedintj = 0; 
j < 10000; ++j)
        {
CCC* pc = c_allc.construct();
SSS* ps = s_allc.construct();
re += pc->data[2];
c_allc.destroy(pc);
DDD* pd = d_allc.construct();
re += ps->data[2];
re += pd->data[2];
s_allc.destroy(ps);
d_allc.destroy(pd);
        }
    }
returnre;
}
intmain(int
argc, char* argv[])
{
doublere1 = 0;
doublere2 = 0;
// 运行内存池测试时，基本上对我机器其它进程没什么影响
time_tbegin = 
time(0);
re1 = test_allocator<boost::object_pool>();
// 使用内存池boost::object_pool
time_tseporator = 
time(0);
// 运行到系统调用测试时，感觉机器明显变慢，
// 如果再加上内存碎片的考虑，对其它进程的影响会更大。
std::cout << long(seporator -
begin) << std::endl;
re2 = test_allocator<new_delete_alloc>();          
// 直接系统调用
std::cout << long(time(0) -
seporator) << std::endl;
std::cout << re1 <<
re2 << std::endl;
}
# 总结：
在一个100000000次的循环中，使用内存池是3秒，使用系统调用是93秒。
可能会有人觉得100000000这个数很大，93秒没什么，但想一下，一个表有几千万行是很正常的，如果每行有十多列，每列有数据类型，数据长度，数据内容。如果在这样的一个循环错误的使用了new和delete。
而且以上测试还没有考虑到碎片的影响，以及运行该程序时对其它程序的影响。而且还有一点，就是机器的内存硬件容量越大，内存分配时，需要搜索的时间就可能越长，如果内存是多条共同工作的，影响就再进一步。
什么算是错误的使用呢，比如返回一个std::string给用户，有人觉得new出来返回指针给用户会更好，你可能会想到如果new的话，只产生一次string的构造，如果直接返回对象可能需要多次构造，所以new效率更高。但事实不是这样，虽然在构造里会有字符串的分配，但其实这个分配是在内存池中进行的，而你直接的那个new就肯定是系统调用。
当然，有些情况是不可说用什么就用什么的，但如果可选的话，优先使用栈上的对象，其次考虑内存池，然后再考虑系统调用。
