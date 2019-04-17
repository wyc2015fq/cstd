# [CareerCup] 13.9 Aligned Malloc and Free Function 写一对申请和释放内存函数 - Grandyang - 博客园







# [[CareerCup] 13.9 Aligned Malloc and Free Function 写一对申请和释放内存函数](https://www.cnblogs.com/grandyang/p/4932046.html)







13.9 Write an aligned malloc and free function that supports allocating memory such that the memory address returned is divisible by a specific power of two.
 EXAMPLE
 align_malloc (1000,128) will return a memory address that is a multiple of 128 and that points to memory of size 1000 bytes.
 aligned_free() will free memory allocated by align_malloc



这道题让我们写一对申请和释放内存的函数，而且要求我们申请的内存块的起始地址要能够被2的倍数整除。题目中给了例子，让我们申请1000字节大小的内存空间，且起始地址要能被128整除。我们知道，在使用malloc申请内存时，我们无法控制堆中具体哪块内存被申请了，如果我们非要首地址被128整除，那么我们需要些小技巧：我们需要再申请127个字节大小，这样我们的首地址即时申请到的不能被128整除，我们也能够调整到能整除的位置，并且后面的内存空间也足够1000字节，可以使用如下的代码来申请：

```
void* aligned_malloc(size_t required_bytes, size_t alignment) {
    int offset = alignment - 1;
    void *p = (void*)malloc(required_bytes + offset);
    void *q = (void*)(((size_t)(p1) + offset) & ~(alignment - 1));
    return q;
}
```

我们申请了额外的空间alignment-1，然后再调整首地址的位置通过和~(alignment-1)相与，得到能被alignment整除的地址，下面我们就要来看如何释放内存，由于我们多申请了offset的内存，我们最终也需要将这些内存释放掉，我们需要一个指针来记录整个内存块的起始位置，由于新加了指针，别忘了还要给指针申请空间，所以我们需要多申请的空间大小为alignment - 1 + sizeof(void*)，参见如下代码：



```
class Solution {
public:
    void* aligned_malloc(size_t required_bytes, size_t alignment) {
        void *p1;
        void **p2;
        int offset = alignment - 1 + sizeof(void*);
        if ((p1 = (void*)malloc(required_bytes + offset)) == NULL) {
            return NULL;
        }
        p2 = (void**)(((size_t)(p1) + offset) & ~(alignment - 1));
        p2[-1] = p1;
        return p2;
    }
    void aligned_free(void *p2) {
        void *p1 = ((void**)p2)[-1];
        free(p1);
    }
};
```

我们定义一个双指针p2，在-1的位置存上p1，也就是整个申请的内存块的起始地址，在0的位置存上能被alignment整除的位置，释放内存时从p2中提取出p1，将p1释放掉即可。












