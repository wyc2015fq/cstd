# Linux根文件系统结构再认识 - maopig的专栏 - CSDN博客
2011年09月14日 13:45:29[maopig](https://me.csdn.net/maopig)阅读数：725标签：[linux																[hierarchy																[system																[工具																[平台																[磁盘](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=hierarchy&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux](https://blog.csdn.net/maopig/article/category/850267)
                
Linux根文件系统结构再认识
刘建文（[http://blog.csdn.net/keminlau](http://blog.csdn.net/keminlau) ）
## INTRO
尽管Linux的根文件系统在形式表现上是一体的（所有数据目录均为根目录下的子目录），但实际它们是多个不同的【逻辑主体】（为了实现不同的逻辑功能）组合在一起的，参见下图。这也解释了为什么有些节点（如/var）可以挂接到不同磁盘分区上。划分这些逻辑部分对清晰把握根文件系统结构，以及整个系统的行为都有很大的好处。在划分之前，先看看“文件系统结构标准”。
![](http://hi.csdn.net/attachment/201003/17/10307_1268834370Qhay.gif)
## 文件系统结构标准
文件系统结构标准 （[Filesystem Hierarchy Standard](http://www.pathname.com/fhs/)，下称FHS）定立的目标之一是，程序可按用途性质被安装到指定的位置，用户也能预知程序的安装的文件和目录的位置。FHS通过定义文件系统各部分的涵义、用途，再加上列举所有的特殊情况，如历史遗留问题，达到了这个目标。
LINUX系统的数据文件可以根据【双重标准】来对它们分类：共享的与独享的（shareable vs. unshareable）、变化的和静态的（variable vs. static）。如果数据文件无法按照双重标准分类，它们必须另起目录存放，这样做的原因是可以把不同【使用性质】（usage characteristics）的数据文件分开不同文件系统（磁盘分区）存放。
- “共享的” （Shareable）是指数据与平台不相关的，如/usr；相反“独享的”（Unshareable）指数据是平台相关的，如配置数据/etc。
- “静态的”数据是相对非管理员用户而言的，没有更变的权限；而“变化的”则相反。分隔静态与动态数据的目的是为了可把静态数据安装到一个单独的只读的介质（文件系统）中，不必与动态数据一起做频繁的事务管理，比如定期备份。
与数据的【静动态性】面向“用户”不同，数据的【共享专用性】是面向“主机”而言，也就是说，有些数据是本机专用的，而另外一些是多个主机（可是本地的FHS兼容主机，可以是远程主机）共享的。比如根目录分区是本机专用的，而/usr所在分区可共享给多个FHS兼容主机。
双重标准是发展而来的，被纳入FHS，为了支持这个标准，过去默认存放在/usr 、/var、 /etc 等目录的数据都做了相应的调整。
## Linux根文件系统逻辑组成
参照FHS，Linux系统按照逻辑功能大致分为以下目录（以具体目录表现逻辑功能，有意思）：
- /
- /usr
- /var
- /opt
- /home
P.S. 以上目录均可单独安装到一个分区上，最终挂接入根下。 
## / （专用的静态的）
根目录的功能由其名可略知，根目录是系统的根本所在（Essential），是系统之所以为本系统的特征表现。也可这样理解，根目录的内容不是可选的，是系统基本功能必备部分。
> 
P.S.如何理解“根本所在”？
我们知道，Linux系统有内核，还会有shell，有gred，还有吗？有vi吧，有GCC吧，有ifconfig吧，还有可能X下大量应用程序。所谓“根本所在”，在内容表现上，就是包括最基本的最通用的部分。当然这部分内容还可能有一些是可选的，这个取决你如何定义基本系统。
根目录的逻辑功能分为两层，第一层是应用的，第二层是管理（administration）的。应用层在上面说了，它用来组成基本系统，包括常用工具（如 ls，放置于/bin），和一些共享库等。第二层是管理用的，对系统进行启动、恢复、修复和配置等。由于根目录是系统的“根本所在”，管理目的，并且是 HOST相关的，如果可以应单独安装在一个尽可能小的分区上。
> 
如何定义基本系统？
由于计算系统基本上是逻辑的，基本系统的定义取决计算任务，而计算任务本身是交错复杂和多样的，所以只能用一个特殊的例子来说明。比如要进行启动、恢复、修复和配置的系统需要些什么？
- To boot a system, enough must be present on the root partition to mount other ﬁlesystems. This includes utilities, conﬁguration, boot loader information, and other essential start-up data.
- To enable recovery and/or repair of a system, those utilities needed by an experienced maintainer to diagnose and reconstruct a damaged system must be present on the root ﬁlesystem.
- To restore a system, those utilities needed to restore from system backups (on ﬂoppy, tape, etc.) must be present on the root ﬁlesystem
### 根目录必选组成目录
- /bin 基本工具或命令
- /sbin 系统管理工具或命令
- /etc 主机相关（Host-speciﬁc）的配置数据
- /lib 基本共享库和内核模块
- /dev 设备文件
- /boot 引导程序
- /root 管理员的工作目录
- /mnt 系统管理员的临时挂接点
- /tmp 系统级临时文件
- /media 移动设备挂接点
## /usr （共享的静态的）
/usr 目录是系统的一大组成部分。很难用一句说明/usr的用途。不过我们可以透过/usr 的双重标准性质与/和/var对比来理解/usr在系统中的角色。首先，与/相同，/usr是静态的，也就是说/usr下的数据普通用户是不能更改的；但与/不同的是，/usr是共享的，也就可知，/usr下的数据与主机平台不相关的。我们大概知道，/usr与/类似安装有二进制程序、共享库和配置数据，那么从/usr 的双重标准性质可推理得，这些数据及代码都是独立于运行主机的。
而与/var对比呢？/var基本上是动态的和共享的（少量是主机相关的，如/var/log）。/var保存大部分是程序运行期的动态生成数据。
### /usr必选组成目录
- /usr/bin Most user commands
- /usr/include Header ﬁles included by C programs
- /usr/blib Libraries
- /usr/local Local hierarchy (empty after main installation)
- /usr/sbin Non-vital system binaries
- /usr/share Architecture-independent data 
## /var
……
## /home : User home directories (optional)
……
