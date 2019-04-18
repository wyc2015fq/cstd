# C++调用matlab编程 - wishchinYang的专栏 - CSDN博客
2014年06月25日 10:02:58[wishchin](https://me.csdn.net/wishchin)阅读数：1849
C++调用Matlab，实在是不得已的方法。原文链接：
[http://kylen314.blog.com/2012/12/11/matlab_for_cplusplus/](http://kylen314.blog.com/2012/12/11/matlab_for_cplusplus/)  这是个很善良的博客，只出现了一个小小的错误！
内容：有少量修改！
噩耗啊噩耗啊！！！！最近本身就忙到吐，什么老板的专题研究，什么人脸性别识别，什么电功率分解算法研究，本身就快虚脱，然后昨天突然说一年半前申请的专利有个公式有点问题，我研究了半天也不知道一年半前的我为什么会写出那样的公式【囧】，然后昨天突然师兄跟我说，老板让我这周内搭出某个系统的演示平台，我勒个去，那个系统因为要从数字通信分析仪上获取数据，所以用NI公司的GPIB采集卡，驱动文件里面提供了C++的接口，然后之后数据我是用matlab处理的，必须妥妥的嘛，结果现在要搭个系统意味着采集和处理要继承在一起，我就怂了，我matlab里面调用了那么多计算方法不明的函数，要怎么移植到C++上啊，其实主要问题还是时间不够，时间充足什么都好说。。。
所以只好飞快研究C++怎么调用matlab的函数了。。。研究了一下，方法好像很多，比如使用MATLAB Engine，它可以在C++里面调用直接使用matlab的函数，但是对于我这个系统，最好可以直接调用整个matlab的function，虽然MATLAB Engine我不知道可不可以，没深究，后来研究了另外一个方法。总结一下当备忘。。
## 系统配置：
环境变量path中要加入matlab的bin目录：即加上X:/XXXXXX/MATLAB/R2009a/bin/win32
虽然，我发现好像装完matlab就已经自动加上了。。。
## C++编译器配置：
不管用VS也好，还是VC6.0也好，VC6中是打开Tool→Options→Directories，VS中是打开Tool→Options→Project and Solution→VC++ Directories,然后添加以下东西：
- Include Files里面添加X:/XXXXXX/MATLAB/R2009a/extern/include
- Include Files里面添加X:/XXXXXX/MATLAB/R2009a/extern/include/win32
- Library Files里面添加X:/XXXXXX/MATLAB/R2009a/extern/lib/win32/microsoft
这样C++端就完成配置了。
## Matlab的配置：
- 命令行中输入mbuild -setup,第一个选y，第二个选你的编译器，VS或者VC6.0对应的号码，一般是2，第三个再选y
- 命令行中输入mex -setup，选项和第一步一样。
配置完上面的东西，就可以了~
## C++调用matlab使用方法:
- 首先写一个正确的matlab的m文件并保存，【额，不正确的我没试过。。】比如说是functin [a b] = function_name(c,d)
- matlab命令行中输入deploytool
- 在Deployment Tool左上角点击Create a new deployment project
- 选择Matlab Compiler中的C++ Share Library
- 并在下面输入你的Project名字和路径
- 右键Add files加入你之前写的m文件
- 点击Build the Project即可完成编译。
- 在你选择的Project的路径下有两个文件夹，distrib和src，src那个不用管，打开distrib
- 里面有三个文件是你要用的，.dll和.lib和.h文件
- 建立你的C++工程，并将上一步中的三个文件加入到你的C++工程中即可。
## C++代码编写：
文件一开始要添加lib文件，代码如下：
#pragma comment(lib,"libdflapack.lib")  #这个文件在2010a里面找不到，要去掉#pragma comment(lib,"libemlrt.lib")#pragma comment(lib,"libeng.lib")#pragma comment(lib,"libfixedpoint.lib")#pragma comment(lib,"libmat.lib")#pragma comment(lib,"libmex.lib")#pragma comment(lib,"libmwlapack.lib")#pragma comment(lib,"libmwservices.lib")#pragma comment(lib,"libmx.lib")#pragma comment(lib,"libut.lib")#pragma comment(lib,"mclcommain.lib")#pragma comment(lib,"mclmcr.lib")#pragma comment(lib,"mclmcrrt.lib")
注意一下倒数第三个，据说matlab后面的版本是mclcommain.lib，之前的版本是mclco.lib，没考证，出了问题你去之前添加的lib路径下看一下就知道了。
然后添加你自己刚刚生成的lib文件
#pragma comment(lib,"XXX.lib")
之后必然要包含你自己的头文件啦。。
#include "XXX.h"
至于怎么用那些函数，简单说明一下，首先声明一下，下文中提到的XXX都是你在上面第五步中写的工程名，也就是上面lib和.h的XXX一样。
调用之前要先初始化，即调用函数：
XXXInitialize();
经过试验，上面那个函数的运行之间极其之长。。。
当然，在结束调用，或者在程序的尾部要调用：
XXXTerminate();
调用函数呢，就是你m文件的函数名，对了，众所周知m文件的文件名要和m文件里面的函数名一样，但是m文件的函数名不一定要和这里设置的Project一样，比如说函数名叫myfunction(),而工程名叫test,那么你生成的就是test.h,test.dll,test.lib，但是你在C++代码里面调用的时候用的却是myfunction。
如果你的myfunction没有输入输出参数，那么调用的时候就直接写myfunction()就可以了，如果有输入输出参数，函数调用格式是：
myfunction(intnargout,mwArray&matrix...,constmwArray&n1....);
直接理解就是输出参数个数nargout
然后nargout个输出参数，数据类型是mwArray,稍有常识的人只要看到函数调用里面有&就基本是输出参数啦~
然后就是各个输入参数，数据类型也是mwArray,看到const就知道应该是输入参数啦~
啥？你问我为什么不用写输入参数个数，好好复习matlab函数去！！！
## 关于mwArray：
这个是matlab生成的dll的接口数据类型，这个看上面的函数调用你就应该清楚这一点了，定义在matlab安装目录下的extern/include/下的mclcppclass.h下，有兴趣的自己看去~
虽然还有一种数据类型是mxArray，但是不推荐使用，而且mwArray本身就是对mxArray的封装！不推荐使用是因为它的内存管理方式不好，而且使用的时候要用指针，而mwArray你可以直接作为对象来操作。【如果要生成接口为mxArray的dll的话，也可以在matlab命令行里面使用申明，但都说了。。不推荐。。】
matlab里面不特别说明数据类型都是double，但是在这里定义一个mwArray数据的时候要说明类型，定义方法如下：
mwArraymatrix(2,2,mxDOUBLE_CLASS);
mwArraymatri2(1,m,mxINT8_CLASS);
如果要赋值或者读取，方法如下：
mwArraymatrix(1,4,mxDOUBLE_CLASS);
doublea[]={1,2,3,4};matrix.SetData(a,4);
cout<<matrix(1,2)<<endl;
double*b =newdouble[4];matrix.GetData(b,4);
cout<<b[0];
## 可能出现的问题：
C++调用matlab生成的lib的时候可能会出现runtime error R6034的错误，虽然你可以关掉这个错误，程序可以继续跑，完全不碍事儿~但是也很烦，官方给出的说法是这样子的：
### Summary
On Windows (XP or Vista) error R6034 is thrown by C runtime.
### Description
On Windows (XP or Vista) error R6034 is thrown by the C runtime. This happens when executing standalone applications, C or C++ user applications built using C or C++ shared libraries, MATLAB Builder JA components, and MATAB Builder NE components.
In this case, the user has a PATH directory containing msvcr80.dll. The error is thrown because the version of tbbmalloc.dll in *mcrinstallroot*|*matlabroot*\bin\*arch* does not contain a manifest.
### Workaround
For standalone executables and C/C++ shared libraries using a supported Microsoft Visual Compiler:
- Download the zip files in the attachment. New option files have been provided for both 32-bit and 64-bit Windows. These new options files create executables with manifests which the operating system uses to locate runtime shared libraries in the Windows
 side X side area. Applications created using the attached options files can be distributed to deployed machines without any change to the target MCR install area.
- Back up *matlabroot*\bin\*arch*\mbuildopts by executing the following at the system command prompt: cd *matlabroot*\bin\*arch* & mkdir mbuildopts.bak & cd mbuildopts & copy *.*..\mbuildopts.bak where *matlabroot* is
 the MATLAB installation directory which can be found by typing: matlabroot at the MATLAB command prompt and *arch* is the platform.
- Unzip either options_win32.zip or options_win64.zip to the root of your MATLAB installation directory.
- Execute mbuild -setup.
- Rebuild your application using the downloaded options files which you enabled by executingmbuild -setup.
For standalone executables and C/C++ shared libraries Using Lcc-win32 compiler:
- Create a manifest file named *application_name*.exe.manifest (if creating an executable) or*application_name*.dll.manifest (if creating a shared library) with the following content:
<?xml version='1.0' encoding='UTF-8' standalone='yes'?> 
<assembly xmlns='urn:schemas-microsoft-com:asm.v1' manifestVersion='1.0'> 
<dependency> 
<dependentAssembly> 
<assemblyIdentity type='win32' name='Microsoft.VC80.CRT' version='8.0.50727.762' processorArchitecture='x86' publicKeyToken='1fc8b3b9a1e18e3b' /> 
</dependentAssembly> 
</dependency> 
</assembly>
You can create this manifest file by copying the above content into a text file.
- Place the manifest in the same folder as the executable. For example, if you have an application called matrixdriver.exe, you would create a manifest file calledmatrixdriver.exe.manifest and put it in the same folder as the application.
For MATLAB Builder for JA and MATLAB Builder for NE, perform the following on your development machine:
- Change your folder to *matlabroot*\bin\*arch*
- Backup tbbmalloc.dll and rename it to tbbmalloc.dll.bak
- Execute the following command at a Visual Studio Command Prompt: mt.exe -inputresource:libut.dll;#2 -outputresource:tbbmalloc.dll;#2
- tbbmalloc.dll now contains a manifest, and must be distributed to any machine on which you have encountered error R6034 when executing a MATLAB Builder JA or MATLAB Builder NE component. You should place the modified tbbmalloc.dll in *mcrroot*\bin\*arch* where *mcrroot* is
 the MATLAB Compiler Runtime (MCR) installation directory.
### Fix
For R2009b, a new version of tbbmalloc.dll is distributed that contains a manifest.
### Attachments
- [options_win64.zip](http://www.mathworks.com/support/bugreports/license/accept_license/2061?fname=options_win64.zip&geck_id=531531)
- [options_win32.zip](http://www.mathworks.com/support/bugreports/license/accept_license/2062?fname=options_win32.zip&geck_id=531531)
但是这个问题也不一定会发生，我在我电脑上跑的时候出现了这个问题，但是在另一台新装matlab的电脑上就没出现，也许和网上传言的一样，重装一下就可以了。。不过好麻烦的说。。。
