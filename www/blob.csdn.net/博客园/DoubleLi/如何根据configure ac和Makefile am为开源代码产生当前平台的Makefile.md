# 如何根据configure.ac和Makefile.am为开源代码产生当前平台的Makefile - DoubleLi - 博客园






|123456789|//根据configure.in和Makefile.am生成makefile的步骤，基于UBUNTU 12.041.autoscan （可选）2.aclocal3.autoconf4.autoheader（可选）5.libtoolize--automake--copy--debug--force（可选）6.automake--add-missing7.autoreconf –f –i –Wall,no–obsolete（可选）8../configure|
|----|----|




简单的方式：

如果拿到的工程文件中，没有Makefile文件，而只有configure.in和Makefile.am文件，我们是不能够直接进行编译的，必须在UBUNTU等Linux系统下，根据configure.in和Makefile.am文件生成编译所需的Makefile文件。具体操作步骤如下：
1、执行autoscan，然后执行aclocal，产生aclocal.m4文件
aclocal是一个perl 脚本程序，它的定义是：“aclocal – create aclocal.m4 by scanning configure.ac”。 aclocal根据configure.in文件的内容，自动生成aclocal.m4文件。而aclocal.m4文件中，包含了生成configure文件所必须的宏。
2、执行autoconf，生成configure文件
autoconf会根据configure.in和aclocal.m4文件，生成configure文件。其实，autoconf就是把configure.in和aclocal.m4文件中定义的内容， 变成检查系统特性、环境变量、软件必须的参数的shell脚本。
3、执行automake命令，产生Makefile.in
具体命令为：automake –add-missing
automake会根据Makefile.am文件产生一些文件，包含最重要的Makefile.in。前面所生成的configure，会根据Makefile.in文件，来生成最终的Makefile文件。
4、执行configure命令，生成Makefile文件
这样，就产生了编译所需要的Makefile文件。运行make，即可编译。







复杂点的方式：

为一个项目源文件生成makefile并make的步骤如下：

操作在包含源文件的项目目录下进行。
- (1)运行autoscan，生成文件configure.scan
- (2)修改configure.scan，改名为configure.in
- (3)运行autoheader，生成文件configure.h.in（现在一般改为configure.ac）
- (4)运行libtoolize，生成ltmain.sh
- (5)运行allocal，生成aclocal.m4
- (6)运行autoconf，生成configure
- (7)运行automake，生成makefile.in，每个包含makefile.am的子目录都生成makefile.in。automake -a选项可以补齐文件config.guess，config.sub，install-sh，missing，depcomp
- (8)运行./configure，生成config.status，config.h，makefile
- (9)运行make，生成中间文件对象文件，库文件，最后生成可执行文件
- (10)运行make install，相应的可执行文件，库文件，头文件拷贝到系统相应位置。
- 

configure运行

configure是一个shell脚本文件，由autoconf生成，它自动为源码包配置编译连接选项，适应不同的硬件平台和POSIX操作系统，输出所需要的Makefile。

configure主管检查你的系统，把结果存放到config.status中，config.status根据它的检查结果实际执行正确的动作。

configure检查与系统相关的一系列变量，这些变量存储到文件config.status中，供makefile调用。这些变量包括编译连接时需要的程序，这些程序在系统中的位置（目录），调用这些程序的选项，比如编译器的目录，编译器的选项-g是否支持等。configure能猜出它运行的系统的规范名字*cpu*–vendor–os，它通过运行脚本文件config.guess输出变量uname来猜出。configure能识别很多系统名字的别名，它通过运行脚本文件config.sub把系统名字变成规范名字。

make运行

makefile.am对makefile的影响：它根据SUBDIRS = add sub让make递归进入每个子目录处理子目录的Makefile。根据main_LDADD = add/libadd.la sub/libsub.la为main连接libadd.la和libsub.la库。

configure.in对makefile的影响：

根据AC_PROG_LIBTOOL让libtool完成编译连接工作。

根据AC_CONFIG_HEADERS([config.h])只需传递预处理宏-DHAVE_CONFIG_H给编译器。

makefile中很多与系统相关的信息都是通过变量获取的，这些变量之前已经由configure检查好存放在config.status里面，预处理 宏存放在config.h里面。比如我们要用到的编译器CC，编译器选项CFLAGS等。makefile中的变量完成替换后，开始实际执行命令，它会递 归执行每一个子目录下的makefile，生成对象文件，连接库文件，最后连接成可执行文件。

交叉编译 Cross-compiling

Q：为别的平台编译可执行程序怎么做？

交叉编译就是在目前的平台上为别的目标平台生成可执行程序或库。可以在运行configure时通过–build，–host，–target参数实现交叉编译。

例如:




|1|./configure--build=i686-pc-linux-gnu--host=m68k-coff|
|----|----|





–build=build-type ：configure和compile软件包的系统类型。默认情况等于config.guess给出的系统类型

