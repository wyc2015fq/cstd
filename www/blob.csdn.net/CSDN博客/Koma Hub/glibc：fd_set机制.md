# glibc：fd_set机制 - Koma Hub - CSDN博客
2019年01月12日 14:11:34[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：35
个人分类：[Glibc](https://blog.csdn.net/Rong_Toa/article/category/8605571)
**目录**
[msg_set.h](#msg_set.h)
[使用](#%E4%BD%BF%E7%94%A8)
# msg_set.h
```cpp
#ifndef _MSG_SET_H
#define _MSG_SET_H
/* The msg_set member is required to be an array of longs.  */
typedef long int __msg_mask;
/* Number of descriptors that can fit in an `msg_set'.  */
#define	__MSG_SETSIZE		256
/* It's easier to assume 8-bit bytes than to get CHAR_BIT.  */
#define __NMSGBITS	(8 * (int) sizeof (__msg_mask))
#define	__MSG_ELT(d)	((d) / __NMSGBITS)
#define	__MSG_MASK(d)	((__msg_mask) (1UL << ((d) % __NMSGBITS)))
/* msg_set for select and pselect.  */
typedef struct
{
    /* XPG4.2 requires this member name.  Otherwise avoid the name
       from the global namespace.  */
#ifdef __USE_XOPEN
    __msg_mask msgs_bits[__MSG_SETSIZE / __NMSGBITS];
# define __MSGS_BITS(set) ((set)->msgs_bits)
#else
    __msg_mask __msgs_bits[__MSG_SETSIZE / __NMSGBITS];
# define __MSGS_BITS(set) ((set)->__msgs_bits)
#endif
} msg_set;
/* We don't use `memset' because this would require a prototype and
   the array isn't too big.  */
#define __MSG_ZERO(s) \
  do {									      \
    unsigned int __i;							      \
    msg_set *__arr = (s);						      \
    for (__i = 0; __i < sizeof (msg_set) / sizeof (__msg_mask); ++__i)	      \
      __MSGS_BITS (__arr)[__i] = 0;					      \
  } while (0)
#define __MSG_SET(d, s) \
  ((void) (__MSGS_BITS (s)[__MSG_ELT(d)] |= __MSG_MASK(d)))
#define __MSG_CLR(d, s) \
  ((void) (__MSGS_BITS (s)[__MSG_ELT(d)] &= ~__MSG_MASK(d)))
#define __MSG_ISSET(d, s) \
  ((__MSGS_BITS (s)[__MSG_ELT (d)] & __MSG_MASK (d)) != 0)
/* Access macros for `msg_set'.  */
#define	MSG_SET(fd, fdsetp)	    __MSG_SET (fd, fdsetp)
#define	MSG_CLR(fd, fdsetp)	    __MSG_CLR (fd, fdsetp)
#define	MSG_ISSET(fd, fdsetp)	__MSG_ISSET (fd, fdsetp)
#define	MSG_ZERO(fdsetp)		__MSG_ZERO (fdsetp)
#endif /* _MSG_SET_H */
```
# 使用
```cpp
#include <stdio.h>
#include "msg_set.h"
int main()
{
    msg_set set;
    
    printf("%d\n", sizeof(long int));
    
    MSG_ZERO(&set);
    
    MSG_SET(0, &set);
    
    printf("%d\n", set.__msgs_bits[0]);
    
    MSG_CLR(0, &set);
    
    printf("%d\n", set.__msgs_bits[0]);
    
    return 0;
}
```
