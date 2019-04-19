# 关于Makefile,Makefile.in,Makefile.am的问题 - 三少GG - CSDN博客
2014年07月30日 11:39:27[三少GG](https://me.csdn.net/scut1135)阅读数：541
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)

# [关于Makefile,Makefile.in,Makefile.am的问题](http://blog.csdn.net/qoo_wzp/article/details/6159744)
![](http://hi.csdn.net/attachment/201101/23/0_1295767248ZNnW.gif)
最近在研究开源的东西，发现有很多makefile.am,makefile.in,makefile文件，它们之间究竟是什么关系呢？能可以用什么工具生成吗？
   无论对于一个初学者还是一个资深的Linux程序员，编写Makefile文件都是一件很麻烦的事；再者，开发人员应该把主要的精力放在程序代码的编写上，而在Makefile文件花费太多的精力显然是不明智的；还有，对于不同的处理器架构，往往编译器不同，环境不同,特别是一些嵌入式系统中的各种程序的编译，于是移植问题也使Makefile文件编写趋于复杂，也显得这个问题很重要。对于这些问题Linux的高手们早已想到了，所以他们开发了一些能够自动生成Makefile文件的工具。他们就是下面这些工具：
〉GNU Automake
〉GNU Autoconf
〉GNU m4
〉perl
〉GNU Libtool
因此您的操作系统必须安装以上软件，否则就不能够自动生成Makefile文件或者在生成的过程中出现各种各样的问题。用 autoconf/automake/autoheader工具来处理各种移植性的问题，用这些工具完成系统配置信息的收集，制作Makefile文件。然后在打算编译源码时只需要通过 "./configure; make"这样简单的命令就可以得到干净利落的编译。
制作Makefile文件需要以下几步：
1〉建立编译目录和源代码目录及源代码文件(即要编译的源文件)
[root@localhost leaf]#mkdir testmk
[root@localhost leaf]#cd testmk
[root@localhost testmk]#vi hello.c
编辑hello.c文件如下内容：
/*filename:hello.c*/
#include <stdio.h>
int main(int argc,char **argv)
{
printf("%s/n","Hello, World!")
return 0;
}
2〉**利用autoscan工具产生一个configure.in文件的模板文件configure.scan文件**：
[root@localhost testmk]#autoscan
[root@localhost testmk]#ls
configure.scan hello.c
3〉**把configure.scan文件更名为configure.in文件，并编译其内容如下**：
[root@localhost testmk]#mv configure.scan configure.in
[root@localhost testmk]#vi configure.in
dnl Process this file with autoconf to produce a configure script.
AC_INIT(hello.c)
dnl Add the file by leaf
AM_INIT_AUTOMAKE(hello,1.0)
dnl Checks for programs.
AC_PROG_CC
dnl Checks for libraries.
dnl Checks for header files.
dnl Checks for typedefs, structures, and compiler characteristics.
dnl Checks for library functions.
AC_OUTPUT(Makefile)
4〉**执行aclocal，会产生aclocal.m4文件**
[root@localhost testmk]#**aclocal**
[root@localhost testmk]#ls
aclocal.m4 configure.in hello.c
5〉**执行autoconf，会产生confiure文件**
[root@localhost testmk]#autoconf
[root@localhost testmk]#ls
aclocal.m4 [autom4te.cache] configure configure.in hello.c
6〉创建文件Makefile.am并编辑其内容如下：
AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=hello
hello_SOURCES=hello.c
其中，hello为编译后产生的可执行文件名称，而第三行等号后面为源文件列表
7〉执行automake程序，automake程序会根据Makefile.am产生一些文件，其中最重要的是Makefile.in文件：
[root@localhost testmk]#**automake --add-missing**
configure.in: installing `./install-sh'
configure.in: installing `./mkinstalldirs'
configure.in: installing `./missing'
Makefile.am: installing `./depcomp'
[root@localhost testmk]#ls
aclocal.m4 [autom4te.cache] configure configure.in depcomp
hello.c install-sh Makefile.am Makefile.in missing
mkinstalldirs
8〉执行configure脚本，生成我们需要的Makefile文件。
[root@localhost testmk]#./configure
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking for gawk... gawk
checking whether make sets $(MAKE)... yes
checking for gcc... gcc
checking for C compiler default output... a.out
checking whether the C compiler works... yes
checking whether we are cross compiling... no
checking for suffix of executables...
checking for suffix of object files... o
checking whether we are using the GNU C compiler... yes
checking whether gcc accepts -g... yes
checking for gcc option to accept ANSI C... none needed
checking for style of include used by make... GNU
checking dependency style of gcc... gcc3
configure: creating ./config.status
config.status: creating Makefile
config.status: executing depfiles commands
9〉最后只执行make就大功告成了：
[root@localhost testmk]#make
source='hello.c' object='hello.o' libtool=no /
depfile='.deps/hello.Po' tmpdepfile='.deps/hello.TPo' /
depmode=gcc3 /bin/sh ./depcomp /
gcc -DPACKAGE_NAME="" -DPACKAGE_TARNAME="" -DPACKAGE_VERSION="" -DPACKAGE_STRING="" -DPACKAGE_BUGREPORT="" -DPACKAGE="hello" -DVERSION="1.0" -I. -I. -g -O2 -c `test -f 'hello.c' || echo './'`hello.c
gcc -g -O2 -o hello hello.o
备注：
1.以上内容均在RedHat Linux 9.0环境下测试通过。
2.参考书目《Linux 程序设计权威指南》于明俭、陈向阳、方汉编著
3.其它国内外网站资料
4.RedHat 9.0下带的程序文件及版本
autoconf-2.57-3.noarch.rpm
automake-1.6.3-5.noarch.rpm
gcc-3.2.2-5.i386.rpm
