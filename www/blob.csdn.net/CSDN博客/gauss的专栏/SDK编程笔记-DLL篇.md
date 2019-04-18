# SDK编程笔记 - DLL篇 - gauss的专栏 - CSDN博客
2011年12月13日 21:54:23[gauss](https://me.csdn.net/mathlmx)阅读数：263标签：[dll																[编程																[callback																[windows																[linker																[扩展](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=linker&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[Window编程](https://blog.csdn.net/mathlmx/article/category/944343)
**SDK编程笔记 - DLL篇**
**1. 动态链接之含义**
　　在链接应用程序时常使用所谓“静态链接”的方法，即将各个目标文件(.obj)、运行时函数库(.lib)以及已编译的资源文件(.res)链接到一起，形成一个可执行文件(.exe)。使用静态链接时，可执行文件需要使用的各种函数和资源都已包含到文件中。这样做的缺点是对于多个程序都使用的相同函数和资源要重复链接到exe文件中，使程序变大、占用内存增加。 “动态链接”是将一些公用的函数或资源组织成动态链接库文件(.dll)，当某个需要使用dll中的函数或资源的程序启动时(准确的说是初始化时)，系统将该dll映射到调用进程的虚拟地址空间、增加该dll的引用计数值，然后当实际使用到该dll时操作系统就将该dll载入内存；如果使用该dll的所有程序都已结束，则系统将该库从内存中移除。使用同一dll的各个进程在运行时共享dll的代码，但是对于dll中的数据则各有一份拷贝(当然也有在dll中共享数据的方法)。
 动态链接库中可以定义两种函数：输出函数和内部函数。输出函数可以被其他模块调用，内部函数只能被动态链接库本身调用。动态链接库也可以输出数据，但这些数据通常只被它自己的函数所使用。
**2. 动态链接的优点**
　　→节约内存；
　　→使应用程序“变瘦”；
　　→可单独修改动态链接库而不必与应用程序重新链接；
　　→可方便实现多语言联合编程(比如用VC++写个dll，然后在VB中调用)；
　　→可将资源打包；
　　→可在应用程序间共享内存
　　→......
**3. 关于扩展名**
　　动态链接库的标准扩展名是dll,其他如exe,drv,fon也可作为扩展名，但只有扩展名为dll的动态链接库才能被Windows自动载入。如果使用其它扩展名的动态链接库，则调用动态链接库的程序中必须使用LoadLibrary或LoadLibraryEx载入动态链接库模块。
**4. 用SDK创建一个简单的dll文件**
　　在VC++中选择新建一个Win32 Dynamic-Link Library。需要建立一个c/c++ head file和一个c/c++ source file并加入工程。头文件中内容为输出函数的声明，源文件中内容为DllMain函数和输出函数的定义。下面是一个最简单的例子。
//dlldemo.h 
#ifdef __cplusplus
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT __declspec(dllexport) 
#endif
EXPORT void CALLBACK DllFoo(void) ;
//dlldemo.c
#include <windows.h>
#include "dlldemo.h" 
int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
　return TRUE ; 
}
EXPORT void CALLBACK DllFoo(void)
{ 
　MessageBox(NULL,TEXT("This function is exported from a DLL"),TEXT("DllFoo"),MB_OK) ;
　return ;
}
　　头文件预处理中的__declspec是微软增加的“C扩展类存储属性”(C Extended Storage-Class Attributes)，它指明一个给出的实例被存储为一种微软特定的类存储属性，可以为thread,naked,dllimport或dllexport.
[MSDN原文：The extended attribute syntax for specifying storage-class information uses the __declspec keyword, which specifies that an instance of a given type is to be stored with a Microsoft-specific storage-class attribute (thread,
 naked, dllimport, or dllexport).] 输出函数必须指明为CALLBACK。 DllMain是dll的入口点函数。也可以不写它。DllMain必须返回TRUE，否则系统将终止程序并弹出一个“启动程序时出错”对话框。 编译链接后，得到动态链接库文件dlldemo.dll和输入库文件dlldemo.lib。
**5.使用dll的两种方式**
　　方法一： load-time dynamic linking 
　　在要调用dll的应用程序链接时，将dll的输入库文件(import library，.lib文件)包含进去。具体的做　　法是在源文件开头加一句#include ，然后就可以在源文件中调用dlldemo.dll中的输出文件了。
　　方法二： run-time dynamic linking 
　　不必在链接时包含输入库文件，而是在源程序中使用LoadLibrary或LoadLibraryEx动态的载入dll。
　　主要步骤为(以demodll.dll为例): 
5)使用FreeLibrary卸载dll
　FreeLibrary(dllHandle) ;
　　使用run-time dynamic linking 比较麻烦，但有它的好处(下面讨论)。MSDN中有一篇文章DLLs the 　　　Dynamic Way讨论使用c的宏创建一个基类pDll完成以上复杂的操作，使用时只需定义一个类继承自类pDll并　　对类和函数使用宏。
　　以上两种方法都要求应用程序能找到dll文件，Windows按以下顺序寻找dll文件：
6)环境变量path中的路径。
　　如果系统不能找到dll文件，将结束调用dll的进程并弹出一个“启动程序时出错”对话框，告诉你“找　　不到所需的dll文件-XXX.dll”。 
**6 使用运行时的动态链接(Run-Time Dynamic Linking)有什么好处？**
　　如果使用载入时的动态链接(Load-Time Dynamic Linking)，当dll文件丢失时，调用此dll的程序将不能运行，你将会看到一个“启动程序时出错”对话框。而如果在运行时载入dll，你有机会处理这个错误，至少可以比较“温柔”的结束程序。
　　如果dll改变了，使用load-time dynamic linking的程序可能会终止，而使用run-time dynamic linking 的程序只有当调用的函数在新的dll中不存在时才会出错。
**7 使用纯资源dll**
　　 一般只在其c文件中写一个空的DllMain，然后向工程中插入资源，最后编译为dll文件。纯资源dll没有任何输出函数，因此不会生成.lib文件，所以必须在运行时用LoadLibrary载入。
**8 在dll中使用共享数据(摘自programming windows)**
// shared memory section (requires /SECTION:shared,RWS in link options)
#pragma data_seg ("shared")
　int iTotal = 0 ;
　WCHAR szStrings [MAX_STRINGS][MAX_LENGTH + 1] = { '/0' } ;
#pragma data_seg () 
#pragma comment(linker,"/SECTION:shared,RWS")
第一个#pragma叙述建立资料段，这里命名为shared。您可以将这段命名为任何一个您喜欢的名字。在这里的#pragma叙述之後的所有初始化了的变数都放在shared资料段中。第二个#pragma叙述标示段的结束。对变数进行专门的初始化是很重要的，否则编译器将把它们放在普通的未初始化资料段中而不是放在shared中。 连结器必须知道有一个「shared」共享资料段。在「Project Settings」对话方块选择「Link」页面标签。选中「STRLIB」时在「Project
 Options」栏位（在Release和Debug设定中均可），包含下面的连结叙述： /SECTION:shared,RWS 字母RWS表示段具有读、写和共用属性。或者，您也可以直接用DLL原始码指定连结选项，就像我们在STRLIB.C那样： #pragma comment(linker,"/SECTION:shared,RWS")
