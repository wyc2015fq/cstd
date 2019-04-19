# shell--指“提供使用者使用界面”的软件（命令解析器） - 三少GG - CSDN博客
2010年04月27日 21:28:00[三少GG](https://me.csdn.net/scut1135)阅读数：1261标签：[shell																[bash																[windows																[powershell																[语言																[login](https://so.csdn.net/so/search/s.do?q=login&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=powershell&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=bash&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
在计算机科学中，Shell俗称壳（用来区别于核），是指“提供使用者使用界面”的软件（命令解析器）。它类似于DOS下的command.com。它接收用户命令，然后调用相应的应用程序。同时它又是一种程序设计语言。作为命令语言，它交互式解释和执行用户输入的命令或者自动地解释和执行预先设定好的一连串的命令；作为程序设计语言，它定义了各种变量和参数，并提供了许多在高阶语言中才具有的控制结构，包括循环和分支。在C++中，Shell是[希尔排序](http://writeblog.csdn.net/view/178698.htm)的名称。
**基本上shell分两大类：　　一：图形界面shell（Graphical User Interface shell 即 GUI shell）**例如：应用最为广泛的 Windows Explorer （微软的windows系列制作系统），还有也包括广为人知的 Linux shell，其中linux shell 包括 X windows manger (BlackBox和FluxBox)，以及功能更强大的CDE、GNOME、KDE、 XFCE。
**二：命令行式shell（Command Line Interface shell ，即CLI shell）**
　　例如：
**bash / sh / ksh / csh（Unix/linux 系统）**
　　COMMAND.COM（MS-DOS 系统）
[cmd.exe](http://writeblog.csdn.net/view/449679.htm)/ 命令提示字符（Windows NT 系统）
　　Windows PowerShell（支援 .NET Framework 技术的 Windows NT 系统）
　　传统意义上的shell指的是命令行式的shell，以后如果不特别注明，shell是指命令行式的shell。
文字[操作系统](http://writeblog.csdn.net/view/880.htm)与外部最主要的接口就叫做shell。shell是操作系统最外面的一层。shell管理你与操作系统之间的交互:等待你输入，向操作系统解释你的输入，并且处理各种各样的操作系统的输出结果。
**shell提供了你与操作系统之间通讯的方式。这种通讯可以以交互方式（从键盘输入，并且可以立即得到响应），或者以shell script(非交互)方式执行。shell script是放在文件中的一串shell和操作系统命令，它们可以被重复使用。本质上，shell script是命令行命令简单的组合到一个文件里面。**
　　Shell基本上是一个命令解释器，类似于[DOS](http://writeblog.csdn.net/view/365.htm)下的command.com。它接收用户命令（如ls等)，然后调用相应的应用程序。较为通用的shell有标准的Bourne shell (sh)和C shell (csh)。
交互式shell和非交互式shell
　　交互式模式就是shell等待你的输入，并且执行你提交的命令。这种模式被称作交互式是因为shell与用户进行交互。这种模式也是大多数用户非常熟悉的：登录、执行一些命令、签退。当你签退后，shell也终止了。
　　shell也可以运行在另外一种模式：非交互式模式。在这种模式下，shell不与你进行交互，而是读取存放在文件中的命令,并且执行它们。当它读到文件的结尾，shell也就终止了。
　　shell的类型
　　在[UNIX](http://writeblog.csdn.net/view/8095.htm)中主要有两大类shell
　　Bourne shell (包括 sh, ksh, and bash)
　　Bourne shell ( sh)
　　Korn shell ( ksh)
　　Bourne Again shell ( bash)
　　POSIX shell ( sh)
　　C shell (包括 csh and tcsh)
　　C shell ( csh)
　　TENEX/TOPS C shell ( tcsh)
　　Bourne Shell
　　首个重要的标准Unix Shell是1970年底在V7 Unix(AT&T第7版)中引入的，并且以它的创始科技部基础条件平台“国家气象网络计算应用节点建设”(2004DKA50730)资助者Stephen Bourne的名字命名。Bourne shell 是一个交换式的命令解释器和命令编程语言。Bourne shell 可以运行为login shell或者login shell的子shell(subshell)。只有login命令可以调用Bourne shell作为一个login shell。此时，shell先读取/etc/profile文件和$HOME/.profile文件。/etc/profile文件为所有的用户定制环境,$HOME/.profile文件为本用户定制环境。最后，shell会等待读取你的输入。
　　C Shell
　　Bill Joy于20世纪80年代早期，在Berkeley的加利福尼亚大学开发了C shell。它主要是为了让用户更容易的使用交互式功能，并把ALGOL风格的语法结构变成了C语言风格。它新增了命令历史、别名、文件名替换、作业控制等功能。
　　Korn Shell
　　有很长一段时间，只有两类shell供人们选择，Bourne shell用来编程，C shell用来交互。为了改变这种状况，AT&T的bell实验室David Korn开发了Korn shell。ksh结合了所有的C shell的交互式特性，并融入了Bourne shell的语法。因此，Korn shell广受用户的欢迎。它还新增了数学计算,进程协作(coprocess)、行内编辑(inline editing)等功能。Korn Shell 是一个交互式的命令解释器和命令编程语言.它符合POSIX——一个操作系统的国际标准.POSIX不是一个操作系统,而是一个目标在于应用程序的移植性的标准——在源程序一级跨越多种平台。
　　Bourne Again Shell (bash)
bash是GNU计划的一部分，用来替代Bourne shell。它用于基于GNU的系统如Linux.大多数的Linux(Red Hat, Slackware, Caldera)都以bash作为缺省的shell，并且运行sh时，其实调用的是bash。
　　POSIX Shell
　　POSIX shell 是Korn shell的一个变种. 当前提供POSIX shell的最大卖主是Hewlett-Packard。在HP-UX 11.0 , POSIX shell 就是/bin/sh,而bsh是/usr/old/bin/sh.
