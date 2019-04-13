
# ioctl幻数 - 嵌入式Linux - CSDN博客

2015年11月16日 15:07:52[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：537


To decode a hex IOCTL code:
Most architectures use this generic format, but check
include/ARCH/ioctl.h for specifics, e.g. powerpc
uses 3 bits to encode read/write and 13 bits for size.
bits    meaning
31-30  00 - no parameters: uses _IO macro
10 - read: _IOR
01 - write: _IOW
11 - read/write: _IOWR
29-16  size of arguments
15-8   ascii character supposedly
unique to each driver
7-0    function \#
**下面这个解释要看清楚**
So for example**0x82187201**is a read with arg length of　0x218,
character ‘r’ function 1. Grepping the source reveals this is:
# ＃define VFAT_IOCTL_READDIR_BOTH         _IOR(‘r’, 1, struct dirent [2])

