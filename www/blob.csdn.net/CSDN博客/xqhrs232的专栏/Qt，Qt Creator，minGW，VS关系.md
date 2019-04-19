# Qt，Qt Creator，minGW，VS关系 - xqhrs232的专栏 - CSDN博客
2017年10月12日 10:40:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：122
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/huaxi1902/article/details/17205587](http://blog.csdn.net/huaxi1902/article/details/17205587)
相关文章
1、Qt： MinGW 与MSVC的区别----[http://blog.csdn.net/rl529014/article/details/52643685](http://blog.csdn.net/rl529014/article/details/52643685)
2、[[置顶] Qt在Windows下的三种编程环境搭建](http://blog.csdn.net/libaineu2004/article/details/17363165)----[http://blog.csdn.net/libaineu2004/article/details/17363165](http://blog.csdn.net/libaineu2004/article/details/17363165)
Qt是 一个跨平台的C++图形用户界面应用程序框架， 它提供给应用程序开发者建立艺术级的图形用户界面所需的所有功能。Qt很容易扩展，并且允许真正地组件编程。基本上，Qt 同 X Window 上的 Motif，Openwin，GTK 等图形界面库和 Windows 平台上的 MFC，OWL，VCL，ATL 是同类型的东西。 Qt library为Qt程序提供头文件、静态链接库和动态链接库
Qt Creator是跨平台的 Qt IDE， 支持的系统包括 Linux（32 位及 64 位）、Mac OS X 以及Windows。Qt Creator 包括项目生成向导、高级的 C++ 代码编辑器IDE、浏览文件及类的工具、集成了 Qt Designer、Qt Assistant、Qt Linguist、图形化的 GDB 调试前端，集成 qmake 构建工具。
MinGW 提供了一套简单方便的Windows下的基于GCC 程序开发环境。MinGW 收集了一系列免费的Windows 使用的头文件和库文件；同时整合了GNU的工具集，特别是GNU 程序开发工具，如经典gcc, g++, make等。MinGW是完全免费的自由软件，它在Windows平台上模拟了Linux下GCC的开发环境，为C++的跨平台开发提供了良好基础支持，为了在Windows下工作的程序员熟悉Linux下的C++工程组织提供了条件。
又称mingw32，是将GCC编译器和GNU Binutils移植到Win32平台下的产物，包括一系列头文件（Win32API）、库和可执行文件。另有mingw64系列工具，是用于产生64位Windows可执行文件的GNU交叉编译器。
Microsoft Visual Studio：简称VS，功能强大，体积也很强大，它捆绑有编译器。但是软件体积和运行速度都显出它的臃肿。它不支持单文件编译，并且“解决方案”中会生成一大堆相关文件。VS是一个基本完整的开发工具集，它包括了整个软件生命周期中所需要的大部分工具，如UML工具、代码管控工具、集成开发环境(IDE)等等。
      新手们（包括一部分有一定编程经历的人）往往会混淆“编译器(Compiler)”和“编辑器(Editor，IDE)”这两个概念。编译器负责把C/C++代码编译成可执行代码，换而言之，它读取.c或.cpp文件并生成.exe文件（Windows下）；编辑器是方便编辑代码用的，它存取.c或.cpp并把里面的代码以比较适合的样式（比如自动缩进、代码高亮）显示在屏幕上，方便编辑，本质上它是一个带有附加功能的记事本。一般来说，我们用编辑器打开代码文件，要编译时，编辑器把文件保存并调用编译器编译这个文件，编译器就会生成可执行文件。现在的Windows,TCC（Turbo
 C的编译器）早已衰落，流行的C/C++编译器有两个：
- Microsoft C++ Compiler：内嵌在Visual C++中，功能很不错，感觉做得很精致，和编辑器的嵌合很好。
- MinGW GCC：GCC全称是GNU Compiler Collection，著名的开源多语言编译器，编译出的代码执行效率比普通编译器高不少，支持单文件编译，得到很多编辑器支持。通常下载到的MinGW GCC都包含包含C编译器gcc-core、C++编译器gcc-g++和另外几种语言的编译器。
简单来说，
Qt是 一个跨平台的C++图形用户界面应用程序框架，图形界面库，Qt library为Qt程序提供头文件、静态链接库和动态链接库。
Qt Creator是跨平台的 Qt IDE。编辑器，编译器，调试器可以自己选。
minGW，是将GCC编译器和一系列免费的Windows 使用的头文件和库文件的组合，编译器，它在Windows平台上模拟了Linux下GCC的开发环境。
VS，一个基本完整的开发工具集，它包括了整个软件生命周期中所需要的大部分工具，如UML工具、代码管控工具、集成开发环境(IDE)等等，它捆绑有内嵌在Visual C++中编译器。
