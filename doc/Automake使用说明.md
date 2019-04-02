# Automake使用说明



------

官网：<http://www.gnu.org/software/automake/#introduction>

官网帮助文档：<http://www.gnu.org/software/automake/manual/automake.html>

automake是一个自动生成Makefile.in的工具。依赖autoconf工具包(操作M4、libtool)，autoconf可以用来生成configure，而configure会使用Makefile.in来生成Makefile。

Automake is a tool for automatically(自动地) generating `Makefile.in` files compliant with(符合) the GNU Coding Standards. Automake requires the use of [Autoconf](http://www.gnu.org/software/autoconf/autoconf.html)(<https://blog.csdn.net/knowledgebao/article/details/84975661>).

> - **autoscan：**是用来扫描源代码目录生成configure.scan文件的 .autoscan可以用目录名做为参数,但如果你不使用参数的话,那么autoscan将认为使用的是当前目录.autoscan将扫描你所指定目录中的源文件,并创建configure.scan文件.
> - **configure.scan:**包含了系统配置的基本选项,里面都是一些宏定义.我们需要将它改名为configure.in
> - **aclocal:**是一个perl脚本程序.aclocal根据configure.in文件的内容,自动生成aclocal.m4文件.aclocal的定义是："aclocal - create aclocal.m4 by scanning configure.ac".
> - **autoconf:**是用来产生configure文件的.configure是一个脚本,它能设置源程序来适应各种不同的操作系统平台,并且根据不同的系统来产生合适的Makefile,从而可以使你的源代码能在不同的操作系统平台上被编译出来.configure.in文件的内容是一些宏,这些宏经过autoconf处理后会变成检查系统特性.环境变量.软件必须的参数的shell脚本.configure.in文件中的宏的顺序并没有规定,但是你必须在所有宏的最前面和最后面分别加上AC_INIT宏和AC_OUTPUT宏.在configure.ini中：#号表示注释,这个宏后面的内容将被忽略.AC_INIT(FILE)这个宏用来检查源代码所在的路径.AM_INIT_AUTOMAKE(PACKAGE,VERSION)这个宏是必须的,它描述了我们将要生成的软件包的名字及其版本号：PACKAGE是软件包的名字,VERSION是版本号.当你使用makedist命令时,它会给你生成一个类似helloworld-1.0.tar.gz的软件发行包,其中就有对应的软件包的名字和版本号.AC_PROG_CC这个宏将检查系统所用的C编译器.AC_OUTPUT(FILE)这个宏是我们要输出的Makefile的名字.我们在使用automake时,实际上还需要用到其他的一些宏,但我们可以用aclocal来帮我们自动产生.执行aclocal后我们会得到aclocal.m4文件.产生了configure.in和aclocal.m4两个宏文件后,我们就可以使用autoconf来产生configure文件了.
> - **Makefile.am:**是用来生成Makefile.in的,需要你手工书写.Makefile.am中定义了一些内容：AUTOMAKE_OPTIONS 这个是automake的选项.在执行automake时,它会检查目录下是否存在标准GNU软件包中应具备的各种文件,例如AUTHORS.ChangeLog.NEWS等文件.我们将其设置成foreign时,automake会改用一般软件包的标准来检查.bin_PROGRAMS这个是指定我们所要产生的可执行文件的文件名.如果你要产生多个可执行文件,那么在各个名字间用空格隔开.helloworld_SOURCES这个是指定产生"helloworld"时所需要的源代码.如果它用到了多个源文件,那么请使用空格符号将它们隔开.比如需要helloworld.h,helloworld.c那么请写成:helloworld_SOURCES=helloworld.h helloworld.c.如果你在bin_PROGRAMS定义了多个可执行文件,则对应每个可执行文件都要定义相对的filename_SOURCES.
> - **automake:**我们使用automake--add-missing来产生Makefile.in.选项--add-missing的定义是"addmissingstandardfilestopackage",它会让automake加入一个标准的软件包所必须的一些文件.我们用automake产生出来的Makefile.in文件是符合GNUMakefile惯例的,接下来我们只要执行configure这个shell脚本就可以产生合适的Makefile文件了.
> - **Makefile:**在符合GNUMakefiel惯例的Makefile中,包含了一些基本的预先定义的操作.
>   **--make:**根据Makefile编译源代码,连接,生成目标文件,可执行文件.
>   **--make clean:**清除上次的make命令所产生的object文件（后缀为".o"的文件）及可执行文件.
>   **--make install:**将编译成功的可执行文件安装到系统目录中,一般为/usr/local/bin目录.
>   **--make dist:**产生发布软件包文件（即distributionpackage）.这个命令将会将可执行文件及相关文件打包成一个tar.gz压缩的文件用来作为发布软件的软件包.它会在当前目录下生成一个名字类似"PACKAGE-VERSION.tar.gz"的文件.PACKAGE和VERSION,是我们在configure.in中定义的AM_INIT_AUTOMAKE(PACKAGE,VERSION).
>   **--make distcheck:**生成发布软件包并对其进行测试检查,以确定发布包的正确性.

 

参考资料：

1，<http://www.laruence.com/2009/11/18/1154.html>

2，<https://blog.csdn.net/qq_30549833/article/details/72955881>