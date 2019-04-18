# VC环境设置 - L_Andy的专栏 - CSDN博客

2012年11月26日 15:07:46[卡哥](https://me.csdn.net/L_Andy)阅读数：902


大家可能一直在用VC开发软件，但是对于这个编译器却未必很了解。原因是多方面的。大多数情况下，我们只停留在“使用”它，而不会想去“了解”它。因为它只是一个工具，我们宁可把更多的精力放在C++语言和软件设计上。我们习惯于这样一种“模式”：建立一个项目，然后写代码，然后编译，反反复复调试。但是，所谓：“工欲善其事，必先利其器”。如果我们精于VC开发环境，我们是不是能够做得更加游刃有余呢？

闲话少说。我们先来看一下VC的处理流程，如下：

大致分为两步：编译和连接。源文件通过编译生成了.obj文件；所有.obj文件和.lib文件通过连接生成.exe文件或.dll文件。下面，我们分别讨论这两个步骤的一些细节。

编译参数的设置。主要通过VC的菜单项Project->Settings->C/C++页来完成。我们可以看到这一页的最下面Project Options中的内容，一般如下：

/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"Debug/WritingDlgTest.pch" /Yu"stdafx.h" /Fo"Debug/" /Fd"Debug/" /FD /GZ /c

各个参数代表的意义，可以参考Msdn。比如/nologo表示编译时不在输出窗口显示这些设置（我们可以把这个参数去掉来看看效果）等等。一般我们不会直接修改这些设置，而是通过这一页最上面的Category中的各项来完成。

1)    General：一些总体设置。Warning level用来控制警告信息，其中None表示不显示警告信息，Level 1只显示很严重的警告信息，其他级别依次增加一些次严重的警告信息；Warnings as errors将警告信息当作错误处理；Optimizations是代码优化，可以在Category的Optimizations项中进行更细的设置；Generate browse info用以生成.sbr文件，记录类、变量等符号信息，可以在Category的Listing Files项中进行更多的设置。Debug
 info，生成调试信息：None，不产生任何调试信息（编译比较快）；Line Numbers Only，仅生成全局的和外部符号的调试信息到.OBJ文件或.EXE文件，减小目标文件的尺寸；C 7.0- Compatible，记录调试器用到的所有符号信息到.OBJ文件和.EXE文件；Program Database，创建.PDB文件记录所有调试信息；Program Database for "Edit & Continue"，创建.PDB文件记录所有调试信息，并且支持调试时编辑。

2)    C++ Language：pointer_to_member representation用来设置类定义/引用的先后关系，一般为Best-Case Always表示在引用类之前该类肯定已经定义了；Enable Exception Handling，进行同步的异常处理；Enable Run-Time Type Information迫使编译器增加代码在运行时进行对象类型检查；Disable Construction Displacements，设置类构造/析构函数调用虚函数问题。

3)    Code Generation：Processor表示代码指令优化，可以为80386、80486、Pentium、Pentium Pro，或者Blend表示混合以上各种优化。Use run-time library用以指定程序运行时使用的运行时库（单线程或多线程，Debug版本或Release版本），有一个原则就是，一个进程不要同时使用几个版本的运行时库。Single-Threaded，静态连接LIBC.LIB库；Debug Single-Threaded，静态连接LIBCD.LIB库；Multithreaded，静态连接LIBCMT.LIB库；Debug
 Multithreaded，静态连接LIBCMTD.LIB库；Multithreaded DLL，动态连接MSVCRT.DLL库；Debug Multithreaded DLL，动态连接MSVCRTD.DLL库。连接了单线程库就不支持多线程调用，连接了多线程库就要求创建多线程的应用程序。Calling convention可以用来设定调用约定，有三种：__cdecl、__fastcall和__stdcall。各种调用约定的主要区别在于，函数调用时，函数的参数是从左到右压入堆栈还是从右到左压入堆栈；在函数返回时，由函数的调用者来清理压入堆栈的参数还是由函数本身来清理；以及在编译时对函数名进行的命名修饰（可以通过Listing
 Files看到各种命名修饰方式）。Struct member alignment用以指定数据结构中的成员变量在内存中是按几字节对齐的，根据计算机数据总线的位数，不同的对齐方式存取数据的速度不一样。这个参数对数据包网络传输等应用尤为重要，不是存取速度问题，而是数据位的精确定义问题，一般在程序中使用#pragma pack来指定。

