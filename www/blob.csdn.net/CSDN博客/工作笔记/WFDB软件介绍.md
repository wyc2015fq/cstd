# WFDB软件介绍 - 工作笔记 - CSDN博客





2012年01月17日 14:05:17[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6935








## What is the WFDB Software Package?

Effective use of PhysioBank data requires specialized software. 

PhysioBank 数据的有效使用需要专门的软件。 



 We have developed a large collection of such software over the past twenty years, and much of it is contained within the WFDB (WaveForm DataBase) Software Package, the
[GPL](http://www.physionet.org/physiotools/wfdb/COPYING)ed successor to the MIT
[DB Software Package](http://www.physionet.org/physiotools/old/). 

在过去的20年里，我们开发了一个很大的这样的软件，多数包含在WFDB（波形数据库）软件包中，它是MIT 数据库软件包的GPL协议下的 后继。（DB Software Package是WFDB 的前身）




The major components of the WFDB Software Package are the [WFDB library](http://www.physionet.org/physiotools/wfdb.shtml#library), the [WFDB applications](http://www.physionet.org/physiotools/wfdb.shtml#applications) for signal processing and automated analysis, and the [*WAVE*](http://www.physionet.org/physiotools/wfdb.shtml#WAVE) software for viewing, annotation, and interactive analysis of waveform data.


WFDB 软件包的主要组成是WFDB库，用于信号处理和自动分析的WFDB应用程序，以及用于可视化、注释、波形数据交互式分析的软件WAVE. 



A comprehensive collection of [documentation](http://www.physionet.org/physiotools/manuals.shtml), including tutorials and reference manuals, is also included in the package.


一个全面的文档集合，包括教程和参考手册，也包含在软件包中



The package is frequently updated; a summary of recent changes can be found [here](http://www.physionet.org/physiotools/wfdb/NEWS). 

软件包经常被更新，近期所做更改的汇总可在这里找到



The package is written in highly portable C and can be used on all popular platforms, including GNU/Linux, MacOS/X, MS-Windows, and all versions of Unix.


该软件包是用高度可移植的C语言编写的，可以在所有流行的平台上使用，包括GNU/Linux，MacOS/ X，MS-Windows，及Unix的所有版本。

### WFDB library

This is a [set of functions](http://www.physionet.org/physiotools/wfdb/lib) (subroutines) for reading and writing files in the formats used by PhysioBank databases (among others). The WFDB library is
[LGPL](http://www.physionet.org/physiotools/wfdb/lib/COPYING.LIB)ed, and can be used by programs written in ANSI/ISO C, K&R C, C++, or Fortran, running under any operating system for which an ANSI/ISO or K&R C compiler is available, including all
 versions of Unix, MS-DOS, MS-Windows, the Macintosh OS, and VMS. 

这是一个函数子集（子程序）用来读写 PhysioBank数据库使用格式（以及其它数据库） 文件。WFDB库满足LGPL协议，能够被ANSI/ISO C,K&R C, C++或Fortran语言所写的程序使用，能运行在任何可以使用ANSI/ISO或K&R编译器的操作系统上，包括所有版本的Unix，MS-DOS, MS-Windows, the Macintosh OS,  VMS.




Optionally, the WFDB library may be compiled with support for reading input directly from web (HTTP) and FTP servers without the use of a web browser or an FTP client. This optional feature allows applications linked with the WFDB library to view or analyze
 data such as those available from [PhysioBank](http://www.physionet.org/physiobank/) without the need to download entire records and to store them locally. To enable this feature, you will also need either the World Wide Web Consortium's
[libwww](http://www.physionet.org/physiotools/libwww/) package, or the modern and enhanced alternative,
[libcurl](http://www.physionet.org/physiotools/libcurl).


可供选择地，WFDB库可以被编译成  支持从网络（HTTP）和FTP服务器直接读入，不需要使用网页浏览器或FTP客户端。这个可选特征允许连接了WFDB的应用程序 能够观察或分析 像PhysioBank中那样的 数据 ，不需要下载整个记录并保存它们到本地。要想使用这个特征，你也将需要
万维网联盟的libwww包，或现代强化版的libcurl。



Two optional packages provide interfaces between the WFDB library and software written in a variety of other languages, so that such software can have access to the full range of capabilities supported by current and future versions of the WFDB library.


The [WFDB Toolbox for Matlab](http://www.physionet.org/physiotools/matlab/wfdb-swig-matlab/) provides this interface for Matlab 7 (R14, 2004) or later, and the
[wfdb-swig](http://www.physionet.org/physiotools/wfdb-swig.shtml) package provides interfaces for software written using Perl, Python, C# (and other .NET languages), and Java. The wfdb-swig package can be readily extended for use with other target
 languages supported by [SWIG](http://www.swig.org/), such as PHP, Ruby, TCL, and several versions of Lisp.


两个可选软件包提供WFDB库和 多种其它语言写的软件 的接口，因此这样的软件能使用 当前和未来版本的WFDB库支持的全部功能。 

用于Matlab的WFDB工具箱为Matlab 7 （R14，2004）或更高版本 提供这样的接口，wfdb-swig 包 为Perl, Python, C# (其它 .NET 语言),  Java  写的软件 提供接口。wfdb-swig 包能容易地进行扩展，使之用于其它支持SWIG的目标语言，例如PHP, Ruby, TCL, 诸多版本的 Lisp。




The WFDB library is documented by the *WFDB Programmer's Guide* (available as
[HTML](http://www.physionet.org/physiotools/wpg/wpg.htm), [PostScript](http://www.physionet.org/physiotools/wpg/wpg.ps), or [PDF](http://www.physionet.org/physiotools/wpg/wpg.pdf)). Texinfo sources for the
*WFDB Programmer's Guide* are included in the [doc](http://www.physionet.org/physiotools/wfdb/doc/) directory of the WFDB Software Package.


 WFDB 库配备的文档是*WFDB Programmer's Guide（有HTML,PS,PDF格式）。*WFDB Programmer's Guide* 的 Tex 源码包含在WFDB软件包的doc目录中。*

### WFDB applications

A large set of well-tested, interoperable command-line tools for signal processing and automated analysis is included in the
[app](http://www.physionet.org/physiotools/wfdb/app/),
[convert](http://www.physionet.org/physiotools/wfdb/convert/), and
[psd](http://www.physionet.org/physiotools/wfdb/psd/) directories of the WFDB Software Package.


一大批经良好测试，交互性的 用于信号处理和自动分析的 命令行工具 包含在WFDB软件包的app，convert，psd目录中。 



These applications are described in the *WFDB Applications Guide* (available as
[HTML](http://www.physionet.org/physiotools/wag/wag.htm), [PostScript](http://www.physionet.org/physiotools/wag/wag.ps), or [PDF](http://www.physionet.org/physiotools/wag/wag.pdf)). The
troff (Unix man page) sources for the
*WFDB Applications Guide* are included in the [doc](http://www.physionet.org/physiotools/wfdb/doc/) directory of the WFDB Software Package.


这些应用程序在文档WFDB Applications Guide中有描述（*有HTML,PS,PDF格式*）。WFDB Applications Guide 的 troff（Unix 主页）源码包含在WFDB软件包的doc目录中。



Two American National Standards, ANSI/AAMI EC38:1998 (Ambulatory Electrocardiographs) and ANSI/AAMI EC57:1998 (Testing and Reporting Performance Results of Cardiac Rhythm and ST Segment Measurement Algorithms) require the use of several of the WFDB applications
 for evaluation of certain devices and algorithms. For details, see [Evaluating ECG Analyzers](http://www.physionet.org/physiotools/wag/%20.htm) in the *WFDB Applications Guide*. 

两个美国国家标准， ANSI/AAMI EC38:1998 (动态心电图机) 和 ANSI/AAMI EC57:1998 (测试和报告心脏节律和ST段测量算法的性能结果)要求使用几个WFDB应用程序来评估某些设备和算法。详情请见WFDB Application Guide 中的[Evaluating ECG Analyzers](http://www.physionet.org/physiotools/wag/%20.htm)（ECG分析器评估）。

### *WAVE* for GNU/Linux, FreeBSD, Mac OS/X, MS-Windows, Solaris, and SunOS

*WAVE* is an extensible interactive graphical environment for manipulating sets of digitized signals with optional annotations.
*WAVE* is built using the WFDB library developed for physiologic signal processing, so it can be applied to any of a wide variety of data formats supported by the WFDB library.
*WAVE* can run on GNU/Linux or FreeBSD PCs, Mac OS X, MS-Windows PCs, and Sun workstations, or on any other systems to which the open-source XView toolkit has been ported; in addition, it can be accessed remotely using networked PCs, Macintoshes, or
 other systems for which X11 servers are available. 

WAVE  是一个可扩展的交互式图形环境，用来操作带有可选注释的数字信号。WAVE是建立在WFDB库基础上，所以它可以被应用到WFDB库支持的多种数据格式。WAVE 能运行在  GNU/Linux 或 FreeBSD PCs, Mac OS X, MS-Windows PCs,  Sun workstations, 或任意其它移植了开源XView 工具箱的系统。 除此之外，它能被联网的PC，Macintoshes,  或其它可以用X11服务器的系统 远程访问。




Among *WAVE*'s capabilities are: 

WAVE包含的功能有： 


- fast display of waveforms and annotations at various calibrated scales







          波形和注释以多种校准尺度快速显示 
- fast access to any portion of a recording, with caching and `read-ahead' heuristics to improve efficiency and reduce network traffic


         快速访问记录的任意位置，利用缓存和‘预读’启发式方法  提高效率并减少网络流量 
- forward and backward searches for annotation patterns 

       注释模式向前和向后搜索
- graphical annotation editing using standard or user-defined annotations

           图形化的注释编辑，使用标准的或是用户定义的注释方法 
- variable-speed superimposition display (simulation of triggered oscilloscope display with persistence; can be run forward or backward)

- 变速叠加显示（模拟 触发式示波器持续性的显示；能向前或向后运行） 
- high-resolution printing of user-selected signal segments 

          用户选择的信号片段的高精度打印
- extremely flexible control of external signal-processing and analysis programs (menus may be reconfigured by the user while
*WAVE* is running) 

极其灵活的控制外部信号处理和分析程序（菜单可以由用户在WAVE运行时重新配置）
- `remote' mode: external programs such as Web browsers can control *WAVE*'s display

           远程模式：外部程序，例如网页浏览器能控制WAVE 的显示 
- on-line `spot' help for all controls, with additional topic-oriented on-line help

         对所有控制  现场式 在线帮助 ，还有面向其它主题的在线帮助



A complete set of sources for *WAVE* is included in the [wave](http://www.physionet.org/physiotools/wfdb/wave/) directory of the WFDB Software Package.


WAVE 的完整源码包含在 WFDB软件包的wave目录



The *WAVE User's Guide* (available as [HTML](http://www.physionet.org/physiotools/wug/wug.htm), [PostScript](http://www.physionet.org/physiotools/wug/wug.ps), or
[PDF](http://www.physionet.org/physiotools/wug/wug.pdf)) contains both tutorial and reference material.


文档*WAVE User's Guide（*有HTML,PS,PDF格式*）包含教程和参考资料*



The LaTeX source for the *WAVE User's Guide* is included in the [doc](http://www.physionet.org/physiotools/wfdb/doc/) directory of the WFDB Software Package.

*WAVE User's Guide* 的LaTex 源码 包含在WFDB软件包的doc目录



For convenience, XView sources are available [here](http://www.physionet.org/physiotools/xview/), as are XView binaries for GNU/Linux, Mac OS X, and MS-Windows, instructions for installing them, as well as information about how to find or create XView libraries for FreeBSD, Solaris, and other versions of Unix.

为方便，XView 源码 在这里可以获得，以及 XView 的 用于GNU/Linux, Mac OS X, MS-Windows的二进制形式源码，安装说明，以及对于FreeBSD, Solaris, 其它版本的Unix 如何寻找或创建XView 库的信息



 The source and binary packages contain the xview and
olgx libraries needed by *WAVE*.


源代码和二进制包 包含WAVE 需要的 xview 和 olgx 库。 

## Downloading

**Quick start guides** are available for [FreeBSD](http://www.physionet.org/physiotools/wfdb-freebsd-quick-start.shtml), [GNU/Linux](http://www.physionet.org/physiotools/wfdb-linux-quick-start.shtml), [Mac OS/X (Darwin)](http://www.physionet.org/physiotools/wfdb-darwin-quick-start.shtml), [MS-Windows 95/98/ME/NT/2000/XP](http://www.physionet.org/physiotools/wfdb-windows-quick-start.shtml), and [Solaris](http://www.physionet.org/physiotools/wfdb-solaris-quick-start.shtml). (If your platform is not one of these, install the package from sources, following the procedure in the
[GNU/Linux](http://www.physionet.org/physiotools/wfdb-linux-quick-start.shtml) quick start guide.)


快速入门指南  是针对FreeBSD, GNU/Linux, Mac OS/X (Darwin), MS-Windows 95/98/ME/NT/2000/XP,  Solaris 系统。（如果你的平台不是这些，从源代码进行安装，按照GNU/Linux 快速入门指南中的步骤）




**Sources:** The current version of the WFDB Software Package (most recently updated on Saturday, 12 March 2011 at 16:45 EST) may be downloaded in source form, as a
[compressed tar archive](http://www.physionet.org/physiotools/wfdb.tar.gz)
(1.9M). A [shorter version](http://www.physionet.org/physiotools/wfdb-no-docs.tar.gz) (779K), without the documentation, is also available. (WinZip users, please read about how to unpack
.tar.gz archives in the [FAQ](http://www.physionet.org/faq.shtml#tar-gz).) You may also browse through the [source tree](http://www.physionet.org/physiotools/wfdb/) to read or download individual files. 

源代码： WFDB软件包当前版本能以源码的形式下载（最近一次更新是在周六，2011年3月12日美国东部时间16:45），是压缩的tar目录（1.9M). 还有一个可以用的短的版本（779K），不包含文挡。（WinZip 用户，请阅读FAQ 了解如何解压.tar.gz 目录。）你也可以通过源代码目录树阅读或下载单个文件，




Development snapshots of the WFDB Software Package may be available as [beta software](http://www.physionet.org/physiotools/beta/). 

WFDB软件包的开发快照 可当成 beta 软件来用（预发布软件）。 



Beta software is available in source form only, to those with the skills, patience, and willingness to contribute to the final stages of the process of testing and debugging upcoming releases of PhysioToolkit software.


Beta 软件只能以源代码形式提供给那些技术熟练，有耐心的人，他们测试和调试即将发布的PhysioToolkit 软件， 有意愿在这一过程的最后阶段作出贡献。




Don't use beta software if you need support! 

如果你使用软件的过程中需要支持，就不要用beta软件。 



Older stable versions of the WFDB Software Package are also available in the [PhysioToolkit Archives](http://www.physionet.org/physiotools/archives/). 

 早期的稳定的WFDB软件包也可以在PhysioToolkit目录中获得。 



[**Ready-to-run, precompiled **](/)[binaries](http://www.physionet.org/physiotools/binaries/) are available for several popular operating systems.


可直接运行的，供几个流行的操作系统使用的 已编译的二进制包也可以获得。 



Please read the quick start guide for your platform before installing any of these binaries! Contributions of binaries for other operating systems are welcome; please write to us first.


在安装任何一个二进制包之前，请阅读针对你的平台的快速入门指南。欢迎提供供其它操作系统使用的二进制形式软件包；请先给我们写信。 



**Documentation**: A comprehensive set of
[tutorials and reference manuals](http://www.physionet.org/physiotools/manuals.shtml) can be read on-line or downloaded and printed.


文档：  一整套教程和参考手册可在线阅读或下载和打印。



