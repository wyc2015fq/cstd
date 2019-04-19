# Matlab+VC混合编程--mclmcr.h等 - 三少GG - CSDN博客
2011年03月13日 15:05:00[三少GG](https://me.csdn.net/scut1135)阅读数：4014
用Matlab建立和VC的应用程序接口；
再将Matlab下写的程序输出成dll或者exe都行
最后在vc下调用就可以了。 
============================================================================
============================================================================
[http://hi.baidu.com/minedreamflowers/blog/item/70593b24b5da913bc9955914.html](http://hi.baidu.com/minedreamflowers/blog/item/70593b24b5da913bc9955914.html)
vs调用通过dll调用matlab程序
2009-03-30 10:34
|程序中使用MATLAB编译产生的DLL （VS2005, MATLAB7.5, mwArray）实验内容：把一个简单的m代码编译成C++接口的DLL，然后在C++程序中调用。为了简单起见，这里的C++程序是一个Win32 Console程序，而不是Windows图形界面的程序，不过不妨碍我们的讨论。下面是这个例子用到的m代码。它定义了一个名为myadd2的函数：CODE:function [y,z] = myadd2(a, b)% dummy function, just to demonstrate the ideay = a+b;z = a+2*b;end首先把这个函数编译成C++接口的DLL。为此，我们需要先设置MATLAB编译器。具体做法是在MATLAB命令行执行“mbuild -setup”。然后用下面这行命令把myadd2函数编译成一个C++接口的DLL：CODE:mcc -W cpplib:libmyadd2 -T link:lib myadd2结果，我们将会得到包含libmyadd2.dll，libmyadd2.ctf，libmyadd2.h，libmyadd2.lib等在内的一些文件。接下来我们只需要这四个文件。请注意：在MATLAB R2007b之前，这些文件都会在和编译的m代码所在的目录中，而在MATLAB R2007b，缺省情况下这些文件会在Documents and Settings中用户的My Documents目录中。然后在VS2005中创建一个Win32 Console的VC++工程，我在测试时取名为testmyadd2_r2007b。把以上四个(全部文件)文件拷贝到VC++工程的源代码所在目录。接下来设置VC++，让它能找到MATLAB接口函数的定义及连接库函数。可以有两种设置方式：一种是改VS2005中关于VC++的设置，这样的好处是每个新的工程都能自动获得这个设定；而另一种是只改当前工程的设置，也就是设置只对该工程有效。这里用后一种方式。在VS2005中打开工程testmyadd2_r2007b，选择菜单“Project-->Properties，在出来的对话框上，把MATLAB提供的include路径加到VC++的头文件搜索路径。如图所示：D:/MATLAB7/extern/include然后把相应的lib所在目录加到linker的额外搜索路径上。如图所示：D:/MATLAB7/extern/lib/win32/microsoft/msvc71接下来，告诉VC++，我们的这个程序需要连接到另外两个额外的库函数：libmyadd2.lib和mclmcrrt.lib。(**Grant:还有mclmcr.lib**)中间用空格隔开。如图所示：[](http://www.simwe.com/forum/attachments/day_071006/20071006_2dbbe4d0a357d0e14865VKYXRTZn4go8.gif)最后则是程序代码。这个程序只有一个main函数CODE:#include "stdafx.h" #include #include "mclmcr.h" #include "mclcppclass.h" #include "libmyadd2.h" int _tmain(int argc, _TCHAR* argv[]) {        std::cout << "Hello world!" << std::endl;        /* Initialize the MCR */        if( !mclInitializeApplication(NULL,0) )        {             std::cout << "Could not initialize the application!" << std::endl;             return -1;        }        // initialize lib        if( !libmyadd2Initialize())        {             std::cout << "Could not initialize libmyadd2!" << std::endl;             return -1;        }        try        {        // declare and initialize a        mwArray a(2, 2,   mxDOUBLE_CLASS);        double *aData;        aData = new double[4];        int i;        for( i=0; i<4; ++i)        {             aData[i] = 1.0*i;        }        // print output        std::cout << "a = " << std::endl;        std::cout << aData[0] << ", " << aData[1] << std::endl;        std::cout << aData[2] << ", " << aData[3] << std::endl;        a.SetData(aData, 4);        // declare and initialize b        mwArray b(2, 2,   mxDOUBLE_CLASS);        b(1,1) = 11.;        b(1,2) = 12.;        b(2,1) = 21.;        b(2,2) = 22.;        mwArray y(2, 2,   mxDOUBLE_CLASS);        mwArray z(2, 2,   mxDOUBLE_CLASS);        // call the function             myadd2(2, y, z, a, b);        // copy data from mwArray to C++ objects        // allocate outputs        double *yData, *zData;        yData = new double[4];        if( yData == NULL )        {             std::cout << "Failed to allocate memory for yData!" << std::endl;             return -1;        }        zData = new double[4];        if( zData == NULL )        {             std::cout << "Failed to allocate memory for zData!" << std::endl;             return -1;        }        // copy data from mwArray to C++        y.GetData(yData, 4);        z.GetData(zData, 4);        // print output        std::cout << "y = " << std::endl;        std::cout << yData[0] << ", " << yData[1] << std::endl;        std::cout << yData[2] << ", " << yData[3] << std::endl;        std::cout << "z = " << std::endl;        std::cout << zData[0] << ", " << zData[1] << std::endl;        std::cout << zData[2] << ", " << zData[3] << std::endl;               // deallocate memory        delete [] aData;        delete [] zData;        delete [] yData;        }        catch( const mwException& e)        {       std::cerr << e.what() << std::endl;                       }        // terminate the lib        libmyadd2Terminate();        // terminate MCR        mclTerminateApplication();        return 0; }实验的结果表明，在C++程序中使用MATLAB编译产生的C++接口DLL非常容易。error:mcc编译m文件出现这样的错误提示（例）: Error: File "psf3" is a script M-file and cannot be compiled with the current Compiler. matlab complier不能编译script M-file，请将script M-file转成function m-file 最简单的一个转法是:如果你的m文件叫a.m,在开头加上一句: function a|程序中使用MATLAB编译产生的DLL （VS2005, MATLAB7.5, mwArray）实验内容：把一个简单的m代码编译成C++接口的DLL，然后在C++程序中调用。为了简单起见，这里的C++程序是一个Win32 Console程序，而不是Windows图形界面的程序，不过不妨碍我们的讨论。下面是这个例子用到的m代码。它定义了一个名为myadd2的函数：CODE:function [y,z] = myadd2(a, b)% dummy function, just to demonstrate the ideay = a+b;z = a+2*b;end首先把这个函数编译成C++接口的DLL。为此，我们需要先设置MATLAB编译器。具体做法是在MATLAB命令行执行“mbuild -setup”。然后用下面这行命令把myadd2函数编译成一个C++接口的DLL：CODE:mcc -W cpplib:libmyadd2 -T link:lib myadd2结果，我们将会得到包含libmyadd2.dll，libmyadd2.ctf，libmyadd2.h，libmyadd2.lib等在内的一些文件。接下来我们只需要这四个文件。请注意：在MATLAB R2007b之前，这些文件都会在和编译的m代码所在的目录中，而在MATLAB R2007b，缺省情况下这些文件会在Documents and Settings中用户的My Documents目录中。然后在VS2005中创建一个Win32 Console的VC++工程，我在测试时取名为testmyadd2_r2007b。把以上四个(全部文件)文件拷贝到VC++工程的源代码所在目录。接下来设置VC++，让它能找到MATLAB接口函数的定义及连接库函数。可以有两种设置方式：一种是改VS2005中关于VC++的设置，这样的好处是每个新的工程都能自动获得这个设定；而另一种是只改当前工程的设置，也就是设置只对该工程有效。这里用后一种方式。在VS2005中打开工程testmyadd2_r2007b，选择菜单“Project-->Properties，在出来的对话框上，把MATLAB提供的include路径加到VC++的头文件搜索路径。如图所示：D:/MATLAB7/extern/include然后把相应的lib所在目录加到linker的额外搜索路径上。如图所示：D:/MATLAB7/extern/lib/win32/microsoft/msvc71接下来，告诉VC++，我们的这个程序需要连接到另外两个额外的库函数：libmyadd2.lib和mclmcrrt.lib。(**Grant:还有mclmcr.lib**)中间用空格隔开。如图所示：[](http://www.simwe.com/forum/attachments/day_071006/20071006_2dbbe4d0a357d0e14865VKYXRTZn4go8.gif)最后则是程序代码。这个程序只有一个main函数CODE:#include "stdafx.h" #include #include "mclmcr.h" #include "mclcppclass.h" #include "libmyadd2.h" int _tmain(int argc, _TCHAR* argv[]) {        std::cout << "Hello world!" << std::endl;        /* Initialize the MCR */        if( !mclInitializeApplication(NULL,0) )        {             std::cout << "Could not initialize the application!" << std::endl;             return -1;        }        // initialize lib        if( !libmyadd2Initialize())        {             std::cout << "Could not initialize libmyadd2!" << std::endl;             return -1;        }        try        {        // declare and initialize a        mwArray a(2, 2,   mxDOUBLE_CLASS);        double *aData;        aData = new double[4];        int i;        for( i=0; i<4; ++i)        {             aData[i] = 1.0*i;        }        // print output        std::cout << "a = " << std::endl;        std::cout << aData[0] << ", " << aData[1] << std::endl;        std::cout << aData[2] << ", " << aData[3] << std::endl;        a.SetData(aData, 4);        // declare and initialize b        mwArray b(2, 2,   mxDOUBLE_CLASS);        b(1,1) = 11.;        b(1,2) = 12.;        b(2,1) = 21.;        b(2,2) = 22.;        mwArray y(2, 2,   mxDOUBLE_CLASS);        mwArray z(2, 2,   mxDOUBLE_CLASS);        // call the function             myadd2(2, y, z, a, b);        // copy data from mwArray to C++ objects        // allocate outputs        double *yData, *zData;        yData = new double[4];        if( yData == NULL )        {             std::cout << "Failed to allocate memory for yData!" << std::endl;             return -1;        }        zData = new double[4];        if( zData == NULL )        {             std::cout << "Failed to allocate memory for zData!" << std::endl;             return -1;        }        // copy data from mwArray to C++        y.GetData(yData, 4);        z.GetData(zData, 4);        // print output        std::cout << "y = " << std::endl;        std::cout << yData[0] << ", " << yData[1] << std::endl;        std::cout << yData[2] << ", " << yData[3] << std::endl;        std::cout << "z = " << std::endl;        std::cout << zData[0] << ", " << zData[1] << std::endl;        std::cout << zData[2] << ", " << zData[3] << std::endl;               // deallocate memory        delete [] aData;        delete [] zData;        delete [] yData;        }        catch( const mwException& e)        {       std::cerr << e.what() << std::endl;                       }        // terminate the lib        libmyadd2Terminate();        // terminate MCR        mclTerminateApplication();        return 0; }实验的结果表明，在C++程序中使用MATLAB编译产生的C++接口DLL非常容易。error:mcc编译m文件出现这样的错误提示（例）: Error: File "psf3" is a script M-file and cannot be compiled with the current Compiler. matlab complier不能编译script M-file，请将script M-file转成function m-file 最简单的一个转法是:如果你的m文件叫a.m,在开头加上一句: function a|
|----|----|
|程序中使用MATLAB编译产生的DLL （VS2005, MATLAB7.5, mwArray）实验内容：把一个简单的m代码编译成C++接口的DLL，然后在C++程序中调用。为了简单起见，这里的C++程序是一个Win32 Console程序，而不是Windows图形界面的程序，不过不妨碍我们的讨论。下面是这个例子用到的m代码。它定义了一个名为myadd2的函数：CODE:function [y,z] = myadd2(a, b)% dummy function, just to demonstrate the ideay = a+b;z = a+2*b;end首先把这个函数编译成C++接口的DLL。为此，我们需要先设置MATLAB编译器。具体做法是在MATLAB命令行执行“mbuild -setup”。然后用下面这行命令把myadd2函数编译成一个C++接口的DLL：CODE:mcc -W cpplib:libmyadd2 -T link:lib myadd2结果，我们将会得到包含libmyadd2.dll，libmyadd2.ctf，libmyadd2.h，libmyadd2.lib等在内的一些文件。接下来我们只需要这四个文件。请注意：在MATLAB R2007b之前，这些文件都会在和编译的m代码所在的目录中，而在MATLAB R2007b，缺省情况下这些文件会在Documents and Settings中用户的My Documents目录中。然后在VS2005中创建一个Win32 Console的VC++工程，我在测试时取名为testmyadd2_r2007b。把以上四个(全部文件)文件拷贝到VC++工程的源代码所在目录。接下来设置VC++，让它能找到MATLAB接口函数的定义及连接库函数。可以有两种设置方式：一种是改VS2005中关于VC++的设置，这样的好处是每个新的工程都能自动获得这个设定；而另一种是只改当前工程的设置，也就是设置只对该工程有效。这里用后一种方式。在VS2005中打开工程testmyadd2_r2007b，选择菜单“Project-->Properties，在出来的对话框上，把MATLAB提供的include路径加到VC++的头文件搜索路径。如图所示：D:/MATLAB7/extern/include然后把相应的lib所在目录加到linker的额外搜索路径上。如图所示：D:/MATLAB7/extern/lib/win32/microsoft/msvc71接下来，告诉VC++，我们的这个程序需要连接到另外两个额外的库函数：libmyadd2.lib和mclmcrrt.lib。(**Grant:还有mclmcr.lib**)中间用空格隔开。如图所示：[](http://www.simwe.com/forum/attachments/day_071006/20071006_2dbbe4d0a357d0e14865VKYXRTZn4go8.gif)最后则是程序代码。这个程序只有一个main函数CODE:#include "stdafx.h" #include #include "mclmcr.h" #include "mclcppclass.h" #include "libmyadd2.h" int _tmain(int argc, _TCHAR* argv[]) {        std::cout << "Hello world!" << std::endl;        /* Initialize the MCR */        if( !mclInitializeApplication(NULL,0) )        {             std::cout << "Could not initialize the application!" << std::endl;             return -1;        }        // initialize lib        if( !libmyadd2Initialize())        {             std::cout << "Could not initialize libmyadd2!" << std::endl;             return -1;        }        try        {        // declare and initialize a        mwArray a(2, 2,   mxDOUBLE_CLASS);        double *aData;        aData = new double[4];        int i;        for( i=0; i<4; ++i)        {             aData[i] = 1.0*i;        }        // print output        std::cout << "a = " << std::endl;        std::cout << aData[0] << ", " << aData[1] << std::endl;        std::cout << aData[2] << ", " << aData[3] << std::endl;        a.SetData(aData, 4);        // declare and initialize b        mwArray b(2, 2,   mxDOUBLE_CLASS);        b(1,1) = 11.;        b(1,2) = 12.;        b(2,1) = 21.;        b(2,2) = 22.;        mwArray y(2, 2,   mxDOUBLE_CLASS);        mwArray z(2, 2,   mxDOUBLE_CLASS);        // call the function             myadd2(2, y, z, a, b);        // copy data from mwArray to C++ objects        // allocate outputs        double *yData, *zData;        yData = new double[4];        if( yData == NULL )        {             std::cout << "Failed to allocate memory for yData!" << std::endl;             return -1;        }        zData = new double[4];        if( zData == NULL )        {             std::cout << "Failed to allocate memory for zData!" << std::endl;             return -1;        }        // copy data from mwArray to C++        y.GetData(yData, 4);        z.GetData(zData, 4);        // print output        std::cout << "y = " << std::endl;        std::cout << yData[0] << ", " << yData[1] << std::endl;        std::cout << yData[2] << ", " << yData[3] << std::endl;        std::cout << "z = " << std::endl;        std::cout << zData[0] << ", " << zData[1] << std::endl;        std::cout << zData[2] << ", " << zData[3] << std::endl;               // deallocate memory        delete [] aData;        delete [] zData;        delete [] yData;        }        catch( const mwException& e)        {       std::cerr << e.what() << std::endl;                       }        // terminate the lib        libmyadd2Terminate();        // terminate MCR        mclTerminateApplication();        return 0; }实验的结果表明，在C++程序中使用MATLAB编译产生的C++接口DLL非常容易。error:mcc编译m文件出现这样的错误提示（例）: Error: File "psf3" is a script M-file and cannot be compiled with the current Compiler. matlab complier不能编译script M-file，请将script M-file转成function m-file 最简单的一个转法是:如果你的m文件叫a.m,在开头加上一句: function a| |
============================================================================
[http://blog.csdn.net/infocarrier/archive/2010/09/01/5854522.aspx](http://blog.csdn.net/infocarrier/archive/2010/09/01/5854522.aspx)
VS2008 C++ 调用MATLAB 2009b 生成的DLL 收藏 
刚开始学习用VC++调用matlab生成的DLL，找了网上一些资料，难以找到vs2008与MATLAB2009b版本的，按照以往版本做的总是有很多错误。经过两天努力，终于调试成功，这里将经验总结一下，以供有需要的人们参考。
实验环境：
Win7
MATLAB 2009b（安装路径：E:/Program Files/MATLAB/R2009a）
VS2008 中文版（安装路径：E:/Program Files/Microsoft Visual Studio 9.0）
1.Matlab 生成DLL
1.1编译器的安装
在matlab中先安装编译器，我在第一次安装的时候一路y下来，只有一个compiler，还是最老的。这教育我们要学会说N，按照以下步骤操作
>> mbuild -setup
Please choose your compiler for building standalone MATLAB applications: 
Would you like mbuild to locate installed compilers [y]/n? n
Select a compiler: 
[1] Lcc-win32 C 2.4.1 
[2] Microsoft Visual C++ 6.0 
[3] Microsoft Visual C++ .NET 2003 
[4] Microsoft Visual C++ 2005 SP1 
[5] Microsoft Visual C++ 2008 Express 
[6] Microsoft Visual C++ 2008 SP1 
[0] None 
Compiler: 6
The default location for Microsoft Visual C++ 2008 SP1 compilers is C:/Program Files/Microsoft Visual Studio 9.0,
but that directory does not exist on this machine.  
Use C:/Program Files/Microsoft Visual Studio 9.0 anyway [y]/n? n
Please enter the location of your compiler: [C:/Program Files/Microsoft Visual Studio 9.0] e:/Program Files/Microsoft Visual Studio 9.0
 （红色部分换成你的vs所安装的地址）
Please verify your choices: 
Compiler: Microsoft Visual C++ 2008 SP1  
Location: e:/Program Files/Microsoft Visual Studio 9.0 
Are these correct [y]/n? y
**************************************************************************** 
  Warning: Applications/components generated using Microsoft Visual Studio   
           2008 require that the Microsoft Visual Studio 2008 run-time       
           libraries be available on the computer used for deployment.       
           To redistribute your applications/components, be sure that the    
           deployment machine has these run-time libraries.                  
**************************************************************************** 
Trying to update options file: C:/Users/Administrator/AppData/Roaming/MathWorks/MATLAB/R2009a/compopts.bat
From template:              E:/PROGRA~1/MATLAB/R2009a/bin/win32/mbuildopts/msvc90compp.bat
Done . . .
1.2 DLL的生成
首先新建一个m文件，文件名为myadd2.m,定义了一个名为myadd2的函数，代码如下：
//////////////////////////////////////////////////////
function [y,z] = myadd2(a, b)
% dummy function, just to demonstrate the idea
y = a+b;
z = a+2*b;
end
/////////////////////////////////////
在MATLAB命令框中输入以下命令：
mcc -W cpplib:libmyadd2 -T link:lib myadd2.m 
生成libmyadd2.lib, libmyadd2.h, libmyadd2.dll 等文件，将这三个文件拷到VS的项目目录下
2. VS调用DLL
2.1 新建一个项目，并设置环境
新建一个win32 控制台应用程序，我取的名字是matlabDll2.当然新建其他的项目类型也可以，我这只是个例子。接下来进行配置，在该项目的属性中进行了配置，只对该项目有效。若建新的项目需要重新配置。项目建好后将libmyadd2.lib, libmyadd2.h, libmyadd2.dll拷贝到项目目录下。
首先配置项目属性页/配置属性/C-C++/常规/附加包含目录，请根据自己电脑上软件的安装位置对照设置，2008与2005不同的地方时这里要加两个目录，如下图所示：
![](http://hi.csdn.net/attachment/201008/31/0_1283273132CkHx.gif)
其次配置项目属性页/配置属性/链接器/常规/附加库目录，请根据自己电脑上软件的安装位置对照设置，如下图所示：
![](http://hi.csdn.net/attachment/201008/31/0_12832731580ZUQ.gif)
然后配置项目属性页/配置属性/链接器/输入/附加依赖性，填入libmyadd2.lib mclmcrrt.lib mclmcr.lib ，如下图所示：
![](http://hi.csdn.net/attachment/201008/31/0_1283273182ciw1.gif)
在这一步俺可吃了大苦头了，有篇文档只说添加前两项，俺就照做了，结果导致运行失败，找了好长时间也没发现错误，两天的功夫都在找，结果就是因为少填了这一项，还有其他的一些包含lib，比如libmex.lib  libmx.lib ,这里没填，好像是VC6上要配的。
配置到此结束
2.2 编写主程序,调试运行
这段代码是从别处拷来的，是一段完整代码，将它粘到matlabDLL2.cpp 主CPP文件中，调试通过：
//////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <iostream>
#include "mclmcr.h"
#include "mclcppclass.h"
#include "libmyadd2.h"
int _tmain(int argc, _TCHAR* argv[])
{
        std::cout << "Hello world!" << std::endl;
        /* Initialize the MCR */ 
       /* if( !mclInitializeApplication(NULL,0) ) 
        { 
                std::cout << "Could not initialize the application!" << std::endl;
                return -1; 
        } */
        // initialize lib
        if( !libmyadd2Initialize())
        {
                std::cout << "Could not initialize libmyadd2!" << std::endl;
                return -1; 
        }
        try
        {
        // declare and initialize a
        mwArray a(2, 2,  mxDOUBLE_CLASS);
        double *aData;
        aData = new double[4];
        int i;
        for( i=0; i<4; ++i)
        {
                aData[i] = 1.0*i;
        }
        // print output
        std::cout << "a = " << std::endl;
        std::cout << aData[0] << ",/t" << aData[1] << std::endl;
        std::cout << aData[2] << ",/t" << aData[3] << std::endl;
        a.SetData(aData, 4);
        // declare and initialize b
        mwArray b(2, 2,  mxDOUBLE_CLASS);
        b(1,1) = 11.;
        b(1,2) = 12.;
        b(2,1) = 21.;
        b(2,2) = 22.;
        mwArray y(2, 2,  mxDOUBLE_CLASS);
        mwArray z(2, 2,  mxDOUBLE_CLASS);
        // call the function
                myadd2(2, y, z, a, b);
        // copy data from mwArray to C++ objects
        // allocate outputs
        double *yData, *zData;
        yData = new double[4];
        if( yData == NULL )
        {
                std::cout << "Failed to allocate memory for yData!" << std::endl;
                return -1;
        }
        zData = new double[4];
        if( zData == NULL )
        {
                std::cout << "Failed to allocate memory for zData!" << std::endl;
                return -1;
        }
        // copy data from mwArray to C++
        y.GetData(yData, 4);
        z.GetData(zData, 4);
        // print output
        std::cout << "y = " << std::endl;
        std::cout << yData[0] << ",/t" << yData[1] << std::endl;
        std::cout << yData[2] << ",/t" << yData[3] << std::endl;
        std::cout << "z = " << std::endl;
        std::cout << zData[0] << ",/t" << zData[1] << std::endl;
        std::cout << zData[2] << ",/t" << zData[3] << std::endl;
        // deallocate memory
        delete [] aData;
        delete [] zData;
        delete [] yData;
        }
        catch( const mwException& e)
        {
          std::cerr << e.what() << std::endl;
        }
        // terminate the lib
        libmyadd2Terminate();
        // terminate MCR
        mclTerminateApplication();
        return 0;
}
/////////////////////////////////////////////////////////////////////////////
运行结果如下图:
![](http://hi.csdn.net/attachment/201008/31/0_1283273188eY4I.gif)
问题
我将代码的标红部分注释掉了，否则会出现错误：
1>c:/users/administrator/documents/visual studio 2008/projects/matlabdll2/matlabdll2/matlabdll2.cpp(14) : error C3861: “mclInitializeApplication_proxy”: 找不到标识符
我不知道为什么，大家在调试的过程如果解决了这个问题，麻烦告诉一声：[liufengjk_ok@126.com](mailto:liufengjk_ok@126.com)。
致谢
感谢taohe等网友提供的参考！
