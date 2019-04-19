# 汇编中bss,data,text,rodata,heap,stack段的作用 - Koma Hub - CSDN博客
2019年03月19日 22:19:50[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：12
个人分类：[Assembly																[汇编](https://blog.csdn.net/Rong_Toa/article/category/8750815)](https://blog.csdn.net/Rong_Toa/article/category/8750816)
原文：[https://blog.csdn.net/bxd1314/article/details/38433837](https://blog.csdn.net/bxd1314/article/details/38433837)
**Table of Contents**
[bss段：](#bss%E6%AE%B5%EF%BC%9A)
[data段：](#data%E6%AE%B5%EF%BC%9A)
[text段：](#text%E6%AE%B5%EF%BC%9A)
[rodata段：](#rodata%E6%AE%B5%EF%BC%9A)
[heap堆：](#heap%E5%A0%86%EF%BC%9A)
[stack栈：](#stack%E6%A0%88%EF%BC%9A)
# bss段：
BSS段（bsssegment）通常是指用来存放程序中未初始化的全局变量的一块内存区域。BSS是英文BlockStartedby Symbol的简称。BSS段属于静态内存分配。
# data段：
数据段（datasegment）通常是指用来存放程序中已初始化的全局变量的一块内存区域。数据段属于静态内存分配。
# text段：
代码段（codesegment/textsegment）通常是指用来存放程序执行代码的一块内存区域。这部分区域的大小在程序运行前就已经确定，并且内存区域通常属于只读,某些架构也允许代码段为可写，即允许修改程序。在代码段中，也有可能包含一些只读的常数变量，例如字符串常量等。
# rodata段：
存放C中的字符串和#define定义的常量
# heap堆：
堆是用于存放进程运行中被动态分配的内存段，它的大小并不固定，可动态扩张或缩减。当进程调用malloc等函数分配内存时，新分配的内存就被动态添加到堆上（堆被扩张）；当利用free等函数释放内存时，被释放的内存从堆中被剔除（堆被缩减）
# stack栈：
是用户存放程序临时创建的局部变量，也就是说我们函数括弧“{}”中定义的变量（但不包括static声明的变量，static意味着在数据段中存放变量）。除此以外，在函数被调用时，其参数也会被压入发起调用的进程栈中，并且待到调用结束后，函数的返回值也会被存放回栈中。由于栈的先进先出特点，所以栈特别方便用来保存/恢复调用现场。从这个意义上讲，我们可以把堆栈看成一个寄存、交换临时数据的内存区
