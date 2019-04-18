# Makefile自动生成工具-----autotools的使用（详细） - weixin_33985507的博客 - CSDN博客
2014年06月24日 13:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
相信每个学习Linux的人都知道Makefile，这是一个很有用的东西，但是编写它是比较复杂，今天介绍一个它的自动生成工具，autotools的使用。很多GNULinux的的软件都是用它生成Makefile的，包括我们非常熟悉的Linux内核源代码。
  1、准备:
  需要工具
  autoscan
  aclocal
  autoheader 
  automake
  autoconf
  auto make 
  在终端敲入命令，哪个没有安装哪个，一般是第一个autoscan没有，其它的我用的Ubuntu10.04下全部都有
  2、测试程序编写：
     建立目录：mkdir include src
     编写程序：include/str.h
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- #include <stdio.h>  
- int str(char *string);  
    编写程序：src/str.c
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- #include "str.h"  
- //print string  
- int str(char *string){  
-         printf("\n----PRINT STRING----\n\"%s\"\n",string);  
- return 0;  
- }  
- 
- //interface of this program  
- int main(int argc , char **argv){  
- char str_read[1024];  
-         printf("Please INPUT something end by [ENTER]\n");  
-         scanf("%s",str_read);  
- return str(str_read );  
- }  
- 
- 
  3、生成configure.ac
    configure.ac是automake的输入文件，所以必须先生成该文件。
    执行命令：
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- [root@localhost str]# ls  
- include  src  
- [root@localhost str]# autoscan  
- autom4te: configure.ac: no such file or directory  
- autoscan: /usr/bin/autom4te failed with exit status: 1  
- [root@localhost str]# ls  
- autoscan.log  configure.scan  include  src  
- [root@localhost str]# cp configure.scan configure.ac   
    修改 configure.ac 
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- #                                               -*- Autoconf -*-  
- # Process this file with autoconf to produce a configure script.  
- 
- AC_PREREQ(2.59)  
- AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)  
- AC_CONFIG_SRCDIR([include/str.h])  
- AC_CONFIG_HEADER([config.h])  
- 
- # Checks for programs.  
- AC_PROG_CC  
- 
- # Checks for libraries.  
- 
- # Checks for header files.  
- 
- # Checks for typedefs, structures, and compiler characteristics.  
- 
- # Checks for library functions.  
- AC_OUTPUT  
修改
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)  
改为：
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- AC_INIT(str,0.0.1, [bug@sounos.org])  
其中：FULL-PACKAGE-NAME 为程序名称，VERSION为当前版本， BUG-REPORT-ADDRESS为bug汇报地址
然后添加两句话：
    AM_INIT_AUTOMAKE
    AC_CONFIG_FILES([Makefile])
