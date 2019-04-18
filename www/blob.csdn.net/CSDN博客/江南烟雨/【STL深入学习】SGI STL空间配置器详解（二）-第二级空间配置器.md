# 【STL深入学习】SGI STL空间配置器详解（二）-第二级空间配置器 - 江南烟雨 - CSDN博客
2013年04月17日 11:10:50[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3389
本文讲解SGI STL空间配置器的第二级配置器。
相比第一级配置器，第二级配置器多了一些机制，避免小额区块造成内存的碎片。不仅仅是碎片的问题，配置时的额外负担也是一个大问题。因为区块越小，额外负担所占的比例就越大。
**额外负担**是指动态分配内存块的时候，位于其头部的额外信息，包括记录内存块大小的信息以及内存保护区（判断是否越界）。要想了解详细信息，请参考MSVC或者其他malloc实现。
## SGI STL第二级配置器具体实现思想
如下：
- 如果要分配的区块大于128bytes，则移交给第一级配置器处理。
- 如果要分配的区块小于128bytes，则以**内存池管理（memory pool）**，又称之次层配置（sub-allocation）：每次配置一大块内存，并维护对应的自由链表（free-list）。下次若有相同大小的内存需求，则直接从free-list中取。如果有小额区块被释放，则由配置器回收到free-list中。
下面详细节介绍内存池管理技术。
在第二级配置器中，小额区块内存需求大小都被上调至8的倍数，比如需要分配的大小是30bytes，就自动调整为32bytes。系统中总共维护16个free-lists，各自管理大小为8,16，...，128bytes的小额区块。
为了维护链表，需要额外的指针，为了避免造成另外一种额外的负担，这里采用了一种技术：用union表示链表节点结构：
```cpp
union obj {
        union obj * free_list_link;//指向下一个节点
        char client_data[1];    /* The client sees this. */
  };
```
union能够实现一物二用的效果，当节点所指的内存块是空闲块时，obj被视为一个指针，指向另一个节点。当节点已被分配时，被视为一个指针，指向实际区块。
以下是第二级配置器总体实现代码概览：
```cpp
template <bool threads, int inst>
class __default_alloc_template {
private:
  // 實際上我們應該使用 static const int x = N
  // 來取代 enum { x = N }, 但目前支援該性質的編譯器還不多。
# ifndef __SUNPRO_CC
    enum {__ALIGN = 8};
    enum {__MAX_BYTES = 128};
    enum {__NFREELISTS = __MAX_BYTES/__ALIGN};
# endif
  static size_t ROUND_UP(size_t bytes) {
        return (((bytes) + __ALIGN-1) & ~(__ALIGN - 1));
  }
__PRIVATE:
  union obj {
        union obj * free_list_link;
        char client_data[1];    /* The client sees this. */
  };
private:
# ifdef __SUNPRO_CC
    static obj * __VOLATILE free_list[]; 
        // Specifying a size results in duplicate def for 4.1
# else
    static obj * __VOLATILE free_list[__NFREELISTS]; 
# endif
  static  size_t FREELIST_INDEX(size_t bytes) {
        return (((bytes) + __ALIGN-1)/__ALIGN - 1);
  }
  // Returns an object of size n, and optionally adds to size n free list.
  static void *refill(size_t n);
  // Allocates a chunk for nobjs of size "size".  nobjs may be reduced
  // if it is inconvenient to allocate the requested number.
  static char *chunk_alloc(size_t size, int &nobjs);
  // Chunk allocation state.
  static char *start_free;
  static char *end_free;
  static size_t heap_size;
 /* n must be > 0      */
  static void * allocate(size_t n){...}
 /* p may not be 0 */
  static void deallocate(void *p, size_t n){...}
 static void * reallocate(void *p, size_t old_sz, size_t new_sz);
template <bool threads, int inst>
char *__default_alloc_template<threads, inst>::start_free = 0;//内存池起始位置
template <bool threads, int inst>
char *__default_alloc_template<threads, inst>::end_free = 0;//内存池结束位置
template <bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;
template <bool threads, int inst>
__default_alloc_template<threads, inst>::obj * __VOLATILE
__default_alloc_template<threads, inst> ::free_list[
# ifdef __SUNPRO_CC
    __NFREELISTS
# else
    __default_alloc_template<threads, inst>::__NFREELISTS
# endif
] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
```

## 空间配置函数allocate()
具体实现如下：
- 要分配的区块小于128bytes，调用第一级配置器。
- 否则，向对应的free-list寻求帮助。
- - 对应的free list有可用的区块，直接拿过来用。
- 如果没有可用的区块，调用函数refill()为free list重新填充空间。
代码如下：
```cpp
/* n must be > 0      */
  static void * allocate(size_t n)
  {
    obj * __VOLATILE * my_free_list;
    obj * __RESTRICT result;
    if (n > (size_t) __MAX_BYTES) {
        return(malloc_alloc::allocate(n));
    }
    my_free_list = free_list + FREELIST_INDEX(n);
    // Acquire the lock here with a constructor call.
    // This ensures that it is released in exit or during stack
    // unwinding.
#       ifndef _NOTHREADS
        /*REFERENCED*/
        lock lock_instance;
#       endif
    result = *my_free_list;
    if (result == 0) {
        void *r = refill(ROUND_UP(n));
        return r;
    }
    *my_free_list = result -> free_list_link;
    return (result);
  };
```
这里需要注意的是，每次都是从对应的free list的头部取出可用的内存块。
图示如下：
![](https://img-my.csdn.net/uploads/201304/17/1366166260_4583.jpg)
图一 从free list取出空闲区块示意图
## refill()-为free list填充空间
当发现对应的free list没有可用的空闲区块时，就需要调用此函数重新填充空间。新的空间将取自于内存池。内存池的管理后面会讲到。
缺省状况下取得20个新区块，但是如果内存池空间不够，取得的节点数就有可能小于20.下面是SGI STL中的源代码：
```cpp
/* Returns an object of size n, and optionally adds to size n free list.*/
/* We assume that n is properly aligned.                                */
/* We hold the allocation lock.                                         */
template <bool threads, int inst>
void* __default_alloc_template<threads, inst>::refill(size_t n)
{
    int nobjs = 20;
    char * chunk = chunk_alloc(n, nobjs);
    obj * __VOLATILE * my_free_list;
    obj * result;
    obj * current_obj, * next_obj;
    int i;
    if (1 == nobjs) return(chunk);
    my_free_list = free_list + FREELIST_INDEX(n);
    /* Build free list in chunk */
      result = (obj *)chunk;
      *my_free_list = next_obj = (obj *)(chunk + n);
      for (i = 1; ; i++) {//将各节点串接起来（注意，索引为0的返回给客端使用）
        current_obj = next_obj;
        next_obj = (obj *)((char *)next_obj + n);
        if (nobjs - 1 == i) {
            current_obj -> free_list_link = 0;
            break;
        } else {
            current_obj -> free_list_link = next_obj;
        }
      }
    return(result);
}
```
## chunk_alloc-从内存池中取空间供free list使用
具体实现思想如下：
- 内存池剩余空间完全满足20个区块的需求量，则直接取出对应大小的空间。
- 内存池剩余空间不能完全满足20个区块的需求量，但是足够供应一个及一个以上的区块，则取出能够满足条件的区块个数的空间。
- 内存池剩余空间不能满足一个区块的大小，则
- - 首先判断内存池中是否有残余零头内存空间，如果有则进行回收，将其编入free list。
- 然后向heap申请空间，补充内存池。- heap空间满足，空间分配成功。
- heap空间不足，malloc()调用失败。则- 搜寻适当的free list（适当的是指：尚有未用区块，并且区块足够大），调整以进行释放，将其编入内存池。然后递归调用chunk_alloc函数从内存池取空间供free list。
- 搜寻free list释放空间也未能解决问题，这时候调用第一级配置器，利用out-of-memory机制尝试解决内存不足问题。如果可以就成功，否则排除bad_alloc异常。
源代码如下：
```cpp
/* We allocate memory in large chunks in order to avoid fragmenting     */
/* the malloc heap too much.                                            */
/* We assume that size is properly aligned.                             */
/* We hold the allocation lock.                                         */
template <bool threads, int inst>
char*
__default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs)
{
    char * result;
    size_t total_bytes = size * nobjs;
    size_t bytes_left = end_free - start_free;
    if (bytes_left >= total_bytes) {
        result = start_free;
        start_free += total_bytes;
        return(result);
    } else if (bytes_left >= size) {
        nobjs = bytes_left/size;
        total_bytes = size * nobjs;
        result = start_free;
        start_free += total_bytes;
        return(result);
    } else {
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);//注意此处申请的空间的大小
        // Try to make use of the left-over piece.
        if (bytes_left > 0) {
            obj * __VOLATILE * my_free_list =
                        free_list + FREELIST_INDEX(bytes_left);
            ((obj *)start_free) -> free_list_link = *my_free_list;
            *my_free_list = (obj *)start_free;
        }
        start_free = (char *)malloc(bytes_to_get);
        if (0 == start_free) {
            int i;
            obj * __VOLATILE * my_free_list, *p;
            // Try to make do with what we have.  That can't
            // hurt.  We do not try smaller requests, since that tends
            // to result in disaster on multi-process machines.
            for (i = size; i <= __MAX_BYTES; i += __ALIGN) {
                my_free_list = free_list + FREELIST_INDEX(i);
                p = *my_free_list;
                if (0 != p) {
                    *my_free_list = p -> free_list_link;
                    start_free = (char *)p;
                    end_free = start_free + i;
                    return(chunk_alloc(size, nobjs));
                    // Any leftover piece will eventually make it to the
                    // right free list.
                }
            }
	    end_free = 0;	// In case of exception.
            start_free = (char *)malloc_alloc::allocate(bytes_to_get);
            // This should either throw an
            // exception or remedy the situation.  Thus we assume it
            // succeeded.
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return(chunk_alloc(size, nobjs));
    }
}
```
注意：从heap中配置内存时，配置的大小为需求量的两倍再加上一个随配置次数逐渐增加的附加量。
内存池实例演示：
![](https://img-my.csdn.net/uploads/201304/17/1366168040_4488.jpg)
图二 内存池实例演示
程序一开始，客户调用chunk_alloc（32,20），因为此时内存池和free list空间均不够，于是调用malloc从heap配置40个32bytes区块，其中一个供使用，另一个交给free_list[3]维护。剩余的20个留给内存池。接下来调用chunk_alloc(64,20)， 此 时 free_list[7] 空空如也，必须向记忆池要求支持。记忆池只够供应  (32*20)/64=10 个
 64bytes区块，就把这 10 个区块传回，第 1 个交给客端，余 9个由 free_list[7] 维护。此时记忆池全空。接下来再呼叫chunk_alloc(96, 20)，此时 free_list[11] 空空如也，必须向记忆池要求支持，而记忆池此时也是空的，于是以malloc()配 置 40+n（附加量）个 96bytes 区块，其中第 1 个交出，另 19 个交给 free_list[11] 维护，余 20+n（附加量）个区块留给记忆池……。 
万一山穷水尽，整个system heap 空间都不够了（以至无法为记忆池注入活水源 头），alloc()行动失败，chunk_alloc()就到处寻找有无可用区块， 且区块够大之free lists。找到的话就挖一块交出，找不到的话就调用第一级配 置器。第一级配置器其实也是使用malloc()来配置内存，但它有 out-of-memory 处理机制（类似 new-handler   机制），或许有机会释放其它的内存拿来此处使用。
 如果可以，就成功，否则发出bad_alloc异常。 
## deallocate()-空间释放函数
- 如果需要回收的区块大于128bytes，则调用第一级配置器。
- 如果需要回收的区块小于128bytes，找到对应的free -list，将区块回收。注意是将区块放入free -list的头部。
SGI STL源代码：
```cpp
/* p may not be 0 */
  static void deallocate(void *p, size_t n)
  {
    obj *q = (obj *)p;
    obj * __VOLATILE * my_free_list;
    if (n > (size_t) __MAX_BYTES) {
        malloc_alloc::deallocate(p, n);
        return;
    }
    my_free_list = free_list + FREELIST_INDEX(n);
    // acquire lock
#       ifndef _NOTHREADS
        /*REFERENCED*/
        lock lock_instance;
#       endif /* _NOTHREADS */
    q -> free_list_link = *my_free_list;
    *my_free_list = q;
    // lock is released here
  }
```
## 参考资料：
[Extreme memory usage for individual dynamic allocation](http://stackoverflow.com/questions/3277898/extreme-memory-usage-for-individual-dynamic-allocation)
[(C) how does a heap allocator handle a 4-byte block header,
 while only returning addresses that are multiples of 8?](http://stackoverflow.com/questions/2947085/c-how-does-a-heap-allocator-handle-a-4-byte-block-header-while-only-returning?lq=1)
下面详细介绍内存池管理技术。
