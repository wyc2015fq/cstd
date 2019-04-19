# BoostPro - 三少GG - CSDN博客
2012年12月13日 14:23:55[三少GG](https://me.csdn.net/scut1135)阅读数：2716
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)

# 在Windows下使用c++ boost库开发(YC) 
###### 作者： 发布时间：APRIL 22, 2012 分类：[原创技术](http://loccs.sjtu.edu.cn/typecho/index.php/category/tech/)
[http://loccs.sjtu.edu.cn/typecho/index.php/archives/19/](http://loccs.sjtu.edu.cn/typecho/index.php/archives/19/)
boost是一个功能强大、包罗万象的C++库。由于其代码量庞大，在开发小型项目时（如LochsEmu）并不将其包含在项目中，而且不上传至版本库。本文主要介绍在Windows下使用boost库开发的方法。 由于boost代码量十分庞大，采用下载源代码并自行编译的方法是不明智的（时间长，占用大量磁盘空间）。因此，我们使用第三方的预编译版本（www.boostpro.com）。
首先，在[http://www.boostpro.com/download/](http://www.boostpro.com/download/) 下载最新版本的installer；
[**http://boostpro.com/download/boost_1_47_setup.exe**](http://boostpro.com/download/boost_1_47_setup.exe)
然后，运行installer，在Select default variants选项页中，Compiler根据需要选择相应的开发环境，Variants根据项目配置，一般选择Multithreaded DLL/Multithreaded Debug DLL即可；安装路径一般可以不安装在系统盘中，这样在重装系统后无需再次安装即可直接使用；
最后，在MSVC中配置boost路径（Tools > Options > Projects and Solutions > VC++ Directories，输入Include files和Library files；如安装了Visual Assist X，同样在其选项中的类似位置配置）。
这样，就可以在C++项目中#include <boost/XXXXX.hpp>来使用boost库了。
ps.在使用某些Header only的库之后编译速度受到严重影响，如使用了boost::spirit库，编译一个使用Spirit的cpp文件用时将近30s（已建立pch），已经赶上整个LochsEmu的编译时间了。
///////////////////
VC++2010下编译STLport，Boost
比来在想向Boost转移，尽力把握Boost代码的过程中，
STLport版本：5.2.1
Boost版本：1.4.6.1 （1.4.7.0也OK）
编译器Visual Studio 2010
STLPort的编译，选择Microsoft Visual Studio 20010下面的Visual Studio Tools下面的号令行景象编译对象，Visual Studio Command Prompt（2010）。
进入STLport目次，运行
```cpp
cd E:\HaveFun\STLport\STLport.5.2.1
configure.bat msvc9
cd buidl/lib
nmake clean install
```
但编译会呈现错误，
```cpp
E:\HaveFun\STLport\STLport.5.2.1\stlport\stl/_cstdlib.h（158） : error C2084: function ""__int64 abs（__int64）"" already has a body
C:\Program Files\Microsoft Visual Studio 10.0\VC\INCLUDE\../include/stdlib.h（471） : see previous definition of ""abs""
NMAKE : fatal error U1077: """C:\Program Files\Microsoft Visual Studio 10.0\VC\BIN\cl.EXE""" : return code ""0 x2""
```
估计是STLport还没有支撑2010的题目导致的，搜检了以代码，和google了一下题目，将_cstdlib.h（158）代码批改为，
```cpp
＃if !defined（_STLP_MSVC） || （_STLP_MSVC < 1600）
inline _STLP_LONG_LONG abs（_STLP_LONG_LONG __x） { return __x < 0 ？ -__x : __x; }
＃endif
```
重视_STLP_MSVC 和 _MSC_VER 是一个东东，就是VC的版本号，
STLPort编译很快就完成了，如今的机械搞定这个库已经是小意思了。默认应当有3个STLport版本。别的有一种_x后缀的版本，用于交叉编译，默认没有编译生成（须要 configure.bat –x参数生成makefile）。
|相干文件|相干的文件|申明|
|----|----|----|
|动态库版本|stlport.5.2.dll stlport.5.2.libstlportd.5.2.dll stlportd.5.2.lib|你编译的工程应用动态runtime library时会主动链接这些库由STLPort内部由宏_STLP_USE_DYNAMIC_LIB把握，（一般不消你添加这个宏，STLPort内部会帮你决意）|
|调试模式的动态库版本 _STLP_DEBUG （STL safe） mode|stlportstld.5.2.dll stlportstld.5.2.lib|你应用宏_STLP_DEBUG把握|
|调试模式的动态库版本 _STLP_DEBUG （STL safe） mode|stlportstld_static.lib||
|静态库版本|stlport_static.libstlportd_static.lib|你编译的工程应用静态runtime library时应用，默认选择这些库，宏_STLP_USE_STATIC_LIB把握，（一般不消你添加这个宏，STLPort内部会帮你决意）|
|用于不合的运行时库交叉编译的库|_x._statix||
STLport会主动进行链接，并且是按照你工程选择的是应用动态runtime library还是静态runtime library进行把握，决意让你应用静态库还是动态库，所以你若是欲望你的工程应用动态runtime library，同时链接STLport的静态库是不成能的（当然本身编译一套另说）。
若是你欲望在你的代码里面优先应用STLport的代码，而不是微软的默认STL实现，你要调剂一下设置，VC2010的目次设置产生了很大的调剂。以前的版本设置路径都在菜单tools\options下VC++ Directory下面进行调剂，便利快捷，VS2010做了一个很让人不爽的批改设计，你必须先打开一个工程，打开菜单view\property Manager，选择工程，选工程下DEBUG|WIN32和RELEASE|WIN32下面的Microsoft.Cpp.Win32.user，重视这两层菜单都可以打开选项界面，然则一个是工程的，一个是全局的（必然要选择Microsoft.Cpp.Win32.user这层），我经常搞混，然后一头雾水。
![](http://pic-server2.byywee.com/M0/S604/604555-0.jpg)
然后再进行目次调剂，重视要去掉持续选项，然后将STLPort的目次要放到所有目次前面，让回将默认的一条条输入到厥后面去（对于持续来的无法调剂目次的先后次序），比本来麻烦不少。据说这个批改是为了VS2010测试版时有人欲望对不合的用户供给不合的Libaray经管，我靠他祖先板板，法度员有几小我和他人共用PC的，并且还要编程目次不一致。
BOOST的编译就加倍疾苦一些了。起首我的4核机械上要编译几个小时才干编译完成。大师要有点耐烦。
当然若是你的请求不高，你可以选择一个简单的办法，获取BOOST已经编译好的库，liangfeng推荐了一个网站给我，www.boostpro.com 。上方会供给各类版本VS版本，各个版本号的BOOST库供你下载，并且都是全主动化的，很是便利。如下图：
![](http://pic-server2.byywee.com/M0/S604/604555-1.jpg)
可以看出，BOOSTPro供给了VC2003，VC2005，VC2008，VC2010的，多线程动态runtime library动态库版本，多线程动态runtime library静态库版本，多线程静态runtime library静态库版本（每个都10多M），单线程静态runtime library静态库版本。重视你如果全手下载的化，有好几个G的文件，下载的过程也要有点耐烦。
当然全主动化的成果是由价格的，一是下载给你的库没有供给pdb文件，若是你想调试boost库的代码，发明题目，这招就不要用了。二是，他供给的下载源代码文件里面只有头文件，三是，若是你想用STLport+BOOST（STLport作为底层库，而不是应用微软的实现）的体式格式，BOOSTPro网站没有供给这种库给你下载。四是，今朝他没有64位的版本。
我因为想用STLPort调换现有的库实现，我也要选择本身进行编译。
要申明的是BOOST库今朝的版本已经很轻易编译了，并且会主动辨认那些库可以编译，那些不克不及编译了。仍然选择Visual Studio Command Prompt（2010） 进入boost的根目次，
cd E:\HaveFun\Boost\boost.1.46.1
bootstrap.bat
bjam.exe就临盆了。
Boost的build申明文档如裹脚布一样，又臭又长，若是你没有耐烦直接看bjam的帮助，
bjam —-help
但不利的是这里面又没有申明若是要用STLPort怎么帮，这个只有看帮助文档。若是要用STLPort作为底层编译BOOST，具体见BOOST的申明，Chapter 33. Boost.Build V2 User Manual 中的Reference
```cpp
STLport library
The STLport library is an alternative implementation of C++ runtime library. Boost.Build supports using that library on Windows plat. Linux is hampered by different naming of libraries in each STLport version and is not officially supported.
Before using STLport， you need to configure it in user-config.jam using the following syntax:
using stlport : [version] : header-path : [library-path] ;
Where version is the version of STLport， for example 5.1.4， headers is the location where STLport headers can be found， and libraries is the location where STLport libraries can be found. The version should always be provided， and the library path should be provided if you""re using STLport""s implementation of iostreams. Note that STLport 5.* always uses its own iostream implementation， so the library path is required.
When STLport is configured， you can build with STLport by requesting stdlib=stlport on the command line.
```
翻译过来就是若是你想欲望默认应用STLPort的库，你要编辑project-config.jam 增长一行
using msvc ;
using stlport : 5.2.1 : E:/HaveFun/STLport/STLport.5.2.1/stlport : E:/HaveFun/STLport/STLport.5.2.1/lib ;
＃重视路径用/，或者\\
申明一下编译参数和属性，
threading=single，multi 当然单线程版本你用的干嘛，不编译还可以节俭一般的时候。
link=shared，static 生成的库是静态还是动态
runtime-link= shared，static，运行时库应用动态还是静态的。
stdlib=stlport 应用STLport
--build-type=commplete，minimal commplete标识所有的版本都编译，minimal只编译多线程的静态库版本。
--stagedir=./stage 指本地构建的存放库的目次，
--show-libraries 显示所有须要编译的库，其实不久不多，天知道为什么要了那么大的空间。
--with-<lib> 零丁编译你想编译的库
--without-<lib> 编译所有的库，除了你制订不编译的以外。
然后就可以开端编译了，重视路径名称
```cpp
bjam threading= multi link=shared，static stdlib=stlport --stagedir=./stage
```
然后就是漫长的守候……，守候……守候……若是全部编译，你至少可以用这个时候看完一部电影……
1.46版本生成的静态库在stage\lib下，动态库和响应的链接库在bin.v2下面。bin.v2下面，目次一层又一层，建议你全部搜刮了拷贝出来，放在一个目次下（我一般喜好把所有的库放到一个目次下面）。1.47版本如同全部放到stage目次下了，省事不少。
Boost是按照每个库生成对应的.lib文件的，其实不熟悉打听为什么不归并成一个库文件。
别的，BOOST也采取了帮助你主动链接的技巧，可以省却你一些麻烦。BOOST库的定名比STLPort还要好目炫纷乱，后缀含义的确是乱花渐欲迷人眼。比如libboost_regex-vc100-sgdp-1_46_1.lib。
前面是库名称，vc100默示编译器，最后面是版本号。下面来申明一下中心那些字符的含义:

|版本号中标识表记标帜的含义|默示的含义|
|----|----|
|s|静态库版本|
|gd|调试版本|
|p|应用STLport作为STL实现的版本，|
|||
重视BOOST DEBUG版本应用的是带调试模式的STLPort，编译你的代码必须定义_STLP_DEBUG=1，这时辰编译器归去链接调试模式的STLport版本。
比来筹算卖力进修一下Boost库，筹办在日常工作中，用Boost调换本身常用的ACE库作为跨平台实现。大师耐烦守候后续的文章。
【本文作者是fullsail（雁渡寒潭），本着自由的精力，你可以在无盈利的景象完全转载此文档，转载时请附上BLOG链接:http://blog.csdn.net/fullsail，不然每字一元不讲价。对Baidu文库加价一倍】
/////////////////
好饱 2012-5-14 11:38:03阅读(535)[评论](http://blog.okbase.net/haobao/archive/3.html#comments)(0)
作者：AndyUk06
翻译：好饱的程序员博客
介绍
这篇文章通过一步一步的介绍帮助初学者学习在Visual Studio环境中使用Boost线程。
这篇文章简单地介绍了在不同的形式中使用Boost线程，帮助你入门，减少编译时的各种麻烦，这往往是初学者最难的地方。
简单的boost::thread例子
我们通过Gavin Baker的 "anonymn" 来展现boost线程编程：
```
```cpp
#include
 <iostream>
```
```cpp
#include
 <boost/thread.hpp>
```
```cpp
#include
 <boost/date_time.hpp>
```
```cpp
```
```cpp
void
```
```cpp
workerFunc()
```
```cpp
{
```
```cpp
```
```cpp
boost::posix_time::seconds
 workTime(3);
```
```cpp
```
```cpp
std::cout
 <<
```
```cpp
"Worker:
 running"
```
```cpp
<< std::endl;
```
```cpp
```
```cpp
```
```cpp
//
 Pretend to do something useful...
```
```cpp
```
```cpp
boost::this_thread::sleep(workTime);
```
```cpp
```
```cpp
std::cout
 <<
```
```cpp
"Worker:
 finished"
```
```cpp
<< std::endl;
```
```cpp
}
```
```cpp
```
```cpp
int
```
```cpp
main(
```
```cpp
int
```
```cpp
argc,
```
```cpp
char
```
```cpp
*
 argv[])
```
```cpp
{
```
```cpp
```
```cpp
std::cout
 <<
```
```cpp
"main:
 startup"
```
```cpp
<< std::endl;
```
```cpp
```
```cpp
boost::
```
```cpp
thread
```
```cpp
workerThread(workerFunc);
```
```cpp
```
```cpp
```
```cpp
std::cout
 <<
```
```cpp
"main:
 waiting for thread"
```
```cpp
<< std::endl;
```
```cpp
```
```cpp
workerThread.join();
```
```cpp
```
```cpp
```
```cpp
std::cout
 <<
```
```cpp
"main:
 done"
```
```cpp
<< std::endl;
```
```cpp
```
```cpp
return
```
```cpp
0;
```
```cpp
}
```
```
例子代码创建了一个boost线程对象，传递给他工作函数的参数，在完成时退出该线程。
这个简单的例子，我用来验证Boost线程库设置是否成功。以下步骤是运行这个简单例子的必备步骤。
1. 下载并安装Boost
如果你还没有安装好Boost,可以从这里下载并安装：http://www.boost.org/users/download/ 或者 http://www.boostpro.com/download ，安装好后在你的Visual Studio项目中，选择配置属性->附加：包含目录。然后输入Boost的安装目录。
![](http://blog.okbase.net/picture/2012/05/14/A113255283-1.png)
完成以上步骤后，首次编译上面的代码可能会遇到如下错误：
LINK : fatal error LNK1104: cannot open file 'libboost_thread-vc100-mt-gd-1_46_1.lib'
意思是找不到库文件，在Boost中，必须建立自己的库。
2. 获得bjam
下一步是建立bjam.exe, 我发现最简单的方法是使用BoostPro提供的安装程序。在我的例子中，使用的是boostpro 1.46.1的安装程序。下载并运行此程序。对于我来说，我最喜欢的还是VS 2003.NET 和VS 2010两个版本。
![](http://blog.okbase.net/picture/2012/05/14/A113327580-2.png)
在安装过程中，选择需要安装什么库，没必要全选，选择你需要的即可。
![](http://blog.okbase.net/picture/2012/05/14/A113412533-3.png)
3. 创建 bjam.exe可执行文件
bjam安装后，打开命令行然后到src目录下，例如：
C:\Program Files\boost_1_46_1\tools\build\v2\engine\src
运行build.bat。注意脚本将在bin.ntx86子目录下创建bjam.exe可执行文件。
C:\Program Files\boost_1_46_1\tools\build\v2\engine\src\bin.ntx86
4. 更新PATH环境变量
现在添加bjam.exe到PATH环境变量中。在Windows 7,可以右键点击“计算机”（或我的电脑），选择属性，然后在高级系统设置中修改。
![](http://blog.okbase.net/picture/2012/05/14/A113443674-4.png)
点击环境变量PATH，选择编辑按钮。
![](http://blog.okbase.net/picture/2012/05/14/A113510815-5.png)
将目录添加进去，例如：C:\Program Files\boost_1_46_1\tools\build\v2\engine\src\bin.ntx86
![](http://blog.okbase.net/picture/2012/05/14/A113552471-6.png)
5. 运行bjam可执行文件
在命令行，到C:\Program Files\boost_1_46_1目录下，输入bjam,程序创建过程需要等待5-15分钟。
![](http://blog.okbase.net/picture/2012/05/14/A113626487-7.png)
6. 设置Visual Studio项目属性
在你的Visual Studio 项目中，选择配置属性->连接器->输入->附加依赖项，然后输入 libboost_thread-vc100-mt-gd-1_46_1.lib
![](http://blog.okbase.net/picture/2012/05/14/A113720080-8.png)
在你的Visual Studio项目中，选择项目配置属性->连接器->常规->附加库目录，
告诉它stage/lib目录。
![](http://blog.okbase.net/picture/2012/05/14/A113751065-9.png)
然后呢，我一开始所展示的Boost线程代码就可以编译运行了，输出如下：
|12345|```cppmain: startup``````cppmain: waiting``````cppfor``````cppthread``````cppWorker: running``````cppWorker: finished``````cppmain: done``````cpp``````cpp``````cpp```////////////////////////```cpp``````cpp```## [Windows环境下使用Boost](http://www.cnblogs.com/chuncn/archive/2012/09/10/2679026.html)一、 下载boost1、boostpro[http://www.boostpro.com/download/](http://www.boostpro.com/download/)2、boost.org(本文下载方式)[http://www.boost.org/users/download/](http://www.boost.org/users/download/)[http://sourceforge.net/projects/boost/files/boost/1.51.0/](http://sourceforge.net/projects/boost/files/boost/1.51.0/)boost_1_51_0.zip 下载并解压到C盘根文件夹二、编译boost1、生成生命行程序　　执行bootstrap.bat2、编译　　执行b2.exe,完成后显示：The Boost C++ Libraries were successfully built!The following directory should be added to compiler include paths:C:/boost_1_51_0The following directory should be added to linker library paths:C:\boost_1_51_0\stage\lib三、使用boost1、创建一个win32 console2、引用bootst　　C/C++ -> Additional Include Directories: C:\boost_1_51_0　　Linker-> Additional Library Directories: C:\boost_1_51_0\stage\lib　　Linker->Input->Additional Dependencies :libboost_signals-vc110-mt-gd-1_51.lib;libboost_regex-vc110-mt-gd-1_51.lib;3、Code如下：[?](http://www.cnblogs.com/chuncn/archive/2012/09/10/2679026.html#)```cpp#include "stdafx.h"``````cpp#include <boost/regex.hpp>``````cpp#include <boost/signals.hpp>``````cpp#include <boost/lambda/lambda.hpp>``````cpp``````cpp#include <iostream>``````cpp#include <cassert>``````cpp``````cppstruct``````cppprint_sum {``````cpp``````cppvoid``````cppoperator()(``````cppint``````cppx,``````cppint``````cppy)``````cppconst``````cpp{ std::cout << x+y << std::endl; }``````cpp};``````cpp``````cppstruct``````cppprint_product {``````cpp``````cpp``````cppvoid``````cppoperator()(``````cppint``````cppx,``````cppint``````cppy)``````cppconst``````cpp{ std::cout << x*y << std::endl; }``````cpp``````cpp};``````cpp``````cppint``````cpp_tmain(``````cppint``````cppargc, _TCHAR* argv[])``````cpp{``````cpp``````cppboost::signal2<``````cppvoid``````cpp,``````cppint``````cpp,``````cppint``````cpp, boost::last_value<``````cppvoid``````cpp>, std::string> sig;``````cpp``````cppsig.connect(print_sum());``````cpp``````cppsig.connect(print_product());``````cpp``````cppsig(3, 5);``````cpp``````cppstd::string line;``````cpp``````cppboost::regex pat(``````cpp"^Subject: (Re: |Aw: )*(.*)"``````cpp);``````cpp``````cppwhile``````cpp(std::cin)``````cpp``````cpp{``````cpp``````cppstd::getline(std::cin, line);``````cpp``````cppboost::smatch matches;``````cpp``````cppif``````cpp(boost::regex_match(line, matches, pat))``````cpp``````cppstd::cout << matches[2] << std::endl;``````cpp``````cpp}``````cpp``````cppreturn``````cpp0;``````cpp}```示例程序在vs2012下通过，输出：815```cpp```|```cpp#include "stdafx.h"``````cpp#include <boost/regex.hpp>``````cpp#include <boost/signals.hpp>``````cpp#include <boost/lambda/lambda.hpp>``````cpp``````cpp#include <iostream>``````cpp#include <cassert>``````cpp``````cppstruct``````cppprint_sum {``````cpp``````cppvoid``````cppoperator()(``````cppint``````cppx,``````cppint``````cppy)``````cppconst``````cpp{ std::cout << x+y << std::endl; }``````cpp};``````cpp``````cppstruct``````cppprint_product {``````cpp``````cpp``````cppvoid``````cppoperator()(``````cppint``````cppx,``````cppint``````cppy)``````cppconst``````cpp{ std::cout << x*y << std::endl; }``````cpp``````cpp};``````cpp``````cppint``````cpp_tmain(``````cppint``````cppargc, _TCHAR* argv[])``````cpp{``````cpp``````cppboost::signal2<``````cppvoid``````cpp,``````cppint``````cpp,``````cppint``````cpp, boost::last_value<``````cppvoid``````cpp>, std::string> sig;``````cpp``````cppsig.connect(print_sum());``````cpp``````cppsig.connect(print_product());``````cpp``````cppsig(3, 5);``````cpp``````cppstd::string line;``````cpp``````cppboost::regex pat(``````cpp"^Subject: (Re: |Aw: )*(.*)"``````cpp);``````cpp``````cppwhile``````cpp(std::cin)``````cpp``````cpp{``````cpp``````cppstd::getline(std::cin, line);``````cpp``````cppboost::smatch matches;``````cpp``````cppif``````cpp(boost::regex_match(line, matches, pat))``````cpp``````cppstd::cout << matches[2] << std::endl;``````cpp``````cpp}``````cpp``````cppreturn``````cpp0;``````cpp}```|
|----|----|----|
|```cpp#include "stdafx.h"``````cpp#include <boost/regex.hpp>``````cpp#include <boost/signals.hpp>``````cpp#include <boost/lambda/lambda.hpp>``````cpp``````cpp#include <iostream>``````cpp#include <cassert>``````cpp``````cppstruct``````cppprint_sum {``````cpp``````cppvoid``````cppoperator()(``````cppint``````cppx,``````cppint``````cppy)``````cppconst``````cpp{ std::cout << x+y << std::endl; }``````cpp};``````cpp``````cppstruct``````cppprint_product {``````cpp``````cpp``````cppvoid``````cppoperator()(``````cppint``````cppx,``````cppint``````cppy)``````cppconst``````cpp{ std::cout << x*y << std::endl; }``````cpp``````cpp};``````cpp``````cppint``````cpp_tmain(``````cppint``````cppargc, _TCHAR* argv[])``````cpp{``````cpp``````cppboost::signal2<``````cppvoid``````cpp,``````cppint``````cpp,``````cppint``````cpp, boost::last_value<``````cppvoid``````cpp>, std::string> sig;``````cpp``````cppsig.connect(print_sum());``````cpp``````cppsig.connect(print_product());``````cpp``````cppsig(3, 5);``````cpp``````cppstd::string line;``````cpp``````cppboost::regex pat(``````cpp"^Subject: (Re: |Aw: )*(.*)"``````cpp);``````cpp``````cppwhile``````cpp(std::cin)``````cpp``````cpp{``````cpp``````cppstd::getline(std::cin, line);``````cpp``````cppboost::smatch matches;``````cpp``````cppif``````cpp(boost::regex_match(line, matches, pat))``````cpp``````cppstd::cout << matches[2] << std::endl;``````cpp``````cpp}``````cpp``````cppreturn``````cpp0;``````cpp}```| | |
示例程序在vs2012下通过，输出：
8
15
```cpp
```

