# windows.h 详解 - 心纯净，行致远 - CSDN博客





2018年05月20日 08:36:45[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：5214








参考[windows.h解构](http://blog.csdn.net/fengningning/article/details/2306650?locationNum=1&fps=1)

刚开头的一段注释是对该头文件的描述：

```
/*++ BUILD Version: 0001    Increment this if a change has global effects

Copyright (c) 1985-1997, Microsoft Corporation

Module Name:


    windows.h

Abstract:

    Master include file for Windows applications.

--*/
```

这个文件似乎没有经历过重大的变化，至今仍是第一个版本。“Increment this if a change has global effects”，意思大概是“如果某个变化有全局性的影响，就将该变化增添至这个头文件”。话本身是没有歧义的，可转而一想，换做是我，我肯定不会擅作主张来修改这个文件的——道理很简单，这样做会导致代码移植更加复杂，还有其他一些不利因素。那么，这句话大概是微软开发人员对“自己人”说的罢。而摘要部分说：“Master include file for windows applications”，就不用多做解释了。

```
#ifndef _WINDOWS_
#define _WINDOWS_
```

这种宏定义应该是最常见的了，一个作用是防止重复包含。

```
#ifndef WINVER
#define WINVER 0x0400
#else   
#if defined(_WIN32_WINNT) && (WINVER < 0x0400) && (_WIN32_WINNT > 0x0400)
#error WINVER setting conflicts with _WIN32_WINNT setting
#endif
#endif
```

WINVER这个宏与Windows版本相关，也就是该宏变量取不同值时对应不同的Windows版本，Platform SDK文档中的相关说明如下：

Windows Server 2003 WINVER>=0x0502 
Windows XP WINVER>=0x0501 
Windows 2000 WINVER>=0x0500 
Windows NT 4.0 WINVER>=0x0400 
Windows Me WINVER>=0x0500 
Windows 98 WINVER>=0x0410 
Windows 95 WINVER>=0x0400 
（如何查看自己Windows操作系统的版本号呢？下面提供其中一种方法：调出任务管理器－>帮助—>关于任务管理器，在Windows XP中如上操作可以查得版本号是5.1，而在Windows server 2003中是5.2，Windows 2000中则是5.0。哈哈，确实如此，和Platform SDK文档中的描述是一致的！）_WIN32_WINNT这个宏其实也代表版本号，因此如果你同时定义了这个宏，却又与WINVER的定义不一致，那么，编译器就提示错误“WINVER setting conflicts with _WIN32_WINNT”。

```
#if(WINVER >= 0x0500)
#pragma message ("")
#pragma message ("NOTE: WINVER has been defined as 0x0500 or greater which enables")
#pragma message ("Windows NT 5.0 and Windows 98 features. When these headers were released,")
#pragma message ("Windows NT 5.0 beta 1 and Windows 98 beta 2.1 were the current versions.")
#pragma message ("")
#pragma message ("For this release when WINVER is defined as 0x0500 or greater, you can only")
#pragma message ("build beta or test applications. To build a retail application,")
#pragma message ("set WINVER to 0x0400 or visit http://www.microsoft.com/msdn/sdk")
#pragma message ("to see if retail Windows NT 5.0 or Windows 98 headers are available.")
#pragma message ("")
#pragma message ("See the SDK release notes for more information.")
#pragma message ("")
#endif
```

如果定义的WINVER>=0x0500，即要求最低的Windows版本是Windows NT 5.0（Windows 2000）和Windows 98，此时编译器在进行编译时会提示以#pragma message定义的一系列信息作为提示。

```
#ifndef _INC_WINDOWS
#define _INC_WINDOWS

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* If defined, the following flags inhibit definition
 *     of the indicated items.
 *
 * NOGDICAPMASKS     - CC_*, LC_*, PC_*, CP_*, TC_*, RC_
 * NOVIRTUALKEYCODES - VK_*
 * NOWINMESSAGES     - WM_*, EM_*, LB_*, CB_*
 *  NOWINSTYLES       - WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
 * NOSYSMETRICS      - SM_*
 * NOMENUS           - MF_*
 * NOICONS           - IDI_*
 * NOKEYSTATES       - MK_*
 * NOSYSCOMMANDS     - SC_*
 * NORASTEROPS       - Binary and Tertiary raster ops
 * NOSHOWWINDOW      - SW_*
 * OEMRESOURCE       - OEM Resource values
 * NOATOM            - Atom Manager routines
 * NOCLIPBOARD       - Clipboard routines
 * NOCOLOR           - Screen colors
 * NOCTLMGR          - Control and Dialog routines
 * NODRAWTEXT        - DrawText() and DT_*
 * NOGDI             - All GDI defines and routines
 * NOKERNEL          - All KERNEL defines and routines
 * NOUSER            - All USER defines and routines
 * NONLS             - All NLS defines and routines
 * NOMB              - MB_* and MessageBox()
 * NOMEMMGR          - GMEM_*, LMEM_*, GHND, LHND, associated routines
 * NOMETAFILE        - typedef METAFILEPICT
 * NOMINMAX          - Macros min(a,b) and max(a,b)
 * NOMSG             - typedef MSG and associated routines
 * NOOPENFILE        - OpenFile(), OemToAnsi, AnsiToOem, and OF_*
 * NOSCROLL          - SB_* and scrolling routines
 * NOSERVICE         - All Service Controller routines, SERVICE_ equates, etc.
 * NOSOUND           - Sound driver routines
 * NOTEXTMETRIC      - typedef TEXTMETRIC and associated routines
 * NOWH              - SetWindowsHook and WH_*
 * NOWINOFFSETS      - GWL_*, GCL_*, associated routines
 * NOCOMM            - COMM driver routines
 * NOKANJI           - Kanji support stuff.
 * NOHELP            - Help engine interface.
 * NOPROFILER        - Profiler interface.
 * NODEFERWINDOWPOS - DeferWindowPos routines
 * NOMCX             - Modem Configuration Extensions
 */
```

接下来的所有内容都是用来定义另外一些需要包含的头文件的，当然也包含了其他信息。 
_MSC_VER这个宏定义了编译器的版本，相关信息如下： 
C Compiler version 6.0 600 
C/C++ compiler version 7.0 700 
Visual C++, Windows, version 1.0 800 
Visual C++, 32-bit, version 1.0 800 
Visual C++, Windows, version 2.0 900 
Visual C++, 32-bit, version 2.x 900 
Visual C++, 32-bit, version 4.0 1000 
Visual C++, 32-bit, version 5.0 1100 
Visual C++, 32-bit, version 6.0 1200 
这个宏是必须定义的，#pragma once指示编译器在编译过程中最多包含一次该头文件。 
有意思的是下面一大段注释，说明了当定义了_MSC_VER这个宏，并且它的版本号>=1020，那么接下来所列出的一系列标志是不能够被定义的。

```
#if defined(RC_INVOKED) && !defined(NOWINRES)

#include <winresrc.h>

#else

#if defined(RC_INVOKED)
/* Turn off a bunch of stuff to ensure that RC files compile OK. */
#define NOATOM
#define NOGDI
#define NOGDICAPMASKS
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NORASTEROPS
#define NOSCROLL
#define NOSOUND
#define NOSYSMETRICS
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOCRYPT
#define NOMCX
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_PPC_) && !defined(_ALPHA_) && !defined(_MIPS_) && !defined(_X86_) && defined(_M_IX86)
#define _X86_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_PPC_) && !defined(_ALPHA_) && !defined(_X86_) && !defined(_MIPS_) && defined(_M_MRX000)
#define _MIPS_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_PPC_) && !defined(_ALPHA_) && !defined(_X86_) && !defined(_MIPS_) && defined(_M_ALPHA)
#define _ALPHA_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_PPC_) && !defined(_ALPHA_) && !defined(_X86_) && !defined(_MIPS_) && defined(_M_PPC)
#define _PPC_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_PPC_) && !defined(_ALPHA_) && !defined(_X86_) && !defined(_MIPS_) && defined(_M_M68K)
#define _68K_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_PPC_) && !defined(_ALPHA_) && !defined(_X86_) && !defined(_MIPS_) && defined(_M_MPPC)
#define _MPPC_
#endif

#ifndef _MAC
#if defined(_68K_) || defined(_MPPC_)
#define _MAC
#endif
#endif

#ifndef RC_INVOKED
#if     ( _MSC_VER >= 800 )
#pragma warning(disable:4001)
#pragma warning(disable:4201)
#pragma warning(disable:4214)
#pragma warning(disable:4514)
#endif
#include <excpt.h>
#include <stdarg.h>
#endif /* RC_INVOKED */
```

这一段有些长，但因为是一体的，所以放在一起分析是必然的。 
宏RC_INVOKED似乎是这一段的核心，就从它开始。RC（资源编译器）定义了这个宏，使得你能够有选择地编译资源头文件的不同部分。为什么会有这种需要呢？当你考虑选择用C编译器还是用RC编译器来编译的资源头文件的时候，你就必须直面这个问题。因为RC所支持的定义语句只是C编译器的一个子集，因此，如果选择用RC来编译，那么就该注意不能用RC所不支持的语法来编写资源头文件。 
NO_WINRES这个宏实在winresrc.h这个头文件里定义的，而winresrc.h这个文件里的内容实在是很少：

```
#ifndef _WINRESRC_
#define _WINRESRC_

#include <winuser.rh>
#include <commctrl.rh>
#include <dde.rh>
#include <winnt.rh>
#include <dlgs.h>
#include <winver.h>

#endif
```

3.文件遍历（查找）

```
long _findfirst( char *filespec, struct _finddata_t *fileinfo );  
// 功  能 : 提供与filespec指定入口泛式匹配的第一个文件.通常后继用_findnext函  
//          数后续使用来完成某泛式下的文件遍历.  
// 头文件 : #include <io.h>  
// 参  数 : filespec - 目标文件规范,可以包含通配符  
//          fileinfo - 文件信息buffer  
// 返回值 : 成功返回唯一的搜索句柄  
//          出错返回-1,且设置errno为如下值:  
//            ENOENT 该泛式无法匹配  
//            EINVAL 无效文件名  
// 注  意 : _finddata_t 说明  

struct _finddata_t  
{  
    unsigned attrib;  
    time_t time_create;  
    time_t time_access;  
    time_t time_write;  
    _fsize_t size;  
    char name[_MAX_FNAME];  
};  
// 其中 ：  
//  unsigned atrrib :  文件属性的存储位置。它存储一个unsigned单元，用于表示文件的  
//                     属性。文件属性是用位表示的，主要有以下一些：_A_ARCH（存档）、  
//                     _A_HIDDEN（隐藏）、_A_NORMAL（正常）、_A_RDONLY（只读）、  
//                     _A_SUBDIR（文件夹）、_A_SYSTEM（系统）。这些都是在<io.h>中  
//                     定义的宏，可以直接使用，而本身的意义其实是一个无符号整型  
//                    （只不过这个整型应该是2的几次幂，从而保证只有一位为1，而其他  
//                     位为0）。既然是位表示，那么当一个文件有多个属性时，它往往是  
//                     通过位或的方式，来得到几个属性的综合。例如只读+隐藏+系统属性，  
//                     应该为：_A_HIDDEN | _A_RDONLY |_A_SYSTEM 。  
// time_t time_create：这里的time_t是一个变量类型，用来存储文件创建时间。  
// time_t time_access: 文件最后一次被访问的时间。  
// time_t time_write :  文件最后一次被修改的时间。  
// _fsize_t size     :  文件的大小。这里的_fsize_t应该可以相当于unsigned整型，表示  
//                      文件的字节数。  
// char name[_MAX_FNAME]：文件的文件名。这里的_MAX_FNAME是一个常量宏，它在<stdlib.h>头  
//                        文件中被定义，表示的是文件名的最大长度。  

int _findnext( long handle, struct _finddata_t *fileinfo );  
// 功  能 : 按照前面_findfirst中的泛式规则，查找下一个符合该泛式的文件，并以此为依据  
//          修改fileinfo中的值  
// 头文件 : #include <io.h>  
// 参  数 : long handle - 搜索句柄(通常由紧靠其前的_findfirst()返回)  
//          fileinfo    - 文件信息buffer  
// 返回值 : 成功返回0  
//          出错返回-1,且设置errno为如下值:  
//            ENOENT 没有更多的符合该泛式的文件  

int _findclose( long handle );  
// 功  能 : 关闭搜寻句柄并释放相应资源  
// 头文件 : #include <io.h>  
// 参  数 : long handle - 搜索句柄(通常由紧靠其前的_findfirst()返回)  
// 返回值 : 成功返回0  
//          出错返回-1,且设置errno为如下值:  
//            ENOENT 没有更多的符合该泛式的文件
```

4.创建目录

```
int _mkdir( const char *dirname );  
// 功  能 : 创建一个新目录,目录名为dirname.  
// 头文件 : #include <direct.h>  
// 返回值 : 成功返回0  
//          失败返回-1，且设置errno为以下三个值之一:  
//            EACCESS 权限不允许  
//            EEXIST   该目录已存在  
//            ENOENT   无该文件或目录
```

5.删除目录

```
int _rmdir( const char *dirname );  
// 功  能 : 删除名为dirname的目录.  
// 头文件 : #include <direct.h>  
// 返回值 : 成功返回0  
//          失败返回-1，且设置errno为以下三个值之一:  
//            EACCESS   : 权限不允许  
//            ENOTEMPTY : dirname不是文件夹;或者该文件夹不空;或  
//                        者dirname为当前工作文件夹;或者dirname  
//                        为当根文件夹;  
//            ENOENT    : 无该文件或目录
```

6.其他操作

```
int _access( const char *path, int mode );  
// 功  能 : 测定文件/目录存取权限.  
// 头文件 : #include <io.h>  
// 参  数 : path - 文件或者目录  
//          mode - 权限设定,其值如下:  
//                   00 Existence only   
//                   02 Write permission   
//                   04 Read permission   
//                   06 Read and write permission  

int _chdrive( int drive );  
// 功  能 : 更改当前工作驱动器.  
// 头文件 : #include <direct.h>  
// 返回值 : 成功返回0  
//          失败返回-1  
// 注  释 : 参数说明  
//            drive =1 :  A盘  
//            drive =2 :  B盘  
//           drive =3 :  C盘 ...  

char* _getdcwd( int drive, char *buffer, int maxlen );  
// 功  能 : 获得指定驱动器的当前工作路径.  
// 头文件 : #include <direct.h>  
// 返回值 : 成功返回指向buffer的pointer  
//          失败返回NULL，且设置errno为以下三个值之一:  
//            ENODEV 无该设备  
//            ENOMEM 内存不够  
//            ERANGE 结果超出范围  
// 注  意 : 当第一个参数为 NULL 时,该函数设置errno为ERANGE
```




