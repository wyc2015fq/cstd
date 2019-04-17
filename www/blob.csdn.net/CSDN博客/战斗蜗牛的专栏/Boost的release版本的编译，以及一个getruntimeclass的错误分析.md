# Boost的release版本的编译，以及一个getruntimeclass的错误分析 - 战斗蜗牛的专栏 - CSDN博客





2011年06月17日 07:49:00[vbskj](https://me.csdn.net/vbskj)阅读数：2678








首
先下载bjam.exe，复制到 $BOOST$ 目录下。或者自己生成bjam，打开Visual Studio 2008 
命令提示窗口$BOOST$/tools/jam/src，执行 build.bat 
会在$BOOST$/tools/jam/src/bin.ntx86 生成 bjam.exe 文件。复制文件 bjam.exe 文件到 
$BOOST$/下。


1.完全编译安装：

bjam --toolset=msvc install

完成后会生成一个bin.v2编译时的临时目录，手动删除。生成另一个目录C:/boost，里面为所有的头文件和库文件。头文件目录为boost_1_34_1/boost目录复制过去的。


2.只编译相应的库文件

bjam --toolset=msvc stage

完成后同样会生成bin.v2临时目录。另一个目录为stage文件，里面有对应的库文件。


3.查看需要编译才能使用的库列表

bjam --show-libraries


4.编译特定的库，如只编译regex

bjam --toolset=msvc --with-regex stage

生成的库文件在stage目录中。


5.不编译某个库，如不编译regex

bjam --toolset=msvc --without-regex stage

生成的库文件在stage目录中。


6.编译特定的库，如只编译regex，生成debug，多线程，共享连接版本，并保存在stage。

bjam --toolset=msvc --with-regex stage debug threading=multi link=shared


7.生成 mt-sgd 的静态库(runtime-link-static)

bjam "-sTOOLS=vc-8_0" --with-thread install debug release runtime-link=static


8.编译regex库。

bjam
 --toolset=msvc --with-regex stage debug release threading=multi 
threading=single link=shared link=static runtime-link=shared 
runtime-link=static


boost的安装方法：


对于DLL版本

bjam --toolset=msvc link=shared runtime-link=shared threading=multi stage debug release install


对于lib版本

bjam --toolset=msvc link=static runtime-link=shared threading=multi stage debug release install



另外，在$BOOST$/tools/build/v2/user-config.jam找到下面的地文


# -------------------

# MSVC configuration.

# -------------------


# Configure msvc (default version, searched for in standard locations and PATH).

# using msvc ;


# Configure specific msvc version (searched for in standard locations and PATH).

# using msvc : 8.0 ;


#在这里添加 vs2008 的配置


using msvc : 9.0 : : /wd4819 /D_CRT_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE /D_SECURE_SCL=0 ;


#在这里添加 vs2005 的配置


using
 msvc : 8.0 : : <compileflags>/wd4819 
<compileflags>/D_CRT_SECURE_NO_DEPRECATE 
<compileflags>/D_SCL_SECURE_NO_DEPRECATE 
<compileflags>/D_SECURE_SCL=0 ;   


然后进入 $BOOST$ 目录，执行bjam.exe 编译命令


//下面的命令的各选项的说明：

//prefix    将boost安装到的路径（生成的头文件和库文件都会放到该路径中）。

//重定义以下变量（利用-s设置）：

//VC80_ROOT　　 vc2005的安装路径，如果未将vc2005安装到默认位置，你必须指定该项。

//TOOLS         使用的编译工具，vc2005对应的是vc-8_0

//PYTHON_ROOT   ython的安装目录，如果未将BOOST安装到默认位置，你必须指定该项。

//BUILD         编译结果选项，默认会生成尽可能多的版本，如调试版／发行版，静态库／动态库，单线程／多线程。



我的命令

编译静态链接库的完全版本

windows:

bjam --toolset=msvc link=static runtime-link=shared runtime-link=static threading=multi stage debug release install


linux:

bjam --toolset=gcc link=static stage debug release install



./bjam --prefix=c:/Boost --with-date_time --with-filesystem 
--with-iostreams --with-math --with-program_options --with-random 
--with-regex --with-serialization --with-signals --with-test 
--with-system --with-thread link=static runtime-link=static 
threading=multi stage debug release install





===================

从http://www.boost.org/下载boost工程, 当前版本1_45_0. 

解开boost_1_45_0.7z到D:/boost_1_45_0/

运行vs2010的命令行"Visual Studio Command Prompt (2010)", 进入到D:/boost_1_45_0/

执行bootstrap.bat, 产生了bjam.exe.

编写一个cmd, 内容如下



[view plain](http://blog.csdn.net/LostSpeed/archive/2010/12/12/6070421.aspx#)[copy to clipboard](http://blog.csdn.net/LostSpeed/archive/2010/12/12/6070421.aspx#)[print](http://blog.csdn.net/LostSpeed/archive/2010/12/12/6070421.aspx#)[?](http://blog.csdn.net/LostSpeed/archive/2010/12/12/6070421.aspx#)

- ./bjam.exe install variant=release link=shared threading=multi runtime-link=shared  


./bjam.exe install variant=release link=shared threading=multi runtime-link=shared

运行这个.cmd.

运行完毕后, 在C:/Boost/lib/生成干净的开发库.

头文件: C:/Boost/include/boost-1_45/boost

库文件和Dll: C:/Boost/lib, 文件命名都类似于boost_xx-vc100-mt-1_45.dll.

开始想用vc6来编译, 编译不过. 看了boost.org的帮助后，发现人家都是用vc7和vc8编译的.

下一步的实验是: 在vc6中使用这些boostDll.

vs2010Release版的boost库依赖msvcp100.dll, msvcr100.dll.

/boost_1_45_0/doc/html/index.html和libraries.html 中有使用boost库的例子.

编译前的/boost_1_45_0/libs目录中的*.cpp都是使用boost的例子.

用vc6实验的结果, 编译的时候，只要包含了boost的.h, 就编译不过. 





=====================

GetRunTimeClass的错误



在。h中增加DECLARE_DYNAMIC(CLogoDlg)

在。cpp头增加

IMPLEMENT_DYNAMIC(CLogoDlg, CDialog)



