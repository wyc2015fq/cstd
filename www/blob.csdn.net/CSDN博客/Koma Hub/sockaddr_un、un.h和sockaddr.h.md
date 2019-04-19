# sockaddr_un、un.h和sockaddr.h - Koma Hub - CSDN博客
2019年01月22日 21:17:36[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：69
```cpp
struct sockaddr_un
{
    unsigned char sun_len;
    sa_family_t sun_family;
    //typedef unsigned short int sa_family_t;
    char sun_path[108];		// Path name.  
};
```
un.h
```cpp
#ifndef	_SYS_UN_H
#define	_SYS_UN_H	1
#include <sys/cdefs.h>
/* Get the definition of the macro to define the common sockaddr members.  */
#include <bits/sockaddr.h>
__BEGIN_DECLS
/* Structure describing the address of an AF_LOCAL (aka AF_UNIX) socket.  */
struct sockaddr_un
  {
    __SOCKADDR_COMMON (sun_);
    char sun_path[108];		/* Path name.  */
  };
#ifdef __USE_MISC
# include <string.h>		/* For prototype of `strlen'.  */
/* Evaluate to actual length of the `sockaddr_un' structure.  */
# define SUN_LEN(ptr) ((size_t) (((struct sockaddr_un *) 0)->sun_path)	      \
		      + strlen ((ptr)->sun_path))
#endif
__END_DECLS
#endif	/* sys/un.h  */
```
sockaddr.h
```cpp
#ifndef _BITS_SOCKADDR_H
#define _BITS_SOCKADDR_H	1
/* POSIX.1g specifies this type name for the `sa_family' member.  */
typedef unsigned char sa_family_t;
/* This macro is used to declare the initial common members
   of the data types used for socket addresses, `struct sockaddr',
   `struct sockaddr_in', `struct sockaddr_un', etc.  */
#define	__SOCKADDR_COMMON(sa_prefix)	\
  unsigned char sa_prefix##len;		\
  sa_family_t sa_prefix##family
#define __SOCKADDR_COMMON_SIZE	(2 * sizeof (unsigned char))
#define _HAVE_SA_LEN	1	/* We have the sa_len field.  */
/* Size of struct sockaddr_storage.  */
#define _SS_SIZE 128
#endif	/* bits/sockaddr.h */
```
