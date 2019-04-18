# VS2012 编译 boost1.53/ boost1.49 - wishchinYang的专栏 - CSDN博客
2013年09月22日 13:47:56[wishchin](https://me.csdn.net/wishchin)阅读数：1260
个人分类：[BOOST/FlANN/Eigen/C+0X](https://blog.csdn.net/wishchin/article/category/1644381)
原文链接：[http://blog.csdn.net/ly131420/article/details/8904122](http://blog.csdn.net/ly131420/article/details/8904122)
[](http://blog.csdn.net/ly131420/article/details/8904122)
一、下载Boost库
boost_1_53_0.zip   (http://www.boost.org/users/download/)，解压到F:boost_1_50_0
要做Python开发的还需下载： python-2.7.3.msi   (http://www.python.org/)
二、编译Boost库
1. 编译jam   
    在工具栏启动Visual Studio 2012 Command Prompt(VS2012命令提示) ， 进入 boost的解压目录，即D:\boost_1_53_0， 输入bootstrap，便在boost根目录下生成bjam.exe文件。具体命令如下：
>D:\
>cd F:\boost_1_53_0
>bootstrap
   【 注：在网上看了许多教程，老版本需要进入tools/jam/目录下运行build_dist.bat 生成bjam.exe，这点与老版本不同。】
一般不需要全部编译，部分编译就够用了。
全部编译的命令行：d:\boost>bjam --toolset=msvc-11.0 --build-type=complete stage
部分编译的命令行：d:\boost>bjam --toolset=msvc-11.0 --with-date_time --with-thread
查看需要编译库的命令行：bjam --show-libraries
编译成64位的命令：bjam --toolset=msvc-11.0 address-model=64 --build-type=complete stage 
三、使用编译的静态库
配置属性-》链接器-》常规-》附加库依赖项-》D:\boost\stage\lib
配置属性-》链接器-》输入-》附加依赖项-》libboost_date_time-vc110-mt-gd-1_53.lib
## VS2010环境下Boost 1.49.0配置笔记
[http://www.yiran.org/boost-149-under-vs2010/](http://www.yiran.org/boost-149-under-vs2010/)
Boost 实际上是由几十个不同功能的函数库组成的 [C++](http://www.yiran.org/tag/cpp/) 函数库集合（Set of libraries），它最初是由 [C++](http://www.yiran.org/tag/cpp/) Standard Committee 的部分委员发起并开发，含有众多能够极大拓展 [C++](http://www.yiran.org/tag/cpp/) 语言功能和易用性的函数。它的风格与标准模板库相似，跨平台并且通用性很强，并且其很多组成库已经被收录在 [C++11](http://en.wikipedia.org/wiki/C%2B%2B11) 新标准中，可以被看作C++标准库的官方扩展版。以下是个人总结的 Boost 在 Visual Studio 2010 下安装配置笔记，仅供参考。
首先下载Boost函数库并解压，截止2012/6/8最新版本为1.49.0，[点此下载](http://ncu.dl.sourceforge.net/project/boost/boost/1.49.0/boost_1_49_0.7z)，也可访问www.boost.org获取最新版boost。
一部分 Boost 函数库（即只包含hpp头文件）可以直接调用，无需编译；但如Filesystem文件系统等另一些重要的函数库需要编译才能正常调用，以下是编译与配置步骤：
- 打开程序菜单，选择Visual Studio Tools里面的 Visual Studio 命令提示，如图：
![程序菜单->VS2010->VS Tools->VS 命令提示(2010)](http://www.yiran.org/wp-content/uploads/2012/06/vs2010_command_prompt.png)
- 转到解压后的Boost所在目录，输入***Bootstrap***，执行完毕会生成b2.exe。
输入（目录下的bjam.exe和b2.exe是完全相同的，可以互换）
`b2 --toolset=msvc-10.0 --build-type=complete stage`
（若要构建Boost 64位版本函数库，则输入`b2 --toolset=msvc-10.0 --build-type=complete architecture=x86 address-model=64 stage`）
编译时间较长，耐心等待。
- 打开VS2010，新建空C++项目，点击项目菜单中的项目属性，找到配置属性->VC++目录，在“包含目录”中添加 [Boost所在目录]，在“库目录”中添加 [Boost目录]\stage\lib，保存设置，至此Boost配置完成。
附：VS2010 静态链接 Boost 函数库的配置方法：
打开项目属性->配置属性->C/C++->代码生成，选择“运行库”为“多线程 (/MT)”（如选择“多线程 DLL (/MD)”，则为动态链接 Boost 库）。
是为记。
