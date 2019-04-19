# 关于__GNU_SOURCE 这个宏 - xqhrs232的专栏 - CSDN博客
2016年10月19日 23:52:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2197
原文地址::[http://blog.csdn.net/stevenliyong/article/details/4160181](http://blog.csdn.net/stevenliyong/article/details/4160181)
相关文章
1、[如何开启_GNU_SOURCE宏](http://blog.csdn.net/satanwxd/article/details/6234577)----[http://blog.csdn.net/satanwxd/article/details/6234577](http://blog.csdn.net/satanwxd/article/details/6234577)
今天发现一个奇怪的问题，即使
#include sys/stat.h
在使用  lstat 函数的时候还是会报
warning: implicit declaration of function 'lstat'
另外同样的问题，即使include 了 sys/time.h
还是会报
warning: implicit declaration of function 'nanosleep'
同样，即使include 了 sys/time.h  time.h
warning: implicit declaration of function 'clock_gettime'
error: 'CLOCK_MONOTONIC' undeclared (first use in this function)
最后发现 sys/time.h sys/stat.h time.h 都include 一个共同的头文件 features.h
原来这是用来让用户配置编译环境的头文件。再看一下_GUN_SOURCE这个宏，这个宏可以让用户打开所有feature.
/* If _GNU_SOURCE was defined by the user, turn on all the other features.  */
#ifdef _GNU_SOURCE
# undef  _ISOC99_SOURCE
# define _ISOC99_SOURCE 1
# undef  _POSIX_SOURCE
# define _POSIX_SOURCE 1
# undef  _POSIX_C_SOURCE
# define _POSIX_C_SOURCE 200112L
# undef  _XOPEN_SOURCE
# define _XOPEN_SOURCE 600
# undef  _XOPEN_SOURCE_EXTENDED
# define _XOPEN_SOURCE_EXTENDED 1
# undef  _LARGEFILE64_SOURCE
# define _LARGEFILE64_SOURCE 1
# undef  _BSD_SOURCE
# define _BSD_SOURCE 1
# undef  _SVID_SOURCE
# define _SVID_SOURCE 1
# undef  _ATFILE_SOURCE
# define _ATFILE_SOURCE 1
#endif
因此解决问题的办法很简单
只要在source file的开头 加上
#define _GNU_SOURCE
就可以了。
另外必须注意include 的先后次序
#include time.h
#include sys/time.h
顺序搞反了的话 clock_gettime CLOCK_MONOTONIC 是编译无法通过的。这是因为time.h是标准头文件
，而sys/time.h 是你当前系统的头文件。
