# 有感于STL的内存管理 - gauss的专栏 - CSDN博客
2013年01月08日 13:03:40[gauss](https://me.csdn.net/mathlmx)阅读数：222
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)
1. 背景
前些天在一个技术分享会上，某大牛说，STL使用了内存池，释放内存的时候，并不释放给OS，而是自己由留着用。
听到这些观点后，我就有些着急了，因为我以前一直是直接使用STL的一些工具类的，比如std::string、std::map、std::vector、std::list等等，从来都没有关注过内存的问题。
带着内存的问题，我花了两三天的时间去阅读STL的代码，并且写一些简单的程序进行测试；下面列举一些心得体会，但是却没有什么大的结论 -.- 
2. 容易误解的简单例子
我们以STL中的map为例，下面有一个使用map的简单例子，大部分人可以在30秒内写好。
void testmap()
{
  map<int, float> testmap;
  for (int i = 0; i < 1000000; i++) {
    testmap[i] = (float)i;
  }
  testmap.clear();
}
为了在调用map::clear()之后查看进程的内存使用量，我们可以加几行代码让程序暂停一下。
void testmap()
{
  map<int, float> testmap;
  for (int i = 0; i < 1000000; i++) {
    testmap[i] = (float)i;
  }
  testmap.clear();
  // 观察点
  int tmp; cout << "use ps to see my momory now, and enter int to continue:"; cin >> tmp;
}
编译运行上面的程序，你会看见这样的情况：ps显示进程的内存使用量为40MB多。这时，你会毫不犹豫地说，STL的map使用了内存池（memory pool）。
然后，我就跑去阅读libstdc++的STL的源代码，STL提供了很多种Allocator的实现，有基于内存池的，但是默认的std::allocator的实现是new_allocator，这个实现只是简单的对new和delete进行了简单的封装，并没有使用内存池。这样，怀疑的对象就转移到glibc的malloc函数了。malloc提供的两个函数来查看当前申请的内存的状态，分别是malloc_stats()和mallinfo()，它们都定义在<malloc.h>里。
为了弄清楚这个问题，我们对上面的例子进行如下的改造：
#include <malloc.h>
void testmap()
{
  malloc_stats();        // <======== 观察点1
  map<int, float> testmap;
  for (int i = 0; i < 1000000; i++) {
    testmap[i] = (float)i;
  }
  malloc_stats();        // <======== 观察点2
  testmap.clear();
  malloc_stats();        // <======== 观察点3
}
这个例子的运行环境是这样的：
[dengmw@my ~]$ g++ -v
Reading specs from /usr/lib/gcc/x86_64-redhat-linux/3.4.6/specs
Configured with: ../configure --prefix=/usr --mandir=/usr/share/man --infodir=/usr/share/info --enable-shared --enable-threads=posix --disable-checking --with-system-zlib --enable-__cxa_atexit --disable-libunwind-exceptions --enable-java-awt=gtk --host=x86_64-redhat-linux
Thread model: posix
gcc version 3.4.6 20060404 (Red Hat 3.4.6-9)
程序的运行结果是这样的：
在观察点1：
*       system bytes     =          0
*       in use bytes     =          0
在观察点2：
*       system bytes     =          48144384
*       in use bytes     =          48005120
在观察点3：
*       system bytes     =          48140288    <==== malloc cache the memory here
*       in use bytes     =          5120
很明显，尽管程序员显式地调用了map::clear()，但是malloc并没有把这些内存归还给OS，而是缓存起来了。所以说，这个例子的罪魁祸首并不是libstdc++的的STL，而是glibc的malloc。
3. 侯捷的《STL源码剖析》有点过时了
在调试上面的例子的时候，我在看了不少的书籍和网上的文章，其中就包括了侯捷的《STL源码剖析》，但是这本书已经过时了，因为他写这本书的时候，g++的版本才2.9。我把g++的各个版本的源代码都下载下来了，并且进行了比较，总结如下：
侯捷的《STL源码剖析》只对于gcc-3.3.*及以前的版本是对的；对于gcc-3.4.*以后的版本，STL中关于内存的代码变了当前，大家使用的gcc大都是3.4.6版本或者更加新的版本gcc-3.3分支从2003-05-13发布第1版，到2005-05-03发布3.3.6gcc-3.3的默认的Allocator，定义在"include/bits/stl_alloc.h"里，确实是带有cache的
 (即常说的memory pool)gcc-3.4的默认的Allocator，定义在"include/bits/allocator.h"里，它的真实的实现是"include/ext/new_allocator.h"，这个实现不带cache，只是new和delete的简单封装
