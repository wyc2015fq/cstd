# 查看Linux系统版本与位数 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年01月03日 09:07:11[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：495
## 查看系统发行版信息
- 查看LSB (Linux Standard Base)本身的版本信息。
**[**ray**@**localhost ~**]**$ lsb_release -v
LSB Version:    :core-3.1-ia32:core-3.1-noarch:graphics-3.1-ia32:graphics-3.1-noarch
- 打印系统发行版的所有信息，比如发行版的ID，描述信息，具体版本，代号：
**[**ray**@**localhost ~**]**$ lsb_release -a
LSB Version:    :core-3.1-ia32:core-3.1-noarch:graphics-3.1-ia32:graphics-3.1-noarch
Distributor ID: RedHatEnterpriseServer
Description:    Red Hat Enterprise Linux Server release 5.5**(**Tikanga**)**
Release:        5.5
Codename:       Tikanga
有些系统上不一定安装了这个命令，可以通过查看/etc/issue文件得到发行版信息。
注:centos 7 需要用cat /etc/redhat-release查看
**[**ray**@**localhost ~**]**$ **cat ****/**etc**/**issue 
Red Hat Enterprise Linux Server release 5.5**(**Tikanga**)**
Kernel \r on an \m
 
**[**ray**@**localhost ~**]**$ **head**-1**/**etc**/**issue
Red Hat Enterprise Linux Server release 5.5**(**Tikanga**)****[**ray**@**localhost ~**]**$ **uname**-r
2.6.18-194.el5
 
**[**ray**@**localhost ~**]**$ **uname**-a
Linux localhost 2.6.18-194.el5 *#1 SMP Tue Mar 16 21:52:43 EDT 2010 i686 i686 i386 GNU/Linux***[**ray**@**localhost ~**]**$ **cat****/**proc**/**version
Linux version 2.6.18-194.el5 **(**mockbuild**@**x86-007.build.bos.redhat.com**)****(****gcc** version 4.1.2 20080704**(**Red Hat 4.1.2-48**)****)***#1 SMP Tue Mar 16 21:52:43 EDT 2010*
## 查看系统是64位还是32位
**uname **-m**[**ray**@**localhost ~**]**$ getconf LONG_BIT
32**[**ray**@**localhost ~**]**$ getconf WORD_BIT
32**[**ray**@**localhost ~**]**$ **file****/**bin**/****ls****/**bin**/**ls: ELF 32-bit LSB executable, Intel 80386, version 1**(**SYSV**)**, **for** GNU**/**Linux 2.6.9,
dynamically linked **(**uses shared libs**)**, **for** GNU**/**Linux 2.6.9, stripped
如果有x86_64就是64位的，没有就是32位的。
下面是64位的 ：
Linux WORKS 2.6.9-11.ELsmp *#1 SMP Fri May 20 18:25:30 EDT 2005 x86_64 x86_64 x86_64 GNU/Linux*
32位的：
Linux backup 2.6.9-67.ELsmp *#1 SMP Wed Nov 7 13:58:04 EST 2007 i686 i686 i386 GNU/Linux*
注意：Ixxx的全是32位的，有X86—64的全是64位。可输入uname –m或arch查看。
