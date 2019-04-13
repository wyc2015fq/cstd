
# 关于uint32_t uint8_t  uint64_t 的问题 - 高科的专栏 - CSDN博客

2012年08月27日 20:22:18[高科](https://me.csdn.net/pbymw8iwm)阅读数：3025


怎么又是u又是_t的？u代表的是unsigned相信大家都知道，那么_t又是什么呢？我认为它就是一个结构的标注，可以理解为type/typedef的缩写，表示它是通过typedef定义的，而不是其它数据类型。
uint8_t，uint16_t，uint32_t等都不是什么新的数据类型，它们只是使用typedef给类型起的别名。不过，不要小看了typedef，它对于你代码的维护会有很好的作用。比如C中没有bool，
一般来说，一个C的工程中一定要做一些这方面的工作，因为你会涉及到跨平台，不同的平台会有不同的字长，所以利用预编译和typedef可以让你最有效的维护你的代码。为了用户的方便，C99标准的C语言硬件为我们定义了这些类型，我们放心使用就可以了。
按照posix标准，一般整形对应的*_t类型为：
1字节uint8_t
2字节uint16_t
4字节uint32_t
8字节uint64_t
附：C99标准中inttypes.h的内容
/* Use [u]intN_t if you need exactly N bits.
XXX - doesn't handle the -mint8 option.*/
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef int int16_t;
typedef unsigned int uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
typedef int16_t intptr_t;
typedef uint16_t uintptr_t;
\#endif

