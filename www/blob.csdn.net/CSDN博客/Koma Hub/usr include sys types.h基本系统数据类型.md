# /usr/include/sys/types.h基本系统数据类型 - Koma Hub - CSDN博客
2018年02月04日 20:29:36[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：564
sys/types.h中文名称为基本系统数据类型。在应用程序源文件中包含 <sys/types.h> 以访问 _LP64 和 _ILP32 的定义。此头文件还包含适当时应使用的多个基本派生类型。所有这些类型在 ILP32 编译环境中保持为 32 位值，并会在 LP64 编译环境中增长为 64 位值。
```
caddr_t 核心地址。
clock_t 表示系统时间(以时钟周期为单位)。
comp_t 压缩的时钟滴答。
dev_t 用于设备号。
fd_set 文件描述集。
fpos_t 文件位置。
gid_t 数组值ID。
ino_t i节点编号。
off_t 用于文件大小和偏移量。
mode_t 文件类型，文件创建模式。
pid_t 进程ID和进程组ID
ptrdiff_t 是一种带符号整型，用于对两个指针执行减法运算后所得的结果。
rlim_t 资源限制;
size_t 反映内存中对象的大小(以字节为单位)。
ssize_t 供返回字节计数或错误提示的函数使用。
time_t 以秒为单位计时。
uid_t 数值用户ID。
wchar_t 能表示所有不同的字符码。
```
```
DESCRIPTION
       The <sys/types.h> header shall include definitions for at least the following types:
       blkcnt_t: Used for file block counts.
       blksize_t: Used for block sizes.
       clock_t: Used for system times in clock ticks or CLOCKS_PER_SEC; see <time.h> .
       clockid_t: Used for clock ID type in the clock and timer functions.
       dev_t  Used for device IDs.
       fsblkcnt_t: Used for file system block counts.
       fsfilcnt_t: Used for file system file counts.
       gid_t  Used for group IDs.
       id_t   Used as a general identifier; can be used to contain at least a pid_t, uid_t, or gid_t.
       ino_t  Used for file serial numbers.
       key_t  Used for XSI interprocess communication.
       mode_t Used for some file attributes.
       nlink_t: Used for link counts.
       off_t  Used for file sizes.
       pid_t  Used for process IDs and process group IDs.
       size_t Used for sizes of objects.
       ssize_t: Used for a count of bytes or an error indication.
       suseconds_t: Used for time in microseconds.
```
尤其是以下类型更为重要：
```cpp
clock_t 表示系统时间（以时钟周期为单位）。　　
dev_t 用于设备号。　　
off_t 用于文件大小和偏移量。　　
ptrdiff_t 是一种带符号整型，用于对两个指针执行减法运算后所得的结果。　　
size_t 反映内存中对象的大小（以字节为单位）。　　
ssize_t 供返回字节计数或错误提示的函数使用。　　
time_t 以秒为单位计时。
```
```cpp
#include <stdio.h>  
#include <stdlib.h> 
#include <sys/types.h>
/*
基本系统数据类型
是Unix/Linux系统的基本系统数据类型的头文件，含有size_t，time_t，pid_t等类型。
一般在/usr/include这个目录下。
*/
int main(int argc,char *argv[])  
{ 
    printf("clock_t = %d\n",sizeof(clock_t));
    printf("dev_t = %d\n",sizeof(dev_t));
    printf("off_t = %d\n",sizeof(off_t));
    //printf("ptrdiff_t = %d\n",sizeof(ptrdiff_t));
    printf("size_t = %d\n",sizeof(size_t));
    printf("ssize_t = %d\n",sizeof(ssize_t));
    printf("time_t = %d\n",sizeof(time_t));
}
```
输出为：
```
$ ./a.out 
clock_t = 8
dev_t = 8
off_t = 8
size_t = 8
ssize_t = 8
time_t = 8
```
linux源代码为：
```cpp
/* Copyright (C) 1991,1992,1994-2002,2006,2010 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */
/*
 *	POSIX Standard: 2.6 Primitive System Data Types	<sys/types.h>
 */
#ifndef	_SYS_TYPES_H
#define	_SYS_TYPES_H	1
#include <features.h>
__BEGIN_DECLS
#include <bits/types.h>
#ifdef	__USE_BSD
# ifndef __u_char_defined
typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;
#  define __u_char_defined
# endif
#endif
typedef __loff_t loff_t;
#ifndef __ino_t_defined
# ifndef __USE_FILE_OFFSET64
typedef __ino_t ino_t;
# else
typedef __ino64_t ino_t;
# endif
# define __ino_t_defined
#endif
#if defined __USE_LARGEFILE64 && !defined __ino64_t_defined
typedef __ino64_t ino64_t;
# define __ino64_t_defined
#endif
#ifndef __dev_t_defined
typedef __dev_t dev_t;
# define __dev_t_defined
#endif
#ifndef __gid_t_defined
typedef __gid_t gid_t;
# define __gid_t_defined
#endif
#ifndef __mode_t_defined
typedef __mode_t mode_t;
# define __mode_t_defined
#endif
#ifndef __nlink_t_defined
typedef __nlink_t nlink_t;
# define __nlink_t_defined
#endif
#ifndef __uid_t_defined
typedef __uid_t uid_t;
# define __uid_t_defined
#endif
#ifndef __off_t_defined
# ifndef __USE_FILE_OFFSET64
typedef __off_t off_t;
# else
typedef __off64_t off_t;
# endif
# define __off_t_defined
#endif
#if defined __USE_LARGEFILE64 && !defined __off64_t_defined
typedef __off64_t off64_t;
# define __off64_t_defined
#endif
#ifndef __pid_t_defined
typedef __pid_t pid_t;
# define __pid_t_defined
#endif
#if (defined __USE_SVID || defined __USE_XOPEN || defined __USE_XOPEN2K8) \
    && !defined __id_t_defined
typedef __id_t id_t;
# define __id_t_defined
#endif
#ifndef __ssize_t_defined
typedef __ssize_t ssize_t;
# define __ssize_t_defined
#endif
#ifdef	__USE_BSD
# ifndef __daddr_t_defined
typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;
#  define __daddr_t_defined
# endif
#endif
#if (defined __USE_SVID || defined __USE_XOPEN) && !defined __key_t_defined
typedef __key_t key_t;
# define __key_t_defined
#endif
#if defined __USE_XOPEN || defined __USE_XOPEN2K8
# define __need_clock_t
#endif
#define	__need_time_t
#define __need_timer_t
#define __need_clockid_t
#include <time.h>
#ifdef __USE_XOPEN
# ifndef __useconds_t_defined
typedef __useconds_t useconds_t;
#  define __useconds_t_defined
# endif
# ifndef __suseconds_t_defined
typedef __suseconds_t suseconds_t;
#  define __suseconds_t_defined
# endif
#endif
#define	__need_size_t
#include <stddef.h>
#ifdef __USE_MISC
/* Old compatibility names for C types.  */
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
#endif
/* These size-specific names are used by some of the inet code.  */
#if !__GNUC_PREREQ (2, 7)
/* These types are defined by the ISO C99 header <inttypes.h>. */
# ifndef __int8_t_defined
#  define __int8_t_defined
typedef	char int8_t;
typedef	short int int16_t;
typedef	int int32_t;
#  if __WORDSIZE == 64
typedef long int int64_t;
#  elif __GLIBC_HAVE_LONG_LONG
__extension__ typedef long long int int64_t;
#  endif
# endif
/* But these were defined by ISO C without the first `_'.  */
typedef	unsigned char u_int8_t;
typedef	unsigned short int u_int16_t;
typedef	unsigned int u_int32_t;
# if __WORDSIZE == 64
typedef unsigned long int u_int64_t;
# elif __GLIBC_HAVE_LONG_LONG
__extension__ typedef unsigned long long int u_int64_t;
# endif
typedef int register_t;
#else
/* For GCC 2.7 and later, we can use specific type-size attributes.  */
# define __intN_t(N, MODE) \
  typedef int int##N##_t __attribute__ ((__mode__ (MODE)))
# define __u_intN_t(N, MODE) \
  typedef unsigned int u_int##N##_t __attribute__ ((__mode__ (MODE)))
# ifndef __int8_t_defined
#  define __int8_t_defined
__intN_t (8, __QI__);
__intN_t (16, __HI__);
__intN_t (32, __SI__);
__intN_t (64, __DI__);
# endif
__u_intN_t (8, __QI__);
__u_intN_t (16, __HI__);
__u_intN_t (32, __SI__);
__u_intN_t (64, __DI__);
typedef int register_t __attribute__ ((__mode__ (__word__)));
/* Some code from BIND tests this macro to see if the types above are
   defined.  */
#endif
#define __BIT_TYPES_DEFINED__	1
#ifdef	__USE_BSD
/* In BSD <sys/types.h> is expected to define BYTE_ORDER.  */
# include <endian.h>
/* It also defines `fd_set' and the FD_* macros for `select'.  */
# include <sys/select.h>
/* BSD defines these symbols, so we follow.  */
# include <sys/sysmacros.h>
#endif /* Use BSD.  */
#if (defined __USE_UNIX98 || defined __USE_XOPEN2K8) \
    && !defined __blksize_t_defined
typedef __blksize_t blksize_t;
# define __blksize_t_defined
#endif
/* Types from the Large File Support interface.  */
#ifndef __USE_FILE_OFFSET64
# ifndef __blkcnt_t_defined
typedef __blkcnt_t blkcnt_t;	 /* Type to count number of disk blocks.  */
#  define __blkcnt_t_defined
# endif
# ifndef __fsblkcnt_t_defined
typedef __fsblkcnt_t fsblkcnt_t; /* Type to count file system blocks.  */
#  define __fsblkcnt_t_defined
# endif
# ifndef __fsfilcnt_t_defined
typedef __fsfilcnt_t fsfilcnt_t; /* Type to count file system inodes.  */
#  define __fsfilcnt_t_defined
# endif
#else
# ifndef __blkcnt_t_defined
typedef __blkcnt64_t blkcnt_t;	   /* Type to count number of disk blocks.  */
#  define __blkcnt_t_defined
# endif
# ifndef __fsblkcnt_t_defined
typedef __fsblkcnt64_t fsblkcnt_t; /* Type to count file system blocks.  */
#  define __fsblkcnt_t_defined
# endif
# ifndef __fsfilcnt_t_defined
typedef __fsfilcnt64_t fsfilcnt_t; /* Type to count file system inodes.  */
#  define __fsfilcnt_t_defined
# endif
#endif
#ifdef __USE_LARGEFILE64
typedef __blkcnt64_t blkcnt64_t;     /* Type to count number of disk blocks. */
typedef __fsblkcnt64_t fsblkcnt64_t; /* Type to count file system blocks.  */
typedef __fsfilcnt64_t fsfilcnt64_t; /* Type to count file system inodes.  */
#endif
/* Now add the thread types.  */
#if defined __USE_POSIX199506 || defined __USE_UNIX98
# include <bits/pthreadtypes.h>
#endif
__END_DECLS
#endif /* sys/types.h */
```
