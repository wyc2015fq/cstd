# Linus 定义 Linux - 文章 - 伯乐在线
原文出处： [Linus Torvalds](http://oldlinux.org/Linus/)   译文出处：[开源中国](https://www.oschina.net/translate/linux-a-free-unix-386-kernel)
# LINUX介绍
## LINUX是什么？
LINUX是一个免费类unix内核，适用于386-AT计算机，附带完整源代码。主要让黑客、计算机科学学生使用，学习和享受。它大部分用C编写，但是一小部分是用gnu格式汇编，而且引导序列用的是因特尔086汇编语言。C代码是相对ANSI的，使用一些GNU增强特性（大多为 __asm__ 和 inline）。
然而有很多可用于386电脑的unices，他们大部分要花很多钱，而且不附带源码。因此他们是使用计算机的理想选择，但是如果你想了解他们如何工作，那是不可能的。
也有一些  Unix 是附带源码的。Minix，Andrew S. Tanenbaum编写的学习工具，已经在大学中作为教学工具使用了很多年了。BSD-386系统是附带源码的，但是有版权限制，而且要花很多钱（我记得起始价格为$995）。GNU内核（Hurd）将会是免费的，但是现在还没有准备好，而且对于了解和学习它们来说有点庞大。
LINUX与Minix是最相似的，由于它很小而且不是非常复杂，因此易于理解（嗯…）。LINUX是基于Minix编写的，因此有相当多的相同点，任何Minix黑客在使用LINUX的时候都感觉非常熟悉。不过，没有在项目中使用Minix代码，因此Minix版权没有限制到这个新系统。它也是完全免费的，而且它的版权非常宽松。因此不像使用Minix，它不需要几兆字节大小的区别。
## LINUX版权
虽然是免费的发布版，我还是从以下几个方面限制了LINUX的使用：
- 你可以自由复制和重新发布源码和二进制，只要是：
- 完全开源。因此不能单独发布二进制，即使你只修改了一点。
- 你不能从发布版获取利益。事实上甚至“装卸费用”都是不被接受的。
- 你要保持完整的适当版权。
- 根据需要你可能会修改源码，但是如果你发布了新系统的一部分（或者只有二进制），必须将新的代码包含进去。
- 除了不包含版权的代码之外，你可能会做一些小的修改。这由你来定，但是如果能将相关内容或者代码告诉我，将不胜感激。
对任何使用或者扩展系统的人来说，这应该足够宽松而不会引起任何担忧。如果你有朋友真的不想要源码，只想要一个能运行的二进制，你当然可以给他而不用担心我会起诉你。不过最好只在朋友之间这么做。
## LINUX运行所需的硬件/软件
LINUX是在一个运行Minix的386-AT上开发的。由于LINUX是一个真正的操作系统，而且需要直接与硬件交互来做一些事情，你必须有一个非常相似的系统来让他顺利运行：
- 386-AT（PS/2之类是不同的，不能正常运行）
- VGA或者EGA屏幕硬件。
- 标准AT硬盘接口，IDE盘可以运行（实际上我用的就是这个）。
- 正常实模式BIOS。一些机器看起来是用虚-86模式运行启动程序，而且在这样的机器LINUX不会启动和正常运行。
LINUX会发展成为一个自给自足的系统，现在需要Minix-386才能正常运行。你需要Minix让初始化启动文件系统，和编译OS二进制。在那之后LINUX是一个自给自足的系统，但是为了做文件系统检查（fsck）和修改之后重编译系统，推荐使用Minix。
## 获取LINUX
LINUX现在可以使用匿名ftp从‘nic.funet.fi’的‘/pub/OS/Linux’目录获取。这个目录包含操作系统的所有源码，还有一些二进制文件，因此你可以真正使用系统了。
**注意！二进制大多是GNU软件，而且版权比LINUX的严格（GNU非盈利性版权）。因此你不能在不发布他们源码的情况下重新发布他们，可以在/pub/GNU中找到。关于GNU非盈利性版权，从任何GNU软件包了解更多。**
此目录中各类文件如下：
- linux-0.03.tar.Z–系统的完全源码，16位tar压缩文件格式。
- Linux.tex–这个文件的LATEX源码。
- bash.Z–在LINUX下运行的bash二进制文件。这个二进制文件应该放到预留给LINUX文件系统中的/bin/sh下（参见installation）。
- update.Z–更新二进制文件，要放到/bin/update。
- gccbin.tar.Z–GNU cc二进制文件需要由一个可运行的编译器。这个tar压缩包含有编译器，加载器，汇编程序和支持程序（nm，strip等）。它还包含一个小型的库，可用于大部分程序。
- include.tar.Z–让gcc运行的必要include文件。
- unistd.tar.Z–unistd库程序的源码（即系统调用接口）。通过这个你可以使用系统独立库源码编译一个大一些的库。
- utilbin.tar.Z–各种GNU工具的二进制文件，包括GNU的fileutils，make和tar。也包含克隆emacs的uemacs。
- README, RELNOTES-0.01, INSTALLATION–包含一些（有点过时的）LINUX相关的信息的ascii文件。
让系统运行的最少文件是OS源码和bash和更新二进制文件。不过只用这些，你做不了什么事。
## 安装
在你拿到了必要LINUX文件之后，你需要编译系统和创建root目录。必要的二进制文件需要放到root文件系统中。按如下操作：
1. 备份你的软件。虽然LINUX从没有毁坏过我的任何文件，但没有什么是必然的。安全胜过遗憾。
2. 选择/创建一个标准MinixHD-分区作为新的LINUX root文件系统。
3. 在新的root创建必要的设备节点。LINUX与Minix使用相同类型的节点，所以使用Minix的mknod命令创建下面的设备：节点号与在Minix中相同。
- /dev/tty
- /dev/tty[0-2]
- /dev/hd[0-9]
4. 将必要文件放到新的root分区。文件应该放在下面目录中：
希望你现在有一个功能正常的unix，而且你已经root权限登录。LINUX现在没有‘init’过程，只要你注销，系统会同步并等待。使用三指键（Ctrl+Alt+Del）重启机器。
- gcc
- 添加链接到你选择的/usr/local/lib中的文件。我将ld，as，nm，strip和size链接到他们相应的 /usr/local/lib/gcc-XXX。
- gccbin.tar.Z中的内容，除了gcc
- include.tar.Z的内容
- utilbin.tar.Z的内容
- sh，即bash.Z
- update
- /bin:
- /usr/bin:
- /usr/include:
- /usr/local/lib:
- /usr/local/bin:
- 编辑系统中的linux/include/linux/config.h。这个文件包含了针对于系统的信息：内存空间，硬盘类型，root分区号（同样的与Minix中的编号相同），键盘类型（现在只有US和Finnish）等。
- 编译LINUX源码。一个简单技巧就可以完成，在你编辑makefiles为适合你的系统之后（即，删除-mstring-insnsflag，和修改适合你的路径。）1.40之前版本gcc的用户可能需要添加gnulib到makefile中‘LIBS=’一行。
- 复制产生的镜像文件到软盘（即，cp Image /dev/PS0 或者之类的）。
- 使用新的软盘重启。启动界面应该告诉你系统正在启动（加载系统…），然后是一些必要的文件系统信息（xxx/XXX inodes/blocks free），接下来是一个确定，还有bash提示（如果你没有.bashrc文件，则初始化bash#）。
**LINUX 缺失/不兼容的东西**
LINUX 是打算作为一个全部自给自足的内核，但现在并非如此。作为上面已经提到的，你需要 Minix 来设置启动设备并且检查文件系统当它运行起来的时候。这里有一些其它的不足之处：
硬件的不兼容。一些 AT 标准特性当前还没有支持。最值得注意的是软盘驱动，利用 LINUX 进行实际工作（备份 etc）当前是不可能的[译者：这个是 oldlinux，这个是 Linus Torvalds 1991 年 10 月写的文章，肯定当时是不行的]。还有串行连接的一些特性没有被实现（2400 bps 波特率的硬连接，没有挂断（hang-up）提示等等 ）。
标准 c 库的不兼容。gcc 分发版的 libc.a 没有完成，我对免费可发布的库功能很感兴趣。
一些系统调用没有完全实现。这些设计绝大多数“极少调用”的特性比如调试（谁无论如何需要它的话，你的程序第一次是无法工作的:-)）以及其它的特性。
如上所述，没有登陆和初始化进程。当前 LINUX 启动在单用户模式，以 root 作为控制台用户。对于一些移植工作足够了，但不是实际可用的。
387支持[译者：硬件浮点，当时 Intel 发布了外接式 FPU] 没有被实现，即使已有一些基础程序被提供出来。”nic.funet.fi” 的 gcc 二进制包使用软浮点（ie 仿真功能调用）来支持 4 个基础数学运算操作。387-支持将尽快实现当我的电脑安装了这个硬件。希望在一个月或者两个月。
现在还没有重要的系统管理命令实现在 LINUX 中。这些包括 mkfs, format, fsck, mknod 等。这些命令需要的内核特性还没有实现（format, mknod），一些命令只需要实现它。作为一个库，我欢迎任何免费分发文件。
如您所见，LINUX还不是一个完整的系统。 感谢您的帮助，使其变得更好。 我对为LINUX重写的Minix命令不感兴趣，除非你自己从头开始编写它们。 您当然可以免费（并鼓励）将您的Minix发行版中的所有内容用于您自己的LINUX系统，但由于Minix的版权，它们无法分发给更广泛的受众。
这里提到的一些问题将由我（即lines/387/floppy支持）尽快修复，但我希望得到库函数的支持。感谢你们提交的错误报告及补丁还有愿望清单，如果你真的有针对问题的补丁，我会立即尝试去修复它。 小的更改将作为补丁形式发送到邮件列表，并在`nic.funet.fi'上设置，如果经过大量重写，或者修复大的补丁，整个系统将在`nic.funet.fi’更新。
## LINUX移植软件
LINUX被设计得让移植相对容易。因此，就有了完整的termios实现和一些POSIX库。我所移植的（诚然相对较少）程序没有任何问题。
尽管LINUX与Minix非常相似，但Minix程序通常并不会比为其他nuix设计的程序更容易移植。因此，我不建议从一个特定程序的Minix版本开始，而应该尝试从头开始移植‘’virgin‘’程序。比BSD更接近SYSV，这意味着当给定一个-DUSG或者-DSYSV标识时，大多数程序很容易移植。
移植过程中最困难的一点就是缺少库函数。这些必须由你来编写，或者从其他的来源复制（Minix可能是个有缘人）。另外，一些程序（特别是GNU）有各种各样的标识，这些标识可以定义哪些函数不可用（一旦在Makefile中添加了足够量的-DXXX_MISSING标识，GNU fileutils将编译的很好）。
## 已经移植的程序
下面这些程序已经移植到LINUX：
- GNU cc (gcc, cc1, cpp)
- GNU assembler (as386)
- GNU binutils (ld, ar, nm, size, strip, ranlib)
- GNU compress (16-bit)
- GNU tar
- GNU make
- GNU bash (Bourne Again SHell)
- GNU sed
- GNU bison (yacc-lookalike)
- GNU awk
- GNU fileutils (ls, cp, rm, mkdir, rmdir, tail etc)
- less
- uemacs
所有上述程序都能在‘nic.funet.fi’(主要在’/pub/gnu’)中找到，大多数LIINUX-binaries都可以在‘/pub/OS/Linux’目录中找到。包括gcc（cc1）有一些我自己增强的功能，所有这些程序都在没有变化的情况下编译的。先尝试自己编译，遇到问题可以将差异或者资源发邮件给我。
另外，我提起过明确地GNU差异编译和运行。
## 技术帮助
LINUX目前有一个邮件列表，您可以通过邮件发送到这个地址订阅：[**Linux-activists-request@niksula.hut.fi**](mailto:Linux-activists-request@niksula.hut.fi) ，并要求包括在列表中。然后你可以通过这个邮箱：[**Linux-activists@niksula.hut.fi**](mailto:Linux-activists@niksula.hut.fi) 提问题，这将复制你的问题/答案/无论什么，并发送给列表中其他所有人。
请注意Linux-activists和Linux0activists-request的不同——第一个用于给列表中的所有人发送邮件，第二个仅用于订阅和取消订阅。
当然，您也可以直接发送邮件至 [**torvalds@kruuna.helsinki.fi**](mailto:torvalds@kruuna.helsinki.fi)。我会尽量在一两天内回答所有的问题。
尽管‘nic.funet.fi’可能会保持合理的更新状态，但是它还有些问题（即，我无法因为个人得到文件，但可以通过几个人）。因此，如果邮件列表上的人想要补丁或二进制文件，他们将会更快得到。
## 感谢
我要感谢学院…
说真的，如果没有其他人的帮助，这个系统将永远不会有曙光，甚至会变得更糟。Bruce Evans 帮助我找到了需要更改的位置，以便gcc能正确地处理浮点数，并提供许多有用的想法/建议（他的Minix-386用于构建系统）。此外，Earl Chew 的estdio包被用于标准的IO库。像这样更自由地分发包！
Alain W Black和Richard Tobin为Minix制作了gcc，没有它我就无法编译这个东西。GNU完成了我在Linux下使用的大部分程序。Alfred Leung发送了美国键盘补丁。
附：“感谢”[wirzeniu@kruuna.helsinki.fi](mailto:wirzeniu@kruuna.helsinki.fi)他的“建设性”批评和“诙谐”的评论。他是我第一个![$alpha$](http://jbcdn2.b0.upaiyun.com/2018/07/412f23f15ba46a043bbcb30c697e34f7.png)-测试者，他应该被授予勇气奖章。
**Linus Torvalds **(**torvalds@kruuna.helsinki.fi**)**  1991**年**10**月**10**日
