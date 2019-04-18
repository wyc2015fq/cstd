# 共享Windows下C++库之内存池篇 - ljx0305的专栏 - CSDN博客
2010年08月23日 16:49:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：880
目录结构
内存池简介 
介绍分析内存池改进及所使用技术 
如何配合STL容器 
性能测试 
如何使用 
内存池简介：
关于内存池的介绍，好文很多（这里，这里，这里，还有这里…）。在如今的PC机器上，内存池的作用也没有那么明显了，操作系统对内存的管理已经相当不错。但，为什么还需要内存池呢？两点：1. 减少内存管理负担，提升性能。（比如，在进行密集型算法前申请足够内存内存，然后使用，最后统一释放） 2.减少页面错误（页面错误是由虚拟内存与物理内存交换导致的），减少内存碎片，提高性能。
介绍分析内存池改进及所使用技术：
该套内存池组件是由SGI内存改造而来，其原理请参考《STL源码剖析》，或者参阅这里，借用一幅图来说明
在此基础上，我增加了几个重要的改进：
增加模板参数是否为多线程，根据该参数判断是否需要使用Lock及变量是否需要volatile修饰。 
增加模板参数分配区块上限，默认为256。 
增加模板参数分配内存策略，提供三种分配方式：VirtualAlloc、HeapAlloc、malloc。 
增加对已申请内存管理，集中释放。 
在此，讲下内存分配策略的选择。
// Win32 上分配内存方式struct VirtualAllocateTraits{    static void *Allocate(size_t size)    {        // 将指定的内存页面始终保存在物理内存上，不许它交换到磁盘页文件中        void *p = ::VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_EXECUTE_READWRITE);        ::VirtualLock(p, size);        return p;    }    static void Deallocate(void *p, size_t size)    {        ::VirtualUnlock(p, size);        ::VirtualFree(p, size, MEM_RELEASE);    }};struct HeapAllocateTraits{    static HANDLE GetHeap()    {        static HANDLE heap = NULL;        if ( heap == NULL )        {            heap = ::HeapCreate(0, 0, 0);            // 设置低碎片堆            ULONG uHeapFragValue = 2;            ::HeapSetInformation(heap, HeapCompatibilityInformation, &uHeapFragValue, sizeof(ULONG));        }        return heap;    }    static void *Allocate(size_t size)    {        return ::HeapAlloc(GetHeap(), HEAP_ZERO_MEMORY, size);;    }    static void Deallocate(void *p, size_t /* size*/)    {        ::HeapFree(GetHeap(), 0, p);    }};struct MallocAllocateTraits{    static void *Allocate(size_t size)    {        return std::malloc(size);    }    static void Deallocate(void *p, size_t)    {        return std::free(p);    }}; 
默认时采用MallocAllocateTraits，即使用malloc来分配内存。如果是需要对内存的进一步控制，比如使用异步读取文件时，需要使用锁定的内存；在网络层中往往需要锁定内存时就可以选择VirtualAllocTraits，或者可以自行定制，仅仅需要满足Allocate和Deallocate接口约束即可。
如何搭配STL容器：
首先，STL提出了allocator的概念，把数据的与内存分配释放分离即内存管理从容器的实现独立出来，这是一个很了不起的革命性变化。请看相关讨论（这里， 这里，这里）。然后，为了使该内存池符合allocator的接口，需要一个adapter—SGIAllocator.
示例：
typedef std::vector<int, SGIAllocator<int, false>> SVector;typedef std::list<int, SGIAllocator<int, false, 1024>> SList;性能测试：
分别以std::new,std::allocator和boost::pool作为参照物进行比较。（loki的不想测试）
在固定循环10000次，按8byte、64byte、1K、4K为大小进行了4次测试。每次测试间隔走5次，取平均值。一下为测试用例的一部分：
for(DWORD i = 0; i != dwCount; ++i){    arr[i] = new char[dwSize];    memset(arr[i], i, dwSize);    if( i % 2 == 0 )        delete arr[i];}for(DWORD i = 0; i != dwCount; ++i){    if( i % 2 != 0 )        delete arr[i];} 
性能比较测试如下，以std::new为基准
很明显，在小数据时，申请释放次数越多，内存池越占优势。当数据较大时，内存池与标准分配方式的差距会更小。但是，作为服务端程序来讲，鼓励使用内存池来进行内存管理。呵呵，不言自明~
内存池使用方式：
// SGIMemoryPool,单线程不加锁 malloc分配策略typedef Allocator<char, SGIMemoryPool<false, dwSize> > SGIAllocT;SGIAllocT alloc2;// SGIMemoryPool,单线程不加锁 malloc分配策略dwLast = 0;{    QPerformanceTimer perf(dwLast);    for(DWORD i = 0; i != dwCount; ++i)    {        arr[i] = alloc2.Allocate(dwSize);        memset(arr[i], i, dwSize);        if( i % 2 == 0 )            alloc2.Deallocate(arr[i], dwSize);    }    for(DWORD i = 0; i != dwCount; ++i)    {        if( i % 2 != 0 )            alloc2.Deallocate(arr[i], dwSize);    }}cout << "SGIAllocT: " << dwLast << endl;不足：
该内存池同样没有提供释放某一部分内存的接口，遗憾~~
欢迎喜欢的朋友进行下载试用，如果有更好的建议请告诉我，我一定会改进，谢谢！
下载：
目前就不放到Google Code上，还是在CSDN老地方下载吧。猛击这里
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/chenyu2202863/archive/2010/08/17/5818929.aspx](http://blog.csdn.net/chenyu2202863/archive/2010/08/17/5818929.aspx)
