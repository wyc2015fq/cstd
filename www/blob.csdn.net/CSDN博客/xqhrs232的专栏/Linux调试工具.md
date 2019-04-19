# Linux调试工具 - xqhrs232的专栏 - CSDN博客
2016年10月14日 23:49:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：384
原文地址::[http://www.cnblogs.com/lidabo/p/4377545.html](http://www.cnblogs.com/lidabo/p/4377545.html)

# 1. 使用printf调试
#ifdef DEBUG
Printf(“valriable x has value = %d\n”, x)
#endif
然后在编译选项中加入-DDEBUG
更复杂的调试应用如：
#define BASIC_DEBUG 1
#define EXTRA_DEBUG 2
#define SUPER_DEBUG 4
#if (DEBUG &EXTRA_DEBUG)
       printf …
#endif
在这种情况下如果设置编译器标志为-DDEBUG=5,将启用BASIC_DEBUG和SUPER_DEBUG。 标志-DDEBUG=0将禁用所有的调试信息，也可以在程序中添加如下语句：
#ifndef DEBUG
#define DEBUG 0
#endif
# 2.使用gdb调试
Gcc编译的时候要加上-g选项，让编译器在程序中添加额外的调试信息。如果正式发布，这些调试信息可以使用strip命令删除。
Gdb:
Backtrace栈跟踪
# 3. 程序静态分析工具splint
splint功能：
常识性测试并产生一些警告信息。它可以检测未经赋值的变量使用，函数的参数未使用等异常情况。
# 4. 程序执行性能分析工具prof/gprof
显示执行所花费的时间具体都用在什么操作上。
# 5. 内存调试
ElectricFence函数库和valgrind可以用来检查动态内存分配的一些问题，包括内存泄漏。
Linux下的调试工具
随着XP的流行，人们越来越注重软件的前期设计、后期的实现，以及贯穿于其中的测试工作，经过这个过程出来的自然是高质量的软件。甚至有人声称XP会淘汰调试器！这当然是有一定道理的，然而就目前的现实来看，这还是一种理想。在日常工作中，调试工具还是必不可少的。在Linux下，调试工具并非只有gdb，还有很多其它调试工具，它们都各有所长，侧重方面也有所不同。本文介绍几种笔者常用的调试工具：
1.         mtrace
在linux下开发应用程序，用C/C++语言的居多。内存泄露和内存越界等内存错误，无疑是其中最头疼的问题之一。glibc为解决内存错误提供了两种方案：
一种是hook内存管理函数。hook内存管理函数后，你可以通过记下内存分配的历史记录，在程序终止时查看是否有内存泄露，这样就可以找出内存泄露的地方了。你也可以通过在所分配内存的首尾写入特殊的标志，在释放内存时检查该标志是否被破坏了，这样就可以达到检查内存越界问题的目的。
另外一种方法更简单，glibc已经为第一种方案提供了默认的实现，你要做的只是在特定的位置调用mtrace/muntrace两个函数，它们的函数原型如下：
       #include <mcheck.h>
       void mtrace(void);
void muntrace(void);
你可能会问，在哪里调这两种函数最好？这没有固定的答案，要视具体情况而定。对于小程序来说，在进入main时调用mtrace，在退出main函数时调用muntrace。对于大型软件，这样做可能会记录过多的信息，分析这些记录会比较慢，这时可以在你所怀疑代码的两端调用。
另外，还需要设置一个环境变量MALLOC_TRACE，它是一个文件名，要保证当前用户有权限创建和写入该文件。glibc的内存管理器会把内存分配的历史信息写入到MALLOC_TRACE指定的文件中。
程序运行完毕后，使用mtrace工具分析这些内存分配历史信息，可以查出内存错误的位置（mtrace在glibc-utils软件包里)。
2.         strace
在编程时，检查函数的返回值是一种好习惯。对于像glibc等标准C的函数，光检查返回值是不够的，还需要检查errno的值。这样的程序往往显得冗长，不够简洁。同时也可能是出于偷懒的原因，大多数程序里并没有做这样的检查。
这样的程序，一旦出现错误，用调试器一步一步定位错误，然后想法查出错误的原因，也是可以的，不过比较麻烦，对调试器来说有些大材小用，不太可取。这时，用strace命令可能会更方便一点。它可以显示各个系统调用/信号的执行过程和结果。比如文件打开出错，一眼就看出来了，连错误的原因(errno)都知道。
3.         binutil
binutil是一系列的工具，你可能根本不知道它们的存在，但是没有它们你却寸步难行。Binutil包括下列工具：
- ld - the GNU linker.
- as - the GNU assembler.
- addr2line - Converts addresses into filenames and line numbers.
- ar - A utility for creating, modifying and extracting from archives.
- c++filt - Filter to demangle encoded C++ symbols.
- gprof - Displays profiling information.
- nlmconv - Converts object code into an NLM.
- nm - Lists symbols from object files.
- objcopy - Copys and translates object files.
- objdump - Displays information from object files.
- ranlib - Generates an index to the contents of an archive.
- readelf - Displays information from any ELF format object file.
- size - Lists the section sizes of an object or archive file.
- strings - Lists printable strings from files.
- strip - Discards symbols.
- windres - A compiler for Windows resource files.
其中部分工具对调试极有帮助，如：
你可以用objdump反汇编，查看目标文件或可执行文件内部信息。
你可以用addr2line把机器地址转换到代码对应的位置。
你可以用nm查看目标文件或可执行文件中的各种符号。
       你可以用gprof分析各个函数的使用情况，找出性能的瓶颈所在(这需要加编译选项)。
4.         ld-linux
现在加载ELF可执行文件的工作，已经落到ld-linux.so.2头上了。你可能会问，这与有调试程序有关系吗？有的。比如，在linux中，共享库里所有非static的函数/全局变量都是export的，更糟的是C语言中没有名字空间这个概念，导致函数名极易冲突。在多个共享库中，名字冲突引起的BUG是比较难查的。这时，你可以通过设置LD_
 DEBUG环境变量，来观察ld-linux.so加载可执行文件的过程，从中可以得到不少帮助信息。LD_ DEBUG的取值如下：
