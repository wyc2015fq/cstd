# size_t 与 ssize_t 类型 - xqhrs232的专栏 - CSDN博客
2016年05月04日 17:30:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：189
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/gowyz/article/details/6606313](http://blog.csdn.net/gowyz/article/details/6606313)
ssize_t是什么类型的？
解释一：为了增强程序的可移植性，便有了size_t，它是为了方便系统之间的移植而定义的，不同的系统上，定义size_t可能不一样。
     在32位系统上 定义为 unsigned int 也就是说在32位系统上是32位无符号整形。在64位系统上定义为 unsigned long 也就是说在64位系统上是64位无符号整形。size_t一般用来表示一种计数，比如有多少东西被拷贝等。例如：sizeof操作符的结果类型是size_t，该类型保证能容纳实现所建立的最大对象的字节大小。 它的意义大致是“适于计量内存中可容纳的数据项目个数的无符号整数类型”。所以，它在数组下标和内存管理函数之类的地方广泛使用。而ssize_t这个数据类型用来表示可以被执行读写操作的数据块的大小.它和size_t类似,但必需是signed.意即：它表示的是signed
 size_t类型的。
typedef unsigned long size_t
解释二：ssize_t是signed size_t，
size_t是标准C库中定义的，应为unsigned int。定义为typedef int ssize_t。
而ssize_t:这个数据类型用来表示可以被执行读写操作的数据块的大小.它和size_t类似,但必需是signed.意即：它表示的是sign size_t类型的。
