# __attribute__ ((__noreturn__)) - Koma Hub - CSDN博客
2019年01月24日 21:41:41[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：57
```cpp
#ifndef _SYS_CDEFS_H
#include <misc/sys/cdefs.h>
#ifndef _ISOMAC
/* The compiler will optimize based on the knowledge the parameter is
   not NULL.  This will omit tests.  A robust implementation cannot allow
   this so when compiling glibc itself we ignore this attribute.  */
# undef __nonnull
# define __nonnull(params)
extern void __chk_fail (void) __attribute__ ((__noreturn__));
libc_hidden_proto (__chk_fail)
rtld_hidden_proto (__chk_fail)
#endif
#endif
```
摘自：glic-2.28   cdefs.h