- libs        display library search paths
- reloc       display relocation processing
- files       display progress for input file
- symbols     display symbol table processing
- bindings    display information about symbol binding
- versions    display version dependencies
- all         all previous options combined
- statistics  display relocation statistics
- unused      determined unused DSOs
- help        display this help message and exit
5.         gdb
对于真正意义的调试器来说，gdb在linux下是独一无二的。它有多种包装，有字符界面的，也有图形界面的，有单独运行的，也有集成到IDE中的。gdb功能强大，图形界面的gdb容易上手一点，但功能无疑受到了一些限制，相信大部分高手还是愿意使用字符界面的。Gdb太常用了，这里不再多说。
6.         gcc/boundschecker
相信很多人用过win32下的BoundsChecker(Compuware公司)和Purify(IBM公司)两个工具吧。它们的功能实在太强大了，绝非能通过重载内存管理函数就可以做到，它们在编译时插入了自己的调试代码。
gcc也有个扩展，通过在编译时插入调试代码，来实现更强大的检查功能。当然这要求重新编译gcc，你可以到[http://sourceforge.net/projects/boundschecking/](http://sourceforge.net/projects/boundschecking/) 下载gcc的补丁。它的可移植性非常好，笔者曾一个ARM 平台项目里使用过，效果不错。
7.         valgrind
最好的东西往往最后才见到。Valgrind是我的最爱，用习惯了，写的程序不在valgrind下跑一遍，就像没有写单元测试程序一样，有点放心不下。它有BoundsChecker/Purify的功能，而且速度更快。
有点遗憾的是valgrind目前只支持x86平台，当然，这对大多数情况已经足够了。
你可以到[http://valgrind.org/](http://valgrind.org/) 下载最新版本。
## Linux 平台上的C语言调试工具！
Debugging Tools for C on Linux Platform
[http://www.linuxgazette.com/node/view/8755](http://www.linuxgazette.com/node/view/8755)
Submitted by Nikhil Bhargava on Tue, 02/17/2004 - 14:37. Articles | General Interest
This article talks about debugging tools for applications in C on Linux platforms. Most of the tools are freely available on all major platforms with very wide user support. The tools help in static analysis of code as well as assist in dynamic evaluation of
 code.
Please note that tools listed here are suggestions of the author. This list is not a standard one. Changes have to be done in it depending upon the nature, scope and details of the application to be developed.
Debugging Tools
1. Dmalloc [http://dmalloc.com/](http://dmalloc.com/)
The debug memory allocation or Dmalloc library is a freeware debugging tool which has been specially designed as a drop in replacement for the system's malloc, realloc, calloc, free and other memory management routines while providing powerful debugging facilities
 configurable at runtime. It makes changes during compile time and donot add runtime changes in binary. These facilities include such things as memory-leak tracking, fence-post write detection, file/line number reporting, and general logging of statistics.
 The library is reasonably portable having been run successfully on at least the following operating systems: AIX, BSD/OS, DG/UX, Free/Net/OpenBSD, GNU/Hurd, HPUX, Irix, Linux, MS-DOG, NeXT, OSF, SCO, Solaris, SunOS, Ultrix, Unixware, Windoze, and even Unicos
 on a Cray T3E. It has full support for programs the debugging of POSIX threads.
The package includes the library, configuration scripts, debug utility application, test program, and documentation.
2. Valgrind [http://valgrind.kde.org/](http://valgrind.kde.org/)
Valgrind is a GPL distributed system for debugging and profiling x86-Linux programs. I can also be helpful for programs for platforms other than x86 since behavior of x86 binary is similar to other binaries. The accompanying tools with Valgrind automatically
 detect many memory management and threading bugs, avoiding hours of frustrating bug-hunting, making programs more stable. It supports a through detailed profiling to help speed up the programs.
The Valgrind distribution includes four tools: two memory error detectors, a thread errordetector, and a cache profiler.
3. Electricfence [http://rpmfind.net/linux/RPM/conecti...-2cl.i386.html](http://rpmfind.net/linux/RPM/conectiva/8/cd1/conectiva/RPMS.001/ElectricFence-2.2.2-2cl.i386.html)
Electric Fence is a freeware library that can be used for C programming and debugging. It can be linked at compile time and it will warn about possible problems such as freeing memory that doesn't exist, etc. It is basically a memory profiling tool. However
 currently it is available only on HP-Unix platform (I am not very sure though).
4. GDB [http://sources.redhat.com/gdb](http://sources.redhat.com/gdb)
This is the Gnome Debugger which comes as a freeware support package with freeware Linux distribution like Red Hat, Slacware, and Debian etc. It has full support of many languages like C, C++, and Perl etc. It helps to debug the binaries of these languages
 in modes like single step, multiple step or complete run. It also has provisions of setting break points and trace value.
It is helpful for stub testing, functional flow checking and bound checking. Further this is readily available with all flavours of Linux andUnix platforms and is amply supported in user community.
5. Insight [http://sources.redhat.com/insight](http://sources.redhat.com/insight)
Insight is a graphical user interface to GDB, the GNU Debugger written in Tcl/Tk by at Red Hat, Inc. and Cygnus Solutions Insight provides all features provided by GDB along with Graphical debugging interface replacing traditional command based interface.
6. Memprof [http://www.gnome.org/projects/memprof](http://www.gnome.org/projects/memprof)
MemProf is a free ware memory Profiling and memory leak detection tool which comes as an addendum to common Linux distributions. It can generate a profile how much memory was allocated by each function in the program. It can scan memory and find blocks that
 have been allocated but are no longer referenced anywhere (dead code).
MemProf works by pre-loading a library to override the C library's memory allocation functions and does not require recompiling the program. One advantage MemProf has over some other similar tools that are available is that it has a nice GUI front-end and is
 relatively easy to use.
I am Nikhil Bhargava from Delhi, India. I am a Computer Engineer currently working in C-DOT, India for past one year. Comments and Suggestions are always welcome.
## 10.3  图形化调试工具
尽管你可以在大多数（即便不是全部）Linux调试 任务中使用GDB，但与长时间坐在GDB命令行前面相比，许多人还是更愿意使用诸如DDD或Eclipse这样的图形化工具。从各方面来看，大多数非常花 哨的图形化调试工具不过是建立在GDB基础上的一个抽象，所以选择哪一种图形化工具完全属于个人爱好。
本节将介绍两个这样的工具，当然还存在其他许多这样的工具（包括GDB的前端GNU insight），它们也被各种开发团队所使用。
从事Linux应用开发一年多了，感觉很不规范很山寨，准备系统地学习一下下linux开发，首先从调试工具的学习开始，以下是从网上看到的一篇linux调试工具介绍，准备好好学习这些工具的使用。
“工欲善其事 必先利其器”现在将一些常见的调试工具进行一下总结.
【1】
名称：MEMWATCH 
功能：
    1、MEMWATCH 支持 ANSI C
    2、它提供结果日志纪录
    3、能检测双重释放（double-free）
    4、错误释放（erroneous free）
    5、没有释放的内存（unfreed memory）、
    6、溢出和下溢等等。
参考文献：
      【1】：
[http://blog.csdn.net/lengxingfei/archive/2006/08/09/1040800.aspx](http://blog.csdn.net/lengxingfei/archive/2006/08/09/1040800.aspx)
【2】
名称：YAMD
功能：
    1、查找 c 和 c++ 中动态的、与内存分配有关的问题
参考文献：
     【1】
[http://www.cs.hmc.edu/~nate/yamd/](http://www.cs.hmc.edu/~nate/yamd/)
     【2】
[http://www.ppsql.com/server/p262/J26232224_2.shtml](http://www.ppsql.com/server/p262/J26232224_2.shtml)
【3】
名称：electric fence 
功能：
    1、检测内存泄露
    2、检查数据越界
    3、分配受保护的内存
参考文献：
    【1】
[http://book.opensourceproject.org.cn/embedded/oreillybuildembed/index.html?page=opensource/belinuxsys-chp-11-sect-4.html](http://book.opensourceproject.org.cn/embedded/oreillybuildembed/index.html?page=opensource/belinuxsys-chp-11-sect-4.html)
    【2】
[http://book.opensourceproject.org.cn/embedded/oreillybuildembed/opensource/belinuxsys-chp-11-sect-4.html](http://book.opensourceproject.org.cn/embedded/oreillybuildembed/opensource/belinuxsys-chp-11-sect-4.html)
    【3】
[http://www.ppsql.com/server/p262/J26232224_3.shtml](http://www.ppsql.com/server/p262/J26232224_3.shtml)
【4】
名称：strace
功能：
    1、显示用户空间程序发出的系统调用
    2、显示调用的参数和返回值
【5】
名称：gdb
功能：
    1、启动程序
    2、使程序能够停止在指定文件的指定位置
    3、查看程序变量值
    4、改变程序执行期间的相关变量、路径等
【6】
名称：Oops 
功能： 显示系统错误信息
参考文献：
    【1】
[http://www.ibm.com/developerworks/cn/linux/sdk/l-debug/](http://www.ibm.com/developerworks/cn/linux/sdk/l-debug/)
【7】
名称：mtrace
功能：
    1、检测一些内存分配和泄漏的失败等
参考文献：
    【1】
[http://www.vbaccelerator.com/home/VB/Code/Techniques/RunTime_Debug_Tracing/mTrace_bas.asp](http://www.vbaccelerator.com/home/VB/Code/Techniques/RunTime_Debug_Tracing/mTrace_bas.asp)
    【2】
[http://www.yuanma.org/data/2006/0612/article_789.htm](http://www.yuanma.org/data/2006/0612/article_789.htm)
    【3】
[http://www.mylinux.com.cn/HTML/1183705958487.html](http://www.mylinux.com.cn/HTML/1183705958487.html)
【8】
名称：binutil
说明：binutil为一个工具集合，包含如下的工具。
     addr2line  把程序地址转换为文件名和行号。在命令行中给它一个地址和一个可执行文件名，它就会使用这个可执行文件的调试信息指出在给出的地址上是哪个文件以及行号。
     ar   建立、修改、提取归档文件。归档文件是包含多个文件内容的一个大文件，其结构保证了可以恢复原始文件内容。
    as   主要用来编译GNU C编译器gcc输出的汇编文件，产生的目标文件由连接器ld连接。
    c++filt  连接器使用它来过滤 C++ 和 Java 符号，防止重载函数冲突。
    gprof   显示程序调用段的各种数据。
    ld   是连接器，它把一些目标和归档文件结合在一起，重定位数据，并链接符号引用。通常，建立一个新编译程序的最后一步就是调用ld。
    nm   列出目标文件中的符号。
    objcopy 把一种目标文件中的内容复制到另一种类型的目标文件中.
    objdump  显示一个或者更多目标文件的信息。显示一个或者更多目标文件的信息。使用选项来控制其显示的信息。它所显示的信息通常只有编写编译工具的人才感兴趣。
    ranlib  产生归档文件索引，并将其保存到这个归档文件中。在索引中列出了归档文件各成员所定义的可重分配目标文件。
    readelf  显示ebf格式可执行文件的信息。
    size   列出目标文件每一段的大小以及总体的大小。默认情况下，对于每个目标文件或者一个归档文件中的每个模块只产生一行输出。
    strings  打印某个文件的可打印字符串，这些字符串最少4个字符长，也可以使用选项-n设置字符串的最小长度。默认情况下，它只打印目标文件初始化和可加载段中的可打印字符；对于其它类型的文件   它打印整个文件的可打印字符，这个程序对于了解非文本文件的内容很有帮助。
    strip   丢弃目标文件中的全部或者特定符号。
    libiberty  包含许多GNU程序都会用到的函数，这些程序有： getopt, obstack, strerror, strtol 和 strtoul.
   libbfd  二进制文件描述库.
   libopcodes  用来处理opcodes的库, 在生成一些应用程序的时候也会用到它, 比如objdump.Opcodes是文本格式可读的处理器操作指令.
功能：程序调试、归档等
参考文献：
[http://man.chinaunix.net/linux/](http://man.chinaunix.net/linux/)
【9】
名称： Purify 
功能：内存检查工具
参考文献：
[http://blog.csdn.net/haoel/archive/2003/12/16/2904.aspx](http://blog.csdn.net/haoel/archive/2003/12/16/2904.aspx)
【10】
名称：pc-lint
功能：C/C++的静态代码检测工具
参考文献：
      【1】
[http://dev.yesky.com/134/2593634.shtml](http://dev.yesky.com/134/2593634.shtml)
【11】
名称：splint
说明：splint是一个动态检查C语言程序安全弱点和编写错误的程序
功能：
    1、空引用错误
    2、未定义的变量错误
    3、类型错误
    4、内存检查
参考文献：
    【1】
[http://www.turbolinux.com.cn/turbo/wiki/doku.php?id=splint&DokuWiki=e3b300af2d101a8bec5a4c805e678823](http://www.turbolinux.com.cn/turbo/wiki/doku.php?id=splint&DokuWiki=e3b300af2d101a8bec5a4c805e678823)
    【2】
[http://www.splint.org/](http://www.splint.org/)
【12】
名称：boundercheck
功能：检查内存泄露
【13】
名称：valgrind
功能：
    1、检查内存错误
参考文献:
[http://blog.donews.com/idlecat511/archive/2006/02/21/736877.aspx](http://blog.donews.com/idlecat511/archive/2006/02/21/736877.aspx)
 IDA最知名的是反汇编器，它是二进制文件执行静态分析的最佳工具之一。由于现代反静态分析技术的复杂性，人们常常将静态分析与动态分析技术结合起来，以利用二者的优势。理想情况下，所有这些工具集成在一个软件包中。Hex-Rays在推出4.5版的IDA调试器时，做出巩固IDA的作用将其作为一个通用的逆向工程工具这一举动。随着IDA的后续版本，调试功能得到改善。在最新版本中，IDA能够在多个不同的平台上进行本地和远程调试，并支持多种不同的处理器。

# Linux调试工具lsof的深入分析
DDD，全称是Data Display Debugger，对于Linux系统中的编程人员来说，它就是windows系统下面的visual studio ，功能强大，数Linux世界中少数有图形界面的程序调试工具
- 
### [跨平台Web抓包分析工具 Rythem](http://www.oschina.net/p/rythem)
Rythem是什么 Rythem是一个与Fiddler同类的软件，和Fiddler一样具有 代理抓包/替换 功能，与Fiddler最大的不同是Rythem是跨平台&开源的。 官方详细介绍： http://www.alloyteam.com/2012/05/web-front-end-tool-rythem-1/ 以下为Rythem运行的截图：(MacO...[](http://www.oschina.net/p/rythem)[更多Rythem信息](http://www.oschina.net/p/rythem)
- 
### [内存泄漏问题跟踪 YAGAC](http://www.oschina.net/p/yagac)
YAGAC 是一个库用来链接到你的C程序，让您可以方便地跟踪应用程序中的内存泄漏问题。根据你的代码部分。它跟踪你的代码只分配内存，不监测第三部分库，你可以有多个垃圾收集。跟踪可以被激活的更新调试标志，而无需重新启动您的应用程序。其目的是为守护程...[更多YAGAC信息](http://www.oschina.net/p/yagac)
最近更新： [YAGAC 0.99-4.0 发布，C 程序内存泄漏检测](http://www.oschina.net/news/36841/agac-0-99-40) 发布于
 1年前
- 
### [C++ 异常堆栈跟踪器 libcsdbg](http://www.oschina.net/p/libcsdbg)
libcsdbg 是一个 C++ 异常堆栈跟踪器。当异常被抛出、捕获和处理时，libcsdbg 提供可工具来创建和处理异常堆栈的跟踪，并通过调用堆栈来传播异常路径。跟踪器提供异常发生的函数以及额外的 addr2line 信息（源码级别） 功能列表： 复杂的异常堆栈跟踪（支持...[](http://www.oschina.net/p/libcsdbg)[更多libcsdbg信息](http://www.oschina.net/p/libcsdbg)
最近更新： [libcsdbg 1.23 发布，C++ 异常堆栈跟踪器](http://www.oschina.net/news/48878/libcsdbg-1-23) 发布于
 4天前
- 
### [程序调试工具 GDB](http://www.oschina.net/p/gdb)
|GDB（GNU symbolic debugger）简单地说就是一个调试工具。它是一个受通用公共许可证即GPL保护的自由软件。 象所有的调试器一样，GDB可以让你调试一个程序，包括让程序在你希望的地方停下，此时 你可以查看变量，寄存器，内存及堆栈。更进一步你可以修改变量...[](http://www.oschina.net/p/gdb)[更多GDB信息](http://www.oschina.net/p/gdb)|[](http://www.oschina.net/p/gdb)|
最近更新： [GDB 7.7 发布，程序调试工具](http://www.oschina.net/news/48581/gdb-7-7) 发布于 12天前
- 
### [可视化调试工具 DDD](http://www.oschina.net/p/ddd)
GNU DDD (Data Display Debuger) 是一个图形化的调试工具，通过对调试命令例如 GDB、DBX、WDB、JDB、XDB 等等命令行工具的封装，提供一个非常友好的调试界面。[](http://www.oschina.net/p/ddd)[更多DDD信息](http://www.oschina.net/p/ddd)
- 
### [程序调试工具 strace](http://www.oschina.net/p/strace)
strace是Linux环境下的一款程序调试工具，用来监察一个应用程序所使用的系统呼叫及它所接收的系统信息。 strace是一个有用的小工具，它可以通过跟踪系统调用来让你知道一个程序在后台所做的事情。Strace是一个基础的调试工具，在大多数Linux系 统上默认已经...[更多strace信息](http://www.oschina.net/p/strace)
- 
### [内存泄露检测工具 Visual Leak Detector](http://www.oschina.net/p/visualleakdetector)
灵活自由是C/C++语言的一大特色，而这也为C/C++程序员出了一个难题。当程序越来越复杂时，内存的管理也会变得越加复杂，稍有不慎就会出现内存问题。内存泄漏是最常见的内存问题之一。内存泄漏如果不是很严重，在短时间内对程序不会有太大的影响，这也使得内...[更多Visual
 Leak Detector信息](http://www.oschina.net/p/visualleakdetector)
- 
### [运行时诊断工具 Valgrind](http://www.oschina.net/p/valgrind)
Valgrind是一个运行时诊断工具，它可以监视 一个指定程序的活动并通知你在你的代码中可能存在的各种各样的内存管理问题。它类似于老式的Electric Fence工具（该工具将标准的内存分配函数替换为自己的函数以提高诊断能力），但被认为更容易使用并且在多个方...[](http://www.oschina.net/p/valgrind)[更多Valgrind信息](http://www.oschina.net/p/valgrind)
最近更新： [Valgrind 3.9.0 发布，运行时诊断工具](http://www.oschina.net/news/45640/valgrind-3-9-0) 发布于
 3个月前
- 
### [图形化的调试工具 KDbg](http://www.oschina.net/p/kdbg)
KDbg 是一个 gdb 调试工具的图形化界面的工具（点击查看大图）。[](http://www.oschina.net/p/kdbg)[更多KDbg信息](http://www.oschina.net/p/kdbg)
最近更新： [KDbg 2.5.4 发布，图形化的调试工具](http://www.oschina.net/news/45799/kdbg-2-5-4) 发布于 3个月前
- 
### [【共享】程序调试工具 OllyDbg](http://www.oschina.net/p/ollydbg)
OLLYDBG是一个新的动态追踪工具，将IDA与SoftICE结合起来的思想，Ring 3级调试器，非常容易上手，己代替SoftICE成为当今最为流行的调试解密工具了.同时还支持插件扩展功能,是目前最强大的调试工具.[](http://www.oschina.net/p/ollydbg)[更多OllyDbg信息](http://www.oschina.net/p/ollydbg)
- 
### [C/C++调试器 LLDB](http://www.oschina.net/p/lldb)
LLDB 是下一代高性能的调试器，构建一组可重用的组件，覆盖很多 LLVM 项目的库，例如 Clang 表达式解析器和 LLVM 反汇编器。目前该项目还处于前期开发状态，但已经注意支持在 Mac OS X 上的 C/C++ 和 Objective-C 的开发。...[更多LLDB信息](http://www.oschina.net/p/lldb)
- 
### [【免费】窗口应用调试工具 Managed Spy](http://www.oschina.net/p/managedspy)
有不少开发者使用Visual Studio®提供的工具Spy++。使用Spy++，你可以了解一个运行中的应用程序的窗体布局或确定一个导致bug的特定窗体消息。然而，当你创建一 个基于Microsoft® .NET框架的应用程序，Spy++变得不太管用了，因为由Spy++截取窗体消息和类不...[](http://www.oschina.net/p/managedspy)[更多Managed
 Spy信息](http://www.oschina.net/p/managedspy)
- 
### [Linux程序调试工具 XBuildStudio](http://www.oschina.net/p/xbuilddev)
您还在使用gdb缓慢地调试UNIX/LINUX的程序吗？XBuildStudio是专为提升UNIX/LINUX的程序的开发而设计的在WINDOWS平台上调试UNIX/LINUX程序的开发工具，操作方式类似VC的环境，有效提升您的工作效率.[](http://www.oschina.net/p/xbuilddev)[更多XBuildStudio信息](http://www.oschina.net/p/xbuilddev)
- 
### [gdb的终端界面增强版 CGDB](http://www.oschina.net/p/cgdb)
cgdb，可以看成是gdb的终端界面增强版，相比windows下的visual studio而言，它的功能显得十分轻量级而没有太多繁杂，能在终端里运行。[](http://www.oschina.net/p/cgdb)[更多CGDB信息](http://www.oschina.net/p/cgdb)
- 
### [调试器 EDB (Evan's Debugger)](http://www.oschina.net/p/edb)
EDB (Evan's Debugger) 是一个基于 Qt4 开发的二进制调试工具，主要是为了跟 OllyDbg 工具看齐，通过插件体系可进行功能的扩充，当前只支持 Linux ，将来会发布其他平台的版本。[](http://www.oschina.net/p/edb)[更多EDB
 (Evan's Debugger)信息](http://www.oschina.net/p/edb)
最近更新： [Evan's Debugger
 0.9.20 发布，二进制调试工具](http://www.oschina.net/news/47883/evan_s-debugger-0-9-20) 发布于 1个月前
- 
### [图形化OpenGL调试器 BuGLe](http://www.oschina.net/p/bugle)
BuGLe 结合图形化的OpenGL调试与选择的过滤器上的OpenGL命令流。调试器可以查看状态、纹理、framebuffers ，着色器，而过滤器允许日志，错误检查，自由相机控制，视频捕捉等。[](http://www.oschina.net/p/bugle)[更多BuGLe信息](http://www.oschina.net/p/bugle)
- 
### [软件开发辅助软件 uuSpy](http://www.oschina.net/p/uuspy)
uuSpy工具很类似于Spy++。此外，还可以查看[***]的密码，甚至取得Internet Explorer的HTML。使用uuSpy你可以知道正在运行的窗口程序的结构。或者监视导致程序出错的特定的窗口消息。甚至你可以输出指定窗口的 style/exstyle，或做某些设置。 uuSpy可以对指...[](http://www.oschina.net/p/uuspy)[更多uuSpy信息](http://www.oschina.net/p/uuspy)
- 
### [Linux内核测试工具 CrackerJack](http://www.oschina.net/p/crackerjack)
Crackerjack Project 是由中日韩三方共同参加开发，该项目主要目的是为Linux内核提供自动化的回归测试功能，以帮助提高内核开发质量等，项目包括一个完整的内核自动化测试框架，以实现自动化载入、执行、管理测试用例，还包括定义新的测试程序（用例），集...[更多CrackerJack信息](http://www.oschina.net/p/crackerjack)
- 
### [开源PE文件分析工具 pev](http://www.oschina.net/p/pev)
pev是一个小巧的基于命令行的windows可执行文件(PE)分析工具，支持linux、windows和OSX系统。特别的是它使用ANSI标准c编写，因此可以容易的在多个平台上编译。pev免费并且开放源代码。[更多pev信息](http://www.oschina.net/p/pev)
- 
### [C 语言内存错误检测工具 memwatch](http://www.oschina.net/p/memwatch)
MEMWATCH 由 Johan Lindh 编写，是一个开放源代码 C 语言内存错误检测工具。只要在代码中添加一个头文件并在 gcc 语句中定义了 MEMWATCH 之后，您就可以跟踪程序中的内存泄漏和错误了。MEMWATCH 支持 ANSI C，它提供结果日志记录，能检测双重释放（double-...[更多memwatch信息](http://www.oschina.net/p/memwatch)
- |- ### [程序跟踪的C库 PinkTrace](http://www.oschina.net/p/pinktrace)PinkTrace是一款轻量级的C库文件，简化了编写应用程序跟踪程序的困难。它由围绕不同ptrace()请求的封装、用于解码参数 （strings, socket addresses）的API和一个实验性的用于编码参数掉的API构成。PinkTrace的目标是为跟踪程序提供一个跨平台的库。另外，...[更多PinkTrace信息](http://www.oschina.net/p/pinktrace)- ### [BASH Debugger](http://www.oschina.net/p/bashdb)Shell 脚本调试器。 We have debuggers for bash, python, zsh, ksh, GNU make 3.81 and Python. In the patched sources for Make, we provide better error handling and tracing. GNU Emacs support is included for all debuggers....[](http://www.oschina.net/p/bashdb)[更多BASH Debugger信息](http://www.oschina.net/p/bashdb)- ### [程序性能跟踪工具 LTTng](http://www.oschina.net/p/lttng)一个高效的Linux 的Tracing Tools。它能够帮助发现并行环境下的性能问题及bug，甚至跨系统tracing也是可能的。[更多LTTng信息](http://www.oschina.net/p/lttng)- ### [程序调试工具 ltrace](http://www.oschina.net/p/ltrace)ltrace 是一个调试程序，可在它退出之前执行指定命令来拦截和记录动态库调用和信号接收过程。它还可以拦截并打印系统调用。要跟踪的项目不需要重新编译,可以直接在二进制文件上使用。[更多ltrace信息](http://www.oschina.net/p/ltrace)最近更新： [ltrace 0.7.1 发布，程序调试工具](http://www.oschina.net/news/35259/ltrace-0-7-1) 发布于 1年前- ### [Linux 调试器 和 跟踪器 KGTP](http://www.oschina.net/p/kgtp)KGTP 是一个 实时 轻量级 Linux 调试器 和 跟踪器 。 使用KGTP 不需要 在Linux内核上打PATCH或者重新编译，只要编译KGTP模块并insmod就可以。 其让Linux内核提供一个远程GDB调试接口，于是在本地或者远程的主机上的GDB可以在不需要停止内核的情况下用GDB t...[更多KGTP信息](http://www.oschina.net/p/kgtp)- ### [调试助手库 DAL](http://www.oschina.net/p/dal)Debug Assistant Library (DAL) 是用于辅助调试的程序库，具有跨平台和统一接口的特点。 主要特性： assistant library for debugging, across platforms with uniform interface supports multi-compiler, support multi-platform Debug Message Asserti...[更多DAL信息](http://www.oschina.net/p/dal)最近更新： [DAL 2.0.32 发布，调试助手库](http://www.oschina.net/news/36019/dal-2-0-32) 发布于 1年前- ### [Ollydbg的TLS回调捕获插件 TLSCatch](http://www.oschina.net/p/tlscatch)TLSCatch是一款Ollydbg插件，它可以简单拦截当前进程地址空间的新模块加载操作，并从中搜索tlscallback。如果发现回调，则在每 个回调上设置一个一次性断点。在恶意软件分析中可以捕获任何tls callback。将 TLSCatch.dll拷贝到Ollydbg的插件目录就可以工作...[更多TLSCatch信息](http://www.oschina.net/p/tlscatch)- ### [故障注入的C库 libfiu](http://www.oschina.net/p/libfiu)libfiu是一个故障注入的C库。它提供用于标识“故障点”（“核心API”）里面的代码，功能启用/禁用这些点的失败（“控制API”）。核心API内部使用上要执行故障注入代码。控制API用于内部测试代码，以控制注入失败。 libfiu is a C library for fault inject...[更多libfiu信息](http://www.oschina.net/p/libfiu)- ### [pstack工具 lsstack](http://www.oschina.net/p/lsstack)lsstack是一个linux实现的的pstack工具，它原本在solaris系统上。 lsstack是一个命令行工具。 它能对指定的进程快速而高效地打印出其的线程和调用堆栈（call stack）。 以及函数在被ar的时候的符号名称。[更多lsstack信息](http://www.oschina.net/p/lsstack)- ### [vim-ruby-debugger](http://www.oschina.net/p/vim-ruby-debugger)vim-ruby-debugger 这是 vim 上调试 Ruby 程序的插件。[更多vim-ruby-debugger信息](http://www.oschina.net/p/vim-ruby-debugger)- ### [在windows下编译和调试远程GDB BVRDE](http://www.oschina.net/p/bvrde)BVRDE是一款“Remote Compile IDE”，主要功能为：在windows上提供一套图形化的界面来编译和调试远程的系统，(当然，BVRDE也支持对本地系统的调试)。方便对GDB等调试工具不熟悉或者不喜欢的用户进行程序调试，提高工作效率。 BVRDE安装在Windows系统上，通...[](http://www.oschina.net/p/bvrde)[更多BVRDE信息](http://www.oschina.net/p/bvrde)- ### [反调试demo Pafish](http://www.oschina.net/p/pafish)Pafish是一个模拟了多个反调试(调试器/虚拟机/沙箱)技巧的demo工具。这些反调试技巧经常被恶意软件使用，干扰调试器工作或动态行为分析工作。(文/pulog)[](http://www.oschina.net/p/pafish)[更多Pafish信息](http://www.oschina.net/p/pafish)- ### [skpd](http://www.oschina.net/p/skpd)skpd 是一个用来将进程 dump 到一个可执行的 ELF 文件并执行。[更多skpd信息](http://www.oschina.net/p/skpd)- ### [Termdebug](http://www.oschina.net/p/termdebug)Termdebug是一组实用程序来记录和回放的输入和输出一个终端程序。它的主要目标是帮助开发和调试终端程序。其他项目如termrec / termplay,nethack-recorder /播放器和脚本/ scriptreplay只记录输出。然而,当调试一个交互式终端程序,输入通常是更重要的输出。...[更多Termdebug信息](http://www.oschina.net/p/termdebug)最近更新： [Termdebug 2.1 发布，输入的记录和回放](http://www.oschina.net/news/41270/termdebug-2-1) 发布于 8个月前- ### [内存函数分析工具 LMDBG](http://www.oschina.net/p/lmdbg)LMDBG (Lightweight modular malloc debugger) 提供一组小工具用来收集和分析 malloc/realloc/memalign/free 函数调用的日志。LMDBG 不提供任何方式用来检测 malloc() 内存分配的超支分界。LMDBG 可检测内存泄漏和重复释放。LMDBG 可生成全堆栈日志。...[更多LMDBG信息](http://www.oschina.net/p/lmdbg)最近更新： [LMDBG 1.2.0 发布，内存分配函数分析](http://www.oschina.net/news/43039/lmdbg-1-2-0) 发布于 6个月前- ### [php 调试工具 ydb](http://www.oschina.net/p/ydb)    ydb 是像gdb一样的php调试工具，用扩展实现，可以在运行过程中查看变量值，也可以进行性能测试，而不用对源代码进行任何更改，就像xdebug和xhprof 中提供的功能，但使用更方便     ydb也可以称为一个在线调试工具，什么叫在线调试？就是在线上生产...[更多ydb信息](http://www.oschina.net/p/ydb)- ### [分布式多线程检查点工具 DMTCP](http://www.oschina.net/p/dmtcp)高性能计算/分布式计算等大量计算需要程序运行几天、几周甚至几个月，如果期间因为电力或者不可避免的问题导致程序中断会浪费大量的时间和人力，还 有超级计算机在这段时间里运行的电力成本。我们没有遇到过电力问题，不过我们最近遇到的场景是实验室需要做...[更多DMTCP信息](http://www.oschina.net/p/dmtcp)- ### [垃圾收集器 bdwgc](http://www.oschina.net/p/bdwgc)bdwgc （Boehm-Demers-Weiser conservative garbage collector）是个垃圾收集器，用来替代 C malloc 或者是 C++ new。bdwgc 也可以用其他编程语言实现，但是要用 C 作为中间转换语言。bdwgc 也可以用来检测 C 或者 C++ 程序的漏洞。旧版的垃圾收集器中包括...[更多bdwgc信息](http://www.oschina.net/p/bdwgc)- ### [C/C++ 程序调试工具 libdwarf-dwarfdump](http://www.oschina.net/p/libdwarf-dwarfdump)libdwarf 可以在 ELF 对象文件中读或者写 DWARF 调试信息。dwarfdump 使用 libdwarf 来读对象文件和打印可读样式的 DWARF 内容。libdwarf 遵从了 DWARF2 及以上标准，源代码中包括一些树搜索（tsearch）算法（平衡，红黑和二进制）。...|PinkTrace是一款轻量级的C库文件，简化了编写应用程序跟踪程序的困难。它由围绕不同ptrace()请求的封装、用于解码参数 （strings, socket addresses）的API和一个实验性的用于编码参数掉的API构成。PinkTrace的目标是为跟踪程序提供一个跨平台的库。另外，...[更多PinkTrace信息](http://www.oschina.net/p/pinktrace)|Shell 脚本调试器。 We have debuggers for bash, python, zsh, ksh, GNU make 3.81 and Python. In the patched sources for Make, we provide better error handling and tracing. GNU Emacs support is included for all debuggers....[](http://www.oschina.net/p/bashdb)[更多BASH Debugger信息](http://www.oschina.net/p/bashdb)|一个高效的Linux 的Tracing Tools。它能够帮助发现并行环境下的性能问题及bug，甚至跨系统tracing也是可能的。[更多LTTng信息](http://www.oschina.net/p/lttng)|ltrace 是一个调试程序，可在它退出之前执行指定命令来拦截和记录动态库调用和信号接收过程。它还可以拦截并打印系统调用。要跟踪的项目不需要重新编译,可以直接在二进制文件上使用。[更多ltrace信息](http://www.oschina.net/p/ltrace)|KGTP 是一个 实时 轻量级 Linux 调试器 和 跟踪器 。 使用KGTP 不需要 在Linux内核上打PATCH或者重新编译，只要编译KGTP模块并insmod就可以。 其让Linux内核提供一个远程GDB调试接口，于是在本地或者远程的主机上的GDB可以在不需要停止内核的情况下用GDB t...[更多KGTP信息](http://www.oschina.net/p/kgtp)|Debug Assistant Library (DAL) 是用于辅助调试的程序库，具有跨平台和统一接口的特点。 主要特性： assistant library for debugging, across platforms with uniform interface supports multi-compiler, support multi-platform Debug Message Asserti...[更多DAL信息](http://www.oschina.net/p/dal)|TLSCatch是一款Ollydbg插件，它可以简单拦截当前进程地址空间的新模块加载操作，并从中搜索tlscallback。如果发现回调，则在每 个回调上设置一个一次性断点。在恶意软件分析中可以捕获任何tls callback。将 TLSCatch.dll拷贝到Ollydbg的插件目录就可以工作...[更多TLSCatch信息](http://www.oschina.net/p/tlscatch)|libfiu是一个故障注入的C库。它提供用于标识“故障点”（“核心API”）里面的代码，功能启用/禁用这些点的失败（“控制API”）。核心API内部使用上要执行故障注入代码。控制API用于内部测试代码，以控制注入失败。 libfiu is a C library for fault inject...[更多libfiu信息](http://www.oschina.net/p/libfiu)|lsstack是一个linux实现的的pstack工具，它原本在solaris系统上。 lsstack是一个命令行工具。 它能对指定的进程快速而高效地打印出其的线程和调用堆栈（call stack）。 以及函数在被ar的时候的符号名称。[更多lsstack信息](http://www.oschina.net/p/lsstack)|vim-ruby-debugger 这是 vim 上调试 Ruby 程序的插件。[更多vim-ruby-debugger信息](http://www.oschina.net/p/vim-ruby-debugger)|BVRDE是一款“Remote Compile IDE”，主要功能为：在windows上提供一套图形化的界面来编译和调试远程的系统，(当然，BVRDE也支持对本地系统的调试)。方便对GDB等调试工具不熟悉或者不喜欢的用户进行程序调试，提高工作效率。 BVRDE安装在Windows系统上，通...[](http://www.oschina.net/p/bvrde)[更多BVRDE信息](http://www.oschina.net/p/bvrde)|Pafish是一个模拟了多个反调试(调试器/虚拟机/沙箱)技巧的demo工具。这些反调试技巧经常被恶意软件使用，干扰调试器工作或动态行为分析工作。(文/pulog)[](http://www.oschina.net/p/pafish)[更多Pafish信息](http://www.oschina.net/p/pafish)|skpd 是一个用来将进程 dump 到一个可执行的 ELF 文件并执行。[更多skpd信息](http://www.oschina.net/p/skpd)|Termdebug是一组实用程序来记录和回放的输入和输出一个终端程序。它的主要目标是帮助开发和调试终端程序。其他项目如termrec / termplay,nethack-recorder /播放器和脚本/ scriptreplay只记录输出。然而,当调试一个交互式终端程序,输入通常是更重要的输出。...[更多Termdebug信息](http://www.oschina.net/p/termdebug)|LMDBG (Lightweight modular malloc debugger) 提供一组小工具用来收集和分析 malloc/realloc/memalign/free 函数调用的日志。LMDBG 不提供任何方式用来检测 malloc() 内存分配的超支分界。LMDBG 可检测内存泄漏和重复释放。LMDBG 可生成全堆栈日志。...[更多LMDBG信息](http://www.oschina.net/p/lmdbg)|ydb 是像gdb一样的php调试工具，用扩展实现，可以在运行过程中查看变量值，也可以进行性能测试，而不用对源代码进行任何更改，就像xdebug和xhprof 中提供的功能，但使用更方便     ydb也可以称为一个在线调试工具，什么叫在线调试？就是在线上生产...[更多ydb信息](http://www.oschina.net/p/ydb)|高性能计算/分布式计算等大量计算需要程序运行几天、几周甚至几个月，如果期间因为电力或者不可避免的问题导致程序中断会浪费大量的时间和人力，还 有超级计算机在这段时间里运行的电力成本。我们没有遇到过电力问题，不过我们最近遇到的场景是实验室需要做...[更多DMTCP信息](http://www.oschina.net/p/dmtcp)|bdwgc （Boehm-Demers-Weiser conservative garbage collector）是个垃圾收集器，用来替代 C malloc 或者是 C++ new。bdwgc 也可以用其他编程语言实现，但是要用 C 作为中间转换语言。bdwgc 也可以用来检测 C 或者 C++ 程序的漏洞。旧版的垃圾收集器中包括...[更多bdwgc信息](http://www.oschina.net/p/bdwgc)|libdwarf 可以在 ELF 对象文件中读或者写 DWARF 调试信息。dwarfdump 使用 libdwarf 来读对象文件和打印可读样式的 DWARF 内容。libdwarf 遵从了 DWARF2 及以上标准，源代码中包括一些树搜索（tsearch）算法（平衡，红黑和二进制）。...|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|PinkTrace是一款轻量级的C库文件，简化了编写应用程序跟踪程序的困难。它由围绕不同ptrace()请求的封装、用于解码参数 （strings, socket addresses）的API和一个实验性的用于编码参数掉的API构成。PinkTrace的目标是为跟踪程序提供一个跨平台的库。另外，...[更多PinkTrace信息](http://www.oschina.net/p/pinktrace)| | | | | | | | | | | | | | | | | | | |
|Shell 脚本调试器。 We have debuggers for bash, python, zsh, ksh, GNU make 3.81 and Python. In the patched sources for Make, we provide better error handling and tracing. GNU Emacs support is included for all debuggers....[](http://www.oschina.net/p/bashdb)[更多BASH Debugger信息](http://www.oschina.net/p/bashdb)| | | | | | | | | | | | | | | | | | | |
|一个高效的Linux 的Tracing Tools。它能够帮助发现并行环境下的性能问题及bug，甚至跨系统tracing也是可能的。[更多LTTng信息](http://www.oschina.net/p/lttng)| | | | | | | | | | | | | | | | | | | |
|ltrace 是一个调试程序，可在它退出之前执行指定命令来拦截和记录动态库调用和信号接收过程。它还可以拦截并打印系统调用。要跟踪的项目不需要重新编译,可以直接在二进制文件上使用。[更多ltrace信息](http://www.oschina.net/p/ltrace)| | | | | | | | | | | | | | | | | | | |
|KGTP 是一个 实时 轻量级 Linux 调试器 和 跟踪器 。 使用KGTP 不需要 在Linux内核上打PATCH或者重新编译，只要编译KGTP模块并insmod就可以。 其让Linux内核提供一个远程GDB调试接口，于是在本地或者远程的主机上的GDB可以在不需要停止内核的情况下用GDB t...[更多KGTP信息](http://www.oschina.net/p/kgtp)| | | | | | | | | | | | | | | | | | | |
|Debug Assistant Library (DAL) 是用于辅助调试的程序库，具有跨平台和统一接口的特点。 主要特性： assistant library for debugging, across platforms with uniform interface supports multi-compiler, support multi-platform Debug Message Asserti...[更多DAL信息](http://www.oschina.net/p/dal)| | | | | | | | | | | | | | | | | | | |
|TLSCatch是一款Ollydbg插件，它可以简单拦截当前进程地址空间的新模块加载操作，并从中搜索tlscallback。如果发现回调，则在每 个回调上设置一个一次性断点。在恶意软件分析中可以捕获任何tls callback。将 TLSCatch.dll拷贝到Ollydbg的插件目录就可以工作...[更多TLSCatch信息](http://www.oschina.net/p/tlscatch)| | | | | | | | | | | | | | | | | | | |
|libfiu是一个故障注入的C库。它提供用于标识“故障点”（“核心API”）里面的代码，功能启用/禁用这些点的失败（“控制API”）。核心API内部使用上要执行故障注入代码。控制API用于内部测试代码，以控制注入失败。 libfiu is a C library for fault inject...[更多libfiu信息](http://www.oschina.net/p/libfiu)| | | | | | | | | | | | | | | | | | | |
|lsstack是一个linux实现的的pstack工具，它原本在solaris系统上。 lsstack是一个命令行工具。 它能对指定的进程快速而高效地打印出其的线程和调用堆栈（call stack）。 以及函数在被ar的时候的符号名称。[更多lsstack信息](http://www.oschina.net/p/lsstack)| | | | | | | | | | | | | | | | | | | |
|vim-ruby-debugger 这是 vim 上调试 Ruby 程序的插件。[更多vim-ruby-debugger信息](http://www.oschina.net/p/vim-ruby-debugger)| | | | | | | | | | | | | | | | | | | |
|BVRDE是一款“Remote Compile IDE”，主要功能为：在windows上提供一套图形化的界面来编译和调试远程的系统，(当然，BVRDE也支持对本地系统的调试)。方便对GDB等调试工具不熟悉或者不喜欢的用户进行程序调试，提高工作效率。 BVRDE安装在Windows系统上，通...[](http://www.oschina.net/p/bvrde)[更多BVRDE信息](http://www.oschina.net/p/bvrde)| | | | | | | | | | | | | | | | | | | |
|Pafish是一个模拟了多个反调试(调试器/虚拟机/沙箱)技巧的demo工具。这些反调试技巧经常被恶意软件使用，干扰调试器工作或动态行为分析工作。(文/pulog)[](http://www.oschina.net/p/pafish)[更多Pafish信息](http://www.oschina.net/p/pafish)| | | | | | | | | | | | | | | | | | | |
|skpd 是一个用来将进程 dump 到一个可执行的 ELF 文件并执行。[更多skpd信息](http://www.oschina.net/p/skpd)| | | | | | | | | | | | | | | | | | | |
|Termdebug是一组实用程序来记录和回放的输入和输出一个终端程序。它的主要目标是帮助开发和调试终端程序。其他项目如termrec / termplay,nethack-recorder /播放器和脚本/ scriptreplay只记录输出。然而,当调试一个交互式终端程序,输入通常是更重要的输出。...[更多Termdebug信息](http://www.oschina.net/p/termdebug)| | | | | | | | | | | | | | | | | | | |
|LMDBG (Lightweight modular malloc debugger) 提供一组小工具用来收集和分析 malloc/realloc/memalign/free 函数调用的日志。LMDBG 不提供任何方式用来检测 malloc() 内存分配的超支分界。LMDBG 可检测内存泄漏和重复释放。LMDBG 可生成全堆栈日志。...[更多LMDBG信息](http://www.oschina.net/p/lmdbg)| | | | | | | | | | | | | | | | | | | |
|ydb 是像gdb一样的php调试工具，用扩展实现，可以在运行过程中查看变量值，也可以进行性能测试，而不用对源代码进行任何更改，就像xdebug和xhprof 中提供的功能，但使用更方便     ydb也可以称为一个在线调试工具，什么叫在线调试？就是在线上生产...[更多ydb信息](http://www.oschina.net/p/ydb)| | | | | | | | | | | | | | | | | | | |
|高性能计算/分布式计算等大量计算需要程序运行几天、几周甚至几个月，如果期间因为电力或者不可避免的问题导致程序中断会浪费大量的时间和人力，还 有超级计算机在这段时间里运行的电力成本。我们没有遇到过电力问题，不过我们最近遇到的场景是实验室需要做...[更多DMTCP信息](http://www.oschina.net/p/dmtcp)| | | | | | | | | | | | | | | | | | | |
|bdwgc （Boehm-Demers-Weiser conservative garbage collector）是个垃圾收集器，用来替代 C malloc 或者是 C++ new。bdwgc 也可以用其他编程语言实现，但是要用 C 作为中间转换语言。bdwgc 也可以用来检测 C 或者 C++ 程序的漏洞。旧版的垃圾收集器中包括...[更多bdwgc信息](http://www.oschina.net/p/bdwgc)| | | | | | | | | | | | | | | | | | | |
|libdwarf 可以在 ELF 对象文件中读或者写 DWARF 调试信息。dwarfdump 使用 libdwarf 来读对象文件和打印可读样式的 DWARF 内容。libdwarf 遵从了 DWARF2 及以上标准，源代码中包括一些树搜索（tsearch）算法（平衡，红黑和二进制）。...| | | | | | | | | | | | | | | | | | | |
分类: [Linux](http://www.cnblogs.com/lidabo/category/587288.html),[异常诊断与调试](http://www.cnblogs.com/lidabo/category/542683.html)
