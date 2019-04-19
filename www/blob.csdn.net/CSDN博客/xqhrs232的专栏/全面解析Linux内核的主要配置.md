# 全面解析Linux内核的主要配置 - xqhrs232的专栏 - CSDN博客
2010年12月29日 11:35:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：678标签：[linux内核																[attributes																[filesystems																[debugging																[security																[module](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=security&t=blog)](https://so.csdn.net/so/search/s.do?q=debugging&t=blog)](https://so.csdn.net/so/search/s.do?q=filesystems&t=blog)](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://linux.chinaitlab.com/safe/741857.html](http://linux.chinaitlab.com/safe/741857.html)
配置内核的命令是： 
# make config 文本配置方式 
# make menuconfig 菜单配置方式 
# make xconfig 图形界面配置方式 
我们可以采用任何一个命令配置内核，一般推荐使用make menuconfig来配置内核。 
**一．代码成熟度选项**
Code maturity level options  --->
[*] Prompt for development and/or incomplete code/drivers
[*]   Select only drivers expected to compile cleanly
在内核中包含了一些不成熟的代码和功能，如果我们想使用这些功能，想打开相关的配置选项，就必需打开这一选项。 
**二．通用设置选项**
General setup  --->
()  Local version - append to kernel release
[*]  Automatically append version information to the version string
[*]  Support for paging of anonymous memory (swap)
[*]  System V IPC
[*]  POSIX Message Queues
[*]  BSD Process Accounting
[*]  BSD Process Accounting version 3 file format
[*]  Sysctl support
[ ]  Auditing support
[*]  Support for hot-pluggable devices
[*]  Kernel Userspace Events
[*]  Kernel .config support
[*]  Enable access to .config through /proc/config.gz
()   Initramfs source file(s)
[*]  Configure standard kernel features (for small systems)  --->
         --- Configure standard kernel features (for small systems)
         [ ]   Load all symbols for debugging/kksymoops
         [ ]    Do an extra kallsyms pass
         [ ]    Enable support for prinlk
         [ ]    BUG()support
         [ ]   Enable full-sinzed data structures for core
         [*]   Enable futex support
         [*]   Enable eventpoll support
         [*]   Optimize for size
         [*]   Use full shmem filesystem
         (0)   Function alignment
         (0)   Label alignment
         (0)   Loop alignment
         (0)   Jump alignment
Local version - append to kernel release：这里填入的是64字符以内的字符串，你在这里填上的字符口串可以用uname -a命令看到。
Support for paging of anonymous memory (swap)：这是使用[交换](http://cisco.chinaitlab.com/List_7.html)分区或者[交换](http://cisco.chinaitlab.com/List_7.html)文件来做为虚拟内存的，当然要选上了。
System V IPC：表示系统5的Inter Process Communication，它用于处理器在程序之间同步和交换信息，如果不选这项，很多程序运行不起来的。
POSIX Message Queues：这是POSIX的消息队列，它同样是一种IPC。建议你最好将它选上。
BSD Process Accounting：这是充许用户进程访问内核将账户信息写入文件中的。这通常被认为是个好主意，建议你最好将它选上。
Sysctl support：这个选项能不重新编译内核修改内核的某些参数和变量，如果你也选择了支持/proc，将能从/proc/sys存取可以影响内核的参数或变量。建议你最好将它选上。
Auditing support：审记支持，用于和内核的某些子模块同时工作，例如SELinux。只有选择此项及它的子项，才能调用有关审记的系统调用。
Support for hot-pluggable devices：是否支持热插拔的选项，肯定要选上。不然USB、PCMCIA等这些设备都用不了。
Kernel Userspace Events：内核中分为系统区和用户区，这里系统区和用户区进行通讯的一种方式，选上。
Kernel .config support：将.config配置信息保存在内核中，选上它及它的子项使得其它用户能从/proc中得到内核的配置。
Configure standard kernel features (for small systems)：这是为了编译某些特殊的内核使用的，通常你可以不选择这一选项，你也不用对它下面的子项操心了。
Load all symbols for debugging/kksymoops：是否装载所有的调试符号表信息，如果你不需要对内核调试，不需要选择此项。
Enable futex support：不选这个内核不一定能正确的运行使用glibc的程序，当然要选上。 
Enable eventpoll support：不选这个内核将不支持事件轮循的系统调用，最好选上。 
Optimize for size：这个选项使gcc使用-Os的参数而不是-O2的参数来优化编译，以获得更小尺寸的内核，建议选上。
Use full shmem filesystem：除非你在很少的内存且不使用交换内存时，才不要选择这项。 
后面的这四项都是在编译时内存中的对齐方式，0表示编译器的默认方式。使用内存对齐能提高程序的运行速度，但是会增加程序对内存的使用量。
**三．可加载模块**
Loadable module support  --->
[*] Enable loadable module support
[*] Module unloading
[ ]     Forced module unloading
[*] Module versioning support (EXPERIMENTAL)
[ ] Source checksum for all modules
[*] Automatic kernel module loading
Enable loadable module support，很多人喜欢将全部功能、硬件支持一股脑的编进内核，而不是使用模块的方式。这样做非常不好（个人觉得）。其实我也做过[嵌入式](http://embed.chinaitlab.com/)的开发，在针对特定硬件的平台下尽可能将内核编小，将始终是支持模块加载的。例如我们开发的[防火墙](http://security.chinaitlab.com/List_1291.html)就是做为内核的模块被加载的。使用模块支持，你的系统能具有更好的可扩充性。还有一个原因就是自己编写的功能模块、设备驱动模块（假设编写的质量不高）以模块方式工作引起Kernel
 Panic的机率要远远低于不支持模块全部编进内核的方式。讲了这么多，终于可以理直气壮的选上这一功能了。
Module unloading，不选这个功能，加载的模块就不能卸载。没什么需要多解释的，建议最好选上。 
Forced module unloading，这个选项能强行卸载模块，即使内核认为这样并不[安全](http://security.chinaitlab.com/)，也就是说你可以把正在使用中的模快卸载掉。如果你不是内核开发人员或者骨灰级的玩家，不要选择这个选项。
Module versioning support (EXPERIMENTAL)，这个功能可以让你使用其它版本的内核模块，由于我自己写一些模块，所以我会用到这个选项，因为内核更新太快了，我的头文件更新根本赶不上内核的更新。还有，虽然我在Gentoo下开发，但实际真实环境用的却是从kernel.org[下载](http://download.chinaitlab.com/)的内核。虽然我选择了这个选项，不过建议你不要选择这个选项。
Source checksum for all modules，这个功能是为了防止更改了内核模块的代码但忘记更改版本号而造成版本冲突。我估计现在没有哪家公司在开发中还没使用版本控制工具，所以不需要这项了。如果你不是自己写内核模块，那就更不需要这一选项了。
Automatic kernel module loading，这个选项能让内核自动的加载部份模块，建议你最好选上。举个例子说明一下，如模块eth1394依赖于模块ieee1394。如果选择了这个选项，可以直接加载模块eth1394；如果没有选择这个选项，必需先加载模块ieee1394，再加载模块eth1394，否则将出错。
**四．总线支持配置**
Bus support --->
       PCCARD (PCMCIA/CardBus) support  --->
           [ ]  Enable PCCARD debugging
           [ ]  16-bit PCMCIA support (NEW)
           [ ]    Load CIS updates from userspace (EXPERIMENTAL)(NEW)
           [ ]    PCMCIA control ioctl (obsolete) (NEW)
           --- PC-card bridges
PCCard (PCMCIA/CardBus) support，你的计算机是否支持PCMCIA卡 
Enable PCCARD debugging，通常不需要选择调试PCMCIA设备，除非你是设备驱动的开发人员。 
16-bit PCMCIA support，16位的PCMCIA总线支持。 
**五．支持的可执行文件格式**
Userspace binary formats -à
[*] Kernel support for ELF binaries
[*] Kernel support for a.out and ECOFF binaries
[*] Kernel support for MISC binaries
[*] RISC OS personality
Kernel support for ELF binaries，ELF是开放平台下最常用的二进制文件，它支持不同的硬件平台。
Kernel support for a.out and ECOFF binaries，这是早期UNIX系统的可执行文件格式，目前已经被ELF格式取代。
Kernel support for MISC binaries，此选项允许插入二进制的封装层到内核中，当使用[Java](http://java.chinaitlab.com/)、.NET、Python、Lisp等语言编写的程序时非常有用。
**六，文件系统**
File systems
       <*> Second extended fs support
       [*]   Ext2 extended attributes
       [*]     Ext2 POSIX Access Control Lists
       [*]     Ext2 Security Labels
       <*> Ext3 journalling file system support
       [*]   Ext3 extended attributes
       [*]     Ext3 POSIX Access Control Lists
       [*]     Ext3 Security Labels
       [ ] JBD (ext3) debugging support
       <*> Reiserfs support
       [ ]   Enable reiserfs debug mode
       [ ]   Stats in /proc/fs/reiserfs
       [*]   ReiserFS extended attributes
       [*]     ReiserFS POSIX Access Control Lists
       [*]     ReiserFS Security Labels
       JFS filesystem support
       [*]   JFS POSIX Access Control Lists
       [ ]   JFS debugging
       [ ]   JFS statistics
       XFS filesystem support
       [*]   Realtime support (EXPERIMENTAL)
       [*]   Quota support
       [*]   Security Label support
       [*]   POSIX ACL support
       < > Minix fs support
       < > ROM file system support
       [*] Quota support
       < >   Old quota format support
       Quota format v2 support
       [*] Dnotify support
       < > Kernel automounter support
       < > Kernel automounter version 4 support (also supports v3)
    CD-ROM/DVD Filesystems  --->
    DOS/FAT/NT Filesystems  --->
    Pseudo filesystems  --->
    Miscellaneous filesystems  --->
    Network File Systems  --->
    Partition Types  --->
    Native Language Support  --->
Second extended fs support，标准的Linux文件系统，建议将这种文件系统编译进内核。
Ext2 extended attributes，Ext2文件系统的结点名称、属性的扩展支持。 
Ext2 POSIX Access Control Lists，POSIX系统的访问权限列表支持。也就是 Owner/Group/Others的Read/Write/Execute权限。请参考Unix标准文件系统权限。
Ext2 Security Labels，扩展的[安全](http://security.chinaitlab.com/)标签，例如SE[linux](http://linux.chinaitlab.com/)之类的安全系统会使用到这样的扩展安全属性。
Ext3 journalling file system support，如果你熟悉Redhat Linux，你一定会习惯Ext3文件系统。
Ext3 extended attributes，Ext3文件系统的结点名称、属性的扩展支持。 
Ext3 POSIX Access Control Lists，POSIX系统的访问权限列表支持。 
Ext3 Security Labels，扩展的安全标签支持。 
JBD (ext3) debugging support，Ext3的调试。除非你是文件系统的开发者，否则不要选上这一项。
Reiserfs support，如果你熟悉Suse Linux，你一定会习惯Reiserfs文件系统。 
Enable reiserfs debug mode，Reiserfs的调试。除非你是文件系统的开发者，否则不要选上这一项。 
Stats in /proc/fs/reiserfs，在/proc/fs/reiserfs文件中显示Reiserfs文件系统的状态。一般来说不需要选择这一项。
ReiserFS extended attributes，Reiserfs，文件系统的结点名称、属性的扩展支持。
ReiserFS POSIX Access Control Lists，POSIX系统的访问权限列表支持。 
ReiserFS Security Labels，扩展的安全标签支持。 
JFS filesystem support，JFS是IBM公司设计用于AIX系统上的文件系统。后来这一文件系统也能应用于Linux系统。
JFS POSIX Access Control Lists，POSIX系统的访问权限列表支持。 
JFS debugging，JFS的调试。除非你是文件系统的开发者，否则不要选上这一项。 
JFS statistics，在/proc/fs/jfs文件中显示Reiserfs文件系统的状态。一般来说不需要选择这一项。
XFS filesystem support，XFS是SGI公司为其图形工作站设计的一种文件系统，后来这一文件系统也能应用于Linux系统。
Realtime support (EXPERIMENTAL)，实时卷的支持，能大幅提高大文件的读写速度。不过并不太安全，建议暂时不要选择这一选项。
Quota support，XFS文件系统的配额支持。 
Security Label support，扩展的安全标签支持。 
POSIX ACL support，POSIX系统的访问权限列表支持。 
Minix fs support，Minix可能是最早的Linux系统所使用的文件系统。后来被Ext2文件系统所取代。
ROM file system support，内存文件系统的支持。除非你是[嵌入式](http://embed.chinaitlab.com/)系统的开发者，明确知道你要干什么，否则不要选这一项。
Quota support，配额支持。也就是说限制某个用户或者某组用户的磁盘占用空间。 
Old quota format support，旧版本的配额支持。 
Quota format v2 support，新版本（第二版）的配额支持。 
Dnotify support，基于目录的文件变化的通知机制。 
Kernel automounter support，内核自动加载远程文件系统的支持。 
Kernel automounter version 4 support (also supports v3)，新的（第四版）的内核自动加载远程文件系统的支持，也支持第三版。
