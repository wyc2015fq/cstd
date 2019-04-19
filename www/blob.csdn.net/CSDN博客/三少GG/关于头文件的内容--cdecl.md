# 关于头文件的内容--cdecl - 三少GG - CSDN博客
2010年12月11日 17:32:00[三少GG](https://me.csdn.net/scut1135)阅读数：1252
其他回答    共 1 条
动态存储分配函数头文件,当对内存区进行操作是,调用相关函数.ANSI标准建议使用stdlib.h头文件,但许多C编译要求用malloc.h,使用时应查阅有关手册. 
===============================================================================
/*      alloc.h
     memory management functions and variables.
     Copyright (c) Borland International 1987,1988
     All Rights Reserved.
*/
#if __STDC__             ;标准C
#define _Cdecl           ;定义C函数声明
#else                
#define _Cdecl      cdecl  
#endif
#ifndef _STDDEF
#define _STDDEF           ;标准定义
#ifndef _PTRDIFF_T        
#define _PTRDIFF_T         ;指针差
#if defined(__LARGE__) || defined(__HUGE__) || defined(__COMPACT__)
                    ;根据内存模式定义指针类型
                    ;实模式下IA-32有6种程序内存模式
                    ;分别是巨(__HUGE__) ,大(__LARGE__),中(__MEDIUM__)
                    ;     小(__SMALL__),微(__TINY__) ,压缩(__COMPACT__)
                    ;对于__HUGE__,__LARGE__,__MEDIUM__ 定义长指针32位
                    ;这是因为这3种类型数据可以存放在多个段
typedef long      ptrdiff_t;
#else
typedef int      ptrdiff_t;
                    ;定义成短指针,因为数据存在一个段
#endif
#endif
#ifndef _SIZE_T
#define _SIZE_T           ;定义偏移类型,无符号int
typedef unsigned size_t;
#endif
#endif
#ifndef NULL             ;定义NULL
#if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#define NULL      0
#else
#define NULL      0L
#endif
#endif
int      _Cdecl brk      (void *addr);
                    ;扩大程序可用数据范围,具体用法不明
                    ;指针addr之前为进程可用范围
                    ;之后为不可用范围(包括指针所指位置)
void      *_Cdecl calloc      (size_t nitems, size_t size);
                    ;分配数组空间,nitems为数组元素个数,size为元素大小
#if defined(__COMPACT__) || defined(__LARGE__) || defined(__HUGE__)
unsigned long _Cdecl coreleft      (void);
#else                 ;计算剩余可分配空间,具体用法不明  
unsigned _Cdecl coreleft      (void);
#endif
void       _Cdecl free      (void *block);
                    ;释放 malloc 分配的内存
void      *_Cdecl malloc      (size_t size);
                    ;分配连续内存空间,以八字节对齐,另多分配4个字节
void      *_Cdecl realloc      (void *block, size_t size);
                    ;重新分配内存
void      *_Cdecl sbrk      (int incr);
                    ;增加进程可用空间,与brk()相对应
#if !__STDC__             ;以上内存分配函数的32位形式
void far * _Cdecl farcalloc      (unsigned long nunits, unsigned long unitsz);
unsigned long _Cdecl farcoreleft(void);
void _Cdecl     farfree      (void far *block);
void far *_Cdecl farmalloc      (unsigned long nbytes);
void far *_Cdecl farrealloc      (void far *oldblock, unsigned long nbytes);
#endif==============================================================================问：我知道头文件由三部分内容组成：
（1）头文件开头处的版权和版本声明。
（2）预处理块。
（3）函数和类结构声明等。
下面是我引用的turboc2.0的头文件。第一部分我已清楚。第二部分和第三部分混在一起了，其中有些部分和标号我不太清楚：
如：
#if __STDC__
#define _Cdecl
#else
#define _Cdecl        cdecl
#endif
这部分是干什么用的？
还有
int        _Cdecl brk        (void *addr);
void        *_Cdecl calloc        (size_t nitems, size_t size);
中的　　_Cdecl　和　*_Cdecl是干什么用的？
下面是引用的头文件。
*******第一部分********
/*        alloc.h
        memory management functions and variables.
        Copyright (c) Borland International 1987,1988
        All Rights Reserved.
*/
*******第二部分和第三部分********
#if __STDC__
#define _Cdecl
#else
#define _Cdecl        cdecl
#endif
#ifndef _STDDEF
#define _STDDEF
#ifndef _PTRDIFF_T
#define _PTRDIFF_T
#if        defined(__LARGE__) || defined(__HUGE__) || defined(__COMPACT__)
typedef long        ptrdiff_t;
#else
typedef int        ptrdiff_t;
#endif
#endif
#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned size_t;
#endif
#endif
#ifndef NULL
#if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#define NULL        0
#else
#define NULL        0L
#endif
#endif
int        _Cdecl brk        (void *addr);
void        *_Cdecl calloc        (size_t nitems, size_t size);
#if defined(__COMPACT__) || defined(__LARGE__) || defined(__HUGE__)
unsigned long _Cdecl coreleft        (void);
#else
unsigned _Cdecl coreleft        (void);
#endif
void         _Cdecl free        (void *block);
void        *_Cdecl malloc        (size_t size);
void        *_Cdecl realloc        (void *block, size_t size);
void        *_Cdecl sbrk        (int incr);
#if !__STDC__
void far  * _Cdecl farcalloc        (unsigned long nunits, unsigned long unitsz);
unsigned long _Cdecl farcoreleft(void);
void _Cdecl      farfree        (void far *block);
void far  *_Cdecl farmalloc        (unsigned long nbytes);
void far  *_Cdecl farrealloc        (void far *oldblock, unsigned long nbytes);
#endif答：**关于头文件的内容**
告诉编译器后面的函数或变量的名称变换方式。
编译后生成的obj文件中你的变量和函数名称实际上和你输入的
不一样C中要在你的函数前加上个下划线。如果你想知道详细的
情况请提出来我给你解释。
Cdecl 是老的标记，新的是cdecl。
表明后面的名称使用规则，大小写敏感前面加下划线。
borland C++中的完整解释如下。
-----------------------------------------------------------------------------
Declares as C-style
Syntax:
    cdecl <data definition>; ;
    cdecl <function definition>; ;
    _cdecl <data definition>; ;
    _cdecl <function definition>; ;
    __cdecl <data definition>; ;
    __cdecl <function definition>; ;
Declares a variable or a function using a C-style naming convention
(case-sensitive, with a leading underscore appended).
In addition, cdecl declares C-style parameter passing conventions when
applied to a function header (last parameter pushed first, and the caller
cleans up the stack).
  Example:
   int cdecl FileCount;
   far cdecl HisFunc(int x);
