# vs2013编译ffmpeg之四十二 zvbi - 枫林港的博客 - CSDN博客





2018年07月17日 23:37:49[枫林港](https://me.csdn.net/u013601132)阅读数：260









- [ffmpeg引用zvbib](#ffmpeg引用zvbib)
- [vs2013编译zvbi](#vs2013编译zvbi)- [1. 建立vs2013的工程](#1-建立vs2013的工程)
- [2. 宏定义](#2-宏定义)
- [3._vbi_inline](#3vbiinline)
- [4. __VA_ARGS__](#4-vaargs)
- [5. FOR_ALL_NODES宏](#5-forallnodes宏)
- [6. init_hcrc](#6-inithcrc)
- [7. case](#7-case)
- [8. unistd.h](#8-unistdh)
- [9. iconv.h](#9-iconvh)
- [10. SSIZE_MAX](#10-ssizemax)
- [11. __PRETTY_FUNCTION__](#11-prettyfunction)
- [12. time.h](#12-timeh)
- [13. strcasecmp](#13-strcasecmp)
- [14. ssize_t](#14-ssizet)
- [15. vbi_xds_demux_feed](#15-vbixdsdemuxfeed)
- [16. 空结构](#16-空结构)
- [17. libintl_printf](#17-libintlprintf)
- [18. S_ISREG](#18-sisreg)
- [19. mingw_fwrite](#19-mingwfwrite)
- [20. putwchar](#20-putwchar)
- [21. OutputDebugPrintf](#21-outputdebugprintf)
- [22. getopt.h](#22-getopth)
- [23. enum interface](#23-enum-interface)
- [24. write/read](#24-writeread)
- [25. C和C++相互调用](#25-c和c相互调用)
- [26. unsetenv](#26-unsetenv)

- [msys编译zvbi](#msys编译zvbi)- [1、langinfo.h](#1langinfoh)

- [sys/ioctl.h](#sysioctlh)- [sys/syslog.h](#syssyslogh)
- [fd_set](#fdset)
- [ntsc-cc.c](#ntsc-ccc)
- [atsc-cc.c](#atsc-ccc)
- [pthread](#pthread)
- [其他编译错误](#其他编译错误)
- [生成dll](#生成dll)




对应ffmpeg configure选项–enable-libzvbi。 
[zvbi](http://zapping.sourceforge.net/ZVBI/)是teletext、colsed caption功能的开源库，能完整支持最新版本的teletext，colsed caption支持不全。teletext虽然是个很古老的东东，但在欧洲很多国家，teletext是产品的准入门槛，没有这个功能，相关的产品就进不了市场。colsed caption也是类似，主要用在北美。

# ffmpeg引用zvbib

不管是用vs2013编译zvbi，还是用msys编译zvbi，其.h文件libzvbi.h需要拷贝出来放到ffmpeg/inc下面，因为zvbi-0.2.35\src下面有io.h，会导致configure检测io.h的时候会出错，导致HAVE_IO_H没有定义。
`H:\MinGW\msys\1.0\home\AA\contribute\MSVC\zvbi-0.2.35\src\io.h(33) : fatal error C1083: Cannot open include file: 'sys/time.h': No such file or directory`
从而编译libavformat/cache.c的时候会出错：

```
libavformat/cache.c(67) : warning C4996: 'unlink': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _unlink. See online help for details.
        C:\Program Files\Microsoft Visual Studio 10.0\VC\INCLUDE\stdio.h(299) : see declaration of 'unlink'
libavformat/cache.c(79) : error C4013: 'read' undefined; assuming extern returning int
libavformat/cache.c(86) : error C4013: 'write' undefined; assuming extern returning int
```

ffmpeg的configure里面的enabled libzvbi规则，需要加上-lintl -lpthread -lm -liconv -lAdvapi32，针对ffmpeg3.3将：
`enabled libzvbi && require libzvbi libzvbi.h vbi_decoder_new -lzvbi &&`
改成：
`enabled libzvbi && require libzvbi libzvbi.h vbi_decoder_new -lzvbi -lintl -lpthread -lm -liconv -lAdvapi32 &&`
# vs2013编译zvbi

为了在window上对比和调试teletext功能，直接弄了个vs2013能编译的zvbi，主要是针对teletext功能相关的代码。 

从[https://sourceforge.net/projects/zapping/](https://sourceforge.net/projects/zapping/)下载zvbi-0.2.35.tar.bz2

## 1. 建立vs2013的工程

zvbi的代码先在msys+gcc下编译通过，生成config.h，并找出编译了哪些文件，为后面创建vs工程做准备。 

vs2013新建解决方案zvbi-0.2.35_MSVC，下面有3个空的工程：zvbi-0.2.35_MSVC、decode、glyph。最主要的是zvbi-0.2.35_MSVC，decode、glyph是测试zvbi用的。 
![这里写图片描述](https://img-blog.csdn.net/20180705000047467?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后为各工程添加源文件： 
![这里写图片描述](https://img-blog.csdn.net/20180705000056790?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180705000106947?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180705000114645?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180705000125585?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

接下来解决编译问题。将最主要的编译问题罗列如下。
## 2. 宏定义

各工程的宏定义设置如下： 

WIN32 

_DEBUG 

_LIB 

HAVE_CONFIG_H 

_REENTRANT 

_GNU_SOURCE 

HAVE_STRUCT_TIMESPEC 

_WIN32_WINNT=0x0502 

_vbi_inline=__inline 

inline=__inline 
**inline**=__inline 

strncasecmp=strnicmp 

strcasecmp=stricmp 

这些宏定义是从ffmpeg的vs2013编译打印里面弄出来的。
## 3._vbi_inline

`__attribute__`关键字vs2013识别不了。

```
#if __GNUC__ >= 2
#  define _vbi_unused __attribute__ ((__unused__))
#  define _vbi_const __attribute__ ((__const__))
#  define _vbi_inline static __inline__
#else
#  define _vbi_unused
#  define _vbi_const
#  define _vbi_inline static
#endif
```

改成：

```
#ifdef _MSC_VER
#  define _vbi_unused
#  define _vbi_const
#else
#if __GNUC__ >= 2
#  define _vbi_unused __attribute__ ((__unused__))
#  define _vbi_const __attribute__ ((__const__))
#  define _vbi_inline static __inline__
#else
#  define _vbi_unused
#  define _vbi_const
#  define _vbi_inline static
#endif
#endif
```

## 4. __VA_ARGS__

vs2013可变参数的用法和gcc的不一样，在编译的过程中有下面的错误：
`h:\mingw\msys\1.0\home\aa\contribute\msvc\zvbi-0.2.35\src\misc.h(368) : error C2010: '.' : unexpected in macro formal parameter list`
.\src\misc.h里面，将：

```
#define _vbi_log(hook, level, templ, args...)               \
do {                                    \
    _vbi_log_hook *_h = hook;                   \
                                    \
    if ((NULL != _h && 0 != (_h->mask & level))         \
        || (_h = &_vbi_global_log, 0 != (_h->mask & level)))    \
        _vbi_log_printf (_h->fn, _h->user_data,     \
                  level, __FILE__, __FUNCTION__,    \
                  templ , ##args);          \
} while (0)
```

改成：

```
#define _vbi_log(hook, level, templ, ...)               \
do {                                    \
    _vbi_log_hook *_h = hook;                   \
                                    \
    if ((NULL != _h && 0 != (_h->mask & level))         \
        || (_h = &_vbi_global_log, 0 != (_h->mask & level)))    \
        _vbi_log_printf (_h->fn, _h->user_data,     \
                  level, __FILE__, __FUNCTION__,    \
                  templ , __VA_ARGS__);         \
} while (0)
```

将：

```
#define error(hook, templ, args...)                 \
    _vbi_log (hook, VBI_LOG_ERROR, templ , ##args)
#define warning(hook, templ, args...)                   \
    _vbi_log (hook, VBI_LOG_ERROR, templ , ##args)
#define notice(hook, templ, args...)                    \
    _vbi_log (hook, VBI_LOG_NOTICE, templ , ##args)
#define info(hook, templ, args...)                  \
    _vbi_log (hook, VBI_LOG_INFO, templ , ##args)
#define debug1(hook, templ, args...)                    \
    _vbi_log (hook, VBI_LOG_DEBUG, templ , ##args)
#define debug2(hook, templ, args...)                    \
    _vbi_log (hook, VBI_LOG_DEBUG2, templ , ##args)
#define debug3(hook, templ, args...)                    \
    _vbi_log (hook, VBI_LOG_DEBUG3, templ , ##args)
```

改成：

```
#define error(hook, templ, ...)                 \
    _vbi_log (hook, VBI_LOG_ERROR, templ , __VA_ARGS__)
#define warning(hook, templ, ...)                   \
    _vbi_log (hook, VBI_LOG_ERROR, templ , __VA_ARGS__)
#define notice(hook, templ, ...)                    \
    _vbi_log (hook, VBI_LOG_NOTICE, templ , __VA_ARGS__)
#define info(hook, templ, ...)                  \
    _vbi_log (hook, VBI_LOG_INFO, templ , __VA_ARGS__)
#define debug1(hook, templ, ...)                    \
    _vbi_log (hook, VBI_LOG_DEBUG, templ , __VA_ARGS__)
#define debug2(hook, templ, ...)                    \
    _vbi_log (hook, VBI_LOG_DEBUG2, templ , __VA_ARGS__)
#define debug3(hook, templ, ...)                    \
    _vbi_log (hook, VBI_LOG_DEBUG3, templ , __VA_ARGS__)
```

有好几个文件里面都要这样修改，如teletext.c里面：

```
#define printv(templ, args...)                      \
do {                                    \
    if (TELETEXT_DEBUG)                     \
        fprintf (stderr, templ ,##args);            \
} while (0)
```

改成：

```
#define printv(templ, ...)                      \
do {                                    \
    if (TELETEXT_DEBUG)                     \
        fprintf (stderr, templ , __VA_ARGS__);          \
} while (0)
```

## 5. FOR_ALL_NODES宏

```
1>e:\mingw\msys\1.0\home\aa\ffmpeg_msvc\contribute\msvc\zvbi-0.2.35_msvc\src\cache.c(298): warning C4013: '__typeof__' undefined; assuming extern returning int
1>e:\mingw\msys\1.0\home\aa\ffmpeg_msvc\contribute\msvc\zvbi-0.2.35_msvc\src\cache.c(298): error C2059: syntax error : ')'
1>e:\mingw\msys\1.0\home\aa\ffmpeg_msvc\contribute\msvc\zvbi-0.2.35_msvc\src\cache.c(298): error C2143: syntax error : missing ';' before ')'
1>e:\mingw\msys\1.0\home\aa\ffmpeg_msvc\contribute\msvc\zvbi-0.2.35_msvc\src\cache.c(301): error C2059: syntax error : '}'
```

这个是由于VS2013对**typeof**支持有问题导致的。FOR_ALL_NODES只在cache.c里面出现，所以直接修改宏定义。解决办法：

```
#define FOR_ALL_NODES(p, p1, l, _node)                  \
for (verify_ring (l), p = PARENT ((l)->_succ, __typeof__ (* p), _node); \
     p1 = PARENT (p->_node._succ, __typeof__ (* p), _node),         \
     &p->_node != (l); p = p1)

#define FOR_ALL_NODES_REVERSE(p, p1, l, _node)              \
for (verify_ring (l), p = PARENT ((l)->_pred, __typeof__ (* p), _node); \
     p1 = PARENT (p->_node._pred, __typeof__ (* p), _node),     \
     &p->_node != (l); p = p1)
```

改成：

```
#define FOR_ALL_NODES(p, _typeofp, p1, l, _node)                    \
for (verify_ring (l), p = PARENT ((l)->_succ, _typeofp, _node); \
     p1 = PARENT (p->_node._succ, _typeofp, _node),         \
     &p->_node != (l); p = p1)

#define FOR_ALL_NODES_REVERSE(p, _typeofp, p1, l, _node)                \
for (verify_ring (l), p = PARENT ((l)->_pred, _typeofp, _node); \
     p1 = PARENT (p->_node._pred, _typeofp, _node),     \
     &p->_node != (l); p = p1)
```

即把类型作为参数传进去。调用的地方：
`FOR_ALL_NODES (cn, cn1, &ca->networks, node) `
改成：

```
FOR_ALL_NODES (cn, cache_network, cn1, &ca->networks, node)
```

## 6. init_hcrc

VS2013不支持`__attribute__`，caption.c代码里面有：
`static void init_hcrc(void) __attribute__ ((constructor));`
这个函数要在main函数之前运行，初始化hcrc数组。constructor是告诉编译器，init_gfx函数会在main函数之前运行。由于在vs2013里面没有查到相关的机制，这个函数只能直接调用。改动如下：

```
#ifdef _MSC_VER
static void init_hcrc(void) __attribute__ ((constructor));
#else
static void init_hcrc(void);
#endif
```

vbi_caption_init里面最开始加上：

```
#ifdef _MSC_VER
    init_hcrc();
#endif
```

init_hcrc()可以不用改，因为这个是CC字幕才会用到，而ffmpeg的CC字幕解码是用的内置解码器。但如果用zvbi下面的CC解码测试代码生成exe文件，这个还是需要修改的。

exp-gfx.c里面的init_gfx也是同样的处理方法，在vbi.c–>vbi_decoder_new()最开始的地方，加上init_gfx()的调用。vbi_decoder_new只会被ffmpeg调用一次。 

vlc的teletext解码也是用的zvbi，如果要用VS编译vlc并替换zvbi库的话，也需要按上面的方法来改。不改的话，若vlc或ffmpeg引用了该库，解码出来的teletext是花屏。这个花屏问题单步调试了很久终于找到了原因。

## 7. case

VS2013不支持case 0x10 … 0x17这种写法，改成： 

        case 0x10: 

        case 0x11: 

        case 0x12: 

        case 0x13: 

        case 0x14: 

        case 0x15: 

        case 0x16: 

        case 0x17: 

有的case如case 0x20 … 0x7F: 就直接用if else替换了。很多文件都需要修改。
## 8. unistd.h

unistd.h用的是ShiftMediaProject上下载的libgpg-error-master里面的unistd.h。内容如下：

```
#ifndef _SMP_UNISTD_H_
#define _SMP_UNISTD_H_

#ifndef _MSC_VER
#   include_next <unistd.h>
#else

#ifndef __STRICT_ANSI__

#include <stdlib.h>
//#include <io.h>
#include <process.h>
#include <direct.h>
#include <fcntl.h>

#define __UNISTD_GETOPT__
//#include <getopt.h>
#undef __UNISTD_GETOPT__

#define R_OK    4       /* Test for read permission.  */
#define W_OK    2       /* Test for write permission.  */
//#define   X_OK    1       /* execute permission - unsupported in windows*/
#define F_OK    0       /* Test for existence.  */

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif

#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

#define srandom srand
#define random rand

#define inline __inline
typedef int mode_t;
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;

#endif /* __STRICT_ANSI__ */

#endif /* _MSC_VER */

#endif /* _SMP_UNISTD_H_ */
```

## 9. iconv.h

工程头文件路径加上../../../libiconv-1.9.2/build/iconv，用的是libiconv-1.9.2-vc10.7z里面的。

## 10. SSIZE_MAX

export.c编译报SSIZE_MAX没有定义，直接取\MinGW\include\limits.h里面的定义：

```
#define LONG_MAX    2147483647L
#define ULONG_MAX   0xffffffffUL
#define SSIZE_MAX LONG_MAX
```

## 11. __PRETTY_FUNCTION__
`\export.c(2061): error C2065: '__PRETTY_FUNCTION__' : undeclared identifier`
直接用**FUNCTION**替换。

## 12. time.h

在编译proxy-client.c时，需要sys/time.h，直接将timeval拷贝进来：

```
#ifdef _MSC_VER

#else
#include <sys/time.h>
#endif

#ifndef _TIMEVAL_DEFINED /* also in winsock[2].h */
#define _TIMEVAL_DEFINED
struct timeval {
  long tv_sec;
  long tv_usec;
};
#define timerisset(tvp)  ((tvp)->tv_sec || (tvp)->tv_usec)
#define timercmp(tvp, uvp, cmp) \
    (((tvp)->tv_sec != (uvp)->tv_sec) ? \
    ((tvp)->tv_sec cmp (uvp)->tv_sec) : \
    ((tvp)->tv_usec cmp (uvp)->tv_usec))
#define timerclear(tvp)  (tvp)->tv_sec = (tvp)->tv_usec = 0
#endif /* _TIMEVAL_DEFINED */
```

另外一个解决办法是加：#include 

## 13. strcasecmp

teletext.c里面注释掉#include 

```
#if defined(_MSC_VER)
#define strcasecmp _stricmp
#endif
```

## 14. ssize_t

VS2013没有定义ssize_t，直接加定义：

```
#if defined(_MSC_VER)
#define strcasecmp _stricmp
#endif
```

## 15. vbi_xds_demux_feed

这个函数里面有段代码很怪异：

```
discard:
        if (sp) {
            sp->count = 0;
            sp->checksum = 0;
        }

        /* fall through */

    case 0x10 ... 0x1F:
        /* Closed Caption. */

        xd->curr_sp = NULL;

        break;
```

前面有语句会goto discard， 但discard后面没有break，不知道是不是笔误。应该后面还会走xd->curr_sp = NULL;语句。 

这个逻辑结构很乱，将这个switch改成if的时候要注意。

## 16. 空结构

proxy-msg.h里面有几个空结构，proxy-client.c编译的时候会报错。提示结构不能为空。 

查了一下资料，C语言不能有空结构，C++可以有空结构，C++里面将结构看作类，类可以为空。 

解决的办法是直接将proxy-client.c重命名为proxy-client.cpp。但这样编译出来可能会影响代码运行，因为gcc编译空结构时，空结构大小为0，而VS的cl编译器则认为空结构大小为1(不考虑对齐)。proxy-client.c代码里面有取空结构的大小进行计算，所以可能会出错。 

proxy-client.c里面的代码看上去是与client通信用的，不知道是啥功能，因为调试时只需要teletext解码功能，proxy-client.c里面的代码可能跑不到，暂先这样修改。
## 17. libintl_printf

> 
1>zvbi.lib(packet.obj) : error LNK2001: unresolved external symbol _libintl_printf


这个是从mingw里面拷贝的libintl.h头文件导致的，直接注释掉里面的：
`#define printf libintl_printf`
snprintf也是类似：
`#define snprintf libintl_snprintf`
## 18. S_ISREG

> 
1>zvbi.lib(export.obj) : error LNK2001: unresolved external symbol _S_ISREG


在export.c里面加上：

```
#ifdef _MSC_VER
#if !S_IRUSR && S_IREAD
# define S_IRUSR S_IREAD
#endif
#if !S_IRUSR
# define S_IRUSR 00400
#endif
#if !S_IRGRP
# define S_IRGRP (S_IRUSR >> 3)
#endif
#if !S_IROTH
# define S_IROTH (S_IRUSR >> 6)
#endif

#if !S_IWUSR && S_IWRITE
# define S_IWUSR S_IWRITE
#endif
#if !S_IWUSR
# define S_IWUSR 00200
#endif
#if !S_IWGRP
# define S_IWGRP (S_IWUSR >> 3)
#endif
#if !S_IWOTH
# define S_IWOTH (S_IWUSR >> 6)
#endif

# ifndef EOVERFLOW
#  define EOVERFLOW 2006
#  define GNULIB_defined_EOVERFLOW 1
# endif

#define LONG_MAX    2147483647L
#define ULONG_MAX   0xffffffffUL

#define SSIZE_MAX LONG_MAX


#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#endif
```

在编译./test/decode.c、./test/sliced.c的时候也需要在sliced.c里面加上。

## 19. mingw_fwrite

> 
1>mingwex.lib(mingw-fseek.o) : error LNK2019: unresolved external symbol __imp___osver referenced in function ___mingw_fwrite


参考[https://github.com/Alexpux/mingw-w64/blob/master/mingw-w64-crt/misc/mingw-fseek.c](https://github.com/Alexpux/mingw-w64/blob/master/mingw-w64-crt/misc/mingw-fseek.c)里面的__mingw_fwrite实现： 

size_t

```
__mingw_fwrite(const void *buffer, size_t size, size_t count, FILE *fp)
{
# undef fwrite 
    if ((_osver & 0x8000) && __mingw_fseek_called)
    {
        ULARGE_INTEGER actual_length;
        LARGE_INTEGER current_position;

        memset(¤t_position, 0, sizeof(LARGE_INTEGER));
        __mingw_fseek_called = 0;
        fflush(fp);
        actual_length.LowPart = GetFileSize((HANDLE)_get_osfhandle(fileno(fp)),
            &actual_length.HighPart);
        if (actual_length.LowPart == 0xFFFFFFFF
            && GetLastError() != NO_ERROR)
            return -1;
        current_position.LowPart = SetFilePointer((HANDLE)_get_osfhandle(fileno(fp)),
            current_position.LowPart,
            ¤t_position.HighPart,
            FILE_CURRENT);
        if (current_position.LowPart == 0xFFFFFFFF
            && GetLastError() != NO_ERROR)
            return -1;

#ifdef DEBUG
        printf("__mingw_fwrite: current %I64u, actual %I64u\n",
            current_position.QuadPart, actual_length.QuadPart);
#endif /* DEBUG */
        if ((size_t)current_position.QuadPart > (size_t)actual_length.QuadPart)
        {
            static char __mingw_zeros[ZEROBLOCKSIZE];
            long long numleft;

            SetFilePointer((HANDLE)_get_osfhandle(fileno(fp)),
                0, 0, FILE_END);
            numleft = current_position.QuadPart - actual_length.QuadPart;

#ifdef DEBUG
            printf("__mingw_fwrite: Seeking %I64d bytes past end\n", numleft);
#endif /* DEBUG */
            while (numleft > 0LL)
            {
                DWORD nzeros = (numleft > ZEROBLOCKSIZE)
                    ? ZEROBLOCKSIZE : numleft;
                DWORD written;
                if (!WriteFile((HANDLE)_get_osfhandle(fileno(fp)),
                    __mingw_zeros, nzeros, &written, NULL))
                {
                    /* Best we can hope for, or at least DJ says so. */
                    SetFilePointer((HANDLE)_get_osfhandle(fileno(fp)),
                        0, 0, FILE_BEGIN);
                    return -1;
                }
                if (written < nzeros)
                {
                    /* Likewise. */
                    SetFilePointer((HANDLE)_get_osfhandle(fileno(fp)),
                        0, 0, FILE_BEGIN);
                    return -1;
                }

                numleft -= written;
            }
            FlushFileBuffers((HANDLE)_get_osfhandle(fileno(fp)));
        }
    }
    return fwrite(buffer, size, count, fp);
}
```

没有找到mingw32的代码。从mingw-w64-crt代码里面搜索_osver，在stdlib.h里面有其定义：

```
#ifndef _osver
#ifdef _MSVCRT_
extern unsigned int _osver;
#else
extern unsigned int * __MINGW_IMP_SYMBOL(_osver);
#define _osver  (* __MINGW_IMP_SYMBOL(_osver))
#endif
#endif
```

直接在zvbi的export.c里面定义：

```
#ifdef _MSC_VER
unsigned int _osver = 0;
#endif
```

就可以解决该问题。但不知道__mingw_fwrite里面对_osver的判断是啥意思，不清楚是否会有影响。

## 20. putwchar

zvbi-0.2.35\test\glyph.c里面有putwchar函数，这个函数在系统中有定义了，glyph.c里面的需要改个名字。

## 21. OutputDebugPrintf

libzvbi里面的打印都是printf、fprintf，要改成能在VS2013 的immediate window里面显示。改动的方法请参考OutputDebugPrintf相关章节。

## 22. getopt.h

./test/decode.c编译的时候需要getopt.h，直接拷贝/mingw/include/getopt.h，去掉了#include _mingw.h，并将_mingw.h的内容放到getopt.h里面。

## 23. enum interface

interface可能是VS2013的关键字，需要将其改成e_interface。

## 24. write/read

sliced.c(改名成sliced.cpp，因为有空结构的问题)里面有调用write/read，但这个函数的定义在VS2013里面找不到，所以拷贝了/mingw/include/io.h里面的定义放到sliced.cpp：

```
int write(int, const void*, unsigned int);
int isatty(int);
long lseek(int, long, int);
int open(const char*, int, ...);
int read(int, void*, unsigned int);
int close(int);
```

## 25. C和C++相互调用

slice.c改名成slice.cpp后，需要在slice.cpp、slice.h里面加上：

```
#ifdef __cplusplus 
extern "C" {
#endif
//slice.cpp原有代码...

#ifdef __cplusplus 
}
#endif
```

extern “C”放在#include后面，#include “src/vbi.h”要放到extern “C”里面，因为slice.cpp会调用vbi_set_log_fn，不然会报缺这个符号。

## 26. unsetenv

pdc.c里面有调用unsetenv，VS2013里面没有此函数，对应的函数是_putenv_s。

# msys编译zvbi

msys编译zvbi的改动如下：

## 1、langinfo.h

编译时遇到下面的错误：

```
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I. -I.. -I.. -D_REENTRANT -D_GNU_SOURCE -DHAVE_STRUCT_TIMESPEC -MT conv.lo -MD -MP -MF .deps/conv.Tpo -c conv.c  -DDLL_EXPORT -DPIC -o .libs/conv.o
conv.c:29:22: fatal error: langinfo.h: No such file or directory
 #include <langinfo.h>
                      ^
compilation terminated.
make[4]: *** [conv.lo] Error 1
make[4]: Leaving directory `/home/AA/ffmpeg_MSVC/contribute/MSVC/tmp/zvbi-0.2.35/src'
make[3]: *** [all-recursive] Error 1
make[3]: Leaving directory `/home/AA/ffmpeg_MSVC/contribute/MSVC/tmp/zvbi-0.2.35/src'
make[2]: *** [all] Error 2
make[2]: Leaving directory `/home/AA/ffmpeg_MSVC/contribute/MSVC/tmp/zvbi-0.2.35/src'
make[1]: *** [all-recursive] Error 1
make[1]: Leaving directory `/home/AA/ffmpeg_MSVC/contribute/MSVC/tmp/zvbi-0.2.35'
make: *** [all] Error 2
```

langinfo.h在m4-1.4.16源代码下面有，直接拷贝此文件放到zvbi-0.2.35\src下面，就可以解决该问题。

# sys/ioctl.h

如果编译的时候遇到下面的错误：

```
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I. -I.. -I.. -D_REENTRANT -D_GNU_SOURCE -DHAVE_STRUCT_TIMESPEC -g -O2 -MT io.lo -MD -MP -MF .dep
s/io.Tpo -c io.c  -DDLL_EXPORT -DPIC -o .libs/io.o
io.c:31:38: fatal error: sys/ioctl.h: No such file or directory
 #include <sys/ioctl.h>  /* ioctl() */
```

sys/ioctl.h是linux下才有的，是针对v4l驱动的，windows下没有这个东东，直接将相关的.c、.h文件去掉，下面是zvbi-0.2.35\src\Makefile的对比，左边是已经去掉的。 

如果去掉后还是有报这个错误，则直接注释掉“#include 

## sys/syslog.h

```
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I. -I.. -I.. -D_REENTRANT -D_GNU_SOURCE -DHAVE_STRUCT_TIMESPEC -g -O2 -MT proxy-client.lo -MD -MP -MF .deps/proxy-client.Tpo -c proxy-client.c  -DDLL_EXPORT -DPIC -o .libs/proxy-client.o
In file included from proxy-client.c:47:0:
proxy-msg.h:71:24: fatal error: sys/syslog.h: No such file or directory
 #include <sys/syslog.h>
```

sys/syslog.h也是linux下独有的，直接注释掉“#include 

## fd_set

```
if gcc -DHAVE_CONFIG_H -I. -I. -I..  -I..  -D_REENTRANT -D_GNU_SOURCE   -DHAVE_STRUCT_TIMESPEC -g -O2 -MT ntsc-cc.o -MD -MP -MF ".deps/ntsc-
cc.Tpo" -c -o ntsc-cc.o ntsc-cc.c; \
        then mv -f ".deps/ntsc-cc.Tpo" ".deps/ntsc-cc.Po"; else rm -f ".deps/ntsc-cc.Tpo"; exit 1; fi
ntsc-cc.c: In function 'main':
ntsc-cc.c:1330:4: error: unknown type name 'fd_set'
```

直接在ntsc-cc.c里面加上头文件#include “winsock.h”，winsock.h位于\MinGW\include下面。

## ntsc-cc.c

生成ntsc-cc.exe的时候报下面的错误：

```
libtool: link: gcc -DHAVE_STRUCT_TIMESPEC -g -O2 -o .libs/zvbi-ntsc-cc.exe ntsc-cc.o  ../src/.libs/libzvbi.a -lintl -lpthread -liconv
ntsc-cc.o: In function `main':
E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\contribute\MSVC\tmp\zvbi-0.2.35\contrib/ntsc-cc.c:1541: undefined reference to `vbi_capture_v4l2_new'
E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\contribute\MSVC\tmp\zvbi-0.2.35\contrib/ntsc-cc.c:1656: undefined reference to `vbi_capture_read'
E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\contribute\MSVC\tmp\zvbi-0.2.35\contrib/ntsc-cc.c:1580: undefined reference to `vbi_capture_parameters
'
E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\contribute\MSVC\tmp\zvbi-0.2.35\contrib/ntsc-cc.c:1556: undefined reference to `vbi_capture_v4l_new'
../src/.libs/libzvbi.a(conv.o): In function `vbi_locale_codeset':
E:\MinGW\msys\1.0\home\AA\ffmpeg_MSVC\contribute\MSVC\tmp\zvbi-0.2.35\src/conv.c:1239: undefined reference to `nl_langinfo'
collect2.exe: error: ld returned 1 exit status
```

缺nl_langinfo符号，直接在zvbi-0.2.35\src\conv.c里面加上下面的代码：

```
/*
The nl_langinfo() function provides access to locale information in a
more flexible way than localeconv(3) does.  Individual and additional
elements of the locale categories can be queried.

Examples for the locale elements that can be specified in item using
the constants defined in <langinfo.h> are:

CODESET (LC_CTYPE)
Return a string with the name of the character encoding used
in the selected locale, such as "UTF-8", "ISO-8859-1", or
"ANSI_X3.4-1968" (better known as US-ASCII).  This is the same
string that you get with "locale charmap".  For a list of
character encoding names, try "locale -m", cf. locale(1).
*/

char *nl_langinfo(nl_item item)
{
    return "UTF-8";
}
```

nl_langinfo的代码来源不明，可能是从m4源代码里面拷贝过来的。

另外几个和v4l相关的函数直接在ntsc-cc.c里面加上空函数：

```
vbi_capture *
vbi_capture_v4l2_new(const char *       dev_name,
int         buffers,
unsigned int *      services,
int         strict,
char **     errstr,
vbi_bool        trace)
{
    return NULL;
}
int
vbi_capture_read(vbi_capture *capture, void *raw_data,
vbi_sliced *sliced_data, int *lines,
double *timestamp, struct timeval *timeout)
{
    return 0;
}
vbi_raw_decoder *
vbi_capture_parameters(vbi_capture *capture)
{
    return NULL;
}
vbi_capture *
vbi_capture_v4l_new(const char *dev_name, int scanning,
unsigned int *services, int strict,
char **errstr, vbi_bool trace)
{
    return NULL;
}
```

## atsc-cc.c

下面的错误：

```
if gcc -DHAVE_CONFIG_H -I. -I. -I..  -I..  -D_REENTRANT -D_GNU_SOURCE   -DHAVE_STRUCT_TIMESPEC -g -O2 -MT atsc-cc.o -MD -MP -MF ".deps/atsc-
cc.Tpo" -c -o atsc-cc.o atsc-cc.c; \
        then mv -f ".deps/atsc-cc.Tpo" ".deps/atsc-cc.Po"; else rm -f ".deps/atsc-cc.Tpo"; exit 1; fi
atsc-cc.c:51:23: fatal error: sys/ioctl.h: No such file or directory
 #include <sys/ioctl.h>

if gcc -DHAVE_CONFIG_H -I. -I. -I..  -I..  -D_REENTRANT -D_GNU_SOURCE   -DHAVE_STRUCT_TIMESPEC -g -O2 -MT atsc-cc.o -MD -MP -MF ".deps/atsc-
cc.Tpo" -c -o atsc-cc.o atsc-cc.c; \
        then mv -f ".deps/atsc-cc.Tpo" ".deps/atsc-cc.Po"; else rm -f ".deps/atsc-cc.Tpo"; exit 1; fi
atsc-cc.c:54:25: fatal error: sys/statvfs.h: No such file or directory
 #include <sys/statvfs.h>

if gcc -DHAVE_CONFIG_H -I. -I. -I..  -I..  -D_REENTRANT -D_GNU_SOURCE   -DHAVE_STRUCT_TIMESPEC -g -O2 -MT atsc-cc.o -MD -MP -MF ".deps/atsc-
cc.Tpo" -c -o atsc-cc.o atsc-cc.c; \
        then mv -f ".deps/atsc-cc.Tpo" ".deps/atsc-cc.Po"; else rm -f ".deps/atsc-cc.Tpo"; exit 1; fi
In file included from atsc-cc.c:66:0:
../src/dvb/dmx.h:27:23: fatal error: asm/types.h: No such file or directory
 #include <asm/types.h>
```

注释掉下面几个头文件include：

```
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/statvfs.h>
#include <sys/wait.h>
#include <asm/types.h>
```

```
if gcc -DHAVE_CONFIG_H -I. -I. -I..  -I..  -D_REENTRANT -D_GNU_SOURCE   -DHAVE_STRUCT_TIMESPEC -g -O2 -MT atsc-cc.o -MD -MP -MF ".deps/atsc-
cc.Tpo" -c -o atsc-cc.o atsc-cc.c; \
        then mv -f ".deps/atsc-cc.Tpo" ".deps/atsc-cc.Po"; else rm -f ".deps/atsc-cc.Tpo"; exit 1; fi
In file included from atsc-cc.c:66:0:
../src/dvb/dmx.h:94:2: error: unknown type name '__u8'
```

在dmx.h里面加上下面的代码：

```
#ifndef _I386_TYPES_H
#define _I386_TYPES_H

typedef unsigned short umode_t;

/*
* __xx is ok: it doesn't pollute the POSIX namespace. Use these in the
* header files exported to user space
*/

typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;

#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
typedef __signed__ long long __s64;
typedef unsigned long long __u64;
#endif
#endif
```

```
atsc-cc.c:1372:13: error: too many arguments to function 'mkdir'
   if (-1 == mkdir (buf, /* mode */ 0777))
```
`if (-1 == mkdir (buf, /* mode */ 0777)) `
改成：
`if (-1 == mkdir (buf))`
下面这个错误没有解决，因为库可以生成了，所以去掉.exe文件的编译，包括：test目录、examples目录、atsc-cc.c等的编译。

```
if gcc -DHAVE_CONFIG_H -I. -I. -I..  -I..  -D_REENTRANT -D_GNU_SOURCE   -DHAVE_STRUCT_TIMESPEC -g -O2 -D_LARGE_FILE -MT atsc-cc.o -MD -MP -MF ".deps/atsc-cc.Tpo" -c -o atsc-cc.o atsc-cc.c; \
        then mv -f ".deps/atsc-cc.Tpo" ".deps/atsc-cc.Po"; else rm -f ".deps/atsc-cc.Tpo"; exit 1; fi
In file included from e:\mingw\include\windows.h:55:0,
                 from e:\mingw\include\winsock.h:19,
                 from atsc-cc.c:69:
atsc-cc.c:353:2: error: expected identifier before '(' token
  EDGE_RAISED,
  ^
atsc-cc.c: In function 'open_minicut_file':
atsc-cc.c:1392:8: error: 'O_LARGEFILE' undeclared (first use in this function)
        O_LARGEFILE | O_WRONLY), 0666);
```

## pthread

下面这个错误是由zvbi库编译加了pthread.lib，
`pthread.lib(dquns00069.o) : error LNK2005: _pthread_once already defined in OpenAL32.lib(helpers.obj)`
查看zvbi的代码，其用到了pthread的函数如pthread_mutex_init，后来在ffmpeg里面找到了这些函数在windows下的实现。直接将这些代码拷贝到zvbi里面，并改下名字，重新编译就可以了。 

后来发现直接改zvbi的代码解决不了问题，不知道为啥找不到InitializeCriticalSection等函数。所以只能改openal的代码，直接将pthread_once改名，并重新编译。

ffmpeg在configure的时候报下面的错误：
`ffmpeg-3.3\inc\libzvbi.h(1072) : fatal error C1083: Cannot open include file: 'pthread.h': No such file or directory`
libzvbi.h里面include了pthread.h，需要将~\msys\1.0\win32\include\下面的pthread.h、sched.h都拷贝到ffmpeg-3.3\inc下面。

## 其他编译错误

win7+mingw32，重新安装了mingw32和msys，重新在msys下面编译zvbi的时候，出现下面的错误：

```
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I. -I.. -I.. -D_REENTRANT -D_GNU_SOURCE -g -O2 -MT bit_slicer.lo -MD -MP -MF .deps/bit_slicer.Tpo -c bit_slicer.c  -DDLL_EXPORT -DPIC -o .libs/bit_slicer.o
In file included from decoder.h:36:0,
                 from sampling_par.h:27,
                 from bit_slicer.h:27,
                 from bit_slicer.c:29:
e:\mingw\include\pthread.h:320:8: error: redefinition of 'struct timespec'
 struct timespec {
        ^
In file included from e:\mingw\include\time.h:53:0,
                 from e:\mingw\include\pthread.h:219,
                 from decoder.h:36,
                 from sampling_par.h:27,
                 from bit_slicer.h:27,
                 from bit_slicer.c:29:
e:\mingw\include\parts\time.h:105:8: note: originally defined here
 struct timespec
        ^
make[4]: *** [bit_slicer.lo] Error 1
```

根据e:\mingw\include\pthread.h里面的相关定义，编译时加上HAVE_STRUCT_TIMESPEC宏定义，编译命令为：

```bash
make CFLAGS="-DHAVE_STRUCT_TIMESPEC -g -O2"
```

## 生成dll

将zvbi编译成dll，在编译完.a文件后，直接执行：

```bash
gcc --share -o .libs/libzvbi.dll  -L /win32/lib  bit_slicer.o cache.o caption.o cc608_decoder.o \
conv.o dvb_mux.o dvb_demux.o event.o exp-html.o exp-templ.o exp-txt.o exp-vtx.o export.o hamm.o \
idl_demux.o lang.o misc.o packet.o teletext.o packet-830.o page_table.o pdc.o pfc_demux.o \
proxy-client.o raw_decoder.o sampling_par.o search.o ure.o sliced_filter.o tables.o trigger.o \
vbi.o vps.o wss.o xds_demux.o  .libs/libzvbiinline_la-decoder.o .libs/libzvbiinline_la-exp-gfx.o \
-lintl -lpthread  -lm -liconv  -lintl -lpthread  -lm -liconv -lgettextlib.dll -lgnutls
```







