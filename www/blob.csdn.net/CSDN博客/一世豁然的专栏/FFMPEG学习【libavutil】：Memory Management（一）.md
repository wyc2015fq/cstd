# FFMPEG学习【libavutil】：Memory Management（一） - 一世豁然的专栏 - CSDN博客





2017年07月18日 10:39:41[一世豁然](https://me.csdn.net/Explorer_day)阅读数：271标签：[ffmpeg](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)
个人分类：[ffmpeg](https://blog.csdn.net/Explorer_day/article/category/6289310)









一、宏对齐

帮助宏用于声明对齐变量。





一）、宏


|#define|[DECLARE_ALIGNED](http://ffmpeg.org/doxygen/trunk/group__lavu__mem__macros.html#ga7a422611f83afb33d6e0da44184b548e)([n](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a76f11d9a0a47b94f72c2d0e77fb32240), t, v)   t __attribute__ ((aligned ([n](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a76f11d9a0a47b94f72c2d0e77fb32240)))) v|
|----|----|

声明在内存中对齐的变量。



```cpp
DECLARE_ALIGNED(16, uint16_t, aligned_int) = 42;
DECLARE_ALIGNED(32, uint8_t, aligned_array)[128];
// The default-alignment equivalent would be
uint16_t aligned_int = 42;
uint8_t aligned_array[128];
```
参数：n：最小对齐字节

   t：变量（或数组元素）的类型

   v：变量的名称







|#define|[DECLARE_ASM_CONST](http://ffmpeg.org/doxygen/trunk/group__lavu__mem__macros.html#ga6e5cdd0ac639d7dea5033df3b64cfe3c)([n](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a76f11d9a0a47b94f72c2d0e77fb32240), t, v)   static const t [av_used](http://ffmpeg.org/doxygen/trunk/attributes_8h.html#aa402443f9068fb645e9a55f41fc99e09) __attribute__ ((aligned ([n](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a76f11d9a0a47b94f72c2d0e77fb32240)))) v|
|----|----|

声明适用于内联汇编代码的静态常量对齐变量。



```cpp
DECLARE_ASM_CONST(16, uint64_t, pw_08) = UINT64_C(0x0008000800080008);
```

参数：n：最小对齐字节

   t：变量（或数组元素）的类型

   v：变量的名称






二、功能属性

功能属性适用于内存处理功能。


这些功能属性可以帮助编译器发出更有用的警告，或者生成更好的代码。





一）、宏


|#define|[av_malloc_attrib](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__attrs.html#gad48ff724186ef4e602ae7a2547e4287c)   __attribute__((__malloc__))|
|----|----|

功能属性表示类似malloc的功能。





|#define|[av_alloc_size](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__attrs.html#gad7ff928d462d1a61b37baa70bf13e0e3)(...)   __attribute__((alloc_size(__VA_ARGS__)))|
|----|----|

在分配内存的函数中使用的函数属性，其大小由指定的参数给出。



```cpp
void *av_malloc(size_t size) av_alloc_size(1);
void *av_calloc(size_t nmemb, size_t size) av_alloc_size(1, 2);
```
参数：...：一个或两个参数索引，用逗号分隔





三、动态数组

使数组在需要时增长的实用程序。


有时，程序员希望有一个可以在需要时增长的数组。 libavutil动态数组实用程序填补了这一点。


libavutil支持将动态分配的数组附加元素的两个系统，第一个存储指向数组中值的指针，第二个直接存储该值。 在两个系统中，调用者负责维护包含数组长度的变量，以及使用后释放数组。


第一个系统存储指向动态分配内存块中的值的指针。 由于只存储指针，所以该函数不需要知道类型的大小。 av_dynarray_add（）和av_dynarray_add_nofree（）都实现了这个系统。




```cpp
type **array = NULL; //< an array of pointers to values
int    nb    = 0;    //< a variable to keep track of the length of the array
type to_be_added  = ...;
type to_be_added2 = ...;
av_dynarray_add(&array, &nb, &to_be_added);
if (nb == 0)
    return AVERROR(ENOMEM);
av_dynarray_add(&array, &nb, &to_be_added2);
if (nb == 0)
    return AVERROR(ENOMEM);
// Now:
//  nb           == 2
// &to_be_added  == array[0]
// &to_be_added2 == array[1]
av_freep(&array);
```


第二个系统将值直接存储在一个内存块中。 因此，函数必须知道类型的大小。 av_dynarray2_add（）实现这个机制。



```cpp
type *array = NULL; //< an array of values
int   nb    = 0;    //< a variable to keep track of the length of the array
type to_be_added  = ...;
type to_be_added2 = ...;
type *addr = av_dynarray2_add((void **)&array, &nb, sizeof(*array), NULL);
if (!addr)
    return AVERROR(ENOMEM);
memcpy(addr, &to_be_added, sizeof(to_be_added));
// Shortcut of the above.
type *addr = av_dynarray2_add((void **)&array, &nb, sizeof(*array),
                              (const void *)&to_be_added2);
if (!addr)
    return AVERROR(ENOMEM);
// Now:
//  nb           == 2
//  to_be_added  == array[0]
//  to_be_added2 == array[1]
av_freep(&array);
```






一）、函数


|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_dynarray_add](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__dynarray.html#ga385b582522cedf5a88ce2cf158c396c9) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *tab_ptr, [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *nb_ptr, [void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *elem)|
|----|----|

将指向元素的指针添加到动态数组。

要增长的数组应该是一个指向结构的指针数组，要添加的元素必须是指向已分配结构的指针。


当数组的大小达到2的幂时，该数组被重新分配。因此，添加元素的摊销成本是恒定的。


在成功的情况下，更新指向数组的指针，以指向新增长的数组，并且增加nb_ptr指向的数字。 如果发生故障，数组将被释放，* tab_ptr设置为NULL，并将* nb_ptr设置为0。


参数：nb_ptr：指向数组的指针增长

nb_ptr：指向数组中的元素数

   elem：要添加的元素





|[av_warn_unused_result](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a93f17e0f0b53a66e7b5eceb3dff2918f)[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_dynarray_add_nofree](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__dynarray.html#ga5e3735b4568e78fffd7c4ea2f793996e) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *tab_ptr, [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *nb_ptr, [void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *elem)|
|----|----|

将一个元素添加到动态数组。

函数与av_dynarray_add（）具有相同的功能，但它不会释放内存失败。 它返回错误代码，并保持当前缓冲区不变。


返回：> = 0成功，否则为否





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_dynarray2_add](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__dynarray.html#ga9f60ff848186092d6705276a52ea0bb9) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) **tab_ptr, [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *nb_ptr, size_t elem_size, const [uint8_t](https://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *elem_data)|
|----|----|

将大小elem_size的元素添加到动态数组。

当元素的数量达到2时，该数组被重新分配。因此，添加元素的摊销成本是不变的。


在成功的情况下，更新指向数组的指针，以指向新增长的数组，并且增加nb_ptr指向的数字。 如果发生故障，数组将被释放，* tab_ptr设置为NULL，并将* nb_ptr设置为0。


参数：tab_ptr：指向数组的指针增长

nb_ptr：指向数组中的元素数

elem_size：数组中元素的字节大小

elem_data：指向要添加的元素的数据。
 如果为NULL，则新添加的元素的空间将被分配但未初始化。

返回：指向要在新分配的空间中复制的元素的数据的指针







四、杂项功能

与内存分配有关的其他功能。





一）、函数


|static [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_size_mult](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__misc.html#ga368c719d3bdce4de169910a4158df66b) (size_t [a](https://ffmpeg.org/doxygen/trunk/h264pred__template_8c.html#a4124bc0a9335c27f086f24ba207a4912), size_t [b](https://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072), size_t *[r](https://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a33ac7b8ce37a3a60855883424fcf8b62))|
|----|----|

乘以两个size_t值来检查溢出。

参数：a，b：乘法操作数

   r：指向操作结果

返回：0成功，AVERROR（EINVAL）溢出





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_max_alloc](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__misc.html#ga2204fc859b4a3ca2d5bfd1bf4e8c4dfb) (size_t max)|
|----|----|

设置一个块中可能分配的最大大小。

使用此函数指定的值对于所有libavutil的堆管理功能都是有效的。


默认情况下，最大值定义为INT_MAX。


参数：max：要设置为新的最大大小的值

警告：使用此函数时请格外小心。 如果您不明白这样做的全部后果，请勿触碰。



