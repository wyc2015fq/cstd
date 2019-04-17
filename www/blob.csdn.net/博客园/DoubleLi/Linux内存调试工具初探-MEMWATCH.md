# Linux内存调试工具初探-MEMWATCH - DoubleLi - 博客园






C 语言作为 Linux 系统上标准的编程语言给予了我们对动态内存分配很大的控制权。这种自由可能会导致严重的内存管理问题，可能导致程序崩溃或随时间的推移导致性能降级。

内存泄漏（即 `malloc()` 内存在对应的 `free()` 调用执行后永不被释放）和缓冲区溢出（例如对以前分配到某数组的内存进行写操作）是一些常见的问题，它们可能很难检测到。这一部分将讨论几个调试工具，它们极大地简化了检测和找出内存问题的过程

**1. Memwatch简介**Memwatch是C语言的内存检测器。除了检测内存的功能外，它同样可以做其它的一些事情，而我们主要还是在于讲述它的基本功能。如果你真的想要知道所有相关的具体细节，可详细查看头文件memwatch.h以及源代码。

它使用C于处理器的功能，Memwatch使用它自己定义的功能函数取代所有在你的程序中用ANSI C定义的内存分配函数，Memwatch的内存分配函数包含了了所有的分配记录信息。

Memwatch可以在交叉编译后的目标板上运行。

**2. Memwatch功能**(1) 主要有基本的内存管理函数
      -mwMalloc()
      -mwFree()
      -mwCalloc()
      -mwRealloc()
(2) MemWatch能够检测的功能包括
      -双重释放（double-free）、
      -错误释放（erroneous free）、
     -内存泄漏（unfreed memory）、
     -溢出(Overflow)、
     -下溢(Underflow)

**3. 编译**    当编译时， 相让memwatch起作用， 编译是加上-DMEMWATCH


**4. memwatch.log内容**    在正常操作的情况下，memwatch会创建一个名叫memwatch.log的文件。但有的时候，memwatch.log文件并不能被创建。此时，memwatch会尝试创建命名类似memwatNN.log的文件，其中NN是01到99之间的数字。如果还是失败，则没有log文件。

**5. 应用实例**

**清单 1. 内存样本（test1.c）**


|#include <stdlib.h>#include <stdio.h>#include "memwatch.h"int main(void){  char *ptr1;  char *ptr2;  ptr1 = malloc(512);  ptr2 = malloc(512);  ptr2 = ptr1;  free(ptr2);  free(ptr1);}|
|----|





清单 1 中的代码将分配两个 512 字节的内存块，然后指向第一个内存块的指针被设定为指向第二个内存块。结果，第二个内存块的地址丢失，从而产生了内存泄漏。

现在我们编译清单 1 的 memwatch.c。下面是一个 makefile 示例：

**test1**


|gcc -DMEMWATCH -DMW_STDIO test1.c memwatchc -o test1|
|----|





当您运行 test1 程序后，它会生成一个关于泄漏的内存的报告。清单 2 展示了示例 memwatch.log 输出文件。

**清单 2. test1 memwatch.log 文件**


|MEMWATCH 2.67 Copyright (C) 1992-1999 Johan Lindh...double-free: <4> test1.c(15), 0x80517b4 was freed from test1.c(14)...unfreed: <2> test1.c(11), 512 bytes at 0x80519e4{FE FE FE FE FE FE FE FE FE FE FE FE ..............}Memory usage statistics (global):  N)umber of allocations made: 	2  L)argest memory usage : 	1024  T)otal of all alloc() calls: 	1024  U)nfreed bytes totals : 	512|
|----|





MEMWATCH 为您显示真正导致问题的行。如果您释放一个已经释放过的指针，它会告诉您。对于没有释放的内存也一样。日志结尾部分显示统计信息，包括泄漏了多少内存，使用了多少内存，以及总共分配了多少内存。









