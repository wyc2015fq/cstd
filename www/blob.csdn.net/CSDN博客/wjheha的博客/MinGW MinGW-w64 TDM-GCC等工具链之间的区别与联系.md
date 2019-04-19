# MinGW MinGW-w64 TDM-GCC等工具链之间的区别与联系 - wjheha的博客 - CSDN博客
2017年08月28日 14:51:45[wjheha](https://me.csdn.net/wjheha)阅读数：2303
不当之处，尽请指正。
先科普一下GNU相关:
![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)
**GNU计划**（英语：**GNU Project**），又译为**革奴计划**，是一个[自由软件](https://zh.wikipedia.org/wiki/%E8%87%AA%E7%94%B1%E8%BB%9F%E9%AB%94)[集体协作](https://zh.wikipedia.org/wiki/%E9%9B%86%E9%AB%94%E5%8D%94%E4%BD%9C)项目，1983年9月27日由[理查德·斯托曼](https://zh.wikipedia.org/wiki/%E7%90%86%E6%9F%A5%E5%BE%B7%C2%B7%E6%96%AF%E6%89%98%E6%9B%BC)在[麻省理工学院](https://zh.wikipedia.org/wiki/%E9%BA%BB%E7%9C%81%E7%90%86%E5%B7%A5%E5%AD%B8%E9%99%A2)公开发起。它的目标是创建一套完全[自由](https://zh.wikipedia.org/wiki/%E8%87%AA%E7%94%B1%E8%BD%AF%E4%BB%B6)的[操作系统](https://zh.wikipedia.org/wiki/%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F)，称为[GNU](https://zh.wikipedia.org/wiki/GNU)。理查德·斯托曼最早在net.unix-wizards[新闻组](https://zh.wikipedia.org/wiki/%E6%96%B0%E9%97%BB%E7%BB%84)上公布该消息，并附带一份《[GNU宣言](https://zh.wikipedia.org/wiki/GNU%E5%AE%A3%E8%A8%80)》等解释为何发起该计划的文章，其中一个理由就是要“重现当年软件界合作互助的团结精神”。
GNU是“**G**NU's **N**ot **U**nix”的[递归缩写](https://zh.wikipedia.org/wiki/%E9%80%92%E5%BD%92%E7%BC%A9%E5%86%99)。为避免与单词gnu（[非洲](https://zh.wikipedia.org/wiki/%E9%9D%9E%E6%B4%B2)[牛羚](https://zh.wikipedia.org/wiki/%E7%89%9B%E7%BE%9A)，发音与“new”相同）混淆，斯托曼宣布GNU发音应为“Guh-NOO”（[/ˈgnuː/](https://upload.wikimedia.org/wikipedia/commons/2/24/En-gnu.ogg)（[帮助](https://zh.wikipedia.org/wiki/Wikipedia:%E5%AA%92%E9%AB%94%E5%B9%AB%E5%8A%A9)·[关于](https://zh.wikipedia.org/wiki/File:En-gnu.ogg)）），与“canoe”发音相似。其中，[Emacs](https://zh.wikipedia.org/wiki/Emacs)就是由这个计划孵化而出。
[UNIX](https://zh.wikipedia.org/wiki/UNIX)是一种广泛使用的商业操作系统的名称。由于GNU将要实现UNIX系统的接口标准，因此GNU计划可以分别开发不同的操作系统。GNU计划采用了部分当时已经可自由使用的软件，例如[TeX](https://zh.wikipedia.org/wiki/TeX)排版系统和[X
 Window](https://zh.wikipedia.org/wiki/X_Window)视窗系统等。不过GNU计划也开发了大批其他的自由软件，这些软件也被移植到其他操作系统平台上，例如[Microsoft Windows](https://zh.wikipedia.org/wiki/Microsoft_Windows)、[BSD](https://zh.wikipedia.org/wiki/BSD)家族、[Solaris](https://zh.wikipedia.org/wiki/Solaris)及[Mac
 OS](https://zh.wikipedia.org/wiki/Mac_OS)。
为保证GNU软件可以自由地“使用、复制、修改和发布”，所有GNU软件都包含一份在禁止其他人添加任何限制的情况下，授权所有权利给任何人的协议条款，[GNU通用公共许可证](https://zh.wikipedia.org/wiki/GNU%E9%80%9A%E7%94%A8%E5%85%AC%E5%85%B1%E8%AE%B8%E5%8F%AF%E8%AF%81)（GNU
 General Public License，GPL）。这个就是被称为“[公共版权](https://zh.wikipedia.org/wiki/Copyleft)”的概念。GNU也针对不同场合，提供[GNU宽通用公共许可证](https://zh.wikipedia.org/wiki/GNU%E5%AE%BD%E9%80%9A%E7%94%A8%E5%85%AC%E5%85%B1%E8%AE%B8%E5%8F%AF%E8%AF%81)与[GNU自由文档许可证](https://zh.wikipedia.org/wiki/GNU%E8%87%AA%E7%94%B1%E6%96%87%E6%A1%A3%E8%AE%B8%E5%8F%AF%E8%AF%81)这两种协议条款。
![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)
**GNU**是一个[自由](https://zh.wikipedia.org/wiki/%E8%87%AA%E7%94%B1%E8%BB%9F%E9%AB%94)[操作系统](https://zh.wikipedia.org/wiki/%E4%BD%9C%E6%A5%AD%E7%B3%BB%E7%B5%B1)，其内容软件完全以[GPL](https://zh.wikipedia.org/wiki/GPL)方式发布。这个操作系统是[GNU计划](https://zh.wikipedia.org/wiki/GNU%E8%A8%88%E5%8A%83)的主要目标，名称来自GNU's
 Not Unix!的[递归缩写](https://zh.wikipedia.org/wiki/%E9%81%9E%E8%BF%B4%E7%B8%AE%E5%AF%AB)，因为GNU的设计[类似Unix](https://zh.wikipedia.org/wiki/%E7%B1%BBUnix)，但它不包含具著作权的Unix代码。GNU的创始人，[理察·马修·斯托曼](https://zh.wikipedia.org/wiki/%E7%90%86%E5%AF%9F%C2%B7%E9%A6%AC%E4%BF%AE%C2%B7%E6%96%AF%E6%89%98%E6%9B%BC)，将GNU视为“达成社会目的技术方法”。
作为操作系统，GNU的发展仍未完成，其中最大的问题是具有完备功能的内核尚未被开发成功。GNU的内核，称为[Hurd](https://zh.wikipedia.org/wiki/GNU_Hurd)，是[自由软件基金会](https://zh.wikipedia.org/wiki/%E8%87%AA%E7%94%B1%E8%BB%9F%E9%AB%94%E5%9F%BA%E9%87%91%E6%9C%83)发展的重点，但是其发展尚未成熟。在实际使用上，多半使用[Linux内核](https://zh.wikipedia.org/wiki/Linux%E5%85%A7%E6%A0%B8)、[FreeBSD](https://zh.wikipedia.org/wiki/FreeBSD)等替代方案，作为系统核心，其中主要的操作系统是Linux的发行版。[Linux](https://zh.wikipedia.org/wiki/Linux)操作系统包涵了[Linux内核](https://zh.wikipedia.org/wiki/Linux%E5%85%A7%E6%A0%B8)与其他自由软件项目中的GNU组件和软件，可以被称为[GNU/Linux](https://zh.wikipedia.org/wiki/GNU/Linux)（见[GNU/Linux命名争议](https://zh.wikipedia.org/wiki/GNU/Linux%E5%91%BD%E5%90%8D%E7%88%AD%E8%AD%B0)）。
![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)
自由软件基金会（英语：**Free Software Foundation**，**FSF**）是一个致力于推广[自由软件](https://zh.wikipedia.org/wiki/%E8%87%AA%E7%94%B1%E8%BB%9F%E4%BB%B6)的[美国](https://zh.wikipedia.org/wiki/%E7%BE%8E%E5%9C%8B)民间非营利性[组织](https://zh.wikipedia.org/wiki/%E7%B5%84%E7%B9%94)。它于1985年10月由[理查德·斯托曼](https://zh.wikipedia.org/wiki/%E7%90%86%E6%9F%A5%E5%BE%B7%C2%B7%E6%96%AF%E6%89%98%E6%9B%BC)建立。其主要工作是运行[GNU](https://zh.wikipedia.org/wiki/GNU)计划，开发更多的自由软件。
![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)
**GNU编译器套装**（英语：GNU Compiler Collection，缩写为**GCC**），一套[编程语言](https://zh.wikipedia.org/wiki/%E7%B7%A8%E7%A8%8B%E8%AA%9E%E8%A8%80)[编译器](https://zh.wikipedia.org/wiki/%E7%BC%96%E8%AF%91%E5%99%A8)，以[GPL](https://zh.wikipedia.org/wiki/GPL)及[LGPL](https://zh.wikipedia.org/wiki/LGPL)许可证所发行的[自由软件](https://zh.wikipedia.org/wiki/%E8%87%AA%E7%94%B1%E8%BB%9F%E9%AB%94)，也是[GNU项目](https://zh.wikipedia.org/wiki/GNU%E8%A8%88%E5%8A%83)的关键部分，也是[GNU工具链](https://zh.wikipedia.org/wiki/GNU%E5%B7%A5%E5%85%B7%E9%93%BE)的主要组成部分之一。GCC（特别是其中的C语言编译器）也常被认为是跨平台编译器的事实标准。1985年由[理查德·马修·斯托曼](https://zh.wikipedia.org/wiki/%E7%90%86%E6%9F%A5%E5%BE%B7%C2%B7%E9%A9%AC%E4%BF%AE%C2%B7%E6%96%AF%E6%89%98%E6%9B%BC)开始发展，现在由[自由软件基金会](https://zh.wikipedia.org/wiki/%E8%87%AA%E7%94%B1%E8%BB%9F%E9%AB%94%E5%9F%BA%E9%87%91%E6%9C%83)负责维护工作。
原名为**GNU C语言编译器**（**GNU C Compiler**），因为它原本只能处理[C语言](https://zh.wikipedia.org/wiki/C%E8%AA%9E%E8%A8%80)。GCC很快地扩展，变得可处理[C++](https://zh.wikipedia.org/wiki/C%2B%2B)。之后也变得可处理[Fortran](https://zh.wikipedia.org/wiki/Fortran)、[Pascal](https://zh.wikipedia.org/wiki/Pascal_(%E7%A8%8B%E5%BC%8F%E8%AA%9E%E8%A8%80))、[Objective-C](https://zh.wikipedia.org/wiki/Objective-C)、[Java](https://zh.wikipedia.org/wiki/Java)、[Ada](https://zh.wikipedia.org/wiki/Ada)，以及[Go](https://zh.wikipedia.org/wiki/Go)与其他语言。
许多操作系统，包括许多[类Unix](https://zh.wikipedia.org/wiki/%E7%B1%BBUnix)系统，如[Linux](https://zh.wikipedia.org/wiki/Linux)及BSD家族都采用GCC作为标准编译器。苹果电脑[Mac
 OS X](https://zh.wikipedia.org/wiki/Mac_OS_X)[操作系统](https://zh.wikipedia.org/wiki/%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F)也采用这个编译器。
GCC原本用C开发，后来因为[LLVM](https://zh.wikipedia.org/wiki/LLVM)、[Clang](https://zh.wikipedia.org/wiki/Clang)的崛起，它更快地将开发语言转换为C++。许多C的爱好者在对C++一知半解的情况下主观认定C++的性能一定会输给C，但是Taylor给出了不同的意见，并表明C++不但性能不输给C，而且能设计出更好，更容易维护的程序。
![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)
**MinGW**（**Min**imalist **G**NU for **W**indows），又称**mingw32**，是将[GCC](https://zh.wikipedia.org/wiki/GCC)编译器和[GNU
 Binutils](https://zh.wikipedia.org/wiki/GNU_Binutils)移植到Win32平台下的产物，包括一系列头文件（[Win32API](https://zh.wikipedia.org/wiki/Windows_API)）、[库](https://zh.wikipedia.org/wiki/%E9%9D%9C%E6%85%8B%E9%80%A3%E7%B5%90%E5%87%BD%E5%BC%8F%E5%BA%AB)和[可执行文件](https://zh.wikipedia.org/wiki/%E5%8F%AF%E6%89%A7%E8%A1%8C%E6%96%87%E4%BB%B6)。
另有可用于产生32位及64位Windows可执行文件的**MinGW-w64**项目，是从原本MinGW产生的分支[[1]](https://zh.wikipedia.org/wiki/MinGW#cite_note-1)。如今已经独立发展
GCC支持的语言大多在MinGW也受支持，其中涵盖[C](https://zh.wikipedia.org/wiki/C%E8%AA%9E%E8%A8%80)、[C++](https://zh.wikipedia.org/wiki/C%2B%2B)、[Objective-C](https://zh.wikipedia.org/wiki/Objective-C)、[Fortran](https://zh.wikipedia.org/wiki/Fortran)及[Ada](https://zh.wikipedia.org/wiki/Ada)。
对于C语言之外的语言，MinGW使用标准的GNU[运行库](https://zh.wikipedia.org/wiki/%E8%BF%90%E8%A1%8C%E6%97%B6%E5%BA%93)，如C++使用GNU [libstdc++](https://zh.wikipedia.org/wiki/Libstdc%2B%2B)。
但是MinGW使用Windows中的C运行库。因此用MinGW开发的程序不需要额外的第三方[DLL](https://zh.wikipedia.org/wiki/%E5%8A%A8%E6%80%81%E9%93%BE%E6%8E%A5%E5%BA%93)支持就可以直接在Windows下运行，而且也不一定必须遵从[GPL](https://zh.wikipedia.org/wiki/GPL)许可证。这同时造成了MinGW开发的程序只能使用Win32API和跨平台的第三方库，而缺少POSIX支持[[3]](https://zh.wikipedia.org/wiki/MinGW#cite_note-3)，大多数GNU软件无法在不修改源代码的情况下用MinGW编译。
MinGW-w64是2005年由OneVision Software根据[洁净室设计原则](https://zh.wikipedia.org/w/index.php?title=%E6%B4%81%E5%87%80%E5%AE%A4%E8%AE%BE%E8%AE%A1%E5%8E%9F%E5%88%99&action=edit&redlink=1)而独立开发，以解决当时的MinGW项目更新缓慢，缺少新的重要的API以及64位支持。2008年MinGW-w64项目开源，由Kai
 Tietz维护。
- [SourceForge.net的MingW64官网提供https://sourceforge.net/projects/mingw-w64/](https://zh.wikipedia.org/wiki/SourceForge)上的提供由MinGW-Builds等项目组制作的预编译版。编译器分64位版和32位版、分别包含sjlj、drawf、seh异常处理机制、分别支持POSIX线程模式或Win32线程模式。用户可根据需要下载不同版本。例如，64位的seh、POSIX线程的最新稳定版本的gcc包的下载目录为：Toolchains
 targetting Win64 --> Personal Builds --> mingw-builds-->4.9.2-->threads-posix-->seh
![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)
相同点：
一、 他们都是用于编译生成Windows应用程序的工具链。
二、 他们都是基于gcc的。
不同之处（只说大的）：
一、项目之间的关系
1. MinGW应该是最先诞生的。
2. MinGW-w64项目最初是做为MinGW的一个分支而诞生，但现在已经独立发展。详细信息参见：
[http://sourceforge.net/apps/trac/mingw-w64/wiki/History](http://sourceforge.net/apps/trac/mingw-w64/wiki/History)
[http://zh.wikipedia.org/wiki/MinGW](http://zh.wikipedia.org/wiki/MinGW)
3. TDM-GCC及其他类似的编译器集成包
 TDM-GCC是非官方组织提供及维护的编译器集成包。换句话说，他不是由MinGW或MinGW-w64项目官方提供的编译器集成包。
但他基于MinGW及MinGW-w64。而且，他也有他的优点：
例如，TDM-GCC集成了：
最新的稳定版本的GCC工具集，
 一些实用的补丁，
MinGW及MinGW-w64中的运行时API。
详细信息，参考：[http://tdm-gcc.tdragon.net/about](http://tdm-gcc.tdragon.net/about)
除了TDM-GCC之外，类似的非官方编译器集成包有
Boost Science Package 、TakeoffGW等（其中大多基于MingGW-w64项目）。
更多编译器集成包参见：[http://zh.wikipedia.org/wiki/MinGW](http://zh.wikipedia.org/wiki/MinGW)
二、功能差别
1.  MinGW
貌似只能编译生成win32程序。
2.  MinGW-w64
支持编译生成Win32与Win64的windows程序。
3. TDM-GCC及其他非官方的编译器集成包
看他们基于什么项目。多数估计能支持编译生成Win32与Win64的windows程序。
三、运行平台
1.  MinGW
只能运行在windows平台。
2.  MinGW-w64
在Win32、Win64、[Linux](http://lib.csdn.net/base/linux) 32、[linux](http://lib.csdn.net/base/linux) 64等主流平台上都有版本可用。
详见：[http://mingw-w64.sourceforge.net/download.php](http://mingw-w64.sourceforge.net/download.php)
3. TDM-GCC及其他非官方的编译器集成包
没细考证过。估计多半是运行在windows平台。
