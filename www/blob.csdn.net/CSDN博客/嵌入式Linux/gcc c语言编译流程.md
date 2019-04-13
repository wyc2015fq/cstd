
# gcc c语言编译流程 - 嵌入式Linux - CSDN博客

2018年09月13日 12:13:50[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：253



# 1前言
最近群里讨论个C语言的小程序，看起来都不是很难，但是大家对答案有争论，所以想讨论编译原理，做嵌入式要对编译原理有一定的了解，所以转了这篇文章。
我们之前讨论的问题如下代码
`#include 
#include 
#define WEIQIFA 0;
int main(void) 
{        
    int i = WEIQIFA;        
    i = i++;
    i++;        
    printf("%d\n",i);
    return 0;
}`
原来是没有那个宏WEIQIFA的，但是我为了举例编译原理，特意加上去，编译的第一步就是做宏替换
预编译后变成下面这样
`int main(void)
{
    int i = 0;;
    i = i++;
    i++;
    printf("%d\n",i);
    return 0;
}`用g++ -g -Wstrict-prototypes -Wall -Wunused -o test test001.c 编译
然后用objdump -j .text -Sl test | more 查看代码可以看到汇编代码如下
`main():
/data/weiqifa/c/bianyiyuanli/test001.c:7
#include 
#define WEIQIFA 0;
int main(void) 
{
  400526:       55                      push   %rbp
  400527:       48 89 e5                mov    %rsp,%rbp
  40052a:       48 83 ec 10             sub    $0x10,%rsp
/data/weiqifa/c/bianyiyuanli/test001.c:9
    int i = WEIQIFA;
  40052e:       c7 45 fc 00 00 00 00    movl   $0x0,-0x4(%rbp)
/data/weiqifa/c/bianyiyuanli/test001.c:11
    i = i++;
  400535:       8b 45 fc                mov    -0x4(%rbp),%eax
  400538:       8d 50 01                lea    0x1(%rax),%edx
  40053b:       89 55 fc                mov    %edx,-0x4(%rbp)
  40053e:       89 45 fc                mov    %eax,-0x4(%rbp)
/data/weiqifa/c/bianyiyuanli/test001.c:12
    i++;
  400541:       83 45 fc 01             addl   $0x1,-0x4(%rbp)
/data/weiqifa/c/bianyiyuanli/test001.c:14
    printf("%d\n",i);
  400545:       8b 45 fc                mov    -0x4(%rbp),%eax
  400548:       89 c6                   mov    %eax,%esi
  40054a:       bf e4 05 40 00          mov    $0x4005e4,%edi
  40054f:       b8 00 00 00 00          mov    $0x0,%eax
  400554:       e8 a7 fe ff ff          callq  400400 <printf@plt>
/data/weiqifa/c/bianyiyuanli/test001.c:16
    return 0;
  400559:       b8 00 00 00 00          mov    $0x0,%eax
/data/weiqifa/c/bianyiyuanli/test001.c:17
}`当时大家讨论很激烈

![](https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPds8e0aicicYI4IlZVKzWiaHmAXX6GV6P0LnCicY7sxHEcrpctzLyYVNLmcZkOu5t71sveJlLImLpJZ5g/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)
![](https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPds8e0aicicYI4IlZVKzWiaHmAyJhmOWSKyvWD7mDACddNVNeHvalyymLDs2jpnQjtEC01vvF0MOlhYQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

# 2 正文
![](https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPciaCQX5E8Kgxicc8HQ16pgKFp4ccWCQGGjvVJN2icibOAqibL2JQBypheSoVuQUiae8cBiaWKY9nehVMdTg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)
大家肯定都知道计算机程序设计语言通常分为机器语言、汇编语言和高级语言三类。高级语言需要通过翻译成机器语言才能执行，而翻译的方式分为两种，一种是编译型，另一种是解释型，因此我们基本上将高级语言分为两大类，一种是编译型语言，例如C，C++，Java，另一种是解释型语言，例如Python、Ruby、MATLAB 、JavaScript。
本文将介绍如何将高层的C/C++语言编写的程序转换成为处理器能够执行的二进制代码的过程，包括四个步骤：
预处理（Preprocessing）
编译（Compilation）
汇编（Assembly）
链接（Linking）

![](https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPciaCQX5E8Kgxicc8HQ16pgKFtpBAmELSKlR4bp0dLv5ibdhY2Znvqf9ib6pzArL1tuFgv9LYD0jpiajug/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

**1、GCC 工具链介绍**
GCC（GNU Compiler Collection，GNU编译器套件），是由 GNU 开发的编程语言编译器。它是以GPL许可证所发行的自由软件，也是 GNU计划的关键部分。GCC原本作为GNU操作系统的官方编译器，现已被大多数类Unix操作系统（如Linux、BSD、Mac OS X等）采纳为标准的编译器，GCC同样适用于微软的Windows。GCC是自由软件过程发展中的著名例子，由自由软件基金会以GPL协议发布。
**C运行库**
C语言标准主要由两部分组成：一部分描述C的语法，另一部分描述C标准库。C标准库定义了一组标准头文件，每个头文件中包含一些相关的函数、变量、类型声明和宏定义，譬如常见的printf函数便是一个C标准库函数，其原型定义在stdio头文件中。
C语言标准仅仅定义了C标准库函数原型，并没有提供实现。因此，C语言编译器通常需要一个C运行时库（C Run Time Libray，CRT）的支持。C运行时库又常简称为C运行库。与C语言类似，C++也定义了自己的标准，同时提供相关支持库，称为C++运行时库。
**2、举个例子**
由于GCC工具链主要是在Linux环境中进行使用，因此本文也将以Linux系统作为工作环境。为了能够演示编译的整个过程，本节先准备一个C语言编写的简单Hello程序作为示例，其源代码如下所示：
`#include`
`#include`
`#define WEIQIFA 0;`
`int main(void)`
`{`
`int i = WEIQIFA;`
`i = i++;`
`i++;`
`printf("%d\n",i);`
`return 0;`
`}`

**3、编译过程**
**3.1 预处理**
预处理的过程主要包括以下过程：
1、将所有的\#define删除，并且展开所有的宏定义，并且处理所有的条件预编译指令，比如\#if \#ifdef \#elif \#else \#endif等。
2、处理\#include预编译指令，将被包含的文件插入到该预编译指令的位置。
3、删除所有注释“//”和“/* */”。
4、添加行号和文件标识，以便编译时产生调试用的行号及编译错误警告行号。
5、保留所有的\#pragma编译器指令，后续编译过程需要使用它们。
使用gcc进行预处理的命令如下：
`$ gcc -E test001.c -o test001.i // 将源文件test001.c文件预处理生成test001.i`
`// GCC的选项-E使GCC在进行完预处理后即停止`
test001.i文件可以作为普通文本文件打开进行查看，其代码片段如下所示：
`extern char *__stpncpy (char *__restrict __dest,
   const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
extern char *stpncpy (char *__restrict __dest,
        const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__ , __leaf__)) __attribute__ ((__nonnull__ (1, 2)));
# 658 "/usr/include/string.h" 3 4
# 3 "test001.c" 2

# 6 "test001.c"
int main(void)
{
    int i = 0;;
    i = i++;
    i++;
    printf("%d\n",i);
    return 0;
}`
**3.2 编译**
编译过程就是对预处理完的文件进行一系列的词法分析，语法分析，语义分析及优化后生成相应的汇编代码。
使用gcc进行编译的命令如下：
`$ gcc -S test001.i -o test001.s // 将预处理生成的test001.i文件编译生成汇编程序test001.s`
`// GCC的选项-S使GCC在执行完编译后停止，生成汇编程序`
上述命令生成的汇编程序test001.s的代码片段如下所示，其全部为汇编代码。
`weiqifa@ubuntu:~/c/bianyiyuanli$ cat test001.s
        .file   "test001.c"
        .section        .rodata
.LC0:
        .string "%d\n"
        .text
        .globl  main
        .type   main, @function
main:
.LFB0:
        .cfi_startproc
        pushq   %rbp
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
        movq    %rsp, %rbp
        .cfi_def_cfa_register 6
        subq    $16, %rsp
        movl    $0, -4(%rbp)
        movl    -4(%rbp), %eax
        leal    1(%rax), %edx
        movl    %edx, -4(%rbp)
        movl    %eax, -4(%rbp)
        addl    $1, -4(%rbp)
        movl    -4(%rbp), %eax
        movl    %eax, %esi
        movl    $.LC0, %edi
        movl    $0, %eax
        call    printf
        movl    $0, %eax
        leave
        .cfi_def_cfa 7, 8
        ret
        .cfi_endproc
.LFE0:
        .size   main, .-main
        .ident  "GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
        .section        .note.GNU-stack,"",@progbits
weiqifa@ubuntu:~/c/bianyiyuanli$`
**3.3 汇编**
汇编过程调用对汇编代码进行处理，生成处理器能识别的指令，保存在后缀为.o的目标文件中。由于每一个汇编语句几乎都对应一条处理器指令，因此，汇编相对于编译过程比较简单，通过调用Binutils中的汇编器as根据汇编指令和处理器指令的对照表一一翻译即可。
当程序由多个源代码文件构成时，每个文件都要先完成汇编工作，生成.o目标文件后，才能进入下一步的链接工作。注意：目标文件已经是最终程序的某一部分了，但是在链接之前还不能执行。
使用gcc进行汇编的命令如下：
`$ gcc -c test001.s -o test001.o // 将编译生成的test001.s文件汇编生成目标文件test001.o`
`// GCC的选项-c使GCC在执行完汇编后停止，生成目标文件`
`//或者直接调用as进行汇编`
`$ as -c test001.s -o test001.o //使用as将hello.s文件汇编生成目标文件`
注意：test001.o目标文件为ELF（Executable and Linkable Format）格式的可重定向文件。
**3.4 链接**
链接也分为静态链接和动态链接，其要点如下：
1、静态链接是指在编译阶段直接把静态库加入到可执行文件中去，这样可执行文件会比较大。链接器将函数的代码从其所在地（不同的目标文件或静态链接库中）拷贝到最终的可执行程序中。为创建可执行文件，链接器必须要完成的主要任务是：符号解析（把目标文件中符号的定义和引用联系起来）和重定位（把符号定义和内存地址对应起来然后修改所有对符号的引用）。
2、动态链接则是指链接阶段仅仅只加入一些描述信息，而程序执行时再从系统中把相应动态库加载到内存中去。
2.1、在Linux系统中，gcc编译链接时的动态库搜索路径的顺序通常为：首先从gcc命令的参数-L指定的路径寻找；再从环境变量LIBRARY_PATH指定的路径寻址；再从默认路径/lib、/usr/lib、/usr/local/lib寻找。
2.2、在Linux系统中，执行二进制文件时的动态库搜索路径的顺序通常为：首先搜索编译目标代码时指定的动态库搜索路径；再从环境变量LD_LIBRARY_PATH指定的路径寻址；再从配置文件/etc/ld.so.conf中指定的动态库搜索路径；再从默认路径/lib、/usr/lib寻找。
3、在Linux系统中，可以用ldd命令查看一个可执行程序依赖的共享库。
由于链接动态库和静态库的路径可能有重合，所以如果在路径中有同名的静态库文件和动态库文件，比如libtest.a和libtest.so，gcc链接时默认优先选择动态库，会链接libtest.so，如果要让gcc选择链接libtest.a则可以指定gcc选项-static，该选项会强制使用静态库进行链接。以Hello World为例：4、如果使用命令“gcc hello.c -o hello”则会使用动态库进行链接，生成的ELF可执行文件的大小（使用size命令查看）和链接的动态库（使用ldd命令查看）如下所示：
`weiqifa@ubuntu:~/c/bianyiyuanli$ gcc test001.c -o test001
weiqifa@ubuntu:~/c/bianyiyuanli$ size test001
   text    data     bss     dec     hex filename
   1208     552       8    1768     6e8 test001
weiqifa@ubuntu:~/c/bianyiyuanli$ ldd test001//可以看出该可执行文件链接了很多其他动态库，主要是Linux的glibc动态库
        linux-vdso.so.1 =>  (0x00007ffe90dbd000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f930be92000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f930c25c000)
weiqifa@ubuntu:~/c/bianyiyuanli$`5、如果使用命令“gcc -static hello.c -o hello”则会使用静态库进行链接，生成的ELF可执行文件的大小（使用Binutils的size命令查看）和链接的动态库（使用Binutils的ldd命令查看）如下所示：
`$ gcc -static test001.c -o test001`
`$ size test001 //使用size查看大小`
`text    data     bss     dec     hex filename`
`823382    7284    6360  837026   cc5a2 test001 //可以看出text的代码尺寸变得极大`
`$ ldd test001`
`not a dynamic executable //说明没有链接动态库`
链接器链接后生成的最终文件为ELF格式可执行文件，一个ELF可执行文件通常被链接为不同的段，常见的段譬如.text、.data、.rodata、.bss等段。
**4、ELF文件分析**
**4.1 ELF文件的段**
ELF文件格式如下图所示，位于ELF Header和Section Header Table之间的都是段（Section）。一个典型的ELF文件包含下面几个段：
text：已编译程序的指令代码段。
rodata：ro代表read only，即只读数据（譬如常数const）。
data：已初始化的C程序全局变量和静态局部变量。
bss：未初始化的C程序全局变量和静态局部变量。
debug：调试符号表，调试器用此段的信息帮助调试。

如果对ELF文件特别感兴趣的，可以看看这个网站讲的非常详细非常好
![](https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPfd4u0AFqrWN56RY0r88hhIjTU8kP1d0ibeZkavaiafza6mboBd7kiaricMXGibz5hZxQ0yf0luUphbfLA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)
可以使用readelf -S查看其各个section的信息如下：
`weiqifa@ubuntu:~/c/bianyiyuanli$ readelf -S test001
There are 33 section headers, starting at offset 0xde500:
Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .note.ABI-tag     NOTE             0000000000400190  00000190
       0000000000000020  0000000000000000   A       0     0     4
  [ 2] .note.gnu.build-i NOTE             00000000004001b0  000001b0
       0000000000000024  0000000000000000   A       0     0     4
  [ 3] .rela.plt         RELA             00000000004001d8  000001d8
       00000000000000f0  0000000000000018  AI       0    24     8
  [ 4] .init             PROGBITS         00000000004002c8  000002c8
       000000000000001a  0000000000000000  AX       0     0     4
  [ 5] .plt              PROGBITS         00000000004002f0  000002f0
       00000000000000a0  0000000000000000  AX       0     0     16
  [ 6] .text             PROGBITS         0000000000400390  00000390
       000000000009e5b4  0000000000000000  AX       0     0     16
  [ 7] __libc_freeres_fn PROGBITS         000000000049e950  0009e950
       0000000000002529  0000000000000000  AX       0     0     16
  [ 8] __libc_thread_fre PROGBITS         00000000004a0e80  000a0e80
       00000000000000de  0000000000000000  AX       0     0     16
  [ 9] .fini             PROGBITS         00000000004a0f60  000a0f60
       0000000000000009  0000000000000000  AX       0     0     4
  [10] .rodata           PROGBITS         00000000004a0f80  000a0f80
       000000000001d244  0000000000000000   A       0     0     32
  [11] __libc_subfreeres PROGBITS         00000000004be1c8  000be1c8
       0000000000000050  0000000000000000   A       0     0     8
  [12] __libc_atexit     PROGBITS         00000000004be218  000be218
       0000000000000008  0000000000000000   A       0     0     8
  [13] .stapsdt.base     PROGBITS         00000000004be220  000be220
       0000000000000001  0000000000000000   A       0     0     1
  [14] __libc_thread_sub PROGBITS         00000000004be228  000be228
       0000000000000008  0000000000000000   A       0     0     8
  [15] .eh_frame         PROGBITS         00000000004be230  000be230
       000000000000af5c  0000000000000000   A       0     0     8
  [16] .gcc_except_table PROGBITS         00000000004c918c  000c918c
       00000000000000a3  0000000000000000   A       0     0     1
  [17] .tdata            PROGBITS         00000000006c9eb8  000c9eb8
       0000000000000020  0000000000000000 WAT       0     0     8
  [18] .tbss             NOBITS           00000000006c9ed8  000c9ed8
       0000000000000030  0000000000000000 WAT       0     0     8
  [19] .init_array       INIT_ARRAY       00000000006c9ed8  000c9ed8
       0000000000000010  0000000000000000  WA       0     0     8
  [20] .fini_array       FINI_ARRAY       00000000006c9ee8  000c9ee8
       0000000000000010  0000000000000000  WA       0     0     8
  [21] .jcr              PROGBITS         00000000006c9ef8  000c9ef8
       0000000000000008  0000000000000000  WA       0     0     8
  [22] .data.rel.ro      PROGBITS         00000000006c9f00  000c9f00
       00000000000000e4  0000000000000000  WA       0     0     32
  [23] .got              PROGBITS         00000000006c9fe8  000c9fe8
       0000000000000010  0000000000000008  WA       0     0     8
  [24] .got.plt          PROGBITS         00000000006ca000  000ca000
       0000000000000068  0000000000000008  WA       0     0     8
  [25] .data             PROGBITS         00000000006ca080  000ca080
       0000000000001ad0  0000000000000000  WA       0     0     32
  [26] .bss              NOBITS           00000000006cbb60  000cbb50
       0000000000001878  0000000000000000  WA       0     0     32
  [27] __libc_freeres_pt NOBITS           00000000006cd3d8  000cbb50
       0000000000000030  0000000000000000  WA       0     0     8
  [28] .comment          PROGBITS         0000000000000000  000cbb50
       0000000000000035  0000000000000001  MS       0     0     1
  [29] .note.stapsdt     NOTE             0000000000000000  000cbb88
       0000000000000f18  0000000000000000           0     0     4
  [30] .shstrtab         STRTAB           0000000000000000  000de392
       0000000000000169  0000000000000000           0     0     1
  [31] .symtab           SYMTAB           0000000000000000  000ccaa0
       000000000000b0a0  0000000000000018          32   710     8
  [32] .strtab           STRTAB           0000000000000000  000d7b40
       0000000000006852  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)
weiqifa@ubuntu:~/c/bianyiyuanli$`
**4.2 反汇编ELF**
由于ELF文件无法被当做普通文本文件打开，如果希望直接查看一个ELF文件包含的指令和数据，需要使用反汇编的方法。
使用objdump -D对其进行反汇编如下：
`$ objdump -D test001
 eeb:   00 6c 69 62             add    %ch,0x62(%rcx,%rbp,2)
 eef:   63 00                   movslq (%rax),%eax
 ef1:   6c                      insb   (%dx),%es:(%rdi)
 ef2:   6f                      outsl  %ds:(%rsi),(%dx)
 ef3:   6e                      outsb  %ds:(%rsi),(%dx)
 ef4:   67 6a 6d                addr32 pushq $0x6d
 ef7:   70 5f                   jo     f58 <data.8386+0xf10>
 ef9:   74 61                   je     f5c <data.8386+0xf14>
 efb:   72 67                   jb     f64 <data.8386+0xf1c>
 efd:   65 74 00                gs je  f00 <data.8386+0xeb8>
 f00:   38 40 25                cmp    %al,0x25(%rax)
 f03:   72 64                   jb     f69 <data.8386+0xf21>
 f05:   69 20 2d 34 40 25       imul   $0x2540342d,(%rax),%esp
 f0b:   65 61                   gs (bad) 
 f0d:   78 20                   js     f2f <data.8386+0xee7>
 f0f:   38 40 25                cmp    %al,0x25(%rax)
 f12:   72 64                   jb     f78 <data.8386+0xf30>
 f14:   78 00                   js     f16 <data.8386+0xece>`
使用objdump -S将其反汇编并且将其C语言源代码混合显示出来：
`weiqifa@ubuntu:~/c/bianyiyuanli$ gcc -o test001 -g test001.c 
weiqifa@ubuntu:~/c/bianyiyuanli$ objdump -S test001          
test001:     file format elf64-x86-64
0000000000400526`
```python
:
#include 
#define WEIQIFA 0;
int main(void) 
{
  400526:       55                      push   %rbp
  400527:       48 89 e5                mov    %rsp,%rbp
  40052a:       48 83 ec 10             sub    $0x10,%rsp
    int i = WEIQIFA;
  40052e:       c7 45 fc 00 00 00 00    movl   $0x0,-0x4(%rbp)
    i = i++;
  400535:       8b 45 fc                mov    -0x4(%rbp),%eax
  400538:       8d 50 01                lea    0x1(%rax),%edx
  40053b:       89 55 fc                mov    %edx,-0x4(%rbp)
  40053e:       89 45 fc                mov    %eax,-0x4(%rbp)
    i++;
  400541:       83 45 fc 01             addl   $0x1,-0x4(%rbp)
    printf("%d\n",i);
  400545:       8b 45 fc                mov    -0x4(%rbp),%eax
  400548:       89 c6                   mov    %eax,%esi
  40054a:       bf e4 05 40 00          mov    $0x4005e4,%edi
  40054f:       b8 00 00 00 00          mov    $0x0,%eax
  400554:       e8 a7 fe ff ff          callq  400400 <printf@plt>
    return 0;
  400559:       b8 00 00 00 00          mov    $0x0,%eax
}
  40055e:       c9                      leaveq 
  40055f:       c3                      retq   
0000000000400560 <__libc_csu_init>:
  400560:       41 57                   push   %r15
  400562:       41 56                   push   %r14
  400564:       41 89 ff                mov    %edi,%r15d
  400567:       41 55                   push   %r13
  400569:       41 54                   push   %r12
  40056b:       4c 8d 25 9e 08 20 00    lea    0x20089e(%rip),%r12        # 600e10 <__frame_dummy_init_array_entry>
  400572:       55                      push   %rbp
  400573:       48 8d 2d 9e 08 20 00    lea    0x20089e(%rip),%rbp        # 600e18 <__init_array_end>
  40057a:       53                      push   %rbx
  40057b:       49 89 f6                mov    %rsi,%r14
  40057e:       49 89 d5                mov    %rdx,%r13
  400581:       4c 29 e5                sub    %r12,%rbp
  400584:       48 83 ec 08             sub    $0x8,%rsp
  400588:       48 c1 fd 03             sar    $0x3,%rbp
  40058c:       e8 37 fe ff ff          callq  4003c8 <_init>
  400591:       48 85 ed                test   %rbp,%rbp
  400594:       74 20                   je     4005b6 <__libc_csu_init+0x56>
  400596:       31 db                   xor    %ebx,%ebx
  400598:       0f 1f 84 00 00 00 00    nopl   0x0(%rax,%rax,1)
  40059f:       00 
  4005a0:       4c 89 ea                mov    %r13,%rdx
  4005a3:       4c 89 f6                mov    %r14,%rsi
  4005a6:       44 89 ff                mov    %r15d,%edi
  4005a9:       41 ff 14 dc             callq  *(%r12,%rbx,8)
  4005ad:       48 83 c3 01             add    $0x1,%rbx
  4005b1:       48 39 eb                cmp    %rbp,%rbx
  4005b4:       75 ea                   jne    4005a0 <__libc_csu_init+0x40>
  4005b6:       48 83 c4 08             add    $0x8,%rsp
  4005ba:       5b                      pop    %rbx
  4005bb:       5d                      pop    %rbp
  4005bc:       41 5c                   pop    %r12
  4005be:       41 5d                   pop    %r13
  4005c0:       41 5e                   pop    %r14
  4005c2:       41 5f                   pop    %r15
  4005c4:       c3                      retq   
  4005c5:       90                      nop
  4005c6:       66 2e 0f 1f 84 00 00    nopw   %cs:0x0(%rax,%rax,1)
  4005cd:       00 00 00 
00000000004005d0 <__libc_csu_fini>:
  4005d0:       f3 c3                   repz retq 
Disassembly of section .fini:
00000000004005d4 <_fini>:
  4005d4:       48 83 ec 08             sub    $0x8,%rsp
  4005d8:       48 83 c4 08             add    $0x8,%rsp
  4005dc:       c3                      retq   
weiqifa@ubuntu:~/c/bianyiyuanli$
```
​
喜欢可以关注微信公众号：嵌入式Linux


