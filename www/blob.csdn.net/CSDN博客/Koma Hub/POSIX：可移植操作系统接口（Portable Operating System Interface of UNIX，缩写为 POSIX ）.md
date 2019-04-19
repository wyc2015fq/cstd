# POSIX：可移植操作系统接口（Portable Operating System Interface of UNIX，缩写为 POSIX ） - Koma Hub - CSDN博客
2019年01月19日 21:55:58[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：69
个人分类：[Just For Fun																[基础知识](https://blog.csdn.net/Rong_Toa/article/category/7587699)](https://blog.csdn.net/Rong_Toa/article/category/7589943)
[POSIX](https://baike.baidu.com/item/POSIX)表示[可移植操作系统接口](https://baike.baidu.com/item/%E5%8F%AF%E7%A7%BB%E6%A4%8D%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E6%8E%A5%E5%8F%A3/12718298)（Portable Operating System Interface of UNIX，缩写为 POSIX ），POSIX标准定义了操作系统应该为应用程序提供的接口标准，是[IEEE](https://baike.baidu.com/item/IEEE)为要在各种UNIX操作系统上运行的软件而定义的一系列API标准的总称，其正式称呼为IEEE 1003，而国际标准名称为ISO/IEC 9945。
POSIX标准意在期望获得[源代码](https://baike.baidu.com/item/%E6%BA%90%E4%BB%A3%E7%A0%81/3969)级别的[软件可移植性](https://baike.baidu.com/item/%E8%BD%AF%E4%BB%B6%E5%8F%AF%E7%A7%BB%E6%A4%8D%E6%80%A7/8320795)。换句话说，为一个POSIX兼容的操作系统编写的程序，应该可以在任何其它的POSIX操作系统（即使是来自另一个厂商）上编译执行。
POSIX 并不局限于 UNIX。许多其它的操作系统，例如 DEC OpenVMS 支持 POSIX 标准，尤其是 IEEE Std. 1003.1-1990（1995 年修订）或 POSIX.1，POSIX.1 提供了源代码级别的 C 语言应用编程接口（API）给操作系统的服务程序，例如读写文件。POSIX.1 已经被国际标准化组织（International Standards Organization，ISO）所接受，被命名为 ISO/IEC 9945-1:1990 标准。
## POSIX的诞生
POSIX的诞生和UNIX的发展是密不可分的，电气和电子工程师协会（Institute of Electrical and Electronics Engineers，IEEE）最初开发 POSIX 标准，是为了提高 UNIX 环境下应用程序的可移植性。[UNIX](https://baike.baidu.com/item/UNIX)于70年代诞生于贝尔实验室，并于80年代向美各大高校分发V7版的源码以做研究。加利福尼亚大学伯克利分校在V7的基础上开发了BSD Unix。后来很多商业厂家意识到UNIX的价值也纷纷以贝尔实验室的System V或BSD为基础来开发自己的Unix，较著名的有Sun OS，AIX，VMS。
然而，POSIX 并不局限于 UNIX。许多其它的操作系统，例如 DEC OpenVMS 支持 POSIX 标准，尤其是 IEEE Std. 1003.1-1990（1995 年修订）或 POSIX.1，POSIX.1 提供了[源代码](https://baike.baidu.com/item/%E6%BA%90%E4%BB%A3%E7%A0%81)级别的 C 语言应用编程接口（API）给操作系统的服务程序，例如读写文件。POSIX.1 已经被国际标准化组织（International Standards Organization，ISO）所接受，被命名为 ISO/IEC 9945-1:1990 标准。
POSIX 已发展成为一个非常庞大的标准族，某些部分正处在开发过程中。POSIX 与 IEEE 1003 和 2003 家族的标准是可互换的。
Windows NT-based 系统不能直接支持新版POSIX接口，仅支持第一版POSIX v1:ISO/IEC9945-1:1990 [1] 版标准。由于仅支持第一版POSIX的Windows NT-based系统不能创建符合POSIX接口标准的线程和窗体、套接字，所以微软公司提供POSIX兼容层Microsoft POSIX subsystem软件包（Windows Services for UNIX）以支持新版POSIX接口，Windows 系统还可以运行其他POSIX兼容层例如[Cygwin](https://baike.baidu.com/item/Cygwin)。
## 常见标准
### 1003.0
管理 POSIX 开放式系统环境（OSE）。IEEE 在 1995 年通过了这项标准。 ISO 的版本是 ISO/IEC 14252:1996。
### 1003.1
被广泛接受、用于源代码级别的可移植性标准。1003.1 提供一个操作系统的 C 语言应用编程接口（API）。IEEE 和 ISO 已经在 1990 年通过了这个标准，IEEE 在 1995 年重新修订了该标准。
### 1003.1b
一个用于实时编程的标准（以前的 P1003.4 或 POSIX.4）。这个标准在 1993 年被 IEEE 通过，被合并进 ISO/IEC 9945-1。
### 1003.1c
一个用于线程（在一个程序中当前被执行的[代码段](https://baike.baidu.com/item/%E4%BB%A3%E7%A0%81%E6%AE%B5)）的标准。以前是 P1993.4 或 POSIX.4 的一部分，这个标准已经在 1995 年被 IEEE 通过，归入 ISO/IEC 9945-1:1996。
### 1003.1g
一个关于协议独立接口的标准，该接口可以使一个应用程序通过网络与另一个应用程序通讯。 1996 年，IEEE 通过了这个标准。
### 1003.2
一个应用于 shell 和 工具软件的标准，它们分别是操作系统所必须提供的命令处理器和工具程序。 1992 年 IEEE 通过了这个标准。ISO 也已经通过了这个标准（ISO/IEC 9945-2:1993）。
### 1003.2d
改进的 1003.2 标准。
### 1003.5
一个相当于 1003.1 的 Ada 语言的 API。在 1992 年，IEEE 通过了这个标准。并在 1997 年对其进行了修订。ISO 也通过了该标准。
### 1003.5b
一个相当于 1003.1b（实时扩展）的 Ada 语言的 API。IEEE 和 ISO 都已经通过了这个标准。ISO 的标准是 ISO/IEC 14519:1999。
### 1003.5c
一个相当于 1003.1q（协议独立接口）的 Ada 语言的 API。在 1998 年， IEEE 通过了这个标准。ISO 也通过了这个标准。
### 1003.9
一个相当于 1003.1 的 FORTRAN 语言的 API。在 1992 年，IEEE 通过了这个标准，并于 1997 年对其再次确认。ISO 也已经通过了这个标准。
### 1003.10
一个应用于超级计算应用环境框架（Application Environment Profile，AEP）的标准。在 1995 年，IEEE 通过了这个标准。
### 1003.13
一个关于应用环境框架的标准，主要针对使用 POSIX 接口的实时应用程序。在 1998 年，IEEE 通过了这个标准。
### 1003.22
一个针对 POSIX 的关于安全性框架的指南。
### 1003.23
一个针对用户组织的指南，主要是为了指导用户开发和使用支持操作需求的[开放式系统](https://baike.baidu.com/item/%E5%BC%80%E6%94%BE%E5%BC%8F%E7%B3%BB%E7%BB%9F)环境（OSE）框架
### 2003
针对指定和使用是否符合 POSIX 标准的测试方法，有关其定义、一般需求和指导方针的一个标准。在 1997 年，IEEE 通过了这个标准。
### 2003.1
这个标准规定了针对 1003.1 的 POSIX 测试方法的提供商要提供的一些条件。在 1992 年，IEEE 通过了这个标准。
### 2003.2
一个定义了被用来检查与 IEEE 1003.2（shell 和 工具 API）是否符合的测试方法的标准。在 1996 年，IEEE 通过了这个标准。
除了 1003 和 2003 家族以外，还有几个其它的 IEEE 标准，例如 1224 和 1228，它们也提供开发可移植应用程序的 API。
## Linux操作系统
▪ [塔能鲍姆–托瓦兹 争论](http://baike.baidu.com/searchword/?word=%E5%A1%94%E8%83%BD%E9%B2%8D%E5%A7%86%E2%80%93%E6%89%98%E7%93%A6%E5%85%B9%20%E4%BA%89%E8%AE%BA&pic=1&sug=1&enc=utf8)▪ [《代码》](http://baike.baidu.com/searchword/?word=%E3%80%8A%E4%BB%A3%E7%A0%81%E3%80%8B&pic=1&sug=1&enc=utf8)
参考资料
- 1.[](https://baike.baidu.com/item/POSIX/3792413?fr=aladdin#ref_%5B1%5D_209573)[Microsoft POSIX subsystem](https://baike.baidu.com/redirect/422bm_cAto3JtKle5937puUmWfja22iVKyZRofOshFWpMvA6Zb6WF5lf2KfkDKCZkDowT9V1AMPEWStM1ljgMZr6dFpfcTi4QBTijLqFUqJSNoY) ．wikipedia[引用日期2012-09-26]
以上内容来自百度百科。
