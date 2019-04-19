# 英特尔的优化库IPP - 三少GG - CSDN博客
2011年03月30日 15:02:00[三少GG](https://me.csdn.net/scut1135)阅读数：3631
有效！！！Grant
**假定你的IPP是安装在“D:/Intel/IPP”目录下。　　首先是配置VC2005的环境。打开VC2005后，点击菜单栏的“工具--选项”，在左侧找到“项目和解决方案--VC++目录”，在“**工作目录**”的目录中添加“D:/Intel/IPP/bin”，在“包含文件”的目录中添加“D:/Intel/IPP/include”，在“库文件”中添加“D:/Intel/IPP/stublib”和“D:/Intel/IPP/lib”，确定即可。**
　而在“项目--xxx属性--配置属性--调试”中，“工作目录”应设为“D:/Intel/IPP/bin”，这样运行时程序才能调用IPP的动态链接库，或者把所需要的dll文件复制到你的Debug或Release文件夹下面也行。
_________________________________________     _______________________   _________________________
英特尔不仅在硬件开发上独占鳌头，在与硬件结合的软件上也有相当的实力，更是其死对头AMD所望尘莫及的。著名的软件有英特尔的C/C++语言编译器，以及系列优化库软件。这里谈一下优化库IPP。
IPP全名是Integrated 
Performance Primitives，有个中文译名，叫英特尔® 群集工具集，名字颇为拗口，但其功能非常直接：提供程序处理中的优化功能。编程的时候，如果在一些很关键且耗用CPU的处理中进行优化处理，可以大幅度提高程序性能和缩短执行时间。这些优化，除了对程序进行合理安排之外，就得使用一些高性能的汇编指令了。不过这么一来，程序变得复杂，而且难以维护，还有就是不同的CPU（甚至包括尚未出现的）的对应也会成问题。对此，英特尔的IPP给出了比较完善的回答。
有了（上）的基础，可以谈一下深一步的内容了。使用IPP函数，程序或者使用动态库连接，或者是静态库连接。
使用动态库的场合，需要在IPP安装目录stublib下的库，如ippi.lib。这时执行程序本身不大，但需要IPP的DLL，它们一般是在IPP安装目录bin下，名字为ippi.dll等，还有和平台相关的DLL，稍后会详细解释。
使用静态库的情况，要使用IPP安装目录lib下的库，如ippimerged.lib。这样生成的程序比较大，因为含有所有执行平台的模块，好处是不再需要IPP的DLL了。
使用动态静态库的问题，是C语言的常识，这里不多加讨论。需要解释一下IPP的工作原理。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   IPP这个东西第一次见到的时候好头大，中文参考很少，文档也都是英文的，而且名字还那么那么的长……当然我现在也不是多么了解的，只是先把我了解到的都写下来，日后也好查找。
        IPP安装后包括了几大部分：doc（存放说明文档），include（*.h文件），stublib（lib文件，包含符号而不包含实现，用于装载Dll），dll（包括函数实现等），lib（静态链接需要加载的lib）。
编译环境设置：
1.传统的方法是在"Tools->Options->Directories"中设置Include files和Library files，分别添加入include和stublib。为了Debug和Release下都可以运行，Setting For 选择"All Configurations"，然后设置"Project Settings->Link->General"，在"Object/library modules"中输入自己需要的lib。编译运行就ok了。
2.如果需要AP在任意的机子上都可以编译运行，而不用装IPP，那么不设置"Tools->Options->Directories"的Include files和Library files，此时先以同样的方法设置"Project Settings->Link->General"，在"Object/library modules"中输入自己需要的lib。然后设置"Project Settings->Link->Input"，在"Additional library references"中输入当前stublib的路径，比方说"./ia32/stublib"。ia32的上一级文件夹就是我当前的工程文件夹。设置"Project
 Settings->Debug->General"，"Working directory"中设置dll的路径。需要在代码中加入"#pragma comment(lib,"ippi.lib")"等需要的lib。这种设置方式，如果要将可执行档发给别人玩，应该将可执行档和dll打包在一个文件夹内，这样才不会出错。
3.如果别人只是需要一个可以玩的可执行档，那么还是采用静态编译比较好。这时候不设置"Options"，先以同样的方法设置"Project Settings->Link->General"，在"Object/library modules"中输入自己需要的lib（前两种方法的lib都是stublib文件夹中的，这次是lib文件夹中的）。然后设置"Project Settings->Link->Input"，在"Additional library references"中输入当前lib的路径。需要在代码中加入"#pragma
 comment(lib,"ippsemerged.lib")"等需要的lib。此种方法需要先调用初始化函数ippStaticInit();和ippStaticInitCpu();声明于ippcore.h中。并且需要-emerge.lib,-merge.lib,ippcorel.lib。
        IPP的命名也是有规律的：比如说：
IppStatus ippiCopy_8u_C3AC4R(const Ipp<datatype>* pSrc, int srcStep, Ipp<datatype>* pDst, int dstStep, IppiSize roiSize);
实际上是：
ipp<data-domain><name>_<datatype>[_<descriptor>](<parameters>);
而IppStatus，基本上表示函数运行成功失败的状态吧。这个目前我没有仔细研究不太确定记得清楚了。
其所用的数据格式，比如IPPSize等在文档开始处有介绍和描述。
        目前主要的应用和了解是ippi和ippcc，说明在文档ippiman中，主要的作用是图像视频处理。
        改天想起哪里写得不太对或者忘了什么，再修改吧……
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/Shelley_Liu/archive/2009/04/21/4097600.aspx](http://blog.csdn.net/Shelley_Liu/archive/2009/04/21/4097600.aspx)
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
IPP是个好东西，里面的函数精而全，希望大家好好利用。因为IPP目前很少中文资料，我们只能靠自己摸索。下面就简单说说IPP在VC2005下的基本使用方法。
　　假定你的IPP是安装在“D:/Intel/IPP”目录下。
　　首先是配置VC2005的环境。打开VC2005后，点击菜单栏的“工具--选项”，在左侧找到“项目和解决方案--VC++目录”，在“可执行文件”的目录中添加“D:/Intel/IPP/bin”，在“包含文件”的目录中添加“D:/Intel/IPP/include”，在“库文件”中添加“D:/Intel/IPP/stublib”和“D:/Intel/IPP/lib”，确定即可。
　　然后就是具体的在项目中使用IPP了。我们这次的项目主要是使用IPP的图像处理函数，这些函数的具体说明可以在“D:/Intel/IPP/doc/ippiman.pdf”这个文件中找到。函数的命名其实也是很有规律的，例如图像拷贝的函数，也就是手册里的“4.Image Data Exchange and Initialization Functions”中的Copy这一功能，对于不同的图像，应当使用不同的Copy函数，它的命名是这样的：首先是前缀“ippi”，所有图像处理的函数都以“ippi”开头；然后是功能名称“Copy”，连起来就是“ippiCopy”；之后连一个下划线和对应的模式“ippiCopy_<mod>”，那个“<mod>”替换成对应的颜色模式，例如“8u_C1R”，其中的“C1R”表示图像只有一个颜色通道，而“8u”表示每个象素的颜色的数据类型都是8位无符号数，也就是说这种图像是一个字节表示一个像素的。我们平时用的比较多的是“8u_C3R”，也就是三个颜色通道，每个通道的数据类型都是8位无符号数。但是显示的时候往往需要4个通道的，也就是除了RGB以外，还多了一个Alpha通道（透明度），这是因为我们的电脑一般都设成32位色深的。这时我们就需要把24位的图像转化成32位的，用“ippiCopy_8u_C3AC4R”这个函数就可以了。其中“8u_C3”就代表原始图像是8位无符号数据，3个通道，而AC4R就表示目标图像是带有Alpha通道的4通道图像。再看看函数的完整形式：
IppStatus ippiCopy_8u_C3AC4R(const Ipp<datatype>* pSrc, int srcStep,
Ipp<datatype>* pDst, int dstStep, IppiSize roiSize);
　　其返回值是IppStatus，这个只要看看后面的返回值说明就知道啦，其实是一个整型值来的，只不过IPP里为了方便为这些值都用宏替换赋了名称，这里不再赘述。再来看看函数的参数，其中pSrc和pDst都是指针来的，pSrc就是源图像的图像数据指针，而pDst则指向目标图像的数据。前面的Ipp<datatype>*中的datatype需要替换成相应的数据类型代码，例如8位无符号数，就是“Ipp8u*”了，大家看看手册前面的Function Naming中的Data Types那一节就知道了。而srcStep和dstStep则是指行扫描宽度，也就是图像的一行占用多少字节，这个参数在许多图像处理的函数中都会用到。例如一个320*240的8u_C3R图像，它的行扫描宽度就是320*(3*8)/8=960了。需要注意的是，行扫描宽度必须是4的整数倍，也就是DWORD对齐的，对于不满足4的整数倍的，则应补零对齐。那么上面那条公式就不能适用所有图像了，不过可使用下面的代码来正确计算行扫描宽度：
(((width*channels*8)+31) >> 5) << 2
　　其中width是图像的宽度,channels是通道数。
　　最后的roiSize其实是一个IppiSize结构体来的，定义如下：
typedef struct {
    int width;
    int height;
} IppiSize;
　　很明显就是图像的宽高了。这些结构体之类的基本类型定义都在手册的Function Naming中有详细说明。
　　至于要使用这个函数需要包含哪个头文件呢？我们看到Copy函数的说明里后面有一句“The function ippiCopy is declared in the ippi.h file.”很好，#include "ippi.h"就是了。但是如果这样就直接编译，链接器便会告诉你有ippiCopy_8u_C3AC4R这么一个无法解析的外部符号。这是因为我们还没把库文件链接进来呢。ippi.h对应的库文件就是ippi.lib了。点击菜单的“项目--xxx属性--配置属性--链接器--输入”，在右边的“附加依赖项”中填入ippi.lib即可。若有多个库要使用，则用回车隔开。另外，为了兼容VC6.0的程序，在“项目--xxx属性--配置属性--常规”，右边的“字符集”应选择“多字节字符集”。
　　而在“项目--xxx属性--配置属性--调试”中，“工作目录”应设为“D:/Intel/IPP/bin”，这样运行时程序才能调用IPP的动态链接库，或者把所需要的dll文件复制到你的Debug或Release文件夹下面也行。
　　好了，至此我们的程序便可以编译运行了。感觉IPP用起来还是比较简便的，说明文档里也讲的很全，就是例子比较少，大家多实践实践应该比较容易上手的了。还是那句，边学边用，印象深，效率高。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/comeonface/archive/2008/03/05/2148486.aspx](http://blog.csdn.net/comeonface/archive/2008/03/05/2148486.aspx)
