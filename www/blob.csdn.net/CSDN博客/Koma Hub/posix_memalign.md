# posix_memalign - Koma Hub - CSDN博客
2019年03月30日 21:01:47[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：16
个人分类：[Posix](https://blog.csdn.net/Rong_Toa/article/category/8812502)
**Table of Contents**
[预对齐内存的分配](#%E9%A2%84%E5%AF%B9%E9%BD%90%E5%86%85%E5%AD%98%E7%9A%84%E5%88%86%E9%85%8D)
[其它和对齐有关的](#%E5%85%B6%E5%AE%83%E5%92%8C%E5%AF%B9%E9%BD%90%E6%9C%89%E5%85%B3%E7%9A%84)
# 预对齐内存的分配
在大多数情况下，编译器和C库透明地帮你处理对齐问题。POSIX 标明了通过*malloc( )*, *calloc( )*, 和 *realloc( )* 返回的地址对于任何的C类型来说都是对齐的。在*Linux*中，这些函数返回的地址在32位系统是以8字节为边界对齐，在64位系统是以16字节为边界对齐的。有时候，对于更大的边界，例如页面，程序员需要动态的对齐。虽然动机是多种多样的，但最常见的是直接块I/O的缓存的对齐或者其它的软件对硬件的交互，因此，*POSIX 1003.1d*提供一个叫做*posix_memalign( )*的函数：
```cpp
void *srslte_vec_malloc(uint32_t size) {
  void *ptr;
  if (posix_memalign(&ptr, SRSLTE_SIMD_BIT_ALIGN, size)) {
    return NULL;
  } else {
    return ptr;
  }
}
void *srslte_vec_realloc(void *ptr, uint32_t old_size, uint32_t new_size) {
#ifndef LV_HAVE_SSE
  return realloc(ptr, new_size);
#else
  void *new_ptr;
  if (posix_memalign(&new_ptr, SRSLTE_SIMD_BIT_ALIGN, new_size)) {
    return NULL;
  } else {
    memcpy(new_ptr, ptr, old_size);
    free(ptr);
    return new_ptr;
  }
#endif
}
```
调用*posix_memalign( )*成功时会返回*size*字节的动态内存，并且这块内存的地址是*alignment*的倍数。参数*alignment*必须是2的幂，还是*void*指针的大小的倍数。返回的内存块的地址放在了*memptr*里面，函数返回值是*0*.
调用失败时，没有内存会被分配，*memptr*的值没有被定义，返回如下错误码之一：
*EINVAL*
参数不是2的幂，或者不是*void*指针的倍数。
*ENOMEM*
没有足够的内存去满足函数的请求。
要注意的是，对于这个函数，*errno*不会被设置，只能通过返回值得到。
由*posix_memalign( )*获得的内存通过*free( )*释放。
```cpp
char *buf;
int ret;
/* allocate 1 KB along a 256-byte boundary */
ret = posix_memalign (&buf, 256, 1024);
if (ret) {
    fprintf (stderr, "posix_memalign: %s\n",
             strerror (ret));
    return -1;
}
/* use 'buf'... */
free (buf);
```
# 其它和对齐有关的
与对齐有关的问题的范围要超过标准类型的自然对齐和动态存储器地分配。例如，非标准和复杂的类型比标准类型有更复杂的要求。另外，对对齐的关注在给指向不同类型的指针赋值和使用强转时显得加倍的重要。
**非标准类型**。非标准和复杂的数据类型的对齐比简单的自然对齐有着更多的要求。这里四个有很有用的方法：
- 一个结构的对齐要求是和它的成员中最大的那个类型一样的。例如，一个结构中最大的是以4字节对齐的32bit的整形，那么这个结构至少以4字节对齐。
- 结构也引入了填充的需要，用来保证每一个成员都符合自己的对齐要求。所以，如果一个char (可能以1字节对齐)后跟着一个int (可能以4字节对齐)，编译器会自动地插入3个字节作为填充来保证int以4字节对齐。
- 程序员有时候排列结构里面的成员-例如，以大小来递减-来是使用作填充的垃圾空间最少。GCC的选项- Wpadded能对这些努力有帮助，因为它使得在编译器偷偷插入填充时产生警告。
- 一个联合的对齐和联合里最大的类型一样。
- 一个数组的对齐和数组里的元素一样。所以，数组的对齐并不比单单的一个成员严格，这样能使数组里面的所有成员都是自然对齐的。
