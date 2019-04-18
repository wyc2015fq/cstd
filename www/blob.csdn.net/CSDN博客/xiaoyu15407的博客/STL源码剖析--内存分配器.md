# STL源码剖析--内存分配器 - xiaoyu15407的博客 - CSDN博客
2015年06月11日 23:49:31[xiaoyu15407](https://me.csdn.net/xiaoyu15407)阅读数：293
### SGI STL的内存分配器
内存分配器是STL中非常重要的一个概念,STL的各种容器都会使用内存分配器来对内存进行管理,SGI STL 提供了两种内存分配器，一个叫**__malloc_alloc_template**一个叫**__default_alloc_template**，他们两个都是以模板的方式实现.下面来看看他们的定义
```cpp
//__malloc_alloc_template的定义如下
template <int inst>
class __malloc_alloc_template 
{
    private:
    static void *oom_malloc(size_t);
    static void *oom_realloc(void *, size_t);
    static void (* __malloc_alloc_oom_handler)();
    public:
    static void * allocate(size_t n)
    {
        void *result = malloc(n);
        if (0 == result) 
            result = oom_malloc(n);
        return result;
    }
    static void deallocate(void *p, size_t /* n */)
    {
        free(p);
    }
    static void * reallocate(void *p, size_t /* old_sz */, size_t new_sz)
    {
        void * result = realloc(p, new_sz);
        if (0 == result)
            result = oom_realloc(p, new_sz);
        return result;
    }
    static void (* set_malloc_handler(void (*f)()))()
    {
        void (* old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return(old);
    }
};
template <int inst>
void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;
template <int inst>
void * __malloc_alloc_template<inst>::oom_malloc(size_t n)
{
    void (* my_malloc_handler)();
    void *result;
    for (;;) {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)();
        result = malloc(n);
        if (result) return(result);
    }
}
template <int inst>
void * __malloc_alloc_template<inst>::oom_realloc(void *p, size_t n)
{
    void (* my_malloc_handler)();
    void *result;
    for (;;) {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)();
        result = realloc(p, n);
        if (result) return(result);
    }
}
```
从上面的定义可以看出__malloc_alloc_template模板类只是对malloc和free进行了简单的封装,加上一些异常处理，当malloc不能成功分配内存的时候,调用oom_malloc不停的进行内存分配直到成功分配到内存为止或者当异常处理函数为空时抛出异常，__malloc_alloc_template对于分配较小的内存并没有什么优势跟直接调用malloc和free没什么两样。当我们申请一块内存的时候，系统会额外的多分配一点内存空间用来存储记录一些内存分配的大小信息，在释放内存时提供参考，所以当分配较小内存的时候，内存的利用率是非常低的，而且还会造成许多内存碎片。因此 SGI STL提供了另一种内存分配器叫__default_alloc_template，从名字可以看出他是默认的内存分配器，这个内存分配器采用内存池的方式管理内存下面来看看它的定义
```cpp
//__default_alloc_template的定义
template <bool threads, int inst>
class __default_alloc_template 
{
private:
    enum {__ALIGN = 8};
    enum {__MAX_BYTES = 128};
    enum {__NFREELISTS = __MAX_BYTES/__ALIGN};
    static size_t ROUND_UP(size_t bytes){
        return (((bytes) + __ALIGN-1) & ~(__ALIGN - 1));
    }
    union obj {
        union obj* free_list_link;
        char client_data[1];
    };
    static obj* __VOLATILE free_list[__NFREELISTS]; 
    static size_t FREELIST_INDEX(size_t bytes) {
        return (((bytes) + __ALIGN-1)/__ALIGN - 1);
    }
    static void *refill(size_t n);
    static char *chunk_alloc(size_t size, int &nobjs);
    static char *start_free;
    static char *end_free;
    static size_t heap_size;
public:
    static void* allocate(size_t n);
    static void  deallocate(void *p, size_t n);
    static void* reallocate(void *p, size_t old_sz, size_t new_sz);
};
```
__default_alloc_template 各个成员的作用 
**enum {__ALIGN = 8}; **指定分配的内存按照8个字节对齐，即申请的内存总是8个字节的倍数 
**enum {__MAX_BYTES = 128};**
指定最大字节数，当申请的内存小于这个值的时候才采用__default_alloc_template分配器分配内存，当大于这个值的时候直接使用__malloc_alloc_template来进行内存分配 
**enum {__NFREELISTS = __MAX_BYTES/__ALIGN};**
指定自由链表指针数组的元素个数 
**static size_t ROUND_UP(size_t bytes);**
按照__ALIGN指定的大小将待分配内存的大小进行向上取整以满足__ALIGN的倍数
