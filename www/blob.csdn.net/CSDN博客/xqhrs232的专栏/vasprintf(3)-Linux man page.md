# vasprintf(3) - Linux man page - xqhrs232的专栏 - CSDN博客
2016年10月20日 18:53:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：657
原文地址::[https://linux.die.net/man/3/vasprintf](https://linux.die.net/man/3/vasprintf)
相关文章
1、**vasprintf的实现----[http://leander.blog.51cto.com/2911819/1151729](http://leander.blog.51cto.com/2911819/1151729)**
## Name
asprintf, vasprintf - print to allocated string
## Synopsis
#define _GNU_SOURCE /*
 See **[feature_test_macros](https://linux.die.net/man/7/feature_test_macros)**(7) */
#include <[stdio.h](https://linux.die.net/include/stdio.h)>
**int asprintf(char *****strp***, const char ****fmt***, ...);**
**int vasprintf(char *****strp***, const char ****fmt***, va_list***ap***);**
## Description
The functions asprintf()
 and vasprintf()
 are analogs of **[sprintf](https://linux.die.net/man/3/sprintf)**(3) and **[vsprintf](https://linux.die.net/man/3/vsprintf)**(3),
 except that they allocate a string large enough to hold the output including the terminating null byte, and return a pointer to it via the first argument. This pointer should be passed to **[free](https://linux.die.net/man/3/free)**(3) to
 release the allocated storage when it is no longer needed.
## Return Value
When successful, these functions return the number of bytes printed, just like **[sprintf](https://linux.die.net/man/3/sprintf)**(3).
 If memory allocation wasn't possible, or some other error occurs, these functions will return -1, and the contents of strp is
 undefined.
## Conforming to
These functions are GNU extensions, not in C or POSIX. They are also available under *BSD. The FreeBSD implementation sets strp to
 NULL on error.
## See Also
**[free](https://linux.die.net/man/3/free)**(3), **[malloc](https://linux.die.net/man/3/malloc)**(3), **[printf](https://linux.die.net/man/3/printf)**(3)
## Referenced By
[talloc](https://linux.die.net/man/3/talloc)(3)
