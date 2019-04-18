# VC项目调试基础 - L_Andy的专栏 - CSDN博客

2012年11月26日 14:54:53[卡哥](https://me.csdn.net/L_Andy)阅读数：779
个人分类：[VC/MFC编程](https://blog.csdn.net/L_Andy/article/category/1099539)



**一．Debug版本和Release版本的区别**

Debug通常称为调试版本，它包含调试信息，并且不作任何优化，便于程序员调试程序。Release称为发布版本，它往往是进行了各种优化，使得程序在代码大小和运行速度上都是最优的，以便用户很好地使用。

只有Debug版本的程序才能设置断点、单步执行、使用TRACE/ASSERT等调试输出语句。Release版本不包含任何调试信息，所以体积小、运行速度快。

一般而言Debug版本会比Release版本多出*.ilk文件和*.pdb文件。

1.*.ilk文件

ilk后缀全称为“Incremental Linking”，意即增量链接。

VC6中，“Project SettingàLink（Category：General）”默认勾选“Link incrementally”；相应VC2005中，“项目属性à配置属性à链接器à常规à启用增量链接”默认选项为“是(/INCREMENTAL)”。

当选定渐增型编译链接时，链接器自动生成ILK文件，记录链接信息，也就是每次重新编译并不编译所有的源文件，只编译改动过的文件。而编译器怎么知道哪些编译过哪些未编译过呢，除了检查修改时间外，这个ilk文件也是很重要的信息。

[在](http://www.cnblogs.com/star250/archive/2007/09/21/901282.html)VS2005中，断点变成了感叹号圆圈，提示“当前不会命中断点。此位置当前尚未加载可执行代码”，则可能是没有打开生成调试信息的/Zi开关。在“项目属性à配置属性àC/C++à常规à调试信息格式”处选择“程序数据库(/Zi)”。在“项目属性à配置属性à链接器à调试à生成调试信息” 处选择“是(/DEBUG) ”。

|/C7|C 7.0- Compatible|目标文件或者可执行文件中包含行号和所有符号调试信息，包括变量名及类型，函数及原型等|
|----|----|----|
|/Zi|Program Database|创建一个程序库(PDB)，包括类型信息和符号调试信息|

某个源文件中设置的断点老是无效，提示“当前不会命中断点。源代码与原始版本不同”信息，清理项目、重编项目、重新拷贝原文件都解决不了问题。“工具à选项à调试”去掉“要求源文件与原始版本完成匹配”前面的勾。

参考：[http://kb.cnblogs.com/a/1530885/](http://kb.cnblogs.com/a/1530885/)

2.*.pdb文件

pdb后缀全称为“Program Debug Database”，意即程序数据库文件。

VC6中，“Project SettingàLink （Category：General）”默认勾选“Generate debug info”, “Project SettingàLink （Category：Customize）”默认勾选“Use program database”；相应VC2005中，“项目属性à配置属性à链接器à调试”，默认“生成调试信息”选项为“是(/DEBUG)”，默认“生成程序数据库文件”处填写“./Debug/*.pdb”。该选项对应的编译开关为/PDB。

符号文件（Symbol Files）是一个数据信息文件，它包含了应用程序二进制文件（比如EXE、DLL等）的调试信息，专门用来作调试之用，最终生成的可执行文件在运行时并不需要这个符号文件，但你的程序中所有的变量信息都记录在这个文件中。所以调试应用程序时，这个文件是非常重要的。用VC和 WinDbg调试程序时都要用到这个文件。

关于符号文件，参考《[PDB Files (C++)](http://msdn.microsoft.com/en-us/library/yd4f8bd1(VS.80).aspx)》《[符号文件—Windows](http://www.vckbase.com/document/viewdoc/?id=1710)应用程序调试必备》。

**二．VC常用调试操作**

|快捷键|作用|
|----|----|
|F5|开始调试，逐断点调试|
|F9|设置（取消）断点|
|Ctrl＋F10|调试到光标所在位置|
|F10|单步调试，遇到子函数跳过|
|F11|单步调试，遇到子函数跟踪进入内部|
|Shift＋F11|从当前函数中跳出|
|Shift+F5|停止调试|

    一般F9设断点，F5启动后，F10、F11和Shift＋F11交替使用，以在多个断点处调试。

**三．联合调试链接库（lib、dll）项目和应用程序（exe）项目**

对于链接库项目，必须要导入一个能够调用它的测试程序进来，通过应用程序的执行来达到调试的目的。

既然是调试，当然静态或动态链接库项目应生成带调试信息的Debug版本，VC6中“Project SettingàLink （Category：General）”默认勾选“Generate debug info”，相应VC2005中的“项目属性à配置属性à链接器à调试à生成调试信息”默认选项为“是(/DEBUG)”。

**关于*.obj文件和*.lib文件**

**obj**：Intermediate file，CPP对应的二进制代码格式，是未经重定位的！

**lib**：Object File Library，若干个obj的集合，本质与obj相同！

使用时，obj和lib是没有本质区别的，lib就是obj，使用lib地方都可以使用obj。

对于*.cpp文件中的static变量或函数，在*.obj或*.lib中既有符号信息又有实体定义。

对于*.cpp文件中的extern变量或函数，在*.obj或*.lib中一般都会有符号信息，实体既可以定义在本文件中，也可以在其他文件中定义。对于extern变量或函数，链接时，将在所有的*.obj或*.lib中查找实体。

**Lib和Exe协调：**

新建解决方案D:/ LibDemo/LibDemo(.dsw,.sln)，在其下新建静态库（Win32 Static Library）项目MyLib（.dsp,.vcproj）和静态库测试项目（Win32 Console Application）MyLibDemo（.dsp,.vcproj）。首先需要为MyLibDemo项目附加头文件包含和链接库包含。

（1）若设置MyLibDemo为启动项目，执行F5调试命令，则在MyLib和MyLibDemo中的断点处都会暂停。在MyLibDemo中可F11进入MyLib中的函数定义处。

（2）若设置MyLib为启动项目，则要配置外部exe调用程序：

VC6中，“Project SettingàDebug（Category：General）”中的“Executable for debug session”处填写exe路径，例如：D:/ LibDemo/MyLibDemo/Debug/MyLibDemo.exe。相应VC2005中，“项目属性à配置属性à调试à命令”处填写exe路径。

执行F5调试命令，则在MyLib和MyLibDemo中的断点处都会暂停。效果同（1）。

对于不同解决方案(.dsw,.sln)下的Lib项目和Exe项目协同调试方法同上。

**Dll和Exe协调：**

dll项目往往会附带生成一个对应的lib，是dll导出函数和符号链接信息。同lib项目生成的lib有点不同，这里只有原型信息，实体代码在dll中。dll是可实际运行的二进制代码，其中有定位代码。

新建解决方案D:/DllDemo/DllDemo(.dsw,.sln)，在其下新建动态库（Win32 Dynamic-Link Library）项目MyDll（.dsp,.vcproj）和动态库测试项目（Win32 Console Application）MyDllDemo（.dsp,.vcproj）。

同Lib和Exe协调一样，首先需要为Exe项目附加头文件（.h）包含和链接库（.lib）包含。

（1）若设置MyDllDemo为启动项目，为了使MyDll.dll对调试exe可见，设置MyDllDemo的调试工作目录（Working directory）为../MyDll/Debug。

执行F5调试命令，则在MyDll和MyDllDemo中的断点处都会暂停。在MyDllDemo中可F11进入MyDll中的函数定义处。

（2）若设置MyDll为启动项目（例如def显式动态链接库的调试），则要配置外部exe调用程序：

VC6中，“Project SettingàDebug（Category：General）”中的“Executable for debug session”处填写exe路径，例如：D:/DllDemo/MyDllDemo/Debug/MyDllDemo.exe。相应VC2005中，“项目属性à配置属性à调试à命令”处填写exe路径。

为了使MyDll.dll对调试exe可见，设置调试工作目录（Working directory）为./Debug。

执行F5调试命令，则在MyDll和MyDllDemo中的断点处都会暂停。效果同（1）。

对于不同解决方案(.dsw,.sln)下的Dll项目和Exe项目协同调试方法同上。

在（1）和（2）中，除了指定调试工作目录外，还可以处理生成后事件，将生成的链接库文件直接复制到应用程序Debug目录下。具体的，在VC6中“Project Setting àPost-built step”或VC2005中“项目属性à配置属性à生成事件à生成后事件”输入生成后处理命令：copy ./Debug/MyDll.dll ../MyDllDemo/Debug/MyDll.dll（有时需要使用xcopy以自动生成目录）。当然，还可以将MyLib（MyDll）的输出目录直接指向MyLibDemo(MyDllDemo)的Debug下。

**四．VC调试小结**

我们在VC2005中printf函数调用处设置断点，F11 将打开printf函数的源码文件C:/Program Files/Microsoft Visual Studio 8/VC/crt/src/printf.c进入printf函数内部，实际上“工具à选项à项目和解决方案àVC++目录à源文件”中包含了crt源代码的路径：$(VCInstallDir)crt/src。

而在VC6中，C:/Program Files/Microsoft Visual Studio/VC98/CRT/SRC不存在，故试图F11进入printf内部时，将弹出“Find Source”对话框，提示“Please enter the path for PRINTF.C”文件。

调试VC时，编译器之所以能够跟踪进入C库函数，是因为编译器本身附带提供了Debug版本的CRT库。

VC6中的MSVCRTD.DLL-MSVCRTD.LIB-MSVCRTD.PDB（对应VC2005中msvcr80d.dll-msvcrtd.lib）为C Runtime Library的Debug版本，其中包含了C运行库的调试信息。

VC6中的MSVCP60D.DLL-MSVCPRTD.LIB-MSVCP60D.PDB（对应VC2005中msvcp80d.dll - msvcprtd.lib）为C++ Runtime Library的Debug版本，其中包含了C++运行库的调试信息。

鉴于Debug版本对调试信息的记录和DllàLib(*.obj，*.lib)àSrc(*.c，*.cpp)的对应关系，在VC中我们就可以设置断点跟踪调试代码。