结果如下：（两句话不是在一起的）
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- #                                               -*- Autoconf -*-  
- # Process this file with autoconf to produce a configure script.  
- 
- AC_PREREQ(2.59)  
- #AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)  
- AC_INIT(str, 0.0.1, [bug@sounos.org])  
- AM_INIT_AUTOMAKE  
- AC_CONFIG_SRCDIR([include/str.h])  
- AC_CONFIG_HEADER([config.h])  
- 
- # Checks for programs.  
- AC_PROG_CC  
- 
- # Checks for libraries.  
- 
- # Checks for header files.  
- 
- # Checks for typedefs, structures, and compiler characteristics.  
- 
- # Checks for library functions.  
- AC_CONFIG_FILES([Makefile])  
- AC_OUTPUT  
4、执行aclocal
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- [root@localhost str]# aclocal  
- /usr/share/aclocal/libfame.m4:6: warning: underquoted definition of AM_PATH_LIBFAME  
-   run info '(automake)Extending aclocal'  
-   or see http://sources.redhat.com/automake/automake.html#Extending-aclocal  
5、制作Makefile.am
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- [root@localhost str]# vi Makefile.am  
- #Makefile.am  
- bin_PROGRAMS    = str  
- str_SOURCES     = include/str.h src/str.c  
- str_CPPFLAGS    = -I include/  
automake 这个命令需要用到这个配置文件。各个选项意思比较直观，不多说。
6、autoheader
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- [root@localhost str]# autoheader  
7、automake必须文件：
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- *  install-sh  
- * missing  
- * INSTALL  
- * NEWS  
- * README  
- * AUTHORS  
- * ChangeLog  
- * COPYING  
- * depcomp   
其中，以下文件在执行automake -a的时候会自动生成
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- * install-sh  
- * missing  
- * INSTALL  
- * COPYING  
- * depcomp   
所以，接下来手动生成剩下的文件
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- [root@localhost str]# touch NEWS README AUTHORS ChangeLog  
8、执行automake -a
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- [root@localhost str]# automake -a  
- configure.ac: installing `./install-sh'  
- configure.ac: installing `./missing'  
- Makefile.am: installing `./INSTALL'  
- Makefile.am: installing `./COPYING'  
- Makefile.am: installing `./compile'  
- Makefile.am: installing `./depcomp'  
9、autoconf
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- [root@localhost str]# autoconf  
- [root@localhost str]# ls  
- aclocal.m4      autoscan.log  config.h.in   configure.scan  include     Makefile.am  NEWS  
- AUTHORS         ChangeLog     configure     COPYING         INSTALL     Makefile.in  README  
- autom4te.cache  compile       configure.ac  depcomp         install-sh  missing      src  
10、执行测试：
      执行./configure
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- [root@localhost str]# ./configure --prefix=/u  
- checking for a BSD-compatible install... /usr/bin/install -c  
- checking whether build environment is sane... yes  
- checking for gawk... gawk  
- checking whether make sets $(MAKE)... yes  
- checking for gcc... gcc  
- checking for C compiler default output file name... a.out  
- checking whether the C compiler works... yes  
- checking whether we are cross compiling... no  
- checking for suffix of executables...  
- checking for suffix of object files... o  
- checking whether we are using the GNU C compiler... yes  
- checking whether gcc accepts -g... yes  
- checking for gcc option to accept ANSI C... none needed  
- checking for style of include used by make... GNU  
- checking dependency style of gcc... gcc3  
- configure: creating ./config.status  
- config.status: creating Makefile  
- config.status: creating config.h  
- config.status: config.h is unchanged  
- config.status: executing depfiles commands  
执行 make
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- [root@localhost str]# make  
- make  all-am  
- make[1]: Entering directory `/data/devel/c/str'  
- if gcc -DHAVE_CONFIG_H -I. -I. -I.  -I include/   -g -O2 -MT str-str.o -MD -MP -MF ".deps/str-str.Tpo" -c -o str-str.o `test -f 'src/str.c' || echo './'`src/str.c; \  
- then mv -f ".deps/str-str.Tpo" ".deps/str-str.Po"; else rm -f ".deps/str-str.Tpo"; exit 1; fi  
- gcc  -g -O2   -o str  str-str.o  
- make[1]: Leaving directory `/data/devel/c/str'  
此时已经生成了 str（可执行文件名字在前面设置Makefile.am的参数时候去顶）这个，可以通过./str直接看到运行结果
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- [root@localhost str]# ./str  
- Please INPUT something end by [ENTER]  
- abcksdhfklsdklfdjlkfd  
- 
- ----PRINT STRING----  
- "abcksdhfklsdklfdjlkfd"  
不过这里我们都做一步，把它安装到系统里面，这样我们只要在终端输入str就可以运行程序了。
 执行 make install：
**[cpp]**[view plain](http://blog.csdn.net/memoryjs/article/details/7860783)[copy](http://blog.csdn.net/memoryjs/article/details/7860783)
- [root@localhost str]# make install  
- make[1]: Entering directory `/data/devel/c/str'  
- test -z "/u/bin" || mkdir -p -- "/u/bin"  
-   /usr/bin/install -c 'str' '/u/bin/str'  
- make[1]: Nothing to be done for `install-data-am'.  
- make[1]: Leaving directory `/data/devel/c/str'       
接下来你可以make clean 清除安装的那些.o 文件了。
这样生成了一个自动的Makefile。
