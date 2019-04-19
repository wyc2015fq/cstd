# 微软研究院Detour开发包之API拦截技术 - 逍遥剑客 - CSDN博客
2008年04月03日 20:28:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3176
我们截获函数执行最直接的目的就是为函数增添功能，修改返回值，或者为调试以及[性能测试](http://www.qqread.com/tag/2142/index.html)加入附加的代码，或者截获函数的输入输出作研究，破解使用。
通过访问源代码，我们可以轻而易举的使用重建（Rebuilding）[操作系统](http://www.qqread.com/tag/1182/index.html)或者应用程序的方法在它们中间插入新的功能或者做功能扩展。然而，在今天这个商业化的开发世界里，以及在只有二进制代码发布的系统中，研究人员几乎没有机会可以得到源代码。本文主要讨论Detour在[Windows](http://www.qqread.com/keys/windows/index.html)二进制PE文件基础上的API截获技术。对于[Linux](http://www.qqread.com/linux/index.html)平台，作这件事情将会非常的简单，由于最初的操作系统设计者引入了LD_PRELOAD。如果你设置 LD_PRELOAD=mylib.so ，那么应用程序在载入 dll时，会先查看mylib.so的符号表，在relocation 的时候会优先使用mylib.so 里的 symbol 。假如你在mylib.so里有个printf() ，那么这个printf就会替代libc的printf。 而在mylib.so里的这个printf可以直接访问 libc.so里的printf函数指针来获得真正的 printf的入口地址。 这样，所有的dll的API HOOK在loader加载dll的时候就已经完成，非常自然，和平台相关的部分全部交给loader去处理。
**一、Detour开发库： **
l         **简介 **
Detours是一个在x86平台上截获任意Win32函数调用的工具库。中断代码可以在运行时动态加载。Detours使用一个无条件转移指令来替换目标函数的最初几条指令，将控制流转移到一个用户提供的截获函数。而目标函数中的一些指令被保存在一个被称为“trampoline” （译注：英文意为蹦床，杂技）的函数中，在这里我觉得翻译成目标函数的部分克隆/拷贝比较贴切。这些指令包括目标函数中被替换的代码以及一个重新跳转到目标函数的无条件分支。而截获函数可以替换目标函数，或者通过执行“trampoline”函数的时候将目标函数作为子程序来调用的办法来扩展功能。 
Detours是执行时被插入的。内存中的目标函数的代码不是在硬盘上被修改的，因而可以在一个很好的粒度上使得截获二进制函数的执行变得更容易。例如，一个应用程序执行时加载的DLL中的函数过程可以被插入一段截获代码（detoured），与此同时，这个DLL还可以被其他应用程序按正常情况执行（译注：也就是按照不被截获的方式执行，因为DLL二进制文件没有被修改，所以发生截获时不会影响其他[进程](http://www.qqread.com/z/windows/process/index.html)空间加载这个DLL）。不同于DLL的重新链接或者静态重定向，Detours库中使用的这种中断技术确保不会影响到应用程序中的方法或者系统代码对目标函数的定位。 
如果其他人为了调试或者在内部使用其他系统检测手段而试图修改二进制代码，Detours将是一个可以普遍使用的开发包。据我所知，Detours是第一个可以在任意平台上将未修改的目标代码作为一个可以通过“trampoline”调用的子程序来保留的开发包。而以前的系统在逻辑上预先将截获代码放到目标代码中，而不是将原始的目标代码作为一个普通的子程序来调用。我们独特的“trampoline”设计对于扩展现有的软件的二进制代码是至关重要的。 
出于使用基本的函数截获功能的目的，Detours同样提供了编辑任何DLL导入表的功能，达到向存在的二进制代码中添加任意数据节表的目的，向一个新进程或者一个已经运行着的进程中注入一个DLL。一旦向一个进程注入了DLL，这个动态库就可以截获任何Win32函数，不论它是在应用程序中或者在系统库中。 
l **基本原理**
1．  WIN32进程的内存管理  
众所周知，WINDOWS NT实现了[虚拟存储](http://www.qqread.com/tag/2149/index.html)器，每一WIN32进程拥有4GB的虚存空间， 关于WIN32进程的虚存结构及其操作的具体细节请参阅WIN32 API手册， 以下仅指出与Detours相关的几点：  
 (1) 进程要执行的指令也放在虚存空间中 
 (2) 可以使用QueryProtectEx函数把存放指令的页面的权限更改为可读可写可执行，再改写其内容，从而修改正在运行的程序  
(3) 可以使用VirtualAllocEx从一个进程为另一正运行的进程分配虚存，再使用 QueryProtectEx函数把页面的权限更改为可读可写可执行，并把要执行的指令以二进制机器码的形式写入，从而为一个正在运行的进程注入任意的代码 。 
2． 拦截WIN32 API的原理 
Detours定义了三个概念： 
    (1) Target函数：要拦截的函数，通常为Windows的API。 
(2) Trampoline函数：Target函数的部分复制品。因为Detours将会改写Target函数，所以先把Target函数的前5个字节复制保存好，一方面仍然保存Target函数的过程调用语义，另一方面便于以后的恢复。 
(3) Detour 函数：用来替代Target函数的函数。  
Detours在Target函数的开头加入JMP Address_of_ Detour_ Function指令（共5个字节）把对Target函数的调用引导到自己的Detour函数， 把Target函数的开头的5个字节加上JMP Address_of_ Target _ Function+5共10个字节作为Trampoline函数。请参考下面的图1和图2。
![微软研究院Detour开发包之API拦截技术（图一）](http://www.qqread.com/ArtImage/20080210/ov26_1.jpg)
**(图1：Detour函数的过程)**![微软研究院Detour开发包之API拦截技术（图二）](http://www.qqread.com/ArtImage/20080210/ov26_2.jpg)
[点击查看大图](http://www.qqread.com/ArtImage/20080210/ov26_2.jpg)[](http://www.qqread.com/ArtImage/20080210/ov26_2.jpg)
**（图2： Detour函数的调用过程）**
**说明： **
u       **目标函数： **
目标函数的函数体（二进制）至少有5个字节以上。按照微软的说明文档Trampoline函数的函数体是拷贝前5个字节加一个无条件跳转指令的话（如果没有特殊处理不可分割指令的话），那么前5个字节必须是完整指令，也就是不能第5个字节和第6个字节是一条不可分割的指令，否则会造成Trampoline函数执行错误，一条完整的指令被硬性分割开来，造成程序崩溃。对于第5字节和第6个字节是不可分割指令需要调整拷贝到杂技函数(Trampoline)的字节个数，这个值可以查看目标函数的汇编代码得到。此函数是目标函数的修改版本，不能在Detour函数中直接调用，需要通过对Trampoline函数的调用来达到间接调用。 
u       **Trampoline函数： **
此函数默认分配了32个字节，函数的内容就是拷贝的目标函数的前5个字节，加上一个JMP Address_of_ Target _ Function+5指令,共10个字节。 
此函数仅供您的Detour函数调用，执行完前5个字节的指令后再绝对跳转到目标函数的第6个字节继续执行原功能函数。 
u       **Detour函数： **
此函数是用户需要的截获API的一个模拟版本，调用方式，参数个数必须和目标函数相一致。如目标函数是__stdcall，则Detour函数声明也必须是__stdcall,参数个数和类型也必须相同，否则会造成程序崩溃。此函数在程序调用目标函数的第一条指令的时候就会被调用（无条件跳转过来的），如果在此函数中想继续调用目标函数，必须调用Trampoline函数（Trampoline函数在执行完目标函数的前5个字节的指令后会无条件跳转到目标函数的5个字节后继续执行），不能再直接调用目标函数，否则将进入无穷递归（目标函数跳转到Detour函数，Detour函数又跳转到目标函数的递归，因为目标函数在内存中的前5个字节已经被修改成绝对跳转）。通过对Trampoline函数的调用后可以获取目标函数的执行结果，此特性对分析目标函数非常有用，而且可以将目标函数的输出结果进行修改后再传回给应用程序。 
Detour提供了向运行中的应用程序注入Detour函数和在二进制文件基础上注入Detour函数两种方式。本章主要讨论第二种工作方式。通过Detours提供的开发包可以在二进制EXE文件中添加一个名称为Detour的节表，如下图3所示，主要目的是实现PE加载器加载应用程序的时候会自动加载您编写的Detours DLL，在Detours Dll中的DLLMain中完成对目标函数的Detour。
![微软研究院Detour开发包之API拦截技术（图三）](http://www.qqread.com/ArtImage/20080210/ov26_3.jpg)
**（图3）**
**二、Detours提供的截获API的相关接口 **
Detours的提供的API 接口可以作为一个共享DLL给外部程序调用，也可以作为一个静态Lib链接到您的程序内部。 
Trampoline函数可以动态或者静态的创建，如果目标函数本身是一个链接符号，使用静态的trampoline函数将非常简单。如果目标函数不能在链接时可见，那么可以使用动态trampoline函数。 
u       要使用静态的trampoline函数来截获目标函数，应用程序生成trampoline的时候必须使用 
DETOUR_TRAMPOLINE宏。DETOUR_TRAMPOLINE有两个输入参数：trampoline的原型和目标函数的名字。 
注意，对于正确的截获模型，包括目标函数，trampoline函数，以及截获函数都必须是完全一致的调用形式，包括参数格式和调用约定。当通过trampoline函数调用目标函数的时候拷贝正确参数是截获函数的责任。由于目标函数仅仅是截获函数的一个可调用分支（截获函数可以调用trampoline函数也可以不调用），这种责任几乎就是一种下意识的行为。 
使用相同的调用约定可以确保寄存器中的值被正确的保存，并且保证调用堆栈在截获函数调用目标函数的时候能正确的建立和销毁。 
可以使用DetourFunctionWithTrampoline函数来截获目标函数。这个函数有两个参数：trampoline函数以及截获函数的指针。因为目标函数已经被加到trampoline函数中，所有不需要在参数中特别指定。 
u       我们可以使用DetourFunction函数来创建一个动态的trampoline函数，它包括两个参数：一个指向目标函数的指针和一个截获函数的指针。DetourFunction分配一个新的trampoline函数并将适当的截获代码插入到目标函数中去。 
当目标函数不是很容易使用的时候，DetourFindFunction函数可以找到那个函数，不管它是DLL中导出的函数，或者是可以通过二进制目标函数的调试符号找到。 
DetourFindFunction接受两个参数：库的名字和函数的名字。如果DetourFindFunction函数找到了指定的函数，返回该函数的指针，否则将返回一个NULL指针。DetourFindFunction会首先使用Win32函数LoadLibrary 和GetProcAddress来定位函数，如果函数没有在DLL的导出表中找到，DetourFindFunction将使用ImageHlp库来搜索有效的调试符号（译注：这里的调试符号是指[Windows](http://www.qqread.com/keys/windows/index.html)本身提供的调试符号，需要单独安装，具体信息请参考Windows的用户诊断支持信息）。DetourFindFunction返回的函数指针可以用来传递给DetourFunction以生成一个动态的trampoline函数。 
我们可以调用DetourRemoveTrampoline来去掉对一个目标函数的截获。 
注意，因为Detours中的函数会修改应用程序的地址空间，请确保当加入截获函数或者去掉截获函数的时候没有其他线程在[进程](http://www.qqread.com/z/windows/process/index.html)空间中执行，这是程序员的责任。一个简单的方法保证这个时候是单线程执行就是在加载Detours库的时候在DllMain中呼叫函数。 
**三、使用Detours实现对API的截获的两种方法**
建立一个[MFC](http://www.qqread.com/keys/mfc/index.html)对话框工程，在对话框的OK按钮的单击事件中加入对MessageBoxA函数的调用，编译后的程序名称MessageBoxApp,效果如图。
点击查看大图
