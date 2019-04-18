# FFMPEG学习【libavutil】：Memory Management（二） - 一世豁然的专栏 - CSDN博客





2017年07月18日 14:10:56[一世豁然](https://me.csdn.net/Explorer_day)阅读数：292








负责分配，释放和复制内存的功能。

所有内存分配功能都具有INT_MAX字节的内置上限。 这可以使用av_max_alloc（）进行更改，但是在执行此操作时要格外小心。








一、函数


|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_malloc](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#ga9722446c5e310ffedfaac9489864796d) (size_t [size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e)) [av_malloc_attrib](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__attrs.html#gad48ff724186ef4e602ae7a2547e4287c) 1(1)|
|----|----|

分配一个适合所有存储器访问的定向存储块（包括CPU上可用的向量）。

参数：size：要分配的内存块的大小（以字节为单位）

返回：指向分配的块，如果块不能被分配，则为NULL





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_mallocz](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#ga0a8cc057ae9723ce3b9203cb5365971a) (size_t [size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e)) [av_malloc_attrib](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__attrs.html#gad48ff724186ef4e602ae7a2547e4287c) 1(1)|
|----|----|

分配一个适用于所有存储器访问的对齐的存储块（包括CPU上可用的向量），并将块的所有字节置零。

参数：size：要分配的内存块的大小（以字节为单位）


返回：指向分配的块，如果块不能被分配，则为NULL






|static [void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_malloc_array](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#gab62b093b46dce5fe4e3e1d58bdb255da) (size_t nmemb, size_t [size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|



|static [void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_mallocz_array](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#gae0443f550815c80af42c5ae967351433) (size_t nmemb, size_t [size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|



|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_calloc](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#gaaf38f6d994f5a85854ef47fd98fa6e9a) (size_t nmemb, size_t [size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e)) [av_malloc_attrib](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__attrs.html#gad48ff724186ef4e602ae7a2547e4287c)|
|----|----|

非内联的等价物av_mallocz_array（）。

创建与calloc（）C函数对称。






|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_realloc](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#ga675a8e3b2f7353329b79a3c55010d612) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *ptr, size_t [size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e)) 1(2)|
|----|----|

分配，重新分配或释放一块内存。

如果ptr为NULL并且size> 0，则分配一个新的块。 如果大小为零，则释放ptr指向的内存块。 否则，根据大小扩展或缩小内存块。


参数：ptr：指向已经分配了av_realloc（）或NULL的内存块的指针

   size：要分配或重新分配的内存块的大小（以字节为单位）

返回：指向新重新分配的块的指针，如果块无法重新分配或该函数用于释放内存块，则为NULL

警告：与av_malloc（）不同，返回的指针不能保证正确对齐。





|[av_warn_unused_result](https://ffmpeg.org/doxygen/trunk/attributes_8h.html#a93f17e0f0b53a66e7b5eceb3dff2918f)[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_reallocp](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#ga9665abf3720f9f460790c2c3b224c052) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *ptr, size_t [size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

通过指针指针分配，重新分配或释放一块内存。

如果* ptr为NULL并且size> 0，则分配一个新的块。 如果大小为零，则释放* ptr指向的内存块。 否则，根据大小扩展或缩小内存块。


参数：ptr：指向已分配有av_realloc（）的内存块的指针，或指向NULL的指针。 指针在成功时更新，或者在失败时释放。

   size：要分配或重新分配的内存块的大小（以字节为单位）

返回：零成功，失败时出现AVERROR错误代码

警告：与av_malloc（）不同，分配的内存不能保证正确对齐。





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_realloc_f](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#gae5ca394e7536fcd2689008b011a211ad) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *ptr, size_t nelem, size_t elsize)|
|----|----|

分配，重新分配或释放一块内存。

此功能与av_realloc（）执行相同的操作，除了：


1、在检查整数溢出的乘法结果之后，需要两个大小的参数并分配nelem * elsize字节。

2、它在出现故障时释放输入块，从而避免了经典的内存泄漏



```cpp
buf = realloc(buf);
if (!buf)
    return -1;
```
模式。





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_realloc_array](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#gaadc230ece36ef112710b262a6601a16b) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *ptr, size_t nmemb, size_t [size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|



|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_reallocp_array](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#gad8fde0c159ac905909339e082a049cde) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *ptr, size_t nmemb, size_t [size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|



|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_fast_realloc](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#ga869fd8884d8d29a7a4910d879926d94a) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *ptr, unsigned [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *[size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e), size_t min_size)|
|----|----|

如果它不够大，重新分配给定的缓冲区，否则什么也不做。

如果给定的缓冲区为NULL，则会分配一个新的未初始化缓冲区。


如果给定的缓冲区不够大，重新分配失败，则返回NULL，并将* size设置为0，但是原始缓冲区不会被更改或释放。


一个典型的使用模式如下：




```cpp
uint8_t *buf = ...;
uint8_t *new_buf = av_fast_realloc(buf, ¤t_size, size_needed);
if (!new_buf) {
    // Allocation failed; clean up original buffer
    av_freep(&buf);
    return AVERROR(ENOMEM);
}
```
参数：ptr：已经分配缓冲区，或NULL

   size：指向当前大小的缓冲区ptr。 成功时，*尺寸会更改为min_size，如果出现故障则更改为0

   min_size：新大小的缓冲区ptr

返回：ptr如果缓冲区足够大，如果缓冲区不够大，则指向新重新分配缓冲区的指针，否则为NULL







|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_fast_malloc](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#ga29d0252b981f157e713a68ffca52f003) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *ptr, unsigned [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *[size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e), size_t min_size)|
|----|----|

分配缓冲区，如果足够大，重用给定的缓冲区。

与av_fast_realloc（）相反，当前的缓冲区内容可能不会被保留，并且出现错误时，旧缓冲区被释放，因此不需要特殊处理来避免memleaks。


* ptr被允许为NULL，在这种情况下，如果size_needed大于0，则始终会发生分配。




```cpp
uint8_t *buf = ...;
av_fast_malloc(&buf, ¤t_size, size_needed);
if (!buf) {
    // Allocation failed; buf already freed
    return AVERROR(ENOMEM);
}
```
参数：ptr：指向指向已分配缓冲区的指针。 * ptr将成功覆盖指向新缓冲区的成功或失败时为NULL

   size:指向缓冲区的当前大小* ptr。 *成功时，尺寸会更改为min_size，如果出现故障则更改为0

   min_size:新的缓冲区大小* ptr






|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_fast_mallocz](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#ga0bfda4adf7f9e943f9eb1a08eeda3791) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *ptr, unsigned [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *[size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e), size_t min_size)|
|----|----|

分配和清除缓冲区，如果足够大，重用给定的缓冲区。

像av_fast_malloc（）一样，但是所有新分配的空间都被初始化。 重新使用的缓冲区不被清除。


* ptr被允许为NULL，在这种情况下，如果size_needed大于0，则始终会发生分配。


参数：ptr：指向指向已分配缓冲区的指针。 * ptr将成功覆盖指向新缓冲区的成功或失败时为NULL

   size：指向缓冲区的当前大小* ptr。 *成功时，尺寸会更改为min_size，如果出现故障则更改为0

   min_size:新的缓冲区大小* ptr





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_free](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#ga0c9096f498624c525aa2315b8a20c411) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *ptr)|
|----|----|

释放已经分配了av_malloc（）或av_realloc（）函数的内存块。

参数：ptr：指向应释放内存块的指针。

注意：显式允许ptr = NULL。

   建议您使用av_freep（），以防止留下悬挂指针。





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_freep](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#ga0cc84043ea2167ad005c86e11d0bcdba) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *ptr)|
|----|----|

释放已经分配了av_malloc（）或av_realloc（）系列的函数的内存块，并将指向它的指针设置为NULL。



```cpp
uint8_t *buf = av_malloc(16);
av_free(buf);
// buf now contains a dangling pointer to freed memory, and accidental
// dereference of buf will result in a use-after-free, which may be a
// security risk.
uint8_t *buf = av_malloc(16);
av_freep(&buf);
// buf is now NULL, and accidental dereference will only result in a
// NULL-pointer dereference.
```
参数：ptr：指向应该释放的内存块的指针

注意：* ptr = NULL是安全的，不会导致任何操作。






|char *|[av_strdup](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#ga7c352f4cff02184f005323691375fea9) (const char *[s](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5)) [av_malloc_attrib](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__attrs.html#gad48ff724186ef4e602ae7a2547e4287c)|
|----|----|

复制一个字符串。

参数：s：要复制的字符

返回：指向一个新分配的字符串，包含一个s的副本，如果该字符串不能被分配，则返回NULL





|char *|[av_strndup](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#gaffde7a717e3a603ed11a998c18d68cad) (const char *[s](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), size_t [len](https://ffmpeg.org/doxygen/trunk/vorbis__enc__data_8h.html#afed088663f8704004425cdae2120b9b3)) [av_malloc_attrib](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__attrs.html#gad48ff724186ef4e602ae7a2547e4287c)|
|----|----|

复制字符串的子字符串。

参数：s：要复制的字符

   len：生成的字符串的最大长度（不计算终止字节）

返回：指向一个新分配的字符串，包含一个子字符串s或NULL（如果该字符串不能被分配）





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_memdup](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#ga4ad5e6363f4a6b6eddfdb427e21166f2) (const [void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *p, size_t [size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

用av_malloc（）复制一个缓冲区。

参数：p：缓冲区要复制

   size：复制缓冲区大小（以字节为单位）

返回：指向不能分配缓冲区的新分配缓冲区的指针，该缓冲区包含p的副本或NULL





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_memcpy_backptr](https://ffmpeg.org/doxygen/trunk/group__lavu__mem__funcs.html#ga3ea4736dcceab4ba033b8abd2c2a180e) ([uint8_t](https://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *dst, [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) back, [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) cnt)|
|----|----|

重写memcpy（）实现。

参数：dst：目的缓冲区

   back：开始复制的字节数（即重叠窗口的初始大小）; 必须> 0

   cnt：要复制的字节数 必须> = 0

注意：cnt> back是有效的，这将复制我们刚刚复制的字节，从而创建一个周期长度为背的重复模式。