4)    Customize：Disable Language Extensions，表示不使用微软为标准C做的语言扩展；Eliminate Duplicate Strings，主要用于字符串优化（将字符串放到缓充池里以节省空间），使用这个参数，使得

char *sBuffer = "This is a character buffer";

char *tBuffer = "This is a character buffer";

sBuffer和tBuffer指向的是同一块内存空间；Enable Function-Level Linking ，告诉编译器将各个函数按打包格式编译；Enables minimal rebuild，通过保存关联信息到.IDB文件，使编译器只对最新类定义改动过的源文件进行重编译，提高编译速度；Enable Incremental Compilation，同样通过.IDB文件保存的信息，只重编译最新改动过的函数；Suppress Startup Banner and Information Messages，用以控制参数是否在output窗口输出。

5)    Listing Files：Generate browse info的功能上面已经提到过。这里可以进行更多的设置。Exclude Local Variables from Browse Info表示是否将局部变量的信息放到.SBR文件中。Listing file type可以设置生成的列表信息文件的内容：Assembly-Only Listing仅生成汇编代码文件（.ASM扩展名）；Assembly With Machine Code生成机器代码和汇编代码文件（.COD扩展名）；Assembly
 With Source Code生成源代码和汇编代码文件（.ASM扩展名）；Assembly, Machine Code, and Source生成机器码、源代码和汇编代码文件（.COD扩展名）。Listing file name为生成的信息文件的路径，一般为Debug或Release目录下，生成的文件名自动取源文件的文件名。

6)    Optimizations：代码优化设置。可以选择Maximize Speed生成最快速的代码，或Minimize Size生成最小尺寸的程序，或者Customize定制优化。定制的内容包括：

Assume No Aliasing，不使用别名（提高速度）； 

Assume Aliasing Across Function Calls，仅函数内部不使用别名；

Global Optimizations，全局优化，比如经常用到的变量使用寄存器保存，或者循环内的计算优化，如

i = -100;

while( i < 0 ){    i += x + y;}

会被优化为

i = -100;

t = x + y;

while( i < 0 ){i += t;}

Generate Intrinsic Functions，使用内部函数替换一些函数调用（提高速度）； 

Improve Float Consistency，浮点运算方面的优化；

Favor Small Code，程序（exe或dll）尺寸优化优先于代码速度优化；

Favor Fast Code，程序（exe或dll）代码速度优化优先于尺寸优化；

Frame-Pointer Omission，不使用帧指针，以提高函数调用速度；

Full Optimization，组合了几种参数，以生成最快的程序代码。

Inline function expansion，内联函数扩展的三种优化（使用内联可以节省函数调用的开销，加快程序速度）：Disable不使用内联；Only __inline，仅函数定义前有inline或__inline标记使用内联；Any Suitable，除了inline或__inline标记的函数外，编译器“觉得”应该使用内联的函数，都使用内联。

7)    Precompiled Headers：预编译头文件的设置。使用预编译可以提高重复编译的速度。VC一般将一些公共的、不大变动的头文件（比如afxwin.h等）集中放到stdafx.h中，这一部分代码就不必每次都重新编译（除非是Rebuild All）。

8)    Preprocessor：预编译处理。可以定义/解除定义一些常量。Additional include directories，可以指定额外的包含目录，一般是相对于本项目的目录，如..\Include。

连接参数的设置。主要通过VC的菜单项Project->Settings->Link页来完成。我们可以看到这一页的最下面Project Options中的内容，一般如下：

/nologo /subsystem:windows /incremental:yes /pdb:"Debug/WritingDlgTest.pdb" /debug /machine:I386 /out:"Debug/WritingDlgTest.exe" /pdbtype:sept

下面我们分别来看一下Category中的各项设置。

1)    General：一些总体设置。可以设置生成的文件路径、文件名；连接的库文件；Generate debug info，生成Debug信息到.PDB文件（具体格式可以在Category->Debug中设置）；Ignore All Default Libraries，放弃所有默认的库连接；Link Incrementally，通过生成. ILK文件实现递增式连接以提高后续连接速度，但一般这种方式下生成的文件（EXE或DLL）较大；Generate Mapfile，生成.MAP文件记录模块相关信息；Enable
 Profiling，这个参数通常与Generate Mapfile参数同时使用，而且如果产生Debug信息的话，不能用.PDB文件，而且必须用Microsoft Format。

