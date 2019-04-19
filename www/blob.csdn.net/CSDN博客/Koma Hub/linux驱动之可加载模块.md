# linux驱动之可加载模块 - Koma Hub - CSDN博客
2019年01月28日 22:16:25[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：31
Linux 的众多优良特性之一就是可以在运行时扩展由内核提供的特性的能力. 这意味着你可以在系统正在运行着的时候增加内核的功能( 也可以去除 ).
每块可以在运行时添加到内核的代码, 被称为一个模块. Linux 内核提供了对许多模块类型的支持, 包括但不限于, 设备驱动. 每个模块由目标代码组成( 没有连接成一个完整可执行文件 ), 可以动态连接到运行中的内核中, 通过 insmod 程序, 以及通过 rmmod 程序去连接.
图 内核的划分 表示了负责特定任务的不同类别的模块, 一个模块是根据它提供的功能来说它属于一个特别类别的. 图 内核的划分 中模块的安排涵盖了最重要的类别, 但是远未完整, 因为在 Linux 中越来越多的功能被模块化了.
![](https://img-blog.csdnimg.cn/2019012822160451.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
