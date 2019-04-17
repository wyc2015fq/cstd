# 从零开始编译Poco C++和VS2015环境配置 - DoubleLi - 博客园






Poco C++是一个C++的开源框架，详细介绍可以问百度介绍更详细，这里我就记录下我从零编译poco-1.9.0和配置 VS2015的环境，以备不时之需。

步骤：
一、安装cmake； 
二、编译poco-1.9.0； 
三、将编译后的poco-1.9.0配置到VS2015的项目中； 
四、示例简单示例。

安装cmake
我的电脑环境： 
win7 64位； 
cmake-3.12.1-win64-x64（官网下载：点击我下载cmake）。 
安装cmake按常规安装即可，但是记得勾选： 

这个主要是为了防止电脑登录其他用户没有cmake，建议把CMake加到系统PATH中。其他的下一步即可。 
关于cmka GUI和命令行的用法（下面会配置PocoC++使用命令行用法），也可以自行百度。

编译Poco C++
前提条件： 
1）Poco C++源码（官网下载：点击我下载）； 
2）已经安装VS2015； 
3）已经安装cmake。

编译poco c++(poco-1.9.0.zip、Visual Studio 2015) 
1）用windows的命令行转到poco-1.9.0文件夹，即源码目录。 
2）执行下面命令（命令最后一句的”..”切记不可遗漏了哦）：

mkdir cmake_build_x64 
cd cmake_build_x64 
cmake -G “Visual Studio 14 2015 Win64” ..

3）然后进入make_build_x64 文件夹里面，用 Visual Studio 2015 打开 Poco.sln，用VS编译 POCO C++ Libraries 的动态链接库 ，建议同时编译debug和release版本（跟普通的编译VS工程一样，即按快捷键F7即可），至此PocoC++编译完成了，在目录cmake_build_x64文件夹下面的的 bin、lib 子目录中，就是我们最终得到的动态链接库等文件。

poco-1.9.0配置到VS2015的项目中
1)在源码目录下新建v1.9.0_compact_MSVC文件夹，如我的：

D:\poco\poco-1.9.0\v1.9.0_compact_MSVC

然后设置系统环境变量指向POCO_ROOT，即点击“”我的电脑“–》右击“属性”–》高级系统设置–》环境变量–》点击系统环境变量下面的“新建”按钮，填写： 

2）将所有的 include下面文件夹集中在 %POCO_ROOT%\include 目录中，如我的D:\poco\poco-1.9.0\Foundation\include\Poco文件下面的东西拷贝D:\poco\poco-1.9.0\v1.9.0_compact_MSVC\include\Poco下面。同理D:\poco\poco-1.9.0\Encodings\include\Poco同样处理（你会留意到有Encodings Foundation JSON Net Util XML这些文件夹下面\include\Poco下面的文件及文件夹需要这样处理）。这样做主要是为包含头文件方便些，最终处理完如下图所示： 

3）在v1.9.0_compact_MSVC文件夹下面建立build/x64文件夹，将编译得到的 bin、lib 文件夹都收集到build/x64下，如下图： 

4）在VS工程连接动态库和头文件： 
a.右击你的VS工程，点击“属性”–》C/C++–》常规–》添加包含目录，输入：

$(POCO_ROOT)\include

b.点击“属性”–》连接器–》附加库目录，输入：

D:\poco\poco-1.9.0\v1.9.0_compact_MSVC\build\x64\lib\Debug 
D:\poco\poco-1.9.0\v1.9.0_compact_MSVC\build\x64\bin\Debug

c.点击“属性”–》连接器–》输入–》附加依赖项。 
针对Debug，输入：

PocoEncodingsd.lib 
PocoFoundationd.lib 
PocoJSONd.lib 
PocoNetd.lib 
PocoUtild.lib

针对Release输入：

PocoEncodings.lib 
PocoFoundation.lib 
PocoJSON.lib 
PocoNet.lib 
PocoUtil.lib 
PocoXML.lib

4）将对应库配置到编译输出目录。“属性”–》生成事件–》命名行，输入：

copy “(POCOROOT)\build$(Platform)\bin$(Configuration)\*.∗"(POCOROOT)\build$(Platform)\bin$(Configuration)\*.∗"(TargetDir)

pooc c++使用简单示例代码：
#include <iostream>
#include "Poco\Timer.h"
#include "Poco\Thread.h"
using Poco::Timer;
using Poco::Thread;
using Poco::TimerCallback;
using namespace std;

class TimerExample
{
public:
    void onTimer(Poco::Timer &timer)
    {
        cout << "func : " << __FUNCTION__ << endl;
    }
};

int main(int argc, char** argv)
{
    TimerExample te;
    Timer timer(2000, 1000);
    timer.start(TimerCallback<TimerExample>(te, &TimerExample::onTimer));
    Thread::sleep(5000);
    timer.stop();
    return 0;
}




已上是我今天编译poco c++、配置VS2015工程环境、简单使用的总结笔记，如有不当之处，还请大侠多多指教哦~

from：https://blog.csdn.net/u013421892/article/details/82467086 











