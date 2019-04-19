# Linux下的Autoconf和AutoMake - xqhrs232的专栏 - CSDN博客
2016年04月14日 22:03:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：220
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.linuxidc.com/Linux/2014-09/107014.htm](http://www.linuxidc.com/Linux/2014-09/107014.htm)
相关文章
1、初学者连载系列之十三：手动安装m4, autoconf, automake, libtool----[https://ruby-china.org/topics/2434](https://ruby-china.org/topics/2434)
2、[总结一下手动安装autoconf、automake和libtool的经历](http://bbs.chinaunix.net/thread-3752960-1-1.html)----[http://bbs.chinaunix.net/thread-3752960-1-1.html](http://bbs.chinaunix.net/thread-3752960-1-1.html)
1、工具安装
在开始使用autoconf和automake之前，首先确认你的系统安装有GNU的如下软件： automake/autoconf/m4/perl。
如果你需要产生共享库（shared library）则还需要GNU Libtool。
如果使用[RedHat](http://www.linuxidc.com/topicnews.aspx?tid=10)的Linux版本，这几个工具会自动安装，我试验中用[SUSE](http://www.linuxidc.com/topicnews.aspx?tid=3)11也安装了所有程序。
autoconf：是一个shell脚本的工具，可以自动地配置软件源代码包以适应多种UNIX类系统。由autoconf生成的配置脚本在运行的时候不需要用户的手工干预；通常它们甚至不需要手工给出参数以确定系统的类型。其中autoconf需要用到m4，便于生成脚本。
automake：是一个从文件Makefile.am自动生成Makefile.in的工具。每个Makefile.am基本上是一系列make的宏定义。生成的Makefile.in，服从GNU Makefile标准。为了生成Makefile.in，automake还需用到perl（它本身就是一个perl程序）。
2、使用流程
![](http://www.linuxidc.com/upload/2014_09/140922051673261.png)
（1）由你的源文件通过autoscan命令，生成configure.scan文件，然后修改configure.scan文件并重命名为configure.in。
configure.in文件的内容是一些宏，这些宏经过autoconf 处理后会变成检查系统特性、环境变量、软件必须的参数的shell脚本。
configure.in文件中的宏的顺序并没有规定，但是你必须在所有宏的最前面和最后面分别加上AC_INIT宏和AC_OUTPUT宏。
（2）由aclocal命令，从configure.in生成aclocal.m4
（3）由autoconf命令，生成configure
（4）编辑各个Makefile.am文件，并由automake --add-missing 命令生成Makefile.in文件,
（5）运行configure命令，从Makefile.in文件生成Makefile
3、如何修改configure.in文件
AM_INIT_AUTOMAKE(PACKAGE,VERSION)
这个是使用 Automake 所必备的宏，PACKAGE 是所要产生软件的名称，VERSION 是版本编号。也可以把包和版本号等信息放在AC_INIT(FILE) 宏里。
AC_PROG_CC：检查系统可用的C 编译器
AC_PROG_RANLIB：如果需要生成静态库，则需要加入这一句
AC_PROG_LIBTOOL：如果需要生成动态库，则加入这一句，表示用libtool自动生成动态库
4、如何编写makefile.am文件
在makefile.am档中需要定义要产生的目标，具体包括：
AUTOMAKE_OPTIONS：设定automake的选项。automake主要是帮助开发GNU软件的人员维护软件套件，一般在执行automake时会检查目录下是否存在标准GNU套件中应具备的文件档案，例如NEWS、AUTHOR、ChangeLog等，设成foreign时，automake会改用一般软件套件标准来检查，而gnu是缺省设置，该级别下将尽可能地检查包是否服从GNU标准，gnits是严格标准，不推荐。
bin_PROGRAMS：定义要产生的执行档名称。产生多个执行档的话，需要用空格符隔开。
hello_SOURCES：定义hello这个执行档的源文件，如果该程序由多个原始档产生，必须把所用到的原始档都列出来，空格符隔开，例如hello_SOURCES = hello.c main.c hello.h。如果定义了多个执行档，则对每个执行档都要定义相对的filename_SOURCES。
举例：
|noinst_LIBRARIES=libhello.alibhello_a_SOURCES=test.cAUTOMAKE_OPTIONS=foreign|第一行noinst 表示生成的是静态库。如果要把静态库生成到其他地方，可以在=后面加上路径。第二行表示用来生成静态库的源文件。第三行AUTOMAKE_OPTIONS 是Automake 的选项。|
|----|----|
|AUTOMAKE_OPTIONS=foreignlib_LTLIBRARIES=libhello.lalibhello_la_SOURCES=test.c|这里用la就表示是生成动态库了|
|AUTOMAKE_OPTIONS=foreignINCLUDES= -I../includebin_PROGRAMS=hellohello_SOURCES=hello.chello_LDADD=../lib/libhello.a|第二行指定头文件的位置，-I 是idirafter 的缩写。第三行指定生成可执行文件名hello，在这里可执行文件生成在src 下，如需要把可执行文件生成到/root/test 目录下。写法为：bin_PROGRAMS=/root/test/hello，后面的第四、五行也相对应地变为：_root_test_hello_SOURCES=hello.c_root_test_hello_LDADD=../lib/libhello.a第四行指定生成可执行文件hello 的源代码文件第五行指定需要使用静态库的位置。|
5、产生的Makefile结构
利用 configure 所产生的 Makefile 档有几个预设的目标可供使用，我们只拿其中几个简述如下：
make all
产生我们设定的目标，即此范例中的执行档。只打 make 也可以，此时会开始编译原始码，然後连结，并且产生执行档。
make clean
清除之前所编译的执行档及目的档 (object file, *.o)。
make distclean
除了清除执行档和目的档外，也把 configure 所产生的 Makefile 也清除掉。
make install
将程序安装至系统中。如果原始码编译无误，且执行结果正确，便可以把程序安装至系统预设的执行档存放路径。如果我们用bin_PROGRAMS 巨集的话，程序会被安装至 /usr/local/bin 这个目录。
make dist
将程序和相关的档案包装成一个压缩档以供散播 (distribution) 。执行完在目录下会产生一个以 PACKAGE-VERSION.tar.gz 为名称的档案。PACKAGE 和 VERSION 这两个变数是根据 configure.in 档中AM_INIT_AUTOMAKE(PACKAGE, VERSION) 的定义。在此范例中会产生'hello-1.0.tar.gz' 的档案。
make distcheck
和 make dist 类似，但是加入检查包装後的压缩档是否正常。这个目标除了把程序和相关档案包装成 tar.gz 档外，还会自动把这个压缩档解开，执行 configure，并且进行 make all 的动作，确认编译无误後，会显示这个 tar.gz 档已经准备好可供散播了。这个检查非常有用，检查过关的套件，基本上可以给任何一个具备 GNU 发展环境的人去重新编译。
6、其他
其它有关开发 GNU 程序或 C 程序设计及 Makefile 的详细运用及技巧，建议从 GNU Coding Standards3 (GNU 编码标准规定) 读起，里面包含了 GNU Makefile 惯例，还有发展 GNU 软体套件的标准程序和惯例。这些 GNU 软体的线上说明文件可以在 http://www.gnu.org/ 这个网站上找到。
7、我的经验
（1）config.status: WARNING: 'Makefile.in' seems to ignore the --datarootdir setting;基本上是操作顺序的错误
（2）编译错误：libtool: line 813: X--tag=CC: command not found
google了半天，终于找到这个：
../libtool: line 763: X–tag=CC: command not found
The libtool on your host computer is causing the problem (using $(echo) when it is not defined). Either install an older libtool version (1.5.26) or define the echo environment variable.
只需要:export echo=echo
（3）configure.in:56: warning: AC_RUN_IFELSE was called before AC_USE_SYSTEM_EXTENSIONS
删除掉自动的检测配置，删除掉临时文件。
**本文永久更新链接地址**：[http://www.linuxidc.com/Linux/2014-09/107014.htm](http://www.linuxidc.com/Linux/2014-09/107014.htm)
![linux](http://www.linuxidc.com/linuxfile/logo.gif)