–host=host-type ：运行软件包的系统类型。默认情况等于build类型

–target=target-type ：很少用，默认情况等于host类型。

交叉编译时，如果编译器，连接器，汇编器名字不是以host_type为前缀，configure都会发出警告。

要搭建交叉变异环境，如交叉编译用的编译器，连接器，汇编器跟本地的不一样，一般以host_type为前缀，如arm-pc-linux-gcc。

安装目录

Q：make install时，文件都安装到哪里去了？

prefix：安装目录的前缀。默认情况下/usr/local 。

bindir：安装时拷贝可执行文件到此目录。默认情况下/usr/local/bin 。

includir：安装时拷贝头文件到此目录。默认情况下/usr/local/include 。

libdir：安装时拷贝库文件到此目录。默认情况下/usr/local/libs 。

定制自己的安装目录，可以–prefix 和 –exec-prefix 给configure。

例如：./configure –prefix=/usr 。




### Configure，Makefile.am, Makefile.in, Makefile文件之间关系



![](http://www.51cos.com/wp-content/uploads/2015/12/0229c674c768a4b3fdc9ca516abd81ee.gif)









|123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116|1.autoscan(autoconf): 扫描源代码以搜寻普通的可移植性问题，比如检查编译器，库，头文件等，生成文件configure.scan,它是configure.ac的一个雏形。your source files---[autoscan*]---[configure.scan]---configure.ac2.aclocal(automake):根据已经安装的宏，用户定义宏和acinclude.m4文件中的宏将configure.ac文件所需要的宏集中定义到文件aclocal.m4中。aclocal是一个perl 脚本程序，它的定义是：“aclocal-create aclocal.m4 by scanning configure.ac”user input files   optional input     process          output files=================================================acinclude.m4-----.V.-------,configure.ac-------------------------|aclocal|{user macro files}--||-------aclocal.m4`-------'3.autoheader(autoconf): 根据configure.ac中的某些宏，比如cpp宏定义，运行m4，声称config.h.inuser input files    optional input     process          output files================    ==============     =======          ============                    aclocal.m4 - - - - - - - .                                             |                                             V                                     .----------,configure.ac ------------------------|autoheader|----- autoconfig.h.in                                     `----------'4.automake:automake将Makefile.am中定义的结构建立Makefile.in，然后configure脚本将生成的Makefile.in文件转换 为Makefile。如果在configure.ac中定义了一些特殊的宏，比如AC_PROG_LIBTOOL，它会调用libtoolize，否则它 会自己产生config.guess和config.subuser input files   optional input   processes          output files===================================================.--------,||----COPYING||----INSTALL||-------install-sh||-------missing|automake|-------mkinstalldirsconfigure.ac------------------------||Makefile.am------------------------||-------Makefile.in||-------stamp-h.in.---+|----config.guess|||----config.sub|`------+-'                                 |          | - - - -- config.guess                                 |libtoolize| - - - -- config.sub                                 |          |--------- ltmain.sh                                 |          |--------- ltconfig                                 `----------'5.autoconf:将configure.ac中的宏展开，生成configure脚本。这个过程可能要用到aclocal.m4中定义的宏。user input files   optional input   processes          output files===================================================aclocal.m4,autoconfig.h.in-------.V.--------,configure.ac------------------------|autoconf|-------configure6../configure的过程.--------------[config.cache]configure*--------------------------+--------------config.log|[config.h.in]-.v.---[autoconfig.h]+-------&gt;config.status*-+Makefile.in---'                         `---  Makefile7. make过程[autoconfig.h] -+--- make* ----  程序  Makefile   ---'----------config.site----||config.cache----|configure|----config.cache|+-,`-+-------' |                                       |         |----- config.status                   config.h.in --------|config-  |----- config.h                   Makefile.in --------|  .status|----- Makefile                                       |         |----- stamp-h                                       |         +--,                                     .-+         |  |                                     | `------+--'|ltmain.sh--------|ltconfig|--------libtool|||`-+------'     |                                       |config.guess|                                       | config.sub |                                       `------------'----------Makefile-------||config.h-------|make|{project sources}-----------------||---------{project targets}.-++--,|`--------'  |                                 |   libtool   |                                 |   missing   |                                 |  install-sh |                                 |mkinstalldirs|                                 `-------------'|
|----|----|







### 针对上面提到的 各个命令,我们再做些详细的介绍

### //http://www.laruence.com/2009/11/18/1154.html






|1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465666768697071727374|1.autoscanautoscan是 用来扫描源代码目录生成configure.scan文件的.autoscan可以用目录名做为参数,但如果你不使用参数的 话,那么autoscan将认为使用的是当前目录.autoscan将扫描你所指定目录中的 源文件,并创建configure.scan文件.2.configure.scanconfigure.scan包含了系统配置的 基本选项,里面都是 一些宏定义.我们需要将它改名为configure.in3.aclocalaclocal是 一个perl 脚本程序.aclocal根据configure.in文件的 内容,自动生成aclocal.m4文件.aclocal的 定义是 ："aclocal - createaclocal.m4 by scanning configure.ac".4.autoconfautoconf是 用来产生configure文件的.configure是 一个脚本,它能设置源程序来适应各种不同的操作系统平台,并且根据不同的 系统来产生合适的Makefile,从而可以使你的源代码能在不同的操作系统平台上被编译出来.configure.in文件的 内容是 一些宏,这些宏经过autoconf 处理后会变成检查系统特性.环境变量.软件必须的 参数的shell脚本.configure.in文件中的 宏的 顺序并没有规定,但是 你必须在 所有宏的 最前面和最后面分别加上AC_INIT宏和AC_OUTPUT宏.　　在configure.ini中：#号表示注释,这个宏后面的 内容将被忽略.AC_INIT(FILE)　　这个宏用来检查源代码所在 的 路径.AM_INIT_AUTOMAKE(PACKAGE,VERSION) 这个宏是 必须的,它描述了我们将要生成的 软件包的 名字及其版本号：PACKAGE是软件包的名字,VERSION是 版本号.当你使用make dist命令时,它会给你生成一个类似helloworld-1.0.tar.gz的 软件发行包,其中就有对应的 软件包的 名字和版本号.AC_PROG_CC　　这个宏将检查系统所用的C编译器.AC_OUTPUT(FILE)　　这个宏是 我们要输出的Makefile的 名字.　　我们在 使用automake时,实际上还需要用到其他的 一些宏,但我们可以用aclocal 来帮我们自动产生.执行aclocal后我们会得到aclocal.m4文件.　　产生了configure.in和aclocal.m4 两个宏文件后,我们就可以使用autoconf来产生configure文件了.5.Makefile.amMakefile.am是 用来生成Makefile.in的,需要你手工书写.Makefile.am中定义了一些内容：AUTOMAKE_OPTIONS　　这个是automake的 选项.在 执行automake时,它会检查目录下是 否存在 标准GNU软件包中应具备的各种文件,例如AUTHORS.ChangeLog.NEWS等文件.我们将其设置成foreign时,automake会改用一般软件包的 标准来检查.bin_PROGRAMS　　这个是 指定我们所要产生的 可执行文件的 文件名.如果你要产生多个可执行文件,那么在各个名字间用空格隔开.helloworld_SOURCES　　这个是 指定产生"helloworld"时所需要的 源代码.如果它用到了多个源文件,那么请使用空格符号将它们隔开.比如需要helloworld.h,helloworld.c那么请写成:helloworld_SOURCES=helloworld.hhelloworld.c.　　如果你在bin_PROGRAMS定义了多个可执行文件,则对应每个可执行文件都要定义相对的filename_SOURCES.6.automake　　我们使用automake--add-missing来产生Makefile.in.　　选项--add-missing的 定义是"add missing standard files to package",它会让automake加入一个标准的 软件包所必须的 一些文件.　　我们用automake产生出来的Makefile.in文件是 符合GNU Makefile惯例的,接下来我们只要执行configure这个shell 脚本就可以产生合适的Makefile 文件了.7.Makefile　　在 符合GNU Makefiel惯例的Makefile中,包含了一些基本的 预先定义的 操作：make　　根据Makefile编译源代码,连接,生成目标文件,可执行文件.make clean　　清除上次的make命令所产生的object文件（后缀为".o"的 文件）及可执行文件.make install　　将编译成功的 可执行文件安装到系统目录中,一般为/usr/local/bin目录.make dist　　产生发布软件包文件（即distribution package）.这个命令将会将可执行文件及相关文件打包成一个tar.gz压缩的 文件用来作为发布软件的 软件包.　　它会在 当前目录下生成一个名字类似"PACKAGE-VERSION.tar.gz"的 文件.PACKAGE和VERSION,是 我们在configure.in中定义的AM_INIT_AUTOMAKE(PACKAGE,VERSION).make distcheck|
|----|----|





错误处理：



When try to configure an open source code, I get error like this :

configure: error: cannot find install-sh, install.sh, or shtool in “.” “./..” “./../..”

<pre>


autoreconf –f –i –Wall,no–obsolete

./configure


</pre>







# “Libtool library used but ‘LIBTOOL’ is undefined”

<pre>sudo apt-get install autoconf
sudo apt-get install automake
sudo apt-get install libtool

重新执行aclocal/autoheader</pre>





required file `build/ltmain.sh' not found`


```
$ automake --add-missing
....
configure.in:18: required file
```
build/ltmain.sh’ not found
….
解决方案(libtoolize配置即可)：

$libtoolize –version
-libtoolize (GNU libtool) 1.4.2
…..
$libtoolize –automake –copy –debug –force











