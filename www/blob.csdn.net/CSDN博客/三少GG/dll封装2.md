# dll封装2 - 三少GG - CSDN博客
2013年03月05日 01:01:35[三少GG](https://me.csdn.net/scut1135)阅读数：857
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
[DLL入门浅析（1）——如何建立DLL](http://www.cppblog.com/suiaiguo/archive/2009/07/20/90619.html)
   初学DLL，结合教程，总结一下自己的所得，希望对DLL初学者们有所帮助。
   动态链接库（DLL）是从C语言函数库和Pascal库单元的概念发展而来的。所有的C语言标准库函数都存放在某一函数库中。在链接应用程序的过程中，链接器从库文件中拷贝程序调用的函数代码，并把这些函数代码添加到可执行文件中。这种方法同只把函数储存在已编译的OBJ文件中相比更有利于代码的重用。但随着Windows这样的多任务环境的出现，函数库的方法显得过于累赘。如果为了完成屏幕输出、消息处理、内存管理、对话框等操作，每个程序都不得不拥有自己的函数，那么Windows程序将变得非常庞大。Windows的发展要求允许同时运行的几个程序共享一组函数的单一拷贝。动态链接库就是在这种情况下出现的。动态链接库不用重复编译或链接，一旦装入内存，DLL函数可以被系统中的任何正在运行的应用程序软件所使用，而不必再将DLL函数的另一拷贝装入内存。
   下面我们一步一步来建立一个DLL。
   一、建立一个DLL工程
   新建一个工程，选择Win32 控制台项目（Win32 Console Application），并且在应用程序设置标签（the advanced tab）上，选择DLL和空项目选项。
   二、声明导出函数
   这里有两种方法声明导出函数：一种是通过使用__declspec(dllexport)，添加到需要导出的函数前，进行声明；另外一种就是通过模块定义文件（Module-Definition File即.DEF）来进行声明。
   第一种方法，建立头文件DLLSample.h，在头文件中，对需要导出的函数进行声明。
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#ifndef _DLL_SAMPLE_H
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#define _DLL_SAMPLE_H
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)// 如果定义了C++编译器，那么声明为C链接方式
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#ifdef __cplusplus
extern"C"![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#endif
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// 通过宏来控制是导入还是导出
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#ifdef _DLL_SAMPLE
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#define DLL_SAMPLE_API __declspec(dllexport)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#else
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#define DLL_SAMPLE_API __declspec(dllimport)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#endif
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// 导出/导入函数声明
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)DLL_SAMPLE_API void TestDLL(int);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#undef DLL_SAMPLE_API
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#ifdef __cplusplus
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#endif
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#endif
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
   这个头文件会分别被DLL和调用DLL的应用程序引入，当被DLL引入时，在DLL中定义_DLL_SAMPLE宏，这样就会在DLL模块中声明函数为导出函数；当被调用DLL的应用程序引入时，就没有定义_DLL_SAMPLE，这样就会声明头文件中的函数为从DLL中的导入函数。 
   第二种方法：模块定义文件是一个有着.def文件扩展名的文本文件。它被用于导出一个DLL的函数，和__declspec(dllexport)很相似，但是.def文件并不是Microsoft定义的。一个.def文件中只有两个必需的部分：LIBRARY 和 EXPORTS。
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)LIBRARY DLLSample
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)DESCRIPTION "my simple DLL"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)EXPORTS
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)        TestDLL @1  ；@1表示这是第一个导出函数
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
   第一行，''LIBRARY''是一个必需的部分。它告诉链接器（linker）如何命名你的DLL。下面被标识为''DESCRIPTION''的部分并不是必需的。该语句将字符串写入 .rdata 节，它告诉人们谁可能使用这个DLL，这个DLL做什么或它为了什么（存在）。再下面的部分标识为''EXPORTS''是另一个必需的部分；这个部分使得该函数可以被其它应用程序访问到并且它创建一个导入库。当你生成这个项目时，不仅是一个.dll文件被创建，而且一个文件扩展名为.lib的导出库也被创建了。除了前面的部分以外，这里还有其它四个部分标识为：NAME,
 STACKSIZE, SECTIONS, 和 VERSION。另外，一个分号(;)开始一个注解，如同''//''在C++中一样。定义了这个文件之后，头文件中的__declspec(dllexport)就不需要声明了。
   三、编写DllMain函数和导出函数
   DllMain函数是DLL模块的默认入口点。当Windows加载DLL模块时调用这一函数。系统首先调用全局对象的构造函数，然后调用全局函数DLLMain。DLLMain函数不仅在将DLL链接加载到进程时被调用，在DLL模块与进程分离时（以及其它时候）也被调用。
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include "stdafx.h"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#define _DLL_SAMPLE
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#ifndef _DLL_SAMPLE_H
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include "DLLSample.h"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#endif
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include "stdio.h"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)//APIENTRY声明DLL函数入口点
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)switch (ul_reason_for_call)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)case DLL_PROCESS_ATTACH:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)case DLL_THREAD_ATTACH:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)case DLL_THREAD_DETACH:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)case DLL_PROCESS_DETACH:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)break;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)　}
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return TRUE;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)void TestDLL(int arg)
![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)  printf("DLL output arg %d\n", arg);
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
   如果程序员没有为DLL模块编写一个DLLMain函数，系统会从其它运行库中引入一个不做任何操作的缺省DLLMain函数版本。在单个线程启动和终止时，DLLMain函数也被调用。
   然后，F7编译，就得到一个DLL了。
