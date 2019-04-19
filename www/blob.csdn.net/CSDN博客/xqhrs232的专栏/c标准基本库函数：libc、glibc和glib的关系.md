# c标准基本库函数：libc、glibc和glib的关系 - xqhrs232的专栏 - CSDN博客
2017年02月21日 09:00:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：690
原文地址::[http://blog.csdn.net/xiaohaijiejie/article/details/51814664](http://blog.csdn.net/xiaohaijiejie/article/details/51814664)
【glibc 和 libc】
glibc 和 libc 都是 [Linux](http://lib.csdn.net/base/linux) 下的
 C 函数库。
libc 是 Linux 下的 ANSI C 函数库；glibc 是 Linux 下的 GUN C 函数库。
ANSI C 和 GNU C 有什么区别呢？
       ANSI C 函数库是基本的 C 语言函数库，包含了 C 语言最基本的库函数。这个库可以根据头文件划分为 15 个部分，其中包括：
- <ctype.h>：包含用来测试某个特征字符的函数的函数原型，以及用来转换大小写字母的函数原型；
- <errno.h>：定义用来报告错误条件的宏；
- <float.h>：包含系统的浮点数大小限制；
- <math.h>：包含数学库函数的函数原型；
- <stddef.h>：包含执行某些计算 C 所用的常见的函数定义；
- <stdio.h>：包含标准输入输出库函数的函数原型，以及他们所用的信息；
- <stdlib.h>：包含数字转换到文本，以及文本转换到数字的函数原型，还有内存分配、随机数字以及其他实用函数的函数原型；
- <string.h>：包含字符串处理函数的函数原型；
- <time.h>：包含时间和日期操作的函数原型和类型；
- <stdarg.h>：包含函数原型和宏，用于处理未知数值和类型的函数的参数列表；
- <signal.h>：包含函数原型和宏，用于处理程序执行期间可能出现的各种条件；
- <setjmp.h>：包含可以绕过一般函数调用并返回序列的函数的原型，即非局部跳转；
- <locale.h>：包含函数原型和其他信息，使程序可以针对所运行的地区进行修改。
- 地区的表示方法可以使计算机系统处理不同的数据表达约定，如全世界的日期、时间、美元数和大数字；
- <assert.h>：包含宏和信息，用于进行诊断，帮助程序调试。
上述库函数在其各种支持 C 语言的 IDE 中都是有的。
       GNU C 函数库是一种类似于第三方插件的东西。由于 Linux 是用 C 语言写的，所以 Linux 的一些操作是用 C 语言实现的，因此，GUN 组织开发了一个 C 语言的库以便让我们更好的利用
 C 语言开发基于 Linux 操作系统的程序。不过现在的不同的 Linux 的发行版本对这两个函数库有不同的处理方法，有的可能已经集成在同一个库里了。
glibc下载地址：http://mirror.hust.edu.cn/gnu/
**glibc**是linux下面c标准库的实现，即GNU C Library。glibc本身是GNU旗下的C标准库，**后来逐渐成为了Linux的标准c库，而Linux下原来的标准c库Linux libc逐渐不再被维护**。Linux下面的标准c库不仅有这一个，如uclibc（https://www.uclibc.org/）、klibc，以及上面被提到的Linux libc，但是**glibc无疑是用得最多的**。glibc在/lib目录下的.so文件为libc.so.6。
查看当前系统的 glibc 版本的两种方法：
1.
|`01`|```[root@Betty ~]``````# /lib/libc.so.6```|
|`02`|```GNU C Library stable release version 2.5, by Roland McGrath et al.```|
|`03`|```Copyright (C) 2006 Free Software Foundation, Inc.```|
|`04`|```This is````free````software; see the````source``for````copying conditions.```|
|`05`|```There is NO warranty; not even````for````MERCHANTABILITY or FITNESS FOR A```|
|`06`|```PARTICULAR PURPOSE.```|
|`07`|```Compiled by GNU CC version 4.1.2 20080704 (Red Hat 4.1.2-52).```|
|`08`|```Compiled on a Linux 2.6.9 system on 2013-01-08.```|
|`09`|```Available extensions:```|
|`10`|`        ````The C stubs add-on version 2.1.2.```|
|`11`|`        ````crypt add-on version 2.1 by Michael Glad and others```|
|`12`|`        ````GNU Libidn by Simon Josefsson```|
|`13`|`        ````GNU libio by Per Bothner```|
|`14`|`        ````NIS(YP)/NIS+ NSS modules 0.19 by Thorsten Kukuk```|
|`15`|`        ````Native POSIX Threads Library by Ulrich Drepper et al```|
|`16`|`        ``BIND-8.2.3-T5B`|
|`17`|`        ````RT using linux kernel aio```|
|`18`|`Thread-``local````storage support included.```|
|`19`|```For bug reporting instructions, please see:```|
|`20`|`<http://www.gnu.org/software/libc/bugs.html>.`|
|`21`|```[root@Betty ~]````#`|
2.
|`1`|```[root@Betty ~]``````# ldd  --version```|
|`2`|```ldd (GNU libc) 2.5```|
|`3`|```Copyright (C) 2006 Free Software Foundation, Inc.```|
|`4`|```This is````free````software; see the````source``for````copying conditions.  There is NO```|
|`5`|```warranty; not even````for````MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.```|
|`6`|```Written by Roland McGrath and Ulrich Drepper.```|
|`7`|```[root@Betty ~]````#`|
【glibc 和 glib】
      错误观点：glib 前面有个 "g" ，所以认为 glib 是 GNU 的东东；同时认为 glibc 是 glib 的一个子集。
      其实，glib 和 glibc 基本上没有太大联系，可能唯一的共同点就是，其都是 C 编程需要调用的库而已。
glib 是 Gtk+ 库和 Gnome 的基础。glib 可以在多个平台下使用，比如 Linux、Unix、Windows 等。glib 为许多标准的、常用的 C 语言结构提供了相应的替代物。
**glib是GTK+的基础库**，它由**基础类型、对核心应用的支持、实用功能、数据类型和对象系统**五个部分组成，可以在[[http://www.gtk.org](http://www.gtk.org/) gtk网站]下载其源代码。是一个**综合用途的实用的轻量级的C程序库**，它提供[C语言](http://lib.csdn.net/base/c)的常用的[数据结构](http://lib.csdn.net/base/datastructure)的定义、**相关的处理函数，有趣而实用的宏，可移植的封装和一些运行时机能，如事件循环、线程、动态调用、对象系统等的API**。GTK+是可移植的，当然glib也是可移植的，你可以在linux下，也可以在windows下使用它。使用gLib2.0（glib的2.0版本）编写的应用程序，在**编译时应该在编译命令中加入**
```
pkg-config
 --cflags --libs glib-2.0
```
，如：
> 
gcc `pkg-config --cflags --libs glib-2.0` hello.c -o hello
使用glib最有名的就是GNOME了。
【官方说明】
Glib
> 
GLib is a general-purpose utility library, which provides many useful data types, macros, type conversions, string utilities, file utilities, a main loop abstraction, and so on. It works on many UNIX-like platforms, Windows, OS/2 and BeOS. GLib is released
 under the GNU Library General Public License (GNU LGPL).
> 
The general policy of GLib is that all functions are invisibly threadsafe with the exception of data structure manipulation functions, where, if you have two threads manipulating the same data structure, they must use a lock to synchronize their operation.
> 
GLib is the low-level core library that forms the basis for projects such as GTK+ and GNOME. It provides data structure handling for C, portability wrappers, and interfaces for such runtime functionality as an event loop, threads, dynamic loading, and an object
 system. 
Glibc 
> 
Overview: 
Any Unix-like operating system needs a C library: the library which defines the ``system calls'' and other basic facilities such as open, malloc, printf, exit... 
The GNU C Library is used as the C library in the GNU systems and most systems with the Linux kernel.
> 
Project Goals:  
The GNU C Library is primarily designed to be a portable and high performance C library. It follows all relevant standards including ISO C11 and POSIX.1-2008. It is also internationalized and has one of the most complete internationalization interfaces known.
> 
History: 
The history of Unix and various standards determine much of the interface of the C library. In general the GNU C Library supports the ISO C and POSIX standards. We also try to support the features of popular Unix variants (including BSD and System V) when those
 do not conflict with the standards. Different compatibility modes (selectable when you compile an application) allow the peaceful coexistence of compatibility support for different varieties of Unix. 
【其他说法】
libc 实际上是一个泛指。凡是符合实现了 C 标准规定的内容，都是一种 libc 。
glibc 是 GNU 组织对 libc 的一种实现。它是 unix/linux 的根基之一。
微软也有自己的 libc 实现，叫 msvcrt 。
[嵌入式](http://lib.csdn.net/base/embeddeddevelopment)行业里还常用
 uClibc ，是一个迷你版的 libc 。
【yasi】
libc, glibc在一个层次，都是C的标准实现库，是[操作系统](http://lib.csdn.net/base/operatingsystem)级别的基石之一。
glib是用C写的一些utilities，即C的工具库，和libc/glibc没有关系。
转自：http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/yasi_xi/article/details/9899599
补充了一些下载源码的地址：
[http://svnweb.freebsd.org/base/head/lib/libc/b/libc/](http://svnweb.freebsd.org/base/head/lib/libc/)
[](http://blog.csdn.net/xiaohaijiejie/article/details/51814664#)[](http://blog.csdn.net/xiaohaijiejie/article/details/51814664#)[](http://blog.csdn.net/xiaohaijiejie/article/details/51814664#)[](http://blog.csdn.net/xiaohaijiejie/article/details/51814664#)[](http://blog.csdn.net/xiaohaijiejie/article/details/51814664#)[](http://blog.csdn.net/xiaohaijiejie/article/details/51814664#)
- 
顶
0- 
踩
