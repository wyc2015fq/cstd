# 【configure】如何用automake、autoconf指令生成configure并创建自己的linux tar.gz安装包【初级篇：简单建立-测试】 - Koma Hub - CSDN博客
2018年10月20日 09:14:36[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：97
**目录**
[流程](#%E6%B5%81%E7%A8%8B)
[一.安装包tar.gz的生成](#%E4%B8%80.%E5%AE%89%E8%A3%85%E5%8C%85tar.gz%E7%9A%84%E7%94%9F%E6%88%90)
[准备源文件](#%E5%87%86%E5%A4%87%E6%BA%90%E6%96%87%E4%BB%B6)
[autoscan](#autoscan)
[生成configure.scan](#%E7%94%9F%E6%88%90configure.scan)
[aclocal](#aclocal)
[autoconf](#autoconf)
[autoheader](#autoheader)
[Makefile.am](#Makefile.am)
[automake](#automake)
[./configure](#.%2Fconfigure)
[make dist \ make distclean](#make%20dist%20%5C%20make%20distclean)
[生成tar.gz安装包](#%E7%94%9F%E6%88%90tar.gz%E5%AE%89%E8%A3%85%E5%8C%85)
[二.安装测试](#%E4%BA%8C.%E5%AE%89%E8%A3%85%E6%B5%8B%E8%AF%95)
[解压tar.gz](#%E8%A7%A3%E5%8E%8Btar.gz)
[./configure](#.%2Fconfigure)
[make](#make)
[make install](#make%20install)
[运行](#%E8%BF%90%E8%A1%8C)
[三.整体过程回顾](#%E4%B8%89.%E6%95%B4%E4%BD%93%E8%BF%87%E7%A8%8B%E5%9B%9E%E9%A1%BE)
[四.后续](#%E4%B8%89.%E5%90%8E%E7%BB%AD)
[本文全部代码、资源地址：](#%E6%9C%AC%E6%96%87%E5%85%A8%E9%83%A8%E4%BB%A3%E7%A0%81%E3%80%81%E8%B5%84%E6%BA%90%E5%9C%B0%E5%9D%80%EF%BC%9A)
# 流程
![](https://img-blog.csdn.net/20181020213248872?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 一.安装包tar.gz的生成
## 准备源文件
首先准备好源文件（夹），如下（一下文件可见[https://blog.csdn.net/Rong_Toa/article/details/82716420](https://blog.csdn.net/Rong_Toa/article/details/82716420)）：
```
$ tree 2048-c/
2048-c/
├── 2048.c
├── 2048.h
└── main.c
0 directories, 3 files
```
## autoscan
### 生成configure.scan
然后进入文件夹，执行autoscan生成configure.scan文件
```
$ cd 2048-c/
$ ls
2048.c*  2048.h*  main.c*
$ autoscan
Unescaped left brace in regex is deprecated here (and will be fatal in Perl 5.30), passed through in regex; marked by <-- HERE in m/\${ <-- HERE [^\}]*}/ at /usr/bin/autoscan-2.69 line 361.
$ ls
2048.c*  2048.h*  autoscan-2.69.log  configure.scan  main.c*
```
将configure.scan文件更名为configure.ac，并进行编辑：
```
$ mv configure.scan configure.ac
$ vim configure.ac
```
修改第5行，添加第6行，第9行选填，并保存：
```
1 #                                               -*- Autoconf -*-
  2 # Process this file with autoconf to produce a configure script.
  3
  4 AC_PREREQ([2.69])
  5 AC_INIT(2048, 1.0, rongtoa@163.com)
  6 AM_INIT_AUTOMAKE(2048, 1.0)
  7 AC_CONFIG_SRCDIR([2048.c])
  8 AC_CONFIG_HEADERS([config.h])
  9 AC_CONFIG_FILES([Makefile])
 10 # Checks for programs.
 11 AC_PROG_CC
 12
 13 # Checks for libraries.
 14
 15 # Checks for header files.
 16 AC_CHECK_HEADERS([stdlib.h string.h termios.h unistd.h])
 17
 18 # Checks for typedefs, structures, and compiler characteristics.
 19
 20 # Checks for library functions.
 21
 22 AC_OUTPUT
~
:wq
```
## aclocal
执行aclocal命令：
```
$ ls
2048.c*  2048.h*  autoscan-2.69.log  configure.ac  main.c*
$ aclocal
$ ls
2048.c*  aclocal.m4       autoscan-2.69.log  main.c*
2048.h*  autom4te.cache/  configure.ac
```
## autoconf
执行autoconf指令：
```
$ autoconf
$ ls
2048.c*  aclocal.m4       autoscan-2.69.log  configure.ac
2048.h*  autom4te.cache/  configure*         main.c*
```
## autoheader
执行autoheader命令：
```
$ autoheader
$ ls
2048.c*  aclocal.m4       autoscan-2.69.log  configure*    main.c*
2048.h*  autom4te.cache/  config.h.in        configure.ac
```
## Makefile.am
接下来创建文件Makefile.am
内容如下（忽略行号）：
```
$ vim Makefile.am
  1 AUTOMAKE_OPTIONS=foreign
  2 bin_PROGRAMS=2048
  3 2048_SOURCES=2048.c 2048.h main.c
~
~
~
:wq
```
## automake
然后automake
```
$ automake --add-missing
configure.ac:6: warning: AM_INIT_AUTOMAKE: two- and three-arguments forms are deprecated.  For more info, see:
configure.ac:6: http://www.gnu.org/software/automake/manual/automake.html#Modernize-AM_005fINIT_005fAUTOMAKE-invocation
$ ls
2048.c*     autom4te.cache/    config.log      install-sh@  Makefile.in
2048.exe*   autoscan-2.69.log  config.status*  main.c*      missing@
2048.h*     compile@           configure*      main.o       stamp-h1
2048.o      config.h           configure.ac    Makefile
aclocal.m4  config.h.in        depcomp@        Makefile.am
```
## ./configure
接下来进行./configure操作生成Makefile文件，具体命令行此处不作讲解：
```
$ ./configure
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking for a thread-safe mkdir -p... /usr/bin/mkdir -p
checking for gawk... gawk
checking whether make sets $(MAKE)... yes
checking whether make supports nested variables... yes
...
$
```
然后make编译、make distclean删除安装包的无关项，make dist生成安装的压缩包tar.gz：
## make dist \ make distclean
```
$ make dist
make  dist-gzip am__post_remove_distdir='@:'
make[1]: 进入目录“/home/rongtao/2048-c”
if test -d "2048-1.0"; then find "2048-1.0" -type d ! -perm -200 -exec chmod u+w {} ';' && rm -rf "2048-1.0" || { sleep 5 && rm -rf "2048-1.0"; }; else :; fi
test -d "2048-1.0" || mkdir "2048-1.0"
test -n "" \
|| find "2048-1.0" -type d ! -perm -755 \
        -exec chmod u+rwx,go+rx {} \; -o \
  ! -type d ! -perm -444 -links 1 -exec chmod a+r {} \; -o \
  ! -type d ! -perm -400 -exec chmod a+r {} \; -o \
  ! -type d ! -perm -444 -exec /bin/sh /home/rongtao/2048-c/install-sh -c -m a+r {} {} \; \
|| chmod -R a+r "2048-1.0"
tardir=2048-1.0 && ${TAR-tar} chof - "$tardir" | eval GZIP= gzip --best -c >2048-1.0.tar.gz
make[1]: 离开目录“/home/rongtao/2048-c”
if test -d "2048-1.0"; then find "2048-1.0" -type d ! -perm -200 -exec chmod u+w {} ';' && rm -rf "2048-1.0" || { sleep 5 && rm -rf "2048-1.0"; }; else :; fi
```
```
$ make distclean
test -z "2048.exe" || rm -f 2048.exe
rm -f *.o
rm -f *.tab.c
test -z "" || rm -f
test . = "." || test -z "" || rm -f
rm -f config.h stamp-h1
rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH tags
rm -f cscope.out cscope.in.out cscope.po.out cscope.files
rm -f config.status config.cache config.log configure.lineno config.status.lineno
rm -rf ./.deps
rm -f Makefile
```
## 生成tar.gz安装包
生成了一个：
```
$ ls -l
-rw-r--r--  1 rongtao None  82991 10月 20 09:03 2048-1.0.tar.gz
```
# 二.安装测试
对生成的tar.gz进行安装
将2018-1.0.tar.gz拷贝至“下载”目录，假装他是下载得到的：
```
$ pwd
/home/rongtao/下载
$ ls
2048-1.0.tar.gz
```
## 解压tar.gz
```
$ tar -xzvf 2048-1.0.tar.gz
2048-1.0/
2048-1.0/2048.c
2048-1.0/2048.h
2048-1.0/aclocal.m4
2048-1.0/compile
2048-1.0/config.h.in
2048-1.0/configure
2048-1.0/configure.ac
2048-1.0/depcomp
2048-1.0/install-sh
2048-1.0/main.c
2048-1.0/Makefile.am
2048-1.0/Makefile.in
2048-1.0/missing
$ ls
2048-1.0/  2048-1.0.tar.gz
```
## ./configure
进入文件夹进行configure
```
$ cd 2048-1.0
$ ls
2048.c*     compile*     configure.ac  main.c*      missing*
2048.h*     config.h.in  depcomp*      Makefile.am
aclocal.m4  configure*   install-sh*   Makefile.in
$ ./configure
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking for a thread-safe mkdir -p... /usr/bin/mkdir -p
checking for gawk... gawk
...
$
```
## make
进行编译make：
```
$ make
make  all-am
make[1]: 进入目录“/home/rongtao/下载/2048-1.0”
gcc -DHAVE_CONFIG_H -I.     -g -O2 -MT main.o -MD -MP -MF .deps/main.Tpo -c -o main.o main.c
mv -f .deps/main.Tpo .deps/main.Po
gcc -DHAVE_CONFIG_H -I.     -g -O2 -MT 2048.o -MD -MP -MF .deps/2048.Tpo -c -o 2048.o 2048.c
mv -f .deps/2048.Tpo .deps/2048.Po
gcc  -g -O2   -o 2048.exe main.o 2048.o
make[1]: 离开目录“/home/rongtao/下载/2048-1.0”
```
## make install
进行安装make install
```
$ make install
make[1]: 进入目录“/home/rongtao/下载/2048-1.0”
 /usr/bin/mkdir -p '/usr/local/bin'
  /usr/bin/install -c 2048.exe '/usr/local/bin'
make[1]: 对“install-data-am”无需做任何事。
make[1]: 离开目录“/home/rongtao/下载/2048-1.0”
```
## 运行
检查是否安装，在终端输入20，然后按Tab直接跳出可执行文件，是不是很nice：
```
$ 2048.exe
```
# 三.整体过程回顾
```
$ cd 2048-c/
$ ls
2048.c*  2048.h*  main.c*
$ ls
2048.c*  2048.h*  main.c*
$ autoscan
输出略...
$ ls
2048.c*  2048.h*  autoscan-2.69.log  configure.scan  main.c*
$ mv configure.scan configure.ac
$ ls
2048.c*  2048.h*  autoscan-2.69.log  configure.ac  main.c*
$ vim configure.ac
configure.ac 内容如下：
  1 #                                               -*- Autoconf -*-
  2 # Process this file with autoconf to produce a configure script.
  3
  4 AC_PREREQ([2.69])
  5 AC_INIT(2048, 1.0.0, rongtoa@163.com)
  6 AM_INIT_AUTOMAKE(2048, 1.0.0)
  7 AC_CONFIG_SRCDIR([2048.c])
  8 AC_CONFIG_HEADERS([config.h])
  9 AC_CONFIG_FILES([Makefile])
 10
 11 # Checks for programs.
 12 AC_PROG_CC
 13
 14 # Checks for libraries.
 15
 16 # Checks for header files.
 17 AC_CHECK_HEADERS([stdlib.h string.h termios.h unistd.h])
 18
 19 # Checks for typedefs, structures, and compiler characteristics.
 20
 21 # Checks for library functions.
 22
 23 AC_OUTPUT
:wq
$ vim Makefile.am
Makefile.am 内容如下：
  1 AUTOMAKE_OPTIONS=foreign
  2 bin_PROGRAMS=2048
  3 2048_SOURCES=main.c 2048.c 2048.h
~
~
:wq
$ aclocal
$ autoconf
$ autoheader
$ automake --add-missing
configure.ac:6: warning: AM_INIT_AUTOMAKE: two- and three-arguments forms are deprecated.  For more info, see:
configure.ac:6: http://www.gnu.org/software/automake/manual/automake.html#Modernize-AM_005fINIT_005fAUTOMAKE-invocation
configure.ac:12: installing './compile'
configure.ac:6: installing './install-sh'
configure.ac:6: installing './missing'
Makefile.am: installing './depcomp'
$ ./configure
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking for a thread-safe mkdir -p... /usr/bin/mkdir -p
checking for gawk... gawk
略去部分中间输出...
configure: creating ./config.status
config.status: creating Makefile
config.status: creating config.h
config.status: executing depfiles commands
$ make
make  all-am
make[1]: 进入目录“/home/rongtao/2048-c”
gcc -DHAVE_CONFIG_H -I.     -g -O2 -MT main.o -MD -MP -MF .deps/main.Tpo -c -o main.o main.c
mv -f .deps/main.Tpo .deps/main.Po
gcc -DHAVE_CONFIG_H -I.     -g -O2 -MT 2048.o -MD -MP -MF .deps/2048.Tpo -c -o 2048.o 2048.c
mv -f .deps/2048.Tpo .deps/2048.Po
gcc  -g -O2   -o 2048.exe main.o 2048.o
make[1]: 离开目录“/home/rongtao/2048-c”
$ make dist
make  dist-gzip am__post_remove_distdir='@:'
make[1]: 进入目录“/home/rongtao/2048-c”
if test -d "2048-1.0.0"; then find "2048-1.0.0" -type d ! -perm -200 -exec chmod u+w {} ';' && rm -rf "2048-1.0.0" || { sleep 5 && rm -rf "2048-1.0.0"; }; else :; fi
test -d "2048-1.0.0" || mkdir "2048-1.0.0"
test -n "" \
|| find "2048-1.0.0" -type d ! -perm -755 \
        -exec chmod u+rwx,go+rx {} \; -o \
  ! -type d ! -perm -444 -links 1 -exec chmod a+r {} \; -o \
  ! -type d ! -perm -400 -exec chmod a+r {} \; -o \
  ! -type d ! -perm -444 -exec /bin/sh /home/rongtao/2048-c/install-sh -c -m a+r {} {} \; \
|| chmod -R a+r "2048-1.0.0"
tardir=2048-1.0.0 && ${TAR-tar} chof - "$tardir" | eval GZIP= gzip --best -c >2048-1.0.0.tar.gz
make[1]: 离开目录“/home/rongtao/2048-c”
if test -d "2048-1.0.0"; then find "2048-1.0.0" -type d ! -perm -200 -exec chmod u+w {} ';' && rm -rf "2048-1.0.0" || { sleep 5 && rm -rf "2048-1.0.0"; }; else :; fi
$ make clean
test -z "2048.exe" || rm -f 2048.exe
rm -f *.o
$ make distclean
test -z "2048.exe" || rm -f 2048.exe
rm -f *.o
rm -f *.tab.c
test -z "" || rm -f
test . = "." || test -z "" || rm -f
rm -f config.h stamp-h1
rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH tags
rm -f cscope.out cscope.in.out cscope.po.out cscope.files
rm -f config.status config.cache config.log configure.lineno config.status.lineno
rm -rf ./.deps
rm -f Makefile
$ ls
2048.c*            autom4te.cache/    configure*    main.c*
2048.h*            autoscan-2.69.log  configure.ac  Makefile.am
2048-1.0.0.tar.gz  compile@           depcomp@      Makefile.in
aclocal.m4         config.h.in        install-sh@   missing@
$ tar -xzvf 2048-1.0.0.tar.gz
2048-1.0.0/
2048-1.0.0/2048.c
2048-1.0.0/2048.h
2048-1.0.0/aclocal.m4
2048-1.0.0/compile
2048-1.0.0/config.h.in
2048-1.0.0/configure
2048-1.0.0/configure.ac
2048-1.0.0/depcomp
2048-1.0.0/install-sh
2048-1.0.0/main.c
2048-1.0.0/Makefile.am
2048-1.0.0/Makefile.in
2048-1.0.0/missing
$ cd 2048-1.0.0
$ ./configure
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking for a thread-safe mkdir -p... /usr/bin/mkdir -p
checking for gawk... gawk
省略中间输出...
checking that generated files are newer than configure... done
configure: creating ./config.status
config.status: creating Makefile
config.status: creating config.h
config.status: executing depfiles commands
$ make
make  all-am
make[1]: 进入目录“/home/rongtao/2048-c/2048-1.0.0”
gcc -DHAVE_CONFIG_H -I.     -g -O2 -MT main.o -MD -MP -MF .deps/main.Tpo -c -o main.o main.c
mv -f .deps/main.Tpo .deps/main.Po
gcc -DHAVE_CONFIG_H -I.     -g -O2 -MT 2048.o -MD -MP -MF .deps/2048.Tpo -c -o 2048.o 2048.c
mv -f .deps/2048.Tpo .deps/2048.Po
gcc  -g -O2   -o 2048.exe main.o 2048.o
make[1]: 离开目录“/home/rongtao/2048-c/2048-1.0.0”
$ make install
make[1]: 进入目录“/home/rongtao/2048-c/2048-1.0.0”
 /usr/bin/mkdir -p '/usr/local/bin'
  /usr/bin/install -c 2048.exe '/usr/local/bin'
make[1]: 对“install-data-am”无需做任何事。
make[1]: 离开目录“/home/rongtao/2048-c/2048-1.0.0”
```
# 四.后续
到此结束，以后会继续更新对一个项目的configure操作，敬请期待，我也很期待。。。
## 本文全部代码、资源地址：
[https://download.csdn.net/download/rong_toa/10733319](https://download.csdn.net/download/rong_toa/10733319)
