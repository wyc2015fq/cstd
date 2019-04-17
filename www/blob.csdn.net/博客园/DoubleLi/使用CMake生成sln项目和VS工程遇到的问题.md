# 使用CMake生成sln项目和VS工程遇到的问题 - DoubleLi - 博客园






# 用vs运行cmake后的工程

参考：http://zhidao.baidu.com/link?url=AZRxI0jGDzo6Pikk68qylee0g7leXbpbZGiVuyiijWbd8scUKWvh9y0E-ZHeToZ5mWXYQD3uLMXiA6Lwf9_r8HEFpoLWvUhCuMcnH4rnOS7


1、单个文件示例：
1) 首先建立文件夹CMakeTest/Src
2) 在文件夹Src中建立两个文件main.c和CMakeLists.txt
3) main.c:
#include <stdio.h>

int main()
{
     printf("hello world.");
     getchar();
     return 0;
}
4) CMakeLists.txt
PROJECT (HELLO)

SET (SRC_LIST main.c)

ADD_EXECUTABLE (hello ${SRC_LIST})
5) 打开：开始--Microsoft Visual Studio 2012--Visual Studio Tools--VS2012 x86 Native Tools Command Prompt

 6) 进入到CMakeTest/build/Src目录，执行以下代码
mkdir build & cdbuild
cmake .. -G"NMake Makefiles"
nmake




# [学习使用CMake构建工程](http://blog.csdn.net/alex_my/article/details/18982363)

转自：http://blog.csdn.net/alex_my/article/details/18982363  （学习使用CMake的详细过程参考链接）



使用CMake生成sln项目



1) 以上是一些基本应用，现在我们将要生成sln项目供VS直接使用，当然，也可以生成供XCODE使用或者linux等等。



2) 本机配置：WIN7 X64，VS2012 X64。本次使用CMakeTest/Src5做为示例。



3) 运行CMake, 将Src5包含在源文件中，在Src5下新建build目录(也可以先不建，会自动生成)，将其包含到生成文件中。



如图：



# 配图 cmake_step71.png



![](http://img.blog.csdn.net/20140208114605468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWxleF9teQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



4) 运行上图中红框指示的Configure按钮，在弹出框中选择Visual Studio 11 WIN64（根据你的需要进行选择，注意的是VS2012 对应



的版本号是110，因此选11， VS2010 对应的版本号是100，因此选10）。点击下图中红框指示的Finish按钮。



# 配图 cmake_step72.png



![](http://img.blog.csdn.net/20140208120103593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWxleF9teQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



5) 运行结束后如下图所示，如果有错误，会在红框指示区提示。请再次点击 Configure按钮。



# 配图 cmake_step73.png



![](http://img.blog.csdn.net/20140208120107156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWxleF9teQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



6) 运行结束后如下图所示, 然后请点击Generate按钮。



# 配图 cmake_step74.png



![](http://img.blog.csdn.net/20140208120111390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWxleF9teQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



7) 打开CMakeTest/Src5/build，会发现生成的HELLO.sln。



# 配图 cmake_step75.png



![](http://img.blog.csdn.net/20140208120117875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWxleF9teQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



8) 用VS2012将项目打开，选择ALL_BUILD为启动项，编译。



# 配图 cmake_step76.png



![](http://img.blog.csdn.net/20140208120126312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWxleF9teQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





ps:


'nmake' 不是内部或外部命令，也不是可运行的程序或批处理文件。 
 参考：http://blog.163.com/lvan100@yeah/blog/static/68117214201332761143157
如果你也遇到了这个问题，那么我敢肯定90%的情况下你使用的cmd是普通的cmd！
'nmake' 不是内部或外部命令，也不是可运行的程序或批处理文件。
解决的问题很简单，使用VS2010自带的cmd。





 若是显示“ 'cmake' 不是内部或外部命令，也不是可运行的程序或批处理文件。”
 参考：http://blog.csdn.net/solox1983/article/details/6567342 
环境变量的配置。
     要在"我的电脑"－>右键属性—>高级－>环境变量－>PATH，增加两个工具的bin文件夹的目录。这样才能在CMD 或者直接编译和生成makefile。
http://www.cmake.org/download/
Windows Source (has \r\n line feeds)    cmake-3.2.0-rc2.zip
Windows ZIP    cmake-3.2.0-rc2-win32-x86.zip







|vs2013（error C2039: “min”: 不是“std”的成员）|
|----|

[http://blog.lishewen.com/post/2013/11/24/cocos2d-x-project-in-vs2013-compiler-error-solution](http://blog.lishewen.com/post/2013/11/24/cocos2d-x-project-in-vs2013-compiler-error-solution)

在VS2013编译cocos2d-x的win32项目时，会报错误：

> 
error C2039: “min”: 不是“std”的成员

error C2873: “min”: 符号不能用在 using 声明中


这时需要打开对于报错的.cpp文件，在头部加入
|1|`#include <algorithm>`|
|----|----|



无法启动程序 系统找不到指定的文件

![](http://img.bbs.csdn.net/upload/201409/04/1409814979_322729.png)

vs默认的项目是ALL_BUILD这个项目，但是我的默认启动项目应该是HelloCmake project ，所以通过移除all_build和zero项目是可以的，另外通过右键将helloCmake设为默认启动项目也是可以的









编译lua源码时，使用vs2012，遇到如下错误。

1>------ 已启动生成: 项目: 20130925, 配置: Debug Win32 ------
1>  stdafx.cpp
1>d:\code\20130925\20130925\stdafx.cpp(18): error C4996: 'fopen': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
1>          d:\vs2012\vc\include\stdio.h(218) : 参见“fopen”的声明
1>  20130925.cpp
1>  正在生成代码...
========== 生成: 成功 0 个，失败 1 个，最新 0 个，跳过 0 个 ========== 

解决方案，项目 ->属性 -> c/c++ -> 预处理器 -> 点击预处理器定义，编辑，加入_CRT_SECURE_NO_WARNINGS，即可。