[DLL入门浅析（2）——如何使用DLL](http://www.cppblog.com/suiaiguo/archive/2009/07/20/90621.html)
上文我简单的介绍了如何建立一个简单DLL，下面再我简单的介绍一下如何使用一个DLL。当一个DLL被生成后，它创建了一个.dll文件和一个.lib文件；这两个都是你需要的。要使用DLL，就需要载入这个DLL。
**隐式链接**
这里有两个方法来载入一个DLL；一个方法是捷径另一个则相比要复杂些。捷径是只链接到你.lib 文件并将.dll文件置入你的新项目的路径中去。因此，创建一个新的空的Win32控制台项目并添加一个源文件。将你做的DLL放入你的新项目相同的目录下。
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include "stdafx.h"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include "DLLSample.h"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#pragma comment(lib, "DLLSample.lib") //你也可以在项目属性中设置库的链接
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int main()
![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        TestDLL(123);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return(1);
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
这就是载入一个DLL的简单方法。
**显式链接**
难点的加载DLL的方法稍微有点复杂。你将需要函数指针和一些Windows函数。但是，通过这种载入DLLs的方法，你不需要DLL的.lib或头文件，而只需要DLL。
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <iostream>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <windows.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)typedef void (*DLLFunc)(int);
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int main()
![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        DLLFunc dllFunc;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        HINSTANCE hInstLibrary = LoadLibrary("DLLSample.dll");
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if (hInstLibrary == NULL)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         FreeLibrary(hInstLibrary);
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        dllFunc = (DLLFunc)GetProcAddress(hInstLibrary, "TestDLL");
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if (dllFunc == NULL)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         FreeLibrary(hInstLibrary);
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        dllFunc(123);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::cin.get();
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        FreeLibrary(hInstLibrary);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return(1);
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
首先你会注意到：这里包括进了文件“windows.h”同时移走了“DLLSample.h”。原因很简单：因为windows.h包含了一些Windows函数，当然你现在将只需要其中几个而已。它也包含了一些将会用到的Windows特定变量。你可以去掉DLL的头文件（DLLSample.h）因为－如我前面所说－当你使用这个方法载入DLL时你并不需要它。
下面你会看到：下面的一句代码:
typedef void (*DLLFunc)(int);
这是一个函数指针类型的定义。指向一个函数是一个int型的参数，返回值为void类型。
一个HINSTANCE是一个Windows数据类型：是一个实例的句柄；在此情况下，这个实例将是这个DLL。你可以通过使用函数LoadLibrary()获得DLL的实例，它获得一个名称作为参数。在调用LoadLibrary函数后，你必需查看一下函数返回是否成功。你可以通过检查HINSTANCE是否等于NULL（在Windows.h中定义为0或Windows.h包含的一个头文件）来查看其是否成功。如果其等于NULL，该句柄将是无效的，并且你必需释放这个库。换句话说，你必需释放DLL获得的内存。如果函数返回成功，你的HINSTANCE就包含了指向DLL的句柄。
一旦你获得了指向DLL的句柄，你现在可以从DLL中重新获得函数。为了这样作，你必须使用函数GetProcAddress()，它将DLL的句柄（你可以使用HINSTANCE）和函数的名称作为参数。你可以让函数指针获得由GetProcAddress()返回的值，同时你必需将GetProcAddress()转换为那个函数定义的函数指针。举个例子，对于Add()函数，你必需将GetProcAddress()转换为AddFunc；这就是它知道参数及返回值的原因。现在，最好先确定函数指针是否等于NULL以及它们拥有DLL的函数。这只是一个简单的if语句；如果其中一个等于NULL，你必需如前所述释放库。
一旦函数指针拥有DLL的函数，你现在就可以使用它们了，但是这里有一个需要注意的地方：你不能使用函数的实际名称；你必需使用函数指针来调用它们。在那以后，所有你需要做的是释放库如此而已。
**模块句柄**
进程中的每个DLL模块被全局唯一的32字节的HINSTANCE句柄标识。进程自己还有一个HINSTANCE句柄。所有这些模块句柄都只有在特定的进程内部有效，它们代表了DLL或EXE模块在进程虚拟空间中的起始地址。在Win32中，HINSTANCE和HMODULE的值是相同的，这个两种类型可以替换使用。进程模块句柄几乎总是等于0x400000，而DLL模块的加载地址的缺省句柄是0x10000000。如果程序同时使用了几个DLL模块，每一个都会有不同的HINSTANCE值。这是因为在创建DLL文件时指定了不同的基地址，或者是因为加载程序对DLL代码进行了重定位。
模块句柄对于加载资源特别重要。Win32 的FindResource函数中带有一个HINSTANCE参数。EXE和DLL都有其自己的资源。如果应用程序需要来自于DLL的资源，就将此参数指定为DLL的模块句柄。如果需要EXE文件中包含的资源，就指定EXE的模块句柄。
但是在使用这些句柄之前存在一个问题，你怎样得到它们呢？如果需要得到EXE模块句柄，调用带有Null参数的Win32函数GetModuleHandle；如果需要DLL模块句柄，就调用以DLL文件名为参数的Win32函数GetModuleHandle。
**应用程序怎样找到DLL文件**
如果应用程序使用LoadLibrary显式链接，那么在这个函数的参数中可以指定DLL文件的完整路径。如果不指定路径，或是进行隐式链接，Windows将遵循下面的搜索顺序来定位DLL：
1． 包含EXE文件的目录，
2． 进程的当前工作目录，
3． Windows系统目录，
4． Windows目录，
5． 列在Path环境变量中的一系列目录。
这里有一个很容易发生错误的陷阱。如果你使用VC＋＋进行项目开发，并且为DLL模块专门创建了一个项目，然后将生成的DLL文件拷贝到系统目录下，从应用程序中调用DLL模块。到目前为止，一切正常。接下来对DLL模块做了一些修改后重新生成了新的DLL文件，但你忘记将新的DLL文件拷贝到系统目录下。下一次当你运行应用程序时，它仍加载了老版本的DLL文件，这可要当心！
**调试DLL程序**
Microsoft 的VC＋＋是开发和测试DLL的有效工具，只需从DLL项目中运行调试程序即可。当你第一次这样操作时，调试程序会向你询问EXE文件的路径。此后每次在调试程序中运行DLL时，调试程序会自动加载该EXE文件。然后该EXE文件用上面的搜索序列发现DLL文件，这意味着你必须设置Path环境变量让其包含DLL文件的磁盘路径，或者也可以将DLL文件拷贝到搜索序列中的目录路径下。
或者当你调试EXE程序时，在Project Setting中，将Debug选项卡中的Category设置为Additional DLLs。就可以同时调试EXE和它调用的DLL（当然，你需要有DLL的源代码）了。
[DLL入门浅析（3）——从DLL中导出变量](http://www.cppblog.com/suiaiguo/archive/2009/07/20/90643.html)
   前面介绍了怎么从DLL中导出函数，下面我们来看一下如何从DLL中导出变量来。
   声明为导出变量时，同样有两种方法：
   第一种是用__declspec进行导出声明
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#ifndef _DLL_SAMPLE_H
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#define _DLL_SAMPLE_H
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)// 如果定义了C++编译器，那么声明为C链接方式
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#ifdef __cplusplus
extern"C"![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#endif
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// 通过宏来控制是导入还是导出
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#ifdef _DLL_SAMPLE
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#define DLL_SAMPLE_API __declspec(dllexport)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#else
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#define DLL_SAMPLE_API __declspec(dllimport)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#endif
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// 导出/导入变量声明
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)DLL_SAMPLE_API externint DLLData;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#undef DLL_SAMPLE_API
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)#ifdef __cplusplus
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#endif
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#endif
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
  第二种是用模块定义文件（.def）进行导出声明
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)LIBRARY DLLSample
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)DESCRIPTION "my simple DLL"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)EXPORTS
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)        DLLData DATA  ；DATA表示这是数据（变量）
   下面是DLL的实现文件
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include "stdafx.h"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#define _DLL_SAMPLE
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#ifndef _DLL_SAMPLE_H
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include "DLLSample.h"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#endif
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include "stdio.h"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int DLLData;
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)//APIENTRY声明DLL函数入口点
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)switch (ul_reason_for_call)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)case DLL_PROCESS_ATTACH:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      DLLData =123;  // 在入口函数中对变量进行初始化
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)break
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)case DLL_THREAD_ATTACH:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)case DLL_THREAD_DETACH:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)case DLL_PROCESS_DETACH:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)break;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)　}
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return TRUE;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
同样，应用程序调用DLL中的变量也有两种方法。
第一种是隐式链接：
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <stdio.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include "DLLSample.h"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#pragma comment(lib,"DLLSample.lib")
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int main(int argc, char*argv[])
![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)　printf("%d ", DLLSample);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return0;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
第二种是显式链接：
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <iostream>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <windows.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int main()
![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)int my_int;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        HINSTANCE hInstLibrary = LoadLibrary("DLLSample.dll");
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if (hInstLibrary == NULL)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         FreeLibrary(hInstLibrary);
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        my_int =*(int*)GetProcAddress(hInstLibrary, "DLLData");
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if (dllFunc == NULL)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)         FreeLibrary(hInstLibrary);
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::cout<<my_int;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        std::cin.get();
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        FreeLibrary(hInstLibrary);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return(1);
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
通过GetProcAddress取出的函数或者变量都是地址，因此，需要解引用并且转类型。
[DLL入门浅析（4）——从DLL中导出类](http://www.cppblog.com/suiaiguo/archive/2009/07/20/90663.html)
 前面介绍了怎么从DLL中导出函数和变量，实际上导出类的方法也是大同小异，废话就不多说了，下面给个简单例子示范一下，也就不多做解释了。
DLL头文件：
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#ifndef _DLL_SAMPLE_H
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#define _DLL_SAMPLE_H
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)// 通过宏来控制是导入还是导出
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#ifdef _DLL_SAMPLE
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#define DLL_SAMPLE_API __declspec(dllexport)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#else
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#define DLL_SAMPLE_API __declspec(dllimport)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#endif
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)// 导出/导入变量声明
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)DLL_SAMPLE_API class DLLClass
![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)public:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)void Show();
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#undef DLL_SAMPLE_API
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#endif
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
DLL实现文件：
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include "stdafx.h"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#define _DLL_SAMPLE
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#ifndef _DLL_SAMPLE_H
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include "DLLSample.h"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#endif
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include "stdio.h"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)//APIENTRY声明DLL函数入口点
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)switch (ul_reason_for_call)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)case DLL_PROCESS_ATTACH:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)case DLL_THREAD_ATTACH:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)case DLL_THREAD_DETACH:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)case DLL_PROCESS_DETACH:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)break;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)　}
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return TRUE;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)void DLLClass::Show()
![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)  printf("DLLClass show!");
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
应用程序调用DLL
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include "DLLSample.h"
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#pragma comment(lib,"DLLSample.lib")
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int main(int argc, char*argv[])
![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)　DLLClass dc;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)  dc.Show();
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return0;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
大家可能发现了，上面我没有使用模块定义文件（.def）声明导出类也没有用显式链接导入DLL。 
用Depends查看前面编译出来的DLL文件，会发现里面导出了很奇怪的symbol，这是因为C++编译器在编译时会对symbol进行修饰。
这是我从别人那儿转来的截图。
![](http://www.cppblog.com/images/cppblog_com/suiaiguo/85DF01C9E3E7ECB0E9B1E2C262F88F76.jpg)
网上找了下，发现了C++编译时函数名的修饰约定规则
__stdcall调用约定：
1、以"?"标识函数名的开始，后跟函数名；
2、函数名后面以"@@YG"标识参数表的开始，后跟参数表；
3、参数表以代号表示：
X——void，
D——char，
E——unsigned char，
F——short，
H——int，
I——unsigned int，
J——long，
K——unsigned long，
M——float，
N——double，
_N——bool，
....
　　PA——表示指针，后面的代号表明指针类型，如果相同类型的指针连续出现，以"0"代替，一个"0"代表一次重复；
4、参数表的第一项为该函数的返回值类型，其后依次为参数的数据类型,指针标识在其所指数据类型前； 
5、参数表后以"@Z"标识整个名字的结束，如果该函数无参数，则以"Z"标识结束。
　　其格式为"?functionname@@YG*****@Z"或[?functionname@@YG*XZ](mailto:?functionname@@YG*XZ)，
    int Test1（char *var1,unsigned long）[-----“?Test1@@YGHPADK@Z”](mailto:-----%E2%80%9C?Test1@@YGHPADK@Z%E2%80%9D)     void Test2（）                          -----“?Test2@@YGXXZ”
__cdecl调用约定：
　　规则同上面的_stdcall调用约定，只是参数表的开始标识由上面的"@@YG"变为"@@YA"。
__fastcall调用约定：
　　规则同上面的_stdcall调用约定，只是参数表的开始标识由上面的"@@YG"变为"@@YI"。
VC++对函数的省缺声明是"__cedcl",将只能被C/C++调用。
虽然因为C++编译器对symbol进行修饰的原因不能直接用def文件声明导出类和显式链接，但是可以用另外一种取巧的方式。
在头文件中类的声明中添加一个友元函数：
friend DLLClass* CreatDLLClass();
然后声明CreatDLLClass()为导出函数，通过调用该函数返回一个DLLClass类的对象，同样达到了导出类的目的。
这样，就可以用显式链接来调用CreatDLLClass()，从而得到类对象了。
[DLL入门浅析（5）——使用DLL在进程间共享数据](http://www.cppblog.com/suiaiguo/archive/2009/07/21/90734.html)
         在Win16环境中，DLL的全局数据对每个载入它的进程来说都是相同的,因为所有的进程用的都收同一块地址空间；而在Win32环境中，情况却发生了变化，每个进程都有了它自己的地址空间，DLL函数中的代码所创建的任何对象（包括变量）都归调用它的进程所有。当进程在载入DLL时，操作系统自动把DLL地址映射到该进程的私有空间，也就是进程的虚拟地址空间，而且也复制该DLL的全局数据的一份拷贝到该进程空间。（在物理内存中，多进程载入DLL时，DLL的代码段实际上是只加载了一次，只是将物理地址映射到了各个调用它的进程的虚拟地址空间中，而全局数据会在每个进程都分别加载）。也就是说每个进程所拥有的相同的DLL的全局数据，它们的名称相同，但其值却并不一定是相同的，而且是互不干涉的。
因此，在Win32环境下要想在多个进程中共享数据，就必须进行必要的设置。在访问同一个Dll的各进程之间共享存储器是通过存储器映射文件技术实现的。也可以把这些需要共享的数据分离出来，放置在一个独立的数据段里，并把该段的属性设置为共享。必须给这些变量赋初值，否则编译器会把没有赋初始值的变量放在一个叫未被初始化的数据段中。
在DLL的实现文件中添加下列代码：
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#pragma data_seg("DLLSharedSection")      // 声明共享数据段，并命名该数据段
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int SharedData =123;       // 必须在定义的同时进行初始化!!!!
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#pragma data_seg()
 在#pragma data_seg("DLLSharedSection")和#pragma data_seg()之间的所有变量将被访问该Dll的所有进程看到和共享。仅定义一个数据段还不能达到共享数据的目的，还要告诉编译器该段的属性，有三种方法可以实现该目的（其效果是相同的），一种方法是在.DEF文件中加入如下语句：
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)SETCTIONS
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)    DLLSharedSection READ WRITE SHARED
另一种方法是在项目设置的链接选项(Project Setting --〉Link)中加入如下语句：
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)/SECTION:DLLSharedSection,rws
还有一种就是使用指令：
#pragma comment(linker,"/section:.DLLSharedSection,rws")
那么这个数据节中的数据可以在所有DLL的实例之间共享了。所有对这些数据的操作都针对同一个实例的，而不是在每个进程的地址空间中都有一份。
当进程隐式或显式调用一个动态库里的函数时，系统都要把这个动态库映射到这个进程的虚拟地址空间里。这使得DLL成为进程的一部分，以这个进程的身份执行，使用这个进程的堆栈。
下面来谈一下在具体使用共享数据段时需要注意的一些问题：
·         所有在共享数据段中的变量，只有在数据段中经过了初始化之后，才会是进程间共享的。如果没有初始化，那么进程间访问该变量则是未定义的。
·         所有的共享变量都要放置在共享数据段中。如何定义很大的数组，那么也会导致很大的DLL。
·         不要在共享数据段中存放进程相关的信息。Win32中大多数的数据结构和值（比如HANDLE）只在特定的进程上下文中才是有效地。
·         每个进程都有它自己的地址空间。因此不要在共享数据段中共享指针，指针指向的地址在不同的地址空间中是不一样的。
·         DLL在每个进程中是被映射在不同的虚拟地址空间中的，因此函数指针也是不安全的。
当然还有其它的方法来进行进程间的数据共享，比如文件内存映射等，这就涉及到通用的进程间通信了，这里就不多讲了。
