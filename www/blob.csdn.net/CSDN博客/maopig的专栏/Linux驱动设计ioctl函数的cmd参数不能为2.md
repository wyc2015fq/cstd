# Linux驱动设计ioctl函数的cmd参数不能为2 - maopig的专栏 - CSDN博客
2017年08月04日 10:58:38[maopig](https://me.csdn.net/maopig)阅读数：1029
个人分类：[Linux](https://blog.csdn.net/maopig/article/category/850267)
[Linux](http://lib.csdn.net/base/linux)驱动程序设计的时候偶然发现的i**octl()函数的cmd参数不能为2**，如果为2,ioctl()函数返回-1，网上说就是这样的，正常，不知道为什么，stack
 overflow上有一个外国学友的建议：
“In general, you want to compose your ioctl commands
 using the [`_IO` family
 of macros](http://lxr.linux.no/linux+v3.3.1/include/asm-generic/ioctl.h#L83), with a unique type, to avoid collisions.
I suggest reading [ioctl-number.txt](http://www.mjmwired.net/kernel/Documentation/ioctl-number.txt) from the kernel documentation for
 more information, including a list of most used types”
In general, you want to compose your ioctl commands using the [`_IO` family
 of macros](http://lxr.linux.no/linux+v3.3.1/include/asm-generic/ioctl.h#L83), with a unique type, to avoid collisions.
I suggest reading [ioctl-number.txt](http://www.mjmwired.net/kernel/Documentation/ioctl-number.txt) from the kernel documentation for
 more information, including a list of most used types”