2)    Customize：这里可以进行使用程序数据库文件的设置。Force File Output ，强制产生输出文件（EXE或DLL）；Print Progress Messages，可以将连接过程中的进度信息输出到Output窗口。

3)    Debug：设置是否生成调试信息，以及调试信息的格式。格式可以有Microsoft Format、COFF Format（Common Object File Format）和Both Formats三种选择；Separate Types，表示将Debug格式信息以独立的.PDB文件存放，还是直接放在各个源文件的.PDB文件中。选中的话，表示采用后者的方式，这种方式调试启动比较快。

4)    Input：这里可以指定要连接的库文件，放弃连接的库文件。还可以增加额外的库文件目录，一般是相对于本项目的目录，如..\Lib。Force Symbol References，可以指定连接特定符号定义的库。

5)    Output：Base Address可以改变程序默认的基地址（EXE文件默认为0x400000，DLL默认为0x10000000），操作系统装载一个程序时总是试着先从这个基地址开始。Entry-Point Symbol可以指定程序的入口地址，一般为一个函数名（且必须采用__stdcall调用约定）。一般Win32的程序，EXE的入口为WinMain，DLL的入口为DllEntryPoint；最好让连接器自动设置程序的入口点。默认情况下，通过一个C的运行时库函数来实现：控制台程序采用mainCRTStartup
 (或wmainCRTStartup)去调用程序的main (或wmain)函数；Windows程序采用WinMainCRTStartup (或 wWinMainCRTStartup)调用程序的WinMain (或 wWinMain，必须采用__stdcall调用约定)；DLL采用_DllMainCRTStartup调用DllMain函数（必须采用__stdcall调用约定）。Stack allocations，用以设置程序使用的堆栈大小（请使用十进制），默认为1兆字节。Version Information告诉连接器在EXE或DLL文件的开始部分放上版本号。

值得注意的是，上面各个参数是大小写敏感的；在参数后加上“-”表示该参数无效；各个参数值选项有“*”的表示为该参数的默认值；可以使用页右上角的“Reset”按钮来恢复该页的所有默认设置。

其它一些参数设置。

1)    Project->Settings->General，可以设置连接MFC库的方式（静态或动态）。如果是动态连接，在你的软件发布时不要忘了带上MFC的DLL。

2)    Project->Settings->Debug，可以设置调试时运行的可执行文件，以及命令行参数等。

3)    Project->Settings->Custom Build，可以设置编译/连接成功后自动执行一些操作。比较有用的是，写COM时希望VC对编译通过的COM文件自动注册，可以如下设置：

Description: Register COM

Commands: regsvr32 /s /c $(TargetPath) 

echo regsvr32 exe.time > $(TargetDir)\$(TargetName).trg

Outputs: $(TargetDir)\$(TargetName).trg

4)    Tools->Options->Directories，设置系统的Include、Library路径。

一些小窍门

1)    有时候，你可能在编译的时候，计算机突然非法关机了（可能某人不小心碰了电源或你的内存不稳定等原因）。当你重启机器后打开刚才的项目，重新进行编译，发现VC会崩掉。你或许以为你的VC编译器坏了，其实不然（你试试编译其它项目，还是好的！），你只要将项目的.ncb、.opt、.aps、.clw文件以及Debug、Release目录下的所有文件都删掉，然后重新编译就行了。

2)    如果你想与别人共享你的源代码项目，但是把整个项目做拷贝又太大。你完全可以删掉以下文件：.dsw、.ncb、.opt、.aps、.clw、. plg文件以及Debug、Release目录下的所有文件。

3)    当你的Workspace中包含多个Project的时候，你可能不能直观地、一眼看出来哪个是当前项目。可以如下设置：Tools->Options->Format，然后在Category中选择Workspace window，改变其默认的字体（比如设成Fixedsys）就行了。

4)    如何给已有的Project改名字？将该Project关掉。然后以文本格式打开.dsp文件，替换原来的Project名字即可。

5)    VC6对类成员的智能提示功能很有用，但有时候会失灵。你可以先关掉项目，将.clw和.ncb删掉，然后重新打开项目，点击菜单项View->ClassWizard，在弹出的对话框中按一下“Add All”按钮；重新Rebuild All。应该可以解决问题。

