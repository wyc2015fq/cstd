# Boost Getting Started 安装文档（翻译1.33版本） - ljx0305的专栏 - CSDN博客
2009年06月29日 15:23:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：869标签：[文档																[microsoft																[python																[工具																[library																[cvs](https://so.csdn.net/so/search/s.do?q=cvs&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)
个人分类：[Linux程序及使用																[linux下软件安装](https://blog.csdn.net/ljx0305/article/category/440323)](https://blog.csdn.net/ljx0305/article/category/394700)
- [概要](#Introduction)
- [下载](#Download)- [.zip 文件](#.zip)
- [.tar.gz and .tar.bz2 文件](#.tar.gz)
- [Boost CVS 库](#CVS)
- [准备](#Preparation)- [配置工具](#Configuring)
- [支持的编译器](#Tools)
- [编译安装](#Build_Install)
- [结果](#Results)
- [在Windows下面自动链接](#auto-link)
- [附加的步骤](#Additional_Steps)
## 概要
这个概要将帮助你开始利用Boost库。通过如下的步骤来取得、生成、安装Boost。下面是生成安装Boost的概要
- [下载 Boost](#step1).
- [安装 Boost.Jam](#step2).
- [配置你的编译工具](#step3).
- [去Boost发行版文件夹](#step4).
- [生成和安装](#step5).
## 下载
|**1**|Boost库通过SourceForge文件发布系统来进行发布。可以**[从SourceForge下载得到Boost发行版](http://sourceforge.net/project/showfiles.php?group_id=7586&package_id=8041)**. 并把它解压缩到一个合适的地方。|
你从网站上可以得到Boost发行版的所有的库和其他文档。他们被打包为[ZIP](#.zip), [TAR.GZ](#.tar.gz), and [TAR.BZ2](#.tar.gz) 文件格式。 （在这里）既可以得到老的版本。
也可以从[CVS 库](#CVS)同步到当前Boost最新的快照版本。 
### .zip file
.zip 格式被自由和商业的压缩/存档工具广泛的支持。如果你还没有一个.zip文件解压器。那么可以从 [Info-ZIP](http://www.info-zip.org/) 网站下载一个，这个网站提供了不少操作系统上面的版本。 
文本文件的行结束符如果打包为.zip 作为被每一个库开发者支持的（东西）（Text file line endings in the .zip file are as supplied by each library developer），在windows下面工作很好,在linux/unix下面就没那么漂亮了。.tar.gz和.tar.bz2支持对于linux/unix更友好的文件换行符 
### .tar.gz 和 .tar.bz2 文件
.tar.gz 格式在Unix/Linux下面被广泛的支持。一些Windows的压缩/归档工具也可以顺利读取。因为gzip文档把所有文件看成一个文档压缩，所以要比分别压缩文件的zip压缩出来的文档要小一些。 
.tar.bz2 格式在Unix/Linux下面被广泛的支持。并且其还被整合到一些tar工具中，这个格式采用了和.tar.gz不同的压缩方法。所以其可以创建更加小的文件。 
为了在Unix/Linux环境下更容易的使用，Text文件换行符，在.tar.gz和.tar.ba2文件中已经被转换成一个新行。
### Boost CVS 库
所有的Boost文档，包括实体发行树(代码)，包括网站HTML文档都被CVS服务器管理。（Boost网站）提供了命令行，GUI，浏览器访问方式。 
#### 通过命令行或者CVS客户端访问CVS
(Boost)库可以从对那些安装了CVS客户端的，可以去访问公共[Boost CVS 仓库](http://sourceforge.net/cvs/?group_id=7586)。Unix/Linux环境的用户可以使用命令行来访问，Windows,Mac或者其他环境的用户则可以通过GUI来访问。 
请参考改良过的SourceForge[CVS使用指南](http://sourceforge.net/docman/?group_id=1)第F节。它包含了各种各样的客户端的访问链接。 
常规的访问方法如下： 
> `cvs -d:pserver:anonymous@cvs.sourceforge.net:/cvsroot/boost login`
[Hit <return> when it asks for a password]
`cvs -z3 -d:pserver:anonymous@cvs.sourceforge.net:/cvsroot/boost checkout boost`
`cvs -d:pserver:anonymous@cvs.sourceforge.net:/cvsroot/boost logout`
请阅读你的CVS手册来获得更多的信息。
这种访问是只读的；如果你是一个库的作者并且打算向CVS提交代码，请联系 [相关人员](moderators.html)。
#### 通过网页浏览器访问Boost CVS
你可以近代的浏览器来访问[Boost CVS 文件](http://cvs.sourceforge.net/cgi-bin/viewcvs.cgi/boost/boost/)   你可以通过颜色的不同看到一个文件被更新了多少次 **注意:**这个接口只是用来显示个别文件的修改历史 从CVS里的BoostBook产生文档
一些Boost文档产生自CVS仓库里面的[BoostBook XML](../doc/html/boostbook.html) 代码，这些代码并没有被转换成可以阅读的HTML参考文档。可以访问[Nightly Generated 文档](http://www.boost.org/regression-logs/cs-win32_metacomm/doc/html/libraries.html)来查看通过"nightly build"生成的HTML文档。 
## 准备
Boost推荐使用[Boost.Build（Boost编译生成系统）](tools/build/v1/build_system.htm)来生成安装Boost库。 本文档剩下的部分解释它的用法。但是用不用这个系统就看你的需求了。注意，一些虽然库同样也包含了非Boost.Build的make文件或者工程文件。 但是Boost.Build却包含了构建Boost所需要的全部文件。 
|**2**|这个生成系统使用[Boost.Jam](../tools/build/jam_src/index.html),一个 [Perforce Jam](http://www.perforce.com/jam/jam.html) 扩展版本--这是*make* 一种轻量级的替代品。 推荐你去SourceForge**[下载一个Boost.jam的可执行文件](http://sourceforge.net/project/showfiles.php?group_id=7586&package_id=72941)** 如果你所使用的环境没有预先编译好的可执行的Boost.Jam或者如果你在使用一个还没有发布的Boost源码。那么你就需要 [从代码手动生成bjam](../tools/build/jam_src/index.html#building_bjam) ， 而bjam的代码也包含在Boost的源码树里面。编译好Boost.Jam以后，把bjam的执行文件放在你的Boost根目录下面就可以了。|
### 配置编译工具
在使用Boost.Build之前，你需要指定编译工具，有两种方法可以做到使得Build.System正常的工作。 
- 用户可以用常规的方法为它他算使用的每一套编译工具集设置一套变量。 例如，设置Microsoft VC++, 就可以在PATH变量中添加...VC98/Bin 或者 .../VC7/Bin的路径， 还需要编译之前调用一次VCVAR32.BAT或者VSVAR32.BAT（这样它就可以用VC来编译Boost了）。 而使用CodeWarrior , 就应该调用cwenv.bat，而且其他的Metrowerks工具/命令行工具也应该在path变量中。 大多数的Unix系统都会预先配置好这些变量而不需要用户来干预。 
- 某一个用户不打算让他的变量被弄乱设置或者他也不是很了解他的编译工具的配置。作为一种替代，他(她)可以设置一个变量指向她的编译工具的安装目录， 这个变量可以事先在命令行环境变量里面声明，或者在调用`bjam`时声明， 知道定义好这个变量， 那些被用到的变量会被自动的找到并且设置好。这个变量就是-s，例如： 
bjam "-sGCC_ROOT_DIRECTORY=/usr/local/gcc-3.3.2"
一些变量，例如工具集变量TOOLS ，可以接收多个用空格分开的值。 其他的，比如上述的path，可以包含空格。对于一些环境，你还可以引用其他适当的设置方法（译者注：这可能是说可以使用shell变量一类的东西）。 
### 被支持的编译工具
|**3**|下面是Boost.Build支持的编译工具，为了[配置](#Configuring) 那些工具，可以点击最左边的名字的链接。|
|**编译工具名**|**说明**|
|----|----|
|[`borland`](../tools/build/v1/borland-tools.html)|[Borland](http://www.borland.com/bcppbuilder/freecompiler) C++|
|[`como`](../tools/build/v1/como-tools.html)|[Comeau C++](http://www.comeaucomputing.com) compiler front-end for non-Windows platforms|
|`[como-win32](../tools/build/v1/como-tools.html)`|[Comeau C++](http://www.comeaucomputing.com) compiler front-end for Windows, using Microsoft [Visual C++](http://msdn.microsoft.com/visualc/)as a back-end.|
|[`cw`](tools/build/v1/cw-tools.html)|[Metrowerks CodeWarrior](http://www.metrowerks.com) Pro 6.x, 7.x, 8.x, and 9.x command-line tools|
|[`darwin`](../tools/build/v1/darwin-tools.html)|Apple Darwin OS hosted [GNU](http://www.gnu.org/)[GCC](http://developer.apple.com/tools/compilers.html)|
|[`dmc`](../tools/build/v1/dmc-tools.html)|[Digital Mars C++](http://www.digitalmars.com/).|
|[`dmc-stlport`](../tools/build/v1/dmc-stlport-tools.html)|[Digital Mars C++](http://www.digitalmars.com/), using the [STLport](http://www.stlport.org) standard library implementation|
|[`edg`](../tools/build/v1/edg-tools.html)|[Edison Design Group](http://www.edg.com/) compiler front-end (evaluation version)|
|[`gcc`](../tools/build/v1/gcc-tools.html)|[GNU GCC](http://gcc.gnu.org) on Unix and [Cygwin](http://www.cygwin.com).|
|[`gcc-stlport`](../tools/build/v1/gcc-stlport-tools.html)|[GNU GCC](http://gcc.gnu.org) on Unix and [Cygwin](http://www.cygwin.com), using the [STLport](http://www.stlport.org) standard library implementation|
|[`gcc-nocygwin`](../tools/build/v1/gcc-nocygwin-tools.html)|[GNU](http://www.gnu.org/) GCC [Cygwin](http://www.cygwin.com/) command line compiler tools running in "no-cygwin" mode (produces commercially redistributable objects)|
|[`intel-linux`](../tools/build/v1/intel-linux-tools.html)|[Intel C++ for Linux](http://www.intel.com/software/products/compilers/c60l/)|
|[`intel-win32`](../tools/build/v1/intel-win32-tools.html)|[Intel C++ for Windows](http://www.intel.com/software/products/compilers/c60/) using the Dinkumware standard library in the Intel-required Microsoft [Visual C++](http://msdn.microsoft.com/visualc/) 6 or 7 installation|
|[`kcc`](../tools/build/v1/kcc-tools.html)|KAI C++|
|[`kylix`](../tools/build/v1/kylix-tools.html)|[Borland C++ for Linux (Kylix).](http://www.borland.com/kylix)|
|[`mingw`](../tools/build/v1/mingw-tools.html)|[GNU](http://www.gnu.org/) GCC and associated tools in [MinGW](http://www.mingw.org) configuration (produces commercially redistributable objects)|
|[`mingw-stlport`](../tools/build/v1/mingw-stlport-tools.html)|[GNU](http://www.gnu.org/) GCC and associated tools in [MinGW](http://www.mingw.org) configuration (produces commercially redistributable objects), using the [STLport](http://www.stlport.org) standard library implementation|
|[`mipspro`](../tools/build/v1/mipspro-tools.html)|SGI [MIPSpro C and C++](http://www.sgi.com/developers/devtools/languages/mipspro.html)|
|[`msvc`](../tools/build/v1/msvc-tools.html)|Microsoft [Visual C++](http://msdn.microsoft.com/visualc/) version 6 command-line tools. NOTE; For version 7.x (the .NET series) use the vc7, vc-7_1, or vc-8_0 toolsets below.|
|[`msvc-stlport`](../tools/build/v1/msvc-stlport-tools.html)|Microsoft [Visual C++](http://msdn.microsoft.com/visualc/) version 6 command-line tools, using the [STLport](http://www.stlport.org) standard library implementation. NOTE; For version 7.x (the .NET series) use the vc7-stlport or vc-7_1-stlport toolsets below.|
|[`sunpro`](../tools/build/v1/sunpro-tools.html)|[SunPRO C++](http://wwws.sun.com/software/sundev/suncc/index.html) compiler|
|[`tru64cxx`](../tools/build/v1/tru64cxx-tools.html)|[Compaq C++](http://www.tru64unix.compaq.com/cplus/) for Tru64 UNIX (versions prior to 6.5)|
|[`tru64cxx65`](../tools/build/v1/tru64cxx-tools.html)|[Compaq C++](http://www.tru64unix.compaq.com/cplus/) Version 6.5 for Tru64 UNIX|
|[`vacpp`](../tools/build/v1/vacpp-tools.html)|[IBM Visual Age C++](http://www-3.ibm.com/software/ad/vacpp/) command-line tools|
|[`vc7`](../tools/build/v1/vc7-tools.html)|Microsoft [Visual C++](http://msdn.microsoft.com/visualc/) command-line tools from Visual Studio .NET.|
|[`vc7-stlport`](../tools/build/v1/vc7-stlport-tools.html)|Microsoft [Visual C++](http://msdn.microsoft.com/visualc/) command-line tools from Visual Studio .NET + STLPort.|
|[`vc-7_1`](../tools/build/v1/vc-7_1-tools.html)|Microsoft [Visual C++](http://msdn.microsoft.com/visualc/) command-line tools from Visual Studio .NET 2003.|
|[`vc-7_1-stlport`](../tools/build/v1/vc-7_1-stlport-tools.html)|Microsoft [Visual C++](http://msdn.microsoft.com/visualc/) command-line tools from Visual Studio .NET 2003 + STLPort.|
|[`vc-8_0`](../tools/build/v1/vc-8_0-tools.html)|Microsoft [Visual C++](http://msdn.microsoft.com/visualc/) command-line tools from Visual Studio .NET 2005.|
## Build and Install
共同的编译安装处理流程都是被top-level build文件([Jamfile](Jamfile))驱动的。
|**4**|首先你需要进入你的Boost发行版的目录，例如：> `chdir boost-1.31.0`|
默认情况下，Boost将试图编译全部的库并且把库和头文件安装到默认的路径中去。 在Unix系统中，默认的路径是"/usr/local"。 而Windows系统则是 "C:/Boost"。Boost库将会被安装到这些文件夹的“lib”子文件夹下面。 而头文件则会被安装到"<include/boost-1_33>"子文件夹里面。所有的这些设定都是来源于你所用的系统。 
|**5**|调用build system，指定 [编译工具](#Tools)来安装，比如说，GNU/GCC。> `bjam "-sTOOLS=gcc" install`如果你只打算生成库并且把编译好的库收集到一个文件夹里面而不打算安装它们。> `bjam "-sTOOLS=gcc" stage`|
这个编译和安装系统可以通过一个GNU风格的参数集合来控制。这些命令参数可以允许你指定安装目录，不安装某一个库， 下面是这些命令和参数的一些介绍。你还可以用bjam --help"来查看这些参数。
> 
`bjam [*options*...] [install|stage]`
-sNO_COMPRESSION=1生成Boost.Iostreams库的时候不使用外部的zlib和libbz2来生成压缩库。 如果你用Windows，那么这个选项默认为1（就是不生成压缩库）；如果你用unix，则压缩库很有可能默认就被生成了，所以如果这个参数不起作用， 你可以参考[Boost.Iostreams安装文档](http://www.boost.org/libs/iostreams/doc/index.html?path=7)
这里有一些 [Boost.Build](../tools/build/v1/build_system.htm) 和 [Boost.Jam](../tools/build/jam_src/index.html) 支持的附加参数。其中最重要的可能就是"-sBUILD="。这个参数允许你覆盖默认的编译目标。参数值"<features/variants>" 是一个用空格分隔的列表。features以一个tag和一个或多个值的方式出现；variant则是features集合做附加说明的一个简单的符号名字。例如默认的编译目标是 "debug release <runtime-link>static/dynamic <threading>single/multi" 其中"debug" 和 "release"就是variant，那么每一个features都会带有这两个variant. 
如果你有些关于编译安装处理的一件。请写信给[Boost.Build mailing list](mailing_lists.htm#jamboost) 我们会认真的阅读您提出的意见。 
## 生成物
生成物包括：静态库，动态库，而不同的环境会让库的名称也不一样。一个简单的Boost库默认将会产生八个不同的库。 例如Boost.Datetime库在Unix 类系统下面将会被处理成：
libboost_date_time-gcc-d-1_31.solibboost_date_time-gcc-mt-d-1_31.solibboost_date_time-gcc-1_31.solibboost_date_time-gcc-mt-1_31.solibboost_date_time-gcc-d-1_31.alibboost_date_time-gcc-mt-d-1_31.alibboost_date_time-gcc-1_31.alibboost_date_time-gcc-mt-1_31.a
.a
### 库前缀
一些系统要求库应该具有“lib”前缀，例如Unix，和其他的例如在Winodws运行的GCC，因此在所有的Unix类型的系统和Windows上使用的Boost静态库上面都会被加上这个前缀。 而Unix共享库，和windows静态库则被分别命名为
lib*.solib*.a
而Winodws的共享库则没有prefix前缀来区别“import libraries”和"static libraries"。因此在Windows下面， 共享库被命名为： 
|*.dll|共享库版本|
|----|----|
|*.lib|dll（共享库）的"Import library"|
|lib*.lib|"Static library"版本|
### 库名称
Boost"会用boost_" 前缀来命名库用以区别您的系统上其他的库。 
### 编译工具
编译工具名字是你基于你所使用的的编译工具的缩写。这个缩写由编译器的名称(2-4个字符)和该编译器的大小出版号组成的。 。举一个例子如果你的编译工具是"gcc-3_2_3" 那么编译工具的tag就会是"gcc32"。各个编译器的缩写如下： 
|**编译工具名称**|**缩写**|
|----|----|
|[`borland`](../tools/build/v1/borland-tools.html)|bcb|
|[`como`](../tools/build/v1/como-tools.html)|como|
|`[como-win32](../tools/build/v1/como-tools.html)`|como|
|[`cw`](tools/build/v1/cw-tools.html)|cw|
|[`darwin`](../tools/build/v1/darwin-tools.html)|osx|
|[`dmc`](../tools/build/v1/dmc-tools.html)|dmc|
|[`dmc-stlport`](../tools/build/v1/dmc-stlport-tools.html)|dmc|
|[`edg`](../tools/build/v1/edg-tools.html)|edg|
|[`gcc`](../tools/build/v1/gcc-tools.html)|gcc|
|[`gcc-stlport`](../tools/build/v1/gcc-stlport-tools.html)|gcc|
|[`gcc-nocygwin`](../tools/build/v1/gcc-nocygwin-tools.html)|gcc|
|[`intel-linux`](../tools/build/v1/intel-linux-tools.html)|il|
|[`intel-win32`](../tools/build/v1/intel-win32-tools.html)|iw|
|[`kcc`](../tools/build/v1/kcc-tools.html)|kcc|
|[`kylix`](../tools/build/v1/kylix-tools.html)|bck|
|[`mingw`](../tools/build/v1/mingw-tools.html)|mgw|
|[`mingw-stlport`](../tools/build/v1/mingw-stlport-tools.html)|mgw|
|[`mipspro`](../tools/build/v1/mipspro-tools.html)|mp|
|[`msvc`](../tools/build/v1/msvc-tools.html)|vc|
|[`msvc-stlport`](../tools/build/v1/msvc-stlport-tools.html)|vc|
|[`sunpro`](../tools/build/v1/sunpro-tools.html)|sw|
|[`tru64cxx`](../tools/build/v1/tru64cxx-tools.html)|tru|
|[`tru64cxx65`](../tools/build/v1/tru64cxx-tools.html)|tru|
|[`vacpp`](../tools/build/v1/vacpp-tools.html)|xlc|
|[`vc7`](../tools/build/v1/vc7-tools.html)|vc|
|[`vc7-stlport`](../tools/build/v1/vc7-stlport-tools.html)|vc|
|[`vc-7_1`](../tools/build/v1/vc-7_1-tools.html)|vc|
|[`vc-7_1-stlport`](../tools/build/v1/vc-7_1-stlport-tools.html)|vc|
|[`vc-8_0`](../tools/build/v1/vc-8_0-tools.html)|vc|
|Others|The first part of the toolset name.|
### 线程模式
这个tag标志着这个库所支持的线程。如果多线程被支持，那么这个tag就会被加上"-mt"，反之则不加。 
### 运行时
这定义了编译好的库的“compiled against”的运行时类型和编译时其二进制编码目标。每一中运行时类型和和生成代码参数都用一个字母表示。
|**关键字**|**特征**|
|----|----|
|s|静态链接库|
|g|调试运行库|
|y|调试Python系统|
|d|激活调试代码|
|p|STLport运行时库，代替宿主编译器的运行时库|
|n|STLport运行时库使用“本地”IO流代替STLportIO流|
举一个例子，如果你用STLport和本地的IO流库来编译一个调试模式的库，那么这个库的命名就是 "-sgdpn"。
### Boost 版本
这标志着Boost库的版本号码。由主版本号和副版本号和一个下划线组成。例如1.31.0对应的就是 
举一个例子，如果你用STLport和本地的IO流库来编译一个调试模式的库，那么这个库的命名就是 "-1_31"。如果这里面还包括了补丁号，比如说1.31.1，那么就命名为"-1_31_1" 
### 库类型
这标识了库的类型，这时根据环境不同而不同的。在windows上面共享库是.dll 静态库则是".lib"。在unix上面静态库是".a"，动态库是".so"。 一些编译工具在Unix还会支持全版本号(例如".so.1.31.0")的符号链接。 
## 在Windows自动链接 
Boost拥有自动链接的特性，即：只要你include了库的头文件就可以链接到正确的库。 为了这个特性，你所使用的编译器一定要支持`#pragma comment(lib, name)` Microsoft Visual C++, Intel C++, Metrowerks C++ , 和 Borland C++ 都支持这个)。 
如果你链接到动态运行库，那么你可以选择链接到静态链接库和动态链接库的其中任意一个， 默认是静态链接，你可以定义 BOOST_WHATEVER_DYN_LINK宏 强制使Boost链接到动态链接库。 
这个特性可以利用BOOST_WHATEVER_NO_LIB强制关闭，或者在Boost全局定义BOOST_ALL_NO_LIB
如果你打算观察库的链接情况，那么就可以定义BOOST_LIB_DIAGNOSTIC。 然后使用`#pragma message` 来观察选择库的情况。
这里有一些Boost库([Boost.Test](../libs/test/doc/index.html) 一个测试特例) 是不支持自动链接的，至于技术原因，请参考每一个库的文档，而 [Boost.Config](libs/config/index.html) 文档提供了更多的配置macro的信息。 下面的列表是当前支持的配置 (只有头文件的库没有列在这里)： 
|**Library**|**Static Link**|**Dynamic Link**|**Default linkage**|**Automatic library selection**|**Comments**| |
|----|----|----|----|----|----|----|
|Date-Time|Yes|Yes|static|Yes|| |
|Filesystem|Yes|Yes|static|Yes|| |
|Graph|Yes|No|static|No||The separate Graph library source is needed only when [reading an AT&T graphviz file.](../libs/graph/doc/read_graphviz.html)|
|Iostreams|Yes|Yes|static|Yes|| |
|Program Options|Yes|Yes|static|Yes|| |
|Python|Yes|Yes|dynamic|No|Since all Python extensions are DLL's it makes sense to dynamic link to the Boost Python library by default (static linking is only really an option if you are embedding python).| |
|Regex|Yes|Yes|static|Yes|| |
|Serialization|Yes|Yes|static|Yes|| |
|Signals|Yes|Yes|static|Yes|| |
|Test|Yes|No|static|No|Which library you link to depends upon which program entry point you define, rather than which Boost.Test features you use.| |
|Thread|Partial|Yes|static (Visual C++), otherwise dynamic|Yes|For technical reasons static linking is supported on only one Windows compiler (Visual C++).| |
|Wave|Yes|Yes|static|Yes|| |
## 附加的步骤
根据环境的不同，你可能在安装配置Boost之前进行一些附加的配置。
Depending on your platform and configuration you may need to perform some additional configuration to get Boost to build and install.
[配置Boost源代码](libs/config/config.htm#configuring)在大多数环境下可能不需要这一步，但是如果你打算在Boost没有支持、测试过的环境下生成Boost库， 那么这一步就是必须的。
如果Boost.Build检测到你的Python的安装有问题，那么Boost.Build将会打印一些短消息来告诉你如何配置才能找到Python。 更多的信息可以看更详细的[Python指令](../libs/python/doc/building.html#building). 
Revised $Date: 2005/11/23 14:09:43 $
Copyright © Rene Rivera 2003.
Copyright © Jens Maurer 2001.
Copyright © John Maddock 2004.
Distributed under the Boost Software License, Version 1.0. (See accompanying file [LICENSE_1_0.txt](LICENSE_1_0.txt) or copy at [www.boost.org/LICENSE_1_0.txt](http://www.boost.org/LICENSE_1_0.txt))
引用于：[http://blog.csdn.net/goodboy1881/archive/2006/03/27/640004.aspx#Download](http://blog.csdn.net/goodboy1881/archive/2006/03/27/640004.aspx#Download)
