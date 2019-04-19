# System V - Koma Hub - CSDN博客
2019年01月19日 21:53:12[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：20
个人分类：[Just For Fun																[基础知识](https://blog.csdn.net/Rong_Toa/article/category/7587699)](https://blog.csdn.net/Rong_Toa/article/category/7589943)
**目录**
[系统介绍](#%E7%B3%BB%E7%BB%9F%E4%BB%8B%E7%BB%8D)
[版本信息](#%E7%89%88%E6%9C%AC%E4%BF%A1%E6%81%AF)
[SVR1](#SVR1)
[SVR2](#SVR2)
[SVR3](#SVR3)
[SVR4](#SVR4)
System V， 曾经也被称为 AT&T System V，是Unix操作系统众多版本中的一支。它最初由 AT&T 开发，在1983年第一次发布。一共发行了4个 System V 的主要版本：版本1、2、3 和 4。System V Release 4，或者称为[SVR4](https://baike.baidu.com/item/SVR4/725424)，是最成功的版本，成为一些[UNIX](https://baike.baidu.com/item/UNIX/219943)共同特性的源头，例如 ”SysV 初始化脚本“ （/etc/init.d），用来控制系统启动和关闭，System V Interface Definition (SVID) 是一个System V 如何工作的标准定义。
## 系统介绍
AT&T 出售运行System V的专有硬件，但许多（或许是大多数）客户在其上运行一个转售的版本，这个版本基于 AT&T 的实现说明。流行的SysV 衍生版本包括 Dell SVR4 和 Bull SVR4。当今广泛使用的 System V 版本是 SCO OpenServer，基于 System V Release 3，以及SUN Solaris 和 SCO UnixWare，都基于 System V Release 4。
System V 是 AT&T 的第一个商业UNIX版本（UNIX System III）的加强。传统上，System V 被看作是两种UNIX"风味"之一（另一个是 BSD）。然而，随着一些并不基于这两者代码的[UNIX](https://baike.baidu.com/item/UNIX/219943)实现的出现，例如 Linux 和 QNX， 这一归纳不再准确，但不论如何，像POSIX这样的标准化努力一直在试图减少各种实现之间的不同。
## 版本信息
### SVR1
System V的第一个版本，发布于1983年。它引进了一些特性，例如vi编辑器和[curses](https://baike.baidu.com/item/curses/1630775)库（这是从加州大学伯克利分校开发的BSD中引进的）。其中也包括了对DEC VAX机器的支持。同时也支持使用消息进行[进程间通讯](https://baike.baidu.com/item/%E8%BF%9B%E7%A8%8B%E9%97%B4%E9%80%9A%E8%AE%AF)，[信号量](https://baike.baidu.com/item/%E4%BF%A1%E5%8F%B7%E9%87%8F)，和[共享内存](https://baike.baidu.com/item/%E5%85%B1%E4%BA%AB%E5%86%85%E5%AD%98)。
### SVR2
System V Release 2在1984年发布。其中添加了shell功能和SVID。
### SVR3
System V Release 3在1987年。它包括STREAMS，远程[文件共享](https://baike.baidu.com/item/%E6%96%87%E4%BB%B6%E5%85%B1%E4%BA%AB/10580065)（remote file sharing，RFS），共享库，以及Transport Layer Interface (TLI)。
### SVR4
System V Release 4.0在1989年11月1日公开，并于1990年发布。它是UNIX Systems Laboratories和Sun联合进行的项目，融合了来自Release 3，4.3BSD，[Xenix](https://baike.baidu.com/item/Xenix/1846442)，以及[SunOS](https://baike.baidu.com/item/SunOS/10693255)的技术：来自[BSD](https://baike.baidu.com/item/BSD/3794498)：TCP/IP支持，csh来自SunOS：[网络文件系统](https://baike.baidu.com/item/%E7%BD%91%E7%BB%9C%E6%96%87%E4%BB%B6%E7%B3%BB%E7%BB%9F)，[内存映射文件](https://baike.baidu.com/item/%E5%86%85%E5%AD%98%E6%98%A0%E5%B0%84%E6%96%87%E4%BB%B6)，以及一个新的共享库系统其他的实现： kshANSI C兼容更好的国际化和本地化支持一个[二进制](https://baike.baidu.com/item/%E4%BA%8C%E8%BF%9B%E5%88%B6/361457)应用接口支持[POSIX](https://baike.baidu.com/item/POSIX/3792413)、X/Open和SVID3标准。
以上内容来自百度百科。