4. STL内存管理的基础知识(gcc-3.4.*及以后的)
通过这次对STL的研究，我学到不不少新的知识。可能这些内容你都已经会了，-.-，我比较弱，下面的内容我是第一次知道的：
STL有很多种allocator，默认采用的是std::allocator，我们沿着这样的头文件路线，可以找到它的最终实现：
-> "include/bits/allocator.h"
-> "include/i386-redhat-linux/bits/c++allocator.h"
-> "include/ext/new_allocator.h"（即是说，std::allocator == __gnu_cxx::new_allocator）
根据C++的标准，STL的allocator，把对象的申请和释放分成了4步：
第1步：申请内存空间，对应函数是allocator::allocate()第2步：执行构造函数，对应函数是allocator::construct()第3步：执行析构函数，对应函数是allocator::destroy()第4步：释放内存空间，对应函数是allocator::deallocate()STL崇尚拷贝，你往容器里放东西或者从容器里取东西，都是要调用拷贝构造函数的。比如，你有一个对象a要插入到map里，过程是这样的：
map先申请一个结点的空间调用拷贝构造函数初始化该结点把新结点插入到map的红黑树中
STL中实现了好多种不同的更为具体的allocator，如下（[](http://gcc.gnu.org/on%3Cwbr%3Elinedocs/libstdc++/manual/memory.html)[GNU
 GCC关于Memory的官方文档](http://gcc.gnu.org/on%3Cwbr%3Elinedocs/libstdc++/manual/memory.html)）：
__gnu_cxx::new_allocator: 简单地封装了new和delete操作符，通常就是std::allocator__gnu_cxx::malloc_allocator: 简单地封装了malloc和free函数__gnu_cxx::array_allocator:
 申请一堆内存__gnu_cxx::debug_allocator: 用于debug__gnu_cxx::throw_allocator: 用于异常__gnu_cxx::__pool_alloc:
 基于内存池__gnu_cxx::__mt_alloc: 对多线程环境进行了优化__gnu_cxx::bitmap_allocator: keep track of the used and unused memory locations.上面的8个allocator的实现中，bitmap_allocator、pool_allocator和__mt_alloc是基于cache的，其它的不基于cache
* 那么？如何指定使用一个特殊的allocator呢？示例如下：
map<int, int> a1;                                    // 方法1
map<int, int, less<int>, std::allocator<pair<int, int> > > a3;      // 方法2
// 方法3，方法1、方法2、方法3都是等价的map<int, int, less<int>, __gnu_cxx::new_allocator<pair<int, int> > > a2; 
// 方法4，使用了基于cache的allocatormap<int, int, less<int>, __gnu_cxx::__pool_alloc<pair<int, int> > >  a4; 
5. 内存碎片是容易被忽视的导致OutOfMemory的原因
这个观点有点类似于磁盘碎片，也可以称为内存碎片吧，当内存碎片过多的时候，极容易出现OutOfMemory错误；
使用STL的map特别容易出现这种情况，往map里插入了海量的小对象，然后释放了一些，然后再想申请内存时，就出现OutOfMemory错误了；
这种现象不只是在使用STL的情况会发现，下面举一个例子来说明内存碎片的问题，尽管这个例子没有使用STL。
举例之前，先说明一下这个例子中使用的两个查看当前进程的内存统计量的2个函数：
int get_max_malloc_length_inMB() : 得到当前可以申请的最长的内存长度(MB)；这个函数不停地调用p=malloc(length*1024*1024)；如果成功，则length++，并且free(p)；如果失败，返回(length-1)。int
 get_free_mem_inKB() : 得到当前可以申请的内存总量(KB)；这个函数不停地调用malloc(1024)来申请1KB的内存；如果成功，把这1KB的内存存起来，并且count++；如果失败，则把所有的1KB内存释放，再返回count。为了测试方便，我在运行程序前，设置了进程的最大内存为200MB，使用的命令如下：
ulimit -m 204800;
ulimit -v 204800;
这个例子把申请到的内存以矩阵的形式存储起来，先按列优先把指针存起来，再按行优先进行free，这样会造成大量的内存碎片；例子的伪代码如下：
typedef char* PtrType;
PtrType ** Ptrs = (PtrType**) malloc( ROW * sizeof(PtrType*) );
...
// 第1步: 占领所有的内存，按列优先进行申请
for(j=0; j<COL; ++j) {
    for(i=0; i<ROW; ++i) {
        Ptrs[j][i] = malloc(1024);
    }
}
// 第2步：按行优先释放所有的内存，在中间多次调用get_max_malloc_length_inMB和get_free_mem_inKB来查看内存使用情况
for (i=0; i<ROW; ++i) {
    for (j=0; j<COL; ++j) {
        free( Ptrs[i][j] );
    }
    free(Ptrs[i]);
    // 得到两个关于内存的统计量
    get_max_malloc_length_inMB();
    get_free_mem_inKB();
}
// 第3步：释放Ptrs，再获取一次内存的统计量
free(Ptrs);
get_max_malloc_length_inMB();
get_free_mem_inKB();
需要关注的是，内存的申请的顺序是按列优先的，而释放的顺序是按行优先的，这种做法就是模拟内存的碎片。<BR>
运行上面的程序后，得到的结果是：在释放内存的过程中，max_malloc_length_inMB长期保持在0 MB，当全部释放完后，max_malloc_length_inMB变成了 193 MB<BR>
max_malloc_length_inMB: 
    196 MB -> 0 MB -> 0 MB -> ... -> 0 MB -> 0 MB -> ... 
-> 0 MB -> 0 MB -> 195 MB
free_mem_inKB: 
    199374 KB -> 528 KB -> 826 KB -> ... -> 96037 KB -> 96424 KB -> ... 
-> 197828 KB -> 198215 KB -> 198730 KB
上面的结果引申出这样的结论：
OutOfMemory错误，并不一定是内存使用得太多；当一个程序申请了大量的小内存块 (比如往std::map中插入海量的小对象)，导致内存碎片过多的话，一样有可能出现OutOfMemory错误
6. 一些别的收获
6.1 libc.so.6和glibc-2.9有什么不同？
参考文献：http://en.wikipedia.org/wiki/GNU_C_Library在80年代，FSF写了glibc；后来，linux kernel的人照着glibc，写了"Linux libc"，一直从libc.so.2到libc.so.5到1997年，FSF发布了glibc-2.0，这个版本有很多优点，比如支持有更多的标准，更可移植；linux kernel的人就把"Linux libc"的项目砍掉了，重新使用glibc-2.0，然后就命名为libc.so.6如果你运行一下这个命令"ls -lh
 /lib/libc.so.6"，你会发现它其实是一个符号链接，在我的电脑上，它指向了"/lib/libc-2.9.so"6.2 申请内存的方式共有多少种？
参考文献：glibc manual中的第3章（见http://www.gnu.org/software/libc/manual/）execfork进程内：global var or static varlocal varmalloc()memory map file
            
