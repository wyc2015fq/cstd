# __KERNEL__ macro - weixin_33985507的博客 - CSDN博客
2014年04月05日 12:52:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
转载：http://blog.csdn.net/kasalyn/article/details/17097639
The `__KERNEL__` macro is defined because there is programs (like libraries) that include kernel code and there is many things that you don't want them to include. So most modules will want the`__KERNEL__` macro to be enabled.
When you compile your kernel, `__KERNEL__` is defined on the command line. (in tree/kernel/Makefile---> KBUILD_CPPFLAGS := -D__KERNEL__)
User-space programs need access to the kernel headers, but some of 
the info in kernel headers is intended only for the kernel. Wrapping 
some statements in an`#ifdef __KERNEL__/#endif` block ensures that user-space programs don't see those statements.
 example:
in the user space ,if you want to include the header using ‘＃ifdef __KERNEL__ XXXX', you should define the __KERNEL__.
/*为了引用kernel中的数据结构,我常用如下样式include那些服务于内核的头文件*/
#ifdef __KERNEL__
#include <linux/list.h>
#include <linux/mm.h>
#undef __KERNEL__
/*将两种类型的头文件隔离开来*/
#include <stdio.h>
#include <errno.h>
对于__KERNEL__举例：
#ifdef __KERNEL__
# define HZ        CONFIG_HZ    /* Internal kernel timer frequency */
# define USER_HZ    100        /* User interfaces are in "ticks" */
# define CLOCKS_PER_SEC    (USER_HZ)    /* like times() */
#else
# define HZ        100
#endif
在.config中：
CONFIG_HZ=100
故ISA-32中,HZ=100,而jiffies =10ms
