# linux内核模块的编译文件 - Koma Hub - CSDN博客
2019年02月02日 08:43:10[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：47
个人分类：[Linux driver																[Linux kernel																[驱动](https://blog.csdn.net/Rong_Toa/article/category/8647778)](https://blog.csdn.net/Rong_Toa/article/category/7348890)](https://blog.csdn.net/Rong_Toa/article/category/8645170)
一个内核模块不是一个可以独立执行的文件，而是需要在运行时刻连接入内核的目标文件。所以，它们需要用-c 选项进行编译。而且，所有的内核模块都必须包含特定的标志：
- __KERNEL__——这个标志告诉头文件此代码将在内核模块中运行，而不是作为用户进程。
- MODULE——这个标志告诉头文件要给出适当的内核模块的定义。
- LINUX——从技术上讲，这个标志不是必要的。但是，如果你希望写一个比较正规的内核模块，在多个操作系统上编译，这个标志将会使你感到方便。它可以允许你在独立于操作系统的部分进行常规的编译。
- 还有其它的一些可被选择包含标志，取决于编译模块是的选项。如果你不能明确内核怎样被编译，可以在in/usr/include/linux/config.h 中查到。
- __SMP__——对称多线程。在内核被编译成支持对称多线程（尽管在一台处理机上运行）是必须定义。如果是这样，还需要做一些别的事情（参见第12 章）。
- CONFIG_MODVERSIONS——如果CONFIG_MODVERSIONS 被激活，你需要在编译是定义它并且包含文件/usr/include/linux/modversions.h。这可以有代码自动完成。
```
# Makefile for a basic kernel module
CC=gcc
MODCFLAGS := -Wall -DMODULE -D__KERNEL__ -DLINUX
hello.o: hello.c /usr/include/linux/version.h
    $(CC) $(MODCFLAGS) -c hello.c
    echo insmod hello.o to turn it on
    echo rmmod hello to turn if off
    echo
    echo X and kernel programming do not mix.
    echo Do the insmod and rmmod from outside
```
所以，并不是剩下的事情就是root（你没有把它编译成root，而是在边缘（注1.1）。对吗？），然后就在你的核心内容里插入或移出hello。当你这样做的时候，要注意到你的新模块在/proc/modules 里。
而且，编译文件不推荐从X 下插入的原因是内核有一条需要用printk 打印的消息，它把它送给了控制台。如果你不使用X，它就送到了你使用的虚拟终端（你用Alt-F<n>选择的哪个）并且你可以看到。相反的，如果你使用了X，就有两种可能性。如果用xterm –C 打开了一个控制台，输出将被送到哪里。如果没有，输出将被送到虚拟终端7——被X“覆盖”的那个。
如果你的内核变得不稳定，你可以在没有X 的情况下得到调试消息。在X 外，printk可以直接从内核中输出到控制台。而如果在X 里，printk 输出到一个用户态的进程（xterm–C）。当进程接收到CPU 时间，它会将其送到X 服务器进程。然后，当X 服务器进程接收到CPU 时间，它将会显示，但是一个不稳定的内核意味着系统将会崩溃或重起，所以你不希望显示错误的消息，然后可能被解释给你什么发生了错误，但是超出了正确的时间。
## 多文件内核模块
有些时候在几个源文件之间分出一个内核模块是很有意义的。在这种情况下，你需要做下面的事情：
- 1. 在除了一个以外的所有源文件中，增加一行#define __NO_VERSION__。这是很重要的，因为module.h 一般包括kernel_version 的定义，这是一个全局变量，包含模块编译的内核版本。如果你需要version.h，你需要把自己把它包含进去，因为如果有__NO_VERSION__的话module.h 不会自动包含。
- 2. 象通常一样编译源文件。
- 3. 把所有目标文件联编成一个。在X86 下，用ld –m elf_i386 –r –o <name of module>.o<1st source file>
这里给出一个这样的内核模块的例子。
### ex start.c
```cpp
/* start.c
* Copyright (C) 1999 by Ori Pomerantz
*
* "Hello, world" - the kernel module version.
* This file includes just the start routine
*/
/* The necessary header files */
/* Standard in kernel modules */
#include <linux/kernel.h> /* We're doing kernel work */
#include <linux/module.h> /* Specifically, a module */
/* Deal with CONFIG_MODVERSIONS */
#if CONFIG_MODVERSIONS==1
#define MODVERSIONS
#include <linux/modversions.h>
#endif
/* Initialize the module */
int init_module()
{
    printk("Hello, world - this is the kernel speaking\n");
    /* If we return a non zero value, it means that
    * init_module failed and the kernel module
    * can't be loaded */
    return 0;
}
```
### ex stop.c
```cpp
/* stop.c
* Copyright (C) 1999 by Ori Pomerantz
*
* "Hello, world" - the kernel module version. This
* file includes just the stop routine.
*/
/* The necessary header files */
/* Standard in kernel modules */
#include <linux/kernel.h> /* We're doing kernel work */
#define __NO_VERSION__ /* This isn't "the" file of the kernel module */
/* Specifically, a module */
#include <linux/module.h> 
/* Not included by module.h because of the __NO_VERSION__ */
#include <linux/version.h> 
/* Deal with CONFIG_MODVERSIONS */
#if CONFIG_MODVERSIONS==1
#define MODVERSIONS
#include <linux/modversions.h>
#endif
/* Cleanup - undid whatever init_module did */
void cleanup_module()
{
    printk("Short is the life of a kernel module\n");
}
```
### ex Makefile
```bash
# Makefile for a multifile kernel module
CC=gcc
MODCFLAGS := -Wall -DMODULE -D__KERNEL__ -DLINUX
hello.o: start.o stop.o
    ld -m elf_i386 -r -o hello.o start.o stop.o
start.o: start.c /usr/include/linux/version.h
    $(CC) $(MODCFLAGS) -c start.c
stop.o: stop.c /usr/include/linux/version.h
    $(CC) $(MODCFLAGS) -c stop.c
```
