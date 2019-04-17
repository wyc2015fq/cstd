# 内存泄漏检测工具VLD在VS2010中的使用举例 - DoubleLi - 博客园






Visual LeakDetector(VLD)是一款用于Visual C++的免费的内存泄露检测工具。它的特点有：(1)、它是免费开源的，采用LGPL协议；(2)、它可以得到内存泄露点的调用堆栈，可以获取到所在文件及行号；(3)、它可以得到泄露内存的完整数据；(4)、它可以设置内存泄露报告的级别。

         默认情况下，只有在Debug模式下才会启用VLD的功能。

         在使用的时候只要将VLD的头文件和lib文件放在工程文件中即可。

         在需要检测内存泄露的源文件中添加#include “vld.h”

         VLD只能在Windows下使用，它应用在C/C++语言中。

         通过修改vld.ini文件(ReportFile选项)，可以设置内存泄露报告的保存位置。

         VLD好像对中文路径不支持。

         VLD的各个版本对应适用于不同的VC/VS编译器，注意选择合适的版本。



1.        从[http://vld.codeplex.com/SourceControl/latest#README.md](http://vld.codeplex.com/SourceControl/latest#README.md)下载最新源代码，解压缩，里面包含有源码、vs2010工程配置、以及一些依赖库，如dbghelp等；

2.          打开vld_vs10.sln，在32位/64位下，分别编译debug和release下vld工程，会在当前目录下生成bin文件夹，里面包含相应的库文件；

3.          新建控制台工程VLDtest，在Debug下运行，代码如下：

stdafx.cpp:


- 



#include "stdafx.h"


- 




- 



#ifdef _DEBUG


- 



#pragma comment(lib, "../../../src/VLD/bin/Win32/Debug/vld.lib")


- 



#else


- 



//#pragma comment(lib, "../../../src/VLD/bin/Win32/Release/vld.lib")


- 



#endif



VLDtest.cpp:




- 



#include "stdafx.h"


- 



#include "../../src/VLD/vld.h"


- 



#include <iostream>


- 




- 



using namespace std;


- 




- 



void fun()


- 



{


- 



char* tmp = new char[10];


- 



memset(tmp, 0, 10);


- 



}


- 




- 



int main(int argc, char* argv[])


- 



{


- 



fun();


- 




- 



cout<<"ok!"<<endl;


- 




- 



return 0;


- 



}





4.          运行提示，“无法启动此程序，因为计算机中丢失vld_x86.dll”，解决方法，将bin/Win32/Debug/vld_x86.dll拷贝到VLDtest执行目录下或将vld_x86.dll路径添加到系统环境变量中；

5.          运行提示，“应用程序无法正常启动(0xc0150002)”，将源代码中的/setup/dbghelp/x86文件夹下的dbghelp.dll和Microsoft.DTfW.DHL.manifest两个文件拷贝到VLDtest执行目录下即可；

6.          直接按F5运行，可以在Output窗口显示内存泄漏的相关信息，包括发生内存泄漏的文件名和行号，双击即可定位到内存泄漏的具体位置；

7.          或者直接运行，相关信息可以显示在命令行窗口中；

8.        也可将内存泄漏信息写入文件中，将源代码中的vld.ini文件拷贝到VLDtest工程目录下，即与VLDtest.vcxproj同目录，修改vld.ini文件中的选项参数，ReportFile = .\../../../lib/dbg/x86_vc10/memory_leak_report.txt，ReportTo =both(原始ReportTo = debugger)，重新运行后，会在执行目录下生成一个memory_leak_report.txt文件；或者将vld.ini文件拷贝到执行目录下，运行VLDtest[dbg_x86_vc10].exe文件也可生成报告文件；

9.          在Release下运行，VLD不起作用，VLDtest工程不用作任何配置和调整；

10. 也可不用编译源代码，直接从[http://vld.codeplex.com/releases](http://vld.codeplex.com/releases)下载，然后安装即可，里面包含需要的库和头文件。

图中为运行测试代码时显示的内存泄漏信息：

![](https://img-blog.csdn.net/20150311110741096)









